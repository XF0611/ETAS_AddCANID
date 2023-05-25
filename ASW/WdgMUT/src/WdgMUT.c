/* *****************************************************************************
 * BEGIN: Banner
 *-----------------------------------------------------------------------------
 *                                 ETAS GmbH
 *                      D-70469 Stuttgart, Borsigstr. 14
 *-----------------------------------------------------------------------------
 *    Administrative Information (automatically filled in by ISOLAR)         
 *-----------------------------------------------------------------------------
 * Name: 
 * Description:
 * Version: 1.0
 *-----------------------------------------------------------------------------
 * END: Banner
 ******************************************************************************

 * Project : ETAS_BIP_PT
 * Component: /ETAS_BIP/WdgMUT
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.1
 * Author: HUR4HC
 * Date : Fri May 20 15:49:04 2022
 ****************************************************************************/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Rte_WdgMUT.h"
#include "WdgM.h"
#include "WdgMUT.h"


/**********************************************************************************************************************
 * Variables
 *********************************************************************************************************************/
#define WdgMUT_START_SEC_VAR_INIT_8
#include "WdgMUT_MemMap.h"
Wdg_Test_t WdgM_AliveTest = WDG_NONE;
Wdg_Test_t WdgM_DeadlineTest = WDG_NONE;
Wdg_Test_t WdgM_LogicalTest = WDG_NONE;
WdgMMode_Test_t WdgM_CurrentMode = WDGM_SLOWMODE;
#define WdgMUT_STOP_SEC_VAR_INIT_8
#include "WdgMUT_MemMap.h"

#define WdgMUT_START_SEC_VAR_NO_INIT_8
#include "WdgMUT_MemMap.h"
WdgM_SupervisedEntityIdType SEIDGet;
Std_ReturnType syncCall1;
WdgM_GlobalStatusType GlobalStatus;
Std_ReturnType syncCall2;
WdgM_ModeType WdgMMode;
Std_ReturnType syncCall3;
Std_ReturnType syncCall4;
WdgM_ModeType SetMode;
Std_ReturnType syncCall5;
WdgM_LocalStatusType LocalStatus;
Std_ReturnType syncCall1;
WdgM_ModeType LocalMode;
WdgM_ModeType GlobalMode;
#define WdgMUT_STOP_SEC_VAR_NO_INIT_8
#include "WdgMUT_MemMap.h"

#define WdgMUT_START_SEC_VAR_INIT_BOOLEAN
#include "WdgMUT_MemMap.h"
boolean SEID_Test = FALSE;
boolean GlobalStatus_Test = FALSE;
boolean WdgMMode_Test = FALSE;
boolean Reset_Test = FALSE;
boolean SetMode_Test = FALSE;
boolean LocalStatus_Test = FALSE;
#define WdgMUT_STOP_SEC_VAR_INIT_BOOLEAN
#include "WdgMUT_MemMap.h"

#define WdgMUT_START_SEC_VAR_INIT_8
#include "WdgMUT_MemMap.h"
uint8 start_Deadline = 0;
uint8 wait10ms = 0;
#define WdgMUT_STOP_SEC_VAR_INIT_8
#include "WdgMUT_MemMap.h"

#define WdgMUT_START_SEC_VAR_INIT_32
#include "WdgMUT_MemMap.h"
uint32 Alive_Count = 0;
uint32 IncCounter = 0;
#define WdgMUT_STOP_SEC_VAR_INIT_32
#include "WdgMUT_MemMap.h"


#define WdgMUT_START_SEC_CODE
#include "WdgMUT_MemMap.h"


void RE_WdgMUT_ASW_10ms_Alive(void)
{
	IncCounter++; //This variable is used to verify if ECU is reset or not//
	Std_ReturnType syncCall;
	static uint16 Entity_Count;
	//get operation mode: Fast or Slow
	syncCall = Rte_Call_RP_WdgM_GlobalSupervision_GetMode(&WdgM_CurrentMode);
  	Alive_Count++;
	switch (WdgM_AliveTest) {
	case ALIVE_OK_LACKOF_ONE_CP_ALIVE_SUPERVISION_ENTITY:
		if (WdgM_CurrentMode == WDGM_FASTMODE) {
        	/*Lack 1 checkpoint in FAST MODE*/
        	Alive_Count = 0;
        	WdgM_AliveTest = WDG_NONE;
    	}
		else if (Alive_Count < 10) {
			/*Don't call Checkpoint if the time still don't reach 100ms in SLOW MODE*/
		}
		else {
        	/*Lack 1 checkpoint in SLOW MODE*/
			Alive_Count = 0;
			WdgM_AliveTest = WDG_NONE;
		}
		break;

	case ALIVE_NOTOK_LACKOF_TWO_ALIVE_SUPERVISION_ENTITY:
		/*lack of 2 CP*/
      	if (WdgM_CurrentMode == WDGM_FASTMODE) {
			/*lack of 2 CP in FAST MODE*/
			if (Entity_Count > 0) {
				WdgM_AliveTest = WDG_NONE;
				Entity_Count = 0;
          		Alive_Count = 0;
			}
			else {
				Entity_Count++;
			}
      	}
		else if (Alive_Count < 10) {
		/*Don't call Checkpoint if the time still don't reach 100ms in SLOW MODE*/
		}
		else {
			/*lack of 2 CP in SLOW MODE*/
			if (Entity_Count > 0) {
				WdgM_AliveTest = WDG_NONE;
				Entity_Count = 0;
			}
			else {
				Entity_Count++;
			}
				Alive_Count = 0;
		}
		break;

	case ALIVE_NOTOK_ADDMORE_ALIVE_SUPERVISION_ENTITY:
    	if (WdgM_CurrentMode == WDGM_FASTMODE) {
			/*Add 1 more CP in FAST MODE*/
			syncCall = Rte_Call_RP_AliveSupervision_ASW_10ms_CheckpointReached();
			syncCall = Rte_Call_RP_AliveSupervision_ASW_10ms_CheckpointReached();
			WdgM_AliveTest = WDG_NONE;
			Alive_Count = 0;
		}
		else if (Alive_Count<10 && WdgM_CurrentMode == WDGM_SLOWMODE) {
			/*Don't call Checkpoint if the time still don't reach 100ms in SLOW MODE*/
		}
		else {
			Alive_Count = 0;
			/*Add 1 more CP in SLOW MODE*/
			syncCall = Rte_Call_RP_AliveSupervision_ASW_10ms_CheckpointReached();
			syncCall = Rte_Call_RP_AliveSupervision_ASW_10ms_CheckpointReached();
			WdgM_AliveTest = WDG_NONE;
		}
		break;

	default:
		/* In 100ms Task trigger 1 AliveSupervision, */
		if (WdgM_CurrentMode == WDGM_FASTMODE) {
        	/*Call 1 CP every 10ms in FAST MODE*/
        	syncCall = Rte_Call_RP_AliveSupervision_ASW_10ms_CheckpointReached();
        	Alive_Count = 0;
    	}
		else if (Alive_Count < 10 && WdgM_CurrentMode == WDGM_SLOWMODE) {
			/*Don't call Checkpoint if the time still don't reach 100ms in SLOW MODE*/
      	}
      	else {
			Alive_Count = 0;
			syncCall = Rte_Call_RP_AliveSupervision_ASW_10ms_CheckpointReached();
		}
		break;
	}
  	GlobalMode = Rte_Mode_RP_WdgM_GlobalMode_currentMode();
  	LocalMode = Rte_Mode_RP_WdgM_WdgM_SE1_Mode_currentMode();
}

void RE_WdgMUT_ASW_10ms_Deadline(void)
{
	Std_ReturnType syncCall;
	//get operation mode: Fast or Slow
	syncCall = Rte_Call_RP_WdgM_GlobalSupervision_GetMode(&WdgM_CurrentMode);
	switch (WdgM_DeadlineTest) { 
	case DEADLINE_OK_ONTIME_CP_WDG_DEADLINE_ENTITY:
    	wait10ms++;
    	if (start_Deadline == 0) {
      		syncCall = Rte_Call_RP_Deadline_Start_ASW_10ms_CheckpointReached();
      		start_Deadline = 1;	
      		wait10ms = 0;
		}
		if (((start_Deadline == 1) && (WdgM_CurrentMode == WDGM_SLOWMODE) && (wait10ms == 99)) || ((start_Deadline == 1) && (WdgM_CurrentMode == WDGM_FASTMODE) )) {
			syncCall = Rte_Call_RP_Deadline_Stop_ASW_10ms_CheckpointReached();
			start_Deadline = 0;
			WdgM_DeadlineTest = WDG_NONE;
		}
		break;

	case DEADLINE_NOTOK_LATE_CP_WDG_DEADLINE_ENTITY:
    	wait10ms++;
    	if (start_Deadline == 0) {
      		syncCall = Rte_Call_RP_Deadline_Start_ASW_10ms_CheckpointReached();
      		start_Deadline = 1;	
      		wait10ms = 0;
		}
		break;

	default:
		break;
	}
}

void RE_WdgMUT_ASW_10ms_Logical(void)
{
	static uint16 logicalCount;
	Std_ReturnType syncCall;
	switch (WdgM_LogicalTest) {
	case LOGICAL_OK_VALID_CP:
    	if (logicalCount == 0) {
    		syncCall = Rte_Call_RP_PFM_CP0_ASW_10ms_CheckpointReached();
        	logicalCount++;
   		}
      	else {
        	syncCall = Rte_Call_RP_PFM_CP1_ASW_10ms_CheckpointReached();
        	logicalCount = 0;
        	WdgM_LogicalTest = WDG_NONE;
      	}
		break;

	case LOGICAL_NOTOK_INVALID_INITIAL_CP:
		syncCall = Rte_Call_RP_PFM_CP1_ASW_10ms_CheckpointReached();
		WdgM_LogicalTest = WDG_NONE;
		break;

	case LOGICAL_NOTOK_INVALID_FINAL_CP:
		syncCall = Rte_Call_RP_PFM_CP0_ASW_10ms_CheckpointReached();
		syncCall = Rte_Call_RP_PFM_CP0_ASW_10ms_CheckpointReached();
		WdgM_LogicalTest = WDG_NONE;
		break;

	default:
		break;
	}
}

void RE_WdgM_GlobalSupervision(void)
{
	Std_ReturnType retValue = RTE_E_OK;

	if (SEID_Test == TRUE) {
		syncCall1 = Rte_Call_RP_WdgM_GlobalSupervision_GetFirstExpiredSEID(&SEIDGet);
		SEID_Test = FALSE;
	}
	if (GlobalStatus_Test == TRUE) {
		syncCall2 = Rte_Call_RP_WdgM_GlobalSupervision_GetGlobalStatus(&GlobalStatus);
		GlobalStatus_Test = FALSE;
	}
	if (WdgMMode_Test == TRUE) {
		syncCall3 = Rte_Call_RP_WdgM_GlobalSupervision_GetMode(&WdgMMode);
		WdgMMode_Test = FALSE;
	}
	if (Reset_Test == TRUE) {
		syncCall4 = Rte_Call_RP_WdgM_GlobalSupervision_PerformReset();
		Reset_Test = FALSE;
	}
	if (SetMode_Test == TRUE) {
		syncCall5 = Rte_Call_RP_WdgM_GlobalSupervision_SetMode(SetMode);
		SetMode_Test = FALSE;
	}
}

void RE_WdgM_LocalSupervisionStatus(void)
{
	Std_ReturnType retValue = RTE_E_OK;
	if (LocalStatus_Test == TRUE) {
		syncCall1 = Rte_Call_RP_WdgM_LocalSupervisionStatus_GetLocalStatus(&LocalStatus);
		LocalStatus_Test = FALSE;
	}
}
#define WdgMUT_STOP_SEC_CODE
#include "WdgMUT_MemMap.h"

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
