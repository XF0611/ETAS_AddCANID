/** @file     Rte_WdgM.h
  *
  * @brief    Application header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef RTE_WDGM_H
#define RTE_WDGM_H

#ifndef RTE_CORE
#ifdef RTE_APPLICATION_HEADER_FILE
#error Multiple application header files included.
#endif /* RTE_APPLICATION_HEADER_FILE */
#define RTE_APPLICATION_HEADER_FILE
#endif /* RTE_CORE */

/*******************************************************
 ***
 *** Includes
 ***
 *******************************************************/

#include "Rte.h"
#include "Rte_Intl.h"
#include "Rte_WdgM_Type.h"

#include "Rte_DataHandleType.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_CheckpointReached)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
#elif defined(RTE_RUNNABLEAPI_GetFirstExpiredSEID)
#elif defined(RTE_RUNNABLEAPI_GetGlobalStatus)
#elif defined(RTE_RUNNABLEAPI_GetLocalStatus)
#elif defined(RTE_RUNNABLEAPI_GetMode)
#elif defined(RTE_RUNNABLEAPI_MainFunction)
#elif defined(RTE_RUNNABLEAPI_PerformReset)
#elif defined(RTE_RUNNABLEAPI_SetMode)
#else
   /* None of above defined so define value including all APIs (rte_sws_2751) */
   #define RTE_PRV_ALL_API
#endif

/*******************************************************
 ***
 *** Constants
 ***
 *******************************************************/

#ifndef RTE_CORE
#define RTE_E_WdgM_GlobalSupervision_E_NOT_OK ((Std_ReturnType)1)
#define RTE_E_WdgM_GlobalSupervision_E_OK ((Std_ReturnType)0)
#define RTE_E_WdgM_LocalSupervisionStatus_E_NOT_OK ((Std_ReturnType)1)
#define RTE_E_WdgM_LocalSupervisionStatus_E_OK ((Std_ReturnType)0)
#define RTE_E_WdgM_LocalSupervision_E_NOT_OK ((Std_ReturnType)1)
#define RTE_E_WdgM_LocalSupervision_E_OK ((Std_ReturnType)0)
#endif /* RTE_CORE */

/*******************************************************
 ***
 *** Public Types
 ***
 *******************************************************/

/* BEGIN: SWC types (core visible) */
struct Rte_PDS_WdgM_WdgM_LocalMode_P {
   Rte_SwitchAckFP_WdgM_WdgM_LocalMode_currentMode SwitchAck_currentMode;
   Rte_SwitchFP_WdgM_WdgM_LocalMode_currentMode    Switch_currentMode;
};
typedef struct Rte_PDS_WdgM_WdgM_LocalMode_P Rte_PDS_WdgM_WdgM_LocalMode_P;
typedef struct Rte_PDS_WdgM_WdgM_LocalMode_P Rte_PDS_WdgM_WdgM_LocalMode_PA[1];
struct Rte_CDS_WdgM {
   /* Port Data Structure Arrays */
   Rte_PDS_WdgM_WdgM_LocalMode_PA mode_WdgM_SE1;
};
typedef struct Rte_CDS_WdgM Rte_CDS_WdgM;
/* END: SWC types (core visible) */

/*******************************************************
 ***
 *** Private Types
 ***
 *******************************************************/

/* BEGIN: SWC types (private) */
#ifndef RTE_CORE
typedef P2CONST(struct Rte_PDS_WdgM_WdgM_LocalMode_P, AUTOMATIC, RTE_CONST) Rte_PortHandle_WdgM_LocalMode_P;
#endif /* RTE_CORE */

/* END: SWC types (private) */

/*******************************************************
 ***
 *** Instance Declarations
 ***
 *******************************************************/

#define RTE_START_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
extern CONST(struct Rte_CDS_WdgM, RTE_CONST) Rte_Inst_WdgM;
#define RTE_STOP_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"


#ifndef RTE_CORE
typedef CONSTP2CONST(struct Rte_CDS_WdgM, AUTOMATIC, RTE_CONST) Rte_Instance;
#endif /* RTE_CORE */

/*******************************************************
 ***
 *** API Mapping macros
 ***
 *******************************************************/

/* API Mapping Macros */
#ifndef RTE_CORE

#define RTE_WDGM_START_SEC_CODE
#include "Rte_MemMap.h"
FUNC(Std_ReturnType, RTE_CODE) Rte_SwitchAck_WdgM_globalMode_currentMode(void);
FUNC(Std_ReturnType, RTE_CODE) Rte_SwitchAck_WdgM_mode_WdgM_SE1_currentMode(void);
FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_WdgM_globalMode_currentMode(VAR(uint8, AUTOMATIC) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_WdgM_mode_WdgM_SE1_currentMode(VAR(uint8, AUTOMATIC) data);
#define RTE_WDGM_STOP_SEC_CODE
#include "Rte_MemMap.h"
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_MainFunction)
#define Rte_SwitchAck_globalMode_currentMode Rte_SwitchAck_WdgM_globalMode_currentMode /* enableTakeAddress */
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_MainFunction)
#define Rte_SwitchAck_mode_WdgM_SE1_currentMode() (Rte_SwitchAck_WdgM_mode_WdgM_SE1_currentMode())
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_MainFunction)
#define Rte_Switch_globalMode_currentMode Rte_Switch_WdgM_globalMode_currentMode /* enableTakeAddress */
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_MainFunction)
#define Rte_Switch_mode_WdgM_SE1_currentMode( data ) (Rte_Switch_WdgM_mode_WdgM_SE1_currentMode(data))
#endif

/* Rte_Port API */
#define Rte_Port_mode_WdgM_SE1() ((Rte_PortHandle_WdgM_LocalMode_P)&(Rte_Inst_WdgM.mode_WdgM_SE1[0]))

/* Rte_NPorts API */
#define Rte_NPorts_WdgM_LocalMode_P() ((uint8)1)

/* Rte_Ports API */
#define Rte_Ports_WdgM_LocalMode_P() ((Rte_PortHandle_WdgM_LocalMode_P)&(Rte_Inst_WdgM.mode_WdgM_SE1[0]))

#endif /* RTE_CORE */

/*******************************************************
 ***
 *** RE Prototypes
 ***
 *******************************************************/

#define WdgM_START_SEC_CODE_FAST
#include "WdgM_MemMap.h"
FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_CheckpointReached(VAR(WdgM_SupervisedEntityIdType, AUTOMATIC) SEID,
                                                            VAR(WdgM_CheckpointIdType, AUTOMATIC) CheckpointID);
FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_GetGlobalStatus(P2VAR(WdgM_GlobalStatusType, AUTOMATIC, RTE_APPL_DATA) Status);
FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_GetLocalStatus(VAR(WdgM_SupervisedEntityIdType, AUTOMATIC) SEID,
                                                         P2VAR(WdgM_LocalStatusType, AUTOMATIC, RTE_APPL_DATA) Status);
FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_GetMode(P2VAR(WdgM_ModeType, AUTOMATIC, RTE_APPL_DATA) Mode);
FUNC(void, WdgM_CODE_FAST) WdgM_PerformReset(void);
FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_SetMode(VAR(WdgM_ModeType, AUTOMATIC) Mode);
#define WdgM_STOP_SEC_CODE_FAST
#include "WdgM_MemMap.h"
#define WdgM_START_SEC_CODE_SLOW
#include "WdgM_MemMap.h"
FUNC(Std_ReturnType, WdgM_CODE_SLOW) WdgM_GetFirstExpiredSEID(P2VAR(WdgM_SupervisedEntityIdType, AUTOMATIC, RTE_APPL_DATA) SEID);
#define WdgM_STOP_SEC_CODE_SLOW
#include "WdgM_MemMap.h"

/* Initial values for data element prototypes */

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !RTE_WDGM_H */