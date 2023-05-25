

#ifndef DCM_DSD_PRV_H

#define DCM_DSD_PRV_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "DcmCore_DslDsd_Inf.h"
#include "DcmCore_DslDsd_Pub.h"
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Roe_Inf.h"
#endif
/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

#define DCM_MANUFACTURERNOTIFICATION        TRUE
#define DCM_SUPPLIERNOTIFICATION            FALSE
#define DCM_RESPONSEONEVENT_SID             0x86u
#define DCM_SECURITYACCESS_SID              0x27u
#define DCM_SERVICE_ISO_LOWERLIMIT          0x40u
#define DCM_SERVICE_ISO_MIDLIMIT            0x7Fu
#define DCM_SERVICE_ISO_UPPERLIMIT          0xC0u
#define DCM_RESPONSEBUFFER_INDEX            0x03u   /* Index from where the Response is to be updated */
#define DCM_REQUESTBUFFER_INDEX             0x00u   /* Index from where the Request data is to be considered  */
#define DCM_SID_LENGTH                      0x01u   /* Length of SID for any request */
#define DCM_SUPPRESPOSITIVERESP_MASK        0x80u   /* Mask to check whether suppress positive response bit is set */
#define DCM_REMOVESUPPRESSRESPONSEBIT_MASK  0x7Fu   /* Mask to clear Suppress poitive response bit */
/* Mask to clear event storage bit in case of ROE , 2 most significant bit*/
#define DCM_REMOVEEVENTSTORAGEBIT_MASK      0xBFu
#define DCM_DEFAULT_VALUE                   0x00u
/* Mask value which will be varied depending on active session/security */
#define DCM_DEFAULT_MASKVALUE               0x00000001uL
#define DCM_UINT16_MAXVALUE                 0xFFFFu

/*   Local Inline function definitions to be used by Dcm_DsdStateMachine */


/***********************************************************************************************************************
 Function name    : Dcm_Prv_IsSubFunctionSupported
 Syntax           : Dcm_Prv_IsSubFunctionSupported()
 Description      : Helper Function to check if requested service supports SubFunction or not
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_IsSubFunctionSupported(void)
{
    /* obtain the address of active service configuration table */
    const Dcm_Dsld_ServiceType* adrActiveService_pcst = &(Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8]);

    return(adrActiveService_pcst->subfunction_exist_b);
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckSuppressPositiveResponse
 Syntax           : Dcm_Prv_CheckSuppressPositiveResponse(subfunction)
 Description      : Helper Function to check if suppress positive response is needed
                     If yes , then do the necessary changes and update the subfunction value
 Parameter        : uint8*
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_CheckSuppressPositiveResponse(uint8* subfunction)
{
    /* Is Suppress Positive Response requested , set suppress positive response info to TRUE
     * And mask this info from request data*/
    if(((*Dcm_DsldMsgContext_st.reqData) & (DCM_SUPPRESPOSITIVERESP_MASK)) == DCM_SUPPRESPOSITIVERESP_MASK)
    {
        Dcm_DsldMsgContext_st.msgAddInfo.suppressPosResponse = TRUE;
        /* remove the MSB(suppress positive bit) of sub function */
        (*Dcm_DsldMsgContext_st.reqData) = ((*Dcm_DsldMsgContext_st.reqData) & DCM_REMOVESUPPRESSRESPONSEBIT_MASK);
    }
    else
    {
        Dcm_DsldMsgContext_st.msgAddInfo.suppressPosResponse = FALSE;
    }
    /* Get the sub-service byte from the request
     * In case of ROE mask the event storage information */
    (*subfunction) = (Dcm_DsldGlobal_st.dataSid_u8 != DCM_RESPONSEONEVENT_SID)? (*Dcm_DsldMsgContext_st.reqData)
                                          :((*Dcm_DsldMsgContext_st.reqData) & DCM_REMOVEEVENTSTORAGEBIT_MASK);
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdObtainSidIndex
 Syntax           : Dcm_Prv_DsdObtainSidIndex(idxIndex_qu8,dataSid_cu8)
 Description      : Linear search to find out the element(Key, SID) in the array of structure
 Parameter        : uint8*,const uint8
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_DsdObtainSidIndex(uint8* idxIndex_qu8, const uint8 dataSid_cu8)
{
    boolean IsSidAvailable_b = FALSE;
    /* Store the number of services configured in currently active service table */
    uint8 nrServices_u8 = Dcm_Dsld_Conf_cs.sid_table_pcs[Dcm_DsldGlobal_st.datActiveSrvtable_u8].num_services_u8;
    *idxIndex_qu8 = DCM_DEFAULT_VALUE;

    while(*idxIndex_qu8 < nrServices_u8)
    {
        if(Dcm_DsldSrvTable_pcst[*idxIndex_qu8].sid_u8 == dataSid_cu8)
        {
            IsSidAvailable_b = TRUE;
            break;
        }
        (*idxIndex_qu8)++;
    }
    return(IsSidAvailable_b);
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ObtainSidIndexOfOBD
 Syntax           : Dcm_Prv_ObtainSidIndexOfOBD(idxIndex_qu8,dataSid_cu8,context)
 Description      : Linear search to find out the element(Key, SID) for OBD in case of parallel processing
 Parameter        : uint8*,const uint8
 Return value     : boolean
***********************************************************************************************************************/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
LOCAL_INLINE boolean Dcm_Prv_ObtainSidIndexOfOBD(uint8* idxIndex_qu8, const uint8 dataSid_cu8)
{
    boolean IsSidAvailable_b = FALSE;
    uint8 nrServices_u8 = Dcm_Dsld_Conf_cs.sid_table_pcs[Dcm_OBDGlobal_st.datActiveSrvtable_u8].num_services_u8;
    *idxIndex_qu8 = DCM_DEFAULT_VALUE;

    while(*idxIndex_qu8 < nrServices_u8)
    {
        if(Dcm_OBDSrvTable_pcst[*idxIndex_qu8].sid_u8 == dataSid_cu8)
        {
            IsSidAvailable_b = TRUE;
            break;
        }
        (*idxIndex_qu8)++;
    }
    return(IsSidAvailable_b);
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdSendNegativeResponse
 Syntax           : Dcm_Prv_DsdSendNegativeResponse(ErrorCode)
 Description      : Trigger Negative Response by DSD
 Parameter        : Dcm_NegativeResponseCodeType
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_DsdSendNegativeResponse(Dcm_NegativeResponseCodeType ErrorCode)
{
    /* If no NRC is set , then set NRC to conditions not correct */
    ErrorCode = (ErrorCode == DCM_DEFAULT_VALUE)?DCM_E_CONDITIONSNOTCORRECT:ErrorCode;
    /* Response given by DSD itself , Set the data response given by DSD to True*/
    Dcm_DsldGlobal_st.dataResponseByDsd_b = TRUE;
    Dcm_SetNegResponse(&Dcm_DsldMsgContext_st,ErrorCode);
    Dcm_ProcessingDone(&Dcm_DsldMsgContext_st);
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDDsdSendNegativeResponse
 Syntax           : Dcm_Prv_OBDDsdSendNegativeResponse(ErrorCode)
 Description      : Trigger Negative Response by DSD for OBD
 Parameter        : Dcm_NegativeResponseCodeType
 Return value     : void
***********************************************************************************************************************/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
LOCAL_INLINE void Dcm_Prv_OBDDsdSendNegativeResponse(Dcm_NegativeResponseCodeType ErrorCode)
{
    /* If no NRC is set , then set NRC to conditions not correct */
    ErrorCode = (ErrorCode == DCM_DEFAULT_VALUE)?DCM_E_CONDITIONSNOTCORRECT:ErrorCode;
    /* Response given by DSD itself , Set the data response given by DSD to True*/
    Dcm_OBDGlobal_st.dataResponseByDsd_b = TRUE;
    Dcm_Prv_SetOBDNegResponse(&Dcm_OBDMsgContext_st,ErrorCode);
    Dcm_OBDProcessingDone(&Dcm_OBDMsgContext_st);
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessServiceNotSupported
 Syntax           : Dcm_Prv_ProcessServiceNotSupported(ErrorCode_u8)
 Description      : Helper Function to send NRC when requested service is not supported
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_ProcessServiceNotSupported(void)
{
    Dcm_NegativeResponseCodeType ErrorCode_u8 = DCM_E_SERVICENOTSUPPORTED;

#if(DCM_CFG_RESPOND_ALLREQUEST == DCM_CFG_OFF)
    uint8 SID_u8 = Dcm_DsldGlobal_st.dataSid_u8;
    /* Check whether the configured service is in proper range as specified in ISO */
    if((SID_u8 < DCM_SERVICE_ISO_LOWERLIMIT) ||
       ((SID_u8 > DCM_SERVICE_ISO_MIDLIMIT)  && (SID_u8 < DCM_SERVICE_ISO_UPPERLIMIT)))
    {
        Dcm_Prv_DsdSendNegativeResponse(ErrorCode_u8);
    }
    else
    {
        /* SID is out of range, ignore the request *
        *  Set the suppressPosResponse flag is set to True */
        Dcm_DsldMsgContext_st.msgAddInfo.suppressPosResponse = TRUE;
        Dcm_DsldGlobal_st.dataResponseByDsd_b = TRUE;
        Dcm_ProcessingDone(&Dcm_DsldMsgContext_st);
    }
#else
    /* Send negative response for all range of SID with NRC service not supported because non configured service */
    Dcm_Prv_DsdSendNegativeResponse(ErrorCode_u8);
#endif
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessOBDServiceNotSupported
 Syntax           : Dcm_Prv_ProcessOBDServiceNotSupported(void)
 Description      : Helper Function to send NRC for OBD when requested service is not supported
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
LOCAL_INLINE void Dcm_Prv_ProcessOBDServiceNotSupported(void)
{
    Dcm_NegativeResponseCodeType ErrorCode_u8 = DCM_E_SERVICENOTSUPPORTED;

#if(DCM_CFG_RESPOND_ALLREQUEST == DCM_CFG_OFF)
    uint8 SID_u8 = Dcm_OBDGlobal_st.dataSid_u8;
    /* Check whether the configured service is in proper range as specified in ISO */
    if((SID_u8 < DCM_SERVICE_ISO_LOWERLIMIT) ||
       ((SID_u8 > DCM_SERVICE_ISO_MIDLIMIT)  && (SID_u8 < DCM_SERVICE_ISO_UPPERLIMIT)))
    {
        Dcm_Prv_OBDDsdSendNegativeResponse(ErrorCode_u8);
    }
    else
    {
        /* SID is out of range, ignore the request *
        *  Set the suppressPosResponse flag is set to True */
        Dcm_OBDMsgContext_st.msgAddInfo.suppressPosResponse = TRUE;
        Dcm_OBDGlobal_st.dataResponseByDsd_b = TRUE;
        Dcm_OBDProcessingDone(&Dcm_OBDMsgContext_st);
    }
#else
    /* Send negative response for all range of SID with NRC service not supported because non configured service */
    Dcm_Prv_OBDDsdSendNegativeResponse(ErrorCode_u8);
#endif
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdVerifySubFncID
 Syntax           : Dcm_Prv_DsdVerifySubFncID(idxSubservice_u8,ErrorCode)
 Description      : Check whether at least one sub-function byte exists and extract
                    requested sub-function index
 Parameter        : uint8,Dcm_NegativeResponseCodeType
 Return value     : Std_ReturnType
***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_DsdVerifySubFncID(
        uint8* idxSubservice_u8,
        const Dcm_Dsld_ServiceType* service_pcs,
        Dcm_NegativeResponseCodeType* ErrorCode)
{
    const Dcm_Dsld_SubServiceType* adrSubservice_pcst;
    const Dcm_MsgContextType* Msgcontext_s = &Dcm_DsldMsgContext_st;
    Std_ReturnType VerificationResult_u8 = E_NOT_OK;
    /* Variable to store the sub-function byte from the request */
    uint8 dataSubfunction_u8;
    *idxSubservice_u8 = DCM_DEFAULT_VALUE;
    *ErrorCode = DCM_DEFAULT_VALUE;  /* Reset the error code to 0x00 */

    /* Check if there is at least one byte (sub function byte exists) */
    if (Msgcontext_s->reqDataLen > DCM_DEFAULT_VALUE)
    {
        /* Get the sub service configuration structure */
        adrSubservice_pcst = service_pcs->ptr_subservice_table_pcs;
        /* Call to check whether suppress positive response is requested
         * if needed , the service updates the necessary flags and returns the updated subfunction value */
        Dcm_Prv_CheckSuppressPositiveResponse(&dataSubfunction_u8);

        /* Loop through to check , whether requested SubFunction is configured */
        while(*idxSubservice_u8 < service_pcs->num_sf_u8)
        {
            if (dataSubfunction_u8 == adrSubservice_pcst[*idxSubservice_u8].subservice_id_u8)
            {
                break;
            }
            (*idxSubservice_u8)++;
        }
        /* If sub service is configured*/
        if (*idxSubservice_u8 < service_pcs->num_sf_u8)
        {
            VerificationResult_u8 = E_OK;
        }
        /* If the sub service is not supported */
        else
        {
            *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        }
    }
    else
    {
        /* If service is outside DSP */
        if (service_pcs->servicelocator_b == FALSE)
        {
            /* Call the DcmAppl API to get the NRC to be returned in case minimum length check fails for
             *  project/vendor specific services  */
            DcmAppl_DcmGetNRCForMinLengthCheck(
                    Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8, service_pcs->sid_u8,
                    ErrorCode);
        }
        if (*ErrorCode == DCM_DEFAULT_VALUE)
        {
            /* If no ErrorCode is set , then NRC to 0x13 as sub function byte is missing in the service request */
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }
    }
    return VerificationResult_u8;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessResult
 Syntax           : Dcm_Prv_ProcessResult(RequestData,Context,ErrorCode
 Description      : The Result of Manufacturer / Supplier checks is further processed here
 Parameter        : Std_ReturnType, boolean,Dcm_NegativeResponseCodeType
 Return value     : void
***********************************************************************************************************************/
#if((DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED != DCM_CFG_OFF)||(DCM_CFG_SUPPLIER_NOTIFICATION_ENABLED != DCM_CFG_OFF))
static void Dcm_Prv_ProcessResult(Std_ReturnType Result,boolean Context,Dcm_NegativeResponseCodeType ErrorCode)
{
    if(Result == E_OK)
    {
        /* Do nothing */
    }
    else if(Result == DCM_E_REQUEST_NOT_ACCEPTED)
    {
#if ((DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS != 0u) || (DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS !=0u))
        Dcm_Dsd_CallRTEConfirmation((Dcm_ConfirmationStatusType)DCM_RES_POS_NOT_OK,Context);
#endif
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
        if(Context == DCM_OBDCONTEXT)
        {
           Dcm_Prv_SetOBDState((DCM_OBD_IDLE));
        }
        else
#endif
        {
           Dcm_Prv_SetDsdState(DSD_IDLE);
           Dcm_Prv_SetDslState((DSL_STATE_IDLE));
           Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
           /* Reset the S3 timer on completion of the requested action in case no response is allowed */
           Dcm_Prv_ReloadS3Timer();

        }
    }
    else
    {
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
       if(Context == DCM_OBDCONTEXT)
       {
           Dcm_Prv_OBDDsdSendNegativeResponse(ErrorCode);
       }
       else
#endif
       {
           Dcm_Prv_DsdSendNegativeResponse(ErrorCode);
       }
    }
#if ((DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS == 0u) && (DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS ==0u) \
      && (DCM_PARALLELPROCESSING_ENABLED == DCM_CFG_OFF))
        (void)Context;
#endif

}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdManufacturerNotification
 Syntax           : Dcm_Prv_DsdManufacturerNotification(RequestData,RequestLength,RequestType,DcmRxPduId,
                                                        SourceAddress,ErrorCode)
 Description      : Invoke Manufacturer Notification to relevant application callbacks
 Parameter        : uint8,uint16,uint8,PduIdType,uint16
 Return value     : Std_ReturnType
***********************************************************************************************************************/
#if (DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED != DCM_CFG_OFF)

LOCAL_INLINE Std_ReturnType Dcm_Prv_DsdManufacturerNotification
                             (Dcm_MsgContextType MsgContext_st,
                               uint8 SID,uint8 RequestType,
                              uint16 SourceAddress,boolean Context)
{
    Std_ReturnType Manufacturer_ReturnValue;
    Dcm_NegativeResponseCodeType ErrorCode = DCM_DEFAULT_VALUE;
    PduLengthType RequestLength = MsgContext_st.reqDataLen;
    PduIdType DcmRxPduId = MsgContext_st.dcmRxPduId;
    const uint8* const RequestData = MsgContext_st.reqData;
#if(DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS !=0)
#if((DCM_CFG_INDICATION_DATASIZE_UINT32==DCM_CFG_OFF))
    uint16 localRequestLength = (uint16)MsgContext_st.reqDataLen;
#else
    PduLengthType localRequestLength = MsgContext_st.reqDataLen;
#endif
#endif
#if(DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS !=0)
    uint32_least index_ui;
    Std_ReturnType ManufacturerNotif_Result = E_OK;
    Dcm_NegativeResponseCodeType dataNegRespCode_u8 = DCM_DEFAULT_VALUE; /* Local variable to get negative response code */
#endif

    /* Call DcmAppl function to indicate new request */
    Manufacturer_ReturnValue = DcmAppl_ManufacturerNotification(SID,RequestData,RequestLength,RequestType,DcmRxPduId,SourceAddress,&Dcm_DsldMsgContext_st,&ErrorCode);
    /* Invoke manufacturer callbacks only when at least one service request indication port is configured */
    if(Manufacturer_ReturnValue != DCM_E_PENDING)
    {
#if(DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS !=0)
    /* Call all configured functions in RTE */
        for(index_ui=0x00u;index_ui<DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS;index_ui++)
        {
            ManufacturerNotif_Result =(*Dcm_Rte_ManufacturerNotification_a[index_ui])(SID,RequestData,localRequestLength,RequestType,SourceAddress,&dataNegRespCode_u8);
            if( (ManufacturerNotif_Result == E_NOT_OK) && (ErrorCode == DCM_DEFAULT_VALUE) )
            {
                ErrorCode = dataNegRespCode_u8;
            }
            /* Store the largest value amongst the two
             * DCM_E_REQUEST_NOT_ACCEPTED has highest priority followed by E_NOT_OK*/
            Manufacturer_ReturnValue = (ManufacturerNotif_Result > Manufacturer_ReturnValue)?ManufacturerNotif_Result:Manufacturer_ReturnValue;
        }
/*DET Error will be reported when pdulength type is uint32 and DcmRbIndicationVerification is configured as false.*/
#if ((DCM_CFG_DET_SUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_INDICATION_DATASIZE_UINT32==DCM_CFG_OFF))

        /*Checking if requested message length is greater than uint16 max value*/
        if(RequestLength>DCM_UINT16_MAXVALUE)
        {
            DCM_DET_ERROR(DCM_MANUFACTURENOTIFICATION_ID , DCM_E_PARAM )
        }
#endif
#endif
        Dcm_Prv_ProcessResult(Manufacturer_ReturnValue,Context,ErrorCode);
    }
    return Manufacturer_ReturnValue;
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdSupplierNotification
 Syntax           : Dcm_Prv_DsdSupplierNotification(RequestData,RequestLength,RequestType,DcmRxPduId,
                                                    SourceAddress,ErrorCode)
 Description      : Invoke Supplier Notification for all configured ports
 Parameter        : uint8,uint16,uint8,PduIdType,uint16
 Return value     : Std_ReturnType
***********************************************************************************************************************/
#if (DCM_CFG_SUPPLIER_NOTIFICATION_ENABLED != DCM_CFG_OFF)

LOCAL_INLINE Std_ReturnType Dcm_Prv_DsdSupplierNotification(Dcm_MsgContextType MsgContext_st,
                                                             uint8 SID,uint8 RequestType,
                                                             uint16 SourceAddress, boolean Context)
{
    Std_ReturnType Supplier_ReturnValue;
    Dcm_NegativeResponseCodeType ErrorCode = DCM_DEFAULT_VALUE;
#if(DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS != DCM_DEFAULT_VALUE)
    uint32_least index_ui;
    Std_ReturnType SupplierNotif_Result = E_OK;
    Dcm_NegativeResponseCodeType dataNegRespCode_u8 = DCM_DEFAULT_VALUE; /* Local variable to get negative response code */
#endif
    PduLengthType RequestLength = MsgContext_st.reqDataLen;
    PduIdType DcmRxPduId = MsgContext_st.dcmRxPduId;
    const uint8* const RequestData = MsgContext_st.reqData;
#if(DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS != DCM_DEFAULT_VALUE)
#if((DCM_CFG_INDICATION_DATASIZE_UINT32==DCM_CFG_OFF))
    uint16 localRequestLength = (uint16)MsgContext_st.reqDataLen;
#else
    PduLengthType localRequestLength = MsgContext_st.reqDataLen;
#endif
#endif
    /* Invoke DcmAppl Supplier Notification */
    Supplier_ReturnValue = DcmAppl_DcmNotification(SID,RequestData,RequestLength,RequestType,DcmRxPduId,SourceAddress,&ErrorCode);
    if(Supplier_ReturnValue != DCM_E_PENDING)
    {
        /* Invoke Supplier callbacks only when at least one service request indication port is configured */
#if(DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS != DCM_DEFAULT_VALUE)
        /* Call all configured functions in RTE */
        for(index_ui= DCM_DEFAULT_VALUE;index_ui<DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS; index_ui++)
        {
            SupplierNotif_Result = (*Dcm_Rte_SupplierNotification_a[index_ui])
                                    (SID,RequestData,localRequestLength,RequestType,SourceAddress,&dataNegRespCode_u8);
            if( (SupplierNotif_Result == E_NOT_OK) && (ErrorCode == DCM_DEFAULT_VALUE) )
            {
                ErrorCode = dataNegRespCode_u8;
            }
            /* Store the largest value amongst the two
             * DCM_E_REQUEST_NOT_ACCEPTED has highest priority followed by E_NOT_OK */
            Supplier_ReturnValue = (SupplierNotif_Result > Supplier_ReturnValue)?
                                     SupplierNotif_Result:Supplier_ReturnValue;
        }

/*DET Error will be reported when pdulength type is uint32 and DcmRbIndicationVerification is configured as false.*/
#if ((DCM_CFG_DET_SUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_INDICATION_DATASIZE_UINT32==DCM_CFG_OFF))

        /*Checking if requested message length is greater than uint16 max value*/
        if(RequestLength>DCM_UINT16_MAXVALUE)
        {
            DCM_DET_ERROR(DCM_SUPPLIERNOTIFICATION_ID , DCM_E_PARAM )
        }
#endif
#endif
        Dcm_Prv_ProcessResult(Supplier_ReturnValue,Context,ErrorCode);
    }
    return Supplier_ReturnValue;
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_UpdateOpStatus
 Description      : Used to update the OpStatus based on Service return
                    - If Service is processed within Dcm
                    - If Service is processed outside Dcm, Update Dcm_ExtSrvOpStatus_u8 based on return
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_UpdateOpStatus(Std_ReturnType ServiceResult_u8)
{
    if(Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8].servicelocator_b != FALSE)
    {
        if((ServiceResult_u8 != DCM_E_PENDING) && (ServiceResult_u8 != DCM_E_FORCE_RCRRP))
        {
            Dcm_SrvOpstatus_u8 = DCM_INITIAL;
        }
    }
    else
    {
        if(ServiceResult_u8 == DCM_E_PENDING)
        {
            Dcm_ExtSrvOpStatus_u8 = DCM_PENDING;
        }
        else if(ServiceResult_u8 == DCM_E_FORCE_RCRRP)
        {
            Dcm_ExtSrvOpStatus_u8 = DCM_FORCE_RCRRP_OK;
        }
        else
        {
            Dcm_ExtSrvOpStatus_u8 = DCM_INITIAL;
        }
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdInitiateResponseTransmission
 Syntax           : Dcm_Prv_DsdInitiateResponseTransmission(ServiceResult_u8,ErrorCode_u8)
 Description      : Is Invoked t0 trigger positive, negative response or
                    Response Pending operation (NRC0x78) or PENDING return
 Parameter        : Std_ReturnType,Dcm_NegativeResponseCodeType
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_DsdInitiateResponseTransmission(Std_ReturnType ServiceResult_u8,
                                    Dcm_NegativeResponseCodeType ErrorCode_u8)
{
    Dcm_Prv_UpdateOpStatus(ServiceResult_u8);
    if (ServiceResult_u8 == E_OK)
    {
        /*Trigger transmission of positive response*/
        Dcm_ProcessingDone(&Dcm_DsldMsgContext_st);
    }
    else if (ServiceResult_u8 == DCM_E_PENDING)
    {
        /*Do nothing here, The Global  Opstatus will be set to required state by the service*/
    }
    else if (ServiceResult_u8 == DCM_E_FORCE_RCRRP)
    {
        (void) Dcm_ForceRespPend();
    }
    else
    {
        /* If no NRC is set by the service send NRC22 as default service, ideally this should nevery occur */
        if (ErrorCode_u8 == DCM_DEFAULT_VALUE)
        {
            ErrorCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
        }
        /*Trigger transmission of negative response*/
        Dcm_SetNegResponse(&Dcm_DsldMsgContext_st, ErrorCode_u8);
        Dcm_ProcessingDone(&Dcm_DsldMsgContext_st);
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdVerifyData
 Syntax           : Dcm_Prv_DsdVerifyData()
 Description      : Perform verification of the requested data
 Parameter        : void
 Return value     : Std_ReturnType
***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_DsdVerifyData(void)
{
    /* local pointer to service configuration structure */
    const Dcm_Dsld_ServiceType* adrService_pcst;
    uint32 ActiveMask_u32;   /* bit mask to represent Session/Security */
    uint8 idxIndex_qu8;      /* To store the Index of the requested Service */
    Dcm_NegativeResponseCodeType ErrorCode_u8 = DCM_DEFAULT_VALUE;  /* Variable to store NRC from application */
    Std_ReturnType  VerificationResult_u8 = E_NOT_OK;  /* To update the return Value */

    /* 1: Verification of the requested SID */
    if (Dcm_Prv_DsdObtainSidIndex(&idxIndex_qu8, Dcm_DsldGlobal_st.dataSid_u8))
    {
        /* SID found here, store the index of requested service */
        Dcm_DsldGlobal_st.idxService_u8 = idxIndex_qu8;
        /* Id context is used as SID */
        Dcm_DsldMsgContext_st.idContext = Dcm_DsldGlobal_st.dataSid_u8;
        /* get the active service configuration structure */
        adrService_pcst = &(Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8]);
        /* generate bit mask for active session */
        ActiveMask_u32 = (uint32) (DCM_DEFAULT_MASKVALUE << Dcm_DsldGlobal_st.idxActiveSession_u8);
        /* 2: Check requested service is allowed in this session */
        if ((adrService_pcst->allowed_session_b32 & ActiveMask_u32) == (ActiveMask_u32))
        {
            /* generate bit mask for active security level */
            ActiveMask_u32 = (uint32) (DCM_DEFAULT_MASKVALUE << Dcm_DsldGlobal_st.idxActiveSecurity_u8);
            /* 3: Check requested service is allowed in this security level */
            if ((adrService_pcst->allowed_security_b32 & ActiveMask_u32) == (ActiveMask_u32))
            {
                /* 4: Check requested service is allowed in configured Mode rule */
                VerificationResult_u8 = (*adrService_pcst->adrUserServiceModeRule_pfct)
                                        (&ErrorCode_u8, Dcm_DsldGlobal_st.dataSid_u8);
#if(DCM_CFG_DSD_MODERULESERVICE_ENABLED != DCM_CFG_OFF)
                /* Check if the mode rule is configured for the sub function */
                if((adrService_pcst->moderule_fp != ((Dcm_ModeRuleType)NULL_PTR)) && (VerificationResult_u8 == E_OK))
                {
                    /* Call the mode rule API configured */
                    VerificationResult_u8 = (adrService_pcst->moderule_fp(&ErrorCode_u8) == TRUE)?E_OK:E_NOT_OK;
                }
#endif
            }
            else
            {
                ErrorCode_u8 = DCM_E_SECURITYACCESSDENIED;
            }
        }
        else
        {
           /* Service not supported in active session
           * Send Appropriate NRC for this based on configuration in service table */
           ErrorCode_u8=Dcm_Dsld_Conf_cs.sid_table_pcs[Dcm_DsldGlobal_st.datActiveSrvtable_u8].nrc_sessnot_supported_u8;
        }

        if (VerificationResult_u8 != E_OK)
        {
            Dcm_Prv_DsdSendNegativeResponse(ErrorCode_u8);
        }
    }
    else
    {
        Dcm_Prv_ProcessServiceNotSupported();
    }
    return VerificationResult_u8;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDVerifyData
 Syntax           : Dcm_Prv_OBDVerifyData()
 Description      : Perform verification of the requested data for OBD in case of parallel processing
 Parameter        : void
 Return value     : Std_ReturnType
***********************************************************************************************************************/

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
LOCAL_INLINE Std_ReturnType Dcm_Prv_OBDVerifyData(void)
{
    /* local pointer to service configuration structure */
    const Dcm_Dsld_ServiceType* adrService_pcst;
    uint32 ActiveMask_u32;   /* bit mask to represent Session/Security */
    uint8 idxIndex_qu8;      /* To store the Index of the requested Service */
    Dcm_NegativeResponseCodeType ErrorCode_u8 = DCM_DEFAULT_VALUE;  /* Variable to store NRC from application */
    Std_ReturnType  VerificationResult_u8 = E_NOT_OK;  /* To update the return Value */

    /* 1: Verification of the requested SID */
    if (Dcm_Prv_ObtainSidIndexOfOBD(&idxIndex_qu8,Dcm_OBDGlobal_st.dataSid_u8))
    {
        /* SID found here, store the index of requested service */
        Dcm_OBDGlobal_st.idxService_u8 = idxIndex_qu8;
        /* Id context is used as SID */
        Dcm_OBDMsgContext_st.idContext = Dcm_OBDGlobal_st.dataSid_u8;
        /* get the active service configuration structure */
        adrService_pcst = &(Dcm_OBDSrvTable_pcst[Dcm_OBDGlobal_st.idxService_u8]);
        /* generate bit mask for active security level */
        ActiveMask_u32 = (uint32) (DCM_DEFAULT_MASKVALUE << Dcm_DsldGlobal_st.idxActiveSecurity_u8);
        /* 2: Check requested service is allowed in this security level */
        if ((adrService_pcst->allowed_security_b32 & ActiveMask_u32) == (ActiveMask_u32))
        {
            /* 3: Check requested service is allowed in configured Mode rule */
            VerificationResult_u8 = (*adrService_pcst->adrUserServiceModeRule_pfct)
                                    (&ErrorCode_u8, Dcm_OBDGlobal_st.dataSid_u8);
#if(DCM_CFG_DSD_MODERULESERVICE_ENABLED != DCM_CFG_OFF)
            /* Check if the mode rule is configured for the sub function */
            if((adrService_pcst->moderule_fp != ((Dcm_ModeRuleType)NULL_PTR)) && (VerificationResult_u8 == E_OK))
            {
                /* Call the mode rule API configured */
                VerificationResult_u8 = (adrService_pcst->moderule_fp(&ErrorCode_u8) == TRUE)?E_OK:E_NOT_OK;
            }
#endif
        }
        else
        {
            ErrorCode_u8 = DCM_E_SECURITYACCESSDENIED;
        }
        if (VerificationResult_u8 != E_OK)
        {
            Dcm_Prv_OBDDsdSendNegativeResponse(ErrorCode_u8);
        }
    }
    else
    {
        Dcm_Prv_ProcessOBDServiceNotSupported();
    }
    return VerificationResult_u8;
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdIsServiceRunning
 Syntax           : Dcm_Prv_DsdIsServiceRunning()
 Description      : If ROE Type 2 is enabled , then check whether the requested service is
                    already running if triggerd by ROE , either RDBI.RDTC service
 Parameter        : void
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_DsdIsServiceRunning(void)
{
    boolean IsServiceRunning_b = FALSE;
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
    /* If ROE Type 2 event is being processed */
    IsServiceRunning_b = ((Dcm_DsdRoe2State_en != DSD_IDLE) &&
                         (Dcm_Roe2MesContext_st.idContext == Dcm_DsldGlobal_st.dataSid_u8));
#endif
    /* In case parallel processing is already ongoing
     * and a session change request is to be processed
     * then delay the processing of session change request until OBD service
     * has finished processing */
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    if((Dcm_DsldGlobal_st.dataSid_u8 == DCM_SID_DIAGNOSTICSESSIONCONTROL) &&
        (Dcm_Prv_GetOBDState()!= DCM_OBD_IDLE ))
    {
        IsServiceRunning_b = TRUE;
    }
#endif
    return IsServiceRunning_b;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdProcessService
 Syntax           : Dcm_Prv_DsdProcessService()
 Description      : Function to invoke requested service
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_DsdProcessService(void)
{
    Std_ReturnType ServiceResult_u8; /* To store the return value of the invoked service */
    const Dcm_Dsld_ServiceType* adrServiceTable_pcst; /* Pointer to service table */
    Dcm_NegativeResponseCodeType ErrorCode_u8 = DCM_DEFAULT_VALUE;

#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_PagedBufferTimeout();
#endif
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    Dcm_DsldRoeTimeOut();
#endif

    /* Check whether an ROE Type 2 Event is not being processed by calling Dcm_Prv_DsdIsServiceRunning
     * If DSD State is changed (by above TimeOutfunction calls) then no need to call service  */
    if(Dcm_Prv_DsdIsServiceRunning() == FALSE)
    {
        if(Dcm_Prv_GetDsdState() == DSD_CALL_SERVICE)
        {
            adrServiceTable_pcst = &Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8];
            /* Check whether the service is external service
             * Based on that call the service with appropriate OpStatus */
            if(Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8].servicelocator_b != FALSE)
            {
                ServiceResult_u8 = (*(adrServiceTable_pcst)->service_handler_fp)(Dcm_SrvOpstatus_u8,&Dcm_DsldMsgContext_st,
                                                  &ErrorCode_u8);
            }
            else
            {
                ServiceResult_u8 = (*(adrServiceTable_pcst)->service_handler_fp)(Dcm_ExtSrvOpStatus_u8,&Dcm_DsldMsgContext_st,
                                                  &ErrorCode_u8);
            }

            /* Do Further Processing based on service return value */
            Dcm_Prv_DsdInitiateResponseTransmission(ServiceResult_u8,ErrorCode_u8);
        }
    }
}

#if((DCM_CFG_SUPPLIER_NOTIFICATION_ENABLED != DCM_CFG_OFF)||(DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED != DCM_CFG_OFF))
/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdNotification
 Syntax           : Dcm_Prv_DsdNotification(Flag,Context)
 Description      : Function to initialize and invoke either supplier/manufacturer notification
 Parameter        : boolean,boolean
 Return value     : Std_ReturnType
***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_DsdNotification(boolean Indication_Flag,boolean Context)
{
    Dcm_MsgContextType MsgContext_st;
    uint8 Sid;
    uint8 RequestType;
    uint16 SourceAddress;
    uint8 Result_u8 = E_NOT_OK;

    /* Initialize the required parameters to be passed to Notification CallBacks */
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    if(Context == DCM_OBDCONTEXT)
    {
        MsgContext_st = Dcm_OBDMsgContext_st;
        Sid = Dcm_OBDGlobal_st.dataSid_u8;
        RequestType = Dcm_OBDMsgContext_st.msgAddInfo.reqType;
        SourceAddress = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_OBDMsgContext_st.dcmRxPduId]].testaddr_u16;
    }
    else
#endif
    {
        MsgContext_st = Dcm_DsldMsgContext_st;
        Sid = Dcm_DsldGlobal_st.dataSid_u8;
        RequestType = Dcm_DsldMsgContext_st.msgAddInfo.reqType;
        SourceAddress = Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_DsldMsgContext_st.dcmRxPduId]].testaddr_u16;
    }

    if(Indication_Flag == DCM_MANUFACTURERNOTIFICATION)
    {
#if(DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED != DCM_CFG_OFF)
        Result_u8 =Dcm_Prv_DsdManufacturerNotification(MsgContext_st,Sid,RequestType,SourceAddress,Context);
#endif
    }
#if(DCM_CFG_SUPPLIER_NOTIFICATION_ENABLED != DCM_CFG_OFF)
    else
    {
        Result_u8 = Dcm_Prv_DsdSupplierNotification(MsgContext_st,Sid,RequestType,SourceAddress,Context);
    }
#endif
    return Result_u8;
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdCheckSubFunction
 Syntax           : Dcm_Prv_DsdCheckSubFunction()
 Description      : Function to do subservice specific checks in DSD
 Parameter        : void
 Return value     : Std_ReturnType
***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_DsdCheckSubFunction(void)
{

    uint8 idxSubservice_u8; /* Index to loop through the sub-service table */
    uint32 dataActiveMask_u32; /* bit mask to represent session */
    Dcm_NegativeResponseCodeType ErrorCode = DCM_DEFAULT_VALUE;
    const Dcm_Dsld_ServiceType* service_pcs = &(Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8]);
    Std_ReturnType VerificationResult_u8 = Dcm_Prv_DsdVerifySubFncID(&idxSubservice_u8,service_pcs,&ErrorCode);
    /* local pointer to sub-service configuration structure*/
    const Dcm_Dsld_SubServiceType* adrSubservice_pcst =
            (Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8]).ptr_subservice_table_pcs;

    if(VerificationResult_u8 == E_OK)
    {
        VerificationResult_u8 = E_NOT_OK; /* Set it to E_NOT_OK again */
        /* Generate bit mask for active session */
        dataActiveMask_u32 = (uint32) (DCM_DEFAULT_MASKVALUE << Dcm_DsldGlobal_st.idxActiveSession_u8);
        /* Check requested sub service is allowed in this session */
        if ((adrSubservice_pcst[idxSubservice_u8].allowed_session_b32 & dataActiveMask_u32) == (dataActiveMask_u32))
        {
            /* generate bit mask for active security level */
            dataActiveMask_u32 = (uint32) (DCM_DEFAULT_MASKVALUE << Dcm_DsldGlobal_st.idxActiveSecurity_u8);
            /* Check if the requested sub-service is allowed in current security */
            if ((adrSubservice_pcst[idxSubservice_u8].allowed_security_b32 & dataActiveMask_u32)
                    == (dataActiveMask_u32))
            {
                /* DcmAppl_UserSubServiceModeRuleService will be invoked in case no mode rule is configured */
                VerificationResult_u8 = (*adrSubservice_pcst[idxSubservice_u8].adrUserSubServiceModeRule_pfct)
                      (&ErrorCode,Dcm_DsldGlobal_st.dataSid_u8, adrSubservice_pcst[idxSubservice_u8].subservice_id_u8);
                /* If the function returns any other value other than E_OK or a set NRC value */
                ErrorCode=((VerificationResult_u8 != E_OK) && (ErrorCode == 0x00))?DCM_E_CONDITIONSNOTCORRECT:ErrorCode;
#if(DCM_CFG_DSD_MODERULESUBFNC_ENABLED != DCM_CFG_OFF)
                /* Check if the mode rule is configured for the sub function */
                if((adrSubservice_pcst[idxSubservice_u8].moderule_fp != (Dcm_ModeRuleType)NULL_PTR) &&
                   (VerificationResult_u8==E_OK))
                {
                    /* Call the mode rule API configured */
                    VerificationResult_u8 = ((*adrSubservice_pcst[idxSubservice_u8].moderule_fp)
                                            (&ErrorCode) == TRUE)?E_OK:E_NOT_OK;
                }
#endif
            }
            /* If sub service is not allowed in  current security */
            else
            {
                ErrorCode = DCM_E_SECURITYACCESSDENIED;
            }
        }
        /* If sub service is not allowed in current session */
        else
        {
            ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;
        }
    }
#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF ) && (DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF ))
    /*Check if Error Code is supported for the requested service shall have sid 0x27 and servicelocator is set to True*/
    if((ErrorCode!=0x00u) && (service_pcs->sid_u8==DCM_SECURITYACCESS_SID) && (service_pcs->servicelocator_b!=FALSE))
    {
        /* To Reset the stored AccessType when the NRC is returned for Seca Service(0x27) from DsdStateMachine*/
        Dcm_ResetAccessType();

    }
#endif
    if(VerificationResult_u8 != E_OK)
    {
        Dcm_Prv_DsdSendNegativeResponse(ErrorCode);
    }
    return VerificationResult_u8;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDServiceTableInit
 Syntax           : Dcm_Prv_OBDServiceTableInit()
 Description      : Function to Initialize the OBD service table
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
LOCAL_INLINE void Dcm_Prv_OBDServiceTableInit(void)
{
    Dcm_MsgItemType* adrRxBuffer_pu8;
    const Dcm_Dsld_protocol_tableType* adrActiveProtocolTable_pcst =
            &(Dcm_DsldProtocol_pcst[Dcm_OBDGlobal_st.idxCurrentProtocol_u8]);

    /* Pointer to the Tx buffer */
    Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8 = adrActiveProtocolTable_pcst->tx_buffer_pa;
    /* Fill the maximum possible response length */
    Dcm_OBDMsgContext_st.resMaxDataLen = adrActiveProtocolTable_pcst->tx_buffer_size_u32 - DCM_SID_LENGTH;
    adrRxBuffer_pu8 = Dcm_DsldProtocol_pcst[Dcm_OBDGlobal_st.idxCurrentProtocol_u8].rx_mainBuffer_pa;
    /* Data response given by DSD is set to False */
    Dcm_OBDGlobal_st.dataResponseByDsd_b = FALSE;
    /* Store SID in a global variable */
    Dcm_OBDGlobal_st.dataSid_u8 = adrRxBuffer_pu8[DCM_REQUESTBUFFER_INDEX];
    /* Index of requested service is initialised to zero */
    Dcm_OBDGlobal_st.idxService_u8 = DCM_DEFAULT_VALUE;
    /* Make the Positive response as the default response */
    Dcm_OBDGlobal_st.stResponseType_en = DCM_DSLD_POS_RESPONSE;
    /* Response length (filled by the service) */
    Dcm_OBDMsgContext_st.resDataLen = DCM_DEFAULT_VALUE;
    Dcm_OBDTransmit_st.TxResponseLength_u32 = DCM_DEFAULT_VALUE;
    Dcm_OBDMsgContext_st.dcmRxPduId = Dcm_OBDGlobal_st.dataActiveRxPduId_u8;
    /* Fill the addressing mode info (physical or functional) */
    Dcm_OBDMsgContext_st.msgAddInfo.reqType = (Dcm_OBDGlobal_st.dataActiveRxPduId_u8 >= DCM_INDEX_FUNC_RX_PDUID)?
                                                DCM_PRV_FUNCTIONAL_REQUEST : DCM_PRV_PHYSICAL_REQUEST;
    /* Fill the request length excluding SID */
    Dcm_OBDMsgContext_st.reqDataLen = (Dcm_MsgLenType) Dcm_OBDGlobal_st.dataRequestLength_u16 - DCM_SID_LENGTH;
    /* Assign the Rx buffer address excluding SID */
    Dcm_OBDMsgContext_st.reqData = &(adrRxBuffer_pu8[DCM_REQUESTBUFFER_INDEX+DCM_SID_LENGTH]);
    /* Assign the Tx buffer address */
    Dcm_OBDMsgContext_st.resData = &(Dcm_OBDGlobal_st.adrActiveTxBuffer_tpu8[DCM_RESPONSEBUFFER_INDEX]);
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdServiceTableInit
 Syntax           : Dcm_Prv_DsdServiceTableInit()
 Description      : Function to Initialize the service table
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
/* Service Table Initialization */
LOCAL_INLINE void Dcm_Prv_DsdServiceTableInit(void)
{
    Dcm_MsgItemType* adrRxBuffer_pu8;
    const Dcm_Dsld_protocol_tableType* adrActiveProtocolTable_pcst;

#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    /* Is it ROE requested service */
    if(Dcm_Prv_GetDslState() == DSL_STATE_ROETYPE1_RECEIVED)
    {
        /* Pointer Tx buffer */
        Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8 = Dcm_DsldRoe_pcst->txbuffer_ptr;

        /* Fill the maximum possible response length */
        Dcm_DsldMsgContext_st.resMaxDataLen = Dcm_DsldRoe_pcst->txbuffer_length_u32 - DCM_SID_LENGTH;

        /* Get the Rx buffer */
        adrRxBuffer_pu8 = Dcm_DsldRoe_pcst->txbuffer_ptr;

        /* Start ROE timer */
        DCM_TimerStart(Dcm_DsldGlobal_st.dataTimerTimeout_u32,DCM_CFG_GET_TIMEOUT,          \
                        Dcm_TimerStartTick_u32,Dcm_CounterValueTimerStatus_uchr)
    }
    else
#endif
    {
        /* Get the address of protocol table to local pointer */
        adrActiveProtocolTable_pcst = &(Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8]);

        /* Pointer to the Tx buffer */
        Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8 = adrActiveProtocolTable_pcst->tx_buffer_pa;

        /* Fill the maximum possible response length */
        Dcm_DsldMsgContext_st.resMaxDataLen = adrActiveProtocolTable_pcst->tx_buffer_size_u32 - DCM_SID_LENGTH;

        /* tester requested service */
        Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest = DCM_UDS_TESTER_SOURCE;

        adrRxBuffer_pu8 = Dcm_GetActiveBuffer();
    }
    /*Data response given by DSD is set to False*/
    Dcm_DsldGlobal_st.dataResponseByDsd_b = FALSE;
    /* Store SID in a global variable */
    Dcm_DsldGlobal_st.dataSid_u8 = adrRxBuffer_pu8[DCM_REQUESTBUFFER_INDEX];
    /* Index of requested service is initialised to zero */
    Dcm_DsldGlobal_st.idxService_u8 = DCM_DEFAULT_VALUE;
    /*  Make the Positive response as the default response */
    Dcm_DsldGlobal_st.stResponseType_en = DCM_DSLD_POS_RESPONSE;
    /* Response length (filled by the service) */
    Dcm_DsldMsgContext_st.resDataLen = DCM_DEFAULT_VALUE;
    Dcm_DslTransmit_st.TxResponseLength_u32 = DCM_DEFAULT_VALUE;
    /* Fill the message context structure with proper data */
    /* Multicore: No locking necessary as Dcm_DsldGlobal_st.dataActiveRxPduId_u8 is an atomic variable and
     there is no parallel writing due to DSL state machine handling */
    Dcm_DsldMsgContext_st.dcmRxPduId = Dcm_DsldGlobal_st.dataActiveRxPduId_u8;
    /* Fill the addressing mode info (physical or functional) */
    Dcm_DsldMsgContext_st.msgAddInfo.reqType = (Dcm_DsldGlobal_st.dataActiveRxPduId_u8 >= DCM_INDEX_FUNC_RX_PDUID) ?
                                                DCM_PRV_FUNCTIONAL_REQUEST : DCM_PRV_PHYSICAL_REQUEST;
    /* Fill the request length excluding SID */
    Dcm_DsldMsgContext_st.reqDataLen = (Dcm_MsgLenType) Dcm_DsldGlobal_st.dataRequestLength_u16 - DCM_SID_LENGTH;
    /* Assign the Rx buffer address excluding SID */
    Dcm_DsldMsgContext_st.reqData = &(adrRxBuffer_pu8[DCM_REQUESTBUFFER_INDEX+DCM_SID_LENGTH]);
    /* assign the Tx buffer address */
    Dcm_DsldMsgContext_st.resData = &(Dcm_DsldGlobal_st.adrActiveTxBuffer_tpu8[DCM_RESPONSEBUFFER_INDEX]);
    /*Resetting the SuppressPosResponce flag to False*/
    Dcm_DsldMsgContext_st.msgAddInfo.suppressPosResponse = FALSE;

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckSuppressPositiveResponseforRC
 Syntax           : Dcm_Prv_CheckSuppressPositiveResponse()
 Description      : Helper Function to check if suppress positive response is needed
                     If yes , then do the necessary changes for RoutineControl service
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_CheckSuppressPositiveResponseforRC(void)
{
    if(Dcm_DsldGlobal_st.dataSid_u8==DCM_DSP_SID_ROUTINECONTROL)
    {
        /* Is Suppress Positive Response requested , set suppress positive response info to TRUE
         * And mask this info from request data*/

        if(((*Dcm_DsldMsgContext_st.reqData) & (DCM_SUPPRESPOSITIVERESP_MASK)) == DCM_SUPPRESPOSITIVERESP_MASK)
        {
            Dcm_DsldMsgContext_st.msgAddInfo.suppressPosResponse = TRUE;
            /* remove the MSB(suppress positive bit) of sub function */
            (*Dcm_DsldMsgContext_st.reqData) = ((*Dcm_DsldMsgContext_st.reqData) & DCM_REMOVESUPPRESSRESPONSEBIT_MASK);
        }

    }

}
#endif /* DCM_DSD_PRV_H */

