/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */


/*
#error "The Dem_PrjSpecificConistencyCheck is a template file, to be used only when the DemRbSupportProjectSpecificConsistencyCheck is enabled in the project,  \
        This template shall be modified to user's need and this error message can be removed.                                   \
        \
        If the  parameter is not enabled, Kindly remove this file from project"
*/


#include "Dem_PrjSpecificConsistencyCheck.h"

#if(DEM_CFG_SUPPORT_PROJECTSPECIFIC_CONSISTENCYCHECK == DEM_CFG_CONSISTENCY_CHECK_ON)

void Dem_ProjectSpecificConsistencyCheck()
{
    /* Project Specific Implementation */
}

#endif  /* INCLUDE_PROTECTION */
