

#include "Dem_Internal.h"
#include "Rte_Dem.h"

#include "Dem_Deb.h"

#include "Dem_DebArTime.h"
#include "Dem_DebMonInternal.h"
#include "Dem_DebCtrBaseClass.h"
#include "Dem_EvMemGen.h"
#include "Dem_DTCs.h"
#include "Dem_OperationCycle.h"
#include "Dem_EventFHandling.h"
#include "Dem_Cfg_ExtPrototypes.h"
#include "Dem_DTCGroup.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"



Std_ReturnType Dem_DebGetDebounceCounter4Calculation(Dem_EventIdType EventId, sint16 *debounceCounter)
{
#if (DEM_CFG_DEBMONINTERNAL == DEM_CFG_DEBMONINTERNAL_ON)
    sint8 faultDetectionCounter;
    const void* paramSet;
#endif
    Std_ReturnType result;

    *debounceCounter = 0;

#if (DEM_CFG_DEBMONINTERNAL == DEM_CFG_DEBMONINTERNAL_ON)
    if (Dem_EvtParam_GetDebounceMethodIndex(EventId) == DEM_DEBMETH_IDX_MONINTERNAL)
    {
        paramSet = Dem_Cfg_DebClasses[Dem_EvtParam_GetDebounceMethodIndex(EventId)].paramSet;

        /* Get the FDC value from the application. */
        /* MR12 RULE 11.5 VIOLATION: a fast and resource optimized support of different debounce startegies is only possible through this mechanism  */
        if(((const Dem_DebounceMonitorInternal*) paramSet)[Dem_EvtParam_GetDebounceParamSettingIndex(EventId)].funcPointer_GetFDC != NULL_PTR)
        {
            /* MR12 RULE 11.5 VIOLATION: a fast and resource optimized support of different debounce startegies is only possible through this mechanism  */
            result = ((const Dem_DebounceMonitorInternal*) paramSet)[Dem_EvtParam_GetDebounceParamSettingIndex(EventId)].funcPointer_GetFDC( &faultDetectionCounter);
        }
#ifdef DEM_CFG_GENERAL_GETFDC
        else if (Dem_EvtParam_GetIsGlobalFDCCallbackConfigured(EventId))
        {
            result = DEM_CFG_GENERAL_GETFDC(EventId, &faultDetectionCounter);
        }
#endif
        else
        {
            return DEM_E_NO_FDC_AVAILABLE;
        }

        *debounceCounter = faultDetectionCounter;
    }
else
#endif
{
    if (Dem_EvtIsResetFailureFilterRequested(EventId)
#if (!DEM_CFG_DTCSETTINGBLOCKSREPORTING)
			 /*MR12 RULE 13.5 VIOLATION: to improve software quality and mantainability, the bit-manipulation
	         * operations of element-attributes are encapsulated in getter/setter functions.
	         * Qac is incapable of recognizing, that a getter function just retrieves information and does
	         * not manipulate any data. therefore it produces lots of false warnings.*/
          || (Dem_EvtIsResetFailureFilterRequestedAfterDtcSetting(EventId) && Dem_IsEventEnabledByDtcSetting(EventId))
#endif
            )
    {
        *debounceCounter = 0;
    }
    else
    {
        *debounceCounter = Dem_EvtGetDebounceLevel(EventId);
    }
    result = E_OK;
}
return result;
}

sint8 Dem_DebCalculateFaultDetectionCounter(Dem_EventIdType EventId, sint16 debounceCounter)
{
	Dem_DebGetLimits funcPoint;
	const void* paramSet;
	sint16_least MinThreshold;
	sint16_least MaxThreshold;

#if (DEM_CFG_DEBMONINTERNAL == DEM_CFG_DEBMONINTERNAL_ON)
	if(Dem_EvtParam_GetDebounceMethodIndex(EventId) == DEM_DEBMETH_IDX_MONINTERNAL)
	{
		/* calculation was already done in Dem_DebGetDebounceCounter4Calculation. */
		return (sint8)debounceCounter;
	}
	else
#endif
	{
		funcPoint
				= Dem_Cfg_DebClasses[Dem_EvtParam_GetDebounceMethodIndex(EventId)].funcPointer_GetLimits;
		paramSet = Dem_Cfg_DebClasses[Dem_EvtParam_GetDebounceMethodIndex(EventId)].paramSet;

		(*funcPoint) (paramSet, Dem_EvtParam_GetDebounceParamSettingIndex(EventId), &MinThreshold,
				&MaxThreshold);

		{
			if (debounceCounter > 0)
			{
				DEM_ASSERT(MaxThreshold > 0,DEM_DET_APIID_GETFAULTDETECTIONCOUNTER, 0);

				return (sint8) (((sint32)debounceCounter * DEM_DEB_DEBLEVEL_MAXTHRESHOLD) / (MaxThreshold));
			}
			else if (debounceCounter < 0)
			{
			    DEM_ASSERT(MinThreshold < 0,DEM_DET_APIID_GETFAULTDETECTIONCOUNTER,1);

				return  (sint8) (((sint32)debounceCounter * DEM_DEB_DEBLEVEL_MINTHRESHOLD) / (MinThreshold));
			}
			else
			{
			   /* Do Nothing */
			}
		}
		return 0;
	}
}



Std_ReturnType Dem_GetFaultDetectionCounter(Dem_EventIdType EventId, sint8* FaultDetectionCounter)
{
	sint16 debounceCounter;
	Std_ReturnType result;
	sint8 faultDetectionCtrVal = 0;

	DEM_ENTRY_CONDITION_CHECK_PREINIT_EVTIDVALID_EVTAVAILABLE(EventId,DEM_DET_APIID_GETFAULTDETECTIONCOUNTER,E_NOT_OK);
	DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR_WITHEVENTID(EventId,FaultDetectionCounter,DEM_DET_APIID_GETFAULTDETECTIONCOUNTER,E_NOT_OK);

	result = Dem_DebGetDebounceCounter4Calculation (EventId, &debounceCounter);
	faultDetectionCtrVal = Dem_DebCalculateFaultDetectionCounter(EventId, debounceCounter);

	if (result == E_OK)
	{
		*FaultDetectionCounter = faultDetectionCtrVal;
	}
	return result;
}

#ifdef RTE_TYPE_H
Std_ReturnType Dem_GetFaultDetectionCounter_GeneralEvtInfo(Dem_EventIdType EventId, sint8* FaultDetectionCounter)
{
    return Dem_GetFaultDetectionCounter(EventId, FaultDetectionCounter);
}
#endif /* RTE_TYPE_H */

Std_ReturnType Dem_ResetEventDebounceStatus (Dem_EventIdType EventId, Dem_DebounceResetStatusType DebounceResetStatus)
{
    DEM_ENTRY_CONDITION_CHECK_PREINIT_EVTIDVALID_EVTAVAILABLE(EventId,DEM_DET_APIID_DEM_RESETEVENTDEBOUNCESTATUS,E_NOT_OK);

    DEM_ENTERLOCK_MON();

	Dem_EvtSetLastReportedEvent(EventId, DEM_EVENT_STATUS_INVALIDREPORT);
	if (DebounceResetStatus == DEM_DEBOUNCE_STATUS_RESET)
	{
		Dem_EvtSetDebounceLevel(EventId, 0);
	}

	DEM_EXITLOCK_MON();
	return E_OK;
}

Std_ReturnType Dem_ManipulateEventDebounceStatus (Dem_EventIdType EventId, Dem_DebugDataType debug0, Dem_DebugDataType debug1, sint16 manipulationValue)
{
	sint32 maxThreshold = 0;
	sint32 minThreshold = 0;
	const void* paramSet;
	uint16 paramIndex = 0;
	sint32 jumpUpValue = 0;
	sint32 jumpDownValue = 0;
	boolean isJumpUpEnabled = FALSE;
	boolean isJumpDownEnabled = FALSE;

	sint32 debLevel;
	Dem_EventStatusType status = DEM_EVENT_STATUS_INVALIDREPORT;

#if (DEM_CFG_DEBUGDATA == DEM_CFG_DEBUGDATA_OFF)
		DEM_UNUSED_PARAM(debug0);
		DEM_UNUSED_PARAM(debug1);
#endif

	if (!Dem_isEventIdValid(EventId))
	{
		return E_NOT_OK;
	}

	if (!Dem_OpMoIsInitialized())
	{
		DEM_DET(DEM_DET_APIID_SETEVENTSTATUS, DEM_E_UNINIT,EventId);
		return E_NOT_OK;
	}

	/* MR12 RULE 13.5 VIOLATION: Getter function is identified as an expression causing side effect. This warning can be ignored. */
	if ( !Dem_EvtAllEnableConditionsFulfilled(EventId)
		|| !Dem_IsOperationCycleStarted(Dem_EvtParam_GetOperationCycleID(EventId))
		|| Dem_EvtIsSuppressed(EventId)
		|| !Dem_IsEventReportingEnabledByDtcSetting(EventId))
	{
		return E_NOT_OK;
	}

	if (Dem_EvtParam_GetDebounceMethodIndex(EventId) == 0)
	{
		return E_NOT_OK;
	}

#if (DEM_CFG_DEBARTIMEBASE == DEM_CFG_DEBARTIMEBASE_ON)
	if(Dem_EvtParam_GetDebounceMethodIndex(EventId) == DEM_DEBMETH_IDX_ARTIME)
	{
		return E_NOT_OK;
	}
#endif

	paramSet = Dem_Cfg_DebClasses[Dem_EvtParam_GetDebounceMethodIndex(EventId)].paramSet;
	paramIndex = Dem_EvtParam_GetDebounceParamSettingIndex(EventId);

#if(DEM_CFG_DEBCOUNTERBASECLASS == DEM_CFG_DEBCOUNTERBASECLASS_ON)
	if(Dem_EvtParam_GetDebounceMethodIndex(EventId) == DEM_DEBMETH_IDX_ARCTRBASECLASS)
	{
        minThreshold = Dem_getDebCtrBaseClassPassedThreshold(paramSet,paramIndex);
        maxThreshold = Dem_getDebCtrBaseClassFailedThreshold(paramSet,paramIndex);
        jumpDownValue = Dem_getDebCtrBaseClassJumpDownValue(paramSet,paramIndex);
        jumpUpValue = Dem_getDebCtrBaseClassJumpUpValue(paramSet,paramIndex);
        isJumpDownEnabled = Dem_getDebCtrBaseClassIsJumpDown(paramSet,paramIndex);
        isJumpUpEnabled = Dem_getDebCtrBaseClassIsJumpUp(paramSet,paramIndex);
	}
#endif

	debLevel = Dem_EvtGetDebounceLevel (EventId);

	if (Dem_EvtIsResetFailureFilterRequested(EventId))
	{
		debLevel = 0;
		DEM_ENTERLOCK_MON();
		Dem_EvtRequestResetFailureFilter(EventId, FALSE);
		DEM_EXITLOCK_MON();
	}

   /*--------DISTURBANCE_MEMORY---------------------------------------------------------------------------------*/
#if (DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)
	if (manipulationValue < 0)
	{
		if ((debLevel < maxThreshold) && (Dem_EvtGetLastReportedEvent(EventId) == DEM_EVENT_STATUS_PREFAILED))
		{
		    Dem_DistMemReportPassed(EventId);
		}
	}
#endif

	/*--------Check for minimum threshold---------------------------------------------------------------------------------*/

	if((debLevel == minThreshold) && (manipulationValue < 0))
	{

		return E_OK;
	}
	/*--------Check for maximum threshold---------------------------------------------------------------------------------*/
	if ((debLevel == maxThreshold) && (manipulationValue > 0))
	{

		return E_OK;
	}


	/*--------Jump down condition full filled?---------------------------------------------------------------------------------*/
	if ((manipulationValue < 0) && (((Dem_EvtGetLastReportedEvent (EventId) == DEM_EVENT_STATUS_PREFAILED) || (Dem_EvtGetLastReportedEvent (
					EventId) == DEM_EVENT_STATUS_FAILED)) && isJumpDownEnabled && (debLevel > jumpDownValue)
					&& ((debLevel == maxThreshold) || (DEM_CFG_LIMIT_RELEVANTFOR_JUMPING != DEM_CFG_LIMIT_RELEVANTFOR_JUMPING_ON))
	))
	{
		debLevel = jumpDownValue;
	}

	/*--------Jump up condition full filled?---------------------------------------------------------------------------------*/
	if ((manipulationValue > 0) && ((((Dem_EvtGetLastReportedEvent (EventId) == DEM_EVENT_STATUS_PREPASSED) || (Dem_EvtGetLastReportedEvent (
			EventId) == DEM_EVENT_STATUS_PASSED))  && isJumpUpEnabled && (debLevel < jumpUpValue))
			&&((debLevel == minThreshold)  || (DEM_CFG_LIMIT_RELEVANTFOR_JUMPING != DEM_CFG_LIMIT_RELEVANTFOR_JUMPING_ON))
	))
	{
		debLevel = jumpUpValue;
	}

	/*------------------Manipulation-----------------------------------------------------------------------*/
	debLevel = debLevel + manipulationValue;
	if (debLevel <= minThreshold)
	{
		debLevel = minThreshold;
		status = DEM_EVENT_STATUS_PASSED;
	}
	else if(debLevel < 0)
	{
		status = DEM_EVENT_STATUS_PREPASSED;
	}
	else
	{
		/* do nothing */
	}

	if (debLevel >= maxThreshold)
	{
		debLevel = maxThreshold;
		status = DEM_EVENT_STATUS_FAILED;
	}
	else if(debLevel > 0)
	{
		status = DEM_EVENT_STATUS_PREFAILED;
	}
	else
	{
		/* do nothing */
	}

	Dem_EvtSetDebounceLevel (EventId, (sint16) debLevel);
	Dem_EvtSetLastReportedEvent (EventId, status);

	if ((status == DEM_EVENT_STATUS_PASSED) || (status == DEM_EVENT_STATUS_FAILED))
    {

	#if (DEM_CFG_SETEVENTSTATUSALLOWEDCALLBACK == DEM_CFG_SETEVENTSTATUSALLOWEDCALLBACK_ON)
		  if (Dem_SetEventStatusAllowedHook(EventId, status) == E_OK)
	#endif
      {
		  /* MR12 RULE 20.7 VIOLATION: Functions like MACROS do not need to be parenthesized.*/
    	  Dem_EvtProcessPassedAndFailed (EventId, status
    	                                 DEM_DEBUGDATA_PARAM(debug0 ,debug1));


         #if (DEM_CFG_SETEVENTSTATUSCALLNOTIFICATION == DEM_CFG_SETEVENTSTATUSCALLNOTIFICATION_ON)
         Dem_SetEventStatusCallNotification (EventId, faultlevel, debug0, debug1);
         #endif
      }
    }

   /*--------DISTURBANCE_MEMORY---------------------------------------------------------------------------------*/
#if(DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)
	if(Dem_DistMemIsReportFailedNecessary(EventId, status))
	{
			/* MR12 RULE 20.7 VIOLATION: Functions like MACROS do not need to be parenthesized.*/
			Dem_DistMemReportFailed(EventId
									DEM_DEBUGDATA_PARAM(debug0 ,debug1));
	}
#endif

   if (status == DEM_EVENT_STATUS_FAILED)
   {
      Dem_EvMemGenReportFailedEvent(EventId);
   }

	return E_OK;
}

/*------------------HandleDebouncerResetConditions-----------------------------------------------------------------------*/
boolean Dem_DebHandleResetConditions (Dem_EventIdType EventId)
{
    boolean continueProcessing = TRUE;

    DEM_ASSERT_ISLOCKED();

    /* optimization only */
     if ((Dem_EvtIsResetFailureFilterRequestedAfterDtcSetting(EventId))
#if (DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO != DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO_OFF)
          || ((Dem_StoCoAreAllFulfilled(Dem_EvtParam_GetStorageConditions(EventId))) && (Dem_EvtIsResetFailureFilterRequestedAfterStorageCondition(EventId)))

#endif
      )
      {
          Dem_EvtSetDebounceLevel(EventId, 0);
          Dem_EvtRequestResetFailureFilterAfterDtcSetting(EventId, FALSE);
#if (DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO != DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO_OFF)
         Dem_EvtRequestResetFailureFilterAfterStorageCondition(EventId, FALSE);
#endif
          return continueProcessing;

       }

    /* MR12 RULE 13.5 VIOLATION: Getter function is identified as an expression causing side effect. This warning can be ignored. */
    if (Dem_IsDtcSettingDisabled() && (!Dem_IsEventEnabledByDtcSetting(EventId)))
    {
#if (DEM_CFG_DTCSETTINGBLOCKSREPORTING)
        if(Dem_EvtParam_GetDebounceBehavior(EventId) == DEM_DEBOUNCE_RESET)
        {
            Dem_EvtSetDebounceLevel(EventId, 0);
        }
        Dem_EvtSetLastReportedEvent(EventId, DEM_EVENT_STATUS_INVALIDREPORT);
        continueProcessing = FALSE;
#else
        if(Dem_EvtParam_GetDebounceBehavior(EventId) == DEM_DEBOUNCE_RESET)
        {
            Dem_EvtRequestResetFailureFilterAfterDtcSetting(EventId, TRUE);
        }
#endif
    }

#if (DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO != DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO_OFF)
    if (!Dem_StoCoAreAllFulfilled(Dem_EvtParam_GetStorageConditions(EventId)))
    {
        if( (!Dem_EvtIsResetFailureFilterRequestedAfterStorageCondition(EventId)) && (Dem_EvtParam_GetDebounceBehavior(EventId) == DEM_DEBOUNCE_RESET) )
        {
            Dem_EvtRequestResetFailureFilterAfterStorageCondition(EventId, TRUE);
        }
    }
#endif

    if (!Dem_EvtAllEnableConditionsFulfilled(EventId))
    {
        if(Dem_EvtParam_GetDebounceBehavior(EventId) == DEM_DEBOUNCE_RESET)
        {
            Dem_EvtSetDebounceLevel(EventId, 0);
        }
        Dem_EvtSetLastReportedEvent(EventId, DEM_EVENT_STATUS_INVALIDREPORT);
        continueProcessing = FALSE;
    }

	if (Dem_EvtIsResetFailureFilterRequested(EventId))
    {
        Dem_EvtRequestResetFailureFilter(EventId, FALSE);
        Dem_EvtSetDebounceLevel (EventId, 0);
    }

    return continueProcessing;
}

DEM_INLINE uint8 Dem_getEventDebouncing(Dem_EventIdType EventId)
{
    sint16 debounceCounter;
    /* BSW-8793 */ /* [$DD_BSWCODE 17257] */
    boolean areStocoFulfilled;
    boolean isEnCoFulfilled;
    /* END BSW-8793 */
    sint8 faultDetectionCtrVal = 0;
    uint8 result = 0;

    (void)Dem_DebGetDebounceCounter4Calculation(EventId, &debounceCounter);
    faultDetectionCtrVal = Dem_DebCalculateFaultDetectionCounter(EventId, debounceCounter);
    /* BSW-8793 */ /* [$DD_BSWCODE 17257] */
    areStocoFulfilled = Dem_StoCoAreAllFulfilled(Dem_EvtParam_GetStorageConditions(EventId));
    isEnCoFulfilled = Dem_EvtAllEnableConditionsFulfilled(EventId);
    /* END BSW-8793 */

    if (faultDetectionCtrVal != 0)
    {
        if ((faultDetectionCtrVal > 0) && (faultDetectionCtrVal < 127))
        {
            result |= DEM_TEMPORARILY_DEFECTIVE;
        }
        if (faultDetectionCtrVal == 127)
        {
            result |= DEM_FINALLY_DEFECTIVE;
        }
        if ((faultDetectionCtrVal > -128) && (faultDetectionCtrVal < 0))
        {
            result |= DEM_TEMPORARILY_HEALED;
        }
        if ((faultDetectionCtrVal == 127) || (faultDetectionCtrVal == -128))
        {
            result |= DEM_TEST_COMPLETE;
        }
        /* BSW-8793 */
        if ((isEnCoFulfilled != 0) && (areStocoFulfilled != 0) && ((faultDetectionCtrVal == 127) || (faultDetectionCtrVal == -128)))
        {
            result |= DEM_DTR_UPDATE;
        }
        /* END BSW-8793 */
    }

    return result;
}

Std_ReturnType Dem_GetDebouncingOfEvent(Dem_EventIdType EventId, Dem_DebouncingStateType* DebouncingState)
{
    DEM_ENTRY_CONDITION_CHECK_PREINIT_EVTIDVALID_EVTAVAILABLE(EventId,DEM_DET_APIID_DEM_GETDEBOUNCINGOFEVENT,E_NOT_OK);
    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR_WITHEVENTID(EventId,DebouncingState,DEM_DET_APIID_DEM_GETDEBOUNCINGOFEVENT,E_NOT_OK);

#if (DEM_CFG_DEBMONINTERNAL == DEM_CFG_DEBMONINTERNAL_ON)
    if(Dem_EvtParam_GetDebounceMethodIndex(EventId) == DEM_DEBMETH_IDX_MONINTERNAL)
    {
        DEM_DET(DEM_DET_APIID_DEM_GETDEBOUNCINGOFEVENT, DEM_E_WRONG_CONFIGURATION,EventId);
        return E_NOT_OK;
    }
#endif

    *DebouncingState = Dem_getEventDebouncing(EventId);
    return E_OK;
}

#ifdef RTE_TYPE_H
Std_ReturnType Dem_GetDebouncingOfEvent_GeneralEvtInfo(Dem_EventIdType EventId, Dem_DebouncingStateType* DebouncingState)
{
    return Dem_GetDebouncingOfEvent(EventId, DebouncingState);
}
#endif /* RTE_TYPE_H */

void Dem_DebMainFunction(void)
{
#if (!DEM_CFG_DEBTIMEBASED_CONFIGURED_CYCLICTASKTIME)
    Dem_TimeBasedDebounceInternMainFunction();
#endif
}

#if (DEM_CFG_DEBTIMEBASED_CONFIGURED_CYCLICTASKTIME) /*configured as seperate cyclic task for Time based Debounce*/
void Dem_TimeBasedDebounceMainFunction(void)
 {
     /*Check if Dem is initialized already, as this function is scheduled independant of Dem_MainFunction*/
     if(Dem_OpMoIsInitialized())
     {
        Dem_TimeBasedDebounceInternMainFunction();
     }
 }
#endif /*DEM_CFG_DEBTIMEBASED_CONFIGURED_CYCLICTASKTIME*/



#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
