#include <string.h>
#include <stdio.h>
#include "math.h"

#include "Os.h"
//#include "Os_Cfg.h"
#include "Std_Types.h"
#include "calterah_limits.h"
#include "calterah_math_funcs.h"
#include "baseband_hw.h"
#include "fft_window.h"
#include "baseband_alps_FM_reg.h"
#include "alps_mmap.h"
#include "alps_dmu_reg.h"
#include "alps_reset_api.h"
#include "alps_clock.h"
#include "CDD_Baseband.h"
#include "system_clock_init.h"
#include "baseband_ext.h"
#include "alps_timer.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define BB_FMCW_SDM_FREQ 360 /* MHz */
#define DIV_BASEBAND(val, F_SD) ((val)*1e3 / 8 / (F_SD)-16) * (1L << 28)
#define INV_DIV_BASEBAND(val, F_SD) (1.0 * (val) / (1L << 28) + 16) * 8 * (F_SD) / 1000
#define BB_FREQ_SYNTH_SD_RATE BB_FMCW_SDM_FREQ

/* TODO: only use for HIL test*/
#define REG_DMU_HIL_ENA_OFFSET (0x0114)
#define REG_DMU_HIL_DAT_OFFSET (0x0118)

#define TWLUTSZ 2048
#define DMLKINITCOEF 0.8

#define DEG2RAD 0.017453292519943295
#ifndef M_PI
#define M_PI 3.1415926535f
#endif

#define CFAR_RDM_RNG_SEP_SZ 3
#define CFAR_RDM_VEL_SEP_SZ 2

#define CFAR_CA_SCALAR_BW 12
#define CFAR_CA_SCALAR_INT 5

#define CFAR_OS_SCALAR_BW 10
#define CFAR_OS_SCALAR_INT 1

#define SHADOW_BNK 4
#define ABIST_SIN_FREQ 5    // analog bist sinewave frequency, 5MHz defaultly
#define ABIST_FFT_CHK_MIN 3 // avoid zero frequency, DC may be greater than sinewave
#define ABIST_FFT_CHK_SIZE 31
#define ABIST_FFT_CHK_HLF ((ABIST_FFT_CHK_SIZE - 1) / 2)

#define BB_LBIST_DELAY 10 /* delay 10ms to wait lbist done*/

#define ENA_PRINT_MEM_READ 0

#define EMU_READ_REG(RN) baseband_read_reg(EMU_REG_##RN)
#define EMU_READ_REG_FEILD(RN, FD) baseband_read_regfield(EMU_REG_##RN, EMU_REG_##RN##_##FD##_SHIFT, EMU_REG_##RN##_##FD##_MASK)
#define EMU_WRITE_REG(RN, val) baseband_write_reg(EMU_REG_##RN, val)

#define BB_READ_REG(RN) baseband_read_reg(BB_REG_##RN)
#define BB_READ_REG_FEILD(RN, FD) baseband_read_regfield(BB_REG_##RN, BB_REG_##RN##_##FD##_SHIFT, BB_REG_##RN##_##FD##_MASK)
#define BB_WRITE_REG(RN, val) baseband_write_reg(BB_REG_##RN, val)
#define BB_MOD_REG(RN, FD, val) baseband_mod_reg(BB_REG_##RN, val, BB_REG_##RN##_##FD##_SHIFT, BB_REG_##RN##_##FD##_MASK)
#define SYS_ENA(MASK, var) (var << SYS_ENABLE_##MASK##_SHIFT)

#define BFM_ANT_IDX_BW 5
#define GRP_ADDRESS_OFFSET (BB_REG_DOA_GRP1_ADR_COE - BB_REG_DOA_GRP0_ADR_COE)

#define AGC_LNA_GAIN_LEVEL 2
#define AGC_TIA_GAIN_LEVEL 4
#define AGC_VGA1_GAIN_LEVEL 6
#define AGC_VGA2_GAIN_LEVEL 6
#define AFE_STAGE_NUM 4
#define AGC_FNL_LEVEL_NUM 64
#define AFE_SATS 3
#define LNA_BW 1
#define TIA_BW 2
/*shared by VGA1 and VGA2*/
#define VGA_BW 3
#define GAIN_BW 8

#define DATA_DUMP_PATTEN 1
#define DATA_DUMP_NUM_2MB (1024 * 256 * 4 / 2) /* rng * vel * ant / 2, (word) */
#define HALF_MAX_ADC_DATA (32768 - 1)          /* 2 << 16 - 1*/

/* To ensure DC calibration works, DO NOT change any static const values defined below */
/* Number of cascaded second order sub-IIR filters */
#define CASCD_SOS_NUM 4
#define SV_IN_FLASH_MAP_BASE 0x100000
#define BYTES_PER_WORD 4
#define BB_CLK_EN 1U

// extern n2d_info_t n2d_info[MAX_FRAME_TYPE];

static n2d_info_t n2d_info[MAX_FRAME_TYPE] = {0};
static BasebandDriverInit_t baseband[MAX_FRAME_TYPE];
static BasebandSystemParams_t baseband_sys_params[MAX_FRAME_TYPE];
static uint8_t cfg_total_num = 0;

/* Coefficients for IIR filter when dec_factor is 2 */
static const uint8_t DEC_2_A1[CASCD_SOS_NUM] = {0xe7, 0xee, 0xf7, 0xfe};
static const uint8_t DEC_2_A2[CASCD_SOS_NUM] = {0x0f, 0x30, 0x6c, 0xc4};
static const uint8_t DEC_2_B1[CASCD_SOS_NUM] = {0x7b, 0x5b, 0x39, 0x27};
static const uint8_t DEC_2_S1[CASCD_SOS_NUM] = {0x0, 0x0, 0x0, 0x0};
static const uint32_t DEC_2_SCL = 0x1e1;
static const uint32_t DEC_2_SHF = 0x1;
/* Coefficients for IIR filter when dec_factor is 3 */
static const uint8_t DEC_3_A1[CASCD_SOS_NUM] = {0xfc, 0x13, 0x2d, 0x40};
static const uint8_t DEC_3_A2[CASCD_SOS_NUM] = {0x0b, 0x47, 0x93, 0xda};
static const uint8_t DEC_3_B1[CASCD_SOS_NUM] = {0x68, 0x0d, 0xda, 0xc8};
static const uint8_t DEC_3_S1[CASCD_SOS_NUM] = {0x0, 0x0, 0x0, 0x0};
static const uint32_t DEC_3_SCL = 0x1ff;
static const uint32_t DEC_3_SHF = 0x1;
/* Coefficients for IIR filter when dec_factor is 4 */
static const uint8_t DEC_4_A1[CASCD_SOS_NUM] = {0x1f, 0x2c, 0x3f, 0x51};
static const uint8_t DEC_4_A2[CASCD_SOS_NUM] = {0x14, 0x42, 0x87, 0xd4};
static const uint8_t DEC_4_B1[CASCD_SOS_NUM] = {0x65, 0x02, 0xd1, 0xc0};
static const uint8_t DEC_4_S1[CASCD_SOS_NUM] = {0x1, 0x0, 0x0, 0x0};
static const uint32_t DEC_4_SCL = 0x168;
static const uint32_t DEC_4_SHF = 0;
/* Coefficients for IIR filter when dec_factor is 8 */
static const uint8_t DEC_8_A1[CASCD_SOS_NUM] = {0x46, 0x53, 0x62, 0x6f};
static const uint8_t DEC_8_A2[CASCD_SOS_NUM] = {0x4f, 0x7c, 0xb4, 0xe6};
static const uint8_t DEC_8_B1[CASCD_SOS_NUM] = {0x2b, 0xb3, 0x9a, 0x93};
static const uint8_t DEC_8_S1[CASCD_SOS_NUM] = {0x2, 0x1, 0x0, 0x0};
static const uint32_t DEC_8_SCL = 0x11f;
static const uint32_t DEC_8_SHF = 0x1;
/* Gain coefficients when not applying IIR filter but dec_factor is large than 1 */
static const uint32_t DEC_SCL_DFLT = 0x1ff; // Default scaler
static const uint8_t DEC_SHF_DFLT = 1;      // Default left shift bits
/* To ensure DC calibration works, DO NOT change any static const values defined above */

static uint8_t AGC_LNA_gain_table[AGC_LNA_GAIN_LEVEL] = {19, 24};                  //LNA gain value
static uint8_t AGC_TIA_gain_table[AGC_TIA_GAIN_LEVEL] = {0, 6, 12, 18};            //250ohm,500ohm,1000ohm,2000ohm TIA gain value
static uint8_t AGC_VGA1_gain_table[AGC_VGA1_GAIN_LEVEL] = {7, 10, 13, 16, 19, 22}; //VGA1 gain value
static uint8_t AGC_VGA2_gain_table[AGC_VGA2_GAIN_LEVEL] = {5, 8, 11, 14, 17, 20};  //VGA2 gain value
static uint8_t LNA_gain_bit[AGC_LNA_GAIN_LEVEL] = {1, 1};                          //LNA gain enable bit 1:enbale, 0:disable
static uint8_t TIA_gain_bit[AGC_TIA_GAIN_LEVEL] = {0, 1, 1, 0};                    //TIA gain enable bit 1:enbale, 0:disable
static uint8_t VGA1_gain_bit[AGC_VGA1_GAIN_LEVEL] = {1, 1, 1, 1, 1, 1};            //VGA1 gain enable bit 1:enbale, 0:disable
static uint8_t VGA2_gain_bit[AGC_VGA2_GAIN_LEVEL] = {1, 1, 1, 1, 1, 1};            //VGA2 gain enable bit 1:enbale, 0:disable
static int tia_level = AGC_TIA_GAIN_LEVEL;                                         //save current maximal TIA level
static int vga1_level = AGC_VGA1_GAIN_LEVEL;                                       //save current maximal VGA1 level
static bool vga2_loop_end = false;                                                 //indicate if need lower down the maximal vga1 level
static bool vga1_loop_end = false;                                                 //indicate if need lower down the maximal TIA level

// HIL from APB
// sine wave, refer to cmodel hil case
//#define HIL_SIN_PERIOD 32
static uint16_t hil_sin[HIL_SIN_PERIOD] = {
        0x0, 0x9fd, 0x1397, 0x1c71, 0x2434, 0x2a92, 0x2f4d, 0x3237,
        0x3333, 0x3237, 0x2f4d, 0x2a92, 0x2434, 0x1c71, 0x1397, 0x9fd,
        0x0, 0xf602, 0xec68, 0xe38e, 0xdbcb, 0xd56d, 0xd0b2, 0xcdc8,
        0xcccc, 0xcdc8, 0xd0b2, 0xd56d, 0xdbcb, 0xe38e, 0xec68, 0xf602
};

/* For generation of sv */
static int32_t tx_order[MAX_NUM_TX] = {0, 0, 0, 0};
static antenna_pos_t ant_pos[MAX_ANT_ARRAY_SIZE];
static float ant_comp_phase[MAX_ANT_ARRAY_SIZE];

/* CFAR pk_mask mapping table */
static uint32_t CFAR_pk_msk[3][3] = {{0x1000, 0x3800, 0x7c00},
        {0x21080, 0x739c0, 0xfffe0},
        {0x421084, 0xe739ce, 0x1ffffff}};
static uint32_t sv_bk[4];
volatile uint8_t cur_radar_cfg_idx = 0;

void baseband_shadow_bnk_init(BasebandInitializationParams_t *cfg);
void baseband_hw_reset_after_force(void);
void switch_to_ddm_config(BasebandInitializationParams_t* cfg, uint8_t frame_type_id);
void recover_from_ddm_config(BasebandInitializationParams_t* cfg, uint8_t frame_type_id);

int32_t baseband_driver_info_register(BasebandInitializationParams_t *config, uint8_t cfg_nums)
{
    int32_t result = E_OK;

    if (cfg_nums > MAX_FRAME_TYPE){
        result = E_PAR;
    }else{
        cfg_total_num = cfg_nums;

        for(uint8_t i = 0;i < cfg_nums;i++){
            baseband[i].cfg = &config[i];
            baseband[i].sys_params = &baseband_sys_params[i];
            baseband[i].frame_type_id = i;
        }
    }
    return result;
}

BasebandDriverInit_t* baseband_driver_info_get(uint8_t frame_id)
{
    if (frame_id >= cfg_total_num){
        return NULL;
    }else{
        return &baseband[frame_id];
    }
}

n2d_info_t *baseband_get_n2d_info(uint8_t frame_id)
{
    n2d_info_t *ret = NULL;

    do {
        if(frame_id > MAX_FRAME_TYPE - 1) {
            break;
        }
        ret = &n2d_info[frame_id];
    } while(0);
    return ret;
}

BasebandDriverInit_t* baseband_cur_driver_info_get(void)
{
    uint8_t frame_type = baseband_get_rtl_frame_type();
    if(cfg_total_num <= frame_type){
        return NULL;
    }else{
        return &baseband[frame_type];
    }
}

void baseband_clock_init(void)
{
    raw_writel(REG_CLKGEN_ENA_BB_CORE, BB_CLK_EN);
    raw_writel(REG_CLKGEN_ENA_BB_TOP, BB_CLK_EN);
}

/* enable/disable baseband irq of DMU */
void baseband_irq_config(uint8_t irq, uint8_t en)
{
    uint32_t shift = 0;
    uint32_t addr = REL_REGBASE_DMU + REG_DMU_IRQ_ENA0_31_OFFSET;

    addr += (irq >> 5) << 2;
    shift = irq & 0x1F;
    if (en)
    {
        raw_writel(addr, raw_readl(addr) | (1 << shift));
    }
    else
    {
        raw_writel(addr, raw_readl(addr) & ~(1 << shift));
    }
}

uint32_t baseband_read_reg(const uint32_t addr)
{
    volatile uint32_t *ptr = (uint32_t *)addr;
    return *ptr;
}

uint32_t baseband_read_regfield(const uint32_t addr, const uint32_t shift, const uint32_t mask)
{
    uint32_t val = baseband_read_reg(addr);
    return (val >> shift) & mask;
}

void baseband_write_reg(const uint32_t addr, const uint32_t val)
{
    volatile uint32_t *ptr = (uint32_t *)addr;
    *ptr = val;
}

void baseband_mod_reg(const uint32_t addr, const uint32_t shift, const uint32_t mask, const uint32_t val)
{
    uint32_t tmp = baseband_read_reg(addr);
    tmp &= ~(mask << shift);      /* reverse mask to identiy the other bits */
    tmp |= (val & mask) << shift; /* combine the new input with the other bits */
    baseband_write_reg(addr, tmp);
}

uint32_t baseband_switch_mem_access(uint32_t table_id)
{
    uint32_t old = BB_READ_REG(SYS_MEM_ACT);
    BB_WRITE_REG(SYS_MEM_ACT, table_id);
    return old;
}

// switch bank active
uint8_t baseband_switch_bnk_act(uint8_t bnk_id)
{
    uint8_t old = BB_READ_REG(SYS_BNK_ACT);
    BB_WRITE_REG(SYS_BNK_ACT, bnk_id);
    return old;
}

/////TODO: frame-interleaving
//// switch bank mode
//uint32_t baseband_switch_bnk_mode(uint8_t bnk_mode)
//{
//        uint32_t old = BB_READ_REG(SYS_BNK_MODE);
//        BB_WRITE_REG(SYS_BNK_MODE, bnk_mode);
//        BB_WRITE_REG(SYS_BNK_RST, 1);
//        return old;
//}

uint32_t baseband_switch_buf_store(BasebandSamSinker_e sinker)
{
    uint32_t old;

    old = BB_READ_REG(SAM_SINKER);
    BB_WRITE_REG(SAM_SINKER, sinker); /* fft direct or adc buffer */
    // #endif
    return old;
}

void baseband_write_mem_table(uint32_t offset, uint32_t value)
{
    volatile uint32_t *mem = (uint32_t *)BB_MEM_BASEADDR;
    mem += offset;
    *mem = value;
}

uint32_t baseband_read_mem_table(uint32_t offset)
{
    volatile uint32_t *mem = (uint32_t *)BB_MEM_BASEADDR;
    mem += offset;
    return *mem;
}

uint32_t baseband_get_obj_info(uint32_t offset)
{
        uint32_t mem = BB_MEM_BASEADDR;
        mem+= offset;
        return mem;
}

uint32_t baseband_int_clear(uint32_t type)
{
	uint32_t result = 0;

	if((BB_IRQ_CLEAR_SAM_DONE != type) && (BB_IRQ_CLEAR_BB_DONE != type) && (BB_IRQ_CLEAR_ALL != type)){
		result = 2;
	}else{
		baseband_write_reg(BB_REG_SYS_IRQ_CLR, type);
	}
	return result;
}

static void get_sample_used(BasebandInitializationParams_t *cfg, uint32_t *samp_skip_arg, uint32_t *samp_used_arg)
{
        uint32_t samp_skip = ceil(cfg->adc_sample_start * cfg->com_params->adc_freq) / cfg->dec_factor;
        samp_skip = (samp_skip % 2 == 0) ? samp_skip : samp_skip + 1;

        uint32_t samp_end = ceil(cfg->adc_sample_end * cfg->com_params->adc_freq) / cfg->dec_factor;
        samp_end = (samp_end % 2 == 0) ? samp_end : samp_end - 1;

        uint32_t samp_used = samp_end - samp_skip;
        samp_used = (samp_used % 2 == 0) ? samp_used : samp_used - 1;

        if (samp_skip_arg)
                *samp_skip_arg = samp_skip;

        if (samp_used_arg)
                *samp_used_arg = samp_used;
}

void baseband_sys_init(BasebandInitializationParams_t *cfg, BasebandSystemParams_t *sys_params)
{
        uint32_t samp_skip;
        uint32_t samp_used;
        get_sample_used(cfg, &samp_skip, &samp_used);

        samp_used = samp_used > cfg->rng_nfft ? cfg->rng_nfft : samp_used;
        uint32_t chip_used = cfg->com_params->nchirp / cfg->nvarray;
        uint32_t chip_buf = chip_used > cfg->vel_nfft ? cfg->vel_nfft : chip_used;
        BB_WRITE_REG(SYS_SIZE_RNG_SKP, samp_skip - 1);
        BB_WRITE_REG(SYS_SIZE_RNG_BUF, samp_used - 1);
        BB_WRITE_REG(SYS_SIZE_RNG_PRD, sys_params->total_cycle * cfg->com_params->adc_freq / FREQ_SYNTH_SD_RATE - 1);
        BB_WRITE_REG(SYS_SIZE_BPM, cfg->nvarray - 1);
        BB_WRITE_REG(SYS_SIZE_RNG_FFT, cfg->rng_nfft - 1);
        BB_WRITE_REG(SYS_SIZE_VEL_FFT, cfg->vel_nfft - 1);
        BB_WRITE_REG(SYS_SIZE_VEL_BUF, chip_buf - 1);
        BB_WRITE_REG(SYS_IRQ_ENA, 1);

        uint16_t bb_status_en = SYS_ENA(AGC, cfg->com_params->agc_mode == 0 ? 0 : 1) | SYS_ENA(SAM, true) | SYS_ENA(FFT_2D, true) | SYS_ENA(CFR, true) | SYS_ENA(BFM, true);
        BB_WRITE_REG(SYS_ENABLE, bb_status_en);
        BB_WRITE_REG(SYS_ECC_ENA, 0); /* if (ECC_MODE != 'O'), write -1 to enable all */
}

static uint32_t agc_cdgn(uint8_t LNA_gain_table[], uint8_t TIA_gain_table[], uint8_t VGA1_gain_table[], uint8_t VGA2_gain_table[], uint32_t Tot_gain_table[])
{
        //Tot_gain_table = {LNA_gain_level,TIA_gain_level,VGA1_gain_level,VGA2_gain_level};
        uint32_t tot_gain = 0;
        uint32_t tot_cod = 0;
        tot_gain = (uint32_t)LNA_gain_table[Tot_gain_table[0]] + (uint32_t)TIA_gain_table[Tot_gain_table[1]];
        tot_gain = tot_gain + VGA1_gain_table[Tot_gain_table[2]] + VGA2_gain_table[Tot_gain_table[3]];
        tot_gain = tot_gain * 2; //GAIN FXR(8,7,U)
        //Gain code mapping rule
        //bit16 bit15 bit14 bit13 bit12 bit11 bit10 bit9 bit8 bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
        //LNA  |    TIA   |      VGA1       |       VGA2    |    GAIN
        // VGA's code equals level + 1 when level begins from 0
        tot_cod = (Tot_gain_table[3] + 1) | ((Tot_gain_table[2] + 1) << VGA_BW) | (Tot_gain_table[1] << (2 * VGA_BW)) | (Tot_gain_table[0] << (2 * VGA_BW + TIA_BW));
        tot_cod = (tot_cod << GAIN_BW) + tot_gain;
        return tot_cod;
}

static void afe_gain_min_max(uint8_t gain_table[], uint8_t gain_bit[], int table_size, int *gain, int *level)
{
        for (int p = 0; p < table_size; p++)
        {
                if (gain_bit[p])
                {
                        *gain = gain_table[p]; //minmum gain
                        *level = p;
                        gain++;
                        level++;
                        break;
                }
        }
        //maximun gain
        for (int p = table_size - 1; p >= 0; p--)
        {
                if (gain_bit[p])
                {
                        *gain = gain_table[p]; //maximum gain
                        *level = p;
                        break;
                }
        }
}

void antpower2gain(uint32_t final_temp[],
                   uint32_t lowest_gain_code[],
                   int ant_power,
                   int adc_target_db,
                   float thres_db[],
                   uint8_t AGC_LNA_gain_table[],
                   uint8_t AGC_TIA_gain_table[],
                   uint8_t AGC_VGA1_gain_table[],
                   uint8_t AGC_VGA2_gain_table[],
                   uint8_t LNA_gain_bit[],
                   uint8_t TIA_gain_bit[],
                   uint8_t VGA1_gain_bit[],
                   uint8_t VGA2_gain_bit[])
{
        int gain_lna_tia = 0;
        int lna_idx = 0;
        int tia_idx = 0;
        int mis_gain = 0;
        uint32_t lna, tia, tot_lna_tia;
        //initialize start
        for (int i = 0; i < AFE_STAGE_NUM; i++)
        {
                final_temp[i] = lowest_gain_code[i];
        }
        for (int l = 0; l < AGC_LNA_GAIN_LEVEL; l++)
        {
                if (vga1_loop_end == true)
                {
                        if (TIA_gain_bit[tia_level - 1] == 0)
                                tia_level = tia_level - 2;
                        else
                                tia_level = tia_level - 1;
                        vga1_loop_end = false;
                        vga1_level = AGC_VGA1_GAIN_LEVEL;
                }
                for (int t = 0; t < tia_level; t++)
                {
                        lna = AGC_LNA_gain_table[l];
                        tia = AGC_TIA_gain_table[t];
                        tot_lna_tia = lna + tia;
                        mis_gain = adc_target_db - ant_power;
                        if (((ant_power + tot_lna_tia) < thres_db[0]) && (tot_lna_tia <= mis_gain) && (LNA_gain_bit[l]) && (TIA_gain_bit[t]))
                        {
                                if (tot_lna_tia > gain_lna_tia)
                                {
                                        gain_lna_tia = tot_lna_tia;
                                        lna_idx = l;
                                        tia_idx = t;
                                }
                                else if ((tot_lna_tia == gain_lna_tia) && (l > lna_idx))
                                {
                                        lna_idx = l;
                                        tia_idx = t;
                                }
                                else
                                {
                                        /* Doing nothing */
                                }
                        }
                } //end loop TIA
        }         //end loop LNA
        if (gain_lna_tia == 0)
                return;
        final_temp[0] = lna_idx;
        final_temp[1] = tia_idx;
        ant_power = ant_power + gain_lna_tia;
        mis_gain = mis_gain - gain_lna_tia;
        for (int stage = 2; stage < AFE_STAGE_NUM; stage++)
        {
                int vga_idx = -1;
                if (stage == 2)
                {
                        if (vga2_loop_end == true)
                        {
                                vga1_level = vga1_level - 1;
                                vga2_loop_end = false;
                        }
                        for (int v = 0; v < vga1_level; v++)
                        {
                                if ((ant_power + AGC_VGA1_gain_table[v]) < thres_db[1] && (AGC_VGA1_gain_table[v] <= mis_gain) && (VGA1_gain_bit[v]))
                                        vga_idx = v;
                        } //end loop VGA1
                        if (vga_idx < 0)
                        {
                                vga1_loop_end = true;
                                return;
                        }
                        final_temp[stage] = vga_idx;
                        mis_gain = mis_gain - AGC_VGA1_gain_table[vga_idx];
                        ant_power = ant_power + AGC_VGA1_gain_table[vga_idx];
                }
                else
                {
                        for (int v = 0; v < AGC_VGA2_GAIN_LEVEL; v++)
                        {
                                if ((ant_power + AGC_VGA2_gain_table[v]) < thres_db[2] && (AGC_VGA2_gain_table[v] <= mis_gain) && (VGA2_gain_bit[v]))
                                        vga_idx = v;
                        } //end loop VGA2
                        if (vga_idx < 0)
                        {
                                vga2_loop_end = true;
                                return;
                        }
                        final_temp[stage] = vga_idx;
                        mis_gain = mis_gain - AGC_VGA2_gain_table[vga_idx];
                        ant_power = ant_power + AGC_VGA2_gain_table[vga_idx];
                }
        }
}

static void agc_mode_with_fixgain(BasebandInitializationParams_t *cfg)
{
        uint32_t temp[AFE_STAGE_NUM];
        uint32_t AGC_CD_entry = 0;
        uint32_t AGC_clip_table_base_address = BB_REG_AGC_CDGN_INIT;
        //translate gain code to gain level
        temp[0] = 1; //default LNA use level1
        temp[1] = (int)(log(cfg->com_params->rf_tia_gain) / log(2));
        temp[2] = cfg->com_params->rf_vga1_gain - 1;
        temp[3] = cfg->com_params->rf_vga2_gain - 1;
        AGC_CD_entry = agc_cdgn(AGC_LNA_gain_table, AGC_TIA_gain_table, AGC_VGA1_gain_table, AGC_VGA2_gain_table, temp);
        //write init gain & clip 1 & clip2 table
        for (int i = 0; i < AGC_CODE_ENTRY_NUM; i++)
        {
                baseband_write_reg(AGC_clip_table_base_address, AGC_CD_entry);
                AGC_clip_table_base_address = AGC_clip_table_base_address + 0x4;
        }
        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_COD);
        uint32_t offset_address = 0;
        //write final gain table
        for (int p = 0; p < AGC_FNL_LEVEL_NUM; p++)
        {
                baseband_write_mem_table(offset_address, AGC_CD_entry);
                offset_address = offset_address + 1;
        }
        baseband_switch_mem_access(old);
        BB_WRITE_REG(AGC_GAIN_MIN, 0x3dc); //protection for work flow
        BB_WRITE_REG(AGC_DB_TARGET, 0x78); //protection for work flow
}

static void baseband_agc_clear(void)
{
        uint32_t AGC_base_address = BB_REG_AGC_SAT_THR_TIA, offset_address = 0;
        int length = 32; //number of AGC related and can be written registers
        /* below operation is must,to avoid generate err saturation status when config AGC*/
        baseband_write_reg(BB_REG_AGC_IRQ_CLR, 0xfff);
        baseband_write_reg(BB_REG_AGC_IRQ_ENA, 0x0);
        baseband_write_reg(BB_REG_AGC_CHCK_ENA, 0x0);
        for (int i = 0; i < length; i++)
        {
                baseband_write_reg(AGC_base_address, 0x0);
                AGC_base_address = AGC_base_address + 0x4;
        }
        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_COD);
        for (int p = 0; p < AGC_FNL_LEVEL_NUM; p++)
        {
                baseband_write_mem_table(offset_address, 0x0);
                offset_address = offset_address + 1;
        }
        baseband_switch_mem_access(old);
}

void baseband_bnk_set(uint8_t frame_type_id)
{
        BB_WRITE_REG(SYS_BNK_ACT, frame_type_id);
}


void baseband_agc_init(BasebandInitializationParams_t *cfg)
{
        baseband_agc_clear();
        if (cfg->com_params->agc_mode == 0)
                return;
        BB_WRITE_REG(AGC_SAT_THR_TIA, 0x10101);
        BB_WRITE_REG(AGC_SAT_THR_VGA1, 0x10101);
        BB_WRITE_REG(AGC_SAT_THR_VGA2, 0x10101);
        BB_WRITE_REG(AGC_DAT_MAX_SEL, 0);
        BB_WRITE_REG(AGC_CODE_LNA_0, 0x4);
        BB_WRITE_REG(AGC_CODE_LNA_1, 0xF);
        BB_WRITE_REG(AGC_CODE_TIA_0, 0x1);
        BB_WRITE_REG(AGC_CODE_TIA_1, 0x2);
        BB_WRITE_REG(AGC_CODE_TIA_2, 0x4);
        BB_WRITE_REG(AGC_CODE_TIA_3, 0x8);
        int AGC_ADC_target_level = 10;
        uint32_t final_temp[AFE_STAGE_NUM], temp[AFE_STAGE_NUM];
        uint32_t AGC_clip_table_base_address = BB_REG_AGC_CDGN_INIT;
        uint32_t AGC_CD_entry = 0;
        if (cfg->com_params->agc_mode == 1)
        {
                //write code table
                for (int i = 0; i < AGC_CODE_ENTRY_NUM; i++)
                {
                        //translate gain level from config file, LSB is LNA gain level
                        temp[0] = cfg->agc_code[i] & 0x1;
                        temp[1] = (cfg->agc_code[i] >> LNA_BW) & 0x3;
                        temp[2] = (cfg->agc_code[i] >> (LNA_BW + TIA_BW)) & 0x7;
                        temp[3] = (cfg->agc_code[i] >> (LNA_BW + TIA_BW + VGA_BW)) & 0x7;
                        AGC_CD_entry = agc_cdgn(AGC_LNA_gain_table, AGC_TIA_gain_table, AGC_VGA1_gain_table, AGC_VGA2_gain_table, temp);
                        baseband_write_reg(AGC_clip_table_base_address, AGC_CD_entry);
                        AGC_clip_table_base_address = AGC_clip_table_base_address + 0x4;
                }
                //gen final gain table//
                int gain_temp[AFE_STAGE_NUM * 2] = {0, 0, 0, 0, 0, 0, 0, 0}, level_temp[AFE_STAGE_NUM * 2] = {0, 0, 0, 0, 0, 0, 0, 0}, *mingain_maxgain, *level;
                int min_power_db, max_power_db, power_step = 1, adc_target_db;
                int gain_level_num = 0, ADC_BiW = 13;
                float thres_db[AFE_SATS], ADC_vpp = 1.2;
                mingain_maxgain = gain_temp;
                level = level_temp;
                afe_gain_min_max(AGC_LNA_gain_table, LNA_gain_bit, AGC_LNA_GAIN_LEVEL, mingain_maxgain, level);
                afe_gain_min_max(AGC_TIA_gain_table, TIA_gain_bit, AGC_TIA_GAIN_LEVEL, mingain_maxgain + 2, level + 2);
                afe_gain_min_max(AGC_VGA1_gain_table, VGA1_gain_bit, AGC_VGA1_GAIN_LEVEL, mingain_maxgain + 4, level + 4);
                afe_gain_min_max(AGC_VGA2_gain_table, VGA2_gain_bit, AGC_VGA1_GAIN_LEVEL, mingain_maxgain + 6, level + 6);
                adc_target_db = (int)(20 * log10f(2) * AGC_ADC_target_level);
                thres_db[0] = 20 * log10f(cfg->com_params->agc_tia_thres * ADC_vpp) + 20 * log10f(2) * ADC_BiW;
                thres_db[1] = 20 * log10f(cfg->com_params->agc_vga1_thres * ADC_vpp) + 20 * log10f(2) * ADC_BiW;
                thres_db[2] = 20 * log10f(cfg->com_params->agc_vga2_thres * ADC_vpp) + 20 * log10f(2) * ADC_BiW;
                //get min_power_db
                min_power_db = adc_target_db - (gain_temp[1] + gain_temp[3] + gain_temp[5] + gain_temp[7]);
                max_power_db = adc_target_db - (gain_temp[0] + gain_temp[2] + gain_temp[4] + gain_temp[6]);
                //RTL FXR(9,8,S), C modul(w10,I9,S) real value -24
                BB_WRITE_REG(AGC_GAIN_MIN, float_to_fx((float)min_power_db, 10, 9, true));
                BB_WRITE_REG(AGC_DB_TARGET, float_to_fx((float)adc_target_db, 8, 7, false));
                gain_level_num = max_power_db - min_power_db + 1;
                uint32_t lowest_gain_code[AFE_STAGE_NUM];
                uint32_t AGC_final_CD_entry = 0, offset_address = 0;
                uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_COD);
                lowest_gain_code[0] = level_temp[0];
                lowest_gain_code[1] = level_temp[2];
                lowest_gain_code[2] = level_temp[4];
                lowest_gain_code[3] = level_temp[6];
                tia_level = AGC_TIA_GAIN_LEVEL;   //save current maximal TIA level
                vga1_level = AGC_VGA1_GAIN_LEVEL; //save current maximal VGA1 level
                vga2_loop_end = false;            //indicate if need lower down the maximal vga1 level
                vga1_loop_end = false;
                for (int p = min_power_db; p <= max_power_db; p = p + power_step)
                {
                        AGC_final_CD_entry = 0;
                        antpower2gain(final_temp,
                                      lowest_gain_code,
                                      p,
                                      adc_target_db,
                                      thres_db,
                                      AGC_LNA_gain_table,
                                      AGC_TIA_gain_table,
                                      AGC_VGA1_gain_table,
                                      AGC_VGA2_gain_table,
                                      LNA_gain_bit,
                                      TIA_gain_bit,
                                      VGA1_gain_bit,
                                      VGA2_gain_bit);
                        AGC_final_CD_entry = agc_cdgn(AGC_LNA_gain_table, AGC_TIA_gain_table, AGC_VGA1_gain_table, AGC_VGA2_gain_table, final_temp);
                        baseband_write_mem_table(offset_address, AGC_final_CD_entry);
                        offset_address = offset_address + 1;
                }
                // added match with RTL
                AGC_final_CD_entry = agc_cdgn(AGC_LNA_gain_table, AGC_TIA_gain_table, AGC_VGA1_gain_table, AGC_VGA2_gain_table, lowest_gain_code);
                for (int p = 0; p < (AGC_FNL_LEVEL_NUM - gain_level_num); p++)
                {
                        baseband_write_mem_table(offset_address, AGC_final_CD_entry);
                        offset_address = offset_address + 1;
                }
                baseband_switch_mem_access(old);
        }
        else
        {
                agc_mode_with_fixgain(cfg);
        }
        //IRQ related register
        BB_WRITE_REG(AGC_IRQ_ENA, 0xFFF);
        BB_WRITE_REG(AGC_CHCK_ENA, 0x1);
        //agc align
        BB_WRITE_REG(AGC_ALIGN_EN, cfg->agc_align_en);
        //adc compensation
        BB_WRITE_REG(AGC_CMPN_EN, cfg->adc_comp_en);
        BB_WRITE_REG(AGC_CMPN_LVL, 2);
        BB_WRITE_REG(AGC_CMPN_ALIGN_EN, 1);
        //        //set up agc radio part
        //        fmcw_radio_agc_setup(&bb->radio);
        //        fmcw_radio_agc_enable(&bb->radio,true);
}

//// TODO: init after bb and radio_init
//void radio_agc_init(){
//	//set up agc radio part
//	fmcw_radio_agc_setup(&bb->radio);
//	fmcw_radio_agc_enable(&bb->radio,true);
//}

void baseband_sam_init(BasebandInitializationParams_t *cfg, uint8_t frame_type_id)
{

        BB_WRITE_REG(SYS_SIZE_FLT, cfg->dec_factor - 1);

        uint32_t samp_skip;
        uint32_t samp_used;
        uint32_t old;

        get_sample_used(cfg, &samp_skip, &samp_used);

        samp_used = samp_used > cfg->rng_nfft ? cfg->rng_nfft : samp_used;

        uint8_t DEC_A1[CASCD_SOS_NUM] = {0x0, 0x0, 0x0, 0x0};
        uint8_t DEC_A2[CASCD_SOS_NUM] = {0x0, 0x0, 0x0, 0x0};
        uint8_t DEC_B1[CASCD_SOS_NUM] = {0x0, 0x0, 0x0, 0x0};
        uint8_t DEC_S1[CASCD_SOS_NUM] = {0x0, 0x0, 0x0, 0x0};
        uint32_t DEC_SCL = DEC_SCL_DFLT; // Default scaler
        uint8_t DEC_SHF = DEC_SHF_DFLT;  // Default left shift bits
        uint8_t dec_fac = cfg->dec_factor;
        //        dec_fac = 1; // Users can set dec_fac = 1, if do not use IIR filters
        uint8_t cascd_sos_ind = 0;
        if (dec_fac == 2)
        {
                for (cascd_sos_ind = 0; cascd_sos_ind < CASCD_SOS_NUM; cascd_sos_ind++)
                {
                        DEC_A1[cascd_sos_ind] = DEC_2_A1[cascd_sos_ind];
                        DEC_A2[cascd_sos_ind] = DEC_2_A2[cascd_sos_ind];
                        DEC_B1[cascd_sos_ind] = DEC_2_B1[cascd_sos_ind];
                        DEC_S1[cascd_sos_ind] = DEC_2_S1[cascd_sos_ind];
                        DEC_SCL = DEC_2_SCL;
                        DEC_SHF = DEC_2_SHF;
                }
        }
        else if (dec_fac == 3)
        {
                for (cascd_sos_ind = 0; cascd_sos_ind < CASCD_SOS_NUM; cascd_sos_ind++)
                {
                        DEC_A1[cascd_sos_ind] = DEC_3_A1[cascd_sos_ind];
                        DEC_A2[cascd_sos_ind] = DEC_3_A2[cascd_sos_ind];
                        DEC_B1[cascd_sos_ind] = DEC_3_B1[cascd_sos_ind];
                        DEC_S1[cascd_sos_ind] = DEC_3_S1[cascd_sos_ind];
                        DEC_SCL = DEC_3_SCL;
                        DEC_SHF = DEC_3_SHF;
                }
        }
        else if (dec_fac > 3 && dec_fac <= 7)
        {
                for (cascd_sos_ind = 0; cascd_sos_ind < CASCD_SOS_NUM; cascd_sos_ind++)
                {
                        DEC_A1[cascd_sos_ind] = DEC_4_A1[cascd_sos_ind];
                        DEC_A2[cascd_sos_ind] = DEC_4_A2[cascd_sos_ind];
                        DEC_B1[cascd_sos_ind] = DEC_4_B1[cascd_sos_ind];
                        DEC_S1[cascd_sos_ind] = DEC_4_S1[cascd_sos_ind];
                        DEC_SCL = DEC_4_SCL;
                        DEC_SHF = DEC_4_SHF;
                }
        }
        else if (dec_fac > 7)
        {
                for (cascd_sos_ind = 0; cascd_sos_ind < CASCD_SOS_NUM; cascd_sos_ind++)
                {
                        DEC_A1[cascd_sos_ind] = DEC_8_A1[cascd_sos_ind];
                        DEC_A2[cascd_sos_ind] = DEC_8_A2[cascd_sos_ind];
                        DEC_B1[cascd_sos_ind] = DEC_8_B1[cascd_sos_ind];
                        DEC_S1[cascd_sos_ind] = DEC_8_S1[cascd_sos_ind];
                        DEC_SCL = DEC_8_SCL;
                        DEC_SHF = DEC_8_SHF;
                }
        }
        else
        {
                /* Doing nothing */
        }

        BB_WRITE_REG(SAM_SINKER, SAM_SINKER_FFT); /* direct or buffer */
        BB_WRITE_REG(SAM_F_0_S1, DEC_S1[0]);
        BB_WRITE_REG(SAM_F_0_B1, DEC_B1[0]);
        BB_WRITE_REG(SAM_F_0_A1, DEC_A1[0]);
        BB_WRITE_REG(SAM_F_0_A2, DEC_A2[0]);
        BB_WRITE_REG(SAM_F_1_S1, DEC_S1[1]);
        BB_WRITE_REG(SAM_F_1_B1, DEC_B1[1]);
        BB_WRITE_REG(SAM_F_1_A1, DEC_A1[1]);
        BB_WRITE_REG(SAM_F_1_A2, DEC_A2[1]);
        BB_WRITE_REG(SAM_F_2_S1, DEC_S1[2]);
        BB_WRITE_REG(SAM_F_2_B1, DEC_B1[2]);
        BB_WRITE_REG(SAM_F_2_A1, DEC_A1[2]);
        BB_WRITE_REG(SAM_F_2_A2, DEC_A2[2]);
        BB_WRITE_REG(SAM_F_3_S1, DEC_S1[3]);
        BB_WRITE_REG(SAM_F_3_B1, DEC_B1[3]);
        BB_WRITE_REG(SAM_F_3_A1, DEC_A1[3]);
        BB_WRITE_REG(SAM_F_3_A2, DEC_A2[3]);
        BB_WRITE_REG(SAM_FNL_SHF, DEC_SHF);
        BB_WRITE_REG(SAM_FNL_SCL, DEC_SCL);
        BB_WRITE_REG(SAM_FORCE, 0);
        BB_WRITE_REG(SAM_DBG_SRC, SAM_DBG_SRC_BF); /* before or after down sampling */
        BB_WRITE_REG(SAM_SIZE_DBG_BGN, samp_skip - 3);
        BB_WRITE_REG(SAM_SIZE_DBG_END, samp_skip + samp_used - 3);
        //BB_WRITE_REG(SAM_FRMT_ADC    , BB_REG_SAM_FRMT_ADC_4IN4); // No such a register for ALPS_FM
        old = baseband_switch_mem_access(BB_MEM_ACT_WIN);

        // fft range window init
        gen_window(cfg->rng_win, samp_used, cfg->rng_win_params[0], cfg->rng_win_params[1], cfg->rng_win_params[2]);
        int i = 0;
        uint16_t rng_fft_offset = (frame_type_id) * (1 << (SYS_SIZE_RNG_WD - 1));
        /* store whole range window */
        /* samp_used   --> constrained by 2n (even, refer to RTL) */
        /* samp_used/2 --> two coes in a memory entry(address space) */
        for (i = 0; i < samp_used / 2; i++)
        {
                uint32_t coe0 = get_win_coeff(2 * i);
                uint32_t coe1 = get_win_coeff(2 * i + 1);
                baseband_write_mem_table(rng_fft_offset * 2, (coe1 << 16) | coe0); //two coes in a word
                rng_fft_offset = rng_fft_offset + 1;
        }
        baseband_switch_mem_access(old);
}

void baseband_spk_init(BasebandInitializationParams_t *cfg)
{
        BB_WRITE_REG(SAM_SPK_RM_EN, cfg->spk_en & 0x1);

        if (!cfg->spk_en)
                return;

        BB_WRITE_REG(SAM_SPK_CFM_SIZE, cfg->spk_buf_len & 0xf);
        BB_WRITE_REG(SAM_SPK_SET_ZERO, cfg->spk_set_zero & 0x1);
        BB_WRITE_REG(SAM_SPK_OVER_NUM, cfg->spk_ovr_num & 0xf);
        BB_WRITE_REG(SAM_SPK_THRES_DBL, cfg->spk_thres_dbl & 0x1);
        BB_WRITE_REG(SAM_SPK_MIN_MAX_SEL, cfg->spk_min_max_sel & 0x1);
}

void baseband_fft_init(BasebandInitializationParams_t *cfg, uint8_t frame_type_id)
{
        BB_WRITE_REG(FFT_SHFT_RNG, cfg->rng_fft_scalar);
        BB_WRITE_REG(FFT_SHFT_VEL, cfg->vel_fft_scalar);
        BB_WRITE_REG(FFT_DBPM_ENA, !((cfg->com_params->anti_velamb_en) || VEL_DEAMB_MF_EN || CUSTOM_VEL_DEAMB_MF || (cfg->com_params->ddm_en)));
        BB_WRITE_REG(FFT_DBPM_DIR, 0); // positive compensation
        BB_WRITE_REG(FFT_NO_WIN, 0);
        BB_WRITE_REG(FFT_NVE_MODE, cfg->fft_nve_bypass);

        // velcocity fft window init
        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_WIN);
        uint32_t chip_used = cfg->com_params->nchirp / cfg->nvarray;
        uint32_t chip_buf = chip_used > cfg->vel_nfft ? cfg->vel_nfft : chip_used;
        float win_factor = gen_window(cfg->vel_win, chip_buf, cfg->vel_win_params[0], cfg->vel_win_params[1], cfg->vel_win_params[2]);
        int i = 0;
        uint16_t vel_fft_offset = frame_type_id * (1 << (SYS_SIZE_VEL_WD - 2));
        /*  chip_buf          --> constrained by 2n (even, refer to RTL) */
        /*  chip_buf/2        --> store half size of window coefficients since the window symemtric*/
        /*  chip_buf/2 + 1    --> nchirps/2 may be a odd value, so add 1 for next "/2" */
        /* (chip_buf/2 + 1)/2 --> two coes in a memory entry(address space) */
        for (i = 0; i < (chip_buf / 2 + 1) / 2; i++)
        {
                uint32_t coe0 = get_win_coeff(2 * i);
                uint32_t coe1 = get_win_coeff(2 * i + 1);
                baseband_write_mem_table(vel_fft_offset * 2 + 1, (coe1 << 16) | coe0); //two coes in a word
                vel_fft_offset = vel_fft_offset + 1;
        }
        baseband_switch_mem_access(old);
        float nve_scl_0_fl = 1.0 / chip_buf;
        float nve_scl_1_fl = win_factor / (cfg->nvarray * MAX_NUM_RX);
        float nve_default_value = cfg->fft_nve_default_value;
        uint32_t nve_scl_0_fx = float_to_fl(nve_scl_0_fl, 6, 0, false, 3, false);
        uint32_t nve_scl_1_fx = float_to_fl(nve_scl_1_fl, 6, 0, false, 3, false);
        BB_WRITE_REG(FFT_NVE_SCL_0, nve_scl_0_fx);
        BB_WRITE_REG(FFT_NVE_SFT, cfg->fft_nve_shift);
        BB_WRITE_REG(FFT_ZER_DPL_ENB, cfg->zero_doppler_cancel);
        BB_WRITE_REG(FFT_NVE_SCL_1, nve_scl_1_fx);
        BB_WRITE_REG(FFT_NVE_CH_MSK, cfg->fft_nve_ch_mask);
        BB_WRITE_REG(FFT_NVE_DFT_VALUE, float_to_fl(nve_default_value, 15, 1, false, 5, false));
}

void baseband_interference_init(BasebandInitializationParams_t *cfg)
{
        uint32_t ant_int_en = cfg->com_params->freq_hopping_on ? 1 : 0;
        ant_int_en |= cfg->com_params->chirp_shifting_on ? 2 : 0;
        ant_int_en |= cfg->com_params->phase_scramble_on ? 4 : 0;
        BB_WRITE_REG(FFT_DINT_ENA, ant_int_en);

        /* phase scrambling is compensated in FFT */
        BB_WRITE_REG(SAM_DINT_ENA, 0);

        BB_WRITE_REG(FFT_DINT_DAT_FH, cfg->com_params->freq_hopping_init_state);
        BB_WRITE_REG(FFT_DINT_MSK_FH, cfg->com_params->freq_hopping_tap);

        BB_WRITE_REG(FFT_DINT_DAT_CS, cfg->com_params->chirp_shifting_init_state);
        BB_WRITE_REG(FFT_DINT_MSK_CS, cfg->com_params->chirp_shifting_init_tap);

        BB_WRITE_REG(FFT_DINT_DAT_PS, cfg->com_params->phase_scramble_init_state);
        BB_WRITE_REG(FFT_DINT_MSK_PS, cfg->com_params->phase_scramble_tap);
        BB_WRITE_REG(SAM_DINT_DAT, cfg->com_params->phase_scramble_init_state);
        BB_WRITE_REG(SAM_DINT_MSK, cfg->com_params->phase_scramble_tap);

        uint32_t dec_factor = cfg->dec_factor;
        float Tu = cfg->com_params->fmcw_chirp_rampup;          /* us */
        uint32_t Fs = cfg->com_params->adc_freq;                /* MHz */
        float B = cfg->com_params->fmcw_bandwidth;  /* MHz */
        float fh_deltaf = cfg->com_params->freq_hopping_deltaf; /* MHz */
        float cs_delay = cfg->com_params->chirp_shifting_delay; /* us */
        uint32_t rng_nfft = cfg->rng_nfft;

        complex_t fh_cpx_f;
        complex_t cs_cpx_f;

        if (cfg->com_params->freq_hopping_on)
        {
                fh_cpx_f.r = cos(2 * M_PI * Fs / dec_factor * fh_deltaf * Tu / B / rng_nfft);
                fh_cpx_f.i = -sin(2 * M_PI * Fs / dec_factor * fh_deltaf * Tu / B / rng_nfft);

                BB_WRITE_REG(FFT_DINT_COE_FH, complex_to_cfx(&fh_cpx_f, 14, 1, true));
        }

        if (cfg->com_params->chirp_shifting_on)
        {
                cs_cpx_f.r = cos(2 * M_PI * Fs / dec_factor * cs_delay / rng_nfft);
                cs_cpx_f.i = sin(2 * M_PI * Fs / dec_factor * cs_delay / rng_nfft);
                BB_WRITE_REG(FFT_DINT_COE_CS, complex_to_cfx(&cs_cpx_f, 14, 1, true));
        }

        if (cfg->com_params->freq_hopping_on && cfg->com_params->chirp_shifting_on)
        {
                complex_t fc_cpx_f;
                cmult(&fh_cpx_f, &cs_cpx_f, &fc_cpx_f);
                BB_WRITE_REG(FFT_DINT_COE_FC, complex_to_cfx(&fc_cpx_f, 14, 1, true));
        }

        if (cfg->com_params->phase_scramble_on)
        {
                complex_t ps_cpx_f;
                for (int logic_idx = 0; logic_idx < cfg->nvarray; logic_idx++)
                {

                        for (int phy_idx = 0; phy_idx < MAX_NUM_TX; phy_idx++)
                        {
                                if (cfg->com_params->tx_groups[phy_idx] == (1 << (4 * logic_idx)))
                                {
                                        ps_cpx_f.r = cos(cfg->com_params->phase_scramble_comp[phy_idx] * DEG2RAD) * (cfg->com_params->phase_scramble_comp_amplitude[phy_idx]);
                                        ps_cpx_f.i = sin(cfg->com_params->phase_scramble_comp[phy_idx] * DEG2RAD) * (cfg->com_params->phase_scramble_comp_amplitude[phy_idx]);

                                        if (cfg->com_params->anti_velamb_en)
                                        {
                                                if (logic_idx == 0)
                                                {
                                                        baseband_write_reg((uint32_t)(((uint32_t *)BB_REG_FFT_DINT_COE_PS_0)),
                                                                           complex_to_cfx(&ps_cpx_f, 14, 1, true));
                                                        baseband_write_reg((uint32_t)(((uint32_t *)BB_REG_FFT_DINT_COE_PS_0) + 1),
                                                                           complex_to_cfx(&ps_cpx_f, 14, 1, true));
                                                }
                                                else
                                                {
                                                        baseband_write_reg((uint32_t)(((uint32_t *)BB_REG_FFT_DINT_COE_PS_0) + logic_idx + 1),
                                                                           complex_to_cfx(&ps_cpx_f, 14, 1, true));
                                                }
                                        }
                                        else
                                        {
                                                baseband_write_reg((uint32_t)(((uint32_t *)BB_REG_FFT_DINT_COE_PS_0) + logic_idx),
                                                                   complex_to_cfx(&ps_cpx_f, 14, 1, true));
                                        }
                                }
                        }
                }
        }
        /* configure the way of resetting xor-chain state*/
        baseband_interference_mode_set(cfg);
}

void baseband_velamb_cd_init(BasebandInitializationParams_t *cfg, BasebandSystemParams_t *sys_param, uint8_t frame_type_id)
{
        uint32_t dec_factor = cfg->dec_factor;
        float Tr = cfg->com_params->fmcw_chirp_period; 				/* us */
        float Tu = cfg->com_params->fmcw_chirp_rampup; 				/* us */
        uint32_t Fs = cfg->com_params->adc_freq;       				/* MHz */
        float B = cfg->com_params->fmcw_bandwidth;      /* MHz */
        float delay = cfg->com_params->anti_velamb_delay;
        int32_t q_min = cfg->anti_velamb_qmin;
        uint32_t nvarray = cfg->nvarray;
        uint32_t nfft1 = cfg->rng_nfft;
        uint32_t nfft2 = cfg->vel_nfft;
        float fc = cfg->com_params->fmcw_startfreq;

        uint32_t samp_used = ceil((cfg->adc_sample_end - cfg->adc_sample_start) * cfg->com_params->adc_freq) / cfg->dec_factor;
        samp_used = (samp_used % 2 == 0) ? samp_used : (samp_used + 1);
        samp_used = samp_used > cfg->rng_nfft ? cfg->rng_nfft : samp_used;

        uint32_t Td = (uint32_t)round((nvarray + 1) * Tr + delay);
        uint32_t q_num = MIN(32, Td / compute_gcd((uint32_t)round(Tr + delay), Td));

        if (cfg->com_params->anti_velamb_en)
        {
                /* enabling velamb-CD mode */
                BB_WRITE_REG(SYS_TYPE_FMCW, 1);

                /* compensate MIMO phases using q calculated by velamb-CD */
                BB_WRITE_REG(DOA_DAMB_TYP, 1);
        }
        else if (VEL_DEAMB_MF_EN || CUSTOM_VEL_DEAMB_MF)
        {
                delay = 0;                                                          // used when configure SAM_DAMB_PRD
                Td = (uint32_t)round(nvarray * Tr);                                 // used when configure DOA_DAMB_FRDTRA and DOA_DAMB_FDTR
                q_num = MIN(32, Td / compute_gcd((uint32_t)round(Tr + delay), Td)); // Is it useful for multi-frame vel_deamb?
                /* disabling velamb-CD mode */
                BB_WRITE_REG(SYS_TYPE_FMCW, 0);
                /* choose CPU calculated q to compensate MIMO phases */
                BB_WRITE_REG(DOA_DAMB_TYP, 2);
        }
        else
        {
                delay = 0;
                /* disabling velamb-CD mode */
                BB_WRITE_REG(SYS_TYPE_FMCW, 0);

                /* bypass compensate MIMO phases */
                BB_WRITE_REG(DOA_DAMB_TYP, 0);
        }

        /* samples of extra chirp plus delay */
        BB_WRITE_REG(SAM_DAMB_PRD, (uint32_t)round((Tr + delay) * Fs) - 1);

        /* q number */
        BB_WRITE_REG(DOA_DAMB_IDX_LEN, q_num - 1);

        /* minimum q */
        BB_WRITE_REG(DOA_DAMB_IDX_BGN, q_min);

        /* frd*(Tr+a) & fd*Tr */
        float frd_tr_a = 2.0 * (Tr + delay) / Td / nfft2;
        float frd_tr = TWLUTSZ * Tr / (Td * nfft2);

        n2d_info[frame_type_id].Tr_2_PI_Over_TDN = 2.0 * M_PI * Tr / (Td * nfft2);
        n2d_info[frame_type_id].ph_comp_flg = ((cfg->com_params->anti_velamb_en) || VEL_DEAMB_MF_EN || CUSTOM_VEL_DEAMB_MF || (cfg->com_params->ddm_en));

        BB_WRITE_REG(DOA_DAMB_FRDTRA, float_to_fx(frd_tr_a, 15, -2, false));
        BB_WRITE_REG(DOA_DAMB_FDTR, float_to_fx(frd_tr, 16, 4, false));

        uint32_t n_half = samp_used >> 1;

        float vel_comp = B * n_half * nfft2 / (fc * Tu * Fs / dec_factor * 1000); /* 1000: unit adjust */
        BB_WRITE_REG(DOA_DAMB_VELCOM, float_to_fx(vel_comp, 16, 1, false));

        if (cfg->high_vel_comp_en)
        {
                if (cfg->high_vel_comp_method == 1)
                {
                        /* high velocity compensation by user defined value */
                        sys_param->vel_comp = cfg->vel_comp_usr;
                        sys_param->rng_comp = cfg->rng_comp_usr;
                }
                else
                {
                        /* compensate automatically */
                        sys_param->vel_comp = vel_comp;
                        uint32_t win2sz = cfg->com_params->nchirp / cfg->nvarray;
                        win2sz = win2sz < nfft2 ? win2sz : nfft2;
                        sys_param->rng_comp = B * (win2sz >> 1) * nfft1 / (fc * Tu * Fs / dec_factor * 1000); /* 1000: unit adjust */
                }
        }
        else
        {
                sys_param->vel_comp = 0;
                sys_param->rng_comp = 0;
        }

        /* Anchor Phase setting */
        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_ANC);

        uint32_t bank_idx = frame_type_id;
        uint32_t bank_offset = bank_idx * 32; /* maximum 32 Qs, and each is stored in 4 Bytes memery */

        for (int idx = 0; idx < q_num; idx++)
        {
                int q = q_min + idx;

                float anchor_phase = 2 * q * (Tr + delay) / Td;

                while (anchor_phase >= 1)
                        anchor_phase -= 2;
                while (anchor_phase < -1)
                        anchor_phase += 2;

                baseband_write_mem_table(bank_offset + idx, float_to_fx(anchor_phase, 15, 2, true));
        }

        baseband_switch_mem_access(old);
}

//generate tx_order, each element in tx order represents which varray group are generated by the corresponding tx antenna
//generate new ant pose and ant phase compensation value by ant calib information
static bool gen_tx_order(BasebandInitializationParams_t *cfg, uint32_t patten[], uint32_t n_va, bool bpm_mode, int32_t tx_order[])
{
        bool valid = true;
        int8_t temp = 0;
        uint32_t chirp_tx_mux[MAX_NUM_TX] = {0, 0, 0, 0};                                 /*chirp_tx_mux[i]:which txs transmit in chirp i*/
        int num_tx = 0;                                                                   //save the number of tx antennas which generate the same varray group
        float x_pose[DOA_MAX_NUM_RX], y_pose[DOA_MAX_NUM_RX], phase_comp[DOA_MAX_NUM_RX]; //for computing new ant_pos and ant_comp, if there is more than 1 tx on in a same chirp
        int tx_ant_idx = 0;                                                               //save the tx antenna index which generate a varray group
        //initalize tx_order first
        for (int tx = 0; tx < MAX_NUM_TX; tx++)
        { //tx loop
                tx_order[tx] = 0;
        }
        //then initalize ant info
        for (int vr = 0; vr < MAX_ANT_ARRAY_SIZE; vr++)
        {
                ant_pos[vr].x = vr * 0.5;
                ant_pos[vr].y = 0.0;
                ant_comp_phase[vr] = 0.0;
        }
        if ((patten[0] == 0) && (patten[1] == 0) && (patten[2] == 0) && (patten[3] == 0))
                return true;
        if (bpm_mode == false)
                valid = get_tdm_tx_antenna_in_chirp(patten, n_va - 1, chirp_tx_mux);
        else
                temp = get_bpm_tx_antenna_in_chirp(patten, n_va - 1, chirp_tx_mux);
        if (temp == -1)
                valid = false;
        if (valid == false)
        {
                return valid;
                //EMBARC_PRINTF("gen_tx_order invalid\n");
        }
        //generate tx_order, each element in tx order represents which varray group are generated by the corresponding tx antenna
        for (int c = 0; c < n_va; c++)
        { //chirp loop
                num_tx = 0;
                for (int rx = 0; rx < DOA_MAX_NUM_RX; rx++)
                {
                        x_pose[rx] = 0.0;
                        y_pose[rx] = 0.0;
                        phase_comp[rx] = 0.0;
                }
                for (int tx = 0; tx < MAX_NUM_TX; tx++)
                { //tx loop
                        if ((chirp_tx_mux[c] >> tx) & 0x1)
                        {
                                tx_ant_idx = tx;
                                break;
                        }
                }
                for (int tx = 0; tx < MAX_NUM_TX; tx++)
                { //tx loop
                        if ((chirp_tx_mux[c] >> tx) & 0x1)
                        {
                                num_tx++;
                                for (int rx = 0; rx < DOA_MAX_NUM_RX; rx++)
                                {
                                        x_pose[rx] = cfg->ant_pos[tx * DOA_MAX_NUM_RX + rx].x + x_pose[rx];
                                        y_pose[rx] = cfg->ant_pos[tx * DOA_MAX_NUM_RX + rx].y + y_pose[rx];
                                        phase_comp[rx] = cfg->ant_comps[tx * DOA_MAX_NUM_RX + rx] + phase_comp[rx];
                                }
                        }
                }
                if (num_tx > 1)
                { //if this varray group are generated by more than one tx antenna, then update the calib info
                        for (int rx = 0; rx < DOA_MAX_NUM_RX; rx++)
                        {
                                ant_pos[tx_ant_idx * DOA_MAX_NUM_RX + rx].x = x_pose[rx] / num_tx;
                                ant_pos[tx_ant_idx * DOA_MAX_NUM_RX + rx].y = y_pose[rx] / num_tx;
                                ant_comp_phase[tx_ant_idx * DOA_MAX_NUM_RX + rx] = phase_comp[rx] / num_tx;
                        }
                }
                else if (num_tx == 1)
                {
                        for (int rx = 0; rx < DOA_MAX_NUM_RX; rx++)
                        {
                                ant_pos[tx_ant_idx * DOA_MAX_NUM_RX + rx].x = cfg->ant_pos[tx_ant_idx * DOA_MAX_NUM_RX + rx].x;
                                ant_pos[tx_ant_idx * DOA_MAX_NUM_RX + rx].y = cfg->ant_pos[tx_ant_idx * DOA_MAX_NUM_RX + rx].y;
                                ant_comp_phase[tx_ant_idx * DOA_MAX_NUM_RX + rx] = cfg->ant_comps[tx_ant_idx * DOA_MAX_NUM_RX + rx];
                        }
                }
                else
                {
                        /* Doing nothing */
                }
                tx_order[tx_ant_idx] = c + 1; //after updata calib info, now we can think each varray group is generated by one antenna
        }
        return valid;
}

static uint16_t count_total_neighors(uint32_t mask[], uint32_t len)
{
        uint16_t cnt = 0;
        for (int i = 0; i < len; i++)
                for (int j = 0; j < 32; j++)
                        if ((mask[i] >> j) & 0x1)
                                cnt++;
        return cnt;
}

static void tx_group_mux(int32_t *tx_groups, uint32_t n_va, uint8_t ant_idx[])
{
        uint8_t cnt = 0;
        for (int n = 1; n <= n_va; n++)
        {
                for (int t = 0; t < MAX_NUM_TX; t++)
                {
                        if (tx_groups[t] == n)
                        { /*need to figure out which tx generate which rx array*/
                                for (int r = 0; r < DOA_MAX_NUM_RX; r++)
                                        ant_idx[cnt * DOA_MAX_NUM_RX + r] = t * DOA_MAX_NUM_RX + r;
                                cnt = cnt + 1;
                        }
                }
        }
}

static void init_cfar_msk_per_grp(uint32_t *buf, uint16_t *region_cfg)
{
        memset((char *)buf, 0, sizeof(uint32_t) * 4);

        for (int idx = 0; idx < CFAR_MAX_RECWIN_MSK_LEN_PERGRP * CFAR_MAX_RECWIN_MSK_LEN_PERGRP; idx++)
        {
                int region_cfg_idx = idx % CFAR_MAX_RECWIN_MSK_LEN_PERGRP;
                int region_cfg_bit_idx = (CFAR_MAX_RECWIN_MSK_LEN_PERGRP - 1) - idx / CFAR_MAX_RECWIN_MSK_LEN_PERGRP;
                int bit = ((region_cfg[region_cfg_idx]) >> region_cfg_bit_idx) & 0x1;

                int buf_idx = idx / 32;
                int buf_bit_idx = (buf_idx == 3 ? 24 : 31) - idx % 32;

                buf[buf_idx] |= (bit << buf_bit_idx);
        }
}

static void set_prm_from_val(uint32_t **prameter_reg_addr, uint32_t *param_len, uint32_t prm_idx,
                             uint32_t cfg_val, uint32_t grp_idx)
{
        uint32_t val_len = param_len[prm_idx];
        uint32_t *reg_original_addr = prameter_reg_addr[prm_idx];

        uint32_t val_num_per_prm = 32 / val_len;
        int32_t tmp_grp_idx = 7 - grp_idx;

        uint32_t prm_offset = tmp_grp_idx / val_num_per_prm;

        uint32_t *reg_addr = reg_original_addr + prm_offset;

        uint32_t region_msk = (1 << val_len) - 1;
        cfg_val = cfg_val & region_msk;

        tmp_grp_idx = ((tmp_grp_idx + val_num_per_prm) / val_num_per_prm) * val_num_per_prm - 1;
        tmp_grp_idx = 7 - tmp_grp_idx;

        if (tmp_grp_idx < 0)
                tmp_grp_idx = 0;

        uint32_t sh = 0;
        while (tmp_grp_idx < grp_idx)
        {
                sh += val_len;
                tmp_grp_idx++;
        }

        uint32_t tmp_val = *reg_addr;

        tmp_val |= cfg_val << sh;

        *reg_addr = tmp_val;
}

void baseband_cfar_init(BasebandInitializationParams_t *cfg)
{
        BB_WRITE_REG(CFR_SIZE_OBJ, MAX_CFAR_OBJS - 1);

        /* The layout of SYS_MEM_ACT_COE is: CFR_MIMO_CombCoe, DBPM, DoASV */
        /* ::todo check initalization of CFG_CFR_MIMO_ADR CFG_DOA_DBPM_ADR CFG_DOA_GRPx_ADR_COE */
        BB_WRITE_REG(CFR_MIMO_ADR, 0);

        /* CFAR detection backoff */
        BB_WRITE_REG(CFR_BACK_RNG, 1);

        uint32_t nchan = DOA_MAX_NUM_RX * cfg->nvarray;
        gen_tx_order(cfg, cfg->com_params->tx_groups, cfg->nvarray, cfg->com_params->bpm_mode, tx_order);
        /* combination */
        if (cfg->cfar_combine_dirs == 0)
        {
                /* SISO-CFAR */
                BB_WRITE_REG(CFR_TYPE_CMB, 0);
                BB_WRITE_REG(CFR_MIMO_NUM, 1 - 1);
        }
        else
        {
                /* programming mimo-combine */
                uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_COE);

                BB_WRITE_REG(CFR_TYPE_CMB, 1);
                BB_WRITE_REG(CFR_MIMO_NUM, cfg->cfar_combine_dirs - 1);

                gen_window(cfg->cfar_mimo_win, nchan, cfg->cfar_mimo_win_params[0], cfg->cfar_mimo_win_params[1], cfg->cfar_mimo_win_params[2]);
                float win[MAX_ANT_ARRAY_SIZE];
                uint32_t w = 0;

                for (w = 0; w < nchan; w++)
                {
                        win[w] = get_win_coeff_float(w);
                }

                /* for alpsMP, cfar combine coe put ahead of others in SYS_MEM_ACT_COE  */
                /* Bug 656: when velamb feature is enabled, the memory of sv for mimo-cfar should be shifted by 16 bytes. */
                uint32_t vec_base_offset = cfg->com_params->anti_velamb_en ? 0x10 >> 2 : 0;

                uint32_t d = 0;
                uint32_t ch = 0;
                uint8_t ant_idx[MAX_ANT_ARRAY_SIZE] = {0};
                tx_group_mux(tx_order, cfg->nvarray, ant_idx);

                /*re-arrange the window for steering vector's generation*/
                arrange_doa_win(ant_pos, ant_idx, nchan, win, 0);

                for (d = 0; d < cfg->cfar_combine_dirs; d++)
                {
                        complex_t tmp_vec[MAX_ANT_ARRAY_SIZE];
                        gen_steering_vec2(tmp_vec, win, ant_pos, ant_comp_phase, nchan,
                                          cfg->cfar_combine_thetas[d] * DEG2RAD, cfg->cfar_combine_phis[d] * DEG2RAD, 't', cfg->com_params->bpm_mode, true, ant_idx);
                        for (int v = 0; v < cfg->nvarray; v++)
                        {
                                for (ch = 0; ch < DOA_MAX_NUM_RX; ch++)
                                        baseband_write_mem_table(vec_base_offset + d * cfg->nvarray * DOA_MAX_NUM_RX + v * DOA_MAX_NUM_RX + ch, complex_to_cfx(&tmp_vec[v * DOA_MAX_NUM_RX + ch], 14, 1, true));
                        }
                }

                baseband_switch_mem_access(old);
        }

        /* RDM Region Partition */

        /* range */
        for (int idx = 0; idx < sizeof(cfg->cfar_region_sep_rng) / sizeof(cfg->cfar_region_sep_rng[0]); idx++)
        {
                baseband_write_reg((uint32_t)(((uint32_t *)BB_REG_CFR_PRT_RNG_00) + idx),
                                   cfg->cfar_region_sep_rng[idx]);
        }

        /* velocity */
        for (int idx = 0; idx < sizeof(cfg->cfar_region_sep_vel) / sizeof(cfg->cfar_region_sep_vel[0]); idx++)
        {
                baseband_write_reg((uint32_t)(((uint32_t *)BB_REG_CFR_PRT_VEL_00) + idx),
                                   cfg->cfar_region_sep_vel[idx]);
        }

        /* Sliding Window, it is NOT region dependent */
        BB_WRITE_REG(CFR_CS_ENA, cfg->cfar_sliding_win);

        if (cfg->cfar_sliding_win == 0)
        {
                uint32_t buf[4];
                uint32_t *Msk_reg_addr[CFAR_MAX_GRP_NUM] = {(uint32_t *)BB_REG_CFR_MSK_DS_70, (uint32_t *)BB_REG_CFR_MSK_DS_60,
                                                            (uint32_t *)BB_REG_CFR_MSK_DS_50, (uint32_t *)BB_REG_CFR_MSK_DS_40,
                                                            (uint32_t *)BB_REG_CFR_MSK_DS_30, (uint32_t *)BB_REG_CFR_MSK_DS_20,
                                                            (uint32_t *)BB_REG_CFR_MSK_DS_10, (uint32_t *)BB_REG_CFR_MSK_DS_00};

                /* retangle window */
                for (uint32_t grp_idx = 0; grp_idx < CFAR_MAX_GRP_NUM; grp_idx++)
                {

                        /* initializing buf[4] */
                        uint16_t *cfg_grp_p = &(cfg->cfar_recwin_msk[grp_idx * CFAR_MAX_RECWIN_MSK_LEN_PERGRP]);

                        /* initializing retangle window mask */
                        init_cfar_msk_per_grp(buf, cfg_grp_p);

                        for (uint32_t reg_idx = 0; reg_idx < 4; reg_idx++)
                        {
                                baseband_write_reg((uint32_t)(Msk_reg_addr[grp_idx] + reg_idx), buf[reg_idx]);
                        }
                }

                /* decimating scheme for retangle window */
                BB_WRITE_REG(CFR_TYP_DS, cfg->cfar_recwin_decimate);
        }
        else
        {
                BB_WRITE_REG(CFR_CS_SIZ_RNG, cfg->cfar_crswin_rng_size);
                BB_WRITE_REG(CFR_CS_SIZ_VEL, cfg->cfar_crswin_vel_size);
                BB_WRITE_REG(CFR_CS_SKP_RNG, cfg->cfar_crswin_rng_skip);
                BB_WRITE_REG(CFR_CS_SKP_VEL, cfg->cfar_crswin_vel_skip);
        }

        /* Peak Detector enabler */
        BB_WRITE_REG(CFR_PK_ENB, cfg->cfar_pk_en);

        /* Peak Detector mask */
        uint32_t *pk_msk_offset = (uint32_t *)BB_REG_CFR_MSK_PK_00;
        uint32_t *pk_thr_offset = (uint32_t *)BB_REG_CFR_PK_THR_0;

        for (uint32_t grp_idx = 0; grp_idx < CFAR_MAX_GRP_NUM; grp_idx++)
        {
                /* make it more readable */
                uint32_t msk = 0;
                int32_t win1_sz = cfg->cfar_pk_win_size1[CFAR_MAX_GRP_NUM - grp_idx - 1];
                int32_t win2_sz = cfg->cfar_pk_win_size2[CFAR_MAX_GRP_NUM - grp_idx - 1];

                win1_sz = (win1_sz == 1) ? 0 : (win1_sz == 3) ? 1
                                           : (win1_sz == 5)   ? 2
                                                              : -1;
                win2_sz = (win2_sz == 1) ? 0 : (win2_sz == 3) ? 1
                                           : (win2_sz == 5)   ? 2
                                                              : -1;

                if (win1_sz >= 0 && win2_sz >= 0)
                        msk = CFAR_pk_msk[win1_sz][win2_sz];

                baseband_write_reg((uint32_t)(pk_msk_offset + grp_idx), msk);
                baseband_write_reg((uint32_t)(pk_thr_offset + grp_idx), cfg->cfar_pk_threshold[CFAR_MAX_GRP_NUM - grp_idx - 1]);
        }

        /* CFAR Parameter Space Management */
        BB_WRITE_REG(CFR_TYP_AL, cfg->cfar_region_algo_type);

        uint32_t *prameter_reg_addr[] = {(uint32_t *)BB_REG_CFR_PRM_0_0, (uint32_t *)BB_REG_CFR_PRM_1_0, (uint32_t *)BB_REG_CFR_PRM_2_0,
                                         (uint32_t *)BB_REG_CFR_PRM_3_0, (uint32_t *)BB_REG_CFR_PRM_4_0, (uint32_t *)BB_REG_CFR_PRM_5_0,
                                         (uint32_t *)BB_REG_CFR_PRM_6_0, (uint32_t *)BB_REG_CFR_PRM_7_0};

        /* clear all CFAR parameter registers, so that it can be |= without impact of previous value */
        memset((void *)BB_REG_CFR_PRM_0_0, 0, (BB_REG_CFR_MSK_DS_00 - BB_REG_CFR_PRM_0_0));

        /* parameter length in bits */
        uint32_t param_len[8] = {7, 10, 10, 12, 7, 7, 12, 1};

        for (uint32_t grp_idx = 0; grp_idx < CFAR_MAX_GRP_NUM; grp_idx++)
        {

                uint32_t cfar_type = (cfg->cfar_region_algo_type >> (grp_idx * 2)) & 0x3;

                uint32_t prm_idx;

                switch (cfar_type)
                {
                case 0: /* CA-CFAR */
                        prm_idx = 0;
                        /* Number of maximum to remove before averaging */
                        uint32_t ca_n = cfg->cfar_ca_n[grp_idx];
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, ca_n, grp_idx);

                        /* Scalar of CUT power before comparing */
                        prm_idx++;
                        uint32_t ca_alpha = float_to_fl(1.0 / cfg->cfar_ca_alpha[grp_idx], 6, 6, false, 4, false);
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, ca_alpha, grp_idx);

                        break;
                case 1: /* OS-CFAR */
                        prm_idx = 0;
                        /* The preselected rank threshold */
                        uint32_t tdec = cfg->cfar_os_tdec[grp_idx];
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, tdec, grp_idx);

                        /* The scalar of CUT's power before ranking */
                        prm_idx++;
                        uint32_t os_alpha = float_to_fl(1.0 / cfg->cfar_os_alpha[grp_idx], 6, 6, false, 4, false);
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, os_alpha, grp_idx);

                        /* Rank Selector: The mux control to select the final source of rank threshold */
                        prm_idx++;

                        uint32_t rnk_sel = cfg->cfar_os_rnk_sel[grp_idx];
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, rnk_sel, grp_idx);

                        /* The scalar of related rank */
                        prm_idx++;
                        uint32_t rnk_ratio = float_to_fx(cfg->cfar_os_rnk_ratio[grp_idx], 12, 0, false);
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, rnk_ratio, grp_idx);

                        break;
                case 2: /* SOGO-CFAR */
                        prm_idx = 0;
                        /* The i-th smallest over the averages from the selected sides */
                        uint32_t sogo_i = cfg->cfar_sogo_i[grp_idx];
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, sogo_i, grp_idx);

                        /* The scalar of CUT's power before comparison */
                        prm_idx++;
                        uint32_t sogo_alpha = float_to_fl(1.0 / cfg->cfar_sogo_alpha[grp_idx], 6, 6, false, 4, false);
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, sogo_alpha, grp_idx);

                        /* The mask control to select the sides of the cross window */
                        prm_idx++;
                        uint32_t sogo_mask = cfg->cfar_sogo_mask[grp_idx];
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, sogo_mask, grp_idx);

                        break;

                case 3: /* NR-CFAR */

                        /* Select output from one of three sub-schemes */
                        prm_idx = 0; /* CFG_CFR_PRM_0 */
                        uint32_t nr_scheme_sel = cfg->cfar_nr_scheme_sel[grp_idx];
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, nr_scheme_sel, grp_idx);

                        /* Scalar of power of CUT before comparison or ranking */
                        prm_idx++; /* CFG_CFR_PRM_1 */
                        uint32_t nr_alpha = float_to_fl(1.0 / cfg->cfar_nr_alpha[grp_idx], 6, 6, false, 4, false);
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, nr_alpha, grp_idx);

                        /* Lower bound scalar in construction of set of reference powers */
                        prm_idx++; /* CFG_CFR_PRM_2 */
                        uint32_t nr_beita1 = float_to_fl(cfg->cfar_nr_beta1[grp_idx], 6, 6, false, 4, false);
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, nr_beita1, grp_idx);

                        /* Upper bound scalar in construction of set of reference powers */
                        prm_idx++; /* CFG_CFR_PRM_3 */
                        uint32_t nr_beita2 = float_to_fl(cfg->cfar_nr_beta2[grp_idx], 6, 6, false, 4, false);
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, nr_beita2, grp_idx);

                        /* CFG_CFR_PRM_4 is skipped */

                        /* Pre-programmed rank threshold for Scheme 2 */
                        prm_idx = 5; /* CFG_CFR_PRM_5 */
                        uint32_t nr_tdec = cfg->cfar_nr_tdec[grp_idx];
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, nr_tdec, grp_idx);

                        /* The scalar of related rank */
                        prm_idx++; /* CFG_CFR_PRM_6 */
                        uint32_t nr_rnk_ratio = float_to_fx(cfg->cfar_nr_rnk_ratio[grp_idx], 12, 0, false);
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, nr_rnk_ratio, grp_idx);

                        /* The mux control to select the final source of rank threshold in Scheme 2 */
                        prm_idx++; /* CFG_CFR_PRM_7 */
                        uint32_t nr_rnk_sel = cfg->cfar_nr_rnk_sel[grp_idx];
                        set_prm_from_val(prameter_reg_addr, param_len, prm_idx, nr_rnk_sel, grp_idx);

                        break;
                default:
                        /* something is wrong */
                        break;
                }
        }

        /* 8 region noise level output select register */
        BB_WRITE_REG(CFR_TYP_NOI, cfg->cfar_noise_type);
}

static uint8_t get_sum_idx_from_mux(uint32_t mask, uint8_t *p_rx_idx)
{
        uint8_t cnt = 0;
        for (int i = 0; i < 32; i++)
        {
                p_rx_idx[i] = 0;
                if ((mask >> i) & 0x1)
                {
                        p_rx_idx[cnt] = i;
                        cnt++;
                }
        }
#ifdef CHIP_CASCADE
        for (int i = 0; i < cnt; i++)
        {
                if (p_rx_idx[i] % DOA_MAX_NUM_RX < MAX_NUM_RX) //master chirp data
                        p_rx_idx[i] = (p_rx_idx[i] / DOA_MAX_NUM_RX) * MAX_NUM_RX + p_rx_idx[i] % MAX_NUM_RX;
                else //slave chirp data
                        p_rx_idx[i] = MAX_ANT_ARRAY_SIZE_SC + (p_rx_idx[i] / DOA_MAX_NUM_RX) * MAX_NUM_RX + p_rx_idx[i] % MAX_NUM_RX;
        }
#endif

        return cnt;
}

static void get_reg_from_idx(uint8_t *p_rx_idx, uint32_t reg_value[])
{
        for (int i = 0; i < 8; i++)
                reg_value[i] = (p_rx_idx[4 * i + 0] << (3 * BFM_ANT_IDX_BW)) | (p_rx_idx[4 * i + 1] << (2 * BFM_ANT_IDX_BW)) | (p_rx_idx[4 * i + 2] << BFM_ANT_IDX_BW) | p_rx_idx[4 * i + 3];
}

static uint8_t mux_map_antidx(uint32_t mask, uint32_t reg_value[])
{
        uint8_t cnt = 0;
        uint8_t antidx[32];
        cnt = get_sum_idx_from_mux(mask, antidx);
        get_reg_from_idx(antidx, reg_value);
        return cnt;
}

static int group_map_tx_ant(int group, int32_t *tx_groups)
{
        for (int t = 0; t < MAX_NUM_TX; t++)
        {
                if (tx_groups[t] == group)
                        return t;
        }
        return 0;
}

static uint8_t gen_ant_idx(uint32_t fft_mux, int32_t *tx_groups, uint8_t rx_ant_idx[])
{
        uint8_t cnt = 0; /*output the size of steering vector*/
        for (int i = 0; i < 32; i++)
        {
                if ((fft_mux >> i) & 0x1)
                {
                        int grp = i / DOA_MAX_NUM_RX + 1;
                        int idx = i % DOA_MAX_NUM_RX;
                        int tx_ant_idx = group_map_tx_ant(grp, tx_groups);
                        rx_ant_idx[cnt] = tx_ant_idx * DOA_MAX_NUM_RX + idx;
                        cnt = cnt + 1;
                }
        }
        return cnt;
}

static void baseband_dbf_init(BasebandInitializationParams_t *cfg, int cfg_grp_num)
{
        uint32_t dbf_grp_base_adr = BB_REG_DOA_GRP0_MOD_SCH;
        uint32_t dbf_grp_adr = 0;
        //configure dbf mode
        int mode = 0;
        if (cfg->bfm_iter_search)
        {
                if (cfg->bfm_mode == 0)
                        mode = 3; //itr peaks -> IPM
                else
                        mode = 4; //itr omp   -> OMP
        }
        else
        {
                if (cfg->bfm_mode == 0)
                        mode = 1; //raw peaks  -> Non IPM
                else
                        mode = 2; //raw omp
        }
        for (int g = 0; g < cfg_grp_num; g++)
        {
                dbf_grp_adr = dbf_grp_base_adr + g * GRP_ADDRESS_OFFSET;
                /*write REG MOD_SCH*/
                baseband_write_reg(dbf_grp_adr, mode);
                dbf_grp_adr = dbf_grp_adr + 0x14; //Fix me later magic number 0x14
                /*write REG SIZ_STP_PKS_CRS*/
                baseband_write_reg(dbf_grp_adr, cfg->bfm_raw_search_step - 1);
                dbf_grp_adr = dbf_grp_adr + 0x4;
                /*write REG SIZ_RNG_PKS_RFD*/
                baseband_write_reg(dbf_grp_adr, cfg->bfm_fine_search_range);
                dbf_grp_adr = dbf_grp_adr + 0x2C; //Fix me later magic number 0x2C
                /*write REG THR_SNR_0*/
                baseband_write_reg(dbf_grp_adr, cfg->bfm_snr_thres[0]);
                dbf_grp_adr = dbf_grp_adr + 0x4;
                /*write REG THR_SNR_1*/
                baseband_write_reg(dbf_grp_adr, cfg->bfm_snr_thres[1]);
                dbf_grp_adr = dbf_grp_adr + 0x4;
                /*write REG SCL_POW*/
                baseband_write_reg(dbf_grp_adr, float_to_fl(cfg->bfm_peak_scalar[g], 6, 6, false, 4, false));
                dbf_grp_adr = dbf_grp_adr + 0x4;
                /*write REG SCL_NOI_y*/
                for (int j = 0; j < 3; j++)
                {
                        baseband_write_reg(dbf_grp_adr, float_to_fl(cfg->bfm_noise_level_scalar[j], 6, 6, false, 4, false));
                        dbf_grp_adr = dbf_grp_adr + 0x4;
                }
        }
}

static void baseband_dml_init(BasebandInitializationParams_t *cfg, int cfg_grp_num)
{
        uint32_t dml_adr_offset;

        for (int g = 0; g < cfg_grp_num; g++)
        {
                uint32_t dml_grp_base_adr = (g == 0) ? BB_REG_DML_GRP0_SV_STP : BB_REG_DML_GRP1_SV_STP;

                /* BB_REG_DML_GRPx_SV_STP */
                dml_adr_offset = 0;
                baseband_write_reg(dml_adr_offset + dml_grp_base_adr, cfg->dml_2dsch_step[g]);

                /* BB_REG_DML_GRPx_SV_START */
                dml_adr_offset = BB_REG_DML_GRP0_SV_START - BB_REG_DML_GRP0_SV_STP;
                baseband_write_reg(dml_adr_offset + dml_grp_base_adr, cfg->dml_2dsch_start[g]);

                /* BB_REG_DML_GRPx_SV_END */
                dml_adr_offset = BB_REG_DML_GRP0_SV_END - BB_REG_DML_GRP0_SV_STP;
                baseband_write_reg(dml_adr_offset + dml_grp_base_adr, cfg->dml_2dsch_end[g]);

                /* BB_REG_DML_GRPx_DC_COE_0 ~  BB_REG_DML_GRPx_DC_COE_4 */
                dml_adr_offset = BB_REG_DML_GRP0_DC_COE_0 - BB_REG_DML_GRP0_SV_STP;

                for (int idx = 0; idx < 5; idx++)
                {
                        /* there are 5 coefficients for a group */
                        baseband_write_reg(dml_adr_offset + dml_grp_base_adr,
                                           float_to_fx(cfg->dml_respwr_coef[g * 5 + idx], 14, 1, true));

                        /* each coefficient occupies 4 bytes memory */
                        dml_adr_offset += 4;
                }

                /* BB_REG_DML_GRPx_EXTRA_EN */
                dml_adr_offset = BB_REG_DML_GRP0_EXTRA_EN - BB_REG_DML_GRP0_SV_STP;

                if (cfg->doa_max_obj_per_bin[g] == 2)
                        /* RTL restriction: if the output object number for dml is 2, BB_REG_DML_GRP0_EXTRA_EN should be set as 0 */
                        baseband_write_reg(dml_adr_offset + dml_grp_base_adr, 0);
                else
                        baseband_write_reg(dml_adr_offset + dml_grp_base_adr, cfg->dml_extra_1d_en[g]);

                /* BB_REG_DML_GRPx_DC_COE_2_EN */
                dml_adr_offset = BB_REG_DML_GRP0_DC_COE_2_EN - BB_REG_DML_GRP0_SV_STP;
                baseband_write_reg(dml_adr_offset + dml_grp_base_adr, cfg->dml_p1p2_en[g]);
        }

        BB_WRITE_REG(DML_MEM_BASE_ADR, (cfg->doa_npoint[0] - 1));
}

static void gen_angle(int g_idx, uint32_t d, double delta_az, double delta_ev, double az_begin, double ev_begin, double *p_a, double *p_b)
{
        if (g_idx == 0)
        {
                *p_a = d * delta_az + az_begin;
                *p_b = 0;
        }
        else if (g_idx == 1)
        {
                *p_a = 0;
                *p_b = d * delta_ev + ev_begin;
        }
        else
        {
                *p_a = d * delta_az + az_begin;
                *p_b = d * delta_ev + ev_begin;
        }
}

static void write_steering_vec(int num_chan, uint32_t addr_coe, uint32_t d, complex_t *vec)
{
        int ch = 0;
        int mem_size = ((num_chan + 3) / 4) * 4;
        int loop = 0;
        //write memory for group which comb_num is inter size of MAX_NUM_RX part
        for (loop = 0; loop < (num_chan / MAX_NUM_RX); loop++)
        {
                for (ch = 0; ch < MAX_NUM_RX; ch++)
                {
                        baseband_write_mem_table(addr_coe + d * mem_size + loop * MAX_NUM_RX + ch, complex_to_cfx(&vec[loop * MAX_NUM_RX + ch], 14, 1, true));
                }
        }
        //write memory for group which comb_num is less than MAX_NUM_RX part
        if (num_chan % MAX_NUM_RX)
        {
                for (ch = 0; ch < (num_chan % MAX_NUM_RX); ch++)
                {
                        baseband_write_mem_table(addr_coe + d * mem_size + loop * MAX_NUM_RX + ch, complex_to_cfx(&vec[loop * MAX_NUM_RX + ch], 14, 1, true));
                }
                int j = 0;
                for (int dummy = (num_chan % MAX_NUM_RX); dummy < MAX_NUM_RX; dummy++)
                {
                        baseband_write_mem_table(addr_coe + d * mem_size + num_chan + j, 0x0);
                        j++;
                }
        }
}

static void gen_siginfo_c2d(BasebandInitializationParams_t *cfg, uint8_t rx_num[], uint8_t *p_size_v, uint32_t grp0_rx_idx[], uint8_t rx_idx_ele[])
{
        uint8_t rx_idx_combined_2d_g0[32] = {0};         //for configuration of DATA_IDX_GRP0 of combined mode
        uint8_t rx_idx_combined_2d_g1[4] = {0, 0, 0, 0}; //for computing steering vector of group 1 of combined mode
        uint8_t n = 0;                                   //recordgen_si total channel number stored in DATA_IDX_GRP0
        uint8_t rxnum_v = 0;                             //record signal number in vertical direction

        for (int g = 0; g < 4; g++)
        {
                uint8_t rx_idx_tmp[32];
                rx_num[g] = get_sum_idx_from_mux(cfg->combined_doa_fft_mux[g], rx_idx_tmp);
                if (rx_num[g] > 0)
                {
                        rx_idx_combined_2d_g1[rxnum_v] = rx_idx_tmp[0];
#ifdef CHIP_CASCADE
                        /*rx_idx_tmp is arranged as master 0~15|slave 16~31,we think it is virtual array and it was used to configure REG DATA_IDX*/
                        /*rx_idx_combined_2d_g1 is arranged as master 0~3|slave4~7|we think it is physical array and it was used to computing steering vector of elevated direction*/
                        uint8_t phy_rx = rx_idx_tmp[0] < MAX_ANT_ARRAY_SIZE_SC ? rx_idx_tmp[0] : (rx_idx_tmp[0] - MAX_ANT_ARRAY_SIZE_SC);
                        uint8_t offset = rx_idx_tmp[0] < MAX_ANT_ARRAY_SIZE_SC ? 0 : MAX_NUM_RX;
                        rx_idx_combined_2d_g1[rxnum_v] = (phy_rx / MAX_NUM_RX) * DOA_MAX_NUM_RX + phy_rx % MAX_NUM_RX + offset;
#endif
                        rxnum_v = rxnum_v + 1;
                        for (int j = 0; j < rx_num[g]; j++)
                        {
                                rx_idx_combined_2d_g0[n] = rx_idx_tmp[j];
                                n = n + 1;
                        }
                }
                else
                {
                        break;
                }
        }

        get_reg_from_idx(rx_idx_combined_2d_g0, grp0_rx_idx);
        cfg->doa_fft_mux[0] = cfg->combined_doa_fft_mux[0];
        uint32_t mux_v = 0;
        for (int i = 0; i < rxnum_v; i++)
        {
                mux_v = mux_v + (1 << rx_idx_combined_2d_g1[i]);
                rx_idx_ele[i] = rx_idx_combined_2d_g1[i]; /*for computing steering vector of group 1(elevated direction) of combined mode */
        }
        cfg->doa_fft_mux[1] = mux_v;
        *p_size_v = rxnum_v;
}

static void gen_dml_dk(BasebandInitializationParams_t *cfg, uint8_t frame_type_id, int g, double delta, double delta2, antenna_pos_t *ant_pos, uint8_t sv_size, uint8_t *ant_idx)
{
        if (cfg->doa_method != 2)
        {
                /* not DML, just return to save CPU time. */
                return;
        }

        /* initial d,k */
        uint32_t bank_idx = frame_type_id;
        uint32_t bank_offset = bank_idx * 0x800;
        uint32_t dk_offset = g == 0 ? 0 : 2 * (cfg->doa_npoint[0] - 1);

        int32_t dnum = cfg->doa_npoint[g] - 1;
        double sin_step = (g == 0) ? delta : delta2;

        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_DML);

        for (int idx = 0; idx < dnum; idx++)
        {
                complex_t d;
                d.r = 0.0;
                d.i = 0.0;

                for (int ch = 0; ch < sv_size; ch++)
                {
                        float tw = (g == 0 ? ant_pos[ant_idx[ch]].x : ant_pos[ant_idx[ch]].y) * sin_step * (idx + 1);
                        complex_t tmp = expj(-2 * M_PI * tw);
                        cadd(&tmp, &d, &d);
                }

                uint64_t u64_data_d = complex_to_cfl_dwords(&d, 18, 1, true, 5, true);

                uint32_t step = cfg->dml_2dsch_step[g];
                float g_i = 1.0;

                if (idx < (step << 1))
                {
                        /* suppressing cost function of two angles closing each other */
                        g_i = DMLKINITCOEF + (1 - DMLKINITCOEF) * idx / (step << 1);
                }

                float k = g_i / (sv_size * sv_size - mag_sqr(&d));

                /* the format of k is FLR(18,1,U,5,S) */
                uint64_t u64_data_k = float_to_fl(k, 18, 1, false, 5, true);
                /* K occupies lower 18+5 bits (length of K is 18+5=23 bits)
                   of 8 bytes memory while the upper part is occupied by d */
                uint64_t dk = u64_data_k | u64_data_d << (18 + 5);

                uint64_t msk = ((uint64_t)1 << 32) - 1;
                /* the lower 32bits of dk */
                baseband_write_mem_table(bank_offset + dk_offset + idx * 2, (uint32_t)(dk & msk));
                /* the upper 32bits of dk */
                baseband_write_mem_table(bank_offset + dk_offset + idx * 2 + 1, (uint32_t)((dk >> 32) & msk));
        }
        baseband_switch_mem_access(old);
}

static void write_steering_vec_from_flash(uint32_t *sv, uint32_t offset, int doa_ant_num, int angle_num, uint32_t write_addr_coe)
{
        int mem_size = ((doa_ant_num + 3) / 4) * 4; //number of words per angle
        for (int d = 0; d < angle_num; d++)
        {
                for (int cnt = 0; cnt < mem_size; cnt++)
                {
                        baseband_write_mem_table(write_addr_coe + d * mem_size + cnt, sv[offset + d * mem_size + cnt]);
                }
        }
}


void baseband_doa_init(BasebandInitializationParams_t *cfg, uint8_t frame_type_id)
{
#ifdef CHIP_CASCADE
        BB_WRITE_REG(DOA_MODE_RUN, 1);
#else
        BB_WRITE_REG(DOA_MODE_RUN, 0);
#endif
        BB_WRITE_REG(DOA_MODE_GRP, cfg->doa_mode);
        int cfg_grp_num = 0;
        if (cfg->doa_mode == 2)
        { //single shot mode
                BB_WRITE_REG(DOA_NUMB_GRP, cfg->bfm_group_idx);
                BB_WRITE_REG(DOA_GRP0_TYP_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP1_TYP_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP2_TYP_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP0_MOD_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP1_MOD_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP2_MOD_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP0_SIZ_STP_PKS_CRS, 0x0);
                BB_WRITE_REG(DOA_GRP1_SIZ_STP_PKS_CRS, 0x0);
                BB_WRITE_REG(DOA_GRP2_SIZ_STP_PKS_CRS, 0x0);
                BB_WRITE_REG(DOA_GRP0_NUM_SCH, 0xB);
                BB_WRITE_REG(DOA_GRP1_NUM_SCH, 0xB);
                BB_WRITE_REG(DOA_GRP2_NUM_SCH, 0xB);
                cfg_grp_num = 3;
        }
        else if (cfg->doa_mode == 1)
        { //combined mode
                BB_WRITE_REG(DOA_NUMB_GRP, 1);
                BB_WRITE_REG(DOA_GRP0_TYP_SCH, cfg->doa_method);
                BB_WRITE_REG(DOA_GRP1_TYP_SCH, cfg->doa_method);
                cfg_grp_num = 2; //indicate the number of group registers need to be configured
                if (cfg->doa_method == 0)
                {
                        baseband_dbf_init(cfg, cfg_grp_num); //config dbf related registers
                }
                else
                {
                        baseband_dml_init(cfg, cfg_grp_num); //config dml related registers
                        BB_WRITE_REG(DOA_GRP0_SIZ_STP_PKS_CRS, 0x0);
                        BB_WRITE_REG(DOA_GRP1_SIZ_STP_PKS_CRS, 0x0);
                        BB_WRITE_REG(DOA_GRP0_MOD_SCH, 0x1); /* a dml restriction caused by RTL reusing implementation of dbf */
                        BB_WRITE_REG(DOA_GRP1_MOD_SCH, 0x1); /* a dml restriction caused by RTL reusing implementation of dbf */
                }
                BB_WRITE_REG(DOA_GRP0_NUM_SCH, cfg->doa_max_obj_per_bin[0] - 1);
                BB_WRITE_REG(DOA_GRP1_NUM_SCH, cfg->doa_max_obj_per_bin[1] - 1);
        }
        else
        {
                BB_WRITE_REG(DOA_NUMB_GRP, cfg->doa_num_groups - 1);
                if (cfg->doa_num_groups == 1)
                {
                        BB_WRITE_REG(DOA_GRP0_TYP_SCH, cfg->doa_method);
                        BB_WRITE_REG(DOA_GRP0_NUM_SCH, cfg->doa_max_obj_per_bin[0] - 1);
                        cfg_grp_num = 1;
                        if (cfg->doa_method == 0)
                        {
                                baseband_dbf_init(cfg, cfg_grp_num); //config dbf related registers
                        }
                        else
                        {
                                baseband_dml_init(cfg, cfg_grp_num); //config dml related registers
                                BB_WRITE_REG(DOA_GRP0_SIZ_STP_PKS_CRS, 0x0);
                                BB_WRITE_REG(DOA_GRP0_MOD_SCH, 0x1); /* a dml restriction caused by RTL reusing implementation of dbf */
                        }
                }
                else
                {
                        BB_WRITE_REG(DOA_GRP0_TYP_SCH, 0x0);
                        BB_WRITE_REG(DOA_GRP1_TYP_SCH, 0x0);
                        BB_WRITE_REG(DOA_GRP2_TYP_SCH, 0x0);
                        cfg_grp_num = cfg->doa_num_groups;
                        baseband_dbf_init(cfg, cfg_grp_num);
                        BB_WRITE_REG(DOA_GRP0_NUM_SCH, cfg->doa_max_obj_per_bin[0] - 1);
                        BB_WRITE_REG(DOA_GRP1_NUM_SCH, cfg->doa_max_obj_per_bin[1] - 1);
                        BB_WRITE_REG(DOA_GRP2_NUM_SCH, cfg->doa_max_obj_per_bin[2] - 1);
                }
        }

        //below for configuring rx idx and steering vector related registers
        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_COE);
        uint32_t bfm_vec_base_offset = 0;
        double bfm_az_begin, bfm_az_end, delta;
        double bfm_ev_begin, bfm_ev_end, delta2;
        bfm_vec_base_offset = DEBPM_COE + CFAR_MIMO_COMB_COE;
        uint8_t doa_comb_num[MAX_BFM_GROUP_NUM] = {0, 0, 0};
        uint32_t doa_grp_addr_coe[MAX_BFM_GROUP_NUM] = {0, 0, 0};
        uint32_t doa_grp_rx_idx[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //4 rx ant idxs stored in an IDX
        uint32_t doa_grp_base_adr = BB_REG_DOA_GRP0_ADR_COE;
        uint32_t doa_grp_adr = 0;
        uint8_t combined_2d_comb_num[4] = {1, 1, 1, 1}; //for REG_SIZ_CMB is in [0,31]
        uint8_t comb_size_v = 0;
        uint32_t combined_2d_grp_rx_idx[8] = {0}, *p, c2d_addr_coe[3] = {0};
        uint8_t combined_2d_ele_rx_idx[4] = {0}; //for generation of steering vector in elevated direction
        doa_grp_addr_coe[0] = (frame_type_id == 0 ? bfm_vec_base_offset : sv_bk[frame_type_id]);

        if (cfg->doa_mode == 1)
        {
                gen_siginfo_c2d(cfg, combined_2d_comb_num, &comb_size_v, combined_2d_grp_rx_idx, combined_2d_ele_rx_idx);
                //update doa_comb_num
                doa_comb_num[0] = combined_2d_comb_num[0];
                doa_comb_num[1] = comb_size_v;
                doa_comb_num[2] = 0;

                n2d_info[frame_type_id].azi_line_num = comb_size_v;
                for (int g = 0; g < comb_size_v; g++)
                {
                        n2d_info[frame_type_id].azi_vector_size[g] = get_sum_idx_from_mux(cfg->combined_doa_fft_mux[g],
                                                                           n2d_info[frame_type_id].azi_logic_channel_idx[g]);
                }
        }

        for (int g = 0; g < cfg_grp_num; g++)
        {
                if (cfg->doa_mode != 1)
                {
                        /* antenna number for current group */
                        doa_comb_num[g] = mux_map_antidx(cfg->doa_fft_mux[g], doa_grp_rx_idx);
                }

                if (g >= 1)
                        doa_grp_addr_coe[g] = doa_grp_addr_coe[g - 1] + cfg->doa_npoint[g - 1] * ((doa_comb_num[g - 1] + 3) / BB_ADDRESS_UNIT) * BB_ADDRESS_UNIT;
                doa_grp_adr = doa_grp_base_adr + g * GRP_ADDRESS_OFFSET;
                /* write REG COE*/
                baseband_write_reg(doa_grp_adr, doa_grp_addr_coe[g] / BB_ADDRESS_UNIT);
                doa_grp_adr = doa_grp_adr + 0x8;
                /* write REG SIZ_RNG_PKS_CRS*/
                uint32_t step = baseband_read_reg(doa_grp_adr + 0x4);
                baseband_write_reg(doa_grp_adr, cfg->doa_npoint[g] / (step + 1) - 1);
                doa_grp_adr = doa_grp_adr + 0xC;
                /* write REG CMB*/
                baseband_write_reg(doa_grp_adr, doa_comb_num[g] - 1); //
                doa_grp_adr = doa_grp_adr + 0x4;
                /* write REG DATA_IDX*/
                if (cfg->doa_mode == 1 && g == 0)
                        p = combined_2d_grp_rx_idx;
                else
                        p = doa_grp_rx_idx;
                for (int i = 0; i < 8; i++)
                {
                        baseband_write_reg(doa_grp_adr, p[i]);
                        doa_grp_adr = doa_grp_adr + 0x4;
                }
        }

        /* below configure C2D totol 6 registers  */
        c2d_addr_coe[0] = doa_grp_addr_coe[cfg_grp_num - 1] + cfg->doa_npoint[cfg_grp_num - 1] * ((doa_comb_num[cfg_grp_num - 1] + 3) / BB_ADDRESS_UNIT) * BB_ADDRESS_UNIT;
        c2d_addr_coe[1] = c2d_addr_coe[0] + cfg->doa_npoint[0] * ((combined_2d_comb_num[1] + 3) / BB_ADDRESS_UNIT) * BB_ADDRESS_UNIT;
        c2d_addr_coe[2] = c2d_addr_coe[1] + cfg->doa_npoint[0] * ((combined_2d_comb_num[2] + 3) / BB_ADDRESS_UNIT) * BB_ADDRESS_UNIT;

        BB_WRITE_REG(DOA_C2D1_ADR_COE, c2d_addr_coe[0] / BB_ADDRESS_UNIT);
        BB_WRITE_REG(DOA_C2D2_ADR_COE, c2d_addr_coe[1] / BB_ADDRESS_UNIT);
        BB_WRITE_REG(DOA_C2D3_ADR_COE, c2d_addr_coe[2] / BB_ADDRESS_UNIT);
        BB_WRITE_REG(DOA_C2D1_SIZ_CMB, combined_2d_comb_num[1] - 1); /* combined_2d_comb_num: combined mode */
        BB_WRITE_REG(DOA_C2D2_SIZ_CMB, combined_2d_comb_num[2] - 1);
        BB_WRITE_REG(DOA_C2D3_SIZ_CMB, combined_2d_comb_num[3] - 1);

        if (cfg->doa_samp_space == 't')
        {
                bfm_az_begin = cfg->bfm_az_left * DEG2RAD;
                bfm_az_end = cfg->bfm_az_right * DEG2RAD;
                bfm_ev_begin = cfg->bfm_ev_down * DEG2RAD;
                bfm_ev_end = cfg->bfm_ev_up * DEG2RAD;
        }
        else
        {
                bfm_az_begin = sin(cfg->bfm_az_left * DEG2RAD);
                bfm_az_end = sin(cfg->bfm_az_right * DEG2RAD);
                bfm_ev_begin = sin(cfg->bfm_ev_down * DEG2RAD);
                bfm_ev_end = sin(cfg->bfm_ev_up * DEG2RAD);
        }
        delta = (bfm_az_end - bfm_az_begin) / cfg->doa_npoint[0];
        delta2 = (bfm_ev_end - bfm_ev_begin) / cfg->doa_npoint[1];
        float win[MAX_ANT_ARRAY_SIZE];
        uint32_t w = 0;
        complex_t tmp_vec[MAX_ANT_ARRAY_SIZE];
        double pm1, pm2;
        uint8_t ant_idx[MAX_ANT_ARRAY_SIZE] = {0}; /*virtualized by TDM va=4 tx_group=[1 16 256 4096], the related physical rx ant idx*/
        uint8_t sv_size = 0;
        uint32_t d = 0;
        bool phase_comp_in_sv = true; /*the flag for if needed to compensate antcalib phase in steering vector*/
        for (int g = 0; g < cfg_grp_num; g++)
        {
                if (cfg->sv_read_from_flash == false)
                {
                        gen_window(cfg->doa_win, doa_comb_num[g], cfg->doa_win_params[0], cfg->doa_win_params[1], cfg->doa_win_params[2]);
                        for (w = 0; w < doa_comb_num[g]; w++)
                                win[w] = get_win_coeff_float(w);

                        sv_size = gen_ant_idx(cfg->doa_fft_mux[g], tx_order, ant_idx);
                        if ((cfg->doa_mode == 1) && (g == 1))
                        {
                                phase_comp_in_sv = false;
                                for (int ele_rx = 0; ele_rx < sv_size; ele_rx++)
                                {
                                        ant_idx[ele_rx] = combined_2d_ele_rx_idx[ele_rx];
                                        //mapping logic Rx to physical Rx
                                        int t_grp = ant_idx[ele_rx] / DOA_MAX_NUM_RX + 1;
                                        int t_idx = ant_idx[ele_rx] % DOA_MAX_NUM_RX;
                                        int tx_ant_idx = group_map_tx_ant(t_grp, tx_order);
                                        ant_idx[ele_rx] = tx_ant_idx * DOA_MAX_NUM_RX + t_idx;
                                        n2d_info[frame_type_id].vertical_dis[ele_rx] = ant_pos[ant_idx[ele_rx]].y;
                                }
                        }
                        arrange_doa_win(ant_pos, ant_idx, sv_size, win, g);

                        for (d = 0; d < cfg->doa_npoint[g]; d++)
                        {
                                gen_angle(g, d, delta, delta2, bfm_az_begin, bfm_ev_begin, &pm1, &pm2);
                                gen_steering_vec2(tmp_vec, win, ant_pos, ant_comp_phase, sv_size,
                                                  pm1, pm2, cfg->doa_samp_space, cfg->com_params->bpm_mode, phase_comp_in_sv, ant_idx);
                                write_steering_vec(doa_comb_num[g], doa_grp_addr_coe[g], d, tmp_vec);
                        } //end d

                        gen_dml_dk(cfg, frame_type_id, g, delta, delta2, ant_pos, sv_size, ant_idx);
                }
                else
                {
                        uint32_t offset = 0;
                        if (g == 1)
                        {
                                for (int cc = 0; cc < comb_size_v; cc++)
                                {
                                        offset += ((combined_2d_comb_num[cc] + 3) / 4) * 4 * cfg->doa_npoint[0];
                                }
                        }
                        write_steering_vec_from_flash(cfg->sv_ptr, offset, doa_comb_num[g], cfg->doa_npoint[g], doa_grp_addr_coe[g]);
                }
        } //end g
        if (frame_type_id + 1 < 4)
                sv_bk[frame_type_id + 1] = doa_grp_addr_coe[cfg_grp_num - 1] + cfg->doa_npoint[cfg_grp_num - 1] * ((doa_comb_num[cfg_grp_num - 1] + 3) / 4) * 4;
        if (cfg->doa_mode == 1)
        {
                n2d_info[frame_type_id].azi_sv_add[0] = doa_grp_addr_coe[0];
				uint32_t offset = 0;
                for (int g = 1; g < comb_size_v; g++)
                { //g = 0 is group 0 and it has been configured
                        int ch_num = combined_2d_comb_num[g];
                        int sv_num = cfg->doa_npoint[0];
                        if (cfg->sv_read_from_flash == false)
                        {
                                gen_window(cfg->doa_win, ch_num, cfg->doa_win_params[0], cfg->doa_win_params[1], cfg->doa_win_params[2]);
                                for (w = 0; w < ch_num; w++)
                                        win[w] = get_win_coeff_float(w);
                                sv_size = gen_ant_idx(cfg->combined_doa_fft_mux[g], tx_order, ant_idx);
                                arrange_doa_win(ant_pos, ant_idx, sv_size, win, g);
                                for (d = 0; d < sv_num; d++)
                                {
                                        gen_angle(0, d, delta, delta2, bfm_az_begin, bfm_ev_begin, &pm1, &pm2);
                                        gen_steering_vec2(tmp_vec, win, ant_pos, ant_comp_phase, sv_size,
                                                          pm1, pm2, cfg->doa_samp_space, cfg->com_params->bpm_mode, true, ant_idx);
                                        write_steering_vec(ch_num, c2d_addr_coe[g - 1], d, tmp_vec);
                                }
                        }
                        else
                        {
                                offset += ((combined_2d_comb_num[g - 1] + 3) / 4) * 4 * cfg->doa_npoint[0];
                                write_steering_vec_from_flash(cfg->sv_ptr, offset, ch_num, sv_num, c2d_addr_coe[g - 1]);
                        }
                        n2d_info[frame_type_id].azi_sv_add[g] = c2d_addr_coe[g - 1];
                }
                int end_idx = comb_size_v - 1;
                if (frame_type_id + 1 < 4)
                        sv_bk[frame_type_id + 1] = c2d_addr_coe[end_idx - 1] + cfg->doa_npoint[0] * ((combined_2d_comb_num[end_idx] + 3) / 4) * 4;
        }
        baseband_switch_mem_access(old);
}

/* FIXME, baseband_amb_init is not ready */
void baseband_amb_init(BasebandInitializationParams_t *cfg)
{
        if (!cfg->de_vel_amb)
                return;

        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_MAC);
        unsigned ridx = 0;
        unsigned offset = 0;
        unsigned val = 0;
        /* need initial memory */
        /* char vel_ambiguity[] = {}; */
        for (int r = 0; r < (cfg->rng_nfft) / 2; r++)
        {
                for (int v = 0; v < (cfg->vel_nfft); v = v + SYS_DATA_COM_WD / FFT_DATA_AMB_WD)
                {
                        val = 0;
                        for (int i = 0; i < SYS_DATA_COM_WD / FFT_DATA_AMB_WD; i++)
                        {
                                if (i == 0)
                                {
                                        /* need initial memory */
                                        /* val = vel_ambiguity[ridx]; */
                                }
                                else
                                {
                                        val <<= FFT_DATA_AMB_WD;
                                        /* need initial memory */
                                        /* val |= vel_ambiguity[ridx]; */
                                }
                                ridx++;
                        }
                        baseband_write_mem_table(offset++, val);
                }
        }
        baseband_switch_mem_access(old);
}

/* FIXME if demode matrix isn't standard hardmard */
void baseband_debpm_init(BasebandInitializationParams_t *cfg, uint8_t frame_type_id)
{
        BB_WRITE_REG(DOA_DBPM_ENA, cfg->com_params->bpm_mode);

        complex_t tmp_matrix[MAX_ANT_ARRAY_SIZE_SC];
        uint32_t offset = CFAR_MIMO_COMB_COE + DEBPM_COE / MAX_FRAME_TYPE * frame_type_id; //space for mimo combine coe
        BB_WRITE_REG(DOA_DBPM_ADR, offset / BB_ADDRESS_UNIT);                              //bb read as 4 words,cpu write as word

        if (!cfg->com_params->bpm_mode)
                return;

        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_COE);

        int size_bpm = cfg->nvarray;
        if (size_bpm == 2)
        {
                tmp_matrix[0].r = 1;
                tmp_matrix[0].i = 0;
                tmp_matrix[1] = tmp_matrix[0];
                tmp_matrix[2].r = 0;
                tmp_matrix[2].i = 0;
                tmp_matrix[3] = tmp_matrix[2];
                tmp_matrix[4].r = 1;
                tmp_matrix[4].i = 0;
                tmp_matrix[5].r = -1;
                tmp_matrix[5].i = 0;
                tmp_matrix[6] = tmp_matrix[2];
                tmp_matrix[7] = tmp_matrix[2];
        }
        else if (size_bpm == 4)
        {
                tmp_matrix[0].r = 1;
                tmp_matrix[0].i = 0;
                tmp_matrix[1] = tmp_matrix[0];
                tmp_matrix[2] = tmp_matrix[1];
                tmp_matrix[3] = tmp_matrix[2];
                tmp_matrix[4] = tmp_matrix[0];
                tmp_matrix[6] = tmp_matrix[4];
                tmp_matrix[8] = tmp_matrix[0];
                tmp_matrix[9] = tmp_matrix[0];
                tmp_matrix[12] = tmp_matrix[0];
                tmp_matrix[15] = tmp_matrix[0];
                tmp_matrix[5].r = -1;
                tmp_matrix[5].i = 0;
                tmp_matrix[7] = tmp_matrix[5];
                tmp_matrix[10] = tmp_matrix[5];
                tmp_matrix[11] = tmp_matrix[5];
                tmp_matrix[13] = tmp_matrix[5];
                tmp_matrix[14] = tmp_matrix[5];
        }
        else
        {
                /* Doing nothing */
        }

        for (int v = 0; v < size_bpm; v++)
        {
                for (int ch = 0; ch < MAX_NUM_RX; ch++)
                        baseband_write_mem_table(offset++, complex_to_cfx(&tmp_matrix[v * MAX_NUM_RX + ch], 14, 1, true));
        }

        baseband_switch_mem_access(old);
}

// TODO: Plase make sure that adc_ldo, lo, and rx is turned on !!!!
void baseband_dc_calib_calc(BasebandInitializationParams_t *cfg, int16_t dc_offset[ANT_NUM]) /* leakage enalbe */
{
        uint32_t offset_vbar = 0;
        int32_t adc_data_sum = 0;

        //        /*TODO: can be removed if power-saving init is after dc_calibration */
        //        /* open radio lo and rx, no tx */
        //#if INTER_FRAME_POWER_SAVE == 1
        //        fmcw_radio_adc_ldo_on(radio, true);
        //        fmcw_radio_lo_on(radio, true);
        //        fmcw_radio_rx_on(radio, true);
        //#endif

        /* save old config */
        uint32_t old_sam_sinker = BB_READ_REG(SAM_SINKER); /* fft direct or adc buffer */
        uint32_t old_sam_force = BB_READ_REG(SAM_FORCE);
        uint32_t old_size_vel_fft = BB_READ_REG(SYS_SIZE_VEL_FFT);
        uint32_t old_size_vel_buf = BB_READ_REG(SYS_SIZE_VEL_BUF);

        /* set new config */
        BB_WRITE_REG(SYS_SIZE_VEL_FFT, 0);        /* only one chirp */
        BB_WRITE_REG(SYS_SIZE_VEL_BUF, 0);        /* only one chirp */
        BB_WRITE_REG(SAM_FORCE, 1);               /* force start */
        BB_WRITE_REG(SAM_SINKER, SAM_SINKER_BUF); /* adc buffer */

        /* start baseband */
        uint16_t bb_status_en = SYS_ENA(SAM, true);
        baseband_hw_start_with_params(bb_status_en, BB_IRQ_ENABLE_SAM_DONE); /* IRQ SAM_DONE, for power saving handler*/

        uint32_t i = 300000 * 10;
        while (i)
        {
                i--;
        }
        /* wait done */
        // while (baseband_hw_is_running() == false)
        //         ;
        // while (baseband_hw_is_running() == true)
        //         ;
        /* restore old config */
        BB_WRITE_REG(SAM_SINKER, old_sam_sinker); /* fft direct or adc buffer */
        BB_WRITE_REG(SAM_FORCE, old_sam_force);
        BB_WRITE_REG(SYS_SIZE_VEL_FFT, old_size_vel_fft);
        BB_WRITE_REG(SYS_SIZE_VEL_BUF, old_size_vel_buf);

        /* VBAR */
        uint32_t offset_vbar_cfg = cfg->rng_nfft / 2; /* memory data are 32 bits, but adc data are 16 bits, so divided-2 */
        uint16_t adc_buf_num = BB_READ_REG(SYS_SIZE_RNG_BUF) + 1;
        int fft_size = BB_READ_REG(SYS_SIZE_RNG_FFT) + 1;

        /* When buf size is greater than fft size, zero is padded, so a smaller value is used for DC */
        adc_buf_num = adc_buf_num < fft_size ? adc_buf_num : fft_size;

        //TODO: use print macro
        //EMBARC_PRINTF("offset_vbar_cfg = %d, adc_buf_num = %d\n", offset_vbar_cfg, adc_buf_num);


        baseband_switch_mem_access(BB_MEM_ACT_BUF);

        for (uint8_t ch_index = 0; ch_index < ANT_NUM; ch_index++)
        {
                for (int32_t i = 0; i < adc_buf_num / 2; i++)
                {
                        int32_t mem_data = baseband_read_mem_table(offset_vbar + i);
                        int16_t mem_data_high = (int16_t)(REG_H16(mem_data)); /* mask the high 16 bits*/
                        int16_t mem_data_low = (int16_t)(REG_L16(mem_data));  /* mask the low 16 bits*/
                        adc_data_sum = adc_data_sum + mem_data_high + mem_data_low;
                }
                dc_offset[ch_index] = adc_data_sum / adc_buf_num;
                ////xprintf("adc_data_sum:0x%x\n", adc_data_sum);
                ////xprintf("adc_buf_num:0x%x\n", adc_buf_num);
                ////xprintf("offset:0x%x\n", adc_data_sum / adc_buf_num);
                offset_vbar = offset_vbar + offset_vbar_cfg;
                adc_data_sum = 0;
        }
}

void baseband_interference_mode_set(BasebandInitializationParams_t *cfg)
{
        uint8_t val_mod = 0;
        uint8_t val_set = 0;

        if (cfg->com_params->freq_hopping_on & 0x2)
        {
                val_mod |= 0x1;
                val_set |= 0x1;
        }

        if (cfg->com_params->chirp_shifting_on & 0x2)
        {
                val_mod |= (0x1 << 1);
                val_set |= (0x1 << 1);
        }

        if (cfg->com_params->phase_scramble_on & 0x2)
        {
                val_mod |= (0x1 << 2);
                val_set |= (0x1 << 2);
        }

        BB_WRITE_REG(FFT_DINT_MOD, val_mod);
        BB_WRITE_REG(FFT_DINT_SET, val_set);
}

//// TODO: dc_calibration
//static void baseband_dc_calib(BasebandInitializationParams_t* cfg, uint8_t frame_type_id, bool leakage_ena, bool dc_calib_print_ena) /* leakage enable */
//{
//        //uint8_t fdb_bnk_act = baseband_read_reg(BB_REG_FDB_SYS_BNK_ACT);
//        if (dc_calib_print_ena) {
//                //EMBARC_PRINTF("fdb_bnk_act = %d \n", fdb_bnk_act);
//                //EMBARC_PRINTF("frame_type_id = %d \n", frame_type_id); // Check bb_hw frame type
//        }
//
//        /* dc calib starts */
//        int ch_index = 0;
//        float dc_power_tx_on[MAX_NUM_RX];
//        float dc_power_tx_off[MAX_NUM_RX];
//
//        int16_t dc_offset_tx_off[ANT_NUM]={};
//        int16_t dc_offset_tx_on[ANT_NUM]={};
//
//        /* save old config */
//        baseband_dc_calib_status_save();    /* save baseband configurations */
//        radio_dc_calib_status_save(frame_type_id);    /* save radio configurations */
//
//        /* set new config */
//        BB_WRITE_REG(SAM_DINT_ENA, 0); /* Disable Rx phase de-scramble during DC calibration */
//
//        /* TODO: Can be removed if radio_agc_init is after dc_calibration */
//        //fmcw_radio_agc_enable(frame_type_id, false); /* disable agc mode */
//
//        /* clear old dc value in radio */
//        for (uint8_t ch_index = 0; ch_index < ANT_NUM; ch_index++)
//        {
//                //fmcw_radio_dc_reg_cfg(frame_type_id, ch_index, 0, dc_calib_print_ena); /* initial config, dc_offset = 0 */
//        }
//
//        // TXs enable bits are cleared before, so dc_offset_tx_off does not contain RF leakage
//        baseband_dc_calib_calc(cfg, dc_offset_tx_off, dc_calib_print_ena); /* leakage enalbe */
//
//        /* Re-config dc value in radio*/
//        //fmcw_radio_dc_reg_cfg(frame_type_id, ch_index, dc_offset_tx_off[ch_index], dc_calib_print_ena);
//
//        /* restore old config */
//        baseband_dc_calib_status_restore();    			/* save baseband configurations */
//        //radio_dc_calib_status_restore(frame_type_id);   /* save radio configurations */
//
//        /******************** caculate the leakage *********************/
//        if (leakage_ena == true) {
//                // TXs enable bits are restored, so dc_offset_tx_on contains RF leakage
//                baseband_dc_calib_calc(cfg, dc_offset_tx_on, dc_calib_print_ena); /* leakage enalbe */
//                for (ch_index = 0; ch_index < ANT_NUM; ch_index++) {
//                        dc_power_tx_off[ch_index] = log10(dc_offset_tx_off[ch_index] * dc_offset_tx_off[ch_index]);
//                        dc_power_tx_on[ch_index]  = log10(dc_offset_tx_on[ch_index] * dc_offset_tx_on[ch_index]);
//                }
//        }
//        /******************** the final dc result *********************/
//        if (dc_calib_print_ena == true) {
//                /*if (leakage_ena == true) {
//                        EMBARC_PRINTF("DC leakage CH0-%.3f  CH1-%.3f  CH2-%.3f  CH3-%.3f\n\r",
//                                                dc_power_tx_on[0] - dc_power_tx_off[0],
//                                                dc_power_tx_on[1] - dc_power_tx_off[1],
//                                                dc_power_tx_on[2] - dc_power_tx_off[2],
//                                                dc_power_tx_on[3] - dc_power_tx_off[3]);
//                }
//                else {
//
//                        EMBARC_PRINTF("DC offset CH0:0x%x  CH1:0x%x  CH2:0x%x   CH3:0x%x \n\r",
//                                                dc_offset_tx_off[0], dc_offset_tx_off[1], dc_offset_tx_off[2], dc_offset_tx_off[3]);
//                        EMBARC_PRINTF("DC offset CH0:%d  CH1:%d  CH2:%d   CH3:%d \n\r",
//                                                dc_offset_tx_off[0], dc_offset_tx_off[1], dc_offset_tx_off[2], dc_offset_tx_off[3]);
//
//                }*/
//        }
//
//}

static int16_t dc_offset[ANT_NUM] = {1, 2, 3, 4};
static uint32_t old_sam_dint_ena = 0;
int16_t *get_dc_offset()
{
        return &dc_offset[0];
}

void baseband_dc_calib_set_cfg(uint8_t frame_type_id)
{
        /***********************Phase 1: bb***************************/
        /* Assert and deassert ADC reset and bb_core reset */
        baseband_hw_reset_after_force();

        (void)baseband_frame_interleave_cfg(frame_type_id);
        //uint8_t fdb_bnk_act = baseband_read_reg(BB_REG_FDB_SYS_BNK_ACT);

        //EMBARC_PRINTF("fdb_bnk_act = %d \n", fdb_bnk_act);
        //EMBARC_PRINTF("frame_type_id = %d \n", frame_type_id); // Check bb_hw frame type

        /* save baseband old config */
        old_sam_dint_ena = BB_READ_REG(SAM_DINT_ENA);
        BB_WRITE_REG(SAM_DINT_ENA, 0); /* Disable Rx phase de-scramble during DC calibration */
}

void baseband_dc_calib_cal_dc(BasebandInitializationParams_t *cfg)
{
        /***************************Phase 3: BB*********************************/
        // TXs enable bits are cleared before, so dc_offset_tx_off does not contain RF leakage
        baseband_dc_calib_calc(cfg, dc_offset); /* leakage enalbe */
        ////xprintf("[bb]dc_offset:ch0:0x%x, ch1:0x%x, ch2:0x%x, ch3:0x%x\n", dc_offset[0], dc_offset[1], dc_offset[2], dc_offset[3]);
}

void baseband_dc_calib_rst_cfg(BasebandInitializationParams_t *cfg, bool leakage_ena)
{
        /* restore bb old config */
        BB_WRITE_REG(SAM_DINT_ENA, old_sam_dint_ena);

        //	 /* Calculate leakage */
        //	 if (leakage_ena == true) {
        //			 // TXs enable bits are restored, so dc_offset_tx_on contains RF leakage
        //			 baseband_dc_calib_calc(cfg, dc_offset_tx_on, dc_calib_print_ena); /* leakage enalbe */
        //			 for (int ch_index = 0; ch_index < ANT_NUM; ch_index++) {
        //					 dc_power_tx_off[ch_index] = log10(dc_offset_tx_off[ch_index] * dc_offset_tx_off[ch_index]);
        //					 dc_power_tx_on[ch_index]  = log10(dc_offset_tx_on[ch_index] * dc_offset_tx_on[ch_index]);
        //			 }
        //	 }
        //	 /* Print DC result */
        //	 if (dc_calib_print_ena == true) {
        //			 if (leakage_ena == true) {
        //					 EMBARC_PRINTF("DC leakage CH0-%.3f  CH1-%.3f  CH2-%.3f  CH3-%.3f\n\r",
        //											 dc_power_tx_on[0] - dc_power_tx_off[0],
        //											 dc_power_tx_on[1] - dc_power_tx_off[1],
        //											 dc_power_tx_on[2] - dc_power_tx_off[2],
        //											 dc_power_tx_on[3] - dc_power_tx_off[3]);
        //			 }
        //			 else {
        //
        //					 EMBARC_PRINTF("DC offset CH0:0x%x  CH1:0x%x  CH2:0x%x   CH3:0x%x \n\r",
        //											 dc_offset_tx_off[0], dc_offset_tx_off[1], dc_offset_tx_off[2], dc_offset_tx_off[3]);
        //					 EMBARC_PRINTF("DC offset CH0:%d  CH1:%d  CH2:%d   CH3:%d \n\r",
        //											 dc_offset_tx_off[0], dc_offset_tx_off[1], dc_offset_tx_off[2], dc_offset_tx_off[3]);
        //
        //			 }
        //	 }

        baseband_hw_reset_after_force();
        baseband_interference_mode_set(cfg);
}
//void baseband_dc_calib_init(BasebandInitializationParams_t* cfg, uint8_t frame_type_id, bool leakage_ena, bool dc_calib_print_ena) /* leakage enable */
//{
//    	/***********************Phase 1: bb***************************/
//        /* Assert and deassert ADC reset and bb_core reset */
//        baseband_hw_reset_after_force();
//
//        (void)baseband_frame_interleave_cfg(frame_type_id);
//         uint8_t fdb_bnk_act = baseband_read_reg(BB_REG_FDB_SYS_BNK_ACT);
//         if (dc_calib_print_ena) {
//                 EMBARC_PRINTF("fdb_bnk_act = %d \n", fdb_bnk_act);
//                 EMBARC_PRINTF("frame_type_id = %d \n", frame_type_id); // Check bb_hw frame type
//         }
//
//         /* dc calib starts */
//         int ch_index = 0;
//         float dc_power_tx_on[MAX_NUM_RX];
//         float dc_power_tx_off[MAX_NUM_RX];
//
//         int16_t dc_offset_tx_off[ANT_NUM]={};
//         int16_t dc_offset_tx_on[ANT_NUM]={};
//
//         /* save baseband old config */
//         uint32_t old_sam_dint_ena = BB_READ_REG(SAM_DINT_ENA);
//         BB_WRITE_REG(SAM_DINT_ENA, 0); /* Disable Rx phase de-scramble during DC calibration */
//
//         /****************************Phase 2: radio ***************************/
//         /* save radio old config */
//         radio_dc_calib_status_save(frame_type_id);
//
//         /* TODO: Can be removed if radio_agc_init is after dc_calibration */
//         fmcw_radio_agc_enable(frame_type_id, false); /* disable agc mode */
//
//         /* clear old dc value in radio */
//         for (uint8_t ch_index = 0; ch_index < ANT_NUM; ch_index++)
//         {
//                 fmcw_radio_dc_reg_cfg(frame_type_id, ch_index, 0, dc_calib_print_ena); /* initial config, dc_offset = 0 */
//         }
//
//         /***************************Phase 3: BB*********************************/
//         // TXs enable bits are cleared before, so dc_offset_tx_off does not contain RF leakage
//         baseband_dc_calib_calc(cfg, dc_offset_tx_off, dc_calib_print_ena); /* leakage enalbe */
//
//         /****************************Phase 4: Radio*********************************/
//         /* Re-config dc value in radio*/
//         for (uint8_t ch_index = 0; ch_index < ANT_NUM; ch_index++)
//         {
//        	 fmcw_radio_dc_reg_cfg(frame_type_id, ch_index, dc_offset_tx_off[ch_index], dc_calib_print_ena);
//         }
//         /* restore radio old config */
//         radio_dc_calib_status_restore(frame_type_id);
//
//         /******************************Phase 5: BB***********************************/
//         /* restore bb old config */
//         BB_WRITE_REG(SAM_DINT_ENA, old_sam_dint_ena);
//
//         /* Calculate leakage */
//         if (leakage_ena == true) {
//                 // TXs enable bits are restored, so dc_offset_tx_on contains RF leakage
//                 baseband_dc_calib_calc(cfg, dc_offset_tx_on, dc_calib_print_ena); /* leakage enalbe */
//                 for (ch_index = 0; ch_index < ANT_NUM; ch_index++) {
//                         dc_power_tx_off[ch_index] = log10(dc_offset_tx_off[ch_index] * dc_offset_tx_off[ch_index]);
//                         dc_power_tx_on[ch_index]  = log10(dc_offset_tx_on[ch_index] * dc_offset_tx_on[ch_index]);
//                 }
//         }
//         /* Print DC result */
//         if (dc_calib_print_ena == true) {
//                 if (leakage_ena == true) {
//                         EMBARC_PRINTF("DC leakage CH0-%.3f  CH1-%.3f  CH2-%.3f  CH3-%.3f\n\r",
//                                                 dc_power_tx_on[0] - dc_power_tx_off[0],
//                                                 dc_power_tx_on[1] - dc_power_tx_off[1],
//                                                 dc_power_tx_on[2] - dc_power_tx_off[2],
//                                                 dc_power_tx_on[3] - dc_power_tx_off[3]);
//                 }
//                 else {
//
//                         EMBARC_PRINTF("DC offset CH0:0x%x  CH1:0x%x  CH2:0x%x   CH3:0x%x \n\r",
//                                                 dc_offset_tx_off[0], dc_offset_tx_off[1], dc_offset_tx_off[2], dc_offset_tx_off[3]);
//                         EMBARC_PRINTF("DC offset CH0:%d  CH1:%d  CH2:%d   CH3:%d \n\r",
//                                                 dc_offset_tx_off[0], dc_offset_tx_off[1], dc_offset_tx_off[2], dc_offset_tx_off[3]);
//
//                 }
//         }
//
//        baseband_hw_reset_after_force();
//        baseband_interference_mode_set(cfg);
//}

int32_t baseband_hw_init(BasebandDriverInit_t *bb)
{
        BasebandInitializationParams_t *cfg = bb->cfg;
        uint8_t frame_type_id = bb->frame_type_id;
        BasebandSystemParams_t *sys_param = bb->sys_params;

        //xprintf("/*** Baseband HW init... ***/\n\r");
        baseband_bnk_set(frame_type_id);
#if 0
//#ifdef FUNC_SAFETY
        clear_all_bb_memory();
#endif
        baseband_sys_init(cfg, sys_param);
        //xprintf("/*** sys params programmed! ***/\n\r");

        baseband_agc_init(cfg);
        //xprintf("/*** agc params programmed! ***/\n\r");

        baseband_amb_init(cfg);
        //xprintf("/*** amb params programmed! ***/\n\r");

        baseband_debpm_init(cfg, frame_type_id);
        //xprintf("/*** debpm params programmed! ***/\n\r");

        baseband_spk_init(cfg);
        //xprintf("/*** spk_rmv params programmed! ***/\n\r");

        baseband_sam_init(cfg, frame_type_id);
        //xprintf("/*** sam params programmed! ***/\n\r");
        baseband_fft_init(cfg, frame_type_id);
        //xprintf("/*** fft params programmed! ***/\n\r");

        baseband_interference_init(cfg);
        //xprintf("/*** inteference params programmed! ***/\n\r");

        baseband_velamb_cd_init(cfg, sys_param, frame_type_id);
        //xprintf("/*** velamb params programmed! ***/\n\r");

        switch_to_ddm_config(cfg, frame_type_id);

        baseband_cfar_init(cfg);
        //xprintf("/*** cfar params programmed! ***/\n\r");

        baseband_doa_init(cfg, frame_type_id);
        //xprintf("/*** doa params programmed! ***/\n\r");

        recover_from_ddm_config(cfg, frame_type_id);
        if (frame_type_id == (NUM_FRAME_TYPE - 1)) { /* run one time */

                baseband_shadow_bnk_init(cfg);
                //uart_printfs("/*** shadow bank programmed! ***/\n\r",NULL);

//TODO: dc_calibration
//#ifndef CHIP_CASCADE
        //baseband_dc_calib_init(cfg, frame_type_id, false, false);
        //EMBARC_PRINTF("/*** dc calib done! ***/\n\r");
//#endif //CHIP_CASCADE
                //baseband_interrupt_init();

//TODO: power-save
#if (INTER_FRAME_POWER_SAVE == 1)
				baseband_interframe_power_save_enable(bb_hw, true);
				uart_printfs("/*** interframe powersaving feature is on! ***/\n\r");
#endif //INTER_FRAME_POWER_SAVE

				baseband_frame_interleave_cfg(0); // init config 0 active
        }

        //EMBARC_PRINTF("/*** Baseband HW init done... ***/\n\r");
        return E_OK;
}

void baseband_hw_start_with_params(uint16_t sys_enable, uint8_t sys_irp_en)
{
        /* align bank of frame type */
        uint8_t bnk_act = BB_READ_REG(FDB_SYS_BNK_ACT); /* read back the bank selected in RTl */
        BB_WRITE_REG(SYS_BNK_ACT, bnk_act);             /* match the bank accessed by CPU */
        /* start baseband */
        BB_WRITE_REG(SYS_ENABLE, sys_enable);
        BB_WRITE_REG(SYS_IRQ_ENA, sys_irp_en);
        BB_WRITE_REG(SYS_START, 0x1);
}

void baseband_hw_running_done(uint16_t sys_enable, uint8_t sys_irq_en)
{
        baseband_hw_start_with_params(sys_enable, sys_irq_en);
        while (baseband_hw_is_running() == false)
                ;
        while (baseband_hw_is_running() == true) /* wait done */
                ;
}

void baseband_hw_stop(void)
{
        BB_WRITE_REG(SYS_START, 0x0);
        BB_WRITE_REG(SYS_IRQ_ENA, 0x0);
}

void baseband_combiner_reset(BasebandInitializationParams_t *cfg)
{
        /* An hw bug (Bug847) in DML-Engine leaving SISO-combiner unreset after doing a DoA Estimation */
        if (cfg->doa_method == 2)
        {
                bb_core_reset(1);
                bb_core_reset(0);
        }
}

bool baseband_hw_is_running(void)
{
        if (BB_READ_REG(SYS_STATUS) == 0)
                return false;
        else
                return true;
}

/* read fft result in memory, all the input index should be based on 0 */
uint32_t baseband_hw_get_fft_mem(BasebandInitializationParams_t *cfg, int ant_index, int rng_index, int vel_index, int bpm_index)
{
        uint32_t addr_sel;

        /* TVAR, [virTual][Velocity][Antenna][Range] */
        addr_sel = bpm_index * (cfg->vel_nfft) * MAX_NUM_RX * (cfg->rng_nfft / 2) + vel_index * MAX_NUM_RX * (cfg->rng_nfft / 2) + ant_index * (cfg->rng_nfft / 2) + rng_index;
        return baseband_read_mem_table(addr_sel);
}

static uint32_t baseband_hw_get_fft_mem_abist(int ch_index, int rng_index, uint32_t fft_size)
{
        uint32_t addr_sel;
        /* TVAR, [virTual][Velocity][Antenna][Range] */
        /* chirp 0 and no virtual array*/
        addr_sel = ch_index * (fft_size / 2) + rng_index;
        return baseband_read_mem_table(addr_sel);
}

/* read sample data of the last 2 chirps in a frame in memory(sample buffer) */
/* vel_index should be 0 or 1, as only 2 chirps stored in sample buffer */
uint32_t baseband_hw_get_sam_buf(int vel_idx, int rng_idx, int ant_idx)
{
        /* RA, [Velocity][Range][Antenna] */
        uint32_t addr_sel = vel_idx * (MAX_FFT_RNG / 2) * MAX_NUM_RX // adc data is 16 bits, 2 adc data in one memory entry(32 bits)
                            + rng_idx * MAX_NUM_RX + ant_idx;

        return baseband_read_mem_table(addr_sel);
}

// TODO: saturation monitor
//void Txbf_bb_satur_monitor(BasebandInitializationParams_t* cfg, unsigned int tx_mux)
//{
//        fmcw_radio_t* radio = &CONTAINER_OF(bb_hw, baseband_t, bb_hw)->radio;
//        uint32_t candi_phase[8] = {0, 45, 90, 135, 180, 225, 270, 315};      //Fix me 8 is the total tx phase number
//        uint16_t bb_status_en = 0;
//        unsigned long sat_cnt[8]={0}; /*the saturation counts of all stages(TIA/VGA1/VGA2) of all Rx chan under a tx phase*/
//        uint32_t agc_reg[MAX_NUM_RX+AFE_SATS*MAX_NUM_RX+MAX_NUM_RX+1];
//        /******************** keep the spot *********************/
//        baseband_dc_calib_status_save(bb_hw);    /* save radio configurations and disable VAM*/
//        fmcw_radio_txphase_status(radio, true);   /* save Tx phase*/
//        fmcw_radio_tx_ch_on(radio, -1, false);   /*turn off original tx*/
//        BB_WRITE_REG(bb_hw, SAM_DINT_ENA, 0); /* Disable Rx phase de-scramble during Txbf */
//
//        /*configure new tx register */
//        int ch = 0;
//        int loop = 0;
//        uint32_t reg_val;
//        while (ch < MAX_NUM_TX) {
//                if ((tx_mux>>ch) & 0x1) {
//                        fmcw_radio_tx_ch_on(radio, ch, true);
//                }
//                ch++;
//        }
//        //enable agc
//        baseband_agc_init(bb_hw, 2);
//        for (loop = 0; loop < 8; loop++) {
//                /*set tx phase*/
//                EMBARC_PRINTF("tx_phase = %d\n\r", candi_phase[loop]);
//                sat_cnt[loop] = 0;
//                for (ch = 0; ch < MAX_NUM_TX; ch++)
//                {
//                        reg_val = phase_val_2_reg_val(candi_phase[loop]);
//                        fmcw_radio_set_tx_phase(radio, ch, reg_val);
//                }
//                MDELAY(1);
//               //run bb
//                bb_status_en = SYS_ENA(AGC, true) | SYS_ENA(SAM, true) | SYS_ENA(FFT_2D, true);
//                baseband_start_with_params(cfg->bb, true, false, bb_status_en, false, 0, false);
//                /* wait done */
//                while (baseband_hw_is_running() == false)
//                         ;
//                while (baseband_hw_is_running() == true)
//                        ;
//                MDELAY(10);
//                //collect results
//                baseband_agc_dbg_reg_dump(2);
//                baseband_agc_dbg_reg_dump(1);
//                baseband_agc_dbg_reg_store(agc_reg);
//                for (int i=0; i< AFE_SATS; i++)
//                        for(int r=0; r<MAX_NUM_RX;r++)
//                                sat_cnt[loop]= agc_reg[MAX_NUM_RX+i*MAX_NUM_RX+r] + sat_cnt[loop];
//                BB_WRITE_REG(AGC_SAT_CNT_CLR_FRA,  1);
//                BB_WRITE_REG(AGC_IRQ_CLR, 1);
//        }
//        //recover the spot
//        baseband_dc_calib_status_restore(bb_hw);
//        fmcw_radio_txphase_status(radio, false);
//        unsigned long out_phase_sat_num = sat_cnt[0];
//        int idx = 0;
//        for (int i=1; i<8; i++) {
//                if (sat_cnt[i]<out_phase_sat_num) {
//                        out_phase_sat_num = sat_cnt[i];
//                        idx = i;
//                }
//        }
//        EMBARC_PRINTF("Tx phase %d gets minimum saturation number\n\r", candi_phase[idx]);
//        EMBARC_PRINTF("Txbf_bb_satur_monitor done\n\r");
//}

void baseband_reg_dump(void)
{
        static const uint32_t bb_reglist[] = {
#include "reg_list_alps_FM.h"
        };
        static const char *bb_regname[] = {
#include "reg_list_str_alps_FM.h"
        };

        for (int idx = 0; idx < sizeof(bb_reglist) / sizeof(uint32_t); idx++)
        {
                int rm_space = 32 - strlen(bb_regname[idx]);
                rm_space = MAX(0, rm_space);
                //EMBARC_PRINTF("BB_REG_%s", bb_regname[idx]);
                //xprintf("BB_REG_%s", bb_regname[idx]);
                for (int space_idx = 0; space_idx < rm_space; space_idx++)
                {
                        //EMBARC_PRINTF("%c", ' ');
                        //xprintf(" ");
                }
                //EMBARC_PRINTF("0x%x  0x%08x\n\r", bb_reglist[idx], baseband_read_reg(bb_reglist[idx]));
                //xprintf("\t0x%x   0x%x\n\r", bb_reglist[idx], baseband_read_reg(bb_reglist[idx]));
        }
}

void baseband_tbl_dump(uint32_t tbl_id, uint32_t offset, uint32_t length)
{
        uint32_t old = baseband_switch_mem_access(tbl_id);
        int i;
        uint32_t d;
        for (i = 0; i < length; i++)
        {
                d = baseband_read_mem_table(offset + i);
                //if (i % 4 == 0)
                //EMBARC_PRINTF("0x%05x: ", offset+i);
                //if (i % 4 != 3)
                //EMBARC_PRINTF("0x%08x, ", d);
                //else
                        //EMBARC_PRINTF("0x%08x\n\r", d);
                mdelay(1); /* add delay to improve the validity of the serial data*/

        }
        baseband_switch_mem_access(old);
}

//TODO: BIST
///* return true if bist success */
//bool baseband_bist_ctrl(bool print_ena)
//{
//        uint8_t old_irq_mask = raw_readl(REL_REGBASE_DMU + REG_DMU_IRQ_ENA32_63_OFFSET);
//        dmu_irq_enable(INT_BB_DONE, 0); // irq mask disable
//        dmu_irq_enable(INT_BB_SAM, 0);  // irq mask disable
//        bb_top_enable(0);               // disable bb_top clock
//
//        // start BB_LBIST
//        EMU_WRITE_REG(BB_LBIST_CLR, 1);
//        EMU_WRITE_REG(BB_LBIST_ENA, 1);
//
//        if (print_ena == true)
//        {
//        	EMBARC_PRINTF("BB_LBIST start, status = %x\n\r",EMU_READ_REG(LBIST_STA));
//        }
//
//        // FIXME, DONE signal always be true(bug in alps_mp), so using extra delay
//        MDELAY(BB_LBIST_DELAY); // wait for done
//
//        // query DONE signal
//        while (EMU_READ_REG_FEILD(LBIST_STA, DONE) == false) {
//                if (print_ena == true) {
//                        EMBARC_PRINTF("BB_LBIST is running ...\n\r");
//                        MDELAY(BB_LBIST_DELAY);
//                }
//        }
//
//        // query result
//        bool bist_result = EMU_READ_REG_FEILD(LBIST_STA, FAIL);
//
//        // reset bb
//        bb_core_reset(1);           // reset bb_core
//        bb_core_reset(0);           // deassert reset
//        bb_top_enable(1);           // enable bb_top clock
//        BB_WRITE_REG(SYS_BNK_RST, 1);                // reset bank
//        BB_WRITE_REG(SYS_IRQ_CLR, BB_IRQ_CLEAR_ALL); // clear irq status, which maybe asserted in bist
//
//        raw_writel(REL_REGBASE_DMU + REG_DMU_IRQ_ENA32_63_OFFSET, old_irq_mask); // restore irq mask
//
//        if (print_ena) {
//                EMBARC_PRINTF("BB_LBIST done, status = %x\n\r",EMU_READ_REG(LBIST_STA));
//                EMBARC_PRINTF("bb_status= %x\n\r",BB_READ_REG(SYS_STATUS));
//                EMBARC_PRINTF("bb_irq= %x\n\r",BB_READ_REG(SYS_IRQ_STATUS));
//        }
//
//        return (!bist_result); /* return ture if bist success */
//}

/* peak_power should be an array with size 4 */
void baseband_fft_peak_calc(BasebandInitializationParams_t *cfg, float *peak_power, uint32_t fft_size)
{
        uint16_t peak_point = 0;

        // find the FFT index near 5MHz
        // adc_freq not in radio shadow bank, should be aligned with radio by using sensor_config_init0.hxx

        uint16_t chk_bgn;
        uint16_t fc_sin = ABIST_SIN_FREQ * fft_size / (cfg->com_params->adc_freq); // center frequency of sinewave

        if (fc_sin < (ABIST_FFT_CHK_HLF + ABIST_FFT_CHK_MIN))
                chk_bgn = ABIST_FFT_CHK_MIN;
        else
                chk_bgn = fc_sin - ABIST_FFT_CHK_HLF;

        baseband_switch_mem_access(BB_MEM_ACT_BUF);
        // search the peak
        // since the sinewave is always 5MHz, so FFT peak search can be narrowed, here using ABIST_FFT_CHK_SIZE
        for (uint8_t ch_index = 0; ch_index < ANT_NUM; ch_index++)
        {
                for (uint8_t i = 0; i < ABIST_FFT_CHK_SIZE; i++)
                {
                        uint16_t rng_index = chk_bgn + i;

                        if (rng_index > (fft_size / 2 - 1))
                                break; // index out of range

                        uint32_t fft_mem = baseband_hw_get_fft_mem_abist(ch_index, rng_index, fft_size);
                        complex_t complex_fft = cfl_to_complex(fft_mem, 14, 14, true, 4, false);
                        float power = 10 * log10f(complex_fft.r * complex_fft.r + complex_fft.i * complex_fft.i);

                        // save the peak
                        if (i == 0)
                        {
                                peak_power[ch_index] = power;
                                peak_point = rng_index;
                        }
                        else if (power > peak_power[ch_index])
                        {
                                peak_power[ch_index] = power;
                                peak_point = rng_index;
                        }
                        else
                        {
                                /* Doing nothing */
                        }
                }
                //EMBARC_PRINTF("\tChannel %d, FFT1D peak = %2.5f dB\t index = %d\t freq = %2.3f MHz\n\r",
                //ch_index, peak_power[ch_index], peak_point, (float)(peak_point)*(cfg->adc_freq)/fft_size);
        }
}

// TODO: dac_playback
///* peak_power should be an array with size 4 */
//void baseband_dac_playback(baseband_hw_t *bb_hw, bool inner_circle, uint8_t inject_num, uint8_t out_num, bool adc_dbg_en, float* peak_power)
//{
//        BasebandInitializationParams_t* cfg = (BasebandInitializationParams_t*)CONTAINER_OF(bb_hw, baseband_t, bb_hw)->cfg;
//        fmcw_radio_t* radio = &CONTAINER_OF(bb_hw, baseband_t, bb_hw)->radio;
//
//        /* Assert and deassert ADC reset and bb_core reset */
//        baseband_hw_reset_after_force();
//
//        /* config radio */
//        fmcw_radio_tx_ch_on(radio, -1, false); /* turn off tx */
//        if (inner_circle == true)
//                fmcw_radio_dac_reg_cfg_inner(radio, inject_num, out_num);
//        else
//                fmcw_radio_dac_reg_cfg_outer(radio);   /* config analog DAC*/
//
//        /* Enter radio shadow bank */
//        fmcw_radio_loop_test_en(radio, true);
//        MDELAY(1); /* FIXME, delay for radio settle, time maybe shorter */
//
//        /* Disable velamb-CD mode in bank 0 */
//        uint32_t old_bnk_act = baseband_switch_bnk_act(0x0);
//        uint32_t old_sys_type_fmcw = BB_READ_REG(SYS_TYPE_FMCW);
//        BB_WRITE_REG(bb_hw, SYS_TYPE_FMCW, 0x0);
//
//        /* switch to baseband shadow bank */
//        baseband_switch_bnk_act(SHADOW_BNK);
//        uint32_t old_bnk_mode = baseband_switch_bnk_mode(SYS_BNK_MODE_SINGLE);
//        uint32_t fft_size = BB_READ_REG(SYS_SIZE_RNG_FFT) + 1;
//
//        /* start baseband */
//        uint16_t bb_status_en = SYS_ENA(SAM, true);
//        if (adc_dbg_en == true)
//                BB_WRITE_REG(SAM_SINKER, SAM_SINKER_BUF); /* sinker adc for debeg*/
//
//        baseband_start_with_params(cfg->bb, false, false, bb_status_en, false, BB_IRQ_ENABLE_SAM_DONE, false);
//
//        /* wait done */
//        while (baseband_hw_is_running() == false)
//                ;
//
//        while (baseband_hw_is_running() == true)
//                ;
//
//        if (adc_dbg_en == true) {
//                BB_WRITE_REG(bb_hw, SAM_SINKER, SAM_SINKER_FFT); /* restore to be sinker fft */
//                baseband_tbl_dump(bb_hw, SYS_MEM_ACT_BUF, 0, ANT_NUM * fft_size/2);
//        }
//
//        /* Restore velamb-CD mode in bank 0 */
//        baseband_switch_bnk_act(0x0);
//        BB_WRITE_REG(bb_hw, SYS_TYPE_FMCW, old_sys_type_fmcw);
//
//        /* restore registers status */
//        baseband_switch_bnk_act(old_bnk_act);
//        baseband_switch_bnk_mode(old_bnk_mode);
//        fmcw_radio_tx_restore(radio);
//
//        /* leave radio shadow bank */
//        fmcw_radio_loop_test_en(radio, false);
//        MDELAY(1);
//
//        /* find the 1d-fft peak */
//        if (adc_dbg_en == false)
//                baseband_fft_peak_calc(bb_hw, peak_power, fft_size);
//
//        baseband_hw_reset_after_force();
//}

// TODO: hil
///* bb states splited when dumping data, as hil input and data dump use the same dbgbus, which should be switched */
//void baseband_hil_state_ctrl(baseband_hw_t *bb_hw, uint16_t bb_ena_0, uint16_t bb_ena_1, uint16_t bb_ena_2)
//{
//        /* 1st run, hil data input */
//        baseband_hw_start_with_params(bb_ena_0, 0);
//        dbgbus_input_config();
//        dbgbus_hil_ready(); /* ready signal of hil sent to FPGA data collection board */
//        dmu_hil_input_mux(HIL_GPIO);
//        while (baseband_hw_is_running() == false)
//                ;
//        while (baseband_hw_is_running() == true) /* wait done */
//                ;
//
//        /* 2nd run, fft data dump */
//        if (bb_ena_1 != 0){
//                dump_reset();  /* reset signal to fpga board */
//                fmcw_radio_lvds_on(NULL, true);
//                baseband_hw_running_done(bb_ena_1, 0);
//        }
//
//        /* 3rd run, run to DOA */
//        if (bb_ena_2 != 0)
//                baseband_hw_running_done(bb_ena_2, 0);
//}
//
//void baseband_hil_on_gpio(baseband_hw_t *bb_hw, uint8_t dmp_mux, int32_t frame_num)
//{
//        uint16_t bb_ena_0, bb_ena_1, bb_ena_2;
//
//        switch (dmp_mux) {
//        case DMP_FFT_1D:
//                bb_ena_0 =  SYS_ENA(HIL    , true); /* 1st run, HIL(FFT_1D shared) */
//                bb_ena_1 =  SYS_ENA(DMP_MID, true); /* 2nd run, dump data          */
//                bb_ena_2 =  SYS_ENA(FFT_2D , true)
//                           |SYS_ENA(CFR    , true)
//                           |SYS_ENA(BFM    , true);  /* 3rd run, FFT_2D, CFAR and DOA */
//                break;
//
//        case DMP_FFT_2D:
//                bb_ena_0 =  SYS_ENA(HIL    , true)
//                           |SYS_ENA(FFT_2D , true)
//                           |SYS_ENA(CFR    , true)
//                           |SYS_ENA(BFM    , true); /* 1st run, HIL(FFT_1D shared), FFT_2D, CFAR and DOA*/
//                bb_ena_1 =  SYS_ENA(DMP_FNL, true); /* 2nd run, dump data */
//                bb_ena_2 =  0;
//                break;
//        default:
//                bb_ena_0 =  SYS_ENA(HIL    , true)
//                           |SYS_ENA(FFT_2D , true)
//                           |SYS_ENA(CFR    , true)
//                           |SYS_ENA(BFM    , true); /* 1st run, HIL(FFT_1D shared), FFT_2D, CFAR and DOA*/
//                bb_ena_1 =  0;
//                bb_ena_2 =  0;
//                break;
//        }
//
//        io_mux_dbgbus_dump(); /* gpio mux */
//        dbgbus_input_config();
//
//        MDELAY(5);
//
//        if (bb_ena_1 != 0)    /* lvds config */
//                lvds_dump_config(DBG_SRC_DUMP_W_SYNC);
//
//        baseband_data_proc_hil(bb_ena_0, bb_ena_1, bb_ena_2);
//
//        /* change frame number */
//        xSemaphoreTake(mutex_frame_count, portMAX_DELAY);
//        frame_count = frame_num;
//        xSemaphoreGive(mutex_frame_count);
//
//}
//

//void baseband_hil_on(baseband_hw_t *bb_hw, bool input_mux, uint8_t dmp_mux, int32_t frame_num)
//{
//        /* HIL input mux, ahb bus or gpio(debug bus) */
//        if (input_mux == HIL_GPIO)
//                baseband_hil_on_gpio(bb_hw, dmp_mux, frame_num);
//        else
//                baseband_hil_on_ahb(bb_hw);
//
//}

//HIL
/* parse cfar and doa result */
void baseband_parse_mem_rlt(BasebandDriverInit_t *bb, bool print_ena)
{
        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_RLT);

        /* get memory offset */
        uint32_t mem_rlt_offset = 0;
        uint16_t cfr_size_obj = BB_READ_REG(CFR_SIZE_OBJ);
        if (cfr_size_obj < 256)
        { /* mem_rlt will be splited to 4 banks when cfar size less than 256 */
                uint8_t bnk_idx = bb->frame_type_id;
                mem_rlt_offset = bnk_idx * (1 << SYS_SIZE_OBJ_WD_BNK) * RESULT_SIZE;
        }
        volatile obj_info_t *obj_info = (volatile obj_info_t *)(BB_MEM_BASEADDR + mem_rlt_offset);

        /* get object number */
        int obj_num = BB_READ_REG(CFR_NUMB_OBJ);
        /* parse result */
        for (int i = 0; i < obj_num; i++)
        {
                /* cfar result */
                uint32_t vel_acc = obj_info[i].vel_acc; // bitwidth 4  ; /* 0x00 */
                uint32_t vel_idx = obj_info[i].vel_idx; // bitwidth 10 ;
                uint32_t rng_acc = obj_info[i].rng_acc; // bitwidth 4  ;
                uint32_t rng_idx = obj_info[i].rng_idx; // bitwidth 10 ;
                uint32_t noi = obj_info[i].noi;         // bitwidth 20 ; /* 0x04 */
                uint32_t amb_idx = obj_info[i].amb_idx; // bitwidth 5  ;
                if (print_ena)
                {
                        EMBARC_PRINTF("\n\rvel_acc     = %x\n\r", vel_acc); // bitwidth 4  ; /* 0x00 */
                        EMBARC_PRINTF("vel_idx     = %x\n\r", vel_idx);     // bitwidth 10 ;
                        EMBARC_PRINTF("rng_acc     = %x\n\r", rng_acc);     // bitwidth 4  ;
                        EMBARC_PRINTF("rng_idx     = %x\n\r", rng_idx);     // bitwidth 10 ;
                        EMBARC_PRINTF("noi         = %x\n\r", noi);         // bitwidth 20 ; /* 0x04 */
                        EMBARC_PRINTF("amb_idx     = %x\n\r", amb_idx);     // bitwidth 5  ;
                }

                /* doa result */
                if (obj_info[i].doa[0].ang_vld_0)
                {
                        uint32_t ang_acc_0 = obj_info[i].doa[0].ang_acc_0; // bitwidth 4  ; /* 0x00 */
                        uint32_t ang_idx_0 = obj_info[i].doa[0].ang_idx_0; // bitwidth 9  ;
                        uint32_t sig_0 = obj_info[i].doa[0].sig_0;         // bitwidth 20 ; /* 0x04 */
                        if (print_ena)
                        {
                                EMBARC_PRINTF("ang_acc_0   = %x\n\r", ang_acc_0); // bitwidth 4  ; /* 0x00 */
                                EMBARC_PRINTF("ang_idx_0   = %x\n\r", ang_idx_0); // bitwidth 9  ;
                                EMBARC_PRINTF("sig_0       = %x\n\r", sig_0);     // bitwidth 20 ; /* 0x04 */
                        }
                }

                if (obj_info[i].doa[0].ang_vld_1)
                {
                        uint32_t ang_acc_1 = obj_info[i].doa[0].ang_acc_1; // bitwidth 4  ; /* 0x08 */
                        uint32_t ang_idx_1 = obj_info[i].doa[0].ang_idx_1; // bitwidth 9  ;
                        uint32_t sig_1 = obj_info[i].doa[0].sig_1;         // bitwidth 20 ; /* 0x0c */
                        if (print_ena)
                        {
                                EMBARC_PRINTF("ang_acc_1   = %x\n\r", ang_acc_1); // bitwidth 4  ; /* 0x08 */
                                EMBARC_PRINTF("ang_idx_1   = %x\n\r", ang_idx_1); // bitwidth 9  ;
                                EMBARC_PRINTF("sig_1       = %x\n\r", sig_1);     // bitwidth 20 ; /* 0x0c */
                        }
                }

                if (obj_info[i].doa[0].ang_vld_2)
                {
                        uint32_t ang_acc_2 = obj_info[i].doa[0].ang_acc_2; // bitwidth 4  ; /* 0x10 */
                        uint32_t ang_idx_2 = obj_info[i].doa[0].ang_idx_2; // bitwidth 9  ;
                        uint32_t sig_2 = obj_info[i].doa[0].sig_2;         // bitwidth 20 ; /* 0x14 */
                        if (print_ena)
                        {
                                EMBARC_PRINTF("ang_acc_2   = %x\n\r", ang_acc_2); // bitwidth 4  ; /* 0x10 */
                                EMBARC_PRINTF("ang_idx_2   = %x\n\r", ang_idx_2); // bitwidth 9  ;
                                EMBARC_PRINTF("sig_2       = %x\n\r", sig_2);     // bitwidth 20 ; /* 0x14 */
                        }
                }

                if (obj_info[i].doa[0].ang_vld_3)
                {
                        uint32_t ang_acc_3 = obj_info[i].doa[0].ang_acc_3; // bitwidth 4  ; /* 0x18 */
                        uint32_t ang_idx_3 = obj_info[i].doa[0].ang_idx_3; // bitwidth 9  ;
                        uint32_t sig_3 = obj_info[i].doa[0].sig_3;         // bitwidth 20 ; /* 0x1c */
                        if (print_ena)
                        {
                                EMBARC_PRINTF("ang_acc_3   = %x\n\r", ang_acc_3); // bitwidth 4  ; /* 0x18 */
                                EMBARC_PRINTF("ang_idx_3   = %x\n\r", ang_idx_3); // bitwidth 9  ;
                                EMBARC_PRINTF("sig_3       = %x\n\r", sig_3);     // bitwidth 20 ; /* 0x1c */
                        }
                }
        } // end for loop

        /* restore memory bank */
        baseband_switch_mem_access(old);
}

void baseband_hil_input_ahb(BasebandInitializationParams_t *cfg)
{
        uint32_t samp_skip;
        uint32_t samp_used;
        get_sample_used(cfg, &samp_skip, &samp_used);
        samp_used = samp_used > cfg->rng_nfft ? cfg->rng_nfft : samp_used;
        // hil input data via ahb bus
        for (int i = 0; i < (cfg->nvarray) * ANT_NUM; i++)
        { /*ant loop*/
                for (int c = 0; c < cfg->vel_nfft; c++)
                { /*chirp loop*/
                        for (int r = 0; r < samp_used / 2; r++)
                        { /*sample loop*/
                                uint32_t hil_data = ((hil_sin[(r * 2 + 1) % HIL_SIN_PERIOD] << 16) | hil_sin[(r * 2) % HIL_SIN_PERIOD]);
                                raw_writel(REL_REGBASE_DMU + REG_DMU_HIL_DAT_OFFSET, hil_data);
                        }
                        for (int m = samp_used; m < cfg->rng_nfft; m = m + 2)
                        { /*fill 0*/
                                uint32_t hil_data = 0;
                                raw_writel(REL_REGBASE_DMU + REG_DMU_HIL_DAT_OFFSET, hil_data);
                        }
                }
        }
}
void baseband_hil_on_ahb(BasebandDriverInit_t *bb)
{
        //dmu_hil_input_mux(HIL_AHB);
        raw_writel(REL_REGBASE_DMU + REG_DMU_HIL_ENA_OFFSET, 0);
        uint16_t bb_status_en = SYS_ENA(HIL, true) | SYS_ENA(FFT_2D, true) | SYS_ENA(CFR, true) | SYS_ENA(BFM, true);
        baseband_hw_start_with_params(bb_status_en, 0);
        /* HIL data starts, sine wave via AHB bus*/
        baseband_hil_input_ahb(bb->cfg);
        /* wait done */
        while (baseband_hw_is_running() == true)
                ;
        /* parse cfar and doa reselt */
        baseband_parse_mem_rlt(bb, true);
        /*  USE for dump FFT mem and need to change SYS_ENA(CFR&BFM) to false */

        //        BasebandInitializationParams_t *cfg = bb->cfg;
        //        int ch_index, vel_index, rng_index;
        //        uint32_t frame_index;
        //        for (frame_index = 1; frame_index <= 1; frame_index++) {
        //             uint32_t old = baseband_switch_mem_access(SYS_MEM_ACT_BUF);
        //             for (ch_index = 0; ch_index < 1; ch_index++) {
        //                  //xprintf("\nA %d\n", ch_index);
        //                  for (vel_index = 0; vel_index < cfg->vel_nfft / 2; vel_index++ ) {
        //                	   //xprintf("V %d\n", vel_index);
        //                	   for (rng_index = 0; rng_index < cfg->rng_nfft / 2; rng_index++ ) {
        //                	   uint32_t fft_mem = baseband_hw_get_fft_mem(cfg, ch_index, rng_index, vel_index, 0);
        //                	   //xprintf("%x\n", fft_mem);
        //                	   if (rng_index % 10 == 0)
        //                	       bb_hw_mdelay(1);
        //                	   }
        //                   }
        //              }
        //              bb_hw_mdelay(1000);
        //              baseband_switch_mem_access(old);
        //         }
}
void baseband_agc_dbg_reg_store(uint32_t *p_dbg_reg)
{
        uint32_t agc_dbg_base_adr = BB_REG_AGC_COD_C0;
        uint32_t agc_dbg_adr = agc_dbg_base_adr;
        //store agc_cod
        for (int i = 0; i < MAX_NUM_RX; i++)
        {
                *p_dbg_reg = baseband_read_reg(agc_dbg_adr);
                p_dbg_reg = p_dbg_reg + 1;
                agc_dbg_adr = agc_dbg_adr + 0x4;
        }
        //store agc_sat_cnt
        for (int i = 0; i < (AFE_SATS * MAX_NUM_RX); i++)
        {
                *p_dbg_reg = baseband_read_reg(agc_dbg_adr);
                p_dbg_reg = p_dbg_reg + 1;
                agc_dbg_adr = agc_dbg_adr + 0x4;
        }
        //only store DAT_MAX_1ST here
        for (int i = 0; i < MAX_NUM_RX; i++)
        {
                *p_dbg_reg = baseband_read_reg(agc_dbg_adr);
                p_dbg_reg = p_dbg_reg + 1;
                agc_dbg_adr = agc_dbg_adr + 0x4;
        }
        //store fdb agc_irq_status
        *p_dbg_reg = BB_READ_REG(AGC_IRQ_STATUS);
}
void baseband_agc_dbg_reg_dump(int item)
{
        uint32_t agc_reg[MAX_NUM_RX + AFE_SATS * MAX_NUM_RX + MAX_NUM_RX + 1];
        baseband_agc_dbg_reg_store(agc_reg);
        int base = 0;
        switch (item)
        {
        case 0:
                //EMBARC_PRINTF("AGC_COD: 0x%08x 0x%08x 0x%08x 0x%08x\n\r", agc_reg[0],agc_reg[1],agc_reg[2],agc_reg[3]);
                break;
        case 1:
                base = MAX_NUM_RX;
                for (int i = 0; i < AFE_SATS; i++)
                {
                        //EMBARC_PRINTF("AGC_AFE_stage%d_SAT_CNT: %d %d %d %d\n\r", i, agc_reg[base + MAX_NUM_RX * i + 0],agc_reg[base + MAX_NUM_RX * i + 1],
                        //agc_reg[base + MAX_NUM_RX * i + 2],agc_reg[base + MAX_NUM_RX * i + 3]);
                        //EMBARC_PRINTF("\n\r");
                }
                break;
        case 2:
                base = MAX_NUM_RX + AFE_SATS * MAX_NUM_RX;
                //EMBARC_PRINTF("AGC_DAT_MAX_1ST: 0x%08x 0x%08x 0x%08x 0x%08x\n\r", agc_reg[base + 0],agc_reg[base + 1],agc_reg[base + 2],agc_reg[base + 3]);
                break;
        case 3:
                //EMBARC_PRINTF("AGC_IRQ_STATUS: 0x%08x\n\r", agc_reg[MAX_NUM_RX+AFE_SATS*MAX_NUM_RX+MAX_NUM_RX]);
                break;
        default:
                break;
        }
}

void baseband_datdump_smoke_test(void)
{
        int j = 0;
        int dat_dmp_pttn = 0;
        BB_WRITE_REG(SYS_MEM_ACT, BB_MEM_ACT_BUF);
        for (int i = 0; i < DATA_DUMP_NUM_2MB; i++)
        {
                if (DATA_DUMP_PATTEN == 0)
                {
                        dat_dmp_pttn = j;
                }
                else if (DATA_DUMP_PATTEN == 1)
                {
                        dat_dmp_pttn = ((j * 2) << 16) + (j * 2 + 1);
                }
                else if (DATA_DUMP_PATTEN == 2)
                {
                        dat_dmp_pttn = 0;
                }
                else
                {
                        /* Doing nothing */
                }

                /* write to memory, the following address format(i) only adapts to NO virtual array */
                baseband_write_mem_table(i, dat_dmp_pttn);

                if (j < HALF_MAX_ADC_DATA) /* loop 0 ~ 32767*/
                {
                        j = j + 1;
                }
                else
                {
                        j = 0;
                }
        }
}

//// TODO: dbg_start
//void baseband_dbg_start(BasebandInitializationParams_t* cfg, uint8_t dbg_mux)
//{
//        baseband_t* bb = cfg->bb;
//
//        uint8_t old_dbg_mux = BB_READ_REG(DBG_BUF_TAR);
//
//        BB_WRITE_REG(DBG_BUF_TAR, dbg_mux); /* turn on debug data dump*/
//
//        uint16_t bb_status_en =  SYS_ENA(AGC   , cfg->agc_mode == 0 ? 0: 1)
//                                |SYS_ENA(SAM   , true         )
//                                |SYS_ENA(FFT_2D, true         )
//                                |SYS_ENA(CFR   , true         )
//                                |SYS_ENA(BFM   , true         );
//        baseband_start_with_params(bb, true, true, bb_status_en, true, BB_IRQ_ENABLE_SAM_DONE, false); /* no need track */
//        while (baseband_hw_is_running() == false) /* wait start */
//                ;
//        while (baseband_hw_is_running() == true) /* wait done */
//                ;
//
//        /* restroe dbg_mux */
//        BB_WRITE_REG(DBG_BUF_TAR, old_dbg_mux); /* turn off debug data dump*/
//}

void baseband_shadow_bnk_init(BasebandInitializationParams_t *cfg)
{
        /* switch baseband active bank */
        uint32_t old_bnk = baseband_switch_bnk_act(SHADOW_BNK);

        /* sys */
        uint16_t bb_status_en = SYS_ENA(SAM, true); // only sample (fft1d shared with sample state )
        BB_WRITE_REG(SYS_ENABLE, bb_status_en);
        BB_WRITE_REG(SYS_SIZE_RNG_SKP, 0); // no need skip
        BB_WRITE_REG(SYS_SIZE_RNG_BUF, cfg->rng_nfft - 1);
        BB_WRITE_REG(SYS_SIZE_RNG_PRD, cfg->rng_nfft + 100); // FIXME, just longthen chirp points for hardware loop
        BB_WRITE_REG(SYS_SIZE_BPM, 0);
        BB_WRITE_REG(SYS_SIZE_RNG_FFT, cfg->rng_nfft - 1);
        BB_WRITE_REG(SYS_SIZE_VEL_FFT, 0); // only 1 chirp
        BB_WRITE_REG(SYS_SIZE_VEL_BUF, 0);

        /* sam */
        BB_WRITE_REG(SAM_SINKER, SAM_SINKER_FFT); /* direct or buffer */
        BB_WRITE_REG(SAM_FORCE, 1);               /* force start */

        /* fft */
        BB_WRITE_REG(FFT_SHFT_RNG, cfg->rng_nfft - 1);
        BB_WRITE_REG(FFT_NO_WIN, 1); /* shadow bank has no window LUT */

        /* restore bank active */
        baseband_switch_bnk_act(old_bnk);
}

// both adc and bb should be reset due to bug 841
void baseband_hw_reset_after_force(void)
{
        // 1st, adc reset asserted
#ifdef CHIP_CASCADE
        if (chip_cascade_status() == CHIP_CASCADE_MASTER)
                raw_writel(REL_REGBASE_DMU + REG_DMU_ADC_RSTN_OFFSET, 0); // asserted reset_n
#else
        // save old status
        int old_mux_reset = raw_readl(REL_REGBASE_DMU + REG_DMU_MUX_RESET_OFFSET);
        int old_mux_sync = raw_readl(REL_REGBASE_DMU + REG_DMU_MUX_SYNC_OFFSET);
        raw_writel(REL_REGBASE_DMU + REG_DMU_MUX_RESET_OFFSET, 4);
        raw_writel(REL_REGBASE_DMU + REG_DMU_MUX_SYNC_OFFSET, 4);
        raw_writel(REL_REGBASE_DMU + REG_DMU_ADC_RSTN_OFFSET, 0); // asserted ADC reset_n
#endif

        // 2nd, bb reset asserted
        bb_core_reset(1); // reset bb_core

        // 3rd, adc reset deasserted
        chip_hw_udelay(10); // delay for ADC reset
#ifdef CHIP_CASCADE
        if (chip_cascade_status() == CHIP_CASCADE_MASTER)
                raw_writel(REL_REGBASE_DMU + REG_DMU_ADC_RSTN_OFFSET, 1); // deasserted reset_n
#else
        raw_writel(REL_REGBASE_DMU + REG_DMU_ADC_RSTN_OFFSET, 1); // deasserted ADC reset_n
        // restore old status
        raw_writel(REL_REGBASE_DMU + REG_DMU_MUX_RESET_OFFSET, old_mux_reset);
        raw_writel(REL_REGBASE_DMU + REG_DMU_MUX_SYNC_OFFSET, old_mux_sync);
#endif

        // 4th, bb reset deasserted
        bb_core_reset(0); // deassert reset
}

/*parse each element of cfg->tx_groups to work state in a chirp way*/
bool bit_parse(uint32_t num, uint16_t bit_mux[])
{
        bool valid_mux = true;
        for (int i = 0; i < MAX_NUM_TX; i++)
        {
                bit_mux[i] = (num >> (4 * i)) & 0x3; //output tx work status in chirp i, bit_mux=0 tx off; bit_mux=1 tx on in phase; bit_mux=2  tx on opposite phase
                if (bit_mux[i] == 3)
                {
                        valid_mux = false;
                        break;
                }
        }
        return valid_mux;
}

/*get the phase of the first chirp, when more than 1 tx is on in the first chirp, return the phase of antenna with small index
mainly used to judge the transmittion order of BPM*/
uint16_t get_ref_tx_antenna(uint32_t patten[])
{
        uint16_t refer_phase = 0;
        uint16_t bit_mux[MAX_NUM_TX] = {0, 0, 0, 0};
        for (int i = 0; i < MAX_NUM_TX; i++)
        {
                bit_parse(patten[i], bit_mux);
                if (bit_mux[0] > 0)
                {
                        refer_phase = bit_mux[0];
                        break;
                }
        }
        return refer_phase;
}

/*to get the tx mux in every chirp by parse all patten*/
bool get_tdm_tx_antenna_in_chirp(uint32_t patten[], uint8_t chip_idx, uint32_t chirp_tx_mux[])
{
        uint16_t bit_mux[MAX_NUM_TX] = {0, 0, 0, 0};
        int32_t tx_groups_tmp[MAX_NUM_TX][MAX_NUM_TX] = {0};
        uint32_t ret = 0;
        for (int i = 0; i < MAX_NUM_TX; i++)
        { //Tx loop
                bit_parse(patten[i], bit_mux);
                for (int c = 0; c < MAX_NUM_TX; c++)
                { //chirp loop
                        if (bit_mux[c] > 0)
                                tx_groups_tmp[i][c] = 1; //output in which chirps Txi transmit
                }
        }
        for (int c = 0; c <= chip_idx; c++)
        { //chirp loop
                ret = 0;
                for (int i = 0; i < MAX_NUM_TX; i++)
                { //Tx loop
                        if (tx_groups_tmp[i][c] > 0)
                        {
                                ret |= (1 << i);
                        }
                }
                if (ret == 0)
                {
                        return false; //mean all tx off in chirp c
                }
                chirp_tx_mux[c] = ret; //ret is the mux presentation of txs which are on in chirp c
        }
        return true;
}

/*check bit_mux of a tx if satisfy the standard bpm pattern
for bpm va = 2, pattern 0 is x x , pattern 1 is x -x
for bpm va = 4, pattern 0 is x x x x, pattern 1 is x -x x -x, pattern 2 is x x -x -x, pattern 3 is x -x -x x*/
int8_t bpm_patten_check(uint16_t bit_mux[], uint8_t chip_idx, uint16_t refer_phase, uint8_t pat_num)
{
        int loop_num = chip_idx / 2 + 1; //for patten 1 2 3 check
        int8_t p = 0;
        for (int m = 0; m <= pat_num; m++)
        {
                p = 0; //indicate which pattern bit_mux shows. p = -1 :mean invalid pattern
                switch (m)
                {
                case 0:
                        for (int j = 0; j <= chip_idx; j++)
                        {
                                if (bit_mux[j] != refer_phase)
                                {
                                        p = -1;
                                        break;
                                }
                        }
                        if (p != -1)
                                p = 0; // patten 0 gotten     1 1 1 1
                        break;
                case 1:
                        for (int j = 0; j < loop_num; j++)
                        {
                                if ((bit_mux[2 * j] != refer_phase) || ((bit_mux[2 * j] + bit_mux[2 * j + 1]) != 3))
                                {
                                        p = -1;
                                        break;
                                }
                        }
                        if (p != -1)
                                p = 1; // patten 1 gotten  1 -1 1 -1
                        break;
                case 2:
                        for (int j = 0; j < loop_num; j++)
                        {
                                uint16_t phase = (j == 0) ? refer_phase : (3 - refer_phase);
                                if ((bit_mux[2 * j] != phase) || (bit_mux[2 * j] != bit_mux[2 * j + 1]))
                                {
                                        p = -1;
                                        break;
                                }
                        }
                        if (p != -1)
                                p = 2; // patten 2 gotten  1 1 -1 -1
                        break;
                case 3:
                        for (int j = 0; j < loop_num; j++)
                        {
                                uint16_t phase = (j == 0) ? refer_phase : (3 - refer_phase);
                                if ((bit_mux[2 * j] != phase) || ((bit_mux[2 * j] + bit_mux[2 * j + 1]) != 3))
                                {
                                        p = -1;
                                        break;
                                }
                        }
                        if (p != -1)
                                p = 3; // patten 3 gotten  1 -1 -1 1
                        break;
                default:
                        break;
                }
                if (p != -1)
                        break;
        }
        return p;
}

int8_t get_bpm_tx_antenna_in_chirp(uint32_t patten[], uint8_t chip_idx, uint32_t chirp_tx_mux[])
{
        uint16_t bit_mux[MAX_NUM_TX] = {0, 0, 0, 0};
        int8_t checker = 0; //checker = -1: pattern not found
        uint16_t refer_phase = 0;
        int i = 0;
        int32_t tx_groups[MAX_NUM_TX] = {0, 0, 0, 0};
        uint32_t ret = 0;
        refer_phase = get_ref_tx_antenna(patten);
        if (refer_phase == 0)
        { //mean all tx off in chirp 0
                checker = -1;
                return checker;
        }
        // get patten 0 first
        for (i = 0; i < MAX_NUM_TX; i++)
        {
                bit_parse(patten[i], bit_mux);
                checker = bpm_patten_check(bit_mux, chip_idx, refer_phase, 0);
                if (checker == 0)
                {
                        tx_groups[i] = 1;
                        break;
                }
                else
                {
                        continue;
                }
        }
        if (checker == -1)
                return checker;
        // get other pattern
        for (int j = 0; j < MAX_NUM_TX; j++)
        {
                if (j == i - 1)
                        continue;
                bit_parse(patten[j], bit_mux);
                uint8_t patten_num = (chip_idx == 1) ? 1 : 3; //chip_idx == 1 mean varray = 2
                checker = bpm_patten_check(bit_mux, chip_idx, refer_phase, patten_num);
                if (checker != -1)
                {
                        tx_groups[j] = checker + 1;
                }
        }
        /*generate the tx mux which are on in the same chirp*/
        for (int c = 0; c <= chip_idx; c++)
        {
                ret = 0;
                for (int i = 0; i < MAX_NUM_TX; i++)
                {
                        if (tx_groups[i] == (c + 1))
                                ret |= (1 << i);
                }
                if (ret == 0)
                {
                        checker = -1;
                        return checker;
                }
                chirp_tx_mux[c] = ret;
        }
        checker = 4; //success over
        return checker;
}

/* TODO: baseband params compute, maybe move to upper layer */
#define LIGHTSPEED 299792458.0

static uint32_t index_wrap(const int32_t N, int32_t idx)
{
        while (idx >= N / 2)
                idx -= N;
        while (idx < -N / 2)
                idx += N;
        return idx;
}

static uint32_t index_mirror(const int32_t N, int32_t idx)
{
        while (idx < 0)
                idx += N;
        while (idx >= N)
                idx -= N;
        if (idx > N / 2)
                idx = N - idx;
        return idx;
}

void radar_param_fft2rv_nowrap(const BasebandSystemParams_t *sys_params,
                               const float k,
                               const float p,
                               float *r,
                               float *v)
{
        *r = sys_params->r_coeff * (sys_params->r_k_coeff * k - sys_params->r_p_coeff * p);
        *v = sys_params->v_coeff * p;
}

static void radar_param_fft2rv(const BasebandSystemParams_t *sys_params,
                               const int32_t k,
                               const int32_t p,
                               float *r,
                               float *v)
{
        int32_t idx_p = index_wrap(sys_params->vel_nfft, p);
        int32_t idx_k = index_wrap(sys_params->rng_nfft, k);
        radar_param_fft2rv_nowrap(sys_params, idx_k, idx_p, r, v);
}

static void radar_param_rv2fft(BasebandInitializationParams_t *cfg, const BasebandSystemParams_t *sys_params,
                               const float r,
                               const float v,
                               int32_t *k,
                               int32_t *p)
{
        float idx = v * 2 * sys_params->vel_nfft;
        idx *= sys_params->chirp_period;
        /* 1e+3 is to balance unit */
        idx *= sys_params->carrier_freq * 1e+3;
        idx /= LIGHTSPEED;
        int32_t tmpp = round(idx);
        *p = index_wrap(sys_params->vel_nfft, tmpp);
        /* 1e+6 is to balance unit */
        float tmp = r * sys_params->bandwidth * 2 * 1e+6;
        tmp /= sys_params->chirp_rampup * LIGHTSPEED;
        tmp -= idx / (sys_params->chirp_period * sys_params->vel_nfft);
        tmp *= sys_params->rng_nfft / sys_params->Fs * cfg->dec_factor; // dividing Fs by dec_factor to correct range calculation
        *k = index_mirror(sys_params->rng_nfft, round(tmp));
}

static void baseband_param_update(BasebandInitializationParams_t *cfg, BasebandSystemParams_t *sys_params)
{
        float r, v;

        bool anti_velamb_en = cfg->com_params->anti_velamb_en;
        float TD = sys_params->chirp_period * cfg->nvarray;

        if (anti_velamb_en)
                TD = sys_params->chirp_period * (cfg->nvarray + 1) + cfg->com_params->anti_velamb_delay;

        /* compute coeff first */
        sys_params->r_k_coeff = sys_params->Fs / cfg->dec_factor / sys_params->rng_nfft; // dividing Fs by dec_factor to correct range calculation
        sys_params->r_p_coeff = 1.0 / (TD * sys_params->vel_nfft);

        sys_params->r_coeff = LIGHTSPEED / 1e+6 / (2 * sys_params->bandwidth) * sys_params->chirp_rampup;
        sys_params->v_coeff = LIGHTSPEED / 1e+3 / (2 * sys_params->vel_nfft * TD * sys_params->carrier_freq);

        radar_param_fft2rv(sys_params, 1, 0, &r, &v);
        sys_params->rng_delta = r;
        radar_param_fft2rv(sys_params, 0, 1, &r, &v);
        sys_params->vel_delta = v;
        sys_params->az_delta_deg = (sys_params->bfm_az_right -
                                    sys_params->bfm_az_left) /
                                   sys_params->doa_npoint[0];
        sys_params->ev_delta_deg = (sys_params->bfm_ev_up -
                                    sys_params->bfm_ev_down) /
                                   sys_params->doa_npoint[1];
        sys_params->az_delta = sys_params->az_delta_deg * DEG2RAD;
        sys_params->ev_delta = sys_params->ev_delta_deg * DEG2RAD;
        sys_params->vel_wrap_max = cfg->track_vel_pos_ind_portion * sys_params->vel_nfft;
}

void baseband_compute_sys_params(BasebandInitializationParams_t *cfg, BasebandSystemParams_t *sys_params)
{

        uint32_t start_freq = DIV_BASEBAND(cfg->com_params->fmcw_startfreq, BB_FREQ_SYNTH_SD_RATE);
        uint32_t stop_freq = DIV_BASEBAND(cfg->com_params->fmcw_startfreq + cfg->com_params->fmcw_bandwidth * 1e-3, BB_FREQ_SYNTH_SD_RATE);
        uint32_t bandwidth = stop_freq - start_freq;

        sys_params->start_freq = start_freq;
        sys_params->stop_freq = sys_params->start_freq + bandwidth;
        sys_params->step_up = (1L << 28) * cfg->com_params->fmcw_bandwidth / (BB_FREQ_SYNTH_SD_RATE * cfg->com_params->fmcw_chirp_rampup * BB_FREQ_SYNTH_SD_RATE * 8);
        sys_params->step_down = (1L << 28) * cfg->com_params->fmcw_bandwidth / (BB_FREQ_SYNTH_SD_RATE * cfg->com_params->fmcw_chirp_down * BB_FREQ_SYNTH_SD_RATE * 8);

        uint32_t gcd = compute_gcd(cfg->com_params->adc_freq, BB_FREQ_SYNTH_SD_RATE);
        uint32_t sd_M = BB_FREQ_SYNTH_SD_RATE / gcd;
        uint32_t total_cycle = round(cfg->com_params->fmcw_chirp_period * gcd) * sd_M;
        uint32_t up_cycle = ceil(1.0 * bandwidth / (sys_params->step_up));
        uint32_t down_cycle = ceil(1.0 * bandwidth / (sys_params->step_down));
        uint32_t wait_cycle = total_cycle - up_cycle - down_cycle;
        sys_params->cnt_wait = wait_cycle + down_cycle;
        sys_params->total_cycle = total_cycle;

        float bb_start_freq = INV_DIV_BASEBAND(sys_params->start_freq, BB_FREQ_SYNTH_SD_RATE);
        float bb_stop_freq = INV_DIV_BASEBAND(sys_params->stop_freq, BB_FREQ_SYNTH_SD_RATE);
        sys_params->carrier_freq = bb_start_freq;
        float bb_bandwidth = (bb_stop_freq - bb_start_freq) * 1e3;
        sys_params->bandwidth = bb_bandwidth;

        float up = 1.0 * (1L << 25) / BB_FREQ_SYNTH_SD_RATE / BB_FREQ_SYNTH_SD_RATE / sys_params->step_up * bb_bandwidth;
        float down = 0;
        float idle = sys_params->cnt_wait * 1.0 / BB_FREQ_SYNTH_SD_RATE;

        sys_params->chirp_rampup = up;
        sys_params->chirp_period = up + down + idle;
        sys_params->Fs = cfg->com_params->adc_freq;
        sys_params->rng_nfft = cfg->rng_nfft;
        sys_params->vel_nfft = cfg->vel_nfft;
        sys_params->doa_npoint[0] = cfg->doa_npoint[0];
        sys_params->doa_npoint[1] = cfg->doa_npoint[1];
        sys_params->bfm_az_left = cfg->bfm_az_left;
        sys_params->bfm_az_right = cfg->bfm_az_right;
        sys_params->bfm_ev_up = cfg->bfm_ev_up;
        sys_params->bfm_ev_down = cfg->bfm_ev_down;

        float asin_l = sin(sys_params->bfm_az_left * DEG2RAD);
        float asin_r = sin(sys_params->bfm_az_right * DEG2RAD);
        float asin_step = (asin_r - asin_l) / cfg->doa_npoint[0];

        float asin_d = sin(sys_params->bfm_ev_down * DEG2RAD);
        float asin_u = sin(sys_params->bfm_ev_up * DEG2RAD);
        float asin_step_ev = (asin_u - asin_d) / cfg->doa_npoint[1];

        sys_params->dml_sin_az_left = asin_l;
        sys_params->dml_sin_az_step = asin_step;
        sys_params->dml_sin_ev_down = asin_d;
        sys_params->dml_sin_ev_step = asin_step_ev;

        sys_params->trk_fps = cfg->track_fps;
        sys_params->trk_fov_az_left = cfg->track_fov_az_left;
        sys_params->trk_fov_az_right = cfg->track_fov_az_right;
        sys_params->trk_fov_ev_down = cfg->track_fov_ev_down;
        sys_params->trk_fov_ev_up = cfg->track_fov_ev_up;
        sys_params->trk_nf_thres = cfg->track_near_field_thres;
        sys_params->trk_capt_delay = cfg->track_capture_delay;
        sys_params->trk_drop_delay = cfg->track_drop_delay;

#ifndef UNIT_TEST
        baseband_param_update(cfg, sys_params);
#endif
}

/* For frame-interleaving */
////TODO: frame-interleaving
void baseband_frame_interleave_reg_write(uint8_t bb_bank_mode, uint8_t bb_bank_que)
{
        BB_WRITE_REG(SYS_BNK_MODE, bb_bank_mode);
        BB_WRITE_REG(SYS_BNK_QUE, bb_bank_que); /* used in rotate mode */
        BB_WRITE_REG(SYS_BNK_RST, 1);
}

void baseband_frame_interleave_pattern(uint8_t frame_loop_pattern)
{
        /* bb bank mode */
        uint8_t bb_bank_mode = SYS_BNK_MODE_ROTATE;

        /* bb bank queue */
        uint8_t bb_bank_que = 1 << frame_loop_pattern;

        baseband_frame_interleave_reg_write(bb_bank_mode, bb_bank_que);
}

/* Set BNK_ACT to FDB_BNK_ACT, and return RTL frame_type */
uint8_t baseband_get_rtl_frame_type()
{
        uint8_t current_frame_type = baseband_read_reg(BB_REG_FDB_SYS_BNK_ACT); /* read back the bank selected in RTL */
        baseband_write_reg(BB_REG_SYS_BNK_ACT, current_frame_type);             /* match the bank accessed by CPU */
        cur_radar_cfg_idx = current_frame_type;
        return current_frame_type;
}

/* configure the loop pattern in frame interleaving */
uint8_t baseband_frame_interleave_cfg(uint8_t frame_type_id)
{
        baseband_frame_interleave_pattern(frame_type_id);
        //fmcw_radio_frame_interleave_pattern(frame_type_id);
        return baseband_get_rtl_frame_type(); /* read back the bank selected in RTL */
}

/* MCAL added function start */
BasebandStatus_e baseband_hw_status_check(void)
{
        return (BasebandStatus_e)BB_READ_REG(SYS_STATUS);
}

int32_t baseband_hw_wait_for_bb_done(uint16_t sys_enable, uint8_t sys_irq_en)
{
        int32_t ret = 0;
        uint32_t timeout = 5000u;

        while (timeout > 0u)
        {
                timeout--;

                if (timeout == 0u)
                {
                        /* bb timeout */
                        ret = 1;
                        break;
                }

                if (sys_enable == 0u)
                {
                        /* No bb flow has enabled */
                        ret = 2;
                        break;
                }

                if ((sys_irq_en & 0x1) == 0u)
                {
                        /* bb_done irq not enabled */
                        ret = 3;
                        break;
                }

                if ((BB_READ_REG(SYS_IRQ_STATUS) & 0x1) == 1u)
                {
                        /* BB_DONE IRQ set */
                        BB_WRITE_REG(SYS_IRQ_CLR, 0x1); // clear bb_done irq
                        break;
                }
        }

        return ret;
}

uint32_t baseband_wait_bb_done(BasebandDoneWait_t *bb_wait)
{
    uint32_t ret = 0;
    uint32_t mode , timeout = 0;
    mode = bb_wait->mode;
    timeout = bb_wait->time_out * 100;
    if(INT_MODE == mode)
    {
//		WaitEvent(OsEvent_1);
//		ClearEvent(OsEvent_1);
    } else {
        while(!(BB_READ_REG(SYS_IRQ_STATUS) & 0x1) && timeout)
        {
                chip_hw_udelay(10);
                timeout--;
        }
        if(timeout) {
                BB_WRITE_REG(SYS_IRQ_CLR, BB_IRQ_CLEAR_BB_DONE);
        } else {
                ret = 2;
				//xprintf("\r\nTime out!!! during polling baseband done\r\n");
        }
    }
    return ret;
}

uint32_t baseband_wait_sam_done(BasebandDoneWait_t *bb_wait)
{
	uint32_t ret = 0;
	uint32_t mode, timeout = 0;
	mode = bb_wait->mode;
	timeout = bb_wait->time_out * 100;
	if (INT_MODE == mode)
	{
		/* TODO: Add a event for Sample Done irq */
	}
	else {
		while (!(BB_READ_REG(SYS_IRQ_STATUS) & 0x4) && timeout)
		{
			chip_hw_udelay(10);
			timeout--;
		}
		if (timeout) {
			BB_WRITE_REG(SYS_IRQ_CLR, BB_IRQ_CLEAR_SAM_DONE);
		}
		else {
			ret = 2;
			//xprintf("\r\nTime out!!! during polling baseband sample done\r\n");
		}
	}
	return ret;
}

/* MCAL added functions end */

#if BB_INTERFERENCE_CHECK
/*
 * Parameters:
 * float * nve_data : to store NVE data read from MEM_NVE
 */
static void read_nve_dat(float *nve_data, int startid, int endid)
{
        uint8_t current_frame_type = baseband_read_reg(BB_REG_FDB_SYS_BNK_ACT);
        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_NVE);
        // NVE bank start address:0x000, 0x1000, 0x2000, 0x3000
        uint32_t mem_nve_bnk_offset = 1 * (1 << 12); // 2^12 is bank address offset
        uint32_t *mem_nve_ptr = (uint32_t *)(BB_MEM_BASEADDR + current_frame_type * mem_nve_bnk_offset);
        for (int rng_index = startid; rng_index < endid; rng_index++)
        {

                uint32_t mem_nve_dat = *(mem_nve_ptr + rng_index);
                *(nve_data + rng_index) = fl_to_float(mem_nve_dat, 15, 1, false, 5, false);
        }

        baseband_switch_mem_access(old);
}

/*estimate final nve based on previous nve data and nve from 2dfft data*/
float estimate_nve(BasebandInitializationParams_t *cfg)
{
        /*calculate NVE value*/
        uint32_t fft_mem;
        uint32_t rx_num = cfg->nvarray * MAX_NUM_RX;
        uint32_t scalar = 10000000;
        uint32_t Rng_start = cfg->fft_nve_rangeindex_start;
        uint32_t Rng_end = cfg->fft_nve_rangeindex_end < NVE_LEN ? cfg->fft_nve_rangeindex_end : NVE_LEN;
        EMBARC_ASSERT(Rng_start < Rng_end);
        /*read previous nve value*/
        read_nve_dat(nve_data_previous, Rng_start, Rng_end);
        float temp_tone_scalar = 1.2;
        int max_channel = totalone(cfg->fft_nve_ch_mask);
        int rx_size = max_channel > rx_num ? rx_num : max_channel;
        float fft_nve_tone_scalar = temp_tone_scalar / rx_size;
        int fft_nve_tone_shift = 4;
        float nve_mean = 0;
        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_BUF);
        for (uint32_t tmp_rng_ind = Rng_start; tmp_rng_ind < Rng_end; tmp_rng_ind++)
        {
                uint32_t Rng_index = tmp_rng_ind - Rng_start;
                uint32_t vel_idx = 0;
                uint32_t cnt = 0;
                float pow_sum[4] = {0};
                float pow_tmp = 0;
                float temp, t1, tmp1, tmp2, t2, minus = 0;
                for (uint32_t t = 0; t < 4; t++)
                {
                        vel_idx = t * cfg->vel_nfft / 4 + 2;
                        for (uint32_t bpm_index = 0; bpm_index < cfg->nvarray; bpm_index++)
                        {
                                for (uint32_t tmp_rx_ind = 0; tmp_rx_ind < MAX_NUM_RX; tmp_rx_ind++)
                                {
                                        fft_mem = baseband_hw_get_fft_mem(tmp_rx_ind, tmp_rng_ind, vel_idx, bpm_index);
                                        complex_t complex_fft = cfl_to_complex(fft_mem, 14, 1, true, 4, false);
                                        pow_tmp = complex_fft.r * complex_fft.r + complex_fft.i * complex_fft.i;
                                        pow_sum[t] += pow_tmp;
                                }
                        }
                }
                /*sort pow_sum as from small to big*/
                quicksort(pow_sum, 0, 3);
                /*average among different channels*/
                for (int vel_idx = 0; vel_idx < 4; vel_idx++)
                {
                        temp = pow_sum[vel_idx] / fft_nve_tone_shift;
                        if (temp < pow_sum[0])
                        {
                                minus += pow_sum[vel_idx];
                                cnt++;
                        }
                }
                switch (cnt)
                {
                case 2:
                        minus = minus / 2;
                        break;
                case 3:
                        t1 = minus / 4;
                        t2 = minus / 8;
                        minus = t1 + t2;
                        break;
                case 4:
                        minus = minus / 4;
                case 1:
                default:
                        break;
                }
                minus = minus * fft_nve_tone_scalar;
                if (cfg->fft_nve_bypass)
                {
                        nve_out[Rng_index] = cfg->fft_nve_default_value;
                }
                else
                {
                        tmp1 = nve_data_previous[tmp_rng_ind] * scalar;
                        tmp2 = minus * scalar;
                        if (tmp1 > tmp2 && tmp2 != 0)
                        {
                                nve_out_buff[Rng_index] = tmp2;
                                nve_out[Rng_index] = 0;
                        }
                        else
                        {
                                nve_out_buff[Rng_index] = tmp1;
                                nve_out[Rng_index] = 0;
                        }
                }
        }
        /*moving average*/
        if (!(cfg->fft_nve_bypass))
        {
                int mov_len = 2;
                int mov_total = 2 * mov_len + 1;
                for (uint32_t tmp_rng_ind = 0; tmp_rng_ind < Rng_end - Rng_start; tmp_rng_ind++)
                {
                        if (mov_len == 0 || tmp_rng_ind < mov_len || Rng_end - Rng_start - tmp_rng_ind < mov_len)
                        {
                                nve_out[tmp_rng_ind] = nve_out_buff[tmp_rng_ind];
                        }
                        else
                        {
                                for (int i = -mov_len; i < mov_len; i++)
                                {
                                        nve_out[tmp_rng_ind] += nve_out_buff[tmp_rng_ind + i] / mov_total;
                                }
                        }
                        /*get the mean value of nve*/
                        nve_mean += nve_out[tmp_rng_ind] / (Rng_end - Rng_start + 1);
                }
        }
        baseband_switch_mem_access(old);
        return nve_mean;
}

/*for calculating ones in mask*/
static int totalone(uint16_t mask)
{
        int count = 0;
        while (mask)
        {
                count += mask & 0x1;
                mask >>= 1;
        }
        return count;
}

/* ================================baseband interference detection function definition =============================== */
/*
 * Parameters:
 * BasebandInitializationParams_t* cfg : current frame type baseband configs
 * output: result of intereference detection
 * */
static bool baseband_interference_detection(BasebandInitializationParams_t *cfg)
{
        float nve_estimation = estimate_nve(CFG);
        /*judge whether there is interference or not */
        if (nve_estimation > ((cfg->fft_nve_without_interference) * (cfg->fft_nve_threash)))
        {
                return true;
        }
        else
        {
                return false;
        }
}

/*
 * Parameters:
 * BasebandInitializationParams_t* cfg : current frame type baseband configs
 * output: result of intereference detection
 * */
// TODO: Invoke in baseband_task after bb_done before track_read
void check_interference(BasebandInitializationParams_t *cfg)
{
        baseband_interference_detection(cfg);
}
#endif // BB_INTERFERENCE_CHECK

// TODO: Dump and HIL related
// void baseband_dump_stop(void)
// {
//         bool param = baseband_stream_off_req();
//         if (param == STREAM_ON_EN) { /* data dump finish */
// #ifdef CHIP_CASCADE
//         lvds_dump_stop();
// #else

// #if (USE_50PIN_DCK_BOARD == 1)
//         /* Do nothing */
// #else
//         dbgbus_dump_stop();
// #endif //USE_50PIN_DCK_BOARD

// #endif // CHIP_CASCADE
//         baseband_switch_buf_store(NULL, SYS_BUF_STORE_FFT);
//         }
// }

// void baseband_hil_input_enable(baseband_t *bb)
// {
//         /* change the dck to the tx status */
//         dump_done();
//         chip_hw_udelay(50);

//         dmu_hil_input_mux(HIL_GPIO);
//         /* ready signal of hil sent to FPGA data collection board */
//         dbgbus_hil_ready();
// }

// void baseband_hil_input_disable(baseband_t *bb)
// {
//         dmu_hil_input_mux(HIL_AHB); /* trun off dbgbus input */
// }

// void baseband_hil_dump_enable(baseband_t *bb)
// {
//         dump_reset();  /* reset signal to fpga board */
//         chip_hw_udelay(50);
//         fmcw_radio_lvds_on(NULL, true);
// }

// void baseband_hil_dump_done(baseband_t *bb)
// {
//         dump_done();  /* done signal to  fpga board */
// }

DML_dk_value dml_dk[MAX_FRAME_TYPE]={0};

/* huangpu project add begin*/
static void gen_dml_dk_all(BasebandInitializationParams_t *cfg, uint8_t frame_type_id, int g, double delta, double delta2, antenna_pos_t * ant_pos, uint8_t sv_size, uint8_t * ant_idx) {
        // THIS FUNCTION USED TO CALCULATE THE ALL OF DK VALUE,INCLUDE SUB-ARRAY
        uint8_t ch_num[3][8];
        uint8_t size_array[3];
        
        if (cfg->doa_method != 2)
        {
                /* not DML, just return to save CPU time. */
                return;
        }

        /* initial d,k */
        size_array[0] = 8;
        size_array[1] = 4;
        size_array[2] = 4;
        ch_num[0][0] = 0;
        ch_num[0][1] = 1;
        ch_num[0][2] = 2;
        ch_num[0][3] = 3;
        ch_num[0][4] = 12;
        ch_num[0][5] = 13;
        ch_num[0][6] = 14;
        ch_num[0][7] = 15;
        ch_num[1][0] = 4;
        ch_num[1][1] = 5;
        ch_num[1][2] = 6;
        ch_num[1][3] = 7;
        ch_num[2][0] = 8;
        ch_num[2][1] = 9;
        ch_num[2][2] = 10;
        ch_num[2][3] = 11;

        int32_t  dnum = cfg->doa_npoint[g] - 1;
        double sin_step = (g == 0) ? delta : delta2;

        if ((frame_type_id == 1) || (frame_type_id == 3))
        {
                for (int line_idx = 0; line_idx < n2d_info[frame_type_id].azi_line_num; line_idx++)
                {
                        for (int idx = 0; idx < dnum; idx++)
                        {
                                complex_t d;
                                d.r = 0.0;
                                d.i = 0.0;
                                for(int ch = 0; ch < size_array[line_idx]; ch++)
                                {
                                        float tw = ant_pos[ch_num[line_idx][ch]].x* sin_step * (idx + 1);
                                        complex_t tmp = expj(2 * M_PI * tw);
                                        cadd(&tmp, &d, &d); 
                                }

                                float g_i = 1.0;
                                float k = g_i / (size_array[line_idx] * size_array[line_idx] - mag_sqr(&d));	
                                
                                dml_dk[frame_type_id].d[idx][line_idx] = d ;
                                dml_dk[frame_type_id].k[idx][line_idx] = k ;
                                /*
                                EMBARC_PRINTF("A  gen_dml_dk_all idx[%d] line_idx [%d] dr= %.5f,di= %.5f  \n\n",idx,line_idx, dml_dk_a.d[idx][line_idx].r,dml_dk_a.d[idx][line_idx].i);
                                EMBARC_PRINTF("A  gen_dml_dk_all k %.5f  \n\n",dml_dk_a.k[idx][line_idx]);
                                */
                        }
                }
        }
}

/* TODO: To be tested on Huangpu project - xdran */
void baseband_doa_init_huangpu(BasebandInitializationParams_t *cfg, uint8_t frame_type_id)
{
// #ifdef CHIP_CASCADE
//         BB_WRITE_REG(DOA_MODE_RUN, 1);
// #else
        BB_WRITE_REG(DOA_MODE_RUN, 0);
// #endif
        BB_WRITE_REG(DOA_MODE_GRP, cfg->doa_mode);
        int cfg_grp_num = 0;
        if (cfg->doa_mode==2) {            //single shot mode
                BB_WRITE_REG(DOA_NUMB_GRP, cfg->bfm_group_idx);
                BB_WRITE_REG(DOA_GRP0_TYP_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP1_TYP_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP2_TYP_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP0_MOD_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP1_MOD_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP2_MOD_SCH, 0x0);
                BB_WRITE_REG(DOA_GRP0_SIZ_STP_PKS_CRS, 0x0);
                BB_WRITE_REG(DOA_GRP1_SIZ_STP_PKS_CRS, 0x0);
                BB_WRITE_REG(DOA_GRP2_SIZ_STP_PKS_CRS, 0x0);
                BB_WRITE_REG(DOA_GRP0_NUM_SCH, 0xB);
                BB_WRITE_REG(DOA_GRP1_NUM_SCH, 0xB);
                BB_WRITE_REG(DOA_GRP2_NUM_SCH, 0xB);
                cfg_grp_num = 3;
        } else if (cfg->doa_mode == 1) {      //combined mode
                BB_WRITE_REG(DOA_NUMB_GRP, 1);
                BB_WRITE_REG(DOA_GRP0_TYP_SCH, cfg->doa_method);
                BB_WRITE_REG(DOA_GRP1_TYP_SCH, cfg->doa_method);
                cfg_grp_num = 2;      //indicate the number of group registers need to be configured
                if (cfg->doa_method == 0) {
                        baseband_dbf_init(cfg, cfg_grp_num);     //config dbf related registers
                } else {
                        baseband_dml_init(cfg, cfg_grp_num);     //config dml related registers
                        BB_WRITE_REG(DOA_GRP0_SIZ_STP_PKS_CRS, 0x0);
                        BB_WRITE_REG(DOA_GRP1_SIZ_STP_PKS_CRS, 0x0);
                        BB_WRITE_REG(DOA_GRP0_MOD_SCH, 0x1);  /* a dml restriction caused by RTL reusing implementation of dbf */
                        BB_WRITE_REG(DOA_GRP1_MOD_SCH, 0x1);  /* a dml restriction caused by RTL reusing implementation of dbf */
                }
                BB_WRITE_REG(DOA_GRP0_NUM_SCH, cfg->doa_max_obj_per_bin[0]-1);
                BB_WRITE_REG(DOA_GRP1_NUM_SCH, cfg->doa_max_obj_per_bin[1]-1);
        } else {
                BB_WRITE_REG(DOA_NUMB_GRP, cfg->doa_num_groups - 1);
                if (cfg->doa_num_groups == 1) {
                        BB_WRITE_REG(DOA_GRP0_TYP_SCH, cfg->doa_method);
                        BB_WRITE_REG(DOA_GRP0_NUM_SCH, cfg->doa_max_obj_per_bin[0]-1);
                        cfg_grp_num = 1;
                        if (cfg->doa_method == 0) {
                                baseband_dbf_init(cfg, cfg_grp_num);     //config dbf related registers
                        } else {
                                baseband_dml_init(cfg, cfg_grp_num);     //config dml related registers
                                BB_WRITE_REG(DOA_GRP0_SIZ_STP_PKS_CRS, 0x0);
                                BB_WRITE_REG(DOA_GRP0_MOD_SCH, 0x1);     /* a dml restriction caused by RTL reusing implementation of dbf */
                        }
                } else {
                        BB_WRITE_REG(DOA_GRP0_TYP_SCH, 0x0);
                        BB_WRITE_REG(DOA_GRP1_TYP_SCH, 0x0);
                        BB_WRITE_REG(DOA_GRP2_TYP_SCH, 0x0);
                        cfg_grp_num = cfg->doa_num_groups;
                        baseband_dbf_init(cfg, cfg_grp_num);
                        BB_WRITE_REG(DOA_GRP0_NUM_SCH, cfg->doa_max_obj_per_bin[0]-1);
                        BB_WRITE_REG(DOA_GRP1_NUM_SCH, cfg->doa_max_obj_per_bin[1]-1);
                        BB_WRITE_REG(DOA_GRP2_NUM_SCH, cfg->doa_max_obj_per_bin[2]-1);
                }
        }

        //below for configuring rx idx and steering vector related registers
        uint32_t old = baseband_switch_mem_access(BB_MEM_ACT_COE);
        uint32_t bfm_vec_base_offset = 0;
        double bfm_az_begin, bfm_az_end, delta;
        double bfm_ev_begin, bfm_ev_end, delta2;
        bfm_vec_base_offset = DEBPM_COE + CFAR_MIMO_COMB_COE;
        uint8_t doa_comb_num[MAX_BFM_GROUP_NUM] = {0, 0, 0};
        uint32_t doa_grp_addr_coe[MAX_BFM_GROUP_NUM] = {0, 0, 0};
        uint32_t doa_grp_rx_idx[8] = {0, 0, 0, 0, 0, 0, 0, 0};   //4 rx ant idxs stored in an IDX
        uint32_t doa_grp_base_adr = BB_REG_DOA_GRP0_ADR_COE;
        uint32_t doa_grp_adr = 0;
        uint8_t combined_2d_comb_num[4] = {1, 1, 1, 1};          //for REG_SIZ_CMB is in [0,31]
        uint8_t comb_size_v = 0;
        uint32_t combined_2d_grp_rx_idx[8] = {0}, *p, c2d_addr_coe[3] = {0};
        uint8_t combined_2d_ele_rx_idx[4] = {0};                //for generation of steering vector in elevated direction
        doa_grp_addr_coe[0] = (frame_type_id == 0 ? bfm_vec_base_offset: sv_bk[frame_type_id]);

        if (cfg->doa_mode == 1) {
                gen_siginfo_c2d(cfg, combined_2d_comb_num, &comb_size_v, combined_2d_grp_rx_idx, combined_2d_ele_rx_idx);
                //update doa_comb_num
                doa_comb_num[0] = combined_2d_comb_num[0];
                doa_comb_num[1] = comb_size_v;
                doa_comb_num[2] = 0;
                /* huangpu project add begin */
                if ((frame_type_id == 1 ) || (frame_type_id == 3)){
                        n2d_info[frame_type_id].azi_line_num = comb_size_v;
                        for (int g = 0; g < comb_size_v; g++) {
                                n2d_info[frame_type_id].azi_vector_size[g] = get_sum_idx_from_mux(cfg->combined_doa_fft_mux[g],
                                                                                   n2d_info[frame_type_id].azi_logic_channel_idx[g]);
                        }
                }
                /* huangpu project add end */
        }

        for (int g = 0; g < cfg_grp_num; g++) {
                if (cfg->doa_mode != 1) {
                        /* antenna number for current group */
                        doa_comb_num[g] = mux_map_antidx(cfg->doa_fft_mux[g], doa_grp_rx_idx);
                }

                if (g >= 1)
                {
                        doa_grp_addr_coe[g] = doa_grp_addr_coe[g-1] + cfg->doa_npoint[g-1] * ((doa_comb_num[g-1]+3)/BB_ADDRESS_UNIT) * BB_ADDRESS_UNIT;
                }
                doa_grp_adr = doa_grp_base_adr + g * GRP_ADDRESS_OFFSET;
                /* write REG COE*/
                baseband_write_reg(doa_grp_adr, doa_grp_addr_coe[g]/BB_ADDRESS_UNIT);
                doa_grp_adr = doa_grp_adr + 0x8;
                /* write REG SIZ_RNG_PKS_CRS*/
                uint32_t step = baseband_read_reg(doa_grp_adr + 0x4);
                baseband_write_reg(doa_grp_adr, cfg->doa_npoint[g]/(step + 1) - 1);
                doa_grp_adr = doa_grp_adr + 0xC;
                /* write REG CMB*/
                baseband_write_reg(doa_grp_adr, doa_comb_num[g] - 1);    //
                doa_grp_adr = doa_grp_adr + 0x4;
                /* write REG DATA_IDX*/
                if ( cfg->doa_mode == 1 && g == 0)
                {
                        p = combined_2d_grp_rx_idx;
                }
                else
                {
                        p = doa_grp_rx_idx;
                }
                for (int i = 0; i < 8; i++) {
                        baseband_write_reg(doa_grp_adr, p[i]);
                        doa_grp_adr = doa_grp_adr + 0x4;
                }
        }

        /* below configure C2D totol 6 registers  */
        c2d_addr_coe[0] = doa_grp_addr_coe[cfg_grp_num - 1] + cfg->doa_npoint[cfg_grp_num - 1] * ((doa_comb_num[cfg_grp_num - 1]+3)/BB_ADDRESS_UNIT) * BB_ADDRESS_UNIT;
        c2d_addr_coe[1] = c2d_addr_coe[0] + cfg->doa_npoint[0] * ((combined_2d_comb_num[1]+3)/BB_ADDRESS_UNIT) * BB_ADDRESS_UNIT;
        c2d_addr_coe[2] = c2d_addr_coe[1] + cfg->doa_npoint[0] * ((combined_2d_comb_num[2]+3)/BB_ADDRESS_UNIT) * BB_ADDRESS_UNIT;

        BB_WRITE_REG(DOA_C2D1_ADR_COE, c2d_addr_coe[0]/BB_ADDRESS_UNIT);
        BB_WRITE_REG(DOA_C2D2_ADR_COE, c2d_addr_coe[1]/BB_ADDRESS_UNIT);
        BB_WRITE_REG(DOA_C2D3_ADR_COE, c2d_addr_coe[2]/BB_ADDRESS_UNIT);
        BB_WRITE_REG(DOA_C2D1_SIZ_CMB, combined_2d_comb_num[1] - 1);    /* combined_2d_comb_num: combined mode */
        BB_WRITE_REG(DOA_C2D2_SIZ_CMB, combined_2d_comb_num[2] - 1);
        BB_WRITE_REG(DOA_C2D3_SIZ_CMB, combined_2d_comb_num[3] - 1);

        if (cfg->doa_samp_space == 't') {
                bfm_az_begin = cfg->bfm_az_left * DEG2RAD;
                bfm_az_end = cfg->bfm_az_right * DEG2RAD;
                bfm_ev_begin = cfg->bfm_ev_down * DEG2RAD;
                bfm_ev_end = cfg->bfm_ev_up * DEG2RAD;
        } else {
                bfm_az_begin = sin(cfg->bfm_az_left * DEG2RAD);
                bfm_az_end = sin(cfg->bfm_az_right * DEG2RAD);
                bfm_ev_begin = sin(cfg->bfm_ev_down * DEG2RAD);
                bfm_ev_end = sin(cfg->bfm_ev_up * DEG2RAD);
        }
        delta = (bfm_az_end - bfm_az_begin) / cfg->doa_npoint[0];
        delta2 = (bfm_ev_end - bfm_ev_begin) / cfg->doa_npoint[1];
        float win[MAX_ANT_ARRAY_SIZE];
        uint32_t w = 0;
        complex_t tmp_vec[MAX_ANT_ARRAY_SIZE];
        double pm1,pm2;
        uint8_t ant_idx[MAX_ANT_ARRAY_SIZE]={0};  /*virtualized by TDM va=4 tx_group=[1 16 256 4096], the related physical rx ant idx*/
        uint8_t sv_size = 0;
        uint32_t d = 0;
        bool phase_comp_in_sv = true;            /*the flag for if needed to compensate antcalib phase in steering vector*/
        for (int g = 0; g < cfg_grp_num; g++) {
                if (cfg->sv_read_from_flash == false) {
                        gen_window(cfg->doa_win, doa_comb_num[g], cfg->doa_win_params[0], cfg->doa_win_params[1], cfg->doa_win_params[2]);
                        for (w = 0; w < doa_comb_num[g]; w++)
                                win[w] = get_win_coeff_float(w);

                        sv_size = gen_ant_idx(cfg->doa_fft_mux[g], tx_order, ant_idx);
                        if ((cfg->doa_mode == 1) && (g == 1)) {
                                phase_comp_in_sv = false;
                                for (int ele_rx = 0; ele_rx < sv_size; ele_rx++) {
                                        ant_idx[ele_rx] = combined_2d_ele_rx_idx[ele_rx];
                                        //mapping logic Rx to physical Rx
                                        int t_grp = ant_idx[ele_rx] / DOA_MAX_NUM_RX + 1;
                                        int t_idx = ant_idx[ele_rx] % DOA_MAX_NUM_RX;
                                        int tx_ant_idx = group_map_tx_ant(t_grp, tx_order);
                                        ant_idx[ele_rx] = tx_ant_idx * DOA_MAX_NUM_RX + t_idx;

                                        /* huangpu peoject add begin */
                                        if ((frame_type_id == 1) || (frame_type_id == 3)){
                                                n2d_info[frame_type_id].vertical_dis[ele_rx] = ant_pos[ant_idx[ele_rx]].y;
                                        }
                                        /* huangpu peoject add end */
                                }
                        }
                        arrange_doa_win(ant_pos, ant_idx, sv_size, win, g);

                        for (d = 0; d < cfg->doa_npoint[g]; d++) {
                                gen_angle(g, d, delta, delta2, bfm_az_begin, bfm_ev_begin, &pm1, &pm2);
                                gen_steering_vec2(tmp_vec, win, ant_pos, ant_comp_phase, sv_size,
                                          pm1, pm2, cfg->doa_samp_space, cfg->com_params->bpm_mode, phase_comp_in_sv, ant_idx);
                                write_steering_vec(doa_comb_num[g], doa_grp_addr_coe[g], d, tmp_vec);
                        } //end d

                        gen_dml_dk(cfg, frame_type_id, g, delta, delta2, ant_pos, sv_size, ant_idx);

                        /* huangpu peoject add begin */
                        if (g==0)
                        {
                                gen_dml_dk_all(cfg, frame_type_id, g, delta, delta2, ant_pos, sv_size, ant_idx);
                        }
                        /* huangpu peoject add end */
        
                } else {
                        sv_size = gen_ant_idx(cfg->doa_fft_mux[g], tx_order, ant_idx);
                        if ((cfg->doa_mode == 1) && (g == 1)) {
                                phase_comp_in_sv = false;
                                for (int ele_rx = 0; ele_rx < sv_size; ele_rx++) {
                                        ant_idx[ele_rx] = combined_2d_ele_rx_idx[ele_rx];
                                        //mapping logic Rx to physical Rx
                                        int t_grp = ant_idx[ele_rx] / DOA_MAX_NUM_RX + 1;
                                        int t_idx = ant_idx[ele_rx] % DOA_MAX_NUM_RX;
                                        int tx_ant_idx = group_map_tx_ant(t_grp, tx_order);
                                        ant_idx[ele_rx] = tx_ant_idx * DOA_MAX_NUM_RX + t_idx;

                                        /* huangpu peoject add begin */
                                        if ((frame_type_id == 1) || (frame_type_id == 3) ){
                                                n2d_info[frame_type_id].vertical_dis[ele_rx] = ant_pos[ant_idx[ele_rx]].y;
                                        }
                                        /* huangpu peoject add end */

                                }
                        }
                        gen_dml_dk(cfg, frame_type_id, g, delta, delta2, ant_pos, sv_size, ant_idx);

                        /* huangpu peoject add begin */
                        if (g==0){
                                gen_dml_dk_all(cfg, frame_type_id, g, delta, delta2, ant_pos, sv_size, ant_idx);
                        }
                        /* huangpu peoject add end */

                        uint32_t offset = 0;
                        if (g == 1)
                        {
                                for (int cc = 0; cc < comb_size_v; cc++)
                                {
                                        offset += ((combined_2d_comb_num[cc]+3)/4) * 4 * cfg->doa_npoint[0];
                                }
                        }
                        write_steering_vec_from_flash(cfg->sv_ptr, offset, doa_comb_num[g], cfg->doa_npoint[g], doa_grp_addr_coe[g]);                
                }
        }//end g
        if (frame_type_id + 1 < 4)
                sv_bk[frame_type_id+1] = doa_grp_addr_coe[cfg_grp_num-1] + cfg->doa_npoint[cfg_grp_num-1] * ((doa_comb_num[cfg_grp_num-1] + 3) / 4) * 4;
        if (cfg->doa_mode == 1) {
                /* huangpu peoject add begin */
                if ((frame_type_id == 1) || (frame_type_id == 3) )
                {
                        n2d_info[frame_type_id].azi_sv_add[0] = doa_grp_addr_coe[0];
                }
                /* huangpu peoject add end */
                uint32_t offset = 0;
                /* Fix bug  end */
                for (int g = 1; g < comb_size_v; g++) {    //g = 0 is group 0 and it has been configured
                        int ch_num = combined_2d_comb_num[g];
                        int sv_num = cfg->doa_npoint[0];
                        if (cfg->sv_read_from_flash == false) {
                                gen_window(cfg->doa_win, ch_num, cfg->doa_win_params[0], cfg->doa_win_params[1], cfg->doa_win_params[2]);
                                for (w = 0; w < ch_num; w++)
                                        win[w] = get_win_coeff_float(w);
                                sv_size = gen_ant_idx(cfg->combined_doa_fft_mux[g], tx_order, ant_idx);
                                arrange_doa_win(ant_pos, ant_idx, sv_size, win, g);
                                for (d = 0; d < sv_num; d++) {
                                        gen_angle(0, d, delta, delta2, bfm_az_begin, bfm_ev_begin, &pm1, &pm2);
                                        gen_steering_vec2(tmp_vec, win, ant_pos, ant_comp_phase, sv_size,
                                                pm1, pm2, cfg->doa_samp_space, cfg->com_params->bpm_mode, true, ant_idx);
                                        write_steering_vec(ch_num, c2d_addr_coe[g - 1], d, tmp_vec);
                                }
                        } else {
                                offset += ((combined_2d_comb_num[g-1]+3)/4) * 4 * cfg->doa_npoint[0] ;
                                write_steering_vec_from_flash(cfg->sv_ptr, offset, ch_num, sv_num, c2d_addr_coe[g - 1]);
                        }
                        /* huangpu peoject add begin */
                        if ((frame_type_id == 1) || (frame_type_id == 3))
                        {
                                n2d_info[frame_type_id].azi_sv_add[g] = c2d_addr_coe[g - 1];
                        }
                        /* huangpu peoject add end */
                }
                int end_idx = comb_size_v - 1;
                if (frame_type_id + 1 < 4)
                {
                        sv_bk[frame_type_id+1] = c2d_addr_coe[end_idx-1] + cfg->doa_npoint[0] * ((combined_2d_comb_num[end_idx] + 3) / 4) * 4;
                }
        }
        baseband_switch_mem_access(old);
}

/* huangpu project add end*/

/* DDM init related function */
/* Peak position orders of different TXs in Doppler domain */
//static uint8_t ddm_tx_vel_pos[MAX_NUM_TX] = {1, 0, 0, 2};  // For Venus, change tx_group accordingly
//static uint8_t ddm_tx_vel_pos[MAX_NUM_TX] = {0, 0, 1, 2};   // For Lyra, change tx_group accordingly
static uint8_t ddm_tx_vel_pos[MAX_NUM_TX] = {1, 2, 3, 4};   // Enable all TXs' phase shifters
static uint8_t tx_on_num[MAX_FRAME_TYPE] = {0, 0, 0, 0};

uint8_t *ddm_get_ddm_tx_vel_pos()
{
        return ddm_tx_vel_pos;
}

uint8_t *ddm_get_tx_on_num()
{
        return tx_on_num;
}

void ddm_tx_pos2tx_group(BasebandInitializationParams_t* cfg) {
        uint8_t tx_order_idx = DDM_CHRP_PHAS_SEQ_PRD;
        for(uint8_t t = 0; t < MAX_NUM_TX; t++) {   // e.g. convert 'ddm_tx_vel_pos={0,0,1,2}' to 'tx_groups={4096,256,1,16}'
            if (ddm_tx_vel_pos[t] == 0) {
                    cfg->com_params->tx_groups[t] = 1u<<((tx_order_idx-1)*4);
                    tx_order_idx--;
            } else {
            		cfg->com_params->tx_groups[t] = 1u<<((ddm_tx_vel_pos[t]-1)*4);
            }
        }
}


static uint8_t init_nvarray[MAX_FRAME_TYPE];
static uint16_t init_vel_nfft[MAX_FRAME_TYPE];
static uint32_t init_tx_groups[MAX_FRAME_TYPE][MAX_NUM_TX];

// Following flags ensure that init_XXX[MAX_FRAME_TYPE] are initialized only once
static bool init_nvarray_flag[MAX_FRAME_TYPE] = {1, 1, 1, 1};
static bool init_vel_nfft_flag[MAX_FRAME_TYPE] = {1, 1, 1, 1};
static bool init_tx_groups_flag[MAX_FRAME_TYPE] = {1, 1, 1, 1};

static void write_init_nvarray(uint8_t frame_type_id, uint8_t cfg_nvarray)
{
        if (init_nvarray_flag[frame_type_id]) {
                init_nvarray[frame_type_id] = cfg_nvarray;
                init_nvarray_flag[frame_type_id] = 0;
        }
}
static void write_init_vel_nfft(uint8_t frame_type_id, uint16_t cfg_vel_nfft) {
        if (init_vel_nfft_flag[frame_type_id]) {
                init_vel_nfft[frame_type_id] = cfg_vel_nfft;
                init_vel_nfft_flag[frame_type_id] = 0;
        }
}
static void write_init_tx_groups(uint8_t frame_type_id, uint32_t* cfg_tx_groups) {
       if (init_tx_groups_flag[frame_type_id]) {
                for (uint8_t t = 0; t < MAX_NUM_TX; t++) {
                        init_tx_groups[frame_type_id][t] = cfg_tx_groups[t];
                }
                init_tx_groups_flag[frame_type_id] = 0;
        }
}

uint8_t read_init_nvarray(uint8_t frame_type_id)
{
        return init_nvarray[frame_type_id];
}

uint16_t read_init_vel_nfft(uint8_t frame_type_id)
{
        return init_vel_nfft[frame_type_id];
}
uint32_t* read_init_tx_groups(uint8_t frame_type_id)
{
        return init_tx_groups[frame_type_id];
}

void switch_to_ddm_config(BasebandInitializationParams_t* cfg, uint8_t frame_type_id)
{
        if(cfg->com_params->ddm_en)
        {
                write_init_nvarray(frame_type_id, cfg->nvarray);
                write_init_vel_nfft(frame_type_id, cfg->vel_nfft);
                write_init_tx_groups(frame_type_id, cfg->com_params->tx_groups);
                cfg->nvarray = DDM_CHRP_PHAS_SEQ_PRD;
                cfg->vel_nfft = cfg->vel_nfft/DDM_CHRP_PHAS_SEQ_PRD;
                ddm_tx_pos2tx_group(cfg);
                for (uint8_t t = 0; t < MAX_NUM_TX; t++) {
                        //xprintf("cfg->com_params->tx_groups [%d] = %d \n", t, cfg->com_params->tx_groups[t]);
                }
                reset_ant_pos_print_flag();
        }

}
void recover_from_ddm_config(BasebandInitializationParams_t* cfg, uint8_t frame_type_id)
{
        if(cfg->com_params->ddm_en)
        {
                cfg->nvarray = read_init_nvarray(frame_type_id);
                cfg->vel_nfft = read_init_vel_nfft(frame_type_id);
                uint32_t* tmp = read_init_tx_groups(frame_type_id);
                for (uint8_t t = 0; t < MAX_NUM_TX; t++) {
                	cfg->com_params->tx_groups [t] = tmp[t];
                        //xprintf("cfg->com_params->tx_groups [%d] = %d \n", t, cfg->com_params->tx_groups[t]);
                }
        }

}


