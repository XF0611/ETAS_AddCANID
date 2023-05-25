
#include "embARC_error.h"
#include "Std_Types.h"
#include "calterah_math.h"
#include "CDD_radio.h"
#include "math.h"
//#include "radio_ctrl.h"
#include "calterah_math.h"
#include "radio_ctrl.h"

void fmcw_radio_compute_reg_value(RadioFrameParams_t *radio,RadioInitializationParams_t* cfg)
{
        uint32 gcd = compute_gcd(cfg->adc_freq, FREQ_SYNTH_SD_RATE);
        uint32 sd_M = FREQ_SYNTH_SD_RATE / gcd;
        uint32 total_cycle = round(cfg->fmcw_chirp_period * gcd) * sd_M;
        radio->nchirp = cfg->nchirp;
        if (cfg->anti_velamb_en)
                radio->nchirp += cfg->nchirp / cfg->nvarray;
        radio->start_freq = DIV_RATIO(cfg->fmcw_startfreq, FREQ_SYNTH_SD_RATE);
        uint32 stop_freq = DIV_RATIO(cfg->fmcw_startfreq + cfg->fmcw_bandwidth * 1e-3, FREQ_SYNTH_SD_RATE);
        uint32 step_up = (1L<<28) * cfg->fmcw_bandwidth / (FREQ_SYNTH_SD_RATE * cfg->fmcw_chirp_rampup * FREQ_SYNTH_SD_RATE * 8);
        uint32 bandwidth = stop_freq - radio->start_freq;
        uint32 up_cycle = ceil(1.0 * bandwidth / step_up);
        uint32 step_down = (1L<<28) * cfg->fmcw_bandwidth / (FREQ_SYNTH_SD_RATE * cfg->fmcw_chirp_down * FREQ_SYNTH_SD_RATE * 8);
        uint32 down_cycle = ceil(1.0 * bandwidth / step_down);
        uint32 wait_cycle = total_cycle - up_cycle - down_cycle;
        uint32 anti_velamb_cycle = round(cfg->anti_velamb_delay * gcd) * sd_M; //cycle alignment for anti-velamb
        uint32 chirp_shifting_cyle = round(cfg->chirp_shifting_delay * gcd) * sd_M; //cycle alignment for chirp shifting

        /*Added for frequency hopping*/
        radio->hp_start_freq = DIV_RATIO(cfg->fmcw_startfreq + cfg->freq_hopping_deltaf * 1e-3, FREQ_SYNTH_SD_RATE);
        radio->hp_stop_freq = radio->hp_start_freq + bandwidth;
        radio->hp_mid_freq = radio->hp_start_freq + bandwidth / 2;

        bandwidth = step_up * up_cycle;
        radio->stop_freq = radio->start_freq + bandwidth;
        radio->mid_freq = radio->start_freq + bandwidth / 2;
        radio->step_up = step_up;
        radio->step_down = step_down;
        radio->up_cycle = up_cycle;
        radio->down_cycle = down_cycle;
        radio->wait_cycle = wait_cycle;
        radio->total_cycle = total_cycle;
        radio->cnt_wait = radio->wait_cycle + radio->down_cycle;
        radio->anti_velamb_cycle = anti_velamb_cycle;
        radio->chirp_shifting_cyle = chirp_shifting_cyle;

/* move doppler spur */
#if DOPPLER_MOVE == 1
		uint32_t doppler_move_opt[4] = {0};
        fmcw_doppler_move(radio,doppler_move_opt);
        if (doppler_move_opt[0] && (cfg->freq_hopping_on == 0) && (cfg->chirp_shifting_on == 0)) {
                radio->step_down = doppler_move_opt[1];
                radio->down_cycle = doppler_move_opt[2];
                radio->wait_cycle = doppler_move_opt[3];
        } else {
                EMBARC_PRINTF("no optimized doppler position found");
        }
#endif
}

sint32 fmcw_radio_init(RadioFrameParams_t *radio,RadioInitializationParams_t* cfg)
{
        fmcw_radio_power_on(cfg);
#if HTOL_TEST == 1
        fmcw_radio_gain_compensation();
#endif
        fmcw_radio_compute_reg_value(radio,cfg);
#ifdef CHIP_CASCADE
        /* For cascade, only master side need to generate fmcw */
        if (chip_cascade_status() == CHIP_CASCADE_MASTER) {
                if (E_OK != fmcw_radio_generate_fmcw(radio,cfg)) {
                fmcw_radio_reset();
              }
        }
#else
        if(E_OK !=  fmcw_radio_generate_fmcw(radio,cfg)) {
                fmcw_radio_reset();
        }
#endif
        return E_OK;
}

void fmcw_radio_start(RadioFrameParams_t *radio,RadioInitializationParams_t *cfg)
{
	fmcw_radio_start_fmcw(radio,cfg);
}

void fmcw_radio_stop(RadioInitializationParams_t *cfg)
{
	fmcw_radio_stop_fmcw(cfg);
}
