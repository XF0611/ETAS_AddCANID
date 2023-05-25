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
 * Component : Target.h
 * Date      : Oct 12, 2021
 * Version   : 2.1
 * Description  : This module implements Timer driver for RTA-OS TASK tick.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/
#ifndef TARGET_H
#define TARGET_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Os.h"

#include "Rtm.h"
#include "StackM.h"


/*
 **********************************************************************************************************************
 *  Defines
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 *  Global Macros definition
 **********************************************************************************************************************
 */
#ifndef RTM_H
#define TARGET_IDLE()                 /* idle without doing anything. */
#else
#define TARGET_IDLE()                 {StackM_ScanCoreStack();}
#endif
#define TARGET_GETSYSTICKS32BITS()    Target_GetSysTicks32Bits()
#define TARGET_GETMSTICKS32BITS()     Target_Get1msTicks()


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */
#define TARGET_START_SEC_CODE
#include "MemMap.h"
extern void Target_Init(void);
extern void Target_TimerInit(void);
extern void Target_EnableInterrupts(void);
extern void Target_ClearPendingInterrupt(void);
extern uint32 Target_GetSysTicks32Bits(void);
extern TickType Target_Get1msTicks(void);
#define TARGET_STOP_SEC_CODE
#include "MemMap.h"


#endif /* TARGET_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
