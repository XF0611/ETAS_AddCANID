#include "Platform_Types.h"
#include "CDD_Baseband.h"
#include "CDD_radio.h"

baseband_fw_fun_set_t g_basebandfw_f;

sint32 baseband_init(BasebandInitializationParams_t *cfg, uint8_t cfg_num,RadioInitializationParams_t *rcfg)
{
    return g_basebandfw_f.baseband_init_set(cfg, cfg_num,rcfg);
}

sint32 baseband_frameConfig(uint8 FrameId, BasebandInitPhase_e BasebandInitPhase)
{
    return g_basebandfw_f.baseband_frameConfig_set(FrameId, BasebandInitPhase);
}

sint32 baseband_run(BasebandDriverContext_t *pBasebandContext)
{
    return g_basebandfw_f.baseband_run_set(pBasebandContext);
}

sint32 baseband_command(BasebandDriverCommand_t *pBasebandCommand)
{
    return g_basebandfw_f.baseband_command_set(pBasebandCommand);
}

sint32 baseband_stop(void)
{
    return g_basebandfw_f.baseband_stop_set();
}

baseband_fw_fun_set_t *hal_get_baseband_handle()
{
    return &g_basebandfw_f;
}

