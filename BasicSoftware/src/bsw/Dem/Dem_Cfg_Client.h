
/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_CLIENT_H
#define DEM_CFG_CLIENT_H

#include "Std_Types.h"
#include "Dem_ClientHandlingTypes.h"

/* ---------------------------------------- */
/* DEM_CFG_CLIENT_BEHAVIOR               */
/* ---------------------------------------- */

/*Number of Clients configured*/
#define DEM_CLIENTS_COUNT_STANDARD    1u
#define DEM_CLIENTID_ARRAYLENGTH_STD      (DEM_CLIENTS_COUNT_STANDARD+1u)

/*ClientId Array length including J1939 client*/
#define DEM_CLIENTID_ARRAYLENGTH_TOTAL      (DEM_CLIENTID_ARRAYLENGTH_STD+1u)

/*Symbolic names of the clients and corresponding ClientIds*/
#define DemConf_DemClient_DemClient     1u

/* J1939Dcm is considered as the client with the least priority and hence the client ID 
 * for the same is given as the last  ClientId i.e. number of clients configured in DemGeneral+1. 
 * This will be updated when J1939 related changes are done as per AR4.3.1 */

#define DEM_CLIENTID_J1939 (DEM_CLIENTS_COUNT_STANDARD+1u)

/* ----------------------------------------
 Invalid ClientId
 ---------------------------------------- */
#define DEM_CLIENTID_INVALID  0x00

/* This structure will be updated/extended for the other elements DemClientFunctionality, DemClientUsesRte 
 * of DemClient container in future.
 * Remark: The structure should be generated with the data structure generator from the diaglib */

#define DEM_CFG_CLIENT_NUMBER_OF_CLIENTS_USING_RTE 0u

#endif

