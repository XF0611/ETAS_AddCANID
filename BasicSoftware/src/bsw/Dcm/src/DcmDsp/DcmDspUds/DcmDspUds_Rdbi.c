

#include "DcmDspUds_Rdbi_Inf.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_StRdbi_ten Dcm_stRdbi_en;                  /* State of RDBI statemachine        */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_RdbiReqDidNb_u16; /* Number of requested DIDs          */
uint16 Dcm_NumOfIndices_u16;
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint32 Dcm_TotalLength_u32;
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_OpStatusType Dcm_DspReadDidOpStatus_u8;  /* Variable to store the opstatus*/
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_DIDIndexType_tst s_Dcm_idxRdbiDidIndexType_st; /*Store the index of DID of either Dcm_DidConfig or Dcm_DIDRangeCOnfig structure*/
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* Flag to indicate whether RTE_CALL for Reading the data length is placed or not */
static boolean s_DcmReadDataLengthRteCallPlaced_b ;
/*Flag to indiciate if RTE_CALL for Reading the Did Data is placed or not */
static boolean s_DcmRteCallPlacedReadData_b;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif


#if ((DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)&& ((DCM_CFG_RDBIPAGEDBUFFERSUPPORT == DCM_CFG_OFF)))
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static boolean Dcm_flgNvmReadPending_b;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if((DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)||(DCM_CFG_LIMITRDBIRESPTOBUFSIZE != DCM_CFG_OFF))
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_MsgLenType Dcm_DspRdbiMaxRespLen_u32;  /* Maximum Response length */
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **************************************************************************************************
 * Dcm_Dsp_RdbiIni : Initialisation function for RDBI Service
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_Dsp_RdbiIni (void)
{
    Dcm_NegativeResponseCodeType dataNegResCode_u8;
    Std_ReturnType dataCondChkRetVal_u8;
    Std_ReturnType dataReadIfcRetVal_u8;
    const Dcm_DIDConfig_tst * ptrDidConfig;
    const Dcm_DataInfoConfig_tst * ptrSigConfig;
    const Dcm_SignalDIDSubStructConfig_tst * ptrControlSigConfig;

    /* Initialisation of local variables */
    dataCondChkRetVal_u8 = E_OK;
    dataReadIfcRetVal_u8 = E_OK;
    dataNegResCode_u8 = 0x0;
    ptrDidConfig =&Dcm_DIDConfig[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16];
    ptrSigConfig  = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
    ptrControlSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];

#if((DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)||(DCM_CFG_DSP_READ_ASYNCH_FNC_ENABLED != DCM_CFG_OFF))
    /* If there are any PENDING operations */

    if((Dcm_DspReadDidOpStatus_u8 == DCM_PENDING) && (Dcm_flgDspDidRangePending_b == FALSE))
    {
#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)
        /* If Asynchronous interfaces are used for this active Signal */
        if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)    ||
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                (ptrSigConfig->usePort_u8 == USE_DATA_RDBI_PAGED_FNC)         ||
#endif
                (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC))
        {
#if(DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
            if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
            {
            /* If the Condition check operation is pending */
            if((Dcm_stRdbi_en == DCM_RDBI_GET_LENGTH) && (ptrControlSigConfig->adrCondChkRdFnc_cpv.CondChkReadFunc3_pfct!=NULL_PTR))
                {
                    /* Call the Rte_Call API to cancel the ongoing operation */
                    dataCondChkRetVal_u8 = (*(ptrControlSigConfig->adrCondChkRdFnc_cpv.CondChkReadFunc3_pfct))(DCM_CANCEL);
                }
                /* If the Read operation is pending */
                if((Dcm_stRdbi_en == DCM_RDBI_GET_DATA)&&       \
                        (ptrSigConfig->adrReadFnc_cpv.ReadFunc11_ptr!=NULL_PTR))
                {
                    /* Invoke the Rte_Read Operation to cancel the Pending Read Operation */
                    dataReadIfcRetVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc11_ptr))(DCM_CANCEL);
                }
            }
            else
#endif
            {
                /* If the Condition check operation is pending */
                if((Dcm_stRdbi_en == DCM_RDBI_GET_LENGTH) && (ptrControlSigConfig->adrCondChkRdFnc_cpv.CondChkReadFunc2_pfct!=NULL_PTR))
                {
                dataCondChkRetVal_u8 = (*(ptrControlSigConfig->adrCondChkRdFnc_cpv.CondChkReadFunc2_pfct))(DCM_CANCEL, &dataNegResCode_u8);
            }
            /* If the Read operation is pending */
            if((Dcm_stRdbi_en == DCM_RDBI_GET_DATA)&&       \
                    (ptrSigConfig->adrReadFnc_cpv.ReadFunc2_ptr!=NULL_PTR))
            {
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                if(ptrSigConfig->usePort_u8 == USE_DATA_RDBI_PAGED_FNC)
                {
                    dataReadIfcRetVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc9_ptr))(DCM_CANCEL,NULL_PTR,NULL_PTR,&dataNegResCode_u8);
                }
                else
#endif
                {
                    dataReadIfcRetVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc2_ptr))(DCM_CANCEL,NULL_PTR);
                }

                }
            }
        }
#endif
    }
#endif

#if (DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)
    /*Check if any pending operations*/

    if((Dcm_DspReadDidOpStatus_u8 == DCM_PENDING) && (Dcm_flgDspDidRangePending_b!=FALSE))
    {
        dataReadIfcRetVal_u8 = (Dcm_DIDRangeConfig_cast[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16].adrReadFnc_pfct) (s_Dcm_idxRdbiDidIndexType_st.dataRangeDid_16,NULL_PTR,DCM_CANCEL,NULL_PTR,&dataNegResCode_u8);
    }
#endif
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
    s_DcmReadDataLengthRteCallPlaced_b  = FALSE;
    s_DcmRteCallPlacedReadData_b = FALSE;
#endif
    /* Set RDBI state machine to DCM_RDBI_IDLE */
    Dcm_stRdbi_en         = DCM_RDBI_IDLE;
    Dcm_StLenCalc_en      = DCM_LENCALC_STATUS_INIT;
    Dcm_GetDataState_en      = DCM_GETDATA_STATUS_INIT;
    Dcm_DidSignalIdx_u16 = 0x0;
    Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
    /*Set the flag for any Did pending operations is to FALSE */
    Dcm_flgDspDidRangePending_b=FALSE;
#if ((DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)&& ((DCM_CFG_RDBIPAGEDBUFFERSUPPORT == DCM_CFG_OFF)))
    /*Set the flag for any Did pending operations is to FALSE*/
    Dcm_flgNvmReadPending_b = FALSE;
#endif

#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
    Dcm_ResetAsynchFlags();
#endif
    Dcm_ResetDIDIndexstruct(&s_Dcm_idxRdbiDidIndexType_st);     /*This function is invoked to reset all the elements of DID index structure to its default value*/
    Dcm_PeriodicSchedulerRunning_b=FALSE;
    (void)dataCondChkRetVal_u8;
    (void)dataReadIfcRetVal_u8;
    (void)dataNegResCode_u8;
    /*Typecasted to void* to avoid any compiler warnings issues*/
    (void )ptrDidConfig;
    (void )ptrSigConfig;
    (void )ptrControlSigConfig;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


/**
 **************************************************************************************************
 * Dcm_Dsp_GetTotalLengthOfDIDs:
 *
 * Calculate the complete length of the DIDs in the request of an RDBI request.  This calculation is the heart
 * of the module. It takes into consideration the current support of the requested DIDs and determines the
 * indexes of the DIDIs. All supported DIDs have a belonging Index in the Dcm_DIDConfig which gets stored to
 * the request buffer. The length is then used to determine the position in which the real data fetching
 * function starts to write their answer to the buffer.
 *
 * The function takes care of the fact that the configured functions can return DCM_E_PENDING during the
 * processing and can therefore restart the calculation at any point in the processing.
 *
 * \param uint8* adrSourceIds_pu8: Pointer to the buffer holding the DIDs of the request. This buffer gets
 *                          overwritten with indexes of the DID in the Dcm_DIDConfig
 *        uint16 nrDids_u16: Number of IDs in the Buffer to which adrSourceIds_pu8 points
 *    uint16* adrNumOfIndices_pu16: In this the number of accepted DIDs gets returned to the calling function in case of success
 *     uint16* adrTotalLength_pu32 In this the overall length of the answer gets returned to the calling function in case of success
 *             dataNegRespCode_u8: Pointer to a Byte in which to store a negative Response code in case of detection of
 *                          an error in the request.
 *
 * \retval
 *      DCM_LENCALC_RETVAL_OK: Calculation finished successfully
 *   DCM_LENCALC_RETVAL_ERROR: Calculation detected an error. The Negative Response code is set
 * DCM_LENCALC_RETVAL_PENDING: Calculation needs more time to finish
 * \seealso
 *
 **************************************************************************************************
 */

/* Variables of the function Dcm_DspGetTotalLengthOfDIDs_en */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_LenCalc_ten Dcm_StLenCalc_en;           /* Variable to store the state of the function               */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint8 * Dcm_IdxList_pu8;    /* Pointer pointing to the DIDs in the request               */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint32 Dcm_NumberOfBytesInResponse_u32; /* Variable to store the total length of all valid DIDs      */
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_NumberOfProcessedDIDs_u16;   /* Variable to track number of DIDs in the request processed */
uint16 Dcm_NumberOfAcceptedDIDs_u16;    /* Variable to store number of valid DIDs in the request     */

static uint16 s_dataDID_Rdbi_u16;                /*Static Variable to hold the requested DID*/
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_LenCalcRet_ten Dcm_DspGetTotalLengthOfDIDs_en (uint8 * adrSourceIds_pu8,
        uint16 nrDids_u16,
        uint16 * adrNumOfIndices_pu16,
        uint32 * adrTotalLength_pu32,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    Dcm_LenCalcRet_ten dataRetVal_u8;            /* read interface return value */
    const Dcm_DIDConfig_tst * ptrDidConfig;
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
    boolean flagRdbi_PgdPort_b;    /*Variable to indicate if the DID has an port of type USE_RDBI_PAGED_FNC*/
    const Dcm_DataInfoConfig_tst * ptrSigConfig= NULL_PTR;
#endif
    dataRetVal_u8 = DCM_LENCALC_RETVAL_OK;
    ptrDidConfig =&Dcm_DIDConfig[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16];

    if (Dcm_StLenCalc_en == DCM_LENCALC_STATUS_INIT)
    {
        Dcm_NumberOfBytesInResponse_u32 = 0;
        Dcm_NumberOfProcessedDIDs_u16   = 0;
        Dcm_NumberOfAcceptedDIDs_u16    = 0;
        Dcm_IdxList_pu8                   = adrSourceIds_pu8;
        Dcm_DidSignalIdx_u16            = 0x0;
        Dcm_StLenCalc_en = DCM_LENCALC_STATUS_GETINDEX;
    }

    while ((dataRetVal_u8 == DCM_LENCALC_RETVAL_OK) &&
            (Dcm_NumberOfProcessedDIDs_u16 < nrDids_u16) &&
            ((Dcm_StLenCalc_en == DCM_LENCALC_STATUS_GETINDEX) ||
                    (Dcm_StLenCalc_en == DCM_LENCALC_STATUS_GETLENGTH) ||
                    (Dcm_StLenCalc_en == DCM_LENCALC_STATUS_GETSUPPORT)))
    {
        if (Dcm_StLenCalc_en == DCM_LENCALC_STATUS_GETINDEX)
        {

            Std_ReturnType dataFuncRetVal_en;            /* read interface return value */

            /* Variable to store DID  from the request */

            s_dataDID_Rdbi_u16 = (uint16)(((uint16)(adrSourceIds_pu8[Dcm_NumberOfProcessedDIDs_u16 * 2]))<<8u)+
                    (uint16)((uint16)(adrSourceIds_pu8[(Dcm_NumberOfProcessedDIDs_u16 * 2)+1]));

            /*TRACE[SWS_Dcm_00438]*/

            /* Check if the DID sent from tester is configured in Server s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16 */
            dataFuncRetVal_en = Dcm_Prv_GetIndexOfDID(s_dataDID_Rdbi_u16, &s_Dcm_idxRdbiDidIndexType_st);

#if(DCM_CFG_DSP_NUMISDIDAVAIL>0)
            /*If no errors occurs or not and Flag indicate if the did is a range did or not*/
            if ((dataFuncRetVal_en == E_OK) && (s_Dcm_idxRdbiDidIndexType_st.dataRange_b == FALSE))
            {
                ptrDidConfig =&Dcm_DIDConfig[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16];
                /* Check if the DID is supported in current variant */
                if(*Dcm_DIDIsAvail[ptrDidConfig->idxDIDSupportedFnc_u16] != NULL_PTR)
                {
                    if((*(IsDIDAvailFnc_pf)(Dcm_DIDIsAvail[ptrDidConfig->idxDIDSupportedFnc_u16]))(s_dataDID_Rdbi_u16)!=E_OK)
                    {
                        dataFuncRetVal_en = E_NOT_OK;
                    }
                }
            }
#endif


            if (dataFuncRetVal_en==E_OK)
            {
                Dcm_StLenCalc_en = DCM_LENCALC_STATUS_GETSUPPORT;
                Dcm_DidSignalIdx_u16 = 0x0;
            }
            else if (dataFuncRetVal_en==DCM_E_PENDING)
            {

                dataRetVal_u8=DCM_LENCALC_RETVAL_PENDING;
            }
            /* If the DID is not supported */
            else
            {
                /* (dataFuncRetVal_en==DCM_E_REQUEST_NOT_ACCEPTED) */
                /* Skip this id */
                Dcm_NumberOfProcessedDIDs_u16++;
                /* Dcm_StLenCalc_en = DCM_LENCALC_STATUS_GETINDEX  (no change of state required) */
            }
        }
        if (Dcm_StLenCalc_en == DCM_LENCALC_STATUS_GETSUPPORT)
        {
            Dcm_SupportRet_t dataSupportInfo_st; /* Variable to store the return value */
            /* Calling the API to check support for the DID */
            ptrDidConfig =&Dcm_DIDConfig[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16];
            dataSupportInfo_st=Dcm_GetSupportOfIndex (&s_Dcm_idxRdbiDidIndexType_st, DCM_SUPPORT_READ,dataNegRespCode_u8 );
            switch (dataSupportInfo_st)
            {
                case DCM_SUPPORT_OK:
                    Dcm_StLenCalc_en = DCM_LENCALC_STATUS_GETLENGTH;
                    Dcm_DidSignalIdx_u16 = 0x0;
                    break;

                case DCM_SUPPORT_SESSION_VIOLATED:
                    Dcm_NumberOfProcessedDIDs_u16++;
                    Dcm_StLenCalc_en = DCM_LENCALC_STATUS_GETINDEX;
                    break;
                case DCM_SUPPORT_SECURITY_VIOLATED:
                    /* Reset the state */
                    Dcm_StLenCalc_en = DCM_LENCALC_STATUS_INIT;
                    dataRetVal_u8 = DCM_LENCALC_RETVAL_ERROR;
                    break;

                case DCM_SUPPORT_CONDITION_VIOLATED:
                    /* If negative response code is DCM_E_REQUESTOUTOFRANGE */
                    if (*dataNegRespCode_u8 == DCM_E_REQUESTOUTOFRANGE)
                    {
                        Dcm_NumberOfProcessedDIDs_u16++;
                        Dcm_StLenCalc_en = DCM_LENCALC_STATUS_GETINDEX;
                    }
                    else
                    {
                        Dcm_StLenCalc_en = DCM_LENCALC_STATUS_INIT;
                        dataRetVal_u8 = DCM_LENCALC_RETVAL_ERROR;
                    }
                    break;
                case DCM_SUPPORT_CONDITION_PENDING:
                    dataRetVal_u8 = DCM_LENCALC_RETVAL_PENDING;
                    break;
                default:
                    /* Reset the state and set negative response code */
                    Dcm_StLenCalc_en = DCM_LENCALC_STATUS_INIT;
                    *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                    dataRetVal_u8 = DCM_LENCALC_RETVAL_ERROR;
                    break;
            }
        }
        if (Dcm_StLenCalc_en == DCM_LENCALC_STATUS_GETLENGTH)
        {
            uint32 dataLength_u32; /* Local variable of store length of DID */
            Std_ReturnType dataFuncRetVal_en; /* Local variable to store function return value */
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
            /*Set the flag  for Rdbi_PgdPort is to FALSE*/
            flagRdbi_PgdPort_b = FALSE;
#endif
            ptrDidConfig =&Dcm_DIDConfig[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16];
            /* Calling the function to get the length of DID */
            dataFuncRetVal_en=Dcm_GetLengthOfDIDIndex(&s_Dcm_idxRdbiDidIndexType_st,&dataLength_u32,s_dataDID_Rdbi_u16);

            if (dataFuncRetVal_en==E_OK )
            {
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                /*Check if the DID has use port of USE_RDBI_PAGED_FNC type*/
                ptrSigConfig= &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[0].idxDcmDspDatainfo_u16];
                if(ptrSigConfig->usePort_u8==USE_DATA_RDBI_PAGED_FNC)
                {
                    /*Set the flag  for Rdbi_PgdPort is to TRUE*/
                    flagRdbi_PgdPort_b=TRUE;
                }
#endif

                if (dataLength_u32>0u)
                {
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                    /*Check fro Maximum length of request for RDBI and Rdbi_PgdPort flag is set TRUE*/
                    if(((dataLength_u32 + DCM_RDBI_SIZE_DID) <= Dcm_DspRdbiMaxRespLen_u32)||(TRUE==flagRdbi_PgdPort_b))
                    {
#endif
#if (DCM_CFG_LIMITRDBIRESPTOBUFSIZE != DCM_CFG_OFF)
                        if (((Dcm_NumberOfBytesInResponse_u32 + dataLength_u32 + 2u) > Dcm_DspRdbiMaxRespLen_u32) && (Dcm_NumberOfAcceptedDIDs_u16 == 0u))
                        {
                            /* Send NRC 0x14 */
                            *dataNegRespCode_u8 = DCM_E_RESPONSETOOLONG;
                            dataRetVal_u8 = DCM_LENCALC_RETVAL_ERROR;
                            Dcm_StLenCalc_en = DCM_LENCALC_STATUS_INIT;
                        }
                        else if((Dcm_NumberOfBytesInResponse_u32 + dataLength_u32 + ((Dcm_NumberOfAcceptedDIDs_u16 + 1) *2 )) > Dcm_DspRdbiMaxRespLen_u32)
                        {
                            /* Do nothing and exit out of loop */
                            dataRetVal_u8 = DCM_LENCALC_RETVAL_OK;
                            break;
                        }
                        else
#endif
                        {

                            /* Update the number of bytes in response length */
                            Dcm_NumberOfBytesInResponse_u32 = (uint32)(Dcm_NumberOfBytesInResponse_u32 + dataLength_u32); /* length of data */

                            /* Update the accepted and processed DIDs */
                            Dcm_NumberOfAcceptedDIDs_u16 ++;
                            Dcm_NumberOfProcessedDIDs_u16++;

                            /* Write the Did back to the communication buffer in case if it is a range of DID(never goes faster than reading the DIDs)*/
                            *Dcm_IdxList_pu8= (uint8)(s_dataDID_Rdbi_u16>>8u);

                            Dcm_IdxList_pu8++;
                            *Dcm_IdxList_pu8= (uint8)s_dataDID_Rdbi_u16;

                            Dcm_IdxList_pu8++;

                            Dcm_StLenCalc_en = DCM_LENCALC_STATUS_GETINDEX;
                        }
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                    }
                    else
                    {
                        *dataNegRespCode_u8    = DCM_E_RESPONSETOOLONG;
                        dataRetVal_u8          = DCM_LENCALC_RETVAL_ERROR;
                        Dcm_StLenCalc_en = DCM_LENCALC_STATUS_INIT;
                    }
#endif
                }
                /* If length is zero */
                else
                {
                    /* skip this ID since length is zero */
                    Dcm_NumberOfProcessedDIDs_u16++;
                    Dcm_StLenCalc_en = DCM_LENCALC_STATUS_GETINDEX;
                }
            }
            /* check for infrastructural errors*/
            else if(dataFuncRetVal_en == DCM_INFRASTRUCTURE_ERROR)
            {
                *dataNegRespCode_u8    = DCM_E_GENERALREJECT;
                dataRetVal_u8          = DCM_LENCALC_RETVAL_ERROR;
                Dcm_StLenCalc_en = DCM_LENCALC_STATUS_INIT;
            }
            /* If any error occurs while calculating length */
            else if (dataFuncRetVal_en == E_NOT_OK)
            {
                /* Set the negative response code and return value */
                *dataNegRespCode_u8    = DCM_E_GENERALREJECT;
                dataRetVal_u8          = DCM_LENCALC_RETVAL_ERROR;
                Dcm_StLenCalc_en = DCM_LENCALC_STATUS_INIT;
            }
            else if (dataFuncRetVal_en == DCM_E_PENDING)
            {
                dataRetVal_u8 = DCM_LENCALC_RETVAL_PENDING;
            }
            else
            {
                if(*dataNegRespCode_u8 == 0x00)
                {
                    *dataNegRespCode_u8    = DCM_E_GENERALREJECT;
                }
                Dcm_StLenCalc_en = DCM_LENCALC_STATUS_INIT;
                dataRetVal_u8          = DCM_LENCALC_RETVAL_ERROR;
            }
        }
    }
    /* if return value is DCM_LENCALC_RETVAL_OK */
    if (dataRetVal_u8 == DCM_LENCALC_RETVAL_OK)
    {
        /* Update the total length , number of valid DIDs  and reset the state to DCM_LENCALC_STATUS_INIT */
        *adrTotalLength_pu32    = Dcm_NumberOfBytesInResponse_u32;
        *adrNumOfIndices_pu16   = Dcm_NumberOfAcceptedDIDs_u16;
        Dcm_StLenCalc_en = DCM_LENCALC_STATUS_INIT;
    }

    return (dataRetVal_u8);
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_MsgType Dcm_DspRdbiRespBufPtr_u8;     /* Pointer to the response buffer */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_MsgLenType s_Dcm_FilledLengthCurrentPage_u32;
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Std_ReturnType s_dataReadRetVal_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if (DCM_SEPARATEBUFFERFORTXANDRX_ENABLED == DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

static uint8 Dcm_DspRdbiDidBuffer_au8[DCM_CFG_RDBIREQUESTBUFFERSIZE]; /* Temporary buffer to store the DIDs in request buffer */

#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static boolean Dcm_RdbiFirstPageSent_b;       /* Whether first page is sent or not? */
static boolean s_Dcm_PagedDid_b;      /*Flag to indicate if DID is sent as a part of response or not yet*/
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **************************************************************************************************
 * Dcm_Dsp_RDBIUpdatePage :
 *  This function is used to update the Page length and the Response buffer pointer .
 *
 * \param           PageBufPtr    Pointer to response buffer
 *                  PageLen       Page length which can be filled
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */

static void Dcm_Dsp_RDBIUpdatePage(
        Dcm_MsgType PageBufPtr,
        Dcm_MsgLenType PageLen
);

static void Dcm_Dsp_RDBIUpdatePage(
        Dcm_MsgType PageBufPtr,
        Dcm_MsgLenType PageLen
)
{
    /* Update the Response length and the Response buffer pointer */
    Dcm_DspRdbiMaxRespLen_u32 = PageLen;
    Dcm_DspRdbiRespBufPtr_u8  = PageBufPtr;

}


/**
 **************************************************************************************************
 * Dcm_Dsp_RDBITriggerProcessPage :
 *  This function is used to trigger the process page .
 *
 * \param           PageBufPtr    Pointer to response buffer
 *                  PageLen       Page length which can be filled
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */
static Dcm_GetDataRet_ten Dcm_Dsp_RDBITriggerProcessPage(Dcm_MsgLenType PageLen);
static Dcm_GetDataRet_ten Dcm_Dsp_RDBITriggerProcessPage(Dcm_MsgLenType PageLen)
{
    /* If first page is not sent yet */

    if(Dcm_RdbiFirstPageSent_b == FALSE)
    {
        /* Set the variable to TRUE indicating first page is being sent */
        /*first page is sent */
        Dcm_RdbiFirstPageSent_b = TRUE;
    }

    /* Send the page for transmission */
    Dcm_ProcessPage(PageLen);
    /*Reset the filled page length to zero, since the response is triggered*/
    s_Dcm_FilledLengthCurrentPage_u32 = 0x0;

    return DCM_GETDATA_PAGED_BUFFER_TX;
}

/**
 **************************************************************************************************
 * Dcm_Dsp_RDBIUpdatePage :
 *  This function is used to update the Page length and the Response buffer pointer .
 *
 * \param           PageBufPtr    Pointer to response buffer
 *                  PageLen       Page length which can be filled
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */
static void Dcm_Dsp_RDBICancelPagebufferHandling(void);
static void Dcm_Dsp_RDBICancelPagebufferHandling(void)
{
    /* Check if first page is already sent or not */

    if(Dcm_RdbiFirstPageSent_b == FALSE)
    {
        /* Do nothing since NRC is already set by DcmAppl_DcmReadDataNRC */
        /*Reset the filled page length to zero, since the response is triggered*/
        s_Dcm_FilledLengthCurrentPage_u32 = 0x0;

    }
    /* First page is already sent so NRC cannot be set */
    else
    {
        /* Call process page with zero length to stop the response transmission immediately */
        Dcm_ProcessPage(0);
        /*Reset the filled page length to zero, since the response is triggered*/
        s_Dcm_FilledLengthCurrentPage_u32 = 0x0;
        /* Report development error "DCM_E_NO_WRITE_ACCESS " to DET module if the DET module is enabled */
        DCM_DET_ERROR(DCM_RDBI_ID , DCM_E_WRONG_STATUSVALUE)

        /* Move the state to default state */
        Dcm_GetDataState_en=DCM_GETDATA_STATUS_INIT;
    }
}

/**
 **************************************************************************************************
 * Dcm_Dsp_RDBIClearbuffer :
 *  This function is used to clear the buffer pointer.
 *
 * \param           adrTargetBuffer_pu8    Pointer to response buffer
 *                  adrTotalLength_u32     Length of data which is to be cleared
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */
static void Dcm_Dsp_RDBIClearbuffer(uint8 * adrTargetBuffer_pu8,uint32 adrTotalLength_u32);
static void Dcm_Dsp_RDBIClearbuffer(uint8 * adrTargetBuffer_pu8,uint32 adrTotalLength_u32)
{
    /*Initialize the target buffer with zero for all data bytes of the DIDs*/
	/*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type pointer is converted to void pointer*/
    DCM_MEMSET(adrTargetBuffer_pu8, (sint32)DCM_CFG_SIGNAL_DEFAULT_VALUE, adrTotalLength_u32);
}

/**
 **************************************************************************************************
 * Dcm_GetTypeofDID :
 *  This function is used to find out the type of DID, if the DID is a normal or a PAGED DID.
 *
 * \param           idxDidIndexType_st    Pointer to DID index structure definition.
 *
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */
static void Dcm_GetTypeofDID(Dcm_DIDIndexType_tst * idxDidIndexType_st)
{
    const Dcm_DIDConfig_tst * ptrDidConfig;
    const Dcm_DataInfoConfig_tst * ptrSigConfig;

    /*Check if the did is a range did or not*/
    if(TRUE != idxDidIndexType_st->dataRange_b)
    {
        ptrDidConfig =&Dcm_DIDConfig[idxDidIndexType_st->idxIndex_u16];
        ptrSigConfig  = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[0].idxDcmDspDatainfo_u16];
        if(ptrSigConfig->usePort_u8==USE_DATA_RDBI_PAGED_FNC)
        {
            /*Set that the DID is a paged DID or special DID*/
            idxDidIndexType_st->flagPageddid_b=TRUE;
            /*Set the flag for s_Dcm_PagedDid is to TRUE */
            s_Dcm_PagedDid_b=TRUE;
        }
    }
}

/**
 **************************************************************************************************
 * Dcm_GetTypeofDID :
 *  This function is used to find out the type of DID, if the DID is a normal or a PAGED DID.
 *
 * \param           idxDidIndexType_st    Pointer to DID index structure definition.
 *
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */
static Std_ReturnType Dcm_GetPagedDIDData(Dcm_DIDIndexType_tst * idxDidIndexType_st,
        uint8 * adrTargetBuffer_pu8);
static Std_ReturnType Dcm_GetPagedDIDData(Dcm_DIDIndexType_tst * idxDidIndexType_st,
        uint8 * adrTargetBuffer_pu8)
{
    uint32 databufferSize_u32 = 0x0u;
    const Dcm_DIDConfig_tst * ptrDidConfig;
    const Dcm_DataInfoConfig_tst * ptrSigConfig;
    ptrDidConfig =&Dcm_DIDConfig[idxDidIndexType_st->idxIndex_u16];
    ptrSigConfig  = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[0].idxDcmDspDatainfo_u16];
    /*Check that the Did is paged did or special did*/
    if(s_Dcm_PagedDid_b==TRUE)
    {
        if((s_Dcm_FilledLengthCurrentPage_u32 + 2uL) <= Dcm_DspRdbiMaxRespLen_u32) 
        {
           /*Calculate the length of available Transmission buffer*/
           /*If first page reduce 2 bytes since, 2 bytes of DID needs to be triggered from the Get_Data function*/
           databufferSize_u32 = (Dcm_DspRdbiMaxRespLen_u32-s_Dcm_FilledLengthCurrentPage_u32-2uL);
        }
    }
    else
    {
        if(Dcm_DspRdbiMaxRespLen_u32 >= s_Dcm_FilledLengthCurrentPage_u32)
		{
          /*Calculate the length of available Transmission buffer*/
          /*If intermediate or last page no need to reduce the buffer size by 2 since the complete buffer is available for data*/
          databufferSize_u32 = (Dcm_DspRdbiMaxRespLen_u32-s_Dcm_FilledLengthCurrentPage_u32);
		}
    }
    /*Clear the buffer and provide it to application.*/
    if(s_dataReadRetVal_u8 != DCM_E_PENDING)
    {
        /*Clear the buffer before providing the buffer to application*/
        Dcm_Dsp_RDBIClearbuffer(&adrTargetBuffer_pu8[0],databufferSize_u32);
    }

    if(NULL_PTR != ptrSigConfig->adrReadFnc_cpv.ReadFunc9_ptr)
    {
        s_dataReadRetVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc9_ptr))(Dcm_DspReadDidOpStatus_u8, &adrTargetBuffer_pu8[0],&databufferSize_u32,&idxDidIndexType_st->dataNegRespCode_u8);
        /*Update the Data provided by application in length information*/
        idxDidIndexType_st->dataSignalLengthInfo_u32 = databufferSize_u32;
    }
    else
    {
        s_dataReadRetVal_u8=E_NOT_OK;
    }

    if(DCM_E_RDBI_DATA_PENDING==s_dataReadRetVal_u8)
    {
        if((databufferSize_u32==0x0uL)|| (databufferSize_u32 < 0x8uL) || (databufferSize_u32 > Dcm_DspRdbiMaxRespLen_u32))
        {
            /*Invalid condition set NRC*/
            s_dataReadRetVal_u8 = E_NOT_OK;
            /*Update the Data provided by application in length information*/
            idxDidIndexType_st->dataSignalLengthInfo_u32 = 0;
        }
    }
    else if(DCM_E_PENDING==s_dataReadRetVal_u8)
    {
        s_dataReadRetVal_u8 = DCM_E_PENDING;
    }
    else if(E_OK==s_dataReadRetVal_u8)
    {
        if(databufferSize_u32==0x0uL)
        {
            /*Invalid condition set NRC*/
            s_dataReadRetVal_u8 = E_NOT_OK;
            /*Update the Data provided by application in length information*/
            idxDidIndexType_st->dataSignalLengthInfo_u32 = 0;
        }
    }
    else
    {
        s_dataReadRetVal_u8 = E_NOT_OK;
        /*Update the Data provided by application in length information*/
        idxDidIndexType_st->dataSignalLengthInfo_u32 = 0;
    }

    return s_dataReadRetVal_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**
 **************************************************************************************************
 * Dcm_GetData_en:
 *
 * This function calculated the answer to the request based on the indexes of the originally requested and
 * supported DIDs of the request. It processes the data from back to front because we use the message buffer
 * as the intermediate memory for the indexes. The insertion point is calculated based on the adrTotalLength_pu32 of
 * the return message as it was calculated by the function Dcm_DspGetTotalLengthOfDIDs_en.
 *
 * \param  *uint8     adrIdBuffer_pcu8: Pointer to the buffer holding the indexes
 *         *uint8 adrTargetBuffer_pu8: Pointer to which the answer shall be written
 *           uint16 nrIndex_u16: Number of indexes in the adrIdBuffer_pcu8
 *        uint16   adrTotalLength_pu32: The number of bytes we expect for the complete message
 *
 * \retval
 *             DCM_GETDATA_RETVAL_OK: Calculation successfully finished
 *  DCM_GETDATA_RETVAL_INTERNALERROR: Fatal internal error during calculation
 *        DCM_GETDATA_RETVAL_PENDING: More time is required to calculate the final answer
 * \seealso Dcm_DspGetTotalLengthOfDIDs_en
 *
 **************************************************************************************************
 */
/* Variables of Dcm_GetData */


#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_GetData_ten Dcm_GetDataState_en;        /* Variable to store the state of the function */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

static Std_ReturnType s_dataReadIfcRetVal_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_GetDataNumOfIndex_u16;          /* Variable to store the number of valid DIDs */

static uint16 s_dataDynamicID_u16;
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint32 Dcm_GetDataTotalLength_u32;         /* Variable to store the total length of all the DIDs */
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_GetDataRet_ten Dcm_GetData_en (const uint8 * adrIdBuffer_pcu8,
        uint8 * adrTargetBuffer_pu8,
        uint16 nrIndex_u16,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8
)
{

#if (DCM_SEPARATEBUFFERFORTXANDRX_ENABLED == DCM_CFG_OFF)
    uint16 idxLoop_u16;
#endif

    Std_ReturnType dataNrcRetval_u8;				/* Variable to hold return value of DcmAppl_DcmGetDataNrc*/
    Dcm_GetDataRet_ten dataRetVal_u8;              /* read interface return value */
    static uint32 Dcm_DataGetLengthOfCurrentData_u32; /* Variable to store the data length of a DID being read */
    static boolean Dcm_Pending_DIDData_b; /* Variable to indicates DID data was pending  */
    /* Initialization of local variables */

    dataRetVal_u8  = DCM_GETDATA_RETVAL_OK;
    dataNrcRetval_u8 = E_OK;
    *dataNegRespCode_u8 = 0x0;


    if (Dcm_GetDataState_en == DCM_GETDATA_STATUS_INIT)
    {
        /* Store the number of DIDs and total length */
        Dcm_GetDataNumOfIndex_u16  = 0;
        Dcm_GetDataState_en            = DCM_GETDATA_STATUS_GETLENGTH;
        Dcm_DataGetLengthOfCurrentData_u32 = 0x0;
        s_Dcm_FilledLengthCurrentPage_u32 = 0x0;
        s_dataReadIfcRetVal_u8 = E_OK;


#if (DCM_SEPARATEBUFFERFORTXANDRX_ENABLED == DCM_CFG_OFF)
        /*Copy the request buffer to a temporary location, if same buffer is used for Tx and Rx*/
        for(idxLoop_u16=0;idxLoop_u16 < (nrIndex_u16 * 2);idxLoop_u16++)
        {
            Dcm_DspRdbiDidBuffer_au8[idxLoop_u16] = adrIdBuffer_pcu8[idxLoop_u16];
        }
#endif
    }
    /* Loop till the data corresponding to all DIDs are read successfully and stored in response buffer */
    while ((Dcm_GetDataNumOfIndex_u16 < nrIndex_u16) &&
            ((Dcm_GetDataState_en == DCM_GETDATA_STATUS_GETDATA) ||
                    (Dcm_GetDataState_en == DCM_GETDATA_STATUS_GETLENGTH)) &&
                    (dataRetVal_u8          == DCM_GETDATA_RETVAL_OK))
    {

        uint32 posnTarget_u32;
        posnTarget_u32 = 0x0;

        if (Dcm_GetDataState_en == DCM_GETDATA_STATUS_GETLENGTH)
        {
            Std_ReturnType dataFuncRetVal_en; /* Variable to store return value */

#if (DCM_SEPARATEBUFFERFORTXANDRX_ENABLED == DCM_CFG_OFF)
            /* MR12 RULE 12.2 VIOLATION: Right hand operand of shift operator is greater than or equal to the width of the underlying type */
            s_dataDynamicID_u16 = (uint16)((Dcm_DspRdbiDidBuffer_au8[((Dcm_GetDataNumOfIndex_u16)*2u)]<<(uint8)8u) + (Dcm_DspRdbiDidBuffer_au8[((Dcm_GetDataNumOfIndex_u16)*2u)+1u]));
#else
            /* MR12 RULE 12.2 VIOLATION: Right hand operand of shift operator is greater than or equal to the width of the underlying type */
            s_dataDynamicID_u16 = (uint16)((adrIdBuffer_pcu8[((Dcm_GetDataNumOfIndex_u16)*2u)]<<(uint8)8u) +  (adrIdBuffer_pcu8[((Dcm_GetDataNumOfIndex_u16)*2u)+1u]));
#endif

            /*Reset the DID indexes*/
            Dcm_ResetDIDIndexstruct(&s_Dcm_idxRdbiDidIndexType_st);


            /* Check if the DID sent from tester is configured in Server s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16 */
            dataFuncRetVal_en = Dcm_Prv_GetIndexOfDID(s_dataDynamicID_u16, &s_Dcm_idxRdbiDidIndexType_st);

            if(E_OK == dataFuncRetVal_en)
            {
                /* Calling function to get the length of DID and the DID parameter is passed with a null since index is sufficient for the
                 * normal DID, The DID value can be obtained from the Dcm_DIDConfig and where as for a range of DID this parameter has to
                 * be passed with the DID value */

                dataFuncRetVal_en=Dcm_GetLengthOfDIDIndex (&s_Dcm_idxRdbiDidIndexType_st,&Dcm_DataGetLengthOfCurrentData_u32,s_dataDynamicID_u16);
                /*Check if the did is a range did or not*/
                if(s_Dcm_idxRdbiDidIndexType_st.dataRange_b!= FALSE)
                {
                    /*Update the length of the data to be obtained from application*/
                    s_Dcm_idxRdbiDidIndexType_st.dataSignalLengthInfo_u32 = Dcm_DataGetLengthOfCurrentData_u32;
                }

            }

            switch (dataFuncRetVal_en)
            {
                /* If length is obtained correctly */
                case (Std_ReturnType)E_OK:
                        Dcm_GetDataState_en = DCM_GETDATA_STATUS_GETDATA;
                        /*Check whether the DID is a normal DID or a paged DID*/
                        Dcm_GetTypeofDID(&s_Dcm_idxRdbiDidIndexType_st);
                        Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                        /* Reset the Signal index */
                        Dcm_DidSignalIdx_u16 = 0x0;
                        break;
                case (Std_ReturnType)DCM_E_PENDING:
                        dataRetVal_u8 = DCM_GETDATA_RETVAL_PENDING;
                        break;
                case (Std_ReturnType)E_NOT_OK:

                case (Std_ReturnType)DCM_INFRASTRUCTURE_ERROR:
                default:
                        dataRetVal_u8 = DCM_GETDATA_RETVAL_INTERNALERROR;
                        /* Reset the state */
                        Dcm_GetDataState_en = DCM_GETDATA_STATUS_INIT;
                        /*Reset the DID indexes*/
                        Dcm_ResetDIDIndexstruct(&s_Dcm_idxRdbiDidIndexType_st);
                        break;
            }
        }

        if (Dcm_GetDataState_en == DCM_GETDATA_STATUS_GETDATA)
        {

            /*Update the status of the opstatus variable passed*/
            s_Dcm_idxRdbiDidIndexType_st.dataopstatus_b = Dcm_DspReadDidOpStatus_u8;

            /*Check if the DID is a normal DID or not*/

            if(s_Dcm_idxRdbiDidIndexType_st.flagPageddid_b!= TRUE)
            {
                if (Dcm_DsldPduInfo_st.SduLength != 0u)
                {
                    if (Dcm_Pending_DIDData_b == FALSE )
                    {
                        s_Dcm_FilledLengthCurrentPage_u32 = s_Dcm_FilledLengthCurrentPage_u32 + Dcm_DsldPduInfo_st.SduLength;
                        Dcm_Pending_DIDData_b = TRUE;
                    }
                    posnTarget_u32 = s_Dcm_FilledLengthCurrentPage_u32  + 2u ;
                }
                else
                {
                    /*Did is a normal DID, hence calculate the position based on the number of DID response filled and the total datalength*/
                    posnTarget_u32 = s_Dcm_FilledLengthCurrentPage_u32 + 2uL;
                }
                if((posnTarget_u32 + Dcm_DataGetLengthOfCurrentData_u32) <= Dcm_DspRdbiMaxRespLen_u32)
                {
                   /*The DID is a normal DID, so use Dcm_GetDIDData() to obtain the Data from the application*/
                   s_dataReadIfcRetVal_u8 = Dcm_GetDIDData(&s_Dcm_idxRdbiDidIndexType_st,&adrTargetBuffer_pu8[posnTarget_u32]);
                }
                else
                {
                    s_dataReadIfcRetVal_u8 = E_NOT_OK;
                    s_Dcm_idxRdbiDidIndexType_st.dataNegRespCode_u8  = DCM_E_GENERALREJECT;
                }
            }
            else
            {
                /*DID is a paged DID, hence calculate the length based on if the DID has been triggered
                 *  as a part of response in a page already or is it intermediate page, if intermediate page the position is from zero*/

                if(s_Dcm_PagedDid_b==TRUE)
                {
                    if(Dcm_Pending_DIDData_b==FALSE)
                    {
                        s_Dcm_FilledLengthCurrentPage_u32 += Dcm_DsldPduInfo_st.SduLength;
                        Dcm_Pending_DIDData_b=TRUE;
                    }
                    posnTarget_u32 = s_Dcm_FilledLengthCurrentPage_u32 + 2uL;
                }
                else
                {
                    if (Dcm_DsldPduInfo_st.SduLength != 0u )
                      {
                        if (Dcm_Pending_DIDData_b == FALSE)
                        {
                            s_Dcm_FilledLengthCurrentPage_u32 = Dcm_DsldPduInfo_st.SduLength;
                            Dcm_Pending_DIDData_b = TRUE;
                        }
                      }
                    posnTarget_u32 = s_Dcm_FilledLengthCurrentPage_u32;
                }
                /* Buffer index shouldn't exceed the response buffer */
                if(posnTarget_u32 < Dcm_DspRdbiMaxRespLen_u32)
                {
                    /*The DID is a Paged DID, so use Dcm_GetPagedDIDData() to obtain the Data from the application, this function
                     * will be invoked multiple times inorder to obtain the data*/
                    s_dataReadIfcRetVal_u8 = Dcm_GetPagedDIDData(&s_Dcm_idxRdbiDidIndexType_st,&adrTargetBuffer_pu8[posnTarget_u32]);
                }
            }

            switch (s_dataReadIfcRetVal_u8)
            {
                case (Std_ReturnType)E_OK:
                    {
                    Dcm_GetDataState_en = DCM_GETDATA_STATUS_GETLENGTH;

                    s_Dcm_idxRdbiDidIndexType_st.dataNegRespCode_u8 = 0x0;

                        /*Check if the DID is a normal DID or a paged DID*/

                        if(s_Dcm_idxRdbiDidIndexType_st.flagPageddid_b != TRUE)
                        {
                            /*Store the DID in the buffer in case of normal DID*/
                            posnTarget_u32=(posnTarget_u32-2u);
                            adrTargetBuffer_pu8[posnTarget_u32]   = (uint8)((s_dataDynamicID_u16)>>8u);
                            adrTargetBuffer_pu8[posnTarget_u32+1u] = (uint8)((s_dataDynamicID_u16));

                            /*Update the length filled in the current page including the DID size*/
                            s_Dcm_FilledLengthCurrentPage_u32 = s_Dcm_FilledLengthCurrentPage_u32 + Dcm_DataGetLengthOfCurrentData_u32 + 2u;
                        }
                        else
                        {
                            /*Check if the did is paged did or normal did*/
                            if(s_Dcm_PagedDid_b==TRUE)
                            {
                                /*Store the paged DID in the buffer in case the application returns E_OK in first call itself, this happens if
                                 * the user configures a paged function whose size is less than the available buffer size*/
                                posnTarget_u32=(posnTarget_u32-2u);
                                adrTargetBuffer_pu8[posnTarget_u32]   = (uint8)((s_dataDynamicID_u16)>>8u);
                                adrTargetBuffer_pu8[posnTarget_u32+1u] = (uint8)((s_dataDynamicID_u16));
                                s_Dcm_FilledLengthCurrentPage_u32 += 2u;
                                /*Did is not paged did*/
                                s_Dcm_PagedDid_b=FALSE;
                            }
                            s_Dcm_FilledLengthCurrentPage_u32 = ((s_Dcm_FilledLengthCurrentPage_u32) + (s_Dcm_idxRdbiDidIndexType_st.dataSignalLengthInfo_u32));
                        }
                        /*Increment the did counter since one of the DID  response is filled and updated in buffer*/
                        Dcm_GetDataNumOfIndex_u16++;
                        /*Trigger process page if Data greater than 7 bytes is available or if the last page*/
                        if(((s_Dcm_FilledLengthCurrentPage_u32 ) >= 7u)||(Dcm_GetDataNumOfIndex_u16==nrIndex_u16))
                        {
                            Dcm_Pending_DIDData_b = FALSE;
                            dataRetVal_u8=Dcm_Dsp_RDBITriggerProcessPage((s_Dcm_FilledLengthCurrentPage_u32 ));
                        }
                        else
                        {
                            Dcm_Pending_DIDData_b = TRUE;

                        }

                        /*Set the opstatus to pending, since more data is expected from the application*/
                        Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;

                    /*Reset the DID indexes structure*/
                    Dcm_ResetDIDIndexstruct(&s_Dcm_idxRdbiDidIndexType_st);
                    break;
                    }
                case (Std_ReturnType)DCM_E_RDBI_DATA_PENDING:
                    {
                    /*If the DID is a paged DID*/
                    /*Trigger process page if Data greater than 7 bytes is available*/
                    s_Dcm_idxRdbiDidIndexType_st.dataNegRespCode_u8 = 0x0;

                        /*Check if the did is a paged did or normal did*/
                        if(s_Dcm_PagedDid_b==TRUE)
                        {
                            /*Store the DID in the buffer*/
                            posnTarget_u32=(posnTarget_u32-2u);
                            adrTargetBuffer_pu8[posnTarget_u32]   = (uint8)((s_dataDynamicID_u16)>>8u);
                            adrTargetBuffer_pu8[posnTarget_u32+1u] = (uint8)((s_dataDynamicID_u16));
                            s_Dcm_FilledLengthCurrentPage_u32 += 2u;
                            /*Set the flag for s_Dcm_PagedDid is to FALSE*/
                            s_Dcm_PagedDid_b=FALSE;
                        }
                        s_Dcm_FilledLengthCurrentPage_u32 = ((s_Dcm_FilledLengthCurrentPage_u32) + (s_Dcm_idxRdbiDidIndexType_st.dataSignalLengthInfo_u32));

                        if((s_Dcm_FilledLengthCurrentPage_u32) >= 7u)
                        {
                            Dcm_Pending_DIDData_b = FALSE;
                            dataRetVal_u8=Dcm_Dsp_RDBITriggerProcessPage((s_Dcm_FilledLengthCurrentPage_u32));
                        }
                        else
                        {
                            Dcm_Pending_DIDData_b = TRUE;
                        }

                        /*Set the opstatus to pending, since more data is expected from the application*/
                        Dcm_DspReadDidOpStatus_u8 = DCM_PENDING;

                    break;
                    }
                case (Std_ReturnType)DCM_E_PENDING:
                    {
                    Dcm_Pending_DIDData_b=TRUE;
                    dataRetVal_u8 = DCM_GETDATA_RETVAL_PENDING;
                    break;
                    }
                case (Std_ReturnType)DCM_E_REQUEST_ENV_NOK:
                    {
                    *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                    dataRetVal_u8 = DCM_GETDATA_RETVAL_INTERNALERROR;
                    /* Reset the state */
                    Dcm_GetDataState_en = DCM_GETDATA_STATUS_INIT;
                    /*Reset the DID indexes structure*/
                    Dcm_ResetDIDIndexstruct(&s_Dcm_idxRdbiDidIndexType_st);

                    break;
                    }
                case (Std_ReturnType)E_NOT_OK:
                    {
                    Dcm_Pending_DIDData_b = FALSE;
                    if(s_Dcm_idxRdbiDidIndexType_st.dataNegRespCode_u8 == 0x0)
                    {
                        /*ReadData Application function has returned E_NOT_OK, Invoke DcmAppl to obtain the NRC value to be passed on to the tester*/
                        dataNrcRetval_u8 = DcmAppl_DcmReadDataNRC(s_dataDynamicID_u16,posnTarget_u32,dataNegRespCode_u8);

                        if(dataNrcRetval_u8!=E_OK)
                        {

                            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;

                        }
                    }
                    else
                    {
                        /*NRC is already set by the application*/
                        *dataNegRespCode_u8 = s_Dcm_idxRdbiDidIndexType_st.dataNegRespCode_u8;
                    }


                    }
                /* MR12 RULE 16.3 VIOLATION: Break statement in case E_NOT_OK is intentionally not inculded-for optimized code*/
                default:
                {
                    /*Cancel the paged buffer processing since the conditions are not correct*/
                    Dcm_Dsp_RDBICancelPagebufferHandling();
                    dataRetVal_u8 = DCM_GETDATA_RETVAL_INTERNALERROR;
                    /* Reset the state */
                    Dcm_GetDataState_en = DCM_GETDATA_STATUS_INIT;
                    /*Reset the DID indexes structure*/
                    Dcm_ResetDIDIndexstruct(&s_Dcm_idxRdbiDidIndexType_st);

                    break;
                }
            }
        }
    }
    if (dataRetVal_u8 == DCM_GETDATA_RETVAL_OK)
    { /* If all are correctly processed we need to set our status back to init for the next call */
        Dcm_GetDataState_en=DCM_GETDATA_STATUS_INIT;
    }
    return dataRetVal_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#else
/**
 **************************************************************************************************
 * Dcm_GetData_en:
 *
 * This function calculated the answer to the request based on the indexes of the originally requested and
 * supported DIDs of the request. It processes the data from back to front because we use the message buffer
 * as the intermediate memory for the indexes. The insertion point is calculated based on the adrTotalLength_pu32 of
 * the return message as it was calculated by the function Dcm_DspGetTotalLengthOfDIDs_en.
 *
 * \param  *uint8     adrIdBuffer_pcu8: Pointer to the buffer holding the indexes
 *         *uint8 adrTargetBuffer_pu8: Pointer to which the answer shall be written
 *           uint16 nrIndex_u16: Number of indexes in the adrIdBuffer_pcu8
 *        uint16   adrTotalLength_pu32: The number of bytes we expect for the complete message
 *
 * \retval
 *             DCM_GETDATA_RETVAL_OK: Calculation successfully finished
 *  DCM_GETDATA_RETVAL_INTERNALERROR: Fatal internal error during calculation
 *        DCM_GETDATA_RETVAL_PENDING: More time is required to calculate the final answer
 * \seealso Dcm_DspGetTotalLengthOfDIDs_en
 *
 **************************************************************************************************
 */

/* Variables of Dcm_GetData */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_GetData_ten Dcm_GetDataState_en;        /* Variable to store the state of the function */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

static Std_ReturnType s_readDataLengthFnc_retVal_u8; /* Local variable to store return value */

static Std_ReturnType s_dataReadIfcVal_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_GetDataNumOfIndex_u16;          /* Variable to store the number of valid DIDs */

static uint16 s_dataID_u16;
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint32 Dcm_GetDataTotalLength_u32;         /* Variable to store the total length of all the DIDs */

static uint32 s_posnTargetSig_u32;

static uint32 s_datalengthinfo_u32;   /* local variable to store the datalength information for all the variable length
                                                               signals to determine the position of the buffer to handle the dynamicity in variable length signals*/
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_GetDataRet_ten Dcm_GetData_en (const uint8 * adrIdBuffer_pcu8,
        uint8 * adrTargetBuffer_pu8,
        uint16 nrIndex_u16,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
        uint32 adrTotalLength_pu32
)
{

    /* Local variables to read the signals */
    const Dcm_DIDConfig_tst * ptrDidConfig;
    const Dcm_DataInfoConfig_tst * ptrSigConfig;
    const Dcm_SignalDIDSubStructConfig_tst * ptrControlSigConfig;

#if (DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)
    uint16 dataLength_u16;
#endif

#if(DCM_CFG_DSP_READ_SR_ENABLED!=DCM_CFG_OFF)
    uint16 nrElements_u16;
    uint16 idxLoop_u16;
    uint32 dataSignal_u32;
    uint8 DataEndiannessType_u8;/*Local variable to store endianness of signal */
#endif
#if(DCM_CFG_DSP_READ_UINT16_SR_ENABLED != DCM_CFG_OFF)
    uint16 dataSignal_u16;
#endif
#if(DCM_CFG_DSP_READ_SINT16_SR_ENABLED != DCM_CFG_OFF)
    sint16 dataSignal_s16;
#endif
#if(DCM_CFG_DSP_READ_SINT32_SR_ENABLED != DCM_CFG_OFF)
    sint32 dataSignal_s32;
#endif
#if(DCM_CFG_DSP_READ_SINT8_SR_ENABLED != DCM_CFG_OFF)
    sint8 dataSignal_s8;
#endif
#if(DCM_CFG_DSP_READ_BOOLEAN_SR_ENABLED != DCM_CFG_OFF)
    boolean dataSignal_b;
#endif
#if(DCM_CFG_DSP_READ_UINT8_SR_ENABLED != DCM_CFG_OFF)
    uint8 dataSignal_u8;
#endif



    Std_ReturnType dataNrcRetval_u8;/* Variable to hold return value of DcmAppl_DcmGetDataNrc*/
    Dcm_GetDataRet_ten dataRetVal_u8;            /* read interface return value */
    static uint32 Dcm_DataLengthOfCurrentData_u32; /* Variable to store the data length of a DID being read */
    boolean FixedLength_b=TRUE; /*Local variable to store the length type of signals*/

    /* Initialization of local variables */
#if(DCM_CFG_DSP_READ_BOOLEAN_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_b   = FALSE;
#endif
#if(DCM_CFG_DSP_READ_UINT8_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_u8  = 0x0u;
#endif
#if(DCM_CFG_DSP_READ_UINT16_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_u16 = 0x0u;
#endif
#if(DCM_CFG_DSP_READ_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_u32 = 0x0u;
    DataEndiannessType_u8=DCM_OPAQUE; /*Local variable to store endianness of signal */
#endif
#if(DCM_CFG_DSP_READ_SINT8_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_s8  = 0x0;
#endif
#if(DCM_CFG_DSP_READ_SINT16_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_s16 = 0x0;
#endif
#if(DCM_CFG_DSP_READ_SINT32_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_s32 = 0x0;
#endif

    dataRetVal_u8     = DCM_GETDATA_RETVAL_OK;
    dataNrcRetval_u8 = E_OK;
    *dataNegRespCode_u8 = 0x0;
    ptrDidConfig = NULL_PTR;
    ptrSigConfig = NULL_PTR;
    ptrControlSigConfig = NULL_PTR;

    if (Dcm_GetDataState_en == DCM_GETDATA_STATUS_INIT)
    {
        /* Store the number of DIDs and total length */
        Dcm_GetDataNumOfIndex_u16  = nrIndex_u16;
        Dcm_GetDataTotalLength_u32 = adrTotalLength_pu32;
        Dcm_GetDataState_en            = DCM_GETDATA_STATUS_GETLENGTH;
        s_dataReadIfcVal_u8    = E_OK;
        s_readDataLengthFnc_retVal_u8 = E_OK;
        s_datalengthinfo_u32 = 0;

    }
    /* Loop till the data corresponding to all DIDs are read successfully and stored in response buffer */
    while ((Dcm_GetDataNumOfIndex_u16>0) &&
            ((Dcm_GetDataState_en == DCM_GETDATA_STATUS_GETDATA) ||
                    (Dcm_GetDataState_en == DCM_GETDATA_STATUS_GETLENGTH)) &&
                    (dataRetVal_u8          == DCM_GETDATA_RETVAL_OK))
    {
        if (Dcm_GetDataState_en == DCM_GETDATA_STATUS_GETLENGTH)
        {
            Std_ReturnType dataFuncRetVal_en; /* Variable to store return value */
            /*uint32 variable for storing DID in a single variable */
            s_dataID_u16 = (uint16)((uint16)((uint16)adrIdBuffer_pcu8[((Dcm_GetDataNumOfIndex_u16-1)*2u)]<<(uint8)8) +
                    ((uint16)adrIdBuffer_pcu8[((Dcm_GetDataNumOfIndex_u16-1u)*2u)+1u]));


            /*Function call to check if the requested DID is available with in any of the Range parameters*/

            /*this index is used if the DID falls in any of the DID range*/
            /* Check if the DID sent from tester is configured in Server s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16 */
            dataFuncRetVal_en = Dcm_Prv_GetIndexOfDID(s_dataID_u16, &s_Dcm_idxRdbiDidIndexType_st);
            if(dataFuncRetVal_en ==  E_OK)
            {
                /* Calling function to get the length of DID and the DID parameter is passed with a null since index is sufficient for the
                 * normal DID, The DID value can be obtained from the Dcm_DIDConfig and where as for a range of DID this parameter has to
                 * be passed with the DID value */
                dataFuncRetVal_en=Dcm_GetLengthOfDIDIndex (&s_Dcm_idxRdbiDidIndexType_st,&Dcm_DataLengthOfCurrentData_u32,s_dataID_u16);
            }

            switch (dataFuncRetVal_en)
            {
                /* If length is obtained correctly */
                case E_OK:
                    Dcm_GetDataState_en = DCM_GETDATA_STATUS_GETDATA;
                    /* Reset the Signal index */
                    Dcm_DidSignalIdx_u16 = 0x0u;
                    s_datalengthinfo_u32 = 0x0u;
					Dcm_DspReadDidOpStatus_u8=DCM_INITIAL;
                    break;
                case DCM_E_PENDING:
                    dataRetVal_u8 = DCM_GETDATA_RETVAL_PENDING;
                    break;
                    /*If length is not obtained correctly*/
                case E_NOT_OK:

                case DCM_INFRASTRUCTURE_ERROR:
                default:
                    dataRetVal_u8 = DCM_GETDATA_RETVAL_INTERNALERROR;
                    /* Reset the state */
                    Dcm_GetDataState_en = DCM_GETDATA_STATUS_INIT;
                    break;
            }
        }
        if (Dcm_GetDataState_en == DCM_GETDATA_STATUS_GETDATA)
        {


#if (DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)
            Std_ReturnType dataNvmGetErrorRetVal_u8;
            Std_ReturnType dataNvmReadBlockRetVal_u8;
            NvM_RequestResultType dataRequestResult_u8;
#endif
            uint32 posnTarget_u32= (Dcm_GetDataTotalLength_u32 - Dcm_DataLengthOfCurrentData_u32);
#if(DCM_CFG_DSP_READ_VARIABLE_LENGTH!=DCM_CFG_OFF)
            uint16 dataSignallength_u16; /* Local variable to store the length for a particular signal */
            boolean rtn_infrastructurefcVal;/* Local variable to store the Dcm_IsInfrastructureErrorPresent_b function */
#endif
            uint32 dataSignallength_u32; /* Local variable to store the length for a particular signal */

            /*Initialize the target buffer with zero for all data bytes of the DIDs*/
            ptrDidConfig =&Dcm_DIDConfig[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16];
#if(DCM_CFG_DSP_READ_VARIABLE_LENGTH!=DCM_CFG_OFF)
            dataSignallength_u16 = 0x0;
#endif
            dataSignallength_u32 = 0x0;

            if (s_dataReadIfcVal_u8 !=DCM_E_PENDING)
            {
                /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type pointer is converted to void pointer*/
                DCM_MEMSET(&adrTargetBuffer_pu8[posnTarget_u32], (sint32)DCM_CFG_SIGNAL_DEFAULT_VALUE, Dcm_DataLengthOfCurrentData_u32);
            }
            /*Check if the did is a range did or not*/
            if(s_Dcm_idxRdbiDidIndexType_st.dataRange_b == FALSE)
            {
#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)
#if(DCM_CFG_ATOMICREAD_DID>0)
                if((TRUE == ptrDidConfig->AtomicorNewSRCommunication_b) && ((USE_ATOMIC_SENDER_RECEIVER_INTERFACE == ptrDidConfig->didUsePort_u8) || (USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE == ptrDidConfig ->didUsePort_u8)) && (ptrDidConfig->AtomicRead_var != NULL_PTR))
                {
                    /*TRACE[SWS_Dcm_01432]*/
                    uint16 AtomicReadIndex_u16;
                    AtomicReadIndex_u16 = Dcm_Prv_AtomicReadDid_index(ptrDidConfig->dataDid_u16);
                    s_dataReadIfcVal_u8 =(Std_ReturnType)(AtomicDidRead_cast[AtomicReadIndex_u16].AtomicRead_pfct)(ptrDidConfig->AtomicRead_var);
                    if(s_dataReadIfcVal_u8 == E_OK)
                    {
                        (void)(AtomicDidRead_cast[AtomicReadIndex_u16].AtomicRead_CopyData_pfct)(&adrTargetBuffer_pu8[posnTarget_u32]);
                    }
                }
                else
#endif
                {
                    /* Call ReadFnc for all DID data elements */
                    /*MR12 RULE 15.4 VIOLATION :More than one 'break' statement has been used to terminate this iteration statement. MISRA C:2012 Rule-15.4*/
                    while((Dcm_DidSignalIdx_u16 < ptrDidConfig->nrSig_u16) && (USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8))
                    {
                        ptrSigConfig  = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
                        ptrControlSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];
#if (DCM_CFG_DSP_ALLSIGNAL_FIXED_LENGTH != DCM_CFG_ON)

                    FixedLength_b=ptrSigConfig->dataFixedLength_b;

#endif
                        if(((s_readDataLengthFnc_retVal_u8==E_OK)&&(s_dataReadIfcVal_u8==E_OK)) || (s_readDataLengthFnc_retVal_u8==DCM_E_PENDING))
                        {
                            /*Check if Did data length is fixed or not*/
                            if(
                                    FALSE != FixedLength_b

            )
                            {
                                /*This portion is executed only for fixed length signal in a DID*/
                            s_posnTargetSig_u32 = (posnTarget_u32+(ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16));

                                /* If the signal is the last boolean signal in a byte or if the boolean signal is the last signal of the DID, increment the data length by 1*/
                                /* If the signal is the last boolean signal in a byte or if the boolean signal is the last signal of the DID, increment the data length by 1*/
                                if((ptrSigConfig->dataType_u8==DCM_BOOLEAN) &&         \
                                        (((Dcm_DidSignalIdx_u16==(ptrDidConfig->nrSig_u16-1)))   ||                                      \
                                            (((ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16) !=              \
                                                    (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16+1].posnSigBit_u16)))))

                                {
                                    s_datalengthinfo_u32+=1u;
                                }
                                else if (ptrSigConfig->dataType_u8!=DCM_BOOLEAN)
                                {
                                s_datalengthinfo_u32  = ((ptrSigConfig->dataSize_u16) + (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16));
                                }
                                else
                                {
                                    /* Do nothing */
                                }
                                s_readDataLengthFnc_retVal_u8 = E_OK;
                            }
                            else
                            {
#if(DCM_CFG_DSP_READ_VARIABLE_LENGTH!=DCM_CFG_OFF)
                                /*This portion is executed only for variable length signal in a DID, to determine the target position for filling the Data in
                                 * the buffer for all the signals based on the length returned by the application*/
                                /*If the DID is a DDDI the length returned will be bytes, where as for a normal DID it will be in bits.*/
                                /*Check if the Did is Dynamically Defined DID or not*/
                                if(FALSE==ptrDidConfig->dataDynamicDid_b)
                                {
                                    /* Get the length from the configured API */
                                    if((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC) || (ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER))
                                    {
                                        s_readDataLengthFnc_retVal_u8 = (*(ptrControlSigConfig->adrReadDataLengthFnc_pfct.ReadDataLengthFnc1_pf))(&dataSignallength_u16);
                                    }
                                    else
                                    {
                                        if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC) || (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)

#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                                                || (ptrSigConfig->usePort_u8 == USE_DATA_RDBI_PAGED_FNC)
#endif
                                        )
                                        {
#if(DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
                                            if( (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                                            {
                                                if(!s_DcmReadDataLengthRteCallPlaced_b)
                                                {
                                                    s_readDataLengthFnc_retVal_u8 = (*(ptrControlSigConfig->adrReadDataLengthFnc_pfct.ReadDataLengthFnc6_pf))(s_Dcm_idxRdbiDidIndexType_st.dataopstatus_b);
                                                    if(s_readDataLengthFnc_retVal_u8 == E_OK)
                                                    {
                                                        s_DcmReadDataLengthRteCallPlaced_b = TRUE;
                                                        s_readDataLengthFnc_retVal_u8 = DCM_E_PENDING;
                                                    }
                                                    else
                                                    {
                                                        /* Set the NRC to General Reject for any other Return value */
                                                        s_readDataLengthFnc_retVal_u8 = E_NOT_OK;
                                                    }
                                                }
                                                else
                                                {
                                                    s_readDataLengthFnc_retVal_u8 = (*(ptrControlSigConfig->adrReadDataLengthFncResults_pfct.ReadDataLenResultFnc1_pf))(&dataSignallength_u16);
                                                    if (s_readDataLengthFnc_retVal_u8 == E_OK)
                                                    {
                                                        s_DcmReadDataLengthRteCallPlaced_b = FALSE;
                                                    }
                                                else if ((s_readDataLengthFnc_retVal_u8 == RTE_E_NO_DATA)||(s_readDataLengthFnc_retVal_u8 == DCM_E_PENDING))
                                                    {
                                                        s_readDataLengthFnc_retVal_u8 = DCM_E_PENDING;
                                                    }
                                                    else
                                                    {
                                                        s_DcmReadDataLengthRteCallPlaced_b = FALSE;
                                                        /* Set the NRC to General Reject for any other Return value */
                                                        s_readDataLengthFnc_retVal_u8 = E_NOT_OK;
                                                    }
                                                }
                                            }
                                            else
#endif
                                            {
                                                s_readDataLengthFnc_retVal_u8 = (*(ptrControlSigConfig->adrReadDataLengthFnc_pfct.ReadDataLengthFnc4_pf))(s_Dcm_idxRdbiDidIndexType_st.dataopstatus_b,&dataSignallength_u16);
                                            }
                                        }
                                    }
                                    dataSignallength_u32 = (uint32)dataSignallength_u16;
                                }
                                else
                                {
                                    /* Get the length from the configured API */
                                    s_readDataLengthFnc_retVal_u8 = (*(ptrControlSigConfig->adrReadDataLengthFnc_pfct.ReadDataLengthFnc2_pf))(&(dataSignallength_u32));
                                }

                                if(E_OK==s_readDataLengthFnc_retVal_u8)
                                {
                                    Dcm_DspReadDidOpStatus_u8=DCM_INITIAL;
                                    /* If the length received in more than the configured maximum length for a length
                               then return E_NOT_OK */
#if(DCM_CFG_READDATALENGTH_BITS!=DCM_CFG_OFF)
                                    /*Check if the Did is Dynamically Defined DID or not and data signal length is set to 0*/
                                    if((ptrDidConfig->dataDynamicDid_b == FALSE) &&  \
                                            ((dataSignallength_u32 > ptrSigConfig->dataSize_u16) ||(dataSignallength_u32==0u)))
#else
                                        /*Check if Did is  Dynamically Defined DID or not and data signal length is set to 0*/
                                        if(((ptrDidConfig->dataDynamicDid_b) == FALSE) &&  \
                                            (((dataSignallength_u32) > (ptrSigConfig->dataSize_u16)) ||(dataSignallength_u32==0u)))
#endif
                                        {
                                            s_dataReadIfcVal_u8 = E_NOT_OK;
                                            /* MR12 RULE 15.4 VIOLATION: More than one break statement is being used to terminate an iteration statement. */
                                            break;
                                        }
                                }
                                else
                                {
                                    rtn_infrastructurefcVal=Dcm_IsInfrastructureErrorPresent_b(s_readDataLengthFnc_retVal_u8);

                                    /*Check for Infrastructure Error*/
                                    if(((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER) || (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)) &&(rtn_infrastructurefcVal != FALSE))
                                    {

                                        s_dataReadIfcVal_u8 = DCM_INFRASTRUCTURE_ERROR;

                                    }
                                    else if(DCM_E_PENDING==s_readDataLengthFnc_retVal_u8)
                                    {
                                        s_dataReadIfcVal_u8 = DCM_E_PENDING;
                                        s_Dcm_idxRdbiDidIndexType_st.dataopstatus_b=DCM_PENDING;
                                    }
                                    else
                                    {
                                        s_dataReadIfcVal_u8 = E_NOT_OK;
                                    }
                                    /* MR12 RULE 15.4 VIOLATION: More than one break statement is being used to terminate an iteration statement. */
                                    break;
                                }
                                /*Check if Did is Dynamically Defined DID or not*/
                                if(FALSE == ptrDidConfig->dataDynamicDid_b)
                                {
#if(DCM_CFG_READDATALENGTH_BITS!=DCM_CFG_OFF)
                                    Dcm_ConvBitsToBytes(&dataSignallength_u32);
#endif
                                }
                                s_posnTargetSig_u32 = (uint32)(posnTarget_u32+s_datalengthinfo_u32);
                                s_datalengthinfo_u32 = (uint32)(s_datalengthinfo_u32+dataSignallength_u32);
#endif
                            }
                        }

                        /* Find the port configured for the Signal */
                        if (((ptrSigConfig->adrReadFnc_cpv.ReadFunc1_pfct)!= NULL_PTR)
#if(DCM_CFG_DSP_READ_NEW_SR_INTERFACE_ENABLED != DCM_CFG_OFF)
                        /* Check Did and Signal useport configured, When Sender receiver interface is enabled */
                             || ((TRUE == ptrDidConfig->AtomicorNewSRCommunication_b) && (USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8) && ((USE_DATA_SENDER_RECEIVER == ptrSigConfig->usePort_u8) || (USE_DATA_SENDER_RECEIVER_AS_SERVICE == ptrSigConfig->usePort_u8)))
#endif
                             )
                        {
                            /* Read operation is synchronous */
                            if((ptrSigConfig->usePort_u8 ==USE_DATA_SYNCH_FNC) ||          \
                                    (ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER))
                            {
#if(DCM_CFG_DSP_READ_SYNCH_FNC_ENABLED != DCM_CFG_OFF)
                                if(FALSE==ptrDidConfig->dataDynamicDid_b)
                                {
                                    /* Dcm437: Call function to check if conditions are fulfilled to read the DID */
                                    s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc1_pfct))(&adrTargetBuffer_pu8[s_posnTargetSig_u32]);
                                }
                                else
                                {
                                    /* Dcm437: Call function to check if conditions are fulfilled to read the DID */
                                    s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc10_pfct))(&adrTargetBuffer_pu8[s_posnTargetSig_u32],dataNegRespCode_u8);
                                }
#endif
                            }

#if(DCM_CFG_DSP_READ_ASYNCH_FNC_ENABLED != DCM_CFG_OFF)
                            /* Read operation is Asynchronous */
                            else if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC) ||
                                    (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER))
                            {
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
                                if( (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                                {
                                    if(!s_DcmRteCallPlacedReadData_b)
                                    {
                                        /* Call the relvant Rte_call with OpStatus Parameter */
                                        s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc11_ptr))(Dcm_DspReadDidOpStatus_u8);
                                        if(s_dataReadIfcVal_u8 == E_OK)
                                        {
                                            s_DcmRteCallPlacedReadData_b = TRUE;
                                            s_dataReadIfcVal_u8 = DCM_E_PENDING;
                                        }
                                        else
                                        {
                                            /* Set the NRC to General Reject for any other Return value */
                                            s_dataReadIfcVal_u8 = E_NOT_OK;
                                        }
                                    }
                                    else
                                    {
                                        /* Call the Rte_Result API with readdata parameter */
                                        s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFncResults_cpv.ReadResultFunc1_pfct))(&adrTargetBuffer_pu8[s_posnTargetSig_u32]);
                                        if (s_dataReadIfcVal_u8 == E_OK)
                                        {
                                            s_DcmRteCallPlacedReadData_b = FALSE;
                                        }
                                    else if((s_dataReadIfcVal_u8 == RTE_E_NO_DATA)||(s_dataReadIfcVal_u8 == DCM_E_PENDING))
                                        {
                                            s_dataReadIfcVal_u8 = DCM_E_PENDING;
                                        }
                                        else
                                        {
                                            s_DcmRteCallPlacedReadData_b = FALSE;
                                            /* Set the NRC to General Reject for any other Return value */
                                            s_dataReadIfcVal_u8 = E_NOT_OK;
                                        }
                                    }
                                }
                                else
#endif
                                {
                                    /* Dcm437: Call function to check if conditions are fulfilled to read the DID */
                                    s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc2_ptr))(Dcm_DspReadDidOpStatus_u8, &adrTargetBuffer_pu8[s_posnTargetSig_u32]);
                                }
                            }
#endif
                            else
                            {
#if(DCM_CFG_DSP_READ_SR_ENABLED != DCM_CFG_OFF)
                                /* Sender Receiver interfaces are used for reading */
                                if((FALSE == ptrDidConfig->AtomicorNewSRCommunication_b) && (USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8) && ((USE_DATA_SENDER_RECEIVER == ptrSigConfig->usePort_u8) || (USE_DATA_SENDER_RECEIVER_AS_SERVICE == ptrSigConfig->usePort_u8)))
                                {
#if( DCM_CFG_DSP_ALLSIGNAL_OPAQUE==DCM_CFG_OFF)
                                    DataEndiannessType_u8=ptrSigConfig->dataEndianness_u8;
#endif
                                    /* Check for the type of signal */
                                    switch(ptrSigConfig->dataType_u8)
                                    {
#if(DCM_CFG_DSP_READ_BOOLEAN_SR_ENABLED != DCM_CFG_OFF)
                                        case DCM_BOOLEAN:
                                        {   /* Read boolean signal */
                                            s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc3_pfct))(&dataSignal_b);
                                            /* Reset the variable before using */
                                            dataSignal_u32 = 0x0u;

                                            /* Copy the data if the signal read is set */
                                            if(dataSignal_b != FALSE)
                                            {
                                                dataSignal_u32 = 0x01u;
                                            }
                                            /* Store the read signal before proceeding to next signal */
                                            /* BSW-17603 *//* [$DD_BSWCODE 40531] */
                                            Dcm_StoreSignal(ptrSigConfig->dataType_u8,
                                                    (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8),
                                                    &adrTargetBuffer_pu8[posnTarget_u32], dataSignal_u32,
DataEndiannessType_u8

                                                        );
                                            break;
                                        }
#endif
#if(DCM_CFG_DSP_READ_UINT8_SR_ENABLED != DCM_CFG_OFF)
                                        case DCM_UINT8:
                                        {
                                        nrElements_u16 = (uint16)((uint32)ptrSigConfig->dataSize_u16);

                                        if(ptrSigConfig->dataSize_u16 == 1)
                                            {   /* Read uint8 signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc1_pfct))(&dataSignal_u8);
                                                Dcm_DspArraySignal_au8[0] = dataSignal_u8;
                                            }
                                            else /* Array of UINT8 */
                                            {   /* Read uint8 array of signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc1_pfct))(Dcm_DspArraySignal_au8);
                                            }
                                            /* Copy the signals to Global buffer */
                                            for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(s_dataReadIfcVal_u8 == E_OK)); idxLoop_u16++)
                                            {
                                                dataSignal_u32 = (uint32)Dcm_DspArraySignal_au8[idxLoop_u16];
                                                /* Store the read signal before proceeding to next signal */
                                            /* BSW-17603 *//* [$DD_BSWCODE 40532] */
                                                Dcm_StoreSignal(ptrSigConfig->dataType_u8,
                                                        (uint16)((ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*8u))),
                                                        &adrTargetBuffer_pu8[posnTarget_u32], dataSignal_u32,
                                                        DataEndiannessType_u8
                                                              );

                                            }
                                            break;
                                        }
#endif
#if(DCM_CFG_DSP_READ_UINT16_SR_ENABLED != DCM_CFG_OFF)
                                        case DCM_UINT16:
                                        {
                                        nrElements_u16 = (uint16)((uint32)(ptrSigConfig->dataSize_u16)/2u);

                                        if(ptrSigConfig->dataSize_u16 == 2)
                                            {   /* Read uint8 signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc4_pfct))(&dataSignal_u16);
                                                Dcm_DspArraySignal_au16[0] = dataSignal_u16;
                                            }
                                            else
                                            {   /* Read uint16 array of signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc4_pfct))(Dcm_DspArraySignal_au16);
                                            }
                                            /* Copy the signals to Global buffer */
                                            for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(s_dataReadIfcVal_u8 == E_OK)); idxLoop_u16++)
                                            {
                                                dataSignal_u32 = (uint32)Dcm_DspArraySignal_au16[idxLoop_u16];
                                                /* Store the read signal before proceeding to next signal */
                                            /* BSW-17603 *//* [$DD_BSWCODE 40533] */
                                                Dcm_StoreSignal(ptrSigConfig->dataType_u8,
                                                        (uint16)((ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint32)(idxLoop_u16*16u))),
                                                        &adrTargetBuffer_pu8[posnTarget_u32], dataSignal_u32,
DataEndiannessType_u8

                                                        );
                                            }
                                            break;
                                        }
#endif
#if(DCM_CFG_DSP_READ_UINT32_SR_ENABLED != DCM_CFG_OFF)
                                        case DCM_UINT32:
                                        {
                                        nrElements_u16 = (uint16)((uint32)(ptrSigConfig->dataSize_u16)/4u);

                                        if(ptrSigConfig->dataSize_u16 == 4)
                                            {   /* Read uint32 signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc5_pfct))(&dataSignal_u32);
                                                Dcm_DspArraySignal_au32[0] = dataSignal_u32;
                                            }
                                            else
                                            {   /* Read uint32 array of signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc5_pfct))(Dcm_DspArraySignal_au32);
                                            }
                                            /* Copy the signals to Global buffer */
                                            for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(s_dataReadIfcVal_u8 == E_OK)); idxLoop_u16++)
                                            {
                                                dataSignal_u32 = Dcm_DspArraySignal_au32[idxLoop_u16];
                                                /* Store the read signal before proceeding to next signal */
                                            /* BSW-17603 *//* [$DD_BSWCODE 40534] */
                                                Dcm_StoreSignal(ptrSigConfig->dataType_u8,
                                                        (uint16)((ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*32u))),
                                                        &adrTargetBuffer_pu8[posnTarget_u32], dataSignal_u32,
DataEndiannessType_u8

                                                        );
                                            }
                                            break;
                                        }
#endif
#if(DCM_CFG_DSP_READ_SINT8_SR_ENABLED != DCM_CFG_OFF)
                                        case DCM_SINT8:
                                        {
                                        nrElements_u16 = (uint16)((uint32)ptrSigConfig->dataSize_u16);

                                        if(ptrSigConfig->dataSize_u16 == 1)
                                            {   /* Read sint8 signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc6_pfct))(&dataSignal_s8);
                                                Dcm_DspArraySignal_as8[0] = dataSignal_s8;
                                            }
                                            else
                                            {   /* Read sint8 array of signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc6_pfct))(Dcm_DspArraySignal_as8);
                                            }
                                            /* Copy the signals to Global buffer */
                                            for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(s_dataReadIfcVal_u8 == E_OK)); idxLoop_u16++)
                                            {
                                                dataSignal_u32 = (uint32)Dcm_DspArraySignal_as8[idxLoop_u16];
                                                /* Store the read signal before proceeding to next signal */
                                            /* BSW-17603 *//* [$DD_BSWCODE 40535] */
                                                Dcm_StoreSignal(ptrSigConfig->dataType_u8,
                                                        (uint16)((ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint32)(idxLoop_u16*8u))),
                                                        &adrTargetBuffer_pu8[posnTarget_u32], dataSignal_u32,
DataEndiannessType_u8

                                                        );
                                            }
                                            break;
                                        }
#endif
#if(DCM_CFG_DSP_READ_SINT16_SR_ENABLED != DCM_CFG_OFF)
                                        case DCM_SINT16:
                                        {
                                        nrElements_u16 = (uint16)((uint32)(ptrSigConfig->dataSize_u16)/2u);

                                        if(ptrSigConfig->dataSize_u16 == 2)
                                            {   /* Read sint8 signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc7_pfct))(&dataSignal_s16);
                                                Dcm_DspArraySignal_as16[0] = dataSignal_s16;
                                            }
                                            else
                                            {   /* Read sint16 array of signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc7_pfct))(Dcm_DspArraySignal_as16);
                                            }
                                            /* Copy the signals to Global buffer */
                                            for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(s_dataReadIfcVal_u8 == E_OK)); idxLoop_u16++)
                                            {
                                                dataSignal_u32 = (uint32)Dcm_DspArraySignal_as16[idxLoop_u16];
                                                /* Store the read signal before proceeding to next signal */
                                            /* BSW-17603 *//* [$DD_BSWCODE 40536] */
                                                Dcm_StoreSignal(ptrSigConfig->dataType_u8,
                                                        (uint16)((ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*16u))),
                                                        &adrTargetBuffer_pu8[posnTarget_u32], dataSignal_u32,
DataEndiannessType_u8

                                                        );
                                            }
                                            break;
                                        }
#endif
#if(DCM_CFG_DSP_READ_SINT32_SR_ENABLED != DCM_CFG_OFF)
                                        case DCM_SINT32:
                                        {
                                        nrElements_u16 = (uint16)((uint32)(ptrSigConfig->dataSize_u16)/4u);

                                        if(ptrSigConfig->dataSize_u16 == 4)
                                            {   /* Read sint8 signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc8_pfct))(&dataSignal_s32);
                                                Dcm_DspArraySignal_as32[0] = dataSignal_s32;
                                            }
                                            else
                                            {   /* Read sint32 array of signal */
                                                s_dataReadIfcVal_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc8_pfct))(Dcm_DspArraySignal_as32);
                                            }
                                            /* Copy the signals to Global buffer */
                                            for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(s_dataReadIfcVal_u8 == E_OK)); idxLoop_u16++)
                                            {
                                                dataSignal_u32 = (uint32)Dcm_DspArraySignal_as32[idxLoop_u16];
                                                /* Store the read signal before proceeding to next signal */
                                            /* BSW-17603 *//* [$DD_BSWCODE 40537] */
                                                Dcm_StoreSignal(ptrSigConfig->dataType_u8,
                                                        (uint16)((ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*32u))),
                                                        &adrTargetBuffer_pu8[posnTarget_u32], dataSignal_u32,
DataEndiannessType_u8

                                                        );
                                            }
                                            break;
                                        }
#endif
                                        default:
                                        {   /* Dummy default case */
                                            break;
                                        }
                                    }
                                }
#if(DCM_CFG_DSP_READ_NEW_SR_INTERFACE_ENABLED != DCM_CFG_OFF)
                                else
                                {
                                    if((TRUE == ptrDidConfig->AtomicorNewSRCommunication_b) && (USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8) && ((USE_DATA_SENDER_RECEIVER == ptrSigConfig->usePort_u8) || (USE_DATA_SENDER_RECEIVER_AS_SERVICE == ptrSigConfig->usePort_u8)))
                                    {
                                        s_dataReadIfcVal_u8 = (Std_ReturnType)(ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].ReadSenderReceiver_pfct)(ptrSigConfig->ReadDataVar);

                                        if(E_OK == s_dataReadIfcVal_u8)
                                        {
                                            /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as StoreSignal function takes void pointer as input and object type pointer is converted to void pointer*/
                                            (void)(ptrSigConfig->StoreSignal_pfct)(&adrTargetBuffer_pu8[posnTarget_u32], ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16);
                                        }
                                    }
                                }
#endif
#endif
                            }
                            if(s_dataReadIfcVal_u8!=E_OK)
                            {
                                /* MR12 RULE 15.4 VIOLATION: More than one break statement is being used to terminate an iteration statement. */
                                break;
                            }
#if(DCM_CFG_DSP_READ_SYNCH_FNC_ENABLED != DCM_CFG_OFF)
                            else if(FALSE != ptrDidConfig->dataDynamicDid_b)
                            {
                                *dataNegRespCode_u8=0;
                            }
#endif
                            else
                            {
                                /* Reset the OpStatus to DCM_INITIAL for the next DID in the loop */
                                Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                            }


                        }
#if (DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)
                        else if(ptrSigConfig->usePort_u8 == USE_BLOCK_ID)

                        {
                            /*Check if Read non volatile memory status is pending or not*/
                            if(Dcm_flgNvmReadPending_b== FALSE)
                            {
                                /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as NvM_ReadBlock takes void pointer as input and object type pointer is converted to void pointer*/
                                dataNvmReadBlockRetVal_u8=NvM_ReadBlock(ptrSigConfig->dataNvmBlockId_u16,&adrTargetBuffer_pu8[s_posnTargetSig_u32]);
                                if(dataNvmReadBlockRetVal_u8 != E_OK)
                                {
                                    Dcm_flgNvmReadPending_b= FALSE;
                                    s_dataReadIfcVal_u8 =E_NOT_OK;
                                }
                                else
                                {
                                    /*Set the flag for non volatile memory ReadPending status is to TRUE*/
                                    Dcm_flgNvmReadPending_b= TRUE;
                                    s_dataReadIfcVal_u8 =DCM_E_PENDING;
                                }
                                /* MR12 RULE 15.4 VIOLATION: More than one break statement is being used to terminate an iteration statement. */
                                break;
                            }
                            else
                            {
                                dataNvmGetErrorRetVal_u8= NvM_GetErrorStatus(ptrSigConfig->dataNvmBlockId_u16,&dataRequestResult_u8);
                                /*Set the flag for non volatile memory ReadPending status is to FALSE*/
                                Dcm_flgNvmReadPending_b =FALSE;
                                if(dataNvmGetErrorRetVal_u8 != E_OK)
                                {
                                    s_dataReadIfcVal_u8 =E_NOT_OK;
                                    /* MR12 RULE 15.4 VIOLATION: More than one break statement is being used to terminate an iteration statement. */
                                    break;
                                }
                                else
                                {
                                    if((dataRequestResult_u8 == NVM_REQ_INTEGRITY_FAILED) || (dataRequestResult_u8 == NVM_REQ_NV_INVALIDATED) ||
                                            (dataRequestResult_u8 == NVM_REQ_CANCELED) || (dataRequestResult_u8 == NVM_REQ_REDUNDANCY_FAILED) ||
                                            (dataRequestResult_u8 == NVM_REQ_RESTORED_FROM_ROM) || (dataRequestResult_u8 == NVM_REQ_NOT_OK))
                                    {
                                        s_dataReadIfcVal_u8 =DCM_E_REQUEST_ENV_NOK;
                                    }
                                    else if(dataRequestResult_u8 == NVM_REQ_PENDING)
                                    {
                                        /*Set the flag for non volatile memory ReadPending status is to TRUE*/
                                        Dcm_flgNvmReadPending_b =TRUE;
                                        s_dataReadIfcVal_u8 = DCM_E_PENDING;
                                    }
                                    else if(dataRequestResult_u8==NVM_REQ_OK)
                                    {
                                        s_dataReadIfcVal_u8 =E_OK;
                                    }
                                    else
                                    {
                                        s_dataReadIfcVal_u8 =E_NOT_OK;
                                    }
                                    if(dataRequestResult_u8!=NVM_REQ_OK)
                                    {
                                        /* MR12 RULE 15.4 VIOLATION: More than one break statement is being used to terminate an iteration statement. */
                                        break;
                                    }
                                }
                            }
                        }
#endif
                        else
                        {
                            /*Dummy Else clause to remove MISRA warnings*/
                        }
                        Dcm_DidSignalIdx_u16++;
                    }
                }

                    switch (s_dataReadIfcVal_u8)
                    {
                        /*If data read successfully*/
                        case E_OK:
                        {
                            posnTarget_u32=(posnTarget_u32-2u);
                            adrTargetBuffer_pu8[posnTarget_u32]   = (uint8)((ptrDidConfig->dataDid_u16)>>8u);
                            adrTargetBuffer_pu8[posnTarget_u32+1u] = (uint8)((ptrDidConfig->dataDid_u16));
                            /* Updating the length to be filled */
                            Dcm_GetDataTotalLength_u32 = (Dcm_GetDataTotalLength_u32 - (Dcm_DataLengthOfCurrentData_u32 + 2u));
                            /* Updating the number of DIDs for which data has to be read  */
                            Dcm_GetDataNumOfIndex_u16--;
                            Dcm_GetDataState_en=DCM_GETDATA_STATUS_GETLENGTH;
                            break;
                        }
                        case DCM_E_PENDING:
                    {
                        *dataNegRespCode_u8=0x00;
                        dataRetVal_u8 = DCM_GETDATA_RETVAL_PENDING;
                        break;
                    }


                        case DCM_E_REQUEST_ENV_NOK:
                        {
                            if(E_OK == s_readDataLengthFnc_retVal_u8)
                            {
                                dataNrcRetval_u8 = DcmAppl_DcmReadDataNRC(s_dataID_u16,s_posnTargetSig_u32,dataNegRespCode_u8);
                            }

                            if(dataNrcRetval_u8!=E_OK)
                            {
                                *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                            }

                            dataRetVal_u8 =DCM_GETDATA_RETVAL_INVALIDCONDITIONS;
                            /* Reset the state */
                            Dcm_GetDataState_en = DCM_GETDATA_STATUS_INIT;
                            Dcm_DidSignalIdx_u16 = 0x0;
                            break;
                        }
                        /*If data read is not done successfully*/
                        case E_NOT_OK:
                        {
                            /*ReadData Application function has returned E_NOT_OK, Invoke DcmAppl to obtain the NRC value to be passed on to the tester*/
                            if(E_OK == s_readDataLengthFnc_retVal_u8)
                            {
                                dataNrcRetval_u8 = DcmAppl_DcmReadDataNRC(s_dataID_u16,s_posnTargetSig_u32,dataNegRespCode_u8);
                            }

                            if(dataNrcRetval_u8!=E_OK)
                            {
                                *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                            }
                        }
                        /* MR12 RULE 16.3 VIOLATION: Break statement in case E_NOT_OK is intentionally not inculded-for optimized code*/
                        default:
                        {

                            dataRetVal_u8 = DCM_GETDATA_RETVAL_INTERNALERROR;
                            /* Reset the state */
                            Dcm_GetDataState_en = DCM_GETDATA_STATUS_INIT;
                            Dcm_DidSignalIdx_u16 = 0x0;
                            break;
                        }
                    }

#endif
            }
            else
            {
#if (DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)
                if ((Dcm_DIDRangeConfig_cast[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16].adrReadFnc_pfct)!= NULL_PTR)
                {
                    *dataNegRespCode_u8=0x0;
                    s_dataReadIfcVal_u8 = (*(Dcm_DIDRangeConfig_cast[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16].adrReadFnc_pfct))
                                                                                                    (s_dataID_u16,&adrTargetBuffer_pu8[posnTarget_u32],Dcm_DspReadDidOpStatus_u8,&dataLength_u16,dataNegRespCode_u8);


                    switch (s_dataReadIfcVal_u8)
                    {
                        case E_OK:
                        {
                            posnTarget_u32=(posnTarget_u32-2u);
                            adrTargetBuffer_pu8[posnTarget_u32]   = (uint8)(s_dataID_u16>>8u);
                            adrTargetBuffer_pu8[posnTarget_u32+1u] = (uint8)(s_dataID_u16);
                            /* Updating the length to be filled */
                            Dcm_GetDataTotalLength_u32 = (Dcm_GetDataTotalLength_u32 - (Dcm_DataLengthOfCurrentData_u32 + 2u));
                            /* Updating the number of DIDs for which data has to be read  */
                            Dcm_GetDataNumOfIndex_u16--;
                            Dcm_GetDataState_en=DCM_GETDATA_STATUS_GETLENGTH;
                            Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                            break;
                        }
                        case DCM_E_PENDING:
                        {

                            /*Set the flag for non volatile memory ReadPending status is to TRUE*/
                            Dcm_flgDspDidRangePending_b = TRUE;
                            dataRetVal_u8 = DCM_GETDATA_RETVAL_PENDING;
                            break;
                        }
                        case E_NOT_OK:
                        {

                            /*ReadData Application function has returned E_NOT_OK, Invoke DcmAppl to obtain the NRC value to be passed on to the tester*/
                            dataNrcRetval_u8 = DcmAppl_DcmReadDataNRC(s_dataID_u16,0x0,dataNegRespCode_u8);
                            if(dataNrcRetval_u8!=E_OK)
                            {
                                *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                            }
                        }
                        /* MR12 RULE 16.3 VIOLATION: The previous case statement need not break as activities in default state also needs to be executed*/
                        default:
                        {

                            dataRetVal_u8 = DCM_GETDATA_RETVAL_INTERNALERROR;
                            /* Reset the state */
                            Dcm_GetDataState_en = DCM_GETDATA_STATUS_INIT;
                            Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                            break;
                        }
                    }
                }
#endif
            }
            /*Added to avoid compiler warnings*/
#if(DCM_CFG_DSP_READ_VARIABLE_LENGTH!=DCM_CFG_OFF)
            (void)dataSignallength_u16;
#endif
            (void)dataSignallength_u32;
        }
    }
    if (dataRetVal_u8 == DCM_GETDATA_RETVAL_OK)
    { /* If all are correctly processed we need to set our status back to init for the next call */
        Dcm_GetDataState_en=DCM_GETDATA_STATUS_INIT;
    }
    return dataRetVal_u8;
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

/**
 **************************************************************************************************
 * Dcm_DcmReadDataByIdentifier (Dcm253):
 *  The ReadDataByIdentifier service allows the client to request data record values from the server
 *  identified by one or more dataIdentifiers.
 *  Function is called by DSD, when ReadDataByIdentifier request is received (SID set to 0x22),
 *  this service provided in DSP module and configured in DCM identifier table.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_DcmReadDataByIdentifier (Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType * pMsgContext,Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    /*TRACE[SWS_Dcm_00253]*/

    Std_ReturnType dataRetVal_u8;
    /* Initialization of local variables */
    *dataNegRespCode_u8   = 0x0u;
    dataRetVal_u8=DCM_E_PENDING;


    /* If OpStatus is set to DCM_CANCEL then call the ini function for resetting */
    if (OpStatus == DCM_CANCEL)
    {
        /* Call the Ini Function */
        Dcm_Dsp_RdbiIni();
        /* Set the return value to E_OK as the Ini function will always be serviced  */
        dataRetVal_u8 = E_OK;
    }
    else
    {
        /* On the first service call */
        if (Dcm_stRdbi_en == DCM_RDBI_IDLE)
        {
            /* Check the request length: */
            /* - Minimum request length must be a DID length */
            /* - Request length must be even */
            if ((pMsgContext->reqDataLen < DCM_RDBI_SIZE_DID) ||
                    ((pMsgContext->reqDataLen & 0x0001u) != 0uL))
            {
                /* Invalid Request Length --> Set corresponding Negative response code */
                *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
            }
            /* If request length is correct */
            else
            {
                /* Get number of requested DIDs within the request */
                /* (at this place pMsgContext->reqDataLen is necessarily even) */
                Dcm_RdbiReqDidNb_u16 = (uint16)(pMsgContext->reqDataLen / DCM_RDBI_SIZE_DID);

                /* Dcm638: If a maximum number of DIDs within one request was configured (non zero)  */
                /* and if the number of requested DIDs in the request exceeds this maximum allowed   */
                if ((DCM_CFG_MAXDIDTOREAD != 0u) &&
                        (Dcm_RdbiReqDidNb_u16 > DCM_CFG_MAXDIDTOREAD))
                {
                    /* Set Negative response code "Incorrect Message Length Or invalid format" */
                    *dataNegRespCode_u8  = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                    Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
                }
                else
                {
                    Dcm_stRdbi_en = DCM_RDBI_GET_LENGTH;
                }
            }
        }
        if (Dcm_stRdbi_en == DCM_RDBI_GET_LENGTH)
        {
            /* Local variable to store return value of function Dcm_DspGetTotalLengthOfDIDs_en */
            Dcm_LenCalcRet_ten dataFuncRetVal_en;
#if((DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)||(DCM_CFG_LIMITRDBIRESPTOBUFSIZE != DCM_CFG_OFF))
            Dcm_DspRdbiMaxRespLen_u32 = (pMsgContext->resMaxDataLen);
#endif
            /* Calling API to get the total length of DIDs */
            dataFuncRetVal_en = Dcm_DspGetTotalLengthOfDIDs_en (&pMsgContext->reqData[0],
                    Dcm_RdbiReqDidNb_u16,
                    &Dcm_NumOfIndices_u16,
                    &Dcm_TotalLength_u32,
                    dataNegRespCode_u8);
            switch (dataFuncRetVal_en)
            {
                case DCM_LENCALC_RETVAL_OK:
                    /* Calculate total length after adding space for the DIDs in the response */
                    Dcm_TotalLength_u32 = (uint32)(Dcm_TotalLength_u32 + (2 * Dcm_NumOfIndices_u16));

                    /* Calculate total response length */
                    pMsgContext->resDataLen = (Dcm_MsgLenType)Dcm_TotalLength_u32;

#if (DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                    /*Check if the data fits within the can frames maximum possible data size allowed by protocol using multiple frames*/
                    if(Dcm_Prv_CheckTotalResponseLength(pMsgContext->resDataLen) != FALSE)
                    {
                        /* Initializing the filled response length in page and the total filled response length */
                        s_Dcm_FilledLengthCurrentPage_u32 = 0x0;

                        /*Variable set to false to indicate that the first page is not set*/

                        Dcm_RdbiFirstPageSent_b = FALSE;

                        /* Move the RDBI state machine to get Data from the application*/
                        Dcm_stRdbi_en = DCM_RDBI_GET_DATA;

                        Dcm_adrUpdatePage_pfct = &Dcm_Dsp_RDBIUpdatePage;

                        /* Call the DCM API and start Paged processing */
                        Dcm_StartPagedProcessing(pMsgContext);
                    }
                    else
                    {
                        /* Response buffer exceeded - set negative response code */
                        *dataNegRespCode_u8     = DCM_E_RESPONSETOOLONG;
                        Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
                    }
#else
                    /* Check if data fit into response buffer */
                    if (Dcm_TotalLength_u32<= pMsgContext->resMaxDataLen)
                    {
                        Dcm_stRdbi_en = DCM_RDBI_GET_DATA;
                    }
                    /* If the total response length is greater than available buffer length */
                    else
                    {
                        /* Set negative response code */
                        *dataNegRespCode_u8  = DCM_E_RESPONSETOOLONG;
                        Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
                    }
#endif
                    /* call the application to validate the response length in case of multi DID request and at least one DID is valid */
                    if((Dcm_RdbiReqDidNb_u16 > 1)&&(Dcm_NumOfIndices_u16!=0)&&(Dcm_stRdbi_en == DCM_RDBI_GET_DATA))
                    {
                        Std_ReturnType dataCheckLenRetVal_u8; /* Local variable to store the return value */
                        /* calling the application to do the response length validation */
                        dataCheckLenRetVal_u8 = DcmAppl_DcmCheckRdbiResponseLength(Dcm_TotalLength_u32,Dcm_NumOfIndices_u16,dataNegRespCode_u8);
                        if((*dataNegRespCode_u8==0) && (dataCheckLenRetVal_u8 != E_OK))
                        {
                            *dataNegRespCode_u8 = DCM_E_RESPONSETOOLONG;
                            Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
                        }
                        else if((*dataNegRespCode_u8 !=0)&&(dataCheckLenRetVal_u8 != E_OK) )
                        {
                            Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
                        }
                        else
                        {
                            /* dummy else to remove the MISRA warning */
                        }
                    }
                    Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                    break;
                case DCM_LENCALC_RETVAL_PENDING:
                    /* Do nothing and call again */
                    Dcm_DspReadDidOpStatus_u8 = DCM_PENDING;
                    dataRetVal_u8=DCM_E_PENDING;
                    break;
                case DCM_LENCALC_RETVAL_ERROR:
                    /* Keep Negative response code of sub-function */
                    Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
                    Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                    break;
                default:
                    /* Set Negative response code  */
                    *dataNegRespCode_u8  = DCM_E_GENERALREJECT;
                    Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
                    Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                    break;
            }
        }
        if (Dcm_stRdbi_en == DCM_RDBI_GET_DATA)
        {
            /*TRACE[SWS_Dcm_00433]*/
            /*TRACE[SWS_Dcm_00438]*/
            /*TRACE[SWS_Dcm_00651]*/
            /* Check if number of valid DIDs (DIDs supported and which has data) is zero */
            if (Dcm_NumOfIndices_u16==0)
            {
                /* Set Negative response code for non supported ID */
                *dataNegRespCode_u8  = DCM_E_REQUESTOUTOFRANGE;
                Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
            }
            else
            {
                /*TRACE[SWS_Dcm_00652]*/
                Dcm_GetDataRet_ten dataFuncRetVal_en; /* Local variable to store the return value */
                /* Calling function to read data corresponding to the valid DIDs in request  */
#if (DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                dataFuncRetVal_en= Dcm_GetData_en(&pMsgContext->reqData[0],
                        Dcm_DspRdbiRespBufPtr_u8,
                        Dcm_NumOfIndices_u16,
                        dataNegRespCode_u8
                );
#else
        dataFuncRetVal_en= Dcm_GetData_en(&pMsgContext->reqData[0],
                pMsgContext->resData,
                Dcm_NumOfIndices_u16,
                dataNegRespCode_u8,
                Dcm_TotalLength_u32
        );
#endif

            switch (dataFuncRetVal_en)
            {
                case DCM_GETDATA_RETVAL_OK:
                    /* Set final response length */
                    pMsgContext->resDataLen =  Dcm_TotalLength_u32;
                    /* Reset Signal Index to 0 */
                    Dcm_DidSignalIdx_u16 = 0x0;
                    dataRetVal_u8=E_OK;
                    /* Update the state machine status to DCM_RDBI_IDLE */
                    Dcm_stRdbi_en = DCM_RDBI_IDLE;
                    Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                    break;
                case DCM_GETDATA_RETVAL_PENDING:
                    /* do nothing and call again */
                    Dcm_DspReadDidOpStatus_u8 = DCM_PENDING;
                    dataRetVal_u8=DCM_E_PENDING;
                    break;
                case DCM_GETDATA_RETVAL_INVALIDCONDITIONS:
                    *dataNegRespCode_u8  = DCM_E_CONDITIONSNOTCORRECT;
                    Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
                    Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                    break;
#if (DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                case DCM_GETDATA_PAGED_BUFFER_TX:
                    /*Do Nothing here, state machine is handle with paged buffer transmission*/
                    dataRetVal_u8=DCM_E_PENDING;
                    break;
#endif
                case DCM_GETDATA_RETVAL_INTERNALERROR:
                default:
                    /* Set Negative response code "internal error" */
                    if(0x0 == *dataNegRespCode_u8)
                    {
                        *dataNegRespCode_u8  = DCM_E_GENERALREJECT;
                    }
                    else
                    {
                        /*Do Nothing NRC is already set by the application */
                    }
                    Dcm_stRdbi_en = DCM_RDBI_NEG_RESP;
                    Dcm_DspReadDidOpStatus_u8 = DCM_INITIAL;
                    break;
                }
            }
        }
        /* If any error was encountered, i.e. negative response code is set */
        if (Dcm_stRdbi_en == DCM_RDBI_NEG_RESP)
        {
            /* Reset Signal Index */
            Dcm_DidSignalIdx_u16 = 0x0;
            dataRetVal_u8=E_NOT_OK;
            /* Update the state machine status to DCM_RDBI_IDLE */
            Dcm_stRdbi_en = DCM_RDBI_IDLE;
        }
    }
    return dataRetVal_u8;
}

/**
 *************************************************************************************************************************************************************
 * Dcm_GetActiveRDBIDid:
 *
 * This API returns the active DID being processed when RDBI service request is being processed. This function is called from the Dcm API Dcm_GetActiveDid
 *
 *
 * \param     uint16* dataDid_u16 : Parameter for updating of the DID under processing. The DID value returned is valid only if return value is E_OK.
 * \retval    Std_ReturnType : E_OK : The DID under processing is a normal DID.The parameter dataDid_u16 contains valid DID value in this case.
 *                             E_NOT_OK: The DID under processing is a range DID. The parameter dataDid_u16 contains invalid data in this case.
 * \seealso
 *
 **************************************************************************************************************************************************************
 */
Std_ReturnType Dcm_GetActiveRDBIDid(uint16 * dataDid_u16)
{
    Std_ReturnType dataRetVal_u8; /* Variable to store the return value */
    const Dcm_DIDConfig_tst * ptrDidConfig;
    ptrDidConfig =&Dcm_DIDConfig[s_Dcm_idxRdbiDidIndexType_st.idxIndex_u16];

    /* Initialisation of local variables  */
    dataRetVal_u8 = E_NOT_OK;

    /* Check if the DID currently under process is a normal DID */

    if(s_Dcm_idxRdbiDidIndexType_st.dataRange_b==FALSE)
    {
#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF )
        /* Update the DID value currently under process */
        *dataDid_u16 = ptrDidConfig->dataDid_u16;
#endif
        /* Update the return value to E_OK */
        dataRetVal_u8 = E_OK;
    }
    else
    {
#if (DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)
        /* Update the DID value currently under process */
        *dataDid_u16 = s_Dcm_idxRdbiDidIndexType_st.dataRangeDid_16;
        /* Update the return value to E_OK */
        dataRetVal_u8 = E_OK;
#endif
    }
    return (dataRetVal_u8);
}


/**
 *******************************************************************************
 * Dcm_Prv_DspRdbiConfirmation : API used for confirmation of response sent for
 *										    ReadDataByIdentifier (0x22) service.
 * \param           dataIdContext_u8    Service Id
 * \param           dataRxPduId_u8      PDU Id on which request is Received
 * \param           dataSourceAddress_u16    Tester Source address id
 * \param           status_u8                Status of Tx confirmation function
 *
 * \retval          None
 * \seealso
 *
 *******************************************************************************
 */
void Dcm_Prv_DspRdbiConfirmation(
	Dcm_IdContextType dataIdContext_u8,
	PduIdType dataRxPduId_u8,
	uint16 dataSourceAddress_u16,
	Dcm_ConfirmationStatusType status_u8)
{
#if (DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
			/* Set the state of main state machine to Initialized state */
			Dcm_Dsp_RdbiIni();
#endif
DcmAppl_DcmConfirmation(dataIdContext_u8,dataRxPduId_u8,dataSourceAddress_u16,
																	status_u8);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif


