

#include "Dem_EnvMain.h"
#include "Dem_Cfg_EnvExtendedData.h"
#include "Dem_Cfg_EnvFreezeFrame.h"
#include "Dem_InternalEnvData.h"
#include "Dem_EvMem.h"
#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"
const Dem_EnvDataMap Dem_Cfg_EnvEventId2EnvData[DEM_EVENTID_ARRAYLENGTH] = DEM_CFG_ENVEVENTID2ENVDATA;
#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
void Dem_EnvCaptureED(Dem_EventIdType EventId, uint8* dest, uint16 destSize  DEM_DEBUGDATA_PARAM(Dem_DebugDataType debug0, Dem_DebugDataType debug1))
{
	uint8* writepos = dest;
	uint16 size;
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
    Dem_InternalEnvData internalEnvData;
    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = debug0;
    internalEnvData.debug1 = debug1;
#endif
    internalEnvData.evMemLocation = NULL_PTR;

	DEM_ASSERT(Dem_isEventIdValid(EventId), DEM_DET_APIID_ENVCAPTUREED, 0);
	if (edId != 0)
	{
		DEM_ASSERT(Dem_EnvEDGetRawByteSize(edId) <= destSize, DEM_DET_APIID_ENVCAPTUREED, 1);

		size = Dem_EnvEDGetRawByteSize (edId);
		Dem_EnvEDCapture (edId, writepos, size, &internalEnvData);
	}
}


#if(DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)

void Dem_EnvCaptureForDisturbanceMemory(Dem_EventIdType EventId, uint8* dest, uint16 destSize  DEM_DEBUGDATA_PARAM(Dem_DebugDataType debug0, Dem_DebugDataType debug1))
{
	uint8* writepos = dest;
	uint16 size;
    Dem_InternalEnvData internalEnvData;

    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = debug0;
    internalEnvData.debug1 = debug1;
#endif
    internalEnvData.evMemLocation = NULL_PTR;
	DEM_ASSERT(Dem_EnvEDGetRawByteSize (DEM_CFG_DIST_MEM_EXTENDED_DATA_ID) <= destSize, DEM_DET_APIID_ENVCAPTUREED, 2);

	size = Dem_EnvEDGetRawByteSize (DEM_CFG_DIST_MEM_EXTENDED_DATA_ID);
	Dem_EnvEDCapture (DEM_CFG_DIST_MEM_EXTENDED_DATA_ID, writepos, size, &internalEnvData);
}

#endif

void Dem_EnvCaptureFF(Dem_EventIdType EventId, uint8* dest, uint16 destSize   DEM_DEBUGDATA_PARAM(Dem_DebugDataType debug0, Dem_DebugDataType debug1))
{
	uint8* writepos;
	uint16 size;
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
	uint8 ffId = Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId;
    Dem_InternalEnvData internalEnvData;
    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = debug0;
    internalEnvData.debug1 = debug1;
#endif
    internalEnvData.evMemLocation = NULL_PTR;

	DEM_ASSERT(Dem_isEventIdValid(EventId),DEM_DET_APIID_ENVCAPTUREFF,0);
	if (ffId != 0)
	{
		DEM_ASSERT((Dem_EnvEDGetRawByteSize (edId) + Dem_EnvFFGetRawByteSize (ffId) )<= destSize, DEM_DET_APIID_ENVCAPTUREFF, 1);

		writepos = (dest + Dem_EnvEDGetRawByteSize(edId));
		size = Dem_EnvFFGetRawByteSize (ffId);
		Dem_EnvFFCapture (ffId, writepos, size, &internalEnvData);
	}
}

void Dem_EnvCopyRawFF(Dem_EventIdType EventId,
		uint8* dest,
		uint16 destSize,
		uint8 ffIndex,
		const uint8* src)
{
	uint8* writepos = dest;
	uint16 size;

	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
	uint8 ffId = Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId;

	if (ffId != 0)
	{
		writepos += Dem_EnvEDGetRawByteSize (edId);
		writepos += (ffIndex * Dem_EnvFFGetRawByteSize (ffId));
		size = Dem_EnvFFGetRawByteSize (ffId);

		DEM_ASSERT((writepos + size) <= (dest + destSize), DEM_DET_APIID_ENVCOPYRAWFF, 0);

		Dem_EnvFFCopyRaw (ffId, writepos, size, src + Dem_EnvEDGetRawByteSize (edId));
	}
}

void Dem_EnvCopyRawED(Dem_EventIdType EventId, uint8* dest, uint16 destSize, const uint8* src, Dem_EnvTriggerParamType *triggerParam)
{
	uint8* writepos = dest;
	const uint8* readpos = src;
	uint16 size;

	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;

	if (edId != 0)
	{
		size = Dem_EnvEDGetRawByteSize (edId);

		DEM_ASSERT(size <= destSize,DEM_DET_APIID_ENVUPDATERAWED, 0);
		Dem_EnvEDCopyRaw (edId, writepos, size, readpos, triggerParam);
	}
}

/* Support functions for DCM-querries: Dem_GetNextFreezeFrameData, Dem_GetNextExtendedDataRecord */

Dem_boolean_least Dem_EnvIsEDRNumberValid(Dem_EventIdType EventId, uint8 RecordNumber, Dem_TriggerType* trigger)
{
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
	uint16 EDRId;
	if (edId != 0)
	{
		return Dem_EnvEDIsRecordNumberValid (edId, RecordNumber, trigger, &EDRId);
	}
	else
	{
		return FALSE;
	}
}

Dem_boolean_least Dem_EnvIsEDRNumberStored(Dem_EventIdType EventId, uint8 RecordNumber, const Dem_EvMemEventMemoryType *EventMemory)
{
    uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
    Dem_TriggerType trigger = 0;
    uint16 EDRId;
    Dem_boolean_least returnValue = FALSE;
    if (Dem_EnvEDIsRecordNumberValid(edId, RecordNumber, &trigger, &EDRId))
    {
        if(Dem_EnvIsTriggerSet(Dem_EvMemGetEventMemTriggerByPtr(EventMemory), trigger))
        {
            returnValue = TRUE;
        }
    }
    return returnValue;
}


Std_ReturnType Dem_EnvRetrieveEDR(Dem_EventIdType EventId,
		uint8 RecordNumber,
		uint8* dest,
		uint16* destSize,
		const uint8* src,
		Dem_EvMemEventMemoryType *evMemLocation)
{
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
    Dem_InternalEnvData internalEnvData;

    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = 0;
    internalEnvData.debug1 = 0;
#endif
    internalEnvData.evMemLocation = evMemLocation;

	DEM_ASSERT(Dem_EnvHasExtendedData(EventId), DEM_DET_APIID_ENVRETRIEVEEDR, 0);
	return Dem_EnvEDRetrieveRawOrFormattedExtendedDataRecord (edId, RecordNumber, dest, destSize, src, &internalEnvData, FALSE);
}

Std_ReturnType Dem_EnvGetSizeOfEDR(Dem_EventIdType EventId,
		uint8 RecordNumber,
		uint16* size)
{
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;

	DEM_ASSERT(Dem_EnvHasExtendedData(EventId), DEM_DET_APIID_ENVGETSIZEOFEDR, 0);
	return Dem_EnvEDGetSizeOfEDR (edId, RecordNumber, size);
}

Std_ReturnType Dem_EnvGetSizeOfEDR_AllOrObdStored(Dem_EventIdType EventId, uint16* size, const Dem_EvMemEventMemoryType *EventMemory, boolean OnlyOBD )
{
    uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
    uint16 IndividualRecSize;
    uint8 RecNum;
    uint16 EDRecId;
    *size = 0;

    DEM_ASSERT(Dem_EnvHasExtendedData(EventId), DEM_DET_APIID_ENVGETSIZEOFEDR_ALLOROBDSTORED, 0);
    for(EDRecId = Dem_Cfg_EnvExtData[edId - 1].extDataRecIndex; EDRecId <Dem_Cfg_EnvExtData[edId].extDataRecIndex; EDRecId++)
    {
        RecNum = Dem_EnvEDRGetRecordNumber(Dem_Cfg_EnvExtData2ExtDataRec[EDRecId]);
        if((! OnlyOBD) || (RecNum >= DEM_START_OF_OBD_ED_REC_NUM))
        {
            if(Dem_EnvIsEDRNumberStored(EventId, RecNum, EventMemory))
            {
                if (Dem_EnvGetSizeOfEDR(EventId, RecNum, &IndividualRecSize) == E_OK)
                {
                    *size = *size + IndividualRecSize ;
                }

            }
        }
    }
    return E_OK;
}

Std_ReturnType Dem_EnvRetrieveFF(Dem_EventIdType EventId,
		uint8* dest,
		uint16* destSize,
		uint8 ffIndex,
		const uint8* src,
		Dem_EvMemEventMemoryType *evMemLocation)
{
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
	uint8 ffId = Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId;
	const uint8* readpos;
    Dem_InternalEnvData internalEnvData;

    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = 0;
    internalEnvData.debug1 = 0;
#endif
    internalEnvData.evMemLocation = evMemLocation;

	DEM_ASSERT(Dem_EnvHasFreezeFrame(EventId),DEM_DET_APIID_ENVRETRIEVEFF,0);

	readpos = src + Dem_EnvEDGetRawByteSize (edId) /* if event does not have extdata, size=0 will be returned */
			+ (Dem_EnvFFGetRawByteSize (ffId) * ffIndex);

	return Dem_EnvFFRetrieve (ffId, Dem_EnvGetFFRecNumFromIndex (EventId, ffIndex), dest,
            destSize, readpos, &internalEnvData);
}

Std_ReturnType Dem_EnvGetSizeOfFF(Dem_EventIdType EventId, uint16* size)
{
    Std_ReturnType returnValue = DEM_NO_SUCH_ELEMENT;
	uint8 ffId = Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId;
	if (Dem_EnvHasFreezeFrame(EventId))
	{
	    returnValue = Dem_EnvFFGetSize (ffId, size);
	}
	return returnValue;
}

uint8 Dem_EnvGetTotalNumberOfStoredFFForEvent(Dem_EventIdType EventId, const Dem_EvMemEventMemoryType* EvMem)
{
    uint8 idx;
    uint8 NumOfStoredFF = 0;

    for(idx = 0; idx < Dem_EvtParam_GetMaxNumberFreezeFrameRecords(EventId); idx++)
    {
        if(Dem_EnvIsFFRecNumStored(EvMem, Dem_EnvGetFFRecNumFromIndex(EventId, idx)))
        {
            NumOfStoredFF++;
        }
    }
    return NumOfStoredFF;
}

/* Support functions for DLT: Dem_GetEventExtendedDataRecord, Dem_GetEventFreezeFrameData */

Std_ReturnType Dem_EnvRetrieveRawED(Dem_EventIdType EventId,
		uint8* dest,
		uint16* destSize,
		const uint8* src,
		Dem_EvMemEventMemoryType *evMemLocation)
{
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
	uint16 size = Dem_EnvEDGetRawByteSize (edId);
    Dem_InternalEnvData internalEnvData;

    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = 0;
    internalEnvData.debug1 = 0;
#endif
    internalEnvData.evMemLocation = evMemLocation;

	if (Dem_EnvHasExtendedData(EventId) && (size <= *destSize))
	{
        (void)Dem_EnvEDRetrieveExtendedData (edId, dest, destSize, src, &internalEnvData);
		*destSize = size;
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}

Dem_boolean_least Dem_EnvRetrieveRawEDR(Dem_EventIdType EventId,
		uint8 RecordNumber,
		uint8* dest,
		uint16* destSize,
		const uint8* src,
		Dem_EvMemEventMemoryType *evMemLocation)
{
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
    Dem_InternalEnvData internalEnvData;

    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = 0;
    internalEnvData.debug1 = 0;
#endif
    internalEnvData.evMemLocation = evMemLocation;

	if (Dem_EnvHasExtendedData(EventId))
	{
        return (Dem_EnvEDRetrieveRawOrFormattedExtendedDataRecord (edId, RecordNumber, dest, destSize, src, &internalEnvData, TRUE)
				== E_OK);
	}
	else
	{
		return FALSE;
	}
}

Std_ReturnType Dem_EnvRetrieveRawFF(Dem_EventIdType EventId,
		uint8* dest,
		uint16* destSize,
		uint8 ffIndex,
		const uint8* src,
		Dem_EvMemEventMemoryType *evMemLocation)
{
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
	uint8 ffId = Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId;
	uint16 size = Dem_EnvFFGetRawByteSize (ffId);
    Dem_InternalEnvData internalEnvData;
	const uint8* readpos = src + Dem_EnvEDGetRawByteSize (edId) + (Dem_EnvFFGetRawByteSize (ffId)
			* ffIndex);

    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = 0;
    internalEnvData.debug1 = 0;
#endif
    internalEnvData.evMemLocation = evMemLocation;

	if (Dem_EnvHasFreezeFrame(EventId) && (size <= *destSize))
	{
        Dem_EnvFFRetrieveRaw (ffId, dest, &size, readpos, &internalEnvData);
		*destSize = size;
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}

/* --- */

Dem_boolean_least Dem_EnvRetrieveRawDid(Dem_EventIdType EventId,
		uint8* dest,
		uint16* destSize,
		uint8 ffIndex,
		uint16 did,
		const uint8* src,
		Dem_EvMemEventMemoryType *evMemLocation)
{
	uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
	uint8 ffId = Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId;
    Dem_InternalEnvData internalEnvData;
	const uint8* readpos = src + Dem_EnvEDGetRawByteSize (edId) + (Dem_EnvFFGetRawByteSize (ffId)
			* ffIndex);
    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = 0;
    internalEnvData.debug1 = 0;
#endif
    internalEnvData.evMemLocation = evMemLocation;

	if (Dem_EnvHasFreezeFrame(EventId))
	{
		return Dem_EnvFFRetrieveDid (ffId, dest, destSize, did, readpos, &internalEnvData);
	}
	else
	{
		return FALSE;
	}
}


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
