/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_NODEID_H
#define DEM_CFG_NODEID_H

#include "Std_Types.h"

#define DEM_NODEID_INVALID             0
#define DEM_NODEID_COUNT               0u
#define DEM_NODEID_ARRAYLENGTH         (DEM_NODEID_COUNT+1u)

/* define type depends on projectspecific number of nodes */
#if (DEM_NODEID_ARRAYLENGTH <= 255)
typedef uint8 Dem_ComponentIdType;
#else
typedef uint16 Dem_ComponentIdType;
#endif

#endif

