
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"


#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"

#if(   (DCM_CFG_DSP_RDTCSUBFUNC_0x02_ENABLED != DCM_CFG_OFF)\
     ||(DCM_CFG_DSP_RDTCSUBFUNC_0x0A_ENABLED != DCM_CFG_OFF)\
     ||(DCM_CFG_DSP_RDTCSUBFUNC_0x15_ENABLED != DCM_CFG_OFF)\
       /* FC_VariationPoint_START */                        \
     ||(DCM_CFG_DSP_RDTCSUBFUNC_0x13_ENABLED != DCM_CFG_OFF)\
       /* FC_VariationPoint_END */                          \
     ||(DCM_CFG_DSP_RDTCSUBFUNC_0x0F_ENABLED != DCM_CFG_OFF)\
     ||(DCM_CFG_DSP_RDTCSUBFUNC_0x17_ENABLED != DCM_CFG_OFF))




/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

static Dcm_MsgLenType Dcm_Dsp_CalTotalResLengthForDTC(uint16 NumOfFltDTC_u16, uint8 reqSubfunction);
static uint8 Dcm_Dsp_GetDTCRequestLength(const uint8 reqSubfunction);
static uint8 Dcm_Dsp_CalDTCActiveMask(const uint8 statusAvailMask_u8,const uint8 reqDtcStatus_u8,const uint8 reqSubfunction);
static void Dcm_Dsp_CalDTCOrigin(Dem_DTCOriginType *dataDtcOrigin_u16,uint8 *dataDtcStatus_u8,const uint8 reqSubfunction, \
        const uint8 reqDtcOriginData);
static void Dcm_Dsp_FillResBytesAndLength(Dcm_MsgContextType *pMsgContext,const uint8 reqSubfunction, \
                                          const uint8 statusAvailMask_u8);
static Std_ReturnType Dcm_Dsp_FillResDtcBuffer(uint32 dtcData_u32,Dcm_MsgLenType *respLenNr_u32, \
                                                       uint8 stDTCStatus_u8,const Dcm_MsgContextType *pMsgContext);


/**
 **************************************************************************************************
 * Dcm_Dsp_CalTotalResLengthForDTC :
 *  This function is used to calcualte the total response length for the all dtcs for paged buffer
 * \param           uint16    Nummer of filtered dtc
 *                  uint8     requested sub function
 *
 * \retval          Dcm_MsgLenType  total length
 *
 **************************************************************************************************
 */
/*TRACE[SWS_Dcm_00587]*/
static Dcm_MsgLenType Dcm_Dsp_CalTotalResLengthForDTC(uint16 NumOfFltDTC_u16, uint8 reqSubfunction)
{
    Dcm_MsgLenType totalLength = 0;

    totalLength = (Dcm_MsgLenType) ((((uint16) NumOfFltDTC_u16) << 0x02u) + 0x02);
#if (DCM_CFG_DSP_RDTCSUBFUNC_0x17_ENABLED != DCM_CFG_OFF)
    if (reqSubfunction == DSP_REPORT_USER_DEFMEMORY_DTC_BY_STATUS_MASK)
    {
        totalLength = (Dcm_MsgLenType) ((((uint16) NumOfFltDTC_u16) << 0x02u) + 0x03);
    }
#else
    (void)reqSubfunction;
#endif
    return totalLength;
}


/**
 ***********************************************************************************************************************
 * Dcm_Dsp_ReportSupportedDTC :
 *  This function is used to implement the following subfunctions:
 *  0x02( Report DTC By Status Mask       )
 *  0x0A( Report Supported DTCs           )
 *  0x15( Report DTC By Permanent Status  )
 *  0x0F( Report Mirror Memory DTC By StatusMask  )
 *  0x17( ReportUserDefMemoryDTCByStatusMask)
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 ***********************************************************************************************************************
 */
#if((DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_RDTCPAGEDBUFFERSUPPORT != DCM_CFG_OFF))

Std_ReturnType Dcm_Dsp_ReportSupportedDTC (Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType *pMsgContext,Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint32            dataDTC_u32;              /* Variable to store DTC */
    uint16            idxLoop_qu16;             /* Variable for loop counter */
    Dcm_MsgLenType    nrRespLen_u32;            /* Local variable for response length */
    Dcm_MsgLenType    nrResMaxDataLen_u32;      /* Variable to store max response length */
    uint16            nrFltDTC_u16;             /* Variable to store number of filtered DTCs */
    uint8             nrReqLength_u8;           /*Variable to store request length */
    uint8             dataStatusAvailMask_u8;   /* Variable to store status availability mask */
    uint8             stDTCStatus_u8;           /* Variable to store effective status mask */
    Std_ReturnType    dataRetGetNextFiltDTC_u8; /* Return type for Dem_GetNextFilteredDTC */
    Std_ReturnType    dataRetNumFltDTC_u8;      /* Return type for Dem_GetNumberOfFilteredDTC */
    Std_ReturnType    dataRetSetDTCFilter_u8;   /* Return type for Dem_SetDTCFilter */
    Dem_DTCOriginType dataDemDTCOrigin_u16;      /* Local variable to store the memory type */
    uint8             dataSubfunc_u8;           /* Local variable to store the subfunction */
    Std_ReturnType    dataretVal_u8 = DCM_E_PENDING;   /* Variable to store the returnType from DcmAppl API */
    boolean           IsProtocolIPCanFD_b;      /* Local variable to check whether the protocol is CANFD SPECIFIC PROTOCOLS*/
    static boolean    isDataPending_flag_b;       /* varible to indicate datapending */

    /* Initialization of local variables */
    *dataNegRespCode_u8      = 0x0u;
    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);

    /* State machine for Report Supported DTC */
    /* Initialization state */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
    {
        /* Copy the subfunction parameter to the local variable */
        dataSubfunc_u8 = pMsgContext->reqData[0];
        /* calculate the request length according to the request data (subfunction No.) */
        nrReqLength_u8 = Dcm_Dsp_GetDTCRequestLength(dataSubfunc_u8);

        /* Check for request length */
        if(pMsgContext->reqDataLen == nrReqLength_u8)
        {
            /* If the available status mask is read successfully */
            if(E_OK == Dem_GetDTCStatusAvailabilityMask(ClientId_u8,&dataStatusAvailMask_u8))
            {
                /* Initializing the filled response length in page and the total filled response length */
                Dcm_DspRDTCFilledRespLen_u32  = 0x00;
                Dcm_DspTotalRespLenFilled_u32 = 0x00;
                /* Set the variable to FALSE to indicate that the zero's should not be filled in page */
                Dcm_DspFillZeroes_b     = FALSE;
                /* initial the local copy of available mask */
                stDTCStatus_u8 = dataStatusAvailMask_u8;

                /*TRACE[SWS_Dcm_00008]*/
                /* calculate the active DTC status mask for the service 0x02,0x13,0x0F and 0x17 */
                stDTCStatus_u8 = Dcm_Dsp_CalDTCActiveMask(dataStatusAvailMask_u8, pMsgContext->reqData[1],dataSubfunc_u8);

                /* If the status mask is 0 */
                if(stDTCStatus_u8 != 0x0)
                {
                    if(dataSubfunc_u8 == DSP_REPORT_OBD_DTC_BY_STATUS_MASK)
                    {
                        /* Assign the DTC Origin as Relevant memory */
                        dataDemDTCOrigin_u16 = DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY;
                    }
                    else
                    {
                        /* Assign the DTC Origin as Primary memory */
                        dataDemDTCOrigin_u16 = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
                    }
                    /* calculation of DTC Origin and DTC kind, update DTC mask */
                    Dcm_Dsp_CalDTCOrigin(&dataDemDTCOrigin_u16,&stDTCStatus_u8,dataSubfunc_u8,pMsgContext->reqData[2]);

                    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3721]*/
                    /* Set the calculated info. to the DTC Filter in DEM */
                    dataRetSetDTCFilter_u8 = Dem_SetDTCFilter(ClientId_u8,
                                                              stDTCStatus_u8,
                                                              DEM_DTC_FORMAT_UDS,
                                                              dataDemDTCOrigin_u16,
                                                              FALSE,
                                                              DEM_SEVERITY_NO_SEVERITY,
                                                              FALSE
                                                             );

                    /* If the Filter Mask sent is accepted by DEM */
                    if(E_OK == dataRetSetDTCFilter_u8)
                    {
                        /* Move the state to calculate the number of filtered DTC */
                        Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFCALCNUMDTC;
                    }
                    /* Filter Mask not accepted by DEM */
                    else
                    {
                        /* Set Negative response */
                        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                        dataretVal_u8 = E_NOT_OK;
                    }
                }

                /*TRACE[SWS_Dcm_00008]*/
                else
                {
                    /* Do Nothing */
                    dataretVal_u8 = E_OK;
                }
                /* Fill response bytes for subfunction 0x02,0x13,0x0F,0x15,0x17 or 0x0A */
                Dcm_Dsp_FillResBytesAndLength(pMsgContext,dataSubfunc_u8, dataStatusAvailMask_u8);
            }
            /* Status mask is not read properly from DEM */
            else
            {
                /* Set Negative response */
                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                dataretVal_u8 = E_NOT_OK;
            }
        }
        /* Request Length not correct */
        else
        {
            /* Set Negative response */
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            dataretVal_u8 = E_NOT_OK;
        }
    }

    /* State to calculate the number of filtered DTC */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFCALCNUMDTC)
    {
        dataSubfunc_u8 = pMsgContext->reqData[0];

        /* Call the DEM API to get filtered number of DTC */
        dataRetNumFltDTC_u8 = Dem_GetNumberOfFilteredDTC(ClientId_u8,&nrFltDTC_u16);

        /* If filtered DTC number is obtained without error */
        if(E_OK == dataRetNumFltDTC_u8)
        {
            /* Check if number of filtered DTCs is nonzero */
            if(nrFltDTC_u16 != 0u)
            {
                /* Calculate total response length */
                pMsgContext->resDataLen = Dcm_Dsp_CalTotalResLengthForDTC(nrFltDTC_u16,dataSubfunc_u8);

                if(FALSE != Dcm_Prv_CheckTotalResponseLength(pMsgContext->resDataLen))
                {
                    /* Set variable to FALSE to indicate that first page has not been sent */
                    Dcm_DspFirstPageSent_b  = FALSE;

                    /* Move the state to next state */
                    Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;

                    Dcm_adrUpdatePage_pfct = &Dcm_Dsp_RDTCUpdatePage;

                    /* Call the DCM API and start Paged processing */
                    Dcm_StartPagedProcessing(pMsgContext);
                    /* Update the filled response length variable and total response length filled */
                    Dcm_DspRDTCFilledRespLen_u32 = 2;
                    Dcm_DspTotalRespLenFilled_u32 = 2;

#if (DCM_CFG_DSP_RDTCSUBFUNC_0x17_ENABLED != DCM_CFG_OFF)
                    if(dataSubfunc_u8 == DSP_REPORT_USER_DEFMEMORY_DTC_BY_STATUS_MASK)
                    {
                        /* Update the filled response length variable and total response length filled */
                        Dcm_DspRDTCFilledRespLen_u32 = 3;
                        Dcm_DspTotalRespLenFilled_u32 = 3;
                    }
#endif
                }
                else
                {
                    /* Response buffer exceeded - set negative response code */
                    *dataNegRespCode_u8     = DCM_E_RESPONSETOOLONG;
                    dataretVal_u8 = E_NOT_OK;
                }
            }
            else
            {
                /* Move the state to default state */
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
                dataretVal_u8 = E_OK;
            }
        }
        /* If more time is needed for getting the number of filtered DTC */
        else if(DEM_PENDING == dataRetNumFltDTC_u8)
        {
            /* Do nothing - remain in the same state and call the API again */
            dataretVal_u8 = DCM_E_PENDING;
        }
        else
        {
            /* Set negative response */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
            dataretVal_u8 = E_NOT_OK;
        }
    }

    /* State to get the filtered DTC and status from DEM then fill the response */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
    {
        idxLoop_qu16 = 0;
        /* Store the maximum response length in local variable */
        nrResMaxDataLen_u32 = Dcm_DspRDTCMaxRespLen_u32;

        /* loop till the maximum DTC's are read and the filtered DTC is read without error */
        do
        {
            idxLoop_qu16++;

            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3721]*/
            /* Call the DEM API and read DTC and its status */
            dataRetGetNextFiltDTC_u8 = Dem_GetNextFilteredDTC(ClientId_u8,&dataDTC_u32, &stDTCStatus_u8);
            /* If the DTC is filtered without error */
            if(E_OK == dataRetGetNextFiltDTC_u8)
            {
                if ((Dcm_DsldPduInfo_st.SduLength != 0u) && (isDataPending_flag_b == FALSE))
                {
                    nrRespLen_u32 = 0 ;
                    Dcm_DspRDTCFilledRespLen_u32 = Dcm_DspRDTCFilledRespLen_u32 + Dcm_DsldPduInfo_st.SduLength;
                    isDataPending_flag_b = TRUE;
                }
                dataRetGetNextFiltDTC_u8 = Dcm_Dsp_FillResDtcBuffer(dataDTC_u32,&nrRespLen_u32,stDTCStatus_u8,pMsgContext);
            }
            /* If more time is needed to get the filtered DTC */
            else if(DEM_PENDING == dataRetGetNextFiltDTC_u8)
            {
                /* Nothing to do - Remain in the same state */
                dataretVal_u8 = DCM_E_PENDING;
                break;
            }
            /* If all the filtered DTC's are read */
            else if(DEM_NO_SUCH_ELEMENT == dataRetGetNextFiltDTC_u8)
            {
                /* Do nothing */
            }
            else
            {
                /* Check if first page is already sent or not */
                if(Dcm_DspFirstPageSent_b == FALSE)
                {
                    /* Set Negative response */
                    *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                    dataretVal_u8 = E_NOT_OK;
                }
                /* First page is already sent so NRC cannot be set */
                else
                {
                    /* Call process page with zero length to stop the response transmission immediately */
                    Dcm_ProcessPage(0);

                    /* Move the state to default state */
                    Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
                }
            }

            IsProtocolIPCanFD_b = Dcm_IsProtocolIPCanFD();
            /* Check if the buffer overflow happens or all filtered DTC's are read */
            if(     ((IsProtocolIPCanFD_b) && ((Dcm_DspRDTCFilledRespLen_u32 + 0x04u) > nrResMaxDataLen_u32))
                ||  ((!IsProtocolIPCanFD_b) && (Dcm_DspRDTCFilledRespLen_u32>=7u))
                ||  (dataRetGetNextFiltDTC_u8 == DEM_NO_SUCH_ELEMENT)
              )
            {
                /* If first page is not sent yet */
                if(Dcm_DspFirstPageSent_b == FALSE)
                {
                    /* Set the variable to TRUE indicating first page is sent */
                    Dcm_DspFirstPageSent_b = TRUE;
                }

                /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3772]*/
                /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3773]*/
                if(dataRetGetNextFiltDTC_u8 == DEM_NO_SUCH_ELEMENT)
                {
                    if((Dcm_DsldPduInfo_st.SduLength != 0u) && (isDataPending_flag_b == FALSE))
                    {
                        nrRespLen_u32 = 0 ;
                        Dcm_DspRDTCFilledRespLen_u32 = Dcm_DspRDTCFilledRespLen_u32 + Dcm_DsldPduInfo_st.SduLength;
                        isDataPending_flag_b = TRUE;
                    }
                    if(pMsgContext->resDataLen > Dcm_DspTotalRespLenFilled_u32)
                    {
                        /* Calculating the number of remaining number of bytes to be filled in response */
                        nrRespLen_u32 = pMsgContext->resDataLen - Dcm_DspTotalRespLenFilled_u32;
                        Dcm_Dsp_RDTCFillZero(nrRespLen_u32);
                    }
                }

                if(  ((Dcm_DspRDTCFilledRespLen_u32 +2u) >= Dcm_DslTransmit_st.TxResponseLength_u32)
                   ||((Dcm_DspRDTCFilledRespLen_u32 +2u) >= nrResMaxDataLen_u32)
                   ||((Dcm_DspTotalRespLenFilled_u32+1u) >= Dcm_DslTransmit_st.TxResponseLength_u32)
                  )
                {
                    /* Move the state to next state */
                    Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFTXPAGE;
                    /* Send the page for transmission */
                    Dcm_ProcessPage(Dcm_DspRDTCFilledRespLen_u32);
                    isDataPending_flag_b =FALSE;
                    /* Reset the response length */
                    Dcm_DspRDTCFilledRespLen_u32 = 0x0;
                    /* Change the return so that the loop won't get executed again */
                    dataRetGetNextFiltDTC_u8 = DEM_NO_SUCH_ELEMENT;
                }
            }

        }while((idxLoop_qu16 < DCM_CFG_RDTC_MAXNUMDTCREAD) && (dataRetGetNextFiltDTC_u8 == E_OK));
    }


    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFTXPAGE)
    {
        /* Wait in this state for transmission of page */
    }

    /*TRACE[SWS_Dcm_00588]*/
    /* State where zero's are filled in page if total response length filled is less than actual response length */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLZERO)
    {
        if((Dcm_DsldPduInfo_st.SduLength != 0u ) && (isDataPending_flag_b == FALSE))
        {
            nrRespLen_u32 = 0 ;
            Dcm_DspRDTCFilledRespLen_u32 = Dcm_DspRDTCFilledRespLen_u32 + Dcm_DsldPduInfo_st.SduLength;
            isDataPending_flag_b = TRUE;
        }
        if(pMsgContext->resDataLen > Dcm_DspTotalRespLenFilled_u32)
        {
            /* Calculating the number of remaining number of bytes to be filled in response */
            nrRespLen_u32 = pMsgContext->resDataLen - Dcm_DspTotalRespLenFilled_u32;
            Dcm_Dsp_RDTCFillZero(nrRespLen_u32);
        }
        /* Move the state to next state */
        Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFTXPAGE;
        /* Send the page for transmission */
        Dcm_ProcessPage(Dcm_DspRDTCFilledRespLen_u32);
        isDataPending_flag_b = FALSE ;
        Dcm_DspRDTCFilledRespLen_u32 = 0;
    }
    return dataretVal_u8;
}



#else

Std_ReturnType Dcm_Dsp_ReportSupportedDTC (Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType *pMsgContext,Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint32  dataDTC_u32;                      /* Variable to store DTC */
    Dcm_MsgLenType  nrRespLen_u32;            /* Local variable for response length */
    Dcm_MsgLenType  nrResMaxDataLen_u32;      /* Variable to store max response length */
    uint16  idxLoop_qu16;                     /* Variable for loop counter */
    uint16  nrFltDTC_u16;                     /* Variable to store number of filtered DTCs */
    uint8  nrReqLength_u8;                    /* Variable to store request length */
    uint8  stDTCStatus_u8;                    /* Variable to store effective status mask */
    uint8  dataStatusAvailMask_u8;            /* Variable to store status availability mask */
    uint8  dataSubfunc_u8;                    /* Local variable to store the subfunction */
    Dem_DTCOriginType  dataDemDTCOrigin_u16;      /* Local variable to store the memory type */
    Std_ReturnType     dataRetSetDTCFilter_u8;   /* Return type for Dem_SetDTCFilter */
    Std_ReturnType     dataRetGetNextFiltDTC_u8; /* Return type for Dem_GetNextFilteredDTC */
    Std_ReturnType     dataretVal_u8 = DCM_E_PENDING;  /* Variable to store the returnType from DcmAppl API */
    Std_ReturnType    dataRetNumFltDTC_u8;      /* Return type for Dem_GetNumberOfFilteredDTC */
    (void)OpStatus;

    /* Initialization of local variables */
    *dataNegRespCode_u8 = 0x0;

    /* State machine for Report Supported DTC */
    /* Initialization state */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
    {
        /* Copy the subfunction parameter to the local variable */
        dataSubfunc_u8 = pMsgContext->reqData[0];
        /* calculate the request length according to the reqest data (subfunction No.) */
        nrReqLength_u8 = Dcm_Dsp_GetDTCRequestLength(dataSubfunc_u8);

        /* check for request length */
        if(pMsgContext->reqDataLen == nrReqLength_u8)
        {
            /* If the available status mask is read successfully */
            if(E_OK == Dem_GetDTCStatusAvailabilityMask(ClientId_u8,&dataStatusAvailMask_u8))
            {
                /* Store Status Availability mask in stDTCStatus_u8 for S/F 0x0A and 0x15 */
                stDTCStatus_u8  = dataStatusAvailMask_u8;

                /* calculate the active DTC status mask for the service 0x02,0x13,0x0F and 0x17 */
                stDTCStatus_u8 = Dcm_Dsp_CalDTCActiveMask(dataStatusAvailMask_u8, pMsgContext->reqData[1],dataSubfunc_u8);

                /*TRACE[SWS_Dcm_00008]*/
                /* If the status mask is 0 */
                if(stDTCStatus_u8 != 0x0u)
                {
                    if(dataSubfunc_u8 == DSP_REPORT_OBD_DTC_BY_STATUS_MASK)
                    {
                        /* Assign the DTC Origin as Relevant memory */
                        dataDemDTCOrigin_u16 = DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY;
                    }
                    else
                    {
                        /* Assign the DTC Origin as Primary memory */
                        dataDemDTCOrigin_u16 = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
                    }

                    /* calculation of DTC Origin and DTC kind, update DTC mask */
                    Dcm_Dsp_CalDTCOrigin(&dataDemDTCOrigin_u16,&stDTCStatus_u8,dataSubfunc_u8,pMsgContext->reqData[2]);

                    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3721]*/
                    /* Set the DTC Filter in DEM */
                    dataRetSetDTCFilter_u8 = Dem_SetDTCFilter(ClientId_u8,
                                                              stDTCStatus_u8,
                                                              DEM_DTC_FORMAT_UDS,
                                                              dataDemDTCOrigin_u16,
                                                              FALSE,
                                                              DEM_SEVERITY_NO_SEVERITY,
                                                              FALSE
                                                             );
                    /* If the Filter Mask sent is accepted by DEM */
                    if(E_OK == dataRetSetDTCFilter_u8)
                    {
                        /* Move the state to calculate the number of fltered dtc */
                        Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFCALCNUMDTC;
                    }
                    /* Filter Mask not accepted by DEM */
                    else
                    {
                        /* Set Negative response */
                        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                        dataretVal_u8=E_NOT_OK;
                    }
                }
                else
                {
                    /* Do nothing. Response buffer already filled with subfunction and status availability mask */
                    dataretVal_u8=E_OK;
                }
                /* Fill the other response bytes for subfunction 0x02,0x13,0x0F,0x15,0x17 or 0x0A */
                Dcm_Dsp_FillResBytesAndLength(pMsgContext,dataSubfunc_u8, dataStatusAvailMask_u8);
            }
            /* Status mask is not read properly from DEM */
            else
            {
                /* Set Negative response */
                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                dataretVal_u8=E_NOT_OK;
            }
        }
        /* Request Length not correct */
        else
        {
            /* Set Negative response */
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            dataretVal_u8=E_NOT_OK;
        }
    }


    /* State to calculate the number of filtered DTC */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFCALCNUMDTC)
    {
        dataSubfunc_u8 = pMsgContext->reqData[0];
        /* Copy the maximum response length */
        nrResMaxDataLen_u32 = pMsgContext->resMaxDataLen;


       /* Call the DEM API to get filtered number of DTC */
       dataRetNumFltDTC_u8 = Dem_GetNumberOfFilteredDTC(ClientId_u8,&nrFltDTC_u16);

       /* If filtered DTC number is obtained without error */
       if(E_OK == dataRetNumFltDTC_u8)
       {
           /* Check if number of filtered DTCs is nonzero */
           if(nrFltDTC_u16 != 0u)
           {
               nrRespLen_u32 = Dcm_Dsp_CalTotalResLengthForDTC(nrFltDTC_u16,dataSubfunc_u8);

               if(nrRespLen_u32 <= nrResMaxDataLen_u32)
               {
                   /* Move the state to fill the response */
                   Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;
               }
               else
               {
                   /* Response buffer exceeded - set negative response code */
                   *dataNegRespCode_u8     = DCM_E_RESPONSETOOLONG;
                   dataretVal_u8 = E_NOT_OK;
               }
           }
           else
           {
               /* Move the state to default state */
               Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
               dataretVal_u8 = E_OK;
           }
       }
       /* If more time is needed for getting the number of filtered DTC */
       else if(DEM_PENDING == dataRetNumFltDTC_u8)
       {
           /* Do nothing - remain in the same state and call the API again */
           dataretVal_u8 = DCM_E_PENDING;
       }
       else
       {
           /* Set negative response */
           *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
           dataretVal_u8 = E_NOT_OK;
       }
    }


    /* State to get the filtered DTC and status from DEM */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
    {
        idxLoop_qu16 = 0x00;
        /* Copy the maximum response length */
        nrResMaxDataLen_u32 = pMsgContext->resMaxDataLen;

        /* Copy the response length */
        nrRespLen_u32 = pMsgContext->resDataLen;


        /* Loop for maximum number of DTC's to read in a cycle */
        do
        {
            idxLoop_qu16++;

            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3721]*/
            /* Get the DTC filtered and its status from DEM */
            dataRetGetNextFiltDTC_u8 = Dem_GetNextFilteredDTC(ClientId_u8,&dataDTC_u32, &stDTCStatus_u8);

            /* If the DTC was filtered without any error */
            if(E_OK == dataRetGetNextFiltDTC_u8)
            {
                /* Check if the response buffer is available for filling the response */
                if((nrRespLen_u32 + 0x04u) <= nrResMaxDataLen_u32)
                {
                    /*TRACE[SWS_Dcm_00828]*/
                   (void)Dcm_Dsp_FillResDtcBuffer(dataDTC_u32, &nrRespLen_u32, stDTCStatus_u8,pMsgContext);
                }
                else
                {
                    /* Response buffer exceeded - set negative response code */
                    *dataNegRespCode_u8     = DCM_E_RESPONSETOOLONG;
                    dataretVal_u8=E_NOT_OK;
                }
            }
            /* If more time is needed to get the filtered DTC */
            else if(DEM_PENDING == dataRetGetNextFiltDTC_u8)
            {
                /* Nothing to do - Remain in the same state */
                dataretVal_u8 = DCM_E_PENDING;
                break;
            }
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3772]*/
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3773]*/
            /* If all the DTC's are read completely */
            else if(DEM_NO_SUCH_ELEMENT == dataRetGetNextFiltDTC_u8)
            {

                /*Reset State to DSP_RDTC_SFINIT*/
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
                idxLoop_qu16           = DCM_CFG_RDTC_MAXNUMDTCREAD;
                dataretVal_u8 = E_OK;
            }
            else
            {
                /* Some problems has occurred in DEM while reading DTC */
                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                dataretVal_u8=E_NOT_OK;
            }
        }while((idxLoop_qu16 < DCM_CFG_RDTC_MAXNUMDTCREAD) && (*dataNegRespCode_u8==0));


        /* Update the message table with response data length */
        pMsgContext->resDataLen = nrRespLen_u32;
    }

    return dataretVal_u8;
}
#endif


/**
 **************************************************************************************************
 * Dcm_Dsp_GetDTCRequestLength :
 *  This function is used to calcualte the request length for different sub function
 * \param           cosnt uint8     requested sub function
 *
 * \retval          uint8           length
 *
 **************************************************************************************************
 */
static uint8 Dcm_Dsp_GetDTCRequestLength(const uint8 reqSubfunction)
{
    uint8 reqLength_u8;

    /* Update the request length for sub functions 0xA and 0x15 */
    reqLength_u8 = DSP_RDTC_0A_15_REQLEN;

    /* Check if the subfunction is 0x02,0x13,0x0F or 0x17 */
#if((DCM_CFG_DSP_RDTCSUBFUNC_0x02_ENABLED != DCM_CFG_OFF)|| \
    /* FC_VariationPoint_START */                           \
    (DCM_CFG_DSP_RDTCSUBFUNC_0x13_ENABLED != DCM_CFG_OFF)|| \
    /* FC_VariationPoint_END */                             \
    (DCM_CFG_DSP_RDTCSUBFUNC_0x0F_ENABLED != DCM_CFG_OFF))

    if (    (reqSubfunction == DSP_REPORT_DTC_BY_STATUS_MASK)
            /* FC_VariationPoint_START */
         || (reqSubfunction == DSP_REPORT_OBD_DTC_BY_STATUS_MASK)
            /* FC_VariationPoint_END */
         || (reqSubfunction == DSP_REPORT_MIRROR_MEMORY_DTC)
       )
    {   /* Update the request length */
        reqLength_u8 = DSP_REPDTCBYSTMASK_REQLEN;
    }
#endif

#if (DCM_CFG_DSP_RDTCSUBFUNC_0x17_ENABLED != DCM_CFG_OFF)
    if (reqSubfunction == DSP_REPORT_USER_DEFMEMORY_DTC_BY_STATUS_MASK)
    {
        /* Update the request length */
        reqLength_u8 = DSP_RDTC_17_REQLEN;
    }
#endif

    return reqLength_u8;
}



/**
 ***********************************************************************************************************************
 * Dcm_Dsp_CalDTCActiveMask :
 *  This function is used to calcualte effective dtc mask for sub function 0x02, 0x13, 0x0F, 0x17
 * \param           const uint8     statusAvailMask_u8
 *                  const uint8     reqDtcStatus_u8
 *                  const uint8     reqSubfunction
 *
 * \retval          uint8     calculated Dtc Status mask
 *
 ***********************************************************************************************************************
 */
static uint8 Dcm_Dsp_CalDTCActiveMask(const uint8 statusAvailMask_u8,const uint8 reqDtcStatus_u8,const uint8 reqSubfunction)
{
    uint8 calDTCStatus_u8 = statusAvailMask_u8;

/* calculate the DTC status mask for the service 0x02,0x13,0x0F and 0x17 */
#if((DCM_CFG_DSP_RDTCSUBFUNC_0x02_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_RDTCSUBFUNC_0x0F_ENABLED != DCM_CFG_OFF) ||  \
    (DCM_CFG_DSP_RDTCSUBFUNC_0x17_ENABLED != DCM_CFG_OFF) || \
    /* FC_VariationPoint_START */                            \
    (DCM_CFG_DSP_RDTCSUBFUNC_0x13_ENABLED != DCM_CFG_OFF))
    /* FC_VariationPoint_END */

    /* Check if the subfunction is 0x02,0x13,0x0F or 0x17 */
    if((reqSubfunction == DSP_REPORT_DTC_BY_STATUS_MASK) || (reqSubfunction == DSP_REPORT_MIRROR_MEMORY_DTC) || \
       (reqSubfunction == DSP_REPORT_USER_DEFMEMORY_DTC_BY_STATUS_MASK) ||
          /* FC_VariationPoint_START */
       (reqSubfunction == DSP_REPORT_OBD_DTC_BY_STATUS_MASK))
          /* FC_VariationPoint_END */
    {
        /* Calculate the effective status mask */
        calDTCStatus_u8 = (statusAvailMask_u8 & reqDtcStatus_u8);
    }
#endif

    return calDTCStatus_u8;
}





/**
 ***********************************************************************************************************************
 * Dcm_Dsp_CalDTCKindAndOrigin :
 *  This function is used to calcualte dtc kind, dtc origin and dtc status for different sub function
 * \param           *Dem_DTCOriginType     dtc origin
 *                  *uint8                 dtc status
 *                  const uint8            requested sub function
 *                  const uint8            requested origin data
 *
 * \retval          none
 *
 ***********************************************************************************************************************
 */
/*MR12 DIR 8.13 VIOLATION:It is not Const since it is being used for sunfunction 13*/
static void Dcm_Dsp_CalDTCOrigin(Dem_DTCOriginType *dataDtcOrigin_u16,uint8 *dataDtcStatus_u8,const uint8 reqSubfunction, \
        const uint8 reqDtcOriginData)
{

#if (DCM_CFG_DSP_RDTCSUBFUNC_0x17_ENABLED != DCM_CFG_OFF)
    if(reqSubfunction == DSP_REPORT_USER_DEFMEMORY_DTC_BY_STATUS_MASK)
    {
        /* Set DTC origin as Memory selection from the request */
        *dataDtcOrigin_u16 = reqDtcOriginData + DCM_DTC_ORIGIN_MEMORY_SELECTION_VALUE;
    }
#else
    DCM_UNUSED_PARAM(reqDtcOriginData);
#endif

#if((DCM_CFG_DSP_RDTCSUBFUNC_0x0A_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_RDTCSUBFUNC_0x15_ENABLED != DCM_CFG_OFF))
    /* If the subfunction sent is 0x0A or 0x15 */
    if((reqSubfunction == DSP_REPORT_SUPPORTED_DTCS)||(reqSubfunction == DSP_REPORT_DTC_PERMANENT_STATUS))
    {
        /* Status Mask assigned 0x00 , to filter all DTCs */
        *dataDtcStatus_u8 = DSP_REPORT_ALL_DTC;
    }
    /* If the sunfunction sent is 0x15 */
    if(reqSubfunction == DSP_REPORT_DTC_PERMANENT_STATUS)
    {
        /* Assign the DTC Origin as Permanent memory */
        *dataDtcOrigin_u16 = DEM_DTC_ORIGIN_PERMANENT_MEMORY;
    }
#endif

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x0F_ENABLED != DCM_CFG_OFF)
    /* Check if the subfunction is 0x0F */
    if(reqSubfunction == DSP_REPORT_MIRROR_MEMORY_DTC)
    {
        /* Assign the DTC Origin as Mirror memory */
        *dataDtcOrigin_u16 = DEM_DTC_ORIGIN_MIRROR_MEMORY;
    }
#endif
}



/**
 ***********************************************************************************************************************
 * Dcm_Dsp_FillResBytesAndLength :
 *  This function is used to fill the response bytes and update the length of pMsgcontext
 * \param           *Dcm_MsgContextType         pMsgContext
 *                  const uint8                 requested sub function
 *                  const uint8                 available status mask
 *
 * \retval          none
 *
 ***********************************************************************************************************************
 */
/*TRACE[SWS_Dcm_00377]*/
static void Dcm_Dsp_FillResBytesAndLength(Dcm_MsgContextType *pMsgContext,const uint8 reqSubfunction, \
                                          const uint8 statusAvailMask_u8)
{
#if(DCM_CFG_DSP_RDTCSUBFUNC_0x17_ENABLED != DCM_CFG_OFF)
    if(reqSubfunction  == DSP_REPORT_USER_DEFMEMORY_DTC_BY_STATUS_MASK)
    {
        pMsgContext->resData[1] = pMsgContext->reqData[2];
        pMsgContext->resData[2] = statusAvailMask_u8;
        pMsgContext->resDataLen = 0x03;
    }
#endif

#if((DCM_CFG_DSP_RDTCSUBFUNC_0x02_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_RDTCSUBFUNC_0x0F_ENABLED != DCM_CFG_OFF) || \
    (DCM_CFG_DSP_RDTCSUBFUNC_0x0A_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_RDTCSUBFUNC_0x15_ENABLED != DCM_CFG_OFF) || \
    /* FC_VariationPoint_START */  \
    (DCM_CFG_DSP_RDTCSUBFUNC_0x13_ENABLED != DCM_CFG_OFF))
    /* FC_VariationPoint_END */


    /* Check if the subfunction is 0x02,0x13,0x0F,0x0A or 0x15 */
    if((reqSubfunction == DSP_REPORT_DTC_BY_STATUS_MASK) || (reqSubfunction == DSP_REPORT_MIRROR_MEMORY_DTC) || \
       (reqSubfunction == DSP_REPORT_SUPPORTED_DTCS)  || (reqSubfunction == DSP_REPORT_DTC_PERMANENT_STATUS) || \
        /* FC_VariationPoint_START */
       (reqSubfunction == DSP_REPORT_OBD_DTC_BY_STATUS_MASK))
        /* FC_VariationPoint_END */
    {
        pMsgContext->resData[1] = statusAvailMask_u8;
        pMsgContext->resDataLen = 0x02;
    }
#endif

    /* Fill the first response byte with subfunction No. */
    pMsgContext->resData[0] = reqSubfunction;
}



/**
 ***********************************************************************************************************************
 * Dcm_Dsp_FillResDtcBuffer :
 *  This function is used to fill response dtc buffer for paged buffer and none paged buffer
 * \param           uint32                      dtc Data
 *                  *Dcm_MsgLenType             response length
 *                  uint8                       dtc status mask
 *                  const * Dcm_MsgContextType  pMsgContext
 *
 * \retval          Std_ReturnType    next get dtc  return state
 *
 ***********************************************************************************************************************
*/
/*TRACE[SWS_Dcm_00377]*/
static Std_ReturnType Dcm_Dsp_FillResDtcBuffer(uint32 dtcData_u32,Dcm_MsgLenType *respLenNr_u32, \
                                                       uint8 stDTCStatus_u8,const Dcm_MsgContextType *pMsgContext)
{
    Std_ReturnType RetGetNextFiltDtc = E_OK;

#if((DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_RDTCPAGEDBUFFERSUPPORT != DCM_CFG_OFF))
    /* Copy the response length to local variable */
    *respLenNr_u32 = Dcm_DspRDTCFilledRespLen_u32;


    /* Fill the response buffer */
    Dcm_DspRDTCRespBufPtr_u8[*respLenNr_u32] = (uint8)(dtcData_u32 >> 16u);
    (*respLenNr_u32)++;
    Dcm_DspRDTCRespBufPtr_u8[*respLenNr_u32] = (uint8)(dtcData_u32 >> 8u);
    (*respLenNr_u32)++;
    Dcm_DspRDTCRespBufPtr_u8[*respLenNr_u32] = (uint8)(dtcData_u32);
    (*respLenNr_u32)++;
    Dcm_DspRDTCRespBufPtr_u8[*respLenNr_u32] = stDTCStatus_u8;
    (*respLenNr_u32)++;

    /* Copy back the response length to static variable */
    Dcm_DspRDTCFilledRespLen_u32 = *respLenNr_u32;

    /* Update the total response length filled with the bytes filled in the response in this iteration*/
    Dcm_DspTotalRespLenFilled_u32 = (Dcm_DspTotalRespLenFilled_u32 + 0x04u);

    /* Check if total response length filled is greater than actual response length calculated */
    if(Dcm_DspTotalRespLenFilled_u32 >= pMsgContext->resDataLen)
    {
        RetGetNextFiltDtc = DEM_NO_SUCH_ELEMENT;
    }
#else

        /* Fill the response buffer with DTC and status */
        pMsgContext->resData[*respLenNr_u32] = (uint8)(dtcData_u32 >> 16u);
        (*respLenNr_u32)++;
        pMsgContext->resData[*respLenNr_u32] = (uint8)(dtcData_u32 >> 8u);
        (*respLenNr_u32)++;
        pMsgContext->resData[*respLenNr_u32] = (uint8)(dtcData_u32);
        (*respLenNr_u32)++;
        pMsgContext->resData[*respLenNr_u32] = stDTCStatus_u8;
        (*respLenNr_u32)++;

#endif
    return RetGetNextFiltDtc;

}



#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif
#endif

