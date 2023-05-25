/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */


 


#ifndef DEM_PRJSPECIFICFAILUREMEMORY_H
#define DEM_PRJSPECIFICFAILUREMEMORY_H

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom extension of the event buffer entries
 *
 * If DemRbSupportProjectSpecificFailureMemory is enabled, these methods will be used to capture the project-specific
 * data. The data structure for capture of the data in the event buffer is defined in
 * \ref Dem_PrjSpecificFailureMemoryType.h_tpl "Dem_PrjSpecificFailureMemoryType.h"
 */
/*
#error "The Dem_PrjSpecificFailureMemory is a template file, to be used only when the DemRbSupportProjectSpecificFailureMemory is enabled in the project,  \
        This template shall be modified to user's need and this error message can be removed.                                   \
        \
        If the DemRbSupportProjectSpecificFailureMemory parameter is not enabled, Kindly remove this file from project"
*/

#include "Dem_EvBuffEvent.h"


#if(DEM_CFG_SUPPORT_PROJECTSPECIFIC_FAILUREMEMORY == TRUE)
/**
 * \brief Capture the project-specific data
 *
 * Called when the entry is inserted into the event buffer. Should capture the data and store it in the custom data
 * structure.
 *
 * \param      EventId           The ID of the event
 * \param      eventType         The reason why the event is inserted into the event buffer. See C_EVENTTYPE_*
 * \param[out] PrjFailureMemory  The custom data structure in the event buffer entry where the captured data should be
 *                               stored.
 */
void Dem_ProjectSpecificFailureMemoryCapture(Dem_EventIdType EventId,Dem_EvBuffEventType eventType,DemRbProjectSpecificFailureMemoryType  *PrjFailureMemory);

/**
 * \brief Process the project-specific data
 *
 * Called when the entry is removed from the event buffer and distributed to the event memory (not necessarily stored
 * there). This method should process the previously captured data (e.g. store it in the project-specific event memory
 * extension. See Dem_PrjEvmemProjectExtension.h_tpl).
 *
 * \param[in] evBuff  Event buffer entry that contains the data.
 */
void Dem_ProjectSpecificFailureMemoryReport(const Dem_EvBuffEvent *evBuff, const DemRbProjectSpecificFailureMemoryType  *PrjFailureMemory);

#endif

#endif /* INCLUDE_PROTECTION */
