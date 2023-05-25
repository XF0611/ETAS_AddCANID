


#include "Dem_Internal.h"
#include "Rte_Dem.h" /* For the callbacks */

#include "Dem_DebBase.h"

#include "Dem_DebSharing.h"
#include "Dem_DebArTime.h"
#include "Dem_DebCtrBaseClass.h"
#include "Dem_DebMonInternal.h"
#include "Dem_Cfg_ExtPrototypes.h"
#include "Dem_Lib.h"


#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"
DEM_CFG_DEB_DEFINE_ALL_PARAMSETS
#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"



#if ( (DEM_CFG_DEBCOUNTERBASECLASS == DEM_CFG_DEBCOUNTERBASECLASS_ON) && (!defined DEM_DEB_FORCE_CONST_CONFIG) )
#define DEM_START_SEC_VAR_INIT
#include "Dem_MemMap.h"
/* MR12 DIR 1.1 VIOLATION: a fast and resource optimized support of different debounce startegies is only possible through this mechanism */
Dem_DebClass Dem_Cfg_DebClasses[DEM_CFG_DEB_NUMBEROFCLASSES] = DEM_CFG_DEB_CLASSES;
#define DEM_STOP_SEC_VAR_INIT
#include "Dem_MemMap.h"
#else
#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"
const Dem_DebClass Dem_Cfg_DebClasses[DEM_CFG_DEB_NUMBEROFCLASSES] = DEM_CFG_DEB_CLASSES;
#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"
#endif



#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
void Dem_DebSwitchToAlternativeParameters(void)
{
#if ( (DEM_CFG_DEBCOUNTERBASECLASS == DEM_CFG_DEBCOUNTERBASECLASS_ON) && (!defined DEM_DEB_FORCE_CONST_CONFIG) )
    /* MR12 DIR 1.1 VIOLATION: a fast and resource optimized support of different debounce startegies is only possible through this mechanism */
    Dem_Cfg_DebClasses[DEM_DEBMETH_IDX_ARCTRBASECLASS].paramSet = &Dem_Cfg_DebCounterBaseClass_Paramsets[DemConf_DemRbDebounceCounterSet_Alternative];
#endif
}


Std_ReturnType Dem_DebSwitchToParameters(uint8 CounterParamSetId)
{
    Std_ReturnType ReturnValue = E_NOT_OK;

#if ( ((DEM_CFG_DEBCOUNTERBASECLASS == DEM_CFG_DEBCOUNTERBASECLASS_ON) && (DEMRB_DEBOUNCECOUNTERSETS_SIZE != 0))\
    && (!defined DEM_DEB_FORCE_CONST_CONFIG) )

   if(CounterParamSetId < DEMRB_DEBOUNCECOUNTERSETS_SIZE)
   {
        /* MR12 DIR 1.1 VIOLATION: A fast and resource optimized support of different debounce startegies is only possible through this mechanism */
        Dem_Cfg_DebClasses[DEM_DEBMETH_IDX_ARCTRBASECLASS].paramSet = &Dem_Cfg_DebCounterBaseClass_Paramsets[CounterParamSetId];
        ReturnValue = E_OK;
   }
   else
   {
        /* To avoid warning */
   }
#endif

   return ReturnValue;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
