
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Cdtcs_Inf.h"
#include "DcmDsp_Lib_Prv.h"
#include "Rte_Dcm.h"

#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF) )

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static boolean Dcm_Cdtcs_IsReEnablePending_b=FALSE;
static boolean Dcm_Cdtcs_IsPermisionPending_b=FALSE;
static boolean Dcm_Cdtcs_ProceedPermissionHandling_b=FALSE;
static boolean Dcm_Cdtcs_IsDisableInProgress_b=FALSE;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_INIT_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
boolean Dcm_DspCDTCStatus_b = TRUE;
#define DCM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint8 Dcm_Cdtcs_SettingType_u8;
static uint8 Dcm_Cdtcs_CurrentClientId_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_8
#include "Dcm_MemMap.h"
static const uint8 Dcm_CdtcsNoNrc_u8=0x00;
#define DCM_STOP_SEC_CONST_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_16
#include "Dcm_MemMap.h"
#if(DCM_CFG_DSP_SUPPORTCDTCRECORD != DCM_CFG_ON)
static const uint16 Dcm_CdtcsExpectedRequestLength_u16 = 1;
#endif
static const uint16 Dcm_CdtcsResponseLength_u16 = 1;
#if(DCM_CFG_DSP_SUPPORTCDTCRECORD == DCM_CFG_ON)
static const uint16 Dcm_CdtcsExpectedTotalLength_u16 = 4;
#endif
#define DCM_STOP_SEC_CONST_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if(DCM_CFG_DSP_SUPPORTCDTCRECORD == DCM_CFG_ON)
#define DCM_START_SEC_CONST_32
#include "Dcm_MemMap.h"
static const uint32 Dcm_CdtcsValidRecord_u32=0xFFFFFF;
#define DCM_STOP_SEC_CONST_32/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#define DCM_CDTCS_ON 1
#define DCM_CDTCS_OFF 2

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

static Std_ReturnType Dcm_CdtcsStartService (const Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* dataNegRespCode_u8);
static Std_ReturnType Dcm_CdtcsPerformService (Dcm_NegativeResponseCodeType* dataNegRespCode_u8);
static Std_ReturnType Dcm_CdtcsIsRequestValid (const Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* dataNegRespCode_u8);
static boolean Dcm_CdtcsIsModeStatusCheckPassed (void);
static Std_ReturnType Dcm_CdtcsProcessServiceResult (Std_ReturnType Dem_Result, Dcm_NegativeResponseCodeType* dataNegRespCode_u8);
static void Dcm_CdtcsInformApplication (void);


static Std_ReturnType Dcm_CdtcsProcessPermissionState (Dcm_NegativeResponseCodeType* dataNegRespCode_u8);
static Std_ReturnType Dcm_CdtcsProcessDisablePermission (Dcm_NegativeResponseCodeType* dataNegRespCode_u8);
static boolean Dcm_CdtcsProceedRequest ( const Dcm_MsgContextType* pMsgContext);

#if(DCM_CFG_DSP_SUPPORTCDTCRECORD == DCM_CFG_ON)
static Std_ReturnType Dcm_CdtcsIsControlOptionRecordValid (const Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* dataNegRespCode_u8);
#endif

/**
 **************************************************************************************************
 * Dcm_DcmControlDTCSetting :
 * This service is used to enable or disable the setting of DTCs
 * This function is called by DSD, when SID is 0x85,
 * this service is provided in DSP module and configured in DCM identifier table.
 * When the DSP receives such a request it executes DEM function Dem_DisableDTCSetting/
 * Dem_EnableDTCSetting to disable/enable the setting of DTCs
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */
Std_ReturnType Dcm_DcmControlDTCSetting (Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType serviceResult = E_NOT_OK;

    switch(OpStatus)
    {
        case DCM_CANCEL:
        if(!Dcm_Cdtcs_IsPermisionPending_b)
        {
            Dcm_Cdtcs_IsReEnablePending_b=TRUE;
        }
        else
        {
            Dcm_Cdtcs_ProceedPermissionHandling_b=TRUE;
        }

        serviceResult=E_OK;
        break;

        case DCM_INITIAL:
        serviceResult=Dcm_CdtcsStartService(pMsgContext, dataNegRespCode_u8);
        break;

        case DCM_CHECKDATA:
        serviceResult=Dcm_CdtcsProcessDisablePermission(dataNegRespCode_u8);
        break;

        case DCM_PROCESSSERVICE:

        if(Dcm_CdtcsProceedRequest(pMsgContext))
        {
            serviceResult=Dcm_CdtcsPerformService(dataNegRespCode_u8);
        }
        else
        {
            *dataNegRespCode_u8=DCM_E_CONDITIONSNOTCORRECT;
        }
        break;

        default:

      /*default should not happen*/
        break;

    }

    if((serviceResult==E_OK) && (OpStatus!=DCM_CANCEL))
    {
        pMsgContext->resDataLen = Dcm_CdtcsResponseLength_u16;
        /* Fill the sub function in response buffer */
        pMsgContext->resData[DCM_PRV_CDTC_POSITION_SUBFUNC] = Dcm_Cdtcs_SettingType_u8;
    }

    return serviceResult;
}

static Std_ReturnType Dcm_CdtcsStartService (const Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType startServiceResult = E_NOT_OK;

    /*perform service only when valid request is received; NRC will be set within validation*/
    if(!Dcm_Cdtcs_IsReEnablePending_b)
    {
        if(Dcm_CdtcsIsRequestValid(pMsgContext, dataNegRespCode_u8)!=E_NOT_OK)
        {
            Dcm_Cdtcs_CurrentClientId_u8=Dcm_Prv_GetDemClientId(DCM_UDSCONTEXT);
            /*update setting type, so in next mainfunction (pending case) it is clear whether enabling or disabling should be processed*/
            Dcm_Cdtcs_SettingType_u8= *(pMsgContext->reqData);
            startServiceResult=Dcm_CdtcsProcessPermissionState(dataNegRespCode_u8);
        }
    }
    else
    {
        *dataNegRespCode_u8=DCM_E_BUSYREPEATREQUEST;
    }

    return startServiceResult;
}

static Std_ReturnType Dcm_CdtcsPerformService(Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType operationResult = E_NOT_OK;

    /*usage of DcmAppls as before 4.3.1 update*/
    switch(Dcm_Cdtcs_SettingType_u8)
    {
        case DCM_CDTCS_ON:
        operationResult=Dem_EnableDTCSetting(Dcm_Cdtcs_CurrentClientId_u8);
        DcmAppl_EnableDTCSetting(operationResult);
        break;
        case DCM_CDTCS_OFF:
            operationResult=Dem_DisableDTCSetting(Dcm_Cdtcs_CurrentClientId_u8);
        break;
        default:
        *dataNegRespCode_u8=DCM_E_SUBFUNCTIONNOTSUPPORTED;
        break;
    }

    /*process result only when Dem was called -> NRC will be overwritten within process result*/
    if(*dataNegRespCode_u8==Dcm_CdtcsNoNrc_u8)
    {
        operationResult=Dcm_CdtcsProcessServiceResult(operationResult, dataNegRespCode_u8);
    }

    return operationResult;
}

static Std_ReturnType Dcm_CdtcsIsRequestValid (const Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType requestValid = E_NOT_OK;

    /*if ControlOptionRecord is enabled, other validation checks needs to be done*/
#if(DCM_CFG_DSP_SUPPORTCDTCRECORD == DCM_CFG_ON)
    requestValid=Dcm_CdtcsIsControlOptionRecordValid(pMsgContext, dataNegRespCode_u8);
#else
    if(pMsgContext->reqDataLen == Dcm_CdtcsExpectedRequestLength_u16)
    {
        requestValid = E_OK;
    }
    else
    {
        *dataNegRespCode_u8=DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
#endif

    return requestValid;
}

#if(DCM_CFG_DSP_SUPPORTCDTCRECORD == DCM_CFG_ON)
static Std_ReturnType Dcm_CdtcsIsControlOptionRecordValid (const Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType controlOptionRecordValid = E_NOT_OK;
    uint32 controlOptionRecord=0x0;

    if((pMsgContext->reqDataLen == Dcm_CdtcsExpectedTotalLength_u16))
    {
        /*calculate ControlOptionRecord from request message; Start with 2nd byte, since SettingType is stored in first byte*/
        controlOptionRecord =((((uint32) pMsgContext->reqData[DCM_PRV_CDTC_POSITION_SUBFUNC+1u]) << 16u) |
                (((uint32) pMsgContext->reqData[DCM_PRV_CDTC_POSITION_SUBFUNC+2u]) << 8u) |
                ((uint32) pMsgContext->reqData[DCM_PRV_CDTC_POSITION_SUBFUNC+3u]));

        /*DcmAppl handling implemented as before update to AR4.3.1 (including NRC handling)*/
        if(DcmAppl_DcmCheckCDTCRecord(&pMsgContext->reqData[1], DCM_PRV_CDTC_RECORD_LENGTH, dataNegRespCode_u8)==E_OK)
        {
            if(controlOptionRecord==Dcm_CdtcsValidRecord_u32)
            {
                controlOptionRecordValid=E_OK;
            }
            else
            {
                *dataNegRespCode_u8=DCM_E_REQUESTOUTOFRANGE;
            }
        }
        else
        {
            if(*dataNegRespCode_u8==Dcm_CdtcsNoNrc_u8)
            {
                *dataNegRespCode_u8=DCM_E_REQUESTOUTOFRANGE;
            }
        }
    }
    else
    {
        *dataNegRespCode_u8=DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }

    return controlOptionRecordValid;
}
#endif
/**
 **************************************************************************************************
 * Dcm_CDTCModeStatusCheck : The function will check whether the check in configured mode rule/DcmAppl API
 * passes if the control Dtc setting is disabled.If the mode rule fails, Dtc setting should be re-enabled..
 * The function has to be cyclically called from the main function.
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

static boolean Dcm_CdtcsIsModeStatusCheckPassed (void)
{
    boolean flgModeRetval_b;
    uint8 dataNegRespCode_u8;
    boolean modeStatusCheckPassed=TRUE;
    dataNegRespCode_u8 =Dcm_CdtcsNoNrc_u8;

    /* Control Dtc setting disabled?*/
    /*IfCDTC state is set to FALSE*/
    if((Dcm_DspCDTCStatus_b == FALSE) || Dcm_Cdtcs_IsDisableInProgress_b)
    {
        /* Call the mode function to check the mode rule status*/
        flgModeRetval_b=Dcm_ControlDtcSettingModecheck_b(&dataNegRespCode_u8);
        /* If the mode rule returns false, enable the Dtc setting in mainfunction*/
        if((flgModeRetval_b == FALSE) || (dataNegRespCode_u8 != 0u))
        {
            /*update setting type so DTC event have to be re-enabled  if mode checks are failing*/
            Dcm_Cdtcs_SettingType_u8=DCM_CDTCS_ON;
            modeStatusCheckPassed=FALSE;

        }

    }

    return modeStatusCheckPassed;

}

#if (DCM_CFG_RTESUPPORT_ENABLED == DCM_CFG_OFF)
/**
 **************************************************************************************************
 * Dcm_GetControlDTCSetting_EnableStatus : Function to get the status of control Dtc setting
 * Returns true, if Dtc setting is enabled and returns false if Dtc setting is disabled
 * Can be used by other SWCs and BSW modules in case RTE is not supported.
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
boolean Dcm_GetControlDTCSetting_EnableStatus(void)
{
    return (Dcm_DspCDTCStatus_b);
}
#endif

/**
 **************************************************************************************************
 * Dcm_ControlDtcSettingExit : The function has to be called when there is a session change.
 * If the control Dtc setting is disabled, the function will re-enable the same, if there
 * is a session change to DEFAULT session or to a session in which the Dtc setting is not supported
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

void Dcm_ControlDtcSettingExit(void)
{
    uint8 idxCdtc_u8;
    uint32 session_active_u32; /* Variable to store the requested session */
    uint32 allowed_session_u32; /* Variable to hold the current active session*/
    /* Get the index of the control Dtc setting service index from the service table*/
    idxCdtc_u8 = Dcm_Dsld_Conf_cs.sid_table_pcs[Dcm_DsldGlobal_st.datActiveSrvtable_u8].cdtc_index_u8;

    /* Get the allowed sessions for the control Dtc setting*/
    if(idxCdtc_u8 != 0xFFu)
    {
        allowed_session_u32=Dcm_Dsld_Conf_cs.sid_table_pcs[Dcm_DsldGlobal_st.datActiveSrvtable_u8].ptr_service_table_pcs[idxCdtc_u8].allowed_session_b32;
    }
    else
    {
        allowed_session_u32=0xffffffffu;
    }
    /* Get the current active session*/
    session_active_u32 = (uint32)(0x00000001uL << Dcm_DsldGlobal_st.idxActiveSession_u8);
    /* check whether the session is default, or the new session is allowed for control Dtc setting*/
    if((session_active_u32 == ((uint32)DCM_DEFAULT_SESSION))||((allowed_session_u32 & session_active_u32) != session_active_u32))
    {
        /*If CDTC state is set to FALSE*/
        if((Dcm_DspCDTCStatus_b == FALSE) || Dcm_Cdtcs_IsDisableInProgress_b)
        {
            Dcm_Cdtcs_SettingType_u8=DCM_CDTCS_ON;
            Dcm_Cdtcs_IsReEnablePending_b=TRUE;

        }

    }
}

/**
 **************************************************************************************************
 * Dcm_Dsp_CDTCSIni : Initialisation function for Control Diagnostic Trouble Code Setting Service
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

void Dcm_Dsp_CDTCSIni(void)
{
    /* Set CDTC state to TRUE */
    Dcm_DspCDTCStatus_b = TRUE;
}

void Dcm_Prv_Cdtcs_Mainfunction (void)
{
    uint8 dataNegRespCode_u8 =Dcm_CdtcsNoNrc_u8;
    boolean isModeCheckPassed=TRUE;

    isModeCheckPassed=Dcm_CdtcsIsModeStatusCheckPassed();

    if(Dcm_Cdtcs_ProceedPermissionHandling_b)
    {
        DcmAppl_DisableDTCSetting(&dataNegRespCode_u8);
        if(dataNegRespCode_u8==0x00)
        {
            Dcm_Cdtcs_IsReEnablePending_b=TRUE;
            Dcm_Cdtcs_ProceedPermissionHandling_b=FALSE;
        }
        if (dataNegRespCode_u8!=DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING)
        {
            Dcm_Cdtcs_ProceedPermissionHandling_b=FALSE;
        }
        else
        {
            /*Try to get permission in next cycle*/
        }
    }

    if(Dcm_Cdtcs_IsReEnablePending_b||(!isModeCheckPassed))
    {

        /*perform service and update pending flag if necessary (for next maincycle)*/
        if(DCM_E_PENDING == Dcm_CdtcsPerformService(&dataNegRespCode_u8))
        {
            Dcm_Cdtcs_IsReEnablePending_b=TRUE;
        }
        else
        {
            Dcm_Cdtcs_IsReEnablePending_b=FALSE;
        }
    }

}

static Std_ReturnType Dcm_CdtcsProcessServiceResult (Std_ReturnType Dem_Result, Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType processingResult = E_NOT_OK;

    switch(Dem_Result)
    {
        /*inform application in case of successfully enabling/disabling*/
        case E_OK:
        Dcm_CdtcsInformApplication();
        processingResult=E_OK;
        break;

        /*this case is only for invalid ClientId passed to Dem*/
        case E_NOT_OK:
        *dataNegRespCode_u8=DCM_E_REQUESTOUTOFRANGE;
        break;

        /*if Dem returns neither OK nor NOK, then we have pendig case*/
        default:
        processingResult=DCM_E_PENDING;
        break;
    }

    return processingResult;
}


static void Dcm_CdtcsInformApplication (void)
{
    if(Dcm_Cdtcs_SettingType_u8==DCM_CDTCS_OFF)
    {
#if (DCM_CFG_RTESUPPORT_ENABLED==DCM_CFG_ON)
        (void)SchM_Switch_Dcm_DcmControlDTCSetting(RTE_MODE_DcmControlDTCSetting_DISABLEDTCSETTING);
#endif
        Dcm_DspCDTCStatus_b=FALSE;
        Dcm_Cdtcs_IsDisableInProgress_b=FALSE;
        DcmAppl_DcmControlDtcSettingEnableStatus(Dcm_DspCDTCStatus_b);
    }
    else
    {
#if (DCM_CFG_RTESUPPORT_ENABLED==DCM_CFG_ON)
        (void)SchM_Switch_Dcm_DcmControlDTCSetting(RTE_MODE_DcmControlDTCSetting_ENABLEDTCSETTING);
#endif
        Dcm_DspCDTCStatus_b=TRUE;
        DcmAppl_DcmControlDtcSettingEnableStatus(Dcm_DspCDTCStatus_b);
    }
}



static Std_ReturnType Dcm_CdtcsProcessPermissionState (Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType permissionResult = E_NOT_OK;


    {
        if(Dcm_Cdtcs_SettingType_u8==DCM_CDTCS_OFF)
        {
            permissionResult=Dcm_CdtcsProcessDisablePermission(dataNegRespCode_u8);
        }

        /* for settingType ON, no permission check necessary */
        else
        {
            Dcm_SrvOpstatus_u8= DCM_PROCESSSERVICE;
            permissionResult = Dcm_CdtcsPerformService(dataNegRespCode_u8);
        }
    }


    return permissionResult;

}


static Std_ReturnType Dcm_CdtcsProcessDisablePermission (Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType permissionResult = E_NOT_OK;

    Dcm_Cdtcs_IsDisableInProgress_b=TRUE;
    /*If setting type is changed back to "ON", re-enable is in progress or already finished, so do NOT proceed with disabling*/
    if(Dcm_Cdtcs_SettingType_u8==DCM_CDTCS_OFF)
    {
        DcmAppl_DisableDTCSetting(dataNegRespCode_u8);
        if(*dataNegRespCode_u8==Dcm_CdtcsNoNrc_u8)
        {
            Dcm_Cdtcs_IsPermisionPending_b=FALSE;
            Dcm_SrvOpstatus_u8= DCM_PROCESSSERVICE;

            permissionResult=Dcm_CdtcsPerformService(dataNegRespCode_u8);

        }
        else if (*dataNegRespCode_u8==DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING)
        {
            Dcm_Cdtcs_IsPermisionPending_b=TRUE;
            Dcm_SrvOpstatus_u8= DCM_CHECKDATA;
            permissionResult = DCM_E_PENDING;
        }
        else
        {
            /* do nothing and return E_NOT_OK*/
        }
    }
    else
    {
        Dcm_Cdtcs_IsDisableInProgress_b=FALSE;
        *dataNegRespCode_u8=DCM_E_CONDITIONSNOTCORRECT;
    }

    return permissionResult;
}

static boolean Dcm_CdtcsProceedRequest(const Dcm_MsgContextType* pMsgContext)
{
    boolean proceedRequest = FALSE;
    uint8 RequestSettingType_u8= *(pMsgContext->reqData);


    if(RequestSettingType_u8==DCM_CDTCS_OFF)
    {
        Dcm_ControlDtcSettingExit();
        (void) Dcm_CdtcsIsModeStatusCheckPassed();
        /*If setting types do not match, there was an update in mainfunction (session change /mode conditions failing)
         * so do NOT PROCEED with current DISABLE request
         */
        if(Dcm_Cdtcs_SettingType_u8==DCM_CDTCS_OFF)
        {
            proceedRequest=TRUE;
        }
    }
    else
    {
        proceedRequest=TRUE;
    }

    return proceedRequest;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif /* (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF) */
