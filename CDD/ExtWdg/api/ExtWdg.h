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
 * Component : ExtWdg.h
 * Date      : March 31 2020
 * Version   : 3.0
 * Description  : This module implements External Wdg integration
 ***********************************************************************************************
*/
#ifndef EXTWDG_H
#define EXTWDG_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"
#include "WdgIf_Types.h"
#include "rba_BswSrv.h"  /* Integration for interface required by WdgM_Prv.h */


/*
 **********************************************************************************************************************
 *  Defines
 **********************************************************************************************************************
 */
#define EXTWDG_AR_RELEASE_MAJOR_VERSION      4
#define EXTWDG_AR_RELEASE_MINOR_VERSION      2
#define EXTWDG_AR_RELEASE_REVISION_VERSION   2

/*
 **********************************************************************************************************************
 * Public Variables declarations
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */
#define EXTWDG_START_SEC_CODE
#include "ExtWdg_MemMap.h" /*lint !e537 permit multiple inclusion */
extern void ExtWdg_Init(void);
extern FUNC(void, EXTWDG_CODE) ExtWdg_SetTriggerCondition(uint16 timeout);
extern FUNC(Std_ReturnType, EXTWDG_CODE) ExtWdg_SetMode(WdgIf_ModeType Mode);
#define EXTWDG_STOP_SEC_CODE
#include "ExtWdg_MemMap.h" /*lint !e537 permit multiple inclusion */
 

#endif /* EXTWDG_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
