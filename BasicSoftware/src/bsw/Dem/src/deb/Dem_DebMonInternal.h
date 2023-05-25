

#ifndef DEM_DEBMONINTERNAL_H
#define DEM_DEBMONINTERNAL_H


#include "Dem_Types.h"
#include "Dem_Events.h"

typedef Std_ReturnType(*Dem_GetFaultDetectionCounterOfIntDebMonitors)(sint8 * faultDetCtr);


typedef struct
{
  /* Pointer to DemCallBackGetFaultDetectionCounter Function */
    Dem_GetFaultDetectionCounterOfIntDebMonitors funcPointer_GetFDC;
}Dem_DebounceMonitorInternal;

#if (DEM_CFG_DEBMONINTERNAL == DEM_CFG_DEBMONINTERNAL_ON)
DEM_INLINE uint8_least Dem_DebMonInternal(Dem_EventStatusType* status)
{
    uint8_least debAction = DEM_DEBACTION_NOOP;

         DEM_ASSERT (*status != DEM_EVENT_STATUS_PREPASSED, DEM_DET_APIID_DEBCALLFILTER, 0);
         DEM_ASSERT (*status != DEM_EVENT_STATUS_PREFAILED, DEM_DET_APIID_DEBCALLFILTER, 1);

 #if(DEM_CFG_SUPPORT_EVENT_FDCTHRESHOLDREACHED)
         if((*status == DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED) || (*status == DEM_EVENT_STATUS_FAILED))
         {
             debAction |= DEM_DEBACTION_SETFDCTHRESHOLDREACHED;
         }
         else if(*status == DEM_EVENT_STATUS_PASSED)
         {
             debAction |= DEM_DEBACTION_RESETFDCTHRESHOLDREACHED;
         }
         else
         {
             /* do nothing */
         }
 #endif
 #if(DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD == DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD_ON)
         if(*status == DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED)
         {
             debAction |= DEM_DEBACTION_ALLOW_BUFFER_INSERT|DEM_DEBACTION_SETFDCTHRESHOLDREACHED;
         }
 #endif

    return debAction;
}
#endif

#endif

