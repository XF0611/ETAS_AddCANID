

#ifndef DEM_EVENTS_H
#define DEM_EVENTS_H

#include "Dem_Internal.h"

#include "Dem_Array.h"
#include "Dem_BitArray.h"
#include "rba_DiagLib_Bits8.h"
#include "rba_DiagLib_Bits16.h"
#include "rba_DiagLib_Bits32.h"
#include "Dem_ISO14229Byte.h"

#include "Dem_Clear.h"
#include "Dem_Cfg_Events.h"
#include "Dem_Cfg_Deb.h"
#include "Dem_Cfg_EvBuff.h"
#include "Dem_Cfg_Nodes.h"
#include "Dem_EventCategory.h"
#include "Dem_StorageCondition.h"       // -> situationfilterCFG
#include "Dem_EnableCondition.h"         // -> enableconditionCFG
#include "Dem_Cfg_Main.h"
#include "Dem_Cfg_OperationCycle.h"
#include "Dem_Cfg_DistMem.h"
#include "Dem_Cfg_StorageCondition.h"
#include "Dem_Nvm.h"
#include "Dem_IndicatorAttributes.h"
#include "Dem_Indicator.h"
#include "Dem_EventStatusNvData.h"

#include "Dem_Cfg_Events_DataStructures.h"
#if (DEM_CFG_EVT_ATTRIBUTE == DEM_CFG_EVT_ATTRIBUTE_ON)
#include "Dem_EventAttribute.h"
#endif


typedef Std_ReturnType (* Dem_InitMForEAllowedFncType) (Dem_InitMonitorReasonType InitMonitorReason);
typedef struct
{
/* Pointer to InitMonitorForEvent Callback */
  uint16 Eventid;
  Dem_InitMForEAllowedFncType InitMForEAllwdCBFnc;
}Dem_InitMonitorForE;

typedef struct
{
    Dem_EvtStateType  state;
    sint16 debounceLevel;
} Dem_EvtState;

typedef struct
{
    Dem_EventStatusType lastReportedEvent;
} Dem_EvtState8;

typedef void (* Dem_EvtStatusChangeFncType) ( Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatusOld, Dem_UdsStatusByteType EventStatusNew );


#define DEM_EVTPARAM_DEBPARAM_SHIFT   4u
#define DEM_EVTPARAM_DEBMETHOD_MASK   0x0Fu

/*----------------------------------------------------------------------------*/

#if (DEM_CFG_EVTCATEGORY == DEM_CFG_EVTCATEGORY_ON)
typedef struct
{
    Dem_EvtCategoryPrioType evtcatprio;
} Dem_EvtCategoryPriority;
#endif

/*----------------------------------------------------------------------------*/

#if(DEM_CFG_DEPENDENCY == DEM_CFG_DEPENDENCY_ON)
#define DEM_EVTS_INIT_SEQUENTIALFAILURBUFFER(X)    ,(X)
#else
#define DEM_EVTS_INIT_SEQUENTIALFAILURBUFFER(X)
#endif

#if (DEM_CFG_STORAGECONDITION == DEM_CFG_STORAGECONDITION_ON)
#define DEM_EVTS_INIT_STORAGECONDITION(X)          ,(X)
#else
#define DEM_EVTS_INIT_STORAGECONDITION(X)
#endif

#if (DEM_CFG_ENABLECONDITION == DEM_CFG_ENABLECONDITION_ON)
#define DEM_EVTS_INIT_ENABLECONDITION(X)           ,(X)
#else
#define DEM_EVTS_INIT_ENABLECONDITION(X)
#endif

#if (DEM_CFG_EVT_STATUS_CHANGE_NUM_CALLBACKS > 0)
#define DEM_EVTS_INIT_CALLBACKEVENTSTATUSCHANGED(X)                ,(X)
#else
#define DEM_EVTS_INIT_CALLBACKEVENTSTATUSCHANGED(X)
#endif

#if (DEM_CFG_EVT_CLEARALLOWEDCALLBACK != DEM_CFG_EVT_CLEARALLOWEDCALLBACK_OFF)
#define DEM_EVTS_INIT_CALLBACKCLEARALLOWED(X)                ,(X)
#else
#define DEM_EVTS_INIT_CALLBACKCLEARALLOWED(X)
#endif

#if (DEM_CFG_EVT_CLEARALLOWEDCALLBACK != DEM_CFG_EVT_CLEARALLOWEDCALLBACK_OFF)
#define DEM_EVTS_INIT_CLEARALLOWEDBEHAVIOR(X)                ,(X)
#else
#define DEM_EVTS_INIT_CLEARALLOWEDBEHAVIOR(X)
#endif






#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"

#if (DEM_CFG_EVTCATEGORY == DEM_CFG_EVTCATEGORY_ON)
/* MR12 RULE 10.4 VIOLATION: The array type and also the init values of Dem_EvtCategoryPrioType and DEM_EVENTCATPRIO_ARRAYLENGTH are unsigned. The MISRA warning is not justified.*/
DEM_ARRAY_DECLARE_CONST(Dem_EvtCategoryPriority,Dem_EvtCategoryPrio,DEM_EVENTCATPRIO_ARRAYLENGTH);
#endif

#if(DEM_CFG_CALLBACK_INIT_MON_FOR_EVENT_SUPPORTED == TRUE)
DEM_ARRAY_DECLARE_CONST(Dem_InitMonitorForE, Dem_Cfg_Callback_InitMForE_List, DEM_CFG_CALLBACK_INITMFORE_LISTLENGTH);
#endif

#if (DEM_CFG_EVT_ATTRIBUTE == DEM_CFG_EVT_ATTRIBUTE_ON)
DEM_ARRAY_DECLARE_CONST(Dem_EventAttributeType, Dem_AllEventsUserAttributes, DEM_EVENTID_ARRAYLENGTH);
#endif

#if (DEM_CFG_EVT_STATUS_CHANGE_NUM_CALLBACKS > 0)
DEM_ARRAY_DECLARE_CONST(Dem_EvtStatusChangeFncType, Dem_AllEventsStatusChangedFnc, DEM_CFG_EVT_STATUS_CHANGE_NUM_CALLBACKS);
#endif

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"


#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

extern uint32 Dem_EvtIsAnyInitMonitoringRequestedMask;
DEM_ARRAY_DECLARE(Dem_EvtState, Dem_AllEventsState, DEM_EVENTID_ARRAYLENGTH);
DEM_ARRAY_DECLARE(Dem_EvtState8, Dem_AllEventsState8, DEM_EVENTID_ARRAYLENGTH);
DEM_BITARRAY_DECLARE(Dem_AllEventsResetDebouncerRequested, DEM_EVENTID_ARRAYLENGTH);
DEM_BITARRAY_DECLARE(Dem_EventWasPassedReported,DEM_EVENTID_ARRAYLENGTH);

extern uint16 Dem_GlobalInitMonitoringCounter;

#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"



/*** FUNCTIONS ****************************************************************/
DEM_INLINE Dem_boolean_least Dem_EvtGetHistoryStatus(Dem_EventIdType EventId)
{
#if(DEM_CFG_ALLOW_HISTORY == DEM_CFG_ALLOW_HISTORY_ON)
    return Dem_BitArrayIsBitSet(Dem_AllEventsHistoryStatus, EventId);
#else
    DEM_UNUSED_PARAM(EventId);
    return FALSE;
#endif
}


DEM_INLINE void Dem_EvtSetHistoryStatus(Dem_EventIdType EventId, boolean newStatus)
{
#if(DEM_CFG_ALLOW_HISTORY == DEM_CFG_ALLOW_HISTORY_ON)
    Dem_BitArrayOverwriteBit(Dem_AllEventsHistoryStatus, EventId, newStatus);
    Dem_NvMWriteBlockOnShutdown(DEM_NVM_ID_HISTORY_STATUS_BYTE);
#else
    DEM_UNUSED_PARAM(EventId);
    DEM_UNUSED_PARAM(newStatus);
#endif
}





/*-- DEBOUNCE LEVEL ----------------------------------------------------------*/

DEM_INLINE sint16 Dem_EvtGetDebounceLevel (Dem_EventIdType EventId)
{
   return Dem_AllEventsState[EventId].debounceLevel;
}


DEM_INLINE void Dem_EvtSetDebounceLevel (Dem_EventIdType EventId, sint16 debounceLevel)
{
   Dem_AllEventsState[EventId].debounceLevel = debounceLevel;
}


DEM_INLINE void Dem_EvtIncDebounceLevel (Dem_EventIdType EventId)
{
   Dem_AllEventsState[EventId].debounceLevel++;
}

DEM_INLINE void Dem_EvtDecDebounceLevel (Dem_EventIdType EventId)
{
   Dem_AllEventsState[EventId].debounceLevel--;
}



/*-- LAST REPORTED EVENT -----------------------------------------------------*/

DEM_INLINE Dem_EventStatusType Dem_EvtGetLastReportedEvent (Dem_EventIdType EventId)
{
   return Dem_AllEventsState8[EventId].lastReportedEvent;
}


DEM_INLINE void Dem_EvtSetLastReportedEvent (Dem_EventIdType EventId, Dem_EventStatusType EventStatus)
{
   Dem_AllEventsState8[EventId].lastReportedEvent = EventStatus;
}




/*-- RECOVERABLE -------------------------------------------------------------*/

Dem_boolean_least Dem_EvtIsRecoverable (Dem_EventIdType EventId);

DEM_INLINE Dem_boolean_least Dem_EvtIsNotRecoverableTOC(Dem_EventIdType EventId)
{
    return (! Dem_EvtIsRecoverable(EventId));
}

/*-- CAUSAL ------------------------------------------------------------------*/

DEM_INLINE Dem_boolean_least Dem_EvtIsCausal (Dem_EventIdType EventId)
{
   return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_CAUSALFAULT);
}


DEM_INLINE void Dem_EvtSetCausal_Flag (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
    DEM_ASSERT_ISLOCKED();
   DEM_EVTSTATE_OVERWRITEBIT(&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_CAUSALFAULT, setBit);
}

DEM_INLINE Dem_boolean_least Dem_EvtIsRecheckedAndWaitingForMonResult (Dem_EventIdType EventId)
{
#if (DEM_CFG_DEPENDENCY == DEM_CFG_DEPENDENCY_ON)
    return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_IS_RECHECKEDANDWAITING_FOR_MONRES);
#else
    DEM_UNUSED_PARAM(EventId);
    return FALSE;
#endif
}


DEM_INLINE void Dem_EvtSetIsRecheckedAndWaitingForMonResult (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
#if (DEM_CFG_DEPENDENCY == DEM_CFG_DEPENDENCY_ON)
    DEM_ASSERT_ISLOCKED();
    DEM_EVTSTATE_OVERWRITEBIT(&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_IS_RECHECKEDANDWAITING_FOR_MONRES, setBit);
#else
    DEM_UNUSED_PARAM(EventId);
    DEM_UNUSED_PARAM(setBit);
#endif
}


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

boolean Dem_EvtClearEventAllowed ( Dem_EventIdType eventId );
void Dem_EvtSetCausal (Dem_EventIdType EventId, Dem_boolean_least causal);
Std_ReturnType Dem_EvtResetIsoByteCallback(void);
void Dem_EvtPreInitEvents (void);
void Dem_EvtInitEvents(void);

#if((DEM_CFG_SUPPRESSION == DEM_EVENT_SUPPRESSION) || (DEM_CFG_SUPPRESSION == DEM_EVENT_AND_DTC_SUPPRESSION))
void Dem_EvtSetSuppression (Dem_EventIdType EventId, Dem_boolean_least newSuppressed);
#endif

#if (DEM_CFG_CHECKAPICONSISTENCY == TRUE)
/*MR12 RULE 8.5 VIOLATION: Duplicate of Dem_GetEventTested to make it also available in the GeneralDiagnosticInfo RTE interface */
Std_ReturnType Dem_GetEventTested_GeneralEvtInfo(Dem_EventIdType EventId, boolean* EventTested);
#endif /* DEM_CFG_CHECKAPICONSISTENCY */

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#if(DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
DEM_INLINE void Dem_EvtHistoryStatusClearNVM (void)
{
    /* Set the EventStatus to its default value */
    DEM_MEMSET(&Dem_AllEventsHistoryStatus, 0x00, DEM_SIZEOF_VAR(Dem_AllEventsHistoryStatus));
    Dem_NvMClearBlockByInvalidate(DEM_NVM_ID_HISTORY_STATUS_BYTE);
}
#endif

#if(DEM_CFG_ALLOW_HISTORY == DEM_CFG_ALLOW_HISTORY_ON)
//Function to Validate the Nv Blocks after startup
DEM_INLINE void Dem_EvtHistoryStatusInitCheckNvM(void)
{
    Dem_NvmResultType NvmResult;


    /* Get the Result of the NvM-Read (NvM_ReadAll) */
    NvmResult = Dem_NvmGetStatus (DEM_NVM_ID_HISTORY_STATUS_BYTE);

    /* Data read successfully */
    if (NvmResult != DEM_NVM_SUCCESS)
    {
        /* Zero the content of event history status */
        DEM_MEMSET( &Dem_AllEventsHistoryStatus, 0x00, DEM_SIZEOF_VAR(Dem_AllEventsHistoryStatus));
        Dem_NvMWriteBlockOnShutdown(DEM_NVM_ID_HISTORY_STATUS_BYTE);
    }


}
#endif

/*-- SUSPICION ---------------------------------------------------------------*/

DEM_INLINE Dem_boolean_least Dem_EvtIsSuspicious (Dem_EventIdType EventId)
{
   return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_SUSPICIONLEVEL);
}


DEM_INLINE void Dem_EvtSetSuspicionLevel (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
   DEM_EVTSTATE_OVERWRITEBIT (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_SUSPICIONLEVEL, setBit);
}



/*-- ROBUSTNESS --------------------------------------------------------------*/
#if(DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD == DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD_ON)
DEM_INLINE Dem_boolean_least Dem_EvtGetFDCThresholdReachedTOC (Dem_EventIdType EventId)
{
  return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_FDCTHRESHOLDREACHEDTOC);
}

DEM_INLINE void Dem_EvtSetFDCThresholdReachedTOC (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
   DEM_EVTSTATE_OVERWRITEBIT (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_FDCTHRESHOLDREACHEDTOC, setBit);
}
#endif


#if(DEM_CFG_SUPPORT_EVENT_FDCTHRESHOLDREACHED)
DEM_INLINE Dem_boolean_least Dem_EvtGetFDCThresholdReached (Dem_EventIdType EventId)
{
  return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_FDCTHRESHOLDREACHED);
}
#endif

DEM_INLINE void Dem_EvtSetFDCThresholdReached (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
#if(DEM_CFG_SUPPORT_EVENT_FDCTHRESHOLDREACHED)
   DEM_EVTSTATE_OVERWRITEBIT (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_FDCTHRESHOLDREACHED, setBit);
#else
   DEM_UNUSED_PARAM(EventId);
   DEM_UNUSED_PARAM(setBit);
#endif
}



/*-- CATEGORY ----------------------------------------------------------------*/

#if (DEM_CFG_EVTCATEGORY == DEM_CFG_EVTCATEGORY_ON)

DEM_INLINE Dem_EvtCategoryPriorityType Dem_GetEventCategoryPriority (Dem_EventCategoryType Category)
{
    return (Dem_EvtCategoryPriorityType) Dem_EvtCategoryPrio[Category].evtcatprio;
}
#endif






/*-- STORAGECONDITION --------------------------------------------------------*/

DEM_INLINE Dem_boolean_least Dem_EvtIsStorageFiltered (Dem_EventIdType EventId)
{
#if (DEM_CFG_STORAGECONDITION == DEM_CFG_STORAGECONDITION_ON)
   return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_ISSTORAGEFILTERED);
#else
   DEM_UNUSED_PARAM(EventId);
   return FALSE;
#endif
}


DEM_INLINE void Dem_EvtSetStorageFiltered (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
#if (DEM_CFG_STORAGECONDITION == DEM_CFG_STORAGECONDITION_ON)
   DEM_EVTSTATE_OVERWRITEBIT (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_ISSTORAGEFILTERED, setBit);
#else
   DEM_UNUSED_PARAM(EventId);
   DEM_UNUSED_PARAM(setBit);
#endif
}
DEM_INLINE Dem_boolean_least Dem_EvtAllEnableConditionsFulfilled (Dem_EventIdType EventId)
{
   return (Dem_EnCoAreAllFulfilled(Dem_EvtParam_GetEnableConditions(EventId)));
}

/*-- RESET CDB ---------------------------------------------------------------*/

DEM_INLINE void Dem_EvtRequestResetFailureFilter(Dem_EventIdType EventId, Dem_boolean_least setBit)
{
    DEM_ASSERT_ISLOCKED();
    Dem_BitArrayOverwriteBit(Dem_AllEventsResetDebouncerRequested, EventId, setBit);
}

DEM_INLINE Dem_boolean_least Dem_EvtIsResetFailureFilterRequested(Dem_EventIdType EventId)
{
    return Dem_BitArrayIsBitSet(Dem_AllEventsResetDebouncerRequested, EventId);
}

/*-- Reset Debounce counter requested for Control DTC Setting-----------------*/

DEM_INLINE Dem_boolean_least Dem_EvtIsResetFailureFilterRequestedAfterDtcSetting (Dem_EventIdType EventId)
{


#if (!DEM_CFG_DTCSETTINGBLOCKSREPORTING)
    return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_DEBCOUNTER_RESET_REQUESTED);
#else
    DEM_UNUSED_PARAM(EventId);
    return FALSE;
#endif
}


DEM_INLINE void Dem_EvtRequestResetFailureFilterAfterDtcSetting (Dem_EventIdType EventId, Dem_boolean_least setBit)
{

    DEM_ASSERT_ISLOCKED();

#if (!DEM_CFG_DTCSETTINGBLOCKSREPORTING)
   DEM_EVTSTATE_OVERWRITEBIT (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_DEBCOUNTER_RESET_REQUESTED, setBit);
#else
   DEM_UNUSED_PARAM(EventId);
   DEM_UNUSED_PARAM(setBit);
#endif
}

/*-- Reset Debounce counter requested for Storage Condition-----------------*/

DEM_INLINE void Dem_EvtRequestResetFailureFilterAfterStorageCondition (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
	DEM_ASSERT_ISLOCKED();
#if (DEM_CFG_STORAGECONDITION == DEM_CFG_STORAGECONDITION_ON)
    DEM_EVTSTATE_OVERWRITEBIT (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_IS_STORAGEFILTERED_EVENT_REPORTED, setBit);
#else
    DEM_UNUSED_PARAM(EventId);
    DEM_UNUSED_PARAM(setBit);
#endif
}

DEM_INLINE Dem_boolean_least Dem_EvtIsResetFailureFilterRequestedAfterStorageCondition (Dem_EventIdType EventId)
{
#if (DEM_CFG_STORAGECONDITION == DEM_CFG_STORAGECONDITION_ON)
    return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_IS_STORAGEFILTERED_EVENT_REPORTED);
#else
    DEM_UNUSED_PARAM(EventId);
	return FALSE;
#endif
}

DEM_INLINE void Dem_EvtIncreaseGlobalInitMonitoringCounter(void)
{
    Dem_GlobalInitMonitoringCounter++;
}


DEM_INLINE void Dem_EvtSetAnyInitMonitoring (Dem_InitMonitorReasonType reason) { rba_DiagLib_Bit32SetBit(&Dem_EvtIsAnyInitMonitoringRequestedMask, reason); }
DEM_INLINE void Dem_EvtResetAnyInitMonitoring (void) { Dem_EvtIsAnyInitMonitoringRequestedMask = 0u; }
DEM_INLINE boolean Dem_EvtIsAnyInitMonitoringRequested (void) { return (Dem_EvtIsAnyInitMonitoringRequestedMask != 0u); }
DEM_INLINE boolean Dem_EvtIsAnyInitMonitoringRequestedClear (void) { return rba_DiagLib_Bit32IsBitSet(Dem_EvtIsAnyInitMonitoringRequestedMask, DEM_INIT_MONITOR_CLEAR); }



DEM_INLINE void Dem_EvtSetInitMonitoring (Dem_EventIdType EventId, Dem_InitMonitorReasonType newReason)
{
    Dem_InitMonitorReasonType oldReason = (Dem_InitMonitorReasonType)DEM_EVTSTATE_GETBITS (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_INITMONITORING, DEM_EVT_BP_STATE_INITMONITORING_SIZE);

    DEM_ASSERT_ISLOCKED();
    if ((oldReason == 0u) || (oldReason > newReason))
    {
        DEM_EVTSTATE_OVERWRITEBITS (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_INITMONITORING, DEM_EVT_BP_STATE_INITMONITORING_SIZE, newReason);
    }
    Dem_EvtIncreaseGlobalInitMonitoringCounter();
    Dem_EvtSetAnyInitMonitoring (newReason);
}


/* if return value is != 0 an InitMon is requested; return value does not match the AR values DEM_INIT_MONITOR_* */
DEM_INLINE uint8 Dem_EvtIsInitMonitoringRequested (Dem_EventIdType EventId)
{
  return ((uint8)(DEM_EVTSTATE_GETBITS (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_INITMONITORING, DEM_EVT_BP_STATE_INITMONITORING_SIZE )));
}

DEM_INLINE void Dem_EvtClearInitMonitoringRequests (Dem_EventIdType EventId)
{
  DEM_EVTSTATE_CLEARBITS (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_INITMONITORING, DEM_EVT_BP_STATE_INITMONITORING_SIZE);
}


#if((DEM_CFG_CALLBACK_INIT_MON_FOR_EVENT_SUPPORTED == TRUE)||(DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACK == TRUE))
DEM_INLINE void Dem_InitMonitorForEFnc (void)
{
    Dem_InitMonitorReasonType InitMonitorReason;
#if(DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACK== TRUE)
    Dem_EventIdIterator eventIt;
    Dem_EventIdType EventId;
#if(DEM_CFG_CALLBACK_INIT_MON_FOR_EVENT_SUPPORTED == TRUE)
    uint16_least IndexInitMonitor=0u;
#endif
#else
    uint16_least i;
#endif
        if(Dem_EvtIsAnyInitMonitoringRequested())
            {
                DEM_ENTERLOCK_MON();

                Dem_EvtResetAnyInitMonitoring();

                DEM_EXITLOCK_MON();
#if(DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACK== TRUE)
                for (Dem_EventIdIteratorNew(&eventIt); Dem_EventIdIteratorIsValid(&eventIt); Dem_EventIdIteratorNext(&eventIt))
                {
                	EventId = Dem_EventIdIteratorCurrent(&eventIt);
                    if(Dem_IsInitMonitorForEventRequested((Dem_EventIdType)EventId, &InitMonitorReason))
                    {
#if(DEM_CFG_CALLBACK_INIT_MON_FOR_EVENT_SUPPORTED == TRUE)
                         if(IndexInitMonitor< DEM_CFG_CALLBACK_INITMFORE_LISTLENGTH)
                         {
                           if(Dem_Cfg_Callback_InitMForE_List[IndexInitMonitor].Eventid == (Dem_EventIdType)EventId)
                           {
                              Dem_Cfg_Callback_InitMForE_List[IndexInitMonitor].InitMForEAllwdCBFnc(InitMonitorReason);
                              IndexInitMonitor++;
                           }
                         }
#endif

                        /*If GlobalInitMonitor Callback function is configured, then below callback has to be called to
                           to notify EventId and reason to Application */
                        (void) DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACKFNC ((Dem_EventIdType)EventId, InitMonitorReason);
                    }
                }
#else
                  for (i=0u; i<DEM_CFG_CALLBACK_INITMFORE_LISTLENGTH; i++)
                        {
                            if(Dem_IsInitMonitorForEventRequested(Dem_Cfg_Callback_InitMForE_List[i].Eventid, &InitMonitorReason))
                            {
                                Dem_Cfg_Callback_InitMForE_List[i].InitMForEAllwdCBFnc(InitMonitorReason);
                            }
                        }

#endif
            }
}
#endif


#if ( DEM_CFG_EVT_STATUS_CHANGE_NUM_CALLBACKS > 0 )
DEM_INLINE void Dem_CallEventStatusChangedCallBack (Dem_EventIdType EventId, Dem_UdsStatusByteType EventStatusOld, Dem_UdsStatusByteType EventStatusNew )
{
  if (Dem_EvtParam_GetCallbackEventStatusChangedIndex(EventId) > 0)
  {
    (*Dem_AllEventsStatusChangedFnc[Dem_EvtParam_GetCallbackEventStatusChangedIndex(EventId)])(EventId, EventStatusOld, EventStatusNew);
  }
  else
  {
    /* No Call-back has been configured for this Event */
  }
}
#endif

/*WIRStatus set by External Source*/

#if(DEM_CFG_EVT_INDICATOR != DEM_CFG_EVT_INDICATOR_PROJECTSPECIFIC)
DEM_INLINE void Dem_EvtSetWIRExtern(Dem_EventIdType EventId, Dem_boolean_least newState)
{
  DEM_EVTSTATE_OVERWRITEBIT (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_WIRSTATUS_EXTERNAL,newState);
}


DEM_INLINE Dem_boolean_least Dem_EvtIsWIRExternal(Dem_EventIdType EventId)
{
  return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_WIRSTATUS_EXTERNAL);
}
#else
#define Dem_EvtSetWIRExtern(X,Y)    do{}while(0);
#define Dem_EvtIsWIRExternal(X)     (FALSE)
#endif




/*-- OPERATION CYCLE ---------------------------------------------------------*/

DEM_INLINE Dem_boolean_least Dem_isEventAffectedByOperationCycleList (Dem_EventIdType EventId, Dem_OperationCycleList operationCycleList)
{
  return ((operationCycleList & (Dem_OperationCycleList)(1u<<Dem_EvtParam_GetOperationCycleID(EventId))) != 0u);
}

DEM_INLINE Dem_boolean_least Dem_isEventAffectedByAgingCycleList (Dem_EventIdType EventId, Dem_OperationCycleList agingCycleList)
{
  return ((agingCycleList & (Dem_OperationCycleList)(1u<<Dem_EvtParam_GetAgingCycleID(EventId))) != 0u);
}

DEM_INLINE Dem_boolean_least Dem_isEventAffectedByFailureCycleList (Dem_EventIdType EventId, Dem_OperationCycleList failureCycleList)
{
   return ((failureCycleList & (Dem_OperationCycleList)(1u<<Dem_EvtParam_GetFailureCycleID(EventId))) != 0u);
}


/*-- DISTURBANCE MEMORY ---------------------------------------------------------------*/
#if(DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)

#define DEM_DIST_MEM_IND_SET_BIT_MASK   (Dem_EvtStateType)(((Dem_EvtStateType)DEM_EVT_BP_STATE_DIST_MEM_IDX_MASK) << DEM_EVT_BP_STATE_DIST_MEM_IDX)
#define DEM_DIST_MEM_IND_CLEAR_BIT_MASK (Dem_EvtStateType)(~((Dem_EvtStateType)DEM_DIST_MEM_IND_SET_BIT_MASK))

DEM_INLINE Dem_boolean_least Dem_EvtIsEventStoredInDistMem (Dem_EventIdType EventId)
{
  return ((Dem_AllEventsState[EventId].state & DEM_DIST_MEM_IND_SET_BIT_MASK) != 0u);
}

DEM_INLINE void Dem_EvtSetEventsDistMemIndex (Dem_EventIdType EventId , uint8 idx)
{
	Dem_AllEventsState[EventId].state = ((Dem_AllEventsState[EventId].state & DEM_DIST_MEM_IND_CLEAR_BIT_MASK) |   \
			((Dem_EvtStateType)(((Dem_EvtStateType)(idx & DEM_EVT_BP_STATE_DIST_MEM_IDX_MASK)) << DEM_EVT_BP_STATE_DIST_MEM_IDX)));
}

DEM_INLINE uint8 Dem_EvtGetEventsDistMemIndex (Dem_EventIdType EventId)
{
	return ((uint8)((Dem_AllEventsState[EventId].state & DEM_DIST_MEM_IND_SET_BIT_MASK) >> DEM_EVT_BP_STATE_DIST_MEM_IDX));
}

#endif

/*------EventSuppression-------------------------------------------------------------------------------*/

DEM_INLINE Dem_boolean_least Dem_EvtIsSuppressed (Dem_EventIdType EventId)
{
  return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_NOTAVAILABLE);
}

/*------NextReportRelevantForMemories----------------------------------------------------------------------*/

DEM_INLINE Dem_boolean_least Dem_EvtIsNextReportRelevantForMemories (Dem_EventIdType EventId)
{
   return DEM_EVTSTATE_ISBITSET (Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_NXT_REPORT_IS_RELEVANT);
}


DEM_INLINE void Dem_EvtSetNextReportRelevantForMemories (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
   DEM_EVTSTATE_OVERWRITEBIT (&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_NXT_REPORT_IS_RELEVANT, setBit);
}


/*------ Failure Cycle Event Status Bits -----------------------------------------------------------------------------*/
#if DEM_CFG_OBD == DEM_CFG_OBD_ON
/*
 * Whether the event was reported as failed in this failure cycle, not respecting reports before startup of the ECU
 *
 * This status information is needed for OBD permanent memory handling and is stored only in RAM. If ever move the
 * TestFailedTFC state handling from EvMem to an "extended event status byte", then we can use that instead and also
 * simplify some code in OBD.
 */
DEM_INLINE Dem_boolean_least Dem_EvtGetTestFailedTFCSincePreinit(Dem_EventIdType EventId)
{
    return DEM_EVTSTATE_ISBITSET(Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_TFTFC_SINCE_PREINIT);
}

/*
 * Whether the event was reported as failed in this failure cycle, not respecting reports before startup of the ECU
 *
 * This status information is needed for OBD permanent memory handling and is stored only in RAM. If ever move the
 * TestFailedTFC state handling from EvMem to an "extended event status byte", then we can use that instead and also
 * simplify some code in OBD.
 */
DEM_INLINE void Dem_EvtSetTestFailedTFCSincePreinit(Dem_EventIdType EventId, Dem_boolean_least newState)
{
    DEM_ASSERT_ISLOCKED();
    DEM_EVTSTATE_OVERWRITEBIT(&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_TFTFC_SINCE_PREINIT, newState);
}

/*
 * Whether the event was tested in this failure cycle, not respecting reports before startup of the ECU
 *
 * This status information is needed for OBD permanent memory handling and is stored only in RAM. If ever move the
 * TestCompleteTFC state handling from EvMem to an "extended event status byte", then we can use that instead and also
 * simplify some code in OBD.
 */
DEM_INLINE Dem_boolean_least Dem_EvtGetTestCompleteTFCSincePreinit(Dem_EventIdType EventId)
{
    return DEM_EVTSTATE_ISBITSET(Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_TCTFC_SINCE_PREINIT);
}

/*
 * Whether the event was tested in this failure cycle, not respecting reports before startup of the ECU
 *
 * This status information is needed for OBD permanent memory handling and is stored only in RAM. If ever move the
 * TestCompleteTFC state handling from EvMem to an "extended event status byte", then we can use that instead and also
 * simplify some code in OBD.
 */
DEM_INLINE void Dem_EvtSetTestCompleteTFCSincePreinit(Dem_EventIdType EventId, Dem_boolean_least newState)
{
    DEM_ASSERT_ISLOCKED();
    DEM_EVTSTATE_OVERWRITEBIT(&Dem_AllEventsState[EventId].state, DEM_EVT_BP_STATE_TCTFC_SINCE_PREINIT, newState);
}
#endif

/*-- REPORTING STATE ---------------------------------------------------------*/

DEM_INLINE void Dem_EvtSetPassedWasReported (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
    DEM_ASSERT_ISLOCKED();
    Dem_BitArrayOverwriteBit (Dem_EventWasPassedReported, EventId, setBit);
}

DEM_INLINE Dem_boolean_least Dem_EvtWasPassedReported (DEM_BITARRAY_CONSTFUNCPARAM(buffer), Dem_EventIdType EventId)
{
    return Dem_BitArrayIsBitSet (buffer, EventId);
}

#endif

