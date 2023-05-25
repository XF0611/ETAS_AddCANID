/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#ifndef DEM_CUSTOMSTATUSBYTE_H
#define DEM_CUSTOMSTATUSBYTE_H

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom status byte
 *
 * If the configuration option DemRbCustomizableDTCStatusByte is set, then these hooks will be called after the
 * internal status byte has been updated. The hooks may then adjust the custom status byte (which is a separate byte)
 * according to the project needs. When the status of an event is retrieved, the custom status byte is returned instead
 * of the internal one.
 */

#include "Dem_Events.h"
#include "Dem_Dtcs.h"
#include "Dem_ISO14229Byte.h"


#if (DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE)


#define DEM_CUSTBYTE_EVENT_UNAVAILABLE                   (0x00u)
#define DEM_CUSTBYTE_NEW_OPERATIONCYCLE_SETMASK          (DEM_ISO14229_BM_TESTNOTCOMPLETE_TOC)
#define DEM_CUSTBYTE_NEW_OPERATIONCYCLE_CLEARMASK        (DEM_ISO14229_BM_TESTFAILED_TOC)

/**
 * \brief Initialize the custom status byte. Called on Dem_PreInit or if the NvM block could not be read.
 */
DEM_INLINE void Dem_EvtSt_CustomInitialization (Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    DEM_UNUSED_PARAM(EventId);
    *statusByte = (Dem_UdsStatusByteType)DEM_ISO14229BYTE_INITVALUE;
}

/**
 * \brief Called when the event is reported as failed
 */
DEM_INLINE void Dem_EvtSt_CustomFailed(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    if (Dem_IsEventEnabledByDtcSetting(EventId))
    {
        Dem_ISO14229ByteSetTestFailed (statusByte, TRUE);
        Dem_ISO14229ByteSetTestFailedTOC (statusByte, TRUE);
        Dem_ISO14229ByteSetTestFailedSLC (statusByte, TRUE);
        Dem_ISO14229ByteSetTestCompleteTOC (statusByte, TRUE);
        Dem_ISO14229ByteSetTestCompleteSLC (statusByte, TRUE);
        Dem_ISO14229ByteSetPendingDTC (statusByte, TRUE);
    }
}

/**
 * \brief Called when the event is reported as passed
 */
DEM_INLINE void Dem_EvtSt_CustomPassed(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    if (Dem_IsEventEnabledByDtcSetting(EventId))
    {
        Dem_ISO14229ByteSetTestFailed (statusByte, FALSE);
        Dem_ISO14229ByteSetTestCompleteTOC (statusByte, TRUE);
        Dem_ISO14229ByteSetTestCompleteSLC (statusByte, TRUE);
    }
}

/**
 * \brief Called when Dem_ResetEventStatus() is called
 */
DEM_INLINE void Dem_EvtSt_CustomResetEventStatus(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    if (Dem_IsEventEnabledByDtcSetting(EventId))
    {
        Dem_ISO14229ByteSetTestFailed(statusByte, FALSE);
    }
}

/**
 * \brief Called when the event gets unsuppressed
 */
DEM_INLINE void Dem_EvtSt_CustomEvtAvailable(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    DEM_UNUSED_PARAM(EventId);
    *statusByte = (Dem_UdsStatusByteType)DEM_ISO14229BYTE_INITVALUE;
}

/**
 * \brief Called when the event gets suppressed
 */
DEM_INLINE void Dem_EvtSt_CustomEvtNotAvailable(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    DEM_UNUSED_PARAM(EventId);
    *statusByte = DEM_CUSTBYTE_EVENT_UNAVAILABLE;
}

/**
 * \brief Called when the event gets cleared
 */
DEM_INLINE void Dem_EvtSt_CustomClear(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    *statusByte = (Dem_UdsStatusByteType)DEM_ISO14229BYTE_INITVALUE;
    if (!Dem_IsEventEnabledByDtcSetting(EventId))
    {
        Dem_ISO14229ByteSetWarningIndicatorRequested(statusByte,Dem_ISO14229ByteIsWarningIndicatorRequested(Dem_EvtGetIsoByte(EventId)));
    }
}

/**
 * \brief Called when the ClearAllowedBehavior is configured as ONLY_THIS_CYCLE_AND_READINESS and the event gets cleared
 */
DEM_INLINE void Dem_EvtSt_CustomClear_OnlyThisCycleAndReadiness(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
/* Custom implementation can be included here */
}

/**
 * \brief Called when the operation cycle of the event starts
 */
DEM_INLINE void Dem_EvtSt_CustomNewOperationCycle(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    if (Dem_IsEventEnabledByDtcSetting(EventId))
    {
        if (Dem_ISO14229ByteIsTestCompleteTOC(*statusByte) && !Dem_ISO14229ByteIsTestFailedTOC(*statusByte))
        {
            Dem_ISO14229ByteSetPendingDTC(statusByte, FALSE);
        }

        Dem_ISO14229ByteSetTestFailedTOC (statusByte, FALSE);
        Dem_ISO14229ByteSetTestCompleteTOC (statusByte, FALSE);
    }
}

/**
 * \brief Called when a warning indicator for the event gets activated
 */
DEM_INLINE void Dem_EvtSt_CustomIndicatorOn(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    if (Dem_IsEventEnabledByDtcSetting(EventId))
    {
        Dem_ISO14229ByteSetWarningIndicatorRequested(statusByte, TRUE);
    }
}

/**
 * \brief Called when a warning indicator for the event gets deactivated
 */
DEM_INLINE void Dem_EvtSt_CustomIndicatorOff(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    if (Dem_IsEventEnabledByDtcSetting(EventId))
    {
        Dem_ISO14229ByteSetWarningIndicatorRequested(statusByte, FALSE);
    }
}

/**
 * \brief Called when the DTC group of the event is enabled via Dem_EnableDTCSetting()
 */
DEM_INLINE void Dem_EvtSt_CustomDTCSettingOn(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    Dem_ISO14229ByteSetWarningIndicatorRequested(statusByte,Dem_ISO14229ByteIsWarningIndicatorRequested(Dem_EvtGetIsoByte(EventId)));
}

/**
 * \brief Called when the event gets confirmed
 */
DEM_INLINE void Dem_EvtSt_CustomConfirmation(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    if (Dem_IsEventEnabledByDtcSetting(EventId))
    {
        Dem_ISO14229ByteSetConfirmedDTC(statusByte, TRUE);
    }
}

/**
 * \brief Called when the event gets aged
 */
DEM_INLINE void Dem_EvtSt_CustomAging(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    DEM_UNUSED_PARAM(EventId);
    Dem_ISO14229ByteSetConfirmedDTC(statusByte, FALSE);
    //Dem_ISO14229ByteSetPendingDTC(statusByte, FALSE);
}

/**
 * \brief Called when the event is aged immediately (i.e. aging threshold == 0)
 */
DEM_INLINE void Dem_EvtSt_CustomSetImmediateAging(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    DEM_UNUSED_PARAM(EventId);
    Dem_ISO14229ByteSetConfirmedDTC(statusByte, FALSE);
    //Dem_ISO14229ByteSetPendingDTC(statusByte, FALSE);
}

/**
 * \brief Called when the event gets displaced
 */
/* MR12 RULE 8.13 VIOLATION: changing value might be needed in customer project */
DEM_INLINE void Dem_EvtSt_CustomDisplacement(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    DEM_UNUSED_PARAM(EventId);
    DEM_UNUSED_PARAM(statusByte);
    /* no bit-change: keep confirmed on displacement */
}

/**
 * \brief Called when event combination on storage (type 1) is used and this event is replaced by a report for another event
 * assigned to the same DTC
 */
DEM_INLINE void Dem_EvtSt_CustomEvCombinationReplacement(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte)
{
    DEM_UNUSED_PARAM(EventId);
    Dem_ISO14229ByteSetConfirmedDTC(statusByte, FALSE);
}

/**
 * \brief Called when the pending status gets set or reset on the event
 *
 * \param newValue The new pending state (if true the event is set to pending, if false pending is cleared)
 */
DEM_INLINE void Dem_EvtSt_CustomSetPending(Dem_EventIdType EventId, Dem_UdsStatusByteType *statusByte, Dem_boolean_least newValue)
{
    if (Dem_IsEventEnabledByDtcSetting(EventId))
    {
        Dem_ISO14229ByteSetPendingDTC(statusByte, newValue);
    }
}


#endif

#endif /* INCLUDE_PROTECTION */

