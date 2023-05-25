/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_DTC_DATASTRUCTURES_H
#define DEM_CFG_DTC_DATASTRUCTURES_H

#include "Dem_Cfg_DTCs.h"
#include "Dem_Prv_Det.h"
#include "Dem_Types.h"
#include "Dem_Lib.h"
#include "rba_DiagLib.h"

/*
 #include "Dem_Lock.h"
 #include "Dem_Cfg_StorageCondition.h"
 #include "Dem_Cfg_EnableCondition.h"
 #include "Dem_Internal.h"
 #include "rba_DiagLib.h"


 #include "Dem_Lib.h"  */

/*
 #define DEM_CFG_STOREWAITINGFORMONITORINGEVENT  FALSE
 #define DEM_CFG_DTCFILTER_EXTERNALPROCESSING_OFF  STD_OFF
 #define DEM_CFG_DTCFILTER_EXTERNALPROCESSING_ON   STD_ON
 #define DEM_CFG_DTCFILTER_EXTERNALPROCESSING      DEM_CFG_DTCFILTER_EXTERNALPROCESSING_OFF
 #define DEM_CFG_DTCSTATUS_AVAILABILITYMASK     0xFFu
 #define DEM_CFG_DTC_TRANSLATION_TYPE           DEM_DTC_TRANSLATION_SAEJ1939_73
 #define DEM_CFG_DTCSTATEMANGER_DTCS_PER_CYCLE   5u
 #define DEM_DTC_BP_STATE_SUPPRESSED             0u
 #define DEM_DTC_BP_DTCSETTING_DISABLED    1u
 #define DEM_DTC_GROUP_DemGroupOfDTC_0_DTCS           0xFA0u
 #define DEM_DTC_GROUP_DemGroupOfDTC_1_DTCS           0x1388u  */

/* --------------------------------------------------- */
/* DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED    */
/* --------------------------------------------------- */
// #define DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED   FALSE

/* --------------------------------------------------- */
/*                   INVALID PARAMS                    */
/* --------------------------------------------------- */
#define DEM_INVALID_DTC                 0xAA000000u   /*As per Autosar, the 4th byte(uint32) is unused which is used here internally to invalidate DTC */

typedef struct {
	uint8 data2;  // Severity
} Dem_Cfg_Dtc_8Type;

typedef struct {
	uint32 data1;  // Kind, Nv_Storage, Func_Unit, DtcCode_Is_Index, DtcCode
} Dem_Cfg_Dtc_32Type;

#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"
extern const Dem_Cfg_Dtc_8Type Dem_Cfg_Dtc_8[13];
extern const Dem_Cfg_Dtc_32Type Dem_Cfg_Dtc_32[13];

LOCAL_INLINE Dem_DTCKindType Dem_Cfg_Dtc_GetKind(Dem_DtcIdType indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 13));
	return (Dem_DTCKindType)(
			RBA_DIAGLIB_GETBITS32(Dem_Cfg_Dtc_32[indx].data1, 0, 2));
}

LOCAL_INLINE Dem_DTCSeverityType Dem_Cfg_Dtc_GetSeverity(Dem_DtcIdType indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 13));
	return Dem_Cfg_Dtc_8[indx].data2;
}

LOCAL_INLINE boolean Dem_Cfg_Dtc_GetNv_Storage(Dem_DtcIdType indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 13));
	return RBA_DIAGLIB_ISBITSET32(Dem_Cfg_Dtc_32[indx].data1, 2);
}

LOCAL_INLINE uint8 Dem_Cfg_Dtc_GetFunc_Unit(Dem_DtcIdType indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 13));
	return (uint8)(RBA_DIAGLIB_GETSINGLEBIT32(Dem_Cfg_Dtc_32[indx].data1, 3));
}

LOCAL_INLINE boolean Dem_Cfg_Dtc_GetDtcCode_Is_Index(Dem_DtcIdType indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 13));
	return RBA_DIAGLIB_ISBITSET32(Dem_Cfg_Dtc_32[indx].data1, 4);
}

/* MR12 RULE 2.7 VIOLATION: the parameter 'indx' is not used because the retrun value is the same for all indices due to the config, it is kept because the code calling the function is not config base but static */
LOCAL_INLINE Dem_DtcCodeType Dem_Cfg_Dtc_GetObd_DtcCode(Dem_DtcIdType indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 13));
	return DEM_INVALID_DTC; /* feature deactivated */
}

LOCAL_INLINE Dem_DtcCodeType Dem_Cfg_Dtc_GetDtcCode(Dem_DtcIdType indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 13));
	return (Dem_DtcCodeType)(
			RBA_DIAGLIB_GETBITS32(Dem_Cfg_Dtc_32[indx].data1, 5, 24));
}

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"

// #define DEM_DTC_DTCSETTING_CALLBACK_SUPPORTED_OFF STD_OFF
// #define DEM_DTC_DTCSETTING_CALLBACK_SUPPORTED_ON STD_ON

#endif
