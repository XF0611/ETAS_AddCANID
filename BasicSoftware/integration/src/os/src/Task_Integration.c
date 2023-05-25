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
 * Component : Task_Integration.c
 * Date      : Oct 22, 2021
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
#include "Os_Cfg.h"
#include "Rtm.h"
#include "MemIf.h"


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

#define RTE_OSTASK_SYSCORE_BSWBG_1MS_START_SEC_CODE
#include "Rte_MemMap.h"
/**********************************************************************************
  Function name     : OsTask_SysCore_BswBg_1ms
  Description       : SysCore core background task.
  Parameter	(in)    : None
  Parameter	(inout) : None
  Parameter	(out)   : None
  Return value      : None
  Remarks: Task to call Mainfunction that run in the background.
***********************************************************************************/
TASK(OsTask_SysCore_BswBg_1ms)
{
    Rtm_ProcessStart(Rtm_Mon_Cfg_ProcessId_MemIf_MainFunction);
    MemIf_Rb_MainFunction();
    Rtm_ProcessEnd(Rtm_Mon_Cfg_ProcessId_MemIf_MainFunction);
    TerminateTask();
}
#define RTE_OSTASK_SYSCORE_BSWBG_1MS_STOP_SEC_CODE
#include "Rte_MemMap.h"


#define RTE_OSTASK_APPCORE_BSW_10MS_START_SEC_CODE
#include "Rte_MemMap.h"
TASK(OsTask_AppCore_BSW_10ms)
{
    TerminateTask();
}
#define RTE_OSTASK_APPCORE_BSW_10MS_STOP_SEC_CODE
#include "Rte_MemMap.h"


#define RTE_OSTASK_COMCORE_BSW_10MS_START_SEC_CODE
#include "Rte_MemMap.h"
TASK(OsTask_ComCore_BSW_10ms)
{
    TerminateTask();
}
#define RTE_OSTASK_COMCORE_BSW_10MS_STOP_SEC_CODE
#include "Rte_MemMap.h"

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
