/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 


#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Cdtcs_Inf.h"
#include "Rte_Dcm.h"
/*TESTCODE-START
#include "DcmTest.h"
TESTCODE-END*/
#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF) )


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCM_TPL
 * DcmAppl_DcmControlDtcSettingEnableStatus :-\n
 * This API is called when the control dtc setting status is changed.
 * ControlDtcSettingEnableStatus is true, if Dtc setting is enabled
 * ControlDtcSettingEnableStatus is false if dtc setting is disabled.
 * Integrator can perform  further actions basecd on the  ControlDtcSettingEnableStatus\n
 *
 * @param[in]  ControlDtcSettingEnableStatus: Value to indicate whether DTC is enabled or not\n
 * @param[out] None
 * @retval None
 */
void DcmAppl_DcmControlDtcSettingEnableStatus(boolean ControlDtcSettingEnableStatus)
{
	(void)(ControlDtcSettingEnableStatus);
	/*TESTCODE-START
	DcmTest_DcmAppl_DcmControlDtcSettingEnableStatus(ControlDtcSettingEnableStatus);
	TESTCODE-END*/
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif /* (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF) */
