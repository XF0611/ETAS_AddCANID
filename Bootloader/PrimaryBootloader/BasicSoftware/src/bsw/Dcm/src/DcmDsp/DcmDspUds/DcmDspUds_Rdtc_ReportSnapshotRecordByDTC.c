

#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"

#if(DCM_CFG_DSP_REPORTDTC_OR_USERDEFINEDMEMORYDTC_SNAPSHOTRECORDBYDTCNUMBER != DCM_CFG_OFF)

#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
static uint8 s_SnapShotRecordRDTCSubFunction_u8;                    /* Variable to store sub function */

static Std_ReturnType s_SubServiceReturnValue_u8;    /* Variable to store subservice return value */
static uint8 s_DTCStatus_u8;                          /* Variable to store DTC status */
static uint8 s_MaxRecordstoRead_Rdtc_u8;
static Dcm_NegativeResponseCodeType s_NegResponse_u8; /* Variable to hold NRC when re-enabling is needed */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
static Dem_DTCOriginType s_DemDTCOriginMemory_u16;           /* Variable to store DTC origin */
#define DCM_STOP_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"

/* BSW-15209 */
#define DCM_START_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
static boolean s_isFreezeFrameRecordSupported_b = FALSE;/* Variable to check whether record is supported */
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
        s_isFreezeFrameRecordSupported_b = FALSE;
        s_SubServiceReturnValue_u8 = E_OK;
    }
    else if(DEM_PENDING == EnableRecordUpdateResult_u8)
    {
        /* Call API again in the next main cycle. */
    }
    else
    {
        /* In case of any negative return value call API */
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(EnableRecordUpdateResult_u8);
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ReportDTCSnapshotRecords
 Description      : Reads the DTCSnapshot data records for the requested DTC selected by API Dem_SelectFreezeFrameData.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3853]
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3854]
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3927]
***********************************************************************************************************************/
static void Dcm_Prv_ReportDTCSnapshotRecords(Dcm_MsgContextType *pMsgContext,
                                                   Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    boolean isDemNoSuchElementReturned_b = FALSE;   /* Variable to check whether DEM_NO_SUCH_ELEMENT returned by DEM */
    boolean isPendingReturned_b = FALSE;            /* Variable to check whether DEM_PENDING returned by Dem */
    uint8 LoopIndex_u8 = 0x00;                      /* Loop counter index for record numbers*/
    uint16 DTCFreezeFrameDataSize_u16;              /* Variable to hold Freeze Frame Data size */
    Dcm_MsgLenType FilledResponseLength_u32;        /* Local variable to store the response length filled*/
    Std_ReturnType GetNextSnapshotRecordResult_u8 = E_NOT_OK;  /* Variable to hold return value of API
                                                                   Dem_GetNextFreezeFrameData*/

    *dataNegRespCode_u8 = 0x00;

    /* Loop through the maximum number of records that can be read at a time */
    do
    {
        LoopIndex_u8++;

        /* Update the response length to local variable */
        FilledResponseLength_u32 = pMsgContext->resDataLen;

        /* Calculate the remaining buffer length available */
        DTCFreezeFrameDataSize_u16 = (uint16)(pMsgContext->resMaxDataLen - FilledResponseLength_u32);

        GetNextSnapshotRecordResult_u8 = Dem_GetNextFreezeFrameData(ClientId_u8,
                                                                    &pMsgContext->resData[FilledResponseLength_u32],
                                                                    &DTCFreezeFrameDataSize_u16);
        if(E_OK == GetNextSnapshotRecordResult_u8)
        {
            /* If at least one byte of data is returned from DEM */
            if(DTCFreezeFrameDataSize_u16 > 0x00)
            {
                /* Calculate the total response length */
                pMsgContext->resDataLen = (DTCFreezeFrameDataSize_u16 + FilledResponseLength_u32);
            }
            s_isFreezeFrameRecordSupported_b = TRUE;
            Dcm_DspRDTCRecordNum_u8++;

        }
        else if(DEM_PENDING == GetNextSnapshotRecordResult_u8)
        {
            /* Call API again in the next main cycle. */
            isPendingReturned_b = TRUE;
        }
        else if(DEM_NO_SUCH_ELEMENT == GetNextSnapshotRecordResult_u8)
        {
            isDemNoSuchElementReturned_b = TRUE;
            break;
        }
        else
        {
            /* In case of any negative return value call API */
            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(GetNextSnapshotRecordResult_u8);
        }

    }while((0x00 == *dataNegRespCode_u8)&&(FALSE == isPendingReturned_b)&& \
           (LoopIndex_u8 < DCM_CFG_RDTC_MAXNUMRECREAD)&&(Dcm_DspRDTCRecordNum_u8 <= s_MaxRecordstoRead_Rdtc_u8));

    if((0x00 == *dataNegRespCode_u8)&&(FALSE == isPendingReturned_b)&& \
            ((isDemNoSuchElementReturned_b == TRUE)||(Dcm_DspRDTCRecordNum_u8 > s_MaxRecordstoRead_Rdtc_u8)))
    {
        if(s_isFreezeFrameRecordSupported_b == TRUE)
        {
            /* If all the requested record numbers read, change to next state to enable the record update */
            RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_ENABLERECORDUPDATE;
        }
        else
        {
            /* TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3855] */
            /* If none of the requested freeze frame data is supported or Dem returns DEM_NO_SUCH_ELEMENT when the
             * requested DTCSnapshot record number is other than 0xFF(Single Freeze Frame data record), then
             * Dcm should send NRC 0x31 */
            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
        }
    }
    if(*dataNegRespCode_u8 != 0x00)
    {
        s_NegResponse_u8 = *dataNegRespCode_u8;
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_REENABLERECORDUPDATE;
    }

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_NumberOfRecordstoReport
 Description      : Gets the number of Snapshotrecords to report based on the DTCSnapshot record number.
                    DTCExendedtDataRecordNumber 0x00 and 0xF0 to 0xFD not supported.
                    if DTCSnapshot record number is 0xFF, need to report all stored DTCSnapshot records data.
                    if DTCSnapshot record number is in range 0x01 to 0xFE, need to report DTCSnapshot record data only
                    for the requested DTCSnapshot record number.
 Parameter        : void
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3929]
***********************************************************************************************************************/
static void Dcm_Prv_NumberOfRecordstoReport(void)
{
    if(0xFF == Dcm_DspRDTCRecordNum_u8)
    {
        /* Request is for the server to report all DTCSnapshot Data records.
         * This is stored in Record Numbers 0x01 to 0xEF */
        Dcm_DspRDTCRecordNum_u8 = 0x01;
        s_MaxRecordstoRead_Rdtc_u8 = 0xFE;
    }
    else
    {
        /* Tester wants to read only one DTCSnapshot data record */
        s_MaxRecordstoRead_Rdtc_u8 = Dcm_DspRDTCRecordNum_u8;
    }

    /* Change to next state to read DTCSnapshot Data Records */
    RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_REPORTDTCRECORDS;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_CheckforSizeOfSnapshotRecords
 Description      : API to get the size of Freeze Frame data for the requested DTC.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3852]
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3856]
***********************************************************************************************************************/
static void Dcm_Prv_CheckforSizeOfSnapshotRecords(const Dcm_MsgContextType *pMsgContext,
                                                  Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType SnapshotRecordSizeResult_u8 = E_NOT_OK;
    uint16 SizeOfSnapshotRecord_u16 = 0x00;

    *dataNegRespCode_u8 = 0x00;

    SnapshotRecordSizeResult_u8 = Dem_GetSizeOfFreezeFrameSelection(ClientId_u8, &SizeOfSnapshotRecord_u16);

    if(E_OK == SnapshotRecordSizeResult_u8)
    {
        if(pMsgContext->resMaxDataLen >= (Dcm_MsgLenType)(SizeOfSnapshotRecord_u16 + pMsgContext->resDataLen))
        {
            /* Change to next state to read DTCSnapshot records data */
            Dcm_Prv_NumberOfRecordstoReport();
        }
        else
        {   /* Response length is too long */
            *dataNegRespCode_u8 = DCM_E_RESPONSETOOLONG;
        }
    }
    else if(DEM_PENDING == SnapshotRecordSizeResult_u8)
    {
        /* Call the API again in the net main cycle. */
    }
    else if(DEM_NO_SUCH_ELEMENT == SnapshotRecordSizeResult_u8)
    {
        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
    }
    else
    {
        /* In case of any negative return value call API */
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(SnapshotRecordSizeResult_u8);
    }
    if(*dataNegRespCode_u8 != 0x00)
    {
        s_NegResponse_u8 = *dataNegRespCode_u8;
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_REENABLERECORDUPDATE;
    }

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_FillResponseMandatoryBytes
 Description      : API to fill the response buffer with mandatory fields like
                    Subfunction, DTC and DTC status for subfunction 0x04
                    Subfunction, DTCOrigin, DTC and DTC status for subfunction 0x18.
 Parameter        : pMsgContext
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3848]
***********************************************************************************************************************/
static void Dcm_Prv_FillResponseMandatoryBytes(Dcm_MsgContextType *pMsgContext)
{
#if(DCM_CFG_DSP_RDTCSUBFUNC_0x18_ENABLED != DCM_CFG_OFF)
    if(DSP_REPORT_USER_DEFMEMORY_DTC_SNAPSHOT_RECORD_BY_DTC_NUMBER == s_SnapShotRecordRDTCSubFunction_u8)
    {
        /* Fill the response buffer */
        pMsgContext->resData[0x05] = s_DTCStatus_u8;
        pMsgContext->resData[0x04] = pMsgContext->reqData[DCM_DSP_RDTC_LOWBYTE];
        pMsgContext->resData[0x03] = pMsgContext->reqData[DCM_DSP_RDTC_MIDDLEBYTE];
        pMsgContext->resData[0x02] = pMsgContext->reqData[DCM_DSP_RDTC_HIGHBYTE];
        pMsgContext->resData[0x01] = (uint8)s_DemDTCOriginMemory_u16; /*As per ISO 14229_1 Response Data should be of 1Byte*/
        pMsgContext->resData[0x00] = s_SnapShotRecordRDTCSubFunction_u8;
        /* Update the Response data length information */
        pMsgContext->resDataLen = 0x06;
    }
#endif
#if(DCM_CFG_DSP_RDTCSUBFUNC_0x04_ENABLED != DCM_CFG_OFF)

    if(DSP_REPORT_SNAPSHOT_RECORD_BY_DTC == s_SnapShotRecordRDTCSubFunction_u8)
    {
        /* Fill the response buffer */
        pMsgContext->resData[0x04] = s_DTCStatus_u8;
        pMsgContext->resData[0x03] = pMsgContext->reqData[DCM_DSP_RDTC_LOWBYTE];
        pMsgContext->resData[0x02] = pMsgContext->reqData[DCM_DSP_RDTC_MIDDLEBYTE];
        pMsgContext->resData[0x01] = pMsgContext->reqData[DCM_DSP_RDTC_HIGHBYTE];
        pMsgContext->resData[0x00] = s_SnapShotRecordRDTCSubFunction_u8;
        /* Update the Response data length information */
        pMsgContext->resDataLen = 0x05;
    }
#endif
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_SnapshotRecordSelection
 Description      : API to select the Freeze Frame data that matches the requested DTC.
                    DTCExendedtDataRecordNumber 0x00 and 0xF0 to 0xFD not supported.
                    if DTCSnapshot record number is 0xFF, need to report all stored DTCSnapshot records data.
                    if DTCSnapshot record number is in range 0x01 to 0xFE, need to report DTCSnapshot record data only
                    for the requested DTCSnapshot record number.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3854]
***********************************************************************************************************************/
static void Dcm_Prv_SnapshotRecordSelection(Dcm_MsgContextType *pMsgContext,
                                            Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType SelectSnapshotRecordResult_u8 = E_NOT_OK;

    *dataNegRespCode_u8 = 0x00;

    /* Copy the record number from the request */
    Dcm_DspRDTCRecordNum_u8 = pMsgContext->reqData[DCM_DSP_RDTC_EXTENDEDDATARECORDNUMBER];

    SelectSnapshotRecordResult_u8 = Dem_SelectFreezeFrameData(ClientId_u8, Dcm_DspRDTCRecordNum_u8);

    if(E_OK == SelectSnapshotRecordResult_u8)
    {
        /* Call API to fill the response buffer with the mandatory fields */
        Dcm_Prv_FillResponseMandatoryBytes(pMsgContext);

        /* Change to next state to get the size of SnapshotRecords */
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_GETRECORDSDATASIZE;
    }
    else if(DEM_PENDING == SelectSnapshotRecordResult_u8)
    {
        /* Call the API again in the net main cycle. */
    }
    else
    {
        /* In case of any negative return value call API */
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(SelectSnapshotRecordResult_u8);
    }
    if(*dataNegRespCode_u8 != 0x00)
    {
        s_NegResponse_u8 = *dataNegRespCode_u8;
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_REENABLERECORDUPDATE;
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DisableSnapshotRecordUpdate
 Description      : Disables the event memory update of requested DTC.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3777],
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3778],
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3785],
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3786]
***********************************************************************************************************************/
static void Dcm_Prv_DisableSnapshotRecordUpdate(Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType DisableDTCRecordUpdateResult_u8 = E_NOT_OK; /*Return type from Dem_DisableDTCRecordUpdate*/

    *dataNegRespCode_u8 = 0x00;

    /* Disable the Record Update */
    DisableDTCRecordUpdateResult_u8 = Dem_DisableDTCRecordUpdate(ClientId_u8);

    if(E_OK == DisableDTCRecordUpdateResult_u8)
    {
        /* Update disabled successfully. Change to next state to select the DTCSnapshot records */
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_SELECTRECORDSDATA;
    }
    else if(DEM_PENDING == DisableDTCRecordUpdateResult_u8)
    {
        /* Call the API again in the net main cycle. */
    }
    else
    {
        /* In case of any negative return value call API */
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(DisableDTCRecordUpdateResult_u8);
    }

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_StatusOfDTC
 Description      : API to get the status of the requested DTC.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3850],
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3849],
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3791],
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3793]
***********************************************************************************************************************/
static void Dcm_Prv_StatusOfDTC(Dcm_MsgContextType *pMsgContext,
                                Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    Std_ReturnType DTCStatusResult_u8 = E_NOT_OK;

    *dataNegRespCode_u8 = 0x00;

    DTCStatusResult_u8 = Dem_GetStatusOfDTC(ClientId_u8, &s_DTCStatus_u8);

    if(E_OK == DTCStatusResult_u8)
    {
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_DISABLERECORDUPDATE;
    }
    else if(DEM_PENDING == DTCStatusResult_u8)
    {
        /* Call the API again in the net main cycle. */
    }
    else if(DEM_NO_SUCH_ELEMENT == DTCStatusResult_u8)
    {
        /*In case of DEM_NO_SUCH_ELEMENT Dcm should send a positive response with SID and subfunction alone*/
        pMsgContext->resData[DCM_DSP_RDTC_SUBFUNCTION] = pMsgContext->reqData[DCM_DSP_RDTC_SUBFUNCTION];
        /* Update the Response data length information */
        pMsgContext->resDataLen = 0x01;
        s_SubServiceReturnValue_u8 = E_OK;
    }
    else
    {
        /* In case of any negative return value call API */
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(DTCStatusResult_u8);
    }

}

/**
 **************************************************************************************************
 Function name    : Dcm_Prv_GetDTCandDTCOrigin
 Description      : Reads the requested DTC and DTC origin.
                    -> If the requested subfunction is 0x04, DTC origin is PRIMARY_MEMORY
                    -> If the requested subfunction is 0x18, set MemorySelection parameter as DTC origin.
 Parameter        : pMsgContext
 Return value     : void
 **************************************************************************************************
 */
static void Dcm_Prv_GetDTCandDTCOrigin(const Dcm_MsgContextType *pMsgContext)
{

    /* Read 3 byte DTC from request */
    Dsp_RdtcDTC_u32 =  ((((uint32) pMsgContext->reqData[DCM_DSP_RDTC_HIGHBYTE]) << DSP_RDTC_LSH_16BITS) |
                        (((uint32) pMsgContext->reqData[DCM_DSP_RDTC_MIDDLEBYTE]) << DSP_RDTC_LSH_08BITS) |
                         ((uint32) pMsgContext->reqData[DCM_DSP_RDTC_LOWBYTE]));

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x18_ENABLED != DCM_CFG_OFF)
    if(DSP_REPORT_USER_DEFMEMORY_DTC_SNAPSHOT_RECORD_BY_DTC_NUMBER == s_SnapShotRecordRDTCSubFunction_u8)
    {
        s_DemDTCOriginMemory_u16 = pMsgContext->reqData[DCM_DSP_RDTC_MEMORYSELECTION] + DCM_DTC_ORIGIN_MEMORY_SELECTION_VALUE;

    }
#endif

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x04_ENABLED != DCM_CFG_OFF)
    if(DSP_REPORT_SNAPSHOT_RECORD_BY_DTC == s_SnapShotRecordRDTCSubFunction_u8)
    {
        s_DemDTCOriginMemory_u16 = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
    }
#endif

}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_GetRequestLengthandSelectDTC
 Description      : 1) Get the request length. If length is correct, read DTC and DTCOrigin and select the DTC.
                        -> If the requested subfunction is 0x04, request length is 5 Bytes.
                           (Subfunction, 3 Bytes DTC(DTCMaskRecord) and DTCSnapshotRecordNumber)
                        -> If the requested subfunction is 0x18, request length is 6 Bytes.
                           (Subfunction, 3 Bytes DTC(DTCMaskRecord), DTCSnapshotRecordNumber and MemorySelection)
                    2) Selects a DTC after getting client information.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
***********************************************************************************************************************/
static void Dcm_Prv_GetRequestLengthandSelectDTC(const Dcm_MsgContextType *pMsgContext,
                                                 Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint8 RequestLength_u8 = 0x00u;  /* Variable to store request length */
    Std_ReturnType SelectDTC_u8 = DEM_BUSY;     /* variable to store the return value of API Dem_SelectDTC */

    *dataNegRespCode_u8 = 0x00;

    /* Get the requested subfunction */
    s_SnapShotRecordRDTCSubFunction_u8 = pMsgContext->reqData[DCM_DSP_RDTC_SUBFUNCTION];

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x04_ENABLED != DCM_CFG_OFF)
    if(DSP_REPORT_SNAPSHOT_RECORD_BY_DTC == s_SnapShotRecordRDTCSubFunction_u8)
    {
        /* Update the request length */
        RequestLength_u8 = DSP_RDTC_04_06_10_REQUEST_LENGTH;
    }
#endif

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x18_ENABLED != DCM_CFG_OFF)
    if(DSP_REPORT_USER_DEFMEMORY_DTC_SNAPSHOT_RECORD_BY_DTC_NUMBER == s_SnapShotRecordRDTCSubFunction_u8)
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

        /* Call API to select a DTC */
        SelectDTC_u8 = Dem_SelectDTC(ClientId_u8, Dsp_RdtcDTC_u32, DEM_DTC_FORMAT_UDS, s_DemDTCOriginMemory_u16);

        /* If DTC selected successfully */
        if(E_OK == SelectDTC_u8)
        {
            RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_STATUSOFDTC;
        }
        /* If different Select operation is currently in progress */
        else
        {
            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(SelectDTC_u8);
        }
    }
    /* Incorrect request length */
    else
    {
        *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }

}

/**
 **************************************************************************************************
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
        *dataNegRespCode_u8 = s_NegResponse_u8;
    }
    else if(Result_u8 == DEM_PENDING)
    {
        Result_u8 = DCM_E_PENDING;
    }
    else
    {
        // log DET, this condition should never occur, DTC Record update is still disabled
        DCM_DET_ERROR(DCM_RDTC_ID,DCM_E_ENABLEDTCRECORD_FAILED);
        /* NRC is needed as that will terminate the service */
        *dataNegRespCode_u8 = s_NegResponse_u8;
        Result_u8 = E_NOT_OK;
    }
    return Result_u8;
}

/**
 **************************************************************************************************
 Function name    : Dcm_Prv_StatestoReportSnapshotRecords
 Description      : Different states involved in reporting DTCSnapshot record data.
 Parameter        : pMsgContext
                    dataNegRespCode_u8
 Return value     : void
 **************************************************************************************************
 */
static void Dcm_Prv_StatestoReportSnapshotRecords(Dcm_MsgContextType *pMsgContext,
                                                  Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    /* Initialization state */
    if(DCM_RDTC_SUBFUNCTION_INITIAL == RDTCExtendedandSnapshotDataState_en)
    {
        /* Call API to get the request length and to check whether DTC is selected */
        Dcm_Prv_GetRequestLengthandSelectDTC(pMsgContext, dataNegRespCode_u8);
    }

    /* State to get the status of DTC sent from tester */
    if(DCM_RDTC_SUBFUNCTION_STATUSOFDTC == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_StatusOfDTC(pMsgContext, dataNegRespCode_u8);
    }

    /* State to disable the memory update of the selected DTC */
    if(DCM_RDTC_SUBFUNCTION_DISABLERECORDUPDATE == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_DisableSnapshotRecordUpdate(dataNegRespCode_u8);
    }

    /* State to select SnapshotRecords that matches the requested DTC and the DTCSnapshot record number */
    if(DCM_RDTC_SUBFUNCTION_SELECTRECORDSDATA == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_SnapshotRecordSelection(pMsgContext, dataNegRespCode_u8);
    }

    /* State to get size of DTCSnapshot data related to requested DTC and DTCSnapshot record number */
    if(DCM_RDTC_SUBFUNCTION_GETRECORDSDATASIZE == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_CheckforSizeOfSnapshotRecords(pMsgContext, dataNegRespCode_u8);
    }

    /* State to report DTCSnapshot data related to requested DTC and DTCSnapshot record number */
    if(DCM_RDTC_SUBFUNCTION_REPORTDTCRECORDS == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_ReportDTCSnapshotRecords(pMsgContext, dataNegRespCode_u8);
    }
}

/**
 **************************************************************************************************
 Function name    : Dcm_Dsp_ReportSnapshotRecordByDTCNumber
 Description      : API to report DTCSnapshot record data for the rquested DTC and DTCSnapshot record number.
 Parameter        : Opstatus
                    pMsgContext
                    dataNegRespCode_u8
 Return value     : E_OK
                    E_NOT_OK
                    DCM_E_PENDING
 TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3848]
 **************************************************************************************************
 */

Std_ReturnType Dcm_Dsp_ReportSnapshotRecordByDTCNumber(Dcm_SrvOpStatusType Opstatus,
                                                       Dcm_MsgContextType *pMsgContext,
                                                       Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{

    /* Initialization of variables */
    *dataNegRespCode_u8 = 0x00;
    s_SubServiceReturnValue_u8 = DCM_E_PENDING;

    DCM_UNUSED_PARAM(Opstatus);

    /* State machine */
    /* Different states involved in reporting Snapshot record data for requested DTC and DTCSnapshot record number */
    Dcm_Prv_StatestoReportSnapshotRecords(pMsgContext, dataNegRespCode_u8);

    if(DCM_RDTC_SUBFUNCTION_ENABLERECORDUPDATE == RDTCExtendedandSnapshotDataState_en)
    {
        Dcm_Prv_EnableRecordUpdate(dataNegRespCode_u8);
    }

    // To enable DTC record update in case of negative response
    // Separate state to handle asynchronous behavior of Enable DTC record update function
    if(DCM_RDTC_SUBFUNCTION_REENABLERECORDUPDATE == RDTCExtendedandSnapshotDataState_en)
    {
        s_SubServiceReturnValue_u8 = Dcm_Prv_ReEnableRecordUpdate(dataNegRespCode_u8);
    }

    if(*dataNegRespCode_u8 != 0)
    {
        RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_INITIAL;
        s_SubServiceReturnValue_u8 = E_NOT_OK;
    }

    return s_SubServiceReturnValue_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif
