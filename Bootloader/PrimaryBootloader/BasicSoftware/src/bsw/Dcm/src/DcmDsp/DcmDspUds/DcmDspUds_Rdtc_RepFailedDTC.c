
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"


#if((DCM_CFG_DSP_RDTCSUBFUNC_0x0B_ENABLED != DCM_CFG_OFF)|| \
        (DCM_CFG_DSP_RDTCSUBFUNC_0x0C_ENABLED != DCM_CFG_OFF)|| \
        (DCM_CFG_DSP_RDTCSUBFUNC_0x0D_ENABLED != DCM_CFG_OFF)|| \
        (DCM_CFG_DSP_RDTCSUBFUNC_0x0E_ENABLED != DCM_CFG_OFF))

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
#define DCM_RDTC_RESPONSELENGTH_0x01   (0x01u)
#define DCM_RDTC_RESPONSELENGTH_0x02   (0x02u)
#define DCM_RDTC_RESPONSELENGTH_0x06   (0x06u)


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **************************************************************************************************
 * Dcm_Dsp_ReportFailedDTC :
 *  This function is used to implement the  subfunctions 0x0B,0x0C,0x0D,0x0E(reportFirstTestFailedDTC, reportFirstConfirmedDTC, reportMostRecentTestFailedDTC,
 *  reportMostRecentConfirmedDTC).
 *  From this, function  Dem_GetDTCStatusAvailabilityMask is called to get  the status availability mask.
 *  Dem_GetDTCByOccurrenceTime is called to get the FirstTestFailedDTC(DTCRequest param set to DEM_FIRST_FAILED_DTC)/
 *  FirstConfirmedDTC(DTCRequest param set to DEM_FIRST_DET_CONFIRMED_DTC)/MostRecentTestFailedDTC(DEM_MOST_RECENT_FAILED_DTC)/
 *  MostRecentConfirmedDTC(DEM_MOST_REC_DET_CONFIRMED_DTC).Then Dem_GetStatusOfDTC is called to get the status of the DTC
 *  which received by calling the Dem_GetDTCByOccurrenceTime.
 *
 *
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3776]*/
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager-8038]*/


Std_ReturnType Dcm_Dsp_ReportFailedDTC (Dcm_SrvOpStatusType OpStatus,  \
                                       Dcm_MsgContextType *pMsgContext,\
                                       Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    static uint8      dataStatusAvailMask_u8;        /* Variable to store status availability mask */
    static uint32     dataDTC_u32;                   /* Variable to store DTC */
    Std_ReturnType    dataRetGetDTCByOccurTime_u8;   /* Return type of Dem_GetDTCByOccurrenceTime */
    Std_ReturnType    dataRetGetStatusOfDTC_u8;      /* Return type of Dem_GetStatusOfDTC */
    Std_ReturnType    dataRetGetSelectDTC_u8;        /* Return type of Dem_GetSelectDTC_u8*/
    Std_ReturnType    dataRetVal_u8;                 /* Local variable to store the service return value */
    uint8             stDTCStatus_u8;                /* Variable to store the DTC status */
    uint8             dataRDTCSubFunc_u8;            /* Local variable to store the subfunction */
    boolean           flgSubFnSupp_b;
    Dem_DTCRequestType DTCRequest;


    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);
    /* Initialization of  variables */
    *dataNegRespCode_u8      =0x00;
    /*Initialize flag flgSubFnSupp_b to TRUE */
    flgSubFnSupp_b = TRUE;
    dataRetGetDTCByOccurTime_u8 = E_NOT_OK;
    /* Copy the subfunction parameter to local variable */
    dataRDTCSubFunc_u8 = pMsgContext->reqData[DSP_RDTC_POSSUBFUNC];
    dataRetVal_u8=DCM_E_PENDING;
    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);
    /* check for request length */

    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
    {
        if(pMsgContext->reqDataLen == DSP_RDTC_B_C_D_E_REQLEN)
        {
            /* Request for status avilabilty mask */
             if(E_OK == Dem_GetDTCStatusAvailabilityMask(ClientId_u8,&dataStatusAvailMask_u8))
             {
                 /* Update the subfunction in the response buffer */
                 pMsgContext->resData[0]=pMsgContext->reqData[DSP_RDTC_POSSUBFUNC];
                 /* Update the response length */
                 pMsgContext->resDataLen=1;

                 if(dataRDTCSubFunc_u8 == DSP_REPORT_FIRST_TEST_FAILED_DTC)
                 {
                     DTCRequest = DEM_FIRST_FAILED_DTC;
                 }
                 else if(dataRDTCSubFunc_u8 == DSP_REPORT_FIRST_CONFIRMED_DTC)
                 {
                     DTCRequest = DEM_FIRST_DET_CONFIRMED_DTC;
                 }
                 else if(dataRDTCSubFunc_u8 == DSP_REPORT_MOST_RECENT_TEST_FAILED_DTC)
                 {
                     DTCRequest = DEM_MOST_RECENT_FAILED_DTC;
                 }
                 else if(dataRDTCSubFunc_u8 == DSP_REPORT_MOST_RECENT_CONFIRMED_DTC)
                 {
                     DTCRequest = DEM_MOST_REC_DET_CONFIRMED_DTC;
                 }
                 else
                 {
                     /*Set the flag flgSubFnSupp_b to FALSE*/
                     flgSubFnSupp_b = FALSE;

                     /*Retun E_OK from the service*/
                     dataRetVal_u8=E_OK;
                 }

                 /*Check if the flag flgSubFnSupp_b is set to TRUE*/
                 if (flgSubFnSupp_b != FALSE)
                 {
                     /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3745]*/
                     /*call Dem_GetDTCByOccurrenceTime to get the most recent confirmed DTC*/
                     dataRetGetDTCByOccurTime_u8 = Dem_GetDTCByOccurrenceTime(ClientId_u8,\
                                                                              DTCRequest,\
                                                                              &dataDTC_u32);

                     if((E_OK == dataRetGetDTCByOccurTime_u8) && (*dataNegRespCode_u8 == 0x00))
                     {
                         dataRetGetSelectDTC_u8 = Dem_SelectDTC(ClientId_u8,\
                                                                dataDTC_u32,\
                                                                DEM_DTC_FORMAT_UDS,\
                                                                DEM_DTC_ORIGIN_PRIMARY_MEMORY);

                         if(E_OK == dataRetGetSelectDTC_u8)
                         {
                             Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;
                         }
                         else
                         {
                             *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetGetSelectDTC_u8);
                             dataRetVal_u8 = E_NOT_OK;
                         }
                     }
                     /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3746]*/
                     /*There is no failed /confirmed DTC available after the last CDI request.So send positive response with only status availability mask.*/
                     else if (DEM_NO_SUCH_ELEMENT == dataRetGetDTCByOccurTime_u8)
                     {
                         pMsgContext->resData[1] = dataStatusAvailMask_u8;
                         pMsgContext->resDataLen = DCM_RDTC_RESPONSELENGTH_0x02;
                         /*Retun E_OK from the service*/
                         dataRetVal_u8=E_OK;
                     }
                     else
                     {
                         *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                         /*Retun E_NOT_OK from the service*/
                         dataRetVal_u8=E_NOT_OK;
                     }
                 }
             }
             else
             {
                 *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                 /*Retun E_NOT_OK from the service*/
                 dataRetVal_u8=E_NOT_OK;
             }
        }
        else
        {
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            /*Retun E_NOT_OK from the service*/
            dataRetVal_u8=E_NOT_OK;
        }
    }

    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
    {
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3744]*/
        /*Received the DTC from Dem_GetDTCByOccurrenceTime correctly.Now request for the DTC's status */
        dataRetGetStatusOfDTC_u8 = Dem_GetStatusOfDTC(ClientId_u8,&stDTCStatus_u8);

        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager-8634]*/
        if(E_OK == dataRetGetStatusOfDTC_u8)
        {
            /*Fill the response, status availability mask,DTC and DTC status*/
            pMsgContext->resData[1] = dataStatusAvailMask_u8;
            pMsgContext->resData[2] = (uint8)(dataDTC_u32>>16u);
            pMsgContext->resData[3] = (uint8)(dataDTC_u32>>8u);
            pMsgContext->resData[4] = (uint8)(dataDTC_u32);
            pMsgContext->resData[5] = stDTCStatus_u8;
            /* Update the message table with response data length */
            pMsgContext->resDataLen = DCM_RDTC_RESPONSELENGTH_0x06;
            /*Retun E_OK from the service*/
            dataRetVal_u8=E_OK;
        }
        /* If the DTC sent by the tester is not correct */
        else if(DEM_NO_SUCH_ELEMENT == dataRetGetStatusOfDTC_u8)
        {
            /*When DEM_NO_SUCH_ELEMENT is returned Dcm should send an positive response with SID and subfunction alone*/
            pMsgContext->resData[0x00] = pMsgContext->reqData[0x00];
            /* Update the Response data length information */
            pMsgContext->resDataLen = DCM_RDTC_RESPONSELENGTH_0x01;
            /*Retun E_OK from the service*/
             dataRetVal_u8=E_OK;
        }
        /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3795]*/
        else if(DEM_PENDING == dataRetGetStatusOfDTC_u8)
        {
            /* Do nothing - remain in the same state and call the API again */
            dataRetVal_u8 = DCM_E_PENDING;
        }
        else
        {
            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetGetStatusOfDTC_u8);
            dataRetVal_u8 = E_NOT_OK;
        }
    }

    return dataRetVal_u8;
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif
#endif
