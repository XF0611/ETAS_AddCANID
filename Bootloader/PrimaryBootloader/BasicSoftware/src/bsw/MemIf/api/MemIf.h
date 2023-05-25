

#ifndef MEMIF_H
#define MEMIF_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"
#include "MemIf_Cfg.h"


#if (defined(MEMIF_FEE_USED) && (MEMIF_FEE_USED == STD_ON))
#include "Fee.h"
#include "Fls.h"
#endif
#if (defined(MEMIF_NUM_OF_EA_DEVICES) && (MEMIF_NUM_OF_EA_DEVICES > 0u))
#include "Ea.h"
#endif


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
#define FEE_DEVICE_INDEX   1u
#define EA_DEVICE_INDEX    2u

#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT != FALSE))
/* Error codes for API used for DET module */
#define MEMIF_E_PARAM_DEVICE          0x01u     /* Error code for wrong device index */

#define MEMIF_RB_E_BUSY               0x10u     /* Error code for multiple mainfunction execution,
                                                   RB specific error code, 0x10 is used*/

/* API Service IDs used for DET module */
#define MEMIF_SID_CHECK_INDEX         0x81u     /* Service ID for Memif chcek device index function.
                                                   AUTOSAR spec doesn't specify any value for this checking and
                                                   hence 0x81 is used */
#define MEMIF_RB_SID_MAIN_FUNCTION    0xFFu     /* Service ID for Memif Rb MainFunction,
                                                   RB specific API, 0xFF is used */
#endif

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"

#if (defined(MEMIF_VERSION_INFO_API) && (MEMIF_VERSION_INFO_API == STD_ON))
extern void MemIf_GetVersionInfo( Std_VersionInfoType* VersionInfoPtr );
#endif /* MEMIF_VERSION_INFO_API */

extern MemIf_StatusType MemIf_Rb_GetStatus(void);
extern void MemIf_CheckDeviceIndex(uint8 DeviceIndex);

/* Remove Rte generated APIs if included in MemIf context */
#ifndef MEMIF_RB_HIDE_RTEAPI
/* Functions: scheduled */
extern void MemIf_Rb_MainFunction(void);
#endif

#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"

/* MEMIF_H */
#endif
