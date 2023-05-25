

#ifndef WDGM_CFG_H
#define WDGM_CFG_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

/*[WDGMFCK] The module shall include SchM_WdgM_Type.h*/
#include "SchM_WdgM_Type.h"
#include "Os.h"
/*
#if (!defined(OS_AR_RELEASE_MAJOR_VERSION) || (OS_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(OS_AR_RELEASE_MINOR_VERSION) || (OS_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/

#include "Std_Types.h"
/*
#if (!defined(STD_TYPES_AR_RELEASE_MAJOR_VERSION) || (STD_TYPES_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(STD_TYPES_AR_RELEASE_MINOR_VERSION) || (STD_TYPES_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/


#include "Det.h"
/*
#if (!defined(DET_AR_RELEASE_MAJOR_VERSION) || (DET_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(DET_AR_RELEASE_MINOR_VERSION) || (DET_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/




#include "WdgIf.h"
/*
#if (!defined(WDGIF_AR_RELEASE_MAJOR_VERSION) || (WDGIF_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(WDGIF_AR_RELEASE_MINOR_VERSION) || (WDGIF_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/

/*
 *********************************************************************************************************
 * Defines/Macros
 *********************************************************************************************************
*/

#define WDGM_VARIANT_PRE_COMPILE                                     (1)
#define WDGM_DEADLINE_TIMEOUT_DETECT                                 (STD_ON)
#define WDGM_DEV_ERROR_DETECT                                        (STD_ON)
#define WDGM_RTE_DISABLED                                            (STD_OFF)
#define WDGM_RB_DEBUG_OPTION_ENABLED                                 (STD_OFF)
#define WDGM_DEM_ALIVE_SUPERVISION_REPORT                            (STD_OFF)
#define WDGM_IMMEDIATE_RESET                                         (STD_OFF)
#define WDGM_OFF_MODE_ENABLED                                        (STD_OFF)
#define WDGM_VERSION_INFO_API                                        (STD_OFF)
#define WDGM_PRV_C_ZERO                                              (0)
#define WDGM_PRV_C_ONE                                               (1)
#define WDGM_PRV_C_TWO                                               (2)
#define WDGM_INVALID_WDGIF_DEVICE_INDEX                              (2)
#define WDGM_INVALID_DEM_EVENT_ID                                    (0)
#define WDGM_INVALID_TIMER_ID                                        ((CounterType)(0))
#define WDGM_PARTITION_ENABLED                                       (STD_OFF)

/* Swtich to check if AR45 is enabled */
#define WdgM_AR45_VERSION                                            (STD_OFF)


#define WDGM_NO_OF_WATCHDOGS                                         (2)
#define WDGM_NO_OF_SUPERVISED_ENTITIES                               ((WdgM_SupervisedEntityIdType)(1))
#define WDGM_NO_OF_CHECKPOINTS                                       ((WdgM_CheckpointIdType)(5))
#define WDGM_NO_OF_DISTINCT_CHECKPOINTS                              ((WdgM_CheckpointIdType)(5))
#define WDGM_MAX_DEADLINE_SUPERVISIONS                               (1)

#define WDGM_MAX_EXTERNAL_GRAPH                                      (0)
#define WDGM_MAX_INTERNAL_GRAPH                                      (1)




/*Enable this to STD_ON if your use case does not involve WdgM_MainFunction being pre-empted by WdgM_SetMode vice versa.*/
#define WDGM_LOCK_DISABLE_FOR_DGS                                    (STD_ON)


/* Symbolic names for Supervised Entities */
#define WdgMConf_WdgMSupervisedEntity_WdgM_SE1                       ((WdgM_SupervisedEntityIdType)(0))

/* Symbolic names for CheckPoint Identifiers */
#define WdgMConf_WdgMCheckpoint_WdgM_SE1_Alive_CP                    ((WdgM_CheckpointIdType)(0))
#define WdgMConf_WdgMCheckpoint_WdgM_SE1_Deadline_CP_0               ((WdgM_CheckpointIdType)(1))
#define WdgMConf_WdgMCheckpoint_WdgM_SE1_Deadline_CP_1               ((WdgM_CheckpointIdType)(2))
#define WdgMConf_WdgMCheckpoint_WdgM_SE1_PFM_CP_0                    ((WdgM_CheckpointIdType)(3))
#define WdgMConf_WdgMCheckpoint_WdgM_SE1_PFM_CP_1                    ((WdgM_CheckpointIdType)(4))


/* Symbolic names for Watchdog instances */
#define WdgMConf_WdgMWatchdog_WdgMWatchdog_Ext                       (WdgIfConf_WdgIfDevice_WdgIfDevice_Ext)
#define WdgMConf_WdgMWatchdog_WdgMWatchdog_Int                       (WdgIfConf_WdgIfDevice_WdgIfDevice_Int)

/* WdgMSupervisionCycle Definitions [ms] */
#define WDGM_INVALID_SUPERVISION_CYCLE                               ((uint32)(0))
#define WDGM_SUPERVISION_CYCLE_0                                     ((uint32)(1))

/* Timer interface selection for Deadline supervision */
#define     WDGM_RB_DEADLINE_TIMER_SELECTION_NONE     (0)
#define     WDGM_RB_DEADLINE_TIMER_SELECTION_MCU      (1)
#define     WDGM_RB_DEADLINE_TIMER_SELECTION_OS       (2)


#define WDGM_RB_DEADLINE_TIMER_SELECTION WDGM_RB_DEADLINE_TIMER_SELECTION_MCU



#define WDGM_INTERNAL_GRAPH_CONFIGURED  STD_ON


#define WDGM_EXTERNAL_GRAPH_CONFIGURED  STD_OFF

#define WdgMConf_WdgMMode_WdgMMode_FastMode                             ((WdgM_ModeType)(0))
#define WdgMConf_WdgMMode_WdgMMode_SlowMode                             ((WdgM_ModeType)(1))
#define WDGM_INVALID_MODE_INDEX                                        ((WdgM_ModeType)(2))
/*
***************************************************************************************************
* Type definitions
***************************************************************************************************
*/



typedef struct
{
    uint16 NoOfDeadlineSupervision;
    uint16 * PtrToDeadlineIndices;
}WdgM_CheckpointDynType;

typedef struct
{
    WdgM_CheckpointDynType * const PtrToCheckpointDyn;
}WdgM_CheckpointType;

typedef struct
{
    uint8 FailedAliveSupervisionRefCycleCtr;              /* To track the failed reference cycles of SE*/
    uint8 FailedAliveSupervisionRefCycleTol;   /* Configuration Value to be copied at Mode change of WdgM from WdgM_LocalStatusParams. */
    uint16 IndividualSupervisionCycleCtr;        /* To track the supervision cycles of SE*/
    uint32 IndividualAliveUpdateCtr;    /* To track the alivecounter of SE*/
    uint16 AliveSupervisionIdx;
    WdgM_LocalStatusType NewLocalStatus;
    WdgM_LocalStatusType OldLocalStatus;          /* Only to be updated in WdgM_MainFunction after current Monitoring Status is reported to RTE */
}WdgM_SupervisedEntityDynType;

typedef struct
{
    uint16 NoOfCheckpoint;
    boolean PartionEnabled;
    CounterType TimerId;
    ApplicationType OsApplicationId;
    const WdgM_CheckpointType * PtrToCheckpoint;
    WdgM_SupervisedEntityDynType * const PtrToSupervisedEntityDyn;
#if WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON
    boolean hasInternalGraph;    /* True means it has Internal Graph, False means it does not have Internal Graph*/
    uint16 idxInternalGraphCPProperty;
#endif    
}WdgM_SupervisedEntityType;

/*TRACE[WDGM085] All the resources required for Alive Monitoring */
typedef struct
{
    uint16 MinMargin;
    uint16 MaxMargin;
    WdgM_CheckpointIdType AliveSupervisionCheckpointId;
    WdgM_SupervisedEntityIdType SupervisedEntityId;
    uint16 ExpectedAliveIndications;
    uint16 SupervisionReferenceCycle;
}WdgM_AliveSupervisionType;

/*TRACE[WDGM293] All the resources required for Deadline Monitoring */
typedef struct
{
    WdgM_CheckpointIdType StartCheckpointId;
    WdgM_CheckpointIdType StopCheckpointId;
    WdgM_SupervisedEntityIdType SupervisedEntityId;
    TickType DeadlineMin;
    TickType DeadlineMax;
}WdgM_DeadlineSupervisionType;

typedef struct
{
    uint8 FailedAliveSupervisionRefCycleTol;
    WdgM_SupervisedEntityIdType SupervisedEntityId;
}WdgM_LocalStatusParamsType;

typedef struct
{
    uint16 TriggerConditionValue;
    uint8 DeviceIdx;
    WdgIf_ModeType WdgMode;
}WdgM_TriggerType;

typedef enum
{
    WDGM_EXTERNALPOSNGRAPH_INITIAL_E = 1,            // Initial checkpoint (value 1)
    WDGM_EXTERNALPOSNGRAPH_FINAL_E,                 // Final checkpoint (value 2)
    WDGM_EXTERNALPOSNGRAPH_INTERMEDIATE_E           // Intermediate checkpoint (value 3)
} WdgM_ExternalPositionInGraph_ten;

typedef struct
{
    WdgM_ExternalPositionInGraph_ten      SourcePosnInGraph_en;
    WdgM_SupervisedEntityIdType           SourceSEId;
    WdgM_CheckpointIdType                 SourceCPId;
    WdgM_SupervisedEntityIdType           DestSEId;
    WdgM_CheckpointIdType                 DestCPId;
    WdgM_ExternalPositionInGraph_ten      DestPosnInGraph_en;
    uint16                                ExternalGraphId;
}WdgM_ExternalGraphType;

#if (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
typedef struct
{
    boolean                      activityFlag;
    WdgM_SupervisedEntityIdType  lastReachedSupervisedEntityId;
    WdgM_CheckpointIdType        lastReachedCheckpointId;
}WdgM_ExternalGraph_StatusType;
#endif 

typedef struct
{
    uint16 ExpiredSupervisionCycleTol;
    Rte_ModeType_WdgMSupervisionCycle SchMWdgMSupervisionCycle;
    uint32 SupervisionCycle;
    uint16 NoOfAliveSupervision;
    uint16 NoOfDeadlineSupervision;
    uint16 NoOfLocalStatusParams;
    uint8 NoOfTrigger;
    const WdgM_AliveSupervisionType * PtrToAliveSupervision;
    const WdgM_DeadlineSupervisionType * PtrToDeadlineSupervision;
    const WdgM_LocalStatusParamsType * PtrToLocalStatusParams;
    const WdgM_TriggerType * PtrToTrigger;  
    uint16 NoOfExternalGraphTransitions;
    const WdgM_ExternalGraphType * PtrToExternalGraph;
}WdgM_PrvModeType;

/* WDGM029 */
typedef struct
{
    WdgM_ModeType InitialMode;
    uint8 NoOfMode;
#if((WDGM_DEM_ALIVE_SUPERVISION_REPORT) == (STD_ON))
    Dem_EventIdType ErrorSupervision;
    Dem_EventIdType ErrorSetMode;
#else /*WDGM_DEM_ALIVE_SUPERVISION_REPORT*/
    uint16 ErrorSupervision;
    uint16 ErrorSetMode;
#endif /*WDGM_DEM_ALIVE_SUPERVISION_REPORT*/
    TickType * const PtrToRunningCounterValue;
    uint16 * const PtrToDeadlineIndices;
    const WdgM_PrvModeType * PtrToMode;
    uint16 * const PtrToExternalGraphsIndices;
}WdgM_ConfigType;

#if (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON)

typedef enum
{
    WDGM_POSNGRAPH_NONE_E = 0,              // Does not belong to graph (value 0)
    WDGM_POSNGRAPH_INITIAL_E,               // Initial checkpoint (value 1)
    WDGM_POSNGRAPH_FINAL_E,                 // Final checkpoint (value 2)
    WDGM_POSNGRAPH_INTERMEDIATE_E           // Intermediate checkpoint (value 3)
} WdgM_PositionInGraph_ten;

typedef struct
{
    WdgM_PositionInGraph_ten    posnInGraph_en;             
    uint16                      nrDestCheckpoints; 
    uint16                      idxDestCheckpoints;
    uint16                      InternalGraphId;
} WdgM_InternalGraph_CPPropertyType;

typedef struct
{
    boolean flgActivity;    // True means graph active, False means graph inactive
    WdgM_CheckpointIdType idLastReachedCheckpoint;
}WdgM_InternalGraph_StatusType;

#endif 

/*
***************************************************************************************************
* External declaration
***************************************************************************************************
*/

#define WDGM_START_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgM_MemMap.h"

extern const WdgM_CheckpointType WdgM_Checkpoint[5];
extern const WdgM_SupervisedEntityType WdgM_SupervisedEntity[1];

extern const WdgM_CheckpointIdType WdgM_InternalGraph_DestCheckpoints[1];
extern const WdgM_InternalGraph_CPPropertyType WdgM_InternalGraph_CPProperty[5];

#define WDGM_STOP_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgM_MemMap.h"



#ifdef WDGM_DBG_TST_ENA 
#define WDGM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
/*
 * TRACE[WDGM241] The For each start Checkpoint, the timestamp when each Checkpoint has
 * been reached shall be available for debugging */
extern TickType WdgM_RunningCounterValue[WDGM_MAX_DEADLINE_SUPERVISIONS];
#define WDGM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_FAST_CLEARED_16
#include "WdgM_MemMap.h"
extern uint16 WdgM_DeadlineIndices[2];
#define WDGM_STOP_SEC_VAR_FAST_CLEARED_16
#include "WdgM_MemMap.h"
#endif





#define WDGM_START_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgM_MemMap.h"
/* Variable declarations from WdgMConfigSet */
extern const WdgM_AliveSupervisionType WdgM_AliveSupervision[2];
extern const WdgM_DeadlineSupervisionType WdgM_DeadlineSupervision[2];
/* no WdgMExternalLogicalSupervision is configured....!!! */
extern const WdgM_LocalStatusParamsType WdgM_LocalStatusParams[2];
extern const WdgM_TriggerType WdgM_Trigger[4];
extern const WdgM_PrvModeType WdgM_PrvMode[2];
extern const WdgM_ConfigType WdgM_Config;
#define WDGM_STOP_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgM_MemMap.h"





#define WDGM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

/*TRACE[WDGM240] The Alive Counters of each Checkpoint shall be available for debugging */
/*TRACE[WDGM241] The Alive Counters of each Checkpoint shall be available for debugging */
/*TRACE[WDGM242] The Alive Counters of each Checkpoint shall be available for debugging */
/*TRACE[WDGM238] The local supervision status of each SE is available for debugging*/

extern WdgM_SupervisedEntityDynType WdgM_SupervisedEntityDyn[1];
extern WdgM_InternalGraph_StatusType WdgM_InternalGraph_StatusDyn[WDGM_MAX_INTERNAL_GRAPH];

#define WDGM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

LOCAL_INLINE WdgM_SupervisedEntityIdType WdgM_Prv_ComplementSeId_to_Inl(WdgM_SupervisedEntityIdType SEID);


/************************************************************************************************************************
* Name              :   WdgM_Prv_ComplementSeId_to_Inl
* Description       :   returns 1's complement of the SEID passed as argument.
* Parameters[in]    :   SEID
* Limitations       :   None
* Return Value      :   WdgM_SupervisedEntityIdType
************************************************************************************************************************/
LOCAL_INLINE WdgM_SupervisedEntityIdType WdgM_Prv_ComplementSeId_to_Inl(WdgM_SupervisedEntityIdType SEID)
{
    return (((WdgM_SupervisedEntityIdType)(0xFF)) - SEID);
}



#endif /* WDGM_CFG_H */

