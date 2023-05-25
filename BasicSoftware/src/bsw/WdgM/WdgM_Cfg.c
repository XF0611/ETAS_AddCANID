

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Os.h"
/*
#if (!defined(OS_AR_RELEASE_MAJOR_VERSION) || (OS_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(OS_AR_RELEASE_MINOR_VERSION) || (OS_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/
#include "WdgM_Prv.h"

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */


#define WDGM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
#ifdef WDGM_DBG_TST_ENA
TickType WdgM_RunningCounterValue[WDGM_MAX_DEADLINE_SUPERVISIONS];
#else
static TickType WdgM_RunningCounterValue[WDGM_MAX_DEADLINE_SUPERVISIONS];
#endif
#define WDGM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_FAST_CLEARED_16
#include "WdgM_MemMap.h"
#ifdef WDGM_DBG_TST_ENA
uint16 WdgM_DeadlineIndices[2];
#else
static uint16 WdgM_DeadlineIndices[2];
#endif
#define WDGM_STOP_SEC_VAR_FAST_CLEARED_16
#include "WdgM_MemMap.h"




/* There is no External graph configured in valid WdgMMode so corresponding Variables, data types are not defined....!!! */



/*
 ***************************************************************************************************
 * Dynamic Variables
 ***************************************************************************************************
 */
#define WDGM_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
/* TRACE[WDGM200] Values of local supervsion should be described in WdgM_LocalStatusType */
WdgM_SupervisedEntityDynType WdgM_SupervisedEntityDyn[1];  /* WDGM242 */
static WdgM_CheckpointDynType WdgM_CheckpointDyn[5];

WdgM_InternalGraph_StatusType WdgM_InternalGraph_StatusDyn[WDGM_MAX_INTERNAL_GRAPH];

#define WDGM_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

#define WDGM_START_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgM_MemMap.h"

const WdgM_CheckpointType WdgM_Checkpoint[5] =
{   
    /* PtrToCheckpointDyn                          Comment */ 
    
    {   &WdgM_CheckpointDyn[0]                       /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 0 */        },
    {   &WdgM_CheckpointDyn[1]                       /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 1 */        },
    {   &WdgM_CheckpointDyn[2]                       /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 2 */        },
    {   &WdgM_CheckpointDyn[3]                       /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 3 */        },
    {   &WdgM_CheckpointDyn[4]                       /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 4 */        }
};


const WdgM_SupervisedEntityType WdgM_SupervisedEntity[1]=
{   
    /* NoOfCheckpoint             PartionEnabled             TimerId                                        OsApplicationId                                PtrToCheckpoint                                PtrToSupervisedEntityDyn                        hasInternalGraph           idxInternalGraphCPProperty                       Comment */         
    
    {  5                        ,  FALSE                    ,  WDGM_INVALID_TIMER_ID                        ,  INVALID_OSAPPLICATION                        ,  &WdgM_Checkpoint[0]                          ,   &WdgM_SupervisedEntityDyn[0]                 ,  TRUE                     ,  0                            /* WdgMSupervisedEntityId: 0 */              }
};

const WdgM_InternalGraph_CPPropertyType WdgM_InternalGraph_CPProperty[5]=
{   
    /* PosinInternalGraph           noofdestCP                      idxofdestCP                     intgraphIdx                     Comment */           
    
    {  WDGM_POSNGRAPH_NONE_E         , 0                             , 0                             ,0                             /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 0 */        } ,
    {  WDGM_POSNGRAPH_NONE_E         , 0                             , 0                             ,0                             /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 1 */        } ,
    {  WDGM_POSNGRAPH_NONE_E         , 0                             , 0                             ,0                             /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 2 */        } ,
    {  WDGM_POSNGRAPH_INITIAL_E      , 1                             , 0                             ,0                             /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 3 */        } ,
    {  WDGM_POSNGRAPH_FINAL_E        , 0                             , 0                             ,0                             /* WdgMSupervisedEntityId: 0, WdgMCheckpointId: 4 */        } 
};

const WdgM_CheckpointIdType WdgM_InternalGraph_DestCheckpoints[1]=
{   
    /* DestCPID                           Comment */ 
    4                              /* WdgMSupervisedEntityId: 0, WdgMSourceCheckpointId: 3 */    
};
const WdgM_AliveSupervisionType WdgM_AliveSupervision[2] =
{
    /* MinMargin          MaxMargin                  AliveSupervisionCheckpointId     SupervisedEntityId              ExpectedAliveIndications   SupervisionReferenceCycle   Comment */ 
   
    {   1                   ,  0                   ,  0                              ,  0                             ,  10                      ,  100                      /* WdgMMode: 0  AliveSuperVision: WdgMAliveSupervision_ASW_10ms */              },   
    {   1                   ,  0                   ,  0                              ,  0                             ,  10                      ,  1000                     /* WdgMMode: 1  AliveSuperVision: WdgMAliveSupervision_ASW_10ms */              }
};


const WdgM_DeadlineSupervisionType WdgM_DeadlineSupervision[2] =
{
    /* StartCheckpointId                StopCheckpointId                 SupervisedEntityId(WDGM313)     DeadlineMin(in Counter ticks)   DeadlineMax(in Counter ticks)   Comment */                                    
    
    {   1                              ,  2                              ,  0                             ,  0                            ,  1000000                      /* WdgMMode: 0  DeadlineSuperVision: WdgMDeadline_Supervision_ASW_10ms */       },    
    {   1                              ,  2                              ,  0                             ,  0                            ,  100000000                    /* WdgMMode: 1  DeadlineSuperVision: WdgMDeadline_Supervision_ASW_10ms */       }        
};


const WdgM_LocalStatusParamsType WdgM_LocalStatusParams[2] =
{
    /* FailedAliveSupervisionRefCycleTol               SupervisedEntityId                              Comment */                                    
    
    {   0                                            ,  0                                              /* WdgMMode: 0  WdgMLocalStatusParams: WdgMLocalStatusParams_SE1 */             },    
    {   0                                            ,  0                                              /* WdgMMode: 1  WdgMLocalStatusParams: WdgMLocalStatusParams_SE1 */             }  
};


const WdgM_TriggerType WdgM_Trigger[4] =
{
    /* TriggerConditionValue(in mili seconds)          DeviceIdx                                       TriggerModeType             Comment */                                    
    
    {   40                                           ,  WdgIfConf_WdgIfDevice_WdgIfDevice_Ext        ,  WDGIF_FAST_MODE            /* WdgMMode: 0  WdgMTrigger: WdgMTrigger_FastMode_ExtWdg */                     },
    {   100                                          ,  WdgIfConf_WdgIfDevice_WdgIfDevice_Int        ,  WDGIF_FAST_MODE            /* WdgMMode: 0  WdgMTrigger: WdgMTrigger_FastMode_IntWdg */                     },    
    {   100                                          ,  WdgIfConf_WdgIfDevice_WdgIfDevice_Ext        ,  WDGIF_SLOW_MODE            /* WdgMMode: 1  WdgMTrigger: WdgMTrigger_SlowMode_ExtWdg */                     },
    {   1000                                         ,  WdgIfConf_WdgIfDevice_WdgIfDevice_Int        ,  WDGIF_SLOW_MODE            /* WdgMMode: 1  WdgMTrigger: WdgMTrigger_SlowMode_IntWdg */                     } 
};

/* No WdgMExternalTransition configured for External Graph so corresponding code is not generated. */


const WdgM_PrvModeType WdgM_PrvMode[2] =
{       /* ExpiredSupervisionCycleTol                  SchMWdgMSupervisionCycle                                 SupervisionCycle(in mili seconds)               NoOfAliveSupervision                            NoOfDeadlineSupervision                         NoOfLocalStatusParams                           NoOfTrigger                                     PtrToAliveSupervision                           PtrToDeadlineSupervision                        PtrToLocalStatusParams                          PtrToTrigger                                    NoOfExternalGraphTransition                     PtrToExternalGraphTransition                    Comment */                                    
    
    {   0                                            ,  RTE_MODE_WdgMSupervisionCycle_SUPERVISION_CYCLE_0      ,  WDGM_SUPERVISION_CYCLE_0                     ,  1                                            ,  1                                            ,  1                                            ,  2                                            ,  &WdgM_AliveSupervision[0]                    ,  &WdgM_DeadlineSupervision[0]                 ,  &WdgM_LocalStatusParams[0]                   ,  &WdgM_Trigger[0]                             ,  0                                            , NULL_PTR                                       /* WdgMMode: WdgMMode_FastMode */            },
    {   0                                            ,  RTE_MODE_WdgMSupervisionCycle_SUPERVISION_CYCLE_0      ,  WDGM_SUPERVISION_CYCLE_0                     ,  1                                            ,  1                                            ,  1                                            ,  2                                            ,  &WdgM_AliveSupervision[1]                    ,  &WdgM_DeadlineSupervision[1]                 ,  &WdgM_LocalStatusParams[1]                   ,  &WdgM_Trigger[2]                             ,  0                                            , NULL_PTR                                       /* WdgMMode: WdgMMode_SlowMode */            }
};

const WdgM_ConfigType WdgM_Config =
{
    1                                                           , /* InitialMode */
    2                                                           , /* NoOfMode */
    WDGM_INVALID_DEM_EVENT_ID                                   , /* ErrorSupervision */
    WDGM_INVALID_DEM_EVENT_ID                                   , /* ErrorSetMode */ 
    &WdgM_RunningCounterValue[0]                                , /* PtrToRunningCounterValue */
    &WdgM_DeadlineIndices[0]                                    , /* PtrToDeadlineIndices */
    &WdgM_PrvMode[0]                                            ,  /* PtrToMode */
    NULL_PTR                                                      /* PtrToExternalGraphIndices */
};


#define      WDGM_STOP_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgM_MemMap.h"

