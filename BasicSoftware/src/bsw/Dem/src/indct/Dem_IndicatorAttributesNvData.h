


#ifndef DEM_INDICATORATTRIBUTESNVDATA_H
#define DEM_INDICATORATTRIBUTESNVDATA_H

#include "Dem_IndicatorAttributesTypes.h"
#include "Dem_Array.h"
#include "Dem_Cfg_EventId.h"
#include "Dem_Cfg_EventIndicators.h"


#if (DEM_CFG_EVT_INDICATOR == DEM_CFG_EVT_INDICATOR_ON)
#define DEM_START_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"
DEM_ARRAY_DECLARE(Dem_EvtIndicatorAttributeState, Dem_AllEventsIndicatorState, DEM_INDICATOR_ATTRIBUTE_ARRAYLENGTH);
#define DEM_STOP_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"

#endif

#endif /* DEM_INDICATORATTRIBUTESTYPESNVDATA_H */
