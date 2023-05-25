/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */




#ifndef DEM_PRJEVTPROJECTEXTENSION_H
#define DEM_PRJEVTPROJECTEXTENSION_H

/* BSW-8482 */
/* The following inclusion is for increasing IUMPR ratio numerators more than one in operation cycle if users have permission for it.
 * Un-comment the code for using it
*/
//#include "rba_DemObdBasic_Iumpr.h"
//#include "rba_DemObdBasic_Ratio.h"
//#include "rba_DemObdBasic.h"
//#include "Dem_Events.h"
/* END BSW-8482 */

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom functionality on every event status report
 *
 * If DemRbEvtProjectExtension is enabled, this method will be called on every event status report
 /* BSW-8482 */
 * The sample implementation for increasing IUMPR EVAP group numerator by one more value is provided as commented code for reference
 /* END BSW-8482 */
 */

/**
 * \brief Called whenever Dem_SetEventStatus(), Dem_ReportErrorStatus() or their related methods are called.
 *
 * Called after parameters are validated and debouncing is handled.
 *
 * \param EventId Identification of event by ID
 * \param EventStatus  The reported status of the event (passed, failed, pre-passed, etc.) after debouncing is handled.
 *                     That means that a reported pre-failed could be qualified to failed in the debouncing and then
 *                     this parameter would be set to failed
 * \param debAction    The debounce action that was taken (see DEM_DEBACTION_*)
 */
DEM_INLINE void Dem_EvtProjectExtensionSetEventStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus, uint8_least debAction)
{
    /* Do something very short here!!!
     * This function is called with every setEventStatus (PASSED/FAILED, PREFAILED/PREPASSED, ...).
     */

/* BSW-8482 */
/* Uncomment the following code for using it if needed */
//#if DEM_CFG_OBD_IUMPR
//   uint8 idxRatio; /* Ratio iterator */
//   Dem_EventIdType EventIDRef;
//   uint16 Numerator;
//   uint16 Denominator;
//   uint8 Group;
//   uint8 RatioStatus;

//    /* Loop through the RatioIds to find the relevant one, which is referred to the active DemEvent.*/
//   for (idxRatio = 0u; idxRatio < DEM_CFG_NUM_ALL_IUMPR_RATIOS; idxRatio++)
//   {
//      Dem_GetIUMPRRatioData(idxRatio, &Numerator, &Denominator, &EventIDRef, &Group);
//      if ((EventId == EventIDRef) && (Group == DEM_OBDIUMPR_GRP_EVAP)){
//         break;
//      }
//   }

//    /* Increase one more value for numerator of EVAP ratio group*/
//   if (idxRatio < DEM_CFG_NUM_ALL_IUMPR_RATIOS){
//      RatioStatus = rba_DemObdBasic_Iumpr_RatioSt(idxRatio);
//      /* If not prevented from further increment in this cycle, due to already it has incremented or is locked by fault */
//      if ((RatioStatus & (DEM_RATIO_STSMASK_NUMER_INCD | DEM_RATIO_STSMASK_NUMER_LOCKED)) == 0u && !Dem_EvtIsSuppressed(EventIDRef)){
//         rba_DemObdBasic_Iumpr_RatioNum(idxRatio)++;
//      }
//   } else {
//      /*The EventId is not related to EVAP ratio group, do nothing.*/
//   }
//#endif
/* END BSW-8482 */
    /* also consider using locks in case of multitasking */
}

/* BSW-8482 */
/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom functionality on every Fault Detect reported for corresponding Ratio from ApplicationSoftware
 *
 * If DemRbEvtProjectExtension is enabled, this method will be called on every Fault Detect report
 * The sample implementation for increasing IUMPR EVAP group numerator by one more value is provided as commented code for reference
 */

/**
 * \brief Called whenever Dem_RepIUMPRFaultDetect are called.
 *
 * Called after parameters are validated, the ratio is of DEM_RATIO_API kind and relevant DemEvent is not suppressed
 *
 * \param RatioID  Ratio of the currently considered ratio record
 */
DEM_INLINE void Dem_ProjectExtensionRepIUMPRFaultDetect(Dem_RatioIdType RatioID)
{
/* Uncomment the following code for using it if needed */
//#if DEM_CFG_OBD_IUMPR
//   Dem_EventIdType EventIDRef;
//   uint16 Numerator;
//   uint16 Denominator;
//   uint8 Group;
//   uint8 RatioStatus;

//   Dem_GetIUMPRRatioData(RatioID, &Numerator, &Denominator, &EventIDRef, &Group);

//   /* Increase one more value for EVAP ratio group */
//   if ((RatioID < DEM_CFG_NUM_ALL_IUMPR_RATIOS) && (Group == DEM_OBDIUMPR_GRP_EVAP)){
//      RatioStatus = rba_DemObdBasic_Iumpr_RatioSt(RatioID);
//      /* If not prevented from further increment in this cycle, due to already it has incremented or is locked by fault */
//      if (((RatioStatus & (DEM_RATIO_STSMASK_NUMER_INCD | DEM_RATIO_STSMASK_NUMER_LOCKED)) == 0u) && (!Dem_EvtIsSuppressed(EventIDRef))){
//         rba_DemObdBasic_Iumpr_RatioNum(RatioID)++;
//      }
//   } else {
//   /* The EventId is not related to EVAP ratio group, do nothing. */
//   }
//#endif

}
/* END BSW-8482 */
#endif /* INCLUDE_PROTECTION */
