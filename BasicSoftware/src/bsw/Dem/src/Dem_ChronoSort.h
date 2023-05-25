

#ifndef DEM_CHRONOSORT_H
#define DEM_CHRONOSORT_H

#include "Dem_Cfg_DTCs.h"

#include "Dem_Cfg_DTC_DataStructures.h"


#if (DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED)

#include "Dem_DTCFilter.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

boolean Dem_IsChronoSortEnabled(uint8 ClientId);

void Dem_ChronoSortDTCFilterInit(uint8 ClientId);

void Dem_ChronoSortEvMemUpdateSortId(uint16_least LocId,uint16_least MemId,uint16_least StatusOld,uint16_least StatusNew,uint16_least *WriteSts);

boolean Dem_ChronoSortSetDTCFilter(uint8 ClientId, const Dem_DTCFilterState* dtcFilter_p);

void Dem_ChronoSortMainFunction(uint8 ClientId, const Dem_DTCFilterState* dtcFilter_p);

Std_ReturnType Dem_ChronoSortGetNextFilteredDTCID (uint8 ClientId, Dem_DtcIdType* dtcId, uint32* DTC, uint8* DTCStatus);

Std_ReturnType Dem_ChronoSortGetNumberOfFilteredDTC(uint8 ClientId, uint16* NumberOfFilteredDTC);

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif

#endif
