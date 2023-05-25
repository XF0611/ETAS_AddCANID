

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
 * Defines/Macros
 **********************************************************************************************************************
 */
#define DCM_PROCESSREQUEST_WHILE_DSL_FREE            (0x00u)
#define DCM_PROCESSREQUEST_WHILE_DSL_BUSY            (0x01u)
#define DCM_PROCESSREQUEST_PRIORITYCHECK             (0x02u)
#define DCM_PROCESS_NO_REQUEST                       (0x03u)
#define DCM_NUMBER_OF_REQUEST_TYPE                   (0x04u)



/***********************************************************************************************************************
 *    Internal Type definitions
 **********************************************************************************************************************
 */
#define Dcm_Prv_RxProcessRequest(DcmRxPduId,infoPtr,TpSduLength,RxBufferSizePtr)  \
        Dcm_ProcessRequest[Dcm_Prv_ValidateRequestType(DcmRxPduId,infoPtr,TpSduLength)](DcmRxPduId,infoPtr,TpSduLength,\
                RxBufferSizePtr)

typedef BufReq_ReturnType (*Dcm_ProcessRequestType)(PduIdType DcmRxPduId,\
        const PduInfoType * infoPtr,PduLengthType TpSduLength,\
        PduLengthType * RxBufferSizePtr);

static BufReq_ReturnType Dcm_Prv_ProcessRequestWhileDslFree(
        PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,
        PduLengthType TpSduLength,
        PduLengthType * RxBufferSizePtr);

static BufReq_ReturnType Dcm_Prv_ProcessRequestWhileDslBusy(
        PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,
        PduLengthType TpSduLength,
        PduLengthType * RxBufferSizePtr);


static BufReq_ReturnType Dcm_Prv_ProcessRequest_CheckPriority(
        PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,
        PduLengthType TpSduLength,
        PduLengthType * RxBufferSizePtr);

static BufReq_ReturnType Dcm_Prv_ProcessNoRequest(
        PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,
        PduLengthType TpSduLength,
        PduLengthType * RxBufferSizePtr);



/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define DCM_START_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"
static const Dcm_ProcessRequestType  Dcm_ProcessRequest[DCM_NUMBER_OF_REQUEST_TYPE] =
{
        &Dcm_Prv_ProcessRequestWhileDslFree,
        &Dcm_Prv_ProcessRequestWhileDslBusy,
        &Dcm_Prv_ProcessRequest_CheckPriority,
        &Dcm_Prv_ProcessNoRequest
};

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_INIT_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
boolean Dcm_LowPrioReject_b = FALSE;
#define DCM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_DslRxPduArray_tst Dcm_DslRxPduArray_ast[DCM_CFG_NUM_RX_PDUID];

// RxPduArray which is used exclusively for only OBD requests when parallel processing is used
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
Dcm_DslOBDRxPduArray_tst Dcm_DslOBDRxPduArray_ast[DCM_CFG_NUM_RX_PDUID];
#endif

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
boolean Dcm_isFuncTPOnOtherConnection_b;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"

#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
boolean Dcm_isObdRequestReceived_b;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#endif




/***********************************************************************************************************************
 *    Inline Function Definitions
 **********************************************************************************************************************/

#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isProtocolSupprortedInActiveConfig
 Syntax           : Dcm_Prv_isProtocolSupprortedInActiveConfig(&adrArrivedProt_pcst)
 Description      : This INLINE API is used to check new protocol is supported in active configuration.
 Parameter        : const Dcm_Dsld_protocol_tableType*
 Return value     : boolean
 ***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isProtocolSupprortedInActiveConfig(
        const Dcm_Dsld_protocol_tableType * adrArrivedProt_pcst)
{
    return((adrArrivedProt_pcst->Config_Mask) & (Dcm_ActiveConfiguration_u8) != 0u);
}
#endif




/***********************************************************************************************************************
 Function name    : Dcm_Prv_isDslStateFree
 Syntax           : Dcm_Prv_isDslStateFree(void)
 Description      : This INLINE API is used to check whether DSL is free to accept new request.
 Parameter        : None
 Return value     : boolean
 ***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isDslStateFree(void)
{
    return ((Dcm_Prv_GetDslState() == DSL_STATE_IDLE)
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
            &&      \
            ((Dcm_Prv_GetDslPreemptionState()   != DSL_SUBSTATE_STOP_PROTOCOL_RECEIVING) \
                    && (Dcm_Prv_GetDslPreemptionState() != DSL_SUBSTATE_STOP_PROTOCOL))
#endif
    );
}



/***********************************************************************************************************************
 Function name    : Dcm_Prv_isDcmInitalised
 Syntax           : Dcm_Prv_isDcmInitalised(void)
 Description      : This INLINE API is used to check whether DCM is initilaise or not and ready to accept new request.
 Parameter        : None
 Return value     : boolean
 ***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isDcmInitalised(void)
{
    return ((Dcm_isInitialised_b != FALSE) && (Dcm_acceptRequests_b != FALSE));
}




#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isRxQueueFree
 Syntax           : Dcm_Prv_isRxQueueFree(ConnectionId,ReturnValue_en)
 Description      : This INLINE API is used to check whether DCM Queue is free to accept new request.
 Parameter        : PduIdType, BufReq_ReturnType
 Return value     : boolean
 ***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isRxQueueFree(
        PduIdType ConnectionId,
        BufReq_ReturnType ReturnValue_en)
{
    return ((Dcm_QueueStructure_st.Dcm_QueHandling_en == DCM_QUEUE_IDLE) && \
            (ReturnValue_en != BUFREQ_E_OVFL)                            && \
            (Dcm_DsldGlobal_st.nrActiveConn_u8 == ConnectionId)          && \
            (Dcm_Prv_GetDslState()    != DSL_STATE_WAITING_FOR_RXINDICATION));
}
#endif






#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isDcmProcessingRoeEvent
 Syntax           : Dcm_Prv_isDcmProcessingRoeEvent(idxProtocol_u8,connectionId_u8)
 Description      : This INLINE API is used to check whether DCM is handling any RoE event
 Parameter        : uint8, uint8
 Return value     : boolean
 ***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isDcmProcessingRoeEvent(uint8 idxProtocol_u8,
        uint8 connectionId_u8)
{
    boolean isRoeEventOn_b = FALSE;

    if(Dcm_Prv_GetDslState() == DSL_STATE_ROETYPE1_RECEIVED)
    {
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
        (void)connectionId_u8;

        if((Dcm_DsldProtocol_pcst[idxProtocol_u8].premption_level_u8 == \
                Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].premption_level_u8))
#else
            (void)idxProtocol_u8;
        if(Dcm_DsldGlobal_st.nrActiveConn_u8 == connectionId_u8)
#endif
        {
            isRoeEventOn_b = TRUE;
        }
    }

    return isRoeEventOn_b;
}
#endif




/***********************************************************************************************************************
 *    Function Definitions
 **********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ReceiveNewReqWhenOBDIdle
 Description      : The request details are stored in Global variables in this function for future use.
                    OBD StateMachine is updated to Request Receiving state to not allow other OBD requests.
 Parameter        : PduIdType, const PduInfoType*, PduLengthType, const uint8
 Return value     : void
 ***********************************************************************************************************************/
static void Dcm_Prv_ReceiveNewReqWhenOBDIdle(PduIdType DcmRxPduId,
        const PduInfoType* info,PduLengthType TpSduLength,const uint8 idxProtocol_u8)
{
    uint8 connectionId_u8 = Dcm_DsldRxTable_pcu8[DcmRxPduId];

    Dcm_Prv_SetOBDState((DCM_OBD_REQUESTRECEIVING));
    /* If the Protocol is not yet started update here
     * It will started in OBD StateMachine when the request is completely received */
    if(Dcm_OBDGlobal_st.idxCurrentProtocol_u8 != idxProtocol_u8)
    {
        Dcm_OBDGlobal_st.flgCommActive_b    = FALSE;
    }
    Dcm_OBDGlobal_st.idxCurrentProtocol_u8  = idxProtocol_u8;
    Dcm_OBDGlobal_st.nrActiveConn_u8        = connectionId_u8;
    Dcm_OBDGlobal_st.dataActiveRxPduId_u8   = DcmRxPduId;
    Dcm_OBDGlobal_st.dataActiveTxPduId_u8   = Dcm_DsldConnTable_pcst[connectionId_u8].txpduid_num_u8;
    Dcm_OBDGlobal_st.dataRequestLength_u16  = TpSduLength;
    Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b = TRUE;
    Dcm_Prv_ResetOBDCopyRxDataStatus(DcmRxPduId);
    Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength = TpSduLength;
    Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr =
            Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_mainBuffer_pa;
#if(DCM_CALLAPPLICATIONONREQRX_ENABLED!=DCM_CFG_OFF)
    (void)DcmAppl_StartOfReception(info->SduDataPtr[0],DcmRxPduId,TpSduLength,
            (Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr));
#else
    (void)info;
#endif
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessNewObdRequest
 Description      : The Received OBD request is further checked
                     - If OBD Specific State Machine is free, then allow reception of the request.
                     - If State Machine is Busy, then check whether NRC21 is needed and return accordingly.
 Parameter        : PduIdType, const PduInfoType*, PduLengthType, PduLengthType*
 Return value     : BufReq_ReturnType
 ***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_ProcessNewObdRequest(PduIdType DcmRxPduId,
        const PduInfoType* info,PduLengthType TpSduLength,PduLengthType* RxBufferSizePtr)
{
    BufReq_ReturnType Result = BUFREQ_OK;
    uint8 idxProtocol_u8 = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8;

    if(DCM_OBD_IDLE == Dcm_Prv_GetOBDState())
    {
        Dcm_Prv_ReceiveNewReqWhenOBDIdle(DcmRxPduId,info,TpSduLength,idxProtocol_u8);
    }
    else
    {
        Result = (FALSE != Dcm_DsldProtocol_pcst[idxProtocol_u8].nrc21_b)?BUFREQ_OK:BUFREQ_E_NOT_OK;
    }

    if(BUFREQ_OK == Result)
    {
        *(RxBufferSizePtr) = (PduLengthType)(Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_buffer_size_u32);
    }
    /* For request on Shared Id , update Obd Request received Flag to TRUE */
#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
    if( (Result == BUFREQ_OK) && (DcmRxPduId == (DCM_CFG_NUM_RX_PDUID-1u)))
    {
        Dcm_isObdRequestReceived_b = TRUE;
    }
#endif
    return Result;
}

#endif  /* (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF) */

#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_PremptionHandling
 Syntax           : Dcm_Prv_PremptionHandling(&ArrivedProtocol_pcst,TpSduLength,RxpduId)
 Description      : This API is used to handle protocol preemption fnctionality
 Parameter        : const Dcm_Dsld_protocol_tableType*, PduLengthType, PduIdType
 Return value     : BufReq_ReturnType
 ***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_PremptionHandling(
        const Dcm_Dsld_protocol_tableType * ArrivedProtocol_pcst,
        PduLengthType TpSduLength,PduIdType RxpduId)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;
    uint8 idxProtocol_u8  = Dcm_DsldGlobal_st.idxCurrentProtocol_u8;
    uint8 connectionId_u8 = 0u;

    if(ArrivedProtocol_pcst->rx_buffer_size_u32 < (uint32)TpSduLength)
    {
        bufRequestStatus_en = BUFREQ_E_OVFL;
    }
    else
    {
        bufRequestStatus_en = BUFREQ_OK;

        /* Check if already high priority protocol request is under reception or not.
           If yes then copy protocol index. */
        if(0xFF != Dcm_DsldGlobal_st.dataNewRxPduId_u8)
        {
            /* Connection on which First high Priority request was received */
            connectionId_u8 = Dcm_DsldRxTable_pcu8[Dcm_DsldGlobal_st.dataNewRxPduId_u8];
            idxProtocol_u8  = Dcm_DsldConnTable_pcst[connectionId_u8].protocol_num_u8;
        }


        if(ArrivedProtocol_pcst->premption_level_u8 < Dcm_DsldProtocol_pcst[idxProtocol_u8].premption_level_u8)
        {
            Dcm_DsldGlobal_st.dataNewRxPduId_u8             = RxpduId;
            Dcm_DsldGlobal_st.dataNewdataRequestLength_u16  = TpSduLength;

            if(Dcm_Prv_GetDslState() == DSL_STATE_WAITING_FOR_RXINDICATION)
            {
               /* Set the flag to reject the ongoing low priorty request reception,
                * when a high priority request is arrived */
               Dcm_LowPrioReject_b = TRUE;
            }

            /*Update the pre-emption state to DSL_SUBSTATE_STOP_PROTOCOL_RECEIVING, the same state will be moved to
             * DSL_STOP_PROTOCOL when the request is fully received i.e., on a positive RxIndication*/
            Dcm_Prv_SetDslPreemptionState((DSL_SUBSTATE_STOP_PROTOCOL_RECEIVING));
            /*Update the DSL statemachine to DSL_STATE_WAITING_FOR_RXINDICATION as request is not completely received and Rxindication is not arrived*/
            Dcm_Prv_SetDslState((DSL_STATE_WAITING_FOR_RXINDICATION));
            Dcm_DslRxPduArray_ast[RxpduId].Dcm_DslCopyRxData_b  = TRUE;
            Dcm_Prv_ResetCopyRxDataStatus(RxpduId);
        }
        else
        {
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3355]*/
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3356]*/
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3422]*/
            if (FALSE == ArrivedProtocol_pcst->nrc21_b)
            {
                bufRequestStatus_en = BUFREQ_E_NOT_OK;
            }
        }
    }
    return bufRequestStatus_en;
}
#endif





/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckFunctionalTesterPresent
 Syntax           : Dcm_Prv_CheckFunctionalTesterPresent(DcmRxPduId,&infoPtr,TpSduLength)
 Description      : This API is used to check whether the received request is Functional Tester Present.
 Parameter        : PduIdType, const PduInfoType*, PduLengthType
 Return value     : boolean
 ***********************************************************************************************************************/
/*TRACE[SWS_Dcm_01168]*/
static boolean Dcm_Prv_CheckFunctionalTesterPresent( PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,
        PduLengthType TpSduLength)
{
    boolean isFuncTesterPresent_b = FALSE;

    if(infoPtr != NULL_PTR)
    {
        if((DcmRxPduId    >= DCM_INDEX_FUNC_RX_PDUID)                  && \
                (TpSduLength  == DCM_DSLD_PARALLEL_DCM_TPR_REQ_LENGTH)     && \
                (infoPtr->SduDataPtr[0] == DCM_DSLD_PARALLEL_TPR_BYTE1)    && \
                (infoPtr->SduDataPtr[1] == DCM_DSLD_PARALLEL_TPR_BYTE2))
        {
            isFuncTesterPresent_b = TRUE;
        }
    }
    return isFuncTesterPresent_b;
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_InformApplication
 Syntax           : Dcm_Prv_InformApplication(DcmRxPduId,&info,TpSduLength,&RxBufferSizePtr)
 Description      : This API is used to process a valid request and inform to the application.
 Parameter        : PduIdType,const PduInfoType*,PduLengthType,PduLengthType,PduLengthType*
 Return value     : BufReq_ReturnType
 ***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_InformApplication(PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,PduLengthType TpSduLength,
        PduLengthType * RxBufferSizePtr,
        BufReq_ReturnType BufRequestStatus_en)
{
    uint8 connectionId_u8 = Dcm_DsldRxTable_pcu8[DcmRxPduId];
    uint8 idxProtocol_u8  = Dcm_DsldConnTable_pcst[connectionId_u8].protocol_num_u8;

    if(BUFREQ_OK == BufRequestStatus_en)
    {
        *(RxBufferSizePtr) = (PduLengthType)(Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_buffer_size_u32);

        if (FALSE != Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b)
        {
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength  = TpSduLength;

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslBufferPtr_pu8 = Dcm_Prv_ProvideFreeBuffer(idxProtocol_u8,FALSE);
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr = Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslBufferPtr_pu8;
#else
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr = Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_mainBuffer_pa;
#endif

#if(DCM_CALLAPPLICATIONONREQRX_ENABLED!=DCM_CFG_OFF)
            (void)DcmAppl_StartOfReception(infoPtr->SduDataPtr[0],DcmRxPduId,TpSduLength,(Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr));
#else
            (void)infoPtr;
#endif
        }

#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
        if(DcmRxPduId == (DCM_CFG_NUM_RX_PDUID-1u))
        {
            Dcm_isObdRequestReceived_b = TRUE;
        }
#endif
    }
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
    else
    {
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2859]*/
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2854]*/
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2848]*/
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2846]*/
        if(FALSE != Dcm_Prv_isRxQueueFree(connectionId_u8,BufRequestStatus_en))
        {
            SchM_Enter_Dcm_Global_NoNest();

            /* copy the buffer address into the queue buffer pointer */
            Dcm_QueueStructure_st.adrBufferPtr_pu8 = Dcm_Prv_ProvideFreeBuffer(idxProtocol_u8,TRUE);
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr = Dcm_QueueStructure_st.adrBufferPtr_pu8;
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength  = TpSduLength;
            Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b              = TRUE;

            /* Application should not be called for copying the data while being queuing the request */
            Dcm_QueueStructure_st.dataQueueReqLength_u16 = TpSduLength;
            Dcm_QueueStructure_st.dataQueueRxPduId_u8    = DcmRxPduId;

            *(RxBufferSizePtr) = (PduLengthType)(Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_buffer_size_u32);

            /* set the flag to indicate that the first buffer is busy */
            /* To indicate that the Queueing of the next request on the same connection has started on buffer 0*/
            Dcm_QueueStructure_st.Dcm_QueHandling_en = DCM_QUEUE_RUNNING;
            BufRequestStatus_en = BUFREQ_OK;

            SchM_Exit_Dcm_Global_NoNest();
        }
    }
#endif

    return BufRequestStatus_en;
}






/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessNoRequest
 Syntax           : Dcm_Prv_ProcessNoRequest(DcmRxPduId,&info,TpSduLength,&RxBufferSizePtr)
 Description      : This API is used to ignore the invalid request received in API Dcm_StartOfReception.
 Parameter        : PduIdType,const PduInfoType*,PduLengthType,PduLengthType*
 Return value     : BufReq_ReturnType
 ***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_ProcessNoRequest( PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,PduLengthType TpSduLength,
        /*MR12 RULE 8.13 VIOLATION:The object addressed by the pointer parameter 'PduInfoPtr' is not modified and so the pointer could be of type 'pointer to const'. MISRA C:2012 Rule-8.13*/
        PduLengthType * RxBufferSizePtr)
{
    (void)DcmRxPduId;
    (void)infoPtr;
    (void)TpSduLength;
    (void)RxBufferSizePtr;

    return BUFREQ_OK;
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessRequest_CheckPriority
 Syntax           : Dcm_Prv_ProcessRequest_CheckPriority(DcmRxPduId,&info,TpSduLength,&RxBufferSizePtr)
 Description      : This API is used to process a new request received in non default session and it belongs to
                    a connection other than the previour request connection.
 Parameter        : PduIdType,const PduInfoType*,PduLengthType,PduLengthType*
 Return value     : BufReq_ReturnType
 ***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_ProcessRequest_CheckPriority( PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,PduLengthType TpSduLength,
        PduLengthType * RxBufferSizePtr)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;
    uint8 connectionId_u8 = Dcm_DsldRxTable_pcu8[DcmRxPduId];
    uint8 idxProtocol_u8  = Dcm_DsldConnTable_pcst[connectionId_u8].protocol_num_u8;

    /*TRACE[SWS_Dcm_01145][SWS_Dcm_01146][SWS_Dcm_01144]*/
    if(FALSE != Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b)
    {
        bufRequestStatus_en = BUFREQ_OK;
    }
    else
    {
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
        bufRequestStatus_en = Dcm_Prv_PremptionHandling(&Dcm_DsldProtocol_pcst[idxProtocol_u8],TpSduLength,DcmRxPduId);
#else
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3355]*/
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3356]*/
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3422]*/
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3358]*/
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3360]*/
        bufRequestStatus_en = (FALSE != Dcm_DsldProtocol_pcst[idxProtocol_u8].nrc21_b)? BUFREQ_OK : BUFREQ_E_NOT_OK;
#endif
    }

    bufRequestStatus_en = Dcm_Prv_InformApplication(DcmRxPduId,infoPtr,TpSduLength,RxBufferSizePtr,bufRequestStatus_en);

    return bufRequestStatus_en;
}






#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessRequestWhileRoeEvent
 Syntax           : Dcm_Prv_ProcessRequestWhileRoeEvent(DcmRxPduId,TpSduLength)
 Description      : This API is used to process the new request received in API Dcm_StartOfReception while
                    DCM is busy processing a RoE event.
 Parameter        : PduIdType,PduLengthType
 Return value     : BufReq_ReturnType
 ***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_ProcessRequestWhileRoeEvent(
        PduIdType DcmRxPduId,PduLengthType TpSduLength)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;
    uint8 connectionId_u8  = Dcm_DsldRxTable_pcu8[DcmRxPduId];
    uint8 idxProtocol_u8   = Dcm_DsldConnTable_pcst[connectionId_u8].protocol_num_u8;

#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    if(Dcm_DsldProtocol_pcst[idxProtocol_u8].premption_level_u8 == \
            Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].premption_level_u8)
    {
        /* Check for high priority request on persistent ROE*/
        bufRequestStatus_en = Dcm_DsldPersistentRoeHandling_en(&Dcm_DsldProtocol_pcst[idxProtocol_u8],TpSduLength,DcmRxPduId);

        if ((bufRequestStatus_en == BUFREQ_E_NOT_OK) && (Dcm_DsldProtocol_pcst[idxProtocol_u8].nrc21_b != FALSE))
        {
            bufRequestStatus_en = BUFREQ_OK;
        }
    }
#else
    if(Dcm_DsldGlobal_st.nrActiveConn_u8 == connectionId_u8)
    {
        /* ROE type1 request under processing and tester request comes. Reset S3 timer */
        Dcm_Prv_ReloadS3Timer();

        bufRequestStatus_en = (FALSE != Dcm_DsldProtocol_pcst[idxProtocol_u8].nrc21_b) ? BUFREQ_OK : BUFREQ_E_NOT_OK;
    }
#endif

    return bufRequestStatus_en;
}
#endif






/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessRequestWhileDslBusy
 Syntax           : Dcm_Prv_ProcessRequestWhileDslBusy(DcmRxPduId,&info,TpSduLength,&RxBufferSizePtr)
 Description      : This API is used to process the new request received in API Dcm_StartOfReception while
                    DSL state is busy processing a previous request.
 Parameter        : PduIdType,const PduInfoType*,PduLengthType,PduLengthType*
 Return value     : BufReq_ReturnType
 ***********************************************************************************************************************/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2862]*/
static BufReq_ReturnType Dcm_Prv_ProcessRequestWhileDslBusy( PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,PduLengthType TpSduLength,
        PduLengthType * RxBufferSizePtr)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;
    uint8 connectionId_u8  = Dcm_DsldRxTable_pcu8[DcmRxPduId];
    uint8 idxProtocol_u8   = Dcm_DsldConnTable_pcst[connectionId_u8].protocol_num_u8;

    /*TRACE[SWS_Dcm_00557][SWS_Dcm_01144]*/
    if(FALSE != Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b)
    {
        bufRequestStatus_en = BUFREQ_OK;
    }
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    else if(FALSE != Dcm_Prv_isDcmProcessingRoeEvent(idxProtocol_u8,connectionId_u8))
    {
        bufRequestStatus_en = Dcm_Prv_ProcessRequestWhileRoeEvent(DcmRxPduId,TpSduLength);
    }
#endif
    else
    {
        if(Dcm_DsldGlobal_st.nrActiveConn_u8 != connectionId_u8)
        {
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
            bufRequestStatus_en = Dcm_Prv_PremptionHandling(&Dcm_DsldProtocol_pcst[idxProtocol_u8],TpSduLength,DcmRxPduId);
#else
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3355]*/
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3356]*/
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3422]*/
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3359]*/
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3361]*/
            bufRequestStatus_en = (FALSE != Dcm_DsldProtocol_pcst[idxProtocol_u8].nrc21_b) ? BUFREQ_OK : BUFREQ_E_NOT_OK;
#endif
        }
    }

    bufRequestStatus_en = Dcm_Prv_InformApplication(DcmRxPduId,infoPtr,TpSduLength,RxBufferSizePtr,bufRequestStatus_en);

    return bufRequestStatus_en;
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessRequestWhileDslFree
 Syntax           : Dcm_Prv_ProcessRequestWhileDslFree(DcmRxPduId,&info,TpSduLength,&RxBufferSizePtr)
 Description      : This API is used to process the new request received in API Dcm_StartOfReception while DSL state is
                    Free to accept new Request.
 Parameter        : PduIdType, const PduInfoType*,PduLengthType,PduLengthType*
 Return value     : BufReq_ReturnType
 ***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_ProcessRequestWhileDslFree( PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,PduLengthType TpSduLength,
        PduLengthType * RxBufferSizePtr)
{

    uint8 connectionId_u8 = Dcm_DsldRxTable_pcu8[DcmRxPduId];
    uint8 idxProtocol_u8  = Dcm_DsldConnTable_pcst[connectionId_u8].protocol_num_u8;

    SchM_Enter_Dcm_Global_NoNest();

    Dcm_Prv_SetDslState((DSL_STATE_WAITING_FOR_RXINDICATION));

    if(Dcm_DsldGlobal_st.idxCurrentProtocol_u8 != idxProtocol_u8)
    {
        Dcm_DsldGlobal_st.flgCommActive_b    = FALSE;
    }
    Dcm_DsldGlobal_st.idxCurrentProtocol_u8  = idxProtocol_u8;
    Dcm_DsldGlobal_st.nrActiveConn_u8        = connectionId_u8;
    Dcm_DsldGlobal_st.dataActiveRxPduId_u8   = DcmRxPduId;
    Dcm_DsldGlobal_st.dataActiveTxPduId_u8   = Dcm_DsldConnTable_pcst[connectionId_u8].txpduid_num_u8;
    Dcm_DsldGlobal_st.dataRequestLength_u16  = TpSduLength;
    Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b = TRUE;

    SchM_Exit_Dcm_Global_NoNest();

    Dcm_Prv_ResetCopyRxDataStatus(DcmRxPduId);

    (void)Dcm_Prv_InformApplication(DcmRxPduId,infoPtr,TpSduLength,RxBufferSizePtr,BUFREQ_OK);

    return BUFREQ_OK;
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_ValidateRequestType
 Syntax           : Dcm_Prv_ValidateRequestType(DcmRxPduId,&infoPtr,TpSduLength)
 Description      : This API is used to validate the request received in API Dcm_StartOfReception.
 Parameter        : PduIdType,const PduInfoType*,PduLengthType
 Return value     : uint8
 ***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00241]*/
static uint8 Dcm_Prv_ValidateRequestType( PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,PduLengthType TpSduLength)
{
    uint8 connectionId_u8 = Dcm_DsldRxTable_pcu8[DcmRxPduId];
    uint8 requestType_u8  = DCM_PROCESS_NO_REQUEST;
    Dcm_isFuncTPOnOtherConnection_b      = FALSE;

    if(FALSE != Dcm_Prv_CheckFunctionalTesterPresent(DcmRxPduId,infoPtr,TpSduLength))
    {
        Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b = TRUE;

        if(Dcm_DsldGlobal_st.nrActiveConn_u8 != connectionId_u8)
        {
            Dcm_isFuncTPOnOtherConnection_b = TRUE;
        }
    }

    /*Check if New request is arrived in Default session or on same connection.If yes then proceeed ahead
      otherwise check for protocol priority*/
    if((Dcm_DsldGlobal_st.idxActiveSession_u8 == DCM_DEFAULT_SESSION_IDX) || \
            (Dcm_DsldGlobal_st.nrActiveConn_u8 == connectionId_u8))
    {
        /*TRACE[SWS_Dcm_00342]*/
        if((FALSE != Dcm_Prv_isDslStateFree()) && (FALSE == Dcm_isFuncTPOnOtherConnection_b))
        {
            requestType_u8 = DCM_PROCESSREQUEST_WHILE_DSL_FREE;
        }
        else
        {
            requestType_u8 = DCM_PROCESSREQUEST_WHILE_DSL_BUSY;
        }
    }
    else
    {
        requestType_u8 = DCM_PROCESSREQUEST_PRIORITYCHECK;
    }


    return requestType_u8;
}





#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessSharedRxPduid
 Syntax           : Dcm_Prv_ProcessSharedRxPduid(&RxPduId,&info)
 Description      : This API is used to Check if RxPduid passed in API Dcm_StartOfReception is shared Pduid
                    between protocol UDS and OBD.
 Parameter        : PduIdType*, const PduInfoType*
 Return value     : boolean
 ***********************************************************************************************************************/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2858]*/
static boolean Dcm_Prv_ProcessSharedRxPduid(PduIdType * RxPduIdPtr,
        const PduInfoType * infoPtr)
{
    boolean processStatus_b  = TRUE;
    uint8 idxProtocol_u8  = 0u;
    uint8 protocolId_u8   = 0u;

    if(FALSE != Dcm_Prv_isRxPduShared(*RxPduIdPtr,infoPtr->SduDataPtr[0]))
    {
        idxProtocol_u8   = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DCM_CFG_NUM_RX_PDUID-1u]].protocol_num_u8;
        protocolId_u8    = Dcm_DsldProtocol_pcst[idxProtocol_u8].protocolid_u8;

        if((protocolId_u8 == DCM_OBD_ON_CAN) || (protocolId_u8  == DCM_OBD_ON_FLEXRAY))
        {
            *RxPduIdPtr = (DCM_CFG_NUM_RX_PDUID-1u);
        }
        else
        {
            processStatus_b = FALSE;
        }
    }
    return processStatus_b;
}
#endif





/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckPermissions
 Syntax           : Dcm_Prv_CheckPermissions(&DcmRxPduId,&infoPtr,TpSduLength)
 Description      : This API is used to check all permissions and conditions before processing the service received in
                    API Dcm_StartOfReception.
 Parameter        : PduIdType*, const PduInfoType*,PduLengthType
 Return value     : Std_ReturnType
 ***********************************************************************************************************************/
/*MR12 RULE 8.13 VIOLATION:RxPduIdPtr will be modified in case of RxPduid is shared so cannot make this CONST*/
static Std_ReturnType Dcm_Prv_CheckPermissions(PduIdType * RxPduIdPtr,
        const PduInfoType * infoPtr,PduLengthType TpSduLength)
{
    Std_ReturnType permissionsStatus  = E_NOT_OK;
    uint8 connectionId_u8 = 0u;
    uint8 idxProtocol_u8  = 0u;
    PduIdType rxPduid     = 0u;

    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2858]*/
#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
    if(FALSE != Dcm_Prv_ProcessSharedRxPduid(RxPduIdPtr,infoPtr))
#endif
    {
        rxPduid = *RxPduIdPtr;
        connectionId_u8 = Dcm_DsldRxTable_pcu8[rxPduid];
        idxProtocol_u8  = Dcm_DsldConnTable_pcst[connectionId_u8].protocol_num_u8;

        if(DCM_CHKNOCOMM_MODE(Dcm_DsldConnTable_pcst[connectionId_u8].channel_idx_u8))
        {
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
            if(FALSE != Dcm_Prv_isProtocolSupprortedInActiveConfig(&Dcm_DsldProtocol_pcst[idxProtocol_u8]))
#endif
            {
                if(E_OK == DcmAppl_DcmGetRxPermission(Dcm_DsldProtocol_pcst[idxProtocol_u8].protocolid_u8,\
                        rxPduid,infoPtr,TpSduLength))
                {
                    permissionsStatus = E_OK;
                }
            }
        }
    }
    return permissionsStatus;
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_StartOfReception_CheckEnvironment
 Syntax           : Dcm_Prv_StartOfReception_CheckEnvironment(DcmRxPduId,&infoPtr,&RxBufferSize)
 Description      : This API is used to validate all parameter passed to API Dcm_StartOfReception.
 Parameter        : PduIdType, Const PduInfoType*, const PduLengthType*
 Return value     : boolean
 ***********************************************************************************************************************/
static boolean Dcm_Prv_StartOfReception_CheckEnvironment(PduIdType DcmRxPduId,
        const PduInfoType * infoPtr,
        const PduLengthType * RxBufferSizePtr)
{
    boolean environmentStatus_b = FALSE;

    if(FALSE == Dcm_Prv_isDcmInitalised())
    {
        if(FALSE == Dcm_isInitialised_b)
        {
            DCM_DET_ERROR(DCM_STARTOFRECEPTION_ID , DCM_E_UNINIT);
        }
    }
#if((DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)||(DCM_CALLAPPLICATIONONREQRX_ENABLED != DCM_CFG_OFF))
    else if (infoPtr == NULL_PTR)
    {
        DCM_DET_ERROR(DCM_STARTOFRECEPTION_ID , DCM_E_PARAM_POINTER);
    }
#endif
    else if(DcmRxPduId >= DCM_CFG_NUM_RX_PDUID)
    {
        DCM_DET_ERROR(DCM_STARTOFRECEPTION_ID ,  DCM_E_DCMRXPDUID_RANGE_EXCEED )
    }
    else if(RxBufferSizePtr == NULL_PTR)
    {
        DCM_DET_ERROR(DCM_STARTOFRECEPTION_ID , DCM_E_PARAM_POINTER)
    }
    else
    {
        environmentStatus_b = TRUE;
    }


#if((DCM_CFG_RXPDU_SHARING_ENABLED == DCM_CFG_OFF) && (DCM_CALLAPPLICATIONONREQRX_ENABLED == DCM_CFG_OFF))
    (void)infoPtr;
#endif

    return environmentStatus_b;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_UpdateRxPduArrayForValidRequest
 Description      : Update the appropriate RxPdu Array based on the id received
 Parameter        : PduIdType
 Return value     : void
 ***********************************************************************************************************************/
static void Dcm_Prv_UpdateRxPduArrayForValidRequest(PduIdType id)
{
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    // If the requested RxPduId is configured for an OBD Protocol
    if(Dcm_Prv_IsRxPduIdOBD(id))
    {
        SchM_Enter_Dcm_Global_NoNest();
        Dcm_DslOBDRxPduArray_ast[id].Dcm_DslCopyRxData_b  = FALSE;
        Dcm_DslOBDRxPduArray_ast[id].Dcm_DslServiceId_u8 = DCM_SERVICEID_DEFAULT_VALUE;
        SchM_Exit_Dcm_Global_NoNest();
    }
    else
#endif
    {
        SchM_Enter_Dcm_Global_NoNest();
        Dcm_DslRxPduArray_ast[id].Dcm_DslCopyRxData_b  = FALSE;
        Dcm_DslRxPduArray_ast[id].Dcm_DslServiceId_u8 = DCM_SERVICEID_DEFAULT_VALUE;
        SchM_Exit_Dcm_Global_NoNest();
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_StartOfReception
 Syntax           : Dcm_Prv_StartOfReception(id,&info,TpSduLength,&RxBufferSize)
 Description      : Dcm_StartOfReception : This call-back function is invoked by medium specific TP (CanTp/FrTp)
                    via PduR to inform the start of reception (i.e. receiving a Single Frame or First Frame indication)
 Parameter        : PduIdType, const PduInfoType*, PduLengthType, PduLengthType*
 Return value     : BufReq_ReturnType
 ***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00094][SWS_Dcm_00833]*/
BufReq_ReturnType Dcm_StartOfReception(PduIdType id,
        const PduInfoType * info,PduLengthType TpSduLength,
        PduLengthType * bufferSizePtr)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;
    uint8 idxProtocol_u8    = 0u;

    if (FALSE != Dcm_Prv_StartOfReception_CheckEnvironment(id,info,bufferSizePtr))
    {
        if(E_OK == Dcm_Prv_CheckPermissions(&id,info,TpSduLength))
        {
            idxProtocol_u8 = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[id]].protocol_num_u8;
            Dcm_Prv_UpdateRxPduArrayForValidRequest(id);

            /*TRACE[SWS_Dcm_00642]*/
            if(TpSduLength == 0u)
            {
                *(bufferSizePtr) = (PduLengthType)(Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_buffer_size_u32);
                bufRequestStatus_en = BUFREQ_OK;
            }
            /*TRACE[SWS_Dcm_00444]*/
            else if((Dcm_MsgLenType)TpSduLength > Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_buffer_size_u32)
            {
                bufRequestStatus_en = BUFREQ_E_OVFL;
            }
            else
            {
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
                // For a request on OBD protocol, handle it separately in parallel to other protocol requests.
                if(Dcm_Prv_IsRxPduIdOBD(id))
                {
                    bufRequestStatus_en = Dcm_Prv_ProcessNewObdRequest(id,info,TpSduLength,bufferSizePtr);
                }
                else
#endif
                {
                    bufRequestStatus_en = Dcm_Prv_RxProcessRequest(id,info,TpSduLength,bufferSizePtr);
                }
            }
        }
    }
    return (bufRequestStatus_en);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



