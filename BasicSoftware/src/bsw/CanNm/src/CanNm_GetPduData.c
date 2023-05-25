

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Inl.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_GetPduData
 Description      : This is the AUTOSAR interface for getting the whole PDU data out of the most
                    recently received NM PDU. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
                  : nmPduDataPtr    - Pointer where the NM PDU data shall be copied to.
 Return value     : E_OK     - No error
                  : E_NOT_OK - Getting of NM PDU data has failed
 ***************************************************************************************************
 */


#if ((CANNM_USER_DATA_ENABLED != STD_OFF) || (CANNM_NODE_DETECTION_ENABLED != STD_OFF) || \
     (CANNM_NODE_ID_ENABLED != STD_OFF))

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_GetPduData(
                                                   NetworkHandleType nmChannelHandle,
                                                   uint8 * nmPduDataPtr
                                                  )
{
    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Pointer to receive buffer */
    uint8 * BufferPtr;

    /* Return value of the API */
    Std_ReturnType RetVal_en;

    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /**** End Of Declarations ****/

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid*/
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_GETPDUDATA, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                               nmChannelHandle, CANNM_SID_GETPDUDATA, CANNM_E_NO_INIT)

    /* Report DET if received pointer is a null pointer*/
    CANNM_DET_REPORT_ERROR_NOK((nmPduDataPtr == NULL_PTR),
                            nmChannelHandle, CANNM_SID_GETPDUDATA, CANNM_E_PARAM_POINTER)

    /*********************************  End: DET  *************************************/

    /* initialize the pointer to RAM data structure */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    /* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /* Check if any NM PDU was received*/
    if(RamPtr_ps->RxStatus_b != FALSE)
    {
        ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

        /* Check that PDU length is non-zero*/
        if (ConfigPtr_pcs->PduLength_u8 != 0)
        {
            /* Get the address of the receive buffer */
            BufferPtr = &(RamPtr_ps->RxBuffer_au8[0]);

            /* TRACE[SWS_CanNm_00138] : Copying shall happen if CanNmNodeDetectionEnabled or CanNmUserDataEnabled or
             * CanNmNodeIdEnabled is enabled. CanNmNodeDetectionEnabled is not checked in below condition as
             * CanNmNodeIdEnabled acts as master for CanNmNodeIdEnabled and CanNmNodeDetectionEnabled */
            if((CANNM_USER_DATA_ENABLED != STD_OFF) || (ConfigPtr_pcs->NodeIdEnabled_b != FALSE))
            {
                /* Enter Critical Section */
                SchM_Enter_CanNm_GetPduDataNoNest();

                /* TRACE[SWS_CanNm_00138] : Copy the received PDU data from Rx buffer into the passed pointer */
                /* This is an internal function with predictable run-time; hence doesn't affect interrupt lock */
                CanNm_CopyBuffer(BufferPtr,nmPduDataPtr,ConfigPtr_pcs->PduLength_u8);

                /* Leave Critical Section */
                SchM_Exit_CanNm_GetPduDataNoNest();

                /* Return successful operation */
                RetVal_en = E_OK;
            }

         }

     }

    /* API is rejected as there is no NM message received during this NM Sleep-Cycle */
    return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


