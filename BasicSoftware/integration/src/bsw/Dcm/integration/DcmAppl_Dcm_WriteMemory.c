/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
#include "rba_BswSrv.h"

#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Wmba_Inf.h"
#include "Rte_Dcm.h"
/*TESTCODE-START
#include "DcmTest.h"
TESTCODE-END*/
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)



#if((DCM_CFG_DSP_WRITEMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_TRANSFERDATA_ENABLED!= DCM_CFG_OFF))


 #define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCM_TPL
 * Dcm_WriteMemory:-\n
 * This API implements Write to memory functionality.User Application should add the necessary code to write the data in to memory address specified by the tester.\n
 *
 *
 * @param[in]       Wmba_Opstatus   :   Information on the opstatus\n
 * @param[in]       memoryid        :   Identifier of the memory block\n
 * @param[in]       memoryaddress   :   Starting address of the server memory address to be copied\n
 * @param[in]       datalength      :   Number of bytes in memory data\n
 * @param[out]      reqbuf          :   Data will be written into this buffer(points to the diagnostic buffer).\n
 * @param[out]      ErrorCode       : The negative response code to be updated by the application\n
 *                                    In case the Wmba_Opstatus value is passed as DCM_CANCEL, then the ErrorCode parameter shall be a NULL_PTR.
 * @retval          DCM_WRITE_OK       :  WRITE was successful\n
 * @retval          DCM_WRITE_FAILED   :  WRITE was not successful\n
 * @retval          DCM_WRITE_PENDING  :  WRITE is not yet finished\n
 * @retval          DCM_WRITE_FORCE_RCRRP : If the Pending response (0x78) has to be triggered by Dcm immediately.
 */
 Dcm_ReturnWriteMemoryType DcmAppl_Dcm_WriteMemory(Dcm_OpStatusType Wmba_Opstatus,
 													uint8 memoryid,
 													uint32 memoryaddress,
 													uint32 datalength,
 													const uint8 * reqbuf,
 													Dcm_NegativeResponseCodeType * ErrorCode)
 {
    Dcm_ReturnWriteMemoryType retVal = DCM_WRITE_OK;
	/* User Application should add the necessary code to write the data in to memory address specified by the tester */
    (void)(Wmba_Opstatus);
    (void)(memoryid);
    (void)(memoryaddress);
    (void)(datalength);
    (void)rba_BswSrv_MemCopy( (void*)memoryaddress, (const void*)reqbuf, datalength);
	/*TESTCODE-START
	retVal = DcmTest_DcmAppl_Dcm_WriteMemory(Wmba_Opstatus,memoryid,memoryaddress,datalength,reqbuf,ErrorCode);
	TESTCODE-END*/

    return(retVal);
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif
