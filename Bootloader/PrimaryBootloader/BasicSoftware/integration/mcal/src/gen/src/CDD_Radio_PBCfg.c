/* CDD_Radio_PBCfg.c
 *
 *  Created on: 2022年02月07日
 *      Author: zhiyuan.gao
 */

/* includes */
#include "CDD_radio.h"
#include "CDD_Radio_Cfg.h"
#include "calterah_math.h"

RadioInitializationParams_t Radio_CfgData[MAX_FRAME_TYPE] = 
{
	{
		.ddm_en = false,
		.fmcw_startfreq = 76.0,
		.fmcw_bandwidth = 200.0,
		.fmcw_chirp_rampup = 25.0,
		.fmcw_chirp_down = 3.0,
		.fmcw_chirp_period = 30.0,
		.nchirp = 256,
		.adc_freq = 20,
		.tx_groups = {0x0,0x1,0x0,0x0},
		.bpm_mode = false,
		.phase_scramble_on = 0,
		.phase_scramble_init_state = 0xdeadbeaf,
		.phase_scramble_tap = 0xc0000401,
		.phase_scramble_comp = {180.0,180.0,180.0,180.0},
		.phase_scramble_comp_amplitude = {1.0,1.0,1.0,1.0},
		.freq_hopping_on = 0,
		.freq_hopping_init_state = 0xbabedead,
		.freq_hopping_tap = 0xc0000401,
		.freq_hopping_deltaf = 6.0,
		.chirp_shifting_on = 0,
		.chirp_shifting_init_state = 0x87654321,
		.chirp_shifting_init_tap = 0xc0000401,
		.chirp_shifting_delay = 1.0,
		.agc_mode = 0,
		.agc_tia_thres = 0.5,
		.agc_vga1_thres = 0.8,
		.agc_vga2_thres = 0.8,
		.rf_tia_gain = 2,
		.rf_vga1_gain = 5,
		.rf_vga2_gain = 3,
		.rf_hpf1 = 2,
		.rf_hpf2 = 1,
		.tx_phase_value = {45,45,45,45},
		.anti_velamb_en = false,
		.anti_velamb_delay = 10,
		.nvarray = 1,
	}
}; //ETAS BIP Porting


