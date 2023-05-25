/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_DEB_H
#define DEM_CFG_DEB_H

#include "Std_Types.h"

/* ---------------------------------------- */
/* DEM_CFG_DEBMONINTBASE                    */
/* ---------------------------------------- */
#define DEM_CFG_DEBMONINTERNAL_OFF  STD_OFF
#define DEM_CFG_DEBMONINTERNAL_ON   STD_ON
#define DEM_CFG_DEBMONINTERNAL  DEM_CFG_DEBMONINTERNAL_OFF

#define DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION_ON STD_ON
#define DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION_OFF STD_OFF
#define DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION DEM_CFG_PRJSPECIFIC_DEB_OPTIMIZATION_OFF

/* ---------------------------------------- */
/* DEM_CFG_DEBARTIMEBASE                    */
/* ---------------------------------------- */
#define DEM_CFG_DEBARTIMEBASE_OFF  STD_OFF
#define DEM_CFG_DEBARTIMEBASE_ON   STD_ON
#define DEM_CFG_DEBARTIMEBASE  DEM_CFG_DEBARTIMEBASE_OFF

/* ---------------------------------------- */
/* DEM_CFG_DEBCOUNTERBASEDCLASS             */
/* ---------------------------------------- */
#define DEM_CFG_DEBCOUNTERBASECLASS_OFF  STD_OFF
#define DEM_CFG_DEBCOUNTERBASECLASS_ON   STD_ON
#define DEM_CFG_DEBCOUNTERBASECLASS  DEM_CFG_DEBCOUNTERBASECLASS_ON

#define DEM_CFG_LIMIT_RELEVANTFOR_JUMPING_OFF  STD_OFF
#define DEM_CFG_LIMIT_RELEVANTFOR_JUMPING_ON   STD_ON
#define DEM_CFG_LIMIT_RELEVANTFOR_JUMPING DEM_CFG_LIMIT_RELEVANTFOR_JUMPING_OFF

#define DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT 32767
#define DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MINLIMIT (-32768)
#define DEM_CFG_DEFAULT_DEBCOUNTERCLASS_JUMPUP_ENABLED TRUE
#define DEM_CFG_DEFAULT_DEBCOUNTERCLASS_JUMPUP_VALUE 0
#define DEM_CFG_DEFAULT_DEBCOUNTERCLASS_JUMPDOWN_ENABLED TRUE
#define DEM_CFG_DEFAULT_DEBCOUNTERCLASS_JUMPDOWN_VALUE 0

#define DEM_CFG_DEBCOUNTERBASECLASS_MINTHRESHOLD_OFF  STD_OFF
#define DEM_CFG_DEBCOUNTERBASECLASS_MINTHRESHOLD_ON   STD_ON
#define DEM_CFG_DEBCOUNTERBASECLASS_MINTHRESHOLD  DEM_CFG_DEBCOUNTERBASECLASS_MINTHRESHOLD_ON

#define DEM_CFG_DEBCOUNTERBASECLASS_MAXTHRESHOLD_OFF  STD_OFF
#define DEM_CFG_DEBCOUNTERBASECLASS_MAXTHRESHOLD_ON   STD_ON
#define DEM_CFG_DEBCOUNTERBASECLASS_MAXTHRESHOLD  DEM_CFG_DEBCOUNTERBASECLASS_MAXTHRESHOLD_ON

#define DEM_CFG_DEBCOUNTERBASECLASS_JUMPUPVALUE_OFF  STD_OFF
#define DEM_CFG_DEBCOUNTERBASECLASS_JUMPUPVALUE_ON   STD_ON
#define DEM_CFG_DEBCOUNTERBASECLASS_JUMPUPVALUE  DEM_CFG_DEBCOUNTERBASECLASS_JUMPUPVALUE_ON

#define DEM_CFG_DEBCOUNTERBASECLASS_JUMPDOWNVALUE_OFF  STD_OFF
#define DEM_CFG_DEBCOUNTERBASECLASS_JUMPDOWNVALUE_ON   STD_ON
#define DEM_CFG_DEBCOUNTERBASECLASS_JUMPDOWNVALUE  DEM_CFG_DEBCOUNTERBASECLASS_JUMPDOWNVALUE_ON

#define DEM_CFG_DEBCOUNTERBASECLASS_ISJUMPDOWN_OFF  STD_OFF
#define DEM_CFG_DEBCOUNTERBASECLASS_ISJUMPDOWN_ON   STD_ON
#define DEM_CFG_DEBCOUNTERBASECLASS_ISJUMPDOWN  DEM_CFG_DEBCOUNTERBASECLASS_ISJUMPDOWN_ON

#define DEM_CFG_DEBCOUNTERBASECLASS_ISJUMPUP_OFF  STD_OFF
#define DEM_CFG_DEBCOUNTERBASECLASS_ISJUMPUP_ON   STD_ON
#define DEM_CFG_DEBCOUNTERBASECLASS_ISJUMPUP  DEM_CFG_DEBCOUNTERBASECLASS_ISJUMPUP_ON

#define DEMRB_DEBOUNCECOUNTERSETS_SIZE                                 2
/** Offset of the CounterSet Id where the custom counter sets start */
#define DEMRB_DEBOUNCECOUNTERSETS_CUSTOMSETSOFFSET                     2

#define DemConf_DemRbDebounceCounterSet_Standard                       0
#define DemConf_DemRbDebounceCounterSet_Alternative                    1

#define DEM_CFG_DEBCOUNTERBASECLASS_PARAMSET_LENGTH 12

/*                 				ISJDOWN             ISJUP               MINLIMIT                      MAXLIMIT                      JDOWNValue          JUPValue            FDC_THRESHOLD       SUSPICIOUS_THRESHOLD STEPUP             STEPDOWN             */

#define DEM_CFG_DEBCOUNTERCLASS_PARAMSETS \
{ \
  { /* Default Debouncing Parameters */ \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
}, \
  { /* Alternative Debouncing Parameters */  \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
   DEM_DEBCOUNTERBASECLASS_INIT (FALSE,              TRUE,               -1,                           1,                            0,                  0,                  DEM_CFG_DEFAULT_DEBCOUNTERCLASS_MAXLIMIT,DEM_MAXSINT16,1,   -1                  ), \
  }, \
}

#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0X000001_COUNTERBASEDALGORITHM  0u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0X0D0400_CAN_NETWORK_ETAS_BUSOFF_COUNTERBASEDALGORITHM  1u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC07388_COUNTERBASEDALGORITHM  2u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC12687_COUNTERBASEDALGORITHM  3u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC14587_COUNTERBASEDALGORITHM  4u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC22A87_COUNTERBASEDALGORITHM  5u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC41487_COUNTERBASEDALGORITHM  6u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XD56217_COUNTERBASEDALGORITHM  7u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XD56316_COUNTERBASEDALGORITHM  8u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XE00141_COUNTERBASEDALGORITHM  9u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XE00142_COUNTERBASEDALGORITHM  10u
#define DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XE26200_COUNTERBASEDALGORITHM  11u

#define DEM_DEBPARAM_IDX_DTC_0X000001_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0X000001_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0X0D0400_CAN_NETWORK_ETAS_BUSOFF_EVENT  DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0X0D0400_CAN_NETWORK_ETAS_BUSOFF_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XC07388_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC07388_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XC12687_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC12687_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XC14587_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC14587_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XC22A87_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC22A87_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XC41487_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XC41487_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XD56217_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XD56217_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XD56316_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XD56316_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XE00141_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XE00141_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XE00142_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XE00142_COUNTERBASEDALGORITHM
#define DEM_DEBPARAM_IDX_DTC_0XE26200_EVENT             DEM_DEBPARAMCLASS_IDX_DEBCNTBAS_DTC_0XE26200_COUNTERBASEDALGORITHM

#define DEM_CFG_DEB_DEFINE_ALL_PARAMSETS \
static const Dem_DebCounterBaseClass_ParamSet  Dem_Cfg_DebCounterBaseClass_Paramsets[DEMRB_DEBOUNCECOUNTERSETS_SIZE][DEM_CFG_DEBCOUNTERBASECLASS_PARAMSET_LENGTH] = DEM_CFG_DEBCOUNTERCLASS_PARAMSETS; \


#define  DEM_CFG_DEB_CLASSES \
{ \
   { \
      NULL_PTR, \
      NULL_PTR, \
      NULL_PTR, \
      0, \
      NULL_PTR \
   } \
   ,{ \
      &Dem_DebCounterBaseClass_GetLimits, \
      NULL_PTR, \
      &Dem_Cfg_DebCounterBaseClass_Paramsets[DemConf_DemRbDebounceCounterSet_Standard], \
      DEM_CFG_DEBCOUNTERBASECLASS_PARAMSET_LENGTH, \
      &Dem_DebCounterBaseClass_Filter \
   } \
}

#define  DEM_CFG_DEB_NUMBEROFCLASSES  2

#define DEM_DEBMETH_IDX_ARCTRBASECLASS      (1u)

/* Support Debouncer Reset After StorageCondition */
#define DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO_OFF                                    0u
#define DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO_ACCORDING_DEBOUNCEBEHAVIOR             1u

#define DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO           DEM_CFG_SUPPORT_DEB_RESET_AFTER_STOCO_OFF

#endif

