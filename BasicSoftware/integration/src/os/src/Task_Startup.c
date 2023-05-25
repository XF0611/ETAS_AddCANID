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
 * Component : Task_Startup.c
 * Date      : Dec 5, 2019
 * Version   : 1.0
 * Description  : This module implement Tasks bodies that are not implemented in RTE context.
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Os.h"
#include "Rtm.h"


/*
 **********************************************************************************************************************
 * Imported Function declarations
 **********************************************************************************************************************
 */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
extern FUNC(void, ECUM_CODE) EcuM_StartupTwo(void);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"


/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/


/**********************************************************************************
  Function name     : OsTask_ECU_Startup
  Description       : master core startup task.
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks: This task is called only once during master core start up
***********************************************************************************/
#define RTE_START_SEC_TASKBODY
/*#include "MemMap.h"*/
TASK(OsTask_ECU_Startup)
{
    RTM_CORECHECKPOINT_CORE_USERSTART();

    EcuM_StartupTwo();

    TerminateTask();
} /* OsTask_ECU_Startup */
#define RTE_STOP_SEC_TASKBODY
/*#include "MemMap.h"*/

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
