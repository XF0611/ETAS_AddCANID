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
 * Component : main.c
 * Date      : Sep 04 2019
 * Version   : 1.0
 * Description  : This module implements main entry point from startup to user application
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Target.h"
#include "VectorTableCore0.h"

/* ETAS BIP Porting */
#include "arc_exception.h"
#include "alps_clock.h"
#include "alps_emu_reg.h"


/**********************************************************************************
  Function name		:	OS_MAIN
  Description		  :	The main entry point from startup to user application
  Parameter	(in)	:	None
  Parameter	(inout)	:	None
  Parameter	(out)	:	None
  Return value		:	None
  Remarks         :
***********************************************************************************/
OS_MAIN() 
{
    raw_writel(REG_EMU_BOOT_DONE, 1);

    Dem_SetOperationCycleState(0u, 0);
    /*Invoking the ECUM Init for ECU Initialization, never return */
    EcuM_Init();
}


/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
