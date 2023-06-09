

#ifndef PDUR_H
#define PDUR_H
/**
 * \defgroup PDUR_H    PduR - AUTOSAR interfaces to Upper Layer and Lower Layer.
 * This interface provides the public export of Component PduR and is to be included by the upper layer Component and Lower Layer Component\n
 * To use this interface include the header <b>PduR.h</b>
 */

/* Identification information */
/**
 * @ingroup PDUR_H
 *
 * Used to declare the Major Release Number of Autosar Main Baseline under which this Component PduR is delivered
 */
#define PDUR_AR_RELEASE_MAJOR_VERSION     4u
/**
 * @ingroup PDUR_H
 *
 * Used to declare the Minor Release Number of Autosar Main Baseline under which this Component PduR is delivered
 */
#define PDUR_AR_RELEASE_MINOR_VERSION     2u
/**
 * @ingroup PDUR_H
 *
 * Used to declare the Revision Release Number of Autosar Main Baseline under which this Component PduR is delivered
 */
#define PDUR_AR_RELEASE_REVISION_VERSION     2u
/**
 * @ingroup PDUR_H
 *
 * Instance Id refers to an ID assigned by Autosar to identify every unique instance of one Module/Component, used to indicate that module instance which is reporting the error during the call of Det_ReportError()\n
 */
#define PDUR_INSTANCE_ID          0u
/* ------------------------------------------------------------------------ */

/* Do not change below 2 files inclusion order. */
#include "PduR_Cfg.h"                              
#include "ComStack_Types.h"                          

#include "Dem.h"

#if(PDUR_ENABLE_INTER_MODULE_CHECKS)
#if (!defined(COMTYPE_AR_RELEASE_MAJOR_VERSION) || (COMTYPE_AR_RELEASE_MAJOR_VERSION != PDUR_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COMTYPE_AR_RELEASE_MINOR_VERSION) || (COMTYPE_AR_RELEASE_MINOR_VERSION != PDUR_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif /* #if(PDUR_ENABLE_INTER_MODULE_CHECKS) */



/* ------------------------------------------------------------------------ */
/* Error IDs */


/**
 * @ingroup PDUR_H
 * PDUR: DET Error ID'd*
 * PDUR_E_CONFIG_PTR_INVALID: API service called with Invalid configuration pointer
 * */
#define PDUR_E_CONFIG_PTR_INVALID      ((uint8)0x00)
/**
 * @ingroup PDUR_H
 * PDUR: DET Error ID'd*
 * PDUR_E_INVALID_REQUEST: API service used without module initialization or PduR_Init called in any state other than PDUR_UNINIT
 * */
#define PDUR_E_INVALID_REQUEST         ((uint8)0x01)
/**
 * @ingroup PDUR_H
 * PDUR: DET Error ID'd*
 * PDUR_E_PDU_ID_INVALID: API service called Invalid PDU identifier
 * */
#define PDUR_E_PDU_ID_INVALID          ((uint8)0x02)
/**
 * @ingroup PDUR_H
 * PDUR: DET Error ID'd*
 * PDUR_E_TP_TX_REQ_REJECTED: TP module rejects a transmit request for a valid PDU identifier
 * */
#define PDUR_E_TP_TX_REQ_REJECTED      ((uint8)0x03)

/**
 * @ingroup PDUR_H
 * PDUR: DET Error ID'd*
 * PDUR_E_ROUTING_TABLE_ID_INVALID: If the routing table is invalid that is given to the PduR_EnableRouting or PduR_DisableRouting functions
 * */
#define PDUR_E_ROUTING_TABLE_ID_INVALID ((uint8)0x08)
/**
 * @ingroup PDUR_H
 * PDUR: DET Error ID'd*
 * PDUR_E_ROUTING_TABLE_ID_INVALID: If the routing table is invalid that is given to the PduR_EnableRouting or PduR_DisableRouting functions
 * */
#define PDUR_E_NULL_POINTER ((uint8)0x09)

/**
 * @ingroup PDUR_H
 * PDUR: DET Error ID'd*
 * PDUR_E_PDU_INSTANCES_LOST: Loss of a PDU instance
 * */
#define PDUR_E_PDU_INSTANCES_LOST ((uint8)0x0a)



/* ------------------------------------------------------------------------ */
/* Service IDs */
/**
 * @ingroup PDUR_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see PduR_Init
 */
#define PDUR_SID_INIT                   (0)    /* PduR_Init */
/**
 * @ingroup PDUR_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see PduR_GetVersionInfo
 */
#define PDUR_SID_GETVERINFO             (1)   /* PduR_GetVersionInfo */
/**
 * @ingroup PDUR_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see PduR_GetConfigurationId
 */
#define PDUR_SID_GETCFGID               (2)   /* PduR_GetConfigurationId */
/**
 * @ingroup PDUR_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see PduR_DisableRouting
 */
#define PDUR_SID_DISABLEROUTING         (7)   /* PduR_DisableRouting */
/**
 * @ingroup PDUR_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see PduR_EnableRouting
 */
#define PDUR_SID_ENABLEROUTING          (8)   /* PduR_EnableRouting */
/**
 * @ingroup PDUR_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see PduR_GF_Tp_TxConfirmation_Func
 */
#define PDUR_SID_GATEWAYTPTXCONFIRMATION          (9)   /* PduR_GF_Tp_TxConfirmation_Func */

/* ------------------------------------------------------------------------ */
/**
 * @ingroup PDUR_H
 *
 * This typedef is used to indicate Routing Path Group ID
 */

typedef  uint16 PduR_RoutingPathGroupIdType;

/**
 * This typedef is used to identify the post-build configuration currently used for routing I-PDUs
 */

typedef  uint16 PduR_PBConfigIdType;


/* ------------------------------------------------------------------------ */
/* Begin a set of function declarations */
 /**
  * @ingroup PDUR_H
  *
  * Section code start
  */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

extern void PduR_Init( const PduR_ConfigType * ConfigPtr );

/* ------------------------------------------------------------------------ */
/* PduR_DisableRouting */
/**
 * @ingroup PDUR_H
 * This API is the Control function to disable routing of PDUs from one src to one or more than one dest layers.\n
 *
 * @param in             id :   RoutingPathGroupId.\n
 * @param in       initialize - Flag to initialize the I-PDU buffer to default values.\n
 *
 * @return None \n
 */
extern void PduR_DisableRouting(PduR_RoutingPathGroupIdType id,
        boolean initialize);

/* PduR_EnableRouting */
/**
 * @ingroup PDUR_H
 * This API is the Control function to enable routing of PDUs from one src to one or more than one dest layers.\n
 *
 * @param in             id :   RoutingPathGroupId.\n
 *
 * @return None \n
 */
extern void PduR_EnableRouting(PduR_RoutingPathGroupIdType id);


/* ------------------------------------------------------------------------ */
/* PduR_GetVersionInfo */

/**
 * @ingroup PDUR_H
 *    This service returns the version information of this module. The version information includes.\n
 *      - Module Id.\n
 *      - Vendor Id.\n
 *      - Vendor specific version numbers.\n
 *
 * @param in        versionInfo - Pointer to where to store the version information of this module.\n
 *
 * @return None \n
 */

extern void PduR_GetVersionInfo( Std_VersionInfoType * versionInfo);


/* ------------------------------------------------------------------------ */
/* PduR_GetConfigurationId */

/**
 * @ingroup PDUR_H
 *    This service returns the active configuration Id of PDU Router module.
 * @param in None \n
 *
 * @return uint32 \n
 */
extern PduR_PBConfigIdType PduR_GetConfigurationId( void );

/* DSMReInit Feature : This function is available only if
 * 1. Dem Event PDUR_E_INIT_FAILED is configured and
 * 2. DSMReInit feature is enabled in the configuration */
extern Std_ReturnType PduR_DemInitMonitorForPDUR_E_INIT_FAILED(Dem_InitMonitorReasonType InitMonitorReason);

/* DSMReInit Feature : This function is available only if
 * 1. Dem Event PDUR_E_PDU_INSTANCE_LOST is configured and
 * 2. DSMReInit feature is enabled in the configuration */

extern Std_ReturnType PduR_DemInitMonitorForPDUR_E_PDU_INSTANCE_LOST(Dem_InitMonitorReasonType InitMonitorReason);

#if ( PDUR_ECUC_RB_RTE_IN_USE == STD_OFF )
/* DSMReInit Feature : This function is available only if
 * 1. DSMReInit feature is enabled in the configuration and one of
 * 2. Dem Event PDUR_E_PDU_INSTANCE_LOST or
 * 3. Dem Event PDUR_E_INIT_FAILED is configured
 * PduR_DsmReInitMainFunction needs to be scheduled as per PduRDsmReInitMainFunctionPeriod parameter*/
extern void PduR_DsmReInitMainFunction(void);
#endif /* #if ( PDUR_ECUC_RB_RTE_IN_USE == STD_OFF ) */

/* ------------------------------------------------------------------------ */
/* End the set of function declarations */

/**
 * @ingroup PDUR_H
 *
 * Stop section code
 */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif /* PDUR_H */

