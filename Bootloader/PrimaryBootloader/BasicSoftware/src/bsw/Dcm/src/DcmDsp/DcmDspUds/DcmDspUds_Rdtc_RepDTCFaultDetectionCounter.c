
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"


#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"

#if((DCM_CFG_DSP_RDTCSUBFUNC_0x14_ENABLED != DCM_CFG_OFF))




#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if((DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_RDTCPAGEDBUFFERSUPPORT != DCM_CFG_OFF))
/**
 **************************************************************************************************
 * Dcm_Dsp_ReportFaultDetectionCounter :
 *  This function is used to implement the  subfunction 0x14(reportDTCFaultDetectionCounter).
 *  From this function  Dem_SetDTCFilter is called to set  the filter and then  Dem_GetNumberOfFilteredDTC
 *  is called to get the number of filtered DTCs. Dem_GetNextFilteredDTCAndFDC will be called to get the
 *  DTC and its corresponding Fault counter.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */




Std_ReturnType Dcm_Dsp_ReportFaultDetectionCounter (Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType *pMsgContext,\
                                                                    Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint8_least                      cntrLoop_qu8;             /* Variable for loop counter */
    sint8                            cntrFault_s8;             /* Variable to store Fault detection counter */
    Std_ReturnType                   dataRetSetDTCFilter_u8;   /* Return type for Dem_SetDTCFilter */
    Std_ReturnType                   dataRetGetNextFiltDTC_u8; /* Return type for Dem_GetNextFilteredDTC */
    Std_ReturnType                   dataRetNumFltDTC_u8;      /* Return type for Dem_GetNumberOfFilteredDTC */
    uint16                           nrFltDTC_u16;             /* Variable to store number of filtered DTCs */
    uint32                           dataDTC_u32;              /* Variable to store DTC */
    Dcm_MsgLenType                   dataRespLen_u32;          /* Local variable for response length */
    Dcm_MsgLenType                   dataResMaxLen_u32;        /* Variable to store max response length */
    Std_ReturnType                   dataServRetval_u8 = DCM_E_PENDING;        /* Variable to store service return value */
    boolean                          isProtocolIPCanFD_b = FALSE;
    static boolean                   RdtcDataPending_flag_b;       /* varible to indicate datapending */

    /* Initialization of local variables */
    *dataNegRespCode_u8      = 0x0;
    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);

    /* State machine for ReportFaultDetectionCounter */
    /* Initialization state */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
    {
        /* check for request length */
        if(pMsgContext->reqDataLen == DSP_RDTC_14_REQLEN)
        {
            /* Initializing the filled response length in page and the total filled response length */
            Dcm_DspRDTCFilledRespLen_u32 = 0x0;
            Dcm_DspTotalRespLenFilled_u32 = 0x0;
            /* Set the variable to FALSE to indicate that the zero's should not be filled in page */

            Dcm_DspFillZeroes_b     = FALSE;

            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3764]*/
            /* Set the DTC Filter in DEM */
            dataRetSetDTCFilter_u8 = Dem_SetDTCFilter(ClientId_u8,
                                                    0x00,
                                                    DEM_DTC_FORMAT_UDS,
                                                    DEM_DTC_ORIGIN_PRIMARY_MEMORY,
                                                    FALSE,
                                                    DEM_SEVERITY_NO_SEVERITY,
                                                    TRUE
                                                    );
            /* If the Filter Mask sent is accepted by DEM */
            if(E_OK == dataRetSetDTCFilter_u8)
            {
                /* Update the subfunction in the response buffer */
                pMsgContext->resData[0]=pMsgContext->reqData[DSP_RDTC_POSSUBFUNC];
                /* Update the response length */
                pMsgContext->resDataLen=1;

                /* Move the state to calculate the total number of DTC's */
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFCALCNUMDTC;
            }
            /* Filter Mask not accepted by DEM */
            else
            {
                *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetSetDTCFilter_u8);
                dataServRetval_u8=E_NOT_OK;
            }
        }
        /* Request Length not correct */
        else
        {
            /* Set Negative response */
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            /*Return E_NOT_OK from the service*/
            dataServRetval_u8=E_NOT_OK;
        }
    }

    /* State to calculate the number of filtered DTC */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFCALCNUMDTC)
    {
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3763]*/
        /* Call the DEM API to get filtered number of DTC */
        dataRetNumFltDTC_u8 = Dem_GetNumberOfFilteredDTC(ClientId_u8,&nrFltDTC_u16);

        /* If filtered DTC number is obtained without error */
        if(E_OK == dataRetNumFltDTC_u8)
        {
            /* Check if number of filtered DTC's is greater than 0 */
            if(nrFltDTC_u16 !=0)
            {
                /* Calculate total response length */
                pMsgContext->resDataLen = (Dcm_MsgLenType)(nrFltDTC_u16);
                pMsgContext->resDataLen <<= (Dcm_MsgLenType)0x02u;
                pMsgContext->resDataLen += (Dcm_MsgLenType)(0x01u);
                /*Check if the total response data length is valid*/
                if(Dcm_Prv_CheckTotalResponseLength(pMsgContext->resDataLen)!=FALSE)
                {
                    /* Set the variable to FALSE to indicate that the first page is not sent */
                    Dcm_DspFirstPageSent_b  = FALSE;

                    /* Move the state to next state */
                    Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;

                    Dcm_adrUpdatePage_pfct = &Dcm_Dsp_RDTCUpdatePage;

                    /* Call the DCm API and start Paged processing */
                    Dcm_StartPagedProcessing(pMsgContext);

                    /* Update the response length variables */
                    Dcm_DspRDTCFilledRespLen_u32 = 0x01;
                    Dcm_DspTotalRespLenFilled_u32 = 0x01;

                }
                else
                {
                    /* Response buffer exceeded - set negative response code */
                    *dataNegRespCode_u8     = DCM_E_RESPONSETOOLONG;

                    /*Return E_NOT_OK from the service*/
                    dataServRetval_u8=E_NOT_OK;
                }
            }
            else
            {
                /* Move the state to default state */
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
                dataServRetval_u8=E_OK;
            }
        }
        /* If more time is needed for getting the number of filtered DTC */
        else if(DEM_PENDING == dataRetNumFltDTC_u8)
        {
            /* Do nothing - remain in the same state and call the API again */
            /*Return E_NOT_OK from the service*/
            dataServRetval_u8=DCM_E_PENDING;
        }
        else
        {
            /* Set negative response */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;

            /*Return E_NOT_OK from the service*/
            dataServRetval_u8=E_NOT_OK;
        }
    }

    /* State to get the filtered DTC, Fault counter value from DEM */
    if( Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
    {
        cntrLoop_qu8 = 0;
        /* Copy the maximum response length */
        dataResMaxLen_u32 = pMsgContext->resMaxDataLen;

        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3762]*/
        do
        {
            cntrLoop_qu8++;
            /* Get the DTC filtered and its fault count from DEM */
            dataRetGetNextFiltDTC_u8 = Dem_GetNextFilteredDTCAndFDC(ClientId_u8,&dataDTC_u32, &cntrFault_s8);

            /* If the DTC was filtered without any error */
            if(E_OK == dataRetGetNextFiltDTC_u8)
            {
                if (Dcm_DsldPduInfo_st.SduLength != 0u)
                {
                    if (RdtcDataPending_flag_b == FALSE)
                    {
                        dataRespLen_u32 = 0 ;
                        Dcm_DspRDTCFilledRespLen_u32 = Dcm_DspRDTCFilledRespLen_u32 + Dcm_DsldPduInfo_st.SduLength;
                        RdtcDataPending_flag_b = TRUE;
                    }
                }

                /* Copy the response length to local variable */
                dataRespLen_u32 = Dcm_DspRDTCFilledRespLen_u32;

                /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3761]*/
                /* Fill the response buffer with DTC and status */
                Dcm_DspRDTCRespBufPtr_u8[dataRespLen_u32] = (uint8)(dataDTC_u32 >> 16u);
                dataRespLen_u32++;
                Dcm_DspRDTCRespBufPtr_u8[dataRespLen_u32] = (uint8)(dataDTC_u32 >> 8u);
                dataRespLen_u32++;
                Dcm_DspRDTCRespBufPtr_u8[dataRespLen_u32] = (uint8)(dataDTC_u32);
                dataRespLen_u32++;
                Dcm_DspRDTCRespBufPtr_u8[dataRespLen_u32] = (uint8)(cntrFault_s8);
                dataRespLen_u32++;

                /* Copy back the response length to static variable */
                Dcm_DspRDTCFilledRespLen_u32 = dataRespLen_u32;

                /* Update the total response length filled with the bytes filled in the response in this iteration*/
                Dcm_DspTotalRespLenFilled_u32 = (Dcm_DspTotalRespLenFilled_u32 + 0x04u);
                /* Check if total response length filled is greater than actual response length calculated */
                if(Dcm_DspTotalRespLenFilled_u32 >=  pMsgContext->resDataLen)
                {
                    dataRetGetNextFiltDTC_u8 = DEM_NO_SUCH_ELEMENT;
                }

            }
            /* If more time is needed to get the filtered DTC */
            else if(DEM_PENDING == dataRetGetNextFiltDTC_u8 )
            {
                /*Return E_NOT_OK from the service*/
                dataServRetval_u8=DCM_E_PENDING;
                break;
            }
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3765],
             *TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3766]*/
            /* If all the DTC's are read completely */
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
                    /*Return E_NOT_OK from the service*/
                    dataServRetval_u8=E_NOT_OK;

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
            /* Check if the buffer overflow happens or all filtered DTC's are read */

            isProtocolIPCanFD_b = Dcm_IsProtocolIPCanFD();
            if((dataRetGetNextFiltDTC_u8 == DEM_NO_SUCH_ELEMENT) || (((Dcm_DspRDTCFilledRespLen_u32 + 0x04u) > dataResMaxLen_u32) && (isProtocolIPCanFD_b!=FALSE))||  ((Dcm_DspRDTCFilledRespLen_u32>=7u)&&(isProtocolIPCanFD_b==FALSE)))
            {

                /* If first page is not sent yet */

                if(Dcm_DspFirstPageSent_b == FALSE)
                {
                    /* Set the variable to TRUE indicating first page is sent */

                    Dcm_DspFirstPageSent_b = TRUE;
                }
                if(dataRetGetNextFiltDTC_u8 == DEM_NO_SUCH_ELEMENT)
                {
                    if(pMsgContext->resDataLen > Dcm_DspTotalRespLenFilled_u32)
                    {
                      /* Calculating the remaining number of bytes to be filled in response */
                      dataRespLen_u32 = pMsgContext->resDataLen - Dcm_DspTotalRespLenFilled_u32;
                      Dcm_Dsp_RDTCFillZero(dataRespLen_u32);
                    }
                }

                /* Move the state to next state */
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFTXPAGE;

                /* Send the page for transmission */
                Dcm_ProcessPage(Dcm_DspRDTCFilledRespLen_u32);
                RdtcDataPending_flag_b = FALSE;
                /* Reset the response length */
                Dcm_DspRDTCFilledRespLen_u32 = 0x0;

                /* Change the return so that the loop won't get executed again */
                dataRetGetNextFiltDTC_u8 = DEM_NO_SUCH_ELEMENT;
            }
        }while((cntrLoop_qu8 < DCM_CFG_RDTC_MAXNUMDTCREAD) && (dataRetGetNextFiltDTC_u8 == E_OK));
    }

    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFTXPAGE)
    {
        /* Wait in this state for transmission of page */
    }
    /* State where zero's are filled in page if total response length filled is less than actual response length */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLZERO)
    {
        if((Dcm_DsldPduInfo_st.SduLength != 0u )&& (RdtcDataPending_flag_b == FALSE))
        {
            dataRespLen_u32 = 0 ;
            Dcm_DspRDTCFilledRespLen_u32 = Dcm_DspRDTCFilledRespLen_u32 + Dcm_DsldPduInfo_st.SduLength;
            RdtcDataPending_flag_b = TRUE;
        }
        /* Calculating the remaining number of bytes to be filled in response */
        if(pMsgContext->resDataLen > Dcm_DspTotalRespLenFilled_u32)
        {
            dataRespLen_u32 = pMsgContext->resDataLen - Dcm_DspTotalRespLenFilled_u32;

            Dcm_Dsp_RDTCFillZero(dataRespLen_u32);
        }
        /* Move the state to next state */
        Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFTXPAGE;

        /* Send the page for transmission */
        Dcm_ProcessPage(Dcm_DspRDTCFilledRespLen_u32);
        RdtcDataPending_flag_b = FALSE;
        Dcm_DspRDTCFilledRespLen_u32 = 0;
    }

    return dataServRetval_u8;
}



#else/*#if((DCM_PAGEDBUFFER_ENABLED!=DCM_CFG_OFF) && (DCM_CFG_RDTCPAGEDBUFFERSUPPORT!=DCM_CFG_OFF))*/


Std_ReturnType Dcm_Dsp_ReportFaultDetectionCounter (Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType *pMsgContext,\
                                                                    Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint8_least                       cntrLoop_qu8;             /* Variable for loop counter */
    sint8                             cntrFault_s8;             /* Variable to store fault detection counter */
    Std_ReturnType                    dataRetSetDTCFilter_u8;   /* Return type for Dem_SetDTCFilter */
    Std_ReturnType                    dataRetGetNextFiltDTC_u8; /* Return type for Dem_GetNextFilteredDTC */
    Std_ReturnType                    dataRetNumFltDTC_u8;      /* Return type for Dem_GetNumberOfFilteredDTC */
    uint16                            nrFltDTC_u16;             /* Variable to store number of filtered DTCs */
    uint32                            dataDTC_u32;              /* Variable to store DTC */
    Dcm_MsgLenType                    dataRespLen_u32;          /* Local variable for response length */
    Dcm_MsgLenType                    dataResMaxLen_u32;        /* Variable to store max response length */
    Std_ReturnType                    dataServRetval_u8 = DCM_E_PENDING;        /* Initialization of local variables */

    (void)(OpStatus);
    *dataNegRespCode_u8 = 0x0;

    /* State machine for ReportFaultDetectionCounter */
    /* Initialization state */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
    {
        /* check for request length */
        if(pMsgContext->reqDataLen == DSP_RDTC_14_REQLEN)
        {
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3764]*/
            /* Set the DTC Filter in DEM */
            dataRetSetDTCFilter_u8 = Dem_SetDTCFilter(ClientId_u8,
                                                    0x00,
                                                    DEM_DTC_FORMAT_UDS,
                                                    DEM_DTC_ORIGIN_PRIMARY_MEMORY,
                                                    FALSE,
                                                    DEM_SEVERITY_NO_SEVERITY,
                                                    TRUE
                                                    );
            /* If the Filter Mask sent is accepted by DEM */
            if(E_OK == dataRetSetDTCFilter_u8 )
            {
                /* Update the subfunction in the response buffer */
                pMsgContext->resData[0]=pMsgContext->reqData[DSP_RDTC_POSSUBFUNC];
                /* Update the response length */
                pMsgContext->resDataLen=1;

                /* Move the state to get the number of filtered DTCs */
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFCALCNUMDTC;
            }
            /* Filter Mask not accepted by DEM */
            else
            {
                *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetSetDTCFilter_u8);
                dataServRetval_u8=E_NOT_OK;
            }
        }
        /* Request Length not correct */
        else
        {
            /* Set Negative response */
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            dataServRetval_u8=E_NOT_OK;
        }
    }


    /* State to calculate the number of filtered DTC */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFCALCNUMDTC)
    {
        /* Copy the maximum response length */
        dataResMaxLen_u32 = pMsgContext->resMaxDataLen;

        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3763]*/
        /* Call the DEM API to get filtered number of DTC */
        dataRetNumFltDTC_u8 = Dem_GetNumberOfFilteredDTC(ClientId_u8,&nrFltDTC_u16);

        /* If filtered DTC number is obtained without error */
        if(E_OK == dataRetNumFltDTC_u8)
        {
            /* Check if number of filtered DTC's is greater than 0 */
            if(nrFltDTC_u16 !=0)
            {
                /* Calculate total response length */
                dataRespLen_u32 = ((((Dcm_MsgLenType)nrFltDTC_u16) << 0x02u) + 0x01u);

                /*Check if the total response data length is valid*/
                if(dataRespLen_u32 <= dataResMaxLen_u32)
                {
                    /* Move the state to fill the response */
                    Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;
                }
                else
                {
                    /* Response buffer exceeded - set negative response code */
                    *dataNegRespCode_u8     = DCM_E_RESPONSETOOLONG;

                    /*Return E_NOT_OK from the service*/
                    dataServRetval_u8=E_NOT_OK;
                }
            }
            else
            {
                /* Move the state to default state */
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
                dataServRetval_u8=E_OK;
            }
        }
        /* If more time is needed for getting the number of filtered DTC */
        else if(DEM_PENDING == dataRetNumFltDTC_u8)
        {
            /* Do nothing - remain in the same state and call the API again */
            /*Return E_NOT_OK from the service*/
            dataServRetval_u8=DCM_E_PENDING;
        }
        else
        {
            /* Set negative response */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;

            /*Return E_NOT_OK from the service*/
            dataServRetval_u8=E_NOT_OK;
        }
    }



    /* State to get the filtered DTC and status from DEM */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
    {
        cntrLoop_qu8 = 0x00;
        /* Copy the maximum response length */
        dataResMaxLen_u32 = pMsgContext->resMaxDataLen;

        /* Copy the response length */
        dataRespLen_u32 = pMsgContext->resDataLen;

        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3762]*/
        /* Loop for maximum number of DTC's to read in a cycle */
        do
        {
            cntrLoop_qu8++;
            /* Get the DTC filtered and its fault count from DEM */
            dataRetGetNextFiltDTC_u8 = Dem_GetNextFilteredDTCAndFDC(ClientId_u8,&dataDTC_u32, &cntrFault_s8);

            /* If the DTC was filtered without any error */
            if(E_OK == dataRetGetNextFiltDTC_u8)
            {
                /* Check if the response buffer is available for filling the response */
                if((dataRespLen_u32 + 0x04u) <= dataResMaxLen_u32)
                {
                    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3761]*/
                    /* Fill the response buffer with DTC and status */
                    pMsgContext->resData[dataRespLen_u32] = (uint8)(dataDTC_u32 >> 16u);
                    dataRespLen_u32++;
                    pMsgContext->resData[dataRespLen_u32] = (uint8)(dataDTC_u32 >> 8u);
                    dataRespLen_u32++;
                    pMsgContext->resData[dataRespLen_u32] = (uint8)(dataDTC_u32);
                    dataRespLen_u32++;
                    pMsgContext->resData[dataRespLen_u32] = (uint8)(cntrFault_s8);
                    dataRespLen_u32++;
                }
                else
                {
                    /* Response buffer exceeded - set negative response code */
                    *dataNegRespCode_u8     = DCM_E_RESPONSETOOLONG;
                    dataServRetval_u8=E_NOT_OK;
                }
            }
            /* If more time is needed to get the filtered DTC */
            else if(DEM_PENDING == dataRetGetNextFiltDTC_u8)
            {
                /* Nothing to do - Remain in the same state */
                dataServRetval_u8=DCM_E_PENDING;
                break;
            }
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3765],
             *TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3766]*/
            /* If all the DTC's are read completely */
            else if(DEM_NO_SUCH_ELEMENT == dataRetGetNextFiltDTC_u8)
            {

            /*Reset State to DSP_RDTC_SFINIT*/
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
                cntrLoop_qu8          = DCM_CFG_RDTC_MAXNUMDTCREAD;
                dataServRetval_u8=E_OK;
            }
            else
            {
            /* Some problems has occured in DEM while reading DTC */
                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                dataServRetval_u8=E_NOT_OK;
            }
        }while((cntrLoop_qu8 < DCM_CFG_RDTC_MAXNUMDTCREAD) && (*dataNegRespCode_u8==0));


        /* Update the message table with response data length */
        pMsgContext->resDataLen = dataRespLen_u32;
    }


    return dataServRetval_u8;
}


#endif/*#if((DCM_PAGEDBUFFER_ENABLED!=DCM_CFG_OFF)&& (DCM_CFG_RDTCPAGEDBUFFERSUPPORT!=DCM_CFG_OFF))*/



#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif

