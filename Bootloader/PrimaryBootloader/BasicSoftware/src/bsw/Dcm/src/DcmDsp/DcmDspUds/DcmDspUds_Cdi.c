
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Cdi_Prv.h"
#include "Rte_Dcm.h"

#if (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_CLEARDIAGNOSTICINFORMATION_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
static uint32 DTCId_u32;       /* Requested DTC or group of DTC */
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
static Std_ReturnType  s_serviceRetVal_u8;    /* Variable to store the return value of CDI service */
static uint8 ClientID_u8;  /* Variable to store the DEM Client ID */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

#if (DCM_CFG_DSP_CDI_CONDITIONCHECK_ENABLED != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name    : Dcm_Prv_CDIConditionCheck
 Description      : Perform Configured Condition Checks for CDI service
 Parameter        : Dcm_NegativeResponseCodeType* dataNegRespCode_u8
 Return value     : Std_ReturnType
***********************************************************************************************************************/
/* TRACE: [SWS_Dcm_01269] */

static Std_ReturnType Dcm_Prv_CDIConditionCheck(Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType Result = E_OK;
    if(Dcm_CDIUserConditionCheck_pf != (CDIUserConditionCheck_pf)NULL_PTR)
    {
        Result = (*Dcm_CDIUserConditionCheck_pf) (DTCId_u32,dataNegRespCode_u8);
    }
    if(Result == E_OK)
    {
        if(Dcm_CdiModeRuleRef_pf != (Cdi_ModeRuleRef_pf)NULL_PTR)
        {
            /* Invoke Mode Rule and convert boolean return value to Std_Return Type */
            Result = ((*Dcm_CdiModeRuleRef_pf) (dataNegRespCode_u8) == TRUE)?E_OK:E_NOT_OK;
        }
    }
    if(Result == E_NOT_OK)
    {
        *dataNegRespCode_u8 = (*dataNegRespCode_u8 == 0x0u)?DCM_E_CONDITIONSNOTCORRECT:*dataNegRespCode_u8;
    }
    else
    {
        *dataNegRespCode_u8 = 0x00u;
    }
    return Result;
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_CDISelectDTCFilter
 Description      : Extracts DTC and selects the DTC in DEM application
 Parameter        : Dcm_SrvOpStatusType OpStatus,const Dcm_MsgContextType* pMsgContext,
                    Dcm_NegativeResponseCodeType* dataNegRespCode_u8
 Return value     : void
***********************************************************************************************************************/
/* TRACE[SWS_Dcm_01263] */
static void Dcm_Prv_CDISelectDTCFilter(Dcm_SrvOpStatusType* OpStatus,\
                                      const Dcm_MsgContextType* pMsgContext,\
                                      Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType Result = E_NOT_OK; /* To store the result of Select DTC Operation */
    /* Check whether atleast a 3-byte DTC is sent in the request data */
    if(pMsgContext->reqDataLen == DCM_CDI_REQ_LEN)
    {
        /* Extract DTC from request Data ,merge the 3 bytes to form the DTC */
        DTCId_u32 = (((uint32)(pMsgContext->reqData[DCM_POS_DTC_HIGHBYTE]) << DCM_POS_DTC_BIT16) | \
                ((uint32)(pMsgContext->reqData[DCM_POS_DTC_MIDDLEBYTE]) << DCM_POS_DTC_BIT8) | \
                (uint32)(pMsgContext->reqData[DCM_POS_DTC_LOWBYTE]));

        Result = Dem_SelectDTC(ClientID_u8,DTCId_u32,DEM_DTC_FORMAT_UDS,DEM_DTC_ORIGIN_PRIMARY_MEMORY);

        if(Result == E_OK)
        {
            /* Update the Opstatus to DCM_CHECKDATA to process service further */
            *OpStatus = DCM_CHECKDATA;
        }
        else
        {
            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(Result);
        }
    }
    else
    {
        *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
}

/***********************************************************************************************************************
 Function name    : DCM_Prv_GetDTCSelectionResult
 Description      : Obtain the Result of DTC selection and verify condition checks
 Parameter        : Dcm_SrvOpStatusType OpStatus,Dcm_NegativeResponseCodeType* dataNegRespCode_u8
 Return value     : void
***********************************************************************************************************************/
static void DCM_Prv_GetDTCSelectionResult(Dcm_SrvOpStatusType* OpStatus,
                                                 Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    /* To store the result of Selection Result for Clear DTC Operation */
    Std_ReturnType Result = Dem_GetDTCSelectionResultForClearDTC(ClientID_u8);

    if(Result == E_OK)
    {
#if (DCM_CFG_DSP_CDI_CONDITIONCHECK_ENABLED != DCM_CFG_OFF)
        if(Dcm_Prv_CDIConditionCheck(dataNegRespCode_u8) == E_OK)
#endif
        {
            *OpStatus = DCM_PROCESSSERVICE;
        }
    }
    else if (Result == DEM_PENDING)
    {
        s_serviceRetVal_u8 = DCM_E_PENDING;
    }
    else
    {
        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(Result);
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_DcmClearDiagnosticInformation
 Description      : Obtain the Result of DTC selection
 Parameter        : Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType* pMsgContext,
                    Dcm_NegativeResponseCodeType* dataNegRespCode_u8
 Return value     : void
***********************************************************************************************************************/
/* TRACE [SWS_Dcm_00005],[SWS_Dcm_00705] */

static void Dcm_Prv_ClearDTCInformation(Dcm_SrvOpStatusType* OpStatus,\
                                               Dcm_MsgContextType* pMsgContext,\
                                               Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType Result = Dem_ClearDTC(ClientID_u8);

    switch(Result)
    {
        /* If the clearing of DTC information is successful */
        case E_OK :
            #if(DCM_ROE_ENABLED != DCM_CFG_OFF)
            DcmAppl_DcmClearRoeEventInformation();
            #endif
            pMsgContext->resDataLen = 0x0u;   /* Set Postive response with response length 0 */
            *OpStatus = DCM_INITIAL;          /* Reset the state machine */
            s_serviceRetVal_u8 = E_OK;
            break;

        /* If DEM needs more time to clear the information */
        case DEM_PENDING:
            /* Do nothing and remain in the same state */
            s_serviceRetVal_u8 = DCM_E_PENDING;
            break;

       /* If the DTC origin or DTC kind is incorrect or some problems in DEM */
        default:
            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(Result);
            break;
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_DcmClearDiagnosticInformation
 Description      : Service is used to clear diagnostic information
                    Used to Remove either one DTC or a group of DTCs and all related information from the fault memory
 Parameter        : Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType* pMsgContext,
                    Dcm_NegativeResponseCodeType* dataNegRespCode_u8
 Return value     : Std_ReturnType
***********************************************************************************************************************/
/* TRACE:[SWS_Dcm_00247] */

Std_ReturnType Dcm_DcmClearDiagnosticInformation(Dcm_SrvOpStatusType OpStatus,\
                                                 Dcm_MsgContextType* pMsgContext,\
                                                 Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    ClientID_u8 = Dcm_Prv_GetDemClientId(DCM_UDSCONTEXT); /* Store the Client ID */
    *dataNegRespCode_u8   = 0x0u;
    s_serviceRetVal_u8 = DCM_E_PENDING;

    if (OpStatus == DCM_CANCEL)
    {
        s_serviceRetVal_u8 = E_OK;
    }
    else
    {
        /* To extract DTC and Select the DTC in DEM */
        if (Dcm_SrvOpstatus_u8 == DCM_INITIAL)
        {
            Dcm_Prv_CDISelectDTCFilter(&Dcm_SrvOpstatus_u8,pMsgContext,dataNegRespCode_u8);
        }

        /* If OpStatus is set to check data  then call the DCM_PrivDTCConditionCheck to
         * check user condition function or mode rule check for DTC */
        if (Dcm_SrvOpstatus_u8 == DCM_CHECKDATA)
        {
            DCM_Prv_GetDTCSelectionResult(&Dcm_SrvOpstatus_u8,dataNegRespCode_u8);
        }

        /* If OpStatus is set to process service then call the Dcm_PrivClearDTCInformation
         * to clear  user DTC information and set positive response if DTC cleared successsfully */
        if (Dcm_SrvOpstatus_u8 == DCM_PROCESSSERVICE)
        {
            Dcm_Prv_ClearDTCInformation(&Dcm_SrvOpstatus_u8, pMsgContext, dataNegRespCode_u8);
        }

        /* If negative response code is set */
        if(*dataNegRespCode_u8 != 0x0u)
        {
            s_serviceRetVal_u8 = E_NOT_OK;
            /* Set response length to 0 */
            pMsgContext->resDataLen = 0x0u;
            /* Reset the state machine */
            Dcm_SrvOpstatus_u8 = DCM_INITIAL;
        }
    }
    return s_serviceRetVal_u8;
}

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*(DCM_CFG_DSP_CLEARDIAGNOSTICINFORMATION_ENABLED != DCM_CFG_OFF)*/
#endif /* (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) */
