/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */



#include "Dem.h"
#include "Rte_Dem.h"

#include "Dem_PrjDisturbanceMemory.h"

#include "Dem_Deb.h"
#include "Dem_Events.h"
#include "Dem_DisturbanceMemory.h"
#include "Dem_EventStatus.h"

/* Please instantiate as many inline functions for as many data elements added in Dem_DistMemPrjDataType */

/* Please fill your implementation here if required the data to be read and stored on disturbances shall be implemeanted here. */


#if(DEM_CFG_DIST_MEM_CALLBACK_ON_DISTURBANCE)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/*Helper INLINE functions*/

DEM_INLINE Dem_DistMemPrjDataType* Dem_GetPrjDataDisturbanceMemory_BasePtr(uint8 RecIdx)
{
    return (&Dem_DistMemLocations[RecIdx].distMemPrjData);
}

DEM_INLINE uint8 Dem_GetPrjDataDisturbanceMemory_projspecData1(uint8 RecIdx)
{

    return (Dem_GetPrjDataDisturbanceMemory_BasePtr(RecIdx)->projspecData1);
}

DEM_INLINE uint8 Dem_SetPrjDataDisturbanceMemory_projspecData1(uint8 RecIdx , uint8 setVal)
{

    Dem_GetPrjDataDisturbanceMemory_BasePtr(RecIdx)->projspecData1 = setVal;
}

/* the function is called from Dem_GetNextDisturbanceData(), when the Disturbance memory is read*/
/*Note: Take care if project data is greater than uint8, the data must be splitted and updated to DestBuffer */
void Dem_GetNextDisturbanceMemProjData(uint8 distIdx, uint8* DestBuffer)
{
    /*example implementation*/
   *DestBuffer = Dem_GetPrjDataDisturbanceMemory_projspecData1(distIdx);
    DestBuffer++;

}


/* Callback is called when the event is getting disturbed */
void  Dem_PrjDistMemCallbackOnDisturbance(Dem_EventIdType EventId)

{
    /*Project specific implenetation*/
    /*For eg.:Dem_SetPrjDataDisturbanceMemory_MyData(Dem_EvtGetEventsDistMemIndex(EventId) , Dem_DebCalculateFaultDetectionCounter(EventId, DebounceLevel) ); */
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

