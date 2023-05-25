

#ifndef DEM_EVBUFFTYPES_H
#define DEM_EVBUFFTYPES_H

#include "Dem_Types.h"
#include "Dem_Cfg_EnvMain.h"
#include "Dem_Cfg_EvBuff.h"

#if (DEM_CFG_SUPPORT_PROJECTSPECIFIC_FAILUREMEMORY == TRUE)
#include "Dem_PrjSpecificFailureMemoryType.h"
#endif

#define Dem_EvBuffEventType uint8
#define C_EVENTTYPE_NOEVENT                        0u
#define C_EVENTTYPE_SET                            1u
#define C_EVENTTYPE_SET_RESET                      2u
#define C_EVENTTYPE_RESET                          3u
#define C_EVENTTYPE_SET_RECONFIRMED                4u
#define C_EVENTTYPE_SET_WAITINGFORMONITORING       5u
#define C_EVENTTYPE_PRESTORE                       6u
#define C_EVENTTYPE_UNROBUST                       7u

#define DEM_CFG_EVBUFF_STORES_ENVDATA_ON        STD_ON
#define DEM_CFG_EVBUFF_STORES_ENVDATA_OFF       STD_OFF
#if ((DEM_CFG_DEPENDENCY == DEM_CFG_DEPENDENCY_ON) || (DEM_CFG_FFPRESTORAGESUPPORT == DEM_CFG_FFPRESTORAGESUPPORT_ON) || (DEM_CFG_ENVIRONMENT_DATA_CAPTURE == DEM_CFG_CAPTURE_SYNCHRONOUS_TO_REPORTING))
#define DEM_CFG_EVBUFF_STORES_ENVDATA       DEM_CFG_EVBUFF_STORES_ENVDATA_ON
#else
#define DEM_CFG_EVBUFF_STORES_ENVDATA       DEM_CFG_EVBUFF_STORES_ENVDATA_OFF
#endif

typedef union
{
#if (DEM_CFG_ENVMINSIZE_OF_RAWENVDATA >= 4)
    /* DO NOT USE THIS MEMBER ELEMENT */
    /* enforces uint32 alignment of this union */
    /* REASON: */
    /* in case of a memcpy operation this may increase the copy speed in case src and dst buffers are uint16 or uint32 aligned */
    uint32 EnforceAlignment;
#endif
    uint8  Buffer[DEM_CFG_ENVMINSIZE_OF_RAWENVDATA];
} Dem_EvBuffEnvDataAlignedType;

typedef struct {
    /* HINT:
     * this is intentional the first element in this structure to achieve 32-Bit alignment to support faster memcpy */
    Dem_EvBuffEnvDataAlignedType envData;

    #if (DEM_CFG_SUPPORT_PROJECTSPECIFIC_FAILUREMEMORY == TRUE)
        DemRbProjectSpecificFailureMemoryType  DemRbProjectSpecificFailureMemoryData;
    #endif

    #if( DEM_BFM_ENABLED == DEM_BFM_ON )
        rba_DemBfm_EnvironmentalData DemRbBfmFailureMemoryData;
    #endif
}Dem_EvBuffEvent_MemData;

typedef struct {
#if (DEM_CFG_EVBUFF_STORES_ENVDATA == DEM_CFG_EVBUFF_STORES_ENVDATA_ON)
    Dem_EvBuffEvent_MemData memData;
#endif
    Dem_EvBuffEventType eventType;

#if (DEM_CFG_DEPENDENCY == DEM_CFG_DEPENDENCY_ON)
    uint8 counter;
#endif

    Dem_EventIdType eventId;

#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    Dem_DebugDataType debug0;          /* debug data0   */
    Dem_DebugDataType debug1;          /* debug data1   */
#endif

    boolean IsEnvDataCaptured;
}Dem_EvBuffEvent;

#endif /* DEM_EVBUFFTYPES_H */
