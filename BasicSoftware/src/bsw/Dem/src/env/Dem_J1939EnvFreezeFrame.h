
#ifndef DEM_J1939ENVFREEZEFRAME_H
#define DEM_J1939ENVFREEZEFRAME_H

#include "Dem_Types.h"

#include "Dem_Cfg_Main.h"
#include "Dem_Cfg_EventId.h"
#include "Dem_Cfg_J1939EnvSpn.h"
#include "Dem_Cfg_J1939EnvFreezeFrame.h"
#include "Dem_Cfg_EnvMain.h"
#include "Dem_EnvDataElement.h"

#if(DEM_CFG_J1939DCM_FREEZEFRAME_SUPPORT || DEM_CFG_J1939DCM_EXPANDED_FREEZEFRAME_SUPPORT)

typedef struct
{
    uint8 J1939FreezeFrameId;
    uint8 J1939ExpFreezeFrameId;
} Dem_J1939EnvDataMap;

typedef struct
{
    uint16 SPNIndex;
    uint16 rawByteSize;
} Dem_J1939EnvFreezeFrame;

typedef struct
{
    uint16 dataElementIndex;
    uint16 SpnID;
} Dem_EnvSPN;

#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"

extern const Dem_J1939EnvDataMap Dem_Cfg_J1939EnvEventId2EnvData[DEM_EVENTID_ARRAYLENGTH];

extern const uint8 Dem_Cfg_J1939EnvFreezeFrame2SPN[];
DEM_ARRAY_DECLARE_CONST(Dem_J1939EnvFreezeFrame, Dem_Cfg_J1939EnvFreezeFrame, DEM_CFG_J1939_ENVFREEZEFRAME_ARRAYLENGTH);

extern const uint8 Dem_Cfg_EnvSpn2DataElement[];
DEM_ARRAY_DECLARE_CONST(Dem_EnvSPN, Dem_Cfg_EnvSpn, DEM_CFG_J1939_ENVSPN_ARRAYLENGTH);

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

boolean Dem_J1939EvMem_IsTriggerFreezeFrame(Dem_EventIdType EventId, uint16_least MemId, uint16_least StatusOld, uint16_least StatusNew);

void Dem_J1939EvMem_CopyFreezeFrame(Dem_EventIdType EventId, uint8* dest, uint16 destSize, const uint8* src);

Dem_ReturnGetNextFilteredElementType Dem_J1939EnvRetrieveFreezeFrame(Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind, Dem_EventIdType EventId, uint8* dest, uint16* destSize, const uint8* src, Dem_EvMemEventMemoryType *evMemLocation);

#if(DEM_CFG_J1939DCM_FREEZEFRAME_SUPPORT)
void Dem_J1939EnvCaptureFF(Dem_EventIdType EventId, uint8* dest, uint16 destSize   DEM_DEBUGDATA_PARAM(Dem_DebugDataType debug0, Dem_DebugDataType debug1));
void Dem_J1939_CopyFF(Dem_EventIdType EventId, uint8* dest, uint16 destSize, const uint8* src);
Dem_ReturnGetNextFilteredElementType Dem_J1939EnvRetrieveFF(Dem_EventIdType EventId, uint8* dest, uint16* destSize, const uint8* src, Dem_EvMemEventMemoryType *evMemLocation);
#endif

#if(DEM_CFG_J1939DCM_EXPANDED_FREEZEFRAME_SUPPORT)
void Dem_J1939EnvCaptureExpFF(Dem_EventIdType EventId, uint8* dest, uint16 destSize   DEM_DEBUGDATA_PARAM(Dem_DebugDataType debug0, Dem_DebugDataType debug1));
void Dem_J1939_CopyExpFF(Dem_EventIdType EventId, uint8* dest, uint16 destSize, const uint8* src);
Dem_ReturnGetNextFilteredElementType Dem_J1939EnvRetrieveExpFF(Dem_EventIdType EventId, uint8* dest, uint16* destSize, const uint8* src, Dem_EvMemEventMemoryType *evMemLocation);
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/***************************************************** FREEZE FRAME CAPTURE*********************************************/
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
DEM_INLINE void Dem_J1939EnvFFCopyRaw(uint8 freezeFrameId, uint8* dest, uint16 bufsize, const uint8* src)
{
    const uint16 bytesize = Dem_Cfg_J1939EnvFreezeFrame[freezeFrameId].rawByteSize;

    DEM_ASSERT(bytesize <= bufsize, DEM_DET_APIID_J1939ENVFFCOPYRAW, 0);
    DEM_MEMCPY (dest, src, bytesize);
}


DEM_INLINE Dem_boolean_least Dem_J1939EnvHasExpFreezeFrame(Dem_EventIdType EventId)
{
    return (Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939ExpFreezeFrameId != 0);
}

DEM_INLINE Dem_boolean_least Dem_J1939EnvHasFreezeFrame(Dem_EventIdType EventId)
{
    return (Dem_Cfg_J1939EnvEventId2EnvData[EventId].J1939FreezeFrameId != 0);
}

DEM_INLINE uint16 Dem_J1939EnvFFGetRawByteSize(uint8 freezeFrameId)
{
    return Dem_Cfg_J1939EnvFreezeFrame[freezeFrameId].rawByteSize;
}

DEM_INLINE uint16 Dem_J1939EnvOffsetforOBDSize (void)
{
    return DEM_CFG_OFFSET_OBDRAWENVDATA;
}

DEM_INLINE void Dem_J1939EnvSpnCapture(uint8 Spn, uint8** start, const uint8* end, const Dem_InternalEnvData *internalEnvData)
{
    uint16_least i;
    for (i = Dem_Cfg_EnvSpn[Spn - 1].dataElementIndex;
         i < Dem_Cfg_EnvSpn[Spn].dataElementIndex;
         i++)
    {
        Dem_EnvDACapture(Dem_Cfg_EnvSpn2DataElement[i], start, end, internalEnvData);
    }
}

DEM_INLINE void Dem_J1939EnvFFCapture(uint8 freezeFrameId, uint8* buffer, uint16 size, const Dem_InternalEnvData* internalEnvData)
{
    uint32 i;
    uint8* end = buffer + size;

    for (i = Dem_Cfg_J1939EnvFreezeFrame[freezeFrameId - 1].SPNIndex; i < Dem_Cfg_J1939EnvFreezeFrame[freezeFrameId].SPNIndex; i++)
    {
        Dem_J1939EnvSpnCapture (Dem_Cfg_J1939EnvFreezeFrame2SPN[i], &buffer, end, internalEnvData);
    }

#if (DEM_CFG_BUILDTARGET==DEM_CFG_BUILDTARGET_DEMTESTSUITE)
    DEM_MEMSET(buffer,0xFF,end-buffer);
#endif
}

/***************************************************** FREEZE FRAME RETRIEVE ******************************************/

/* copies all dataelements of DID from input stream to output stream */

/* MR12 RULE 8.13 VIOLATION: parameter src not made const, as it is modified in subfunction */
DEM_INLINE Dem_boolean_least Dem_EnvSPNRetrieveRaw(uint8 Spn, uint8** start, const uint8* end, const uint8** src, const Dem_InternalEnvData* internalEnvData)
{
    uint16_least i;

    for (i = Dem_Cfg_EnvSpn[Spn - 1].dataElementIndex;
         i < Dem_Cfg_EnvSpn[Spn].dataElementIndex;
         i++)
    {
        if (!Dem_EnvDARetrieve(Dem_Cfg_EnvSpn2DataElement[i], start, end, src, internalEnvData))
        {
            return FALSE;
        }
    }
    return TRUE;
}

DEM_INLINE Dem_ReturnGetNextFilteredElementType Dem_J1939EnvFFRetrieve(uint8 freezeFrameId,
        uint8* dest,
        uint16* bufsize,
        const uint8* src,
        const Dem_InternalEnvData* internalEnvData)
{
    uint8* writepos = dest;
    uint8* end = dest + *bufsize;
    uint16_least i;

    if (*bufsize < Dem_J1939EnvFFGetRawByteSize(freezeFrameId))
    {
        return DEM_FILTERED_BUFFER_TOO_SMALL;
    }

    for (i = Dem_Cfg_J1939EnvFreezeFrame[freezeFrameId - 1].SPNIndex; i < Dem_Cfg_J1939EnvFreezeFrame[freezeFrameId].SPNIndex; i++)
    {
        if (!Dem_EnvSPNRetrieveRaw (Dem_Cfg_J1939EnvFreezeFrame2SPN[i], &writepos, end, &src, internalEnvData))
        {
            return DEM_FILTERED_BUFFER_TOO_SMALL;
        }
    }

    *bufsize = (uint16)(writepos - dest); /* current writing position - starting writing position */
    return DEM_FILTERED_OK;
}

#endif /* DEM_CFG_J1939DCM_FREEZEFRAME_SUPPORT || DEM_CFG_J1939DCM_EXPANDED_FREEZEFRAME_SUPPORT */

#endif
