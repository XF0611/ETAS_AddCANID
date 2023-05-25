#include "fusa_alps_radio.h"
#include "fusa_alps.h"
#include "alps_mp_radio_reg.h"
#include "alps_mp_radio_spi_cmd_reg.h"
#include "fusa_alps_dmu.h"
#include "calterah_limits.h"
#include "alps_otp_mmap.h"
#include "CDD_Fusa.h"
#include "math.h"
#include "baseband_alps_FM_reg.h"
#include "arc.h"
#include "fusa_alps_core.h"
#include "fusa_alps_radar.h"
#include <stdlib.h>

#define FUSA_RADIO_WRITE_BANK_REG(bk, addr, val) fusa_radio_reg_write(RADIO_BK##bk##_##addr, val)
#define FUSA_RADIO_WRITE_BANK_CH_REG(bk, ch, addr, val) fusa_radio_reg_write((ch == 0) ? RADIO_BK##bk##_CH0_##addr : \
                                                                             (ch == 1) ? RADIO_BK##bk##_CH1_##addr : \
                                                                             (ch == 2) ? RADIO_BK##bk##_CH2_##addr : \
                                                                             RADIO_BK##bk##_CH3_##addr, \
                                                                             val)

#define FUSA_RADIO_MOD_BANK_REG(bk, addr, field, val) fusa_radio_reg_mod(RADIO_BK##bk##_##addr, \
                                                               RADIO_BK##bk##_##addr##_##field##_SHIFT, \
                                                               RADIO_BK##bk##_##addr##_##field##_MASK, val)

#define FUSA_RADIO_MOD_BANK_CH_REG(bk, ch, addr, field, val) \
        fusa_radio_reg_mod((ch == 0) ? RADIO_BK##bk##_CH0_##addr :      \
                           (ch == 1) ? RADIO_BK##bk##_CH1_##addr :      \
                           (ch == 2) ? RADIO_BK##bk##_CH2_##addr :      \
                           RADIO_BK##bk##_CH3_##addr,                   \
                           (ch == 0) ? RADIO_BK##bk##_CH0_##addr##_##field##_SHIFT : \
                           (ch == 1) ? RADIO_BK##bk##_CH1_##addr##_##field##_SHIFT : \
                           (ch == 2) ? RADIO_BK##bk##_CH2_##addr##_##field##_SHIFT : \
                           RADIO_BK##bk##_CH3_##addr##_##field##_SHIFT, \
                           (ch == 0) ? RADIO_BK##bk##_CH0_##addr##_##field##_MASK : \
                           (ch == 1) ? RADIO_BK##bk##_CH1_##addr##_##field##_MASK : \
                           (ch == 2) ? RADIO_BK##bk##_CH2_##addr##_##field##_MASK : \
                           RADIO_BK##bk##_CH3_##addr##_##field##_MASK,  \
                           val)

#define FUSA_RADIO_READ_BANK_REG(bk, addr) fusa_radio_reg_read(RADIO_BK##bk##_##addr)
#define FUSA_RADIO_READ_BANK_REG_FIELD(bk, addr, field) fusa_radio_reg_read_field(RADIO_BK##bk##_##addr, \
                                                                             RADIO_BK##bk##_##addr##_##field##_SHIFT, \
                                                                             RADIO_BK##bk##_##addr##_##field##_MASK)

#define REG_L(data) ((data >>  0) & 0xff)
#define REG_M(data) ((data >>  8) & 0xff)
#define REG_H(data) ((data >> 16) & 0xff)

#define FUSA_RADIO_LDO_NUMBER       39
/*
    High_th Low_th
    threshold
    2.5V
    1.3V
    1.2V
    1.1V
    1V
*/
static fusa_config_sm1_t ldo_monitor_cfg = {0};
static float if_loopback_threshold_h = 20;
static float if_loopback_threshold_l = 16;
static float rf_loopback_threshold = 15;
void fusa_radar_ldo_monitor_threshold_set(fusa_config_sm1_t *sm1_cfg)
{

    ldo_monitor_cfg = *sm1_cfg;
}
void fusa_radar_ldo_monitor_threshold_get(fusa_config_sm1_t *thr)
{

    *thr = ldo_monitor_cfg;
}


uint32 fusa_radio_spi_cmd_mode(uint32 mode)
{
        volatile uint32 *dest = (uint32 *)RADIO_SPI_CMD_SRC_SEL;
        *dest = mode;
        return mode;
}

void fusa_radio_spi_cmd_write(char addr, char data)
{
        volatile uint32 *dest = (uint32 *)RADIO_SPI_CMD_OUT;
        *dest  = (  (RADIO_SPI_CMD_OUT_WR_EN_MASK << RADIO_SPI_CMD_OUT_WR_EN_SHIFT)
                  + ((addr & RADIO_SPI_CMD_OUT_ADDR_MASK) << RADIO_SPI_CMD_OUT_ADDR_SHIFT)
                  + ((data & RADIO_SPI_CMD_OUT_DATA_MASK) << RADIO_SPI_CMD_OUT_DATA_SHIFT) );
}

uint32 fusa_radio_spi_cmd_read(char addr)
{
        volatile uint32 *dest = (uint32 *)RADIO_SPI_CMD_OUT;
        *dest = (addr & RADIO_SPI_CMD_OUT_ADDR_MASK) << RADIO_SPI_CMD_OUT_ADDR_SHIFT;
        dest = (uint32 *)RADIO_SPI_CMD_IN;
        return *dest;
}

EMBARC_NOINLINE char fusa_radio_reg_read(char addr)
{
        uint32 cmd_mode_pre;
        char cmd_rd_data;
        cmd_mode_pre = fusa_radio_spi_cmd_mode(RADIO_SPI_CMD_CPU);
        cmd_rd_data = fusa_radio_spi_cmd_read(addr);
        fusa_radio_spi_cmd_mode(cmd_mode_pre);
        return cmd_rd_data;
}

EMBARC_NOINLINE char fusa_radio_reg_read_field(char addr, char shift, char mask)
{
        uint32 cmd_mode_pre;
        char cmd_rd_data;
        cmd_mode_pre = fusa_radio_spi_cmd_mode(RADIO_SPI_CMD_CPU);
        cmd_rd_data = fusa_radio_spi_cmd_read(addr);
        fusa_radio_spi_cmd_mode(cmd_mode_pre);
        return ((cmd_rd_data >> shift) & mask);
}

EMBARC_NOINLINE void fusa_radio_reg_write(char addr, char data)
{
        uint32 cmd_mode_pre;
        cmd_mode_pre = fusa_radio_spi_cmd_mode(RADIO_SPI_CMD_CPU);
        fusa_radio_spi_cmd_write(addr, data);
        fusa_radio_spi_cmd_mode(cmd_mode_pre);
}

EMBARC_NOINLINE void fusa_radio_reg_mod(char addr, char shift, char mask, char data)
{
        uint32 cmd_mode_pre;
        char cmd_rd_data;
        cmd_mode_pre = fusa_radio_spi_cmd_mode(RADIO_SPI_CMD_CPU);
        cmd_rd_data = fusa_radio_spi_cmd_read(addr);
        cmd_rd_data &= ~(mask << shift);
        cmd_rd_data |= (data & mask) << shift;
        fusa_radio_spi_cmd_write(addr, cmd_rd_data);
        fusa_radio_spi_cmd_mode(cmd_mode_pre);
}

uint8 fusa_radio_switch_bank(uint8 bank)
{
        uint8 old_bank = FUSA_RADIO_READ_BANK_REG(0, REG_BANK);
        if(old_bank != bank)
            FUSA_RADIO_WRITE_BANK_REG(0, REG_BANK, bank);
        return old_bank;
}

uint8 fusa_radio_get_rtl_frame_type()
{
    uint8 old_bank = fusa_radio_switch_bank(3);
    uint8 active_fmcw_bank = fusa_radio_reg_read(RADIO_BK3_FMCW_FIL0);
    fusa_radio_switch_bank(old_bank);
    return active_fmcw_bank;
}
//status_buf len is 5
sint32 fusa_radio_tx_ch_status(uint8 *status_buf,sint8 len)
{
    sint32 ret = E_FUSA_OK;
    if(len >= 5)
    {
        uint8 old_bank = fusa_radio_switch_bank(1);
        *status_buf++ = FUSA_RADIO_READ_BANK_REG(1, TX_LDO_EN);
        *status_buf++ = FUSA_RADIO_READ_BANK_REG(1, CH0_TX_EN0);
        *status_buf++ = FUSA_RADIO_READ_BANK_REG(1, CH1_TX_EN0);
        *status_buf++ = FUSA_RADIO_READ_BANK_REG(1, CH2_TX_EN0);
        *status_buf++ = FUSA_RADIO_READ_BANK_REG(1, CH3_TX_EN0);
        fusa_radio_switch_bank(old_bank);
    }
    else
    {
        ret = E_FUSA_BOVR;
    }
    return ret;
}
sint32 fusa_radio_tx_ch_restore(uint8 *status_buf,sint8 len)
{
    sint32 ret = E_FUSA_OK;
    if(len >= 5)
    {
        uint8 old_bank = fusa_radio_switch_bank(1);
        FUSA_RADIO_WRITE_BANK_REG(1, TX_LDO_EN, *status_buf++);
        FUSA_RADIO_WRITE_BANK_REG(1, CH0_TX_EN0, *status_buf++);
        FUSA_RADIO_WRITE_BANK_REG(1, CH1_TX_EN0, *status_buf++);
        FUSA_RADIO_WRITE_BANK_REG(1, CH2_TX_EN0, *status_buf++);
        FUSA_RADIO_WRITE_BANK_REG(1, CH3_TX_EN0, *status_buf++);
        fusa_radio_switch_bank(old_bank);
    }
    else
    {
        ret = E_FUSA_BOVR;
    }
    return ret;
}
void fusa_radio_tx_ch_on(sint32 channel_index, uint32 enable)
{
        uint8 old_bank = fusa_radio_switch_bank(1);
        uint8 enable_4bit;
        uint8 enable_8bit;

        if (enable == TRUE) {
                enable_4bit = 0x0f;
                enable_8bit = 0xff;
        } else {
                enable_4bit = 0x00;
                enable_8bit = 0x00;
        }
        if (channel_index == -1) {
                FUSA_RADIO_WRITE_BANK_REG(1, TX_LDO_EN, enable_8bit);
                FUSA_RADIO_WRITE_BANK_REG(1, CH0_TX_EN0, enable_4bit);
                FUSA_RADIO_WRITE_BANK_REG(1, CH1_TX_EN0, enable_4bit);
                FUSA_RADIO_WRITE_BANK_REG(1, CH2_TX_EN0, enable_4bit);
                FUSA_RADIO_WRITE_BANK_REG(1, CH3_TX_EN0, enable_4bit);
        } else if (channel_index == 0) {
                FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX0_EN, enable);
                FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX0_PA_EN, enable);
                FUSA_RADIO_WRITE_BANK_REG(1, CH0_TX_EN0, enable_4bit);
        } else if (channel_index == 1) {
                FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX1_EN, enable);
                FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX1_PA_EN, enable);
                FUSA_RADIO_WRITE_BANK_REG(1, CH1_TX_EN0, enable_4bit);
        } else if (channel_index == 2) {
                FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX2_EN, enable);
                FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX2_PA_EN, enable);
                FUSA_RADIO_WRITE_BANK_REG(1, CH2_TX_EN0, enable_4bit);
        } else if (channel_index == 3) {
                FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX3_EN, enable);
                FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX3_PA_EN, enable);
                FUSA_RADIO_WRITE_BANK_REG(1, CH3_TX_EN0, enable_4bit);
        }
        fusa_radio_switch_bank(old_bank);
}

void fusa_fmcw_lockstep_en(uint32 state)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint8 temp = fusa_radio_reg_read(RADIO_BK10_ITF_IRQ_CTRL_3);
    if(state)
    {
        temp |= (0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_LOCK_STEP_EN_SHIFT);
    }
    else
    {
        temp &= ~(0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_LOCK_STEP_EN_SHIFT);
    }
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,temp);
    fusa_radio_switch_bank(old_bank);
}

void fusa_fmcw_lockstep_rst(uint32 state)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint8 temp = fusa_radio_reg_read(RADIO_BK10_ITF_IRQ_CTRL_3);
    if(state)
    {
        temp &= ~(0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_IRQ_RSTN_SHIFT);
    }
    else
    {
        temp |= (0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_IRQ_RSTN_SHIFT);
    }
    // temp &= ~(0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_IRQ_RSTN_SHIFT);
    // fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,temp);//enable rst
    // temp |= (0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_IRQ_RSTN_SHIFT);
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,temp);//release rst
    fusa_radio_switch_bank(old_bank);
}

void fusa_fmcw_lockstep_start()
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint8 temp = fusa_radio_reg_read(RADIO_BK10_ITF_IRQ_CTRL_3);

    temp &= ~(0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_ITF_FMCW_TEST_START_SHIFT);
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,temp);
    temp |= (0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_ITF_FMCW_TEST_START_SHIFT);
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,temp);
    temp &= ~(0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_ITF_FMCW_TEST_START_SHIFT);
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,temp);
    fusa_radio_switch_bank(old_bank);
}
void fusa_fmcw_lockstep_irq_clr()
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint8 temp = fusa_radio_reg_read(RADIO_BK10_ITF_IRQ_CTRL_3);

    temp &= ~(0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_ITF_FMCW_CLEAR_SHIFT);
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,temp);
    temp |= (0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_ITF_FMCW_CLEAR_SHIFT);
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,temp);
    temp &= ~(0x01 < RADIO_BK10_ITF_IRQ_CTRL_3_ITF_FMCW_CLEAR_SHIFT);
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,temp);
    fusa_radio_switch_bank(old_bank);
}

void fusa_fmcw_chirp_size_set(uint8 fmcw_bank,uint32 chirp_size)
{
    uint8 old_bank = fusa_radio_switch_bank(5 + fmcw_bank);
    fusa_radio_reg_write(RADIO_BK5_FMCW_CHIRP_SIZE_1_0, chirp_size);
    fusa_radio_reg_write(RADIO_BK5_FMCW_CHIRP_SIZE_1_1, (chirp_size >> 8));
    fusa_radio_switch_bank(old_bank);
}

uint32 fusa_fmcw_chirp_size_get(uint8 fmcw_bank)
{
    uint8 old_bank = fusa_radio_switch_bank(5 + fmcw_bank);
    uint32 chirp_size = fusa_radio_reg_read(RADIO_BK5_FMCW_CHIRP_SIZE_1_1);
    chirp_size =(chirp_size << 8) | fusa_radio_reg_read(RADIO_BK5_FMCW_CHIRP_SIZE_1_0);
    fusa_radio_switch_bank(old_bank);
    return chirp_size;
}

void fusa_fmcw_start()
{
    uint8 old_bank = fusa_radio_switch_bank(3);
    /* Reset before start */
    FUSA_RADIO_WRITE_BANK_REG(3, FMCW_MODE_SEL, 0x2);
    FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, START_SPI, 0x0);

    /* DMU triggers FMCW_START */
    FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, START_SEL, 1);
    fusa_radio_switch_bank(old_bank);
    /* Enable CMD */
    raw_writel(RADIO_SPI_CMD_SRC_SEL, RADIO_SPI_CMD_FMCW  );
    raw_writel(REG_DMU_FMCW_START, 1);
}
void fusa_fmcw_lockstep_err_inject()
{
    uint8 active_fmcw = fusa_radio_get_rtl_frame_type();
    uint32 old_chirp_size = fusa_fmcw_chirp_size_get(active_fmcw);
    fusa_fmcw_chirp_size_set(active_fmcw, 20);

    uint8 old_bank = fusa_radio_switch_bank(10);
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x00);//disable lockstep
    fusa_fmcw_start();
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x08);//enable lockstep
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x0c);//release IRQ reset
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x0e);//start lockstep
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x0c);//start lockstep
    fusa_delay_ms(1);
    fusa_radio_reg_write(0x70,0x0d);//clear irq
    fusa_radio_reg_write(0x70,0x0c);
    fusa_radio_switch_bank(old_bank);

    fusa_fmcw_chirp_size_set(active_fmcw, old_chirp_size);
}
void fusa_fmcw_lockstep_init()
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    // fusa_fmcw_lockstep_rst(TRUE);
    // fusa_fmcw_lockstep_en(TRUE);
    // fusa_fmcw_lockstep_rst(FALSE);
    // fusa_fmcw_lockstep_start();
    // fusa_fmcw_lockstep_irq_clr();

    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x08);//enable lockstep
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x0c);//release IRQ reset
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x0e);//start lockstep
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x0c);//start lockstep
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x0d);//clear irq
    fusa_radio_reg_write(RADIO_BK10_ITF_IRQ_CTRL_3,0x0c);
    fusa_radio_switch_bank(old_bank);
}

void fusa_radio_dac_reg_cfg_inner(uint8 inject_num, uint8 out_num) /* inner circle */
{
        uint8 old_bank = fusa_radio_switch_bank(9);
        /* turn on dac */
        FUSA_RADIO_WRITE_BANK_REG(9, LP_TST_EN1, 0x70);
        FUSA_RADIO_WRITE_BANK_REG(9, LP_TST_EN2, 0xC0);
        FUSA_RADIO_WRITE_BANK_REG(9, DAC_LP_TST, 0x11);

        for(uint8 ch = 0; ch < MAX_NUM_RX; ch++) {
                /* inject choice */
                switch (inject_num) {
                case 0: /* TIA injected*/
                        FUSA_RADIO_WRITE_BANK_REG(9, LP_TST_EN3, 0xFF);
                        break;
                case 1: /* HPF1 injected*/
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, TSTMUX_SEL_LP_TST, 0x8);
                        break;
                case 2: /* VGA1 injected*/
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, TSTMUX_SEL_LP_TST, 0x4);
                        break;
                case 3: /* HPF2 injected*/
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, TSTMUX_SEL_LP_TST, 0x2);
                        break;
                case 4: /* VGA2 injected*/
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, TSTMUX_SEL_LP_TST, 0x1);
                        break;
                default: /* HPF1 injected*/
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, TSTMUX_SEL_LP_TST, 0x8);
                        break;
                }

                /* out choice */
                switch (out_num) {
                case 0: /* TIA out */
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, OUTMUX_SEL_LP_TST, 0x8);
                        break;
                case 1: /* HPF1 out */
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, OUTMUX_SEL_LP_TST, 0x4);
                        break;
                case 2: /* VGA1 out */
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, OUTMUX_SEL_LP_TST, 0x2);
                        break;
                case 3: /* VGA2 out */
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, OUTMUX_SEL_LP_TST, 0x1);
                        break;
                default: /* VGA2 out */
                        FUSA_RADIO_MOD_BANK_CH_REG(9, ch, RXBB, OUTMUX_SEL_LP_TST, 0x1);
                        break;
                }
        }

        fusa_radio_switch_bank(2);
        FUSA_RADIO_MOD_BANK_REG(2, BIST_EN1, BIST_EN_SPARE, 0x1);
        fusa_radio_switch_bank(old_bank);
}

void fusa_radio_dac_reg_cfg_outer() /* outer circle */
{
        uint8 old_bank = fusa_radio_switch_bank(9);
        /* turn on dac */
        FUSA_RADIO_WRITE_BANK_REG(9, LP_TST_EN1, 0x7F);
        FUSA_RADIO_WRITE_BANK_REG(9, LP_TST_EN2, 0xBF);
        FUSA_RADIO_WRITE_BANK_REG(9, LP_TST_EN3, 0x00);
        FUSA_RADIO_WRITE_BANK_REG(9, DAC_LP_TST, 0x11);
        for(uint8 ch = 0; ch < MAX_NUM_RX; ch++) {
                FUSA_RADIO_WRITE_BANK_CH_REG(9, ch, RXBB, 0x01);
        }
        fusa_radio_switch_bank(2);
        FUSA_RADIO_MOD_BANK_REG(2, BIST_EN1, BIST_EN_SPARE, 0x0);
        fusa_radio_switch_bank(old_bank);
}
void fusa_radio_loop_test_en(boolean enable)
{
        uint8 old_bank = fusa_radio_switch_bank(9);
        uint32 val = (enable == TRUE) ? 1 : 0; //todo:need reconfirm val
        FUSA_RADIO_WRITE_BANK_REG(9, LP_TST_EN, val);
        /* reset SDM */
        fusa_radio_switch_bank(3);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, RSTN_SDM_MASH, 0x0);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, RSTN_SDM_MASH, 0x1);
        /* In loop test, rstn_fmcw_gen must be 0 */
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, START_SPI, !val);
        fusa_radio_switch_bank(old_bank);
}
//MHZ
uint32 fusa_radio_get_adc_freq()
{
    uint32 adc_freq = 0;//MHZ
    uint8 old_bank = fusa_radio_switch_bank(2);
    uint8 temp_sel = FUSA_RADIO_READ_BANK_REG_FIELD(2, CH1_ADC_FILTER, BDW_SEL);
    if (temp_sel == 0x00){ //check bdm_sel
        adc_freq = 20;
    }
    else{
        adc_freq = 25;
    }
    fusa_radio_switch_bank(0);
    temp_sel = FUSA_RADIO_READ_BANK_REG_FIELD(0, PLL_CLK_OUT, REFPLL_SDMADC_CLKSEL);
    if(temp_sel == 0x01){ //check sdm clk sel
        adc_freq = adc_freq * 2;
    }
    fusa_radio_switch_bank(old_bank);
    return adc_freq;
}
void fusa_radio_cpu_clk_lock_det_init()
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    fusa_radio_reg_write(0x7d, 0x15);
    fusa_radio_reg_write(0, 0xa);
    fusa_radio_reg_write(0x6e, 0x0);
    fusa_radio_reg_write(0x6e, 0x2);
    fusa_radio_switch_bank(old_bank);
}
void fusa_radio_cpu_clk_lock_err_inject()
{
    fusa_delay_ms(100);
    fusa_radio_switch_bank(0);
    uint8 temp = FUSA_RADIO_READ_BANK_REG(0, REFPLL_DIV);
    FUSA_RADIO_WRITE_BANK_REG(0, REFPLL_DIV, temp & 0xf8);
    fusa_delay_ms(20);
    fusa_radio_switch_bank(0);
    FUSA_RADIO_WRITE_BANK_REG(0, REFPLL_DIV, temp);
    fusa_delay_ms(20);
    //configure radio
    fusa_radio_reg_write(0, 0);
    fusa_radio_reg_write(0x7d, 0x05);
    fusa_radio_reg_write(0x7d, 0x15);
    fusa_radio_reg_write(0, 0xa);
    fusa_radio_reg_write(0x6e, 0x0);
    fusa_radio_reg_write(0x6e, 0x2);
}
//get agc enable state
uint8 fusa_radio_agc_en_state(uint8 frame_type)
{
    uint8 old_bank = fusa_radio_switch_bank(5 + frame_type);
    uint8 state = FUSA_RADIO_READ_BANK_REG(5,FMCW_AGC_EN_1) & 0x01;
    fusa_radio_switch_bank(old_bank);
    return state;
}
/* enable/disable fmcw agc mode */
void fusa_radio_agc_enable(uint8 frame_type, boolean enable)
{
        uint8 old_bank = fusa_radio_switch_bank(5 + frame_type);
        uint8 val = (enable == TRUE)?1:0;
        FUSA_RADIO_MOD_BANK_REG(5,FMCW_AGC_EN_1,AGC_EN_1,val);   //agc enable
        fusa_radio_switch_bank(0);
        for (int ch = 0; ch < MAX_NUM_RX; ch++) {
                FUSA_RADIO_MOD_BANK_CH_REG(0, ch, RX_PDT, PKD_VGA, val);     //RF BB VGA2 enable
                FUSA_RADIO_MOD_BANK_CH_REG(0, ch, RX_PDT, PKD_EN, val);      //RF BB VGA1 enable
                FUSA_RADIO_MOD_BANK_CH_REG(0, ch, RX_PDT, SAT_EN, val);      //TIA SAT enable
        }
        fusa_radio_switch_bank(old_bank);
}
/********************************************************************
;
; This routine:
;       enable 5m auxadc clock
; arg:
;       enable : [bool] 1-enable 0-disable
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_5m_aux_adc_clk_on(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    FUSA_RADIO_MOD_BANK_REG(1, DTSMD1_MUXIN_SEL, ADC_CLK_5M_EN, enable);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       enable auxadc1 & reset
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc1_on_reset_on(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(1, DTSDM1_EN, 0x7f);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       auxadc1 reset release
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc1_reset_release(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(1, DTSDM1_EN, 0x7e);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       disable auxadc1
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc1_off(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(1, DTSDM1_EN, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       enable auxadc1 filter1
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc1_filter1_on(void)
{
    uint8 old_bank = fusa_radio_switch_bank(2);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(2, DC_FILTER1_RST_EN, 0x1);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       enable auxadc1 filter1
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc1_filter1_rst(void)
{
    uint8 old_bank = fusa_radio_switch_bank(2);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(2, DC_FILTER1_RST_EN, 0x01);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       set auxadc1 input mux to test_point_mux1
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_set_auxadc1_inputmux_to_test_point_mux1(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* auxadc1 input mux : test_point_mux1 */
    FUSA_RADIO_MOD_BANK_REG(1, DTSMD1_MUXIN_SEL, DTSDM_MUXIN_SEL, 0x3);
    fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, TPANA1, EN, 0x0);
    FUSA_RADIO_MOD_BANK_REG(0, TPANA1, TEST_MUX_1_EN, 0x1);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       unset auxadc1 input mux
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_unset_auxadc1_inputmux(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* disable auxadc1 input mux */
    FUSA_RADIO_MOD_BANK_REG(1, DTSMD1_MUXIN_SEL, DTSDM_MUXIN_SEL, 0x0);
    FUSA_RADIO_MOD_BANK_REG(1, DTSMD1_MUXIN_SEL, ADC_MUXN_EN, 0x0);
    FUSA_RADIO_MOD_BANK_REG(1, DTSMD1_MUXIN_SEL, ADC_MUXP_EN, 0x0);
    fusa_radio_switch_bank(0);
    FUSA_RADIO_WRITE_BANK_REG(0, TPANA1, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/***********************************************
;
; This routine:
;       enable auxadc1 voltage measurement of testpoint_mux1
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************/
void fusa_radio_auxadc1_voltage_on(void)
{
    /* enable 5m auxadc clock */
    fusa_radio_5m_aux_adc_clk_on(true);
    /* auxadc1 on */
    fusa_radio_auxadc1_on_reset_on();
    /* set auxadc1 mux */
    fusa_radio_set_auxadc1_inputmux_to_test_point_mux1();
    /* auxadc1 reset release */
    fusa_radio_auxadc1_reset_release();
    /* filter1 on */
    fusa_radio_auxadc1_filter1_on();
    /* filter1 reset release and wait 1.2ms */
    fusa_radio_auxadc1_filter1_rst();
    fusa_delay_us(1200);//TODO:
}

/***********************************************
;
; This routine:
;       disable auxadc1 voltage measurement of testpoint_mux1
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************/
void fusa_radio_auxadc1_voltage_off(void)
{
    /* unset auxadc1 mux */
    fusa_radio_unset_auxadc1_inputmux();
    /* disable auxadc1 */
    fusa_radio_auxadc1_off();
    /* disable 5m auxadc clock */
    fusa_radio_5m_aux_adc_clk_on(false);
}

/***********************************************
;
; This routine:
;       get auxadc1 readback register value
; arg:
;       void
; return:
;       uint32
; Change tracking:
;       Ver0.0.1 :
;***********************************************/
uint32 fusa_radio_get_auxadc1_dout(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 doutL, doutM, doutH;
    uint32 dout;
    doutL = FUSA_RADIO_READ_BANK_REG(1, DTSDM1_DAT0);
    doutM = FUSA_RADIO_READ_BANK_REG(1, DTSDM1_DAT1);
    doutH = FUSA_RADIO_READ_BANK_REG(1, DTSDM1_DAT2);
    dout = doutL + ( doutM << 8 ) + ( doutH << 16 );
    fusa_radio_switch_bank(old_bank);
    return dout;
}

/********************************************************************
;
; This routine:
;       enable auxadc2 & reset
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc2_on_reset_on(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(1, DTSDM2_EN, 0x7f);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       auxadc2 reset release
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc2_reset_release(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(1, DTSDM2_EN, 0x7e);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       disable auxadc2
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc2_off(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(1, DTSDM2_EN, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       enable auxadc2 filter2
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc2_filter2_on(void)
{
    uint8 old_bank = fusa_radio_switch_bank(2);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(2, DC_FILTER2_RST_EN, 0x1);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       enable auxadc2 filter2
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_auxadc2_filter2_rst(void)
{
    uint8 old_bank = fusa_radio_switch_bank(2);
    /* auxadc1 on and reset */
    FUSA_RADIO_WRITE_BANK_REG(2, DC_FILTER2_RST_EN, 0x01);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       set auxadc2 input mux to temp sensor
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_set_auxadc2_inputmux_to_ts(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* auxadc2 input mux : temp sensor */
    FUSA_RADIO_WRITE_BANK_REG(1, DTSMD2_MUXIN_SEL, 0x90);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       set auxadc2 input mux to test_point_mux1
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_set_auxadc2_inputmux_to_test_point_mux2(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* auxadc2 input mux : test_point_mux2 */
    FUSA_RADIO_WRITE_BANK_REG(1, DTSMD2_MUXIN_SEL, 0x93);
    fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, TPANA2, TPANA2_EN, 0x0);
    FUSA_RADIO_MOD_BANK_REG(0, TPANA2, TEST_MUX_2_EN, 0x1);
    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       unset auxadc2 input mux
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*******************************************************************/
void fusa_radio_unset_auxadc2_inputmux(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* disable auxadc1 input mux */
    FUSA_RADIO_WRITE_BANK_REG(1, DTSMD2_MUXIN_SEL, 0x0);
    fusa_radio_switch_bank(0);
    FUSA_RADIO_WRITE_BANK_REG(0, TPANA2, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/***********************************************
;
; This routine:
;       enable auxadc2 voltage measurement of testpoint_mux2
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************/
void fusa_radio_auxadc2_voltage_on(void)
{
    /* enable 5m auxadc clock */
    fusa_radio_5m_aux_adc_clk_on(true);
    /* auxadc1 on */
    fusa_radio_auxadc2_on_reset_on();
    /* set auxadc1 mux */
    fusa_radio_set_auxadc2_inputmux_to_test_point_mux2();
    /* auxadc1 reset release */
    fusa_radio_auxadc2_reset_release();
    /* filter1 on */
    fusa_radio_auxadc2_filter2_on();
    /* filter1 reset release and wait 1.2ms */
    fusa_radio_auxadc2_filter2_rst();
    fusa_delay_us(1200);
}

/***********************************************
;
; This routine:
;       enable auxadc2 measurement of temp sensor
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************/
void fusa_radio_auxadc2_ts_on(void)
{
    /* enable 5m auxadc clock */
    fusa_radio_5m_aux_adc_clk_on(true);
    /* auxadc1 on */
    fusa_radio_auxadc2_on_reset_on();
    /* set auxadc1 mux */
    fusa_radio_set_auxadc2_inputmux_to_ts();
    /* auxadc1 reset release */
    fusa_radio_auxadc2_reset_release();
    /* filter1 on */
    fusa_radio_auxadc2_filter2_on();
    /* filter1 reset release and wait 1.2ms */
    fusa_radio_auxadc2_filter2_rst();
    fusa_delay_us(1200);
}

/***********************************************
;
; This routine:
;       disable auxadc2 voltage measurement of testpoint_mux1
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************/
void fusa_radio_auxadc2_voltage_off(void)
{
    /* unset auxadc2 mux */
    fusa_radio_unset_auxadc2_inputmux();
    /* disable auxadc2 */
    fusa_radio_auxadc2_off();
    /* disable 5m auxadc clock */
    fusa_radio_5m_aux_adc_clk_on(false);
}

/***********************************************
;
; This routine:
;       get auxadc2 readback register value
; arg:
;       void
; return:
;       uint32
; Change tracking:
;       Ver0.0.1 :
;***********************************************/
uint32 fusa_radio_get_auxadc2_dout(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 doutL, doutM, doutH;
    uint32 dout;
    doutL = FUSA_RADIO_READ_BANK_REG(1, DTSDM2_DAT0);
    doutM = FUSA_RADIO_READ_BANK_REG(1, DTSDM2_DAT1);
    doutH = FUSA_RADIO_READ_BANK_REG(1, DTSDM2_DAT2);
    dout = doutL + ( doutM << 8 ) + ( doutH << 16 );
    fusa_radio_switch_bank(old_bank);
    return dout;
}

/**************************************************************
;
; This routine:
;       get pa bias status for all channels
; arg:
;       : [uint8 pointer] tx status array
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_get_tx_status(uint8_t *tx_status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* get tx0 status */
    tx_status[0] = FUSA_RADIO_READ_BANK_REG(1, CH0_TX_EN0);
    /* get tx1 status */
    tx_status[1] = FUSA_RADIO_READ_BANK_REG(1, CH1_TX_EN0);
    /* get tx2 status */
    tx_status[2] = FUSA_RADIO_READ_BANK_REG(1, CH2_TX_EN0);
    /* get tx3 status */
    tx_status[3] = FUSA_RADIO_READ_BANK_REG(1, CH3_TX_EN0);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       set pa bias status for all channels
; arg:
;       tx_status: [uint8 pointer] tx status array
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_tx_status(uint8_t *tx_status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    /* set tx0 status */
    FUSA_RADIO_WRITE_BANK_REG(1, CH0_TX_EN0, tx_status[0]);
    /* set tx1 status */
    FUSA_RADIO_WRITE_BANK_REG(1, CH1_TX_EN0, tx_status[1]);
    /* set tx2 status */
    FUSA_RADIO_WRITE_BANK_REG(1, CH2_TX_EN0, tx_status[2]);
    /* set tx3 status */
    FUSA_RADIO_WRITE_BANK_REG(1, CH3_TX_EN0, tx_status[3]);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       enable all tx ldo
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_tx_ldo_on(void)
{
        uint8 old_bank = fusa_radio_switch_bank(1);
        FUSA_RADIO_WRITE_BANK_REG(1, TX_LDO_EN, 0xff);
        fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       set tx ldo and pa bias status for all channels
; arg:
;       channel_index: [sint32] -1/0/1/2/3
;        enable: [uint32] true/false
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_tx_ch_status(sint32 channel_index, uint32 enable)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 enable_4bit;

    if (enable == true) {
        enable_4bit = 0x0f;
    } else {
        enable_4bit = 0x00;
    }
    if (channel_index == -1) {
        FUSA_RADIO_WRITE_BANK_REG(1, CH0_TX_EN0, enable_4bit);
        FUSA_RADIO_WRITE_BANK_REG(1, CH1_TX_EN0, enable_4bit);
        FUSA_RADIO_WRITE_BANK_REG(1, CH2_TX_EN0, enable_4bit);
        FUSA_RADIO_WRITE_BANK_REG(1, CH3_TX_EN0, enable_4bit);
    } else if (channel_index == 0) {
        FUSA_RADIO_WRITE_BANK_REG(1, CH0_TX_EN0, enable_4bit);
    } else if (channel_index == 1) {
        FUSA_RADIO_WRITE_BANK_REG(1, CH1_TX_EN0, enable_4bit);
    } else if (channel_index == 2) {
        FUSA_RADIO_WRITE_BANK_REG(1, CH2_TX_EN0, enable_4bit);
    } else if (channel_index == 3) {
        FUSA_RADIO_WRITE_BANK_REG(1, CH3_TX_EN0, enable_4bit);
    }
    fusa_radio_switch_bank(old_bank);
}

/***********************************************
;
; This routine:
;       get start freq of current frame
; arg:
;       void
; return:
;       uint32
; Change tracking:
;       Ver0.0.1 :
;***********************************************/
float fusa_radio_get_start_freq(uint32 freq_reg)
{
    uint8 frame_type_id, freq_reg_l, freq_reg_m, freq_reg_h, freq_reg_int;
    float start_freq;

    frame_type_id = fusa_radio_get_rtl_frame_type();
    uint8 old_bank = fusa_radio_switch_bank(5 + frame_type_id);
    /* get start frequency */
    freq_reg_l = FUSA_RADIO_READ_BANK_REG(5, FMCW_START_FREQ_1_0);
    freq_reg_m = FUSA_RADIO_READ_BANK_REG(5, FMCW_START_FREQ_1_1);
    freq_reg_h = FUSA_RADIO_READ_BANK_REG(5, FMCW_START_FREQ_1_2);
    freq_reg_int = FUSA_RADIO_READ_BANK_REG(5, FMCW_START_FREQ_1_3);
    freq_reg = freq_reg_l + (freq_reg_m << 8) + (freq_reg_h << 16) + (freq_reg_int << 24);
    //FmcwMiddleFreq = currfreq/3.2-16;
    //round( FmcwMiddleFreq * (1<<28) );
    start_freq = ((float)freq_reg / (1<<28) + 16) * 3.2;
    fusa_radio_switch_bank(old_bank);
    return start_freq;

}

/**************************************************************
;
; This routine:
;       enable ldo25 and ldo11 for safety mechanism
; arg:
;       enable: [bool] true/false
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_ldo_for_sm_on(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, LDO25_SM, LDO_EN, enable);
    FUSA_RADIO_MOD_BANK_REG(0, LDO11_SM, LDO_EN, enable);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       enable ldo for rxrfs
; arg:
;       enable: [bool] true/false
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_ldo_for_rxrfs_on(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, RX_LDO0, LDO11_RFN_EN, enable);
    FUSA_RADIO_MOD_BANK_REG(0, RX_LDO1, LDO11_RFS_EN, enable);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for lvds25 status
; arg:
;       void
; return:
;       uint8: 0/1
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_lvds25_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(2);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(2, LVDS_LDO25, LDO25_LVDS_LDO_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable ldo for lvds25
; arg:
;       status: [uint8] 0/1
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_lvds25(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(2);
    FUSA_RADIO_MOD_BANK_REG(2, LVDS_LDO25, LDO25_LVDS_LDO_EN, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for tx0 status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_tx0_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO_EN, LDO11_TX0_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable ldo for tx0
; arg:
;       status: [uint8]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_tx0(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX0_EN, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for pa0 status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_pa0_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO_EN, LDO11_TX0_PA_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable ldo for pa0
; arg:
;       status: [uint8]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_pa0(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX0_PA_EN, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for tx1 status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_tx1_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO_EN, LDO11_TX1_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable ldo for tx1
; arg:
;       status: [uint8]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_tx1(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX1_EN, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for pa1 status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_pa1_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO_EN, LDO11_TX1_PA_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable ldo for pa1
; arg:
;       status: [uint8]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_pa1(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX1_PA_EN, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for tx2 status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_tx2_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO_EN, LDO11_TX2_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable ldo for tx2
; arg:
;       status: [uint8]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_tx2(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX2_EN, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for pa2 status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_pa2_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO_EN, LDO11_TX2_PA_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable ldo for pa2
; arg:
;       status: [uint8]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_pa2(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX2_PA_EN, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for tx3 status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_tx3_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO_EN, LDO11_TX3_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable ldo for tx3
; arg:
;       status: [uint8]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_tx3(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX3_EN, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for pa3 status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_pa3_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO_EN, LDO11_TX3_PA_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable ldo for pa3
; arg:
;       status: [uint8]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_pa3(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(1);
    FUSA_RADIO_MOD_BANK_REG(1, TX_LDO_EN, LDO11_TX3_PA_EN, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       enable ldo monitor
; arg:
;       ldo_item: [fusa_sm1_item_e]
;       enable: [bool] true/false
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_sm_ldo_monitor_on(fusa_sm1_item_e ldo_item, bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(10);

    switch(ldo_item) {
    case SM1_LDO25_FMCWPLL:
        fusa_radio_reg_mod(0x40, 1, 1, enable);
        break;
    case SM1_LDO25_RXBBS:
        fusa_radio_reg_mod(0x40, 3, 1, enable);
        break;
    case SM1_LDO25_RXBBN:
        fusa_radio_reg_mod(0x40, 4, 1, enable);
        break;
    case SM1_LDO25_REFPLL_XTAL:
        fusa_radio_reg_mod(0x40, 5, 1, enable);
        break;
    case SM1_LDO25_REFPLL_PLL:
        fusa_radio_reg_mod(0x40, 6, 1, enable);
        break;
    case SM1_LDO25_PMU:
        fusa_radio_reg_mod(0x40, 7, 1, enable);
        break;
    case SM1_LDO25_LVDS:
        fusa_radio_reg_mod(0x41, 0, 1, enable);
        break;
    case SM1_LDO25_ADC23:
        fusa_radio_reg_mod(0x41, 1, 1, enable);
        break;
    case SM1_LDO25_ADC01:
        fusa_radio_reg_mod(0x41, 2, 1, enable);
        break;
    case SM1_LDO13_ADC23:
        fusa_radio_reg_mod(0x41, 3, 1, enable);
        break;
    case SM1_LDO13_ADC01:
        fusa_radio_reg_mod(0x41, 4, 1, enable);
        break;
    case SM1_LDO12_FMCWPLL_MMD:
        fusa_radio_reg_mod(0x41, 5, 1, enable);
        break;
    case SM1_LDO11_TX3:
        fusa_radio_reg_mod(0x42, 1, 1, enable);
        break;
    case SM1_LDO11_TX2:
        fusa_radio_reg_mod(0x42, 2, 1, enable);
        break;
    case SM1_LDO11_TX1:
        fusa_radio_reg_mod(0x42, 3, 1, enable);
        break;
    case SM1_LDO11_TX0:
        fusa_radio_reg_mod(0x42, 4, 1, enable);
        break;
    case SM1_LDO11_SPI:
        fusa_radio_reg_mod(0x42, 5, 1, enable);
        break;
    case SM1_LDO11_RXRFS:
        fusa_radio_reg_mod(0x42, 6, 1, enable);
        break;
    case SM1_LDO11_RXRFN:
        fusa_radio_reg_mod(0x42, 7, 1, enable);
        break;
    case SM1_LDO11_REFPLL_VCO:
        fusa_radio_reg_mod(0x43, 0, 1, enable);
        break;
    case SM1_LDO11_REFPLL_MMD2:
        fusa_radio_reg_mod(0x43, 1, 1, enable);
        break;
    case SM1_LDO11_REFPLL_MMD:
        fusa_radio_reg_mod(0x43, 2, 1, enable);
        break;
    case SM1_LDO11_LOMAIN:
        fusa_radio_reg_mod(0x43, 4, 1, enable);
        break;
    case SM1_LDO11_FMCWPLL_VCO:
        fusa_radio_reg_mod(0x43, 5, 1, enable);
        break;
    case SM1_LDO11_ADC23:
        fusa_radio_reg_mod(0x43, 7, 1, enable);
        break;
    case SM1_LDO11_ADC01:
        fusa_radio_reg_mod(0x44, 0, 1, enable);
        break;
    case SM1_LDO10_TXLO:
        fusa_radio_reg_mod(0x44, 1, 1, enable);
        break;
    case SM1_LDO10_PA3:
        fusa_radio_reg_mod(0x44, 2, 1, enable);
        break;
    case SM1_LDO10_PA2:
        fusa_radio_reg_mod(0x44, 3, 1, enable);
        break;
    case SM1_LDO10_PA1:
        fusa_radio_reg_mod(0x44, 4, 1, enable);
        break;
    case SM1_LDO10_PA0:
        fusa_radio_reg_mod(0x44, 5, 1, enable);
        break;
    case SM1_LDO10_RXLO:
        fusa_radio_reg_mod(0x44, 6, 1, enable);
        break;
    default:
        break;
    }

    fusa_radio_switch_bank(old_bank);
}

void fusa_radio_sm_ldo_monitor_en(uint64 en_status)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint8 temp0 = (en_status >> 0) & 0xff;
    uint8 temp1 = (en_status >> 8) & 0xff;
    uint8 temp2 = (en_status >> 16) & 0xff;
    uint8 temp3 = (en_status >> 24) & 0xff;
    uint8 temp4 = (en_status >> 32) & 0xff;
    // log_fusa("ldo monitor en 4~0 =0x%x  %x  %x  %x  %x\r\n", temp4, temp3, temp2, temp1, temp0);
    fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_0, temp0);
    fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_1, temp1);
    fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_2, temp2);
    fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_3, temp3);
    fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_4, temp4);
    // fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_0, (en_status >> 0 ) && 0xff);
    // fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_1, (en_status >> 8 ) && 0xff);
    // fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_2, (en_status >> 16) && 0xff);
    // fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_3, (en_status >> 24) && 0xff);
    // fusa_radio_reg_write(RADIO_BK10_LDO_MONITOR_EN_4, (en_status >> 32) && 0xff);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       get ldo voltage selection
; arg:
;       ldo_item: [fusa_sm1_item_e]
; return:
;       uint8: ldo voltage selection
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_sm_ldo_monitor_get_ldo_sel(fusa_sm1_item_e ldo_item)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    uint8 ldo_vsel = 0;

    switch(ldo_item) {
    case SM1_LDO25_FMCWPLL:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, FMCWPLL_LDO1, LDO25_VOUT_SEL);
        break;
    case SM1_LDO25_RXBBS:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, RX_LDO3, LDO25_BBS_VOUT_SEL);
        break;
    case SM1_LDO25_RXBBN:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, RX_LDO2, LDO25_BBN_VOUT_SEL);
        break;
    case SM1_LDO25_REFPLL_XTAL:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, REFPLL_LDO0, LDO25_XTAL_VSEL);
        break;
    case SM1_LDO25_REFPLL_PLL:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, REFPLL_LDO1, LDO25_PLL_VSEL);
        break;
    case SM1_LDO25_PMU:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, LDO25_PMU, VOUT_SEL);
        break;
    case SM1_LDO25_LVDS:
        fusa_radio_switch_bank(2);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(2, LVDS_LDO25, LDO25_LVDS_VSEL);
        break;
    case SM1_LDO25_ADC23:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, ADC_LDO5, LDO25_ADC34_VOUT_SEL);
        break;
    case SM1_LDO25_ADC01:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, ADC_LDO2, LDO25_ADC12_VOUT_SEL);
        break;
    case SM1_LDO13_ADC23:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, ADC_LDO4, LDO12_ADC34_VOUT_SEL);
        break;
    case SM1_LDO13_ADC01:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, ADC_LDO1, LDO12_ADC12_VOUT_SEL);
        break;
    case SM1_LDO12_FMCWPLL_MMD:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, FMCWPLL_LDO3, LDO11_MMD_VOUT_SEL);
        break;
    case SM1_LDO11_TX3:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO3, LDO11_TX3_VOUT_SEL);
        break;
    case SM1_LDO11_TX2:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO2, LDO11_TX2_VOUT_SEL);
        break;
    case SM1_LDO11_TX1:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO1, LDO11_TX1_VOUT_SEL);
        break;
    case SM1_LDO11_TX0:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, TX_LDO0, LDO11_TX0_VOUT_SEL);
        break;
    case SM1_LDO11_SPI:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, POR, LDO11_SPI_VOUT_SEL);
        break;
    case SM1_LDO11_RXRFS:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, RX_LDO1, LDO11_RFS_VOUT_SEL);
        break;
    case SM1_LDO11_RXRFN:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, RX_LDO0, LDO11_RFN_VOUT_SEL);
        break;
    case SM1_LDO11_REFPLL_VCO:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, REFPLL_LDO2, LDO11_VCO_VSEL);
        break;
    case SM1_LDO11_REFPLL_MMD2:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, REFPLL_LDO4, LDO11_MMD2_VSEL);
        break;
    case SM1_LDO11_REFPLL_MMD:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, REFPLL_LDO3, LDO11_MMD_VSEL);
        break;
    case SM1_LDO11_LOMAIN:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, LO_LDO0, LDO11_LOMAINPATH_VOUT_SEL);
        break;
    case SM1_LDO11_FMCWPLL_VCO:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, FMCWPLL_LDO2, LDO11_VCO_VOUT_SEL);
        break;
    case SM1_LDO11_ADC23:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, ADC_LDO3, LDO11_ADC34_VOUT_SEL);
        break;
    case SM1_LDO11_ADC01:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, ADC_LDO0, LDO11_ADC12_VOUT_SEL);
        break;
    case SM1_LDO10_TXLO:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, LO_LDO2, LDO11_TXLO_VOUT_SEL);
        break;
    case SM1_LDO10_PA3:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, CH3_PA_LDO, LDO11_TX_PA_VOUT_SEL);
        break;
    case SM1_LDO10_PA2:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, CH2_PA_LDO, LDO11_TX_PA_VOUT_SEL);
        break;
    case SM1_LDO10_PA1:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, CH2_PA_LDO, LDO11_TX_PA_VOUT_SEL);
        break;
    case SM1_LDO10_PA0:
        fusa_radio_switch_bank(1);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(1, CH2_PA_LDO, LDO11_TX_PA_VOUT_SEL);
        break;
    case SM1_LDO10_RXLO:
        fusa_radio_switch_bank(0);
        ldo_vsel = FUSA_RADIO_READ_BANK_REG_FIELD(0, LO_LDO3, LDO11_RXLO_VOUT_SEL);
        break;
    default:
        break;
    }

    fusa_radio_switch_bank(old_bank);
    return ldo_vsel;
}

/**************************************************************
;
; This routine:
;       get ldo threshold selection
; arg:
;       ldo_item: [fusa_sm1_item_e]
; return:
;       fusa_sm1_thres_level_e: 1V/1.1V/1.2V/1.3V/2.5V
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
fusa_sm1_thres_level_e fusa_radio_sm_ldo_monitor_get_ldo_thres(fusa_sm1_item_e ldo_item)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    fusa_sm1_thres_level_e thres = 0;

    switch(ldo_item) {
    case SM1_LDO25_FMCWPLL:
    case SM1_LDO25_RXBBS:
    case SM1_LDO25_RXBBN:
    case SM1_LDO25_REFPLL_XTAL:
    case SM1_LDO25_REFPLL_PLL:
    case SM1_LDO25_PMU:
    case SM1_LDO25_LVDS:
    case SM1_LDO25_ADC23:
    case SM1_LDO25_ADC01:
        thres = SM1_THRES_2V5;
        break;
    case SM1_LDO13_ADC23:
    case SM1_LDO13_ADC01:
        thres = SM1_THRES_1V3;
        break;
    case SM1_LDO12_FMCWPLL_MMD:
        thres = SM1_THRES_1V2;
        break;
    case SM1_LDO11_TX3:
    case SM1_LDO11_TX2:
    case SM1_LDO11_TX1:
    case SM1_LDO11_TX0:
    case SM1_LDO11_SPI:
    case SM1_LDO11_RXRFS:
    case SM1_LDO11_RXRFN:
    case SM1_LDO11_REFPLL_VCO:
    case SM1_LDO11_REFPLL_MMD2:
    case SM1_LDO11_REFPLL_MMD:
    case SM1_LDO11_LOMAIN:
    case SM1_LDO11_FMCWPLL_VCO:
    case SM1_LDO11_ADC23:
    case SM1_LDO11_ADC01:
        thres = SM1_THRES_1V1;
        break;
    case SM1_LDO10_TXLO:
    case SM1_LDO10_PA3:
    case SM1_LDO10_PA2:
    case SM1_LDO10_PA1:
    case SM1_LDO10_PA0:
    case SM1_LDO10_RXLO:
        thres = SM1_THRES_1V;
        break;
    default:
        break;
    }

    fusa_radio_switch_bank(old_bank);
    return thres;
}

/**************************************************************
;
; This routine:
;       calculate ldo voltage value from ldo voltage selection
; arg:
;       ldo_item: [fusa_sm1_item_e]
;       thres: [fusa_sm1_thres_level_e] 1V/1.1V/1.2V/1.3V/2.5V
;       ldo_sel: [uint8] ldo voltage selection 0-7 or 0-15
; return:
;       float: ldo voltage value
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
float fusa_radio_sm_ldo_monitor_ldo_volt_cal(fusa_sm1_item_e ldo_item, fusa_sm1_thres_level_e thres, uint8 ldo_sel)
{
    float Vldo = 0.0;
    switch(thres) {
    case SM1_THRES_2V5:
        Vldo = (2.3 + (float)ldo_sel * 0.05);
        break;
    case SM1_THRES_1V3:
        Vldo = (1.1 + (float)ldo_sel * 0.05);
        break;
    case SM1_THRES_1V2:
        Vldo = (1 + (float)ldo_sel * 0.05);
        break;
    case SM1_THRES_1V1:
        if ((ldo_item == SM1_LDO11_REFPLL_MMD) || (ldo_item == SM1_LDO11_REFPLL_MMD2)) {
            Vldo = (1 + (float)ldo_sel * 0.05);
        } else {
             Vldo = (0.9 + (float)ldo_sel * 0.05);
        }
        break;
    case SM1_THRES_1V:
        if ((ldo_item == SM1_LDO10_TXLO) || (ldo_item == SM1_LDO10_RXLO)) {
            Vldo = (0.8 + (float)ldo_sel * 0.05);
        } else {
            Vldo = (0.68 + (float)ldo_sel * 0.032);
        }
         break;
    default:
        break;
    }
    return Vldo;
}

/**************************************************************
;
; This routine:
;       calculate reg value from Vnominal and Vthreshold
; arg:
;       Vnominal: [float] nominal voltage 1V/1.1V/1.2V/1.3V/2.5V
;       reg_th:  [float] threshold, i.e. -0.1, 0.15
; return:
;       uint32: threshold reg value
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint32 fusa_radio_sm_threshold_reg_calc(float Vnominal, float reg_th)
{
//    log_fusa("Vnominal = %f, reg_th = %f\r\n", Vnominal, reg_th);
    uint32 calc_reg_value = floor(((Vnominal * (1 + reg_th/100) - 1.65) / 3.3 * (1 << 17)) + (1 << 17));
    return calc_reg_value;
}

/**************************************************************
;
; This routine:
;       write threshold value into accordingly regs for Safety Monitor1: LDO Monitor
; arg:
;       thres: [fusa_sm1_thres_level_e] 1V/1.1V/1.2V/1.3V/2.5V
;       range: [fusa_sm1_thres_range_e] high/low
;       reg_calc: nominal value calculated by function fmcw_radio_sm_threshold_reg_calc
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_sm_ldo_monitor_set_threshold(fusa_sm1_thres_level_e thres, fusa_sm1_thres_range_e range, uint32 reg_calc)
{
    uint8 old_bank = fusa_radio_switch_bank(10);

    switch(thres) {
    case SM1_THRES_2V5:
        switch (range) {
        case SM1_THRES_RANGE_L:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH25150_LOW_0,  REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH25150_LOW_1,  REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH25150_LOW_2,  REG_H(reg_calc));
            break;
        case SM1_THRES_RANGE_H:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH25150_HIGH_0, REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH25150_HIGH_1, REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH25150_HIGH_2, REG_H(reg_calc));
            break;
        default:
            break;
        }
        break;
    case SM1_THRES_1V3:
        switch (range) {
        case SM1_THRES_RANGE_L:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH13150_LOW_0,  REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH13150_LOW_1,  REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH13150_LOW_2,  REG_H(reg_calc));
            break;
        case SM1_THRES_RANGE_H:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH13150_HIGH_0, REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH13150_HIGH_1, REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH13150_HIGH_2, REG_H(reg_calc));
            break;
        default:
            break;
        }
        break;
    case SM1_THRES_1V2:
        switch (range) {
        case SM1_THRES_RANGE_L:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH12150_LOW_0,  REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH12150_LOW_1,  REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH12150_LOW_2,  REG_H(reg_calc));
            break;
        case SM1_THRES_RANGE_H:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH12150_HIGH_0, REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH12150_HIGH_1, REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH12150_HIGH_2, REG_H(reg_calc));
            break;
        default:
            break;
        }
        break;
    case SM1_THRES_1V1:
        switch (range) {
        case SM1_THRES_RANGE_L:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH11150_LOW_0,  REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH11150_LOW_1,  REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH11150_LOW_2,  REG_H(reg_calc));
            break;
        case SM1_THRES_RANGE_H:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH11150_HIGH_0, REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH11150_HIGH_1, REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH11150_HIGH_2, REG_H(reg_calc));
            break;
        default:
            break;
        }
        break;
    case SM1_THRES_1V:
        switch (range) {
        case SM1_THRES_RANGE_L:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH10150_LOW_0,  REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH10150_LOW_1,  REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH10150_LOW_2,  REG_H(reg_calc));
            break;
        case SM1_THRES_RANGE_H:
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH10150_HIGH_0, REG_L(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH10150_HIGH_1, REG_M(reg_calc));
            FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTH10150_HIGH_2, REG_H(reg_calc));
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    fusa_radio_switch_bank(old_bank);

}

/**************************************************************
;
; This routine:
;       set ldo monitor part
; arg:
;       current_part: [uint8] 0/1/2/3 total 4 parts
;       ldo_part_array: [fusa_sm1_item_e] ldo_part_array
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_sm_ldo_monitor_set_part(uint8 current_part, fusa_sm1_item_e *ldo_part_array)
{
    switch(current_part) {
    case 0:
        ldo_part_array[0] = SM1_LDO25_FMCWPLL;
        ldo_part_array[1] = SM1_LDO25_RXBBS;
        ldo_part_array[2] = SM1_LDO25_RXBBN;
        ldo_part_array[3] = SM1_LDO25_REFPLL_XTAL;
        ldo_part_array[4] = SM1_LDO25_REFPLL_PLL;
        ldo_part_array[5] = SM1_LDO25_PMU;
        ldo_part_array[6] = SM1_LDO25_LVDS;
        ldo_part_array[7] = SM1_LDO25_ADC23;
        break;
    case 1:
        ldo_part_array[0] = SM1_LDO25_ADC01;
        ldo_part_array[1] = SM1_LDO13_ADC23;
        ldo_part_array[2] = SM1_LDO13_ADC01;
        ldo_part_array[3] = SM1_LDO11_SPI;
        ldo_part_array[4] = SM1_LDO11_RXRFS;
        ldo_part_array[5] = SM1_LDO11_RXRFN;
        ldo_part_array[6] = SM1_LDO11_ADC23;
        ldo_part_array[7] = SM1_LDO11_ADC01;
        break;
    case 2:
        ldo_part_array[0] = SM1_LDO12_FMCWPLL_MMD;
        ldo_part_array[1] = SM1_LDO11_REFPLL_VCO;
        ldo_part_array[2] = SM1_LDO11_REFPLL_MMD2;
        ldo_part_array[3] = SM1_LDO11_REFPLL_MMD;
        ldo_part_array[4] = SM1_LDO11_LOMAIN;
        ldo_part_array[5] = SM1_LDO11_FMCWPLL_VCO;
        ldo_part_array[6] = SM1_LDO10_TXLO;
        ldo_part_array[7] = SM1_LDO10_RXLO;
        break;
    case 3:
        ldo_part_array[0] = SM1_LDO11_TX3;
        ldo_part_array[1] = SM1_LDO11_TX2;
        ldo_part_array[2] = SM1_LDO11_TX1;
        ldo_part_array[3] = SM1_LDO11_TX0;
        ldo_part_array[4] = SM1_LDO10_PA3;
        ldo_part_array[5] = SM1_LDO10_PA2;
        ldo_part_array[6] = SM1_LDO10_PA1;
        ldo_part_array[7] = SM1_LDO10_PA0;
        break;
    default:
        break;
    }
}

/********************************************************************************
;
; This routine:
;       get nominal value of 5 voltage levels calculating from all ldos
; arg:
;       part_array: [fusa_sm1_item_e*] ldo_item_part_array
;       Vnominal_array: [float*] threshold voltage array
; return:
;       float: threshold voltage pointer
; Change tracking:
;       Ver0.0.1 :
;*********************************************************************************/
void fusa_radio_sm_ldo_monitor_threshold_nomial_cal(float *Vnominal_array)
{
    fusa_sm1_thres_level_e thres = 0;
    uint8_t ldo_sel;
    float Vldo = 0.0;
    float Vldo_2V5_min = 5.0;
    float Vldo_1V3_min = 5.0;
    float Vldo_1V2_min = 5.0;
    float Vldo_1V1_min = 5.0;
    float Vldo_1V_min  = 5.0;
    float Vldo_2V5_max = 0.0;
    float Vldo_1V3_max = 0.0;
    float Vldo_1V2_max = 0.0;
    float Vldo_1V1_max = 0.0;
    float Vldo_1V_max  = 0.0;

    for (uint8 i = 0; i < 32; i++) {
        /* get ldo sel */
        ldo_sel = fusa_radio_sm_ldo_monitor_get_ldo_sel(i);
        thres = fusa_radio_sm_ldo_monitor_get_ldo_thres(i);
        /* get ldo voltage */
        Vldo = fusa_radio_sm_ldo_monitor_ldo_volt_cal(i, thres, ldo_sel);
        /* get max and min voltage */
        switch (thres) {
        case SM1_THRES_2V5:
            Vldo_2V5_min = fmin(Vldo_2V5_min, Vldo);
            Vldo_2V5_max = fmax(Vldo_2V5_max, Vldo);
            //log_fusa("Vldo_2V5_min = %f, Vldo_2V5_max = %f\r\n", Vldo_2V5_min, Vldo_2V5_max);
            break;
        case SM1_THRES_1V3:
            Vldo_1V3_min = fmin(Vldo_1V3_min, Vldo);
            Vldo_1V3_max = fmax(Vldo_1V3_max, Vldo);
            //log_fusa("Vldo_1V3_min = %f, Vldo_1V3_max = %f\r\n", Vldo_1V3_min, Vldo_1V3_max);
            break;
        case SM1_THRES_1V2:
            Vldo_1V2_min = fmin(Vldo_1V2_min, Vldo);
            Vldo_1V2_max = fmax(Vldo_1V2_max, Vldo);
            //log_fusa("Vldo_1V2_min = %f, Vldo_1V2_max = %f\r\n", Vldo_1V2_min, Vldo_1V2_max);
            break;
        case SM1_THRES_1V1:
            Vldo_1V1_min = fmin(Vldo_1V1_min, Vldo);
            Vldo_1V1_max = fmax(Vldo_1V1_max, Vldo);
            //log_fusa("Vldo_1V1_min = %f, Vldo_1V1_max = %f\r\n", Vldo_1V1_min, Vldo_1V1_max);
            break;
        case SM1_THRES_1V:
            Vldo_1V_min = fmin(Vldo_1V_min, Vldo);
            Vldo_1V_max = fmax(Vldo_1V_max, Vldo);
            //log_fusa("Vldo_1V_min = %f, Vldo_1V_max = %f\r\n", Vldo_1V_min, Vldo_1V_max);
            break;
        default:
            break;
        }
    }

    /* calculate nominal voltage using average of min and max voltage */
    Vnominal_array[0] = (Vldo_2V5_min + Vldo_2V5_max)/2;
    Vnominal_array[1] = (Vldo_1V3_min + Vldo_1V3_max)/2;
    Vnominal_array[2] = (Vldo_1V2_min + Vldo_1V2_max)/2;
    Vnominal_array[3] = (Vldo_1V1_min + Vldo_1V1_max)/2;
    Vnominal_array[4]  = (Vldo_1V_min + Vldo_1V_max)/2;
    //log_fusa("Vnominal = %f, %f, %f, %f, %f\r\n", Vnominal_array[0], Vnominal_array[1], Vnominal_array[2], Vnominal_array[3], Vnominal_array[4]);
}
// current part ldo voltage avrage for each range
// V_mid must ponit to an array len five
void fusa_radio_sm_ldo_monitor_v_mid(uint64 cur_part,float *V_mid)
{
    fusa_sm1_thres_level_e thres = 0;
    uint8_t ldo_sel = 0;
    float Vldo = 0.0;
    float Vldo_2V5_min = 5.0;
    float Vldo_1V3_min = 2.6;
    float Vldo_1V2_min = 2.4;
    float Vldo_1V1_min = 2.2;
    float Vldo_1V_min  = 2.0;
    float Vldo_2V5_max = 0.0;
    float Vldo_1V3_max = 0.0;
    float Vldo_1V2_max = 0.0;
    float Vldo_1V1_max = 0.0;
    float Vldo_1V_max  = 0.0;

    for (uint8 i = 0; i < FUSA_RADIO_LDO_NUMBER; i++) {
        if( (cur_part >> i) & 0x01 )
        {
            /* get ldo sel */
            ldo_sel = fusa_radio_sm_ldo_monitor_get_ldo_sel((fusa_sm1_item_e)i);
            thres = fusa_radio_sm_ldo_monitor_get_ldo_thres((fusa_sm1_item_e)i);
            /* get ldo voltage */
            Vldo = fusa_radio_sm_ldo_monitor_ldo_volt_cal((fusa_sm1_item_e)i, thres, ldo_sel);
            /* get max and min voltage */
            switch (thres) {
            case SM1_THRES_2V5:
                Vldo_2V5_min = fmin(Vldo_2V5_min, Vldo);
                Vldo_2V5_max = fmax(Vldo_2V5_max, Vldo);
                //log_fusa("Vldo_2V5_min = %f, Vldo_2V5_max = %f\r\n", Vldo_2V5_min, Vldo_2V5_max);
                break;
            case SM1_THRES_1V3:
                Vldo_1V3_min = fmin(Vldo_1V3_min, Vldo);
                Vldo_1V3_max = fmax(Vldo_1V3_max, Vldo);
                //log_fusa("Vldo_1V3_min = %f, Vldo_1V3_max = %f\r\n", Vldo_1V3_min, Vldo_1V3_max);
                break;
            case SM1_THRES_1V2:
                Vldo_1V2_min = fmin(Vldo_1V2_min, Vldo);
                Vldo_1V2_max = fmax(Vldo_1V2_max, Vldo);
                //log_fusa("Vldo_1V2_min = %f, Vldo_1V2_max = %f\r\n", Vldo_1V2_min, Vldo_1V2_max);
                break;
            case SM1_THRES_1V1:
                Vldo_1V1_min = fmin(Vldo_1V1_min, Vldo);
                Vldo_1V1_max = fmax(Vldo_1V1_max, Vldo);
                //log_fusa("Vldo_1V1_min = %f, Vldo_1V1_max = %f\r\n", Vldo_1V1_min, Vldo_1V1_max);
                break;
            case SM1_THRES_1V:
                Vldo_1V_min = fmin(Vldo_1V_min, Vldo);
                Vldo_1V_max = fmax(Vldo_1V_max, Vldo);
                //log_fusa("Vldo_1V_min = %f, Vldo_1V_max = %f\r\n", Vldo_1V_min, Vldo_1V_max);
                break;
            default:
                break;
            }
        }
    }

    /* calculate nominal voltage using average of min and max voltage */
    V_mid[0] = (Vldo_2V5_min + Vldo_2V5_max) / 2;
    V_mid[1] = (Vldo_1V3_min + Vldo_1V3_max) / 2;
    V_mid[2] = (Vldo_1V2_min + Vldo_1V2_max) / 2;
    V_mid[3] = (Vldo_1V1_min + Vldo_1V1_max) / 2;
    V_mid[4] = (Vldo_1V_min + Vldo_1V_max) / 2;
    //log_fusa("Vnominal = %f, %f, %f, %f, %f\r\n", Vnominal_array[0], Vnominal_array[1], Vnominal_array[2], Vnominal_array[3], Vnominal_array[4]);
}
/**************************************************************
;
; This routine:
;       save ldo status
; arg:
;       ldo_item: [fusa_sm1_item_e]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
static uint8 lvds_ldo_status, tx0_ldo_status, pa0_ldo_status, tx1_ldo_status, pa1_ldo_status = 0, \
             tx2_ldo_status, pa2_ldo_status, tx3_ldo_status, pa3_ldo_status = 0;
void fusa_radio_sm_ldo_monitor_get_ldo_status(fusa_sm1_item_e ldo_item)
{
    switch (ldo_item) {
    case SM1_LDO25_LVDS:
        /* save ldo25 lvds status */
        lvds_ldo_status = fusa_radio_get_ldo_for_lvds25_status();
        /* enable ldo25 lvds */
        fusa_radio_set_ldo_for_lvds25(0x1);
        break;
    case SM1_LDO11_TX0:
        /* save ldo11 tx0 status */
        tx0_ldo_status = fusa_radio_get_ldo_for_tx0_status();
        /* enable ldo11 tx0 */
        fusa_radio_set_ldo_for_tx0(0x1);
        break;
    case SM1_LDO11_TX1:
        /* save ldo11 tx1 status */
        tx1_ldo_status = fusa_radio_get_ldo_for_tx1_status();
        /* enable ldo11 tx1 */
        fusa_radio_set_ldo_for_tx1(0x1);
        break;
    case SM1_LDO11_TX2:
        /* save ldo11 tx2 status */
        tx2_ldo_status = fusa_radio_get_ldo_for_tx2_status();
        /* enable ldo11 tx2 */
        fusa_radio_set_ldo_for_tx2(0x1);
        break;
    case SM1_LDO11_TX3:
        /* save ldo11 tx3 status */
        tx3_ldo_status = fusa_radio_get_ldo_for_tx3_status();
        /* enable ldo11 tx3 */
        fusa_radio_set_ldo_for_tx3(0x1);
        break;
    case SM1_LDO10_PA0:
        /* save ldo10 pa0 status */
        pa0_ldo_status = fusa_radio_get_ldo_for_pa0_status();
        /* enable ldo10 pa0 */
        fusa_radio_set_ldo_for_pa0(0x1);
        break;
    case SM1_LDO10_PA1:
        /* save ldo10 pa0 status */
        pa1_ldo_status = fusa_radio_get_ldo_for_pa1_status();
        /* enable ldo10 pa0 */
        fusa_radio_set_ldo_for_pa1(0x1);
        break;
    case SM1_LDO10_PA2:
        /* save ldo10 pa2 status */
        pa2_ldo_status = fusa_radio_get_ldo_for_pa2_status();
        /* enable ldo10 pa2 */
        fusa_radio_set_ldo_for_pa2(0x1);
        break;
    case SM1_LDO10_PA3:
        /* save ldo10 pa3 status */
        pa3_ldo_status = fusa_radio_get_ldo_for_pa3_status();
        /* enable ldo10 pa3 */
        fusa_radio_set_ldo_for_pa3(0x1);
        break;
    default:
        break;
    }
}

/**************************************************************
;
; This routine:
;       return ldo status
; arg:
;       ldo_item: [fusa_sm1_item_e]
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_sm_ldo_monitor_return_ldo_status(fusa_sm1_item_e ldo_item)
{
    switch (ldo_item) {
    case SM1_LDO25_LVDS:
        /* return ldo25 lvds status */
        fusa_radio_set_ldo_for_lvds25(lvds_ldo_status);
        break;
    case SM1_LDO11_TX0:
        /* return ldo11 tx0 status */
        fusa_radio_set_ldo_for_tx0(tx0_ldo_status);
        break;
    case SM1_LDO11_TX1:
        /* return ldo11 tx1 status */
        fusa_radio_set_ldo_for_tx1(tx1_ldo_status);
        break;
    case SM1_LDO11_TX2:
        /* return ldo11 tx2 status */
        fusa_radio_set_ldo_for_tx2(tx2_ldo_status);
        break;
    case SM1_LDO11_TX3:
        /* return ldo11 tx3 status */
        fusa_radio_set_ldo_for_tx3(tx3_ldo_status);
        break;
    case SM1_LDO10_PA0:
        /* return ldo10 pa0 status */
        fusa_radio_set_ldo_for_pa0(pa0_ldo_status);
        break;
    case SM1_LDO10_PA1:
        /* return ldo10 pa0 status */
        fusa_radio_set_ldo_for_pa1(pa1_ldo_status);
        break;
    case SM1_LDO10_PA2:
        /* return ldo10 pa2 status */
        fusa_radio_set_ldo_for_pa2(pa2_ldo_status);
        break;
    case SM1_LDO10_PA3:
        /* return ldo10 pa3 status */
        fusa_radio_set_ldo_for_pa3(pa3_ldo_status);
        break;
    default:
        break;
    }
}

/**************************************************************
;
; This routine:
;       set ldo monitor timing threshold
; arg:
;       radio
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_timing_th(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    /* set threshold of LDO Monitor timing */
    FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_TIME_CNT_1, 0x3F);
    /* set ldo monitor switch time 1.2ms */
    FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_CTU_SIZE, 0x18);
    fusa_radio_switch_bank(old_bank);
}

/*******************************************************************
;
; This routine:
;       set ldo monitor voltage threshold as vth150
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;******************************************************************/
void fusa_radio_set_threshold_for_ldo_Vth(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_VTHSEL, 0x1f);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       enable ldo monitor
; arg:
;       enable: [bool] true / false
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_reset_ldo_monitor(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_WRITE_BANK_REG(10, LDO_MONITOR_RSTN, enable);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       clear ldo monitor
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_clear_ldo_monitor(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_LDO_CLEAR, 0x00);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_LDO_CLEAR, 0x01);
    fusa_delay_us(100);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_LDO_CLEAR, 0x00);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       start ldo monitor
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_ldo_monitor_test_on(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_LDO_TEST_START, 0x00);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_LDO_TEST_START, 0x01);
    fusa_delay_us(300);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_LDO_TEST_START, 0x00);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       return ldo monitor irq
; arg:
;       void
; return:
;       Uint32
;       0 - ldo monitor works normally
;       1 - ldo monitor works abnormally
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint32 fusa_radio_return_ldo_monitor_irq(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint32 irq_value = FUSA_RADIO_READ_BANK_REG_FIELD(10, ITF_IRQ_1, ITF_IRQ_SUPPLY_LDO);
    fusa_radio_switch_bank(old_bank);
    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm901: ldo monitor fault injection for power on self test
; arg:
;       void
; return:
;       uint32
;       1 - pass
;       0 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_ldo_monitor_fault_injection(void)
{
    uint32 irq_value, reg_calc;

    /* enable auxadc1 */
    fusa_radio_auxadc1_voltage_on();
    /* set ldo monitor timing and voltage threshold */
    fusa_radio_set_ldo_timing_th();
    fusa_radio_set_threshold_for_ldo_Vth();
    /* calculate threshold reg value with nominal voltage 2.5V and threshold 30% */
    reg_calc = fusa_radio_sm_threshold_reg_calc(2.5, 30);
    /* set low threshold as 2.5V + 30% */
    fusa_radio_sm_ldo_monitor_set_threshold(SM1_THRES_2V5, SM1_THRES_RANGE_L, reg_calc);
    /* set high threshold as 2.5V + 30% */
    fusa_radio_sm_ldo_monitor_set_threshold(SM1_THRES_2V5, SM1_THRES_RANGE_H, reg_calc);
    /* enable ldo monitor ldo25 fmcwpll */
    fusa_radio_sm_ldo_monitor_on(SM1_LDO25_FMCWPLL, true);
    fusa_radio_reset_ldo_monitor(true);
    /* clear irq of ldo monitor */
    fusa_radio_clear_ldo_monitor();
    /* start irq of ldo monitor */
    fusa_radio_ldo_monitor_test_on();
    fusa_delay_ms(4);
    /* read irq of ldo monitor */
    irq_value = fusa_radio_return_ldo_monitor_irq();
    /* clear irq status for monitoring */
    fusa_radio_clear_ldo_monitor();
    /* enable ldo monitor ldo25 fmcwpll */
    fusa_radio_sm_ldo_monitor_on(SM1_LDO25_FMCWPLL, false);
    /* disable ldo monitor */
    fusa_radio_reset_ldo_monitor(false);
    /* disable auxadc1 */
    fusa_radio_auxadc1_voltage_off();
    return irq_value;
}

/**************************************************************
;
; This routine:
;       set ldo monitor
; arg:
;       current_part: [uint8] 0/1/2/3...
; return:
;       float32 :current part need time(ms)
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
float32 fusa_radio_sm_ldo_monitor_setting(uint8 part_num)
{
    fusa_config_sm1_t *p = &ldo_monitor_cfg;
    uint64 cur_part = p->part[part_num];
    // log_fusa("part = %d  cfg = 0x%x %x\r\n",part_num, (uint32)(cur_part >> 32), (uint32)(cur_part));
    uint32 reg_calc = 0;
    float32 monitor_ldo_number = 0;
    float32 thres_voltage[5] = {0.0};
    uint32 i = 0;
    /* enable auxadc1 */
    fusa_radio_auxadc1_voltage_on();
    /* set ldo monitor timing and voltage threshold */
    fusa_radio_set_ldo_timing_th();
    fusa_radio_set_threshold_for_ldo_Vth();
    for (i = 0; i < FUSA_RADIO_LDO_NUMBER; i++) {
        if((cur_part >> i) & 0x01)
        {
            monitor_ldo_number += 1;
            fusa_radio_sm_ldo_monitor_get_ldo_status((fusa_sm1_item_e)i);
        }
    }
    /* get avrage voltage of each range of current part  */
    fusa_radio_sm_ldo_monitor_v_mid(cur_part, thres_voltage);
    /* set thres_level */
    for (i = 0; i < 5; i++) {
        /* calculate threshold reg value with nominal voltage thres_voltage and thres_low */
        reg_calc = fusa_radio_sm_threshold_reg_calc(thres_voltage[i], p->thr[i].thr_l);
        /* set low threshold as thres_voltage + thres_low */
        fusa_radio_sm_ldo_monitor_set_threshold((fusa_sm1_thres_level_e)i, SM1_THRES_RANGE_L, reg_calc);
        /* calculate threshold reg value with nominal voltage thres_voltage and thres_high */
        reg_calc = fusa_radio_sm_threshold_reg_calc(thres_voltage[i], p->thr[i].thr_h);
        /* set high threshold as thres_voltage + thres_high */
        fusa_radio_sm_ldo_monitor_set_threshold((fusa_sm1_thres_level_e)i, SM1_THRES_RANGE_H, reg_calc);
    }
    fusa_radio_reset_ldo_monitor(true);
    /* clear irq of ldo monitor */
    fusa_radio_clear_ldo_monitor();
    /* start irq of ldo monitor */
    fusa_radio_sm_ldo_monitor_en(cur_part);
    fusa_radio_ldo_monitor_test_on();
    return monitor_ldo_number * 1.2 +0.5;
}

/************************************************************************************
;
; This routine:
;       get sm1 ldo monitor irq
; arg:
;       current_part: [uint8] 0/1/2/3
; return:
;       uint32
;       0 - pass
;       1 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_ldo_monitor_irq(uint8 current_part)
{
    uint32 irq_value;
    fusa_sm1_item_e ldo_item_part_array[8] = {0};
    fusa_radio_sm_ldo_monitor_set_part(current_part, ldo_item_part_array);
    /* read irq of ldo monitor */
    irq_value = fusa_radio_return_ldo_monitor_irq();
    /* clear irq status for monitoring */
    fusa_radio_clear_ldo_monitor();
    /* disable current part ldo monitor*/
    for (uint8 i = 0; i < 8; i++) {
        fusa_radio_sm_ldo_monitor_on(ldo_item_part_array[i], false);
        fusa_radio_sm_ldo_monitor_return_ldo_status(ldo_item_part_array[i]);
    }
    /* disable ldo monitor */
    fusa_radio_reset_ldo_monitor(false);
    /* disable auxadc1 */
    fusa_radio_auxadc1_voltage_off();
    return irq_value;
}
//uint8 cur_part number
//return irq status
uint32 fusa_radio_sm_ldo_monitor_resume(uint8 part_num)
{
    uint32 irq_value;
    uint64 cur_part_cfg = ldo_monitor_cfg.part[part_num];
    /* read irq of ldo monitor */
    irq_value = fusa_radio_return_ldo_monitor_irq();
    /* clear irq status for monitoring */
    fusa_radio_clear_ldo_monitor();
    /* disable current part ldo monitor*/
    for (uint8 i = 0; i < FUSA_RADIO_LDO_NUMBER ; i++) {
        if( (cur_part_cfg >> i) & 0x01)
        {
            fusa_radio_sm_ldo_monitor_return_ldo_status((fusa_sm1_item_e)i);
        }
    }
    /* disable ldo monitor */
    fusa_radio_sm_ldo_monitor_en(0);
    fusa_radio_reset_ldo_monitor(false);
    /* disable auxadc1 */
    fusa_radio_auxadc1_voltage_off();
    return irq_value;
}
/**************************************************************
;
; This routine:
;       enable avdd33 detector
; arg:
;       enable: [bool] true/false
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_voltage_detector_3v3_on(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, TEST_CBC2, CBC33_MONITOR_RDAC_EN, enable);
    FUSA_RADIO_MOD_BANK_REG(0, TEST_CBC2, CBC33_MONITOR_EN, enable);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       set avdd33 voltage detector threshold
; arg:
;       avdd33_th0: [uint8] avdd33_monitor0 threshold
;       avdd33_th1: [uint8] avdd33_monitor1 threshold
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_voltage_3v3_th(uint8 avdd33_th0, uint8 avdd33_th1)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_WRITE_BANK_REG(0, CBC33_MONITOR0, avdd33_th0);
    FUSA_RADIO_WRITE_BANK_REG(0, CBC33_MONITOR1, avdd33_th1);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       calculate avdd33 voltage detector threshold to reg val
; arg:
;       thr: [float] avdd33_monitor threshold
; return:
;       uint8 : reg val
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_3v3_th_calc_reg(float32 thr)
{
    uint8 reg_val = 0;
    float32 v = 3.3 * (1 + thr/100);
    if (v >= 3.875)
    {
        reg_val = 0x0f;
    }
    else if(v <= 2.6275)
    {
        reg_val = 0x00;
    }
    else
    {
        reg_val = (v - 2.6275) / ((3.875 - 2.6275) / 16);
    }
    return reg_val;
}
/**************************************************************
;
; This routine:
;       avdd33 voltage detector threshold set
; arg:
;       thr_h: [float] avdd33_monitor high threshold -15 ~ +15 (±15%)
        thr_l: [float] avdd33_monitor low threshold  -15 ~ +15 (±15%)
; return:
;       NA
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_3v3_th_set(float32 thr_h,float32 thr_l)
{
    uint8 reg_val0 = 0x00;
    uint8 reg_val1 = 0xf0;

    uint8 tem = fusa_radio_3v3_th_calc_reg(thr_h);
    (tem > 0xe) ? (tem = 0xe) : tem;
    (tem < 0x1) ? (tem = 0x1) : tem;
    reg_val1 |= tem;

    tem = fusa_radio_3v3_th_calc_reg(thr_l);
    (tem > 0xe) ? (tem = 0xe) : tem;
    (tem < 0x1) ? (tem = 0x1) : tem;
    reg_val0 |= tem << 4;

    fusa_radio_set_voltage_3v3_th(reg_val0, reg_val1);
}
/**************************************************************
;
; This routine:
;       avdd33 voltage detector threshold get
; arg:
;       *thr_h: [float] avdd33_monitor high threshold -15 ~ +15 (±15%)
        *thr_l: [float] avdd33_monitor low threshold  -15 ~ +15 (±15%)
; return:
;       NA
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_3v3_th_get(float32 *thr_h,float32 *thr_l)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    uint8 reg_val0 = FUSA_RADIO_READ_BANK_REG(0, CBC33_MONITOR0);
    uint8 reg_val1 = FUSA_RADIO_READ_BANK_REG(0, CBC33_MONITOR1);
    fusa_radio_switch_bank(old_bank);
    float32 v_h = (reg_val1 & 0x0f) * (3.875 - 2.6275) / 16 + 2.6275;
    float32 v_l = ((reg_val0 >> 4) & 0x0f) * (3.875 - 2.6275) / 16 + 2.6275;

    *thr_h = 100 * (v_h - 3.3) / 3.3;
    *thr_l = 100 * (v_l - 3.3) / 3.3;
    // log_fusa("thr_h=%f  thr_l=%f\r\n", *thr_h, *thr_l);
}
/**************************************************************
;
; This routine:
;       clear avdd33 voltage detector
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_clear_voltage_detector_3v3(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_CBC33_CLEAR, 0x0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_CBC33_CLEAR, 0x1);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_CBC33_CLEAR, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       start avdd33 detector
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_start_voltage_detector_3v3(void)
{
    uint8_t old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_CBC33_TEST_START, 0x0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_CBC33_TEST_START, 0x1);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_CBC33_TEST_START, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       return irq of avdd33 detector
; arg:
;       void
; return:
;       uint32
;       0 - avdd33 works normally
;       other - avdd33 works abnormally
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint32 fusa_radio_return_voltage_detector_3v3_irq(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint8 irq_value_l, irq_value_h;
    uint32 irq_value;

    irq_value_h = FUSA_RADIO_READ_BANK_REG_FIELD(10, ITF_IRQ_1, ITF_IRQ_SUPPLY_CBC33);
    fusa_radio_switch_bank(0);
    irq_value_l = FUSA_RADIO_READ_BANK_REG(0, MS) & 0x03;
    irq_value = (irq_value_h << 2) + irq_value_l;
    fusa_radio_switch_bank(old_bank);

    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm902:avdd33 monitor fault injection for power on self test
; arg:
;       radio
; return:
;       uint32
;       7 - pass
;       0, 4, 5, 6 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_avdd33_monitor_fault_injection(void)
{
    uint32 irq_value;

    /* set avdd33 voltage detector threshold as 0 */
    fusa_radio_set_voltage_3v3_th(0, 0);
    /* enable avdd33 safety monitor */
    fusa_radio_voltage_detector_3v3_on(true);
    /* delay 100us */
    fusa_delay_us(100);
    /* clear irq of avdd33 voltage detector */
    fusa_radio_clear_voltage_detector_3v3();
    /* start irq of avdd33 voltage detector */
    fusa_radio_start_voltage_detector_3v3();
    /* read irq of avdd33 voltage detector */
    irq_value = fusa_radio_return_voltage_detector_3v3_irq();
    /* set avdd33 threshold for monitoring */
    fusa_radio_set_voltage_3v3_th(0x54, 0xcb);
    /* clear IRQ of avdd33 voltage detector */
    fusa_radio_clear_voltage_detector_3v3();

    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm2:avdd33 monitor which is a continuous run item
; arg:
;       radio
; return:
;       uint32
;       0 - pass
;       4, 5, 6, 7 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_avdd33_monitor_irq(void)
{
    uint32 irq_value;

    /* set avdd33 voltage detector threshold as 0 */
    // fusa_radio_set_voltage_3v3_th(0x54, 0xcb);
    /* enable avdd33 safety monitor */
    fusa_radio_voltage_detector_3v3_on(true);
    /* delay 100us */
    fusa_delay_us(100);
    /* clear irq of avdd33 voltage detector */
    fusa_radio_clear_voltage_detector_3v3();
    /* start irq of avdd33 voltage detector */
    fusa_radio_start_voltage_detector_3v3();
    /* read irq of avdd33 voltage detector */
    irq_value = fusa_radio_return_voltage_detector_3v3_irq();

    return irq_value;
}

/**************************************************************
;
; This routine:
;       enable dvdd11 detector
; arg:
;       enable: [bool] true/false
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_voltage_detector_1v1_on(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, TEST_CBC2, DVDD11_MONITOR_EN, enable);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       set dvdd11 voltage detector threshold
; arg:
;       dvdd11_th_high: [uint8] dvdd11_monitor high threshold
;       dvdd11_th_low:  [uint8] dvdd11_monitor low threshold
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_voltage_1v1_th(uint8 dvdd11_th_high, uint8 dvdd11_th_low)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, DVDD11_MONITOR, DVDD11_MONITOR_VHSEL, dvdd11_th_high);
    FUSA_RADIO_MOD_BANK_REG(0, DVDD11_MONITOR, DVDD11_MONITOR_VLSEL, dvdd11_th_low);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       get dvdd11 voltage detector threshold
; arg:
;       dvdd11_th_high: [uint8] dvdd11_monitor high threshold
;       dvdd11_th_low:  [uint8] dvdd11_monitor low threshold
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_get_voltage_1v1_th(uint8 *dvdd11_th_high, uint8 *dvdd11_th_low)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    *dvdd11_th_high = FUSA_RADIO_READ_BANK_REG_FIELD(0, DVDD11_MONITOR, DVDD11_MONITOR_VHSEL);
    *dvdd11_th_low = FUSA_RADIO_READ_BANK_REG_FIELD(0, DVDD11_MONITOR, DVDD11_MONITOR_VLSEL);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       clear dvdd11 voltage detector
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_clear_voltage_detector_1v1(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_DVDD11_CLEAR, 0x0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_DVDD11_CLEAR, 0x1);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_DVDD11_CLEAR, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       start dvdd11 detector
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_start_voltage_detector_1v1(void)
{
    uint8_t old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_DVDD11_TEST_START, 0x0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_DVDD11_TEST_START, 0x1);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_SUPPLY_DVDD11_TEST_START, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       return irq of dvdd11 detector
; arg:
;       void
; return:
;       uint32
;       0 - dvdd11 works normally
;       1 - dvdd11 works abnormally
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint32 fusa_radio_return_voltage_detector_1v1_irq(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint32 irq_value;
    irq_value = FUSA_RADIO_READ_BANK_REG_FIELD(10, ITF_IRQ_1, ITF_IRQ_SUPPLY_DVDD11);
    fusa_radio_switch_bank(old_bank);
    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm903: dvdd11 monitor fault injection for power on self test
; arg:
;       radio
; return:
;       uint32
;       1 - pass
;       0 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_dvdd11_monitor_fault_injection(void)
{
    uint32 irq_value;

    /* set dvdd11 voltage detector threshold as 0 */
    fusa_radio_set_voltage_1v1_th(0x0, 0x0);
    /* enable dvdd11 safety monitor */
    fusa_radio_voltage_detector_1v1_on(true);
    /* delay 100us */
    fusa_delay_us(100);
    /* clear irq of dvdd11 voltage detector */
    fusa_radio_clear_voltage_detector_1v1();
    /* start irq of dvdd11 voltage detector */
    fusa_radio_start_voltage_detector_1v1();
    /* read irq of dvdd11 voltage detector */
    irq_value = fusa_radio_return_voltage_detector_1v1_irq();
    /* set dvdd11 threshold for monitoring */
    fusa_radio_set_voltage_1v1_th(0x1, 0x1);
    /* clear IRQ of dvdd11 voltage detector */
    fusa_radio_clear_voltage_detector_1v1();

    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm3: dvdd11 monitor which is a continuous run item
; arg:
;       radio
; return:
;       uint32
;       0 - pass
;       1 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_dvdd11_monitor_irq(void)
{
    uint32 irq_value;

    /* set dvdd11 voltage detector threshold as 0 */
    fusa_radio_set_voltage_1v1_th(0x1, 0x1);
    /* enable dvdd11 safety monitor */
    fusa_radio_voltage_detector_1v1_on(true);
    /* delay 100us */
    fusa_delay_us(100);
    /* clear irq of dvdd11 voltage detector */
    fusa_radio_clear_voltage_detector_1v1();
    /* start irq of dvdd11 voltage detector */
    fusa_radio_start_voltage_detector_1v1();
    /* read irq of dvdd11 voltage detector */
    irq_value = fusa_radio_return_voltage_detector_1v1_irq();

    return irq_value;
}

/**************************************************************
;
; This routine:
;       enable bandgap voltage monitor
; arg:
;       enable: [bool] true/false
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_vbg_monitor_on(boolean enable)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, TEST_CBC2, VBG_MONITOR_EN, enable);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       set bandgap voltage monitor threshold
; arg:
;       vbg_th_high: [uint8] vbg_monitor high threshold
;       vbg_th_low:  [uint8] vbg_monitor low threshold
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fmcw_radio_set_vbg_monitor_vth(uint8 vbg_th_high, uint8 vbg_th_low)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, VBG_MONITOR, VBG_MONITOR_VHSEL, vbg_th_high);
    FUSA_RADIO_MOD_BANK_REG(0, VBG_MONITOR, VBG_MONITOR_VLSEL, vbg_th_low);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get bandgap voltage monitor threshold
; arg:
;       vbg_th_high: [uint8] vbg_monitor high threshold
;       vbg_th_low:  [uint8] vbg_monitor low threshold
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fmcw_radio_get_vbg_monitor_vth(uint8 *vbg_th_high, uint8 *vbg_th_low)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    *vbg_th_high = FUSA_RADIO_READ_BANK_REG_FIELD(0, VBG_MONITOR, VBG_MONITOR_VHSEL);
    *vbg_th_low = FUSA_RADIO_READ_BANK_REG_FIELD(0, VBG_MONITOR, VBG_MONITOR_VLSEL);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       clear bandgap voltage monitor
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_clear_vbg_monitor(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_VBG_CLEAR, 0x0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_VBG_CLEAR, 0x1);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_VBG_CLEAR, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       start bandgap voltage monitor
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_start_vbg_monitor(void)
{
    uint8_t old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_VBG_TEST_START, 0x0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_VBG_TEST_START, 0x1);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_1, ITF_VBG_TEST_START, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       return irq of bandgap voltage monitor
; arg:
;       void
; return:
;       uint32
;       0 - vbg works normally
;       1 - vbg works abnormally
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint32 fusa_radio_return_vbg_monitor_irq(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint32 irq_value;
    irq_value = FUSA_RADIO_READ_BANK_REG_FIELD(10, ITF_IRQ_1, ITF_IRQ_VBG);
    fusa_radio_switch_bank(old_bank);
    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm904: vbg monitor fault injection for power on self test
; arg:
;       radio
; return:
;       uint32
;       1 - pass
;       0 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_vbg_monitor_fault_injection(void)
{
    uint32 irq_value;

    /* set vbg monitor threshold as 0 */
    fmcw_radio_set_vbg_monitor_vth(0x0, 0x0);
    /* enable vbg safety monitor */
    fusa_radio_vbg_monitor_on(true);
    /* delay 100us */
    fusa_delay_us(100);
    /* clear irq of vbg monitor */
    fusa_radio_clear_vbg_monitor();
    /* start irq of vbg monitor */
    fusa_radio_start_vbg_monitor();
    /* read irq of vbg voltage detector */
    irq_value = fusa_radio_return_vbg_monitor_irq();
    /* set vbg threshold for monitoring */
    fmcw_radio_set_vbg_monitor_vth(0x1, 0x1);
    /* clear IRQ of vbg monitor */
    fusa_radio_clear_voltage_detector_1v1();

    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm4: vbg monitor which is a continuous run item
; arg:
;       radio
; return:
;       uint32
;       0 - pass
;       1 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_vbg_monitor_irq(void)
{
    uint32 irq_value;

    /* set vbg monitor threshold as 0 */
    fmcw_radio_set_vbg_monitor_vth(0x1, 0x1);
    /* enable vbg safety monitor */
    fusa_radio_vbg_monitor_on(true);
    /* delay 100us */
    fusa_delay_us(100);
    /* clear irq of vbg monitor */
    fusa_radio_clear_vbg_monitor();
    /* start irq of vbg monitor */
    fusa_radio_start_vbg_monitor();
    /* read irq of vbg monitor */
    irq_value = fusa_radio_return_vbg_monitor_irq();

    return irq_value;
}

/********************************************************************
;
; This routine:
;       choose test_point_mux2 to channel 0/1/2/3
; arg:
;       ch_index: [uint8] 0/1/2/3
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;********************************************************************/
void fusa_radio_set_tpana_to_test_tx(uint8 ch_index)
{
    uint8 old_bank = fusa_radio_switch_bank(0);

    switch (ch_index) {
    case 0:
        FUSA_RADIO_MOD_BANK_REG(0, TPANA2, TEST_MUX_2_SEL, 0x28);
        break;
    case 1:
        FUSA_RADIO_MOD_BANK_REG(0, TPANA2, TEST_MUX_2_SEL, 0x29);
        break;
    case 2:
        FUSA_RADIO_MOD_BANK_REG(0, TPANA2, TEST_MUX_2_SEL, 0x2A);
        break;
    case 3:
        FUSA_RADIO_MOD_BANK_REG(0, TPANA2, TEST_MUX_2_SEL, 0x2B);
        break;
    default:
        break;
    }

    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       set pdt to calon
; arg:
;       ch_index: [uint8] 0/1/2/3
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;********************************************************************/
void fusa_radio_set_tx_power_detector_for_Vcalon(uint8 ch_index)
{
    uint8 old_bank = fusa_radio_switch_bank(1);

    switch (ch_index) {
    case 0:
        FUSA_RADIO_WRITE_BANK_REG(1, CH0_TX_TUNE2, 0x6c);
        break;
    case 1:
        FUSA_RADIO_WRITE_BANK_REG(1, CH1_TX_TUNE2, 0x6c);
        break;
    case 2:
        FUSA_RADIO_WRITE_BANK_REG(1, CH2_TX_TUNE2, 0x6c);
        break;
    case 3:
        FUSA_RADIO_WRITE_BANK_REG(1, CH3_TX_TUNE2, 0x6c);
        break;
    default:
        break;
    }

    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       set pdt to caloff
; arg:
;       ch_index: [uint8] 0/1/2/3
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;********************************************************************/
void fusa_radio_set_tx_power_detector_for_Vcaloff(uint8 ch_index)
{
    uint8 old_bank = fusa_radio_switch_bank(1);

    switch (ch_index) {
    case 0:
        FUSA_RADIO_WRITE_BANK_REG(1, CH0_TX_TUNE2, 0x2c);
        break;
    case 1:
        FUSA_RADIO_WRITE_BANK_REG(1, CH1_TX_TUNE2, 0x2c);
        break;
    case 2:
        FUSA_RADIO_WRITE_BANK_REG(1, CH2_TX_TUNE2, 0x2c);
        break;
    case 3:
        FUSA_RADIO_WRITE_BANK_REG(1, CH3_TX_TUNE2, 0x2c);
        break;
    default:
        break;
    }

    fusa_radio_switch_bank(old_bank);
}

/********************************************************************
;
; This routine:
;       set pdt to paon
; arg:
;       ch_index: [uint8] 0/1/2/3
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;********************************************************************/
void fusa_radio_set_tx_power_detector_for_Vpaon(uint8 ch_index)
{
    uint8 old_bank = fusa_radio_switch_bank(1);

    switch (ch_index) {
    case 0:
        FUSA_RADIO_WRITE_BANK_REG(1, CH0_TX_TUNE2, 0x29);
        break;
    case 1:
        FUSA_RADIO_WRITE_BANK_REG(1, CH1_TX_TUNE2, 0x29);
        break;
    case 2:
        FUSA_RADIO_WRITE_BANK_REG(1, CH2_TX_TUNE2, 0x29);
        break;
    case 3:
        FUSA_RADIO_WRITE_BANK_REG(1, CH3_TX_TUNE2, 0x29);
        break;
    default:
        break;
    }

    fusa_radio_switch_bank(old_bank);
}

/**********************************
;
; This routine:
;       get fmcwpll vco status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*********************************/
uint8 fusa_radio_get_fmcw_vco_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(0, FMCWPLL_EN, VCO_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**********************************
;
; This routine:
;       enable fmcwpll vco
; arg:
;       enable : [bool] 1-set 0-unset
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*********************************/
void fusa_radio_set_fmcw_vco(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(0);

    FUSA_RADIO_MOD_BANK_REG(0, FMCWPLL_EN, VCO_EN, enable);

    fusa_radio_switch_bank(old_bank);
}

/**********************************
;
; This routine:
;       get lo main path status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*********************************/
uint8 fusa_radio_get_lomain_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    uint8 status = FUSA_RADIO_READ_BANK_REG(0, LO_EN0);
    // log_fusa("lomain status before = %d\r\n", status);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**********************************
;
; This routine:
;       restore lo main path
; arg:
;       status: [uint8] lo main status
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*********************************/
void fusa_radio_restore_lomain_status(uint8_t status)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_WRITE_BANK_REG(0, LO_EN0, status);
    // log_fusa("lomain status after = %d\r\n", status);
    fusa_radio_switch_bank(old_bank);
}

/**********************************
;
; This routine:
;       disable lo main path
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*********************************/
void fusa_radio_lomain_off(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);

    /* disable lo main */
    FUSA_RADIO_WRITE_BANK_REG(0, LO_EN0, 0x00);

    fusa_radio_switch_bank(old_bank);
}

/**********************************
;
; This routine:
;       get txlo status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*********************************/
uint8 fusa_radio_get_txlo_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    uint8 status = FUSA_RADIO_READ_BANK_REG(0, LO_EN1);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**********************************
;
; This routine:
;       restore txlo
; arg:
;       status: [uint8] lo main status
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*********************************/
void fusa_radio_restore_txlo_status(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_WRITE_BANK_REG(0, LO_EN1, status);
    fusa_radio_switch_bank(old_bank);
}

/**********************************
;
; This routine:
;       disable txlo
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*********************************/
void fusa_radio_txlo_off(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);

    FUSA_RADIO_WRITE_BANK_REG(0, LO_EN1, 0x00);

    fusa_radio_switch_bank(old_bank);
}

/**********************************
;
; This routine:
;       set power detector registers
; arg:
;       pdt_type : [fusa_pdt_type_e] CAL_ON/CAL_OFF/PA_ON/PA_OFF
;       ch_index: [uint8] 0/1/2/3
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*********************************/
typedef enum{
    CAL_ON,
    CAL_OFF,
    PA_ON,
    PA_OFF,
}fusa_pdt_type_e;
void fusa_radio_pdt_reg(fusa_pdt_type_e pdt_type, uint8 ch_index)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    /* select TPANA mux of channel */
    fusa_radio_set_tpana_to_test_tx(ch_index);
    /* select pdt_type */
    switch (pdt_type) {
    case CAL_ON:
        fusa_radio_set_tx_power_detector_for_Vcalon(ch_index);
        break;
    case CAL_OFF:
        fusa_radio_set_tx_power_detector_for_Vcaloff(ch_index);
        break;
    case PA_ON:
        fusa_radio_set_tx_power_detector_for_Vpaon(ch_index);
        break;
    case PA_OFF:
        fusa_radio_set_tx_power_detector_for_Vpaon(ch_index);
        fusa_radio_set_fmcw_vco(false);
        fusa_radio_lomain_off();
        fusa_radio_txlo_off();
        break;
    default:
        break;
    }
    fusa_radio_switch_bank(old_bank);
}

/**********************************************************************
;
; This routine:
;       get chip part number
; arg:
;       void
; return:
;       uint8
;       part_no: 0x45-ewlb, 0x42-aip
; Change tracking:
;       Ver0.0.1 :
;*********************************************************************/
uint8 fusa_get_part_number(void)
{
        uint8 part_no = REG_L(raw_readl(OTP_PART_NO));
        return part_no;
}

/**********************************************************************
;
; This routine:
;       calculate power detector low threshold
; arg:
;       ch_index: [uint8] 0/1/2/3
;       pdt: [float] low power threshold of pdt
;       pdt_th_low: [uint8 pointer] low power threshold reg array
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*********************************************************************/
void fusa_radio_get_power_detector_low_threshold(uint8 ch_index, double pdt_th, uint8 *pdt_th_low)
{
    //TODO:sensor_config_t* cfg = (sensor_config_t*)CONTAINER_OF(radio, baseband_t, radio)->cfg;
    float Vcalon, Vcaloff, Vpaon, Vpaoff, Dconst1, cut_freq;
    uint32 dout, pa_dout;
    uint8 part_num;
    uint8 lomain_status = fusa_radio_get_lomain_status();
    uint8 vco_status = fusa_radio_get_fmcw_vco_status();
    uint8 txlo_status = fusa_radio_get_txlo_status();
    /* set cal_on reg */
    fusa_radio_pdt_reg(CAL_ON, ch_index);
    /* enable auxadc2 */
    fusa_radio_auxadc2_voltage_on();
    /* disable 5m auxadc clock */
    fusa_radio_5m_aux_adc_clk_on(false);
    /* get dout code */
    dout = fusa_radio_get_auxadc2_dout();
    /* calculate voltage */
    Vcalon = ((float)dout / (1<<17) -1) * 3.3 + 1.67;
    /* set cal_off reg */
    fusa_radio_pdt_reg(CAL_OFF, ch_index);
    /* enable auxadc2 */
    fusa_radio_auxadc2_voltage_on();
    /* disable 5m auxadc clock */
    fusa_radio_5m_aux_adc_clk_on(false);
    /* get dout code */
    dout = fusa_radio_get_auxadc2_dout();
    /* calculate voltage */
    Vcaloff = ((float)dout / (1<<17) -1) * 3.3 + 1.67;
    /* set pa off reg */
    fusa_radio_pdt_reg(PA_OFF, ch_index);
    /* enable auxadc2 */
    fusa_radio_auxadc2_voltage_on();
    /* disable 5m auxadc clock */
    fusa_radio_5m_aux_adc_clk_on(false);
    /* get dout code */
    dout = fusa_radio_get_auxadc2_dout();
    /* calculate voltage */
    Vpaoff = ((float)dout / (1<<17) -1) * 3.3 + 1.67;
    /* after pa off, we should restore pll, lo again */
    fusa_radio_restore_lomain_status(lomain_status);
    fusa_radio_set_fmcw_vco(vco_status);
    fusa_radio_restore_txlo_status(txlo_status);
    /* use formula rf_power = 12 * log10 ((pa_on - pa_off)/(cal_on - cal_off)) + offset to calculate Vpaon*/
    /* check ewlb and AIP part number, ewlb=0x45, AIP=0x42*/
    part_num = fusa_get_part_number();
    switch (part_num) {
    case 0x45:
        cut_freq = 78;
        break;
    case 0x42:
        cut_freq = 79;
        break;
    default:
        cut_freq = 78;
        //log_fusa("chip part number not distinguished\r\n");
        break;
    }
    uint32 freq_reg = 0;
    float start_freq = fusa_radio_get_start_freq(freq_reg);
    TODO:if (start_freq <= cut_freq) {
        Dconst1 = pow(10, (pdt_th - 9.5)/12);
    } else {
        Dconst1 = pow(10, (pdt_th - 7.5)/12);
    }
    Vpaon = Dconst1 * (Vcalon - Vcaloff) + Vpaoff;
    pa_dout = ((Vpaon - 1.67)/3.3 + 1)*(1<<17);
    pdt_th_low[0] = REG_L(pa_dout);
    pdt_th_low[1] = REG_M(pa_dout);
    pdt_th_low[2] = REG_H(pa_dout);
//    log_fusa("Vcalon = %f, Vcaloff = %f, Vpaoff = %f, Vpaon = %f, pa_dout = %d\r\n", Vcalon, Vcaloff, Vpaoff, Vpaon, pa_dout);
//    log_fusa("cut_freq = %f, start_freq = %f\r\n", cut_freq, start_freq);
}

/**********************************************************************
;
; This routine:
;       set customized power detector threshold for monitoring
; arg:
;       ch_index: [uint8] 0/1/2/3
;       pdt: [float] low power threshold of pdt
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*********************************************************************/
void fusa_radio_set_power_detector_threshold(uint8 ch_index, double pdt_th)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    /* set voltage low threshold */
    uint8_t pdt_th_low_reg[3] = {0};
    fusa_radio_get_power_detector_low_threshold(ch_index, pdt_th, pdt_th_low_reg);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_LOW_0, pdt_th_low_reg[0]);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_LOW_1, pdt_th_low_reg[1]);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_LOW_2, pdt_th_low_reg[2]);
    /* set voltage high threshold */
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_HIGH_0, 0xFF);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_HIGH_1, 0xFF);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_HIGH_2, 0x3);

    fusa_radio_switch_bank(old_bank);
}

/***************************************************************
;
; This routine:
;       set power detector threshold for fault injection
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_power_detector_threshold_fault_injection(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    /* set voltage low threshold to max voltage for fault injection */
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_LOW_2, 0x03);
    /* set voltage high threshold */
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_HIGH_0, 0xFF);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_HIGH_1, 0xFF);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TXVTH_HIGH_2, 0x03);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       set power detector timing threshold
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_power_detector_timing_th(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_TIME_CNT_1, 0x3F);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_CTU_SIZE, 0x02);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       enable power detector
; arg:
;       ch_index: [sint8] -1(all channels)/0(ch0)/1(ch1)/2(ch2)/3(ch3)
;        enable: [bool] true/false
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_power_detector(sint8 ch_index, bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_WRITE_BANK_REG(10, RFPOWER_MONITOR_RSTN, enable);
    switch (ch_index) {
    case -1:
        FUSA_RADIO_MOD_BANK_REG(10, RFPOWER_MONITOR_EN_1, TX0_EN, enable);
        FUSA_RADIO_MOD_BANK_REG(10, RFPOWER_MONITOR_EN_0, TX1_EN, enable);
        FUSA_RADIO_MOD_BANK_REG(10, RFPOWER_MONITOR_EN_0, TX2_EN, enable);
        FUSA_RADIO_MOD_BANK_REG(10, RFPOWER_MONITOR_EN_0, TX3_EN, enable);
        break;
    case 0:
        FUSA_RADIO_MOD_BANK_REG(10, RFPOWER_MONITOR_EN_1, TX0_EN, enable);
        break;
    case 1:
        FUSA_RADIO_MOD_BANK_REG(10, RFPOWER_MONITOR_EN_0, TX1_EN, enable);
        break;
    case 2:
        FUSA_RADIO_MOD_BANK_REG(10, RFPOWER_MONITOR_EN_0, TX2_EN, enable);
        break;
    case 3:
        FUSA_RADIO_MOD_BANK_REG(10, RFPOWER_MONITOR_EN_0, TX3_EN, enable);
        break;
    default:
        break;
    }
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       clear power detector
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_clear_power_detector(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_2, ITF_RFPOWER_CLEAR, 0x0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_2, ITF_RFPOWER_CLEAR, 0x1);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_2, ITF_RFPOWER_CLEAR, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       start power detector
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_start_power_detector(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_2, ITF_RFPOWER_TEST_START, 0x0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_2, ITF_RFPOWER_TEST_START, 0x1);
    fusa_delay_ms(1);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_2, ITF_RFPOWER_TEST_START, 0x0);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       return power detector IRQ
; arg:
;       void
; return:
;       uint8
;       0 - power detector works normally
;       1 - power detector works abnormally
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_return_power_detector_irq(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint8 irq_value;
    irq_value = FUSA_RADIO_READ_BANK_REG_FIELD(10, ITF_IRQ_2, ITF_IRQ_RFPOWER);
    fusa_radio_switch_bank(old_bank);
    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm6:power detector
; arg:
;       ch: [uint32] bit3~bit0 refer to tx3~tx0, 1-ch on, 0-ch off
; return:
;       int
;       0 - pass
;       1 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_power_detector_irq(uint32 ch)
{
//   uint32 dout = 0; // this can be deleted
    uint32 irq_value = 0;
    uint8 tx_status[4] = {0};
    uint8 ch_index[4] = {0};
//   float Vpaon; // this can be deleted
    /* save tx ldo and bias status */
    fusa_radio_get_tx_status(tx_status);
    /* disable all tx bias */
    fusa_radio_set_tx_ch_status(-1, false);
    /* enable all tx ldo */
    fusa_radio_tx_ldo_on();
    /* get detecting tx */
    ch_index[3] = (ch & 0x8) >> 3;
    ch_index[2] = (ch & 0x4) >> 2;
    ch_index[1] = (ch & 0x2) >> 1;
    ch_index[0] = ch & 0x1;

    /* detector tx_on channels */
    for (int i = 0; i < 4; i++) {
        if (ch_index[i] == 1) {
            /* enable ch[i] tx bias */
            fusa_radio_set_tx_ch_status(i, true);
            /* enable power detector */
            fusa_radio_set_power_detector(i, true);
//            log_fusa("CH%d ", i);
            /* select pdt_type as pa on */
            fusa_radio_pdt_reg(PA_ON, i);
            /* enable auxadc2 */
            fusa_radio_auxadc2_voltage_on();
            /* below can be commented out from here */
//           /* disable 5m auxadc clock */
//           fusa_radio_5m_aux_adc_clk_on(false);
//           /* get dout code */
//           dout = fusa_radio_get_auxadc2_dout();
//           /* calculate voltage */
//           Vpaon = ((float)dout / (1<<17) -1) * 3.3 + 1.67;
//           log_fusa("pa on voltage = %f\r\n", Vpaon);
           /* commented out end here */
           /* enable adc 5m clock */
           fusa_radio_5m_aux_adc_clk_on(true);
            /* clear IRQ of power detector */
            fusa_radio_clear_power_detector();
            /* start IRQ of power detector */
            fusa_radio_start_power_detector();
            /* read IRQ of power detector */
            irq_value = irq_value || fusa_radio_return_power_detector_irq();
//            log_fusa("irq = %d\r\n", irq_value);
            /* clear irq of power detector */
            fusa_radio_clear_power_detector();
            /* disable power detector */
            fusa_radio_set_power_detector(i, false);
            /* disable ch[i] tx bias */
            fusa_radio_set_tx_ch_status(i, false);
        } else {
            continue;
        }
    }
    /* disable auxadc2 */
    fusa_radio_auxadc2_voltage_off();
    /* disable power detector */
    fusa_radio_set_power_detector(-1, false);
    /* restore tx ldo and bias status */
    fusa_radio_set_tx_status(tx_status);

    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm905:power detector fault injection for power on self test
; arg:
;       pdt_th: [double] threshold of power detector, if power is below pdt_th,
;       sm6 raises irq
;       ch: [uint32] bit3~bit0 refer to tx3~tx0, 1-ch on, 0-ch off
; return:
;       int
;       0 - fail
;       1 - pass
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_power_detector_fault_injection(double pdt_th, uint32 ch)
{
    sint8 ch_index[4] = {0};
    uint32 irq_value = 1;
    uint8 tx_status[4] = {0};
    uint32 dout; //this can be deleted
    float Vpaon;
    /* set power detector voltage threshold for fault injection */
    fusa_radio_set_power_detector_threshold_fault_injection();
    /* set power detector timing threshold */
    fusa_radio_set_power_detector_timing_th();
    /* save tx ldo and bias status */
    fusa_radio_get_tx_status(tx_status);
    /* get detecting tx */
    ch_index[0] = (ch & 0x8) >> 3;
    ch_index[1] = (ch & 0x4) >> 2;
    ch_index[2] = (ch & 0x2) >> 1;
    ch_index[3] = ch & 0x1;
    /* disable all tx bias */
    fusa_radio_set_tx_ch_status(-1, false);
    /* enable all tx ldo */
    fusa_radio_tx_ldo_on();
    /* detector tx_on channels */
    for (int i = 0; i < 4; i++) {
        if (ch_index[i] == 1) {
            /* enable ch[i] tx bias */
            fusa_radio_set_tx_ch_status(i, true);
            /* enable power detector */
            fusa_radio_set_power_detector(i, true);
            //log_fusa("CH%d\r\n", i);
            /* select pdt_type as pa on */
            fusa_radio_pdt_reg(PA_ON, i);
            /* enable auxadc2 */
            fusa_radio_auxadc2_voltage_on();
            /* below can be commented out from here */
            /* disable 5m auxadc clock */
            fusa_radio_5m_aux_adc_clk_on(false);
            /* get dout code */
            dout = fusa_radio_get_auxadc2_dout();
            /* calculate voltage */
            Vpaon = ((float)dout / (1<<17) -1) * 3.3 + 1.67;
//            log_fusa("pa on voltage = %f\r\n", Vpaon);
            /* enable adc 5m clock */
            fusa_radio_5m_aux_adc_clk_on(true);
            /* commented out end here */
            /* clear IRQ of power detector */
            fusa_radio_clear_power_detector();
            /* start IRQ of power detector */
            fusa_radio_start_power_detector();
            /* read IRQ of power detector */
            irq_value = irq_value && fusa_radio_return_power_detector_irq();
            /* clear irq of power detector */
            fusa_radio_clear_power_detector();
            /* disable power detector */
            fusa_radio_set_power_detector(i, false);
            /* disable ch[i] tx bias */
            fusa_radio_set_tx_ch_status(i, false);
        } else {
            continue;
        }
    }

    /* set power detector voltage threshold for monitoring */
    for (int i = 0; i < 4; i++) {
        if (ch_index[i] == 1) {
            /* enable ch[i] tx bias */
            fusa_radio_set_tx_ch_status(i, true);
            /* set threshold */
            fusa_radio_set_power_detector_threshold(i, pdt_th);
            /* disable ch[i] tx bias */
            fusa_radio_set_tx_ch_status(i, false);
            break;
        }
    }
    /* disable auxadc2 */
    fusa_radio_auxadc2_voltage_off();
    /* disable power detector */
    fusa_radio_set_power_detector(-1, false);
    /* restore tx ldo and bias status */
    fusa_radio_set_tx_status(tx_status);

    return irq_value;
}

/*************************************************************************************
;
; This routine:
;       save rxbb gain
; arg:
;       rxbb_para: [uint8* array] store rxbb gain parameters
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;************************************************************************************/
void fusa_radio_save_rx_gain(uint8 (*rxbb_para)[3])
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    /* get certain radio register value */
    uint8 ch0_tia_gain  = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE0, TIA_RFB_SEL);
    uint8 ch0_vga1_gain = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE1, VGA1_GAINSEL);
    uint8 ch0_vga2_gain = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE1, VGA2_GAINSEL);
    uint8 ch1_tia_gain  = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE0, TIA_RFB_SEL);
    uint8 ch1_vga1_gain = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE1, VGA1_GAINSEL);
    uint8 ch1_vga2_gain = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE1, VGA2_GAINSEL);
    uint8 ch2_tia_gain  = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE0, TIA_RFB_SEL);
    uint8 ch2_vga1_gain = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE1, VGA1_GAINSEL);
    uint8 ch2_vga2_gain = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE1, VGA2_GAINSEL);
    uint8 ch3_tia_gain  = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE0, TIA_RFB_SEL);
    uint8 ch3_vga1_gain = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE1, VGA1_GAINSEL);
    uint8 ch3_vga2_gain = FUSA_RADIO_READ_BANK_REG_FIELD(0, CH0_RX_TUNE1, VGA2_GAINSEL);
    /* save integer and float type parameters to arrays */
    uint8_t rxbb_para_name[4][3] = {{ch0_tia_gain, ch0_vga1_gain, ch0_vga2_gain}, \
                                    {ch1_tia_gain, ch1_vga1_gain, ch1_vga2_gain}, \
                                    {ch2_tia_gain, ch2_vga1_gain, ch2_vga2_gain}, \
                                    {ch3_tia_gain, ch3_vga1_gain, ch3_vga2_gain}};
    /* put parameters in array to pointer */
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            rxbb_para[i][j] = rxbb_para_name[i][j];
        }
    }
    fusa_radio_switch_bank(old_bank);
}

/*************************************************************************************
;
; This routine:
;       restore rxbb gain
; arg:
;      rxbb_para: [uint8* array] store rxbb gain parameters
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;************************************************************************************/
void fusa_radio_restore_rx_gain(uint8 (*rxbb_para)[3])
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    /* restore certain radio register value */
    uint8 ch0_tia_gain  = rxbb_para[0][0];
    uint8 ch0_vga1_gain = rxbb_para[0][1];
    uint8 ch0_vga2_gain = rxbb_para[0][2];
    uint8 ch1_tia_gain  = rxbb_para[1][0];
    uint8 ch1_vga1_gain = rxbb_para[1][1];
    uint8 ch1_vga2_gain = rxbb_para[1][2];
    uint8 ch2_tia_gain  = rxbb_para[2][0];
    uint8 ch2_vga1_gain = rxbb_para[2][1];
    uint8 ch2_vga2_gain = rxbb_para[2][2];
    uint8 ch3_tia_gain  = rxbb_para[3][0];
    uint8 ch3_vga1_gain = rxbb_para[3][1];
    uint8 ch3_vga2_gain = rxbb_para[3][2];
//    log_fusa("gain = %x,%x,%x", ch0_tia_gain, ch0_vga1_gain, ch0_vga2_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH0_RX_TUNE0, TIA_RFB_SEL,  ch0_tia_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH0_RX_TUNE1, VGA1_GAINSEL, ch0_vga1_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH0_RX_TUNE1, VGA2_GAINSEL, ch0_vga2_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH1_RX_TUNE0, TIA_RFB_SEL,  ch1_tia_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH1_RX_TUNE1, VGA1_GAINSEL, ch1_vga1_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH1_RX_TUNE1, VGA2_GAINSEL, ch1_vga2_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH2_RX_TUNE0, TIA_RFB_SEL,  ch2_tia_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH2_RX_TUNE1, VGA1_GAINSEL, ch2_vga1_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH2_RX_TUNE1, VGA2_GAINSEL, ch2_vga2_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH3_RX_TUNE0, TIA_RFB_SEL,  ch3_tia_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH3_RX_TUNE1, VGA1_GAINSEL, ch3_vga1_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH3_RX_TUNE1, VGA2_GAINSEL, ch3_vga2_gain);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       set rx gain to certain gain selection for all channels
; arg:
;       radio
;       tia_rfb   : [int] 1 - 250ohms, 2 - 500ohms
;       vga1_gain : [int] 1-6, means 7dB-22dB, 3dB/step
;       vga2_gain : [int] 1-6, means 5dB-20dB, 3dB/step
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_set_rx_gain(uint8 tia_rfb, uint8 vga1_gain, uint8 vga2_gain)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, CH0_RX_TUNE0, TIA_RFB_SEL, tia_rfb);
    FUSA_RADIO_MOD_BANK_REG(0, CH0_RX_TUNE1, VGA2_GAINSEL, vga2_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH0_RX_TUNE1, VGA1_GAINSEL, vga1_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH1_RX_TUNE0, TIA_RFB_SEL, tia_rfb);
    FUSA_RADIO_MOD_BANK_REG(0, CH1_RX_TUNE1, VGA2_GAINSEL, vga2_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH1_RX_TUNE1, VGA1_GAINSEL, vga1_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH2_RX_TUNE0, TIA_RFB_SEL, tia_rfb);
    FUSA_RADIO_MOD_BANK_REG(0, CH2_RX_TUNE1, VGA2_GAINSEL, vga2_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH2_RX_TUNE1, VGA1_GAINSEL, vga1_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH3_RX_TUNE0, TIA_RFB_SEL, tia_rfb);
    FUSA_RADIO_MOD_BANK_REG(0, CH3_RX_TUNE1, VGA2_GAINSEL, vga2_gain);
    FUSA_RADIO_MOD_BANK_REG(0, CH3_RX_TUNE1, VGA1_GAINSEL, vga1_gain);
    fusa_radio_switch_bank(old_bank);
}
/************************************************************************************
;
; This routine:
;       set if loopback threshold
; arg:
;       thr_h     : [float32] 20 means 20dB
;       thr_l     : [float32] 16 means 16dB
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_if_loopback_thr_set(float32 thr_h, float32 thr_l)
{
    if_loopback_threshold_h = thr_h;
    if_loopback_threshold_l = thr_l;
}
/************************************************************************************
;
; This routine:
;       get if loopback threshold
; arg:
;       thr_h     : [float32 *] 20 means 20dB
;       thr_l     : [float32 *] 16 means 16dB
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_if_loopback_thr_get(float32 *thr_h, float32 *thr_l)
{
    *thr_h = if_loopback_threshold_h;
    *thr_l = if_loopback_threshold_l;
}
/************************************************************************************
;
; This routine:
;       get irq value of sm907:if loopback fault injection for power on self test
; arg:
;       void
; return:
;       uint32
;       0 - fail
;       1 - pass
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_if_loopback_fault_injection_irq(void)
{
    uint32 irq_value = 1;
    uint8 rx_gain[4][3] = {0};
    float tia_250_vga2_1_gain[4] = {0.0};
    float tia_500_vga2_3_gain[4] = {0.0};
    float rxbb_gain_diff[4] = {0.0};

    /* save rx gain */
    fusa_radio_save_rx_gain(rx_gain);
    /* set rx gain 250 1 1 */
    fusa_radio_set_rx_gain(1, 1, 1);
    /* get peak power stored in tia_250_vga2_1_gain, inner, tia in, vga2 out */
    fusa_dac_playback(true, 0, 3, tia_250_vga2_1_gain);
    /* set rx gain 500 3 3 */
    fusa_radio_set_rx_gain(2, 3, 3);
    /* get peak power stored in tia_500_vga2_3_gain, inner, tia in, vga2 out */
    fusa_dac_playback(true, 0, 3, tia_500_vga2_3_gain);
    /* calculated gain diff between tia_500_vga2_3_gain and tia_133_vga2_1_gain */
    rxbb_gain_diff[0] = tia_500_vga2_3_gain[0] - tia_250_vga2_1_gain[0];
    rxbb_gain_diff[1] = tia_500_vga2_3_gain[1] - tia_250_vga2_1_gain[1];
    rxbb_gain_diff[2] = tia_500_vga2_3_gain[2] - tia_250_vga2_1_gain[2];
    rxbb_gain_diff[3] = tia_500_vga2_3_gain[3] - tia_250_vga2_1_gain[3];
    for (int i = 0; i< 4; i++) {
        /* if gain diff between 16 to 20dB, fault injection successfully, irq = 1 */
        if ((rxbb_gain_diff[0] >= if_loopback_threshold_l) && (rxbb_gain_diff[0] <= if_loopback_threshold_h)) {
            continue;
        } else {
            irq_value = 0;
            break;
        }
    }

    /* restore rx gain */
    fusa_radio_restore_rx_gain(rx_gain);
    return irq_value;
}
/************************************************************************************
;
; This routine:
;       get irq value of sm11:if loopback
; arg:
;       void
; return:
;       uint32
;       0 - pass
;       1 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_if_loopback_irq(void)
{
    uint32 irq_value = 0;
    uint8 rx_gain[4][3] = {0};
    float tia_250_vga2_1_gain[4] = {0.0};
    float tia_500_vga2_3_gain[4] = {0.0};
    float rxbb_gain_diff[4] = {0.0};

    /* save rx gain */
    fusa_radio_save_rx_gain(rx_gain);
    /* set rx gain 250 1 1 */
    fusa_radio_set_rx_gain(1, 1, 1);
    /* get peak power stored in tia_250_vga2_1_gain, inner, tia in, vga2 out */
    fusa_dac_playback(true, 0, 3, tia_250_vga2_1_gain);
    /* set rx gain 500 3 3 */
    fusa_radio_set_rx_gain(2, 3, 3);
    /* get peak power stored in tia_500_vga2_3_gain, inner, tia in, vga2 out */
    fusa_dac_playback(true, 0, 3, tia_500_vga2_3_gain);
    /* calculated gain diff between tia_500_vga2_3_gain and tia_133_vga2_1_gain */
    rxbb_gain_diff[0] = tia_500_vga2_3_gain[0] - tia_250_vga2_1_gain[0];
    rxbb_gain_diff[1] = tia_500_vga2_3_gain[1] - tia_250_vga2_1_gain[1];
    rxbb_gain_diff[2] = tia_500_vga2_3_gain[2] - tia_250_vga2_1_gain[2];
    rxbb_gain_diff[3] = tia_500_vga2_3_gain[3] - tia_250_vga2_1_gain[3];
    for (int i = 0; i< 4; i++) {
        /* if gain diff between 16 to 20dB, fault injection successfully, irq = 1 */
        if ((rxbb_gain_diff[0] >= if_loopback_threshold_l) && (rxbb_gain_diff[0] <= if_loopback_threshold_h)) {
            continue;
        } else {
            irq_value = 1;
            break;
        }
    }

    /* restore rx gain */
    fusa_radio_restore_rx_gain(rx_gain);
    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get rxrf status of 4 channels
; arg:
;       status : [uint8 pointer] 4 channel rxrf status
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_get_rx_rf_status(uint8 *status)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    status[0] = FUSA_RADIO_READ_BANK_REG(0, CH0_RX_RF_EN);
    status[1] = FUSA_RADIO_READ_BANK_REG(0, CH1_RX_RF_EN);
    status[2] = FUSA_RADIO_READ_BANK_REG(0, CH2_RX_RF_EN);
    status[3] = FUSA_RADIO_READ_BANK_REG(0, CH3_RX_RF_EN);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       set rxrf status of 4 channels
; arg:
;       status : [uint8 pointer] 4 channel rxrf status
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_set_rx_rf(uint8 *status)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_WRITE_BANK_REG(0, CH0_RX_RF_EN, status[0]);
    FUSA_RADIO_WRITE_BANK_REG(0, CH1_RX_RF_EN, status[1]);
    FUSA_RADIO_WRITE_BANK_REG(0, CH2_RX_RF_EN, status[2]);
    FUSA_RADIO_WRITE_BANK_REG(0, CH3_RX_RF_EN, status[3]);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       enable rxrf bias
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_bias_for_rx_rf_off(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, CH0_RX_RF_EN, RXLOBUFF_BIAS_EN, 0);
    FUSA_RADIO_MOD_BANK_REG(0, CH0_RX_RF_EN, LNA2_BIAS_EN, 0);
    FUSA_RADIO_MOD_BANK_REG(0, CH1_RX_RF_EN, RXLOBUFF_BIAS_EN, 0);
    FUSA_RADIO_MOD_BANK_REG(0, CH1_RX_RF_EN, LNA2_BIAS_EN, 0);
    FUSA_RADIO_MOD_BANK_REG(0, CH2_RX_RF_EN, RXLOBUFF_BIAS_EN, 0);
    FUSA_RADIO_MOD_BANK_REG(0, CH2_RX_RF_EN, LNA2_BIAS_EN, 0);
    FUSA_RADIO_MOD_BANK_REG(0, CH3_RX_RF_EN, RXLOBUFF_BIAS_EN, 0);
    FUSA_RADIO_MOD_BANK_REG(0, CH3_RX_RF_EN, LNA2_BIAS_EN, 0);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       get ldo for rxlo status
; arg:
;       void
; return:
;       uint8: ldo for rxlo status
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint8 fusa_radio_get_ldo_for_rxlo_vout_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(0, LO_LDO3, LDO11_RXLO_VOUT_SEL);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/************************************************************************************
;
; This routine:
;       set ldo for rxlo as desired status
; arg:
;       status: [uint8] ldo for rxlo status
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_set_ldo_for_rxlo_vout(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, LO_LDO3, LDO11_RXLO_VOUT_SEL, status);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       get ldo for rxrf status
; arg:
;       status : [uint8 pointer] ldo for rxrf status
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_get_ldo_for_rx_rf_status(uint8 *status)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    status[0] = FUSA_RADIO_READ_BANK_REG_FIELD(0, RX_LDO0, LDO11_RFN_EN);
    status[1] = FUSA_RADIO_READ_BANK_REG_FIELD(0, RX_LDO1, LDO11_RFS_EN);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       set ldo for rxrf as certain status
; arg:
;       status : [uint8 pointer] ldo for rxrf status
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_set_ldo_for_rx_rf(uint8 *status)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, RX_LDO0, LDO11_RFN_EN, status[0]);
    FUSA_RADIO_MOD_BANK_REG(0, RX_LDO1, LDO11_RFS_EN, status[1]);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       set ldo for rxrf as certain status
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_ldo_for_rx_rf_off(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, RX_LDO0, LDO11_RFN_EN, 0);
    FUSA_RADIO_MOD_BANK_REG(0, RX_LDO1, LDO11_RFS_EN, 0);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       set rf loopback threshold
; arg:
;       thr     : [float32] 15 means 16dB
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_rf_loopback_thr_set(float32 thr)
{
    rf_loopback_threshold = thr;
}
/************************************************************************************
;
; This routine:
;       get rf loopback threshold
; arg:
;       thr     : [float32 *] 15 means 16dB
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_rf_loopback_thr_get(float32 *thr)
{
    *thr = rf_loopback_threshold;
}
/************************************************************************************
;
; This routine:
;       get irq value of sm908:rf loopback fault injection for power on self test
; arg:
;       void
; return:
;       uint32
;       0 - fail
;       1 - pass
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_rf_loopback_fault_injection_irq(void)
{
    uint32 irq_value = 1;
    uint8 rx_gain[4][3] = {0};
    uint8 ldo_for_rxrf[2] = {0};
    float rf_gain_on[4] = {0.0};
    float rf_gain_off[4] = {0.0};
    float rf_gain_diff[4] = {0.0};

    /* save rx gain */
    fusa_radio_save_rx_gain(rx_gain);
    /* set rx gain 250 1 1 */
    fusa_radio_set_rx_gain(1, 1, 1);
    /* get peak power stored in rf_gain_on, outer, tia in, vga2 out */
    fusa_dac_playback(false, 0, 3, rf_gain_on);
    /* save ldo for rxrf status */
    fusa_radio_get_ldo_for_rx_rf_status(ldo_for_rxrf);
    /* disable rx rf ldo for fault injection */
    fusa_radio_ldo_for_rx_rf_off();
    /* get peak power stored in rf_gain_off, outer, tia in, vga2 out */
    fusa_dac_playback(false, 0, 3, rf_gain_off);
    /* calculated rf gain diff between rf_gain_on and rf_gain_off_gain*/
    rf_gain_diff[0] = rf_gain_on[0] - rf_gain_off[0];
    rf_gain_diff[1] = rf_gain_on[1] - rf_gain_off[1];
    rf_gain_diff[2] = rf_gain_on[2] - rf_gain_off[2];
    rf_gain_diff[3] = rf_gain_on[3] - rf_gain_off[3];
    /* if rf gain diff > 40dB, fault injection successfully, irq = 1 */
    for (int i = 0; i< 4; i++) {
        if (rf_gain_diff[0] > 30) {
            continue;
        } else {
            irq_value = 0;
            break;
        }
    }

    /* restore rx rf ldo */
    fusa_radio_set_ldo_for_rx_rf(ldo_for_rxrf);
    /* restore rx gain */
    fusa_radio_restore_rx_gain(rx_gain);

    return irq_value;
}
/************************************************************************************
;
; This routine:
;       get IRQ value of sm12:rf loopback
; arg:
;       radio
; return:
;       int
;       0 - pass
;       1 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_rf_loopback_irq(void)
{
    uint32 irq_value = 0;
    uint8 rx_gain[4][3] = {0};
    uint8 rxrf_status[4] = {0};
    uint8 ldo_for_rxlo_vout = 0;
    float rf_gain_on[4] = {0.0};
    float rf_gain_off[4] = {0.0};
    float rf_gain_diff[4] = {0.0};

    /* save rx gain */
    fusa_radio_save_rx_gain(rx_gain);
    /* set rx gain 250 1 1 */
    fusa_radio_set_rx_gain(1, 1, 1);
    /* get peak power stored in rf_gain_on, outer, tia in, vga2 out */
    fusa_dac_playback(false, 0, 3, rf_gain_on);
    /* save rx rf status */
    fusa_radio_get_rx_rf_status(rxrf_status);
    /* disable rxlobuf_bias, lna2_bias then get peak power stored in rf_gain_off, outer, tia in, vga2 out */
    fusa_radio_bias_for_rx_rf_off();
    /* save ldo for rxlo status */
    ldo_for_rxlo_vout = fusa_radio_get_ldo_for_rxlo_vout_status();
    /* disable ldo for rxlo */
    fusa_radio_set_ldo_for_rxlo_vout(0);
    fusa_dac_playback(false, 0, 3, rf_gain_off);
    /* calculated rf gain diff between rf_gain_on and rf_gain_off_gain*/
    rf_gain_diff[0] = rf_gain_on[0] - rf_gain_off[0];
    rf_gain_diff[1] = rf_gain_on[1] - rf_gain_off[1];
    rf_gain_diff[2] = rf_gain_on[2] - rf_gain_off[2];
    rf_gain_diff[3] = rf_gain_on[3] - rf_gain_off[3];
    /* if rf gain diff < 15dB, irq = 1 */
    for (int i = 0; i< 4; i++) {
        if (rf_gain_diff[0] >= rf_loopback_threshold) {
            continue;
        } else {
            irq_value = 1;
            break;
        }
    }
    /* restor ldo for rxlo */
    fusa_radio_set_ldo_for_rxlo_vout(ldo_for_rxlo_vout);
    /* restore rx rf */
    fusa_radio_set_rx_rf(rxrf_status);
    /* restore rx gain */
    fusa_radio_restore_rx_gain(rx_gain);
    return irq_value;
}

/**************************************************************
;
; This routine:
;       get ldo for fmcwpll chirp monitor status
; arg:
;       void
; return:
;       [uint8] ldo for cm status
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_fmcwpll_cm_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    uint8 status = FUSA_RADIO_READ_BANK_REG(0, FMCWPLL_LDO4);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       get ldo for fmcwpll chirp monitor status
; arg:
;       status: [uint8] ldo for cm status
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_restore_ldo_for_fmcwpll_cm_status(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_WRITE_BANK_REG(0, FMCWPLL_LDO4, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       set ldo for fmcwpll chirp monitor to maximum
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_fmcwpll_cm_max(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, FMCWPLL_LDO4, LDO11_CM_EN, 0x1);
    FUSA_RADIO_MOD_BANK_REG(0, FMCWPLL_LDO4, LDO11_CM_VOUT_SEL, 0x7);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get ldo for spi status
; arg:
;       void
; return:
;       [uint8] ldo for spi status
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_ldo_for_spi_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    uint8 status = FUSA_RADIO_READ_BANK_REG(0, POR);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       get ldo for fmcwpll chirp monitor status
; arg:
;       status: [uint8] ldo for spi status
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_restore_ldo_for_spi_status(uint8 status)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_WRITE_BANK_REG(0, POR, status);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       set ldo for spi to maximum
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_ldo_for_spi_max(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, POR, LDO11_SPI_VOUT_SEL, 0x7);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get fmcwpll chirp monitor status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_fmcwpll_chirp_monitor_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(0, FMCWPLL_EN, CM_EN);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       enable fmcwpll chirp monitor
; arg:
;       enable : [bool] 1-enable 0-disable
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_fmcwpll_chirp_monitor_on(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(0);
    FUSA_RADIO_MOD_BANK_REG(0, FMCWPLL_EN, CM_EN, enable);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get fmcwpll chirp monitor status
; arg:
;       void
; return:
;       uint8
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint8 fusa_radio_get_chirp_monitor_mash_status(void)
{
    uint8 old_bank = fusa_radio_switch_bank(3);
    uint8 status = FUSA_RADIO_READ_BANK_REG_FIELD(3, FMCW_START, RSTN_SDM_MASH);
    fusa_radio_switch_bank(old_bank);
    return status;
}

/**************************************************************
;
; This routine:
;       reset sdm
; arg:
;       enable : [bool] 1-enable 0-disable
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_chirp_monitor_mash(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(3);
    FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, RSTN_SDM_MASH, enable);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       get chirp number
; arg:
;       void
; return:
;       uint32
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_get_chirp_size(void)
{
    uint8 frame_type_id = fusa_radio_get_rtl_frame_type();
    uint8 old_bank = fusa_radio_switch_bank(5 + frame_type_id);
    uint8 size0 = fusa_radio_reg_read(RADIO_BK5_FMCW_CHIRP_SIZE_1_0);
    uint8 size1 = fusa_radio_reg_read(RADIO_BK5_FMCW_CHIRP_SIZE_1_1);
    uint32 chirp_size = size0 + (size1 << 8);
    fusa_radio_switch_bank(old_bank);
    return chirp_size;
}

/************************************************************************************
;
; This routine:
;       set chirp number
; arg:
;       chirp_size: [uint32] value write into FMCW_CHIRP_SIZE
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_set_chirp_size(uint32 chirp_size)
{
    uint8 frame_type_id = fusa_radio_get_rtl_frame_type();
    uint8 old_bank = fusa_radio_switch_bank(5 + frame_type_id);
    fusa_radio_reg_write(RADIO_BK5_FMCW_CHIRP_SIZE_1_0, REG_L(chirp_size));
    fusa_radio_reg_write(RADIO_BK5_FMCW_CHIRP_SIZE_1_1, REG_M(chirp_size));
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       generate a random number based on running time
; arg:
;       chirp_size: [uint32] value write into FMCW_CHIRP_SIZE
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_random_num_generation(void)
{
    uint32 ticks = core_reg_read(AUX_RTC_LOW);
    srand(ticks);
    return rand();
}

/************************************************************************************
;
; This routine:
;       use logic seeds to set fmcw phase scramble init state
; arg:
;       random_seeds: [uint32] a random logic seeds
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_logic_seeds_for_fmcw(uint32 random_seeds)
{
    uint8 old_bank = fusa_radio_switch_bank(3);
    uint8 ps_state_L, ps_state_M, ps_state_H, ps_state_INT;
    ps_state_L =   random_seeds & 0xff;
    ps_state_M =   (random_seeds >> 8) & 0xff;
    ps_state_H =   (random_seeds >> 16) & 0xff;
    ps_state_INT = (random_seeds >> 24) & 0xff;
    FUSA_RADIO_WRITE_BANK_REG(3, FMCW_PS_STATE0, ps_state_L);
    FUSA_RADIO_WRITE_BANK_REG(3, FMCW_PS_STATE1, ps_state_M);
    FUSA_RADIO_WRITE_BANK_REG(3, FMCW_PS_STATE2, ps_state_H);
    FUSA_RADIO_WRITE_BANK_REG(3, FMCW_PS_STATE3, ps_state_INT);
    fusa_radio_switch_bank(old_bank);
}

/************************************************************************************
;
; This routine:
;       use logic seeds to set baseband
; arg:
;       random_seeds: [uint32] a random logic seeds
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
void fusa_radio_logic_seeds_for_bb(uint32 random_seeds)
{
    raw_writel(BB_REG_FFT_DINT_DAT_PS, random_seeds);
    raw_writel(BB_REG_SAM_DINT_DAT,    random_seeds);
}

/****************************************
;
; This routine:
;       generate fmcw
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;***************************************/
void fusa_radio_generate_fmcw(void)
{
        uint8 old_bank = fusa_radio_switch_bank(3);
        //0x02 predefined
        FUSA_RADIO_WRITE_BANK_REG(3, FMCW_MODE_SEL, 0x2);
        /* Only need to reset once at beginning */
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, ADDER_RSTN, 0x0);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, ADDER_RSTN, 0x1);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, START_SPI, 0x0);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, RSTN_SDM_MASH, 0x0);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_SYNC, EN, 0x0);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, RSTN_SDM_MASH, 0x1);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_SYNC, SEL, 0x3);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_SYNC_DLY, SYNC_DLY_EN, 0x1);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_SYNC, EN, 0x1);
        FUSA_RADIO_MOD_BANK_REG(3, FMCW_START, START_SPI, 0x0);
        fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       set fmcwpll chirp monitor threshold
; arg:
;       thr_h: 1~31  filter, means how many fault occured in a chirp will report an err
;       thr_l: 1~7   threshold
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;********************************************************************/
void fusa_radio_chirp_mon_thr_set(uint8 thr_h,uint8 thr_l)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, FM_CTRL_1, FM_DIFFERS_TH, thr_l);
    FUSA_RADIO_MOD_BANK_REG(10, FM_CTRL_1, FM_DIFFERS_CNTSIZE, thr_h);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       get fmcwpll chirp monitor threshold
; arg:
;       thr_h: 1~31  filter, means how many fault occured in a chirp will report an err
;       thr_l: 1~7   threshold
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;********************************************************************/
void fusa_radio_chirp_mon_thr_get(uint8 *thr_h,uint8 *thr_l)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    *thr_l = FUSA_RADIO_READ_BANK_REG_FIELD(10, FM_CTRL_1, FM_DIFFERS_TH);
    *thr_h = FUSA_RADIO_READ_BANK_REG_FIELD(10, FM_CTRL_1, FM_DIFFERS_CNTSIZE);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       enable fmcwpll chirp monitor
; arg:
;       enable : [bool] 1-enable 0-disable
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;********************************************************************/
void fusa_radio_set_chirp_monitor(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, FM_CTRL_0, FM_TEST_EN, enable);
    FUSA_RADIO_MOD_BANK_REG(10, FM_CTRL_0, FM_RSTN, enable);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_4, FM_IRQ_RSTN, enable);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       reset release fmcwpll chirp monitor
; arg:
;       enable : [bool] 1-enable 0-disable
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;********************************************************************/
void fusa_radio_chirp_monitor_reset_release(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_4, FM_IRQ_RSTN, 0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_4, FM_IRQ_RSTN, 1);
    fusa_radio_switch_bank(old_bank);
}

/*********************************************************************
;
; This routine:
;       clear fmcwpll chirp monitor irq
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_clear_chirp_monitor(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_4, ITF_FM_CLEAR, 0x00);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_4, ITF_FM_CLEAR, 0x01);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_4, ITF_FM_CLEAR, 0x00);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       start fmcwpll chirp monitor irq
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_start_chirp_monitor(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_4, ITF_FM_TEST_START, 0x00);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_4, ITF_FM_TEST_START, 0x01);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_4, ITF_FM_TEST_START, 0x00);
    fusa_radio_switch_bank(old_bank);
}

/**************************************************************
;
; This routine:
;       return chirp monitor IRQ
; arg:
;       void
; return:
;       uint32
;       0 - chirp monitor works normally
;       1 - chirp monitor works abnormally
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint32 fusa_radio_return_chirp_monitor_irq(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint32 irq_value;
    irq_value = FUSA_RADIO_READ_BANK_REG(10, ITF_IRQ_4);
    fusa_radio_switch_bank(old_bank);
    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm911:chirp monitor fault injection for power on self test
; arg:
;       void
; return:
;       uint32
;       0 - fail
;       1 - pass
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_chirp_monitor_fault_injection(void)
{
    uint32 irq_value = 0;

    /* set chirp monitor threshold */
    fusa_radio_chirp_mon_thr_set(0x1f,0x06);
    /* save ldo for chirp monitor status */
    uint8 ldo_for_cm_status = fusa_radio_get_ldo_for_fmcwpll_cm_status();
    /* set ldo for chirp monitor to max */
    fusa_radio_set_ldo_for_fmcwpll_cm_max();
    /* save ldo for spi status */
    uint8 ldo_for_spi_status = fusa_radio_get_ldo_for_spi_status();
    /* set ldo for spi to max */
    fusa_radio_set_ldo_for_spi_max();
    /* save chirp monitor status */
    uint8 cm_status = fusa_radio_get_fmcwpll_chirp_monitor_status();
    /* enable chirp monitor */
    fusa_radio_fmcwpll_chirp_monitor_on(true);
    /* enable irq of chirp monitor */
    fusa_radio_set_chirp_monitor(true);
    /* chirp monitor reset release */
    fusa_radio_chirp_monitor_reset_release();
    /* clear irq of chirp monitor */
    fusa_radio_clear_chirp_monitor();
    /* start irq of chirp monitor */
    fusa_radio_start_chirp_monitor();
    /* generate fmcw */
    fusa_radio_generate_fmcw();
    /* save chirp monitor mash status */
    uint8 mash_status = fusa_radio_get_chirp_monitor_mash_status();
    /* reset sdm for fault injection */
    fusa_radio_set_chirp_monitor_mash(false);
    /* start fmcw */
    fusa_fmcw_start();
    fusa_delay_us(500);
    /* read irq of chirp monitor */
    irq_value = fusa_radio_return_chirp_monitor_irq();
    /* release reset sdm for monitoring */
    fusa_radio_set_chirp_monitor_mash(mash_status);
    /* clear irq of chirp monitor */
    fusa_radio_clear_chirp_monitor();
    /* disable irq of chirp monitor */
    fusa_radio_set_chirp_monitor(false);
    /* disable chirp monitor */
    fusa_radio_fmcwpll_chirp_monitor_on(cm_status);
    /* restore ldo for spi status */
    fusa_radio_restore_ldo_for_spi_status(ldo_for_spi_status);
    /* restore ldo for fmcwpll chirp monitor status */
    fusa_radio_restore_ldo_for_fmcwpll_cm_status(ldo_for_cm_status);

    return irq_value;
}

/************************************************************************************
;
; This routine:
;       get irq value of sm13:chirp monitor
; arg:
;       void
;       uint32
;       0 - pass
;       1 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_chirp_monitor_irq(void)
{
    uint32 irq_value;
    uint32 random_seeds;
    uint8 tx_status[4] = {0};

    // /* set chirp monitor threshold */
    // fusa_radio_chirp_mon_thr_set(0x1f,0x06);
    /* save tx status */
    fusa_radio_get_tx_status(tx_status);
    /* turn off tx */
    fusa_radio_set_tx_ch_status(-1, false);
    /* save fmcw chirp size */
    uint32 chirp_size = fusa_radio_get_chirp_size();
    /* save ldo for chirp monitor status */
    uint8 ldo_for_cm_status = fusa_radio_get_ldo_for_fmcwpll_cm_status();
    /* enable ldo for chirp monitor */
    fusa_radio_set_ldo_for_fmcwpll_cm_max();
    /* save ldo for spi status */
    uint8 ldo_for_spi_status = fusa_radio_get_ldo_for_spi_status();
    /* set ldo for spi to max */
    fusa_radio_set_ldo_for_spi_max();
    /* save chirp monitor status */
    uint8 cm_status = fusa_radio_get_fmcwpll_chirp_monitor_status();
    /* enable chirp monitor */
    fusa_radio_fmcwpll_chirp_monitor_on(true);
    /* enable irq of chirp monitor */
    fusa_radio_set_chirp_monitor(true);
    /* chirp monitor reset release */
    fusa_radio_chirp_monitor_reset_release();
    /* clear irq of chirp monitor */
    fusa_radio_clear_chirp_monitor();
    /* start irq of chirp monitor */
    fusa_radio_start_chirp_monitor();
    /* set chirp number and phase scramble init state*/
    fusa_radio_set_chirp_size(0x1);
    /* start fmcw */
    fusa_fmcw_start();
    fusa_delay_us(500);
    /* read irq of chirp monitor */
    irq_value = fusa_radio_return_chirp_monitor_irq();
    /* clear irq of chirp monitor */
    fusa_radio_clear_chirp_monitor();
    /* disable irq of chirp monitor */
    fusa_radio_set_chirp_monitor(false);
    /* restore chirp monitor status */
    fusa_radio_fmcwpll_chirp_monitor_on(cm_status);
    /* restore ldo for spi status */
    fusa_radio_restore_ldo_for_spi_status(ldo_for_spi_status);
    /* restore ldo for fmcwpll chirp monitor status */
    fusa_radio_restore_ldo_for_fmcwpll_cm_status(ldo_for_cm_status);
    /* resume fmcw chirp size*/
    fusa_radio_set_chirp_size(chirp_size);
    /* set BB the same logic seeds as FMCW */
    random_seeds = fusa_radio_random_num_generation();
    fusa_radio_logic_seeds_for_fmcw(random_seeds);
    fusa_radio_logic_seeds_for_bb(random_seeds);
    /* resume tx */
    fusa_radio_set_tx_status(tx_status);

    return irq_value;
}

/**************************************************************
;
; This routine:
;       set over temp detector threshold
; arg:
;       uint32: over_temp_th
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_temp_detector_threshold_set(float32 over_temp_th)
{
    uint32 reg_val = (over_temp_th - 125) * 107.04 + 107170;

    uint8 old_bank = fusa_radio_switch_bank(10);
    /* set threshold  */
    FUSA_RADIO_WRITE_BANK_REG(10, TEMP_MONITOR_VTH_HIGH_2, REG_H(reg_val));
    FUSA_RADIO_WRITE_BANK_REG(10, TEMP_MONITOR_VTH_HIGH_1, REG_M(reg_val));
    FUSA_RADIO_WRITE_BANK_REG(10, TEMP_MONITOR_VTH_HIGH_0, REG_L(reg_val));
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       get over temp detector threshold
; return:
;       float: over_temp_th
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
float32 fusa_radio_temp_detector_threshold_get()
{
    uint32 reg_val = 0;
    uint8 old_bank = fusa_radio_switch_bank(10);
    /* set threshold  */
    reg_val |= ((uint32)FUSA_RADIO_READ_BANK_REG(10, TEMP_MONITOR_VTH_HIGH_2)) << 16;
    reg_val |= ((uint32)FUSA_RADIO_READ_BANK_REG(10, TEMP_MONITOR_VTH_HIGH_1)) << 8;
    reg_val |= ((uint32)FUSA_RADIO_READ_BANK_REG(10, TEMP_MONITOR_VTH_HIGH_0)) << 0;
    fusa_radio_switch_bank(old_bank);
    return(float32)((reg_val - 107170) / 107.04 + 125);
}

/**************************************************************
;
; This routine:
;       enable over temp detector
; arg:
;       enable : [bool] 1-enable 0-disable
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_set_over_temp_detector(bool enable)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_5, TEMP_IRQ_RSTN, enable);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       clear over temp detector
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_clear_over_temp_detector(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_5, ITF_TEMP_CLEAR, 0x0);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_5, ITF_TEMP_CLEAR, 0x1);
    fusa_delay_us(100);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_5, ITF_TEMP_CLEAR, 0x0);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       start over temp detector
; arg:
;       void
; return:
;       void
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
void fusa_radio_start_over_temp_detector(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_5, ITF_TEMP_TEST_START, 0x1);
    fusa_delay_us(300);
    FUSA_RADIO_MOD_BANK_REG(10, ITF_IRQ_CTRL_5, ITF_TEMP_TEST_START, 0x0);
    fusa_radio_switch_bank(old_bank);
}
/**************************************************************
;
; This routine:
;       return over temp detector IRQ
; arg:
;       void
; return:
;       uint32
;       0 - over temperature detector works normally
;       1 - over temperature detector abnormally
; Change tracking:
;       Ver0.0.1 :
;*************************************************************/
uint32 fusa_radio_return_over_temp_detector_irq(void)
{
    uint8 old_bank = fusa_radio_switch_bank(10);
    uint32 irq_value;
    irq_value = FUSA_RADIO_READ_BANK_REG(10, ITF_IRQ_5);
    fusa_radio_switch_bank(old_bank);
    return irq_value;
}
/************************************************************************************
;
; This routine:
;       get irq value of sm910: over temp detector fault injection for power on self test
; arg:
;       void
; return:
;       uint32
;       0 - fail
;       1 - pass
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_over_temp_detector_fault_injection(void)
{
    uint32 irq_value;
    /* set over temperature detector threshold as 0 for fault injection */
    fusa_radio_temp_detector_threshold_set(-100.0);
    /* disable over temperature detector */
    fusa_radio_set_over_temp_detector(false);
    /* use auxadc2 and muxsel ts vptat, measure temperature */
    fusa_radio_auxadc2_ts_on();
    /* enable over temperature detector */
    fusa_radio_set_over_temp_detector(true);
    /* clear irq of over temperature detector */
    fusa_radio_clear_over_temp_detector();
    /* start irq of over temperature detector */
    fusa_radio_start_over_temp_detector();
    /* read irq of over temperature detector */
    irq_value = fusa_radio_return_over_temp_detector_irq();
    /* disable auxadc2 */
    fusa_radio_auxadc2_voltage_off();
    /* set over temperature detector threshold as 125C dout code for monitoring */
    fusa_radio_temp_detector_threshold_set(125.0);
    /* disable over temperature detector */
    fusa_radio_set_over_temp_detector(false);
    /* clear irq of over temperature detector */
    fusa_radio_clear_over_temp_detector();

    return irq_value;
}
/************************************************************************************
;
; This routine:
;       get IRQ value of sm14:over temp detector
; arg:
;       radio
; return:
;       uint32
;       0 - pass
;       1 - fail
; Change tracking:
;       Ver0.0.1 :
;***********************************************************************************/
uint32 fusa_radio_sm_over_temp_detector_irq(void)
{
    uint32 irq_value;
    /* disable over temperature detector */
    fusa_radio_set_over_temp_detector(false);
    /* set over temperature detector threshold for monitoring */
    // fusa_radio_temp_detector_threshold_set(125.0);
    /* use auxadc2 and muxsel ts vptat, measure temperature */
    fusa_radio_auxadc2_ts_on();
    /* enable over temperature detector */
    fusa_radio_set_over_temp_detector(true);
    /* clear irq of over temperature detector */
    fusa_radio_clear_over_temp_detector();
    /* start irq of over temperature detector */
    fusa_radio_start_over_temp_detector();
    /* read irq of over temperature detector */
    irq_value = fusa_radio_return_over_temp_detector_irq();
    /* disable auxadc2 */
    fusa_radio_auxadc2_voltage_off();
    /* disable over temperature detector */
    fusa_radio_set_over_temp_detector(false);
    /* clear irq of over temperature detector */
    fusa_radio_clear_over_temp_detector();
    return irq_value;
}
