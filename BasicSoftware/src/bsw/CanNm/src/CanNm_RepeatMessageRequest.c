

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Prv.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_RepeatMessageRequest
 Description      : This is the AUTOSAR interface to set Repeat Message Request Bit in the Control
                    Bit Vector for NM PDUs transmitted next on the bus. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
 Return value     : E_OK - No error
                  : E_NOT_OK - Setting of Repeat Message Request Bit has failed or is not configured for this network handle
 ***************************************************************************************************/

#if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_RepeatMessageRequest(
                                                             NetworkHandleType nmChannelHandle
                                                            )
{
    /* pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle ;

    /* Return value of the API */
    Std_ReturnType RetVal_en;

    /**** End Of Declarations ****/

    /* Receive CanNm channel index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                                nmChannelHandle, CANNM_SID_REPEATMESSAGEREQUEST, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialised */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                            nmChannelHandle, CANNM_SID_REPEATMESSAGEREQUEST, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    /* Set the pointer to RAM structure of channel CanNm_NetworkHandle */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

	ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    /* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /* TRACE[SWS_CanNm_00137] : API is valid only for Normal Operation and Ready Sleep State */
    if ((FALSE != ConfigPtr_pcs->NodeDetectionEnabled_b) && (NM_MODE_NETWORK == RamPtr_ps->Mode_en) && (NM_STATE_REPEAT_MESSAGE != RamPtr_ps->State_en))
    {
        /* Store the Repeat Message request */
        RamPtr_ps->TxRptMsgStatus_b = TRUE;

        /* TRACE[SWS_CanNm_00113,SWS_CanNm_00121] : Set Repeat message bit in CBV for the message transmitted next
           on the bus if node detection is enabled for the channel */
        RamPtr_ps->TxCtrlBitVector_u8 = (RamPtr_ps->TxCtrlBitVector_u8 | CANNM_READ_RPTMSG_MASK );

        /* Return successful operation */
        RetVal_en = E_OK;

    }

        /* TRACE[SWS_CanNm_00137] : CanNm is in Repeat Message State, Prepare Bus-Sleep Mode or Bus-Sleep Mode */
        return RetVal_en;

}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif

