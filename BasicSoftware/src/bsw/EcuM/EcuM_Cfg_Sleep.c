
#include "EcuM.h" /*BSW_HeaderInc_002*/

/*Extern module Headers*/
#include "Os.h" /* required for Osresource */
// #if (!defined(OS_AR_RELEASE_MAJOR_VERSION) || (OS_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(OS_AR_RELEASE_MINOR_VERSION) || (OS_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif

/*EcuM Private headers*/
#include "EcuM_Prv.h"

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

/*********************************************************************
 Function name     :   EcuM_Prv_GetResource
 Description       :   This call aquires the EcuM resource for the core which id is passed as parameter.
 This API is used for Resource mapping with respect to the Core_ID.
 Parameter (in)    :   Core_ID
 Parameter (inout) :   None
 Parameter (out)   :   None
 Return value      :   StatusType. E_OK - Will be returned if GetResource is successful
 E_OS_ID - Will be returned if resource is not valid/configured
 Remarks           :
 ***********************************************************************/

StatusType EcuM_Prv_GetResource(CoreIdType core_ID) {
	/*Local variable declaration for API return value.*/
	StatusType Return_Value = E_OK;

	(void) core_ID;
	Return_Value = GetResource(RES_AUTOSAR_ECUM);	//Single Core

	return Return_Value;

}

/*********************************************************************
 Function name     :   EcuM_Prv_ReleaseResource
 Description       :   This call releases the EcuM resource for the core which id is passed as parameter.
 This API is used for Resource mapping with respect to the Core_ID.
 Parameter (in)    :   Core_ID
 Parameter (inout) :   None
 Parameter (out)   :   None
 Return value      :   StatusType. E_OK - Will be returned if ReleaseResource is successful
 E_OS_ID - Will be returned if resource is not valid/configured
 Remarks           :
 ***********************************************************************/

StatusType EcuM_Prv_ReleaseResource(CoreIdType core_ID) {
	/*Local variable declaration for API return value.*/
	StatusType Return_Value = E_OK;

	(void) core_ID;
	Return_Value = ReleaseResource(RES_AUTOSAR_ECUM);
	return Return_Value;

}

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

