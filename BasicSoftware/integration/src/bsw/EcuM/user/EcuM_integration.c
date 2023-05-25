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
 * Component : EcuM_Integration.c
 * Date      : Dec 20, 2019
 * Version   : 2.0
 * Description  : This module implements EcuM integration functions
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "EcuM_Integration.h"
#include "Os.h"
#include "Dem.h"
#include "Rte_Main.h"


/**********************************************************************************
  Function name     :   EcuM_GoOffOne
  Description       :   This call performs EcuM GO OFF I state.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks           :   This function shall be call in shutdown phase to stop Scheduler.
***********************************************************************************/
#define ECUM_START_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"
void EcuM_GoOffOne(void)
{
    #if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
    /* MR12 RULE 8.4 VIOLATION: The declaration of GetCoreID is expected from Os */
    if(GetCoreID() == ECUM_CFG_STARTUP_CORE)
    {
    #endif/* ECUM_CFG_MULTICORE_ENABLED */
				(void)Rte_Stop();
				Dem_SetOperationCycleState(0,DEM_CYCLE_STATE_END);
				(void)Dem_Shutdown();
    #if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
		}
    #endif/* ECUM_CFG_MULTICORE_ENABLED */
}
#define ECUM_STOP_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
