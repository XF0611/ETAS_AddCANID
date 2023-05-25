

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"

#if((DCM_CFG_DSP_RDTCSUBFUNC_0x01_ENABLED != DCM_CFG_OFF)||\
    (DCM_CFG_DSP_RDTCSUBFUNC_0x11_ENABLED != DCM_CFG_OFF)||\
    /* FC_VariationPoint_START */ \
    (DCM_CFG_DSP_RDTCSUBFUNC_0x12_ENABLED != DCM_CFG_OFF)||\
    /* FC_VariationPoint_END */ \
    (DCM_CFG_DSP_RDTCSUBFUNC_0x07_ENABLED != DCM_CFG_OFF))


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
#define DCM_SEVERITYMASK               (0x1Fu) //Only bit 7 to 5 of the severity mask byte are valid
#define DCM_SEVERITY_0x00              (0x00u)
#define DCM_STATUS_0x00                (0x00u)
#define DCM_RDTC_RESPONSELENGTH_0x05   (0x05u)

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/



/***********************************************************************************************************************
 *    Function Definitions
 **********************************************************************************************************************/
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



/***********************************************************************************************************************
 Function name    : Dcm_Prv_isRequestLengthValid
 Syntax           : Dcm_Prv_isRequestLengthValid(RDTCSubFunc_u8)
 Description      : This API is used to check if request Length is Valid or not.
 Parameter        : uint8
 Return value     : uint8
***********************************************************************************************************************/
static void Dcm_Prv_GetRequestLength(uint8 RDTCSubFunc_u8,uint8 *RequestLenPtr_u8)
{

#if((DCM_CFG_DSP_RDTCSUBFUNC_0x01_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_RDTCSUBFUNC_0x11_ENABLED != DCM_CFG_OFF) || \
    /* FC_VariationPoint_START */ \
    (DCM_CFG_DSP_RDTCSUBFUNC_0x12_ENABLED != DCM_CFG_OFF))
    /* FC_VariationPoint_END */

    *RequestLenPtr_u8 = DSP_RDTC_01_11_12_REQUEST_LENGTH;
#endif

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x07_ENABLED != DCM_CFG_OFF)
    if(RDTCSubFunc_u8 == DSP_REPORT_NUM_SEVERMASK_DTC)
    {
        *RequestLenPtr_u8 = DSP_RDTC_07_REQUEST_LENGTH;
    }
#else
    (void)RDTCSubFunc_u8;
#endif
}


/***********************************************************************************************************************
 Function name    : Dcm_Prv_GetSeverity
 Syntax           : Dcm_Prv_GetSeverity(ReqData,*dataNegRespCode_u8,*dataretVal)
 Description      : This Function is used to used to get the SeverityMask from the request.
 Parameter        : Dcm_MsgContextType*,Dcm_NegativeResponseCodeType*,Std_ReturnType*
 Return value     : Dem_DTCSeverityType
***********************************************************************************************************************/
static Dem_DTCSeverityType Dcm_Prv_GetSeverity(const Dcm_MsgContextType *pMsgContext,\
                                               Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    /* Set dataDTCSeverity to DEM_SEVERITY_NO_SEVERITY for subfunctions 0x01,0x11 and 0x12 */
    uint8 DTCSeverity_u8 = DEM_SEVERITY_NO_SEVERITY;

    if(pMsgContext->reqData[DSP_RDTC_POSSUBFUNC] == DSP_REPORT_NUM_SEVERMASK_DTC)
    {
        /*Store Severity for SubFunction 0x07*/
        DTCSeverity_u8 = pMsgContext->reqData[1];

        /*Check if SeverityMask in receoved request is Zero or not*/
        if(DTCSeverity_u8 != DCM_SEVERITY_0x00)
        {
            /*Check if only Bits 7 to 5 in SeverityMask are Valid*/
            if((DTCSeverity_u8 & DCM_SEVERITYMASK) != 0x0u)
            {
                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
            }
        }
    }

    return DTCSeverity_u8;
}

/**
 ***************************************************************************************************
 * Dcm_Dsp_ReportNumberOfDTC
 * This function is used to implement the RDTC service for the following sub functions:
 * 0x01(reportNumberOfDTCByStatusMask),
 * 0x07 (reportNumberOfDTCBySeverityMaskRecord),
 * 0x11(reportNumberOfMirrorMemoryDTCByStatusMask),
 \ifnot COMP_DCM
 * FC_VariationPoint_START
 \endif
 * 0x12(reportNumberOfEmissionsRelatedOBDDTCByStatusMask)
  \ifnot COMP_DCM
 * FC_VariationPoint_END
  \endif

 * The request contains the Status mask sent from the tester for subfunctions 0x01,0x11,0x12 and contains
 * severity mask and status mask for subfunction 0x07. The valid bits of the DTC Severity Mask are bits 5
 * to 7 as described in ISO-14229-1. Response sent to the tester contains the Status Availability Mask of
 * the ECU,the format Identifier and the Count of number of DTCs matching the tester Request.
 * \param            pMsgContext    Pointer to message structure

 * \retval           None

 * \seealso
 *
 ***************************************************************************************************
 */
Std_ReturnType Dcm_Dsp_ReportNumberOfDTC (Dcm_SrvOpStatusType OpStatus,\
                                          Dcm_MsgContextType *pMsgContext,\
                                          Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint16              nrFltDTC_u16;                                /* Number of filtered DTCs */
    uint8               dataStatusAvailMask_u8;                      /* Variable to store status availability mask */
    uint8               stDTCStatus_u8;                              /* Variable to store the DTC status */
    uint8               nrReqLength_u8;                              /* Variable to store request length */
    uint8               dataRDTCSubFunc_u8;                          /* Local Variable to store sub-function level */
    Dem_DTCOriginType   dataDTCOrigin_u16;                            /* Local Variable to store DTC ORIGIN*/
    Dem_DTCSeverityType dataDTCSeverity;                          /* Variable to store DTC Severity Mask */
    boolean             dataDTCSeverityType;              /* Variable to store Severity type */
    Std_ReturnType              dataRetSetDTCFilter;              /* Return type for Dem_SetDTCFilter*/
    Std_ReturnType              dataRetGetNumOfFiltDTC;           /* Return type Dem_GetNumberOfFilteredDTC */
    Std_ReturnType              dataretVal;                       /* Variable to store the returnType from DcmAppl API          */

    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);

    /* Initialization to zero - No error */
    *dataNegRespCode_u8 = 0;
    dataretVal = DCM_E_PENDING;

    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
    {
        /*Store Subfunction*/
        dataRDTCSubFunc_u8 = pMsgContext->reqData[DSP_RDTC_POSSUBFUNC];

        /*Store RequestLength*/
        Dcm_Prv_GetRequestLength(dataRDTCSubFunc_u8,&nrReqLength_u8);

        /*Check if Request length is Valid*/
        if(pMsgContext->reqDataLen == nrReqLength_u8)
        {
            /*Store Severity*/
            dataDTCSeverity = Dcm_Prv_GetSeverity(pMsgContext,dataNegRespCode_u8);
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3776]*/
            if (*dataNegRespCode_u8 == 0x0)
            {
                /* If the status availability mask is read successfully */
                if(E_OK == Dem_GetDTCStatusAvailabilityMask(ClientId_u8,&dataStatusAvailMask_u8))
                {
                       /* Fill the response bytes */
                    stDTCStatus_u8 = pMsgContext->reqData[1];

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x07_ENABLED != DCM_CFG_OFF)
                    if(dataRDTCSubFunc_u8 == DSP_REPORT_NUM_SEVERMASK_DTC)
                    {
                        stDTCStatus_u8 = pMsgContext->reqData[2];
                    }
#endif
                    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager-8048]*/
                    pMsgContext->resData[0] = dataRDTCSubFunc_u8;
                    pMsgContext->resData[1] = dataStatusAvailMask_u8;

                    /* Get the DTC Format Identifier and fill into the response buffer */
                    pMsgContext->resData[2] = Dem_GetTranslationType(ClientId_u8);

                    /* Calculate the effective status Mask */
                    stDTCStatus_u8 = (dataStatusAvailMask_u8 & stDTCStatus_u8);

                    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3779]*/
                    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3781]*/
                    /* If effective status mask is not equal to zero */
                    if(stDTCStatus_u8 != DCM_STATUS_0x00)
                    {
#if(DCM_CFG_DSP_RDTCSUBFUNC_0x07_ENABLED != DCM_CFG_OFF)
                       if((pMsgContext->reqData[DSP_RDTC_POSSUBFUNC] == DSP_REPORT_NUM_SEVERMASK_DTC) &&\
                               (dataDTCSeverity == DCM_SEVERITY_0x00))
                       {
                              pMsgContext->resData[3] = 0x0;
                              pMsgContext->resData[4] = 0x0;
                              pMsgContext->resDataLen = DCM_RDTC_RESPONSELENGTH_0x05;
                              dataretVal=E_OK;
                       }
                       else
#endif
                       {
                            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3717]*/
                            /*Update DTC origin and SevarityType*/
                            switch (dataRDTCSubFunc_u8)
                            {
                                case DSP_REPORT_NUM_STATUS_MASK_DTC:
                                dataDTCOrigin_u16   = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
                                dataDTCSeverityType = FALSE;
                                break;

                                case DSP_REPORT_NUM_MIRROR_DTC:
                                dataDTCOrigin_u16   = DEM_DTC_ORIGIN_MIRROR_MEMORY;
                                dataDTCSeverityType = FALSE;
                                break;
                                /* FC_VariationPoint_START */
                                case DSP_REPORT_NUM_EMISSION_DTC:
                                dataDTCOrigin_u16   = DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY;
                                dataDTCSeverityType = FALSE;
                                break;
                                /* FC_VariationPoint_END */
                                case DSP_REPORT_NUM_SEVERMASK_DTC:
                                default:
                                dataDTCOrigin_u16   = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
                                dataDTCSeverityType = TRUE;
                                break;
                            }

                            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3717]*/
                            /* Set the DTC Filter in DEM */
                            dataRetSetDTCFilter = Dem_SetDTCFilter(ClientId_u8,
                                                                   stDTCStatus_u8,
                                                                   DEM_DTC_FORMAT_UDS,
                                                                   dataDTCOrigin_u16,
                                                                   dataDTCSeverityType,
                                                                   dataDTCSeverity,
                                                                   FALSE
                                                                 );
                            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3768]*/
                            /* If the Filter Mask sent is accepted by DEM */
                            if(E_OK == dataRetSetDTCFilter)
                            {
                                /* Move the state to fill the response */
                                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;
                            }
                            /* Filter Mask or Parameters passed are not accepted by DEM*/
                            else
                            {
                                /* Set Negative response */
                                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                dataretVal=E_NOT_OK;
                            }
                       }
                    }
                    /* If effective status mask is equal to zero, fill response with DTC count = 0x0 */
                    else
                    {
                        /* No record to read and fill the DTC count as 0x0000*/
                        pMsgContext->resData[3] = 0x0;
                        pMsgContext->resData[4] = 0x0;
                        pMsgContext->resDataLen = DCM_RDTC_RESPONSELENGTH_0x05;
                        dataretVal=E_OK;
                    }
                }
                /* Status mask is not read properly from DEM */
                else
                {
                    /* Set Negative response */
                    *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                    dataretVal=E_NOT_OK;
                }
            }
            else
            {
                /* Do nothing */
                dataretVal=E_NOT_OK;
            }
        }
        /* Request Length not correct */
        else
        {
            /* Set Negative response */
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            dataretVal=E_NOT_OK;
        }
    }

    if (Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
    {
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3717]*/
        dataRetGetNumOfFiltDTC = Dem_GetNumberOfFilteredDTC (ClientId_u8, &nrFltDTC_u16);

        if (E_OK == dataRetGetNumOfFiltDTC)
        {
            /*TRACE[SWS_Dcm_00376]*/
            /* Fill the high byte of the DTC count */
            pMsgContext->resData[3] = (uint8) (nrFltDTC_u16>>0x08u);

            /*Fill Low Byte of DTC Count*/
            pMsgContext->resData[4] = (uint8) (nrFltDTC_u16);

            /* Update the response length */
            pMsgContext->resDataLen = DCM_RDTC_RESPONSELENGTH_0x05;

            /* Reset the state to default state */
            Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
            dataretVal=E_OK;
        }
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3795]*/
        else if (DEM_PENDING == dataRetGetNumOfFiltDTC)
        {
             /* Do Nothing, the function will be called in the next Raster */
             dataretVal=DCM_E_PENDING;
        }
        else
        {
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
            dataretVal=E_NOT_OK;
        }
    }
    return dataretVal;
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif

