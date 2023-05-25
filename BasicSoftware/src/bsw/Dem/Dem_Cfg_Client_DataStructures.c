/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#include "Dem_Cfg_Client_DataStructures.h"
#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"

/* MR12 RULE 10.3 VIOLATION: due to the invalid index beeing used the first row includes values not matching the datatype, this row is never used */
const Dem_Cfg_ClientParams_8Type Dem_Cfg_ClientParams_8[2] = {
/*MR12 RULE 11.9 VIOLATION: The first row is a invalid index and will never be used*/
{ 0, 0, 0 }, { DEM_CFG_DTCSEVERITY_AVAILABILITY_MASK_CLIENTID_1,
		DEM_DTC_TRANSLATION_ISO14229_1, 0xFF } /* DemClient */
};

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"
