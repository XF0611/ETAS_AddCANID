/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_OPERATIONCYCLE_DATASTRUCTURES_H
#define DEM_CFG_OPERATIONCYCLE_DATASTRUCTURES_H

#include "Dem_Types.h"
#include "Dem_Lib.h"
#include "Dem_Cfg_OperationCycle.h"

typedef struct {
	boolean IsAllowedToBeStartedDirectly;
	Dem_OperationCycleList DependentCycleMask;
} Dem_Cfg_OperationCycleType;

#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"

extern const Dem_Cfg_OperationCycleType Dem_Cfg_OperationCycle[1];

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"

LOCAL_INLINE Dem_OperationCycleList Dem_Cfg_OperationCycle_GetDependentCycleMask(
		Dem_OperationCycleIdType indx) {
	DEM_ASSERT_RANGECHECK(indx < 1);
	return Dem_Cfg_OperationCycle[indx].DependentCycleMask;
}

LOCAL_INLINE boolean Dem_Cfg_OperationCycle_GetIsAllowedToBeStartedDirectly(
		Dem_OperationCycleIdType indx) {
	DEM_ASSERT_RANGECHECK(indx < 1);
	return Dem_Cfg_OperationCycle[indx].IsAllowedToBeStartedDirectly;
}

#endif
