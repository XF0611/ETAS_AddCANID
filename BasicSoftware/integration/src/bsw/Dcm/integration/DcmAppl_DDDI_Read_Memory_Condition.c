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
#include "DcmDspUds_Dddi_Inf.h"
#include "Rte_Dcm.h"
/*TESTCODE-START
#include "DcmTest.h"
TESTCODE-END*/
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF)

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCM_TPL
 * DcmAppl_DDDI_Read_Memory_Condition:-\n
 * This API will check if it is permitted to read from the memory of the ECU as per the specification in a DynamicalDefinedID record.
 *
 * @param[in]       record      : The pointer to the Dcm_DDDI_DEF_MEM_t specifies from where this function shall read and how many bytes. \n
 * @param[out]      NegRespCode : A pointer to a NegRespCode which shall be returned in case the function returns E_NOT_OK.\n
 *                                NegRespCode stays unchanged if other return codes then E_NOT_OK are returned.\n
 *
 * @retval          E_OK        :  in case of success\n
 *                  DCM_E_PENDING: in case of more time is required and the function needs to be called again\n
 *                  E_NOT_OK    :  in case the memory is not accessible.In this case the NegRespCode to return to the tester is initialized.\n
 * @see             DcmAppl_DDDI_Read_Memory
 */
Std_ReturnType DcmAppl_DDDI_Read_Memory_Condition (
    const Dcm_DDDI_DEF_MEM_t * record,
    Dcm_NegativeResponseCodeType * NegRespCode
                                                                  )
{
    Std_ReturnType retVal = E_OK;
	*NegRespCode = 0;
	/* not supported by autosar 3.1 */
    /* fill in customer specific */
    (void)(record);

	/*TESTCODE-START
	retVal = DcmTest_DcmAppl_DDDI_Read_Memory_Condition(record, NegRespCode);
	TESTCODE-END*/

    return retVal;
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif
