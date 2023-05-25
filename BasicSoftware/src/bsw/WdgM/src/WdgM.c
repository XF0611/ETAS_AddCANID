

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "WdgM_Prv.h"

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */
#define WDGM_START_SEC_VAR_FAST_INIT_8
#include "WdgM_MemMap.h"

/* TRACE[WDGM213] Only one global supervision Status for the whole WdgM */
/*WDGM_DBG_TST_ENA macro can be defined only for testing purpose.
 *It is used to change the scope of the variable from static to extern.
 *If we define it as global(extern) variable then, there is a chance that it is written outside WdgM module,
 *which can lead to unexpected behavior.
 *Hence, caution: WDGM_DBG_TST_ENA macro should not be defined for Production software.*/
#ifdef WDGM_DBG_TST_ENA
WdgM_GlobalStatusType WdgM_GlobalStatus = WDGM_GLOBAL_STATUS_DEACTIVATED;
#else
static WdgM_GlobalStatusType WdgM_GlobalStatus = WDGM_GLOBAL_STATUS_DEACTIVATED;
#endif
static WdgM_GlobalStatusType WdgM_Prv_SetMode_GlobalStatus = WDGM_GLOBAL_STATUS_DEACTIVATED;

#define WDGM_STOP_SEC_VAR_FAST_INIT_8
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE_SLOW
#include "WdgM_MemMap.h"
/*
 ***************************************************************************************************
 * Public Function Definations
 ***************************************************************************************************
 */


/****************************************************************************************
* Name                :    WdgM_Init
* Description        :    Intilize the Watchdog Manager as per Configurations
* Parameters[in]    :   ConfigPtr
* Limitations        :    Care has to be taken while calling this function as it is not reentrant.
* ReturnType        :    Void
*****************************************************************************************/
/* TRACE[WDGM151] Implementation of WdgM_Init*/
/* TODO: TRACE[WDGM298] WdgMDeadlineStartRef will be set to 0 */
/* TODO: TRACE[WDGM225] Partial Shutdown pending due to lack of support from BswM*/
/* TODO: TRACE[WDGM002] The watchdog shall support mutilple watchdog*/
void WdgM_Init(const WdgM_ConfigType * ConfigPtr)
{
#if((WDGM_OFF_MODE_ENABLED)  == (STD_OFF))
	uint8 TriggerIdx;
	const WdgM_TriggerType * TriggerPtr;
#endif /*((WDGM_OFF_MODE_ENABLED)  == (STD_OFF))*/
	WdgM_SupervisedEntityIdType SEID;
#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
	uint16 DeadlineSupervisionIdx;
#endif /*((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO)) */
	Std_ReturnType ReturnStatus;
#if((WDGM_RTE_DISABLED) == (STD_OFF))
	Rte_PortHandle_WdgM_LocalMode_P PortHandle = Rte_Ports_WdgM_LocalMode_P();
#endif /*(WDGM_RTE_DISABLED == STD_OFF)*/
#if (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON)
	uint8 grpIdx;
#endif /* (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON) */
	/* TODO: Check whether BSW00406(AUTOSAR_SRS_BSWGeneral.pdf) is really required even if
	 * we can detect WdgM initialized or not, without explicit static status variable...!! */

#if (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
	uint16 GraphCounter;
#endif /*(WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)*/

	/* TRACE[WDGM288] To check if WdgM is Deinitilized if not then log error in Det. */
	if (WdgM_GlobalStatus != WDGM_GLOBAL_STATUS_DEACTIVATED)
	{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_APIID , WDGM_E_NO_DEINIT);
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
		return;
	}

#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
	WdgM_MainFunction_Cnt_u32 = WDGM_PRV_C_ZERO;
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/

	/*
	 * WdgM_ConfigSetPtr is assigned according to the variants
	 * If WdgM_ConfigSet is configured at precompile then the sanity checks are done via the script.
	 * Hence in this case no more sanity checking
	 */
#if (defined(WDGM_VARIANT_PRE_COMPILE))
	/* The parameter "ConfigPtr" is void casted, as it is unused during Precompile*/
		(void)ConfigPtr;
		WdgM_ConfigSetPtr = &WdgM_Config;
	/* Else if not Precompile ie POST build hence perform sanity check on ConfigSet*/
#else /*defined(WDGM_VARIANT_PRE_COMPILE)*/
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
	/* TRACE[WDGM255] Check for NULL_PTR*/
	if (ConfigPtr == NULL_PTR)
	{
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_APIID, WDGM_E_INV_POINTER);
		return;
	}
	else
	{
		/* TRACE[WDGM010] Perform Sanity Check fo the ConfigData and load the values */
		ReturnStatus = WdgM_Prv_PbCfgCheck_tu8_Inl(ConfigPtr);
		if(ReturnStatus != E_OK)
		{
			/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
			(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_APIID, WDGM_E_PARAM_CONFIG);
			return;
		}
	}
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
	WdgM_ConfigSetPtr = ConfigPtr;
#endif /*defined(WDGM_VARIANT_PRE_COMPILE)*/
#if((WDGM_OFF_MODE_ENABLED)  == (STD_OFF))
	TriggerPtr = WdgM_ConfigSetPtr->PtrToMode[WdgM_ConfigSetPtr->InitialMode].PtrToTrigger;
	for(TriggerIdx = WDGM_PRV_C_ZERO;
			TriggerIdx < WdgM_ConfigSetPtr->PtrToMode[WdgM_ConfigSetPtr->InitialMode].NoOfTrigger;
			TriggerIdx++)
	{
		if (TriggerPtr[TriggerIdx].WdgMode == WDGIF_OFF_MODE)
		{
			/* TRACE[WDGM030] Since WdgM Mode is in Off Mode Do not intilize the WdgM*/
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
			/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
			(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_APIID, WDGM_E_DISABLE_NOT_ALLOWED);
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
			return;
		}
	}
#endif
	/*TRACE[WDGM269]: Transition: 11: Initally all local SE are DEACTIVATED */
	for(SEID = WDGM_PRV_C_ZERO; SEID < WDGM_NO_OF_SUPERVISED_ENTITIES; SEID++)
	{
		WdgM_SupervisedEntityDyn[SEID].OldLocalStatus = WDGM_LOCAL_STATUS_DEACTIVATED;
		WdgM_SupervisedEntityDyn[SEID].NewLocalStatus = WDGM_LOCAL_STATUS_DEACTIVATED;
		/* TRACE[SWS_WdgM_00296] Init flgActivity to FALSE and idLastReachedCheckpoint to zero
		 * for all WdgM_SupervisedEntityDyn Entries of Internal Graph*/
#if (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON)
		for(grpIdx = WDGM_PRV_C_ZERO; grpIdx < WDGM_MAX_INTERNAL_GRAPH; grpIdx++)
		{
		    WdgM_InternalGraph_StatusDyn[grpIdx].flgActivity = FALSE;
		    WdgM_InternalGraph_StatusDyn[grpIdx].idLastReachedCheckpoint = WDGM_PRV_C_ZERO;
		}
#endif
#if((WDGM_RTE_DISABLED) == (STD_OFF))
		PortHandle[SEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_DEACTIVATED);
#endif/*(WDGM_RTE_DISABLED == STD_OFF)*/
	}
	/*Init flgActivity to FALSE , lastReachedCheckpointId and lastReachedSupervisedEntityId to zero
		 * for all External Graphs Entries */
#if (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
	for(GraphCounter = WDGM_PRV_C_ZERO; GraphCounter < WDGM_MAX_EXTERNAL_GRAPH; GraphCounter++)
	{
		WdgM_ExternalGraph_StatusDyn[GraphCounter].activityFlag = FALSE;
		WdgM_ExternalGraph_StatusDyn[GraphCounter].lastReachedCheckpointId = WDGM_PRV_C_ZERO;
		WdgM_ExternalGraph_StatusDyn[GraphCounter].lastReachedSupervisedEntityId = WDGM_PRV_C_ZERO;
	}
#endif
	WdgM_Prv_SetMode_GlobalStatus = WDGM_GLOBAL_STATUS_DEACTIVATED;


    /* TRACE[WDGM285] Init Complete (except WdgM_SetMode()). Set GLobal Status To OK  */
	/*If WdgM_SetMode() fails, Gloal Status will be changed to STOPPED*/
    WdgM_GlobalStatus = WDGM_GLOBAL_STATUS_OK;
	/* User sets many modes via WdgMMode[].
	 * He will configure one of the Modes as the Initial Mode from WdgM.
	 * Here the user set Initial Mode as WdgM Inital Mode*/
	/* TRACE[WDGM135] Set the Initial Mode of the WdgM*/
	ReturnStatus = WdgM_SetMode(WdgM_ConfigSetPtr->InitialMode);
	/* Configure the Supervision Entities to be used its LOCAL_STATUS to ACTIVE*/
	if(ReturnStatus == E_OK)
	{
		/* TRACE [WDGM350] Clear the Complementary Entries of SEID at Init*/
		WdgM_FirstExpiredSupervisedEntityId = WDGM_PRV_C_ZERO;
		/* Wrong complement is given to detect first expired Supervision */
		WdgM_FirstExpiredSupervisedEntityId_Comp = WDGM_PRV_C_ZERO;
		WdgM_ExpiredSupervisionCycleCtr = WDGM_PRV_C_ZERO;

#if((WDGM_RB_DEBUG_OPTION_ENABLED) == (STD_ON))
		if(WdgM_Rb_FirstExpiredEntity_Cnt_u8 == WDGM_PRV_C_ZERO)
		{
			WdgM_Rb_DebugVariables_Init_v_Inl();
		}
#endif/*(WDGM_RB_DEBUG_OPTION_ENABLED == STD_ON)*/

		/* Alive supervision */
		/* Nothing to do. Already values are set in WdgM_SetMode */
#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
		/* Deadline supervision */
		/* TRACE[WDGM298] All Deadline Start Checkpoints set their timestamps to 0  */
		for(DeadlineSupervisionIdx = WDGM_PRV_C_ZERO;
				DeadlineSupervisionIdx < WDGM_MAX_DEADLINE_SUPERVISIONS;
				DeadlineSupervisionIdx++)
		{
			if(WdgM_ConfigSetPtr->PtrToRunningCounterValue != NULL_PTR)
			{
				WdgM_ConfigSetPtr->PtrToRunningCounterValue[DeadlineSupervisionIdx] = WDGM_PRV_C_ZERO;
			}
		}
#endif /*((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO)) */

	}
	else
	{
		/*TRACE[WDGM139] WdgIf_SetMode is being called from WdgM_SetMode
		 *If SetMode fails then set the GlobalStatus to STOPPED */
		WdgM_GlobalStatus = WDGM_GLOBAL_STATUS_STOPPED;
	}

	return;
}


/*******************************************************************************************
* Name              :   WdgM_DeInit
* Description       :   DeIntilize the Watchdog Manager.
* Parameters[in]    :   None
* Limitations       :   Care has to be taken while calling this function as it is not reentrant.
* ReturnType        :   Void
*******************************************************************************************/
/* TRACE[WDGM261] Implementation of WdgM_DeInit*/
void WdgM_DeInit(void)
{
	/* TRACE[WDGM288] To check if WdgM is Initilized if not then log error in Det. */
	if (WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
	{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_DE_INIT_APIID , WDGM_E_UNINIT);
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
		return;
	}

	if(WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_OK)
	{
		SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
		/* TRACE[WDGM286] Set global status to Deactivated, if Wdg global status is OK*/
		WdgM_GlobalStatus = WDGM_GLOBAL_STATUS_DEACTIVATED;
		SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
	}
	return;
}

#if( (WDGM_VERSION_INFO_API) == (STD_ON) )
/*******************************************************************************************
* Name              :   WdgM_GetVersionInfo
* Description       :   Returns the version information of this module.
* Parameters[out]   :   VersionInfo
* Limitations       :   Care has to be taken while calling this function as it is not reentrant.
* ReturnType        :   Void
*******************************************************************************************/
/* TRACE[WDGM110] Implementation of WDGM_GetVersionInfo whcih can be disabled at preCompile time*/
void WdgM_GetVersionInfo (Std_VersionInfoType * VersionInfo)
{
#if ((WDGM_DEV_ERROR_DETECT) == (STD_ON))
	/* TRACE[WDGM256]  Perform sanity checks on VersionInfo*/
	if (VersionInfo == NULL_PTR)
	{
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_VERSION_INFO_APIID, WDGM_E_INV_POINTER);
		return;
	}
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/

	VersionInfo->vendorID   = WDGM_VENDOR_ID;
	VersionInfo->moduleID   = WDGM_MODULE_ID;
	VersionInfo->sw_major_version = WDGM_SW_MAJOR_VERSION;
	VersionInfo->sw_minor_version = WDGM_SW_MINOR_VERSION;
	VersionInfo->sw_patch_version = WDGM_SW_PATCH_VERSION;
	return;
}
#endif

#define WDGM_STOP_SEC_CODE_SLOW
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE_FAST
#include "WdgM_MemMap.h"

/*******************************************************************************************
* Name              :   WdgM_CheckpointReached
* Description       :   Indicates to the Watchdog Manager that a Checkpoint within a Supervised
*                       Entity has been reached.
* Parameters[in]    :   SEID, CheckpointID
* Limitations       :   None
* ReturnType        :   Std_ReturnType
*******************************************************************************************/
/*TRACE[WDGM263] Implementation of WdgM_CheckpointReached*/
/*TRACE[WDGM200] The local supervision status is monitored via WdgM_CheckpointReached */
/* Port defined argument value has been replaced with a macro as the generated variable could not have a user defined name */

Std_ReturnType WdgM_CheckpointReached(WdgM_SupervisedEntityIdType SEID,
WdgM_CheckpointIdType CheckpointID)
{
    uint16 SvIdx;
    Std_ReturnType ReturnStatus = E_OK;

#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
    Std_ReturnType DeadlineReturnStatus;
#endif

#if (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON) || (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
    Std_ReturnType LogicalReturnStatus;
#endif

    /* TRACE[WDGM279] Check if WdgM is initilized  */
    if (WdgM_GlobalStatus != WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
        /* TRACE[WDGM278] Check for vaild SEID */
        if (SEID < WDGM_NO_OF_SUPERVISED_ENTITIES)
        {
            /* TRACE[WDGM319] Check if for the given SE ,the checkpoint ID belongs to is set to Active in current mode*/
            /* TRACE[WDGM284] Check for checkpointID is within the associated CheckpointID's for a SE*/
            if(CheckpointID < WdgM_SupervisedEntity[SEID].NoOfCheckpoint)
            {
#endif /* ((WDGM_DEV_ERROR_DETECT) == (STD_ON)) */
                SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
                /* Lock introduced:
                 * 1.In WdgM_SupervisedEntityDyn we check for Local status deactivated,
                 * if mode changes here then for newely active or deactive mode we will peform unncessary operation.
                 */
                /*TRACE[WDGM208] Since SEActive is False for DEACTIVATED, no supervision process is performed*/
                if ((WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & WDGM_LOCAL_STATUS_DEACTIVATED) != WDGM_LOCAL_STATUS_DEACTIVATED)
                {
                    /* Alive Supervision */
                    /* Lock introduced:
                     * 1. In WdgM_SupervisedEntityDyn we check for AliveSupervisionIdx,
                     *  which could be corrupted if Mode chage occurs and  WdgM_SupervisedEntityDyn is modified.
                     */
                    SvIdx = WdgM_SupervisedEntityDyn[SEID].AliveSupervisionIdx;
                    /*TRACE[WDGM321] Increment the alive indication counter of reported Checkpoint*/
                    if((SvIdx != WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].NoOfAliveSupervision)&&
                            (WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].PtrToAliveSupervision[SvIdx].AliveSupervisionCheckpointId == CheckpointID))
                    {
                        /* Lock introduced:
                         * 1. the ++ is a read modify write operation ,
                         * read: old value
                         * preempt: WdgM_MainFunction: modify source value to 0
                         * modify : old value + 1
                         * write : old value +1 (source has been made 0 hence final should be 0+1)
                         */
                        WdgM_SupervisedEntityDyn[SEID].IndividualAliveUpdateCtr++;
                    }
                    SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
                }
                else
                {
                    /*Lock Disable: On failure to enter if case release lock*/
                    SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
                    /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
                    (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_APIID, WDGM_E_SEDEACTIVATED);
                    /*When CPR are reported which do not belong to the current mode no action should be taken hence return E_OK */
                    /*only when DET is enabled should reporting be enabled and E_NOT_OK */
                    ReturnStatus = E_NOT_OK;
#endif /* ((WDGM_DEV_ERROR_DETECT) == (STD_ON)) */
                }
                /* Deadline Supervision */
                /* TRACE[WDGM322] Perform Deadline supervision*/
                /* Lock introduced:
                 * 1. During Deadline Checkpoint Reached we refer the WdgM_Mode which has to be atomic.
                 * 2. Local status is modified here hence to prevent
                 */
#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
                WdgM_Prv_DeadlineSupervisionCheckpointReached_v_Inl(SEID, CheckpointID, &DeadlineReturnStatus);
                if (DeadlineReturnStatus != E_OK)
                {
                    ReturnStatus = E_NOT_OK;
                }
#endif
                /* Logical Supervision for Internal Graph*/
#if (WDGM_INTERNAL_GRAPH_CONFIGURED == STD_ON)
                LogicalReturnStatus = WdgM_Prv_LogicalSupervisionCheckpointReached_v_Inl(SEID, CheckpointID);
                if (LogicalReturnStatus != E_OK)
                {
                    ReturnStatus = E_NOT_OK;
                }
#endif
                /* Logical Supervision for External Grpah */
#if (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
                LogicalReturnStatus = WdgM_Prv_ExternalGraphCheckpointReached_v_Inl(SEID, CheckpointID);
                if (LogicalReturnStatus != E_OK)
                {
                    ReturnStatus = E_NOT_OK;
                }
#endif

#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
            }
            else
            {
                /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
                (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_APIID , WDGM_E_CPID);
                ReturnStatus = E_NOT_OK;
            }
        }
        else
        {
            /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
            (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_APIID , WDGM_E_PARAM_SEID);
            ReturnStatus = E_NOT_OK;
        }
#endif /* ((WDGM_DEV_ERROR_DETECT) == (STD_ON)) */
    }
    else
    {
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
        /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_APIID , WDGM_E_UNINIT);
#endif /* ((WDGM_DEV_ERROR_DETECT) == (STD_ON)) */
        ReturnStatus = E_NOT_OK;
    }

    return ReturnStatus;
}

/*******************************************************************************************
* Name              :   WdgM_SetMode
* Description       :   Sets the current mode of Watchdog Manager.
* Parameters[in]    :   Mode
* Limitations       :   Care has to be taken while calling this function as it is not reentrant.
* ReturnType        :   void
*******************************************************************************************/
Std_ReturnType WdgM_SetMode(WdgM_ModeType Mode)
/*TRACE[WDGM154] Implementation of WdgM_SetMode*/
/* TODO: Check whether voilation is possible for AUTOSAR_SRS_BSWGeneral.pdf: WDGM345: BSW00433, BSW00429 */
{
/*Generated only when WDGM_OFF_MODE is disabled*/
#if(WDGM_OFF_MODE_ENABLED  == STD_OFF)
	uint8 ctr;
	const WdgM_TriggerType * TriggerPtr;
#endif/* ((WDGM_OFF_MODE_ENABLED  == STD_OFF)) */

	Std_ReturnType ReturnStatus;

	ReturnStatus = E_NOT_OK;
	/* TRACE[WDGM338] Watchdog Manager Service*/
		/* TRACE[WDGM021] Check if WdgM is initilized if not then log DET */
		if (WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
		{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
			/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
			(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_APIID , WDGM_E_UNINIT);
#endif/* ((WDGM_DEV_ERROR_DETECT) == (STD_ON)) */
			return E_NOT_OK;
		}

		/* TRACE[WDGM020] If the mode is not a valid Mode*/
		if (Mode >= WdgM_ConfigSetPtr->NoOfMode)
		{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
			/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
			(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_APIID, WDGM_E_PARAM_MODE);
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
			return E_NOT_OK;
		}


		/* TRACE [WDGM031] WdgM Disable Mode is not enabled and if off_Mode has been requested then log error */
#if((WDGM_OFF_MODE_ENABLED)  == (STD_OFF))
		TriggerPtr = WdgM_ConfigSetPtr->PtrToMode[Mode].PtrToTrigger;
		for(ctr= WDGM_PRV_C_ZERO; ctr < WdgM_ConfigSetPtr->PtrToMode[Mode].NoOfTrigger; ctr++)
		{
			if (TriggerPtr[ctr].WdgMode == WDGIF_OFF_MODE)
			{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
				/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
				(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_APIID , WDGM_E_DISABLE_NOT_ALLOWED);
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
				return E_NOT_OK;
			}
		}
#endif /*((WDGM_OFF_MODE_ENABLED)  == (STD_OFF))*/

	/*TRACE[WDGM145] perform SetMode only if the Global status is OK or FAILED*/
	if((WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_OK)||(WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_FAILED))
	{
		/* Trigger Condition */
		WdgM_Prv_UpdateTriggerDyn_v_Inl(Mode, &ReturnStatus);
		if(ReturnStatus == E_OK)
		{
			/* Deadline supervision */
			/* Lock introduced:
			* 1. If Mode change is a relevent request we perform update of deadline timings information,
			* which is mode dependent. Hence placed under a lock. so that no CPR occurs here.
			* 2. Update of Global mode shoule be a one time continous activity when set mode operation is valid.
			* 3. The Supervised information is updated in WdgM_Prv_UpdateSupervisedEntityDyn_v_Inl hence in lock
			* 3. Flag update for SetMode indication if not in a lock will
			* cause a problem at End of WdgM_MainFunction. This will cause missing of TRUE to FALSE change and not reset
			*/
			SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
			/* Alive supervision */
			WdgM_Prv_UpdateAliveSupervisionDyn_v_Inl();
#if ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO))
			WdgM_Prv_UpdateDeadlineSupervisionDyn_v_Inl(Mode);
#endif

#if (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON)
			WdgM_Prv_UpdateExternalGraphDyn_v_Inl();
#endif

#if (((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO)) || (WDGM_EXTERNAL_GRAPH_CONFIGURED == STD_ON))
			WdgM_Prv_UpdateSupervisionDyn_v_Inl(Mode);
#endif
			/* WDGM317:RFC request in AutoSAR bugzilla Id : 55705 */
			WdgM_Prv_UpdateSupervisedEntityDyn_v_Inl(Mode);
			WdgM_Mode = Mode;
			SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
			/* LocalStatusParams */
#if((WDGM_RTE_DISABLED) == (STD_OFF))
			if(SchM_Mode_WdgM_Current_WdgMSupervisionCycle() != WdgM_ConfigSetPtr->PtrToMode[Mode].SchMWdgMSupervisionCycle)
			{
				if(SchM_Switch_WdgM_WdgMSupervisionCycle(WdgM_ConfigSetPtr->PtrToMode[Mode].SchMWdgMSupervisionCycle)!=SCHM_E_OK)
				{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
				    /* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
				    (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_APIID, WDGM_E_SCHM_SWITCH);
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
				    return E_NOT_OK;
				}
			}
#endif /*(WDGM_RTE_DISABLED == STD_OFF)*/
#if(WDGM_DEM_ALIVE_SUPERVISION_REPORT == STD_ON)
			if(WdgM_ConfigSetPtr->ErrorSetMode != WDGM_INVALID_DEM_EVENT_ID)
			{
#if(WdgM_AR45_VERSION == STD_ON)
			    (void)Dem_SetEventStatus(WdgM_ConfigSetPtr->ErrorSetMode, DEM_EVENT_STATUS_PASSED);
#else
			    Dem_ReportErrorStatus(WdgM_ConfigSetPtr->ErrorSetMode, DEM_EVENT_STATUS_PASSED);
#endif
			}
#endif /*((WDGM_DEM_ALIVE_SUPERVISION_REPORT) == (STD_ON))*/
		}
		/* SWS_WdgM_00186 & SWS_WdgM_00142 Returns E_NOT_OK when a call to WdgIf_SetMode service fails*/
		else
	   {   /* TRACE[WDGM142] Watchdog Manager shall report to the Diagnostic Event Manager an error with the value WDGM_E_SET_MODE */
			SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
			/* WDGM317: RFC request in AutoSAR bugzilla Id : 55705 */
			WdgM_Prv_SetMode_GlobalStatus = WDGM_GLOBAL_STATUS_STOPPED;
			SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#if(WDGM_DEM_ALIVE_SUPERVISION_REPORT == STD_ON)
			if(WdgM_ConfigSetPtr->ErrorSetMode != WDGM_INVALID_DEM_EVENT_ID)
			{
#if(WdgM_AR45_VERSION == STD_ON)
			    (void)Dem_SetEventStatus(WdgM_ConfigSetPtr->ErrorSetMode, DEM_EVENT_STATUS_FAILED);
#else
			    Dem_ReportErrorStatus(WdgM_ConfigSetPtr->ErrorSetMode, DEM_EVENT_STATUS_FAILED);
#endif
			}
#endif /*((WDGM_DEM_ALIVE_SUPERVISION_REPORT) == (STD_ON))*/
		}
	}
	return (ReturnStatus);
}


/*******************************************************************************************
* Name              :   WdgM_GetMode
* Description       :   Returns the current mode of the Watchdog Manager.
* Parameters[out]   :   Mode
* Limitations       :   None
* ReturnType        :   Std_ReturnType
*******************************************************************************************/
/*[WDGM168] Implementation of WdgM_GetMode*/
/* TODO : TRACE[WDGM170] yet to me mapped*/
Std_ReturnType WdgM_GetMode (WdgM_ModeType * Mode)
{
	/*TRACE[WDGM253] If WdgM is not intilized GetMode should not be service anymore*/
	if (WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED) /*WDGM253*/
	{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_MODE_APIID , WDGM_E_UNINIT);
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
		return E_NOT_OK;
	}
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
	/*TRACE[WDGM254] Check for null parameters*/
	if (Mode == NULL_PTR)
	{
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_MODE_APIID, WDGM_E_INV_POINTER);
		return E_NOT_OK;
	}
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
	*Mode = WdgM_Mode;
	return E_OK;
}

/*******************************************************************************************
* Name              :   WdgM_GetLocalStatus
* Description       :   Returns the supervision status of an individual Supervised Entity.
* Parameters[in]    :   SEID
* Parameters[out]   :   Status
* Limitations       :   None
* ReturnType        :   Std_ReturnType
*******************************************************************************************/
/* TRACE[WDGM169] Implementation of WdgM_GetLocalStatus*/
Std_ReturnType WdgM_GetLocalStatus(WdgM_SupervisedEntityIdType SEID,
	WdgM_LocalStatusType * Status)
{
	/* TRACE[WDGM171] Exclude some checks based on the flags set*/
	/* TRACE[WDGM257] Check if WdgM is initilized*/
	if (WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
	{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_APIID , WDGM_E_UNINIT);
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
		return E_NOT_OK;
	}
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
	/* TRACE[WDGM172] Check for valid SEID*/
	if (SEID >= WDGM_NO_OF_SUPERVISED_ENTITIES)
	{
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_APIID , WDGM_E_PARAM_SEID);
		return E_NOT_OK;
	}
	/* TRACE[WDGM257] Check for valid pointer*/
	if (Status == NULL_PTR)
	{
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_APIID, WDGM_E_INV_POINTER);
		return E_NOT_OK;
	}
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
	/*Interrupt lock: The fetch from a structure could be a non atomic operation*/
	SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
	*Status = WdgM_SupervisedEntityDyn[SEID].OldLocalStatus;
	SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
	return E_OK;
}

/*******************************************************************************************
* Name              :   WdgM_GetGlobalStatus
* Description       :   Returns the global supervision status of the Watchdog Manager.
* Parameters[out]   :   Status
* Limitations       :   None
* ReturnType        :   Std_ReturnType
*******************************************************************************************/
/* TRACE[WDGM175] Implementation of WdgM_GetGlobalStatus*/
/* TODO: TRACE[WDGM258] yet to be mapped */
Std_ReturnType WdgM_GetGlobalStatus(WdgM_GlobalStatusType * Status)
{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
	/* TRACE[WDGM176] Check if WdgM is Initilized*/
	if (WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
	{
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_GLOBAL_STATUS_APIID , WDGM_E_UNINIT);
		return E_NOT_OK;
	}
	/* TRACE[WDGM344] Check if status is a Null Pointer*/
	if (Status == NULL_PTR)
	{
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_GLOBAL_STATUS_APIID, WDGM_E_INV_POINTER);
		return E_NOT_OK;
	}
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
	/* No Lock: 8 bit access is assumed to be atomic*/
	*Status = WdgM_GlobalStatus;
	return E_OK;
}

/*******************************************************************************************
* Name              :   WdgM_PerformReset
* Description       :   Instructs the Watchdog Manager to cause a watchdog reset.
* Parameters[in]    :   void
* Limitations       :   None
* ReturnType        :   void
*******************************************************************************************/
/* TRACE[WDGM264] Implementation of WdgM_PerformReset*/
/* TODO: TRACE[WDGM233] not yet mapped */
void WdgM_PerformReset(void)
{

	/* TRACE[WDGM270] Check for WdgM is initilized*/
	if (WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
	{
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_PERFORM_RESET_APIID , WDGM_E_UNINIT);
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
		return;
	}
	/* TRACE[WDGM232] Set trigger for all configured to zero */
	/* Deactived so that WdgM Main Function in the next cycle will set value to Zero*/
	SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
	WdgM_GlobalStatus = WDGM_GLOBAL_STATUS_DEACTIVATED;
	SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
	WdgM_Prv_SetTriggerCondition(WDGM_GLOBAL_STATUS_DEACTIVATED);
	return;
}

#define WDGM_STOP_SEC_CODE_FAST
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE_SLOW
#include "WdgM_MemMap.h"

/*******************************************************************************************
* Name              :   WdgM_GetFirstExpiredSEID
* Description       :   Returns SEID that first reached the state WDGM_LOCAL_STATUS_EXPIRED.
* Parameters[out]   :   SEID
* Limitations       :   None
* ReturnType        :   Std_ReturnType
* NOTE: Source is WdgM specification version 4.0.3 (Its done for future compatibility)
*******************************************************************************************/
/* TRACE[WDGM346] Implementation of WdgM_GetFirstExpiredSEID*/
/* TODO: TRACE[WDGM348] yet to be mapped*/
Std_ReturnType WdgM_GetFirstExpiredSEID(WdgM_SupervisedEntityIdType * SEID)
{
	Std_ReturnType ReturnStatus = E_NOT_OK;
	WdgM_SupervisedEntityIdType FirstExpiredSupervisedEntityIdTemp;

#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
	/* TRACE[WDGM347] Check for NULL pointer*/
	if (SEID == NULL_PTR)
	{
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_FIRST_EXPIRED_SEID_APIID, WDGM_E_INV_POINTER);
		return E_NOT_OK;
	}
#endif /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/
	/*As per rules a correct entry is one copy of the  FirstExpiredSupervisedEntityId and second copy is
	 *the complement of the FirstExpiredSupervisedEntityId both should match whne either one is inverted*/
	/*TRACE[WDGM349] implement the specific format as defined in the specifcations for SEID*/

	FirstExpiredSupervisedEntityIdTemp = WdgM_Prv_ComplementSeId_to_Inl(WdgM_FirstExpiredSupervisedEntityId_Comp);
	if(WdgM_FirstExpiredSupervisedEntityId == FirstExpiredSupervisedEntityIdTemp)
	{
		*SEID = WdgM_FirstExpiredSupervisedEntityId;
		ReturnStatus = E_OK;
	}/*If no correct entry saved in the LOG. This also takes care of corrupt entry case*/
	else
	{
		*SEID = WDGM_PRV_C_ZERO;
	}
	return ReturnStatus;
}

#define WDGM_STOP_SEC_CODE_SLOW
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"


/*******************************************************************************************
* Name              :   WdgM_MainFunction
* Description       :   Performs the processing of the cyclic Watchdog Manager jobs.
* Parameters[in]    :   void
* Limitations       :   None
* ReturnType        :   void
*******************************************************************************************/
/* TRACE[WDGM159] Implementation of WdgM_MainFunction*/
void WdgM_MainFunction(void)
{  /* AUTOSAR_SRS_BSWGeneral.pdf: WDGM345: BSW00433, BSW00429 allows to be called before OS starts. */
	WdgM_SupervisedEntityIdType SEID;
	WdgM_GlobalStatusType NewGlobalStatus = WDGM_GLOBAL_STATUS_DEACTIVATED;
	/*Introduce GlobalStatusCached:
	*1. If RTE update required we store the WdgM_GlobalStatus and compare with NewGlobalStatus.
	*/
#if(((WDGM_DEM_ALIVE_SUPERVISION_REPORT) == (STD_ON))||((WDGM_RTE_DISABLED) == (STD_OFF))||((WDGM_IMMEDIATE_RESET) == (STD_ON)))
	WdgM_GlobalStatusType GlobalStatusCached ;
#endif

	/*TRACE[SWS_WdgM_00039] If DET is enabled then proceed the global status check.*/
	/*TRACE[SWS_WdgM_00223] If global status is DEACTIVATED then return without calling Set Trigger*/
#if((WDGM_DEV_ERROR_DETECT) == (STD_ON))
	WdgM_MainFunction_Cnt_u32++;
	if (WdgM_GlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
	{
		/* As per AUTOSAR spec of DET, the function 'Det_ReportError' always returns E_OK. So the return value can be ignored, and hence it is type casted to type void.*/
		(void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_MAIN_FUNCTION_APIID , WDGM_E_UNINIT);
		return;
	}
#endif  /*((WDGM_DEV_ERROR_DETECT) == (STD_ON))*/

#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
#else /*(WDGM_LOCK_DISABLE_FOR_DGS == STD_OFF)*/
	SchM_Enter_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
#if(((WDGM_DEM_ALIVE_SUPERVISION_REPORT) == (STD_ON))||((WDGM_RTE_DISABLED) == (STD_OFF))||((WDGM_IMMEDIATE_RESET) == (STD_ON)))
	GlobalStatusCached = WdgM_GlobalStatus;
#endif
	if((WdgM_GlobalStatus != WDGM_GLOBAL_STATUS_DEACTIVATED)&&(WdgM_GlobalStatus != WDGM_GLOBAL_STATUS_STOPPED)&&(WdgM_Prv_SetMode_GlobalStatus != WDGM_GLOBAL_STATUS_STOPPED))
	{   /*Enter here if global mode is OK or FAILED*/
		if (WdgM_GlobalStatus != WDGM_GLOBAL_STATUS_EXPIRED)
		{
			/* Update Alive Supervision for All the SupervisedEntity */
			WdgM_Prv_AliveSupervisionMainFunction_v_Inl();
			/* Update Deadline Supervision for All the SupervisedEntity */
			/* Feature enhancement as per comment 11 of Bugzilla Id : 56735 */
#if((WDGM_DEADLINE_TIMEOUT_DETECT == STD_ON) && ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO)))
			WdgM_Prv_DeadlineSupervisionMainFunction_v_Inl();
#endif /*((WDGM_DEADLINE_TIMEOUT_DETECT == STD_ON) && ((WDGM_MAX_DEADLINE_SUPERVISIONS) > (WDGM_PRV_C_ZERO)))*/
			/* Local State Machine:  */
			WdgM_Prv_LocalStatusMainFunction();
			/* Global State Machine:  */
			/* TRACE[WDGM214] Global State Machine should run after Local State Machine*/
			/* TRACE[WDGM218] If global is OK or FAILED and if local is DEACTIVATE or OK then global state is set to OK*/
			/* TRACE[WDGM078] If global is OK and if local is DEACTIVATE then global state is set to OK*/
			NewGlobalStatus = WDGM_GLOBAL_STATUS_OK;
			for(SEID = WDGM_PRV_C_ZERO; SEID < WDGM_NO_OF_SUPERVISED_ENTITIES; SEID++)
			{
				/* Check for all SE if any local status is EXPIRED, if yes the change gloabal status to STOPPED*/
				/* No lock for Local status as if previous is ok or expired then we work on old value and finally we change to expired*/
				if ((WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & WDGM_LOCAL_STATUS_EXPIRED) == WDGM_LOCAL_STATUS_EXPIRED)
				{
					/* TRACE[WDGM215] If Global status is OK and any local
					 * status is EXPIRED  then global status is EXPIRED and increment Expried Supervision Cycle Counter*/
					/* TRACE[WDGM077] If Global status is FAILED an any local
					 *  status is EXPIRED  then global status is EXPIRED and increment Expried Supervision Cycle Counter */
					NewGlobalStatus = WDGM_GLOBAL_STATUS_EXPIRED;
					WdgM_ExpiredSupervisionCycleCtr++;
					/* TRACE[WDGM216] Check if global status is OK, any SEID's
					 * local status is EXPIRED? if yes then global status is EXPIRED
					 * further if expired tolerance is zero change global status to STOPPED */
					/* TRACE[WDGM117]  Check if global status is FAILED any SEID's
					 * local status is EXPIRED? if yes then global status is EXPIRED
					 * further if expired tolerance is zero change global status to STOPPED */
					if (WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].ExpiredSupervisionCycleTol == WDGM_PRV_C_ZERO)
					{
						NewGlobalStatus = WDGM_GLOBAL_STATUS_STOPPED;
					}
					break;
				}
				/* TRACE[WDGM217] if global and local status is FAILED
				 * and no local STATUS is EXPIRED the global status will remain FAILED*/
				/* TRACE[WDGM076] if global status is OK and any local status is FAILED
				 * and no local is EXPIRED the global status will remain FAILED*/
				/* No lock for Local status as if previous is ok or expired then we work on old local status*/
				else if ((WdgM_SupervisedEntityDyn[SEID].NewLocalStatus & WDGM_LOCAL_STATUS_FAILED)==WDGM_LOCAL_STATUS_FAILED)
				{
					NewGlobalStatus = WDGM_GLOBAL_STATUS_FAILED;
				}
				else
				{
					/* Code should not come here this else is for MISRA warning*/
				}
			}
		}
		else
		{
			NewGlobalStatus = WDGM_GLOBAL_STATUS_EXPIRED;

			if (WdgM_ExpiredSupervisionCycleCtr == WdgM_ConfigSetPtr->PtrToMode[WdgM_Mode].ExpiredSupervisionCycleTol)
			{
				/*Interrupt Lock: Disabled
				 *1. Global Status is changed to STOPPED, outside the lock
				 *   only SetMode can change to STOPPED , so no need for lock hence forth.
				 */
				/* TRACE[WDGM220] If global is EXPIRED and local status atleast one is
				 * EXPIRED and counter equal to tolerance make global STOPPED */
				NewGlobalStatus = WDGM_GLOBAL_STATUS_STOPPED;
				/* TRACE[WDGM129] When global status changed to STOPPED inform DEM*/
			}
			else
			{
            /* TRACE[WDGM219] If global is EXPIRED and local status atleast one is
             * EXPIRED and counter less than tolerance keep global
             * EXPIRED and increment counter*/
			WdgM_ExpiredSupervisionCycleCtr++;
			}

		} /* TRACE[WDGM221] Since no check for global state STOPPED hence the global status will remain STOPPED */
	}
	else
	{
		if(WdgM_Prv_SetMode_GlobalStatus == WDGM_GLOBAL_STATUS_STOPPED)
		{
			NewGlobalStatus = WDGM_GLOBAL_STATUS_STOPPED;
		}
		else
		{
			NewGlobalStatus = WdgM_GlobalStatus;
		}
	}
#if((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))
#else /*(WDGM_LOCK_DISABLE_FOR_DGS == STD_OFF)*/
	SchM_Exit_WdgM(WDGM_SCHM_EXCLSV_AREA);
#endif/*((WDGM_LOCK_DISABLE_FOR_DGS) == (STD_ON))*/
	/*Move DEM outside the interrupt lock */
#if(WDGM_DEM_ALIVE_SUPERVISION_REPORT == STD_ON)
	if(WdgM_ConfigSetPtr->ErrorSupervision != WDGM_INVALID_DEM_EVENT_ID)
	{
	  if((NewGlobalStatus == WDGM_GLOBAL_STATUS_STOPPED) && (GlobalStatusCached != WDGM_GLOBAL_STATUS_STOPPED))
	  {
#if(WdgM_AR45_VERSION == STD_ON)
          (void)Dem_SetEventStatus(WdgM_ConfigSetPtr->ErrorSupervision, DEM_EVENT_STATUS_FAILED);
#else
          Dem_ReportErrorStatus(WdgM_ConfigSetPtr->ErrorSupervision, DEM_EVENT_STATUS_FAILED);
#endif
	  }
      else
      {
#if(WdgM_AR45_VERSION == STD_ON)
          (void)Dem_SetEventStatus(WdgM_ConfigSetPtr->ErrorSupervision, DEM_EVENT_STATUS_PASSED);
#else
          Dem_ReportErrorStatus(WdgM_ConfigSetPtr->ErrorSupervision, DEM_EVENT_STATUS_PASSED);
#endif
      }
	}
#endif /*((WDGM_DEM_ALIVE_SUPERVISION_REPORT) == (STD_ON))*/
#if((WDGM_RTE_DISABLED) == (STD_OFF))
	/* TRACE[WDGM198] */
	if(((GlobalStatusCached!= WDGM_GLOBAL_STATUS_DEACTIVATED) && (GlobalStatusCached!=WDGM_GLOBAL_STATUS_STOPPED))||(WdgM_Prv_Rte_Switch_globalMode_Status!=RTE_E_OK))
	{
		if ((NewGlobalStatus != GlobalStatusCached)||(WdgM_Prv_Rte_Switch_globalMode_Status!=RTE_E_OK))
		{
			switch (NewGlobalStatus)
			{
				case WDGM_GLOBAL_STATUS_OK:
				    WdgM_Prv_Rte_Switch_globalMode_Status = Rte_Switch_WdgM_globalMode_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_OK);
					break;
				case WDGM_GLOBAL_STATUS_FAILED:
				    WdgM_Prv_Rte_Switch_globalMode_Status = Rte_Switch_WdgM_globalMode_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_FAILED);
					break;
				case WDGM_GLOBAL_STATUS_EXPIRED:
				    WdgM_Prv_Rte_Switch_globalMode_Status = Rte_Switch_WdgM_globalMode_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_EXPIRED);
					break;
				case WDGM_GLOBAL_STATUS_STOPPED:
					/* TRACE[WDGM133] Perform MCU Reset*/
#if((WDGM_IMMEDIATE_RESET) == (STD_ON))
				    if (GlobalStatusCached != WDGM_GLOBAL_STATUS_STOPPED)
				    {
				        Mcu_PerformReset();
				    }
					/* TRACE[WDGM134] Incase of a MCU reset do not inform RTE*/
#else/*(WDGM_IMMEDIATE_RESET == STD_OFF)*/
				    WdgM_Prv_Rte_Switch_globalMode_Status = Rte_Switch_WdgM_globalMode_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_STOPPED);
#endif /*((WDGM_IMMEDIATE_RESET) == (STD_ON))*/
					break;
				default: /* WDGM_GLOBAL_STATUS_DEACTIVATED: Reset is pending...!!! */
					break;
			}
		}
	}
#elif((WDGM_IMMEDIATE_RESET) == (STD_ON))
	if((NewGlobalStatus == WDGM_GLOBAL_STATUS_STOPPED)&& (GlobalStatusCached != WDGM_GLOBAL_STATUS_STOPPED))
	{
		Mcu_PerformReset();
	}
#endif /*(WDGM_RTE_DISABLED == STD_OFF)*/
	/* TRACE[WDGM223] Based on global ststua the trigger conditions are set */
	/*Send trigger values the Watchdog*/
	WdgM_Prv_SetTriggerCondition(NewGlobalStatus);
	WdgM_GlobalStatus = NewGlobalStatus;
	return;
}

#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
