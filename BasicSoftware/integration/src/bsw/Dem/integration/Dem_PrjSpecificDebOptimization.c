/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#include "Dem_PrjSpecificDebOptimization.h"

#if (DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION == DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION_ON)

Dem_boolean_least Dem_PrjSpecificDebOptimizationDone(Dem_EventIdType EventId, Dem_EventStatusType* status)
{
    /* This function will be called in internal function Dem_DebCallFilter where return value of this function would decide
     * whether the functions Dem_DebArTime_Filter or Dem_DebCounterBaseClass_Filter must be called for further calculations or not.
     * If used, the function should always return a boolean value based on whether optimization was performed or not
     * TRUE - If Optimization was performed and functions Dem_DebArTime_Filter or Dem_DebCounterBaseClass_Filter need not be called.
     * FALSE - If Optimization was not performed and functions Dem_DebArTime_Filter or Dem_DebCounterBaseClass_Filter must be called.*/

    Dem_boolean_least returnValue = FALSE;

    /* Reference Implementation*/
    if((*status == DEM_EVENT_STATUS_PREPASSED) || (*status == DEM_EVENT_STATUS_PASSED))
    {
        /* Debounce level is compared with zero, as in case of Clear, the debounce level will get reset to zero irrespective of the previous report*/
        if((Dem_EvtGetLastReportedEvent (EventId) == DEM_EVENT_STATUS_PASSED) && \
                (Dem_EvtGetDebounceLevel (EventId) != 0 ))

        {
#if (DEM_CFG_DEBARTIMEBASE != DEM_CFG_DEBARTIMEBASE_OFF)
            if(Dem_EvtParam_GetDebounceMethodIndex(EventId) != DEM_DEBMETH_IDX_ARTIME)
#endif
            {
                *status = DEM_EVENT_STATUS_PASSED;
            }
            returnValue = TRUE;
        }
    }
    return returnValue;
}
#endif /* DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION == DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION_ON */
