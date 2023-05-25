
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "DcmCore_DslDsd_Prot.h"
#include "Rte_Dcm.h"
/***********************************************************************************************************************
 *    Function Definitions
 **********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"



/***********************************************************************************************************************
 Function name    : Dcm_Prv_isFunctionalTesterPresentReceived
 Syntax           : Dcm_Prv_isFunctionalTesterPresentReceived(DcmRxPduId)
 Description      : This Inline funtion is used to check whether Functional Tester present request has arrived
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_01168]*/
LOCAL_INLINE boolean Dcm_Prv_isFunctionalTesterPresentReceived(PduIdType DcmRxPduId)
{
    return ((Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslFuncTesterPresent_b) && \
            !(Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b));
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_isValidRequestReceived
 Syntax           : Dcm_Prv_isValidRequestReceived(DcmRxPduId)
 Description      : This Inline funtion is used to check whether a valid request is received and to be processed by DSP
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isValidRequestReceived(PduIdType DcmRxPduId)
{
    return(Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b);
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_isLowPriorityRequestReceived
 Syntax           : Dcm_Prv_isLowPriorityRequestReceived(DcmRxPduId)
 Description      : This Inline Function is used to check whether Low priority request has arrived
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isLowPriorityRequestReceived(PduIdType DcmRxPduId)
{
  return((Dcm_DsldProtocol_pcst[(Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8)].nrc21_b)&& \
            (DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].channel_idx_u8)));
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_CopyDataToRxBuffer
 Syntax           : Dcm_Prv_CopyDataToRxBuffer(DcmRxPduId,&PduInfoPtr,&RxBufferSizePtr)
 Description      : This function is used to perform actual copy operation.
 Parameter        : PduIdType,const PduInfoType*, PduLengthType*
 Return value     : None
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00443]*/
static void Dcm_Prv_CopyDataToRxBuffer(PduIdType DcmRxPduId,
        const PduInfoType * PduInfoPtr,
        PduLengthType * RxBufferSizePtr)
{
    /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object
     * type pointer is converted to void pointer*/
    DCM_MEMCOPY(Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr,PduInfoPtr->SduDataPtr,
            PduInfoPtr->SduLength);

    Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr += PduInfoPtr->SduLength;
    Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength  -= PduInfoPtr->SduLength;

#if(DCM_CALLAPPLICATIONONREQRX_ENABLED!=DCM_CFG_OFF)
    #if(DCM_BUFQUEUE_ENABLED !=DCM_CFG_OFF)
        if(Dcm_QueueStructure_st.Dcm_QueHandling_en == DCM_QUEUE_IDLE)
    #endif
        {
            (void)DcmAppl_CopyRxData(DcmRxPduId,PduInfoPtr->SduLength);
        }
#endif

   *(RxBufferSizePtr) = Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength;
}






/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProvideRxBufferSize
 Syntax           : Dcm_Prv_ProvideRxBufferSize(DcmRxPduId,&RxBufferSizePtr)
 Description      : This function is used to Provide available Rx buffer size
 Parameter        : PduIdType, PduLengthType*
 Return value     : none
***********************************************************************************************************************/
static void Dcm_Prv_ProvideRxBufferSize(PduIdType DcmRxPduId,
        PduLengthType * RxBufferSizePtr)
{
    uint8 idxProtocol_u8  = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8;

    if (FALSE !=  Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b)
    {
       /*Dcm needs to update to the underlying TP on the no. of remaining bytes left */
       *(RxBufferSizePtr) = Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength;
    }
    else
    {
       /* Simulating reception without copying, thus update to TP that Buffer is available for any length.
          The available Rx buffer is updated in this case. */
       *(RxBufferSizePtr) = (PduLengthType)(Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_buffer_size_u32);
    }
}






/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessCopyRxData
 Syntax           : Dcm_Prv_ProcessCopyRxData(DcmRxPduId,&PduInfoPtr,&RxBufferSizePtr)
 Description      : This API is used to process request copy operation
 Parameter        : PduIdType,const PduInfoType*, PduLengthType*
 Return value     : BufReq_ReturnType
***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_ProcessCopyRxData(PduIdType DcmRxPduId,
        const PduInfoType * PduInfoPtr,
        PduLengthType * RxBufferSizePtr)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;

    if (FALSE != Dcm_Prv_isValidRequestReceived(DcmRxPduId))
    {
        Dcm_Prv_CopyDataToRxBuffer(DcmRxPduId,PduInfoPtr,RxBufferSizePtr);
        bufRequestStatus_en = BUFREQ_OK;
    }
    else
    {
        if((FALSE != Dcm_Prv_isLowPriorityRequestReceived(DcmRxPduId))&&(TRUE != Dcm_LowPrioReject_b))
        {
            /*Save ServiceId For NRC21 Handling In TxTpConfirmation*/
            SchM_Enter_Dcm_Global_NoNest();
            if (Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8 == DCM_SERVICEID_DEFAULT_VALUE)
            {
                Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8 = (uint8)PduInfoPtr->SduDataPtr[0];
            }
            SchM_Exit_Dcm_Global_NoNest();

            bufRequestStatus_en = BUFREQ_OK;
        }
        /* Low prio request reception is in progress and a new high prio request arrived*/
        if(TRUE == Dcm_LowPrioReject_b)
        {
            /* To reject the on going low prio request silently */
            bufRequestStatus_en = BUFREQ_E_NOT_OK;
            /* Reset the flag */
            Dcm_LowPrioReject_b = FALSE;
        }
     }

    return bufRequestStatus_en;
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_CopyRxData_CheckEnvironment
 Syntax           : Dcm_Prv_CopyRxData_CheckEnvironment(DcmRxPduId,&PduInfoPtr,&RxBufferSizePtr)
 Description      : This API is used to validate all parameters passed to Dcm_CopyRxData
 Parameter        : PduIdType,const PduInfoType*, const PduLengthType*
 Return value     : boolean
***********************************************************************************************************************/
static boolean Dcm_Prv_CopyRxData_CheckEnvironment(PduIdType DcmRxPduId,
        const PduInfoType * PduInfoPtr,
        const PduLengthType * RxBufferSizePtr)
{
    boolean environmentStatus_b = FALSE;

    if (DcmRxPduId >= DCM_CFG_NUM_RX_PDUID)
    {
        DCM_DET_ERROR(DCM_COPYRXDATA_ID , DCM_E_DCMRXPDUID_RANGE_EXCEED)
    }
    else if ((PduInfoPtr == NULL_PTR) || (RxBufferSizePtr == NULL_PTR))
    {
        DCM_DET_ERROR(DCM_COPYRXDATA_ID , DCM_E_PARAM_POINTER)
    }
    else if( (PduInfoPtr->SduLength != 0u) && (PduInfoPtr->SduDataPtr == NULL_PTR))
    {
        DCM_DET_ERROR(DCM_COPYRXDATA_ID , DCM_E_PARAM_POINTER)
    }
    else
    {
        environmentStatus_b = TRUE;
    }

    return environmentStatus_b;
}


#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProvideOBDRxBufferSize
 Description      : This API is used to provide available buffer size for an OBD protocol
 Parameter        : PduIdType,PduLengthType*
 Return value     : void
***********************************************************************************************************************/
static void Dcm_Prv_ProvideOBDRxBufferSize(PduIdType DcmRxPduId,PduLengthType* RxBufferSizePtr)
{
    uint8 idxProtocol_u8;

    if (FALSE !=  Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b)
    {
       /* Dcm needs to update to the underlying TP on the no. of remaining bytes left */
       *(RxBufferSizePtr) = Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength;
    }
    else
    {
       /* Simulating reception without copying, thus update to TP that Buffer is available for any length.
          The available Rx buffer is updated in this case. */
        idxProtocol_u8 = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[DcmRxPduId]].protocol_num_u8;
       *(RxBufferSizePtr) = (PduLengthType)(Dcm_DsldProtocol_pcst[idxProtocol_u8].rx_buffer_size_u32);
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_CopyOBDDataToRxBuffer
 Description      : This API is used to copy the received data into OBD buffer
 Parameter        : PduIdType,const PduInfoType*, PduLengthType*
 Return value     : void
***********************************************************************************************************************/
static void Dcm_Prv_CopyOBDDataToRxBuffer(PduIdType DcmRxPduId, const PduInfoType* PduInfoPtr,
                                                      PduLengthType* RxBufferSizePtr)
{
    /* MR12 DIR 1.1 VIOLATION: This is required for implementation as DCM_MEMCOPY takes void pointer as input
     * and object type pointer is converted to void pointer */
    DCM_MEMCOPY(Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr,PduInfoPtr->SduDataPtr,
                PduInfoPtr->SduLength);
    Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduDataPtr += PduInfoPtr->SduLength;
    Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength  -= PduInfoPtr->SduLength;

#if(DCM_CALLAPPLICATIONONREQRX_ENABLED!=DCM_CFG_OFF)
   (void)DcmAppl_CopyRxData(DcmRxPduId,PduInfoPtr->SduLength);
#endif
   *(RxBufferSizePtr) = Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength;
}

/***********************************************************************************************************************
 Function name  : Dcm_Prv_CheckOBDRxData
 Description    : Based on RxPduId, the request is segregated to:
                     - Normal CopyRxData - here, store the the Rx data into Dcm buffer
                     - NRC 21 request, simulate pseudo reception
                     - else reject the request
 Parameter      : PduIdType, const PduInfoType*, const PduLengthType*
 Return value   : BufReq_ReturnType
***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_CheckOBDRxData(PduIdType DcmRxPduId, const PduInfoType* PduInfoPtr,
                                                      PduLengthType* RxBufferSizePtr)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;

    if (FALSE != Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b)
    {
        Dcm_Prv_CopyOBDDataToRxBuffer(DcmRxPduId,PduInfoPtr,RxBufferSizePtr);
        bufRequestStatus_en = BUFREQ_OK;
    }
    else
    {
        if(FALSE != Dcm_Prv_isLowPriorityRequestReceived(DcmRxPduId))
        {
            /* Save ServiceId For NRC21 Handling In TpTxConfirmation */
            SchM_Enter_Dcm_Global_NoNest();
            if (Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8 == DCM_SERVICEID_DEFAULT_VALUE)
            {
                Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslServiceId_u8 = (uint8)PduInfoPtr->SduDataPtr[0];
            }
            SchM_Exit_Dcm_Global_NoNest();
            bufRequestStatus_en = BUFREQ_OK;
        }
    }
    return bufRequestStatus_en;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDCopyRxData
 Description      : This API is used to process CopyRx Data for an OBD request reception
 Parameter        : PduIdType,const PduInfoType*,PduLengthType*
 Return value     : BufReq_ReturnType
***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_OBDCopyRxData(PduIdType DcmRxPduId,
       const PduInfoType* PduInfoPtr, PduLengthType* RxBufferSizePtr)
{
    BufReq_ReturnType Result = BUFREQ_E_NOT_OK;
    if(PduInfoPtr->SduLength == 0u)
    {
        Dcm_Prv_ProvideOBDRxBufferSize(DcmRxPduId,RxBufferSizePtr);
        Result = BUFREQ_OK;
    }
    else
    {
        if((PduInfoPtr->SduLength <= Dcm_DslOBDRxPduArray_ast[DcmRxPduId].Dcm_DslRxPduBuffer_st.SduLength) ||
                (Dcm_Prv_isLowPriorityRequestReceived(DcmRxPduId)))
        {
            Result = Dcm_Prv_CheckOBDRxData(DcmRxPduId,PduInfoPtr,RxBufferSizePtr);
        }
        else
        {
            DCM_DET_ERROR(DCM_COPYRXDATA_ID , DCM_E_INTERFACE_BUFFER_OVERFLOW)
        }
    }
    return Result;
}

#endif /*  (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF) */



/***********************************************************************************************************************
 Function name    : Dcm_CopyRxData
 Syntax           : Dcm_CopyRxData(id,&PduInfoPtr,&bufferSizePtr)
 Description      : This Call back API is invoked by the lower layer to copy the request in DCM
 Parameter        : PduIdType,const PduInfoType*, PduLengthType*
 Return value     : BufReq_ReturnType
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00556][SWS_Dcm_00831]*/
BufReq_ReturnType Dcm_CopyRxData( PduIdType id,
    const PduInfoType * PduInfoPtr,
    PduLengthType * bufferSizePtr)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;

    if(FALSE != Dcm_Prv_CopyRxData_CheckEnvironment(id,PduInfoPtr,bufferSizePtr))
    {
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2858]*/
#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
        if ((NULL_PTR != PduInfoPtr->SduDataPtr) && (FALSE != Dcm_isObdRequestReceived_b))
        {
            if(FALSE != Dcm_Prv_isRxPduShared(id,PduInfoPtr->SduDataPtr[0]))
            {
                id = (DCM_CFG_NUM_RX_PDUID-1u);
            }
        }
#endif
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
        if(Dcm_Prv_IsRxPduIdOBD(id))
        {
            bufRequestStatus_en = Dcm_Prv_OBDCopyRxData(id,PduInfoPtr,bufferSizePtr);
        }
        else
#endif
        {
            /*TRACE[SWS_Dcm_00996]*/
            if(PduInfoPtr->SduLength == 0u)
            {
                Dcm_Prv_ProvideRxBufferSize(id,bufferSizePtr);
                bufRequestStatus_en = BUFREQ_OK;
            }
            else if(FALSE != Dcm_Prv_isFunctionalTesterPresentReceived(id))
            {
                bufRequestStatus_en = BUFREQ_OK;
            }
            else
            {
                /* Accept the request if Data to be copied is less than the buffer size available OR Low priority
                   Request has arrived */
                if((PduInfoPtr->SduLength <= Dcm_DslRxPduArray_ast[id].Dcm_DslRxPduBuffer_st.SduLength) || \
                        (FALSE != Dcm_Prv_isLowPriorityRequestReceived(id)))
                {
                    bufRequestStatus_en = Dcm_Prv_ProcessCopyRxData(id,PduInfoPtr,bufferSizePtr);
                }
                else
                {
                    DCM_DET_ERROR(DCM_COPYRXDATA_ID , DCM_E_INTERFACE_BUFFER_OVERFLOW )
                }
            }
        }
    }
    return (bufRequestStatus_en);
}

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
