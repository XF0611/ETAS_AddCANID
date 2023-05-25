

#include "Dem_Types.h"
#include "Dem_Array.h"
#include "Dem_InternalEnvData.h"
#include "Dem_J1939EnvFreezeFrame.h"
#include "Dem_EnvMain.h"
#include "Dem_EnvExtendedData.h"
#include "Dem_EnvFreezeFrame.h"

#if(DEM_CFG_J1939DCM_FREEZEFRAME_SUPPORT || DEM_CFG_J1939DCM_EXPANDED_FREEZEFRAME_SUPPORT)

#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"
const Dem_J1939EnvDataMap Dem_Cfg_J1939EnvEventId2EnvData[DEM_EVENTID_ARRAYLENGTH] = DEM_CFG_J1939_ENVEVENTID2ENVDATA;

const uint8 Dem_Cfg_J1939EnvFreezeFrame2SPN[] = DEM_CFG_J1939_ENVFREEZEFRAME2SPN;
/* MR12 RULE 20.7 VIOLATION: Due to the fact that array content and array size are both generated based on the same input it is ensured that the data always fit in to the array */
DEM_ARRAY_DEFINE_CONST(Dem_J1939EnvFreezeFrame, Dem_Cfg_J1939EnvFreezeFrame, DEM_CFG_J1939_ENVFREEZEFRAME_ARRAYLENGTH, DEM_CFG_J1939_ENVFREEZEFRAME);

const uint8 Dem_Cfg_EnvSpn2DataElement[] = DEM_CFG_J1939_ENVSPN2DATAELEMENT;
/* MR12 RULE 20.7 VIOLATION: Due to the fact that array content and array size are both generated based on the same input it is ensured that the data always fit in to the array */
DEM_ARRAY_DEFINE_CONST(Dem_EnvSPN, Dem_Cfg_EnvSpn, DEM_CFG_J1939_ENVSPN_ARRAYLENGTH, DEM_CFG_J1939_ENVSPN);

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/***************************************************** FREEZE FRAME CAPTURE*********************************************/

void Dem_J1939EnvCaptureFF(Dem_EventIdType EventId, uint8* dest, uint16 destSize   DEM_DEBUGDATA_PARAM(Dem_DebugDataType debug0, Dem_DebugDataType debug1))
{
    uint8* writepos;
    uint16 size;
    uint8 J1939ffId = Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939FreezeFrameId;
    uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
    uint8 ffId = Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId;
    Dem_InternalEnvData internalEnvData;
    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = debug0;
    internalEnvData.debug1 = debug1;
#endif
    internalEnvData.evMemLocation = NULL_PTR;

    DEM_ASSERT(Dem_isEventIdValid(EventId),DEM_DET_APIID_J1939ENVCAPTUREFF,0);
    if (J1939ffId != 0)
    {
        DEM_ASSERT((Dem_EnvEDGetRawByteSize (edId) + Dem_EnvFFGetRawByteSize (ffId) +Dem_J1939EnvOffsetforOBDSize() + Dem_J1939EnvFFGetRawByteSize (J1939ffId))<= destSize, DEM_DET_APIID_J1939ENVCAPTUREFF, 1);

        writepos = dest + Dem_EnvEDGetRawByteSize(edId) + Dem_EnvFFGetRawByteSize(ffId) + Dem_J1939EnvOffsetforOBDSize();
        size = Dem_J1939EnvFFGetRawByteSize (J1939ffId);
        Dem_J1939EnvFFCapture (J1939ffId, writepos, size, &internalEnvData);
    }
}

void Dem_J1939EnvCaptureExpFF(Dem_EventIdType EventId, uint8* dest, uint16 destSize   DEM_DEBUGDATA_PARAM(Dem_DebugDataType debug0, Dem_DebugDataType debug1))
{
    uint8* writepos;
    uint16 size;
    uint8 J1939expffId = Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939ExpFreezeFrameId;
    uint8 J1939ffId = Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939FreezeFrameId;
    uint8 edId = Dem_Cfg_EnvEventId2EnvData[EventId].extDataId;
    uint8 ffId = Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId;
    Dem_InternalEnvData internalEnvData;
    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = debug0;
    internalEnvData.debug1 = debug1;
#endif
    internalEnvData.evMemLocation = NULL_PTR;

    DEM_ASSERT(Dem_isEventIdValid(EventId),DEM_DET_APIID_J1939ENVCAPTUREEXPFF,0);
    if (J1939expffId != 0)
    {
        DEM_ASSERT((Dem_EnvEDGetRawByteSize (edId) + Dem_EnvFFGetRawByteSize (ffId) + Dem_J1939EnvOffsetforOBDSize() + Dem_J1939EnvFFGetRawByteSize(J1939ffId) + Dem_J1939EnvFFGetRawByteSize (J1939expffId))<= destSize, DEM_DET_APIID_J1939ENVCAPTUREEXPFF, 1);

        writepos = dest + Dem_EnvEDGetRawByteSize(edId) + Dem_EnvFFGetRawByteSize(ffId) + Dem_J1939EnvOffsetforOBDSize() + Dem_J1939EnvFFGetRawByteSize(J1939ffId);
        size = Dem_J1939EnvFFGetRawByteSize (J1939expffId);
        Dem_J1939EnvFFCapture (J1939expffId, writepos, size, &internalEnvData);
    }
}

boolean Dem_J1939EvMem_IsTriggerFreezeFrame(Dem_EventIdType EventId, uint16_least MemId, uint16_least StatusOld, uint16_least StatusNew)
{
    boolean trigger = FALSE;

    DEM_UNUSED_PARAM(MemId); // Implement MemID check if needed

    if (Dem_J1939EnvHasFreezeFrame(EventId) || Dem_J1939EnvHasExpFreezeFrame(EventId))
    {
        if ((StatusOld & DEM_EVMEM_STSMASK_CONFIRMED) != DEM_EVMEM_STSMASK_CONFIRMED)
        {
            trigger = Dem_EvMemIsEdgeTrigger(StatusOld, StatusNew, DEM_EVMEM_STSMASK_PENDING);
        }
    }

    return trigger;
}

void Dem_J1939_CopyFF(Dem_EventIdType EventId, uint8* dest, uint16 destSize, const uint8* src)
{
    uint8* writepos;
    uint16 size;
    const uint8* J1939FFpos;

    uint8 J1939ffId = Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939FreezeFrameId;

    uint16 obdFFsize = Dem_J1939EnvOffsetforOBDSize();
    uint16 EDsize = Dem_EnvEDGetRawByteSize(Dem_Cfg_EnvEventId2EnvData[EventId].extDataId);
    uint16 FFsize = Dem_EnvFFGetRawByteSize(Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId);

    J1939FFpos = src + EDsize + FFsize + obdFFsize;
    writepos = dest + EDsize + (Dem_EvtParam_GetMaxNumberFreezeFrameRecords(EventId) * FFsize) + obdFFsize;
    size = Dem_J1939EnvFFGetRawByteSize (J1939ffId);

    DEM_ASSERT((writepos + size) <= (dest + destSize), DEM_DET_APIID_J1939ENVCOPYRAWFF, 0);

    Dem_J1939EnvFFCopyRaw(J1939ffId, writepos, size, J1939FFpos);
}

void Dem_J1939_CopyExpFF(Dem_EventIdType EventId, uint8* dest, uint16 destSize, const uint8* src)
{
    uint8* writepos;
    uint16 size;
    const uint8* J1939FFpos;

    uint8 J1939expffId = Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939ExpFreezeFrameId;
    uint8 J1939ffId = Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939FreezeFrameId;

    uint16 obdFFsize = Dem_J1939EnvOffsetforOBDSize();
    uint16 EDsize = Dem_EnvEDGetRawByteSize(Dem_Cfg_EnvEventId2EnvData[EventId].extDataId);
    uint16 FFsize = Dem_EnvFFGetRawByteSize(Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId);
    uint16 J1939FFSize = Dem_J1939EnvFFGetRawByteSize(J1939ffId);

    J1939FFpos = src + EDsize + FFsize + obdFFsize + J1939FFSize;
    writepos = dest + EDsize + (Dem_EvtParam_GetMaxNumberFreezeFrameRecords(EventId) * FFsize) + obdFFsize + J1939FFSize;
    size = Dem_J1939EnvFFGetRawByteSize (J1939expffId);

    DEM_ASSERT((writepos + size) <= (dest + destSize), DEM_DET_APIID_J1939ENVCOPYRAWEXPFF, 0);

    Dem_J1939EnvFFCopyRaw(J1939expffId, writepos, size, J1939FFpos);
}

void Dem_J1939EvMem_CopyFreezeFrame(Dem_EventIdType EventId, uint8* dest, uint16 destSize, const uint8* src)
{

#if(DEM_CFG_J1939DCM_FREEZEFRAME_SUPPORT)
    if(Dem_J1939EnvHasFreezeFrame(EventId))
    {
        Dem_J1939_CopyFF (EventId, dest, destSize, src);
    }
#endif

#if(DEM_CFG_J1939DCM_EXPANDED_FREEZEFRAME_SUPPORT)
    if(Dem_J1939EnvHasExpFreezeFrame(EventId))
    {
        Dem_J1939_CopyExpFF (EventId, dest, destSize, src);
    }
#endif

}

/***************************************************** FREEZE FRAME RETRIEVE ******************************************/

Dem_ReturnGetNextFilteredElementType Dem_J1939EnvRetrieveFF(Dem_EventIdType EventId,
        uint8* dest,
        uint16* destSize,
        const uint8* src,
        Dem_EvMemEventMemoryType *evMemLocation)
{

    uint8 J1939ffId = Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939FreezeFrameId;

    uint16 obdFFsize = Dem_J1939EnvOffsetforOBDSize();
    uint16 EDsize = Dem_EnvEDGetRawByteSize(Dem_Cfg_EnvEventId2EnvData[EventId].extDataId);
    uint16 FFsize = Dem_EnvFFGetRawByteSize(Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId);

    const uint8* readpos;
    Dem_InternalEnvData internalEnvData;

    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = 0;
    internalEnvData.debug1 = 0;
#endif
    internalEnvData.evMemLocation = evMemLocation;

   DEM_ASSERT(Dem_J1939EnvHasExpFreezeFrame(EventId),DEM_DET_APIID_J1939ENVRETRIEVEFF,0);

    readpos = src + EDsize + (Dem_EvtParam_GetMaxNumberFreezeFrameRecords(EventId) * FFsize) + obdFFsize;

    return Dem_J1939EnvFFRetrieve (J1939ffId, dest, destSize, readpos, &internalEnvData);

}

Dem_ReturnGetNextFilteredElementType Dem_J1939EnvRetrieveExpFF(Dem_EventIdType EventId,
        uint8* dest,
        uint16* destSize,
        const uint8* src,
        Dem_EvMemEventMemoryType *evMemLocation)
{

    uint8 J1939expffId = Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939ExpFreezeFrameId;
    uint8 J1939ffId = Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939FreezeFrameId;

    uint16 obdFFsize = Dem_J1939EnvOffsetforOBDSize();
    uint16 EDsize = Dem_EnvEDGetRawByteSize(Dem_Cfg_EnvEventId2EnvData[EventId].extDataId);
    uint16 FFsize = Dem_EnvFFGetRawByteSize(Dem_Cfg_EnvEventId2EnvData[EventId].freezeFrameId);
    uint16 J1939FFSize = Dem_J1939EnvFFGetRawByteSize(J1939ffId);

    const uint8* readpos;
    Dem_InternalEnvData internalEnvData;

    internalEnvData.eventId = EventId;
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    internalEnvData.debug0 = 0;
    internalEnvData.debug1 = 0;
#endif
    internalEnvData.evMemLocation = evMemLocation;

    DEM_ASSERT(Dem_J1939EnvHasFreezeFrame(EventId),DEM_DET_APIID_J1939ENVRETRIEVEEXPFF,0);

    readpos = src + EDsize + (Dem_EvtParam_GetMaxNumberFreezeFrameRecords(EventId) * FFsize) + obdFFsize + J1939FFSize;

    return Dem_J1939EnvFFRetrieve (J1939expffId, dest, destSize, readpos, &internalEnvData);

}

Dem_ReturnGetNextFilteredElementType Dem_J1939EnvRetrieveFreezeFrame(Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind,
        Dem_EventIdType EventId,
        uint8* dest,
        uint16* destSize,
        const uint8* src,
        Dem_EvMemEventMemoryType *evMemLocation)
{

#if(DEM_CFG_J1939DCM_FREEZEFRAME_SUPPORT)
    if(FreezeFrameKind == DEM_J1939DCM_FREEZEFRAME)
    {
        return Dem_J1939EnvRetrieveFF(EventId, dest, destSize, src, evMemLocation);
    }
    else
#endif
#if(DEM_CFG_J1939DCM_EXPANDED_FREEZEFRAME_SUPPORT)
        if(FreezeFrameKind == DEM_J1939DCM_EXPANDED_FREEZEFRAME)
        {
            return Dem_J1939EnvRetrieveExpFF(EventId, dest, destSize, src, evMemLocation);
        }
        else
#endif
        {
            return DEM_FILTERED_NO_MATCHING_DTC;
        }

}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif /* DEM_CFG_J1939DCM_FREEZEFRAME_SUPPORT || DEM_CFG_J1939DCM_EXPANDED_FREEZEFRAME_SUPPORT */
