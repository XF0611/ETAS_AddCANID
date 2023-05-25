#ifndef FUSA_ALPS_RADIO_H
#define FUSA_ALPS_RADIO_H
#include "Platform_Types.h"
#include "CDD_Fusa.h"

uint32 fusa_radio_spi_cmd_mode(uint32 mode);
void fusa_radio_spi_cmd_write(char addr, char data);
uint32 fusa_radio_spi_cmd_read(char addr);
char fusa_radio_reg_read( char addr);
char fusa_radio_reg_read_field( char addr, char shift, char mask);
void fusa_radio_reg_write( char addr, char data);
void fusa_radio_reg_mod( char addr, char shift, char mask, char data);
uint8 fusa_radio_switch_bank(uint8 bank);

uint8 fusa_radio_get_rtl_frame_type(void);
void fusa_fmcw_chirp_size_set(uint8 fmcw_bank, uint32 chirp_size);
uint32 fusa_fmcw_chirp_size_get(uint8 fmcw_bank);
void fusa_fmcw_lockstep_en(uint32 state);
void fusa_fmcw_lockstep_rst(uint32 state);
void fusa_fmcw_lockstep_start(void);
void fusa_fmcw_lockstep_irq_clr(void);
void fusa_fmcw_lockstep_err_inject(void);
void fusa_fmcw_lockstep_init(void);
void fusa_radio_tx_ch_on(sint32 channel_index, uint32 enable);
sint32 fusa_radio_tx_ch_status(uint8 *status_buf, sint8 len);
sint32 fusa_radio_tx_ch_restore(uint8 *status_buf, sint8 len);
void fusa_radio_dac_reg_cfg_inner(uint8 inject_num, uint8 out_num);
void fusa_radio_dac_reg_cfg_outer(void);
void fusa_radio_loop_test_en(boolean enable);
uint32 fusa_radio_get_adc_freq(void);
void fusa_radio_cpu_clk_lock_det_init(void);
void fusa_radio_cpu_clk_lock_err_inject(void);
void fusa_radar_ldo_monitor_threshold_set(fusa_config_sm1_t *thr);
void fusa_radar_ldo_monitor_threshold_get(fusa_config_sm1_t *thr);
uint8 fusa_radio_agc_en_state(uint8 frame_type);
void fusa_radio_agc_enable(uint8 frame_type, boolean enable);

/* auxadc1 */
void fusa_radio_auxadc1_voltage_on(void);
void fusa_radio_auxadc1_voltage_off(void);
uint32 fusa_radio_get_auxadc1_dout(void);
/* auxadc2 */
void fusa_radio_auxadc2_voltage_on(void);
void fusa_radio_auxadc2_ts_on(void);
void fusa_radio_auxadc2_voltage_off(void);
uint32 fusa_radio_get_auxadc2_dout(void);


typedef enum{
    SM1_THRES_2V5,
    SM1_THRES_1V3,
    SM1_THRES_1V2,
    SM1_THRES_1V1,
    SM1_THRES_1V,
}fusa_sm1_thres_level_e;

typedef enum{
    SM1_THRES_RANGE_H,
    SM1_THRES_RANGE_L,
}fusa_sm1_thres_range_e;
/* sm1 */
uint32 fusa_radio_return_ldo_monitor_irq(void);
// void fusa_radio_sm_ldo_monitor_setting(float thres_low, float thres_high, uint8 current_part);
float32 fusa_radio_sm_ldo_monitor_setting(uint8 part_num);
uint32 fusa_radio_sm_ldo_monitor_resume(uint8 part_num);
uint32 fusa_radio_sm_ldo_monitor_irq(uint8 current_part);
/* sm901 */
uint32 fusa_radio_sm_ldo_monitor_fault_injection(void);
/* sm2 */
uint32 fusa_radio_sm_avdd33_monitor_irq(void);
void fusa_radio_clear_voltage_detector_3v3(void);
void fusa_radio_start_voltage_detector_3v3(void);
uint32 fusa_radio_return_voltage_detector_3v3_irq(void);
void fusa_radio_3v3_th_set(float32 thr_h, float32 thr_l);
void fusa_radio_3v3_th_get(float32 *thr_h, float32 *thr_l);
/* sm902 */
uint32 fusa_radio_sm_avdd33_monitor_fault_injection(void);
/* sm3 */
uint32 fusa_radio_sm_dvdd11_monitor_irq(void);
void fusa_radio_clear_voltage_detector_1v1(void);
void fusa_radio_start_voltage_detector_1v1(void);
void fusa_radio_set_voltage_1v1_th(uint8 dvdd11_th_high, uint8 dvdd11_th_low);
void fusa_radio_get_voltage_1v1_th(uint8 *dvdd11_th_high, uint8 *dvdd11_th_low);
/* sm903 */
uint32 fusa_radio_sm_dvdd11_monitor_fault_injection(void);
/* sm4 */
uint32 fusa_radio_sm_vbg_monitor_irq(void);
void fusa_radio_vbg_monitor_on(boolean enable);
void fusa_radio_clear_vbg_monitor(void);
void fusa_radio_start_vbg_monitor(void);
void fmcw_radio_set_vbg_monitor_vth(uint8 vbg_th_high, uint8 vbg_th_low);
void fmcw_radio_get_vbg_monitor_vth(uint8 *vbg_th_high, uint8 *vbg_th_low);
/* sm904 */
uint32 fusa_radio_sm_vbg_monitor_fault_injection(void);
/* sm6 conflict with sm2 */
uint32 fusa_radio_sm_power_detector_irq(uint32 ch);
void fusa_radio_clear_power_detector(void);
void fusa_radio_set_power_detector_threshold(uint8 ch, double pdt_th);
/* sm905 */
uint32 fusa_radio_sm_power_detector_fault_injection(double pdt_th, uint32 ch);
/* sm11 conflict with agc */
uint32 fusa_radio_sm_if_loopback_irq(void);
void fusa_radio_if_loopback_thr_set(float32 thr_h, float32 thr_l);
void fusa_radio_if_loopback_thr_get(float32 *thr_h, float32 *thr_l);
/* sm907 conflict with agc */
uint32 fusa_radio_sm_if_loopback_fault_injection_irq(void);
/* sm12 conflict with agc */
uint32 fusa_radio_sm_rf_loopback_irq(void);
void fusa_radio_rf_loopback_thr_set(float32 thr);
void fusa_radio_rf_loopback_thr_get(float32 *thr);
/* sm908 conflict with agc */
uint32 fusa_radio_sm_rf_loopback_fault_injection_irq(void);
/* sm13 */
uint32 fusa_radio_sm_chirp_monitor_irq(void);
void fusa_radio_clear_chirp_monitor(void);
void fusa_radio_chirp_mon_thr_set(uint8 thr_h, uint8 thr_l);
void fusa_radio_chirp_mon_thr_get(uint8 *thr_h,uint8 *thr_l);
/* sm911 */
void fusa_radio_generate_fmcw(void);
void fusa_fmcw_start();
uint32 fusa_radio_sm_chirp_monitor_fault_injection(void);
/* sm14 conflict with sm2 */
uint32 fusa_radio_sm_over_temp_detector_irq(void);
void fusa_radio_temp_detector_threshold_set(float32 over_temp_th);
float32 fusa_radio_temp_detector_threshold_get(void);
/* sm910 */
uint32 fusa_radio_sm_over_temp_detector_fault_injection(void);
uint32 fusa_radio_random_num_generation(void);
#endif
