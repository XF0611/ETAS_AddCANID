

#ifndef DEM_OPERATIONCYCLE_H
#define DEM_OPERATIONCYCLE_H

#include "Dem_Cfg_OperationCycle.h"
#include "Dem_Cfg_OperationCycle_DataStructures.h"
#include "Dem_Types.h"

#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

extern Dem_OperationCycleList Dem_OperationCycleStates;

#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

DEM_INLINE boolean Dem_IsOperationCycleStarted(Dem_OperationCycleIdType OperationCycleId)
{
    return DEM_OPERATIONCYCLE_ISBITSET(Dem_OperationCycleStates, OperationCycleId);
}

void Dem_OperationCycleInit(void);
void Dem_OperationCycleInitCheckNvm(void);
boolean Dem_OperationCyclesMainFunction(void);
Std_ReturnType Dem_ResetCycleQualified(Dem_OperationCycleIdType OperationCycleId);

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif

