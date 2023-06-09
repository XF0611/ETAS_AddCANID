

#ifndef WDGM_PRV_H
#define WDGM_PRV_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
/* MR12 RULE 20.5 VIOLATION: Macro WDGM_PRV_AVOID_RTE_PROVIDED_FUNC_DECLARATION is undefined to access WdgM_MainFunction, WdgM_GetLocalStatus, WdgM_GetGlobalStatus, WdgM_GetMode, WdgM_PerformReset, WdgM_SetMode, WdgM_GetFirstExpiredSEID, and WdgM_CheckpointReached Api's for dependency module, thus the warning canbe ignored */
#define WDGM_PRV_AVOID_RTE_PROVIDED_FUNC_DECLARATION
#include "WdgM.h"
#undef WDGM_PRV_AVOID_RTE_PROVIDED_FUNC_DECLARATION
#if((WDGM_RTE_DISABLED) == (STD_OFF))
#include "SchM_WdgM.h"
#include "WdgM_Cfg_SchM.h" /* TODO: To be removed when RTE supports Exclusive Area. */
#include "Rte_WdgM.h"
#else
#include "WdgM_Cfg_SchM.h" /* It is mandatory if RTE is disabled. */
#endif

#include "WdgIf.h"
/*
#if (!defined(WDGIF_AR_RELEASE_MAJOR_VERSION) || (WDGIF_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(WDGIF_AR_RELEASE_MINOR_VERSION) || (WDGIF_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/

#if (((WDGM_IMMEDIATE_RESET) == (STD_ON)) || ((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_MCU)))
#include "Mcu.h"
/*
#if (!defined(MCU_AR_RELEASE_MAJOR_VERSION) || (MCU_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(MCU_AR_RELEASE_MINOR_VERSION) || (MCU_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/
#endif


#if((WDGM_PARTITION_ENABLED) == (STD_ON))
#include "BswM_WdgM.h"
/*
#if (!defined(BSWM_AR_RELEASE_MAJOR_VERSION) || (BSWM_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(BSWM_AR_RELEASE_MINOR_VERSION) || (BSWM_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/
#endif

#include "Os.h" /* specified in SWS. required for GetElapsedCounterValue Api */
/*
#if (!defined(OS_AR_RELEASE_MAJOR_VERSION) || (OS_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(OS_AR_RELEASE_MINOR_VERSION) || (OS_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/

/*
 *********************************************************************************************************
 * Defines/Macros
 *********************************************************************************************************
*/

/* For debugging */
#if ((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
#define WDGM_RB_FAILED_IN_ALIVE_MAINFUNCTION                            ((uint8)(1))
#define WDGM_RB_FAILED_IN_DEADLINE_MAINFUNCTION                         ((uint8)(2))
#define WDGM_RB_FAILED_IN_DEADLINE_CHECKPOINTREACHED                    ((uint8)(3))
#define WDGM_RB_FAILED_IN_LOGICAL_INTGRAPH_CPREACHED_GRAPHNOTSTARTED    ((uint8)(4))
#define WDGM_RB_FAILED_IN_LOGICAL_INTGRAPH_CPREACHED_GRAPHSTARTED       ((uint8)(5))
#define WDGM_RB_FAILED_IN_LOGICAL_EXTGRAPH_CPREACHED_GRAPHNOTSTARTED    ((uint8)(6))
#define WDGM_RB_FAILED_IN_LOGICAL_EXTGRAPH_CPREACHED_GRAPHSTARTED       ((uint8)(7))

typedef struct
{

    TickType FirstElapsedCounterValue;
    uint16 FirstExpiredCheckpointId_t;
    uint16 FirstExpiredAdditionalCheckpointId;
    WdgM_SupervisedEntityIdType FirstExpiredSupervisedEntityId_t;
    WdgM_SupervisedEntityIdType FirstExpiredAdditionalSupervisedEntityId;
    uint8 FirstExpiredLocation_t;
    WdgM_ModeType FirstExpiredModeId;
}WdgM_Rb_FirstExpiredEntityInfo_tst;

#endif /*WDGM_RB_DEBUG_OPTION_ENABLED*/

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define WDGM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

extern const WdgM_ConfigType * WdgM_ConfigSetPtr;

#define WDGM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "WdgM_MemMap.h"

extern boolean WdgM_AliveSupervisionActive;

#define WDGM_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_FAST_CLEARED_8
#include "WdgM_MemMap.h"

extern uint8 WdgM_Mode;

#define WDGM_STOP_SEC_VAR_FAST_CLEARED_8
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_CLEARED_16
#include "WdgM_MemMap.h"

extern uint16 WdgM_ExpiredSupervisionCycleCtr;

#define WDGM_STOP_SEC_VAR_CLEARED_16
#include "WdgM_MemMap.h"

#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
#define WDGM_START_SEC_VAR_CLEARED_32
#include "WdgM_MemMap.h"

extern uint32 WdgM_MainFunction_Cnt_u32;  /* This variable is used only for debug purpose. */

#define WDGM_STOP_SEC_VAR_CLEARED_32
#include "WdgM_MemMap.h"
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/

#define WDGM_START_SEC_VAR_SLOW_INIT_BOOLEAN
#include "WdgM_MemMap.h"

extern boolean WdgM_Prv_FirstDeactivation_b;

#define WDGM_STOP_SEC_VAR_SLOW_INIT_BOOLEAN
#include "WdgM_MemMap.h"

#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
#define WDGM_START_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

extern WdgM_Rb_FirstExpiredEntityInfo_tst WdgM_Rb_FirstExpiredEntityInfo_st;

#define WDGM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
#endif /*(WDGM_RB_DEBUG_OPTION_ENABLED == STD_ON)*/

#define WDGM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

/* TRACE[WDGM349] Implementation of non-initialized RAM location, stored as a double-inverse value */
extern WdgM_SupervisedEntityIdType WdgM_FirstExpiredSupervisedEntityId;
extern WdgM_SupervisedEntityIdType WdgM_FirstExpiredSupervisedEntityId_Comp;

#define WDGM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
#define WDGM_START_SEC_VAR_FAST_POWER_ON_CLEARED_8
#include "WdgM_MemMap.h"

extern uint8 WdgM_Rb_FirstExpiredEntity_Cnt_u8;

#define WDGM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_8
#include "WdgM_MemMap.h"
#endif /*(WDGM_RB_DEBUG_OPTION_ENABLED == STD_ON)*/


#define WDGM_START_SEC_VAR_CLEARED_8
#include "WdgM_MemMap.h"

#if((WDGM_RTE_DISABLED) == (STD_OFF))
extern Std_ReturnType WdgM_Prv_Rte_Switch_globalMode_Status;
#endif /*(WDGM_RTE_DISABLED == STD_OFF)*/

#define WDGM_STOP_SEC_VAR_CLEARED_8
#include "WdgM_MemMap.h"
/*
 ***********************************************************************************************************************
 * Private Function Declaration
 ***********************************************************************************************************************
*/

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"

extern void WdgM_Prv_SetTriggerCondition(WdgM_GlobalStatusType globalStatus_tu8);
extern void WdgM_Prv_LocalStatusMainFunction(void);

#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
#define WDGM_START_SEC_CODE_FAST
#include "WdgM_MemMap.h"

extern void WdgM_Rb_FirstExpiredEntity(TickType ElapsedCounterValue, uint16 CheckpointID,
        uint16 AdditionalCheckpointID, WdgM_SupervisedEntityIdType SEID, WdgM_SupervisedEntityIdType AdditionalSEID,
        uint8 ExpiredLocation, WdgM_ModeType ModeId);

#define WDGM_STOP_SEC_CODE_FAST
#include "WdgM_MemMap.h"
#endif/*(WDGM_RB_DEBUG_OPTION_ENABLED == STD_ON)*/

LOCAL_INLINE void WdgM_Prv_AliveSupervisionMainFunction_v_Inl(void);
LOCAL_INLINE void WdgM_Prv_UpdateAliveSupervisionDyn_v_Inl(void);
LOCAL_INLINE void WdgM_Prv_UpdateTriggerDyn_v_Inl(WdgM_ModeType Mode, Std_ReturnType * ReturnStatus);
LOCAL_INLINE void WdgM_Prv_UpdateSupervisedEntityDyn_v_Inl(WdgM_ModeType Mode);

#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
LOCAL_INLINE void WdgM_Rb_DebugVariables_Init_v_Inl(void);
#endif/*(WDGM_RB_DEBUG_OPTION_ENABLED == STD_ON)*/

#if((WDGM_DEADLINE_TIMEOUT_DETECT == STD_ON) && ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO)))
LOCAL_INLINE void WdgM_Prv_DeadlineSupervisionMainFunction_v_Inl(void);
#endif

#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
LOCAL_INLINE void WdgM_Prv_DeadlineSupervisionCheckpointReached_v_Inl(WdgM_SupervisedEntityIdType SEID,
WdgM_CheckpointIdType CheckpointID, Std_ReturnType * ReturnStatus);
LOCAL_INLINE void WdgM_Prv_UpdateDeadlineSupervisionDyn_v_Inl(WdgM_ModeType Mode);
#endif

#if (((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO)) ||(WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON))
LOCAL_INLINE void WdgM_Prv_UpdateSupervisionDyn_v_Inl(WdgM_ModeType Mode);
#endif

#if (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON)
LOCAL_INLINE Std_ReturnType WdgM_Prv_LogicalSupervisionCheckpointReached_v_Inl(WdgM_SupervisedEntityIdType SEID,
WdgM_CheckpointIdType CheckpointID);
#endif

#if (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
LOCAL_INLINE Std_ReturnType WdgM_Prv_ExternalGraphCheckpointReached_v_Inl(WdgM_SupervisedEntityIdType SEID,
WdgM_CheckpointIdType CheckpointID);
LOCAL_INLINE void WdgM_Prv_UpdateExternalGraphDyn_v_Inl(void);
#endif
/*
 ************************************************************************************************************************
 * Inline Function Definitions
 ************************************************************************************************************************
 */
#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
/************************************************************************************************************************
 * Name              :   WdgM_Rb_DebugVariables_Init_v_Inl
 * Description       :   Initialises the debug Variables
 * Parameters[in]    :   None
 * Limitations       :   None
 * Return Value      :   None
 ************************************************************************************************************************/
LOCAL_INLINE void WdgM_Rb_DebugVariables_Init_v_Inl(void)
{
    WdgM_Rb_FirstExpiredEntityInfo_st.FirstElapsedCounterValue                    = WDGM_PRV_C_ZERO;
    WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredCheckpointId_t                  = WDGM_RB_MAX_CHECKPOINTID;
    WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredAdditionalCheckpointId          = WDGM_RB_MAX_CHECKPOINTID;
    WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredSupervisedEntityId_t            = WDGM_RB_MAX_SUPERVISEDENTITYID;
    WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredAdditionalSupervisedEntityId    = WDGM_RB_MAX_SUPERVISEDENTITYID;
    WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredLocation_t                      = WDGM_RB_DEFAULT_EXPIREDLOCATION;
    WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredModeId                          = WDGM_RB_MAX_MODEID;

      WdgM_Rb_FirstExpiredEntity_Cnt_u8 = WDGM_PRV_C_ONE;
}
#endif/*(WDGM_RB_DEBUG_OPTION_ENABLED == STD_ON)*/

/************************************************************************************************************************
 * Name              :   WdgM_Prv_AliveSupervisionMainFunction_v_Inl
 * Description       :   Update the Alive Supervision in WdgM_MainFunction
 * Parameters[in]    :   None
 * Limitations       :   None
 * Return Value      :   None
 ************************************************************************************************************************/
LOCAL_INLINE void WdgM_Prv_AliveSupervisionMainFunction_v_Inl(void)
{
    WdgM_SupervisedEntityIdType SEID;
    uint16 AliveSupervisionIdx;
    const WdgM_AliveSupervisionType * AliveSupervisionPtr;
    uint32 IndividualAliveUpdateCtrCache = WDGM_PRV_C_ZERO;
    AliveSupervisionPtr = WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToAliveSupervision;

    for(AliveSupervisionIdx= WDGM_PRV_C_ZERO;
    AliveSupervisionIdx < WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].NoOfAliveSupervision;
    AliveSupervisionIdx++)
    {
        SEID = AliveSupervisionPtr[AliveSupervisionIdx].SupervisedEntityId;
        /* TRACE[WDGM208] If SE was in local state deactivated then leave those SE
         * in that state and start processing of supervision for other SE's  */
        /* TRACE[WDGM083] Check for Alive Supervison config, If NO then do not
         * perform Alive counter examination*/
        if(((WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & WDGM_LOCAL_STATUS_DEACTIVATED)!= WDGM_LOCAL_STATUS_DEACTIVATED)&&
                (WdgM_AliveSupervisionActive == TRUE))
        {
            WdgM_SupervisedEntityDyn[SEID].IndividualSupervisionCycleCtr++;
            /*TRACE[WDGM098] Perform checking only at the configured Supervision Reference Cycle*/
            if (WdgM_SupervisedEntityDyn[SEID].IndividualSupervisionCycleCtr ==
                    AliveSupervisionPtr[AliveSupervisionIdx].SupervisionReferenceCycle)
            {
                /*Interrupt lock to cache IndividualAliveUpdateCtr for calculation. Reset alive Counter here*/
                /*Interrupt Lock:
                 * 1. Cache and set to 0 should be an atomic operation so as to capture new CP.
                 * If no Lock then after caching we can get CPR and the counter is incremented coming back to mainFunction
                 * we will set the IndividualAliveUpdateCtr to 0. Hence One count will be lost.
                 * */
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                IndividualAliveUpdateCtrCache = WdgM_SupervisedEntityDyn[SEID].IndividualAliveUpdateCtr;
                WdgM_SupervisedEntityDyn[SEID].IndividualAliveUpdateCtr = WDGM_PRV_C_ZERO;
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                /*[WDGM074] Check if AliveCounter is within limit*/
                /*Check for if Alive counter is within the range hence "correct" state*/
                if (((IndividualAliveUpdateCtrCache )>=
                ((uint32)AliveSupervisionPtr[AliveSupervisionIdx].ExpectedAliveIndications - (uint32)AliveSupervisionPtr[AliveSupervisionIdx].MinMargin))&&
                ((IndividualAliveUpdateCtrCache) <=
                ((uint32)AliveSupervisionPtr[AliveSupervisionIdx].ExpectedAliveIndications + (uint32)AliveSupervisionPtr[AliveSupervisionIdx].MaxMargin)))
                {
                    /* TRACE[WDGM300] If SE is in LS status FAILED and results of
                     * AS are correct  and the counter for failed supervision reference cycle is >1 */
                    if(WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleCtr > WDGM_PRV_C_ONE)
                    {
                        /*Lock Enabled: Lock required because when preempted by CRP then the newly modified value will be replaced by MainFunction here*/
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                        SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                        WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_FAILED;
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                        SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                        WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleCtr--;
                    }
                    /* TRACE[WDGM205] If SE is in LS status FAILED and results of AS
                     * are correct and the counter for failed supervision
                     * reference cycle is equal 1 */
                    else if(WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleCtr == WDGM_PRV_C_ONE)
                    {
                        /*Lock Enabled: Lock required because when preempted by CRP then the newly modified value will be replaced by MainFunction here*/
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                        SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                        WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & ((WdgM_LocalStatusType)(~WDGM_LOCAL_STATUS_FAILED));
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                        SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                        WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleCtr = WDGM_PRV_C_ZERO;
                    }
                    else
                    {
                        /*Lock Enabled: Lock required because when preempted by CRP then the newly modified value will be replaced by MainFunction here*/
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                        SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                        WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & ((WdgM_LocalStatusType)(~WDGM_LOCAL_STATUS_FAILED));
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                        SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                    }
                }
                /*[WDGM115] If alive indiaction not within tolerance the incorrect*/
                /*Else since Alive counter is  not within the range hence "incorrect" state*/
                else
                {   /* TRACE[WDGM202] If SE was in LS status OK and results of
                     * atleast one AS is incorrect and the fault tolerence == 0*/
                    /* TRACE[WDGM203] If SE was in LS status OK and results of
                     * atleast one AS is incorrect and the failure fault tolerence > 0*/
                    /* TRACE[WDGM204] If SE was in LS status FAILURE and results of
                    * atleast one AS is incorrect and the failure supervision refrence cycle < tolerence*/
                    /*Lock Enabled: Lock required because when preempted by CRP then the newly modified value will be replaced by MainFunction here*/
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                    SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                    WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_FAILED;
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                    SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                    /* TRACE[WDGM206] If SE was in LS status FAILED and results of
                     * atleast one AS is incorrect and the counter for failed supervision reference cycle == Ref Cycle*/
                    if (WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleCtr == WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleTol)
                    {
                        /*Lock Enabled: Lock required because when preempted by CRP then the newly modified value will be replaced by MainFunction here*/
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                        SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
                        WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_EXPIRED;
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                        SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                        WdgM_Rb_FirstExpiredEntity(WDGM_PRV_C_ZERO, (uint16)AliveSupervisionPtr[AliveSupervisionIdx].AliveSupervisionCheckpointId,
                                                   WDGM_RB_MAX_CHECKPOINTID, SEID, WDGM_RB_MAX_SUPERVISEDENTITYID, WDGM_RB_FAILED_IN_ALIVE_MAINFUNCTION, WdgM_Mode);
#endif/*(WDGM_RB_DEBUG_OPTION_ENABLED == STD_ON)*/
                    }
                    else
                    {
                        WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleCtr++;
                    }
                }
                WdgM_SupervisedEntityDyn[SEID].IndividualSupervisionCycleCtr = WDGM_PRV_C_ZERO;
            }
        }
        else
        {
            /* This is required to do the syncronization of different Alive Supervisions after Mode change happens
               When mode change occurs the counters have to be reset to 0 */

            /*Lock Placed:
              Any modification operation(Eg:incrementing the counter value) on WdgM_SupervisedEntityDyn[SEID].IndividualAliveUpdateCtr
              variable is done in atomic in other places[WdgM_CheckpointReached()]. So here also modification(setting to 0) should be done in atomic.
              So that modification operation is not preempted by each other
            */

#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
            SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/

            WdgM_SupervisedEntityDyn[SEID].IndividualAliveUpdateCtr = WDGM_PRV_C_ZERO;

#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
            SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/

            /*Clear IndividualSupervisionCycleCtr and FailedAliveSupervisionRefCycleCtr When Mode change happens  */
            WdgM_SupervisedEntityDyn[SEID].IndividualSupervisionCycleCtr = WDGM_PRV_C_ZERO;
            WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleCtr = WDGM_PRV_C_ZERO;

        }
    }

    if(WdgM_AliveSupervisionActive == FALSE)
    {
        WdgM_AliveSupervisionActive = TRUE;
    }

}

#if((WDGM_DEADLINE_TIMEOUT_DETECT == STD_ON) && ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO)))
/************************************************************************************************************************
 * Name              :   WdgM_Prv_DeadlineSupervisionMainFunction_v_Inl
 * Description       :   Update the Deadline Supervision in WdgM_MainFunction for the checkpoints for which Start checkpoint
 *                       has been reached but the stop checkpoint has not been reached and it has expired.
 *                       New RFC request in AutoSAR bugzilla Id : 56735
 * Parameters[in]    :   None
 * Limitations       :   This update will be done only for Timers accessible from WdgM_MainFunction calling TASK context....!!
 * Return Value      :   None
 ************************************************************************************************************************/
LOCAL_INLINE void WdgM_Prv_DeadlineSupervisionMainFunction_v_Inl(void)
{
    WdgM_SupervisedEntityIdType SEID;
    uint16 DeadlineIdx;
    TickType RunningCounterValueTemp;
    TickType ElapsedCounterValueTemp;
    const WdgM_DeadlineSupervisionType * DeadlineSupervisionPtr;
    StatusType returnStatus = E_NOT_OK;
    DeadlineSupervisionPtr = WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToDeadlineSupervision;


    for(DeadlineIdx = WDGM_PRV_C_ZERO; DeadlineIdx < WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].NoOfDeadlineSupervision; DeadlineIdx++)
    {
        RunningCounterValueTemp = WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx];

        /*
         * No need to do the evaluation to update or not to update NewLocalStatus if Deadline supervision is not
         * running or Deadline supervision is running but Stop checkpoint has already reached.
         */
        if(RunningCounterValueTemp != ((TickType)WDGM_PRV_C_ZERO))
        {
            SEID = DeadlineSupervisionPtr[DeadlineIdx].SupervisedEntityId;
/*
 * If the WdgM_Prv_DeadlineSupervisionMainFunction_v_Inl is not called under lock in WdgM_MainFunction
 */
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
                /*
                 * Set variable ElapsedCounterValueTemp as 0(WDGM_PRV_C_ZERO), so that NewLocalStatus will not be updated
                 * if RunningCounterValueTemp is 0.
                 * RunningCounterValueTemp is 0 mean either Deadline supervision is not running or Deadline supervision
                 * is running but Stop checkpoint has already reached.
                 *
                 * Info: ElapsedCounterValueTemp will also have non zero value only when RunningCounterValueTemp will have
                 * no zero value, otherwise ElapsedCounterValueTemp will have value 0 .
                 */
                ElapsedCounterValueTemp = WDGM_PRV_C_ZERO;

                /* Lock Placed:
                 * Reading of captured running counter value and then calculation of elapsed counter value w.r.t current running
                 * counter value should not be pre-empted by other task(WdgM_CheckpointReached). So these operations are done under lock.
                 * For more information refer Errata 4077.
                 */
                SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);

                /* Read once again Running Counter to have latest value for comparison.
                 * This is required if function WdgM_Prv_DeadlineSupervisionMainFunction_v_Inl is
                 * not called under lock.
                 */
                RunningCounterValueTemp = WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx];

                /*
                 * Get the Elapsed counter value if Start checkpoint is reached but End checkpoint is not yet reached
                 * (that is, when Deadline Supervision is running)
                */
                if(RunningCounterValueTemp != ((TickType)WDGM_PRV_C_ZERO))
                {

#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_OS))
                    returnStatus = GetElapsedCounterValue(WdgM_SupervisedEntity[SEID].TimerId, &RunningCounterValueTemp, &ElapsedCounterValueTemp);
#endif
#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_MCU))
                    ElapsedCounterValueTemp = RunningCounterValueTemp;
                    RunningCounterValueTemp = (TickType)Mcu_Rb_GetSysTicks();
                    ElapsedCounterValueTemp = RunningCounterValueTemp - ElapsedCounterValueTemp;
                    returnStatus = E_OK;
#endif
                }
                else
                {
                    returnStatus = E_NOT_OK;
                }

                SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
/*
 * If the WdgM_Prv_DeadlineSupervisionMainFunction_v_Inl is called under lock in WdgM_MainFunction
 */
#else /*(WDGM_LOCK_DISABLE_FOR_DGS) == (STD_OFF) */
#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_OS))
                returnStatus = GetElapsedCounterValue(WdgM_SupervisedEntity[SEID].TimerId, &RunningCounterValueTemp, &ElapsedCounterValueTemp);
#endif
#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_MCU))
                ElapsedCounterValueTemp = RunningCounterValueTemp;
                RunningCounterValueTemp = (TickType)Mcu_Rb_GetSysTicks();
                ElapsedCounterValueTemp = RunningCounterValueTemp - ElapsedCounterValueTemp;
                returnStatus = E_OK;
#endif

#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/

                /*
                 * 1. Update the NewLocalStatus value as expired if
                 * Elapsed Counter value is obtained successfully and
                 * Elapsed Counter value is greater than Deadline max time
                 *
                 * 2. NewLocalStatus will not get updated if either Deadline supervision is not running or Deadline supervision
                 * is running but Stop checkpoint has already reached
                 * (In case WDGM_LOCK_DISABLE_FOR_DGS is STD_ON, ElapsedCounterValueTemp will be 0. Thereby taken care)
                 */

                if((returnStatus == E_OK) && (ElapsedCounterValueTemp > DeadlineSupervisionPtr[DeadlineIdx].DeadlineMax))
                {
                    /*Lock Not required since CPR will at worst modify local to EXPIRED which is done here anyway*/
                    WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_EXPIRED;
# if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                    WdgM_Rb_FirstExpiredEntity(ElapsedCounterValueTemp, DeadlineSupervisionPtr[DeadlineIdx].StartCheckpointId,
                                               DeadlineSupervisionPtr[DeadlineIdx].StopCheckpointId, SEID, WDGM_RB_MAX_SUPERVISEDENTITYID, WDGM_RB_FAILED_IN_DEADLINE_MAINFUNCTION, WdgM_Mode);
# endif
                }
        }
    }
}
#endif


#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
/*******************************************************************************************
* Name              :   WdgM_Prv_DeadlineSupervisionCheckpointReached_v_Inl
* Description       :   Does the Deadline Monitoring part of WdgM_CheckpointReached().
* Parameters[in]    :   SEID, CheckpointID
* Parameters[inout] :   ReturnStatus
* Limitations       :   None
* ReturnType        :   Std_ReturnType
*******************************************************************************************/
LOCAL_INLINE void WdgM_Prv_DeadlineSupervisionCheckpointReached_v_Inl(WdgM_SupervisedEntityIdType SEID,
WdgM_CheckpointIdType CheckpointID, Std_ReturnType * ReturnStatus)
{
    uint16 SvIdx;
    uint16 DeadlineIdx;
    TickType RunningCounterValueTemp;
    TickType ElapsedCounterValueTemp;
    const WdgM_DeadlineSupervisionType * DeadlineSupervisionPtr;
    uint16 * DeadlineIdxPtr;
#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_OS))
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
    boolean Report_E_OsCounter_Det;
    Report_E_OsCounter_Det = FALSE;
#endif
#endif

    *ReturnStatus = E_OK;

    /* TRACE[WDGM322] Perform Deadline supervision*/

    SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
    if ((WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & WDGM_LOCAL_STATUS_DEACTIVATED) != WDGM_LOCAL_STATUS_DEACTIVATED)
    {
        DeadlineIdxPtr = WdgM_SupervisedEntity[SEID].PtrToCheckpoint[CheckpointID].PtrToCheckpointDyn->PtrToDeadlineIndices;
        DeadlineSupervisionPtr = WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToDeadlineSupervision;
        /* TRACE[WDGM299] Perform check if deadline active or inactive */
        for(SvIdx = WDGM_PRV_C_ZERO;  SvIdx < WdgM_SupervisedEntity[SEID].PtrToCheckpoint[CheckpointID].PtrToCheckpointDyn->NoOfDeadlineSupervision; SvIdx++)
        {
            DeadlineIdx = DeadlineIdxPtr[SvIdx];
            RunningCounterValueTemp = WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx];

            if ((CheckpointID == DeadlineSupervisionPtr[DeadlineIdx].StopCheckpointId)&&
                        (RunningCounterValueTemp != ((TickType)WDGM_PRV_C_ZERO)))
            {
#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_OS))
                /*
                 * Evaluation should be done only if return value of GetElapsedCounterValue is E_OK.
                 * Running counter value (timestamp) should be cleared if return value of GetElapsedCounterValue
                 * is other than E_OK. And report error to DET if DET is enabled
                 * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2820]
                 */
                if(E_OK == (GetElapsedCounterValue(WdgM_SupervisedEntity[SEID].TimerId, &RunningCounterValueTemp, &ElapsedCounterValueTemp)))
                {
                    /*
                     * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2819]
                     */
                    /* TRACE[WDGM229] Perform checking when deadline endpoint reached*/
                    if ((ElapsedCounterValueTemp >= DeadlineSupervisionPtr[DeadlineIdx].DeadlineMin) &&
                            (ElapsedCounterValueTemp <= DeadlineSupervisionPtr[DeadlineIdx].DeadlineMax))
                    {
                        /* Update of Local supervision status (WdgM_SupervisedEntityDyn[SEID].NewLocalStatus) not needed */
                    }
                    /*TRACE [WDGM294] Since the time diffrence is not within range the deadline result is
                     * incorrect */
                    else
                    {
                        WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_EXPIRED;
#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                        WdgM_Rb_FirstExpiredEntity(ElapsedCounterValueTemp, DeadlineSupervisionPtr[DeadlineIdx].StartCheckpointId,
                                                   DeadlineSupervisionPtr[DeadlineIdx].StopCheckpointId, SEID, WDGM_RB_MAX_SUPERVISEDENTITYID, WDGM_RB_FAILED_IN_DEADLINE_CHECKPOINTREACHED, WdgM_Mode);
#endif /*(WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON)*/
                    }
                    /*
                     * Clear the RunningCounterValueTemp if start and stop checkpoints are not same
                     * If start and stop checkpoints are same then no need to clear the RunningCounterValueTemp
                     */
                    if(DeadlineSupervisionPtr[DeadlineIdx].StopCheckpointId != DeadlineSupervisionPtr[DeadlineIdx].StartCheckpointId)
                    {
                        RunningCounterValueTemp = WDGM_PRV_C_ZERO;
                    }
                    else
                    {
                        /*
                         * This is to avoid not to detect already started Deadline Supervision exactly at 0.
                         * Anyway the resolution of RunningCounterValueTemp (of type TickType) is very high, impact of addition of 1, w.r.t Seconds
                         * is very less.
                         */
                        if(RunningCounterValueTemp == ((TickType)WDGM_PRV_C_ZERO))
                        {

                            RunningCounterValueTemp = (TickType)WDGM_PRV_C_ONE;
                        }
                    }
                    WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx] = RunningCounterValueTemp;
                }
                else
                {
                    /*
                     * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2860]
                     */
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
                    Report_E_OsCounter_Det = TRUE;
#endif
                    WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx] = WDGM_PRV_C_ZERO;
                    *ReturnStatus = E_NOT_OK;
                 }
#endif /*((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_OS)) */
#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_MCU))
                ElapsedCounterValueTemp = RunningCounterValueTemp;
                /*
                 * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2821]
                 */
                RunningCounterValueTemp = (TickType)Mcu_Rb_GetSysTicks();
                ElapsedCounterValueTemp = RunningCounterValueTemp - ElapsedCounterValueTemp;
                /*
                 * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2861]
                 */
                /* TRACE[WDGM229] Perform checking when deadline endpoint reached*/
                if ((ElapsedCounterValueTemp >= DeadlineSupervisionPtr[DeadlineIdx].DeadlineMin) &&
                        (ElapsedCounterValueTemp <= DeadlineSupervisionPtr[DeadlineIdx].DeadlineMax))
                {
                    /* Update of Local supervision status (WdgM_SupervisedEntityDyn[SEID].NewLocalStatus) not needed */
                }
                /*TRACE [WDGM294] Since the time diffrence is not within range the deadline result is
                 * incorrect */
                else
                {
                    WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_EXPIRED;
#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                    WdgM_Rb_FirstExpiredEntity(ElapsedCounterValueTemp, DeadlineSupervisionPtr[DeadlineIdx].StartCheckpointId,
                                               DeadlineSupervisionPtr[DeadlineIdx].StopCheckpointId, SEID, WDGM_RB_MAX_SUPERVISEDENTITYID, WDGM_RB_FAILED_IN_DEADLINE_CHECKPOINTREACHED, WdgM_Mode);
#endif /*(WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON)*/
                }
                if(DeadlineSupervisionPtr[DeadlineIdx].StopCheckpointId != DeadlineSupervisionPtr[DeadlineIdx].StartCheckpointId)
                {
                    WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx] = WDGM_PRV_C_ZERO;
                }
                else
                {
                    if(RunningCounterValueTemp == ((TickType)WDGM_PRV_C_ZERO))
                    {
                        /*
                         * This is to avoid not to detect already started Deadline Supervision exactly at 0.
                         * Anyway the resolution of RunningCounterValueTemp (of type TickType) is very high, impact of addition of 1, w.r.t Seconds
                         * is very less.
                         */
                        RunningCounterValueTemp = (TickType)WDGM_PRV_C_ONE;
                    }
                    /*
                     * This is for the special case to detect Start of Deadline Supervision, where for cyclic
                     * Task Deadline Start and destination Checkpoints are configured same.
                     */
                    WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx] = RunningCounterValueTemp;
                }
#endif /* ((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_MCU)) */
            }
            else if (CheckpointID == DeadlineSupervisionPtr[DeadlineIdx].StartCheckpointId)
            {
                /* Reference: OS384 */
                /* TRACE[WDGM228] Record the time stamp if deadline started*/
                /* Note : SWS_WdgM_00228 means that it is not considered as an error by Deadline Supervision if
                 * a given Deadline Start Checkpoint is reached several times without reaching the corresponding
                 * Deadline End Checkpoint (each time the timestamp is just updated)*/
#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_OS))
                /*
                 * Running counter value should be captured/stored only if GetElapsedCounterValue returns E_OK.
                 * If the return value is other than E_OK then set *ReturnStatus as E_NOT_OK. If DET is enabled
                 * then report error to DET.
                 *TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2820]
                 */
                if(E_OK == (GetElapsedCounterValue(WdgM_SupervisedEntity[SEID].TimerId, &RunningCounterValueTemp, &ElapsedCounterValueTemp)))
                {
                    if(RunningCounterValueTemp == ((TickType)WDGM_PRV_C_ZERO))
                    {
                        /*
                         * This is to avoid not to detect already started Deadline Supervision exactly at 0.
                         * Anyway the resolution of RunningCounterValueTemp (of type TickType) is very high,
                         * impact of addition of 1, w.r.t Seconds is very less.
                         */
                        RunningCounterValueTemp = (TickType)WDGM_PRV_C_ONE;
                    }
                    /*
                     * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2818]
                     */
                    WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx] = RunningCounterValueTemp;
                }
                else
                {
                    /*
                     * Running Counter will not be modified. It will remain as 0
                     * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2858]
                     */
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
                    Report_E_OsCounter_Det = TRUE;
#endif
                    WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx] = WDGM_PRV_C_ZERO;
                    *ReturnStatus = E_NOT_OK;
                }
#endif /* ((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_OS)) */
#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_MCU))
                /*
                 * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2821]
                 * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2859]
                 */
                RunningCounterValueTemp = (TickType)Mcu_Rb_GetSysTicks();
                if(RunningCounterValueTemp == ((TickType)WDGM_PRV_C_ZERO))
                {
                    /*
                     * This is to avoid not to detect already started Deadline Supervision exactly at 0.
                     * Anyway the resolution of RunningCounterValueTemp (of type TickType) is very high, impact of
                     * addition of 1, w.r.t Seconds is very less.
                     */
                    RunningCounterValueTemp = (TickType)WDGM_PRV_C_ONE;
                }
                WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineIdx] = RunningCounterValueTemp;
#endif /* ((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_MCU)) */
            }
            else
            {
                /*
                 * ReturnStatus should be ok for following condition
                 * TRACE[SWS_WdgM_00354]Do not consider it as error when End Check point reached before Start check point or
                 * when Deadline End Checkpoint is reached several times in a sequence.
                 * Running counter value == 0 & check point == stop
                 * ReturnStatus = E_OK;
                 */
            }
        }/* End of for loop */
        SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#if((WDGM_RB_DEADLINE_TIMER_SELECTION) == (WDGM_RB_DEADLINE_TIMER_SELECTION_OS))
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
        /*
        * TRACE[BSW_SWS_AR4_0_R2_WatchDogManager_Ext-2862]
        */
        if(Report_E_OsCounter_Det == TRUE)
        {
            /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
            (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_APIID, WDGM_E_OS_COUNTER);
        }
#endif
#endif
    }
    else
    {
        SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
        /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_APIID, WDGM_E_SEDEACTIVATED);
        *ReturnStatus = E_NOT_OK;
#endif /* ((WDGM_DEV_ERROR_DETECT) == (STD_ON)) */
    }
}
#endif


#if (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON)
/*******************************************************************************************
* Name              :   WdgM_Prv_LogicalSupervisionCheckpointReached_v_Inl
* Description       :   Does the Internal Graph Logical Monitoring part of WdgM_CheckpointReached().
* Parameters[in]    :   SEID, CheckpointID
* Parameters[inout] :   ReturnStatus
* Limitations       :   None
* ReturnType        :   Std_ReturnType
*******************************************************************************************/
LOCAL_INLINE Std_ReturnType WdgM_Prv_LogicalSupervisionCheckpointReached_v_Inl(WdgM_SupervisedEntityIdType SEID,
WdgM_CheckpointIdType CheckpointID)
{
    uint16 cntrDestCPs;
    uint16 idxStartDestCPs;
    uint16 idxCPProperty_givenCP;
    uint16 InternalGraphIdx;
    WdgM_PositionInGraph_ten posnInGraph_en;
    uint16 nrDestCheckpoints;
    Std_ReturnType ReturnStatus;
#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
    uint8 ExpiredLocation;
    WdgM_CheckpointIdType idLastReachedCheckpoint;
    WdgM_ModeType Mode;

    ExpiredLocation = WDGM_RB_DEFAULT_EXPIREDLOCATION;
    idLastReachedCheckpoint = WDGM_PRV_C_ZERO;
    Mode = WDGM_RB_MAX_MODEID;
#endif
    ReturnStatus = E_OK;

    if(WdgM_SupervisedEntity[SEID].hasInternalGraph == TRUE)
    {
        idxCPProperty_givenCP = WdgM_SupervisedEntity[SEID].idxInternalGraphCPProperty + CheckpointID;
        posnInGraph_en = WdgM_InternalGraph_CPProperty[idxCPProperty_givenCP].posnInGraph_en;
        /*When reported CPID which do not belong to the current Internal graph no action should be taken hence this check is done
         *TRACE[SWS_WdgM_00297]For any reported Checkpoint that does not belong to any Graph, then it
         * shall be ignore and not update the result of the Logical Supervision for the Supervised Entity.*/
        if(posnInGraph_en != WDGM_POSNGRAPH_NONE_E)
        {

            /* Lock introduced:
             * 1.In WdgM_SupervisedEntityDyn we check for Local status deactivated,
             * if mode changes here and the reported Supervised entity becomes deactivated in the new mode then,
             * the reported checkpoint shall not be processed.*/
            /*TRACE[SWS_WdgM_00208] Since SEActive is False for DEACTIVATED, no supervision process is performed*/
            SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
            if ((WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & WDGM_LOCAL_STATUS_DEACTIVATED) != WDGM_LOCAL_STATUS_DEACTIVATED)
            {

                /* TRACE[SWS_WdgM_00332 and SWS_WdgM_00273] If the function WdgM_CheckpointReached the result correct, and
                 * the Checkpoint is defined as a initial one, then the function WdgM_CheckpointReached shall set the
                 * Activity Flag of the corresponding graph to true.*/
                InternalGraphIdx =  WdgM_InternalGraph_CPProperty[idxCPProperty_givenCP].InternalGraphId;
                if(WdgM_InternalGraph_StatusDyn[InternalGraphIdx].flgActivity == FALSE)
                {
                    if(posnInGraph_en == WDGM_POSNGRAPH_INITIAL_E)
                    {
                        WdgM_InternalGraph_StatusDyn[InternalGraphIdx].idLastReachedCheckpoint = CheckpointID;
                        WdgM_InternalGraph_StatusDyn[InternalGraphIdx].flgActivity = TRUE;
                    }
                    /*Update the NewLocalStatus value as expired if
                     * Reported Checkpoint is not an Initial Checkpoint and
                     * Internal Graph is not active*/
                    else
                    {
                        WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_EXPIRED;
                        #if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                            ExpiredLocation = WDGM_RB_FAILED_IN_LOGICAL_INTGRAPH_CPREACHED_GRAPHNOTSTARTED;
                            idLastReachedCheckpoint = WdgM_InternalGraph_StatusDyn[InternalGraphIdx].idLastReachedCheckpoint;
                            Mode = WdgM_Mode;
                        #endif
                    }
                }

                else
                {
                    /* TRACE[SWS_WdgM_00246 and SWS_WdgM_00274] Iterate through all the transition
                     * to find Reported checkpoint has valid transition
                     * (i.e. Is it Successor of perviously reported Checkpoint) */
                    idxCPProperty_givenCP = WdgM_SupervisedEntity[SEID].idxInternalGraphCPProperty + WdgM_InternalGraph_StatusDyn[InternalGraphIdx].idLastReachedCheckpoint;
                    idxStartDestCPs = WdgM_InternalGraph_CPProperty[idxCPProperty_givenCP].idxDestCheckpoints;
                    nrDestCheckpoints = WdgM_InternalGraph_CPProperty[idxCPProperty_givenCP].nrDestCheckpoints;
                    for(cntrDestCPs=0;cntrDestCPs<nrDestCheckpoints;cntrDestCPs++)
                    {
                        if(WdgM_InternalGraph_DestCheckpoints[idxStartDestCPs + cntrDestCPs] == CheckpointID)
                        {
                            WdgM_InternalGraph_StatusDyn[InternalGraphIdx].idLastReachedCheckpoint = CheckpointID;
                            break;
                        }
                    }
                    /*Update the NewLocalStatus value as expired if
                    * Reported Checkpoint has no valid transition and Internal Graph is active*/
                    if(cntrDestCPs == nrDestCheckpoints)
                    {
                        WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_EXPIRED;
                        #if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                            ExpiredLocation = WDGM_RB_FAILED_IN_LOGICAL_INTGRAPH_CPREACHED_GRAPHSTARTED;
                            idLastReachedCheckpoint = WdgM_InternalGraph_StatusDyn[InternalGraphIdx].idLastReachedCheckpoint;
                            Mode = WdgM_Mode;
                        #endif
                    }
                    /* TRACE[SWS_WdgM_00331 and SWS_WdgM_00274] When the reported checkpoint is a final
                     * checkpoint then deactivate corresponding Internal graph
                     * (i.e. by setting SEID Activity Flag to false), hence this check is done.*/
                    else if (posnInGraph_en == WDGM_POSNGRAPH_FINAL_E)
                    {
                        WdgM_InternalGraph_StatusDyn[InternalGraphIdx].flgActivity = FALSE;
                    }
                    else
                    {
                        /*TRACE[SWS_WdgM_00274]If the reported Checkpoint is a successor of the stored Checkpoint within
                         * the Graph, then the result of the Logical Supervision of the Supervised Entity is correct.*/
                    }
                }
                SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
                #if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                if((ExpiredLocation == WDGM_RB_FAILED_IN_LOGICAL_INTGRAPH_CPREACHED_GRAPHNOTSTARTED) ||
                        (ExpiredLocation == WDGM_RB_FAILED_IN_LOGICAL_INTGRAPH_CPREACHED_GRAPHSTARTED))
                {
                    WdgM_Rb_FirstExpiredEntity(WDGM_PRV_C_ZERO, CheckpointID, idLastReachedCheckpoint,
                                               SEID, WDGM_RB_MAX_SUPERVISEDENTITYID, ExpiredLocation, Mode);
                }
                #endif /*(WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON)*/
            }
            else
            {
                SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
                #if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
                    /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored,
                     * and hence it is type casted to type void.*/
                    (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_APIID, WDGM_E_SEDEACTIVATED);
                    ReturnStatus = E_NOT_OK;
                #endif /* ((WDGM_DEV_ERROR_DETECT) == (STD_ON)) */
            }
        }
    }
    return ReturnStatus;
}
#endif

#if (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
/*******************************************************************************************
* Name              :   WdgM_Prv_ExternalGraphCheckpointReached_v_Inl
* Description       :   Does the External Graph Logical Monitoring part of WdgM_CheckpointReached().
* Parameters[in]    :   SEID, CheckpointID
* Parameters[inout] :   ReturnStatus
* Limitations       :   None
* ReturnType        :   Std_ReturnType
*******************************************************************************************/
LOCAL_INLINE Std_ReturnType WdgM_Prv_ExternalGraphCheckpointReached_v_Inl(WdgM_SupervisedEntityIdType SEID,
WdgM_CheckpointIdType CheckpointID)
{
    uint16 cntrDestCPs;
    uint16 ExternalGraphIdx;
    WdgM_ExternalPositionInGraph_ten SourcePosnInGraph_en;
    WdgM_ExternalPositionInGraph_ten DestPosnInGraph_en;
    Std_ReturnType ReturnStatus;
    uint16 externalGraphId;
    uint16 PresentInNoofTrans;
    uint16 * ExternalGraphIdxPtr;
    const WdgM_ExternalGraphType * PtrToExternalGraph;
#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
    uint8 ExpiredLocation;
    WdgM_CheckpointIdType idLastReachedCheckpoint;
    WdgM_SupervisedEntityIdType idLastReachedSupervisedEntity;

    WdgM_ModeType Mode;

    ExpiredLocation = WDGM_RB_DEFAULT_EXPIREDLOCATION;
    idLastReachedCheckpoint = WDGM_PRV_C_ZERO;
    idLastReachedSupervisedEntity = WDGM_PRV_C_ZERO;
    Mode = WDGM_RB_MAX_MODEID;
#endif
    ReturnStatus = E_OK;
    /*Check in current Mode External graph is configured or not*/
    if(WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].NoOfExternalGraphTransitions > WDGM_PRV_C_ZERO)
    {
        /*Check in current recived Checkpoint is present inside any External graph or not*/
        if(WdgM_SupervisedEntity[SEID].PtrToCheckpoint[CheckpointID].PtrToCheckpointDyn->PresentInNoofTrans > WDGM_PRV_C_ZERO)
        {
            PtrToExternalGraph = WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToExternalGraph;
            SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
            if ((WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & WDGM_LOCAL_STATUS_DEACTIVATED) != WDGM_LOCAL_STATUS_DEACTIVATED)
            {
                ExternalGraphIdx = WdgM_SupervisedEntity[SEID].PtrToCheckpoint[CheckpointID].PtrToCheckpointDyn->PtrToExternalGraphsIndices[0];
                externalGraphId = PtrToExternalGraph[ExternalGraphIdx].ExternalGraphId;
                /*If the function WdgM_CheckpointReached the result correct, and
                 * the Checkpoint is defined as a initial one, then the function WdgM_CheckpointReached shall set the
                 * Activity Flag of the corresponding External graph to true.*/
                if(WdgM_ExternalGraph_StatusDyn[externalGraphId].activityFlag == FALSE)
                {
                    SourcePosnInGraph_en = PtrToExternalGraph[ExternalGraphIdx].SourcePosnInGraph_en;
                    if(SourcePosnInGraph_en == WDGM_EXTERNALPOSNGRAPH_INITIAL_E)
                    {
                        WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedCheckpointId = CheckpointID;
                        WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedSupervisedEntityId = SEID;
                        WdgM_ExternalGraph_StatusDyn[externalGraphId].activityFlag = TRUE;
                    }
                    /*Update the NewLocalStatus value as expired if
                    * Reported Checkpoint is not an Initial Checkpoint and
                    * External Graph is not active*/
                    else
                    {
                        WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_EXPIRED;
                        #if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                            ExpiredLocation = WDGM_RB_FAILED_IN_LOGICAL_EXTGRAPH_CPREACHED_GRAPHNOTSTARTED;
                            idLastReachedCheckpoint = WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedCheckpointId;
                            idLastReachedSupervisedEntity = WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedSupervisedEntityId;
                            Mode = WdgM_Mode;
                        #endif
                    }
                }
                else
                {
                    /*  Iterate through all the transition
                     * to find Reported checkpoint has valid transition
                     * (i.e. Is it Successor of perviously reported Checkpoint) */

                    idLastReachedCheckpoint = WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedCheckpointId;
                    idLastReachedSupervisedEntity = WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedSupervisedEntityId;

                    ExternalGraphIdxPtr = WdgM_SupervisedEntity[idLastReachedSupervisedEntity].PtrToCheckpoint[idLastReachedCheckpoint].PtrToCheckpointDyn->PtrToExternalGraphsIndices;

                    PresentInNoofTrans = WdgM_SupervisedEntity[idLastReachedSupervisedEntity].PtrToCheckpoint[idLastReachedCheckpoint].PtrToCheckpointDyn->PresentInNoofTrans;

                    for(cntrDestCPs = WDGM_PRV_C_ZERO; cntrDestCPs < PresentInNoofTrans; cntrDestCPs++)
                    {
                        ExternalGraphIdx = ExternalGraphIdxPtr[cntrDestCPs];
                        if ((CheckpointID == PtrToExternalGraph[ExternalGraphIdx].DestCPId)&&
                                                (SEID == PtrToExternalGraph[ExternalGraphIdx].DestSEId))
                        {
                            WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedCheckpointId = CheckpointID;
                            WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedSupervisedEntityId = SEID;
                            break;
                        }
                    }
                    DestPosnInGraph_en = PtrToExternalGraph[ExternalGraphIdx].DestPosnInGraph_en;
                    /*Update the NewLocalStatus value as expired if
                    * Reported Checkpoint has no valid transition and External Graph is active*/
                    if(cntrDestCPs == PresentInNoofTrans)
                    {
                        WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WdgM_SupervisedEntityDyn[SEID].NewLocalStatus | WDGM_LOCAL_STATUS_EXPIRED;
                        #if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                            ExpiredLocation = WDGM_RB_FAILED_IN_LOGICAL_EXTGRAPH_CPREACHED_GRAPHSTARTED;
                            idLastReachedCheckpoint = WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedCheckpointId;
                            idLastReachedSupervisedEntity = WdgM_ExternalGraph_StatusDyn[externalGraphId].lastReachedSupervisedEntityId;
                            Mode = WdgM_Mode;
                        #endif
                    }
                    /*  When the reported checkpoint is a final
                     * checkpoint then deactivate corresponding External graph
                     * (i.e. by setting SEID Activity Flag to false), hence this check is done.*/
                    else if (DestPosnInGraph_en == WDGM_EXTERNALPOSNGRAPH_FINAL_E)
                    {
                        WdgM_ExternalGraph_StatusDyn[externalGraphId].activityFlag = FALSE;
                    }
                    else
                    {
                        /*If the reported Checkpoint is a successor of the stored Checkpoint within
                         * the Graph, then the result of the Logical Supervision of the Supervised Entity is correct.*/
                    }
                }
                SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
                #if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
                if((ExpiredLocation == WDGM_RB_FAILED_IN_LOGICAL_EXTGRAPH_CPREACHED_GRAPHNOTSTARTED) ||
                        (ExpiredLocation == WDGM_RB_FAILED_IN_LOGICAL_EXTGRAPH_CPREACHED_GRAPHSTARTED))
                {
                    WdgM_Rb_FirstExpiredEntity(WDGM_PRV_C_ZERO, CheckpointID, idLastReachedCheckpoint,
                                               SEID, idLastReachedSupervisedEntity, ExpiredLocation, Mode);
                }
                #endif /*(WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON)*/
            }
            else
            {
                SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
                #if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
                    /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored,
                     * and hence it is type casted to type void.*/
                    (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_APIID, WDGM_E_SEDEACTIVATED);
                    ReturnStatus = E_NOT_OK;
                #endif /* ((WDGM_DEV_ERROR_DETECT) == (STD_ON)) */
            }
        }
    }
    return ReturnStatus;
}
#endif

/************************************************************************************************************************
 * Name              :   WdgM_Prv_UpdateAliveSupervisionDyn_v_Inl
 * Description       :   Update the AliveSupervision Variables at mode change
 * Parameters[in]    :   None
 * Limitations       :   None
 * Return Value      :   None
 ************************************************************************************************************************/
LOCAL_INLINE void WdgM_Prv_UpdateAliveSupervisionDyn_v_Inl(void)
{

    /*
     * Disable the Alive Supervision upto next WdgM_MainFunction call to synchronise the expected indication and
     * the new Supervision Cycle. Other variables are not required to be reset here as anyway in WdgM_MainFunction
     * it will be reinitialized at the time of start of Alive Supervision, when WdgM_AliveSupervisionActive
     * becomes true.
     *
     */
    WdgM_AliveSupervisionActive = FALSE;
}

#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
/************************************************************************************************************************
 * Name              :   WdgM_Prv_UpdateDeadlineSupervisionDyn_v_Inl
 * Description       :   Update the Checkpoint Linking to the different Deadline Supervisions in current mode
 * Parameters[in]    :   Mode
 * Limitations       :   None
 * Return Value      :   None
 ************************************************************************************************************************/
LOCAL_INLINE void WdgM_Prv_UpdateDeadlineSupervisionDyn_v_Inl(WdgM_ModeType Mode)
{
    uint16 OldIdx;
    uint16 NewIdx;

    TickType RunningCounterValueTemp[WDGM_MAX_DEADLINE_SUPERVISIONS] = {0}; /* Initializing RunningCounterValueTemp with zero to avoid violation of MR12 RULE 9.1 */
    const WdgM_DeadlineSupervisionType * OldDeadlineSupervisionPtr;
    const WdgM_DeadlineSupervisionType * NewDeadlineSupervisionPtr;

    /*
     * Transfer all the Running Counter values for the start checkpoints from previous Mode to
     * current mode for all Deadline supervisions which were active in previous mode and are active
     * in current mode also.
     */

    OldDeadlineSupervisionPtr = WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToDeadlineSupervision;
    NewDeadlineSupervisionPtr = WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToDeadlineSupervision;

    for(OldIdx = WDGM_PRV_C_ZERO; OldIdx < WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].NoOfDeadlineSupervision; OldIdx++)
    {
        RunningCounterValueTemp[OldIdx] = WdgM_ConfigSetPtr->PtrToRunningCounterValue[OldIdx];
        WdgM_ConfigSetPtr->PtrToRunningCounterValue[OldIdx] = WDGM_PRV_C_ZERO;
    }

    for(NewIdx = WDGM_PRV_C_ZERO; NewIdx < WdgM_ConfigSetPtr->PtrToMode[Mode].NoOfDeadlineSupervision; NewIdx++)
    {
        for(OldIdx = WDGM_PRV_C_ZERO; OldIdx < WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].NoOfDeadlineSupervision; OldIdx++)
        {
            if((OldDeadlineSupervisionPtr[OldIdx].StartCheckpointId == NewDeadlineSupervisionPtr[NewIdx].StartCheckpointId)&&
                (OldDeadlineSupervisionPtr[OldIdx].StopCheckpointId == NewDeadlineSupervisionPtr[NewIdx].StopCheckpointId)&&
                (OldDeadlineSupervisionPtr[OldIdx].SupervisedEntityId == NewDeadlineSupervisionPtr[NewIdx].SupervisedEntityId))
            {
                WdgM_ConfigSetPtr->PtrToRunningCounterValue[NewIdx] = RunningCounterValueTemp[OldIdx];
            }
        }
    }
}
#endif

#if (((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO)) || (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON))
/************************************************************************************************************************
 * Name              :   WdgM_Prv_UpdateSupervisionDyn_v_Inl
 * Description       :   Update the Checkpoint Linking to the different Deadline Supervisions in current mode
 * Parameters[in]    :   Mode
 * Limitations       :   None
 * Return Value      :   None
 ************************************************************************************************************************/
LOCAL_INLINE void WdgM_Prv_UpdateSupervisionDyn_v_Inl(WdgM_ModeType Mode)
{
    WdgM_SupervisedEntityIdType SEID;
    WdgM_CheckpointIdType CheckpointID;

#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
    uint16 DeadlineIdx;
    uint16 Deadline_Offset;
    uint16 SingleCheckpointDeadlineSupervisionSize;
    const WdgM_DeadlineSupervisionType * DeadlineSupervisionPtr;
#endif

#if (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
    uint16 ExternalTransitionIdx;
    uint16 Ext_Offset;
    uint16 ExternalGraphSize;
    const WdgM_ExternalGraphType * ExternalGraphPtr;
#endif

#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
    Deadline_Offset = WDGM_PRV_C_ZERO;
    DeadlineSupervisionPtr = WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToDeadlineSupervision;
#endif

#if (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
    ExternalGraphPtr = WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToExternalGraph;
    Ext_Offset = WDGM_PRV_C_ZERO;
#endif

    for(SEID = WDGM_PRV_C_ZERO; SEID < WDGM_NO_OF_SUPERVISED_ENTITIES; SEID++)
    {
        for(CheckpointID = WDGM_PRV_C_ZERO; CheckpointID < WdgM_SupervisedEntity[SEID].NoOfCheckpoint; CheckpointID++)
        {

#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
            SingleCheckpointDeadlineSupervisionSize = WDGM_PRV_C_ZERO;
            WdgM_SupervisedEntity[SEID].PtrToCheckpoint[CheckpointID].PtrToCheckpointDyn->PtrToDeadlineIndices =
                    &WdgM_ConfigSetPtr->PtrToDeadlineIndices[Deadline_Offset];

            for(DeadlineIdx = WDGM_PRV_C_ZERO; DeadlineIdx < WdgM_ConfigSetPtr->PtrToMode[Mode].NoOfDeadlineSupervision; DeadlineIdx++)
            {
                if(((DeadlineSupervisionPtr[DeadlineIdx].StartCheckpointId == CheckpointID)||
                        (DeadlineSupervisionPtr[DeadlineIdx].StopCheckpointId == CheckpointID))&&
                        (DeadlineSupervisionPtr[DeadlineIdx].SupervisedEntityId == SEID))
                {
                    WdgM_ConfigSetPtr->PtrToDeadlineIndices[Deadline_Offset] = DeadlineIdx; /* Store this DeadlineIdx (valid)*/
                    Deadline_Offset++;
                    SingleCheckpointDeadlineSupervisionSize++;
                }
            }
            WdgM_SupervisedEntity[SEID].PtrToCheckpoint[CheckpointID].PtrToCheckpointDyn->NoOfDeadlineSupervision = SingleCheckpointDeadlineSupervisionSize;
#endif

#if WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON
            ExternalGraphSize = WDGM_PRV_C_ZERO;
            WdgM_SupervisedEntity[SEID].PtrToCheckpoint[CheckpointID].PtrToCheckpointDyn->PtrToExternalGraphsIndices =
            &WdgM_ConfigSetPtr->PtrToExternalGraphsIndices[Ext_Offset];

            for(ExternalTransitionIdx = WDGM_PRV_C_ZERO; ExternalTransitionIdx < WdgM_ConfigSetPtr->PtrToMode[Mode].NoOfExternalGraphTransitions; ExternalTransitionIdx++)
            {

                if(((ExternalGraphPtr[ExternalTransitionIdx].SourceSEId == SEID) && (ExternalGraphPtr[ExternalTransitionIdx].SourceCPId == CheckpointID)) ||
                    ((ExternalGraphPtr[ExternalTransitionIdx].DestSEId == SEID) && (ExternalGraphPtr[ExternalTransitionIdx].DestCPId == CheckpointID) && (ExternalGraphPtr[ExternalTransitionIdx].DestPosnInGraph_en == WDGM_EXTERNALPOSNGRAPH_FINAL_E)))
                {
                    WdgM_ConfigSetPtr->PtrToExternalGraphsIndices[Ext_Offset] = ExternalTransitionIdx;
                    Ext_Offset++;
                    ExternalGraphSize++;
                }
             }

             WdgM_SupervisedEntity[SEID].PtrToCheckpoint[CheckpointID].PtrToCheckpointDyn->PresentInNoofTrans = ExternalGraphSize;
#endif
        }
    }
}
#endif


#if WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON
/************************************************************************************************************************
 * Name              :   WdgM_Prv_UpdateExternalGraphDyn_v_Inl
 * Description       :   Update the Checkpoint Linking to the different External Graphs in current mode
 * Parameters[in]    :   None
 * Limitations       :   None
 * Return Value      :   None
 ************************************************************************************************************************/
LOCAL_INLINE void WdgM_Prv_UpdateExternalGraphDyn_v_Inl(void)
{
    uint16 GraphCounter;

    for(GraphCounter = WDGM_PRV_C_ZERO; GraphCounter < WDGM_MAX_EXTERNAL_GRAPH; GraphCounter++)
    {
        WdgM_ExternalGraph_StatusDyn[GraphCounter].activityFlag = FALSE;
        WdgM_ExternalGraph_StatusDyn[GraphCounter].lastReachedCheckpointId = WDGM_PRV_C_ZERO;
        WdgM_ExternalGraph_StatusDyn[GraphCounter].lastReachedSupervisedEntityId = WDGM_PRV_C_ZERO;
    }
}
#endif

/************************************************************************************************************************
 * Name              :   WdgM_Prv_UpdateTriggerDyn_v_Inl
 * Description       :   Update the Trigger condition and Wdg Mode in current mode
 * Parameters[in]    :   Mode
 * Parameters[inout] :   ReturnStatus
 * Limitations       :   None
 * Return Value      :   None
 ************************************************************************************************************************/
LOCAL_INLINE void WdgM_Prv_UpdateTriggerDyn_v_Inl(WdgM_ModeType Mode, Std_ReturnType * ReturnStatus)
{
    uint8 triggerIdx_u8;
    const WdgM_TriggerType * TriggerPtr;
    *ReturnStatus = E_OK;
    /* Trigger the Watchdogs for new trigger conditionvalue */

    for(triggerIdx_u8 = WDGM_PRV_C_ZERO; triggerIdx_u8 < WdgM_ConfigSetPtr->PtrToMode[Mode].NoOfTrigger; triggerIdx_u8++)
    {
        if(WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToTrigger[triggerIdx_u8].WdgMode != WDGIF_OFF_MODE)
        {
            WdgIf_SetTriggerCondition(WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToTrigger[triggerIdx_u8].DeviceIdx,
                    WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToTrigger[triggerIdx_u8].TriggerConditionValue);
        }
    }

    /* Setting the Wdg driver Modes */
    TriggerPtr = WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToTrigger;
    for(triggerIdx_u8= WDGM_PRV_C_ZERO; triggerIdx_u8<WdgM_ConfigSetPtr->PtrToMode[Mode].NoOfTrigger; triggerIdx_u8++)
    {
        *ReturnStatus = WdgIf_SetMode(TriggerPtr[triggerIdx_u8].DeviceIdx, TriggerPtr[triggerIdx_u8].WdgMode);
        if (*ReturnStatus != E_OK)
        {
            break;
        }
    }
}


/************************************************************************************************************************
 * Name              :   WdgM_Prv_UpdateSupervisedEntityDyn_v_Inl
 * Description       :   Update the WdgM_SupervisedEntityDyn Status of All the Supervised Entity in current mode
 * Parameters[in]    :   Mode
 * Limitations       :   None
 * Return Value      :   None
 ************************************************************************************************************************/
LOCAL_INLINE void WdgM_Prv_UpdateSupervisedEntityDyn_v_Inl(WdgM_ModeType Mode)
{
    uint16 ctr;
#if (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON)
    uint16 grpIdx;
#endif
    WdgM_SupervisedEntityIdType SEID;
    WdgM_SupervisedEntityIdType LspIdx;
    const WdgM_AliveSupervisionType * AliveSupervisionPtr;
    const WdgM_LocalStatusParamsType * LocalStatusParamsPtr;

    AliveSupervisionPtr = WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToAliveSupervision;
    LocalStatusParamsPtr = WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToLocalStatusParams;

    for(SEID = WDGM_PRV_C_ZERO; SEID < WDGM_NO_OF_SUPERVISED_ENTITIES; SEID++)
    {
        for(LspIdx = WDGM_PRV_C_ZERO; LspIdx < WdgM_ConfigSetPtr->PtrToMode[Mode].NoOfLocalStatusParams; LspIdx++)
        {
            if(LocalStatusParamsPtr[LspIdx].SupervisedEntityId == SEID)
            {
                if(WdgM_SupervisedEntityDyn[SEID].OldLocalStatus == WDGM_LOCAL_STATUS_DEACTIVATED)
                {
                    /* TRACE[WDGM209] IF local status is DEACTIVATED and SetMode changes
                     * SE to ACTIVE, local status of SE is OK*/
                    /* TRACE[WDGM268] Transition: 9 : Local status OK if active:
                     * When SetMode changes mode.. SE's Set to OK here*/
                    /* SE is ACTIVE and local status is DEACTIVATED then local status is OK */
                    /* TRACE[WDGM268] WdgM Init Successful hence set the local Mode to OK*/
                    /* Set Failed Alive Supervision Reference Cycle counter to zero*/
                    WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WDGM_LOCAL_STATUS_OK;
                    WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleCtr = WDGM_PRV_C_ZERO;
                    WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleTol = LocalStatusParamsPtr[LspIdx].FailedAliveSupervisionRefCycleTol;
                }

                break; /* This break is very important in order to detect whether SEID is active or not (in below check)..!! */
            }
        }

        /* TRACE[WDGM269] Change local status DEACTIVATED if not active*/
        /* TRACE[WDGM291]  If local state is FAILED and Set mode has changed
         * the Mode to Deactivate then change local status to DEACTIVATED*/
        /* TRACE[WDGM207] If local state is OK and Set mode has changed
         * the Mode to Deactivate then change local status to DEACTIVATED*/
        /* TRACE[WDGM208] : Transition 12, 7, 8 */
        /* If SE is NOT ACTIVE and local state is NOT EXPIRED then change local status to DEACTIVATED */
        /* TRACE[SWS_WdgM_00296] Init flgActivity to FALSE and idLastReachedCheckpoint to zero
         * for WdgM_SupervisedEntityDyn is NOT ACTIVE*/
        if((LspIdx == WdgM_ConfigSetPtr->PtrToMode[Mode].NoOfLocalStatusParams)&&
                ((WdgM_SupervisedEntityDyn[SEID].OldLocalStatus & WDGM_LOCAL_STATUS_EXPIRED) != WDGM_LOCAL_STATUS_EXPIRED))
        {
            WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WDGM_LOCAL_STATUS_DEACTIVATED;
            WdgM_SupervisedEntityDyn[SEID].FailedAliveSupervisionRefCycleTol = WDGM_PRV_C_ZERO;

            #if (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON)
                    for(grpIdx = WDGM_PRV_C_ZERO; grpIdx < WDGM_MAX_INTERNAL_GRAPH; grpIdx++)
                    {
                        WdgM_InternalGraph_StatusDyn[grpIdx].flgActivity = FALSE;
                        WdgM_InternalGraph_StatusDyn[grpIdx].idLastReachedCheckpoint = WDGM_PRV_C_ZERO;
                    }
            #endif
        }


        for(ctr = WDGM_PRV_C_ZERO; ctr < WdgM_ConfigSetPtr->PtrToMode[Mode].NoOfAliveSupervision; ctr++)
        {
            if(AliveSupervisionPtr[ctr].SupervisedEntityId == SEID)
            {
                break;  /* This break is very important in order to detect whether Alive Supervision is configured for this SEID ..!! */
            }
        }

        WdgM_SupervisedEntityDyn[SEID].AliveSupervisionIdx = ctr; /* In case Alive supervision not configured AliveSupervisionIdx
                                                                     will have NoOfAliveSupervision to indicate as invalid value */
    }
}

#endif /*WDGM_PRV_H*/
