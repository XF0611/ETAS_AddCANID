

#ifndef DEM_DISTURBANCEMEMORYNVDATA_H
#define DEM_DISTURBANCEMEMORYNVDATA_H

#include "Dem_DisturbanceMemoryTypes.h"
#include "Dem_Array.h"

#if(DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)

#define DEM_START_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"
DEM_ARRAY_DECLARE(Dem_DistMemLocationType, Dem_DistMemLocations, (DEM_CFG_DISTURBANCE_MEMORY_ARRAYLENGTH));
#define DEM_STOP_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"

#endif
#endif /* DEM_DISTURBANCEMEMORYNVDATA_H */
