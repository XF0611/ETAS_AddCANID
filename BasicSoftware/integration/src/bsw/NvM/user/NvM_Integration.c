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
 * Component : NvM_Integration.c
 * Date      : Jan 16 2020
 * Version   : 2.0
 * Description  : This module implements NvM integration functions
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM_Integration.h"
#include "Fls.h"
#include "Fee.h"
#include "NvM.h"
#include "MemIf.h"
#include "Wdg.h"
#include "ExtWdg.h"
#include "Rtm.h"


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/**********************************************************************************
  Function name		:	NvM_Integration_ReadAll
  Description		:	This function perform readall function.
  Parameter	(in)	:	None
  Parameter	(inout)	:	None
  Parameter	(out)	:	None
  Return value		:	None
  Remarks:	This function should be call in init phase and Scheduler isn't started
***********************************************************************************/
void NvM_Integration_ReadAll(void)
{
	NvM_Rb_StatusType	NvM_Status = NVM_RB_STATUS_UNINIT;
	MemIf_StatusType	MemIf_Status = MEMIF_UNINIT;
	Rtm_ProcessStart(Rtm_Mon_Cfg_ProcessId_NvM_ReadAll);
	/* Initialize Memory stack */

	/* Trigger reading all NvM blocks */
	NvM_ReadAll();

	/*
	 * Call Mainfunction to handle ReadAll request.
	 * Check the status of request.
	 *  */
	do{
		/* Request Memory layers handle request */
		NvM_MainFunction();
		MemIf_Rb_MainFunction();

		/* Get the status of request */
		(void)NvM_Rb_GetStatus(&NvM_Status);
		MemIf_Status = MemIf_Rb_GetStatus();

	}while((NvM_Status != NVM_RB_STATUS_IDLE) || (MemIf_Status != MEMIF_IDLE));
	Rtm_ProcessEnd(Rtm_Mon_Cfg_ProcessId_NvM_ReadAll);
}

/**********************************************************************************
  Function name		:	NvM_Integration_WriteAll
  Description		:	This function perform Write all function.
  Parameter	(in)	:	None
  Parameter	(inout)	:	None
  Parameter	(out)	:	None
  Return value		:	None
  Remarks:	This function should be call in shutdown phase and Scheduler shall stopped
***********************************************************************************/
void NvM_Integration_WriteAll(void)
{
	NvM_Rb_StatusType	NvM_Status = NVM_RB_STATUS_UNINIT;
	MemIf_StatusType	MemIf_Status = MEMIF_UNINIT;
	Rtm_ProcessStart(Rtm_Mon_Cfg_ProcessId_NvM_WriteAll);
	/* Schedule table has been stopped before NvM_WriteAll. */
	NvM_WriteAll();

	do{
		/* Request Memory layers handle request */
		NvM_MainFunction();
		MemIf_Rb_MainFunction();

		/* Get the status of request */
		(void)NvM_Rb_GetStatus(&NvM_Status);
		MemIf_Status = MemIf_Rb_GetStatus();
		//IntWdg_SetTriggerCondition(1);
	} while((NvM_Status == NVM_RB_STATUS_BUSY) || (MemIf_Status == MEMIF_BUSY));
	Rtm_ProcessEnd(Rtm_Mon_Cfg_ProcessId_NvM_WriteAll);
	/* Schedule table no need to be restart again as WriteAll designed after Rte Stopped. */
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
