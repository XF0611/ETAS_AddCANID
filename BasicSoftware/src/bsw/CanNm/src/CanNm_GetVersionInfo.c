

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanNm_Prv.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_GetVersionInfo
 Description      : This service returns the version informations - Module Id, Vendor Id and
                    vendor specific version numbers of this module. This API shall be called by Nm.
 Parameter        : versioninfo - Pointer to where to store the version information of this module
 Return value     : None
 ***************************************************************************************************/

/* CanNm_GetVersionInfo is available only if CANNM_VERSION_INFO_API is set to TRUE */
#if (CANNM_VERSION_INFO_API != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

void CanNm_GetVersionInfo (Std_VersionInfoType * versioninfo)
{

    /* Report DET if received pointer is a null pointer */
    CANNM_DET_REPORT_ERROR((NULL_PTR == versioninfo), (0u), CANNM_SID_GETVERSIONINFO, CANNM_E_PARAM_POINTER)

    /* TRACE[SWS_CanNm_00224] : Copy the version information of this module into the passed pointer */
    versioninfo->vendorID = CANNM_VENDOR_ID;
    versioninfo->moduleID = CANNM_MODULE_ID;
    versioninfo->sw_major_version = CANNM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = CANNM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = CANNM_SW_PATCH_VERSION;

}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


