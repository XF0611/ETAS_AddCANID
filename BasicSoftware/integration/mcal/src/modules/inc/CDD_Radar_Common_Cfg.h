#ifndef SRC_MCAL_INC_CDD_RADAR_COMMON_CFG_H_
#define SRC_MCAL_INC_CDD_RADAR_COMMON_CFG_H_

#include <stdbool.h>
#include "common.h"
#include "calterah_limits.h"

/*!
    struct  RadarCommonInitializationParams_t
*/
typedef struct
{
    bool ddm_en;                      		/* DDM enable */
    uint8_t adc_freq;                  	    /* ADC sampling freq in MHz */
    int8_t agc_mode;                   	    /* AutoGainCtrl mode on/off */
    float agc_tia_thres;              		/* threshold for saturation decision of TIA */
    float agc_vga1_thres;             		/* threshold for saturation decision of VGA1 */
    float agc_vga2_thres;             		/* threshold for saturation decision of VGA2 */
    bool anti_velamb_en;           			/* velocity de-ambiguity (chirp delay mode) */
    uint8_t anti_velamb_delay;         	    /* velocity de-ambiguity: delay between the extra chirp and normal chirp in virtual group 0 */
    bool bpm_mode;                 			/* BPM mode */
    float chirp_shifting_delay;      		/* chirp shifting mode delay, us */
    uint8_t chirp_shifting_on;          	/* chirp shifting mode */
    uint32_t chirp_shifting_init_state; 	/* chirp shifting mode init state */
    uint32_t chirp_shifting_init_tap;   	/* chirp shifting mode tap  value */
    uint8_t freq_hopping_on;            	/* frequency hopping mode */
    uint32_t freq_hopping_init_state;   	/* frequency hopping mode init state */
    uint32_t freq_hopping_tap;          	/* frequency hopping mode tap value */
    float freq_hopping_deltaf;        		/* frequency hopping, delta frequency, MHz */
    double fmcw_startfreq;            		/* start frequency in GHz */
    double fmcw_bandwidth;            		/* bandwidth in MHz */
    float fmcw_chirp_rampup;          		/* chirp ramp up time in us */
    float fmcw_chirp_down;            		/* chirp ramp down time in us */
    float fmcw_chirp_period;          		/* chirp period in us */
    int16_t nchirp;                    		/* num of chirps per frame */
    uint32_t nvarray;                   	/* put nvarray here to avoid tx_group config conflict to nvarray */
    uint8_t phase_scramble_on;          	/* phase scramble mode */
    uint32_t phase_scramble_init_state; 	/* phase scramble mode init state */
    uint32_t phase_scramble_tap;        	/* phase scramble mode tap value */
    float phase_scramble_comp[4];           /* phase compensation angle */
    float phase_scramble_comp_amplitude[4]; /* compensation amplitude */
    uint8_t rf_tia_gain;                    /* TIA gain */
    uint8_t rf_vga1_gain;                   /* VGA1 gain */
    uint8_t rf_vga2_gain;                   /* VGA2 gain */
    uint8_t rf_hpf1;                        /* Highpass 1 setting */
    uint8_t rf_hpf2;                    	/* Highpass 2 setting */
    uint32_t tx_groups[MAX_FRAME_TYPE]; 	/* each element in tx_groups denotes the tx_antenna work state in four-chirp period. First nibble (LSB) denotes the first chirp, forth nibble denotes the forth chirp. Nibble value: 0:off, 1: in phase,  2: opposite phase, 3 and above: reserved */
    uint16_t tx_phase_value[4];         	/* Tx phase value for each Channel - 0 / 22 / 45 / 67 / 90 / 112 / 135 / 157 / 180 / 202 / 225 / 247 / 270 / 292 / 315 / 337 */
    int16_t* dc_offset;
    uint8_t* p_tx_on_num;
    uint8_t* p_ddm_tx_vel_pos;
} RadarCommonInitializationParams_t;

#endif
