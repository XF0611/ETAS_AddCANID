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

#if (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF)

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

 /**
 * @ingroup DCM_TPL
 * DcmAppl_UserDTCSettingEnableModeRuleService :-\n
 * This API is invoked to check for conditions to decide if DTC Setting state needs to be enabled
 *
 * @param[in]     None
 * @param[out]    None
 * @retval        E_OK   : Check to decide if the DTC setting state needs to be enabled passed successfully \n
 * @retval        E_NOT_OK  : Check to decide if the DTC setting state needs to be enabled  failed \n
 *
 */
Std_ReturnType DcmAppl_UserDTCSettingEnableModeRuleService(void)
{
	Std_ReturnType retVal_u8 = E_OK;
	/*TESTCODE-START
	retVal_u8=DcmTest_DcmAppl_UserDTCSettingEnableModeRuleService();
	TESTCODE-END*/
	return (retVal_u8);
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

