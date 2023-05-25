/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_EVENTS_H
#define DEM_CFG_EVENTS_H

#include "Dem_Cfg_EventId.h"

/* ---------------------------------------- */
/* DEM_CFG_EVTDISABLESTATUSUPDATE           */
/* ---------------------------------------- */
#define DEM_CFG_EVTDISABLESTATUSUPDATE_OFF  STD_OFF
#define DEM_CFG_EVTDISABLESTATUSUPDATE_ON   STD_ON
#define DEM_CFG_EVTDISABLESTATUSUPDATE  DEM_CFG_EVTDISABLESTATUSUPDATE_OFF

/* ---------------------------------------- */
/* DEM_CFG_TRIGGERFIMREPORTS                */
/* ---------------------------------------- */
#define DEM_CFG_TRIGGERFIMREPORTS_OFF  STD_OFF
#define DEM_CFG_TRIGGERFIMREPORTS_ON   STD_ON
#define DEM_CFG_TRIGGERFIMREPORTS  DEM_CFG_TRIGGERFIMREPORTS_OFF

#define DEM_CFG_FIM_OFF STD_OFF
#define DEM_CFG_FIM_ON  STD_ON
#define DEM_CFG_FIM_USED DEM_CFG_FIM_ON
/* ---------------------------------------- */
/* DEM_CFG_TRIGGERDLTREPORTS                */
/* ---------------------------------------- */
#define DEM_CFG_TRIGGERDLTREPORTS_OFF  STD_OFF
#define DEM_CFG_TRIGGERDLTREPORTS_ON   STD_ON
#define DEM_CFG_TRIGGERDLTREPORTS  DEM_CFG_TRIGGERDLTREPORTS_OFF
/* ---------------------------------------- */
/* DEM_CFG_ALLOW_HISTORY            		*/
/* ---------------------------------------- */
#define DEM_CFG_ALLOW_HISTORY_ON   STD_ON
#define DEM_CFG_ALLOW_HISTORY_OFF  STD_OFF
#define DEM_CFG_ALLOW_HISTORY  DEM_CFG_ALLOW_HISTORY_OFF

/* ---------------------------------------- */
/* DEM_CFG_LOCK_ALLFAILUREINFO            	*/
/* ---------------------------------------- */
#define DEM_CFG_LOCK_ALLFAILUREINFO_ON   STD_ON
#define DEM_CFG_LOCK_ALLFAILUREINFO_OFF  STD_OFF
#define DEM_CFG_LOCK_ALLFAILUREINFO  DEM_CFG_LOCK_ALLFAILUREINFO_OFF

/* ---------------------------------------- */
/* DEM_CFG_EVENTMEMORYENTRY_ONFDCTHRESHOLD  */
/* ---------------------------------------- */
#define DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD_ON   STD_ON
#define DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD_OFF  STD_OFF
#define DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD  DEM_CFG_SUPPORTEVENTMEMORYENTRY_ONFDCTHRESHOLD_OFF
/* ----------------------------------------- */
/* DEM_CFG_SUPPORT_EVENT_FDCTHRESHOLDREACHED */
/* ----------------------------------------- */
#define DEM_CFG_SUPPORT_EVENT_FDCTHRESHOLDREACHED   FALSE
/* ---------------------------------------- */
/* DEM_CFG_SETEVENTSTATUSCALLNOTIFICATION   */
/* ---------------------------------------- */
#define DEM_CFG_SETEVENTSTATUSCALLNOTIFICATION_OFF  STD_OFF
#define DEM_CFG_SETEVENTSTATUSCALLNOTIFICATION_ON   STD_ON

#if (defined DEM_SETEVENTSTATUSCALLNOTIFICATION)
   #define DEM_CFG_SETEVENTSTATUSCALLNOTIFICATION  DEM_CFG_SETEVENTSTATUSCALLNOTIFICATION_ON
#else
#define DEM_CFG_SETEVENTSTATUSCALLNOTIFICATION  DEM_CFG_SETEVENTSTATUSCALLNOTIFICATION_OFF
#endif

/* ---------------------------------------- */
/* DEM_CFG_SETEVENTSTATUSALLOWEDCALLBACK    */
/* ---------------------------------------- */
#define DEM_CFG_SETEVENTSTATUSALLOWEDCALLBACK_OFF  STD_OFF
#define DEM_CFG_SETEVENTSTATUSALLOWEDCALLBACK_ON   STD_ON
#define DEM_CFG_SETEVENTSTATUSALLOWEDCALLBACK  DEM_CFG_SETEVENTSTATUSALLOWEDCALLBACK_OFF

/* ---------------------------------------- */
/* BOOLEAN configuration switches           */
/* ---------------------------------------- */
#define DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE                   FALSE
#define DEM_CFG_CLEARDTCCLEARSALLBITS                       TRUE
#define DEM_CFG_DTCSETTINGBLOCKSREPORTING                   TRUE
#define DEM_CFG_PRJSPECIFICEVENTSTATUSHANDLING              FALSE
#define DEM_CFG_SUPPORT_AGINGCNTR_ONLY_WHEN_AGING_ALLOWED   FALSE
#define DEM_CFG_STORAGECONDITIONBLOCKSPASSEDREPORTS         FALSE

/* ---------------------------------------- */
/* DEM_CFG_DTC_STATUSCHANGEDCALLBACK        */
/* ---------------------------------------- */
#define DEM_CFG_DTC_STATUSCHANGEDCALLBACK_OFF	STD_OFF
#define DEM_CFG_DTC_STATUSCHANGEDCALLBACK_ON	STD_ON
#define DEM_CFG_DTC_STATUSCHANGEDCALLBACK       DEM_CFG_DTC_STATUSCHANGEDCALLBACK_OFF
/* ---------------------------------------- */
/* DEM_CFG_EVT_CLEARALLOWEDCALLBACK        */
/* ---------------------------------------- */
#define DEM_CFG_EVT_CLEARALLOWEDCALLBACK_OFF	STD_OFF
#define DEM_CFG_EVT_CLEARALLOWEDCALLBACK_ON		STD_ON
#define DEM_CFG_EVT_CLEARALLOWEDCALLBACK       	DEM_CFG_EVT_CLEARALLOWEDCALLBACK_ON
/* ---------------------------------------- */
/* DEM_CFG_EVT_GLOBALCLEARALLOWEDCALLBACK        */
/* ---------------------------------------- */
#define DEM_CFG_EVT_GLOBALCLEARALLOWEDCALLBACK_OFF		STD_OFF
#define DEM_CFG_EVT_GLOBALCLEARALLOWEDCALLBACK_ON		STD_ON
#define DEM_CFG_EVT_GLOBALCLEARALLOWEDCALLBACK       	DEM_CFG_EVT_GLOBALCLEARALLOWEDCALLBACK_OFF

/* ---------------------------------------- */
/* DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACK        */
/* ---------------------------------------- */

#define DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACK_OFF   STD_OFF
#define DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACK_ON    STD_ON
#define DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACK       DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACK_OFF
/* ---------------------------------------- */
/* DEM_CFG_EVT_DATACHANGEDCALLBACK          */
/* ---------------------------------------- */
#define DEM_CFG_EVT_DATACHANGEDCALLBACK_OFF    STD_OFF
#define DEM_CFG_EVT_DATACHANGEDCALLBACK_ON     STD_ON
#define DEM_CFG_EVT_DATACHANGEDCALLBACK        DEM_CFG_EVT_DATACHANGEDCALLBACK_OFF

#define DEM_CFG_EVT_PROJECT_EXTENSION  FALSE

/* ---------------------------------------- */

/* ---------------------------------------- */
/* DEM_CFG_GLOBAL_EVT_ST_CH_CALLBACK        */
/* ---------------------------------------- */
#define DEM_CFG_GLOBAL_EVT_ST_CH_CALLBACK_OFF    STD_OFF
#define DEM_CFG_GLOBAL_EVT_ST_CH_CALLBACK_ON     STD_ON
#define DEM_CFG_GLOBAL_EVT_ST_CH_CALLBACK        DEM_CFG_GLOBAL_EVT_ST_CH_CALLBACK_OFF
/* ---------------------------------------- */

#define DEM_CFG_BSWERRORBUFFERSIZE  10u

#define DEM_CFG_EVT_STATUS_CHANGE_NUM_CALLBACKS 	0u

/* --------------------------------------------------- */
/* Reset of TestFailedSinceLastClear bit               */
/* --------------------------------------------------- */
#define DEM_CFG_TFSLC_RESET_AFTER_AGING_AND_DISPLACEMENT       FALSE

#define DEM_CFG_EVT_ATTRIBUTE_OFF  STD_OFF
#define DEM_CFG_EVT_ATTRIBUTE_ON  STD_ON
#define DEM_CFG_EVT_ATTRIBUTE DEM_CFG_EVT_ATTRIBUTE_OFF

/* --------------------------------------------------- */
/* DEM EVENT STATE BITPOSITION                         */
/* --------------------------------------------------- */
#define DEM_EVT_BP_STATE_CAUSALFAULT            0u

#define DEM_EVT_BP_STATE_SUSPICIONLEVEL         1u
#define DEM_EVT_BP_STATE_NOTAVAILABLE           2u
#define DEM_EVT_BP_STATE_INITMONITORING         3u
#define DEM_EVT_BP_STATE_INITMONITORING_MASK    (0x07u)
#define DEM_EVT_BP_STATE_INITMONITORING_SIZE    3u
#define DEM_EVT_BP_STATE_WIRSTATUS_EXTERNAL     6u

#define DEM_EVT_BP_STATE_NXT_REPORT_IS_RELEVANT      7u

typedef uint8 Dem_EvtStateType;
#define DEM_EVTSTATE_ISBITSET      rba_DiagLib_Bit8IsBitSet
#define DEM_EVTSTATE_OVERWRITEBIT  rba_DiagLib_Bit8OverwriteBit
#define DEM_EVTSTATE_OVERWRITEBITS rba_DiagLib_Bit8OverwriteBits
#define DEM_EVTSTATE_CLEARBIT      rba_DiagLib_Bit8ClearBit
#define DEM_EVTSTATE_SETBIT        rba_DiagLib_Bit8SetBit
#define DEM_EVTSTATE_GETBITS       rba_DiagLib_Bit8GetBits
#define DEM_EVTSTATE_CLEARBITS     rba_DiagLib_Bit8ClearBits

/* --------------------------------------------------- */
/* DEM EVENT PARAM BITPOSITION                         */
/* --------------------------------------------------- */

#define DEM_CFG_TRIGGERMONITORINITBEFORECLEAROK  FALSE

#define DEM_CFG_PROJECTEXTENSION_CLEAR_PENDING_NOTIFICATION  FALSE

#define DEM_CFG_CALLBACK_INIT_MON_FOR_EVENT_SUPPORTED   FALSE

/* ---------------------------------------- */

#define DEM_CFG_QUEUEING_UNROBUST_EVENTS_SUPPORTED  FALSE
/* --------------------------------------------------- */
#endif

