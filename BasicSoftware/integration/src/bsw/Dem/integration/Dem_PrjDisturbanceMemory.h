/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#ifndef DEM_PRJ_DISTURBANCEMEMORY_H
#define DEM_PRJ_DISTURBANCEMEMORY_H

#include "Dem_cfg_DistMem.h"
#include "Dem_Types.h"


#if(DEM_CFG_DIST_MEM_CALLBACK_ON_DISTURBANCE)


typedef struct
{
    /*a project specific data that is required to be stored in addition to the default disturbance memory data. For e.g the data stored during disturbances*/
    uint8 projspecData1;

} Dem_DistMemPrjDataType;
/*size in bytes of the elements returned by function Dem_GetDistMemPrjData*/
#define DEM_CFG_DIST_MEM_PRJ_DATA_SIZE 1u


/*function used to read the data from disturbance memory e.g. for Dcm service*/
/*The project has to take care of typecasting and convertion where ever it is required*/
void Dem_GetNextDisturbanceMemProjData(uint8 distIdx, uint8* DestBuffer);

/* Project specific Callback*/
void  Dem_PrjDistMemCallbackOnDisturbance(Dem_EventIdType EventId);



#endif
#endif
