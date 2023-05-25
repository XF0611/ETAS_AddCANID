/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */



/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * See Dem_PrjSpecificIndiHandling.h_tpl for documentation
 */

#include "Dem_EventStatus.h"



Std_ReturnType Dem_GetIndicatorStatus(uint8 IndicatorId, Dem_IndicatorStatusType* IndicatorStatus)
{
  return E_NOT_OK;
}

// void Dem_IndicatorAttributeMainFunction(void)
// {
//     /* Project Specific Implementation */
// }

// void Dem_IndicatorAttributeInit(void)
// {
//     /* Project Specific Implementation */
// }

// void Dem_SetIndicatorDeActivation(Dem_EventIdType EventId, Dem_UdsStatusByteType isoByteOld, Dem_UdsStatusByteType isoByteNew)
// {
//     /* Project Specific Implementation */
// }

// void Dem_SetIndicatorActivation(Dem_EventIdType EventId, Dem_UdsStatusByteType isoByteOld, Dem_UdsStatusByteType isoByteNew)
// {
//     /* Project Specific Implementation */
// }

// void Dem_IndicatorAttribute_ConsistencyCheck(Dem_EventIdType EventId, uint16_least EventStatus )
// {
//     /* Project Specific Implementation */
// }


// Std_ReturnType Dem_SetWIRStatus (
//         Dem_EventIdType EventId,
//         boolean WIRStatus
// )
// {
//     return E_NOT_OK;
// }



