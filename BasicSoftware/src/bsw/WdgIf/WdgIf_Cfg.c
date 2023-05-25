

/*
 ******************************************************************************************************************
 * Includes
 ******************************************************************************************************************
 */
 /*TRACE[WDGIF003] WdgIf shall not add functionality not abstract from properties from watchdog drivers*/
#include "WdgIf.h"

#if (WDGIF_DEV_ERROR_DETECT==STD_ON)
#include "Det.h"
/*
#if (!defined(DET_AR_RELEASE_MAJOR_VERSION) || (DET_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(DET_AR_RELEASE_MINOR_VERSION) || (DET_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/
#endif /*(WDGIF_DEV_ERROR_DETECT==STD_ON)*/

  
 /*
 *************************************************************************************************************************************************
 * Constants
 *************************************************************************************************************************************************
 */

#define WDGIF_START_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgIf_MemMap.h"

/* TRACE[WDGIF018] Use DeviceIndex for selection of watchdog drivers if more then one driver*/
/* TRACE[WDGIF013] Use uint8 for Device ID*/

const WdgIf_DeviceType WdgIf_Device[WDGIF_NUMBER_OF_DEVICES] =
{   /*  WdgIf_SetModefptr                  , WdgIf_Triggerfptr                             */

    {   &ExtWdg_SetMode                    , &ExtWdg_SetTriggerCondition                  },
    {   &Wdg_SetMode                       , &Wdg_SetTriggerCondition                     }
};
#define WDGIF_STOP_SEC_CONFIG_DATA_FAST_UNSPECIFIED
#include "WdgIf_MemMap.h"


 /*
 **************************************************************************************************************************
 * Function Definitions
 **************************************************************************************************************************
 */


#define WDGIF_START_SEC_CODE_FAST
#include "WdgIf_MemMap.h"
/* TRACE[WDGIF047] Mandatory Interfaces Wdg_SetMode, Wdg_SetTriggerCondition Impemented*/
/* TRACE[WDGIF048] Optional Interface Det_ReportError used*/

/************************************************************************************************************************************************
* Name              :   WdgIf_SetMode
* Description       :   Map the service WdgIf_SetMode to the service Wdg_SetMode of
*                       the corresponding Watchdog driver
* Parameters[in]    :   Device index to identify the watchdog driver instance,
*                       WdgMode(Watchdog driver mode)
* Limitations       :   None
* Return Value      :   Std_ReturnType
*************************************************************************************************************************************************/
/* TRACE[WDGIF042],[WDGIF043] Sets the current mode of the watchdog driver */
/* TRACE[WDGIF026] Inteface to provide Mode Switching*/
Std_ReturnType WdgIf_SetMode(uint8 DeviceIndex, WdgIf_ModeType WdgMode)
{
    Std_ReturnType ReturnStatus;
    /* TRACE[WDGIF028] Perform Check for Device Index ID*/
    
    if (DeviceIndex >= WDGIF_NUMBER_OF_DEVICES)
    {   /* TRACE[WDGIF032] Perform reporting of Detected development errors*/
        /* TRACE[WDGIF031] Perform checking of DET parameters*/
        /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
        (void)Det_ReportError(WDGIF_MODULE_ID, WDGIF_INSTANCE_ID, WDGIF_SETMODE_SERVICEID, WDGIF_E_PARAM_DEVICE);
        return E_NOT_OK;
    }

    /* TRACE[WDGIF057] Return value from Wdg_SetMode as return from WdgIf_SetMode*/
	ReturnStatus = WdgIf_Device[DeviceIndex].SetMode(WdgMode);
    return ReturnStatus;
}

/*************************************************************************************************************************************************
* Name              :	WdgIf_SetTriggerCondition
* Description       :   Map the service WdgIf_SetTriggerCondition to the service
*                       Wdg_SetTriggerCondition of the corresponding Watchdog driver
* Parameters[in]    :   Device Index to identify the watchdog driver instance,
*                       Timeout value for setting the trigger counter
* Limitations       :   None
* Return Value	    :	Void
***************************************************************************************************************************************************/
/* TRACE[WDGIF044],[WDGIF045] Sets the trigger condition to reset the watchdog timeout counter*/
/* TRACE[WDGIF026] Inteface to provide Trigger*/
void WdgIf_SetTriggerCondition(uint8 DeviceIndex, uint16 Timeout)
{
    /* TRACE[WDGIF028] Perform Check for Device Index ID*/
    
    if (DeviceIndex >= WDGIF_NUMBER_OF_DEVICES)
    {
        /* TRACE[WDGIF032] Perform reporting of Detected development errors*/
        /* TRACE[WDGIF031] Perform checking of DET parameters*/
        /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
        (void)Det_ReportError(WDGIF_MODULE_ID, WDGIF_INSTANCE_ID, WDGIF_TRIGGER_SERVICEID, WDGIF_E_PARAM_DEVICE);
        return;
    }

    WdgIf_Device[DeviceIndex].SetTriggerCondition(Timeout);
    return;
}

#define WDGIF_STOP_SEC_CODE_FAST
#include "WdgIf_MemMap.h"




