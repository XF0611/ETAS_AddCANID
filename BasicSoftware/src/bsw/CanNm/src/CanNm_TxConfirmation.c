
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
 Function name    : CanNm_TxConfirmation
 Description      : This is the AUTOSAR interface to notify confirmation of a tranmsitted NM message over CAN.
                    This function is called by the CanIf after a CAN NM I-PDU has been transmitted
 param            : TxPduId - Identification of the NM-channel
 Return value     : None
 ***************************************************************************************************/

/* CanNm_TxConfirmation is available only if CANNM_IMMEDIATE_TXCONF_ENABLED is set to FALSE */
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
void CanNm_TxConfirmation( PduIdType TxPduId )
{
#if ((CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF))

    #if (CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
    /* pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;
    #endif

    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /**** End Of Declarations ****/

    /********************************* Start: DET *************************************/

    /* Report DET if TxPduId is Invalid */
    CANNM_DET_REPORT_ERROR((TxPduId >= CANNM_NUMBER_OF_CHANNELS),
                            (uint8)TxPduId, CANNM_SID_TXCONFIRMATION, CANNM_E_INVALID_PDUID)

    /* Report DET if CANNM is Uninitialized */
    CANNM_DET_REPORT_ERROR((CanNm_RamData_s[TxPduId].State_en == NM_STATE_UNINIT),
                            (uint8)TxPduId, CANNM_SID_TXCONFIRMATION, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    #if (CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
    /* Initialize pointer to configuration structure */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(TxPduId);
    #endif

    /* Initialize pointer to RAM structure */
    RamPtr_ps = &CanNm_RamData_s[TxPduId];

    /* Start timeout timer if transmission has not stopped */
    if(RamPtr_ps->MsgTxStatus_b != FALSE)
    {
        RamPtr_ps->PrevMsgTimeoutTimestamp = RamPtr_ps->ctSwFrTimer;
    }

    /* Set the transmit confirmation status */
    RamPtr_ps->TxConfirmation_b = TRUE;

    /* Stop monitoring Msg timeout time after getting Tx confirmation */
    RamPtr_ps->TxTimeoutMonitoringActive_b = FALSE;

    #if (CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
    PduR_CanNmTxConfirmation(ConfigPtr_pcs->PduRId);
    #endif

#else
    (void)TxPduId;
#endif

    return;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"



