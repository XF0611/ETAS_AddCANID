/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_DTCS_H
#define DEM_CFG_DTCS_H

#include "Std_Types.h"

/* ---------------------------------------- */
/* DEM_CFG_STOREWAITINGFORMONITORINGEVENT     */
/* ---------------------------------------- */
#define DEM_CFG_STOREWAITINGFORMONITORINGEVENT  FALSE

/* ---------------------------------------- */
/* DEM_CFG_DTCFILTER_EXTERNALPROCESSING     */
/* ---------------------------------------- */
#define DEM_CFG_DTCFILTER_EXTERNALPROCESSING_OFF  STD_OFF
#define DEM_CFG_DTCFILTER_EXTERNALPROCESSING_ON   STD_ON
#define DEM_CFG_DTCFILTER_EXTERNALPROCESSING      DEM_CFG_DTCFILTER_EXTERNALPROCESSING_OFF

/* --------------------------------------------------- */
/* DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED    */
/* --------------------------------------------------- */
#define DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED   FALSE

#define DEM_CFG_DTCSTATUS_AVAILABILITYMASK     0xFFu

#define DEM_CFG_DTCSTATEMANGER_DTCS_PER_CYCLE   10u

/* --------------------------------------------------- */
/* DEM DTC STATE BITPOSITION                         */
/* --------------------------------------------------- */

#define DEM_DTC_BP_STATE_SUPPRESSED             0u
#define DEM_DTC_BP_DTCSETTING_DISABLED    1u

typedef uint8 Dem_DtcStateType;
#define DEM_DTCSTATE_ISBITSET                    rba_DiagLib_Bit8IsBitSet
#define DEM_DTCSTATE_OVERWRITEBIT                rba_DiagLib_Bit8OverwriteBit
#define DEM_DTCSTATE_CLEARBIT                    rba_DiagLib_Bit8ClearBit
#define DEM_DTCSTATE_SETBIT                      rba_DiagLib_Bit8SetBit

/**
 * Selects a group of DTCs.
 */

/*                DTCGROUPCODE  */

#define DEM_CFG_DTCGROUPPARAMS \
{ \
    DEM_DTCGROUPS_INIT(0       )/* No DTCGROUP Assigned */ \
,DEM_DTCGROUPS_INIT(DEM_DTC_GROUP_ALL_DTCS) /* DEM_DTC_GROUP_ALL_DTCS */\
}

#define DEM_DTC_FILTER_NUMBER_OF_EVENTS_PER_CYCLE    25u
#define DEM_DTC_FILTER_RETRIEVE_NUMBER_OF_DTCS       50u

#define DEM_NUMBER_OF_DTCS_TO_PROCESS_PER_LOCK    1u

#define DEM_DTC_DTCSETTING_CALLBACK_SUPPORTED_OFF STD_OFF
#define DEM_DTC_DTCSETTING_CALLBACK_SUPPORTED_ON STD_ON

#define DEM_DTC_DTCSETTING_CALLBACK_SUPPORTED DEM_DTC_DTCSETTING_CALLBACK_SUPPORTED_OFF

#endif

