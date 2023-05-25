/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#ifndef DEM_PRJSPECIFICDEBOPTIMIZATION_H
#define DEM_PRJSPECIFICDEBOPTIMIZATION_H

#include "Dem.h"
#include "Dem_Events.h"

#if (DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION == DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION_ON)
/**
 * \file
 * \ingroup DEM_PRJ_TPL
 *
 * If DemRbProjectSpecificDebOptimization is enabled, then a callout-point will be enabled and
 * the callout function has to be defined. And it must return a boolean value.
 * Refer documentation in the header file for more details.
 */

Dem_boolean_least Dem_PrjSpecificDebOptimizationDone(Dem_EventIdType EventId, Dem_EventStatusType* status);

#endif /* DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION == DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION_ON */

#endif /* INCLUDE_PROTECTION */
