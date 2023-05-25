


#ifndef ECUM_H
#define ECUM_H

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/

#include "Std_Types.h" /*Rule BSW_HeaderInc_004:*/


#include "EcuM_Types.h" /* Required for EcuM exported data types */

#include "EcuM_Cbk.h" /*Callback headers*/

/***************************************************************************************************
 * Fix Type declarations
 ***************************************************************************************************/
/**
 *
 */
#define ECUM_VENDOR_ID (6U)

/**
 * @brief ECUM_H
 * ECU Autosar release major version
 */
#define ECUM_AR_RELEASE_MAJOR_VERSION   (4U)

/**
 * @brief ECUM_H
 * ECU Autosar realse minor version
 *
 */
#define ECUM_AR_RELEASE_MINOR_VERSION   (2U)
/**
 * @brief ECMU_H
 * Autosar release revision version
 *
 */
#define ECUM_AR_RELEASE_REVISION_VERSION    (2U)

#define ECUM_SW_MAJOR_VERSION   (2U)
#define ECUM_SW_MINOR_VERSION   (0U)

#define ECUM_SW_PATCH_VERSION   (0U)

/* wakeup status modes */
#define ECUM_WKSTATUS_NONE      0
#define ECUM_WKSTATUS_PENDING   1
#define ECUM_WKSTATUS_VALIDATED 2
#define ECUM_WKSTATUS_EXPIRED   3

/* EcuM_RunStatusType*/

#define ECUM_RUNSTATUS_UNKNOWN                           (EcuM_RunStatusType)(0U)
#define ECUM_RUNSTATUS_REQUESTED                         (EcuM_RunStatusType)(1U)
#define ECUM_RUNSTATUS_RELEASED                           (EcuM_RunStatusType)(2U)

/*EcuM_StateType*/

#define ECUM_SUBSTATE_MASK                   (EcuM_StateType)(0x0fU)
#define ECUM_STATE_STARTUP                   (EcuM_StateType)(0x10U)
#define ECUM_STATE_RUN                       (EcuM_StateType)(0x30U)
#define ECUM_STATE_APP_RUN                   (EcuM_StateType)(0x32U)
#define ECUM_STATE_APP_POST_RUN              (EcuM_StateType)(0x33U)
#define ECUM_STATE_SHUTDOWN                  (EcuM_StateType)(0x40U)
#define ECUM_STATE_SLEEP                     (EcuM_StateType)(0x50U)

/*Disable CAN Trcv ECUM support */
#define CANTRCV_ECUM_USED   STD_OFF

#endif /*ECUM_H*/
