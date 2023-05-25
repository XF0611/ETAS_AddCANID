
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"

#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
StatusType Dcm_PagedBufferTimerStatus_uchr; /* global variable to get the return value of GetCounterValue for Timer related funtions*/
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

/*This function pointer that stores the function for update page*/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
void (*Dcm_adrUpdatePage_pfct) (Dcm_MsgType PageBufPtr,Dcm_MsgLenType PageLen);
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"





/***********************************************************************************************************************
 *    Inline Function Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Function name    : Dcm_Prv_isPageLengthValid
 Syntax           : Dcm_Prv_isPageLengthValid(FilledPageLen)
 Description      : This Inline function is used to check whether page lenght is valid
 Parameter        : Dcm_MsgLenType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isPageLengthValid(Dcm_MsgLenType FilledPageLen)
{
    boolean isPageLenValid_b = FALSE;
    Dcm_DsdStatesType_ten stDsdStateTemp_en  = Dcm_Prv_GetDsdState();

    if((FilledPageLen <= (Dcm_DsldMsgContext_st.resMaxDataLen+1uL)) && \
            (stDsdStateTemp_en == DSD_CALL_SERVICE))
    {
        isPageLenValid_b = TRUE;
    }

    return isPageLenValid_b;
}





/***********************************************************************************************************************
 Function name    : DCM_Prv_PagedBufferTimerStart
 Syntax           : DCM_Prv_PagedBufferTimerStart(void)
 Description      : This Inline function is used to start paged buffer timer
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
LOCAL_INLINE void DCM_Prv_PagedBufferTimerStart(void)
{
    DCM_TimerStart(Dcm_DsldGlobal_st.dataPagedBufferTimeout_u32, 0x00u,
                   Dcm_PagedBufferStartTick_u32, Dcm_PagedBufferTimerStatus_uchr)

}




/***********************************************************************************************************************
 Function name    : DCM_Prv_PagedBufferTimerProcess
 Syntax           : DCM_Prv_PagedBufferTimerProcess(void)
 Description      : This Inline function is used to process paged buffer timer
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
LOCAL_INLINE void DCM_Prv_PagedBufferTimerProcess(void)
{
    DCM_TimerProcess(Dcm_DsldGlobal_st.dataPagedBufferTimeout_u32,Dcm_PagedBufferStartTick_u32,
            Dcm_PagedBufferTimerStatus_uchr)
}





/***********************************************************************************************************************
 *    Function Definitions
 **********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessFirstPositiveResponse
 Syntax           : Dcm_Prv_ProcessFirstPositiveResponse(void)
 Description      : This function is used to Process first positive response for page transmission
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessFirstPositiveResponse(void)
{
    /* This first call of Dcm_ProcessPage function. Frame positive response */
    Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[2] = Dcm_DsldGlobal_st.dataSid_u8 | DCM_SERVICEID_ADDEND;

    /* Active buffer pointer starts from first element in buffer */
    Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8 = &Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[2];

    /* Including Sid current page length increment by one */
    Dcm_DsldGlobal_st.dataCurrentPageRespLength_u32++;

    /* Set the flag to indicate start of paged buffer */
    Dcm_DsldGlobal_st.flgPagedBufferTxOn_b = TRUE;

    /* Change the DSD state to DSD_WAITFORTXCONF */
    Dcm_Prv_SetDsdState(DSD_WAITFORTXCONF);

    /* Trigger response in DSL_SUBSTATE_SEND_FINAL_RESPONSE state */
    /* Multicore: No Lock needed here as if Dcm gets Tx confirmation of waitpend before this line
     * DSL will be WaitForTxCOnfirm and will transmit the page immediately */
    /* And if the confirmation is received after this line then page is transmitted in the next DSL state machine */
    Dcm_Prv_SetDslNextState((DSL_STATE_RESPONSE_TRANSMISSION));
    Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_SEND_FINAL_RESPONSE));

    /* Check whether DSL is sending the wait pend or not */
    /* Multicore: No lock needed here as Dsl state is an atomic read operation */
    /* When this code is reached and paged buffer is actve there are no chances of parallel update to DSL state */
    if(Dcm_Prv_GetDslState() == DSL_STATE_WAITING_FOR_TXCONFIRMATION)
    {
        /* Empty if statement, intended here */
    }
    /*Check if positive response is sent and wait pending counter is set to Zero  */
    else if ((Dcm_DsldMsgContext_st.msgAddInfo.suppressPosResponse !=FALSE) &&
        (Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 == 0))
    {
        /* Start P2 Timer with timer value as zero,so that 0x78 can be sent in the next immediate proc. */
        DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32, 0x00u, Dcm_P2OrS3StartTick_u32,
                Dcm_P2OrS3TimerStatus_uchr)
    }
    else
    {
        /* Update data in PDU structure to register total number bytes can send on paged buffer */
        Dcm_DsldPduInfo_st.SduDataPtr = &Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[0];
        Dcm_DsldPduInfo_st.SduLength  = (PduLengthType) Dcm_DslTransmit_st.TxResponseLength_u32;

        /* change the DSL such that in next call of Dcm_provideTxBuffer page should be transmitted */
        /* Multicore: No Lock needed here because when this code is reached, paged buffer is active hence
         * there are no chances of parallel update to DSL state */
        Dcm_Prv_SetDslState((DSL_STATE_PAGEDBUFFER_TRANSMISSION));
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_DATA_READY));
        Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 = 0x0u;

        /* after getting the Tx confirmation DSL state changed to DSL_STATE_IDLE state */
        Dcm_Prv_SetDslNextState((DSL_STATE_IDLE));
        Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));

        Dcm_Prv_SendResponse(&Dcm_DsldPduInfo_st);
    }
}





/**
 **************************************************************************************************
 * Dcm_StartPagedProcessing : With this API, the application gives the complete response length to DCM
 *                          and starts PagedBuffer handling. Complete response length information
 *                          (in bytes) is given in pMsgContext-> resDataLen Callback functions are used
 *                          to provide paged buffer handling in DSP and RTE. More information can be found
 *                          in the sequence chart in chapter 9.3.6 Process Service Request with PagedBuffer.
 *
 * \param           pMsgContext: message context table given by the service
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
/* Violation of AUTOSAR to over come MISRA warning */
void Dcm_StartPagedProcessing (const Dcm_MsgContextType * pMsgContext )
{
    /* Check whether the service is sending less than maximum allowed paged buffer response buffer size for the protocol
     *  and this function is called only once*/
    if( (FALSE != Dcm_Prv_CheckTotalResponseLength(pMsgContext->resDataLen)) && \
                                                                (Dcm_DslTransmit_st.TxResponseLength_u32 == 0x00uL))
    {
        /* Store the total number of bytes that can be transmitted on paged buffer info to the global variable */
        Dcm_DslTransmit_st.TxResponseLength_u32 = pMsgContext->resDataLen+1uL;

        /* Inform the service to fill the data into page */
        if(Dcm_adrUpdatePage_pfct != NULL_PTR)
        {
            (*Dcm_adrUpdatePage_pfct)(&Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].tx_buffer_pa[3],
                             Dcm_DsldMsgContext_st.resMaxDataLen);
        }
    }
    else
    {
        DCM_DET_ERROR(DCM_PROCESSINGDONE_ID, DCM_E_INTERFACE_BUFFER_OVERFLOW)
    }
}





/**
 **************************************************************************************************
 * Dcm_ProcessPage:Application requests transmission of filled page More information can be found
 *                 in the sequence chart in chapter 9.3.6 Process Service Request with PagedBuffer.
 * \param           FilledPageLen: Filled data length in current page
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_ProcessPage(Dcm_MsgLenType FilledPageLen )
{
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    if(FALSE == Dcm_Prv_isProtocolPreemptionInitiated())
#endif
    {
        if(FALSE != Dcm_Prv_isPageLengthValid(FilledPageLen))
        {
            Dcm_DsldGlobal_st.dataCurrentPageRespLength_u32 = FilledPageLen;

            if(FALSE != Dcm_Prv_isPagedBufferTxStarted())
            {
                if (FilledPageLen != 0u)
                {
                    /* Change the DSD state to DSD_WAITFORTXCONF
                     * Multicore: DSD state is changed only in MainFunction context OR
                     * in ROE/TxConfirmation APIs when DSD is IDLE/DSD is in SEND state.
                     * So there is no parallel writing when DSD is set to DSD_CALL_SERVICE
                     */

                    Dcm_Prv_SetDsdState(DSD_WAITFORTXCONF);

                    /* This call is for sending the consecutive pages
                     * In next call of Dcm_provideTxBuffer this page will be sent
                     * Multicore: No lock needed here as Dsl state is an atomic operation
                     * Also when this part of code is reached there is no chance that
                     * there is a parallel update to DSL state as the state machine is blocked by paged buffer
                    */
                    Dcm_Prv_SetDslState((DSL_STATE_PAGEDBUFFER_TRANSMISSION));
                    Dcm_Prv_SetDslSubState((DSL_SUBSTATE_DATA_READY));
                }
                else
                {
                    /* Start Paged buffer timeout timer */
                    DCM_Prv_PagedBufferTimerStart();
                    Dcm_Prv_PagedBufferTimeout();
                }
            }
            else
            {
                Dcm_Prv_ProcessFirstPositiveResponse();
            }
        }
    }
}




/**
 **************************************************************************************************
 * Dcm_Prv_PagedBufferTimeout : Static function to monitor the Paged buffer timeout
 *
 * \param           None
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_Prv_PagedBufferTimeout(void)
{
    if(FALSE != Dcm_Prv_isPagedBufferTxStarted())
    {
        /* Process paged buffer timer */
        DCM_Prv_PagedBufferTimerProcess();

        /* Check if paged buffer timer is elapsed */
        if(FALSE != DCM_TimerElapsed(Dcm_DsldGlobal_st.dataPagedBufferTimeout_u32))
        {
            DCM_DET_ERROR(DCM_PAGEDBUFFER_ID , DCM_E_INTERFACE_TIMEOUT)

            /* Lock required here to keep a consistency between DSL and DSD state */
            SchM_Enter_Dcm_Global_NoNest();
            Dcm_Prv_SetDsdState(DSD_IDLE);
            Dcm_Prv_SetDslState((DSL_STATE_IDLE));
            Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
            SchM_Exit_Dcm_Global_NoNest();

            /* To reset the service, call the callback application */
            DcmAppl_DcmCancelPagedBufferProcessing(Dcm_DsldGlobal_st.dataSid_u8);

            Dcm_DsldGlobal_st.flgPagedBufferTxOn_b = FALSE;
        }
    }
}




/**
 **************************************************************************************************
 * Dcm_Prv_CheckTotalResponseLength : API called to check if the total response length has exceeded the
 * maximum possible response length for the API.
 *
 * \param           None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
boolean Dcm_Prv_CheckTotalResponseLength(Dcm_MsgLenType TotalResLen_u32)
{
    boolean isRespLenValid_b = FALSE;

    /* If the total response length is less than or equal to the maximum response lnegth configured for
     * paged buffer response -1(considering the SID)*/
    if(TotalResLen_u32 <= ((Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].maxResponseLength_u32)-1uL))
    {
        isRespLenValid_b = TRUE;
    }

    return (isRespLenValid_b);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
