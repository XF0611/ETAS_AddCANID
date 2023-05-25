/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 

 

#ifndef DEM_PRJERASEALLNVMDATAHANDLING_H
#define DEM_PRJERASEALLNVMDATAHANDLING_H

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Erasure of NvM data on Dem_Init() if data semantic has changed
 *
 * If DemRbEraseAllNvMDataSupported is enabled, this function decides whether all data persisted in the NvM should
 * be erased on Dem_Init().
 */

#include "Dem_Types.h"
/**
 * \brief This method is called in Dem_Init() and decides which part of the NvM should be erased
 *
 * When there is a significant change in the Dem configuration, the entries stored in the event memory might become
 * inconsistent and if events are renumbered, status byte information could be assigned to the wrong events. Therefore
 * it should be erased whenever such a change occurs. This function should run some detection routine and then return
 * which data shall be erased.
 * CAUTION: If this interface Dem_PrjEraseAllNvMDataHandling returns DEM_ERASE_ALL_REQUEST_ALL,then the OBD permanent
 * memory will also be cleared.THIS FEATURE SHOULD ONLY BE USED AFTER FLASHING NEW SOFTWARE OR AT END OF PRODUCTION.
 * NEVER CALL THIS FUNCTION FOR OTHER PURPOSES!!! OTHERWISE OBD CONFORMITY IS VIOLATED.
 *
 * \param HashIdCheckResult Whether the Dem configuration has changed in some significant way.
 *
 *                      DEM_HASH_ID_CHECK_DISABLED - Hash Id check is disabled in configuration
 *                      DEM_HASH_ID_CHECK_NOT_POSSIBLE - Check not possible as Nvm Read for the HashId block failed
 *                      DEM_HASH_ID_NO_CHANGE - No change in HashId
 *                      DEM_HASH_ID_MISMATCH - HashId is changed (Change in Order of Events or Number of Events
 *                                             or Event ID or Event Shortname is detected)
 *
 * \return              Which part of the NvM should be deleted:
 *
 *                      - DEM_ERASE_ALL_REQUEST_NONE: Do not delete anything
 *                      - DEM_ERASE_ALL_REQUEST_ALL: Erase all Dem NvM blocks
 */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
Dem_EraseAllRequestType Dem_PrjEraseAllNvMDataHandling(Dem_HashIdCheckResultType HashIdCheckResult);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
