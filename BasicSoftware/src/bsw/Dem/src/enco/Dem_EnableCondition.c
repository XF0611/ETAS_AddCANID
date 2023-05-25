

#include "Dem_Internal.h"
#include "Rte_Dem.h"

#include "Dem_Events.h"
#include "Dem_EnableCondition.h"
#include "Dem_Lock.h"
#include "Dem_Prv_Det.h"
#include "Dem_Cfg_Events_DataStructures.h"

#define DEM_START_SEC_VAR_INIT
#include "Dem_MemMap.h"

#if (DEM_CFG_ENABLECONDITION == DEM_CFG_ENABLECONDITION_ON)
Dem_EnCoState Dem_EnCoAllStates = { DEM_CFG_ENCO_INITIALSTATE };
static Dem_EnCoList Dem_EnCoReEnabledBitMask = 0;
#endif

#define DEM_STOP_SEC_VAR_INIT
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
Std_ReturnType Dem_SetEnableCondition (uint8 EnableConditionID,
                                       boolean ConditionFulfilled)
{
#if (DEM_CFG_ENABLECONDITION == DEM_CFG_ENABLECONDITION_ON)

    Dem_EnCoList enableConditionBitmask = (1u << EnableConditionID);
    Dem_boolean_least isEnCoAlreadyFulfilled;

    if (EnableConditionID >=  DEM_ENABLECONDITION_COUNT)
    {
        DEM_DET(DEM_DET_APIID_SETENABLECONDITION, DEM_E_WRONG_CONFIGURATION,0u);
        DEM_ASSERT_ISNOTLOCKED();
        return E_NOT_OK;
    }

    DEM_ENTERLOCK_MON();

    if (ConditionFulfilled)
    {
        isEnCoAlreadyFulfilled = Dem_EnCoFulfilled (enableConditionBitmask);

        if(!isEnCoAlreadyFulfilled)
        {
            Dem_EnCoReEnabledBitMask |= enableConditionBitmask;
        }
        Dem_EnCoAllStates.isActive |= enableConditionBitmask;
    }
    else
    {
        Dem_EnCoAllStates.isActive &= (~enableConditionBitmask);
    }

    DEM_EXITLOCK_MON();

    return E_OK;

#else
    DEM_UNUSED_PARAM(EnableConditionID);
    DEM_UNUSED_PARAM(ConditionFulfilled);
    return E_NOT_OK;
#endif
}

/* MR12 RULE 8.13 VIOLATION: The pointer will be modified based on the configuration. */
Std_ReturnType Dem_GetEnableCondition (uint8 EnableConditionID,
                                       boolean* ConditionFulfilled)
{
#if (DEM_CFG_ENABLECONDITION == DEM_CFG_ENABLECONDITION_ON)

    Dem_EnCoList enableConditionBitmask;

    /* Entry Condition Check */
    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(ConditionFulfilled,DEM_DET_APIID_GETENABLECONDITION,E_NOT_OK);
    if (EnableConditionID >=  DEM_ENABLECONDITION_COUNT)
    {
        DEM_DET(DEM_DET_APIID_GETENABLECONDITION, DEM_E_WRONG_CONFIGURATION,0u);
        return E_NOT_OK;
    }

    enableConditionBitmask = (1u << EnableConditionID);

    *ConditionFulfilled = Dem_EnCoFulfilled(enableConditionBitmask);

    return E_OK;

#else
   DEM_UNUSED_PARAM(EnableConditionID);
   DEM_UNUSED_PARAM(ConditionFulfilled);
   return E_NOT_OK;
#endif
}

#if (DEM_CFG_ENABLECONDITION == DEM_CFG_ENABLECONDITION_ON)
void Dem_EnCoCallbacks(void)
{
    Dem_EventIdIterator eventIt;
    Dem_EventIdType eventId;
    Dem_EnCoList enableCondition;
    Dem_EnCoList currentEnCoReEnabledBitMask;

    if(Dem_EnCoReEnabledBitMask != 0)
    {
        DEM_ENTERLOCK_MON();

        currentEnCoReEnabledBitMask = Dem_EnCoReEnabledBitMask;
        Dem_EnCoReEnabledBitMask = 0;

        DEM_EXITLOCK_MON();

        for (Dem_EventIdIteratorNew(&eventIt); Dem_EventIdIteratorIsValid(&eventIt); Dem_EventIdIteratorNext(&eventIt))
        {
            eventId = Dem_EventIdIteratorCurrent(&eventIt);
            enableCondition = Dem_EvtParam_GetEnableConditions(eventId);

            /** Ensure Enable condition is Valid and currentEnCoReEnabledBitMask is in the Enablecondition List **/
            if((enableCondition != 0) && ((currentEnCoReEnabledBitMask & enableCondition) != 0))
            {
                if(Dem_EnCoAreAllFulfilled(enableCondition))
                {
                    DEM_ENTERLOCK_MON();
                    Dem_EvtSetInitMonitoring (eventId, DEM_INIT_MONITOR_REENABLED);
                    DEM_EXITLOCK_MON();
                }
            }
        }
    }
}
#endif
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
