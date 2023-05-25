


#ifndef DEM_DISTURBANCEMEMORYTYPES_H
#define DEM_DISTURBANCEMEMORYTYPES_H

#include "Dem_Types.h"
#include "Dem_Cfg_DistMem.h"
#include "Dem_Cfg_EnvMain.h"
#include "Dem_Cfg.h"

#if(DEM_CFG_DIST_MEM_CALLBACK_ON_DISTURBANCE == DEM_CFG_DIST_MEM_CALLBACK_ON_DISTURBANCE_ON)
#include "Dem_PrjDisturbanceMemory.h"
#endif

#if(DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)


typedef struct
{
    uint8 disturbanceCtr;
#if(DEM_CFG_DIST_MEM_EXTENDED_DATA_USED == STD_ON)
    uint8 extData[DEM_CFG_DIST_MEM_EXT_DATA_SIZE];
#endif
    Dem_EventIdType eventId;
    #if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
    Dem_DebugDataType debug0;
    Dem_DebugDataType debug1;
    #endif
#if(DEM_CFG_DIST_MEM_CALLBACK_ON_DISTURBANCE == DEM_CFG_DIST_MEM_CALLBACK_ON_DISTURBANCE_ON)
    Dem_DistMemPrjDataType distMemPrjData;
#endif
} Dem_DistMemLocationType;

#endif

#endif /* DEM_DISTURBANCEMEMORYTYPES_H */
