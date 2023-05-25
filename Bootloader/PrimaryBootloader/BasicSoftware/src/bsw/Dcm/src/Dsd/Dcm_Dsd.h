

#ifndef DCM_DSD_H

#define DCM_DSD_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "ComStack_Types.h"
#include "Dcm_Types.h"
#include "Dcm_Cfg_DslDsd.h"
/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/* DSD State Machine */
typedef enum
{
    DSD_IDLE,                     /* Default state of DSD state machine */
    DSD_VERIFY_DIAGNOSTIC_DATA,   /* State does the partial verification of Diagnostic data */
    DSD_CALL_SERVICE,             /* Call of the service  */
    DSD_WAITFORTXCONF,            /* Response is Transmitted , waiting for Confirmation */
    DSD_SENDTXCONF_APPL           /* Invoke confirmation api's once confirmation from lower layer is obtained */
}Dcm_DsdStatesType_ten;

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
extern Dcm_DsdStatesType_ten stDsdState_en; /* State variable of DSD */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

extern void Dcm_Prv_DsdStateMachine(void);
extern void Dcm_Dsd_ServiceIni(uint8 ServiceTableIndex_u8);
extern void Dcm_Prv_DsdSendTxConfirmation(void);
#if ((DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS != 0u) || (DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS !=0u))
extern void Dcm_Dsd_CallRTEConfirmation(Dcm_ConfirmationStatusType ConfirmationStatus_u8,boolean context);
#endif
extern void Dcm_Prv_ResetDsdSubStateMachine(void);
extern boolean Dcm_Prv_IsVerifyDataProcessing(void);

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline Functions
 **********************************************************************************************************************
*/

/***********************************************************************************************************************
 Function name    : Dcm_Prv_GetDsdState
 Syntax           : Dcm_Prv_GetDsdState()
 Description      : Helper Function to Read the State of DSD StateMachine
 Parameter        : None
 Return value     : Dcm_DsdStatesType_ten
***********************************************************************************************************************/
LOCAL_INLINE Dcm_DsdStatesType_ten Dcm_Prv_GetDsdState(void)
{
    return stDsdState_en;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_SetDsdState
 Syntax           : Dcm_Prv_SetDsdState(State)
 Description      : Helper function to write into DSD StateMachine state
 Parameter        : Dcm_DsdStatesType_ten
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_SetDsdState(Dcm_DsdStatesType_ten State)
{
    stDsdState_en = State;
}

#endif /* DCM_DSD_H */
