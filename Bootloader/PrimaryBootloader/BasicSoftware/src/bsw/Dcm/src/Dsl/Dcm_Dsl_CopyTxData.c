

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Dcm_Cfg_Prot.h"
#include "rba_BswSrv.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Dcm_Dsd_Prv.h"
#include "Rte_Dcm.h"
/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
static boolean Dcm_isNrc21responseSet_b;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
static boolean Dcm_isOBDNrc21responseSet_b; // Nrc 21 to be sent for an OBD request
#endif
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint8 adrDataPtr_u8[3]; /* Response Data for NRC21 */
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
static uint8 ObdadrDataPtr_u8[3]; /* Response Data for OBD NRC21 */
#endif
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
static PduInfoType Dcm_adrDataPtr_pst;
static PduInfoType * Dcm_PduInfo_pst;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
static PduInfoType Dcm_ObdadrDataPtr_pst;
static PduInfoType* Dcm_ObdPduInfo_pst;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"

/***********************************************************************************************************************
 *    Inline Function Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Function name    : Dcm_Prv_isRetryRequested
 Syntax           : Dcm_Prv_isRetryRequested(&RetryInfoPtr,&PduInfoPtr,&RetValPtr)
 Description      : This Inline function is used to check if retry is requested from lower layer
 Parameter        : const RetryInfoType*,const PduInfoType*,BufReq_ReturnType*
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isRetryRequested(const RetryInfoType * RetryInfoPtr,
        const PduInfoType * PduInfoPtr,BufReq_ReturnType * RetValPtr)
{
    boolean isRetryRequested_b = FALSE;

    if(RetryInfoPtr != NULL_PTR)
    {
        if(RetryInfoPtr->TpDataState == TP_DATARETRY)
        {
            if((PduInfoPtr->SduDataPtr != NULL_PTR) && (RetryInfoPtr->TxTpDataCnt > 0u ))
            {
                isRetryRequested_b   = TRUE;
            }
            else
            {
                *RetValPtr = BUFREQ_E_NOT_OK;
            }
        }
    }

    return isRetryRequested_b;
}





/***********************************************************************************************************************
 Function name    : Dcm_Prv_isNormalResponseAvailable
 Syntax           : Dcm_Prv_isNormalResponseAvailable(DcmTxPduId)
 Description      : This Inline function is used to check whether Normal response is available.
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isNormalResponseAvailable(PduIdType DcmTxPduId)
{
   return((Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId] == Dcm_DsldGlobal_st.dataActiveTxPduId_u8) && \
           ((Dcm_Prv_GetDslState() == DSL_STATE_WAITING_FOR_TXCONFIRMATION)  || \
            (Dcm_Prv_GetDslState() == DSL_STATE_ROETYPE1_RECEIVED))

         );
}




#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isPagedBufferResponseAvailable
 Syntax           : Dcm_Prv_isPagedBufferResponseAvailable(DcmTxPduId)
 Description      : This Inline function is used to check whether Paged buffer response is available
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isPagedBufferResponseAvailable(PduIdType DcmTxPduId)
{
   return((Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId] == Dcm_DsldGlobal_st.dataActiveTxPduId_u8) && \
        (Dcm_Prv_GetDslState() == DSL_STATE_PAGEDBUFFER_TRANSMISSION));
}
#endif





#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isRoeType2ResponseAvailable
 Syntax           : Dcm_Prv_isRoeType2ResponseAvailable(DcmTxPduId)
 Description      : This funcion is used to
 Parameter        : PduIdType
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isRoeType2ResponseAvailable(PduIdType DcmTxPduId)
{
    return (Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_Roe2MesContext_st.dcmRxPduId]].roetype2_txpdu_u8 == \
            Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId]);
}
#endif






#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isCurrentPageTransmitted
 Syntax           : Dcm_Prv_isCurrentPageTransmitted(SduLength,&RetryInfoPtr)
 Description      : This Inline Function is used to check if Current page is Transmitted in case of paged buffer Tx
 Parameter        : PduLengthType,const RetryInfoType*
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isCurrentPageTransmitted(PduLengthType SduLength,
        const RetryInfoType * RetryInfoPtr)
{
    boolean pageStatus_b = FALSE;

    if(SduLength == 0u)
    {
        if(RetryInfoPtr != NULL_PTR)
        {
            if(RetryInfoPtr->TpDataState == TP_DATACONF)
            {
                pageStatus_b = TRUE;
            }
        }
        else
        {
            pageStatus_b = TRUE;
        }
    }

    return pageStatus_b;
}
#endif



/***********************************************************************************************************************
 *    Function Definitions
 **********************************************************************************************************************/
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessPagedBufferResponse
 Syntax           : Dcm_Prv_ProcessPagedBufferResponse(&PduInfoPtr,&RetryInfoPtr)
 Description      : This Function is used to handle Panged buffer states and process the response.
 Parameter        : const PduInfoType*,const RetryInfoType*
 Return value     : BufReq_ReturnType
***********************************************************************************************************************/
/*TRACE[SWS_Dcm_00028]*/
static BufReq_ReturnType Dcm_Prv_ProcessPagedBufferResponse(
        const PduInfoType * PduInfoPtr,
        const RetryInfoType * RetryInfoPtr)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;
    uint8 subStateTemp_u8 = Dcm_Prv_GetDslSubState();

    switch(subStateTemp_u8)
    {
        case DSL_SUBSTATE_DATA_READY :

            /* When the TP requests Dcm to copy payload, check if length indicated to copy is greater than the available
               Dcm response length.
               If yes then send DET error when first Dcm_TpCopyTxData fails for the paged buffer in the first page  */

            if(PduInfoPtr->SduLength <= (PduLengthType)Dcm_DsldGlobal_st.dataCurrentPageRespLength_u32)
            {
                Dcm_DsldPduInfo_st.SduDataPtr = &Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[0];
                Dcm_DsldPduInfo_st.SduLength  = (PduLengthType)Dcm_DsldGlobal_st.dataCurrentPageRespLength_u32;

                Dcm_PduInfo_pst = &Dcm_DsldPduInfo_st;

                Dcm_Prv_SetDslState((DSL_STATE_PAGEDBUFFER_TRANSMISSION));
                Dcm_Prv_SetDslSubState((DSL_SUBSTATE_WAIT_PAGE_TXCONFIRM));
                bufRequestStatus_en = BUFREQ_OK;
            }
            else
            {
                bufRequestStatus_en = BUFREQ_E_NOT_OK;
                DCM_DET_ERROR(DCM_COPYTXDATA_ID , DCM_E_INVALID_LENGTH )
            }
            break;


        /*TRACE[SWS_Dcm_01186]*/
        case DSL_SUBSTATE_WAIT_FOR_DATA :

            bufRequestStatus_en = BUFREQ_E_BUSY;
            break;



        case DSL_SUBSTATE_WAIT_PAGE_TXCONFIRM :

            /* Current page transmission is over. Give the page back to service to fill next page */
            if(FALSE != Dcm_Prv_isCurrentPageTransmitted(Dcm_DsldPduInfo_st.SduLength,RetryInfoPtr))
            {
                Dcm_Prv_SetDslState((DSL_STATE_PAGEDBUFFER_TRANSMISSION));
                Dcm_Prv_SetDslSubState((DSL_SUBSTATE_WAIT_FOR_DATA));
                bufRequestStatus_en = BUFREQ_E_BUSY;
            }
            else
            {
                if((Dcm_DsldPduInfo_st.SduLength < PduInfoPtr->SduLength ) && (Dcm_DsldGlobal_st.flgPagedBufferTxOn_b))
                {
                    /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMSET takes void pointer
                      as input and object type pointer is converted to void pointer*/
                    DCM_MEMSET(& Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[0], (sint32)DCM_CFG_SIGNAL_DEFAULT_VALUE,
                                    Dcm_DsldPduInfo_st.SduLength);

                    /* copy the reaming bytes to Start page address */
                    /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type
                    pointer is converted to void pointer*/
                    DCM_MEMCOPY(& Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[0], Dcm_DsldPduInfo_st.SduDataPtr,
                                    Dcm_DsldPduInfo_st.SduLength);

                    Dcm_Prv_SetDslState((DSL_STATE_PAGEDBUFFER_TRANSMISSION));
                    Dcm_Prv_SetDslSubState((DSL_SUBSTATE_WAIT_FOR_DATA));
                    bufRequestStatus_en = BUFREQ_E_BUSY;
                }
                else
                {
                    /* Current page transmission is not yet over. To copy the requested data */
                    Dcm_PduInfo_pst = &Dcm_DsldPduInfo_st;
                    bufRequestStatus_en = BUFREQ_OK;
                }
            }
            break;

        default :
            /*nothing to do*/
            break;

    }
    return bufRequestStatus_en;
}
#endif


/***********************************************************************************************************************
 Function name    : Dcm_Prv_isNrc21ResponseAvailable
 Syntax           : Dcm_Prv_isNrc21ResponseAvailable(&ServiceIdPtr,DcmTxPduId)
 Description      : This Function is used to check whether NRC21 response is available
 Parameter        : uint16*, PduIdType
 Return value     : boolean
***********************************************************************************************************************/
static boolean Dcm_Prv_isNrc21ResponseAvailable(uint16 * ServiceIdPtr,
        PduIdType DcmTxPduId)
{
    boolean isNrc21Available_b  = FALSE;
    uint8 connectionId_u8     = 0u;
    uint8 idxProtocol_u8      = 0u;
    PduIdType idxTxpduid_u8   = 0u;
    PduIdType idxPduId_u16   = 0u;

    for ( idxPduId_u16 = 0; idxPduId_u16 < DCM_CFG_NUM_RX_PDUID ; idxPduId_u16++ )
    {
        if(DCM_SERVICEID_DEFAULT_VALUE != Dcm_DslRxPduArray_ast[idxPduId_u16].Dcm_DslServiceId_u8)
        {
            *ServiceIdPtr         = Dcm_DslRxPduArray_ast[idxPduId_u16].Dcm_DslServiceId_u8;
            connectionId_u8       = Dcm_DsldRxTable_pcu8[idxPduId_u16];
            idxProtocol_u8        = Dcm_DsldConnTable_pcst[connectionId_u8].protocol_num_u8;
            idxTxpduid_u8         = Dcm_DsldConnTable_pcst[connectionId_u8].txpduid_num_u8;

            /*nrc21 flag for the transmission is set to True*/
            if (( idxTxpduid_u8 == Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId]) &&
                    (FALSE != Dcm_DsldProtocol_pcst[idxProtocol_u8].nrc21_b))
            {
                isNrc21Available_b = TRUE;
                break;
            }
        }
    }
    return isNrc21Available_b;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDisNrc21ResponseAvailable
 Syntax           : Dcm_Prv_OBDisNrc21ResponseAvailable(&ServiceIdPtr,DcmTxPduId)
 Description      : This Function is used to check whether NRC21 response is available for OBD
 Parameter        : uint16*, PduIdType
 Return value     : boolean
***********************************************************************************************************************/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
static boolean Dcm_Prv_OBDisNrc21ResponseAvailable(uint8* ServiceIdPtr,PduIdType DcmTxPduId)
{
    boolean isNrc21Available_b = FALSE;
    uint8 connectionId_u8 = 0u;
    uint8 idxProtocol_u8 = 0u;
    PduIdType idxTxpduid_u8 = 0u;
    PduIdType idxPduId_u16 = 0u;

    for(idxPduId_u16 = 0; idxPduId_u16 < DCM_CFG_NUM_RX_PDUID; idxPduId_u16++)
    {
        if(DCM_SERVICEID_DEFAULT_VALUE != Dcm_DslOBDRxPduArray_ast[idxPduId_u16].Dcm_DslServiceId_u8)
        {
            *ServiceIdPtr = Dcm_DslOBDRxPduArray_ast[idxPduId_u16].Dcm_DslServiceId_u8;
            connectionId_u8 = Dcm_DsldRxTable_pcu8[idxPduId_u16];
            idxProtocol_u8 = Dcm_DsldConnTable_pcst[connectionId_u8].protocol_num_u8;
            idxTxpduid_u8 = Dcm_DsldConnTable_pcst[connectionId_u8].txpduid_num_u8;

            /* Nrc21 flag for the transmission is set to True */
            if ((idxTxpduid_u8 == Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId]) &&
            (FALSE != Dcm_DsldProtocol_pcst[idxProtocol_u8].nrc21_b))
            {
                isNrc21Available_b = TRUE;
                break;
            }
        }
    }
    return isNrc21Available_b;
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ValidateCopyTxDataType
 Syntax           : Dcm_Prv_ValidateCopyTxDataType(DcmTxPduId,&PduInfoPtr,&RetryInfoPtr)
 Description      : This Function is used to update the Dcm_PduInfo_pst Structure based on following type of Responses
                    1)Normal Response
                    2)Paged Buffer Response
                    3)RoE Type 2 Response
                    4)NRC21 response
 Parameter        : PduIdType,const PduInfoType*,const RetryInfoType*
 Return value     : BufReq_ReturnType
***********************************************************************************************************************/
static BufReq_ReturnType Dcm_Prv_ValidateCopyTxDataType (PduIdType DcmTxPduId,
        const PduInfoType * PduInfoPtr,
        const RetryInfoType * RetryInfoPtr)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;
    uint16 serviceId_u16 = 0u;


    /* Set the NRC21 flag to FALSE */
    Dcm_isNrc21responseSet_b = FALSE;

    if(FALSE != Dcm_Prv_isNormalResponseAvailable(DcmTxPduId))
    {
        Dcm_PduInfo_pst = &Dcm_DsldPduInfo_st;
        bufRequestStatus_en = BUFREQ_OK;
    }
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    else if(FALSE != Dcm_Prv_isPagedBufferResponseAvailable(DcmTxPduId))
    {
        bufRequestStatus_en = Dcm_Prv_ProcessPagedBufferResponse(PduInfoPtr,RetryInfoPtr);
    }
#endif
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
    else if(FALSE != Dcm_Prv_isRoeType2ResponseAvailable(DcmTxPduId))
    {
        Dcm_PduInfo_pst = &Dcm_DsldRoe2PduInfo_st;
        bufRequestStatus_en = BUFREQ_OK;
    }
#endif
    else
    {
        if(FALSE !=  Dcm_Prv_isNrc21ResponseAvailable(&serviceId_u16,DcmTxPduId))
        {
            /* Set the flag when NRC21 Response is available */
            Dcm_isNrc21responseSet_b = TRUE;
            bufRequestStatus_en = BUFREQ_OK;

            adrDataPtr_u8[0] = DCM_NEGRESPONSE_INDICATOR;
            adrDataPtr_u8[1] = (uint8)serviceId_u16;
            adrDataPtr_u8[2] = DCM_E_BUSYREPEATREQUEST;

            Dcm_adrDataPtr_pst.SduLength = DCM_NEGATIVE_RESPONSE_LENGTH;
            Dcm_adrDataPtr_pst.SduDataPtr = &adrDataPtr_u8[0];

            /* update pduinfo with valid address */
            Dcm_PduInfo_pst = &Dcm_adrDataPtr_pst;


        }
    }

#if(DCM_PAGEDBUFFER_ENABLED == DCM_CFG_OFF)
    (void)PduInfoPtr;
    (void)RetryInfoPtr;
#endif

    return bufRequestStatus_en;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDValidateCopyTxDataType
 Syntax           : Dcm_Prv_OBDValidateCopyTxDataType(DcmTxPduId,&PduInfoPtr,&RetryInfoPtr)
 Description      : This Function is used to update the Dcm_ObdPduInfo_pst Structure
 Parameter        : DcmTxPduId
 Return value     : BufReq_ReturnType
***********************************************************************************************************************/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
static BufReq_ReturnType Dcm_Prv_OBDValidateCopyTxDataType(PduIdType DcmTxPduId)
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;
    uint8 serviceId_u8 = 0u;

    /* Set the NRC21 flag to FALSE */
    Dcm_isOBDNrc21responseSet_b = FALSE;

    if( (Dcm_Dsld_Conf_cs.ptr_txtable_pca[DcmTxPduId] == Dcm_OBDGlobal_st.dataActiveTxPduId_u8)
        && (Dcm_Prv_GetOBDState() == DCM_OBD_WAITFORTXCONF) )
    {
        Dcm_ObdPduInfo_pst = &Dcm_OBDPduInfo_st;
        bufRequestStatus_en = BUFREQ_OK;
    }
    else
    {
        if(FALSE != Dcm_Prv_OBDisNrc21ResponseAvailable(&serviceId_u8,DcmTxPduId))
        {
            /* Set the flag when NRC21 Response is available */
            Dcm_isOBDNrc21responseSet_b = TRUE;
            bufRequestStatus_en = BUFREQ_OK;

            ObdadrDataPtr_u8[0] = DCM_NEGRESPONSE_INDICATOR;
            ObdadrDataPtr_u8[1] = serviceId_u8;
            ObdadrDataPtr_u8[2] = DCM_E_BUSYREPEATREQUEST;

            Dcm_ObdadrDataPtr_pst.SduLength = DCM_NEGATIVE_RESPONSE_LENGTH;
            Dcm_ObdadrDataPtr_pst.SduDataPtr = &ObdadrDataPtr_u8[0];

            /* update Obdpduinfo with valid address */
            Dcm_ObdPduInfo_pst = &Dcm_ObdadrDataPtr_pst;
        }
    }
    return bufRequestStatus_en;
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_CopyTxData_CheckEnvironment
 Syntax           : Dcm_CopyTxData_CheckEnvironment(DcmTxPduId,&PduInfoPtr)
 Description      : This Function is used to validate all parameter passed to API Dcm_CopyTxData from Lower layer
 Parameter        : PduIdType,const PduInfoType*
 Return value     : boolean
***********************************************************************************************************************/
static boolean Dcm_CopyTxData_CheckEnvironment (PduIdType DcmTxPduId,
        const PduInfoType * PduInfoPtr)
{

    boolean environmentStatus_b = FALSE;

    if(PduInfoPtr == NULL_PTR)
    {
        DCM_DET_ERROR(DCM_COPYTXDATA_ID , DCM_E_PARAM_POINTER )
    }
    else if((PduInfoPtr->SduDataPtr == NULL_PTR) && (PduInfoPtr->SduLength != 0u))
    {
        DCM_DET_ERROR(DCM_COPYTXDATA_ID , DCM_E_PARAM_POINTER )
    }
    else if( DcmTxPduId >= DCM_NUM_TX_PDUID )
    {
        DCM_DET_ERROR(DCM_COPYTXDATA_ID ,DCM_E_DCMTXPDUID_RANGE_EXCEED)
    }
    else
    {
        environmentStatus_b = TRUE;
    }
    return environmentStatus_b;
}




/*********************************************************************************************************************
 * Dcm_CopyTxData : This call-back function is invoked by medium specific TP (CanTp/FrTp) via PduR to inform the
 * Dcm once upon reception of each segment. Within this call, the received data is copied from the receive TP buffer
 * to the DCM receive buffer.
 *
 * \param           id   : Address of global variable Passed by the appl
 *                  info :
 *                  retry:
 *                  availableDataPtr :
 *
 * \retval          BufReq_ReturnType:
 *
 *
 *
 * \seealso
 * \usedresources
 *********************************************************************************************************************/
/*TRACE[SWS_Dcm_00092][SWS_Dcm_00832]*/
BufReq_ReturnType Dcm_CopyTxData(PduIdType id,
const PduInfoType * info,
/*MR12 RULE 8.13 VIOLATION:retry is not made P2CONST since it is not CONST according to DCM AR42.2.MISRA C:2012 Rule-8.13*/
RetryInfoType * retry,
PduLengthType * availableDataPtr )
{
    BufReq_ReturnType bufRequestStatus_en = BUFREQ_E_NOT_OK;
    PduInfoType* TempPduInfo_pst;
    boolean isNrc21ResponseSet;

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    boolean context = (Dcm_Prv_IsTxPduIdOBD(id)?DCM_OBDCONTEXT:DCM_UDSCONTEXT);
#endif

    if(FALSE != Dcm_CopyTxData_CheckEnvironment(id,info))
    {
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
        if(context == DCM_OBDCONTEXT)
        {
            bufRequestStatus_en = Dcm_Prv_OBDValidateCopyTxDataType(id);
            isNrc21ResponseSet = Dcm_isOBDNrc21responseSet_b;
            TempPduInfo_pst = Dcm_ObdPduInfo_pst;
        }
        else
#endif
        {
            bufRequestStatus_en = Dcm_Prv_ValidateCopyTxDataType(id,info,retry);
            isNrc21ResponseSet = Dcm_isNrc21responseSet_b;
            TempPduInfo_pst = Dcm_PduInfo_pst;
        }

        if((BUFREQ_OK == bufRequestStatus_en) && (TempPduInfo_pst != NULL_PTR))
        {
            if (info->SduLength == 0u)
            {
                *(availableDataPtr ) = TempPduInfo_pst->SduLength;
            }
            else
            {
                if(FALSE != Dcm_Prv_isRetryRequested(retry,info,&bufRequestStatus_en))
                {
                    TempPduInfo_pst->SduDataPtr = TempPduInfo_pst->SduDataPtr - retry->TxTpDataCnt;
                    TempPduInfo_pst->SduLength = TempPduInfo_pst->SduLength + retry->TxTpDataCnt;
                }

                /* If the requested Tx buffer is less than the available Tx buffer, copy the data */
                if((info->SduLength <= TempPduInfo_pst->SduLength) && (BUFREQ_OK == bufRequestStatus_en))
                {
                    /*TRACE[SWS_Dcm_00346]*/
                    /*MR12 DIR 1.1 VIOLATION:This is required for implementation as DCM_MEMCOPY takes void pointer as
                     input and object type pointer is converted to void pointer*/
                    DCM_MEMCOPY(info->SduDataPtr, TempPduInfo_pst->SduDataPtr, info->SduLength);
                    TempPduInfo_pst->SduDataPtr = TempPduInfo_pst->SduDataPtr + info->SduLength;
                    TempPduInfo_pst->SduLength = (uint16)((uint16)TempPduInfo_pst->SduLength -  \
                    (uint16)((FALSE != isNrc21ResponseSet) ? 0u : info->SduLength));

                    /*TRACE[SWS_Dcm_00350]*/
                    *(availableDataPtr ) = TempPduInfo_pst->SduLength;
                }
            }
        }
    }
    return(bufRequestStatus_en);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
