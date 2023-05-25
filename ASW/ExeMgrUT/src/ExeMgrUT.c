/* *****************************************************************************
 * BEGIN: Banner
 *-----------------------------------------------------------------------------
 *                                 ETAS GmbH
 *                      D-70469 Stuttgart, Borsigstr. 14
 *-----------------------------------------------------------------------------
 *    Administrative Information (automatically filled in by ISOLAR)         
 *-----------------------------------------------------------------------------
 * Name: 
 * Description:
 * Version: 1.0
 *-----------------------------------------------------------------------------
 * END: Banner
 ******************************************************************************

 * Project : ETAS_BIP_PT
 * Component: /ServiceLayer/ExeMgrUT
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.1
 * Author: TAN9SGH
 * Date : Mon Aug 23 15:37:08 2021
 ****************************************************************************/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Rte_ExeMgrUT.h"
#include "ExeMgrUT.h"
#include "BswM_Integration.h"
#include "rba_BswSrv.h"
// #include "NmUT.h"

/*
 **********************************************************************************************************************
 * Define
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define ExeMgrUT_START_SEC_VAR_INIT_8
#include "ExeMgrUT_MemMap.h"
Testing_t StateMachine_Test = TESTING_NONE;
ExeMgr_StartupSourceType WakeupSrc = ~STARTUP_SOURCE_NONE;
uint8 AppMode_Test = RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_APP_RUN;	
#define ExeMgrUT_STOP_SEC_VAR_INIT_8
#include "ExeMgrUT_MemMap.h"

#define ExeMgrUT_START_SEC_VAR_CLEARED_8
#include "ExeMgrUT_MemMap.h"
ShutdownTarget_type LastShutdownTarget_rtn;
ShutdownTarget_type ShutdownTarget_rtn;
ShutdownCause_type ShutdownCause_rtn; 
#define ExeMgrUT_STOP_SEC_VAR_CLEARED_8
#include "ExeMgrUT_MemMap.h"

#define ExeMgrUT_START_SEC_VAR_CLEARED_16
#include "ExeMgrUT_MemMap.h"
EcuM_ShutdownModeType LastShutdownMode_rtn;	
EcuM_ShutdownModeType ShutdownMode_rtn;	                                                                                                                       
#define ExeMgrUT_STOP_SEC_VAR_CLEARED_16
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_MainFunction
  Description       :   Execution manager processes AppMode request and wakeup signals.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks: This function is scheduled by OS periodically
***********************************************************************************/
FUNC (void, ExeMgrUT_CODE) RE_ExeMgr_MainFunction(void)
{
    Std_ReturnType syncCall1;
    /* Trigger validation of current wakeup source again. */
    Rte_IrTrigger_RE_ExeMgr_GetWakeupSource_ITP_GetWakeupSource();
    syncCall1 = Rte_IrvIRead_RE_ExeMgr_EvaluateWakeup_ExeMgr_StartupSource();
    RE_ExeMgr_EvaluateWakeup();

    switch (StateMachine_Test)
    {
        case APP_MODE_REQUEST_APP_RUN:
            Rte_Switch_PP_BswM_APPMode_MDGP_APPMode(RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_APP_RUN);
            StateMachine_Test = TESTING_NONE;
            break;
        case APP_MODE_REQUEST_POST_RUN:
            Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownTarget(ECUM_SHUTDOWN_TARGET_OFF, ECUM_RESET_MCU);
            Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownCause(ECUM_CAUSE_ECU_STATE);
            Rte_Switch_PP_BswM_APPMode_MDGP_APPMode(RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_POST_RUN);
            StateMachine_Test = TESTING_NONE;
            break;
        case LAST_SHUTDOWN_TARGET_INTERFACE:
            Rte_Call_RP_EcuM_SelectResetTarget_GetLastShutdownTarget(&LastShutdownTarget_rtn,&LastShutdownMode_rtn);
            StateMachine_Test = TESTING_NONE;
            break;
        case SHUTDOWN_TARGET_INTERFACE:
            Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownTarget(ECUM_SHUTDOWN_TARGET_RESET,ECUM_RESET_IO);
            Rte_Call_RP_EcuM_SelectResetTarget_GetShutdownTarget(&ShutdownTarget_rtn,&ShutdownMode_rtn);
            StateMachine_Test = TESTING_NONE;
            break;
        case SHUTDOWN_CAUSE_INTERFACE:
            Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownCause(ECUM_CAUSE_WDGM);
            Rte_Call_RP_EcuM_SelectResetTarget_GetShutdownCause(&ShutdownCause_rtn);
            StateMachine_Test = TESTING_NONE;

        default:
        /* Not a valid request, ignore without do anything */
        break;
    }
}
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_GetWakeupSource
  Description       :   This function get current wakeup source values for validation.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks: This function read project specific hardware wakeup sources synchronously.
***********************************************************************************/
FUNC (void, ExeMgrUT_CODE) RE_ExeMgr_GetWakeupSource(void)
{
           								
    /**********************************************************************************
      Startup source bit field:
      bit7  |  bit6 |  bit5 |  bit4  |  bit3  |  bit2  |  bit1  |  bit0
    ***********************************************************************************/

    /*test code to overwrite default startup source value*/  
    switch (StateMachine_Test)
    {
        case INVALID_WAKEUP_SOURCE:
            WakeupSrc = StateMachine_Test;
            break;
        case VALID_WAKEUP_SOURCE:
            WakeupSrc = StateMachine_Test;
            break;
        default:
            break;
    }

    /* Provide project specific hardware wakeup sources. */
    Rte_IrvIWrite_RE_ExeMgr_GetWakeupSource_ExeMgr_StartupSource(WakeupSrc);
}
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_ValidateWakeup
  Description       :   This function confirms wakeup source validation during startup.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks: This function is called by EcuM callout EcuM_CheckValidation on Master Core.
***********************************************************************************/
FUNC (Std_ReturnType, ExeMgrUT_CODE) RE_ExeMgr_ValidateWakeup(void)
{
    VAR(Std_ReturnType, AUTOMATIC) return_value = E_NOT_OK;

    /* Trigger validation of current wakeup source again. */
    Rte_IrTrigger_RE_ExeMgr_GetWakeupSource_ITP_GetWakeupSource();

    if(STARTUP_SOURCE_NONE != Rte_IrvIRead_RE_ExeMgr_EvaluateWakeup_ExeMgr_StartupSource())
    {
        /* The confirmed Wakeup Events will be reported to EcuM_ValidateWakeupEvent. */
        return_value = E_OK;
    }
    else
    {
        /* The pending Wakeup Events becomes invalid. */        
        return_value = E_NOT_OK;
    }
    
    return return_value;
}
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_EvaluateWakeup
  Description       :   This function processes wakeup source evaluation during startup.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks           :   This function is called only once by Startup Task after initialization.
***********************************************************************************/
FUNC (void, ExeMgrUT_CODE) RE_ExeMgr_EvaluateWakeup(void)
{
    /* Trigger validation of current wakeup source at startup. */
    Rte_IrTrigger_RE_ExeMgr_GetWakeupSource_ITP_GetWakeupSource();

    /* The pending Wakeup Events will be reported to EcuM by EcuM_SetWakeupEvent immediately after StartOS. */    
    if(STARTUP_SOURCE_NONE != Rte_IrvIRead_RE_ExeMgr_EvaluateWakeup_ExeMgr_StartupSource())
    {
        EcuM_SetWakeupEvent(EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_POWER);
        EcuM_ValidateWakeupEvent(EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_POWER);
    }
    else
    {
        EcuM_ClearWakeupEvent(EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_POWER);
    }

}
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_ExecuteShutdown
  Description       :   This function shutoff power supply to external circuits.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks           :   This function is called only once at EcuM OnGoOffTwo phase.
***********************************************************************************/
FUNC (void, ExeMgrUT_CODE) RE_ExeMgr_ExecuteShutdown(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.4	EcuM Shutdown
     * User Integration callback for actions right before shutdown where NvM saved.
     * Please see the integration guide section for further details.
     */
}
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_ExecuteReset
  Description       :   This function manages reset request and perform reset when
                        condition fulfills.
  Parameter (in)    :   resetReason
                        resetGroup
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks:
***********************************************************************************/
FUNC (void , ExeMgrUT_CODE) RE_ExeMgr_ExecuteReset
(
        VAR(EcuM_ShutdownCauseType, AUTOMATIC) resetReason,
        VAR(EcuM_ShutdownModeType, AUTOMATIC) resetGroup
)
{
    Std_ReturnType syncCall1;
    Std_ReturnType syncCall2;

    syncCall1 = Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownTarget(ECUM_SHUTDOWN_TARGET_RESET, resetGroup);
    syncCall2 = Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownCause(resetReason);

    if(((syncCall1) == E_OK) && ((syncCall2) == E_OK))  /* Configured known reset group and reset reason. */
    {
        Rte_Switch_PP_BswM_APPMode_MDGP_APPMode(RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_POST_RUN);
    }
    else
    {
        /* Do nothing if either EcuM not initialized or any of ResetGroup, ResetReason unknow. */
    }
}
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_EvaluateReset
  Description       :   This function processes reset evaluation during startup.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks: This function is called only once by Startup Task after initialization.
***********************************************************************************/
FUNC (void, ExeMgrUT_CODE) RE_ExeMgr_EvaluateReset(void)
{
    EcuM_ShutdownTargetType shutdownTarget;
    EcuM_ShutdownModeType shutdownMode;
    Std_ReturnType syncCall;

    syncCall = Rte_Call_RP_EcuM_SelectResetTarget_GetLastShutdownTarget(&shutdownTarget,&shutdownMode);

    /**
     * @brief ETAS BIP Integration Guide: 5.3.1	Mode Handling
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     */
}
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_ResetInfo
  Description       :   This function extracts latest non-hw triggered reset information.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   resetReason, reason of current requested shutdown cause
  Parameter (out)   :   resetGroup, reason of previous requested reset mode before last reset
  Return value      :   None
  Remarks: This function is intended to be called by user application as a service operation.
***********************************************************************************/
FUNC (void , ExeMgrUT_CODE) RE_ExeMgr_ResetInfo
(
        CONSTP2VAR(EcuM_ShutdownCauseType, AUTOMATIC, RTE_APPL_DATA) resetReason,
        CONSTP2VAR(EcuM_ShutdownModeType, AUTOMATIC, RTE_APPL_DATA) resetGroup
)
{
    Std_ReturnType syncCall1;
    Std_ReturnType syncCall2;
    EcuM_ShutdownTargetType shutdownTarget;
    EcuM_ShutdownModeType   shutdownMode;
    EcuM_ShutdownCauseType  shutdownCause;

    syncCall1 = Rte_Call_RP_EcuM_SelectResetTarget_GetLastShutdownTarget(&shutdownTarget,&shutdownMode);
    /* Note: GetShutdownCause returns current requested shutdown cause rather than previous shutdown
     * cause before reset. Therefore, it is not intended to retrieve previous detailed reset cause. */
    syncCall2 = Rte_Call_RP_EcuM_SelectResetTarget_GetShutdownCause(&shutdownCause);

    if(((syncCall1) == E_OK) && ((syncCall2) == E_OK))
    {
        *resetGroup  = shutdownMode;
        *resetReason = shutdownCause;
    }
    else
    {
        /* Return a default unknown source if either EcuM not initialized or NvM not read out or
         * any of ResetTarget, ResetGroup, ResetReason unknow. */
        *resetReason = 0xFF;
        *resetGroup  = 0xFF;
    }
}
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_StartupSource
  Description       :   This function extracts latest available startup source.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks: This function is intended to be called by user application as a service operation.
***********************************************************************************/
FUNC (void , ExeMgrUT_CODE) RE_ExeMgr_StartupSource
(
        CONSTP2VAR(ExeMgr_StartupSourceType, AUTOMATIC, RTE_APPL_DATA) StartupSrc
)
{
    *StartupSrc = Rte_IrvIRead_RE_ExeMgr_StartupSource_ExeMgr_StartupSource();
}
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#define ExeMgrUT_START_SEC_CODE
#include "ExeMgrUT_MemMap.h"
/**********************************************************************************
  Function name     :   RE_ExeMgr_TriggerBswBg
  Description       :   This function is used for user background job with low priority.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks: This function is run in 1ms bsw task.
***********************************************************************************/
FUNC (void, ExeMgrUT_CODE) RE_ExeMgr_TriggerBswBg(void)
{
    Rte_Enter_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA();

    TaskStateType stTaskState;

	/* Get Task State of background task.*/
	(void)GetTaskState(OsTask_SysCore_BswBg_1ms, &stTaskState);

	if(stTaskState == SUSPENDED)
	{
		/* If back ground task terminate, just activate the background task. */
		(void)ActivateTask(OsTask_SysCore_BswBg_1ms);
	}
	else
	{
		/* Do nothing, expect WdgM will detect this fault of OsTask_SysCore_BswBg_1ms. */
	}


    Rte_Exit_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA();
}
#define ExeMgrUT_STOP_SEC_CODE
#include "ExeMgrUT_MemMap.h"


/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
