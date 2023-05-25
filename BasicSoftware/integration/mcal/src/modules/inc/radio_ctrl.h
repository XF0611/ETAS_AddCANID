#ifndef _RADIO_CTRL_H_
#define _RADIO_CTRL_H_

#ifdef UNIT_TEST
#include "calterah_unit_test.h"
#else
#include "embARC_toolchain.h"
#endif

#include "CDD_radio.h"

typedef enum {
        CH_ALL = -1,
        CH0    = 0,
        CH1    = 1,
        CH2    = 2,
        CH3    = 3,
} CHANNEL_INDEX_t;

typedef enum{
        TX_POWER_INDEX0 = 0,
        TX_POWER_INDEX1 = 1,
        TX_POWER_INDEX2 = 2,
        TX_POWER_INDEX3 = 3,
} TX_POWER_t;

/* temp sensor related enumeration */
typedef enum {
        T_SENSOR_K = 0,
        T_SENSOR_D = 1,
        T_SENSOR_T = 2,
} TS_t;

typedef enum {
        NORM_TO_TEST = 0,
        TEST_TO_NORM = 1,
} LvdsMode_e;

#define TX_POWER_DEFAULT       0xAA
#define TX_POWER_MAX           0xFF
#define TX_POWER_MAX_SUB5      0x88
#define TX_POWER_MAX_SUB10     0x00

// Higher byte for IDAC Sign and IDAC Amplitude, lower byte for QDAC Sign and QDAC Amplitude
#define TX_PHASE_0       0x0f0fu
#define TX_PHASE_45      0x000fu
#define TX_PHASE_90      0x1f0fu
#define TX_PHASE_135     0x1f00u
#define TX_PHASE_180     0x1f1fu
#define TX_PHASE_225     0x001fu
#define TX_PHASE_270     0x0f1fu
#define TX_PHASE_315     0x0f00u

#define AUXADC1_MainBG_VPTAT     0x00
#define AUXADC1_TestMUXN         0x01
#define AUXADC1_TestMUXP         0x10
#define AUXADC1_TPANA1           0x11

#define AUXADC2_TS_VPTAT     0x00
#define AUXADC2_TS_VBG       0x01
#define AUXADC2_TestMUXN     0x10
#define AUXADC2_TPANA2       0x11

#define RADIO_WRITE_BANK_REG(bk, addr, val) fmcw_radio_reg_write(RADIO_BK##bk##_##addr, val)
#define RADIO_WRITE_BANK_CH_REG(bk, ch, addr, val) fmcw_radio_reg_write((ch == 0) ? RADIO_BK##bk##_CH0_##addr : \
                                                                        (ch == 1) ? RADIO_BK##bk##_CH1_##addr : \
                                                                        (ch == 2) ? RADIO_BK##bk##_CH2_##addr : \
                                                                        RADIO_BK##bk##_CH3_##addr, \
                                                                        val)

#define RADIO_MOD_BANK_FWCW_TX_REG(bk, tx, addr, field, val) \
        fmcw_radio_reg_mod((tx == 0) ? RADIO_BK##bk##_FMCW_TX0_##addr :      \
                           (tx == 1) ? RADIO_BK##bk##_FMCW_TX1_##addr :      \
                           (tx == 2) ? RADIO_BK##bk##_FMCW_TX2_##addr :      \
                           RADIO_BK##bk##_FMCW_TX3_##addr,                   \
                           (tx == 0) ? RADIO_BK##bk##_FMCW_TX0_##addr##_##field##_SHIFT :      \
                           (tx == 1) ? RADIO_BK##bk##_FMCW_TX1_##addr##_##field##_SHIFT :      \
                           (tx == 2) ? RADIO_BK##bk##_FMCW_TX2_##addr##_##field##_SHIFT :      \
                           RADIO_BK##bk##_FMCW_TX3_##addr##_##field##_SHIFT,  \
                           (tx == 0) ? RADIO_BK##bk##_FMCW_TX0_##addr##_##field##_MASK :      \
                           (tx == 1) ? RADIO_BK##bk##_FMCW_TX1_##addr##_##field##_MASK :      \
                           (tx == 2) ? RADIO_BK##bk##_FMCW_TX2_##addr##_##field##_MASK :      \
                           RADIO_BK##bk##_FMCW_TX3_##addr##_##field##_MASK,  \
                           val)

#define RADIO_READ_BANK_REG(bk, addr) fmcw_radio_reg_read(RADIO_BK##bk##_##addr)
#define RADIO_READ_BANK_CH_REG(bk, ch, addr) fmcw_radio_reg_read((ch == 0) ? RADIO_BK##bk##_CH0##_##addr : \
                                                                 (ch == 1) ? RADIO_BK##bk##_CH1##_##addr : \
                                                                 (ch == 2) ? RADIO_BK##bk##_CH2##_##addr : \
                                                                 RADIO_BK##bk##_CH3##_##addr)

#define RADIO_MOD_BANK_REG(bk, addr, field, val) fmcw_radio_reg_mod(RADIO_BK##bk##_##addr, \
                                                               RADIO_BK##bk##_##addr##_##field##_SHIFT, \
                                                               RADIO_BK##bk##_##addr##_##field##_MASK, val)

uint32_t phase_val_2_reg_val(uint32_t phase_val);
uint32_t reg_val_2_phase_val(uint32_t reg_val);

uint32_t radio_spi_cmd_mode(uint32_t mode);
void radio_spi_cmd_write(char addr, char data);
uint32_t radio_spi_cmd_read(char addr);

char fmcw_radio_reg_read( char addr);
char fmcw_radio_reg_read_field( char addr, char shift, char mask);
void fmcw_radio_reg_write( char addr, char data);
void fmcw_radio_reg_mod( char addr, char shift, char mask, char data);
void fmcw_radio_reg_dump(char bank_index);

void fmcw_radio_power_on(RadioInitializationParams_t *cfg);
void fmcw_radio_power_off(void);
uint8_t fmcw_radio_switch_bank( uint8_t bank);
void fmcw_radio_rx_buffer_on(void);
void fmcw_radio_ldo_on(uint32 *tx_groups);
void fmcw_radio_refpll_on(void);
int32_t fmcw_radio_pll_on(void);
void fmcw_radio_lo_on( bool enable);
void fmcw_radio_tx_on(void);
int32_t fmcw_radio_rx_on(bool enable,RadioInitializationParams_t *cfg);
int32_t fmcw_radio_adc_on(uint8 adc_freq);
int32_t fmcw_radio_do_refpll_cal(void);
int32_t fmcw_radio_do_pll_cal( uint32_t lock_freq);
bool fmcw_radio_is_refpll_locked(void);
bool fmcw_radio_is_pll_locked(void);
void fmcw_radio_frame_interleave_pattern( uint8_t frame_loop_pattern);
void fmcw_radio_frame_type_reset(void);
void fmcw_radio_fmcw_pll_hw_lock(void);
int32_t fmcw_radio_fmcw_pll_sw_lock(void);
int32_t fmcw_radio_generate_fmcw(RadioFrameParams_t *radio,RadioInitializationParams_t *cfg);
void fmcw_radio_start_fmcw(RadioFrameParams_t *radio,RadioInitializationParams_t *cfg);
void fmcw_radio_stop_fmcw(RadioInitializationParams_t *cfg);
int32_t fmcw_radio_single_tone(uint32_t start_freq,double freq, bool enable);
void fmcw_radio_DDM_cmd_cfg(RadioFrameParams_t *radio, RadioInitializationParams_t *cfg);

void fmcw_radio_set_tia_gain( int32_t channel_index, int32_t gain);
int32_t fmcw_radio_get_tia_gain( int32_t channel_index);
void fmcw_radio_set_vga1_gain( int32_t channel_index, int32_t gain);
int32_t fmcw_radio_get_vga1_gain( int32_t channel_index);
void fmcw_radio_set_vga2_gain( int32_t channel_index, char gain);
int32_t fmcw_radio_get_vga2_gain( int32_t channel_index);
void fmcw_radio_set_tx_status(RadioInitializationParams_t* cfg,int32_t channel_index, char status);
int32_t fmcw_radio_get_tx_status( int32_t channel_index);
void fmcw_radio_set_tx_power( int32_t channel_index, char power_index);
int32_t fmcw_radio_get_tx_power( int32_t channel_index);
void fmcw_radio_set_tx_phase( int32_t channel_index, int32_t phase_index);
int32_t fmcw_radio_get_tx_phase( int32_t channel_index);
float fmcw_radio_get_temperature(void);
void fmcw_radio_gain_compensation(void);
int32_t fmcw_radio_vctrl_on(void);
int32_t fmcw_radio_vctrl_off(void);
void fmcw_radio_if_output_on( int32_t channel_index, bool enable);
void fmcw_radio_if_output_off( int32_t channel_index);
void fmcw_radio_tx_ch_on( int32_t channel_index, bool enable);
void fmcw_radio_rc_calibration(void);
void fmcw_radio_adc_cmp_calibration(uint8 adc_freq);
void fmcw_radio_set_hpf1( int32_t channel_index, int32_t filter_index);
int32_t fmcw_radio_get_hpf1( int32_t channel_index);
void fmcw_radio_set_hpf2( int32_t channel_index, int32_t filter_index);
int32_t fmcw_radio_get_hpf2( int32_t channel_index);
void fmcw_radio_dc_reg_cfg(uint8_t frame_type_id,int32_t channel_index, int16_t dc_offset);
void fmcw_radio_dac_reg_cfg_outer(void);
void fmcw_radio_dac_reg_cfg_inner( uint8_t inject_num, uint8_t out_num);
void fmcw_radio_dac_reg_restore(void);
void fmcw_radio_tx_restore(uint32_t *tx_groups);
void fmcw_radio_agc_enable(uint8_t frame_type_id,bool enable);
void fmcw_radio_agc_setup(RadioInitializationParams_t *cfg);
void fmcw_radio_special_mods_off(uint8_t frame_type_id);
int32_t fmcw_radio_pll_clock_en(void);
void fmcw_radio_hp_auto_ch_on( int32_t channel_index);
void fmcw_radio_hp_auto_ch_off( int32_t channel_index);
void fmcw_radio_tx_auto_ch_on( int32_t channel_index, bool enable);
void fmcw_radio_set_auto_tx_mode( int32_t channel_index, int32_t mode_sel);
void fmcw_radio_sdm_reset(RadioInitializationParams_t* cfg);
void fmcw_radio_vam_status_save(uint8_t frame_type_id);
void fmcw_radio_vam_disable(uint8_t frame_type_id);
void fmcw_radio_vam_status_restore(uint8_t frame_type_id);
void fmcw_radio_cmd_cfg(RadioFrameParams_t *radio,RadioInitializationParams_t *cfg,bool enable);
void fmcw_radio_frame_interleave_config( uint32_t fil_que, uint8_t fil_prd);
void fmcw_radio_auxadc_trim(void);
uint32_t fmcw_radio_rfbist_trim(void);
void fmcw_radio_temp_sensor_trim(void);
uint8_t fmcw_radio_part_number(void);
int32_t fmcw_radio_lvds_on( bool enable);
void fmcw_radio_lvds_mode(LvdsMode_e lvdsMode);
void pll_clock_init (void);
void fmcw_radio_clk_out_for_cascade(void);
float fmcw_radio_auxadc1_voltage( int32_t muxin_sel);
float fmcw_radio_auxadc2_voltage( int32_t muxin_sel);
float auxadc_read(int32_t mode);
void auxadc_enable(int32_t mode,  int32_t muxin_sel);
void fmcw_radio_txphase_status( bool save);
bool fmcw_radio_is_tx_ch_on(int32_t channel_index);
bool fmcw_radio_is_rx_on(void);
bool fmcw_radio_is_adc_on(void);

/* FUNC_SAFETY related function start */
// #ifdef FUNC_SAFETY
#if 0
uint8_t fmcw_radio_sm_ldo_monitor_IRQ( uint8_t ldo_part);
uint8_t fmcw_radio_sm_ldo_monitor_fault_injection_IRQ(void);
uint8_t fmcw_radio_sm_avdd33_monitor_IRQ( bool fault_injection);
uint8_t fmcw_radio_sm_dvdd11_monitor_IRQ( bool fault_injection);
uint8_t fmcw_radio_sm_bg_monitor_IRQ( bool fault_injection);
uint8_t fmcw_radio_sm_rfpower_detector_fault_injection_IRQ( double power_th);
uint8_t fmcw_radio_sm_rfpower_detector_IRQ(void);
uint8_t fmcw_radio_sm_if_loopback_IRQ( bool fault_injection);
uint8_t fmcw_radio_sm_rf_loopback_IRQ( bool fault_injection);
uint8_t fmcw_radio_sm_chirp_monitor_fault_injection_IRQ(void);
uint8_t fmcw_radio_sm_chirp_monitor_IRQ(void);
uint8_t fmcw_radio_sm_over_temp_detector_IRQ( bool fault_injection);
uint32_t fmcw_radio_check_txon_channel(void);
uint32_t fmcw_radio_auxadc2_dout(void);
uint32_t fmcw_radio_random_num_generation(void);
uint32_t fmcw_get_pa_dout( int32_t channel_index, double freq, double power_th);
void fmcw_radio_sm_fault_injection(void);
void fmcw_radio_sm_ldo_monitor_setting( uint8_t ldo_part);
void fmcw_radio_sm_ldo_monitor_threshold(void);
void fmcw_radio_sm_ldo_monitor_IRQ_resume(void);
void fmcw_radio_sm_avdd33_monitor_setting(void);
void fmcw_radio_sm_avdd33_monitor_threshold( bool fault_injection);
void fmcw_radio_sm_dvdd11_monitor_setting(void);
void fmcw_radio_sm_dvdd11_monitor_threshold( bool fault_injection);
void fmcw_radio_sm_bg_monitor_setting(void);
void fmcw_radio_sm_bg_monitor_threshold( bool fault_injection);
void fmcw_radio_sm_rfpower_detector_setting( int32_t channel_index);
void fmcw_radio_sm_rfpower_detector_fault_injection_threshold(void);
void fmcw_radio_sm_rfpower_detector_threshold( double freq, double power_th, int32_t channel_index);
void fmcw_radio_sm_rf_power_detector_IRQ_resume(void);
void fmcw_radio_pdt_reg_resume( int32_t channel_index);
void fmcw_radio_rf_loopback_reg_resume(void);
void fmcw_radio_sm_chirp_monitor_setting(void);
void fmcw_radio_sm_chirp_monitor_fault_injection( bool fault_injection);
void fmcw_radio_sm_chirp_monitor_IRQ_resume(void);
void fmcw_radio_sm_over_temp_detector_setting(void);
void fmcw_radio_sm_over_temp_detector_threshold( bool fault_injection);
void fmcw_radio_sm_over_temp_detector_IRQ_resume(void);
uint8_t fmcw_radio_sm_saturation_detector_fault_injection_IRQ(void);
#endif
/* FUNC_SAFETY related function end */

int32_t fmcw_radio_adc_ldo_on( bool enable);
void fmcw_radio_loop_test_en( bool enable);
void fmcw_radio_txlobuf_on(uint32 *tx_groups);
float fmcw_radio_rf_comp_code(void);
void fmcw_doppler_move(RadioFrameParams_t *radio,uint32_t* opt);
void fmcw_radio_pdt_reg( int8_t pdt_type, int32_t channel_index);
double fmcw_radio_rf_power_detector(RadioFrameParams_t *radio,RadioInitializationParams_t *cfg,int32_t channel_index, double freq);
void fmcw_radio_reset(void);
void fmcw_radio_reboot_cause_set(uint32_t cause);
uint32_t fmcw_radio_reboot_cause(void);
void fmcw_radio_refpll_cbank( float temp);
int32_t fmcw_radio_pll_recal(void);
void radio_interframe_power_save_enable(RadioInitializationParams_t *cfg, bool tx_en, bool rx_en, bool enable);
void fmcw_radio_compute_reg_value(RadioFrameParams_t *radio,RadioInitializationParams_t* cfg);

/* rf compensation related function start */
#if RF_COMP == 1
float fmcw_radio_rf_comp_code(void);
#endif
/* rf compensation related function end */

#define CMD_CYCLE_MARGIN 20

#define REG_L(data)                       (data >>  0) & 0xff
#define REG_M(data)                       (data >>  8) & 0xff
#define REG_H(data)                       (data >> 16) & 0xff
#define REG_INT(data)                     (data >> 24) & 0xff
#define REG_H7(data)                      (data >>  8) & 0x7f

#if REFPLL_CBANK == 1
#define REFPLL_CBANK_BOOT_TEMP          30
#endif

/* For baseband_dc_calib_init */
//void radio_dc_calib_status_save(uint8_t frame_type_id);
//void radio_dc_calib_status_restore(uint8_t frame_type_id);
void radio_dc_calib_set_cfg(uint8_t frame_type_id);
void radio_dc_calib_wr_dc(uint8_t frame_type_id, int16_t *dc_offset);
void radio_dc_calib_rst_cfg(uint8_t frame_type_id);
void radio_dc_calib_setting(void);
bool fmcw_radio_is_running(uint8_t frame_type_id);

#endif // _RADIO_CTRL_H_
