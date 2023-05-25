
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: PduR  / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#ifndef PDUR_CFG_H
#define PDUR_CFG_H

#include "ComStack_Types.h"
#include "PduR_Cfg_SymbolicNames.h"

#define AUTOSAR_VERSION_AR42                   STD_ON

/**
 * @ingroup PDUR_CFG_H
 *
 * Vendor Id refers to a unique ID assigned by Autosar to every Member of Autosar Consortium, used to indicate the vendor during the call of Det_ReportError()\n
 */
#define PDUR_VENDOR_ID            6u

/**
 * @ingroup PDUR_CFG_H
 *
 * Module Id refers to a unique ID assigned by Autosar to every Module/Component within the Autosar BSW,\n
 * used to indicate that module which is reporting the error during the call of Det_ReportError()\n
 */
#define PDUR_MODULE_ID            51u

/**
 * @ingroup PDUR_CFG_H
 *
 * Used to declare Software Major Version number of Component PduR, used during the call of PduR_GetVersionInfo()
 */
#define PDUR_SW_MAJOR_VERSION     10u

/**
 * @ingroup PDUR_CFG_H
 *
 * Used to declare Software Minor Version number of Component PduR, used during the call of PduR_GetVersionInfo()
 */
#define PDUR_SW_MINOR_VERSION     0u

/**
 * @ingroup PDUR_CFG_H
 *
 * Used to declare Software Patch Version number of Component PduR, used during the call of PduR_GetVersionInfo()
 */
#define PDUR_SW_PATCH_VERSION     0u

/**
 * @ingroup PDUR_CFG_H
 *
 * To be used only internal to this module, other modules shall not use this macro
 */
#define PDUR_ENABLE_INTER_MODULE_CHECKS                         FALSE

#if !defined(PDUR_CANIF_SUPPORT)
#define PDUR_CANIF_SUPPORT    (0)
#endif /* PDUR_CANIF_SUPPORT */

#if !defined(PDUR_CANNM_SUPPORT)
#define PDUR_CANNM_SUPPORT    (0)
#endif /* PDUR_CANNM_SUPPORT */

#if !defined(PDUR_UDPNM_SUPPORT)
#define PDUR_UDPNM_SUPPORT    (0)
#endif /* PDUR_UDPNM_SUPPORT */

#if !defined(PDUR_CANTP_SUPPORT)
#define PDUR_CANTP_SUPPORT    (1)
#endif /* PDUR_CANTP_SUPPORT */

#if !defined(PDUR_FRIF_SUPPORT)
#define PDUR_FRIF_SUPPORT    (0)
#endif /* PDUR_FRIF_SUPPORT */

#if !defined(PDUR_FRNM_SUPPORT)
#define PDUR_FRNM_SUPPORT    (0)
#endif /* PDUR_FRNM_SUPPORT */

#if !defined(PDUR_FRTP_SUPPORT)
#define PDUR_FRTP_SUPPORT    (0)
#endif /* PDUR_FRTP_SUPPORT */

#if !defined(PDUR_FRARTP_SUPPORT)
#define PDUR_FRARTP_SUPPORT    (0)
#endif /* PDUR_FRARTP_SUPPORT */

#if !defined(PDUR_LINIF_SUPPORT)
#define PDUR_LINIF_SUPPORT    (0)
#endif /* PDUR_LINIF_SUPPORT */

#if !defined(PDUR_LINTP_SUPPORT)
#define PDUR_LINTP_SUPPORT    (0)
#endif /* PDUR_LINTP_SUPPORT */

#if !defined(PDUR_SOADIF_SUPPORT)
#define PDUR_SOADIF_SUPPORT    (0)
#endif /* PDUR_SOADIF_SUPPORT */

#if !defined(PDUR_J1939TP_SUPPORT)
#define PDUR_J1939TP_SUPPORT    (0)
#endif /* PDUR_J1939TP_SUPPORT */

#if !defined(PDUR_SOADTP_SUPPORT)
#define PDUR_SOADTP_SUPPORT    (0)
#endif /* PDUR_SOADTP_SUPPORT */

#if !defined(PDUR_DOIP_SUPPORT)
#define PDUR_DOIP_SUPPORT    (0)
#endif /* PDUR_DOIP_SUPPORT */

#if !defined(PDUR_COM_SUPPORT)
#define PDUR_COM_SUPPORT    (0)
#endif /* PDUR_COM_SUPPORT */

#if !defined(PDUR_J1939RM_SUPPORT)
#define PDUR_J1939RM_SUPPORT    (0)
#endif /* PDUR_J1939RM_SUPPORT */

#if !defined(PDUR_J1939DCM_SUPPORT)
#define PDUR_J1939DCM_SUPPORT    (0)
#endif /* PDUR_J1939DCM_SUPPORT */

#if !defined(PDUR_IPDUM_SUPPORT)
#define PDUR_IPDUM_SUPPORT    (0)
#endif /* PDUR_IPDUM_SUPPORT */

#if !defined(PDUR_SECOC_SUPPORT)
#define PDUR_SECOC_SUPPORT    (0)
#endif /* PDUR_SECOC_SUPPORT */

#if !defined(PDUR_SOMEIPTP_SUPPORT)
#define PDUR_SOMEIPTP_SUPPORT    (0)
#endif /* PDUR_SOMEIPTP_SUPPORT */

#if !defined(PDUR_DCM_SUPPORT)
#define PDUR_DCM_SUPPORT    (1)
#endif /* PDUR_DCM_SUPPORT */

#if !defined(PDUR_DLT_SUPPORT)
#define PDUR_DLT_SUPPORT    (0)
#endif /* PDUR_DLT_SUPPORT */

#if !defined(PDUR_LDCOM_SUPPORT)
#define PDUR_LDCOM_SUPPORT    (0)
#endif /* PDUR_LDCOM_SUPPORT */

#define PDUR_ECUC_RB_RTE_IN_USE  ( STD_OFF )

#if !defined(PDUR_COM_CANCELRX_SUPPORT)
#define PDUR_COM_CANCELRX_SUPPORT   0
#endif /* PDUR_COM_CANCELRX_SUPPORT */

#define PDUR_MAX_QUEUEDEPTH (1)

/**
 * @ingroup PDUR_CFG_H
 * This structure represents config types passed by EcuM. \n
 * typedef struct\n
 *  {\n
 *  uint32                                   configId;                  config ID \n
 *  const void * PduR_ConfigDataPtr;        Pointer to PduR Base structure \n
 * } PduR_ConfigType;
 */
typedef struct {
	const void *PduR_PBConfigDataPtr;

} PduR_ConfigType;

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/**
 * @ingroup PDUR_CFG_H
 *
 *  PduR PB config type
 */
extern const PduR_ConfigType PduR_Config;

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
#endif /* PDUR_CFG_H */

