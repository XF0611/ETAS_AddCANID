/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_CLIENT_DATASTRUCTURES_H
#define DEM_CFG_CLIENT_DATASTRUCTURES_H

#include "Dem_Prv_Det.h"
#include "Dem_Types.h"
#include "Dem_Lib.h"

/* PROVIDE DTC SEVERITY FOR EACH SUPPORTED DTC CONFIGURED IN EVENT MEMORY SET LINKED TO A CLIENT */

#define DEM_CFG_DTCSEVERITY_AVAILABILITY_MASK_CLIENTID_1    (DEM_SEVERITY_NO_SEVERITY) /* DemClient */

typedef struct {
	uint8 data1;  // DTCSeverityAvailabilityMask
	uint8 data2;  // DTCTranslationFormat
	uint8 data3;  // DTCStatusAvailabilityMask
} Dem_Cfg_ClientParams_8Type;

#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"
extern const Dem_Cfg_ClientParams_8Type Dem_Cfg_ClientParams_8[2];

LOCAL_INLINE Dem_UdsStatusByteType Dem_Cfg_ClientParams_GetDTCStatusAvailabilityMask(
		uint8 indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 2));
	return Dem_Cfg_ClientParams_8[indx].data3;
}

LOCAL_INLINE Dem_DTCTranslationFormatType Dem_Cfg_ClientParams_GetDTCTranslationFormat(
		uint8 indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 2));
	return Dem_Cfg_ClientParams_8[indx].data2;
}

LOCAL_INLINE uint8 Dem_Cfg_ClientParams_GetDTCSeverityAvailabilityMask(
		uint8 indx) {
	DEM_ASSERT_RANGECHECK((0 < indx) && (indx < 2));
	return Dem_Cfg_ClientParams_8[indx].data1;
}

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"

#endif
