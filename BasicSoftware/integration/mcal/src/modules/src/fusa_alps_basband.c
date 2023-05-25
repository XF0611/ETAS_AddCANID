#include "fusa_alps_basband.h"
#include "baseband_alps_FM_reg.h"
#include "Platform_Types.h"
#include "common.h"
#include "alps_clock.h"
#include "fusa_alps_emu.h"
#include "fusa_alps_dmu.h"
#include "fusa_alps.h"
#include "CDD_Fusa.h"

//baseband in power on or lbist done state
static volatile  boolean bb_in_power_on_rst_state = true;
boolean fusa_bb_state_get()
{
    return bb_in_power_on_rst_state;
}
void fusa_bb_state_set(boolean state)
{
    bb_in_power_on_rst_state = state;
}
uint32 fusa_bb_switch_mem_access(uint32 table_id)
{
    uint32 old = raw_readl(BB_REG_SYS_MEM_ACT);
    raw_writel(BB_REG_SYS_MEM_ACT, table_id);
    return old;
}
// switch bank active
uint32 fusa_bb_switch_bnk_act(uint32 bnk_id)
{
        uint32 old = raw_readl(BB_REG_SYS_BNK_ACT);
        raw_writel(BB_REG_SYS_BNK_ACT, bnk_id);
        return old;
}

// switch bank mode
uint32 fusa_bb_switch_bnk_mode(uint32 bnk_mode)
{
        uint32 old = raw_readl(BB_REG_SYS_BNK_MODE);
        raw_writel(BB_REG_SYS_BNK_MODE, bnk_mode);
        raw_writel(BB_REG_SYS_BNK_RST, 1);
        return old;
}
void fusa_bb_mem_ecc_en(uint32 state)
{
    if(state)
    {
        raw_writel(BB_REG_SYS_ECC_ENA, 0x7fff);
    }
    else
    {
        raw_writel(BB_REG_SYS_ECC_ENA, 0x0000);
    }
}

void fusa_bb_mem_ecc_err_clr()
{
    raw_writel(BB_REG_SYS_ECC_SB_CLR, raw_readl(BB_REG_SYS_ECC_SB_STATUS));
    raw_writel(BB_REG_SYS_ECC_DB_CLR, raw_readl(BB_REG_SYS_ECC_DB_STATUS));
}
static void bb_mem_clear(uint32 len, uint32 val)
{
    volatile uint32 *p = (volatile uint32 *)BB_MEM_BASEADDR;
    for (uint32 i = 0; i < len; i++)
    {
        *p++ = val;
    }
}
void fusa_bb_clear_memory(uint32 bank_id)
{
    uint32 old = 0;
    float32 start_time = fusa_current_time_ms();
    fusa_bb_mem_ecc_en(TRUE); //ecc enable
    uint32 old_bank = fusa_bb_switch_bnk_act(bank_id);//
    //clear BB MEM_MAC
    old = fusa_bb_switch_mem_access(BB_MEM_ACT_MAC); //Store bank selected
    bb_mem_clear(16384, 0);

    //clear BB MEM_RLT_DOA
    fusa_bb_switch_mem_access(BB_MEM_ACT_RLT); //Store bank selected
    memset((void *)BB_MEM_BASEADDR, 0, 1024 * sizeof(obj_info_t));

    fusa_bb_switch_mem_access(BB_MEM_ACT_WIN); //Store bank selected
    bb_mem_clear(8192, 0);
    fusa_bb_switch_mem_access(old); // Restore back to old bank
    fusa_bb_switch_bnk_act(old_bank);
    //log_fusa("%s time=%f\r\n", __func__, fusa_current_time_ms() - start_time);
}

void fusa_bb_top_clk_en(uint32 state)
{
    bb_top_enable(state);
}

void fusa_bb_core_clk_en(uint32 state)
{
    bb_core_enable(state);
}

void fusa_bb_core_reset(uint32 state)
{
    if (state) {
        raw_writel(REL_REGBASE_CLKGEN + 0x30c, 0);
    } else {
        raw_writel(REL_REGBASE_CLKGEN + 0x30c, 1);
    }
}
uint8 fusa_bb_switch_buf_store(uint8 buf_id)
{
    uint8 old = raw_readl(BB_REG_SAM_SINKER);
    raw_writel(BB_REG_SAM_SINKER, buf_id); /* fft direct or adc buffer */
    return old;
}

void fusa_bb_frame_interleave_pattern(uint8 frame_loop_pattern)
{
        /* bb bank mode */
        uint8 bb_bank_mode = SYS_BNK_MODE_ROTATE;

        /* bb bank queue */
        uint8 bb_bank_que = 1 << frame_loop_pattern;
        raw_writel(BB_REG_SYS_BNK_MODE, bb_bank_mode);
        raw_writel(BB_REG_SYS_BNK_QUE, bb_bank_que); /* used in rotate mode */
        raw_writel(BB_REG_SYS_BNK_RST, 1);
}

uint32 fusa_bb_read_first_chirp_data(uint32 ch_index,uint32 data_index,uint32 data_index_max)
{
    volatile uint32 *mem = (uint32 *)BB_MEM_BASEADDR;
    mem += ch_index * data_index_max + data_index;
    return *mem;
}
void fusa_bb_hw_reset_after_force()
{
    // 1st, adc reset asserted
#ifdef CHIP_CASCADE
    if (chip_cascade_status() == CHIP_CASCADE_MASTER)
            raw_writel(REG_DMU_ADC_RSTN, 0); // asserted reset_n
#else
    // save old status
    uint32 old_mux_reset = raw_readl(REG_DMU_MUX_RESET);
    uint32 old_mux_sync = raw_readl(REG_DMU_MUX_SYNC);

    raw_writel(REG_DMU_MUX_RESET, 4);
    raw_writel(REG_DMU_MUX_SYNC, 4);
    raw_writel(REG_DMU_ADC_RSTN, 0); // asserted ADC reset_n

#endif

    // 2nd, bb reset asserted
    fusa_bb_core_reset(1);           // reset bb_core

    // 3rd, adc reset deasserted
    fusa_delay_us(10);                 // delay for ADC reset
#ifdef CHIP_CASCADE
    if (chip_cascade_status() == CHIP_CASCADE_MASTER)
            raw_writel(REG_DMU_ADC_RSTN, 1); // deasserted reset_n
#else
    raw_writel(REG_DMU_ADC_RSTN, 1); // deasserted ADC reset_n
    // restore old status
    raw_writel(REG_DMU_MUX_RESET, old_mux_reset);
    raw_writel(REG_DMU_MUX_SYNC, old_mux_sync);
#endif

    // 4th, bb reset deasserted
    fusa_bb_core_reset(0);           // deassert reset

}



sint32 fusa_bb_lbist()
{
    sint32 ret = E_FUSA_OK;
    uint32 bb_done_irq_enabled = fusa_dmu_irq_enabled(FUSA_BB_IRQ_DONE);
    uint32 bb_sam_done_irq_enabled = fusa_dmu_irq_enabled(FUSA_BB_IRQ_SAM_DONE);
    uint32 bb_ecc_sb_irq_enabled = fusa_dmu_irq_enabled(FUSA_BB_IRQ_ECC_SB);
    fusa_dmu_irq_en(FUSA_BB_IRQ_ECC_SB, FALSE);  // irq mask disable
    fusa_dmu_irq_en(FUSA_BB_IRQ_DONE, FALSE); // irq mask disable
    fusa_dmu_irq_en(FUSA_BB_IRQ_SAM_DONE, FALSE);  // irq mask disable

    fusa_bb_top_clk_en(FALSE);               // disable bb_top clock

    // start BB_LBIST
    emu_bb_lbist_clear();
    emu_bb_lbist_en();

    fusa_delay_ms(10); // wait for done, FIXME, DONE signal always be true(bug in alps_mp), so using extra delay
    if(emu_lbist_state() & 0x02)
    {
        ret = E_FUSA_OBJ;
    }

    // reset bb
    fusa_bb_hw_reset_after_force();
    fusa_bb_top_clk_en(TRUE);
    raw_writel(BB_REG_SYS_BNK_RST, 1);
    raw_writel(BB_REG_SYS_IRQ_CLR, BB_IRQ_CLEAR_ALL);
    fusa_bb_mem_ecc_err_clr();
    // restore irq mask
    if (bb_done_irq_enabled)
    {
        fusa_dmu_irq_en(FUSA_BB_IRQ_DONE, TRUE);
    }
    if(bb_sam_done_irq_enabled)
    {
        fusa_dmu_irq_en(FUSA_BB_IRQ_SAM_DONE, TRUE);
    }
    if(bb_ecc_sb_irq_enabled)
    {
        fusa_dmu_irq_en(FUSA_BB_IRQ_ECC_SB, TRUE);
    }
    fusa_bb_state_set(TRUE);
    return ret; /* return ture if bist success */
}

uint32 fusa_bb_get_rtl_frame_type()
{
    return raw_readl(BB_REG_FDB_SYS_BNK_ACT);
}
void fusa_bb_agc_irq_en(boolean state)
{
    if(state)
    {
        raw_writel(BB_REG_AGC_IRQ_ENA, 0xffff);
    }
    else
    {
        raw_writel(BB_REG_AGC_IRQ_ENA, 0x0000);
    }
}
void fusa_bb_agc_clr_irq_status()
{
    raw_writel(BB_REG_AGC_SAT_CNT_CLR_FRA,  1);
    raw_writel(BB_REG_AGC_IRQ_CLR, raw_readl(BB_REG_AGC_IRQ_STATUS));
}
