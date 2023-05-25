

#ifndef DEM_DTCFILTER_H
#define DEM_DTCFILTER_H


#include "Dem_Types.h"
#include "Dem_DTCs.h"
#include "Dem_DTCStatusByte.h"
#include "Dem_Main.h"

typedef struct
{
   boolean isNewFilterCriteria;
   boolean filterSet;
   uint8 DTCStatusMask;
   Dem_DTCFormatType DTCFormat;
   Dem_DTCOriginType DTCOrigin;
   boolean FilterWithSeverity;
   Dem_DTCSeverityType DTCSeverityMask;
   boolean FilterForFaultDetectionCounter;
   uint16 numberOfMatchingDTCs;
   Dem_DtcIdIterator searchIt, retrieveIt;
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
   rba_DemObdBasic_DTCFilterState obdFilter;
#endif
} Dem_DTCFilterState;

#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"
    DEM_BITARRAY_DECLARE (Dem_DTCFilterMatching[DEM_CLIENTID_ARRAYLENGTH_STD],DEM_DTCID_ARRAYLENGTH);
#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

Std_ReturnType Dem_GetNumberOfStandardFilteredDTC(uint8 ClientId, uint16* NumberOfFilteredDTC);
Std_ReturnType Dem_GetNextFilteredStandardDTCID (uint8 ClientId, Dem_DtcIdType* dtcId, uint32* DTC, uint8* DTCStatus, sint8* DTCFaultDetectionCounter);
void Dem_DTCFilterStandardMainFunction(uint8 ClientId);

void Dem_SetDTCFilterstartIterator(uint8 ClientId);
boolean Dem_IsStandardFilterFinished(const Dem_DtcIdIterator *it);
void Dem_FilterInit(void);
void Dem_DtcFilterInit(uint8 ClientId);
void Dem_DTCFilterMainFunction(void);
Dem_boolean_least Dem_DTCFilterMatches (const Dem_DTCFilterState* dtcFilter_p, Dem_DtcIdType dtcId);

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
