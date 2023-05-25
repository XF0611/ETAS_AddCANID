/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#ifndef DEM_PRJEVENTSTATUS_H
#define DEM_PRJEVENTSTATUS_H

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom immediate aging handling
 *
 * If DemRbProjectSpecificEventStatusHandling is enabled, this method replaces the Dem-internal
 * Dem_EvtSt_HandleImmediateAging()
 */

/**
 * Called when an event is reported as passed, may be aged and the aging threshold is zero.
 *
 * \warning If this project-specific override is used, the status byte will not be updated by the Dem. If that is
 *          needed, it has to be modified in this method too. As an alternative you may want to consider to use
 *          Dem_EvtSt_CustomSetPending() instead, which is called in addition to the Dem-internal behaviour.
 */
DEM_INLINE void Dem_EvtSt_HandleImmediateAging(Dem_EventIdType EventId)
{
    /*Fill with Project-specific implementation for ImmediateAging*/
}


#endif /* INCLUDE_PROTECTION */

