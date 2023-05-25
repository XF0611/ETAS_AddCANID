

#ifndef WDGIF_CFG_H
#define WDGIF_CFG_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */



#include "ExtWdg.h"
/*
#if (!defined(EXTWDG_AR_RELEASE_MAJOR_VERSION) || (EXTWDG_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(EXTWDG_AR_RELEASE_MINOR_VERSION) || (EXTWDG_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/


#include "Wdg.h"
/*
#if (!defined(WDG_AR_RELEASE_MAJOR_VERSION) || (WDG_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(WDG_AR_RELEASE_MINOR_VERSION) || (WDG_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/


#include "WdgIf_Types.h"

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

#define WDGIF_DEV_ERROR_DETECT                                       (STD_ON)
#define WDGIF_VERSION_INFO_API                                       (STD_OFF)
#define WDGIF_NUMBER_OF_DEVICES                                      (2)
#define WDGIF_VARIANT_PRE_COMPILE                                    (1)

/* Symbolic names for Watchdog instances */
#define WdgIfConf_WdgIfDevice_WdgIfDevice_Ext                        ((uint8) (0))
#define WdgIfConf_WdgIfDevice_WdgIfDevice_Int                        ((uint8) (1))

/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */
 
typedef WdgIf_Rb_ExtWdgdriver_tst WdgIf_ConfigType;


/* WdgIf_ModeType WdgMode is recommended but to avoid MISRA warning (Mismatch in AUTOSAR spec of WdgIf & Wdg) */
typedef Std_ReturnType (*WdgIf_SetModefptr) (WdgIf_ModeType Mode);
/* uint16 Timeout  is recommended but to avoid MISRA warning (Mismatch in AUTOSAR spec of WdgIf & Wdg) */
typedef void (*WdgIf_Triggerfptr) (uint16 timeout);

typedef struct
{
	WdgIf_SetModefptr SetMode;
	WdgIf_Triggerfptr SetTriggerCondition;
}WdgIf_DeviceType;

#define WDGIF_START_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgIf_MemMap.h"

extern const WdgIf_DeviceType WdgIf_Device[WDGIF_NUMBER_OF_DEVICES];

#define WDGIF_STOP_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgIf_MemMap.h"




/*
***************************************************************************************************
* External declaration
***************************************************************************************************
*/

/* WdgIf Autosar Api's */
#define WDGIF_START_SEC_CODE_FAST
#include "WdgIf_MemMap.h"

extern Std_ReturnType WdgIf_SetMode(uint8 DeviceIndex, WdgIf_ModeType WdgMode);
extern void WdgIf_SetTriggerCondition(uint8 DeviceIndex,uint16 Timeout);

#define WDGIF_STOP_SEC_CODE_FAST
#include "WdgIf_MemMap.h"



#endif /*WDGIF_CFG_H*/
