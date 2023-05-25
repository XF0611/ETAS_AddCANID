


#ifndef DEM_EVMEMAPI_H
#define DEM_EVMEMAPI_H

#include "Dem_EvMemTypes.h"
#include "Dem_Cfg_EvMem.h"
#include "Dem_EvMemBase.h"
#include "Dem_Cfg_Client.h"

#if DEM_CFG_SERVICE_READDTCINFORMATION_SUBFUNC_0x03
typedef struct
{
  uint16_least      FilteredRecordLocIdIterator;
  uint16_least      FilteredRecordLocIdOfDTC;   //Initialize to invalid LocId in init.
  uint8             FilteredRecordFreezeFrameIdIterator;
  boolean           IsFFRecordOfDTCReported[DEM_CFG_MAX_NUMBER_EVENT_ENTRY_PRIMARY];
  Dem_DtcIdType     CurrentDtcId;
} Dem_Client_FF_FilterParamsType;

#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"
DEM_ARRAY_DECLARE(Dem_Client_FF_FilterParamsType, Dem_Client_FF_FilterParams, DEM_CLIENTID_ARRAYLENGTH_TOTAL);
#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"
#endif
/* ----------------------------------------------------------------------------
   Inline Functions
   ----------------------------------------------------------------------------
*/

DEM_INLINE Dem_boolean_least Dem_EvMemIsDtcKindValid (Dem_DTCKindType DTCKind)
{
    return (DTCKind == DEM_DTC_KIND_ALL_DTCS);
}


DEM_INLINE Dem_boolean_least Dem_EvMemIsDtcOriginValid (Dem_DTCOriginType  *DTCOrigin)
{
    if(*DTCOrigin == DEM_DTC_ORIGIN_USERDEFINED_MEMORY)
    {
        *DTCOrigin = DEM_DTC_ORIGIN_SECONDARY_MEMORY;
    }
    if(*DTCOrigin == DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY)
    {
        *DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
    }
    return (
               (*DTCOrigin == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
#if DEM_CFG_MAX_NUMBER_EVENT_ENTRY_SECONDARY > 0
            || (*DTCOrigin == DEM_DTC_ORIGIN_SECONDARY_MEMORY)
#endif
#if (DEM_CFG_MAX_NUMBER_EVENT_ENTRY_MIRROR > 0) || DEM_CFG_EVMEM_SHADOW_MEMORY_SUPPORTED
            || (*DTCOrigin == DEM_DTC_ORIGIN_MIRROR_MEMORY)
#endif
           );
}

#endif
