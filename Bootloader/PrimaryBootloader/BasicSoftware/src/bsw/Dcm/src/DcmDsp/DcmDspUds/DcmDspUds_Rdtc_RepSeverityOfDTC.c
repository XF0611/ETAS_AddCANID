
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x09_ENABLED != DCM_CFG_OFF)


#define DCM_RDTC_RESPONSELENGTH_0x01   (0x01u)
#define DCM_RDTC_RESPONSELENGTH_0x08   (0x08u)



#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **************************************************************************************************
 * Dcm_Dsp_ReportSeverityInfoOfDTC :
 * This function is used to read the Severity and Functional unit information of DTC.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */
Std_ReturnType Dcm_Dsp_ReportSeverityOfDTC(Dcm_SrvOpStatusType OpStatus,\
                                           Dcm_MsgContextType *pMsgContext,\
                                           Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint32 dataDTC_u32;                        /* Local variable for getting DTC from request*/
    uint8  dataHighByte_u8;                    /* Local variable to copy byte 1 of request */
    uint8  dataMiddleByte_u8;                  /* Local variable to copy byte 2 of request */
    uint8  dataLowByte_u8;                     /* Local variable to copy byte 3 of request */
    uint8  dataFunctionalUnit_u8;              /* Local variable to store functional unit information */

    Std_ReturnType  dataRetGetStatusOfDTC;    /* Return type of Dem_GetStatusOfDTC */
    Std_ReturnType  dataRetGetSelectDTC;      /* Return type of Dem_GetSelectDTC */
    Std_ReturnType  dataRetGetSeverityOfDTC;  /* Return type of Dem_GetSeverityOfDTC */
    Std_ReturnType  dataRetGetFuncUnitOfDTC_u8;  /* Return type of Dem_GetFunctionalUnitOfDTC */
    Std_ReturnType  dataretVal;               /* Variable to store the returnType from DcmAppl API*/

    static Dem_DTCSeverityType dataDTCSeverity_u8; /* Variable to get the severity of DTC */
    static uint8  dataRDTC0x09StMask_u8;           /* variable to calculate available status mask */
    static uint8  stDTCStatus_Rdtc_u8;                  /* Variable to store the DTC status */


    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);

    /* Initialization of variables */
    *dataNegRespCode_u8 = 0x00;
    dataretVal = DCM_E_PENDING;

    /* Check for the correct request length */
    if(pMsgContext->reqDataLen == DSP_RDTC_09_REQUEST_LENGTH)
    {
        /* copy the 3 byte DTC into Dsp_RdtcDTC_u32 from request*/
        dataHighByte_u8     =   pMsgContext->reqData[0x01];
        dataMiddleByte_u8   =   pMsgContext->reqData[0x02];
        dataLowByte_u8      =   pMsgContext->reqData[0x03];
        dataDTC_u32         =   dataHighByte_u8;
        dataDTC_u32         =   (dataDTC_u32 << 0x8u);
        dataDTC_u32         =   (dataDTC_u32 | dataMiddleByte_u8);
        dataDTC_u32         =   (dataDTC_u32 << 0x8u);
        dataDTC_u32         =   (dataDTC_u32 | dataLowByte_u8);

        if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
        {
            /* Get the DTC Status Availability Mask */
            if(E_OK == Dem_GetDTCStatusAvailabilityMask(ClientId_u8,&dataRDTC0x09StMask_u8))
            {
                /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3728]*/
                dataRetGetSelectDTC = Dem_SelectDTC(ClientId_u8,
                                                    dataDTC_u32,
                                                    DEM_DTC_FORMAT_UDS,
                                                    DEM_DTC_ORIGIN_PRIMARY_MEMORY
                                                   );

                /*Check if DEM has returned positived reponse*/
                if(E_OK == dataRetGetSelectDTC)
                {
                    Dcm_DspRDTCSubFunc_en = DSP_RDTC_GETDTCSTATUS;
                }
                else
                {
                    *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetGetSelectDTC);
                    dataretVal=E_NOT_OK;
                }

            }
            /* If the available status mask is not correct */
            else
            {
                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                dataretVal=E_NOT_OK;
            }
        }

        /*After selecting a DTC using API Dem_SelectDTC(), next operations should be performed in next Main Cycle as DEM will need it */
        if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_GETDTCSTATUS)
        {
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3731]*/
            /* Call DEM API to get the status of DTC sent from tester */
            dataRetGetStatusOfDTC = Dem_GetStatusOfDTC(ClientId_u8,&stDTCStatus_Rdtc_u8);

            /* If the available status mask is read successfully */
            if(E_OK == dataRetGetStatusOfDTC)
            {
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_GETDTCSEVERITY;
            }
            /* If the DTC sent by the tester is not correct */
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3793]*/
            else if(DEM_NO_SUCH_ELEMENT == dataRetGetStatusOfDTC)
            {
                /*When DEM_STATUS_FAILED Dcm should send an positive response with SID and subfunction alone*/
                pMsgContext->resData[0x00] = pMsgContext->reqData[0x00];
                /* Update the Response data length information */
                pMsgContext->resDataLen = DCM_RDTC_RESPONSELENGTH_0x01;
                dataretVal = E_OK;
            }
            else if(DEM_PENDING == dataRetGetStatusOfDTC)
            {
                /* Do nothing - remain in the same state and call the API again */
                dataretVal = DCM_E_PENDING;
            }
            else
            {
                /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3734]*/
                *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetGetStatusOfDTC);
                dataretVal=E_NOT_OK;
            }

        }

        if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_GETDTCSEVERITY)
        {
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3729]*/
            /* Get the Severity of the DTC */
            dataRetGetSeverityOfDTC = Dem_GetSeverityOfDTC(ClientId_u8,&dataDTCSeverity_u8);

            /* If the Severity of DTC was read without any errors OR */
            /* If the severity information is not available for this DTC */
            if(E_OK == dataRetGetSeverityOfDTC)
            {
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;
            }
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3795]*/
            else if(DEM_PENDING == dataRetGetSeverityOfDTC)
            {
                /* Do nothing - remain in the same state and call the API again */
                dataretVal = DCM_E_PENDING;
            }
            else
            {
                /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3733]*/
                *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetGetSeverityOfDTC);
                dataretVal=E_NOT_OK;
            }
        }

        if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
        {
            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3730]*/
            dataRetGetFuncUnitOfDTC_u8 = Dem_GetFunctionalUnitOfDTC(ClientId_u8,&dataFunctionalUnit_u8);

            /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3726]*/
            if(E_OK == dataRetGetFuncUnitOfDTC_u8)
            {
               /* Fill the response buffer */
               pMsgContext->resData[0x00] = pMsgContext->reqData[0x0];  /* Sub function */
               pMsgContext->resData[0x01] = dataRDTC0x09StMask_u8;      /* Availability Mask */
               pMsgContext->resData[0x02] = dataDTCSeverity_u8;         /* DTC Severity */
               pMsgContext->resData[0x03] = dataFunctionalUnit_u8;      /* Functional Unit */
               pMsgContext->resData[0x04] = dataHighByte_u8;            /* DTC High Byte */
               pMsgContext->resData[0x05] = dataMiddleByte_u8;          /* DTC Middle Byte */
               pMsgContext->resData[0x06] = dataLowByte_u8;             /* DTC Low Byte */
               pMsgContext->resData[0x07] = stDTCStatus_Rdtc_u8;             /* DTC Status */
               pMsgContext->resDataLen    = DCM_RDTC_RESPONSELENGTH_0x08;
               dataretVal=E_OK;
            }
            else if(DEM_PENDING == dataRetGetFuncUnitOfDTC_u8)
            {
               /* Do nothing - remain in the same state and call the API again */
               dataretVal = DCM_E_PENDING;
            }
            else
            {
                /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3732]*/
                *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetGetFuncUnitOfDTC_u8);
                dataretVal=E_NOT_OK;
            }
        }
    }
    else
    {
        /* Request Length not correct */
        *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        dataretVal=E_NOT_OK;
    }
    return dataretVal;
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif

