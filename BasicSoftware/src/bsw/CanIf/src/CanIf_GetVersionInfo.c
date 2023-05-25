


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */


#include "CanIf_Prv.h"


/*************************************************************************************************
 * Function name    : CanIf_GetVersionInfo
 * Syntax           : void CanIf_GetVersionInfo(Std_VersionInfoType *VersionInfo)
 * Description      : This service returns the version information of this module.
 * Parameter        : VersionInfo
 * Return value     : None
 **************************************************************************************************/
#if (CANIF_PUBLIC_VERSIONINFO_API != STD_OFF)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_GetVersionInfo (Std_VersionInfoType * VersionInfo)
{


    CANIF_DET_REPORT_ERROR_VOID_RET(VersionInfo == NULL_PTR, CANIF_GET_VERSION_INFO_SID, CANIF_E_PARAM_POINTER)
    /*Update Vendor Id, Module Id, Major, minor and patch version*/
    VersionInfo->vendorID = CANIF_VENDOR_ID;
    VersionInfo->moduleID = CANIF_MODULE_ID;
    VersionInfo->sw_major_version = CANIF_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = CANIF_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = CANIF_SW_PATCH_VERSION;

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
