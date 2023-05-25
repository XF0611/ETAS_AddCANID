


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#ifndef COM_CFG_H
#define COM_CFG_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Cfg_SymbolicNames.h"


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
/**
 * @ingroup COM_CFG_H
 *
 * TRACE[SWS_Com_00442]
 * Com module id(according to AUTOSAR BSW module list)
 */
#define COM_MODULE_ID                                           50u

#define COM_VENDOR_ID                                           6u

/* Version of AUTOSAR Com specification */
/**
 * @ingroup COM_CFG_H
 *
 * Major Version of AUTOSAR Com specification
 */
#define COM_AR_RELEASE_MAJOR_VERSION                            4u
/**
 * @ingroup COM_CFG_H
 *
 * Minor Version of AUTOSAR Com specification
 */
#define COM_AR_RELEASE_MINOR_VERSION                            2u
/**
 * @ingroup COM_CFG_H
 *
 * Revision Version of AUTOSAR Com specification
 */
#define COM_AR_RELEASE_REVISION_VERSION                         2u

/* Version of AUTOSAR Com implementation */
/**
 * @ingroup COM_CFG_H
 *
 * Major Version of AUTOSAR Com implementation
 */
#define COM_SW_MAJOR_VERSION                                    10u
/**
 * @ingroup COM_CFG_H
 *
 * Minor Version of AUTOSAR Com implementation
 */
#define COM_SW_MINOR_VERSION                                    0u
/**
 * @ingroup COM_CFG_H
 *
 * Patch Version of AUTOSAR Com implementation
 */
#define COM_SW_PATCH_VERSION                                    0u

#define COM_NUM_ARR_IPDUGRPVECT                                 1u

#define COM_ECUC_RB_RTE_IN_USE                                  STD_ON

/*#define COM_ECUC_AR_RELEASE_AR45*/

/**
 * @ingroup COM_CFG_H
 *
 * To be used only internal to this module, other modules shall not use this macro
 */
#define COM_ENABLE_INTER_MODULE_CHECKS                         FALSE

/************************ SIGNAL GROUP switches *************************/
/************************************************************************/
/* SIGNAL GROUP switches */

/* #define COM_TX_SIGNALGROUP */

/* #define COM_RX_SIGNALGROUP */
/************************************************************************/


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/
/* Start: Type Declarations*/

/* TRACE[SWS_Com_00820]: The AUTOSAR COM module's signal object identifier. */
typedef uint16 Com_SignalIdType;

/* TRACE[SWS_Com_00821]: The AUTOSAR COM module's signal group object identifier. */
typedef uint16 Com_SignalGroupIdType;

/* TRACE[SWS_Com_00822]: The AUTOSAR COM I-PDU module's group object identifier. */
typedef uint16 Com_IpduGroupIdType;

typedef uint8 Com_IpduGroupVector[COM_NUM_ARR_IPDUGRPVECT];

/* TRACE[SWS_Com_00819]: Status value returned by the API service Com_GetStatus() */
typedef enum
{
    COM_UNINIT,     /* This status shall have the value 0. */
    COM_INIT
} Com_StatusType;

/* TRACE[SWS_Com_00825]: Type of the data structure containing the initialization data for COM. */
typedef struct
{
    const void *                                    configData_pcv;
    const void *                                    commonData_pcv;
    const PduLengthType *                           rxPduLength_pcauo;
    const PduLengthType *                           txPduLength_pcauo;
    const PduIdType *                               txPduRSrcPduId_pcauo;

} Com_ConfigType;

/*  END: Type Declaration  */

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
/* START: Global constant declarations */

#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
/* If pre compile variant is configured by user, ComConfig is still required
 * Extern this variable and define this configuration struct.*/
extern const Com_ConfigType ComConfig;
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

/* END: Global constant declarations */

#if ( COM_ECUC_RB_RTE_IN_USE == STD_OFF )

#define COM_START_SEC_CODE
#include "Com_MemMap.h"

/* Rx-MainFunction declarations */

/* Tx-MainFunction declarations */

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if ( COM_ECUC_RB_RTE_IN_USE == STD_OFF ) */

#endif /* #ifndef COM_CFG_H */


