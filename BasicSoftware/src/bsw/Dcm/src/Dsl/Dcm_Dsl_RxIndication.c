

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
#define DCM_KWP_PROTOCOL         (0x80u)
#define DCM_KWP_MASK             (0xF0u)


/***********************************************************************************************************************
 *    FUNCTIONLIKE MACROS
***********************************************************************************************************************/
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
#define Dcm_Prv_isHighPrioRequestReceived(DcmRxPduId)                (DcmRxPduId == Dcm_DsldGlobal_st.dataNewRxPduId_u8)

#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    #define Dcm_Prv_isHighPrioReqReceivedWhileRoeEvent(DcmRxPduId)   (DcmRxPduId == Dcm_DsldGlobal_st.dataPassRxPduId_u8)
#endif
#endif

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
PduInfoType Dcm_DsldPduInfo_st;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
PduInfoType Dcm_OBDPduInfo_st;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"

/***********************************************************************************************************************
 *    Inline Function Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckLowPriorityRequestReceived
 Syntax           : Dcm_Prv_CheckLowPriorityRequestReceived(DcmRxPduId)
 Description      : This Inline function is used to check whether request received in Low Priority than current protocol
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_CheckLowPriorityRequestReceived(PduIdType DcmRxPduId)
{
    return (Dcm_DsldProtocol_pcst[Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8].nrc21_b);
}




#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isConsecutiveRequestReceived
 Syntax           : Dcm_Prv_isConsecutiveRequestReceived(DcmRxPduId)
 Description      : This Inline Function is used to check whether request is consecutive request
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isConsecutiveRequestReceived(PduIdType DcmRxPduId)
{
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED == DCM_CFG_OFF)
    (void)DcmRxPduId;
#endif

    /*If the Communication Protocol is Active */
    return((Dcm_DsldGlobal_st.flgCommActive_b != FALSE)
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
       && (DcmRxPduId != Dcm_DsldGlobal_st.dataNewRxPduId_u8)
#endif
       );
}
#endif




#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isKWPRequestReceived
 Syntax           : Dcm_Prv_isKWPRequestReceived(idxProtocol_u8)
 Description      : This Inline function is used to Check if Request received is of KWP protocol
 Parameter        : uint8
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isKWPRequestReceived(uint8 idxProtocol_u8)
{
    return((Dcm_DsldProtocol_pcst[idxProtocol_u8].protocolid_u8 & DCM_KWP_MASK) == DCM_KWP_PROTOCOL);
}
#endif




/***********************************************************************************************************************
 Function name    : Dcm_Prv_StartP2Timer
 Syntax           : Dcm_Prv_StartP2Timer(void)
 Description      : This Inline function is used to Reload P2 Timer
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_StartP2Timer(void)
{
    DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32,
                  (Dcm_DsldTimer_st.dataTimeoutP2max_u32 - \
                          Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].dataP2TmrAdjust),
                   Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_GetRequestLength
 Syntax           : Dcm_Prv_GetRequestLength(DcmRxPduId)
 Description      : This Inline Function is used to get the request length
 Parameter        : PduIdType
 Return value     : PduLengthType
***********************************************************************************************************************/
LOCAL_INLINE PduLengthType Dcm_Prv_GetRequestLength(PduIdType DcmRxPduId)
{
    PduLengthType RequestLength = Dcm_DsldGlobal_st.dataRequestLength_u16;

#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    if(FALSE != Dcm_Prv_isHighPrioRequestReceived((DcmRxPduId)))
    {
        RequestLength = Dcm_DsldGlobal_st.dataNewdataRequestLength_u16;
    }
    #if(DCM_ROE_ENABLED != DCM_CFG_OFF)
        else
        {
            if(FALSE != Dcm_Prv_isHighPrioReqReceivedWhileRoeEvent((DcmRxPduId)))
            {
                RequestLength = Dcm_DsldGlobal_st.dataPassdataRequestLength_u16;
            }
        }
    #endif
#else
    (void)DcmRxPduId;
#endif

    return RequestLength;
}



/***********************************************************************************************************************
 *    Function Definitions
 **********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"


/***********************************************************************************************************************
 Function name    : Dcm_Prv_ResetCopyRxDataStatus
 Syntax           : Dcm_Prv_ResetCopyRxDataStatus(Result)
 Description      : Function to reset CopyRxData status of all other dataRxPduId_u8 except that dataRxPduId_u8 which
                    has been passed as parameter
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
void Dcm_Prv_ResetCopyRxDataStatus (PduIdType RxPduId)
{
    PduIdType idxRxPduid;

    for ( idxRxPduid = 0 ; idxRxPduid < DCM_CFG_NUM_RX_PDUID ; idxRxPduid++ )
    {
        /*Check if the CopyRxData status is set and dataRxPduId_u8 which has been not passed as parameter */
        if ((idxRxPduid != RxPduId) && (Dcm_DslRxPduArray_ast[idxRxPduid].Dcm_DslCopyRxData_b != FALSE))
        {
            Dcm_DslRxPduArray_ast[idxRxPduid].Dcm_DslCopyRxData_b = FALSE;
        }
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DiscardRequest
 Syntax           : Dcm_Prv_DiscardRequest(DcmRxPduId,Result)
 Description      : This Function is used to Discard the request by Resetting the DSL states and reloading the s3 timer
 Parameter        : PduIdType,Std_ReturnType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DiscardRequest(PduIdType DcmRxPduId,
        Std_ReturnType Result)
{

#if(DCM_CALLAPPLICATIONONREQRX_ENABLED != DCM_CFG_OFF)
    if(FALSE != Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b)
    {
        (void)DcmAppl_TpRxIndication(DcmRxPduId,Result);
    }
#else
    (void)Result;
#endif

    Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b  = FALSE;
    Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8        = DCM_SERVICEID_DEFAULT_VALUE;

    if((Dcm_DsldGlobal_st.dataActiveRxPduId_u8 == DcmRxPduId) && \
            (Dcm_Prv_GetDslState() == DSL_STATE_WAITING_FOR_RXINDICATION))
    {
        Dcm_Prv_SetDslState((DSL_STATE_IDLE));
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
        /*TRACE[SWS_Dcm_00141]*/
        Dcm_Prv_ReloadS3Timer();
    }

#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    if(FALSE != Dcm_Prv_isHighPrioRequestReceived((DcmRxPduId)))
    {
        Dcm_DsldGlobal_st.dataNewRxPduId_u8 = DCM_PDUID_DEFAULT_VALUE;
        Dcm_Prv_SetDslPreemptionState((DSL_STATE_IDLE));
    }
    #if(DCM_ROE_ENABLED != DCM_CFG_OFF)
        if(FALSE != Dcm_Prv_isHighPrioReqReceivedWhileRoeEvent((DcmRxPduId)))
        {
            Dcm_DsldGlobal_st.dataPassRxPduId_u8 = DCM_PDUID_DEFAULT_VALUE;
            Dcm_Prv_SetDslState((DSL_STATE_IDLE));

            /* If ROE TxConfirmation has come; then the DSL state would be changes to RECEIVING.. */
            /* In case of Discarded RxIndication; The DSL next State has to be reset back to READY FOR RECEPTION */
            if(Dcm_Prv_GetDslState() != DSL_STATE_WAITING_FOR_RXINDICATION)
            {
                Dcm_Prv_SetDslNextState((DSL_STATE_IDLE));
                Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
            }
            /*TRACE[SWS_Dcm_00141]*/
            Dcm_Prv_ReloadS3Timer();
        }

    #endif
#endif

}




#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessHighPriorityRequest
 Syntax           : Dcm_Prv_ProcessHighPriorityRequest(DcmRxPduId)
 Description      : This Function is used to do following activities
                    1) Update preemption state
                    2) Inactivate channel for previous protocol
                    3) Start P2* timer
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessHighPriorityRequest(PduIdType DcmRxPduId)
{
    PduIdType dataActiveRxPduId_u8 = 0u;
    uint8 idxProtocol_u8 = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8;

    /* Store the active tx pdu id number */
    Dcm_DsldGlobal_st.dataOldtxPduId_u8 = \
    Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_DsldGlobal_st.dataActiveRxPduId_u8]].txpduid_num_u8;

    /* Multicore: Lock needed here to avoid reception of ROE/RDPI events when the DSL has already become free */
    SchM_Enter_Dcm_Global_NoNest();

    /*Dont change this sequence in future*/
    Dcm_DsldGlobal_st.dataActiveTxPduId_u8    = DCM_PDUID_DEFAULT_VALUE;
    Dcm_Prv_SetDslPreemptionState((DSL_SUBSTATE_STOP_PROTOCOL));
    dataActiveRxPduId_u8 = Dcm_DsldGlobal_st.dataActiveRxPduId_u8;
    Dcm_DsldGlobal_st.dataActiveRxPduId_u8    = DcmRxPduId;

    SchM_Exit_Dcm_Global_NoNest();
    /* If parallel processing is used, check whether the same channel is not used by OBD protocol */
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    if( ((Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[dataActiveRxPduId_u8]].channel_idx_u8].ComMChannelId)
       != (Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_OBDGlobal_st.dataActiveRxPduId_u8]]
       .channel_idx_u8].ComMChannelId)) || (Dcm_Prv_GetOBDState() == DCM_OBD_IDLE))
#endif
    {
        /* Invoke ComM_Inactive Diagnosis for the old protocol */
        if(Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].channel_idx_u8].ComMChannelId !=
           Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[dataActiveRxPduId_u8]].channel_idx_u8].ComMChannelId)
        {
            /*TRACE[SWS_Dcm_00168]*/
            ComM_DCM_InactiveDiagnostic (Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[dataActiveRxPduId_u8]].channel_idx_u8].ComMChannelId);
        }
    }
    Dcm_DsldGlobal_st.dataNewRxPduId_u8 = DCM_PDUID_DEFAULT_VALUE;
	
	/* Reset the length, needed esp. for paged buffer processing */
    Dcm_DsldPduInfo_st.SduLength = 0x00u;

#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    Dcm_DsldGlobal_st.dataPassRxPduId_u8 = DCM_PDUID_DEFAULT_VALUE;
#endif

    /* Start P2Star timer */
    DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32,
                  (Dcm_DsldTimer_st.dataTimeoutP2max_u32 - Dcm_DsldProtocol_pcst[idxProtocol_u8].dataP2TmrAdjust),
                   Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
}
#endif





#if((DCM_ROE_ENABLED != DCM_CFG_OFF) && (DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF))
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessHighPriorityReqWhileRoeEvent
 Syntax           : Dcm_Prv_ProcessHighPriorityReqWhileRoeEvent(DcmRxPduId)
 Description      : This Function is used to do the following activities
                    1)ROE tx confirmation has not yet come; ROE is in processing. Stop Roe by moving
                    Active tx pdu id to a invalid value 0xFF
                    2)setting the Dsl next state to DSL_SUBSTATE_STOP_ROE
                    3)Start P2* timer
 Parameter        : PduIdType
 Return value     : void
***********************************************************************************************************************/
static void Dcm_Prv_ProcessHighPriorityReqWhileRoeEvent(PduIdType DcmRxPduId)
{
    /* Store the active tx pdu id number */
    Dcm_DsldGlobal_st.dataOldtxPduId_u8 = \
    Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_DsldGlobal_st.dataActiveRxPduId_u8]].txpduid_num_u8;

    /* Multicore: Lock needed here to avoid reception of ROE/RDPI events when the DSL has already become free */
    SchM_Enter_Dcm_Global_NoNest();

    if(Dcm_Prv_GetDslState() == DSL_STATE_WAITING_FOR_RXINDICATION)
    {
        Dcm_Prv_SetDslState((DSL_STATE_REQUEST_RECEIVED));
        Dcm_Prv_SetDslSubState((DSL_SUBSTATE_START_PROTOCOL));
        Dcm_DsldGlobal_st.dataActiveRxPduId_u8   = DcmRxPduId;
        Dcm_DsldGlobal_st.dataRequestLength_u16  = Dcm_DsldGlobal_st.dataPassdataRequestLength_u16;
        Dcm_DsldGlobal_st.dataPassRxPduId_u8     = DCM_PDUID_DEFAULT_VALUE;
    }
    else
    {
        Dcm_DsldGlobal_st.dataActiveTxPduId_u8    = DCM_PDUID_DEFAULT_VALUE;
        Dcm_Prv_SetDslPreemptionState((DSL_SUBSTATE_STOP_ROE));
        Dcm_DsldGlobal_st.dataRequestLength_u16   = Dcm_DsldGlobal_st.dataPassdataRequestLength_u16;
        Dcm_DsldGlobal_st.dataActiveRxPduId_u8    = DcmRxPduId;
        Dcm_DsldGlobal_st.dataPassRxPduId_u8      = DCM_PDUID_DEFAULT_VALUE;

        /* After Stopping ROE , the Active service table has to be restored from the old Service Table */
        /* Change the service table to originally active table */
        if(Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest == DCM_ROE_SOURCE)
        {
            Dcm_DsldGlobal_st.datActiveSrvtable_u8  = Dcm_DsldGlobal_st.dataOldSrvtable_u8;
        }
        Dcm_DsldSrvTable_pcst = Dcm_Dsld_Conf_cs.sid_table_pcs[Dcm_DsldGlobal_st.datActiveSrvtable_u8].ptr_service_table_pcs;
    }
    SchM_Exit_Dcm_Global_NoNest();

    /* Start P2 timer */
    DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32, \
                  (Dcm_DsldTimer_st.dataTimeoutP2max_u32 -   \
                  Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].dataP2TmrAdjust), \
                  Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
}
#endif






/***********************************************************************************************************************
 Function name   : Dcm_Prv_ReloadP2maxValue
 Syntax          : Dcm_Prv_ReloadP2maxValue(DcmRxPduId,idxProtocol_u8)
 Description     : If it is new UDS protocol then we need to start P2 timer with default P2 max.
                   If it is new KWP then we need to start P2 timer with P2 max from corresponding protocol
                   Do nothing for consecutive requests.
                   If protocol preemption is not enabled and KWP protocol is not present then only UDS protocol is present
                   So already the UDS P2 timing is updated during Initialization. As a result no need to load the time again
                   Also after transition from non default to default session, default P2 timer is loaded in Dcm_Prv_SetSesCtrlType API
 Parameter       : PduIdType,uint8
 Return value    : None
***********************************************************************************************************************/
static void Dcm_Prv_ReloadP2maxValue(PduIdType DcmRxPduId,uint8 idxProtocol_u8)
{
#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
    uint8 idxKwpTiming_u8 = 0u;
#endif

#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
    if(FALSE == Dcm_Prv_isConsecutiveRequestReceived(DcmRxPduId))
    {
        if(FALSE != Dcm_Prv_isKWPRequestReceived(idxProtocol_u8))
        {
            /* This is required for the first request of KWP. For consecutive requests  "Dcm_DsldTimer_st.dataTimeoutP2max_u32"
             *  get updated in protocol activation and might be get modified by ATP service */
            idxKwpTiming_u8 = Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].timings_idx_u8;
            Dcm_DsldTimer_st.dataTimeoutP2max_u32 = Dcm_Dsld_default_timings_acs[idxKwpTiming_u8].P2_max_u32;
        }
        else
        {
            Dcm_DsldTimer_st.dataTimeoutP2max_u32 = DCM_CFG_DEFAULT_P2MAX_TIME;
        }
    }
#else
    #if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
        if(FALSE != Dcm_Prv_isHighPrioRequestReceived((DcmRxPduId)))
        {
            Dcm_DsldTimer_st.dataTimeoutP2max_u32 = DCM_CFG_DEFAULT_P2MAX_TIME;
        }
    #endif
        (void)idxProtocol_u8;
#endif

#if((DCM_CFG_KWP_ENABLED == DCM_CFG_OFF) && (DCM_CFG_PROTOCOL_PREMPTION_ENABLED == DCM_CFG_OFF))
        (void)DcmRxPduId;
#endif

}

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDSendNRC21
 Description      : This Function sends NRC21
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_OBDSendNRC21(PduIdType DcmRxPduId)
{
/* BSW-10604 - pduInfo_st with 2 initialize values is removed */
/* BSW-10604 */ /* [$DD_BSWCODE 28466] */
#if (ECUC_RB_METADATA_ENABLE == TRUE)
    PduInfoType pduInfo_st = {NULL_PTR,NULL_PTR,DCM_NEGATIVE_RESPONSE_LENGTH};
#else 
    PduInfoType pduInfo_st = {NULL_PTR,DCM_NEGATIVE_RESPONSE_LENGTH};
#endif
/* END BSW-10604 */

    if(DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].channel_idx_u8))
    {
        if(E_OK != PduR_DcmTransmit(Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].txpduid_num_u8,&pduInfo_st))
        {
            Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8 = DCM_SERVICEID_DEFAULT_VALUE;
        }
    }
    else
    {
        DCM_DET_ERROR(DCM_TPRXIND_ID , DCM_E_FULLCOMM_DISABLED )
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ResetOBDCopyRxDataStatus
 Description      : Reset OBDRxPduArray CopyRxData to false for all ID's except for the one received
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
void Dcm_Prv_ResetOBDCopyRxDataStatus(PduIdType id)
{
    PduIdType idxRxPduid = 0x00u;

    while(idxRxPduid < DCM_CFG_NUM_RX_PDUID)
    {
        /* Reset CopyRxData status for all RxPduId's except the one passed to this function */
        if(idxRxPduid != id)
        {
            Dcm_DslOBDRxPduArray_ast[idxRxPduid].Dcm_DslCopyRxData_b = FALSE;
        }
        idxRxPduid++;
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_AcceptOBDRequest
 Description      : This Function
                    1)updates OBD StateMachine
                    2)Start P2 timer
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_AcceptOBDRequest(PduIdType id)
{
    if(id == Dcm_OBDGlobal_st.dataActiveRxPduId_u8)
    {
        Dcm_Prv_SetOBDState((DCM_OBD_REQUESTRECEIVED));
        DCM_TimerStart(Dcm_OBDGlobal_st.dataTimeoutMonitor_u32,
                          (DCM_CFG_DEFAULT_P2MAX_TIME - \
                           Dcm_DsldProtocol_pcst[Dcm_OBDGlobal_st.idxCurrentProtocol_u8].dataP2TmrAdjust),
                           Dcm_OBDP2OrS3StartTick_u32,Dcm_OBDP2OrS3TimerStatus_uchr)
     }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DiscardOBDRequest
 Description      : If reception of OBD request was unsuccessful, discard the request
                    and update the Global variables
 Parameter        : PduIdType,Std_ReturnType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_DiscardOBDRequest(PduIdType id,Std_ReturnType Result)
{
#if(DCM_CALLAPPLICATIONONREQRX_ENABLED != DCM_CFG_OFF)
    if(FALSE != Dcm_DslOBDRxPduArray_ast[id].Dcm_DslCopyRxData_b)
    {
        (void)DcmAppl_TpRxIndication(id,Result);
    }
#else
    (void)Result;
#endif

    Dcm_DslOBDRxPduArray_ast[id].Dcm_DslServiceId_u8 = DCM_SERVICEID_DEFAULT_VALUE;

    if((Dcm_OBDGlobal_st.dataActiveRxPduId_u8 == id) && \
       (Dcm_Prv_GetOBDState() == DCM_OBD_REQUESTRECEIVING))
    {
        Dcm_Prv_SetOBDState((DCM_OBD_IDLE));
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDRxIndication
 Description      : Based on the Id on which confirmation is received this function either
                    1: Accepts the request (in this case Active Diagnosis call is made)
                    2: Or Sends NRC21
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_OBDRxIndication(PduIdType DcmRxPduId)
{
    if(FALSE == Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b)
    {
        if(Dcm_Prv_CheckLowPriorityRequestReceived(DcmRxPduId))
        {
            Dcm_Prv_OBDSendNRC21(DcmRxPduId);
        }
    }
    else
    {
        Dcm_CheckActiveDiagnosticStatus(Dcm_active_commode_e[Dcm_DsldConnTable_pcst
                                       [Dcm_DsldRxTable_pcu8[DcmRxPduId]].channel_idx_u8].ComMChannelId);
#if(DCM_CALLAPPLICATIONONREQRX_ENABLED!=DCM_CFG_OFF)
        (void)DcmAppl_TpRxIndication(DcmRxPduId,E_OK);
#endif
        Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8 = DCM_SERVICEID_DEFAULT_VALUE;
        Dcm_Prv_AcceptOBDRequest(DcmRxPduId);
    }
}

#endif  /* (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF) */

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessRequest
 Syntax           : Dcm_Prv_ProcessRequest(DcmRxPduId)
 Description      : This Function is used to do the following activities
                    1)Process request
                    2)update DSL states
                    3)Start P2* timer
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessRequest(PduIdType DcmRxPduId)
{
    uint8 idxProtocol_u8 = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8;

    Dcm_Prv_ReloadP2maxValue(DcmRxPduId,idxProtocol_u8);

#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    if(FALSE != Dcm_Prv_isHighPrioRequestReceived((DcmRxPduId)))
    {
        Dcm_Prv_ProcessHighPriorityRequest(DcmRxPduId);
    }
    #if(DCM_ROE_ENABLED != DCM_CFG_OFF)
        else if(FALSE != Dcm_Prv_isHighPrioReqReceivedWhileRoeEvent((DcmRxPduId)))
        {
            Dcm_Prv_ProcessHighPriorityReqWhileRoeEvent(DcmRxPduId);
        }
    #endif
    else
#endif
    {
        if(DcmRxPduId == Dcm_DsldGlobal_st.dataActiveRxPduId_u8)
        {
            /* updating the buffer pointer with the address which has the request to be processed.
               This buffer is used for further processing of the request.  */
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
            Dcm_DsldGlobal_st.adrBufferPtr_pu8 = Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslBufferPtr_pu8;
#endif
            Dcm_Prv_SetDslState((DSL_STATE_REQUEST_RECEIVED));
            Dcm_Prv_SetDslSubState((DSL_SUBSTATE_START_PROTOCOL));
            Dcm_Prv_StartP2Timer();
         }
    }
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_SendNrc21
 Syntax           : Dcm_Prv_SendNrc21(DcmRxPduId)
 Description      : This is the indication of faking complete reception of the low-priority protocol, now trigger NRC-21
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3355]*/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3356]*/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3421]*/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3422]*/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3358]*/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3360]*/
static void Dcm_Prv_SendNrc21(PduIdType DcmRxPduId)
{
/* BSW-10604 - pduInfo_st with 2 initialize values is removed */
/* BSW-10604 */ /* [$DD_BSWCODE 28467] */
#if (ECUC_RB_METADATA_ENABLE == TRUE)
    PduInfoType pduInfo_st = {NULL_PTR,NULL_PTR,DCM_NEGATIVE_RESPONSE_LENGTH};
#else 
    PduInfoType pduInfo_st = {NULL_PTR,DCM_NEGATIVE_RESPONSE_LENGTH};
#endif
/* END BSW-10604 */

    if(DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].channel_idx_u8))
    {
        if(E_OK != PduR_DcmTransmit(Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].txpduid_num_u8, &pduInfo_st))
        {
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8 = DCM_SERVICEID_DEFAULT_VALUE;
        }
    }
    else
    {
        DCM_DET_ERROR(DCM_TPRXIND_ID , DCM_E_FULLCOMM_DISABLED )
    }
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckDiagnosticStatus
 Syntax           : Dcm_Prv_CheckDiagnosticStatus(DcmRxPduId,idxProtocol_u8,&RxBuffer_pu8)
 Description      : This function is used to Check the Diagnostic status and inform ComM module so that channel enters in
                    full communication mode.
 Parameter        : PduIdType,uint8,const uint8*
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_01050][SWS_Dcm_00169]*/
static void Dcm_Prv_CheckDiagnosticStatus(PduIdType DcmRxPduId,
        uint8 idxProtocol_u8,const uint8 * RxBuffer_pu8)
{
    if(FALSE == Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b)
    {
        if (idxProtocol_u8 == Dcm_DsldGlobal_st.idxCurrentProtocol_u8)
        {
            if((Dcm_DsldGlobal_st.idxActiveSession_u8 == DCM_DEFAULT_SESSION_IDX) && (RxBuffer_pu8 != NULL_PTR))
            {
                Dcm_CheckActiveDiagnosticStatus(Dcm_active_commode_e[Dcm_DsldConnTable_pcst[\
                                                Dcm_DsldRxTable_pcu8[DcmRxPduId]].channel_idx_u8].ComMChannelId);
            }
        }
        else
        {
            if(FALSE != Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b)
            {
                Dcm_CheckActiveDiagnosticStatus(Dcm_active_commode_e[Dcm_DsldConnTable_pcst[\
                                                    Dcm_DsldRxTable_pcu8[DcmRxPduId]].channel_idx_u8].ComMChannelId);
            }
        }
    }
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_UpdateDSLstate
 Syntax           : Dcm_Prv_UpdateDSLstate(DcmRxPduId)
 Description      : This Function is used to update the DSL state and reset S3 timer when FunctTesterPresent is received
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_UpdateDSLstate(PduIdType DcmRxPduId)
{
    /* Reset S3 timer only if request received is for running protocol else ignore */
    if(Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8 == Dcm_DsldGlobal_st.idxCurrentProtocol_u8)
    {
#if((DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF) && (DCM_ROE_ENABLED != DCM_CFG_OFF))
        /*Check if High Priority Request is Arrived When Dcm Is Processing Roe Event*/
        if(FALSE != Dcm_Prv_isHighPrioReqReceivedWhileRoeEvent((DcmRxPduId)))
        {
            Dcm_Prv_SetDslNextState((DSL_STATE_IDLE));
            Dcm_Prv_SetDslNextSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
            Dcm_DsldGlobal_st.dataPassRxPduId_u8  = DCM_PDUID_DEFAULT_VALUE;

            if(Dcm_Prv_GetDslState() == DSL_STATE_WAITING_FOR_RXINDICATION)
            {
                Dcm_Prv_SetDslState((DSL_STATE_IDLE));
                Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
            }
            Dcm_Prv_ReloadS3Timer();
        }
        else
#endif

        {
            Dcm_Prv_SetDslState((DSL_STATE_IDLE));
            Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
            Dcm_Prv_ReloadS3Timer();
        }
    }
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    else
    {
        /* In case of parallel tester present. The nexRxPdudid has to be reset for two levels of*/
        /* protocol preemption */
        Dcm_DsldGlobal_st.dataNewRxPduId_u8 = DCM_PDUID_DEFAULT_VALUE;
    }
#endif
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_isFunctionalTesterPresentProcessed
 Syntax           : Dcm_Prv_isFunctionalTesterPresentProcessed(DcmRxPduId)
 Description      : This Function is used to do the following activities
                    1) Check if funct Tester present is rceived
                    2) Reload S3 timer
                    3) Update DSL state
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00112][SWS_Dcm_00113][SWS_Dcm_01168]*/
static boolean Dcm_Prv_isFunctionalTesterPresentProcessed(PduIdType DcmRxPduId)
{
    boolean processStatus_b = FALSE;

    if(FALSE != Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b)
    {
        if(FALSE == Dcm_isRequestTobeProcessedByDSP(DcmRxPduId))
        {
            processStatus_b = TRUE;
            DcmAppl_DcmIndicationFuncTpr();
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b = FALSE;
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8       = DCM_SERVICEID_DEFAULT_VALUE;

            if(((Dcm_Prv_GetDslState() == DSL_STATE_IDLE) && (FALSE == Dcm_isFuncTPOnOtherConnection_b)) || \
                (Dcm_Prv_GetDslState() == DSL_STATE_ROETYPE1_RECEIVED))
            {
                Dcm_Prv_ReloadS3Timer();
            }
        }
        else
        {
            if((DCM_DSLD_PARALLEL_DCM_TPR_REQ_LENGTH == Dcm_Prv_GetRequestLength(DcmRxPduId)) && \
                    (DCM_IS_KWPPROT_ACTIVE() == FALSE) && (DCM_IS_KLINEPROT_ACTIVE() == FALSE))
            {
                processStatus_b = TRUE;
                DcmAppl_DcmIndicationFuncTpr();
                Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b = FALSE;

                Dcm_Prv_UpdateDSLstate(DcmRxPduId);
            }
        }
    }
    return processStatus_b;
}




#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isRequestQueued
 Syntax           : Dcm_Prv_isRequestQueued(DcmRxPduId,Result)
 Description      : This function is used to check whether request is queued and update the Dcm Queue state
 Parameter        : PduIdType,Std_ReturnType
 Return value     : boolean
***********************************************************************************************************************/
static boolean Dcm_Prv_isRequestQueued(PduIdType DcmRxPduId,
        Std_ReturnType Result)
{
    boolean isRequestQueued = TRUE;
    uint8 idxProtocol_u8    = 0;

    if((Dcm_QueueStructure_st.Dcm_QueHandling_en == DCM_QUEUE_IDLE) ||\
                    (Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b != FALSE))
    {
        isRequestQueued = FALSE;
    }
    else
    {
        /* if the Rx indication is for the queued request */
        SchM_Enter_Dcm_Global_NoNest();
        if((Dcm_QueueStructure_st.Dcm_QueHandling_en == DCM_QUEUE_RUNNING) && \
                (Dcm_Prv_GetDslState() != DSL_STATE_IDLE))
        {
            if(E_OK == Result)
            {
                /* Queing of the second request is completed */
                Dcm_QueueStructure_st.Dcm_QueHandling_en = DCM_QUEUE_COMPLETED;
            }
            else
            {
                idxProtocol_u8 = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8;

                (void)Dcm_Prv_ProvideFreeBuffer(idxProtocol_u8,TRUE);

                Dcm_QueueStructure_st.Dcm_QueHandling_en = DCM_QUEUE_IDLE;
            }
        }
        SchM_Exit_Dcm_Global_NoNest();
    }

    return isRequestQueued;
}
#endif






#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessSharedPduId
 Syntax           : Dcm_Prv_ProcessSharedPduId(&RxPduIdPtr)
 Description      : This Function is used to update the shared Pduid
 Parameter        : PduIdType*
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessSharedPduId(PduIdType * RxPduIdPtr)
{
    if(((*RxPduIdPtr) < (DCM_CFG_NUM_RX_PDUID-1u)) && ((*RxPduIdPtr) == DCM_CFG_SHARED_RX_PDUID) && \
            (Dcm_isObdRequestReceived_b != FALSE))
    {
        *RxPduIdPtr = (DCM_CFG_NUM_RX_PDUID-1u);
        Dcm_isObdRequestReceived_b = FALSE;
    }
}
#endif






/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessRxIndication
 Syntax           : Dcm_Prv_ProcessRxIndication(DcmRxPduId)
 Description      : This Function is used to Process the request funrther based on the Indication received from the
                    Lower layer
 Parameter        : PduIdType
 Return value     : None
***********************************************************************************************************************/
static void Dcm_Prv_ProcessRxIndication(PduIdType DcmRxPduId)
{
    uint8 idxProtocol_u8  = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8;
    const uint8 * rxBuffer_pu8 = NULL_PTR;

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
    rxBuffer_pu8 = (Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslBufferPtr_pu8);
#else
    rxBuffer_pu8 = Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_mainBuffer_pa;
#endif

    Dcm_Prv_CheckDiagnosticStatus(DcmRxPduId,idxProtocol_u8,rxBuffer_pu8);

    if(FALSE == Dcm_isRequestTobeProcessedByDSP(DcmRxPduId))
    {
        /*This check is for Functional Tester Present which is parallel to the ongoing request*/
        if(FALSE == Dcm_Prv_isFunctionalTesterPresentProcessed(DcmRxPduId))
        {
            if(FALSE != Dcm_Prv_CheckLowPriorityRequestReceived(DcmRxPduId))
            {
                Dcm_Prv_SendNrc21(DcmRxPduId);
            }
        }
    }
    else
    {
#if(DCM_CALLAPPLICATIONONREQRX_ENABLED!=DCM_CFG_OFF)
        (void)DcmAppl_TpRxIndication(DcmRxPduId,E_OK);
#endif
        Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8 = DCM_SERVICEID_DEFAULT_VALUE;

        /*This check is for Functional Tester Present which is not in parallel to the ongoing request*/
        if(FALSE == Dcm_Prv_isFunctionalTesterPresentProcessed(DcmRxPduId))
        {
            Dcm_Prv_ProcessRequest(DcmRxPduId);
        }
    }
}



 /*
***********************************************************************************************************
*  Dcm_TpRxIndication :This function is called by the lower layer (in general the PDU Router):
*  - with Result = E_OK after the complete DCM I-PDU has successfully been received, i.e. at the very
*    end of the segmented TP receive cycle or after receiving an unsegmented N-PDU.
*  - with Result = E_NOT_OK it is indicated that an error (e.g. timeout) has occurred during
*    the reception of the DCM I-PDU. This passes the receive buffer back to DCM and allows error handling.
*    It is undefined which part of the buffer contains valid data in this case, so the DCM shall not evaluate
*    that buffer. By calling this service only the DCM is allowed to access the buffer.
*
*  \param:   id             ID of DCM I-PDU that has been received. Identifies the data that has been received.
*            result         Result of the N-PDU reception:
*                           E_OK if the complete N-PDU has been received.
*                           E_NOT_OK if an error occurred during reception, used to enable
*                           unlocking of the receive buffer.
*  \retval   None
*  \seealso
*  \usedresources
***********************************************************************************************************/
/*TRACE[SWS_Dcm_00093][SWS_Dcm_00345][SWS_Dcm_00111]*/
void Dcm_TpRxIndication (PduIdType id,Std_ReturnType result)
{
    if (id >= DCM_CFG_NUM_RX_PDUID)
    {
        DCM_DET_ERROR(DCM_TPRXINDICATION_ID ,DCM_E_DCMRXPDUID_RANGE_EXCEED)
    }
    else
    {
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2858]*/
#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
        Dcm_Prv_ProcessSharedPduId(&id);
#endif

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
        if(FALSE == Dcm_Prv_isRequestQueued(id,result))
#endif
        {
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
            if(Dcm_Prv_IsRxPduIdOBD(id))
            {
                (E_OK == result)?Dcm_Prv_OBDRxIndication(id):Dcm_Prv_DiscardOBDRequest(id,result);
            }
            else
#endif
            {
                (E_OK == result)?Dcm_Prv_ProcessRxIndication(id):Dcm_Prv_DiscardRequest(id,result);
            }
        }
    }
}


#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
