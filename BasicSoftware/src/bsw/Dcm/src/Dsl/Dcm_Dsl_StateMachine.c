

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "DcmCore_Main_Inf.h"
#include "Rte_Dcm.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
#define DCM_MAX_LIMIT_WAITPEND        (0xFFu)




/***********************************************************************************************************************
 *    Internal Type definitions
 **********************************************************************************************************************/
#define Dcm_Prv_ProcessDslStateMachine(stDslState_en)  Dcm_ProcessDslState[stDslState_en]()
typedef void (*Dcm_DslStateType)(void);

static void Dcm_Prv_DslState_Idle(void);
static void Dcm_Prv_DslState_WaitingForRxIndication(void);
static void Dcm_Prv_DslState_RequestReceived(void);
static void Dcm_Prv_DslState_ResponseTransmission(void);
static void Dcm_Prv_DslState_WaitingForTxConfirmation(void);
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_DslState_ProtocolPreemption(void);
#endif
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_DslState_PagedBufferTransmission(void);
#endif


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define DCM_START_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
static const Dcm_DslStateType  Dcm_ProcessDslState[DCM_NUMBER_OF_DSL_STATES] =
{
    Dcm_Prv_DslState_Idle,
    Dcm_Prv_DslState_WaitingForRxIndication,
    Dcm_Prv_DslState_RequestReceived,
    Dcm_Prv_DslState_ResponseTransmission,
    Dcm_Prv_DslState_WaitingForTxConfirmation,
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_DslState_ProtocolPreemption,
#endif
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_DslState_PagedBufferTransmission,
#endif
    Dcm_Prv_DslState_Idle //For Roe Type1
};


#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"


#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
uint8 Dcm_DslWaitPendBuffer_au8[8];
uint8 Dcm_DslState_u8;
uint8 Dcm_DslSubState_u8;
uint8 Dcm_DslNextState_u8;
uint8 Dcm_DslNextSubState_u8;
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
uint8 Dcm_DslPreemptionState_u8;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"


#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
const Dcm_Dsld_ServiceType * Dcm_DsldSrvTable_pcst;
Dcm_DsldTimingsType_tst Dcm_DsldTimer_st;
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
Dcm_QueueStructure_tst Dcm_QueueStructure_st;
#endif
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
static Std_ReturnType stCancelTx_u8;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"


#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_Dsld_KwpTimerServerType Dcm_DsldKwpReqTiming_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#endif


#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
boolean Dcm_isGeneralRejectSent_b;
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
static boolean flgRoeOn_b;
    #if(DCM_ROE_ENABLED != DCM_CFG_OFF)
       static boolean flgPersistRoe_b;
    #endif
#endif
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"


#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
StatusType Dcm_P2OrS3TimerStatus_uchr;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
StatusType Dcm_OBDP2OrS3TimerStatus_uchr;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"




/***********************************************************************************************************************
 *    Function Definitions
 **********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"


#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProvideFreeBuffer
 Syntax           : Dcm_Prv_ProvideFreeBuffer(idxProtocolIndex_u8,isQueuedReq_b)
 Description      : The function to provide the buffer when there is a new request.
                    This function provides the buffer as the index if it is a normal request.
                    If it is a queuing request, it will switch the index and provides the other buffer which is free,
                    So that the queuing of the request can happen in this buffer
 Parameter        : uint8,boolean
 Return value     : Dcm_MsgItemType
***********************************************************************************************************************/
Dcm_MsgItemType* Dcm_Prv_ProvideFreeBuffer(
        uint8 idxProtocolIndex_u8,boolean isQueuedReq_b)
{
    uint8 * RxBuffer_pu8 = NULL_PTR;

    /* It is for the queued request switch the buffer */
    if(isQueuedReq_b == TRUE)
    {
        if(Dcm_QueueStructure_st.idxBufferIndex_u8 == 1)
        {
            Dcm_QueueStructure_st.idxBufferIndex_u8 = 2u;
        }
        else
        {
            Dcm_QueueStructure_st.idxBufferIndex_u8 = 1u;
        }
    }

    /* return the buffer based on the index */
    if(Dcm_QueueStructure_st.idxBufferIndex_u8 == 1u)
    {
        RxBuffer_pu8 = Dcm_DsldProtocol_pcst[idxProtocolIndex_u8].rx_mainBuffer_pa;
    }
    else
    {
        RxBuffer_pu8 = Dcm_DsldProtocol_pcst[idxProtocolIndex_u8].rx_reserveBuffer_pa;
    }

    return (RxBuffer_pu8);
}
#endif /* #if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF) */






/***********************************************************************************************************************
 Function name    : Dcm_GetActiveBuffer
 Syntax           : Dcm_GetActiveBuffer(void)
 Description      : This function is used to return the buffer pointer which has the request to be processed
 Parameter        : None
 Return value     : Dcm_MsgItemType
***********************************************************************************************************************/
Dcm_MsgItemType* Dcm_GetActiveBuffer(void)
{
    /* return the buffer pointer which has the request to be processed */
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
    return Dcm_DsldGlobal_st.adrBufferPtr_pu8;
#else
    return Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].rx_mainBuffer_pa;
#endif /* #if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF) */
}






/***********************************************************************************************************************
 Function name    : Dcm_Prv_SetIntermediateResponse
 Syntax           : Dcm_Prv_SetIntermediateResponse(dataNegativeResponse_u8)
 Description      : This function is used to frame Negative response in separate buffer
 Parameter        : uint8
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00119]*/
static void Dcm_Prv_SetIntermediateResponse(uint8 dataNegativeResponse_u8)
{
    /* change the state to wait for tx confirmation */
    /* Multicore: No lock needed here as Dsl state is an atomic operation */
    /* Also if the state is changed here the changes to the Dsl state cannot happen from any other place */
    Dcm_Prv_SetDslState((DSL_STATE_WAITING_FOR_TXCONFIRMATION));
    /* frame intermediate Negative response based on the NRC*/
    Dcm_DslWaitPendBuffer_au8[0] = DCM_NEGRESPONSE_INDICATOR;
    Dcm_DslWaitPendBuffer_au8[1] = Dcm_DsldGlobal_st.dataSid_u8;
    Dcm_DslWaitPendBuffer_au8[2] = dataNegativeResponse_u8 ;

    /* update data in PDU structure */
    Dcm_DsldPduInfo_st.SduDataPtr = Dcm_DslWaitPendBuffer_au8;
    Dcm_DsldPduInfo_st.SduLength = DCM_NEGATIVE_RESPONSE_LENGTH;
}





#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_Check_ProtocolPreemptionStatus
 Syntax           : Dcm_Prv_Check_ProtocolPreemptionStatus(void)
 Description      : This function is used to check if new high protocol is received and update the DSL states.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_Check_ProtocolPreemptionStatus(void)
{
    SchM_Enter_Dcm_Global_NoNest();
    if(Dcm_Prv_GetDslPreemptionState() == DSL_SUBSTATE_STOP_PROTOCOL)
    {
        SchM_Exit_Dcm_Global_NoNest();

        /*cancel ongoing transmission in Lower layers */
        stCancelTx_u8 = Dcm_Prv_CancelTransmit();

        SchM_Enter_Dcm_Global_NoNest();
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
        if(Dcm_Prv_GetDslState() == DSL_STATE_ROETYPE1_RECEIVED)
        {
            flgRoeOn_b = TRUE;
        }
#else
        (void)flgRoeOn_b;
#endif
        /* High priority protocol arrived, start new protocol */
        Dcm_Prv_SetDslState((DSL_STATE_PROTOCOL_PREEMPTION));
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_STOP_PROTOCOL));
    }
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    else
    {
        if(Dcm_Prv_GetDslPreemptionState() == DSL_SUBSTATE_STOP_ROE)
        {
            if(Dcm_Prv_GetDslState() == DSL_STATE_ROETYPE1_RECEIVED)
            {
                flgPersistRoe_b = TRUE;
                SchM_Exit_Dcm_Global_NoNest();

                stCancelTx_u8 = Dcm_Prv_CancelTransmit();

                SchM_Enter_Dcm_Global_NoNest();
            }

            /* If high priority protocol request is arrived, stop ROE */
            Dcm_Prv_SetDslState((DSL_STATE_PROTOCOL_PREEMPTION));
            Dcm_Prv_SetDslSubState((DSL_SUBSTATE_STOP_ROE));
        }
    }
#endif
    SchM_Exit_Dcm_Global_NoNest();
}
#endif






#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_SetDefaultTimerValues
 Syntax           : Dcm_Prv_SetDefaultTimerValues(void)
 Description      : This function is used to set timer values after protocol is started.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_SetDefaultTimerValues(void)
{
    uint8 idxKwpTiming_u8 = 0u;
    /* Lock is needed here to have P2Max, P3Max and P2StrMax to be consistent as a unit */
    SchM_Enter_Dcm_DsldTimer_NoNest();
    if(DCM_IS_KWPPROT_ACTIVE() != FALSE)
    {
        /* KWP is active. Use KWP session table  */
        Dcm_DsldSessionTable_pcu8 = Dcm_Dsld_KWPsupported_sessions_acu8;
        idxKwpTiming_u8 = Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].timings_idx_u8;
        Dcm_DsldTimer_st.dataTimeoutP2max_u32 = Dcm_Dsld_default_timings_acs[idxKwpTiming_u8].P2_max_u32;
        Dcm_DsldTimer_st.dataTimeoutP3max_u32 = Dcm_Dsld_default_timings_acs[idxKwpTiming_u8].P3_max_u32;
        Dcm_DsldTimer_st.dataTimeoutP2StrMax_u32 = Dcm_DsldTimer_st.dataTimeoutP3max_u32;
    }
    else
    {
        /* UDS is active, use UDS session table  */
        Dcm_DsldSessionTable_pcu8 = Dcm_Dsld_Conf_cs.session_lookup_table_pcau8;
        Dcm_DsldTimer_st.dataTimeoutP2max_u32    =  DCM_CFG_DEFAULT_P2MAX_TIME;
        Dcm_DsldTimer_st.dataTimeoutP2StrMax_u32 =  DCM_CFG_DEFAULT_P2STARMAX_TIME;
    }
    SchM_Exit_Dcm_DsldTimer_NoNest();
}
#endif





/***********************************************************************************************************************
 Function name    : Dcm_Prv_StartProtocol
 Syntax           : Dcm_Prv_StartProtocol(void)
 Description      : This function is used to start new protocol.
 Parameter        : None
 Return value     : Std_ReturnType
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00036][SWS_Dcm_00144][SWS_Dcm_00145][SWS_Dcm_00146][SWS_Dcm_00147]*/
static Std_ReturnType Dcm_Prv_StartProtocol(void)
{
    Std_ReturnType startProtocolStatus = E_NOT_OK;

    /* Invoke StopProtocol only when there is already a protocol currently running and StopProtol is not already invoked */
    if((Dcm_CurProtocol_u8 != DCM_NO_ACTIVE_PROTOCOL) && (!Dcm_isStopProtocolInvoked_b))
    {
        Dcm_StopProtocol(Dcm_CurProtocol_u8);
    }

    /* Activate the protocol because this is first request */
    startProtocolStatus = Dcm_StartProtocol(Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8);

    if(E_OK == startProtocolStatus)
    {
        Dcm_DsldGlobal_st.flgCommActive_b = TRUE;
        Dcm_DsldGlobal_st.datActiveSrvtable_u8 = Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].
                sid_tableid_u8;
        Dcm_DsldSrvTable_pcst = Dcm_Dsld_Conf_cs.sid_table_pcs[Dcm_DsldGlobal_st.datActiveSrvtable_u8].ptr_service_table_pcs;
        Dcm_Dsd_ServiceIni(Dcm_DsldGlobal_st.datActiveSrvtable_u8);

#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
        Dcm_Prv_SetDefaultTimerValues();
#endif

#if(DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED!=DCM_CFG_OFF)

#if (DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF)
        (void)SchM_Switch_Dcm_DcmDiagnosticSessionControl(RTE_MODE_DcmDiagnosticSessionControl_DEFAULT_SESSION);
#endif
        (void)DcmAppl_Switch_DcmDiagnosticSessionControl(Dcm_DsldSessionTable_pcu8[DCM_DEFAULT_SESSION_IDX]);
#endif

        /* Set the default session, default security level and default timing */
        Dcm_Prv_SetSesCtrlType(Dcm_DsldSessionTable_pcu8[DCM_DEFAULT_SESSION_IDX]);

        /* Update the variable with the id of the new Requested Protocol */
        Dcm_CurProtocol_u8 = Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8;
    }

    return startProtocolStatus;
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_SendNrcforStartProtocolFailure
 Syntax           : Dcm_Prv_SendNrcforStartProtocolFailure(void)
 Description      : This function is used to send Negative reposne to when protocol could not be started.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00674]*/
static void Dcm_Prv_SendNrcforStartProtocolFailure(void)
{
    Dcm_NegativeResponseCodeType dataNrc_u8 = 0x22;          /* Variable to store error code */
    const Dcm_MsgItemType * adrRxBuffer_pu8 = NULL_PTR;

    Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8 = (&(Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8]))->tx_buffer_pa;
    Dcm_DsldGlobal_st.dataResponseByDsd_b = TRUE;

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
    adrRxBuffer_pu8 = Dcm_GetActiveBuffer();
#else
    adrRxBuffer_pu8 = (&(Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8]))->rx_mainBuffer_pa;
#endif

    Dcm_DsldGlobal_st.dataSid_u8     = adrRxBuffer_pu8[0];
    Dcm_DsldMsgContext_st.idContext  = Dcm_DsldGlobal_st.dataSid_u8;

    /* Multicore: No locking necessary as Dcm_DsldGlobal_st.dataActiveRxPduId_u8 is an atomic variable and
       there is no parallel writing due to DSL state machine handling */
    Dcm_DsldMsgContext_st.dcmRxPduId = Dcm_DsldGlobal_st.dataActiveRxPduId_u8;
    Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest = DCM_UDS_TESTER_SOURCE;
    Dcm_DsldMsgContext_st.resDataLen = 0u;

    /* Get the request type */
    /* Multicore: No locking necessary as Dcm_DsldGlobal_st.dataActiveRxPduId_u8 is an atomic variable and
                       there is no parallel writing due to DSL state machine handling */
    if(Dcm_DsldGlobal_st.dataActiveRxPduId_u8 >= DCM_INDEX_FUNC_RX_PDUID)
    {
        Dcm_DsldMsgContext_st.msgAddInfo.reqType = DCM_PRV_FUNCTIONAL_REQUEST;
    }
    else
    {
        Dcm_DsldMsgContext_st.msgAddInfo.reqType = DCM_PRV_PHYSICAL_REQUEST;
    }

    /* By default make the response type as Positive response */
    Dcm_DsldGlobal_st.stResponseType_en      = DCM_DSLD_POS_RESPONSE;
    /* Change the DSL state */
    /* Multicore: No lock needed here as Dsl state is an atomic operation */
    /* Also if the state is changed here the changes to the Dsl state cannot happen from any other place due to state machine handling*/
    Dcm_Prv_SetDslState((DSL_STATE_REQUEST_RECEIVED));
    Dcm_Prv_SetDslSubState((DSL_SUBSTATE_P2MAX_TIMEMONITORING));
    Dcm_DsldGlobal_st.dataActiveTxPduId_u8   = Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].txpduid_num_u8;
    Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 = 0;
    Dcm_DslTransmit_st.isForceResponsePendRequested_b    = FALSE;

    Dcm_SetNegResponse(&Dcm_DsldMsgContext_st ,dataNrc_u8);
    Dcm_ProcessingDone(&Dcm_DsldMsgContext_st);
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_UpdateStatesForStartProtocol
 Syntax           : Dcm_Prv_UpdateStatesForStartProtocol(void)
 Description      : This function is used to update DSL and DSD state once the protocol is started.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_UpdateStatesForStartProtocol(void)
{
    /*TRACE[SWS_Dcm_01142]*/
    /* Communication is active now for UDS request(subsequent requests)*/
    if((DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8)) && \
            (FALSE == (DCM_TimerElapsed(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32))))
    {
        /*TRACE[SWS_Dcm_00111]*/
        Dcm_isGeneralRejectSent_b = FALSE;

        /* Go to next state  */
        /* Multicore: No lock needed here as Dsl state is an atomic operation */
        /* Also if the state is changed here the changes to the Dsl state cannot happen from any other place due to state machine handling*/
        Dcm_Prv_SetDslState((DSL_STATE_REQUEST_RECEIVED));
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_P2MAX_TIMEMONITORING));

        /* The following statement is moved from the case DSL_SEND_WAITPEND */
        Dcm_Prv_SetDslNextState((DSL_STATE_REQUEST_RECEIVED));
        Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_P2MAX_TIMEMONITORING));

        /* Indicate to DSD,change the DSD state machine to DSD_VERIFY_DIAGNOSTIC_DATA state */
        Dcm_Prv_SetDsdState(DSD_VERIFY_DIAGNOSTIC_DATA);
        Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 = 0x0u;

        /* Get the Tx PDU id */
        Dcm_DsldGlobal_st.dataActiveTxPduId_u8   = Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].txpduid_num_u8;

        /* Update the sid from the request buffer as the sid value if required to send BUSYREPEAT response from
        DSL_SEND_WAITPEND state even before processing of the request happens in DSD state machine */
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
        Dcm_DsldGlobal_st.dataSid_u8 = *(Dcm_GetActiveBuffer());
#else
        Dcm_DsldGlobal_st.dataSid_u8 = Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].rx_mainBuffer_pa[0];
#endif
    }
    /* Check P2 max expired or not and non default session active or not*/
    else if((Dcm_DsldGlobal_st.idxActiveSession_u8 != DCM_DEFAULT_SESSION_IDX) \
            || (FALSE != DCM_TimerElapsed(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32)))
    {
        /*DCM is not in FULL Communication mode ,reject request*/
        DcmAppl_DcmComModeError(Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId);

        /* Set the DSL and DSD state to default state */
        Dcm_Prv_SetDslState((DSL_STATE_IDLE));
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
        Dcm_Prv_SetDslNextState((DSL_STATE_IDLE));
        Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
        Dcm_Prv_SetDsdState(DSD_IDLE);
    }
    /* P2 max not yet over. Stay in this state */
    else
    {
        DCM_TimerProcess(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32,Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
    }
}



#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_StopLowPriorityProtocol
 Syntax           : Dcm_Prv_StopLowPriorityProtocol(void)
 Description      : This function is used to stop low priority protocol.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_01046]*/
static void Dcm_Prv_StopLowPriorityProtocol(void)
{
    uint8 NrcValue_u8 = 0u;

#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF))
    if(FALSE != Dcm_Prv_isProtocolStarted())
    {
        Dcm_ControlDtcSettingExit();
    }
#endif

    /* Cancel the ongoing service of low prio protocol */
    /* MR12 RULE 13.5 VIOLATION: The function "Dcm_Prv_GetDsdState" does not have any persistent side effects */
    if((Dcm_Prv_GetDsdState() != DSD_IDLE) && \
            (Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8].service_handler_fp != NULL_PTR))
    {
        (void)(*Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8].service_handler_fp)\
                (DCM_CANCEL,&Dcm_DsldMsgContext_st,&NrcValue_u8);

        Dcm_SrvOpstatus_u8 = DCM_INITIAL;
        Dcm_ExtSrvOpStatus_u8 = DCM_INITIAL;
    }

    /* If DSD calling the service and Paged buffer Tx is on then inform to application */
    if(Dcm_Prv_GetDsdState() == DSD_CALL_SERVICE)
    {
        Dcm_isStopProtocolInvoked_b = TRUE;
        Dcm_StopProtocol(Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8);
    }
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    else
    {
        if(Dcm_DsldGlobal_st.flgPagedBufferTxOn_b != FALSE)
        {
            Dcm_isStopProtocolInvoked_b = TRUE;

            Dcm_StopProtocol(Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8);

            Dcm_DsldGlobal_st.flgPagedBufferTxOn_b = FALSE;
            Dcm_adrUpdatePage_pfct = NULL_PTR;
        }
    }
#endif

#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    /* This variable will be TRUE when either ROE type1 is under processing */
    if(FALSE != flgRoeOn_b)
    {
        if(Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest == DCM_ROE_SOURCE)
        {
            /* Call  ROE confirmation because DCM killing ROE requested service */
            DcmAppl_DcmConfirmation(Dcm_DsldGlobal_st.dataSid_u8,
                    Dcm_DsldMsgContext_st.dcmRxPduId,
                    Dcm_Dsld_RoeRxToTestSrcMappingTable[Dcm_DsldMsgContext_st.dcmRxPduId].testsrcaddr_u16,
                    DCM_RES_POS_NOT_OK);
        }
    }
#endif

}
#endif


/***********************************************************************************************************************
 *                    START OF DSL SUB-STATES
 **********************************************************************************************************************/


#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslSubState_StopProtocol
 Syntax           : Dcm_Prv_DslSubState_StopProtocol(void)
 Description      : This function is used to process sub state DSL_SUBSTATE_STOP_PROTOCOL
                    In this sub state ongoing protocol is stopped and DSL states are changed to start new high
                    priority protocol
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/

static void Dcm_Prv_DslSubState_StopProtocol(void)
{
    /* Multicore: No locking necessary as Dcm_DsldGlobal_st.dataActiveRxPduId_u8 is an atomic variable and
       there is no parallel writing due to DSL state machine handling */

    uint8 idxConnection_u8 = Dcm_DsldRxTable_pcu8[Dcm_DsldGlobal_st.dataActiveRxPduId_u8];
    uint8 idxProtocol_u8   = Dcm_DsldConnTable_pcst[idxConnection_u8].protocol_num_u8;

    Dcm_Prv_StopLowPriorityProtocol();


    /* Update active connection and protocol id */
    Dcm_DsldGlobal_st.idxCurrentProtocol_u8 = idxProtocol_u8;
    Dcm_DsldGlobal_st.nrActiveConn_u8       = idxConnection_u8;

    if(Dcm_DsldGlobal_st.idxActiveSession_u8 != DCM_DEFAULT_SESSION_IDX)
    {
        /* Set the default session, default security level and default timing */
        Dcm_Prv_SetSesCtrlType(Dcm_DsldSessionTable_pcu8[DCM_DEFAULT_SESSION_IDX]);
    }

    /* Reset the bit to start the new high prio protocol */
    Dcm_DsldGlobal_st.flgCommActive_b = FALSE;
    Dcm_DsldGlobal_st.dataRequestLength_u16  = Dcm_DsldGlobal_st.dataNewdataRequestLength_u16;

    /* Multicore: No lock needed here as Dsl state is written here when Dcm is free after protocol preemption */
    /* Also even if there is a request received when this code is running, there is no impact on functionality */

    /* Go to next state to start new higher prio protocol */
    Dcm_Prv_SetDslState((DSL_STATE_REQUEST_RECEIVED));
    Dcm_Prv_SetDslSubState((DSL_SUBSTATE_START_PROTOCOL));
    Dcm_Prv_SetDsdState(DSD_IDLE);
    /* Reset the state */
    Dcm_Prv_ResetDsdSubStateMachine();
    Dcm_Prv_SetDslPreemptionState((DSL_STATE_IDLE));

    /* If the ongoing transmission was cancelled in this state, the new request should not be processed in the
       current cycle as there is a chance that service can finish the processing in the same cycle and
       PduR_Transmit can return E_NOT_OK as transmission of old protocol is under progress */

    if((stCancelTx_u8 == E_OK) && \
            (Dcm_DsldGlobal_st.dataOldtxPduId_u8 == Dcm_DsldConnTable_pcst[idxConnection_u8].txpduid_num_u8))
    {
        /* Break out of the state machine and the request will be processed in the next DCM cycle */
        DCM_TimerProcess(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32,Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
    }
}
#endif






#if((DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF) && (DCM_ROE_ENABLED != DCM_CFG_OFF))
/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslSubState_StopRoe
 Syntax           : Dcm_Prv_DslSubState_StopRoe(void)
 Description      : This function is used to process sub state DSL_SUBSTATE_STOP_ROE.
                    In this sub state ongoing Roe event is stopped as new high priority is received.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslSubState_StopRoe(void)
{
    /*Check if the Persist-Roe flag value is set*/
    if(flgPersistRoe_b != FALSE)
    {
        /* Multicore: No locking necessary as Dcm_DsldGlobal_st.dataActiveRxPduId_u8 is an atomic variable and
           there is no parallel writing due to DSL state machine handling */

        uint8 idxConnection_u8 = Dcm_DsldRxTable_pcu8[Dcm_DsldGlobal_st.dataActiveRxPduId_u8];
        uint8 idxProtocol_u8   = Dcm_DsldConnTable_pcst[idxConnection_u8].protocol_num_u8;

        /* Call  ROE confirmation because DCM killing ROE requested service */
        DcmAppl_DcmConfirmation(Dcm_DsldGlobal_st.dataSid_u8,
                Dcm_DsldMsgContext_st.dcmRxPduId,
                Dcm_Dsld_RoeRxToTestSrcMappingTable[Dcm_DsldMsgContext_st.dcmRxPduId].testsrcaddr_u16,
                DCM_RES_POS_NOT_OK);

        Dcm_DsldGlobal_st.idxCurrentProtocol_u8 = idxProtocol_u8;
        Dcm_DsldGlobal_st.nrActiveConn_u8       = idxConnection_u8;
        Dcm_DsldGlobal_st.dataRequestLength_u16 = Dcm_DsldGlobal_st.dataPassdataRequestLength_u16;

        /* Multicore: No lock needed here as Dsl state is an atomic operation
           Also if the state is changed here the changes to the Dsl state cannot happen from any other place
           This code is reached during protocol preemption and so no parallel update.*/

        /* Go to next state to start protocol */
        Dcm_Prv_SetDslState((DSL_STATE_REQUEST_RECEIVED));
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_START_PROTOCOL));
        Dcm_Prv_SetDsdState(DSD_IDLE);
        /* Reset the state */
        Dcm_Prv_ResetDsdSubStateMachine();
        Dcm_Prv_SetDslPreemptionState((DSL_STATE_IDLE));

        /* If the ongoing transmission was cancelled in this state, the new request should not be processed in the
        current cycle as there is a chance that service can finish the processing in the same cycle and
        PduR_Transmit can return E_NOT_OK as transmission of old protocol is under progress */

        if((stCancelTx_u8 == E_OK) &&
                (Dcm_DsldGlobal_st.dataOldtxPduId_u8 == Dcm_DsldConnTable_pcst[idxConnection_u8].txpduid_num_u8))
        {
            /* Break out of the state machine and the request will be processed in the next DCM cycle */
            DCM_TimerProcess(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32,Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
        }
    }
}
#endif






/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslSubState_SendFinalResponse
 Syntax           : Dcm_Prv_DslSubState_SendFinalResponse(void)
 Description      : This function is used to process Sub state DSL_SUBSTATE_SEND_FINAL_RESPONSE.
                    In this sub state Final response will be trigerred to Lower layer
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslSubState_SendFinalResponse(void)
{
     Dcm_DsldPduInfo_st.SduDataPtr = Dcm_DslTransmit_st.TxBuffer_tpu8;
     Dcm_DsldPduInfo_st.SduLength = (PduLengthType)Dcm_DslTransmit_st.TxResponseLength_u32;

     /* Multicore: No lock needed here as Dsl state is an atomic operation */
     /* Also if the state is changed here the changes to the Dsl state cannot happen from any other place */

     /* change the state to wait for Tx confirmation */
     Dcm_Prv_SetDslState((DSL_STATE_WAITING_FOR_TXCONFIRMATION));

#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
     /* This is to trigger the PduR for paged buffer transmission */
     if(Dcm_DsldGlobal_st.flgPagedBufferTxOn_b != FALSE)
     {
         /* Change the state such that DCM should give this Page to TP for next call of Dcm_Provide_TxBuffer()  */
         /* Multicore: No lock needed here as Dsl state is an atomic operation */
         /* Also if the state is changed here the changes to the Dsl state cannot happen from any other place */
         Dcm_Prv_SetDslState((DSL_STATE_PAGEDBUFFER_TRANSMISSION));
         Dcm_Prv_SetDslSubState((DSL_SUBSTATE_DATA_READY));
     }
#endif

     /* This is final response. To indicate that cntrWaitpendCounter_u8 is made as zero */
     Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 = 0x0u;

     /* After getting the Tx confirmation DSL state changed to DSL_STATE_IDLE state */
     Dcm_Prv_SetDslNextState((DSL_STATE_IDLE));
     Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));

     Dcm_Prv_SendResponse(& Dcm_DsldPduInfo_st);
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslSubState_SendGeneralReject
 Syntax           : Dcm_Prv_DslSubState_SendGeneralReject(void)
 Description      : This fucntion is used to process sub state DSL_SUBSTATE_SEND_GENERAL_REJECT.
                    In this sub state General reject will be triggered to the lower layer
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00120]*/
static void Dcm_Prv_DslSubState_SendGeneralReject(void)
{
    uint8 NrcValue_u8 = 0u;

    /* Configured number of wait pend is sent, Now send general reject */
    if(Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 == DCM_CFG_MAX_WAITPEND)
    {
        if(Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8].service_handler_fp != NULL_PTR)
        {
            /* Call service initialisation function */
            (void)(*Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8].service_handler_fp) \
                                (DCM_CANCEL,&Dcm_DsldMsgContext_st,&NrcValue_u8);

            /* Re-assign the opstatus to DCM_INITIAL after cancellation */
            Dcm_SrvOpstatus_u8 = DCM_INITIAL;
            Dcm_ExtSrvOpStatus_u8 = DCM_INITIAL;
        }
        Dcm_DsldGlobal_st.idxService_u8       = 0x00u;
        Dcm_DsldGlobal_st.dataResponseByDsd_b = TRUE;
        Dcm_DslTransmit_st.isForceResponsePendRequested_b = FALSE;

        /* Reset SubState Machine */
        Dcm_Prv_ResetDsdSubStateMachine();
        Dcm_SetNegResponse( &Dcm_DsldMsgContext_st ,DCM_E_GENERALREJECT);

        Dcm_isGeneralRejectSent_b = TRUE;
#if (DCM_CFG_RBA_DIAGADAPT_SUPPORT_ENABLED != DCM_CFG_OFF)
        /* Call the rba_DiagAdapt API to indicate the trigger of NRC 0x10 from DSL state machine to reset the rba_DiagAdapt */
        rba_DiagAdapt_Confirmation_GeneralReject(Dcm_DsldGlobal_st.dataSid_u8);
#endif
        Dcm_ProcessingDone (&Dcm_DsldMsgContext_st);
    }
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslSubState_SendPendingResponse
 Syntax           : Dcm_Prv_DslSubState_SendPendingResponse(void)
 Description      : This function is used to process Sub state DSL_SUBSTATE_SEND_PENDING_RESPONSE.
                    In this sub state following functionality executed,
                    1)BusyRepeatRequest is send when P2* timer is elapse even befor first service call
                    2)Pending response is sent when P2*max timer is elapsed and service needs more time to fetch the data
                    3)General reject is sent when wait pend counter is elapsed or wait pend counter is configured to 0
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslSubState_SendPendingResponse(void)
{
    uint8 currentprotocolId_u8 ;

    if(FALSE == Dcm_Prv_Check_PendingResponseForKWP())
    {
        /* If the request has arrived to trigger the waitpend even before first service call */
        if(TRUE == Dcm_Prv_IsVerifyDataProcessing())
        {
            /* Set the DSL state to default state */
            Dcm_Prv_SetDslNextState((DSL_STATE_IDLE));
            Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
            Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));

            currentprotocolId_u8=Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8;
            if(!((currentprotocolId_u8== DCM_OBD_ON_CAN)||(currentprotocolId_u8== DCM_OBD_ON_FLEXRAY)||
                 (currentprotocolId_u8== DCM_OBD_ON_IP)||(currentprotocolId_u8== DCM_OBD_ON_KLINE)))
            {
                /* To call Inactive Diagnostic function, DSD state set to DSD_WAITFORTXCONF */
			    Dcm_Prv_SetDsdState(DSD_WAITFORTXCONF);
                /* Frame the Busy repeat response */
                Dcm_Prv_SetIntermediateResponse(DCM_E_BUSYREPEATREQUEST);
                Dcm_Prv_SendResponse(& Dcm_DsldPduInfo_st);
            }
            else
            {
                /* Set the DSL and DSD state to default state */
			    Dcm_Prv_SetDsdState(DSD_IDLE);
                Dcm_Prv_SetDslState((DSL_STATE_IDLE));
            }
        }
        else
        {
/*TRACE[SWS_Dcm_00024]*/
#if (DCM_CFG_MAX_WAITPEND > 0)
            if((Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 < DCM_CFG_MAX_WAITPEND) || \
                    (DCM_CFG_MAX_WAITPEND == DCM_MAX_LIMIT_WAITPEND))
            {
                Dcm_DsldMsgContext_st.msgAddInfo.suppressPosResponse = FALSE;
                /* Frame wait pend response */
                Dcm_Prv_SetIntermediateResponse(DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING);

                if(Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 < DCM_MAX_LIMIT_WAITPEND)
                {
                    /* increment wait pend counter to indicate one wait pend is sent */
                    Dcm_DsldGlobal_st.cntrWaitpendCounter_u8++;
                }
                Dcm_Prv_SendResponse(&Dcm_DsldPduInfo_st);
            }
            else
            {
                /*TRACE[SWS_Dcm_00120]*/
                Dcm_Prv_DslSubState_SendGeneralReject();
            }
#else
                Dcm_Prv_DslSubState_SendGeneralReject();
#endif
        }
    }

}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslSubState_MonitorP2Max
 Syntax           : Dcm_Prv_DslSubState_MonitorP2Max(void)
 Description      : This function is used to process Sub state DSL_SUBSTATE_P2MAX_TIMEMONITORING.
                    In this sub state P2max timer will be monitored.If this timer is elapsed then Pending response will be
                    triggered.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslSubState_MonitorP2Max(void)
{
    DCM_TimerProcess(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32,Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)

    /* Check P2 max expired or not */
    if(FALSE != DCM_TimerElapsed(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32))
    {
        Dcm_Prv_DslSubState_SendPendingResponse();
    }
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslSubState_StartProtocol
 Syntax           : Dcm_Prv_DslSubState_StartProtocol
 Description      : This Function is used to process Sub state DSL_SUBSTATE_START_PROTOCOL.
                    In this sub state Protocol start functionality is executed
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslSubState_StartProtocol(void)
{
    if(FALSE == Dcm_Prv_isProtocolStarted())
    {
        if(E_OK != Dcm_Prv_StartProtocol())
        {
            Dcm_Prv_SendNrcforStartProtocolFailure();
        }

        Dcm_isStopProtocolInvoked_b = FALSE;
    }

    if(FALSE != Dcm_Prv_isProtocolStarted())
    {
        Dcm_Prv_UpdateStatesForStartProtocol();
    }
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslSubState_S3_OR_P3_Timeout
 Syntax           : Dcm_Prv_DslSubState_S3_OR_P3_Timeout(idxActiveSession_u8)
 Description      : This function is used to process DSL sub state DSL_SUBSTATE_S3_OR_P3_TIMEOUT
                    In this Sub state timeout functionality will be perfomed.
 Parameter        : uint8
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00140]*/
static void Dcm_Prv_DslSubState_S3_OR_P3_Timeout(uint8 idxActiveSession_u8)
{
#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
    if(DCM_IS_KWPPROT_ACTIVE() != FALSE)
    {
        Dcm_DsldGlobal_st.flgCommActive_b  = FALSE;
        DcmAppl_P3TimeoutIndication();
    }
#endif
    /* Full communication mode is not required when in non default session*/
    if (idxActiveSession_u8 != DCM_DEFAULT_SESSION_IDX)
    {
        /*TRACE[SWS_Dcm_00168]*/
        ComM_DCM_InactiveDiagnostic (Dcm_active_commode_e \
                [Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId);
    }

#if(DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED!=DCM_CFG_OFF)

#if (DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF)

    (void)SchM_Switch_Dcm_DcmDiagnosticSessionControl(RTE_MODE_DcmDiagnosticSessionControl_DEFAULT_SESSION);
#endif

    (void)DcmAppl_Switch_DcmDiagnosticSessionControl(Dcm_DsldSessionTable_pcu8[DCM_DEFAULT_SESSION_IDX]);
#endif

    Dcm_Prv_SetSesCtrlType(Dcm_DsldSessionTable_pcu8[DCM_DEFAULT_SESSION_IDX]);
    Dcm_DsldGlobal_st.flgMonitorP3timer_b = FALSE;
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslSubState_S3_OR_P3_TimeMoniotor
 Syntax           : Dcm_Prv_DslSubState_S3_OR_P3_TimeMoniotor(void)
 Description      : This function is used to Perform DSL Sub state DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING
                    In this sub state S3 Or P3 timer will be monitored and processd
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslSubState_S3_OR_P3_TimeMoniotor(void)
{
    /* Check if data Timeout has occured*/
    if(FALSE == (DCM_TimerElapsed(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32)))
    {
        DCM_TimerProcess(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32,Dcm_P2OrS3StartTick_u32,\
                Dcm_P2OrS3TimerStatus_uchr)
    }
    else
    {
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEOUT));
    }
}



/***********************************************************************************************************************
 *                    START OF DSL STATES
 **********************************************************************************************************************/


#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslState_PagedBufferTransmission
 Syntax           : Dcm_Prv_DslState_PagedBufferTransmission(void)
 Description      : This Function is used to process DSL state DSL_STATE_PAGEDBUFFER_TRANSMISSION.
                    In this state Updating page and transmitting to lower layer functionalities are executed
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslState_PagedBufferTransmission(void)
{
    uint16 pageLen_u16 = 0u;

    if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_WAIT_FOR_DATA)
    {
        if(Dcm_Prv_GetDsdState() == DSD_WAITFORTXCONF)
        {
            /* Start Paged buffer timeout timer */
            DCM_TimerStart(Dcm_DsldGlobal_st.dataPagedBufferTimeout_u32, DCM_PAGEDBUFFER_TIMEOUT,
                    Dcm_PagedBufferStartTick_u32,Dcm_PagedBufferTimerStatus_uchr)

            /* Call the service in DSD state machine  */
            Dcm_Prv_SetDsdState(DSD_CALL_SERVICE);

            /* Inform the service to fill the data into page */
            if(Dcm_adrUpdatePage_pfct != NULL_PTR)
            {
                pageLen_u16 = (uint16)(Dcm_DsldMsgContext_st.resMaxDataLen+1uL);

                (*Dcm_adrUpdatePage_pfct)(&Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].tx_buffer_pa[2],
                        pageLen_u16);
            }
        }
    }
    else
    {
        if((Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_DATA_READY) || \
                (Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_WAIT_PAGE_TXCONFIRM))
        {
            /*do nothing*/
        }
    }
}
#endif





/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslState_WaitingForTxConfirmation
 Syntax           : Dcm_Prv_DslState_WaitingForTxConfirmation(void)
 Description      : This Function is used to process DSL state DSL_STATE_WAITING_FOR_TXCONFIRMATION.
                    In this state DCM is waitng for the confirmation from the lower layer for the response it had sent.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslState_WaitingForTxConfirmation(void)
{
    uint8 Dcm_DslStateTemp_u8 = 0u;
    (void)Dcm_DslStateTemp_u8;
    /*Do nothing*/
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslState_ResponseTransmission
 Syntax           : Dcm_Prv_DslState_ResponseTransmission(void)
 Description      : This function is used to process DSL state DSL_STATE_RESPONSE_TRANSMISSION.
                    In this state Pending ,General Reject and Final response will be sent.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslState_ResponseTransmission(void)
{
    if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_SEND_PENDING_RESPONSE)
    {
        Dcm_Prv_DslSubState_SendPendingResponse();
    }

    if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_SEND_GENERAL_REJECT)
    {
        Dcm_Prv_DslSubState_SendGeneralReject();
    }

    if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_SEND_FINAL_RESPONSE)
    {
        Dcm_Prv_DslSubState_SendFinalResponse();
    }
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslState_RequestReceived
 Syntax           : Dcm_Prv_DslState_RequestReceived(void)
 Description      : This function is used to process DSL state  DSL_STATE_REQUEST_RECEIVED.
                    In this state start of new protocol and P2max time monitoring functionality will be executed.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslState_RequestReceived(void)
{
    if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_START_PROTOCOL)
    {
        Dcm_Prv_DslSubState_StartProtocol();
    }

    if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_P2MAX_TIMEMONITORING)
    {
        Dcm_Prv_DslSubState_MonitorP2Max();
    }
}




#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslState_ProtocolPreemption
 Syntax           : Dcm_Prv_DslState_ProtocolPreemption(void)
 Description      : This Function is used to process DSL state DSL_STATE_PROTOCOL_PREEMPTION.In this state
                    protocol preemption functionality will be execuetd
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00015][SWS_Dcm_00459][SWS_Dcm_00625]*/
static void Dcm_Prv_DslState_ProtocolPreemption(void)
{
    if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_STOP_PROTOCOL)
    {
        Dcm_Prv_DslSubState_StopProtocol();
    }
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    else
    {
        if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_STOP_ROE)
        {
            Dcm_Prv_DslSubState_StopRoe();
        }
    }
#endif
}

#endif





/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslState_WaitingForRxIndication
 Syntax           : Dcm_Prv_DslState_WaitingForRxIndication(void)
 Description      : This function is to process DSL state DSL_STATE_WAITING_FOR_TXCONFIRMATION.
                    When DSL is in this state DCM is waiting for the Indication from the lower layer for
                    complete receptionof the request
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DslState_WaitingForRxIndication(void)
{
    uint8 Dcm_DslStateTemp_u8 = 0u;
    (void)Dcm_DslStateTemp_u8;
    /*Do nothing*/
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslState_Idle
 Syntax           : Dcm_Prv_DslState_Idle(void)
 Description      : This Function is used to process DSL state DSL_STATE_IDLE.
                    In this state S3 and P3 Time monitoring and timeout functionality will be handled
 Parameter        : None
 Return value     : none
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00140][SWS_Dcm_00141]*/
static void Dcm_Prv_DslState_Idle(void)
{
    uint8 idxActiveSession_u8 = 0u;

    SchM_Enter_Dcm_Global_NoNest();
    idxActiveSession_u8 = Dcm_DsldGlobal_st.idxActiveSession_u8;
    SchM_Exit_Dcm_Global_NoNest();

    if((idxActiveSession_u8 != DCM_DEFAULT_SESSION_IDX) || (Dcm_DsldGlobal_st.flgMonitorP3timer_b != FALSE))
    {
        if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING)
        {
            Dcm_Prv_DslSubState_S3_OR_P3_TimeMoniotor();
        }

        if(Dcm_Prv_GetDslSubState() == DSL_SUBSTATE_S3_OR_P3_TIMEOUT)
        {
            Dcm_Prv_DslSubState_S3_OR_P3_Timeout(idxActiveSession_u8);
            Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
        }
    }
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_DslStateMachine
 Syntax           : Dcm_Prv_DslStateMachine(void)
 Description      : This Function is used to execute the DSL state machine which is called in Dcm_MainFunction()
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
void Dcm_Prv_DslStateMachine(void)
{
    uint8 DslStateTemp_u8 = 0u;

#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_Check_ProtocolPreemptionStatus();
#endif

    SchM_Enter_Dcm_Global_NoNest();
    DslStateTemp_u8 = Dcm_Prv_GetDslState();
    SchM_Exit_Dcm_Global_NoNest();

    Dcm_Prv_ProcessDslStateMachine(DslStateTemp_u8);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

