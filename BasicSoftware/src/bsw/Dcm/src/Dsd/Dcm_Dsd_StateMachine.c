
#include "Dcm_Cfg_Prot.h"
#include "Dcm_Dsd_Prv.h"
#include "Rte_Dcm.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
Dcm_SrvOpStatusType Dcm_SrvOpstatus_u8; /* Global Opstatus used by all services within Dcm*/
Dcm_OpStatusType    Dcm_ExtSrvOpStatus_u8; /* Dcm OpStatus, set by Dcm to be used by all services outside Dcm */
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
Dcm_SrvOpStatusType Dcm_OBDSrvOpstatus_u8;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_DsdStatesType_ten stDsdState_en; /* State variable of DSD */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"

/* DSD Sub-State machine definitions */

/* Service Table Initialization is done */
#define DSDSUBSTATE_SERVICETABLE_INI      (1u)
/* Manufacturer notification for Tester Requested service */
#define DSDSUBSTATE_MANUFACTURER_NOTIFICATION  (2u)
/* The request data is verified  */
#define DSDSUBSTATE_VERIFYDATA            (3u)
/* Supplier notification for Tester Requested service */
#define DSDSUBSTATE_SUPPLIER_NOTIFICATION (4u)
/* If Sub-function is valid , then sub-function related checks are done */
#define DSDSUBSTATE_SUBFUNCTION_CHECK     (5u)


#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/* Sub-State machine declared as static -
 *    -Should be initialized only once
 *    -Value should be retained in consecutive calls */
static uint8 stDsdSubState_u8 = DSDSUBSTATE_SERVICETABLE_INI;


/***********************************************************************************************************************
 Function name    : Dcm_Prv_ResetDsdSubStateMachine
 Syntax           : Dcm_Prv_ResetDsdSubStateMachine()
 Description      : Helper Function to Reset DSD SUB-StateMachine in cases where
                    execution is to be cancelled before the service is invoked
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
void Dcm_Prv_ResetDsdSubStateMachine(void)
{
    stDsdSubState_u8 = DSDSUBSTATE_SERVICETABLE_INI;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_IsVerifyDataProcessing
 Syntax           : Dcm_Prv_IsVerifyDataProcessing(void)
 Description      : Helper Function to check if DSD StateMachine is in Verify Data State
                    and whether Dsd SubState Machine is in IDLE state. This is done to check whether
					Service has not yet been triggered and Supplier Notification check is not yet called.
 Parameter        : void
 Return value     : boolean
***********************************************************************************************************************/
boolean Dcm_Prv_IsVerifyDataProcessing(void)
{
    boolean Status = FALSE;
    if((stDsdState_en == DSD_VERIFY_DIAGNOSTIC_DATA) && (stDsdSubState_u8 == DSDSUBSTATE_SERVICETABLE_INI))
    {
        Status = TRUE;
    }
    /* In case OBD request is running parallely , DSC request execution is delayed until P2 elapses
     * Then Busy repeat request NRC is sent and execution is stopped
     */
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    if((stDsdState_en == DSD_CALL_SERVICE) && (Dcm_DsldGlobal_st.dataSid_u8 == DCM_SID_DIAGNOSTICSESSIONCONTROL)
         && (Dcm_Prv_GetOBDState()!= DCM_OBD_IDLE ))
    {
        Status = TRUE;
    }
#endif
    return Status;
}

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdVerifyData_StateMachine
 Syntax           : Dcm_Prv_DsdVerifyData_StateMachine(void)
 Description      : SubState Machine for Verifying request Data
 Parameter        : None
 Return value     : Std_ReturnType
***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_DsdVerifyData_StateMachine(void)
{
    Std_ReturnType VerificationResult_u8 = E_OK;
    switch(stDsdSubState_u8)
    {
        /* Perform Service table initialization and invoke Manufacturer indication */
        case DSDSUBSTATE_SERVICETABLE_INI:
        {
            Dcm_Prv_DsdServiceTableInit();
#if (DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED != DCM_CFG_OFF)
                stDsdSubState_u8 = DSDSUBSTATE_MANUFACTURER_NOTIFICATION;
#else
                stDsdSubState_u8 = DSDSUBSTATE_VERIFYDATA;
#endif
        }
        /* MR12 RULE 16.3 VIOLATION: break statement intentionally not added for Execution to fall through */
        /* Invoke Manufacturer Notification only for Tester triggered requests */
#if(DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED != DCM_CFG_OFF)
        case DSDSUBSTATE_MANUFACTURER_NOTIFICATION:
        {
              VerificationResult_u8 = (Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest == DCM_UDS_TESTER_SOURCE)?  \
                                       Dcm_Prv_DsdNotification(DCM_MANUFACTURERNOTIFICATION,DCM_UDSCONTEXT):E_OK;
              if(VerificationResult_u8 != E_OK)
              {
               break;
              }
              else
              {
               stDsdSubState_u8 = DSDSUBSTATE_VERIFYDATA;
              }
        }
#endif
       /* MR12 RULE 16.3 VIOLATION: break statement intentionally not added for Execution to fall through */
        /* Verify the requested service data */
        case DSDSUBSTATE_VERIFYDATA:
        {
            if(Dcm_Prv_DsdVerifyData() != E_OK)
            {
                VerificationResult_u8 = E_NOT_OK;
                break;
            }
            else
            {
#if (DCM_CFG_SUPPLIER_NOTIFICATION_ENABLED != DCM_CFG_OFF)
                stDsdSubState_u8 = DSDSUBSTATE_SUPPLIER_NOTIFICATION;
#else
                stDsdSubState_u8 = DSDSUBSTATE_SUBFUNCTION_CHECK;
#endif
            }
        }
        /* MR12 RULE 16.3 VIOLATION: break statement intentionally not added her for Execution to fall through */
        /* Invoke Supplier notification for tester triggered requests */
#if (DCM_CFG_SUPPLIER_NOTIFICATION_ENABLED != DCM_CFG_OFF)
        case DSDSUBSTATE_SUPPLIER_NOTIFICATION:
        {
             VerificationResult_u8 = (Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest == DCM_UDS_TESTER_SOURCE)?   \
                                      Dcm_Prv_DsdNotification(DCM_SUPPLIERNOTIFICATION,DCM_UDSCONTEXT):E_OK;

             if(VerificationResult_u8 == E_OK)
             {
                 stDsdSubState_u8 = DSDSUBSTATE_SUBFUNCTION_CHECK;
             }
             else
             {
                 break;
             }
        }
#endif
        /* MR12 RULE 16.3 VIOLATION: break statement intentionally not added her for Execution to fall through */
        case DSDSUBSTATE_SUBFUNCTION_CHECK:
        {
            /* Perform SubFunction checks if configured */
            if(Dcm_Prv_IsSubFunctionSupported())
            {
                VerificationResult_u8 = Dcm_Prv_DsdCheckSubFunction();
            }
            else
            {
                Dcm_Prv_CheckSuppressPositiveResponseforRC();
            }
        }
        break;
         /* Should never reach here */
         default:
         {
             VerificationResult_u8 = E_NOT_OK;
         }
         break;
    }
    /* If Pending is returned , only then continue in the same state */
    if(VerificationResult_u8 != DCM_E_PENDING)
    {
        stDsdSubState_u8 = DSDSUBSTATE_SERVICETABLE_INI;
    }
    return VerificationResult_u8;
}

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdStateMachine
 Syntax           : Dcm_Prv_DsdStateMachine(void)
 Description      : State Machine for Diagnostic Service Dispatcher(DSD). Invoked in every Dcm Main Function call
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
void Dcm_Prv_DsdStateMachine(void)
{
    switch(stDsdState_en)
    {
        /* Verification of a new request is done by DSL
         * DSD is updated to this state for further checks */
        case DSD_VERIFY_DIAGNOSTIC_DATA:
            if(Dcm_Prv_DsdVerifyData_StateMachine() != E_OK)
            {
                break;
            }
            else
            {
                Dcm_Prv_SetDsdState(DSD_CALL_SERVICE);
            }

        /*MR12 RULE 16.3 VIOLATION: break statement intentionally not added for Execution to fall through */
        case DSD_CALL_SERVICE:
            /* After Verification is done the relevant service interpreter is called here */
            Dcm_Prv_DsdProcessService();
            break;

        /* Transmission is triggered by Dcm
         * Continue in this state until confirmation is obtained or transmission is cancelled */
        case DSD_WAITFORTXCONF:
            break;

        /* Confirmation obtained from lower layers
         * Inform application callbacks regarding the same  */
        case DSD_SENDTXCONF_APPL:
            Dcm_Prv_DsdSendTxConfirmation();
            break;

        default:
            /* For DSD_IDLE State */
            break;
    }
}

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
