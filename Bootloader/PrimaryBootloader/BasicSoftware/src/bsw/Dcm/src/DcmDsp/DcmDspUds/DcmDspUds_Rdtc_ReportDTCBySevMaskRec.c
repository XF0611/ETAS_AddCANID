

#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x08_ENABLED != DCM_CFG_OFF)

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
#define DCM_SEVERITYMASK               (0x1Fu) //Only bit 7 to 5 of the severity mask byte are valid



#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
 /**
 **************************************************************************************************
 * Dcm_Dsp_ReportDTCBySeverityMaskRecord :
 *  This function is used to read a list of DTC severity and functional unit information,
 *  which satisfies a client-defined severity mask record
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */

#if((DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_RDTCPAGEDBUFFERSUPPORT != DCM_CFG_OFF))

Std_ReturnType Dcm_Dsp_ReportDTCBySeverityMaskRecord(Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType *pMsgContext,\
                                                                       Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint32                   dataDTC_u32;               /* Variable to store DTC */
    Dcm_MsgLenType           nrResDataLen_u32;          /* Local variable for response length */
    Dcm_MsgLenType           nrResMaxDataLen_u32;       /* Variable to store max response length */
    uint16                   nrFltDTC_u16;              /* Variable to store number of filtered DTCs */
    uint8_least              idxLoop_qu8;               /* Variable for loop counter */
    uint8                    dataDTCFunctionalUnit_u8;  /* Variable to store DTC functional unit */
    uint8                    dataStatusAvailMask_u8;
    uint8                    stDTCStatus_u8;            /* Variable for storing effective status mask */
    Std_ReturnType           dataRetSetDTCFilter_u8;    /* Return value for Dem_SetDTCFilter */
    Std_ReturnType           dataRetNumFltDTC_u8;
    Std_ReturnType           dataretGetNextFiltDTC_u8;  /* Return from Dem_GetNextFilteredDTC */
    Dem_DTCSeverityType      dataDTCSeverity_u8;        /* Variable to store DTC Severity Mask */
    Std_ReturnType           dataretVal_u8 = DCM_E_PENDING ;
    static boolean           dataPending_flag_b;        /* varible to indicate datapending */

    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);
    /* Initialization of local variables */
    *dataNegRespCode_u8     = 0x00;

    /* State machine for Report DTC By Severity Mask */
    /* Initialization state */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
    {
        /* check for request length */
        if(pMsgContext->reqDataLen == DSP_REPDTCBYSEVMASK_REQLEN )
        {
            /* Store the severity mask to local variable */
            dataDTCSeverity_u8 = pMsgContext->reqData[1] ;

            /*Check for any invalid Severity Bits set*/
            if ((dataDTCSeverity_u8 & DCM_SEVERITYMASK) == 0x00u)
            {
                /* If the available status mask is read successfully */
                if(E_OK == Dem_GetDTCStatusAvailabilityMask(ClientId_u8,&dataStatusAvailMask_u8))
                {
                    /* Initializing the filled response length in page and the total filled response length */
                    Dcm_DspRDTCFilledRespLen_u32 = 0x0;
                    Dcm_DspTotalRespLenFilled_u32 = 0x0;

                    /* Set the variable to FALSE to indicate that the zero's should not be filled in page */

                    Dcm_DspFillZeroes_b     = FALSE;

                    /* Copy the status mask to the local variable */
                    stDTCStatus_u8 = pMsgContext->reqData[2];

                    /*TRACE[SWS_Dcm_00379]*/
                    /* Fill the response bytes */
                    pMsgContext->resData[0] = DSP_REPORT_DTC_BY_SEVERITY_MASK;
                    pMsgContext->resData[1] = dataStatusAvailMask_u8;
                    pMsgContext->resDataLen = 0x02;

                    /*Get the effective status mask*/
                    stDTCStatus_u8 = (stDTCStatus_u8 & dataStatusAvailMask_u8);

                    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3779]*/
                    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3781]*/
                    if((stDTCStatus_u8 != 0x00u) && (dataDTCSeverity_u8 != 0x00u))
                    {
                        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3723]*/
                        /*Call API to set the DTC Filter */
                        dataRetSetDTCFilter_u8 = Dem_SetDTCFilter(ClientId_u8,
                                                                 stDTCStatus_u8,
                                                                 DEM_DTC_FORMAT_UDS,
                                                                 DEM_DTC_ORIGIN_PRIMARY_MEMORY,
                                                                 TRUE,
                                                                 dataDTCSeverity_u8,
                                                                 FALSE
                                                             );
                        if (E_OK == dataRetSetDTCFilter_u8)
                        {
                            /* Move the state to fill the response */
                            Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFCALCNUMDTC;
                        }
                        else
                        {
                            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetSetDTCFilter_u8);
                            dataretVal_u8 = E_NOT_OK;
                        }
                    }
                    else
                    {
                        /* Do Nothing */
                        dataretVal_u8 = E_OK;
                    }

                }
                /* Status mask is not read properly from DEM */
                else
                {
                    /* Set Negative response */
                    *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                    dataretVal_u8 = E_NOT_OK;
                }
            }
            else
            {
                /*Set Negative Response*/
                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
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
        /* Call the DEM API to get filtered number of DTC */
        dataRetNumFltDTC_u8 = Dem_GetNumberOfFilteredDTC(ClientId_u8,&nrFltDTC_u16);

        /* If filtered DTC number is obtained without error */
        if(E_OK == dataRetNumFltDTC_u8)
        {
            if(nrFltDTC_u16 !=0)
            {
                /* Calculate total response length */
                pMsgContext->resDataLen = (Dcm_MsgLenType)((Dcm_MsgLenType)(nrFltDTC_u16 * 0x06u) + (Dcm_MsgLenType)0x02);
                /*Check if  Positive response data length is set to TRUE and max dat length is greater than 7*/
                if((Dcm_Prv_CheckTotalResponseLength(pMsgContext->resDataLen)!=FALSE) && (pMsgContext->resMaxDataLen > 7u))
                {
                    /* Set the variable to FALSE to indicate that the first page is not sent */
                    Dcm_DspFirstPageSent_b  = FALSE;

                    /* Move the state to next state */
                    Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;

                    Dcm_adrUpdatePage_pfct = &Dcm_Dsp_RDTCUpdatePage;

                    /* Call the DCm API and start Paged processing */
                    Dcm_StartPagedProcessing(pMsgContext);

                    /* Update the response length variables */
                    Dcm_DspRDTCFilledRespLen_u32 = 0x02;
                    Dcm_DspTotalRespLenFilled_u32 = 0x02;

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
        }
        else
        {
            /* Set negative response */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
            dataretVal_u8 = E_NOT_OK;
        }
    }

    /*TRACE[SWS_Dcm_00379]*/
    /* State to get the filtered DTC, severity, status and functional unit information from DEM */
    if( Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
    {
        idxLoop_qu8 = 0;

        /* Store the maximum response length in local variable */
        nrResMaxDataLen_u32 = Dcm_DspRDTCMaxRespLen_u32;

        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3723]*/
        /* Loop for maximum number of DTC's to read in a cycle */
        do
        {
            idxLoop_qu8++;

            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3723]*/
            /* Get the DTC, severity, status and functional unit from DEM */
            dataretGetNextFiltDTC_u8 = Dem_GetNextFilteredDTCAndSeverity(ClientId_u8,
                                                                     &dataDTC_u32,
                                                                     &stDTCStatus_u8,
                                                                     &dataDTCSeverity_u8,
                                                                     &dataDTCFunctionalUnit_u8);
            /* If the DTC was filtered without any error */
            if(E_OK == dataretGetNextFiltDTC_u8)
            {
                if (Dcm_DsldPduInfo_st.SduLength != 0u)
                {
                    if (dataPending_flag_b == FALSE)
                    {
                        nrResDataLen_u32 = 0 ;
                        Dcm_DspRDTCFilledRespLen_u32 = Dcm_DspRDTCFilledRespLen_u32 + Dcm_DsldPduInfo_st.SduLength;
                        dataPending_flag_b = TRUE;
                    }
                }

                /* Copy the response length to local variable */
                 nrResDataLen_u32 = Dcm_DspRDTCFilledRespLen_u32;

                 /*TRACE[SWS_Dcm_00379]*/
                /* Fill the response buffer with DTC and status */
                Dcm_DspRDTCRespBufPtr_u8[nrResDataLen_u32] = dataDTCSeverity_u8;
                nrResDataLen_u32++;
                Dcm_DspRDTCRespBufPtr_u8[nrResDataLen_u32] = dataDTCFunctionalUnit_u8;
                nrResDataLen_u32++;
                Dcm_DspRDTCRespBufPtr_u8[nrResDataLen_u32] = (uint8)(dataDTC_u32>>16u);
                nrResDataLen_u32++;
                Dcm_DspRDTCRespBufPtr_u8[nrResDataLen_u32] = (uint8)(dataDTC_u32>>8u);
                nrResDataLen_u32++;
                Dcm_DspRDTCRespBufPtr_u8[nrResDataLen_u32] = (uint8)(dataDTC_u32);
                nrResDataLen_u32++;
                Dcm_DspRDTCRespBufPtr_u8[nrResDataLen_u32] = stDTCStatus_u8;
                nrResDataLen_u32++;

                /* Copy back the response length to static variable */
                Dcm_DspRDTCFilledRespLen_u32 = nrResDataLen_u32;

                /* Update the total response length filled with the bytes filled in the response in this iteration*/
                Dcm_DspTotalRespLenFilled_u32 = Dcm_DspTotalRespLenFilled_u32 + (uint32)0x06;

                /* Check if total response length filled is greater than actual response length calculated */
                if(Dcm_DspTotalRespLenFilled_u32 >=  pMsgContext->resDataLen)
                {
                    dataretGetNextFiltDTC_u8 = DEM_NO_SUCH_ELEMENT;
                }
            }
            /* If more time is needed to get the filtered DTC */
            else if(DEM_PENDING == dataretGetNextFiltDTC_u8)
            {
                dataretVal_u8 = DCM_E_PENDING;
                break;
            }
            /* If all the DTC's are read completely */
            else if(DEM_NO_SUCH_ELEMENT == dataretGetNextFiltDTC_u8)
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

            /* Check if the buffer overflow happens or all filtered DTC's are read */
            /*If Return from Dem_GetNextFilteredDTC is equal to 0x01 and Dcm_IsProtocolIPCanFD is set to TRUE and Dcm_DspRDTC FilledRespLen is greater than or equal to 7*/

            /* MR12 RULE 13.5 VIOLATION: The right hand operand of '&&' or '||' has side effects - The statements inside needs to be executed only if all the conditions are satisfied */
            if((dataretGetNextFiltDTC_u8 == DEM_NO_SUCH_ELEMENT) || (((Dcm_DspRDTCFilledRespLen_u32 + 0x06u) > nrResMaxDataLen_u32) && (Dcm_IsProtocolIPCanFD()!=FALSE))|| ((Dcm_DspRDTCFilledRespLen_u32>=7u)&&(Dcm_IsProtocolIPCanFD()==FALSE)))
            {
                /* If first page is not sent yet */

                if(Dcm_DspFirstPageSent_b == FALSE)
                {
                    /* Set the variable to TRUE indicating first page is sent */

                    Dcm_DspFirstPageSent_b = TRUE;
                }


                if(dataretGetNextFiltDTC_u8 == DEM_NO_SUCH_ELEMENT)
                {
                    if((Dcm_DsldPduInfo_st.SduLength != 0u)&& (dataPending_flag_b == FALSE))
                    {
                        nrResDataLen_u32 = 0 ;
                        Dcm_DspRDTCFilledRespLen_u32 = Dcm_DspRDTCFilledRespLen_u32 + Dcm_DsldPduInfo_st.SduLength;
                        dataPending_flag_b = TRUE;
                    }
                    if(pMsgContext->resDataLen > Dcm_DspTotalRespLenFilled_u32)
                    {
                        /* Calculating the remaining number of bytes to be filled in response */
                        nrResDataLen_u32 = pMsgContext->resDataLen - Dcm_DspTotalRespLenFilled_u32;
                        Dcm_Dsp_RDTCFillZero(nrResDataLen_u32);
                    }
                }
                if(
                        ((Dcm_DspRDTCFilledRespLen_u32 + 0x6u) >= Dcm_DslTransmit_st.TxResponseLength_u32)
                        ||
                        ((Dcm_DspRDTCFilledRespLen_u32 + 0x6u )>=   nrResMaxDataLen_u32)
                        ||
                        ((Dcm_DspTotalRespLenFilled_u32 + 1u)>= Dcm_DslTransmit_st.TxResponseLength_u32)
                        )
                {
                    /* Move the state to next state */
                    Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFTXPAGE;
                    /* Send the page for transmission */
                    Dcm_ProcessPage(Dcm_DspRDTCFilledRespLen_u32);
                    dataPending_flag_b = FALSE;
                    /* Reset the response length */
                    Dcm_DspRDTCFilledRespLen_u32 = 0x0;
                    /* Change the return so that the loop won't get executed again */
                    dataretGetNextFiltDTC_u8 = DEM_NO_SUCH_ELEMENT;
                }
            }

        }while((idxLoop_qu8 < DCM_CFG_RDTC_MAXNUMDTCREAD) && (dataretGetNextFiltDTC_u8 == E_OK));
    }


    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFTXPAGE)
    {
        /* Wait in this state for transmission of page */
    }

    /* State where zero's are filled in page if total response length filled is less than actual response length */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLZERO)
    {
        if((Dcm_DsldPduInfo_st.SduLength != 0u )&& (dataPending_flag_b == FALSE))
        {
            nrResDataLen_u32 = 0 ;
            Dcm_DspRDTCFilledRespLen_u32 = Dcm_DspRDTCFilledRespLen_u32 + Dcm_DsldPduInfo_st.SduLength;
            dataPending_flag_b = TRUE;
        }
        if(pMsgContext->resDataLen > Dcm_DspTotalRespLenFilled_u32)
        {
            /* Calculating the number of remaining number of bytes to be filled in response */
            nrResDataLen_u32 = pMsgContext->resDataLen - Dcm_DspTotalRespLenFilled_u32;
            Dcm_Dsp_RDTCFillZero(nrResDataLen_u32);
        }
        /* Move the state to next state */
        Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFTXPAGE;
        /* Send the page for transmission */

        Dcm_ProcessPage(Dcm_DspRDTCFilledRespLen_u32);
        dataPending_flag_b =FALSE;

        Dcm_DspRDTCFilledRespLen_u32 = 0;
    }
        return dataretVal_u8;
}

#else

Std_ReturnType Dcm_Dsp_ReportDTCBySeverityMaskRecord (Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType *pMsgContext,\
        Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint32                          dataDTC_u32;               /* Variable to store DTC */
    Dcm_MsgLenType                  nrResDataLen_u32;          /* Local variable for response length */
    Dcm_MsgLenType                  nrResMaxDataLen_u32;       /* Variable to store max response length */
    uint8_least                     idxLoop_qu8;               /* Variable for loop counter */
    uint8                           dataDTCFunctionalUnit_u8;  /* Variable to store DTC functional unit */
    uint8                           dataStatusAvailMask_u8;    /* Variable to fill status availability mask*/
    Std_ReturnType                  dataRetSetDTCFilter_u8;    /* Return value for Dem_SetDTCFilter */
    Std_ReturnType                  dataretGetNextFiltDTC_u8;  /* Return from Dem_GetNextFilteredDTC */
    uint8                           stDTCStatus_u8;            /* Variable for available status mask */
    Dem_DTCSeverityType             dataDTCSeverity_u8;        /* Variable to store DTC Severity Mask */
    Std_ReturnType                  dataretVal_u8 = DCM_E_PENDING;             /* Variable to store the service return value */
    Std_ReturnType                  dataRetNumFltDTC_u8;        /* Return from Dem_GetNumberOfFilteredDTC */
    uint16                   nrFltDTC_u16;                      /* Variable to store number of filtered DTCs */

    (void)OpStatus;
    /* Initialization of local variables */
    *dataNegRespCode_u8     = 0x00;

    /* State machine for Report DTC By Severity Mask  */
    /* Initialization state */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
    {
        /* check for request length */
        if(pMsgContext->reqDataLen == DSP_REPDTCBYSEVMASK_REQLEN )
        {
            /* Store the severity mask to local variable */
            dataDTCSeverity_u8 = pMsgContext->reqData[1] ;

            /*Check for any invalid Severity Bits set*/

            if ((dataDTCSeverity_u8 & DCM_SEVERITYMASK) == 0x00u)
            {
                /* If the available status mask is read successfully */
                if(E_OK == Dem_GetDTCStatusAvailabilityMask(ClientId_u8,&dataStatusAvailMask_u8))
                {
                    /* Copy the status mask to the local variable */
                    stDTCStatus_u8 = pMsgContext->reqData[2];

                    /*TRACE[SWS_Dcm_00379]*/
                    /* Fill the response bytes */
                    pMsgContext->resData[0] = DSP_REPORT_DTC_BY_SEVERITY_MASK;
                    pMsgContext->resData[1] = dataStatusAvailMask_u8;
                    pMsgContext->resDataLen = 0x02;

                    /*Get the effective status mask*/
                    stDTCStatus_u8 = (stDTCStatus_u8 & dataStatusAvailMask_u8);

                    if((stDTCStatus_u8 != 0x00u) && (dataDTCSeverity_u8 != 0x00u))
                    {
                        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3723]*/
                        /*Call API to set the DTC Filter */
                        dataRetSetDTCFilter_u8 = Dem_SetDTCFilter(ClientId_u8,
                                                                 stDTCStatus_u8,
                                                                 DEM_DTC_FORMAT_UDS,
                                                                 DEM_DTC_ORIGIN_PRIMARY_MEMORY,
                                                                 TRUE,
                                                                 dataDTCSeverity_u8,
                                                                 FALSE
                                                             );
                        if (E_OK == dataRetSetDTCFilter_u8)
                        {
                            /* Move the state to calculate the number of fltered dtc */
                            Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFCALCNUMDTC;
                        }
                        else
                        {
                            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetSetDTCFilter_u8);
                            dataretVal_u8 = E_NOT_OK;
                        }
                    }
                    else
                    {
                        /* Do nothing. Response buffer is already filled with subfunction and availability status */
                        dataretVal_u8 = E_OK;
                    }

                }
                /* Status mask is not read properly from DEM */
                else
                {
                    /* Set Negative response */
                    *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                    dataretVal_u8 = E_NOT_OK;
                }
            }
            else
            {
                /*Set Negative Response*/
                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
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


    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFCALCNUMDTC)
    {
        /* Copy the maximum response length */
        nrResMaxDataLen_u32 = pMsgContext->resMaxDataLen;

        /* Call the DEM API to get filtered number of DTC */
        dataRetNumFltDTC_u8 = Dem_GetNumberOfFilteredDTC(ClientId_u8,&nrFltDTC_u16);

        /* If filtered DTC number is obtained without error */
        if(E_OK == dataRetNumFltDTC_u8)
        {
            /* Check if number of filtered DTCs is nonzero */
            if(nrFltDTC_u16 !=0)
            {
                /* Calculate total response length */
                nrResDataLen_u32 = (Dcm_MsgLenType)((Dcm_MsgLenType)(nrFltDTC_u16 * 0x06u) + (Dcm_MsgLenType)0x02);

                if(nrResDataLen_u32 <= nrResMaxDataLen_u32)
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
            /*remain in the same state and call the API again */
            dataretVal_u8 = DCM_E_PENDING;
        }
        else
        {
            /* Set negative response */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
            dataretVal_u8 = E_NOT_OK;
        }
    }


    /*TRACE[SWS_Dcm_00379]*/
    /* State to get the filtered DTC , severity , status and functional unit information from DEM */
    if( Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
    {
        idxLoop_qu8 = 0;

        /* Copy the maximum response length */
        nrResMaxDataLen_u32 = pMsgContext->resMaxDataLen;

        /* Copy the response length */
        nrResDataLen_u32 = pMsgContext->resDataLen;

        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3723]*/
        /* Loop for maximum number of DTC's to read in a cycle */
        do
        {
            idxLoop_qu8++;

            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3723]*/
            /* Get the DTC, severity, status and functional unit from DEM */
            dataretGetNextFiltDTC_u8 = Dem_GetNextFilteredDTCAndSeverity(ClientId_u8,
                                                                     &dataDTC_u32,
                                                                     &stDTCStatus_u8,
                                                                     &dataDTCSeverity_u8,
                                                                     &dataDTCFunctionalUnit_u8);
            /* If the DTC was filtered without any error */
            if(E_OK == dataretGetNextFiltDTC_u8)
            {
                /* Check if the response buffer is available for filling the response */
                if((nrResDataLen_u32 + 0x06u) <= nrResMaxDataLen_u32)
                {
                    /*TRACE[SWS_Dcm_00379]*/
                    /* Fill the response buffer with DTC and status */
                    pMsgContext->resData[nrResDataLen_u32] = dataDTCSeverity_u8;
                    nrResDataLen_u32++;
                    pMsgContext->resData[nrResDataLen_u32] = dataDTCFunctionalUnit_u8;
                    nrResDataLen_u32++;
                    pMsgContext->resData[nrResDataLen_u32] = (uint8)(dataDTC_u32>>16u);
                    nrResDataLen_u32++;
                    pMsgContext->resData[nrResDataLen_u32] = (uint8)(dataDTC_u32>>8u);
                    nrResDataLen_u32++;
                    pMsgContext->resData[nrResDataLen_u32] = (uint8)(dataDTC_u32);
                    nrResDataLen_u32++;
                    pMsgContext->resData[nrResDataLen_u32] = stDTCStatus_u8;
                    nrResDataLen_u32++;
                }
                else
                {
                    /* Response buffer exceeded - set negative response code */
                    *dataNegRespCode_u8 = DCM_E_RESPONSETOOLONG;
                    dataretVal_u8 = E_NOT_OK;
                }
            }
            /* If more time is needed to get the filtered DTC */
            else if(DEM_PENDING == dataretGetNextFiltDTC_u8)
            {
                dataretVal_u8 = DCM_E_PENDING;
                break;
            }
            /* If all the DTC's are read completely */
            else if(DEM_NO_SUCH_ELEMENT == dataretGetNextFiltDTC_u8)
            {
                /*Reset State to DSP_RDTC_SFINIT*/
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
                idxLoop_qu8            = DCM_CFG_RDTC_MAXNUMDTCREAD;
                dataretVal_u8 = E_OK;
            }
            else
            {
                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                dataretVal_u8 = E_NOT_OK;
            }
        }while((idxLoop_qu8 < DCM_CFG_RDTC_MAXNUMDTCREAD) && (*dataNegRespCode_u8==0));

        /* Update the message table with response data length */
        pMsgContext->resDataLen = nrResDataLen_u32;
    }
    else
    {
        /* Do nothing */
    }
    return dataretVal_u8;
}
#endif
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif


