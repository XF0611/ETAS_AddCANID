

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


#if (COM_VERSION_INFO_API == STD_ON)
/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/
/*
 **********************************************************************************************************************
 Function name    : Com_GetVersionInfo
 Description      : Service for passing the Information through a pointer "*VersionInfo"
 Parameter        : versioninfo_pst --> pointer to an address where all the version information needs to be copied.
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_GetVersionInfo(Std_VersionInfoType * versioninfo_pst)
{
    /* TRACE[SWS_Com_00804] Error code(COM_E_UNINIT) is not reported,
     * if Com_GetVersionInfo service is called
     * before the AUTOSAR COM module was initialized with Com_Init or after a call to Com_DeInit */
#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (versioninfo_pst == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_GetVersionInfo, COM_E_PARAM_POINTER);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        versioninfo_pst->vendorID = COM_VENDOR_ID;
        versioninfo_pst->moduleID = COM_MODULE_ID;
        versioninfo_pst->sw_major_version = COM_SW_MAJOR_VERSION;
        versioninfo_pst->sw_minor_version = COM_SW_MINOR_VERSION;
        versioninfo_pst->sw_patch_version = COM_SW_PATCH_VERSION;
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if (COM_VERSION_INFO_API == STD_ON) */

