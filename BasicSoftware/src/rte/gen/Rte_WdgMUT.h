/** @file     Rte_WdgMUT.h
  *
  * @brief    Application header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef RTE_WDGMUT_H
#define RTE_WDGMUT_H

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
#include "Rte_WdgMUT_Type.h"

#include "Rte_DataHandleType.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Alive)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
#elif defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Deadline)
#elif defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Logical)
#elif defined(RTE_RUNNABLEAPI_RE_WdgM_GlobalSupervision)
#elif defined(RTE_RUNNABLEAPI_RE_WdgM_LocalSupervisionStatus)
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
struct Rte_CDS_WdgMUT {
   uint8 Rte_Dummy;
};
typedef struct Rte_CDS_WdgMUT Rte_CDS_WdgMUT;
/* END: SWC types (core visible) */

/*******************************************************
 ***
 *** Private Types
 ***
 *******************************************************/

/* BEGIN: SWC types (private) */
/* END: SWC types (private) */

/*******************************************************
 ***
 *** Instance Declarations
 ***
 *******************************************************/

#define RTE_START_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
extern CONST(struct Rte_CDS_WdgMUT, RTE_CONST) Rte_Inst_WdgMUT;
#define RTE_STOP_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"


#ifndef RTE_CORE
typedef CONSTP2CONST(struct Rte_CDS_WdgMUT, AUTOMATIC, RTE_CONST) Rte_Instance;
#endif /* RTE_CORE */

/*******************************************************
 ***
 *** API Mapping macros
 ***
 *******************************************************/

/* API Mapping Macros */
#ifndef RTE_CORE

#define RTE_START_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
extern CONST(Rte_SelfType_WdgM, RTE_CONST) Rte_Self_CPT_WdgM;
#define RTE_STOP_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
#define WdgM_START_SEC_CODE_FAST
#include "WdgM_MemMap.h"
FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_CheckpointReached(VAR(WdgM_SupervisedEntityIdType, AUTOMATIC) SEID,
                                                            VAR(WdgM_CheckpointIdType, AUTOMATIC) CheckpointID);

FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_GetGlobalStatus(P2VAR(WdgM_GlobalStatusType, AUTOMATIC, RTE_APPL_DATA) Status);

FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_GetLocalStatus(VAR(WdgM_SupervisedEntityIdType, AUTOMATIC) SEID,
                                                         P2VAR(WdgM_LocalStatusType, AUTOMATIC, RTE_APPL_DATA) Status);

FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_GetMode(P2VAR(WdgM_ModeType, AUTOMATIC, RTE_APPL_DATA) Mode);

FUNC(Std_ReturnType, WdgM_CODE_FAST) WdgM_SetMode(VAR(WdgM_ModeType, AUTOMATIC) Mode);

FUNC(void, WdgM_CODE_FAST) WdgM_PerformReset(void);

#define WdgM_STOP_SEC_CODE_FAST
#include "WdgM_MemMap.h"
#define WdgM_START_SEC_CODE_SLOW
#include "WdgM_MemMap.h"
FUNC(Std_ReturnType, WdgM_CODE_SLOW) WdgM_GetFirstExpiredSEID(P2VAR(WdgM_SupervisedEntityIdType, AUTOMATIC, RTE_APPL_DATA) SEID);

#define WdgM_STOP_SEC_CODE_SLOW
#include "WdgM_MemMap.h"
#define RTE_ECUCPARTITION_SYSCORE_START_SEC_VAR_CLEARED_8
#include "Rte_MemMap.h"
extern VAR(uint8, RTE_DATA) Rte_ModeCurrent_11;
#define RTE_ECUCPARTITION_SYSCORE_STOP_SEC_VAR_CLEARED_8
#include "Rte_MemMap.h"
#define RTE_ECUCPARTITION_SYSCORE_START_SEC_VAR_CLEARED_8
#include "Rte_MemMap.h"
extern VAR(uint8, RTE_DATA) Rte_ModeCurrent_12;
#define RTE_ECUCPARTITION_SYSCORE_STOP_SEC_VAR_CLEARED_8
#include "Rte_MemMap.h"
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Alive)
/* Inline function call optimization; Rte_Call_RP_AliveSupervision_ASW_10ms_CheckpointReached to WdgM_CheckpointReached */
#define Rte_Call_RP_AliveSupervision_ASW_10ms_CheckpointReached() WdgM_CheckpointReached(((VAR(WdgM_SupervisedEntityIdType, AUTOMATIC))0), ((VAR(WdgM_CheckpointIdType, AUTOMATIC))0))

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Deadline)
/* Inline function call optimization; Rte_Call_RP_Deadline_Start_ASW_10ms_CheckpointReached to WdgM_CheckpointReached */
#define Rte_Call_RP_Deadline_Start_ASW_10ms_CheckpointReached() WdgM_CheckpointReached(((VAR(WdgM_SupervisedEntityIdType, AUTOMATIC))0), ((VAR(WdgM_CheckpointIdType, AUTOMATIC))1))

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Deadline)
/* Inline function call optimization; Rte_Call_RP_Deadline_Stop_ASW_10ms_CheckpointReached to WdgM_CheckpointReached */
#define Rte_Call_RP_Deadline_Stop_ASW_10ms_CheckpointReached() WdgM_CheckpointReached(((VAR(WdgM_SupervisedEntityIdType, AUTOMATIC))0), ((VAR(WdgM_CheckpointIdType, AUTOMATIC))2))

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Logical)
/* Inline function call optimization; Rte_Call_RP_PFM_CP0_ASW_10ms_CheckpointReached to WdgM_CheckpointReached */
#define Rte_Call_RP_PFM_CP0_ASW_10ms_CheckpointReached() WdgM_CheckpointReached(((VAR(WdgM_SupervisedEntityIdType, AUTOMATIC))0), ((VAR(WdgM_CheckpointIdType, AUTOMATIC))3))

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Logical)
/* Inline function call optimization; Rte_Call_RP_PFM_CP1_ASW_10ms_CheckpointReached to WdgM_CheckpointReached */
#define Rte_Call_RP_PFM_CP1_ASW_10ms_CheckpointReached() WdgM_CheckpointReached(((VAR(WdgM_SupervisedEntityIdType, AUTOMATIC))0), ((VAR(WdgM_CheckpointIdType, AUTOMATIC))4))

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgM_GlobalSupervision)
/* Inline function call optimization; Rte_Call_RP_WdgM_GlobalSupervision_GetFirstExpiredSEID to WdgM_GetFirstExpiredSEID */
#define Rte_Call_RP_WdgM_GlobalSupervision_GetFirstExpiredSEID( SEID ) WdgM_GetFirstExpiredSEID(SEID)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgM_GlobalSupervision)
/* Inline function call optimization; Rte_Call_RP_WdgM_GlobalSupervision_GetGlobalStatus to WdgM_GetGlobalStatus */
#define Rte_Call_RP_WdgM_GlobalSupervision_GetGlobalStatus( Status ) WdgM_GetGlobalStatus(Status)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgM_GlobalSupervision)
/* Inline function call optimization; Rte_Call_RP_WdgM_GlobalSupervision_GetMode to WdgM_GetMode */
#define Rte_Call_RP_WdgM_GlobalSupervision_GetMode( Mode ) WdgM_GetMode(Mode)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgM_GlobalSupervision)
/* Inline function call optimization; Rte_Call_RP_WdgM_GlobalSupervision_PerformReset to WdgM_PerformReset */
#define Rte_Call_RP_WdgM_GlobalSupervision_PerformReset() ( WdgM_PerformReset(), ((VAR(Std_ReturnType, AUTOMATIC))RTE_E_OK) )

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgM_GlobalSupervision)
/* Inline function call optimization; Rte_Call_RP_WdgM_GlobalSupervision_SetMode to WdgM_SetMode */
#define Rte_Call_RP_WdgM_GlobalSupervision_SetMode( Mode ) WdgM_SetMode(Mode)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgM_LocalSupervisionStatus)
/* Inline function call optimization; Rte_Call_RP_WdgM_LocalSupervisionStatus_GetLocalStatus to WdgM_GetLocalStatus */
#define Rte_Call_RP_WdgM_LocalSupervisionStatus_GetLocalStatus( Status ) WdgM_GetLocalStatus(((VAR(WdgM_SupervisedEntityIdType, AUTOMATIC))0), Status)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Alive)
/* Inline read optimization; Rte_Mode_RP_WdgM_GlobalMode_currentMode to direct read */
#define Rte_Mode_RP_WdgM_GlobalMode_currentMode()  ( Rte_ModeCurrent_11 )
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_WdgMUT_ASW_10ms_Alive)
/* Inline read optimization; Rte_Mode_RP_WdgM_WdgM_SE1_Mode_currentMode to direct read */
#define Rte_Mode_RP_WdgM_WdgM_SE1_Mode_currentMode()  ( Rte_ModeCurrent_12 )
#endif

#endif /* RTE_CORE */

/*******************************************************
 ***
 *** RE Prototypes
 ***
 *******************************************************/

#define WdgMUT_START_SEC_CODE
#include "WdgMUT_MemMap.h"
FUNC(void, WdgMUT_CODE) RE_WdgMUT_ASW_10ms_Alive(void);
FUNC(void, WdgMUT_CODE) RE_WdgMUT_ASW_10ms_Deadline(void);
FUNC(void, WdgMUT_CODE) RE_WdgMUT_ASW_10ms_Logical(void);
FUNC(void, WdgMUT_CODE) RE_WdgM_GlobalSupervision(void);
FUNC(void, WdgMUT_CODE) RE_WdgM_LocalSupervisionStatus(void);
#define WdgMUT_STOP_SEC_CODE
#include "WdgMUT_MemMap.h"

/* Initial values for data element prototypes */

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !RTE_WDGMUT_H */