/** @file     Rte_Const.h
  *
  * @brief    RTA-RTE related configuration constants
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef RTE_CONST_H
#define RTE_CONST_H

#define OSENV_RTAOS40                                                  (1)
#define RTE_CALPRM_NONE                                                (1)
#define RTE_COMPATIBILITY_MODE                                         (1)
#define RTE_MAINFUNCTION_PERIOD_MS                                     (10)
#define RTE_MAINFUNCTION_PERIOD_NS                                     (10000000)
#define RTE_MAINFUNCTION_PERIOD_US                                     (10000)
#define RTE_MAX_USED_CORE_ID                                           (0)
#define RTE_MAX_USED_OSAPP_ID                                          (3)
#define RTE_MEASUREMENT_ENABLED                                        (1)
#define RTE_NUM_TASKS                                                  (11)
#define RTE_NUM_TIMEOUT_ALARMS                                         (0)
#define RTE_OSAPI_AUTOSAR_R40                                          (1)
#define RTE_OSCFG_AUTOSAR_R40                                          (1)
#define RTE_PERIODIC_COUNTER_TICK_INTERVAL_US                          (1000)
#define RTE_REQUIRES_COM                                               (1)
#define RTE_REQUIRES_IOC                                               (1)
#define RTE_REQUIRES_NVM                                               (1)
#define RTE_RIPS_SUPPORT                                               (0)
#define RTE_TASKS_ARRAYSIZE                                            (11)
#define RTE_WOWP_EVENTS                                                (0)
#define SCHM_IMODEINIT                                                 (1)
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_BswM         CPT_BswM
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_CalUT        CPT_CalUT
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_ComM         CPT_ComM
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_Dcm          CPT_Dcm
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_Dem          CPT_Dem
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_Det          CPT_Det
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_DiagUT       CPT_DiagUT
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_EcuM         CPT_EcuM
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_ExeMgrUT     CPT_ExeMgrUT
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_FiM          CPT_FiM
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_INP_SWC      CPT_INP_SWC
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_NmUT         CPT_NmUT
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_NvBlock      CPT_NvBlock
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_NvM          CPT_NvM
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_NvMUT        CPT_NvMUT
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_OUTP_SWC     CPT_OUTP_SWC
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_ParameterSWC CPT_ParameterSWC
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_Rtm          CPT_Rtm
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_StackM       CPT_StackM
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_TstM         CPT_TstM
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_WdgM         CPT_WdgM
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_WdgMUT       CPT_WdgMUT
#define _ETAS_FlatView_SwComponentTypes_ETAS_FlatView_CPT_rba_DiagLib  CPT_rba_DiagLib
/* Box: Rte: OS Environment begin */
/* The following OSENV_ macros are supported by this OS DLL: */
/* OSENV_UNSUPPORTED */
/* OSENV_RTAOS40 */
#if defined(OSENV_RTAOS40)
#if defined(OSENV_UNSUPPORTED)
#error "Please define exactly one of [ OSENV_RTAOS40, OSENV_UNSUPPORTED ] for OS_AUTOSAR40 OS API."
#endif /* defined(OSENV_UNSUPPORTED) */
#elif defined(OSENV_UNSUPPORTED)
#else
#error "Please define exactly one of [ OSENV_RTAOS40, OSENV_UNSUPPORTED ] for OS_AUTOSAR40 OS API."
#endif
/* Box: User declared atomic BSW types: begin */
/* Box: User declared atomic BSW types: end */
/* Box: Rte: OS Environment end */

#endif /* !RTE_CONST_H */