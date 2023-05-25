#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "embARC.h"
//#include "calterah_math.h"
#include "Platform_Types.h"
#include "calterah_limits.h"
#include "CDD_radio.h"


radio_fw_fun_set_t g_radiofw_f;

sint32 radio_init(void)
{
    return g_radiofw_f.radio_init_set();
}

sint32 radio_frameConfig(uint8 frameId, RadioInitPhase_e radioInitPhase, RadioInitializationParams_t *pParams)
{
    return g_radiofw_f.radio_frameConfig_set(frameId,radioInitPhase, pParams);
}

sint32 radio_powerSave(uint8 frameId, bool tx_en, bool rx_en, bool enable)
{
    return g_radiofw_f.radio_powerSave_set(frameId, tx_en, rx_en, enable);
}

sint32 radio_auxAdc_enable(int32_t mode,  int32_t muxin_sel)
{
    return g_radiofw_f.radio_auxadc_enable(mode, muxin_sel);
}

sint32 radio_auxAdc_read(int32_t mode, float32 *val)
{
    return g_radiofw_f.radio_auxadc_read(mode, val);
}

sint32 radio_frameStart(uint8 frameId, RadioDriverContext_t *pRadioContext)
{
    return g_radiofw_f.radio_frameStart_set(frameId, pRadioContext);
}

sint32 radio_frameStop(uint8 frameId)
{
    return g_radiofw_f.radio_frameStop_set(frameId);
}

sint32 radio_statusReport(uint8 frameId,RadioStatusReport_t *p_devStatus)
{
    return g_radiofw_f.radio_statusReport_set(frameId,p_devStatus);
}

sint32 radio_eventSet(uint8 frameId,RadioEventsComplete_t evt,void *pParams)
{
    return g_radiofw_f.radio_eventSet_set(frameId,evt,pParams);
}

sint32 radio_command(RadioDriverCommand_t *pRadioCommand)
{
    return g_radiofw_f.radio_command_set(pRadioCommand);
}

radio_fw_fun_set_t *hal_get_radio_handle()
{
    return &g_radiofw_f;
}

