

#ifndef DEM_EVENTSTATUSNVDATA_H
#define DEM_EVENTSTATUSNVDATA_H

#include "Platform_Types.h"
#include "Dem_Array.h"
#include "Dem_Cfg_Events.h"
#include "Dem_BitArray.h"


#define DEM_START_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"

DEM_ARRAY_DECLARE(uint8, Dem_AllEventsStatusByte, DEM_EVENTID_ARRAYLENGTH);

#if(DEM_CFG_ALLOW_HISTORY == DEM_CFG_ALLOW_HISTORY_ON)
DEM_BITARRAY_DECLARE  (Dem_AllEventsHistoryStatus, DEM_EVENTID_ARRAYLENGTH);
#endif

#define DEM_STOP_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"

#endif /* DEM_EVENTSTATUSNVDATA_H*/
