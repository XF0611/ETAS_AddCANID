


#ifndef DEM_EVENTSTATUS_H
#define DEM_EVENTSTATUS_H

#include "Dem_ISO14229Byte.h"
#include "Dem_Array.h"
#include "Dem_Cfg_Events.h"
#include "Dem_Cfg_EventId.h"
#include "Dem_Cfg_OperationCycle.h"
#include "Dem_Cfg_EvMem.h"
#include "Dem_Cfg_Clear.h"
#include "Dem_Events.h"
#include "Dem_EventStatusNvData.h"
/* BSW-11069 */
#include "Dem_PendingBitHandling.h"
/* END BSW-11069 */

#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
DEM_ARRAY_DECLARE(      uint8, Dem_AllEventsStatusByteCust, DEM_EVENTID_ARRAYLENGTH);
#endif

#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"



#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

DEM_INLINE Dem_UdsStatusByteType Dem_EvtGetIsoByte (Dem_EventIdType EventId)
{
    return Dem_AllEventsStatusByte[EventId];
}
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
DEM_INLINE Dem_UdsStatusByteType Dem_EvtGetCustIsoByte (Dem_EventIdType EventId)
{
    return Dem_AllEventsStatusByteCust[EventId];
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
#include "Dem_CustomStatusByte.h"
#endif


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"



#define DEM_STATUSBYTE_EVENT_UNAVAILABLE                   (0x00)
#define DEM_STATUSBYTE_NEW_OPERATIONCYCLE_SETMASK          (DEM_ISO14229_BM_TESTNOTCOMPLETE_TOC)
#define DEM_STATUSBYTE_NEW_OPERATIONCYCLE_CLEARMASK        (DEM_ISO14229_BM_TESTFAILED_TOC)

#define DEM_INIT_ISOBITS_CLEARMASK									(DEM_ISO14229_BM_TESTFAILED)



void Dem_EventStatusInitCheckNvM(void);
void Dem_ClearEvent(Dem_EventIdType eventId, boolean ClearFully);
void Dem_EvtAdvanceOperationCycle(Dem_OperationCycleList operationCycleList);

#if (DEM_CFG_CHECKAPICONSISTENCY == TRUE)
/*MR12 RULE 8.5 VIOLATION: Duplicate of Dem_GetEventStatus to make it also available in the GeneralDiagnosticInfo RTE interface */
Std_ReturnType Dem_GetEventStatus_GeneralEvtInfo(Dem_EventIdType EventId, Dem_UdsStatusByteType* EventStatusExtended);
#endif /* DEM_CFG_CHECKAPICONSISTENCY */


DEM_INLINE void Dem_InitEventStatusTestFailed(void)
{
#if (!DEM_CFG_EVMEM_STORAGE_TESTFAILED_SUPPORTED)
	Dem_EventIdIterator eventIt;
	Dem_EventIdType eventId;
	Dem_UdsStatusByteType statusNew;

	for (Dem_EventIdIteratorNew(&eventIt); Dem_EventIdIteratorIsValid(&eventIt); Dem_EventIdIteratorNext(&eventIt))
	{
		eventId = Dem_EventIdIteratorCurrent(&eventIt);
		statusNew = Dem_EvtGetIsoByte(eventId);
		if (!Dem_EvtParam_GetStoreTestFailedToNextOC(eventId))
		{
			statusNew &= (Dem_UdsStatusByteType)(~DEM_INIT_ISOBITS_CLEARMASK);
		}
		Dem_AllEventsStatusByte[eventId] = statusNew;
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
		Dem_AllEventsStatusByteCust[eventId] = statusNew;
#endif
	}
#endif
}


DEM_INLINE Dem_UdsStatusByteType Dem_EvtGetIsoByte4DtcCalculation (Dem_EventIdType EventId)
{
#if !DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    return Dem_AllEventsStatusByte[EventId];
#else
    return Dem_AllEventsStatusByteCust[EventId];
#endif
}


/*
 * Called in Dem_EvtProcessPassedAndFailed() to check whether the reported status would change the event status information
 */
DEM_INLINE Dem_boolean_least Dem_EvtSt_IsUpdateNeeded(Dem_EventIdType EventId, Dem_boolean_least reportIsFailed)
{
    return (
           (Dem_ISO14229ByteIsTestFailed(Dem_EvtGetIsoByte(EventId)) != reportIsFailed)
        || (!Dem_ISO14229ByteIsTestCompleteTOC(Dem_EvtGetIsoByte(EventId)))

#if DEM_CFG_OBD == DEM_CFG_OBD_ON
        || (reportIsFailed && !Dem_EvtGetTestFailedTFCSincePreinit(EventId))
        || (!Dem_EvtGetTestCompleteTFCSincePreinit(EventId))
#endif

#if (DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE)
        || (Dem_ISO14229ByteIsTestFailed(Dem_EvtGetCustIsoByte(EventId)) != reportIsFailed)
        || (!Dem_ISO14229ByteIsTestCompleteTOC(Dem_EvtGetCustIsoByte(EventId)))
#endif
    );
}


DEM_INLINE void Dem_EvtSt_HandleInitialization (Dem_EventIdType EventId)
{
    Dem_AllEventsStatusByte[EventId] = DEM_ISO14229BYTE_INITVALUE;

    /* We don't need to reset Test*TFCSincePreinit here because it is already properly initialized by static
     * initialization with zeros on ECU startup */

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomInitialization(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}


DEM_INLINE void Dem_EvtSt_HandleFailed(Dem_EventIdType EventId)
{
    Dem_ISO14229ByteSetTestFailed (&(Dem_AllEventsStatusByte[EventId]), TRUE);
    Dem_ISO14229ByteSetTestFailedTOC (&(Dem_AllEventsStatusByte[EventId]), TRUE);
    Dem_ISO14229ByteSetTestFailedSLC (&(Dem_AllEventsStatusByte[EventId]), TRUE);
    Dem_ISO14229ByteSetTestCompleteTOC (&(Dem_AllEventsStatusByte[EventId]), TRUE);
    Dem_ISO14229ByteSetTestCompleteSLC (&(Dem_AllEventsStatusByte[EventId]), TRUE);

/* BSW-11069 */
/* [$DD_BSWCODE 27164] */
#if (DEM_CFG_SETTING_PENDINGDTC_INDEPENDENT_FROM_STOCO == TRUE)
    Dem_ISO14229ByteSetPendingDTC (&(Dem_AllEventsStatusByte[EventId]), TRUE);
#endif
/* END BSW-11069 */
    /* only valid with failurecycle=operationcycle
    */
#if DEM_CFG_OBD == DEM_CFG_OBD_ON
    Dem_EvtSetTestFailedTFCSincePreinit(EventId, TRUE);
    Dem_EvtSetTestCompleteTFCSincePreinit(EventId, TRUE);
#endif

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomFailed(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandlePassed(Dem_EventIdType EventId)
{
    Dem_ISO14229ByteSetTestFailed (&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetTestCompleteTOC (&(Dem_AllEventsStatusByte[EventId]), TRUE);
    Dem_ISO14229ByteSetTestCompleteSLC (&(Dem_AllEventsStatusByte[EventId]), TRUE);

#if DEM_CFG_OBD == DEM_CFG_OBD_ON
    Dem_EvtSetTestCompleteTFCSincePreinit(EventId, TRUE);
#endif

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomPassed(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleResetEventStatus(Dem_EventIdType EventId)
{
    Dem_ISO14229ByteSetTestFailed(&(Dem_AllEventsStatusByte[EventId]), FALSE);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomResetEventStatus(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleEvtAvailable(Dem_EventIdType EventId)
{
    Dem_AllEventsStatusByte[EventId] = DEM_ISO14229BYTE_INITVALUE;
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomEvtAvailable(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleEvtNotAvailable(Dem_EventIdType EventId)
{
    Dem_AllEventsStatusByte[EventId] = DEM_STATUSBYTE_EVENT_UNAVAILABLE;
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomEvtNotAvailable(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleClear(Dem_EventIdType EventId)
{
#if DEM_CFG_CLEARDTCCLEARSALLBITS
    Dem_AllEventsStatusByte[EventId] = DEM_ISO14229BYTE_INITVALUE;
#else
    Dem_ISO14229ByteSetTestCompleteTOC (&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetTestCompleteSLC (&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetTestFailedTOC (&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetTestFailedSLC (&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetPendingDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetConfirmedDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);
    if(!(Dem_ISO14229ByteIsTestFailed(Dem_AllEventsStatusByte[EventId])))
    {
        Dem_ISO14229ByteSetWarningIndicatorRequested(&(Dem_AllEventsStatusByte[EventId]), FALSE);
    }
#endif

#if DEM_CFG_OBD == DEM_CFG_OBD_ON
    Dem_EvtSetTestFailedTFCSincePreinit(EventId, FALSE);
    Dem_EvtSetTestCompleteTFCSincePreinit(EventId, FALSE);
#endif

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomClear(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleClear_OnlyThisCycleAndReadiness(Dem_EventIdType EventId)
{
    Dem_ISO14229ByteSetTestFailedTOC (&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetTestFailedSLC (&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetTestCompleteTOC (&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetTestCompleteSLC (&(Dem_AllEventsStatusByte[EventId]), FALSE);

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomClear_OnlyThisCycleAndReadiness(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif

    /* Hint : As Events mapped to OBD DTCs are not allowed to configure ClearAllowedBehavior as ONLY_THIS_CYCLE_AND_READINESS,
     * no additional functions are called when DEM_CFG_OBD == DEM_CFG_OBD_ON */
}

DEM_INLINE void Dem_EvtSt_HandleClear_ClearTestInformation(Dem_EventIdType EventId)
{
    Dem_ISO14229ByteSetTestFailedTOC (&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetTestCompleteTOC (&(Dem_AllEventsStatusByte[EventId]), FALSE);
}

DEM_INLINE void Dem_EvtSt_HandleNewOperationCycle(Dem_EventIdType EventId)
{
    if(!Dem_EvtIsSuppressed(EventId))
    {
        /* only valid with failurecycle=operationcycle
         */
        Dem_AllEventsStatusByte[EventId] &= (Dem_UdsStatusByteType)(~DEM_STATUSBYTE_NEW_OPERATIONCYCLE_CLEARMASK);
        Dem_AllEventsStatusByte[EventId] |= DEM_STATUSBYTE_NEW_OPERATIONCYCLE_SETMASK;
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
        Dem_EvtSt_CustomNewOperationCycle(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
    }
}

DEM_INLINE void Dem_EvtSt_HandleNewFailureCycle(Dem_EventIdType EventId)
{
#if DEM_CFG_OBD == DEM_CFG_OBD_ON
    Dem_EvtSetTestFailedTFCSincePreinit(EventId, FALSE);
    Dem_EvtSetTestCompleteTFCSincePreinit(EventId, FALSE);
#else
    DEM_UNUSED_PARAM(EventId);
#endif
}

DEM_INLINE void Dem_EvtSt_HandleIndicatorOn(Dem_EventIdType EventId)
{
    Dem_ISO14229ByteSetWarningIndicatorRequested(&(Dem_AllEventsStatusByte[EventId]), TRUE);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomIndicatorOn(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleIndicatorOff(Dem_EventIdType EventId)
{
    Dem_ISO14229ByteSetWarningIndicatorRequested(&(Dem_AllEventsStatusByte[EventId]), FALSE);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomIndicatorOff(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleDTCSettingOn(Dem_EventIdType EventId)
{
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomDTCSettingOn(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#else
    DEM_UNUSED_PARAM(EventId);
#endif
}

DEM_INLINE void Dem_EvtSt_HandleConfirmation(Dem_EventIdType EventId)
{
/* BSW-11069 */ /* [$DD_BSWCODE 27165] */
#if (DEM_CFG_SETTING_PENDINGDTC_INDEPENDENT_FROM_STOCO == TRUE)
    /* Do nothing, PendingDTC bit is set by Dem_EvtSt_HandleFailed */
#else
/* END BSW-11069 */
    Dem_ISO14229ByteSetPendingDTC(&(Dem_AllEventsStatusByte[EventId]), TRUE);
/* BSW-11069 */
#endif
/* END BSW-11069 */
    Dem_ISO14229ByteSetConfirmedDTC(&(Dem_AllEventsStatusByte[EventId]), TRUE);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomConfirmation(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleAging(Dem_EventIdType EventId)
{
    if ( Dem_LibGetParamBool(DEM_CFG_TFSLC_RESET_AFTER_AGING_AND_DISPLACEMENT) )
    {
        Dem_ISO14229ByteSetTestFailedSLC(&(Dem_AllEventsStatusByte[EventId]), FALSE);
    }
    Dem_ISO14229ByteSetConfirmedDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomAging(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleAgingOfConfirmed(Dem_EventIdType EventId)
{

    Dem_ISO14229ByteSetConfirmedDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomAging(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

#if (DEM_CFG_PRJSPECIFICEVENTSTATUSHANDLING == FALSE)
DEM_INLINE void Dem_EvtSt_HandleImmediateAging(Dem_EventIdType EventId)
{
    if ( Dem_LibGetParamBool(DEM_CFG_TFSLC_RESET_AFTER_AGING_AND_DISPLACEMENT) )
    {
        Dem_ISO14229ByteSetTestFailedSLC(&(Dem_AllEventsStatusByte[EventId]), FALSE);
    }
    Dem_ISO14229ByteSetConfirmedDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);
    Dem_ISO14229ByteSetPendingDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomSetImmediateAging(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}
#else
#include "Dem_PrjEventStatus.h"
#endif

DEM_INLINE void Dem_EvtSt_HandleDisplacement(Dem_EventIdType EventId)
{
    if ( Dem_LibGetParamBool(DEM_CFG_EVMEM_RESET_CONFIRMED_BIT_ON_OVERFLOW) )
    {
        if ( Dem_LibGetParamBool(DEM_CFG_TFSLC_RESET_AFTER_AGING_AND_DISPLACEMENT))
        {
            Dem_ISO14229ByteSetTestFailedSLC(&(Dem_AllEventsStatusByte[EventId]), FALSE);
        }
        Dem_ISO14229ByteSetConfirmedDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);
    }
    Dem_ISO14229ByteSetPendingDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomDisplacement(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}

DEM_INLINE void Dem_EvtSt_HandleEvCombinationReplacement(Dem_EventIdType EventId)
{
    /* this function is used for event combination "on storage" for the event of a dtc, which is replaced by another event of the same dtc */
    if ( Dem_LibGetParamBool(DEM_CFG_TFSLC_RESET_AFTER_AGING_AND_DISPLACEMENT) )
    {
        Dem_ISO14229ByteSetTestFailedSLC(&(Dem_AllEventsStatusByte[EventId]), FALSE);
    }
    Dem_ISO14229ByteSetConfirmedDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);

    Dem_ISO14229ByteSetPendingDTC(&(Dem_AllEventsStatusByte[EventId]), FALSE);

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomEvCombinationReplacement(EventId, &(Dem_AllEventsStatusByteCust[EventId]));
#endif
}


DEM_INLINE void Dem_EvtSt_HandlePendingDTC(Dem_EventIdType EventId, Dem_boolean_least setBit)
{
    Dem_ISO14229ByteSetPendingDTC(&(Dem_AllEventsStatusByte[EventId]), setBit);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_EvtSt_CustomSetPending(EventId, &(Dem_AllEventsStatusByteCust[EventId]), setBit);
#endif
}



/****** ATTENTION: May only be used by consistency checks; otherwise use the HandleXXX functions! ******/
DEM_INLINE Dem_boolean_least Dem_EvtSt_GetTestFailed(Dem_EventIdType EventId)
{
    return Dem_ISO14229ByteIsTestFailed(Dem_AllEventsStatusByte[EventId]);
}
DEM_INLINE Dem_boolean_least Dem_EvtSt_GetTestFailedTOC(Dem_EventIdType EventId)
{
    return Dem_ISO14229ByteIsTestFailedTOC(Dem_AllEventsStatusByte[EventId]);
}
DEM_INLINE Dem_boolean_least Dem_EvtSt_GetTestCompleteTOC (Dem_EventIdType EventId)
{
    return Dem_ISO14229ByteIsTestCompleteTOC(Dem_AllEventsStatusByte[EventId]);
}

DEM_INLINE Dem_boolean_least Dem_EvtSt_GetPending(Dem_EventIdType EventId)
{
    return Dem_ISO14229ByteIsPendingDTC(Dem_AllEventsStatusByte[EventId]);
}

/* This function is used in Project Specific Implementation of Indicator, so this function should not be removed. */
DEM_INLINE Dem_boolean_least Dem_EvtSt_GetWIR (Dem_EventIdType EventId)
{
    return Dem_ISO14229ByteIsWarningIndicatorRequested(Dem_AllEventsStatusByte[EventId]);
}


DEM_INLINE void Dem_EvtSt_SetTestFailed(Dem_EventIdType EventId, Dem_boolean_least setBit)
{
    Dem_ISO14229ByteSetTestFailed(&(Dem_AllEventsStatusByte[EventId]), setBit);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_ISO14229ByteSetTestFailed(&(Dem_AllEventsStatusByteCust[EventId]), setBit);
#endif
}
DEM_INLINE void Dem_EvtSt_SetTestFailedSLC(Dem_EventIdType EventId, Dem_boolean_least setBit)
{
    Dem_ISO14229ByteSetTestFailedSLC(&(Dem_AllEventsStatusByte[EventId]), setBit);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_ISO14229ByteSetTestFailedSLC(&(Dem_AllEventsStatusByteCust[EventId]), setBit);
#endif
}
DEM_INLINE void Dem_EvtSt_SetTestCompleteSLC (Dem_EventIdType EventId, Dem_boolean_least setBit)
{
    Dem_ISO14229ByteSetTestCompleteSLC(&(Dem_AllEventsStatusByte[EventId]), setBit);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_ISO14229ByteSetTestCompleteSLC(&(Dem_AllEventsStatusByteCust[EventId]), setBit);
#endif
}
DEM_INLINE void Dem_EvtSt_SetConfirmedDTC(Dem_EventIdType EventId, Dem_boolean_least setBit)
{
    Dem_ISO14229ByteSetConfirmedDTC(&(Dem_AllEventsStatusByte[EventId]), setBit);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_ISO14229ByteSetConfirmedDTC(&(Dem_AllEventsStatusByteCust[EventId]), setBit);
#endif
}
DEM_INLINE void Dem_EvtSt_SetPendingDTC(Dem_EventIdType EventId, Dem_boolean_least setBit)
{
    Dem_ISO14229ByteSetPendingDTC(&(Dem_AllEventsStatusByte[EventId]), setBit);
#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
    Dem_ISO14229ByteSetPendingDTC(&(Dem_AllEventsStatusByteCust[EventId]), setBit);
#endif
}

#if(DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
DEM_INLINE void Dem_EvtStClearNvm(void)
{
    /* Set the EventStatus to its default value */
    DEM_MEMSET(&Dem_AllEventsStatusByte, (sint32)DEM_ISO14229BYTE_INITVALUE, DEM_SIZEOF_VAR(Dem_AllEventsStatusByte));
    Dem_NvMClearBlockByInvalidate(DEM_NVM_ID_EVT_STATUSBYTE);
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

