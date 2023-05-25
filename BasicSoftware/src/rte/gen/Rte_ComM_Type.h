/** @file     Rte_ComM_Type.h
  *
  * @brief    Application types header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef RTE_ComM_TYPE_H
#define RTE_ComM_TYPE_H

#include "Rte_Type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* BEGIN: SWC specific types */
#ifndef RTE_CORE
#ifndef RTE_MODETYPE_COMMMODE
#define RTE_MODETYPE_COMMMODE

typedef uint8 Rte_ModeType_ComMMode;

#endif /* !RTE_MODETYPE_COMMMODE */
#endif /* RTE_CORE */

/* END: SWC specific types */

/* BEGIN: SWC specific definitions */
#ifndef RTE_CORE
#ifndef RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION
#define RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION   2U
#endif /* !RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION */
#ifndef RTE_MODE_ComMMode_COMM_NO_COMMUNICATION
#define RTE_MODE_ComMMode_COMM_NO_COMMUNICATION     0U
#endif /* !RTE_MODE_ComMMode_COMM_NO_COMMUNICATION */
#ifndef RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION
#define RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION 1U
#endif /* !RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION */
#ifndef RTE_TRANSITION_ComMMode
#define RTE_TRANSITION_ComMMode                     3U
#endif /* !RTE_TRANSITION_ComMMode */
#endif /* RTE_CORE */

#if !defined(RTE_CORE)

/* Box: /AUTOSAR_ComM/ImplementationDataTypes/ComM_ModeType begin */
/* Box: "COMM_NO_COMMUNICATION" begin */
#ifndef COMM_NO_COMMUNICATION
#define COMM_NO_COMMUNICATION (0U)
#endif /* !COMM_NO_COMMUNICATION */
/* Box: "COMM_NO_COMMUNICATION" end */
/* Box: "COMM_SILENT_COMMUNICATION" begin */
#ifndef COMM_SILENT_COMMUNICATION
#define COMM_SILENT_COMMUNICATION (1U)
#endif /* !COMM_SILENT_COMMUNICATION */
/* Box: "COMM_SILENT_COMMUNICATION" end */
/* Box: "COMM_FULL_COMMUNICATION" begin */
#ifndef COMM_FULL_COMMUNICATION
#define COMM_FULL_COMMUNICATION (2U)
#endif /* !COMM_FULL_COMMUNICATION */
/* Box: "COMM_FULL_COMMUNICATION" end */
/* Box: /AUTOSAR_ComM/ImplementationDataTypes/ComM_ModeType end */

/* Box: /AUTOSAR_Platform/ImplementationDataTypes/boolean begin */
/* Box: "FALSE" begin */
#ifndef FALSE
#define FALSE (0U)
#endif /* !FALSE */
/* Box: "FALSE" end */
/* Box: "TRUE" begin */
#ifndef TRUE
#define TRUE (1U)
#endif /* !TRUE */
/* Box: "TRUE" end */
/* Box: /AUTOSAR_Platform/ImplementationDataTypes/boolean end */
#endif /* !defined(RTE_CORE) */
/* END: SWC specific definitions */

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !RTE_ComM_TYPE_H */
