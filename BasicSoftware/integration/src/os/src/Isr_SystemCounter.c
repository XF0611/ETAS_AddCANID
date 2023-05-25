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
 * Component : Isr_SystemCounter.c
 * Date      : Nov 13, 2022
 * Version   : 1.0
 * Description  : This module implement ISR body for Os counter tick.
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Target.h"


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/

/**********************************************************************************
  Function name     : SystemCounter
  Description       : SysCore core background task.
  Parameter	(in)    : None
  Parameter	(inout) : None
  Parameter	(out)   : None
  Return value      : None
  Remarks: Task to call Mainfunction that run in the background.
***********************************************************************************/
ISR(SystemCounter)
{
    Target_ClearPendingInterrupt();

    (void)IncrementCounter(Millisecond_TickCounter);
    (void)IncrementCounter(Rte_TickCounter);
}

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
