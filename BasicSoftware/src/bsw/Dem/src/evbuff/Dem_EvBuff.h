

#ifndef DEM_EVBUFF_H
#define DEM_EVBUFF_H


#include "Dem_Cfg_EvBuff.h"
#include "Dem_Cfg_Nodes.h"
#include "Dem_EvBuffEvent.h"
#include "Dem_Types.h"
#include "Dem_Mapping.h"
#include "Dem_Events.h"

typedef struct
{
    uint16 OverflowCounter;
    uint16 OverflowCounterSet;
    Dem_EvBuffEvent  Locations[DEM_CFG_EVBUFF_SIZE];
} Dem_EvtBufferState;


/************************************
 * DEM-internal interfaces
 ************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"
extern Dem_EvtBufferState Dem_EvtBuffer;
#define DEM_STOP_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"

#if (DEM_CFG_FFPRESTORAGE_NONVOLATILE == DEM_CFG_FFPRESTORAGE_NONVOLATILE_ON)
#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"
DEM_ARRAY_DECLARE_CONST(Dem_NvmBlockIdType, Dem_PreStoredFFNvmId, DEM_CFG_PRESTORED_FF_SIZE);
#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"
#endif

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"


void Dem_EvBuffRemoveAllPrestored (void);

void Dem_EvBuffMainFunction(void);


#if (DEM_CFG_FFPRESTORAGESUPPORT == DEM_CFG_FFPRESTORAGESUPPORT_ON)
void Dem_PreStoredFFInitCheckNvM(void);
void Dem_PreStoredFFShutdown(void);
#endif


Dem_boolean_least Dem_EvBuffInsert (Dem_EvBuffEventType eventType,
        Dem_EventIdType eventId
        DEM_DEBUGDATA_PARAM(Dem_DebugDataType debug0,Dem_DebugDataType debug1));

const Dem_EvBuffEvent* Dem_EvBuffGetEvent (uint32* locationIndex);

void Dem_EvBuffRemoveEvent (uint32 locationIndex);

void Dem_EvBuffEnvCaptureData(
    Dem_EventIdType EventId,
    uint8 *EnvData
    DEM_DEBUGDATA_PARAM(Dem_DebugDataType debug0,Dem_DebugDataType debug1));


#if (DEM_CFG_DEPENDENCY == DEM_CFG_DEPENDENCY_ON)
uint8_least Dem_EvBuffClearSequentialFailures (Dem_EventIdType EventId, Dem_ComponentIdType nodeID, uint8 counterInit);
#else

DEM_INLINE uint8_least Dem_EvBuffClearSequentialFailures (Dem_EventIdType EventId, Dem_ComponentIdType nodeID, uint8 counterInit) {
    DEM_UNUSED_PARAM(EventId);
    DEM_UNUSED_PARAM(nodeID);
    DEM_UNUSED_PARAM(counterInit);
    return 0;
}

#endif


void Dem_EvBuffClear (Dem_EventIdType EventId);
void Dem_EvBuffCaptureAllEnvData(Dem_EvBuffEvent *evBuffEvent, Dem_EvBuffEvent_MemData *memData);
Dem_boolean_least Dem_EvBuffIsEventPending (Dem_EventIdType EventId);

DEM_INLINE Dem_boolean_least Dem_EvBuffIsAnyEventPending (void)
{
    return Dem_EvBuffIsEventPending (DEM_EVENTID_INVALID);
}

#if(DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED) && (DEM_CFG_FFPRESTORAGESUPPORT == DEM_CFG_FFPRESTORAGESUPPORT_ON) && (DEM_CFG_FFPRESTORAGE_NONVOLATILE == DEM_CFG_FFPRESTORAGE_NONVOLATILE_ON)
DEM_INLINE void Dem_EvBuffClearNvm(void)
{
    uint8 i;
    for (i = 0; i < DEM_CFG_PRESTORED_FF_SIZE; i++)
    {
        /* Zero the content of this particular location */
        DEM_MEMSET(&Dem_EvtBuffer.Locations[i], 0, DEM_SIZEOF_VAR(Dem_EvtBuffer.Locations[i]));
        Dem_NvMClearBlockByInvalidate(Dem_PreStoredFFNvmId[i]);
    }
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
