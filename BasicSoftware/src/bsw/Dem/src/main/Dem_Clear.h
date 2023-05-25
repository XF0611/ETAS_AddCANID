


#ifndef DEM_CLEAR_H
#define DEM_CLEAR_H

#include "Dem_Cfg_Clear.h"
#include "Dem_Types.h"
#include "Dem_Cfg_Events.h"
#include "Dem_Mapping.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

void Dem_ClearMainFunction(void);
boolean Dem_ClearIsInProgress (void);

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


#endif
