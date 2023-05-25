

#ifndef WDGM_H
#define WDGM_H


/*TRACE[WDGM014] Follow predefined file inculde structure*/
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "WdgM_Cfg.h"
#if(defined(WDGM_VARIANT_POST_BUILD))
#include "WdgM_PBcfg.h"
#endif

#if((WDGM_RTE_DISABLED) == (STD_OFF))
/* TRACE[WDGMFCK] The module header file WdgM.h shall include Rte_WdgM_Type.h */
#include "Rte_WdgM_Type.h"
#endif/*(WDGM_RTE_DISABLED == STD_OFF)*/

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

#define WDGM_AR_RELEASE_MAJOR_VERSION               (4)
#define WDGM_AR_RELEASE_MINOR_VERSION               (2)
#define WDGM_AR_RELEASE_REVISION_VERSION            (2)

#define WDGM_SW_MAJOR_VERSION                       (9)
#define WDGM_SW_MINOR_VERSION                       (0)
#define WDGM_SW_PATCH_VERSION                       (0)

#define WDGM_MODULE_ID                              (13)
#define WDGM_VENDOR_ID                              (6)

/* Interface API IDs for DET interface */
#define  WDGM_INSTANCE_ID                           ((uint8)(0))
#define  WDGM_INIT_APIID                            ((uint8)0x00)
#define  WDGM_DE_INIT_APIID                         ((uint8)0x01)
#define  WDGM_GET_VERSION_INFO_APIID                ((uint8)0x02)
#define  WDGM_SET_MODE_APIID                        ((uint8)0x03)
#define  WDGM_UPDATE_ALIVE_COUNTER_APIID            ((uint8)0x04)
#define  WDGM_CHECKPOINT_REACHED_APIID              ((uint8)0x0e)
#define  WDGM_MAIN_FUNCTION_APIID                   ((uint8)0x08)
#define  WDGM_GET_MODE_APIID                        ((uint8)0x0b)
#define  WDGM_GET_LOCAL_STATUS_APIID                ((uint8)0x0c)
#define  WDGM_GET_GLOBAL_STATUS_APIID               ((uint8)0x0d)
#define  WDGM_PERFORM_RESET_APIID                   ((uint8)0x0f)
#define  WDGM_GET_FIRST_EXPIRED_SEID_APIID          ((uint8)0x10)

/* Det error Ids */
#define  WDGM_E_UNINIT                              ((uint8)(0x10))
#define  WDGM_E_PARAM_CONFIG                        ((uint8)(0x11))
#define  WDGM_E_PARAM_MODE                          ((uint8)(0x12))
#define  WDGM_E_PARAM_SEID                          ((uint8)(0x13))
#define  WDGM_E_INV_POINTER                         ((uint8)(0x14))
#define  WDGM_E_DISABLE_NOT_ALLOWED                 ((uint8)(0x15))
#define  WDGM_E_CPID                                ((uint8)(0x16))
#define  WDGM_E_SEDEACTIVATED                       ((uint8)(0x19))
#define  WDGM_E_NO_DEINIT                           ((uint8)(0x1A))

#define  WDGM_E_OS_COUNTER                          ((uint8)(0x80))
#define  WDGM_E_SCHM_SWITCH                          ((uint8)(0x81))

/* Global Status */
#define WDGM_GLOBAL_STATUS_OK                       ((WdgM_GlobalStatusType)(0))
#define WDGM_GLOBAL_STATUS_FAILED                   ((WdgM_GlobalStatusType)(1))
#define WDGM_GLOBAL_STATUS_EXPIRED                  ((WdgM_GlobalStatusType)(2))
#define WDGM_GLOBAL_STATUS_STOPPED                  ((WdgM_GlobalStatusType)(3))
#define WDGM_GLOBAL_STATUS_DEACTIVATED              ((WdgM_GlobalStatusType)(4))

/* Local Status */
#define WDGM_LOCAL_STATUS_OK                        ((WdgM_LocalStatusType)(0))
#define WDGM_LOCAL_STATUS_FAILED                    ((WdgM_LocalStatusType)(1))
#define WDGM_LOCAL_STATUS_EXPIRED                   ((WdgM_LocalStatusType)(2))
#define WDGM_LOCAL_STATUS_DEACTIVATED               ((WdgM_LocalStatusType)(4))

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
#ifdef WDGM_DBG_TST_ENA
#define WDGM_START_SEC_VAR_FAST_INIT_8
#include "WdgM_MemMap.h"

/* TRACE[WDGM239] Global supervsion status should be available for debugging */
extern WdgM_GlobalStatusType WdgM_GlobalStatus;

#define WDGM_STOP_SEC_VAR_FAST_INIT_8
#include "WdgM_MemMap.h"
#endif

/* API-Functions Prototype Declaration */
#if(((defined(WDGM_PRV_AVOID_RTE_PROVIDED_FUNC_DECLARATION)) && ((WDGM_RTE_DISABLED) == (STD_ON))) || (!defined(WDGM_PRV_AVOID_RTE_PROVIDED_FUNC_DECLARATION)))
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"

extern void WdgM_MainFunction(void);

#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE_FAST
#include "WdgM_MemMap.h"
extern Std_ReturnType WdgM_CheckpointReached(WdgM_SupervisedEntityIdType SEID, WdgM_CheckpointIdType CheckpointID);
extern Std_ReturnType WdgM_GetGlobalStatus(WdgM_GlobalStatusType * Status);
extern Std_ReturnType WdgM_GetLocalStatus(WdgM_SupervisedEntityIdType SEID, WdgM_LocalStatusType * Status);
extern Std_ReturnType WdgM_GetMode (WdgM_ModeType * Mode);
extern void WdgM_PerformReset(void);
extern Std_ReturnType WdgM_SetMode(WdgM_ModeType Mode);
#define WDGM_STOP_SEC_CODE_FAST
#include "WdgM_MemMap.h"
#endif

#define WDGM_START_SEC_CODE_SLOW
#include "WdgM_MemMap.h"
#if(((defined(WDGM_PRV_AVOID_RTE_PROVIDED_FUNC_DECLARATION)) && ((WDGM_RTE_DISABLED) == (STD_ON))) || (!defined(WDGM_PRV_AVOID_RTE_PROVIDED_FUNC_DECLARATION)))
extern Std_ReturnType WdgM_GetFirstExpiredSEID(WdgM_SupervisedEntityIdType * SEID);
#endif
extern void WdgM_Init(const WdgM_ConfigType * ConfigPtr);
extern void WdgM_DeInit(void);
#if ( (WDGM_VERSION_INFO_API) == (STD_ON) )
extern void WdgM_GetVersionInfo(Std_VersionInfoType * VersionInfo);
#endif /*(WDGM_VERSION_INFO_API) == (STD_ON)*/

#define WDGM_STOP_SEC_CODE_SLOW
#include "WdgM_MemMap.h"

#endif /*WDGM_H*/
