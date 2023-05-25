


#ifndef DEM_STORAGECONDITION_H
#define DEM_STORAGECONDITION_H



#include "Dem_Cfg_StorageCondition.h"
#include "Dem_Mapping.h"
#include "Dem_Lock.h"
#include "Dem_Helpers.h"
#include "Dem_Cfg_Events.h"





#if (DEM_CFG_STORAGECONDITION == DEM_CFG_STORAGECONDITION_ON)


typedef struct {
   Dem_StoCoList isActive;
   Dem_StoCoList isReplacementEventRequested;
   Dem_StoCoList isReplacementEventStored;

#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
   Dem_EventIdType eventId[DEM_STORAGECONDITION_ARRAYLENGTH];
   /* MR12 RULE 1.2 VIOLATION: The compiler treats the array of length 1 as the final member. But a change of the sequence leads to padding, because different values are based on configuration. This warning can be ignored. */
   Dem_DebugDataType debug1[DEM_STORAGECONDITION_ARRAYLENGTH];
#endif
} Dem_StoCoState;


typedef struct {
   /* MR12 RULE 1.2 VIOLATION: Parameter DEM_STORAGECONDITION_ARRAYLENGTH is generated with different value based on configuration. This warning can be ignored. */
   Dem_EventIdType replacementEvent[DEM_STORAGECONDITION_ARRAYLENGTH];
} Dem_StoCoParam;

#define DEM_START_SEC_VAR_INIT
#include "Dem_MemMap.h"

extern Dem_StoCoState Dem_StoCoAllStates;

#define DEM_STOP_SEC_VAR_INIT
#include "Dem_MemMap.h"


#endif



#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"


/*** INTERNAL FUNCTIONS *******************************************************/

Dem_EventIdType Dem_Stoco_GetReplacementEventID(Dem_StoCoList index);

DEM_INLINE boolean Dem_StoCoAreAllFulfilled(Dem_StoCoList storageConditionList)
{
#if (DEM_CFG_STORAGECONDITION == DEM_CFG_STORAGECONDITION_ON)
   return ((storageConditionList & Dem_StoCoAllStates.isActive) == storageConditionList);
#else
   DEM_UNUSED_PARAM(storageConditionList);
   return TRUE;
#endif
}


#if (DEM_CFG_STORAGECONDITION == DEM_CFG_STORAGECONDITION_ON)


void Dem_StoCoMainFunction(void);
void Dem_StoCoRecheckReplacementStorage(Dem_StoCoList storageConditions);
void Dem_StoCoClearReplacementStoredFlag(void);
void Dem_StoCoSetHasFilteredEvent(Dem_StoCoList storageConditionList
                                             DEM_DEBUGDATA_PARAM(Dem_DebugDataType EventId, Dem_DebugDataType debug1));

#else

DEM_INLINE void Dem_StoCoMainFunction(void) {}
DEM_INLINE void Dem_StoCoRecheckReplacementStorage(uint32 storageConditions) { DEM_UNUSED_PARAM(storageConditions); }
DEM_INLINE void Dem_StoCoSetHasFilteredEvent(Dem_StoCoList storageConditionList
                                             DEM_DEBUGDATA_PARAM(Dem_DebugDataType EventId, Dem_DebugDataType debug1)){

   DEM_UNUSED_PARAM(storageConditionList);
#if (DEM_CFG_DEBUGDATA != DEM_CFG_DEBUGDATA_OFF)
   DEM_UNUSED_PARAM(EventId);
   DEM_UNUSED_PARAM(debug1);
#endif
}

#endif


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"



#endif

