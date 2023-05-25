

#include "Dem_Internal.h"
#include "Rte_Dem.h"

#include "Dem_EventStatus.h"

#include "Dem_Events.h"
#include "Dem_Cfg_Main.h"
#include "Dem_Cfg_EvBuff.h"
#include "Dem_Dependencies.h"
#include "Dem_Nvm.h"
#include "Dem_EventFHandling.h"
#if(DEM_CFG_TRIGGERFIMREPORTS == DEM_CFG_TRIGGERFIMREPORTS_ON)
#include "FiM.h"
#endif
#include "Dem_Prv_CallEvtStChngdCbk.h"
#include "Dem_Obd.h"
#include "Dem_Prv_CallDtcStChngdCbk.h"

#define DEM_START_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"

DEM_ARRAY_DEFINE(      uint8, Dem_AllEventsStatusByte, DEM_EVENTID_ARRAYLENGTH);

#define DEM_STOP_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
DEM_ARRAY_DEFINE(      uint8, Dem_AllEventsStatusByteCust, DEM_EVENTID_ARRAYLENGTH);
#endif

#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"


/* Called from Dem_Init to validate the Nv block */
void Dem_EventStatusInitCheckNvM(void)
{
    Dem_NvmResultType NvmResult;

    /* Get the Result of the NvM-Read (NvM_ReadAll) */
    NvmResult = Dem_NvmGetStatus (DEM_NVM_ID_EVT_STATUSBYTE);

    /* Data read successfully */
    if (NvmResult != DEM_NVM_SUCCESS)
    {
        /* Set the EventStatus to its default value */
        DEM_MEMSET( &Dem_AllEventsStatusByte, (sint32)DEM_ISO14229BYTE_INITVALUE, DEM_SIZEOF_VAR(Dem_AllEventsStatusByte));

        //Set the Dirty flag
        Dem_NvMWriteBlockImmediate(DEM_NVM_ID_EVT_STATUSBYTE);
    }
}


Std_ReturnType Dem_GetEventStatus(Dem_EventIdType EventId,
        Dem_UdsStatusByteType* EventStatusExtended)
{
    DEM_ENTRY_CONDITION_CHECK_DEMINIT_OR_FIM_IS_IN_INIT_OR_OPMO_ALLFAILUREINFOLOCKED_EVTIDVALID_EVTAVAILABLE(EventId, DEM_DET_APIID_DEM_GETEVENTSTATUS , E_NOT_OK);
	DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR_WITHEVENTID(EventId,EventStatusExtended,DEM_DET_APIID_DEM_GETEVENTSTATUS,E_NOT_OK);

    *EventStatusExtended = Dem_EvtGetIsoByte(EventId);
    return E_OK;
}

#ifdef RTE_TYPE_H
Std_ReturnType Dem_GetEventStatus_GeneralEvtInfo(Dem_EventIdType EventId,
        Dem_UdsStatusByteType* EventStatusExtended)
{
    return Dem_GetEventStatus(EventId, EventStatusExtended);
}
#endif /* RTE_TYPE_H */

/* Function to query the Last Reported Event Status */

Dem_EventStatusType Dem_EvtGetLastReportedEventStatus (Dem_EventIdType EventId)
{
    return Dem_EvtGetLastReportedEvent(EventId);
}

void Dem_ClearEvent(Dem_EventIdType eventId, boolean ClearFully)
{
    Dem_UdsStatusByteType statusOld, statusNew;
    Dem_UdsStatusByteType dtcStByteOld;

    statusOld = DEM_ISO14229BYTE_INITVALUE;
    statusNew = DEM_ISO14229BYTE_INITVALUE;
    dtcStByteOld = DEM_ISO14229BYTE_INITVALUE;

    if (!Dem_EvtIsSuppressed(eventId))
    {
        DEM_ENTERLOCK_MON();
        Dem_StatusChange_GetOldStatus(eventId, &statusOld, &dtcStByteOld);
        if (ClearFully)
        {
            Dem_EvtSt_HandleClear(eventId);

            statusNew = Dem_EvtGetIsoByte(eventId);

            Dem_EvtSetCausal(eventId, FALSE);
            Dem_EvtSetInitMonitoring(eventId, DEM_INIT_MONITOR_CLEAR);
            Dem_EvtSetLastReportedEvent(eventId, DEM_EVENT_STATUS_INVALIDREPORT);
            Dem_EvtRequestResetFailureFilter(eventId, TRUE);

#if (DEM_CFG_DEBUGDATA_FORTIMEBASEDDEBOUNCING == DEM_CFG_DEBUGDATA_FORTIMEBASEDDEBOUNCING_ON)
            if(Dem_EvtParam_GetDebounceMethodIndex (eventId) == DEM_DEBMETH_IDX_ARTIME)
            {
                Dem_DebArTimeDebugValues[Dem_EvtParam_GetDebounceParamSettingIndex(eventId)][0] = 0;
                Dem_DebArTimeDebugValues[Dem_EvtParam_GetDebounceParamSettingIndex(eventId)][1] = 0;
            }
#endif

            /*Reset FDC-Threshold_reached-flags whenever the event is cleared from the event memory*/
           #if(DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD == DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD_ON)
               Dem_EvtSetFDCThresholdReachedTOC(eventId,FALSE);
           #endif
           #if(DEM_CFG_SUPPORT_EVENT_FDCTHRESHOLDREACHED)
               Dem_EvtSetFDCThresholdReached(eventId,FALSE);
           #endif

            if (statusNew != statusOld)
            {
                Dem_ClearIndicatorAttributes(eventId,statusOld,statusNew);
                Dem_NodeSetRecheckOnClear(Dem_NodeIdFromEventId(eventId), TRUE);
            }
        }
        else
        {
            Dem_EvtSt_HandleClear_OnlyThisCycleAndReadiness(eventId);
            statusNew = Dem_EvtGetIsoByte(eventId);
        }
        DEM_EXITLOCK_MON();
        Dem_TriggerOn_EventStatusChange(eventId,statusOld,statusNew,dtcStByteOld);
    }
}

#if ( DEM_CFG_DTC_STATUSCHANGEDCALLBACK == DEM_CFG_DTC_STATUSCHANGEDCALLBACK_ON )
DEM_INLINE void Dem_InitializeDTCStatusAndUpdateInfo(Dem_DTCStatusAndUpdateInfoType* Dem_DTCStatusAndUpdateInfo)
{
    Dem_DtcIdIterator dtcIt;
    Dem_DtcIdType DtcId;

    for(Dem_DtcIdIteratorNew(&dtcIt); Dem_DtcIdIteratorIsValid(&dtcIt); Dem_DtcIdIteratorNext(&dtcIt))
    {
        DtcId = Dem_DtcIdIteratorCurrent(&dtcIt);
        Dem_DTCStatusAndUpdateInfo[DtcId].isStatusChangeToBeCalculated = FALSE;	/* Initialzing for all DTC Ids done intentionally */
        if(Dem_DtcUsesOrigin(DtcId, DEM_DTC_ORIGIN_PRIMARY_MEMORY))
        {
        	Dem_DTCStatusAndUpdateInfo[DtcId].status = Dem_DtcStatusByteRetrieve (DtcId) & DEM_CFG_DTCSTATUS_AVAILABILITYMASK;
		}
    }
}
#endif

void Dem_EvtAdvanceOperationCycle(Dem_OperationCycleList operationCycleList)
{
    Dem_EventIdIterator eventIt;
    Dem_EventIdType eventId;
    Dem_UdsStatusByteType statusNew,statusOld;
    Dem_EventIdType CBeventId[DEM_CFG_ADVANCEOPERATIONCYCLE_EVENTSPERLOCK] = {0};
    Dem_UdsStatusByteType CBStatusOld[DEM_CFG_ADVANCEOPERATIONCYCLE_EVENTSPERLOCK] = {0};
    Dem_UdsStatusByteType CBStatusNew[DEM_CFG_ADVANCEOPERATIONCYCLE_EVENTSPERLOCK] = {0};
    uint32 CBindex=0;
    uint32 i;
    uint32 eventsProcessed = 0;
    Dem_DTCStatusAndUpdateInfoType Dem_DTCStatusAndUpdateInfo[DEM_DTCID_ARRAYLENGTH];

#if ( DEM_CFG_DTC_STATUSCHANGEDCALLBACK == DEM_CFG_DTC_STATUSCHANGEDCALLBACK_ON )
    Dem_InitializeDTCStatusAndUpdateInfo(Dem_DTCStatusAndUpdateInfo);
#endif
    DEM_ENTERLOCK_MON();
    for (Dem_EventIdIteratorNew(&eventIt); Dem_EventIdIteratorIsValid(&eventIt); Dem_EventIdIteratorNext(&eventIt))
    {
        eventsProcessed++;
        eventId = Dem_EventIdIteratorCurrent(&eventIt);
        if (Dem_isEventAffectedByOperationCycleList(eventId, operationCycleList))
        {
            /* Set iso status-byte to next operation cycle */
            statusOld = Dem_EvtGetIsoByte(eventId);
            Dem_EvtSt_HandleNewOperationCycle(eventId);

            Dem_SetIndicatorDeActivation_OnOperationCycleChange(eventId, statusOld, Dem_EvtGetIsoByte(eventId));

            /* Updated Status */
            statusNew = Dem_EvtGetIsoByte(eventId);

            //Reset FDC-Threshold_reached-flag whenever the operation cycle starts/restarts
#if(DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD == DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD_ON)
            Dem_EvtSetFDCThresholdReachedTOC(eventId,FALSE);
#endif
            Dem_EvtSetInitMonitoring(eventId, DEM_INIT_MONITOR_RESTART);
            Dem_EvtRequestResetFailureFilter(eventId, TRUE);
            Dem_EvtSetLastReportedEvent(eventId,DEM_EVENT_STATUS_INVALIDREPORT);

            CBeventId[CBindex]=eventId;
            CBStatusOld[CBindex]=statusOld;
            CBStatusNew[CBindex]=statusNew;
            CBindex++;

        }

        if (Dem_isEventAffectedByFailureCycleList(eventId, operationCycleList))
        {
            Dem_EvtSt_HandleNewFailureCycle(eventId);
        }

        if (eventsProcessed >= DEM_CFG_ADVANCEOPERATIONCYCLE_EVENTSPERLOCK)
        {
            eventsProcessed=0;
            DEM_EXITLOCK_MON();
            for(i=0; i<CBindex; i++)
            {
                Dem_TriggerOn_MultipleEventStatusChange(CBeventId[i],CBStatusOld[i],CBStatusNew[i], Dem_DTCStatusAndUpdateInfo);
            }
            CBindex=0;
            DEM_ENTERLOCK_MON();
        }
    }
    DEM_EXITLOCK_MON();
    for(i=0; i<CBindex; i++)
    {
        Dem_TriggerOn_MultipleEventStatusChange(CBeventId[i],CBStatusOld[i],CBStatusNew[i], Dem_DTCStatusAndUpdateInfo);
    }
#if ( DEM_CFG_DTC_STATUSCHANGEDCALLBACK == DEM_CFG_DTC_STATUSCHANGEDCALLBACK_ON )
    Dem_TriggerOn_MultipleDTCStatusChange(Dem_DTCStatusAndUpdateInfo);
#endif
}



Std_ReturnType Dem_OverwriteWIRStatus( Dem_EventIdType EventId, boolean WIRStatus )
{
    Std_ReturnType ret_val = E_NOT_OK;

    if( Dem_isEventIdValid(EventId) )
    {
        DEM_ENTERLOCK_MON();

        if (WIRStatus)
        {
            Dem_EvtSt_HandleIndicatorOn(EventId);
        }
        else
        {
            Dem_EvtSt_HandleIndicatorOff(EventId);
        }

        DEM_EXITLOCK_MON();
        ret_val = E_OK;
    }

    return ret_val;
}


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
