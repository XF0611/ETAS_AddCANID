/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */



#ifndef DEM_KL15PERSISTENCE_H
#define DEM_KL15PERSISTENCE_H

#include "Dem_Types.h"

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Increased persistence for diagnosis data in clamp 15 ECUs
 *
 * In clamp 15 ECUs the shutdown phase may not run to completion. But in Dem the data is written to Nvm in the shutdown
 * phase only (except event-related data for event which are marked for immediate storage). This template provides a
 * task which shall be scheduled cyclically and triggers the storage of all blocks that have changed since the last
 * storage if a relevant condition has been detected in the last cycle.
 *
 * For example you could have an event status changed callback that checks if the Pending bit has toggled in the status
 * byte, and if so schedule the storage of the data in the next Dem_KL15PersistenceMain cycle. You can also trigger a
 * storage immediately (for example if a critical situation has been detected or you detect that a power-off is
 * imminent).
 */

/**
 * \brief Cyclic task for the clamp 15 persistence
 *
 * Store all Dem blocks to Nvm if Dem_KL15PersistenceScheduleStorage() has been called at least once since the last
 * cycle.
 *
 * The task should probably be scheduled in long intervals to avoid flash trashing in case of toggling events. This
 * task does not actually store the blocks, but the storage happens in the next Dem_Main cycle. It uses the
 * Dem_TriggerStorageToNvm() interface.
 */
void Dem_KL15PersistenceMain(void);

/**
 * \brief Schedule the storage of all data for the next cycle of the Dem_KL15PersistenceMain task
 */
void Dem_KL15PersistenceScheduleStorage(void);

/**
 * \brief Store all data immediately in the next Dem_Main cycle
 *
 * \return Returns result from Dem_TriggerStorageToNvm()
 */
Std_ReturnType Dem_KL15PersistenceImmediateStorage(void);

/**
 * \brief Checks for a significant change an schedules storage for the next Dem_KL15PersistenceMain cycle
 *
 * This is only an example. Rename, implement, modify or replace as you see fit. Needs to be called by your code or
 * configured as a callback (for example via one of the DemCallbackEventStatusChanged configuration parameters).
 */
DEM_INLINE void Dem_KL15PersistenceCheckEventStatus(Dem_EventIdType EventId,
        Dem_EventStatusType EventStatusOld, Dem_EventStatusType EventStatusNew)
{
    DEM_UNUSED_PARAM(EventId);

    if (((EventStatusOld ^ EventStatusNew) & DEM_UDS_STATUS_PDTC) != 0)
    {
        Dem_KL15PersistenceScheduleStorage();
    }
}

#endif /* DEM_KL15PERSISTENCE_H */
