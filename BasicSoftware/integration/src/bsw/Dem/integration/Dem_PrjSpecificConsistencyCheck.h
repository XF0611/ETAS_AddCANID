/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
#ifndef DEM_PRJSPECIFICCONSISTENCYCHECK_H
#define DEM_PRJSPECIFICCONSISTENCYCHECK_H

#include "Dem_EvBuffEvent.h"
/*
#error "The Dem_PrjSpecificConistencyCheck is a template file, to be used only when the DemRbSupportProjectSpecificConsistencyCheck is enabled in the project,  \
        This template shall be modified to user's need and this error message can be removed.                                   \
        \
        If the  parameter is not enabled, Kindly remove this file from project"
*/
/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom Consistency Check during Init
 *
 * \See Dem_PrjSpecificConsistencyCheck.h_tpl for documentation
 */
#if(DEM_CFG_SUPPORT_PROJECTSPECIFIC_CONSISTENCYCHECK == DEM_CFG_CONSISTENCY_CHECK_ON)

/**
 * \brief Any Project Specific Consistency Checks
 *
 * Called at the start of Dem_Init to allow any project specific consistency checks
 * Implementation of the checks need to be present in the function definition.
 *
 */
void Dem_ProjectSpecificConsistencyCheck();

#endif

#endif  /* INCLUDE_PROTECTION */
