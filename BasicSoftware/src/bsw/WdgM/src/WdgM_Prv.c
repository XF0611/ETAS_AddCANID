

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "WdgM_Prv.h"

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

#define WDGM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

const WdgM_ConfigType * WdgM_ConfigSetPtr;

#define WDGM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "WdgM_MemMap.h"

boolean WdgM_AliveSupervisionActive;

#define WDGM_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_FAST_CLEARED_8
#include "WdgM_MemMap.h"

uint8 WdgM_Mode;

#define WDGM_STOP_SEC_VAR_FAST_CLEARED_8
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_CLEARED_16
#include "WdgM_MemMap.h"

uint16 WdgM_ExpiredSupervisionCycleCtr;

#define WDGM_STOP_SEC_VAR_CLEARED_16
#include "WdgM_MemMap.h"

#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
#define WDGM_START_SEC_VAR_CLEARED_32
#include "WdgM_MemMap.h"

uint32 WdgM_MainFunction_Cnt_u32;  /* This variable is used only for debug purpose. */

#define WDGM_STOP_SEC_VAR_CLEARED_32
#include "WdgM_MemMap.h"
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/

#define WDGM_START_SEC_VAR_SLOW_INIT_BOOLEAN
#include "WdgM_MemMap.h"

boolean WdgM_Prv_FirstDeactivation_b = TRUE;

#define WDGM_STOP_SEC_VAR_SLOW_INIT_BOOLEAN
#include "WdgM_MemMap.h"

#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
#define WDGM_START_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

WdgM_Rb_FirstExpiredEntityInfo_tst WdgM_Rb_FirstExpiredEntityInfo_st;

#define WDGM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
#endif  /*(WDGM_RB_DEBUG_OPTION_ENABLED == STD_ON)*/

#define WDGM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

/* TRACE[WDGM349] Implementation of non-initialized RAM location, stored as a double-inverse value */
WdgM_SupervisedEntityIdType WdgM_FirstExpiredSupervisedEntityId;
WdgM_SupervisedEntityIdType WdgM_FirstExpiredSupervisedEntityId_Comp;

#define WDGM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
#define WDGM_START_SEC_VAR_FAST_POWER_ON_CLEARED_8
#include "WdgM_MemMap.h"

uint8 WdgM_Rb_FirstExpiredEntity_Cnt_u8;

#define WDGM_STOP_SEC_VAR_FAST_POWER_ON_CLEARED_8
#include "WdgM_MemMap.h"
#endif  /*(WDGM_RB_DEBUG_OPTION_ENABLED == STD_ON)*/

#define WDGM_START_SEC_VAR_CLEARED_8
#include "WdgM_MemMap.h"

#if((WDGM_RTE_DISABLED) == (STD_OFF))
Std_ReturnType WdgM_Prv_Rte_Switch_globalMode_Status;
#endif /*(WDGM_RTE_DISABLED == STD_OFF)*/

#define WDGM_STOP_SEC_VAR_CLEARED_8
#include "WdgM_MemMap.h"


#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"

/* Local Functions */

/************************************************************************************************************************
 * Name              :   WdgM_Prv_SetTriggerCondition
 * Description       :   Based on global supervsion status trigger the watchdog with configured TriggerConditionValue.
 * Parameters[in]    :   globalStatus_tu8
 * Limitations       :   None
 * Return Value      :   void
 ************************************************************************************************************************/
void WdgM_Prv_SetTriggerCondition(WdgM_GlobalStatusType globalStatus_tu8)
{
    uint8 triggerIdx_u8;

    for(triggerIdx_u8 = WDGM_PRV_C_ZERO; triggerIdx_u8 < WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].NoOfTrigger; triggerIdx_u8++)
    {
        if(WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToTrigger[triggerIdx_u8].WdgMode != WDGIF_OFF_MODE)
        {
            if(globalStatus_tu8 == WDGM_GLOBAL_STATUS_STOPPED) /* WDGM122 */
            {
                WdgIf_SetTriggerCondition(WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToTrigger[triggerIdx_u8].DeviceIdx,
                WDGM_PRV_C_ZERO);
            }
            else if(globalStatus_tu8 == WDGM_GLOBAL_STATUS_DEACTIVATED) /* WDGM292 and (WDGM223: 2)*/
            {
                if(WdgM_Prv_FirstDeactivation_b == TRUE)
                {
                    WdgIf_SetTriggerCondition(WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToTrigger[triggerIdx_u8].DeviceIdx,
                    WDGM_PRV_C_ZERO);
                }
            }
            else /* WDGM292 */
            {
                WdgIf_SetTriggerCondition(WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToTrigger[triggerIdx_u8].DeviceIdx,
                WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToTrigger[triggerIdx_u8].TriggerConditionValue);
            }
        }
    }
    /*To avoid multiple deactivation*/
    if(globalStatus_tu8 == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
        WdgM_Prv_FirstDeactivation_b = FALSE;
    }
}

/************************************************************************************************************************
 * Name              :   WdgM_Prv_LocalStatusMainFunction
 * Description       :   Update the Local State Machine in WdgM_MainFunction
 * Parameters[in]    :   None
 * Limitations       :   None
 * Return Value      :   None
 ************************************************************************************************************************/
void WdgM_Prv_LocalStatusMainFunction(void)
{
    WdgM_SupervisedEntityIdType SEID = WDGM_PRV_C_ZERO;
#if((WDGM_RTE_DISABLED) == (STD_OFF))
    Rte_PortHandle_WdgM_LocalMode_P PortHandle = Rte_Ports_WdgM_LocalMode_P();
#endif /*(WDGM_RTE_DISABLED == STD_OFF)*/
    WdgM_LocalStatusType NewLocalStatusCache = WDGM_PRV_C_ZERO;
    WdgM_LocalStatusType OldLocalStatusCache = WDGM_PRV_C_ZERO;
    for(SEID = (WdgM_SupervisedEntityIdType)(WDGM_PRV_C_ZERO); SEID < WDGM_NO_OF_SUPERVISED_ENTITIES; SEID++)
    {
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
         SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
        /* TRACE[WDGM206] : Transition 2, 6 */
        if((WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & WDGM_LOCAL_STATUS_EXPIRED) == WDGM_LOCAL_STATUS_EXPIRED)
        {
            /*No lock: Here the Local status is changed to worst case ie expired*/
            WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WDGM_LOCAL_STATUS_EXPIRED;

            if(WdgM_FirstExpiredSupervisedEntityId == WdgM_FirstExpiredSupervisedEntityId_Comp) /* First Expiry */
            {
                WdgM_FirstExpiredSupervisedEntityId = SEID; /* WDGM351 */
                WdgM_FirstExpiredSupervisedEntityId_Comp = WdgM_Prv_ComplementSeId_to_Inl(SEID); /* WDGM351 */
            }
        }
        NewLocalStatusCache =   WdgM_SupervisedEntityDyn[SEID].NewLocalStatus;
        OldLocalStatusCache =   WdgM_SupervisedEntityDyn[SEID].OldLocalStatus;
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
        SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
        /* TRACE[WDGM201] Transition: 1 : If supervision status is correct and local status is OK leave in OK State */
        if (NewLocalStatusCache != OldLocalStatusCache)
        {
            /*No Lock required since we cache and work on calculated status*/
            WdgM_SupervisedEntityDyn[SEID].OldLocalStatus = NewLocalStatusCache;
#if((WDGM_RTE_DISABLED) == (STD_OFF))
            /* TRACE[WDGM197] */
            switch (NewLocalStatusCache)
            {
                case WDGM_LOCAL_STATUS_OK:
                    PortHandle[SEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_OK);
                    break;
                    /* TRACE[WDGM225] */
                case WDGM_LOCAL_STATUS_FAILED:
                    PortHandle[SEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_FAILED);
#if((WDGM_PARTITION_ENABLED) == (STD_ON))
                    if(WdgM_SupervisedEntity[SEID].PartionEnabled == TRUE)
                    {
                        BswM_WdgM_RequestPartitionReset(WdgM_SupervisedEntity[SEID].OsApplicationId);
                    }
#endif
                    break;
                case WDGM_LOCAL_STATUS_EXPIRED:
                    PortHandle[SEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_EXPIRED);
                    break;
                case WDGM_LOCAL_STATUS_DEACTIVATED:
                    PortHandle[SEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_DEACTIVATED);
                    break;
                default:
                    /* Default clause is empty by design. Every relevant case is implemented.*/
                    break;
            }
#else
#if((WDGM_PARTITION_ENABLED) == (STD_ON))
            if((NewLocalStatusCache == WDGM_LOCAL_STATUS_FAILED)&&(WdgM_SupervisedEntity[SEID].PartionEnabled == TRUE))
            {
                BswM_WdgM_RequestPartitionReset(WdgM_SupervisedEntity[SEID].OsApplicationId);
            }
#endif
#endif
        }/* if (WdgM_SupervisedEntityDyn[SEID].NewLocalStatus != WdgM_SupervisedEntityDyn[SEID].OldLocalStatus) */
    }
}

#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"


#define WDGM_START_SEC_CODE_FAST
#include "WdgM_MemMap.h"
#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))

/************************************************************************************************************************
 * Name              :   WdgM_Rb_FirstExpiredEntity
 * Description       :   Stores the info of first expired entity in a structure.
 * Parameters[in]    :   ElapsedCounterValue, CheckpointID, AdditionalCheckpointID, SEID, AdditionalSEID, ExpiredLocation, ModeId
 * Limitations       :   None
 * Return Value      :   void
 ************************************************************************************************************************/
void WdgM_Rb_FirstExpiredEntity( TickType ElapsedCounterValue, uint16 CheckpointID,
        uint16 AdditionalCheckpointID, WdgM_SupervisedEntityIdType SEID, WdgM_SupervisedEntityIdType AdditionalSEID,
        uint8 ExpiredLocation, WdgM_ModeType ModeId)
{
    SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
    if(WdgM_Rb_FirstExpiredEntity_Cnt_u8 == WDGM_PRV_C_ONE)
    {
        WdgM_Rb_FirstExpiredEntityInfo_st.FirstElapsedCounterValue                     = ElapsedCounterValue;
        WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredCheckpointId_t                   = CheckpointID;
        WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredAdditionalCheckpointId           = AdditionalCheckpointID;
        WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredSupervisedEntityId_t             = SEID;
        WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredAdditionalSupervisedEntityId     = AdditionalSEID;
        WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredLocation_t                       = ExpiredLocation;
        WdgM_Rb_FirstExpiredEntityInfo_st.FirstExpiredModeId                           = ModeId;
        WdgM_Rb_FirstExpiredEntity_Cnt_u8 = WDGM_PRV_C_TWO;
    }
    SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
    return;

}
#endif /*(WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON)*/
#define WDGM_STOP_SEC_CODE_FAST
#include "WdgM_MemMap.h"
