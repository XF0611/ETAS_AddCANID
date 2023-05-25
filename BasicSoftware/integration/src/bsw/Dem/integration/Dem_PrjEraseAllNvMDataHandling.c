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
 * See Dem_PrjEraseAllNvMDataHandling.h_tpl for documentation
 */

#include "Dem_PrjEraseAllNvMDataHandling.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"


Dem_EraseAllRequestType Dem_PrjEraseAllNvMDataHandling(Dem_HashIdCheckResultType HashIdCheckResult)
{
    Dem_EraseAllRequestType eraseAll = DEM_ERASE_ALL_REQUEST_NONE;

    /* ProjectSpecific logic for executing clear all
     * Just an Example implementation given here */

    if(HashIdCheckResult == DEM_HASH_ID_MISMATCH)
    {
        eraseAll = DEM_ERASE_ALL_REQUEST_ALL;
    }

    return eraseAll;
}


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

