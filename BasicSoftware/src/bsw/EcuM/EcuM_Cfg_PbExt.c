
/*
 ***************************************************************************************************
 * 									Includes
 ***************************************************************************************************
 */

/**************************************************************************************
 Function name :   uint8 EcuM_Prv_GetPbVariantIndex(void )
 Description   :   This API shall return the index of the Post build variant depending upon the
 current level of the Portpins.The mapping between the index and pattern is derived
 from configuration
 Parameter in  :   Void
 Parameter out :   None
 Return value  :   None
 Remarks       :

 ****************************************************************************************/

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

void EcuM_Prv_GetPbVariantIndex(void);
void EcuM_Prv_GetPbVariantIndex(void) {
	/*This function is empty because the EcuMRbDeterminePb container in EcuMGeneral is not Configured*/
	return;

}

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

