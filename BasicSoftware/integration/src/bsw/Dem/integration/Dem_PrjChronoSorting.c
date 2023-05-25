/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * See Dem_ChronoSorting.h for prototype declarations
 */

#include "Dem_ChronoSort.h"
#include "Dem_PrjEvmemProjectExtension.h"
#include "Dem_EvMem.h"
#include "Dem_Dtcs.h"

#if (DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED)

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

boolean Dem_IsChronoSortEnabled(uint8 ClientId)
{
    /* Please fill your implementation here if required! */
    return FALSE; /* Dummy return value provided */
}

/**
 * Not called from Dem. Intended to be called internally from the project-specific code to get a new counter value for
 * the specified event memory which is then used for chronological sorting.
 */
static uint32 Dem_ChronoSortGetNewEvMemSortId(uint16_least MemId)
{
    /* Please fill your implementation here if required! */
}

void Dem_ChronoSortEvMemUpdateSortId(uint16_least LocId,uint16_least MemId,uint16_least StatusOld,uint16_least StatusNew,uint16_least *WriteSts)
{
    /* Please fill your implementation here if required! */
}

void Dem_ChronoSortDTCFilterInit(uint8 ClientId)
{
    /* Please fill your implementation here if required! */
}

boolean Dem_ChronoSortSetDTCFilter(Dem_DTCFilterState* dtcFilter_p)
{
    /* Please fill your implementation here if required! */
    return FALSE; /* Dummy return value provided */
}

void Dem_ChronoSortMainFunction(Dem_DTCFilterState* dtcFilter_p)
{
    /* Please fill your implementation here if required! */
}

Std_ReturnType Dem_ChronoSortGetNextFilteredDTCID (uint8 ClientId, Dem_DtcIdType* dtcId, uint32* DTC, uint8* DTCStatus)
{
    /* Please fill your implementation here if required! */
}

Std_ReturnType Dem_ChronoSortGetNumberOfFilteredDTC(uint8 ClientId, uint16* NumberOfFilteredDTC)
{
    /* Please fill your implementation here if required! */
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
