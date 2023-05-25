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
#error "The Dem_PrjSpecificFailureMemory is a template file, to be used only when the DemRbSupportProjectSpecificFailureMemory is enabled in the project,  \
        This template shall be modified to user's need and this error message can be removed.                                   \
        \
        If the DemRbSupportProjectSpecificFailureMemory parameter is not enabled, Kindly remove this file from project"
*/


#include "Dem_PrjSpecificFailureMemory.h"

#if(DEM_CFG_SUPPORT_PROJECTSPECIFIC_FAILUREMEMORY == TRUE)

/* The project shall provide the Project Specific Failure Memory Data to Dem */
void Dem_ProjectSpecificFailureMemoryCapture(Dem_EventIdType EventId,Dem_EvBuffEventType eventType,DemRbProjectSpecificFailureMemoryType  *PrjFailureMemory)
{
    DEM_UNUSED_PARAM(EventId);
    DEM_UNUSED_PARAM(eventType);
    PrjFailureMemory->SampleData = 0;
       /* Project Specific Implementation */
}


/* This function shall be called during distribution of Events to Event Memory*/
void Dem_ProjectSpecificFailureMemoryReport(const Dem_EvBuffEvent *evBuff, const DemRbProjectSpecificFailureMemoryType  *prjFailureMemory)
{
    DEM_UNUSED_PARAM(*evBuff);
    DEM_UNUSED_PARAM(*prjFailureMemory);
    /* Project Specific Implementation */

}

#endif
