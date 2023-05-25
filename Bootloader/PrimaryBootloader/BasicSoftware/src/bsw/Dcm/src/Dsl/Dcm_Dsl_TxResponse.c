


/*
**********************************************************************************************************************
* Includes
**********************************************************************************************************************
*/
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "DcmCore_DslDsd_Prot.h"
#include "Rte_Dcm.h"
/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_DslTxType_tst Dcm_DslTransmit_st;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
Dcm_OBDTxType_tst Dcm_OBDTransmit_st;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"


#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
boolean Dcm_isSessionStored_b;
#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF) && (DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
boolean Dcm_isFirstReponseSent_b;  /* flag for KWP first response sent when the split response feature is enabled */
static boolean Dcm_isApplicationCalled_b;   /* flag to indicate whether DcmAppl_DcmConfirmation has been called or not in case of splitting of responses */
#endif
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"






/***********************************************************************************************************************
 *    Inline Function Definitions
 **********************************************************************************************************************/

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : DCM_P2TimerStart
 Syntax           : DCM_P2TimerStart
 Description      : This Inline function is used to start P2 timer
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
LOCAL_INLINE void DCM_P2TimerStart(void)
{
    DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32,(Dcm_DsldTimer_st.dataTimeoutP2max_u32 - \
             Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].dataP2TmrAdjust), \
             Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
}
#endif


/***********************************************************************************************************************
 Function name    : Dcm_Prv_isConfirmationOnActiveConnection
 Syntax           : Dcm_Prv_isConfirmationOnActiveConnection(DcmTxPduId)
 Description      : This Inline function is used to check confirmation received on active connection
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isConfirmationOnActiveConnection(PduIdType DcmTxPduId)
{
    return (Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId] == Dcm_DsldGlobal_st.dataActiveTxPduId_u8);
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_isOBDConfirmationOnActiveConnection
 Syntax           : Dcm_Prv_isOBDConfirmationOnActiveConnection(DcmTxPduId)
 Description      : This Inline function is used to check confirmation received on active connection for OBD
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
/* Check whether OBD TxConfirmation received is for the active connection */
LOCAL_INLINE boolean Dcm_Prv_isOBDConfirmationOnActiveConnection(PduIdType DcmTxPduId)
{
    return (Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId] == Dcm_OBDGlobal_st.dataActiveTxPduId_u8);
}
#endif
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isPositiveResponseSupressed
 Syntax           : Dcm_Prv_isPositiveResponseSupressed(&pMsgContext)
 Description      : This Inline function is used to check if Positive reponse is to be supressed
 Parameter        : Dcm_MsgContextType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isPositiveResponseSupressed(\
        const Dcm_MsgContextType * pMsgContext)
{
    return((FALSE != (pMsgContext->msgAddInfo).suppressPosResponse) && \
            (Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 == 0x00u));
}


/***********************************************************************************************************************
 Function name    : Dcm_Prv_isNegativeResponseSupressed
 Syntax           : Dcm_Prv_isNegativeResponseSupressed(&pMsgContext,NrcValue_u8)
 Description      : This Inline function is used to check if Negative response is to be supressed
 Parameter        : Dcm_MsgContextType,uint8
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isNegativeResponseSupressed(\
        const Dcm_MsgContextType * pMsgContext,uint8 NrcValue_u8)
{
    /*
     * This is functional UDS request, suppress the response for NRCs which are configured for suppression in
     * the generated MACRO DCM_CFG_SUPPRESS_NRC(NrcValue_u8)
     * These NRCs are suppressed only if a previous wait pend response is not sent
     * For KWP request below suppression is not allowed
     */
    return((pMsgContext->dcmRxPduId >= DCM_INDEX_FUNC_RX_PDUID) && (DCM_IS_KWPPROT_ACTIVE() == FALSE) && \
               (DCM_CFG_SUPPRESS_NRC(NrcValue_u8)) && (Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 == 0x00u));
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_isDcmWaitingForTxConfirmation
 Syntax           : Dcm_Prv_isDcmWaitingForTxConfirmation(void)
 Description      : This Inline Function is used to check if Dcm is wating for the Confirmation from the lower layer
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isDcmWaitingForTxConfirmation(void)
{
    return (!(((Dcm_Prv_GetDslState() == DSL_STATE_REQUEST_RECEIVED)           && \
            (Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_P2MAX_TIMEMONITORING))   || \
            ((Dcm_Prv_GetDslState()   == DSL_STATE_RESPONSE_TRANSMISSION)      && \
            (Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_SEND_GENERAL_REJECT))    || \
            (Dcm_Prv_GetDslState()    == DSL_STATE_ROETYPE1_RECEIVED)));
}



/***********************************************************************************************************************
 Function name    : Dcm_Prv_isNormalResponseConfirmationProcessed
 Syntax           : Dcm_Prv_isNormalResponseConfirmationProcessed(void)
 Description      : This Inline function is used to check whether processing of confirmation for the Normal response is
                    Completed
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isNormalResponseConfirmationProcessed(void)
{
    return((Dcm_Prv_GetDsdState() == DSD_SENDTXCONF_APPL) && \
       (Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest == DCM_UDS_TESTER_SOURCE) && \
       (Dcm_DsldGlobal_st.idxActiveSession_u8 == DCM_DEFAULT_SESSION_IDX)
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
       && (Dcm_Prv_GetDslPreemptionState() != DSL_SUBSTATE_STOP_PROTOCOL)
#endif
        );
}




#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF) && (DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isKWPSplitResponseTimeout
 Syntax           : Dcm_Prv_isKWPSplitResponseTimeout(void)
 Description      : This Inline function is used to check check if KWP split response timeout has occured
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isKWPSplitResponseTimeout(void)
{
    boolean isResponseTimeout_b = FALSE;
    Dcm_DsdStatesType_ten stDsdStateTemp_en  = Dcm_Prv_GetDsdState();

    if((DCM_IS_KWPPROT_ACTIVE() != FALSE) && (Dcm_isFirstReponseSent_b!=FALSE) && \
            (Dcm_DsldGlobal_st.idxActiveSession_u8 == DCM_DEFAULT_SESSION_IDX) && \
       ((stDsdStateTemp_en == DSD_CALL_SERVICE)||(stDsdStateTemp_en == DSD_WAITFORTXCONF)))
    {
        isResponseTimeout_b = TRUE;
    }

    return isResponseTimeout_b;
}
#endif



/***********************************************************************************************************************
 Function name    : Dcm_Prv_isConfirmationForPendingResponse
 Syntax           : Dcm_Prv_isConfirmationForPendingResponse(void)
 Description      : This Inline function is used to check if confirmation received is for Pending Response
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isConfirmationForPendingResponse(void)
{
    return (Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 > 0x0u);
}





#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF) && (DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isConfirmationForKWPResponse
 Syntax           : Dcm_Prv_isConfirmationForKWPResponse(Result)
 Description      : This Inline function is used to check if Confirmation received is for KWP request
 Parameter        : Result
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isConfirmationForKWPResponse(Std_ReturnType Result)
{
    return ((Result == E_OK) && \
            (DCM_IS_KWPPROT_ACTIVE() != FALSE) && \
            (Dcm_DsldGlobal_st.dataResponseByDsd_b == FALSE ) && \
            (Dcm_DsldGlobal_st.stResponseType_en == DCM_DSLD_POS_RESPONSE));
}
#endif



#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isConfirmationForRoeType2Response
 Syntax           : Dcm_Prv_isConfirmationForRoeType2Response(DcmTxPduId)
 Description      : This Inline function is used to check if confirmation for Roe Type2 is to be processed
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isConfirmationForRoeType2Response(PduIdType DcmTxPduId)
{
    return ((Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_Roe2MesContext_st.dcmRxPduId]].roetype2_txpdu_u8 == \
            Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId]) && \
            (Dcm_DsdRoe2State_en == DSD_WAITFORTXCONF));
}
#endif




/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckWaitPendCounterOverflow
 Syntax           : Dcm_Prv_CheckWaitPendCounterOverflow(void)
 Description      : This Inline function is used to check whether wait pend counter is overflow
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_CheckWaitPendCounterOverflow(void)
{
    return((Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 < DCM_CFG_MAX_WAITPEND) \
            || (DCM_CFG_MAX_WAITPEND == DCM_MAXNUMRESPONSEPENDING));
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_isConfirmationReceivedForNrc21Response
 Syntax           : Dcm_Prv_isConfirmationReceivedForNrc21Response(DcmTxPduId,idxRxPduId)
 Description      : This Inline function is used to check if confirmation for NRC21 response to be processed
 Parameter        : PduIdType,uint8
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isConfirmationReceivedForNrc21Response(PduIdType DcmTxPduId,
        uint8 idxRxPduId)
{
    boolean confirmStatus_b = FALSE;
    uint8 ConnectionId_u8     = Dcm_DsldRxTable_pcu8[idxRxPduId];
    uint8 idxProtocol_u8      = Dcm_DsldConnTable_pcst[ConnectionId_u8].protocol_num_u8;
    PduIdType idxTxpduid_u8   = Dcm_DsldConnTable_pcst[ConnectionId_u8].txpduid_num_u8;

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    uint8 ServiceId =    Dcm_Prv_IsTxPduIdOBD(DcmTxPduId)?
                         Dcm_DslOBDRxPduArray_ast[idxRxPduId].Dcm_DslServiceId_u8:
                         Dcm_DslRxPduArray_ast[idxRxPduId].Dcm_DslServiceId_u8;
#else
    uint8 ServiceId = Dcm_DslRxPduArray_ast[idxRxPduId].Dcm_DslServiceId_u8;
#endif
    /* Check whether NRC21 is set to TRUE in protocol configuration
     * Whether a valid ServiceId is stored (done only when NRC21 has to be sent)
     * Match the looping RxPduId to the TxPduId obtained during confirmation
     */
    if (( FALSE   != Dcm_DsldProtocol_pcst[idxProtocol_u8].nrc21_b)          && \
        ( DCM_SERVICEID_DEFAULT_VALUE !=  ServiceId )   && \
        ( idxTxpduid_u8 == Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId] ))
    {
        confirmStatus_b = TRUE;
    }
    return confirmStatus_b;
}


#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED != DCM_CFG_OFF) )
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isResponseSentForDSCService
 Syntax           : Dcm_Prv_isResponseSentForDSCService(void)
 Description      : This Inline function is used to check if response sent for DSC service
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isResponseSentForDSCService(void)
{
    return ((Dcm_DsldGlobal_st.dataSid_u8 == DCM_SID_DIAGNOSTICSESSIONCONTROL) &&
            (Dcm_DsldGlobal_st.stResponseType_en == DCM_DSLD_POS_RESPONSE) &&
            (Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest == DCM_UDS_TESTER_SOURCE));
}
#endif





#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isHighPriorityRequestReceiving
 Syntax           : Dcm_Prv_isHighPriorityRequestReceiving(void)
 Description      : This Function is used to check whether High priority request is being received.
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isHighPriorityRequestReceiving(void)
{
    boolean isRequestReceiving_b = FALSE;
    Dcm_DsdStatesType_ten stDsdStateTemp_en  = Dcm_Prv_GetDsdState();

    if(((Dcm_Prv_GetDslPreemptionState() == DSL_SUBSTATE_STOP_PROTOCOL_RECEIVING) ||\
             (Dcm_Prv_GetDslPreemptionState() == DSL_SUBSTATE_STOP_PROTOCOL))          &&\
             (stDsdStateTemp_en == DSD_WAITFORTXCONF))
    {
        isRequestReceiving_b = TRUE;
    }

    return isRequestReceiving_b;
}
#endif





/***********************************************************************************************************************
 *    Function Definitions
 **********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/***********************************************************************************************************************
 Function name    : Dcm_Prv_CanComMBeInactivated
 Syntax           : Dcm_Prv_CanComMBeInactivated(Context)
 Description      : If Parallel processing is enabled, check whether the same channel is shared
                     If yes, only when the other state machine is free, invoke inactive diagnosic
                     else inacttive diagnostic can be directly invoked
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
static boolean Dcm_Prv_CanComMBeInactivated(boolean Context)
{
    boolean AllowInactivation = FALSE;

    /* If same channel is not shared, then channel can be inactivated */
    if( (Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId)
      != (Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_OBDGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId))
    {
        AllowInactivation = TRUE;
    }
    /* Check whether Dcm State Machine is free, only then inactive the channel */
    else if(Context == DCM_OBDCONTEXT)
    {
        if(Dcm_Prv_GetDslState() == DSL_STATE_IDLE)
        {
            AllowInactivation = TRUE;
        }
    }
    /* Check whether OBD State Machine is free, only then inactive the channel */
    else
    {
        if(Dcm_Prv_GetOBDState() == DCM_OBD_IDLE)
        {
            AllowInactivation = TRUE;
        }
    }
    return AllowInactivation;
}
#endif
/***********************************************************************************************************************
 Function name    : Dcm_Prv_InactivateComMChannel
 Syntax           : Dcm_Prv_InactivateComMChannel(void)
 Description      : This Function is used to release ComM channel after the reception of confirmation is processed.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00165][SWS_Dcm_00166][SWS_Dcm_00697][SWS_Dcm_00168][SWS_Dcm_00170]*/
static void Dcm_Prv_InactivateComMChannel(void)
{

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    if(FALSE != Dcm_Prv_CanComMBeInactivated(DCM_UDSCONTEXT))
#endif
    {
        SchM_Enter_Dcm_Global_NoNest();
        if(FALSE != Dcm_Prv_isNormalResponseConfirmationProcessed())
        {
            ComM_DCM_InactiveDiagnostic(Dcm_active_commode_e[\
                             Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId);
        }
        SchM_Exit_Dcm_Global_NoNest();

#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF) && (DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
        if(FALSE != Dcm_Prv_isKWPSplitResponseTimeout())
        {
            ComM_DCM_InactiveDiagnostic(Dcm_active_commode_e[\
                             Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId);
        }
#endif
    }
}




#if((DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED != DCM_CFG_OFF) && (DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF))
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessSessionChangeOnWarmResp
 Syntax           : Dcm_Prv_ProcessSessionChangeOnWarmResp(void)
 Description      : This function is used to process session chnage after warm response
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessSessionChangeOnWarmResp (void)
{
    Dcm_SesChgOnWarmResp_b = FALSE;

#if (DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF)
    (void)SchM_Switch_Dcm_DcmDiagnosticSessionControl(Dcm_Dsp_Session[Dcm_ctDiaSess_u8].SessionMode);
#endif
    (void)DcmAppl_Switch_DcmDiagnosticSessionControl(Dcm_Dsp_Session[Dcm_ctDiaSess_u8].session_level);

    /* Update the P2 Timer values */
    Dcm_DsldSetsessionTiming(Dcm_Dsp_Session[Dcm_ctDiaSess_u8].P2str_max_u32,Dcm_Dsp_Session[Dcm_ctDiaSess_u8].P2_max_u32);

    /* Activate New Session requested */
    Dcm_Prv_SetSesCtrlType(Dcm_Dsp_Session[Dcm_ctDiaSess_u8].session_level);
}
#endif





/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdConfirmation
 Syntax           : Dcm_Prv_DsdConfirmation(void)
 Description      : In this function is used to change the DSD state once the response is sent
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00117]*/
static void Dcm_Prv_DsdConfirmation (void)
{
    if(Dcm_Prv_GetDslState() != DSL_STATE_ROETYPE1_RECEIVED)
    {
        /*TRACE[SWS_Dcm_00141]*/
        Dcm_Prv_ReloadS3Timer();
    }

    if(Dcm_Prv_GetDsdState() == DSD_WAITFORTXCONF)
    {
        /* In next call of Dcm_MainFunction() confirmation is called to Application*/
        Dcm_Prv_SetDsdState(DSD_SENDTXCONF_APPL);
    }
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    else
    {
        /*Check if this is to trigger the PduR for paged buffer transmission*/
        if(Dcm_DsldGlobal_st.flgPagedBufferTxOn_b != FALSE)
        {
            Dcm_Prv_SetDsdState(DSD_SENDTXCONF_APPL);
        }
    }
#endif

#if((DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED != DCM_CFG_OFF) && (DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF))
    if (Dcm_SesChgOnWarmResp_b == TRUE)
    {
        Dcm_Prv_ProcessSessionChangeOnWarmResp();
    }
#endif
        Dcm_Prv_InactivateComMChannel();
}


#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED != DCM_CFG_OFF) )
/***********************************************************************************************************************
 Function name    : Dcm_Prv_SetNewSession
 Syntax           : Dcm_Prv_SetNewSession(void)
 Description      : In this function the session is changed immediately to avoid delays in setting
                    a new session there by ensuring that the new request coming in with lower priority or same priority
                    is not accepted for reception. Also this function shall be called only from the DSC confirmation results
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_SetNewSession(void)
{
    uint8 nrSessions_u8 = 0u;
    uint8 idxSession_u8 = 0u;

    if(FALSE != Dcm_Prv_isResponseSentForDSCService())
    {
        /* store old session for invoking the session change later in next MainFunction to get new timings */
        Dcm_DsldGlobal_st.PreviousSessionIndex = Dcm_DsldGlobal_st.idxActiveSession_u8;

        /* Calculate the number of sessions configured in ECU for particular protocol*/
#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
        if(DCM_IS_KWPPROT_ACTIVE() != FALSE)
        {
            nrSessions_u8 = DCM_CFG_NUM_KWP_SESSIONS;
        }
        else
        {
            nrSessions_u8 = DCM_CFG_NUM_UDS_SESSIONS;
        }
#else
        nrSessions_u8 = DCM_CFG_NUM_UDS_SESSIONS;
#endif

        /* get the index of requested session id in session look up table */
        for(idxSession_u8 = 0x0; idxSession_u8 < nrSessions_u8 ; idxSession_u8++)
        {
            if(Dcm_DsldSessionTable_pcu8[idxSession_u8]== Dcm_Dsp_Session[Dcm_ctDiaSess_u8].session_level)
            {
                /* session found - Set the new session to ensure that there are no requests of lower/same
                 * priority protocol received*/
                Dcm_DsldGlobal_st.idxActiveSession_u8  = idxSession_u8;
                break;
            }
        }
        /* Update the status flag to identify that the session is stored in confirmation */
        Dcm_isSessionStored_b =  TRUE;
    }
}
#endif





/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckP2StarTimer
 Syntax           : Dcm_Prv_CheckP2StarTimer(void)
 Description      : This Function is used to Check if time difference between two response pend is greater the half
                    of P2*
 Parameter        : None
 Return value     : Std_ReturnType
***********************************************************************************************************************/
static Std_ReturnType Dcm_Prv_CheckP2StarTimer(void)
{
    Std_ReturnType halfP2timeStatus = E_NOT_OK;
    uint32 halfP2timer_u32   = 0u;

#if(DCM_CFG_OSTIMER_USE != FALSE)
    uint32 currentOSTimerTicks_u32 = 0u;
#endif

    if(Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 == 0u)
    {
        halfP2timeStatus = E_OK;
    }
    else
    {
        halfP2timer_u32 = Dcm_DsldTimer_st.dataTimeoutP2StrMax_u32 >> 1u;

#if(DCM_CFG_OSTIMER_USE != FALSE)
        /* Get the current ticks from system timer  */
        Dcm_P2OrS3TimerStatus_uchr = Dcm_GetCounterValue(DCM_CFG_COUNTERID, (&currentOSTimerTicks_u32));

        if (E_OK == Dcm_P2OrS3TimerStatus_uchr)
        {
            if((DCM_CFG_TICKS2US_COUNTER(currentOSTimerTicks_u32 - Dcm_P2OrS3StartTick_u32)) >= (halfP2timer_u32))
            {
                halfP2timeStatus = E_OK;
            }
        }
        else
        {
            if(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32 < (halfP2timer_u32/DCM_CFG_TASK_TIME_US))
            {
                halfP2timeStatus = E_OK;
            }
        }
#else
        if(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32 < (halfP2timer_u32/DCM_CFG_TASK_TIME_US))
        {
            halfP2timeStatus = E_OK;
        }
#endif
    }

    return(halfP2timeStatus);
}




/*
***********************************************************************************************************
*  Dcm_TriggerTransmit :  The lower layer communication module requests the buffer of the SDU
*                         for transmission from the upper layer module.
*
*  \param   DcmRxPduId     ID of DCM I-PDU that has been transmitted.
*            PduInfoPtr    Contains a pointer to a buffer (SduDataPtr)
*                          to where the SDU shall be copied to. On return,
*                          the service will indicate the length of
*                          the copied SDU data in SduLength.
*  \retval:    None
*  \seealso
*
***********************************************************************************************************/
/*MR12 RULE 8.13 VIOLATION:The object addressed by the pointer parameter 'PduInfoPtr' is not modified and so the pointer could be of type 'pointer to const'. MISRA C:2012 Rule-8.13*/
void Dcm_TriggerTransmit(PduIdType TxPduId,PduInfoType * PduInfoPtr )
{
    /*Do Nothing, this function is not used by Dcm and is provided only to solve compilation issues*/
    (void)TxPduId;
    (void)PduInfoPtr;
}






#if((DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF) && (DCM_CALLAPPLICATIONONREQRX_ENABLED != DCM_CFG_OFF))
/***********************************************************************************************************************
 Function name    : Dcm_Prv_InformApplicationAfterQueueComplete
 Syntax           : Dcm_Prv_InformApplicationAfterQueueComplete(QueueHandlingTemp_en)
 Description      : This Function is used to inform application when Queued request is processed
 Parameter        : Dcm_DsldQueHandling_ten
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_InformApplicationAfterQueueComplete (Dcm_DsldQueHandling_ten QueueHandlingTemp_en)
{
    if(QueueHandlingTemp_en == DCM_QUEUE_RUNNING)
    {
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2852]*/
        (void)DcmAppl_StartOfReception(Dcm_DslRxPduArray_ast[Dcm_DsldGlobal_st.dataActiveRxPduId_u8].Dcm_DslBufferPtr_pu8[0],\
                Dcm_DsldGlobal_st.dataActiveRxPduId_u8,Dcm_DsldGlobal_st.dataRequestLength_u16,\
                (Dcm_DslRxPduArray_ast[Dcm_DsldGlobal_st.dataActiveRxPduId_u8].Dcm_DslBufferPtr_pu8));

        (void)DcmAppl_CopyRxData(Dcm_DsldGlobal_st.dataActiveRxPduId_u8,\
                ((Dcm_QueueStructure_st.dataQueueReqLength_u16 - Dcm_DslRxPduArray_ast[\
                      Dcm_DsldGlobal_st.dataActiveRxPduId_u8].Dcm_DslRxPduBuffer_st.SduLength)));

    }
    else
    {
        if(QueueHandlingTemp_en == DCM_QUEUE_COMPLETED)
        {
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2852]*/
            (void)DcmAppl_StartOfReception(Dcm_DsldGlobal_st.adrBufferPtr_pu8[0],Dcm_DsldGlobal_st.dataActiveRxPduId_u8,\
                    Dcm_DsldGlobal_st.dataRequestLength_u16,(Dcm_DsldGlobal_st.adrBufferPtr_pu8));

            (void)DcmAppl_CopyRxData(Dcm_DsldGlobal_st.dataActiveRxPduId_u8,Dcm_DsldGlobal_st.dataRequestLength_u16);

            (void)DcmAppl_TpRxIndication(Dcm_DsldGlobal_st.dataActiveRxPduId_u8,E_OK);
        }
    }
}
#endif





#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessRequestInQueue
 Syntax           : Dcm_Prv_ProcessRequestInQueue(void)
 Description      : This Function is used to take up the Queued request for processing
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessRequestInQueue(void)
{
    Dcm_DsldQueHandling_ten QueueHandlingTemp_en;

    SchM_Enter_Dcm_Global_NoNest();
    QueueHandlingTemp_en = Dcm_QueueStructure_st.Dcm_QueHandling_en;
    SchM_Exit_Dcm_Global_NoNest();

    switch(QueueHandlingTemp_en)
    {
    case DCM_QUEUE_IDLE :
            SchM_Enter_Dcm_Global_NoNest();
            Dcm_Prv_SetDslState(DSL_STATE_IDLE);
            Dcm_Prv_SetDslSubState(DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING);
            SchM_Exit_Dcm_Global_NoNest();
        break;

    case DCM_QUEUE_RUNNING :

            SchM_Enter_Dcm_Global_NoNest();
            Dcm_DsldGlobal_st.dataActiveRxPduId_u8  = Dcm_QueueStructure_st.dataQueueRxPduId_u8;
            Dcm_DsldGlobal_st.dataRequestLength_u16 = Dcm_QueueStructure_st.dataQueueReqLength_u16;
            Dcm_DslRxPduArray_ast[Dcm_DsldGlobal_st.dataActiveRxPduId_u8].Dcm_DslBufferPtr_pu8 = \
                                                                            Dcm_QueueStructure_st.adrBufferPtr_pu8;

            Dcm_QueueStructure_st.Dcm_QueHandling_en = DCM_QUEUE_IDLE;
            Dcm_Prv_SetDslState(DSL_STATE_WAITING_FOR_RXINDICATION);
            SchM_Exit_Dcm_Global_NoNest();

#if(DCM_CALLAPPLICATIONONREQRX_ENABLED != DCM_CFG_OFF)
            Dcm_Prv_InformApplicationAfterQueueComplete(DCM_QUEUE_RUNNING);
#endif

        break;

    case DCM_QUEUE_COMPLETED :

            SchM_Enter_Dcm_Global_NoNest();
            if (Dcm_DsldGlobal_st.idxActiveSession_u8 == DCM_DEFAULT_SESSION_IDX)
            {
                 Dcm_CheckActiveDiagnosticStatus(Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[\
                                          Dcm_QueueStructure_st.dataQueueRxPduId_u8]].channel_idx_u8].ComMChannelId);
            }

            /* move all the Dcm Queue variables to active variable */
            Dcm_DsldGlobal_st.dataActiveRxPduId_u8  = Dcm_QueueStructure_st.dataQueueRxPduId_u8;
            Dcm_DsldGlobal_st.dataRequestLength_u16 = Dcm_QueueStructure_st.dataQueueReqLength_u16;
            Dcm_DsldGlobal_st.adrBufferPtr_pu8      = Dcm_QueueStructure_st.adrBufferPtr_pu8;

            Dcm_QueueStructure_st.Dcm_QueHandling_en = DCM_QUEUE_IDLE;

            Dcm_Prv_SetDslState(DSL_STATE_REQUEST_RECEIVED);
            Dcm_Prv_SetDslSubState(DSL_SUBSTATE_START_PROTOCOL);
            SchM_Exit_Dcm_Global_NoNest();
            DCM_P2TimerStart();

#if(DCM_CALLAPPLICATIONONREQRX_ENABLED != DCM_CFG_OFF)
            Dcm_Prv_InformApplicationAfterQueueComplete(DCM_QUEUE_COMPLETED);
#endif
        break;

    default :
        /*Do nothing*/
        break;
    }
}
#endif





/***********************************************************************************************************************
 Function name    : Dcm_SetNegResponse
 Syntax           : Dcm_SetNegResponse(&pMsgContext,ErrorCode)
 Description      : This API is used to update Tx Buffer with the Negative response.
 Parameter        : const pMsgContext*, ErrorCode
 Return value     : None
***********************************************************************************************************************/
void Dcm_SetNegResponse ( const Dcm_MsgContextType * pMsgContext,
                                         Dcm_NegativeResponseCodeType ErrorCode)
{

#if((DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF))
    Dcm_MsgItemType * responseBuf_pu8 = NULL_PTR;
#endif

    if((pMsgContext->msgAddInfo.sourceofRequest == DCM_UDS_TESTER_SOURCE) || \
          (Dcm_Prv_GetDslState() == DSL_STATE_ROETYPE1_RECEIVED))
    {
        if(pMsgContext->dcmRxPduId == Dcm_DsldGlobal_st.dataActiveRxPduId_u8)
        {
            /* check if this is the first call of Dcm_SetNegResponse() response */
            /* Multicore: Lock is necessary to ensure that the protocol preemption is not started when the below code is being executed */
            SchM_Enter_Dcm_Global_NoNest();
            if(Dcm_DsldGlobal_st.stResponseType_en == DCM_DSLD_POS_RESPONSE)
            {
                Dcm_DsldGlobal_st.stResponseType_en = DCM_DSLD_NEG_RESPONSE;

                Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[0]= DCM_NEGRESPONSE_INDICATOR;
                Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[1]= Dcm_DsldGlobal_st.dataSid_u8;
                Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[2]= ErrorCode;
            }
            SchM_Exit_Dcm_Global_NoNest();
        }
    }
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
    else if(pMsgContext->msgAddInfo.sourceofRequest == DCM_ROE_SOURCE)
    {
        /* Check whether this is the first call */
        if(Dcm_Roe2ResponseType_en == DCM_DSLD_POS_RESPONSE)
        {
            /* Set the negative response type  */
            Dcm_Roe2ResponseType_en = DCM_DSLD_NEG_RESPONSE;
            responseBuf_pu8 = Dcm_DsldRoe2_pcst->txbuffer_ptr;

            responseBuf_pu8[0] = DCM_NEGRESPONSE_INDICATOR;
            responseBuf_pu8[1] = pMsgContext->idContext;
            responseBuf_pu8[2] = ErrorCode;
        }
    }
#endif
    else
    {
#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
        if(pMsgContext->msgAddInfo.sourceofRequest == DCM_RDPI_SOURCE)
        {
            /* Check if this is the first call */
            if(Dcm_Rdpi2ResponseType_en == DCM_DSLD_POS_RESPONSE)
            {
                /* Set the negative response type  */
                Dcm_Rdpi2ResponseType_en = DCM_DSLD_NEG_RESPONSE;
            }
        }
#endif
    }
}



#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_UpdateTxPduIdOnRoeEvent
 Syntax           : Dcm_Prv_UpdateTxPduIdOnRoeEvent(void)
 Description      : This Function is used to update TxPduid if Roe event is being processed.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static boolean Dcm_Prv_UpdateTxPduIdOnRoeEvent(void)
{
    boolean retVal_b        = TRUE;
    uint8 connectionId_u8   = 0u;

    connectionId_u8 = Dcm_DsldRxTable_pcu8[Dcm_DsldGlobal_st.dataActiveRxPduId_u8];

#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    if(FALSE == Dcm_Prv_isProtocolPreemptionInitiated())
    {
        Dcm_DsldGlobal_st.dataActiveTxPduId_u8 = Dcm_DsldConnTable_pcst[connectionId_u8].txpduid_num_u8;
    }
    else
    {
        retVal_b = FALSE;
    }
#else
    Dcm_DsldGlobal_st.dataActiveTxPduId_u8 = Dcm_DsldConnTable_pcst[connectionId_u8].txpduid_num_u8;
#endif

    return retVal_b;
}
#endif



/***********************************************************************************************************************
 Function name    : Dcm_Prv_SendResponse
 Syntax           : Dcm_Prv_SendResponse(&adrPduStrucutre_pcst)
 Description      : This Function is used to Send ALL types of responses to Lower layer by calling PduR_DcmTransmit
 Parameter        : const PduInfoType*
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00115][SWS_Dcm_00118]*/
void Dcm_Prv_SendResponse(const PduInfoType * adrPduStrucutre_pcst)
{
    boolean Result_b = TRUE;

#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    if(Dcm_Prv_GetDslState() == DSL_STATE_ROETYPE1_RECEIVED)
    {
        Result_b = Dcm_Prv_UpdateTxPduIdOnRoeEvent();
    }
    if(FALSE != Result_b)
#endif
    {
        Result_b = FALSE;
        if( DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8) )
        {
            if(E_NOT_OK != PduR_DcmTransmit(Dcm_DsldGlobal_st.dataActiveTxPduId_u8, adrPduStrucutre_pcst))
            {
                Result_b = TRUE;
            }
        }
    }

    if(FALSE == Result_b)
    {
        if(Dcm_Prv_GetDslNextState() != DSL_STATE_IDLE)
        {
            /* Unable to send wait pend, again go to P2 max monitoring */
            Dcm_Prv_SetDslState((DSL_STATE_REQUEST_RECEIVED));
            Dcm_Prv_SetDslSubState((DSL_SUBSTATE_P2MAX_TIMEMONITORING));

            if(FALSE != Dcm_Prv_isForcePendingResponse())
            {
                Dcm_DslTransmit_st.isForceResponsePendRequested_b = FALSE;

#if(DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)
                Dcm_Prv_ConfirmationRespPendForBootloader(DCM_RES_NEG_NOT_OK);
#endif
                (void)Dcm_ConfirmationRespPend(DCM_RES_NEG_NOT_OK);
            }
        }
        else
        {
            Dcm_DsldGlobal_st.dataResult_u8 = E_NOT_OK;
            Dcm_isGeneralRejectSent_b       = FALSE;
            Dcm_Prv_DsdConfirmation();

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
            Dcm_Prv_ProcessRequestInQueue();
#else
            Dcm_Prv_SetDslState((DSL_STATE_IDLE));
            Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
#endif
        }
    }
}



/**********************************************************************************************************************
 *                                  TRIGGER TRANSMIT FUNCTIONALITY
 *********************************************************************************************************************/


/***********************************************************************************************************************
 Function name    : Dcm_Prv_TransmitNormalResponse
 Syntax           : Dcm_Prv_TransmitNormalResponse(void)
 Description      : This Function is used to transmit the Normal Response
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_TransmitNormalResponse(void)
{
    boolean SendResponse_b = FALSE;

    /*Check if Respose is to be suppressed.If not then send current response*/
    if(Dcm_DslTransmit_st.TxResponseLength_u32 == 0u)
    {
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2861]*/
#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED != DCM_CFG_OFF) )
        Dcm_Prv_SetNewSession();
#endif

        /* simulate successful transmission of response */
        Dcm_DsldGlobal_st.dataResult_u8 = E_OK;
        Dcm_Prv_DsdConfirmation();

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
        Dcm_Prv_ProcessRequestInQueue();
#else
        Dcm_Prv_SetDslState((DSL_STATE_IDLE));
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
    #if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
        Dcm_Prv_SetDslPreemptionState((DSL_STATE_IDLE));
    #endif
#endif
    }
    else
    {
        SchM_Enter_Dcm_Global_NoNest();
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
        if(FALSE == Dcm_Prv_isProtocolPreemptionInitiated())
#endif
        {
            if(FALSE == Dcm_Prv_isDcmWaitingForTxConfirmation())
            {
                /* update data in pdu structure */
                Dcm_DsldPduInfo_st.SduDataPtr = Dcm_DslTransmit_st.TxBuffer_tpu8;
                Dcm_DsldPduInfo_st.SduLength = (PduLengthType) Dcm_DslTransmit_st.TxResponseLength_u32;

                /* final response comes now. reset the wait pend counter */
                Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 = 0x0u;

                if(Dcm_Prv_GetDslState() != DSL_STATE_ROETYPE1_RECEIVED)
                {
                    Dcm_Prv_SetDslState((DSL_STATE_WAITING_FOR_TXCONFIRMATION));
                    Dcm_Prv_SetDslNextState((DSL_STATE_IDLE));
                    Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
                }

                SendResponse_b = TRUE;
            }
            else
            {
                if(Dcm_Prv_GetDslState() == DSL_STATE_WAITING_FOR_TXCONFIRMATION)
                {
                    Dcm_Prv_SetDslNextState((DSL_STATE_RESPONSE_TRANSMISSION));
                    Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_SEND_FINAL_RESPONSE));
                }
            }
        }
        SchM_Exit_Dcm_Global_NoNest();

        if(SendResponse_b != FALSE)
        {
            Dcm_Prv_SendResponse(&Dcm_DsldPduInfo_st);
        }
    }
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_TransmitPendingReponse
 Syntax           : Dcm_Prv_TransmitPendingReponse(void)
 Description      : This Function is used to transmit Pending response
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_TransmitPendingReponse(void)
{
    if ((Dcm_Prv_GetDslState() == DSL_STATE_REQUEST_RECEIVED) &&\
            (Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_P2MAX_TIMEMONITORING))
    {
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
        if(FALSE == Dcm_Prv_isProtocolPreemptionInitiated())
#endif
        {
            /* Multicore: Locking is necessary here to delay parallel Rx and buffer tampering- protocol prremption */
            SchM_Enter_Dcm_Global_NoNest();

            /* Frame intermediate Negative response based on the NRC */
            Dcm_DslWaitPendBuffer_au8[0] = DCM_NEGRESPONSE_INDICATOR;
            Dcm_DslWaitPendBuffer_au8[1] = Dcm_DsldGlobal_st.dataSid_u8;
            Dcm_DslWaitPendBuffer_au8[2] = DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING ;

            /* update data in pdu structure */
            Dcm_DsldPduInfo_st.SduDataPtr = Dcm_DslWaitPendBuffer_au8;
            Dcm_DsldPduInfo_st.SduLength  = DCM_NEGATIVE_RESPONSE_LENGTH;

            if(Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 < DCM_MAXNUMRESPONSEPENDING)
            {
                /* increment wait pend counter to indicate one wait pend is sent */
                Dcm_DsldGlobal_st.cntrWaitpendCounter_u8++;
            }
            Dcm_Prv_SetDslState((DSL_STATE_WAITING_FOR_TXCONFIRMATION));
            Dcm_Prv_SetDslNextState((DSL_STATE_REQUEST_RECEIVED));
            Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_P2MAX_TIMEMONITORING));

            SchM_Exit_Dcm_Global_NoNest();

            Dcm_Prv_SendResponse(&Dcm_DsldPduInfo_st);
        }
    }
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_TriggerTransmit
 Syntax           : Dcm_Prv_TriggerTransmit(void)
 Description      : This Function is used to Transmit Current Response (e.g.Pending and Normal Response)
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00114]*/
void Dcm_Prv_TriggerTransmit(void)
{
    if(FALSE != Dcm_Prv_isForcePendingResponse())
    {
        /* CHeck if time difference between two response pend is greater the half of P2* max  */
        if(E_OK == Dcm_Prv_CheckP2StarTimer())
        {
            if(FALSE != Dcm_Prv_CheckWaitPendCounterOverflow())
            {
                Dcm_Prv_TransmitPendingReponse();
            }
            else
            {
                Dcm_Prv_SetDslState((DSL_STATE_RESPONSE_TRANSMISSION));
                Dcm_Prv_SetDslSubState((DSL_SUBSTATE_SEND_GENERAL_REJECT));
                Dcm_DslTransmit_st.isForceResponsePendRequested_b = FALSE;

#if(DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)
                Dcm_Prv_ConfirmationRespPendForBootloader(DCM_RES_NEG_NOT_OK);
#endif
                (void)Dcm_ConfirmationRespPend(DCM_RES_NEG_NOT_OK);
            }
        }
        else
        {
            Dcm_DslTransmit_st.isForceResponsePendRequested_b = FALSE;

#if(DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)
            Dcm_Prv_ConfirmationRespPendForBootloader(DCM_RES_NEG_OK);
#endif
            (void)Dcm_ConfirmationRespPend(DCM_RES_NEG_OK);
        }
    }
    else
    {
        Dcm_Prv_TransmitNormalResponse();
    }
}



/**********************************************************************************************************************
 *                                  PROCESSING DONE FUNCTIONALITY
 *********************************************************************************************************************/


#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_TransmitRdpiType2Response
 Syntax           : Dcm_Prv_TransmitRdpiType2Response(void)
 Description      : This Function is used transmit Rdpi type 2 response to the lower layer
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3487]*/
static void Dcm_Prv_TransmitRdpiType2Response(void)
{
    Std_ReturnType trasnsmitStatus = E_OK;
    PduIdType rdpiTxPduId = 0u;
    uint8 idxTxPduId_u8   = 0u;

    if(Dcm_Rdpi2ResponseType_en == DCM_DSLD_POS_RESPONSE)
    {
        if(DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8))
        {
            /*Obtain the index ofr TYpe2 TxPduId on which the response is to be triggered*/
            Dcm_GetRdpiType2Index(&idxTxPduId_u8);

            rdpiTxPduId = Dcm_Dsld_RDPIType2tx_table[idxTxPduId_u8].txpduid_num_u8;

            Dcm_DsldRdpi2pduinfo_ast[idxTxPduId_u8].SduDataPtr = &Dcm_Dsld_RDPIType2tx_table[idxTxPduId_u8].txbuffer_ptr[0];
            Dcm_DsldRdpi2pduinfo_ast[idxTxPduId_u8].SduLength = (PduLengthType) (Dcm_Rdpi2MesContext_st.resDataLen);

            /*Initialize Rdpi type TxPduId on which the response is to be triggered*/
            Dcm_Dsld_RDPIType2tx_table[idxTxPduId_u8].isTxPduId_Busy = TRUE;
            Dcm_Dsld_RDPIType2tx_table[idxTxPduId_u8].cntFreeTxPduRdpi2Cntr_u8 = DCM_CFG_RDPITYPE2FREETXPDUCOUNTER;

            /* Trigger the Tx in CanIf or FrIf via PduR */
            trasnsmitStatus = PduR_DcmTransmit(rdpiTxPduId,&Dcm_DsldRdpi2pduinfo_ast[idxTxPduId_u8]);
        }
        else
        {
            trasnsmitStatus = E_NOT_OK;
            DCM_DET_ERROR(DCM_PROCESSINGDONE_ID , DCM_E_FULLCOMM_DISABLED )
        }
    }

    /* Give the confirmation to application */
    DcmAppl_DcmConfirmationRDPI(DCM_RDPI_SID,Dcm_Rdpi2MesContext_st.dcmRxPduId,
                (trasnsmitStatus == E_OK) ? DCM_RES_POS_OK : DCM_RES_POS_NOT_OK) ;
}
#endif




#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_TransmitRoeType2Response
 Syntax           : Dcm_Prv_TransmitRoeType2Response(void)
 Description      : This Function is used to transmit RoeType2 Response to the lower layer
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_TransmitRoeType2Response(void)
{
    boolean TransmitSuccess_b = TRUE;

    Dcm_DsdRoe2State_en = DSD_WAITFORTXCONF;

    if(Dcm_Roe2ResponseType_en == DCM_DSLD_POS_RESPONSE)
    {
        /* Check if Response is supressed*/
        if(FALSE == Dcm_Roe2MesContext_st.msgAddInfo.suppressPosResponse)
        {
            Dcm_DsldRoe2_pcst->txbuffer_ptr[2] = Dcm_Roe2MesContext_st.idContext|DCM_SERVICEID_ADDEND;
            Dcm_DsldRoe2PduInfo_st.SduLength   = (PduLengthType) Dcm_Roe2MesContext_st.resDataLen + 1u ;
            Dcm_DsldRoe2PduInfo_st.SduDataPtr  = &Dcm_DsldRoe2_pcst->txbuffer_ptr[2];
        }
        else
        {
            Dcm_DsldRoe2PduInfo_st.SduLength = 0x00u;
        }
    }
    else
    {
        /* Check if Negative Response is supressed*/
        if((Dcm_Roe2MesContext_st.dcmRxPduId >= DCM_INDEX_FUNC_RX_PDUID) && \
                (DCM_CFG_SUPPRESS_NRC(Dcm_DsldRoe2_pcst->txbuffer_ptr[2])))
        {
            Dcm_DsldRoe2PduInfo_st.SduLength = 0x00;
        }
        else
        {
            Dcm_DsldRoe2PduInfo_st.SduLength = (PduLengthType) ( Dcm_Roe2MesContext_st.resDataLen + 3u ) ;
            Dcm_DsldRoe2PduInfo_st.SduDataPtr = &Dcm_DsldRoe2_pcst->txbuffer_ptr[0];
        }
    }



    /* Check if Response is Suppressed*/
    if(0x00u == Dcm_DsldRoe2PduInfo_st.SduLength)
    {
        /* No need to trigger Tx in PduR. Give confirmation */
        Dcm_Roe2TxResult_u8 = E_OK;
        Dcm_DsdRoe2State_en = DSD_SENDTXCONF_APPL;
    }
    else
    {
        if(DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_Roe2MesContext_st.dcmRxPduId]].channel_idx_u8))
        {
            if(E_NOT_OK == PduR_DcmTransmit(Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_Roe2MesContext_st.dcmRxPduId]].\
                    roetype2_txpdu_u8,&Dcm_DsldRoe2PduInfo_st))
            {
                TransmitSuccess_b  = FALSE;
                DCM_DET_ERROR(DCM_PROCESSINGDONE_ID , DCM_E_RET_E_NOT_OK ) //Need to check
            }
        }
        else
        {
            TransmitSuccess_b  = FALSE;
            DCM_DET_ERROR(DCM_PROCESSINGDONE_ID , DCM_E_FULLCOMM_DISABLED )
        }

        if(FALSE == TransmitSuccess_b)
        {
            /* Tx failed in PduR. Give confirmation */
            Dcm_Roe2TxResult_u8 = E_NOT_OK;
            Dcm_DsdRoe2State_en = DSD_SENDTXCONF_APPL;
        }
    }
}
#endif



/***********************************************************************************************************************
 Function name    : Dcm_Prv_TransmitCurrentResponse
 Syntax           : Dcm_Prv_TransmitCurrentResponse(&pMsgContext)
 Description      : This Function is used to Send Current Response to the Lower layer
 Parameter        : const Dcm_MsgContextType*
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_TransmitCurrentResponse(const Dcm_MsgContextType * pMsgContext)
{
    uint8 NrcValue_u8     = 0u;
    uint32 bufferSize_u32 = 0u;
    Dcm_DsdStatesType_ten DsdState_en = Dcm_Prv_GetDsdState();

    /*Check if response is given by DSD and for Consecutive call of the service */
    if((DsdState_en == DSD_CALL_SERVICE)||(FALSE != Dcm_DsldGlobal_st.dataResponseByDsd_b))
    {
        Dcm_Prv_SetDsdState(DSD_WAITFORTXCONF);
    }

    if(Dcm_DsldGlobal_st.stResponseType_en == DCM_DSLD_POS_RESPONSE)
    {
        if(FALSE == Dcm_DsldGlobal_st.dataResponseByDsd_b)
        {
            /* Application can add extra bytes to the service response */
            bufferSize_u32 = pMsgContext->resMaxDataLen - pMsgContext->resDataLen;

            DcmAppl_DcmModifyResponse(Dcm_DsldGlobal_st.dataSid_u8, NrcValue_u8,\
                    &(pMsgContext->resData[pMsgContext->resDataLen]), &bufferSize_u32);
        }

        if(FALSE != Dcm_Prv_isPositiveResponseSupressed(pMsgContext))
        {
            Dcm_DslTransmit_st.TxBuffer_tpu8 = NULL_PTR;
            Dcm_DslTransmit_st.TxResponseLength_u32 = 0x00u;
        }
        else
        {
            Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[2] = Dcm_DsldGlobal_st.dataSid_u8 | DCM_SERVICEID_ADDEND;
            Dcm_DslTransmit_st.TxBuffer_tpu8 = &Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[2] ;

            /* fill the response length including Sid */
            Dcm_DslTransmit_st.TxResponseLength_u32 = pMsgContext->resDataLen + 1u + bufferSize_u32;
        }
    }
    else
    {
        if(FALSE == Dcm_DsldGlobal_st.dataResponseByDsd_b)
        {
            /* Application can add extra bytes to the service response */
            bufferSize_u32 = pMsgContext->resMaxDataLen - pMsgContext->resDataLen;
            DcmAppl_DcmModifyResponse(Dcm_DsldGlobal_st.dataSid_u8, Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[2], \
                    &(Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[2]), &bufferSize_u32);
        }

        /*Need to check*/
        if(FALSE != Dcm_Prv_isNegativeResponseSupressed(pMsgContext,Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[2]))
        {
            Dcm_DslTransmit_st.TxBuffer_tpu8 = NULL_PTR ;
            Dcm_DslTransmit_st.TxResponseLength_u32 = 0x00u;
        }
        else
        {
            Dcm_DslTransmit_st.TxBuffer_tpu8 = Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8;
            Dcm_DslTransmit_st.TxResponseLength_u32 = DCM_NEGATIVE_RESPONSE_LENGTH;
        }
    }
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    if(FALSE == Dcm_Prv_isProtocolPreemptionInitiated())
#endif
    {
        if(Dcm_Prv_GetDsdState() == DSD_WAITFORTXCONF)
        {
            Dcm_Prv_TriggerTransmit();
        }
    }
}



/***********************************************************************************************************************
 Function name    : Dcm_ProcessingDone
 Syntax           : Dcm_ProcessingDone(&pMsgContext)
 Description      : This API is used to Process following Types of responses
                    1) Current response
                    2) Roe Type2 Response
                    3) RDPI Type2 Response
 Parameter        : const Dcm_MsgContextType*
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00114]*/
void Dcm_ProcessingDone(const Dcm_MsgContextType * pMsgContext)
{
    if(((pMsgContext->msgAddInfo.sourceofRequest == DCM_UDS_TESTER_SOURCE)         || \
                (Dcm_Prv_GetDslState() == DSL_STATE_ROETYPE1_RECEIVED))            && \
                (pMsgContext->dcmRxPduId == Dcm_DsldGlobal_st.dataActiveRxPduId_u8))
    {
        Dcm_Prv_TransmitCurrentResponse(pMsgContext);
    }
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
    else if(pMsgContext->msgAddInfo.sourceofRequest == DCM_ROE_SOURCE)
    {
        Dcm_Prv_TransmitRoeType2Response();
    }
#endif
    else
    {
#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
        if(pMsgContext->msgAddInfo.sourceofRequest == DCM_RDPI_SOURCE)
        {
            Dcm_Prv_TransmitRdpiType2Response();
        }
#endif
    }
}



/**********************************************************************************************************************
 *                                  CONFIRMATION FUNCTIONALITY
 *********************************************************************************************************************/


#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF) && (DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
/***********************************************************************************************************************
 Function name    : Dcm_KWPConfirmationForSplitResp
 Syntax           : Dcm_KWPConfirmationForSplitResp(status)
 Description      : This function is called for the handling of DSL,DSD states, Timer and the ComM channel when either
                    the entire response is sent or when the timeout between the responses occurs in the KWP split
                    response feature
 Parameter        : Dcm_ConfirmationStatusType
 Return value     : None
***********************************************************************************************************************/
void Dcm_KWPConfirmationForSplitResp(Dcm_ConfirmationStatusType status)
{
    /*TRACE[SWS_Dcm_00141]*/
    Dcm_Prv_ReloadS3Timer();
    Dcm_Prv_InactivateComMChannel();

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_ProcessRequestInQueue();
#else
    /* Lock required here to keep a consistency between DSL and DSD state */
    SchM_Enter_Dcm_Global_NoNest();
    Dcm_Prv_SetDslState((DSL_STATE_IDLE));
    Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
    SchM_Exit_Dcm_Global_NoNest();
#endif

    if(Dcm_isApplicationCalled_b == FALSE)
    {
        DcmAppl_DcmConfirmation(Dcm_DsldMsgContext_st.idContext,Dcm_DsldMsgContext_st.dcmRxPduId, \
            Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_DsldMsgContext_st.dcmRxPduId]].testaddr_u16, \
            status);
    }

#if ((DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS != 0) || (DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS !=0))
        Dcm_Dsd_CallRTEConfirmation(status,DCM_UDSCONTEXT);
#endif
    /* make DSD state as IDLE */
    Dcm_Prv_SetDsdState(DSD_IDLE);
}
#endif




#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF) && (DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessConfirmationForKWPResponse
 Syntax           : Dcm_Prv_ProcessConfirmationForKWPResponse(void)
 Description      : This Function is used to Process Confirmation received for KWP Response
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessConfirmationForKWPResponse(void)
{
    PduLengthType ResponseLength = 0u;

    DcmAppl_DcmConfirmation(Dcm_DsldMsgContext_st.idContext,Dcm_DsldMsgContext_st.dcmRxPduId,
            Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_DsldMsgContext_st.dcmRxPduId]].testaddr_u16,
            DCM_RES_POS_OK);

    /* call the application to know if some more response bytes are to be sent yet by splitting of responses. */
    DcmAppl_DcmGetRemainingResponseLength(Dcm_DsldMsgContext_st.idContext,&ResponseLength);

    if(0u != ResponseLength)
    {
        /* If the KWP service has some responses to be sent, then
        call the service again in the next main cycle if it is scheduled by Dcm */
        if(Dcm_isFirstReponseSent_b == FALSE)
        {
            /* set the below flag to TRUE when the Confirmation for the first split response is received */
            Dcm_isFirstReponseSent_b = TRUE;
        }

        /* start the split response timer - DCM_CFG_SPLITRESPONSETIMEFORKWP (configured) */
        DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32,(DCM_CFG_SPLITRESPONSETIMEFORKWP),
        Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)

        /* Multicore: No lock needed here as Dsl state is an atomic operation and
        at this point the DCM statemachine is already blocked so there is no question of accepting
        a parallel request thus no locks needed*/
        Dcm_Prv_SetDslState((DSL_STATE_REQUEST_RECEIVED));
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_P2MAX_TIMEMONITORING));
        Dcm_Prv_SetDsdState(DSD_CALL_SERVICE);
    }
    else
    {
        /* DcmAppl_DcmConfirmation is already called above.Hence do not call again. */
        Dcm_isApplicationCalled_b = TRUE;

        Dcm_KWPConfirmationForSplitResp(DCM_RES_POS_OK);

        /* in case of negative Tx confirmation or if all the split responses have been sent */
        Dcm_isFirstReponseSent_b = FALSE;
        Dcm_isApplicationCalled_b = FALSE;
    }
}
#endif




/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessConfirmationForCurrentResponse
 Syntax           : Dcm_Prv_ProcessConfirmationForCurrentResponse(Result)
 Description      : This Function is used to Process Confirmation received for current Response
                    1) Set New session
                    2) Update DSL and DSD states
                    3) Process Queued request
 Parameter        : Std_ReturnType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessConfirmationForCurrentResponse (Std_ReturnType Result)
{
   Dcm_DsldGlobal_st.dataResult_u8 = Result;

#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED != DCM_CFG_OFF) )
   Dcm_Prv_SetNewSession();
#endif

   /* Reset the length, needed esp. for paged buffer processing */
   Dcm_DsldPduInfo_st.SduLength = 0x00u;
   Dcm_Prv_DsdConfirmation();

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
   Dcm_Prv_ProcessRequestInQueue();
#else
    #if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
       if(FALSE == Dcm_Prv_isHighPriorityRequestReceiving())
       {
           Dcm_Prv_SetDslState((Dcm_Prv_GetDslNextState()));
           Dcm_Prv_SetDslSubState((Dcm_Prv_GetDslNextSubState()));
       }
    #else
       Dcm_Prv_SetDslState(Dcm_Prv_GetDslNextState());
       Dcm_Prv_SetDslSubState(Dcm_Prv_GetDslNextSubState());
    #endif
#endif
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessConfiramationForPendingResponse
 Syntax           : Dcm_Prv_ProcessConfiramationForPendingResponse(Result)
 Description      : This Function is used to Process confirmation received for pending response
 Parameter        : Std_ReturnType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessConfiramationForPendingResponse (Std_ReturnType Result)
{
   Dcm_ConfirmationStatusType confirmationStatus = DCM_RES_NEG_NOT_OK;

   if(E_OK == Result)
   {
       /* extend the time to Spl p2max, start SplP2 timer */
       DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32, \
            (Dcm_DsldTimer_st.dataTimeoutP2StrMax_u32 - \
            Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].dataP2StarTmrAdjust),\
            Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)

        confirmationStatus = DCM_RES_NEG_OK;
   }

   if(FALSE != Dcm_Prv_isForcePendingResponse())
   {
#if(DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)
       Dcm_Prv_ConfirmationRespPendForBootloader(confirmationStatus);
#endif
       /* indicate to application regarding the wait pend sent  */
       (void)Dcm_ConfirmationRespPend(confirmationStatus);

       Dcm_DslTransmit_st.isForceResponsePendRequested_b = FALSE;
   }

   Dcm_Prv_SetDslState((Dcm_Prv_GetDslNextState()));
   Dcm_Prv_SetDslSubState((Dcm_Prv_GetDslNextSubState()));
}

#if(DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDSendResponse
 Description      : Transmit the stored OBD Response
 Parameter        : const PduInfoType*
 Return value     : None
***********************************************************************************************************************/

void Dcm_Prv_OBDSendResponse(const PduInfoType* adrPduStrucutre_pcst)
{
    boolean dataResult_b = TRUE;

    /* ComM should be in FULL COM mode to sending the response */
    if(DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_OBDGlobal_st.nrActiveConn_u8].channel_idx_u8))
    {
        /* Trigger response in PduR */
        if(PduR_DcmTransmit(Dcm_OBDGlobal_st.dataActiveTxPduId_u8,adrPduStrucutre_pcst) == E_NOT_OK)
        {
            dataResult_b = FALSE; // Unable to Transmit
        }
    }
    else
    {
        dataResult_b = FALSE; /* COMM not in Full Communication Mode */
    }

    if(dataResult_b == FALSE)
    {
        // Transmission is unsuccessful for wait pend transmission
        if(Dcm_OBDGlobal_st.cntrWaitpendCounter_u8 > 0x0u)
        {
            /* Unable to send wait pend, again go to P2 max monitoring */
            Dcm_Prv_SetOBDState((Dcm_Prv_GetOBDPreviousState()));
        }
        else
        {
            /* Triggering of Tx fails */
            Dcm_OBDGlobal_st.dataResult_u8 = E_NOT_OK;
            Dcm_OBDisGeneralRejectSent_b = FALSE;
            Dcm_ObdSendTxConfirmation_b = TRUE;
            Dcm_Prv_SetOBDState((DCM_OBD_IDLE));
            if(FALSE != Dcm_Prv_CanComMBeInactivated(DCM_OBDCONTEXT))
            {
                ComM_DCM_InactiveDiagnostic(Dcm_active_commode_e[\
                 Dcm_DsldConnTable_pcst[Dcm_OBDGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId);
            }
        }
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDTriggerTransmit
 Description      : Function to prepare the response in case of OBD reponse for parallel OBD/UDS transmission
 Parameter        : void
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_OBDTriggerTransmit(void)
{
    /* Check if the call is for Suppression of Response */
    if(Dcm_OBDTransmit_st.TxResponseLength_u32 != 0x00u)
    {
        /* Multicore: Lock necessary here to block DSL/DSD state changes when parallel Rx or Dcm_MainFunction is running */
        SchM_Enter_Dcm_Global_NoNest();
            /* update data in pdu structure */
            Dcm_OBDPduInfo_st.SduDataPtr = Dcm_OBDTransmit_st.TxBuffer_tpu8;
            Dcm_OBDPduInfo_st.SduLength = (PduLengthType) Dcm_OBDTransmit_st.TxResponseLength_u32;
            /* final response comes now. reset the wait pend counter */
            Dcm_OBDGlobal_st.cntrWaitpendCounter_u8 = 0x0u;
            // Update OBD State to Wait for Tx Confirmation
        SchM_Exit_Dcm_Global_NoNest();
        //TODO: Check this variable is this because of lock, or is it unwanted?
        /* Trigger  response */
        Dcm_Prv_OBDSendResponse(&Dcm_OBDPduInfo_st);
    }
    else
    {
        /* simulate successful transmission of response */
        Dcm_OBDGlobal_st.dataResult_u8 = E_OK;
        /* indicate to DSD  */
        Dcm_ObdSendTxConfirmation_b = TRUE;
        Dcm_OBDMsgContext_st.msgAddInfo.suppressPosResponse = FALSE;
        Dcm_Prv_SetOBDState((DCM_OBD_IDLE));
        if(FALSE != Dcm_Prv_CanComMBeInactivated(DCM_OBDCONTEXT))
        {
            ComM_DCM_InactiveDiagnostic(Dcm_active_commode_e[\
            Dcm_DsldConnTable_pcst[Dcm_OBDGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId);
        }
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_SetOBDNegResponse
 Description      : Function to set Negative response for OBD
 Parameter        : const Dcm_MsgContextType*,Dcm_NegativeResponseCodeType
 Return value     : None
***********************************************************************************************************************/
void Dcm_Prv_SetOBDNegResponse(const Dcm_MsgContextType* pMsgContext,
                                         Dcm_NegativeResponseCodeType ErrorCode)
{
    if(pMsgContext->dcmRxPduId == Dcm_OBDGlobal_st.dataActiveRxPduId_u8)
    {
        /* check if this is the first call of Dcm_SetNegResponse() response */
        if(Dcm_OBDGlobal_st.stResponseType_en == DCM_DSLD_POS_RESPONSE)
        {
            Dcm_OBDGlobal_st.stResponseType_en = DCM_DSLD_NEG_RESPONSE;
            Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8[0]= DCM_NEGRESPONSE_INDICATOR;
            Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8[1]= Dcm_OBDGlobal_st.dataSid_u8;
            Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8[2]= ErrorCode;
        }
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_OBDProcessingDone
 Description      : Function to initialize the transmission structure before response transmission for OBD
 Parameter        : const Dcm_MsgContextType*
 Return value     : None
***********************************************************************************************************************/
void Dcm_OBDProcessingDone(const Dcm_MsgContextType* pMsgContext)
{
    uint8 dataNrc_u8 = 0x00;
    uint32 AvailBufSize_u32 = 0x00;

    /* Ensure transmission is for an accepted request of a client (DcmRxPduId)
     * Should not be processed in other cases (eg: BusyRepeatRequest Tx) for a different client */
    if(pMsgContext->dcmRxPduId == Dcm_OBDGlobal_st.dataActiveRxPduId_u8)
    {
        Dcm_Prv_SetOBDState((DCM_OBD_WAITFORTXCONF));
        // Remaining buffer size in Dcm after entire response is filled
        AvailBufSize_u32 = pMsgContext->resMaxDataLen - pMsgContext->resDataLen;
        if(Dcm_OBDGlobal_st.stResponseType_en == DCM_DSLD_POS_RESPONSE)
        {
            /* Response is not triggered by DSD */
            if(Dcm_OBDGlobal_st.dataResponseByDsd_b == FALSE )
            {
                // Application can add extra bytes at the end of response
                DcmAppl_DcmModifyResponse(Dcm_OBDGlobal_st.dataSid_u8,dataNrc_u8,
                        &(pMsgContext->resData[pMsgContext->resDataLen]),&AvailBufSize_u32);
            }
            if(FALSE != (pMsgContext->msgAddInfo).suppressPosResponse)
            {
                Dcm_OBDTransmit_st.TxBuffer_tpu8 = NULL_PTR;
                Dcm_OBDTransmit_st.TxResponseLength_u32 = 0x00u;
            }
            else
            {
                /* Frame the positive response. For positive response 0x40 is added to SID */
                Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8[2] = Dcm_OBDGlobal_st.dataSid_u8 | 0x40u;
                Dcm_OBDTransmit_st.TxBuffer_tpu8 = & Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8[2];
                /* Fill the total response length including Sid */
                Dcm_OBDTransmit_st.TxResponseLength_u32 = pMsgContext->resDataLen + 1u + AvailBufSize_u32;
            }
        }
        else
        {
            /* Negative response not triggered by DSD */
            if(Dcm_OBDGlobal_st.dataResponseByDsd_b == FALSE )
            {
                // Application can modify the NRC value
                DcmAppl_DcmModifyResponse(Dcm_OBDGlobal_st.dataSid_u8, Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8[2],
                                          &(Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8[2]),&AvailBufSize_u32);
            }
            if(pMsgContext->dcmRxPduId >= DCM_INDEX_FUNC_RX_PDUID)
            {
                dataNrc_u8 = Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8[2];
                /*Check for the negative response code and whether Wait pend counter is set to 0*/
                if((DCM_CFG_SUPPRESS_NRC(dataNrc_u8))
                        &&(Dcm_OBDGlobal_st.cntrWaitpendCounter_u8 == 0x00u))
                {
                    /* Reset the P2 timer and DSL state machine and get the confirmation
                     suppress the Negative response */
                    Dcm_OBDTransmit_st.TxBuffer_tpu8 = NULL_PTR;
                    Dcm_OBDTransmit_st.TxResponseLength_u32 = 0x00u;
                }
                else
                {
                    /* give the Tx pointer from the starting of the tx buffer */
                    Dcm_OBDTransmit_st.TxBuffer_tpu8 = Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8;
                    Dcm_OBDTransmit_st.TxResponseLength_u32 = 0x03u;
                }
            }
            else
            {
                /* This is Physical request, suppression is not allowed.Send Negative response */
                Dcm_OBDTransmit_st.TxBuffer_tpu8 = Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8;
                /* response data length including 0x7f, Sid, NRC */
                Dcm_OBDTransmit_st.TxResponseLength_u32 = 0x03u;
            }
        }
        Dcm_Prv_OBDTriggerTransmit();
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBD_ProcessConfirmationForPendingResponse
 Description      : Confirmation is received for Pending Transmission (NRC0x78)
 Parameter        : Std_ReturnType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_OBD_ProcessConfirmationForPendingResponse(Std_ReturnType Result)
{
    if(E_OK == Result)
    {
        /* extend the time to Spl p2max, start SplP2 timer */
        DCM_TimerStart(Dcm_OBDGlobal_st.dataTimeoutMonitor_u32, \
             (DCM_CFG_DEFAULT_P2STARMAX_TIME - \
             Dcm_DsldProtocol_pcst[Dcm_OBDGlobal_st.idxCurrentProtocol_u8].dataP2StarTmrAdjust),\
             Dcm_OBDP2OrS3StartTick_u32,Dcm_OBDP2OrS3TimerStatus_uchr)
    }
    // Let OBD State Machine continue processing in its previous state before P2 timeout occured
    Dcm_Prv_SetOBDState((Dcm_Prv_GetOBDPreviousState()));
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDProcessConfirmationForCurrentResponse
 Description      : Confirmation is received for Normal Response transmission
                    No need to reset S3 timer as OBD is always processed in only default session
 Parameter        : Std_ReturnType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_OBDProcessConfirmationForCurrentResponse(Std_ReturnType Result)
{
   // Store the Result of Transmission. This is later on used to inform application
    Dcm_OBDGlobal_st.dataResult_u8 = Result;

   // Set OBD State to Send Confirmation to Application
   Dcm_ObdSendTxConfirmation_b = TRUE;
   Dcm_Prv_SetOBDState((DCM_OBD_IDLE));
   /* If UDS State Machine is also free only then release */
   if(FALSE != Dcm_Prv_CanComMBeInactivated(DCM_OBDCONTEXT))
   {
       ComM_DCM_InactiveDiagnostic(Dcm_active_commode_e[\
             Dcm_DsldConnTable_pcst[Dcm_OBDGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId);
   }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDProcessTxConfirmation
 Description      :  Tx Confirmation has come for an OBD protocol triggered Response
 Parameter        : PduIdType, Std_ReturnType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_OBDProcessTxConfirmation(PduIdType DcmTxPduId,Std_ReturnType Result)
{
    uint8 RxPduidCounter_u8;
    /* Is confirmation received on active connection?
       Can be False in case of Busy Repeat request confirmation for different client */
    if(FALSE != Dcm_Prv_isOBDConfirmationOnActiveConnection(DcmTxPduId))
    {
        // Confirmation of NRC 0x78 due to P2/P2* timeout?
       if(Dcm_OBDGlobal_st.cntrWaitpendCounter_u8 > 0x0u)
       {
           Dcm_Prv_OBD_ProcessConfirmationForPendingResponse(Result);
       }
       else
       {
           Dcm_Prv_OBDProcessConfirmationForCurrentResponse(Result);
       }
    }

    /* If confirmation obtained is due to NRC21 transmission when Dcm is busy processing another request
       Loop through all RxPduIds and match with TxPduId for which confirmation is received */
    for (RxPduidCounter_u8 = 0x00u; RxPduidCounter_u8 < DCM_CFG_NUM_RX_PDUID; RxPduidCounter_u8++)
    {
        if(FALSE != Dcm_Prv_isConfirmationReceivedForNrc21Response(DcmTxPduId,RxPduidCounter_u8))
        {
            // Reset ServiceId as confirmation is received. Should be set only in case of NRC21 transmission
            Dcm_DslOBDRxPduArray_ast[RxPduidCounter_u8].Dcm_DslServiceId_u8 = DCM_SERVICEID_DEFAULT_VALUE;
            break;
        }
    }
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessTxConfirmation
 Syntax           : Dcm_Prv_ProcessTxConfirmation(TxPduId,result)
 Description      : This function is used to process the Confirmation received based on the Type of confirmation e.g.
                    1) Confirmation for Response on active connection
                    2) Confirmation for Roe type 2 Response
                    3) Confirmation for NRC21 Response
 Parameter        : PduIdType, Std_ReturnType
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00118]*/
static void Dcm_Prv_ProcessTxConfirmation(PduIdType TxPduId,
        Std_ReturnType result)
{
    uint8 idxRxPduId_u8;

    if(FALSE != Dcm_Prv_isConfirmationOnActiveConnection(TxPduId))
    {
       if(FALSE != Dcm_Prv_isConfirmationForPendingResponse())
       {
           Dcm_Prv_ProcessConfiramationForPendingResponse(result);
       }
       else
       {
#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF) && (DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
           if(FALSE != Dcm_Prv_isConfirmationForKWPResponse(result))
           {
               Dcm_Prv_ProcessConfirmationForKWPResponse();
           }
           else
#endif
           {
               Dcm_Prv_ProcessConfirmationForCurrentResponse(result);
           }
       }
    }
#if((DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF))
    else
    {
        if(FALSE != Dcm_Prv_isConfirmationForRoeType2Response(TxPduId))
        {
            Dcm_Roe2TxResult_u8 = result;
            Dcm_DsdRoe2State_en = DSD_SENDTXCONF_APPL;
        }
    }
#endif

    for (idxRxPduId_u8 = 0; idxRxPduId_u8 < DCM_CFG_NUM_RX_PDUID ; idxRxPduId_u8++)
    {
       if(FALSE != Dcm_Prv_isConfirmationReceivedForNrc21Response(TxPduId,idxRxPduId_u8))
       {
           Dcm_DslRxPduArray_ast[idxRxPduId_u8].Dcm_DslServiceId_u8 = DCM_SERVICEID_DEFAULT_VALUE;
           break;
       }
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_TpTxConfirmation
 Syntax           : Dcm_TpTxConfirmation(DcmTxPduId,Result)
 Description      : This function is called by the lower layer (in general the PDU Router):
                    Result = E_OK after the complete DCM I-PDU has successfully been transmitted,i.e. at the very end
                    of the segmented TP transmit cycle.Within this function, the DCM shall unlock the transmit buffer.
                    Result = E_NOT_OK if an error (e.g. timeout) has occurred during the transmission of the DCM  I-PDU.
                    This enables unlocking of the transmit buffer and error handling.
 Parameter        : PduIdType, Std_ReturnType
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00351]*/
void Dcm_TpTxConfirmation (PduIdType id,Std_ReturnType result)
{
    if(id >= DCM_NUM_TX_PDUID)
    {
         DCM_DET_ERROR(DCM_TPTXCONFIRMATION_ID ,DCM_E_DCMTXPDUID_RANGE_EXCEED)
    }
    else
    {
#if(DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
        /* Received TxPduId belongs to OBD? */
        if(Dcm_Prv_IsTxPduIdOBD(id))
        {
            Dcm_Prv_OBDProcessTxConfirmation(id,result);
        }
        else
#endif
        {

            if(FALSE != Dcm_isCancelTransmitInvoked_b)
            {
                Dcm_isCancelTransmitInvoked_b = FALSE;
            }
            else
            {
            Dcm_Prv_ProcessTxConfirmation(id,result);
            }
        }
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_TxConfirmation
 Syntax           : Dcm_TxConfirmation(DcmTxPduId)
 Description      : The lower layer communication interface module confirms the transmission of a PDU,
                    or the failure to transmit a PDU for periodic transmission e.g RDPI
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_01092][BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3486]*/
void Dcm_TxConfirmation (PduIdType DcmTxPduId)
{
    uint8 idxTxPduid_u8 = 0u;

    if ( DcmTxPduId >= DCM_NUM_TX_PDUID )
    {
        DCM_DET_ERROR(DCM_TXCONFIRMATION_ID ,DCM_E_DCMRXPDUID_RANGE_EXCEED)
    }
#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
    else
    {
        for(idxTxPduid_u8 = 0; idxTxPduid_u8 < DCM_CFG_NUM_RDPITYPE2_TXPDU; idxTxPduid_u8++)
        {
            if(Dcm_Dsld_RDPIType2tx_table[idxTxPduid_u8].txpduid_num_u8 == Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId])
            {
                Dcm_Dsld_RDPIType2tx_table[idxTxPduid_u8].isTxPduId_Busy = FALSE;
                break;
            }
        }
    }
#else
    (void)idxTxPduid_u8;
#endif
}


#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
