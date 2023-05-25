/*
 ***********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 ***********************************************************************************************************************
 ************************************************************************************************
 * Component : Rtm_Driver_Prv.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef RTM_DRIVER_PRV_H
#define RTM_DRIVER_PRV_H

/*! \file
 *
 * \brief Runtime measurement driver private header
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */

#include "Rtm.h"


/*
 ***********************************************************************************************************************
 * Preprocessor defines
 ***********************************************************************************************************************
 */

/* Macro to define the not called function to deactivate the measure */
/* MISRA RULE 11.3 VIOLATION: Cast from function pointer to integral type is necessary in this case */
#define NOT_CALLED (uint32)&Rtm_Driver_PauseFunction


/*
 ***********************************************************************************************************************
 * Prototypes
 ***********************************************************************************************************************
 */

#if (RTM_CTRL == STD_ON)

# define RTM_START_SEC_CODE_SLOW
# include "Rtm_MemMap.h"

FUNC (void, RTM_CODE_SLOW) Rtm_Driver_PreOSInit (void);
/* Function to set a breakpoint to a function that is never called */
FUNC (void, RTM_CODE_SLOW) Rtm_Driver_PauseFunction (void);

# define RTM_STOP_SEC_CODE_SLOW
# include "Rtm_MemMap.h"

# define RTM_START_SEC_CODE
# include "Rtm_MemMap.h"

/* Rtm_Driver_#target#.c */
FUNC (void, RTM_CODE) Rtm_Driver_Init (uint8_least coreId_qu8);
FUNC (void, RTM_CODE) Rtm_Driver_Shutdown (uint8_least coreId_qu8);
FUNC (void, RTM_CODE) Rtm_Driver_Setup (uint8_least coreId_qu8);
FUNC (void, RTM_CODE) Rtm_Driver_Disable (uint8_least coreId_qu8);

# define RTM_STOP_SEC_CODE
# include "Rtm_MemMap.h"

#endif /* if (RTM_CTRL == STD_ON) */


#endif /* RTM_DRIVER_PRV_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

