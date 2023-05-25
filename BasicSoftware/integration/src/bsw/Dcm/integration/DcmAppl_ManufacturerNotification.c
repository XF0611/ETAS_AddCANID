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


#if (DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCM_TPL
 * DcmAppl_ManufacturerNotification :-\n
 *  This function indicates that successful reception of a new request to manufacturer application
 *  and it is called right before before the DSD verification (SID, security access,diagnostic session).
 *  Within this function application can examine the permission of the diagnostic service /
 *  environment (e.g. ECU state afterrun).
 *
 * @param[in]           RequestData   : Pointer to the request data buffer.
 * @param[in]           SID           : Value of service identifier
 * @param[in]           RequestLength : Request data length.
 * @param[in]           RequestType   : It says whether the request is FUNCTIONAL/PHYSICAL.
 * @param[in]           DcmRxPduId    : RxPduId on which the request is received.
 * @param[in]           SourceAddress : This variable gives the source address.
 * @param[in]           pMsgContext   : This variable holds the details of the completed received msg.
 * @param[out]          ErrorCode     : Ponter to the variable to update the error code.
 * @retval              E_OK: diagnostic request accepted.\n
 * @retval              DCM_E_REQUEST_NOT_ACCEPTED: application rejects diagnostic request -> DSD will ignore
 *                      diagnostic request, no further processing of request, no response will be send.\n
 * @retval              DCM_E_PENDING : application needs more time to respond whether to accept/reject/proccess this request
 * @retval              E_NOT_OK : application rejected the diagnostic request, and negative response response will be send
 */
Std_ReturnType DcmAppl_ManufacturerNotification (
                                              uint8 SID,
                                              const uint8 * const RequestData,
                                              PduLengthType RequestLength,
                                              uint8 RequestType,
                                              PduIdType DcmRxPduId,
                                              uint16 SourceAddress,
                                              Dcm_MsgContextType * adrMsgContext_pcst,
                                              Dcm_NegativeResponseCodeType * ErrorCode
                                                                  )
{
    /* dummy code to remove compiler warning */
    /* User hints:
     * Application should not use manufacturer notification function to call services, which supports page buffer handling.
     * Dcm will not support, DCM_E_PENDING state to the services called from the appl function, which means that,
     * the service should be completed in a single raster.
     * Dcm_ProcessingDone will be called by DCM.
     */
    Std_ReturnType retVal = E_OK;
	(void)(RequestData);
    (void)(SID);
    (void)(RequestLength);
    (void)(RequestType);
    (void)(DcmRxPduId);
    (void)(SourceAddress);

	/*TESTCODE-START
	retVal = DcmTest_DcmAppl_ManufacturerNotification(SID, RequestData, RequestLength, RequestType, DcmRxPduId, SourceAddress, adrMsgContext_pcst, ErrorCode);
	TESTCODE-END*/

    return(retVal);
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

