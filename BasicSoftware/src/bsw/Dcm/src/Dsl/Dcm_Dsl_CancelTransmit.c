

#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"

#if((DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF) || (RBA_DCMPMA_CFG_PLANTMODEACTIVATION_ENABLED !=  DCM_CFG_OFF))
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/***********************************************************************************************************************
 *    Function Definition
 **********************************************************************************************************************/
/**
 **************************************************************************************************
 * Dcm_Prv_CancelTransmit : Function to cancel the ongoing transmission of the response during
 *                      protocol preemption.
 *
 * \param           None
 *
 *
 * \retval          E_NOT_OK  Ongoing transmission is not canceled
 *                  E_OK      Ongoing transmission is canceled
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
Std_ReturnType Dcm_Prv_CancelTransmit(void)
{
    Std_ReturnType cancelTxStatus = E_NOT_OK;
    Dcm_DsdStatesType_ten DsdState_en;
    uint8 DslState_u8 = 0u;
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    boolean isPagedBufferTransmissionInProgress_b = FALSE;
#endif

    /*Multicore: Lock added here to ensure that the DSL, DSD states are updated together here.
      The active request should not modify the state machine variables separately only during
      this protocol pre-emption scenario*/
    SchM_Enter_Dcm_Global_NoNest();
    DsdState_en = Dcm_Prv_GetDsdState();
    DslState_u8   = Dcm_Prv_GetDslState();
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    isPagedBufferTransmissionInProgress_b = Dcm_DsldGlobal_st.flgPagedBufferTxOn_b;
#endif
    SchM_Exit_Dcm_Global_NoNest();

    /* If Normal Positive/Negative Response is under transmission, DSL state is DSL_WAIT_FOR_CONFIRMATION */
    /* If ROE/RDPI Type1 response is under transmission, DSL state is DSL_ROETYPE1_RECIEVED or DSL_RDPITYPE1_RECIEVED
       and DSD in DSD_SEND */
    if((DslState_u8 == DSL_STATE_WAITING_FOR_TXCONFIRMATION) || \
      ((DslState_u8 == DSL_STATE_ROETYPE1_RECEIVED) && (DsdState_en == DSD_WAITFORTXCONF)))
    {
        /* Set the status to Cancel the transmission */
        /*Set the cancel transmit status flag to True */
        Dcm_isCancelTransmitInvoked_b = TRUE;
    }

#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    /* If paged buffer is active then the following cases can occur */
    /* 1. First page is not sent yet as there is ongoing waitpend response */
    /* 2. Consecutive pages are being sent */
    /* 3. All pages are sent to PduR and waiting for Tx confirmation */
    /*Check if the paged buffer flag is active*/
    if(FALSE != isPagedBufferTransmissionInProgress_b)
    {
        /* Set the status to Cancel the transmission */
        /*Set the cancel transmit status flag to True*/
        Dcm_isCancelTransmitInvoked_b = TRUE;
        /* Cancel the paged buffer processing for the active Service */
        DcmAppl_DcmCancelPagedBufferProcessing(Dcm_DsldGlobal_st.dataSid_u8);
    }
#endif
    /* If Cancellation of transmission is needed */
    /*Check if the cancel transmit status flag is set to True*/
    if(FALSE != Dcm_isCancelTransmitInvoked_b)
    {
        /*TRACE[SWS_Dcm_00079]*/
		/* Call the PduR API to cancel the initiated Transmission */
        cancelTxStatus = PduR_DcmCancelTransmit(Dcm_DsldGlobal_st.dataOldtxPduId_u8);

        /*TRACE[SWS_Dcm_00460]*/
		/* If cancellation is not successfull */
        if(E_OK != cancelTxStatus)
        {
            /* Set this flag to False as Cancellation was unsuccessful by lower layer , so a Txconfirmation call cannot be expected for cancelled transmit */
            Dcm_StopProtocol(Dcm_CurProtocol_u8);
            Dcm_isStopProtocolInvoked_b   = TRUE;
            Dcm_isCancelTransmitInvoked_b = FALSE;
        }
    }
    return(cancelTxStatus);
}

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
