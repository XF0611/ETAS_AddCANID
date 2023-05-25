

#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"

#if(DCM_CFG_DSP_REPORTDTC_OR_MIRRORMEMORYDTC_OR_USERDEFINEDDTC_EXTENDEDDATARECORDBYDTCNUMBER != DCM_CFG_OFF)

#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
static uint8 s_RDTCSubFunction_u8;                    /* Variable to store sub function */

static Std_ReturnType s_SubfunctionReturnValue_u8;    /* Variable to store subservice return value */
static uint8 s_DTCStatus_Rdtc_u8;                          /* Variable to store DTC status */
static uint8 s_MaxRecordstoRead_u8;
static Dcm_NegativeResponseCodeType s_NegResponseCode_u8; /* Variable to hold NRC when re-enabling is needed */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"

static Dem_DTCOriginType s_DemDTCOrigin_Rdtc_u16;
#define DCM_STOP_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"

/* BSW-15209 */
#define DCM_START_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
static boolean s_isExtendedDataRecordSupported_b = FALSE; /* Variable to check whether record is supported */
#define DCM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
/* END BSW-15209 */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"



/***********************************************************************************************************************
 Function name    : Dcm_Prv_EnableRecordUpdate
 Description      : Enables the memory update of the DTC disabled by Dem_DisableDTCRecordUpdate() before.
 Parameter        : dataNegRespCode_u8
 Return value     : void
***********************************************************************************************************************/
static void Dcm_Prv_EnableRecordUpdate(Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType EnableRecordUpdateResult_u8 = E_NOT_OK;

    *dataNegRespCode_u8 = 0x00;

    /* Call the API to Enable Record update */
    EnableRecordUpdateResult_u8 = Dem_EnableDTCRecordUpdate(ClientId_u8);

    /* DTC record update enabled successfully */
    if(E_OK == EnableRecordUpdateResult_u8)
    {
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_INITIAL;
        s_isExtendedDataRecordSupported_b = FALSE;
        s_SubfunctionReturnValue_u8 = E_OK;
    }
    else if(DEM_PENDING == EnableRecordUpdateResult_u8)
    {
        /* Call API again in the next main cycle. */
    }
    else
    {
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(EnableRecordUpdateResult_u8);
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ReportDTCRecords
 Description      : Reads the ExtendedDataRecords for the requested DTC selected by API Dem_SelectExtendedDataRecord.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3844]
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3845]
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3810]
***********************************************************************************************************************/
static void Dcm_Prv_ReportDTCRecords(Dcm_MsgContextType *pMsgContext,
                                                Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    boolean isNoSuchElemetReturned_b = FALSE;        /* variable to check whether DEM_NO_SUCH_ELEMENT returned by Dem*/
    boolean isPendingReturned_b = FALSE;             /* Variable to check whether Pending is returned */
    Std_ReturnType GetNextExtendedDataResult_u8 = E_NOT_OK;/* Variable to hold return value of API
                                                               Dem_GetNextExtendedDataRecord */
    uint8 LoopIndex_u8 = 0x00;                       /* Loop counter index for record numbers */
    uint16 DTCExtendedDataSize_u16;                  /* Variable to hold Extended Data size */
    Dcm_MsgLenType FilledResponseLength_u32;         /* Variable to store the response length filled */

    *dataNegRespCode_u8 = 0x00;

    /* Loop through the maximum number of records that can be read at a time */
    do
    {
        LoopIndex_u8++;

        /* Update the response length to local variable */
        FilledResponseLength_u32 = pMsgContext->resDataLen;

        /* Calculate the remaining buffer length available */
        DTCExtendedDataSize_u16 = (uint16)(pMsgContext->resMaxDataLen - FilledResponseLength_u32);

        GetNextExtendedDataResult_u8 = Dem_GetNextExtendedDataRecord(ClientId_u8,
                                                                  &pMsgContext->resData[FilledResponseLength_u32],
                                                                  &DTCExtendedDataSize_u16);
        if(E_OK == GetNextExtendedDataResult_u8)
        {
            /* If API returns E_OK and Bufsize 0(empty buffer), Dcm shall omit the DTCExtendedDataRecordNumber for the
             * related record in the response. */
            if(DTCExtendedDataSize_u16 > 0x00)
            {
                /* Calculate the total response length */
                pMsgContext->resDataLen = (DTCExtendedDataSize_u16 + FilledResponseLength_u32);
            }
            s_isExtendedDataRecordSupported_b = TRUE;
            Dcm_DspRDTCRecordNum_u8++;
        }
        else if(DEM_PENDING == GetNextExtendedDataResult_u8)
        {
            /* Call API again in the next main cycle. */
            isPendingReturned_b = TRUE;
        }
        else if(DEM_NO_SUCH_ELEMENT == GetNextExtendedDataResult_u8)
        {
            /* When Dem returns DEM_NO_SUCH_ELEMENT Dcm should stop calling this API again. */
            isNoSuchElemetReturned_b = TRUE;
            break;
        }
        else
        {
            /* In case of any negative return value call API */
            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(GetNextExtendedDataResult_u8);
        }

    }while((0x00 == *dataNegRespCode_u8)&&(FALSE == isPendingReturned_b)&& \
           (LoopIndex_u8 < DCM_CFG_RDTC_MAXNUMRECREAD)&&(Dcm_DspRDTCRecordNum_u8 <= s_MaxRecordstoRead_u8));

    if((0x00 == *dataNegRespCode_u8)&&(FALSE == isPendingReturned_b)&& \
       ((isNoSuchElemetReturned_b == TRUE)||(Dcm_DspRDTCRecordNum_u8 > s_MaxRecordstoRead_u8)))
    {
        if(s_isExtendedDataRecordSupported_b == TRUE)
        {
            /* If all the requested DTCExtendedData records read, change state to enable the record update. */
            RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_ENABLERECORDUPDATE;
        }
        else
        {
            /* TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager-8390] */
            /* If none of the requested DTCExtendedData record is supported or Dem returns DEM_NO_SUCH_ELEMENT when the
             * DTCExtendedData record number is other than 0xFF or 0xFE, then Dcm should send NRC 0x31 */
            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
        }
    }
    if(*dataNegRespCode_u8 != 0x00)
    {
        s_NegResponseCode_u8 = *dataNegRespCode_u8;
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_REENABLERECORDUPDATE;
    }

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_NumberOfRecordstoReport
 Description      : Gets the number of ExtendedDataRecords to report based on the DTCExendedtDataRecordNumber.
                    DTCExtendedtDataRecordNumber 0x00 and 0xF0 to 0xFD not supported.
                    if DTCExendedtDataRecordNumber is 0xFF need to report all stored DTCExtendedData records.
                    if DTCExendedtDataRecordNumber is 0xFE need to report OBD stored DTCExtendedData records
                    (0x90 to 0xEF).
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
static void Dcm_Prv_NumberOfRecordstoReport(void)
{
    /* If the tester wants to read only one record of the DTC */
    /* TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3810] */
    if(Dcm_DspRDTCRecordNum_u8 <= 0xEF)
    {
        s_MaxRecordstoRead_u8 = Dcm_DspRDTCRecordNum_u8;
    }
    else if(0xFE == Dcm_DspRDTCRecordNum_u8)
    {
        /* Request is for the server to report legislated OBD DTC Extended Data records.
         * This is stored in Record Numbers 0x90 to 0xEF.
         *TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3922] */
        Dcm_DspRDTCRecordNum_u8 = 0x90;
        s_MaxRecordstoRead_u8 = 0xEF;
    }
    else
    {
        if(0xFF == Dcm_DspRDTCRecordNum_u8)
        {
            /* Request is for the server to report all Extended Data records.
             * This is stored in Record Numbers 0x01 to 0xEF */
            /* TRACE[SWS_Dcm_00296] */
            Dcm_DspRDTCRecordNum_u8 = 0x01;
            s_MaxRecordstoRead_u8 = 0xEF;
        }
    }
    /* Change to next state to read Extended Data Records */
    RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_REPORTDTCRECORDS;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckforSizeOfExtendedDataRecords
 Description      : API to get the size of ExtendedDataRecords for the requested DTC and DTCExtendedData record number.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 TRACE[SWS_Dcm_01242]
***********************************************************************************************************************/
static void Dcm_Prv_CheckforSizeOfExtendedDataRecords(const Dcm_MsgContextType *pMsgContext,
                                                      Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType ExtendedDataRecordSizeResult_u8 = E_NOT_OK;
    uint16 SizeOfExtendedDataRecord_u16 = 0x00;

    *dataNegRespCode_u8 = 0x00;

    ExtendedDataRecordSizeResult_u8 = Dem_GetSizeOfExtendedDataRecordSelection(ClientId_u8,
                                                                               &SizeOfExtendedDataRecord_u16);
    /* Size retrieved successfully */
    if(E_OK == ExtendedDataRecordSizeResult_u8)
    {
        if(pMsgContext->resMaxDataLen >= (Dcm_MsgLenType)(SizeOfExtendedDataRecord_u16 + pMsgContext->resDataLen))
        {
            Dcm_Prv_NumberOfRecordstoReport();
        }
        else
        {   /* Response length is too long */
            *dataNegRespCode_u8 = DCM_E_RESPONSETOOLONG;
        }
    }
    else if(DEM_PENDING == ExtendedDataRecordSizeResult_u8)
    {
        /* Retrieving size is still in progress. Call API again */
    }
    else if(DEM_NO_SUCH_ELEMENT == ExtendedDataRecordSizeResult_u8)
    {
        /* TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3790] */
        /* Record number is not supported */
        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
    }
    else
    {
        /* In case of any negative return value call API */
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(ExtendedDataRecordSizeResult_u8);
    }
    if(*dataNegRespCode_u8 != 0x00)
    {
        s_NegResponseCode_u8 = *dataNegRespCode_u8;
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_REENABLERECORDUPDATE;
    }

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_FillResponseMandatoryBytes
 Description      : API to fill the response buffer with mandatory fields of response.
                    -> If the requested subfunction is 0x06 or 0x10 the response must contains:
                       Subfunction, DTC and Status of DTC.
                    -> If the requested subfunction is 0x19, the response must contains:
                       Subfunction, MemorySelection, DTC and Status of DTC.
 Parameter        : pMsgContext
 Return value     : void
***********************************************************************************************************************/
static void Dcm_Prv_FillResponseMandatoryBytes(Dcm_MsgContextType *pMsgContext)
{
#if(DCM_CFG_DSP_RDTCSUBFUNC_0x19_ENABLED != DCM_CFG_OFF)
    if(DSP_REPORT_USER_DEFMEMORY_DTC_EXTDATA_RECORD_BY_DTC_NUMBER == s_RDTCSubFunction_u8)
    {
        /* Fill the response buffer */
        pMsgContext->resData[0x05] = s_DTCStatus_Rdtc_u8;
        pMsgContext->resData[0x04] = pMsgContext->reqData[DCM_DSP_RDTC_LOWBYTE];
        pMsgContext->resData[0x03] = pMsgContext->reqData[DCM_DSP_RDTC_MIDDLEBYTE];
        pMsgContext->resData[0x02] = pMsgContext->reqData[DCM_DSP_RDTC_HIGHBYTE];
        pMsgContext->resData[0x01] = (uint8)s_DemDTCOrigin_Rdtc_u16; /*As per ISO 14229_1 Response Data should be of 1Byte*/
        pMsgContext->resData[0x00] = s_RDTCSubFunction_u8;
        /* Update the Response data length information */
        pMsgContext->resDataLen = 0x06;
    }
#endif
#if((DCM_CFG_DSP_RDTCSUBFUNC_0x06_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_RDTCSUBFUNC_0x10_ENABLED != DCM_CFG_OFF))

    if((DSP_REPORT_EXTENDED_DATA_RECORD == s_RDTCSubFunction_u8)|| \
       (DSP_REPORT_EXTENDED_REC_MIRROR_MEM == s_RDTCSubFunction_u8))
    {
        /* Fill the response buffer */
        pMsgContext->resData[0x04] = s_DTCStatus_Rdtc_u8;
        pMsgContext->resData[0x03] = pMsgContext->reqData[DCM_DSP_RDTC_LOWBYTE];
        pMsgContext->resData[0x02] = pMsgContext->reqData[DCM_DSP_RDTC_MIDDLEBYTE];
        pMsgContext->resData[0x01] = pMsgContext->reqData[DCM_DSP_RDTC_HIGHBYTE];
        pMsgContext->resData[0x00] = s_RDTCSubFunction_u8;
        /* Update the Response data length information */
        pMsgContext->resDataLen = 0x05;
    }
#endif
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ExtendedDataRecordSelection
 Description      : API to select the ExtendedDataRecords that matches the requested DTC and
                    DTCExtendedData record number.
                    DTCExendedtDataRecordNumber 0x00 and 0xF0 to 0xFD not supported.
                    if DTCExendedtDataRecordNumber is 0xFF need to report all stored DTCExtendedData records.
                    if DTCExendedtDataRecordNumber is 0xFE need to report OBD stored DTCExtendedData records
                    (0x90 to 0xEF).
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
***********************************************************************************************************************/
static void Dcm_Prv_ExtendedDataRecordSelection(Dcm_MsgContextType *pMsgContext,
                                                Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType SelectExtendedDataRecordResult_u8 = E_NOT_OK;

    *dataNegRespCode_u8 = 0x00;

    /* Copy the record number from request */
    Dcm_DspRDTCRecordNum_u8 = pMsgContext->reqData[DCM_DSP_RDTC_EXTENDEDDATARECORDNUMBER];

    if((0x00 == Dcm_DspRDTCRecordNum_u8)|| \
       ((Dcm_DspRDTCRecordNum_u8 >= 0xF0) && (Dcm_DspRDTCRecordNum_u8 <= 0xFD)))
    {
        /* FC_VariationPoint_START */
        /* 0x00 is reserved for future OBD use */
        /* 0xF0 - 0xFD reserved for future use */
        /* FC_VariationPoint_END */
        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
    }
    else
    {
        SelectExtendedDataRecordResult_u8 = Dem_SelectExtendedDataRecord(ClientId_u8, Dcm_DspRDTCRecordNum_u8);

        if(E_OK == SelectExtendedDataRecordResult_u8)
        {
            /* Extended Data records selected successfully.
             * Call API to fill the response buffer with the mandatory fields */
            Dcm_Prv_FillResponseMandatoryBytes(pMsgContext);
            RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_GETRECORDSDATASIZE;
        }
        else if(DEM_PENDING == SelectExtendedDataRecordResult_u8)
        {
            /* Selecting the extended data record is still in progress. Call API again */
        }
        else
        {
            /* In case of any negative return value call API */
            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(SelectExtendedDataRecordResult_u8);
        }
    }
    if(*dataNegRespCode_u8 != 0x00)
    {
        s_NegResponseCode_u8 = *dataNegRespCode_u8;
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_REENABLERECORDUPDATE;
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DisableExtendedDataRecordUpdate
 Description      : Disables the memory update of a specific DTC.
 Parameter        : dataNegRespCode_u8
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3777]
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3785]
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3786]
***********************************************************************************************************************/
static void Dcm_Prv_DisableExtendedDataRecordUpdate(Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType DisableDTCRecordUpdateResult_u8 = E_NOT_OK; /*Return type from Dem_DisableDTCRecordUpdate*/

    *dataNegRespCode_u8 = 0x00;

    /* Disable the Record Update */
    DisableDTCRecordUpdateResult_u8 = Dem_DisableDTCRecordUpdate(ClientId_u8);

    if(E_OK == DisableDTCRecordUpdateResult_u8)
    {
        /* Update disabled successfully. Change to next state to select the DTCExtendedData records */
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_SELECTRECORDSDATA;
    }
    else if(DEM_PENDING == DisableDTCRecordUpdateResult_u8)
    {
        /* Disabling the record update is still in progress. Call the API again in next main cycle.
         * TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3778] */
    }
    else
    {
        /* In case of any negative return value call API */
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(DisableDTCRecordUpdateResult_u8);
    }

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_GetStatusOfDTC
 Description      : API to get the status of the requested DTC.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3791]
***********************************************************************************************************************/
static void Dcm_Prv_GetStatusOfDTC(Dcm_MsgContextType *pMsgContext,
                                                   Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType DTCStatusResult_u8 = E_NOT_OK;/* Variable to store the return value of API Dem_GetStatusOfDTC */

    *dataNegRespCode_u8 = 0x00;

    DTCStatusResult_u8 = Dem_GetStatusOfDTC(ClientId_u8, &s_DTCStatus_Rdtc_u8);

    if(E_OK == DTCStatusResult_u8)
    {
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_DISABLERECORDUPDATE;
    }
    else if(DEM_PENDING == DTCStatusResult_u8)
    {
        /* Call API again in the next main cycle. */
    }
    else if(DEM_NO_SUCH_ELEMENT == DTCStatusResult_u8)
    {
        /*In case of DEM_NO_SUCH_ELEMENT Dcm should send a positive response with SID and subfunction alone.
         * TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3793] */
        pMsgContext->resData[DCM_DSP_RDTC_SUBFUNCTION] = pMsgContext->reqData[DCM_DSP_RDTC_SUBFUNCTION];
        /* Update the Response data length information */
        pMsgContext->resDataLen = 0x01;
        s_SubfunctionReturnValue_u8 = E_OK;
    }
    else
    {
        /* In case of any negative return value call API */
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(DTCStatusResult_u8);
    }

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_GetDTCandDTCOrigin
 Description      : Reads the requested DTC and DTC origin.
                    -> If the requested subfunction is 0x06, DTC origin is PRIMARY_MEMORY
                    -> If the requested subfunction is 0x10, DTC origin is MIRROR_MEMORY
                    -> If the requested subfunction is 0x19, set MemorySelection parameter as DTC origin.
 Parameter        : pMsgContext
 Return value     : void
 **********************************************************************************************************************/
static void Dcm_Prv_GetDTCandDTCOrigin(const Dcm_MsgContextType *pMsgContext)
{

    /* Read 3 byte DTC from request */
    Dsp_RdtcDTC_u32 =  ((((uint32) pMsgContext->reqData[DCM_DSP_RDTC_HIGHBYTE]) << DSP_RDTC_LSH_16BITS) |
                        (((uint32) pMsgContext->reqData[DCM_DSP_RDTC_MIDDLEBYTE]) << DSP_RDTC_LSH_08BITS) |
                         ((uint32) pMsgContext->reqData[DCM_DSP_RDTC_LOWBYTE]));

    s_DemDTCOrigin_Rdtc_u16 = DEM_DTC_ORIGIN_PRIMARY_MEMORY;

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x19_ENABLED != DCM_CFG_OFF)
    if(DSP_REPORT_USER_DEFMEMORY_DTC_EXTDATA_RECORD_BY_DTC_NUMBER == s_RDTCSubFunction_u8)
    {
        s_DemDTCOrigin_Rdtc_u16 = pMsgContext->reqData[DCM_DSP_RDTC_MEMORYSELECTION] + DCM_DTC_ORIGIN_MEMORY_SELECTION_VALUE;
    }
#endif

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x10_ENABLED != DCM_CFG_OFF)
    if(DSP_REPORT_EXTENDED_REC_MIRROR_MEM == s_RDTCSubFunction_u8)
    {
        s_DemDTCOrigin_Rdtc_u16 = DEM_DTC_ORIGIN_MIRROR_MEMORY;
    }
#endif

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_RequestLengthandSelectDTC
 Description      : 1) Get the request length. If length is correct then read DTC and DTCOrigin.
                        -> If the requested subfunction is 0x06 or 0x10, request length is 5 Bytes.
                           (Subfunction, 3 Bytes of DTC(DTCMaskRecord) and DTCExtDataRecordNumber)
                        -> If the requested subfunction is 0x19, request length is 6 Bytes.
                           (Subfunction, 3 Bytes of DTC(DTCMaskRecord), DTCExtDataRecordNumber and MemorySelection)
                    2) Selects a DTC.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 **********************************************************************************************************************/
static void Dcm_Prv_RequestLengthandSelectDTC(const Dcm_MsgContextType *pMsgContext,
                                              Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint8 RequestLength_u8 = 0x00;         /* Variable to store request length */
    Std_ReturnType SelectDTC_u8 = DEM_BUSY;    /* variable to store the return value of API Dem_SelectDTC */

    *dataNegRespCode_u8 = 0x00;
    s_RDTCSubFunction_u8 = pMsgContext->reqData[DCM_DSP_RDTC_SUBFUNCTION];

#if((DCM_CFG_DSP_RDTCSUBFUNC_0x06_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_RDTCSUBFUNC_0x10_ENABLED != DCM_CFG_OFF))
    if((DSP_REPORT_EXTENDED_DATA_RECORD == s_RDTCSubFunction_u8)|| \
       (DSP_REPORT_EXTENDED_REC_MIRROR_MEM == s_RDTCSubFunction_u8))
    {
        /* Update the request length */
        RequestLength_u8 = DSP_RDTC_04_06_10_REQUEST_LENGTH;
    }
#endif

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x19_ENABLED != DCM_CFG_OFF)
    if(DSP_REPORT_USER_DEFMEMORY_DTC_EXTDATA_RECORD_BY_DTC_NUMBER == s_RDTCSubFunction_u8)
    {
        /* Update the request length */
        RequestLength_u8 = DSP_RDTC_18_19_REQUEST_LENGTH;
    }
#endif

    /* Check if request length is correct */
    if(RequestLength_u8 == pMsgContext->reqDataLen)
    {
        /* Get requested DTC and set DTC origin */
        Dcm_Prv_GetDTCandDTCOrigin(pMsgContext);

        /* Call API to select a DTC.*/
        SelectDTC_u8 = Dem_SelectDTC(ClientId_u8, Dsp_RdtcDTC_u32, DEM_DTC_FORMAT_UDS, s_DemDTCOrigin_Rdtc_u16);

        /* If DTC selected successfully */
        if(E_OK == SelectDTC_u8)
        {
            /* TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3843] */
            RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_STATUSOFDTC;
        }
        /* If different Select operation is currently in progress i.e; API returns DEM_BUSY */
        else
        {
            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(SelectDTC_u8);
        }

    }
    /* Request Length not correct */
    else
    {
        *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
}

/***************************************************************************************************
 Function name    : Dcm_Prv_ReEnableRecordUpdate
 Description      : Function to handle Enable DTC record update
                    Needed when Negative response is set after successful disable DTC record
 Parameter        : dataNegRespCode_u8
 Return value     : Std_ReturnType
 ***************************************************************************************************/
static Std_ReturnType Dcm_Prv_ReEnableRecordUpdate(Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType Result_u8;
    // Reset NRC as we dont want to trigger response before compeltion of enable DTC
    *dataNegRespCode_u8 = 0x00u;

    Result_u8 = Dem_EnableDTCRecordUpdate(ClientId_u8);
    if(Result_u8 == E_OK)
    {
        // set stored NRC to dataNegRespCode_u8 and trigger final negative response
        *dataNegRespCode_u8 = s_NegResponseCode_u8;
    }
    else if(Result_u8 == DEM_PENDING)
    {
        Result_u8 = DCM_E_PENDING;
    }
    else
    {
        // log DET, this condition should neer occur, DTC Record update is still disabled
        DCM_DET_ERROR(DCM_RDTC_ID,DCM_E_ENABLEDTCRECORD_FAILED);
        /* NRC is needed as that will terminate the service */
        *dataNegRespCode_u8 = s_NegResponseCode_u8;
        Result_u8 = E_NOT_OK;
    }
    return Result_u8;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_StatestoReportExtendedData
 Description      : Different states involved in reporting DTCExtendedData records.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 **********************************************************************************************************************/
static void Dcm_Prv_StatestoReportExtendedData(Dcm_MsgContextType *pMsgContext,
                                               Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    /* Initial state to check for request length and also to select the DTC send by the tester */
    if(DCM_RDTC_SUBFUNCTION_INITIAL == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_RequestLengthandSelectDTC(pMsgContext, dataNegRespCode_u8);
    }

    /* State to get the status of DTC send by tester */
    if(DCM_RDTC_SUBFUNCTION_STATUSOFDTC == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_GetStatusOfDTC(pMsgContext, dataNegRespCode_u8);
    }

    /* State to disable the memory update of the selected DTC */
    if(DCM_RDTC_SUBFUNCTION_DISABLERECORDUPDATE == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_DisableExtendedDataRecordUpdate(dataNegRespCode_u8);
    }

    /* State to select ExtendedDataRecords that matches the requested DTC and the DTCExtendedData record number */
    if(DCM_RDTC_SUBFUNCTION_SELECTRECORDSDATA == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_ExtendedDataRecordSelection(pMsgContext, dataNegRespCode_u8);
    }

    /* State to get size of DTCExtendedData related to requested DTC and DTCExtendedData record number */
    if(DCM_RDTC_SUBFUNCTION_GETRECORDSDATASIZE == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_CheckforSizeOfExtendedDataRecords(pMsgContext, dataNegRespCode_u8);
    }

    /* State to report DTCExtendedData related to requested DTC and DTCExtendedData record number */
    if(DCM_RDTC_SUBFUNCTION_REPORTDTCRECORDS == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_ReportDTCRecords(pMsgContext, dataNegRespCode_u8);
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Dsp_ReportExtendedDataRecordByDTCNumber
 Description      : API to report ExtendedD.ataRecords for the rquested DTC and DTCExtendedDataRecordNumber.
 Parameter        : Opstatus
                    pMsgContext
                    dataNegRespCode_u8
 Return value     : E_OK
                    E_NOT_OK
                    DCM_E_PENDING
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3810]
***********************************************************************************************************************/
Std_ReturnType Dcm_Dsp_ReportExtendedDataRecordByDTCNumber(Dcm_SrvOpStatusType Opstatus,
                                                           Dcm_MsgContextType *pMsgContext,
                                                           Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{

    *dataNegRespCode_u8 = 0x00;
    s_SubfunctionReturnValue_u8 = DCM_E_PENDING;

    DCM_UNUSED_PARAM(Opstatus);

    /* State machine */
    /* Different states involved in reporting ExtendedDataRecords for requested DTC and DTCExtendedDataRecordNumber */
    Dcm_Prv_StatestoReportExtendedData(pMsgContext, dataNegRespCode_u8);

    /* State to enable DTC record update */
    if(DCM_RDTC_SUBFUNCTION_ENABLERECORDUPDATE == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_EnableRecordUpdate(dataNegRespCode_u8);
    }

    // To enable DTC record update in case of negative response
    // Separate state to handle asynchronous behavior of Enable DTC record update function
    if(DCM_RDTC_SUBFUNCTION_REENABLERECORDUPDATE == RDTCExtendedandSnapshotDataState_en)
    {
        s_SubfunctionReturnValue_u8 = Dcm_Prv_ReEnableRecordUpdate(dataNegRespCode_u8);
    }

    if(*dataNegRespCode_u8 != 0u)
    {
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_INITIAL;
        s_SubfunctionReturnValue_u8 = E_NOT_OK;
    }

    return s_SubfunctionReturnValue_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif
