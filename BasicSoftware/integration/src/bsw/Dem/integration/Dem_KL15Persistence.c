/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */



#include "Dem_KL15Persistence.h"

#include "Dem.h"

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * See Dem_KL15Persistence.h_tpl for documentation
 */

/* Internal flag. Please use the KL15Persistence functions instead of setting it directly.
 * Will be automatically initialized to 0. */
static boolean Dem_Prv_KL15PersistenceIsStorageScheduled;

void Dem_KL15PersistenceMain(void)
{
    if (Dem_Prv_KL15PersistenceIsStorageScheduled)
    {
        Dem_Prv_KL15PersistenceIsStorageScheduled = FALSE;
        #if(DEM_CFG_TRIGGER_TO_STORE_NVM_SUPPORTED)
        (void)Dem_TriggerStorageToNvm();
        #endif
    }
}

void Dem_KL15PersistenceScheduleStorage(void)
{
    Dem_Prv_KL15PersistenceIsStorageScheduled = TRUE;
}

Std_ReturnType Dem_KL15PersistenceImmediateStorage(void)
{
    Dem_Prv_KL15PersistenceIsStorageScheduled = FALSE;
    #if(DEM_CFG_TRIGGER_TO_STORE_NVM_SUPPORTED)
    return Dem_TriggerStorageToNvm();
    #else
    return E_OK;
    #endif
    
}
