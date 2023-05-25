#include "fusa_alps_radar.h"
#include "fusa_alps_radio.h"
#include "fusa_alps.h"
#include "alps_mp_radio_reg.h"
#include "alps_mp_radio_spi_cmd_reg.h"
#include "fusa_alps_dmu.h"
#include "fusa_alps_basband.h"
#include "baseband_alps_FM_reg.h"
#include "Platform_Types.h"
#include "common.h"
#include "alps_clock.h"
#include "fusa_alps_emu.h"
#include "CDD_Fusa.h"
#include "CDD_radio.h"
#include "CDD_Baseband.h"
#include "calterah_limits.h"
#include <math.h>
#include "calterah_data_conversion.h"

#define ABIST_SIN_FREQ     5       // analog bist sinewave frequency, 5MHz defaultly
#define ABIST_FFT_CHK_MIN  3       // avoid zero frequency, DC may be greater than sinewave
#define ABIST_FFT_CHK_SIZE 31
#define ABIST_FFT_CHK_HLF  ((ABIST_FFT_CHK_SIZE - 1)/2)

static float32 consistence_check_threshold = (5.0); //default ±6db


void fusa_radar_consistence_threshold_set(float32 threshold)
{
    consistence_check_threshold = threshold;
}
float32 fusa_radar_consistence_threshold_get()
{
    return consistence_check_threshold;
}
sint32 fusa_radar_gain_consistence_check()
{
    sint32 ret = E_FUSA_OK;
    uint8 old_buf_store =fusa_bb_switch_buf_store(SYS_BUF_STORE_ADC);
    uint8 cur_bank = raw_readl(BB_REG_FDB_SYS_BNK_ACT); /* read back the bank selected in RTL */
    uint16 old_status_en = raw_readl(BB_REG_SYS_ENABLE);
    uint32 old_irq_state = fusa_dmu_irq_enabled(FUSA_BB_IRQ_DONE);
    uint8 tx_status[5] = {0};
    //save tx status
    fusa_radio_tx_ch_status(tx_status, 5);
    fusa_dmu_irq_en(FUSA_BB_IRQ_DONE,FALSE);

    uint8 old_bank = fusa_radio_switch_bank(5 + cur_bank);
    uint32 old_chirp_size_0 = fusa_radio_reg_read(RADIO_BK5_FMCW_CHIRP_SIZE_1_0);
    uint32 old_chirp_size_1 = fusa_radio_reg_read(RADIO_BK5_FMCW_CHIRP_SIZE_1_1);
    uint32 old_bb_sys_size_vel = raw_readl(BB_REG_SYS_SIZE_VEL_BUF);
    uint32 old_bb_sys_size_fft = raw_readl(BB_REG_SYS_SIZE_VEL_FFT);
    uint32 old_bb_sys_bpm = raw_readl(BB_REG_SYS_SIZE_BPM);

    fusa_radio_reg_write(RADIO_BK5_FMCW_CHIRP_SIZE_1_0, 1);
    fusa_radio_reg_write(RADIO_BK5_FMCW_CHIRP_SIZE_1_1, 0);
    raw_writel(BB_REG_SYS_SIZE_VEL_BUF, 0);
    raw_writel(BB_REG_SYS_SIZE_VEL_FFT, 0);
    raw_writel(BB_REG_SYS_SIZE_BPM, 0);

    /* turn off zero doppler removel */
    //bool old_zer_dpl = BB_READ_REG(bb_hw, FFT_ZER_DPL_ENB);
    //BB_WRITE_REG(bb_hw, FFT_ZER_DPL_ENB  , 0);
    //dmu_adc_reset(); // ADC should reset in cascade

    // close all tx
    fusa_radio_tx_ch_on(-1, FALSE);

    BasebandDriverContext_t bb_driver = {
        .sys_en = (1 << SYS_ENABLE_SAM_SHIFT),
        .sys_irq_en = FALSE,
    };
    baseband_run(&bb_driver);

    RadioDriverContext_t radio_driver = {
        .fmcw_en = TRUE,
        .rx_en = TRUE,
        .tx_en = FALSE,
    };
    radio_frameStart(cur_bank, &radio_driver);

    BasebandDoneWait_t bb_wait = {
        .mode = POL_MODE,
        .time_out = 20,
    };

    uint32_t result = 0;
    BasebandDriverCommand_t bb_cmd = {0};
    bb_cmd.command_id = BB_DONE_WAIT;
    bb_cmd.pParam = (uint32_t *)&bb_wait;
    bb_cmd.pResult = &result;
    baseband_command(&bb_cmd);
    // Search test target peak in 2D-FFT plane
    uint32 old = fusa_bb_switch_mem_access(BB_MEM_ACT_BUF);
    //gpio_write(LED_D1_NO, LED_D1_OFF);//output low level

    uint32_t fft_mem = 0;
    sint32 ch_index = 0;
    sint32 rang_index = 0;
    sint32 rang_index_max = (raw_readl(BB_REG_SYS_SIZE_RNG_FFT) + 1) / 2;
    //int16_t *BufPtr;
    sint16 temp_value1 = 0, temp_value2 = 0;
    sint32 rx_sum_src_value_buf[MAX_NUM_RX] = {0};
    float32 rx_average_src_value_buf[MAX_NUM_RX] = {0};
    uint32 rx_sum_square_value_buf[MAX_NUM_RX] = {0};
    float32 rx_sum_diff[MAX_NUM_RX] = {0};

    for (ch_index = 0; ch_index < MAX_NUM_RX; ch_index++) {
        rang_index = 0;
        for (; rang_index < rang_index_max; rang_index++) {
            fft_mem = fusa_bb_read_first_chirp_data(ch_index, rang_index, rang_index_max);
            temp_value1 = ((sint16)((fft_mem >> 16) & 0xFFFF)) / 4;
            temp_value2 = ((sint16)(fft_mem & 0xFFFF)) / 4;
            // log_fusa("%d ", temp_value1); // Print int16_t to UART
            // log_fusa("%d ", temp_value2); // Print int16_t  to UART

            rx_sum_src_value_buf[ch_index] += (temp_value1 + temp_value2);
            rx_sum_square_value_buf[ch_index] += (temp_value1 * temp_value1 + temp_value2 * temp_value2);
        }
        // log_fusa("\r\n");
        rx_average_src_value_buf[ch_index] = rx_sum_src_value_buf[ch_index] / rang_index_max;
    }
    //log_fusa("one frame done!\n");
    //log_fusa("rx_sum_src_value_buf:  %d  %d  %d  %d  \n", rx_sum_src_value_buf[0], rx_sum_src_value_buf[1], rx_sum_src_value_buf[2], rx_sum_src_value_buf[3]);
    //log_fusa("rx_average_src_value_buf:  %.4f  %.4f  %.4f  %.4f  \n", rx_average_src_value_buf[0], rx_average_src_value_buf[1], rx_average_src_value_buf[2], rx_average_src_value_buf[3]);
    //log_fusa("rx_sum_square_value_buf:  %d  %d  %d  %d  \n", rx_sum_square_value_buf[0], rx_sum_square_value_buf[1], rx_sum_square_value_buf[2], rx_sum_square_value_buf[3]);

    for (int k = 0; k < MAX_NUM_RX; k++) {
        rx_sum_diff[k] = rx_sum_square_value_buf[k] - rang_index_max * rx_average_src_value_buf[k] * rx_average_src_value_buf[k];
    }
    //log_fusa("rx_sum_diff:  %.4f  %.4f  %.4f  %.4f  \n", rx_sum_diff[0], rx_sum_diff[1], rx_sum_diff[2], rx_sum_diff[3]);

    //gpio_write(LED_D1_NO, LED_D1_OFF);//output low level
    //log_fusa("\r\n");
    float32 logrx[MAX_NUM_RX], diff[MAX_NUM_RX], avg;

    logrx[0] = 10 * log10f(rx_sum_diff[0]);
    logrx[1] = 10 * log10f(rx_sum_diff[1]);
    logrx[2] = 10 * log10f(rx_sum_diff[2]);
    logrx[3] = 10 * log10f(rx_sum_diff[3]);

    // avg = (logrx[0] + logrx[1] + logrx[2] + logrx[3])/4;
    avg = 10 * log10f(rx_sum_diff[0]/4 + rx_sum_diff[1]/4 + rx_sum_diff[2]/4 + rx_sum_diff[3]/4);

    diff[0] = fabs(logrx[0] - avg);
    diff[1] = fabs(logrx[1] - avg);
    diff[2] = fabs(logrx[2] - avg);
    diff[3] = fabs(logrx[3] - avg);
    // restore tx parameter
    fusa_radio_tx_ch_restore(tx_status, 5);

    if ((diff[0] > consistence_check_threshold) || \
            (diff[1] > consistence_check_threshold) || \
            (diff[2] > consistence_check_threshold) || \
            (diff[3] > consistence_check_threshold)) {
            ret = E_FUSA_SYS;
            //log_fusa("consistence_check result, diff0: %f diff1: %f diff2: %f diff3: %f avg = %f; ret = %d\r\n",diff[0], diff[1], diff[2], diff[3], avg, ret);
    }
    // log_fusa("consistence_check result, diff0: %f diff1: %f diff2: %f diff3: %f avg = %f; ret = %d\r\n",diff[0], diff[1], diff[2], diff[3], avg, ret);
    fusa_bb_switch_mem_access(old);
    /* restore zero doppler removel */
    //BB_WRITE_REG(bb_hw, FFT_ZER_DPL_ENB, old_zer_dpl);
    raw_writel(BB_REG_SYS_BNK_ACT, cur_bank);
    raw_writel(BB_REG_SYS_ENABLE, old_status_en);
    fusa_bb_switch_buf_store(old_buf_store);
    /* set chirp number and phase scramble init state*/
    // fmcw_radio_switch_bank(&bb->radio, 5 + bb->radio.frame_type_id);
    // fmcw_radio_reg_write(&bb->radio, RADIO_BK5_FMCW_CHIRP_SIZE_1_0, old_chirp_size_0);
    // fmcw_radio_reg_write(&bb->radio, RADIO_BK5_FMCW_CHIRP_SIZE_1_1, old_chirp_size_1);
    // BB_WRITE_REG(bb_hw, SYS_SIZE_VEL_BUF, old_bb_sys_size_vel);

    old_bank = fusa_radio_switch_bank(5 + cur_bank);
    fusa_radio_reg_write(RADIO_BK5_FMCW_CHIRP_SIZE_1_0, old_chirp_size_0);
    fusa_radio_reg_write(RADIO_BK5_FMCW_CHIRP_SIZE_1_1, old_chirp_size_1);
    raw_writel(BB_REG_SYS_SIZE_VEL_BUF, old_bb_sys_size_vel);
    raw_writel(BB_REG_SYS_SIZE_VEL_FFT, old_bb_sys_size_fft);
    raw_writel(BB_REG_SYS_SIZE_BPM, old_bb_sys_bpm);
    fusa_radio_switch_bank(old_bank);
    raw_writel(BB_REG_SYS_IRQ_CLR, BB_IRQ_CLEAR_ALL);

    if(old_irq_state)
    {
        fusa_dmu_irq_en(FUSA_BB_IRQ_DONE,TRUE);
    }
    return ret;
}

/* peak_power should be an array with size 4 */
void fusa_bb_fft_peak_calc(float32 *peak_power, uint32 fft_size)
{
    uint32 adc_fre = fusa_radio_get_adc_freq();
    uint16 peak_point = 0;

    // find the FFT index near 5MHz
    // adc_freq not in radio shadow bank, should be aligned with radio by using sensor_config_init0.hxx

    uint16 chk_bgn;
    uint16 fc_sin = ABIST_SIN_FREQ * fft_size / adc_fre; // center frequency of sinewave

    if (fc_sin < (ABIST_FFT_CHK_HLF + ABIST_FFT_CHK_MIN))
    {
        chk_bgn = ABIST_FFT_CHK_MIN;
        } else {
                chk_bgn = fc_sin - ABIST_FFT_CHK_HLF;
        }

        fusa_bb_switch_mem_access(BB_MEM_ACT_BUF);
        // search the peak
        // since the sinewave is always 5MHz, so FFT peak search can be narrowed, here using ABIST_FFT_CHK_SIZE
        for (uint8 ch_index = 0; ch_index < ANT_NUM; ch_index++){
                for (uint8 i = 0; i < ABIST_FFT_CHK_SIZE; i++) {
                        uint16 rng_index = chk_bgn + i;

                        if (rng_index > (fft_size/2 - 1)) {
                                break; // index out of range
                }
              uint32 fft_mem = fusa_bb_read_first_chirp_data(ch_index, rng_index, fft_size / 2);
            //   uint32 fft_mem = fusa_bb_read_first_chirp_data(ch_index, rng_index + fft_size * 2, fft_size / 2);//if sdm not reset,read the seconed chirp data to ensure correctness
              complex_t complex_fft = cfl_to_complex(fft_mem, 14, 14, true, 4, false);
              float power = 10 * log10f(complex_fft.r * complex_fft.r + complex_fft.i * complex_fft.i);

              // save the peak
              if (i == 0)
              {
                  peak_power[ch_index] = power;
                  peak_point = rng_index;
                        }
                        else {
            if (power > peak_power[ch_index]) {
                                peak_power[ch_index] = power;
                                peak_point           = rng_index;
            }
                        }
                }
                // log_fusa("Channel %d, FFT1D peak = %f dB index = %d freq = %f MHz\r\n",
                //                    ch_index, peak_power[ch_index], peak_point, (float)(peak_point)*(adc_fre)/fft_size);
        }
}
/* peak_power should be an array with size 4 */
void fusa_dac_playback(boolean inner_circle, uint8 inject_num, uint8 out_num, float32* peak_power)
{
    /* Assert and deassert ADC reset and bb_core reset */
    fusa_bb_hw_reset_after_force();

    /* config radio */
    uint8 old_tx_status[5];
    fusa_radio_tx_ch_status(old_tx_status, 5);
    fusa_radio_tx_ch_on(-1, FALSE); /* turn off tx */

    if (inner_circle == true) {
            fusa_radio_dac_reg_cfg_inner(inject_num, out_num);
    } else {
            fusa_radio_dac_reg_cfg_outer();   /* config analog DAC*/
    }

    /* Enter loop test mode,radio shadow bank not needed */
    fusa_radio_loop_test_en(TRUE);

    /* Disable velamb-CD mode in bank 0 [bug1433] */
    uint32 old_bnk_act = fusa_bb_switch_bnk_act(0x0);
    uint32 old_sys_type_fmcw = raw_readl(BB_REG_SYS_TYPE_FMCW);
    raw_writel(BB_REG_SYS_TYPE_FMCW, 0x0);

    /* switch to baseband shadow bank */
    fusa_bb_switch_bnk_act(SHADOW_BNK);
    uint32 old_bnk_mode = fusa_bb_switch_bnk_mode(SYS_BNK_MODE_SINGLE);
    uint32 fft_size = raw_readl(BB_REG_SYS_SIZE_RNG_FFT) + 1;
    uint8 old_buf_store =fusa_bb_switch_buf_store(SYS_BUF_STORE_FFT);

    RadioDriverContext_t radio_driver = {
    .fmcw_en = FALSE,
    .rx_en = FALSE,
    .tx_en = FALSE,
    };
    radio_frameStart(0, &radio_driver); //just for sdm rst,if not,you need to use the second chirp data to calculate

    // dmu_adc_reset(); // ADC should reset in cascade

    /* start baseband */
    BasebandDriverContext_t bb_driver = {
        .sys_en = (1 << SYS_ENABLE_SAM_SHIFT),
                    // (1 << SYS_ENABLE_FFT_1D_SHIFT),
        .sys_irq_en = FALSE,
    };
    baseband_run(&bb_driver);

    // log_fusa("current time ms 1 = %f\r\n", fusa_current_time_ms());
    // fusa_delay_ms(10);
    BasebandDoneWait_t bb_wait = {
        .mode = POL_MODE,
        .time_out = 20,
    };
    uint32_t result = 0;
    BasebandDriverCommand_t bb_cmd = {0};
    bb_cmd.command_id = BB_DONE_WAIT;
    bb_cmd.pParam = (uint32_t *)&bb_wait;
    bb_cmd.pResult = &result;
    baseband_command(&bb_cmd);
    // fmcw_radio_reg_dump(0);
    // baseband_reg_dump();
    // log_fusa("current time ms 2 = %f\r\n", fusa_current_time_ms());

    //recover
    fusa_bb_switch_buf_store(old_buf_store);
    /* Restore velamb-CD mode in bank 0 */
    fusa_bb_switch_bnk_act(0x0);
    raw_writel(BB_REG_SYS_TYPE_FMCW, old_sys_type_fmcw);

    /* restore registers status */
    fusa_bb_switch_bnk_act(old_bnk_act);
    fusa_bb_switch_bnk_mode(old_bnk_mode);

    fusa_radio_tx_ch_restore(old_tx_status, 5);

    /* leave radio shadow bank */
    fusa_radio_loop_test_en(FALSE);

    fusa_bb_fft_peak_calc(peak_power, fft_size);
    fusa_bb_hw_reset_after_force();

}

void fusa_radar_scan_one_time(uint32 frame_id)
{
    float32 start_time = fusa_current_time_ms();
    BasebandDriverCommand_t bb_cmd = {
        .command_id = BB_SET_FRAME_TYPE_ID,
        .pParam = &frame_id,
    };
    sint32 ret_temp = baseband_command(&bb_cmd);

    BasebandDriverContext_t bb_driver = {
        .sys_en = (1 << SYS_ENABLE_SAM_SHIFT)   \
                | (1 << SYS_ENABLE_FFT_2D_SHIFT)\
                | (1 << SYS_ENABLE_CFR_SHIFT)   \
                | (1 << SYS_ENABLE_BFM_SHIFT),
        .sys_irq_en = FALSE,
    };
    baseband_run(&bb_driver);

    RadioDriverContext_t radio_driver = {
        .fmcw_en = TRUE,
        .rx_en = TRUE,
        .tx_en = TRUE,
    };
    radio_frameStart(frame_id, &radio_driver);

    BasebandDoneWait_t bb_wait = {
        .mode = POL_MODE,
        .time_out = 50,
    };

    uint32_t result = 0;
    bb_cmd.command_id = BB_DONE_WAIT;
    bb_cmd.pParam = (uint32_t *)&bb_wait;
    bb_cmd.pResult = &result;
    ret_temp = baseband_command(&bb_cmd);
    //log_fusa("%s  ecc sb_status = 0x%x\r\n",__func__, raw_readl(BB_REG_SYS_ECC_SB_STATUS));
    //log_fusa("%s  ecc db_status = 0x%x\r\n",__func__, raw_readl(BB_REG_SYS_ECC_DB_STATUS));
    fusa_bb_mem_ecc_err_clr();
    //log_fusa("%s time=%f\r\n", __func__, fusa_current_time_ms() - start_time);
}
