/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
#if (DEM_CFG_EVMEM_PROJECT_EXTENSION)


/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * See Dem_PrjEvmemProjectExtension.h_tpl for documentation
 */

#include "Dem_Types.h"
#include "Dem_PrjEvmemProjectExtension.h"
#include "Dem_EvMemBase.h"


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

void Dem_EvMemProjectExtensionInit (void)
{
    /* Please fill your implementation here if required! */
}

void Dem_EvMemProjectExtensionMain(void)
{
    /* Please fill your implementation here if required! */
}

void Dem_EvMemProjectExtensionUnRobust(Dem_EventIdType EventId, uint16_least LocId, uint16_least *writeSts)
{
    /* Please fill your implementation here if required! */
}

void Dem_EvMemProjectExtensionFailed(Dem_EventIdType EventId, uint16_least LocId, uint16_least *writeSts)
{
    /* Please fill your implementation here if required! */
}

void Dem_EvMemProjectExtensionStartOpCycle(uint16_least LocId, uint16_least *writeSts)
{
    /* Please fill your implementation here if required! */
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
