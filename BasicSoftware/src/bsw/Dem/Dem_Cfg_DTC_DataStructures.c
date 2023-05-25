/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#include "Dem_Cfg_DTC_DataStructures.h"
#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"

/* MR12 RULE 10.3 VIOLATION: due to the invalid index beeing used the first row includes values not matching the datatype, this row is never used */
const Dem_Cfg_Dtc_8Type Dem_Cfg_Dtc_8[13] = {
/*MR12 RULE 11.9 VIOLATION: The first row is a invalid index and will never be used*/
{ 0 }, { DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0x000001 */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0x0d0400_CAN_Network_ETAS_BusOff */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0xc07388 */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0xc12687 */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0xc14587 */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0xc22a87 */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0xc41487 */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0xd56217 */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0xd56316 */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0xe00141 */
{ DEM_SEVERITY_NO_SEVERITY }, /* DemDTC_DTC_0xe00142 */
{ DEM_SEVERITY_NO_SEVERITY } /* DemDTC_DTC_0xe26200 */
};

const Dem_Cfg_Dtc_32Type Dem_Cfg_Dtc_32[13] = {
/*MR12 RULE 11.9 VIOLATION: The first row is a invalid index and will never be used*/
{ 0 }, { ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0)
		+ ((uint32)(1u & 0x1u) << 2) + ((uint32)(0x00u & 0x1u) << 3)
		+ ((uint32)(0u & 0x1u) << 4) + ((uint32)(0x1u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0x000001 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xD0400u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0x0d0400_CAN_Network_ETAS_BusOff */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xC07388u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0xc07388 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xC12687u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0xc12687 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xC14587u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0xc14587 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xC22A87u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0xc22a87 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xC41487u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0xc41487 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xD56217u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0xd56217 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xD56316u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0xd56316 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xE00141u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0xe00141 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xE00142u & 0xFFFFFFu) << 5) }, /* DemDTC_DTC_0xe00142 */
{ ((uint32)(DEM_DTC_KIND_ALL_DTCS & 0x3u) << 0) + ((uint32)(1u & 0x1u) << 2)
		+ ((uint32)(0x00u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
		+ ((uint32)(0xE26200u & 0xFFFFFFu) << 5) } /* DemDTC_DTC_0xe26200 */
};

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"
