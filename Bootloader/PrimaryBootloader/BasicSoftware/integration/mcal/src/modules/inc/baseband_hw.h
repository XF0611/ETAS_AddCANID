#ifndef _BASEBAND_HW_H_
#define _BASEBAND_HW_H_

#include "baseband_reg.h"
#include "calterah_math.h"
#include "CDD_Baseband.h"
#include "Platform_Types.h"

/* CTRL CMD defines */
#define BB_CTRL_CMD_ENABLE 0x1

#define MAX_CFAR_OBJS MAX_OUTPUT_OBJS

#define DIV_RATIO(val, F_SD) ((val) * 1e3 / 8 / (F_SD) - 16) * (1L<<28)
#define INV_DIV_RATIO(val, F_SD) (1.0 * (val) / (1L<<28) + 16) * 8 * (F_SD) / 1000
#define FREQ_SYNTH_SD_RATE FMCW_SDM_FREQ /* in MHz */

typedef enum {
    BB_SAM_SINKER_BUF = 0u,     /* Store ADC to memory for data dumping */
    BB_SAM_SINKER_FFT,          /* Sink ADC directly to FFT module */
} BasebandSamSinker_e;
typedef enum {
    BB_IDLE = 0u,
    BB_AGC_BUSY,
    BB_HIL_FFT1D_BUSY,
    BB_SAM_BUSY,
    BB_RSV,
    BB_FFT1D_BUSY,
    BB_FFT2D_BUSY,
    BB_CFAR_BUSY,
    BB_DOA_BUSY,
    BB_DUMP_BUSY,
} BasebandStatus_e;

typedef struct N2D_INFO {
    uint8_t azi_line_num;
    uint8_t azi_vector_size[4];
    uint8_t azi_logic_channel_idx[4][32];
    uint32_t azi_sv_add[4];
    float   vertical_dis[4];

    /* fft2d data in fft-memory needs to be
        phase compensated when velamb is enabled */
    bool  ph_comp_flg;
    float Tr_2_PI_Over_TDN;
} n2d_info_t;

//#define BB_READ_REG(RN) baseband_read_reg(BB_REG_##RN)
//#define BB_READ_REG_FEILD(RN, FD) baseband_read_regfield(BB_REG_##RN, BB_REG_##RN##_##FD##_SHIFT, BB_REG_##RN##_##FD##_MASK)
//#define BB_WRITE_REG(RN, val) baseband_write_reg(BB_REG_##RN, val)
//#define BB_MOD_REG(RN, FD, val) baseband_mod_reg(BB_REG_##RN, val, BB_REG_##RN##_##FD##_SHIFT, BB_REG_##RN##_##FD##_MASK)
#define SYS_ENA(MASK, var) (var << SYS_ENABLE_##MASK##_SHIFT)

#define REG_H16(data)                     (data >> 16) & 0xffff
#define REG_L16(data)                     (data >>  0) & 0xffff
#define REG_L8(data)                      (data >>  0) & 0xff

#define HIL_AHB                0 // hil input from ahb bus
#define HIL_GPIO               1 // hil input from gpio(debug bus)
#define HIL_SIN_PERIOD         32  /* sine period used as hil input */


int32_t baseband_driver_info_register(BasebandInitializationParams_t *config, uint8_t cfg_cnt);
BasebandDriverInit_t* baseband_driver_info_get(uint8_t frame_id);
BasebandDriverInit_t* baseband_cur_driver_info_get(void);
void baseband_clock_init(void);
void baseband_irq_config(uint8_t irq, uint8_t en);
uint32_t baseband_read_reg(const uint32_t addr);
uint32_t baseband_read_regfield(const uint32_t addr, const uint32_t shift, const uint32_t mask);
void baseband_write_reg(const uint32_t addr, const uint32_t val);
void baseband_mod_reg(const uint32_t addr, const uint32_t val, const uint32_t shift, const uint32_t mask);
void baseband_reg_dump(void);
void baseband_tbl_dump(uint32_t tbl_id, uint32_t offset, uint32_t length);
void baseband_write_mem_table(uint32_t offset, uint32_t value);
uint32_t  baseband_read_mem_table(uint32_t offset);
uint32_t baseband_switch_mem_access(uint32_t table_id);
uint32_t baseband_switch_buf_store(BasebandSamSinker_e sinker);

int32_t baseband_hw_init(BasebandDriverInit_t *bb);

void baseband_hw_start_with_params(uint16 sys_enable, uint8_t sys_irp_en);
void baseband_hw_stop(void);
int32_t baseband_hw_dump_init(BasebandInitializationParams_t* cfg, bool sys_buf_store);
bool baseband_hw_is_running(void);
uint32_t baseband_hw_get_fft_mem(BasebandInitializationParams_t* cfg, int ant_index, int rng_index, int vel_index, int bpm_index);
void baseband_interference_mode_set(BasebandInitializationParams_t* cfg);


//void baseband_dc_calib_init(BasebandInitializationParams_t* cfg, uint8_t frame_type_id, bool leakage_ena, bool dc_calib_print_ena);
//bool baseband_bist_ctrl(bool print_ena);


//void baseband_dac_playback(baseband_hw_t *bb_hw, bool inner_circle, uint8_t inject_num, uint8_t out_num, bool adc_dbg_en, float* peak_power);
//void baseband_hil_on(baseband_hw_t *bb_hw, bool input_mux, uint8_t dmp_mux, int32_t frame_num);


void baseband_agc_dbg_reg_dump(int item);
void baseband_agc_dbg_reg_store(uint32_t *p_dbg_reg);
void baseband_datdump_smoke_test(void);
//void baseband_dbg_start(BasebandInitializationParams_t* cfg, uint8_t dbg_mux);
void baseband_frame_interleave_pattern(uint8_t frame_loop_pattern);
uint8_t baseband_get_rtl_frame_type();
uint32_t baseband_hw_get_sam_buf(int vel_idx, int rng_idx, int ant_idx);

//void baseband_interrupt_init(void);
//#if (INTER_FRAME_POWER_SAVE == 1)
//void baseband_interframe_power_save_enable(baseband_hw_t *bb_hw, bool enable);
//#endif
//void Txbf_bb_satur_monitor(baseband_hw_t *bb_hw, unsigned int tx_mux);

void baseband_combiner_reset(BasebandInitializationParams_t *cfg);
void baseband_hw_reset_after_force(void);
void baseband_agc_init(BasebandInitializationParams_t* cfg);

void baseband_compute_sys_params(BasebandInitializationParams_t* cfg, BasebandSystemParams_t *sys_params);

bool bit_parse(uint32_t num, uint16_t bit_mux[]);
bool get_tdm_tx_antenna_in_chirp(uint32_t patten[], uint8_t chip_idx, uint32_t chirp_tx_mux[]);
int8_t get_bpm_tx_antenna_in_chirp(uint32_t patten[], uint8_t chip_idx, uint32_t chirp_tx_mux[]);

/* For test cli */
void baseband_bnk_set(uint8_t frame_type_id);
void baseband_sys_init(BasebandInitializationParams_t* cfg, BasebandSystemParams_t *sys_params);
void baseband_agc_init(BasebandInitializationParams_t* cfg);
void baseband_amb_init(BasebandInitializationParams_t* cfg);
void baseband_debpm_init(BasebandInitializationParams_t* cfg, uint8_t frame_type_id);
void baseband_spk_init(BasebandInitializationParams_t* cfg);
void baseband_sam_init(BasebandInitializationParams_t* cfg, uint8_t frame_type_id);
void baseband_fft_init(BasebandInitializationParams_t* cfg, uint8_t frame_type_id);
void baseband_interference_init(BasebandInitializationParams_t* cfg);
void baseband_velamb_cd_init(BasebandInitializationParams_t* cfg, BasebandSystemParams_t * sys_param, uint8_t frame_type_id);
void baseband_cfar_init(BasebandInitializationParams_t* cfg);
void baseband_doa_init(BasebandInitializationParams_t* cfg, uint8_t frame_type_id);
void baseband_shadow_bnk_init(BasebandInitializationParams_t* cfg);
void baseband_dc_calib_set_cfg(uint8_t frame_type_id);
void baseband_dc_calib_cal_dc(BasebandInitializationParams_t* cfg);
void baseband_dc_calib_rst_cfg(BasebandInitializationParams_t* cfg, bool leakage_ena);
uint8_t baseband_frame_interleave_cfg(uint8_t frame_type_id);
int16_t* get_dc_offset();
BasebandStatus_e baseband_hw_status_check(void);
int32_t baseband_hw_wait_for_bb_done(uint16_t sys_enable, uint8_t sys_irq_en);
uint32_t baseband_wait_bb_done(BasebandDoneWait_t *bb_wait);
uint32_t baseband_wait_sam_done(BasebandDoneWait_t *bb_wait);
uint8_t baseband_switch_bnk_act(uint8_t bnk_id);

void baseband_hil_on_ahb(BasebandDriverInit_t *bb);
void radar_param_fft2rv_nowrap(const BasebandSystemParams_t *sys_params, const float k, const float p, float *r, float *v);
uint32_t baseband_get_obj_info(uint32_t offset);
uint32_t baseband_int_clear(uint32_t type);
n2d_info_t *baseband_get_n2d_info(uint8_t frame_id);

/* huangpu peoject add end */
void baseband_doa_init_huangpu(BasebandInitializationParams_t *cfg, uint8_t frame_type_id);
#define doa_npoint_num 359
#define ALN 4   //azi_line_num
typedef struct DML_DK {
	complex_t d[doa_npoint_num][ALN];
	float k[doa_npoint_num][ALN];
} DML_dk_value;
/* huangpu peoject add end */

uint8_t *ddm_get_ddm_tx_vel_pos();
uint8_t *ddm_get_tx_on_num();
void ddm_tx_pos2tx_group(BasebandInitializationParams_t* cfg);
void switch_to_ddm_config(BasebandInitializationParams_t* cfg, uint8_t frame_type_id);
void recover_from_ddm_config(BasebandInitializationParams_t* cfg, uint8_t frame_type_id);
uint8_t read_init_nvarray(uint8_t frame_type_id);
uint16_t read_init_vel_nfft(uint8_t frame_type_id);
uint32_t* read_init_tx_groups(uint8_t frame_type_id);
#endif // _BASEBAND_HW_H_


