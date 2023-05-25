/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 

 


#ifndef DEM_PRJSPECIFICFAILUREMEMORYTYPE_H
#define DEM_PRJSPECIFICFAILUREMEMORYTYPE_H

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * If DemRbSupportProjectSpecificFailureMemory is enabled, this data structure will be embedded into the event buffer
 * entry. The methods to capture and store the data can be found in
 * \ref Dem_PrjSpecificFailureMemory.h_tpl "Dem_PrjSpecificFailureMemory.h"
 */

#include "Dem_Cfg_EvBuff.h"
#include "Dem_Types.h"


#if(DEM_CFG_SUPPORT_PROJECTSPECIFIC_FAILUREMEMORY == TRUE)

/**
 * \brief Data structure that will be embedded into each event buffer entry.
 *
 * See Dem_PrjSpecificFailureMemoryType.h_tpl. The content is fully customizable, but the name has to be kept.
 */
typedef struct
{
    uint8 SampleData;
}DemRbProjectSpecificFailureMemoryType;


#endif

#endif /* INCLUDE_PROTECTION */
