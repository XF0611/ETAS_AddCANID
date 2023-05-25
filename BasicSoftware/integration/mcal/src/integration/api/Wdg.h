/*
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 ************************************************************************************************
 * Component : Wdg.h
 * Date      : April 6 2021
 * Version   : 1.0
 * Description  : This module implements heaer file integration for MCAL Wdg module.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef _WDG_H_
#define _WDG_H_

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "WdgIf_Types.h"

/*
 **********************************************************************************************************************
 * Defines
 **********************************************************************************************************************
 */
#define WDG_AR_RELEASE_MAJOR_VERSION      (4U)
#define WDG_AR_RELEASE_MINOR_VERSION      (2U)
#define WDG_AR_RELEASE_REVISION_VERSION   (2U)


#define WDG_START_SEC_CODE
// #include "Wdg_MemMap.h"

typedef struct
{
    uint32 reaction;

    uint32 windowSize;
    /**< Different Window Sizes
     *   Refer Watchdog Window Size section above for different Window sizes */
    uint32 timeOutVal;
    /**< Watchdog timeout value in milli seconds */
} Wdg_ModeInfoType;

typedef struct Wdg_ConfigType_s
{
    WdgIf_ModeType   defaultMode;
    /**< Default mode */
    Wdg_ModeInfoType fastModeCfg;
    /**< Fast mode config */
    Wdg_ModeInfoType slowModeCfg;
    /**< Slow mode config */
} Wdg_ConfigType;

extern FUNC(Std_ReturnType, WDG_CODE) Wdg_SetMode(WdgIf_ModeType l_WdgMode);
extern FUNC(void, WDG_CODE) Wdg_SetTriggerCondition(uint16 timeout);
extern void Wdg_Init(const void * ConfigPtr);
#define Wdg_Config     Wdg_ConfigData
extern Wdg_ConfigType Wdg_ConfigData;

#define WDG_STOP_SEC_CODE
// #include "Wdg_MemMap.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */



#endif /* WDG_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
