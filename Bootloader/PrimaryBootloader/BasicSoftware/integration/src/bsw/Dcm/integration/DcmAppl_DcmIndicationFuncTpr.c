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
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"
/*TESTCODE-START
#include "DcmTest.h"
TESTCODE-END*/

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 *  @ingroup DCM_TPL
 *  DcmAppl_DcmIndicationFuncTpr:-\n
 *  This API is called by DCM if function functional tester present request is received with suppress positive bit set in the request.\n
 *  User has to add code if some action is intended.
 *
 * @param[in]   None
 *
 * @param[out]  None
 *
 * @retval      None
 *
  */
void DcmAppl_DcmIndicationFuncTpr(void)
{
    /* User has to add code if some action is intended */
	/*TESTCODE-START
	DcmTest_DcmAppl_DcmIndicationFuncTpr();
	TESTCODE-END*/
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
