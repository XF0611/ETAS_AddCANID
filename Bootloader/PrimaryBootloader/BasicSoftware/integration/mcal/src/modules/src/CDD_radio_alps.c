#include <string.h>
#include "embARC_toolchain.h"
#include "embARC_assert.h"
#include "embARC_error.h"
#include "calterah_math.h"
#include "Std_Types.h"
#include "calterah_limits.h"
#include "CDD_radio.h"
#include "radio_ctrl.h"
#include "system_clock_init.h"
#include "alps_timer.h"

extern uint8_t vam_status[4];
extern uint8_t txphase_status[8];
void dummy_printf(const char*fmt, ...)
{
}

static RadioInitializationParams_t *gCfgParams;
static RadioFrameParams_t gFrameParams[MAX_FRAME_TYPE];


static sint32 alps_radio_init(void)
{
    sint32 ret = E_OK;
    pll_clock_init();

    ret = E_OK;
    return ret;
}

static sint32 alps_radio_frameConfig(uint8 frameId, RadioInitPhase_e radioInitPhase, RadioInitializationParams_t *pParams)
{
    sint32 ret = 0;

    switch(radioInitPhase)
    {
    case RADIO_SET_FRM_CFG:
        //1. compute reg value
        fmcw_radio_power_on(&pParams[frameId]);
        fmcw_radio_compute_reg_value(&gFrameParams[frameId],&pParams[frameId]);
        gCfgParams = pParams;
        //memcpy(&gCfgParams,pParams,sizeof(RadioInitializationParams_t));
        //2. generate fmcw
        gFrameParams[frameId].frame_type_id = frameId;
        if(E_OK !=  fmcw_radio_generate_fmcw(&gFrameParams[frameId],&pParams[frameId]))
        {
            fmcw_radio_reset();
        }
        break;

    case RADIO_SET_DC_CFG:
        radio_dc_calib_setting();
        radio_dc_calib_set_cfg(frameId);
        fmcw_radio_rx_on(true, pParams);
        break;

    case RADIO_WR_DC_VAL:
        radio_dc_calib_wr_dc(frameId, pParams[frameId].dc_offset);
        break;

    case RADIO_RST_DC_CFG:
        radio_dc_calib_rst_cfg(frameId);
        break;

    default:
        break;
    }

    return ret;
}

static sint32 alps_radio_powerSave(uint8 frameId, bool tx_en, bool rx_en, bool enable)
{
    sint32 ret = 0;

    //power save on or off
    radio_interframe_power_save_enable(&gCfgParams[frameId], tx_en, rx_en, enable);


    return ret;
}

static sint32 alps_radio_auxadc_enable(int32_t mode,  int32_t muxin_sel)
{
    sint32 ret = 0;

    /*
     * mode : External voltage / Internal temperature /Internal  voltage
     *      #define AUXADC_read_ex_voltage        1
     *      #define AUXADC_read_in_temp           2
     *      #define AUXADC_read_in_voltage        3
     * inputmux selection :  TestMUXN / TestMUXP / TPANA1
     *      #define AUXADC_TestMUXN         1
     *      #define AUXADC_TestMUXP         2
     *      #define AUXADC_TPANA1           3
     */

    auxadc_enable(mode, muxin_sel);

    return ret;
}

static sint32 alps_radio_auxadc_read(int32_t mode,float32 *val)
{
    sint32 ret = 0;

    /* note: After enabling, it needs to delay 2ms to read
     * mode : External voltage / Internal temperature /Internal  voltage
     *      #define AUXADC_read_ex_voltage       1
     *      #define AUXADC_read_in_temp           2
     *      #define AUXADC_read_in_voltage        3
     */
    if(val != NULL)
    {
        *val = auxadc_read(mode);
    }
    else
    {
        ret = -1;
    }
    return ret;
}
static sint32 alps_radio_frameStart(uint8 frameId, RadioDriverContext_t *pRadioContext)
{
    sint32 ret = 0;

    if(pRadioContext->fmcw_en){
        (void)radio_powerSave(frameId, pRadioContext->tx_en, pRadioContext->rx_en, false);
    }

    fmcw_radio_frame_interleave_pattern(frameId);
    fmcw_radio_sdm_reset(&gCfgParams[frameId]);
    chip_hw_udelay(100);

    if(pRadioContext->fmcw_en){
        fmcw_radio_start_fmcw(&gFrameParams[frameId], &gCfgParams[frameId]);
    }

    return ret;
}

static sint32 alps_radio_frameStop(uint8 frameId)
{
    sint32 ret = 0;

    // fmcw stop
    fmcw_radio_stop_fmcw(&gCfgParams[frameId]);
    return ret;
}

static sint32 alps_radio_statusReport(uint8 frameId,RadioStatusReport_t *p_devStatus)
{
    sint32 ret = 0;
    sint32 i,j;

    bool is_running = fmcw_radio_is_running(gFrameParams[frameId].frame_type_id);
    p_devStatus->is_running = is_running;

    bool is_txon = fmcw_radio_is_tx_ch_on(frameId);
    p_devStatus->is_txon = is_txon;

    bool is_rxon = fmcw_radio_is_rx_on();
    p_devStatus->is_rxon = is_rxon;

    bool is_adcon = fmcw_radio_is_adc_on();
    p_devStatus->is_adcon = is_adcon;

    //todo lock freq,adc freq

    fmcw_radio_vam_status_save(frameId);
    fmcw_radio_txphase_status(true);
    for(i = 0; i < 4; i++){
        p_devStatus->vam_status[i] = vam_status[i];
    }
    for(j = 0; j < 8; j++){
        p_devStatus->txphase_status[j] = txphase_status[j];
    }

    return ret;
}

static sint32 alps_radio_eventSet(uint8 frameId,RadioEventsComplete_t evt,void *pParams)
{
    sint32 ret = 0;

    //add code here

    return ret;
}

static sint32 alps_radio_command(RadioDriverCommand_t *pRadioCommand)
{
    sint32 ret = 0;

    switch(pRadioCommand->cmd_id){

    case RADIO_LVDS_MODE_NORM_TO_TEST:
        fmcw_radio_lvds_mode(NORM_TO_TEST);
        break;

    case RADIO_LVDS_MODE_TEST_TO_NORM:
        fmcw_radio_lvds_mode(TEST_TO_NORM);
        break;
    case RADIO_LVDS_MODE_ON:
        fmcw_radio_lvds_on(*pRadioCommand->pParam);
        break;

    case RADIO_DDM_CFG:
        if(*pRadioCommand->pParam > (NUM_FRAME_TYPE - 1))
        {
            ret = 1;
            break;
        }
        fmcw_radio_DDM_cmd_cfg(&gFrameParams[*pRadioCommand->pParam], &gCfgParams[*pRadioCommand->pParam]);

        break;

    case RADIO_READ_REG:
        if(pRadioCommand->pResult == NULL)
        {
            ret = 2;
            break;
        }
        *((char *)pRadioCommand->pResult) = fmcw_radio_reg_read(*pRadioCommand->pParam);
        break;

    case RADIO_WRITE_REG:
        fmcw_radio_reg_write(*pRadioCommand->pParam++, *pRadioCommand->pParam);
        break;

    case RADIO_GET_TEM:
        if(pRadioCommand->pResult == NULL)
        {
            ret = 2;
            break;
        }
        *((float *)pRadioCommand->pResult) = fmcw_radio_get_temperature();
        break;

    case RADIO_TX_CH_ON:
        if(*pRadioCommand->pParam > (MAX_NUM_TX - 1))
        {
            ret = 1;
            break;
        }
        fmcw_radio_tx_ch_on(*pRadioCommand->pParam++, *pRadioCommand->pParam);

        break;

    case RADIO_SET_TX_PHASE:
        if(*pRadioCommand->pParam > (MAX_NUM_TX - 1))
        {
            ret = 1;
            break;
        }
        fmcw_radio_set_tx_phase(*pRadioCommand->pParam++, *pRadioCommand->pParam);

        break;

    case RADIO_GET_TX_PHASE:
        if(*pRadioCommand->pParam > (MAX_NUM_TX - 1))
        {
            ret = 1;
            break;
        }

        if(pRadioCommand->pResult == NULL)
        {
            ret = 2;
            break;
        }
        *pRadioCommand->pResult = fmcw_radio_get_tx_phase(*pRadioCommand->pParam);

        break;

    case RADIO_PHASE_VAL:
        if(pRadioCommand->pResult == NULL)
        {
            ret = 2;
            break;
        }
        *pRadioCommand->pResult = reg_val_2_phase_val(*pRadioCommand->pParam);
        break;

    case RADIO_REG_VAL:
        if(pRadioCommand->pResult == NULL)
        {
            ret = 2;
            break;
        }
        *pRadioCommand->pResult = phase_val_2_reg_val(*pRadioCommand->pParam);
        break;

    case RADIO_IF_OUT_ON:
        if(*pRadioCommand->pParam > (MAX_NUM_RX - 1))
        {
            ret = 1;
            break;
        }
        fmcw_radio_if_output_on(*pRadioCommand->pParam++, *pRadioCommand->pParam);

        break;

    case RADIO_SINGLE_TONE:
        if(pRadioCommand->pResult == NULL)
        {
            ret = 2;
            break;
        }
        *pRadioCommand->pResult = fmcw_radio_single_tone( *pRadioCommand->pParam++, *pRadioCommand->pParam++, *pRadioCommand->pParam);
        break;

    case RADIO_SET_TIA_GAIN:
        if(*pRadioCommand->pParam > (MAX_NUM_RX - 1))
        {
            ret = 1;
            break;
        }
        fmcw_radio_set_tia_gain(*pRadioCommand->pParam++, *pRadioCommand->pParam);

        break;

    case RADIO_GET_TIA_GAIN:
        if(*pRadioCommand->pParam > (MAX_NUM_RX - 1))
        {
            ret = 1;
            break;
        }
        if(pRadioCommand->pResult == NULL)
        {
            ret = 2;
            break;
        }
        *pRadioCommand->pResult = fmcw_radio_get_tia_gain(*pRadioCommand->pParam);

        break;

    case RADIO_SET_VGA1_GAIN:
        if(*pRadioCommand->pParam > (MAX_NUM_RX - 1))
        {
            ret = 1;
            break;
        }
        fmcw_radio_set_vga1_gain(*pRadioCommand->pParam++, *pRadioCommand->pParam);

        break;

    case RADIO_GET_VGA1_GAIN:
        if(*pRadioCommand->pParam > (MAX_NUM_RX - 1))
        {
            ret = 1;
            break;
        }
        if(pRadioCommand->pResult == NULL)
        {
            ret = 2;
            break;
        }
        *pRadioCommand->pResult = fmcw_radio_get_vga1_gain(*pRadioCommand->pParam);

        break;

    case RADIO_SET_VGA2_GAIN:
        if(*pRadioCommand->pParam > (MAX_NUM_RX - 1))
        {
            ret = 1;
            break;
        }
        fmcw_radio_set_vga2_gain(*pRadioCommand->pParam++, *pRadioCommand->pParam);

        break;

    case RADIO_GET_VGA2_GAIN:
        if(*pRadioCommand->pParam > (MAX_NUM_RX - 1))
        {
            ret = 1;
            break;
        }
        if(pRadioCommand->pResult == NULL)
        {
            ret = 2;
            break;
        }
        *pRadioCommand->pResult = fmcw_radio_get_vga2_gain(*pRadioCommand->pParam);

        break;

    case RADIO_AGC_ENABLE:
        if(*pRadioCommand->pParam > (NUM_FRAME_TYPE - 1))
        {
            ret = 1;
            break;
        }

        if(*(pRadioCommand->pParam + 1) == true)
        {
            fmcw_radio_agc_setup(&gCfgParams[*pRadioCommand->pParam]);
        }

        fmcw_radio_agc_enable(*pRadioCommand->pParam++, *pRadioCommand->pParam);

        break;

    default:
        break;
    }

    return ret;
}

void attach_radiofw_fun_set_alps()
{
    radio_fw_fun_set_t *p_funset = hal_get_radio_handle();

    p_funset->radio_init_set = alps_radio_init;

    p_funset->radio_frameConfig_set = alps_radio_frameConfig;

    p_funset->radio_powerSave_set = alps_radio_powerSave;

    p_funset->radio_auxadc_enable = alps_radio_auxadc_enable;

    p_funset->radio_auxadc_read = alps_radio_auxadc_read;

    p_funset->radio_frameStart_set = alps_radio_frameStart;

    p_funset->radio_frameStop_set = alps_radio_frameStop;

    p_funset->radio_statusReport_set = alps_radio_statusReport;

    p_funset->radio_eventSet_set = alps_radio_eventSet;

    p_funset->radio_command_set = alps_radio_command;

}
