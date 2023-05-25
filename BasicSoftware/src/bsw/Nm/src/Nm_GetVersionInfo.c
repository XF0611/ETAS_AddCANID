

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
/* NM Interface main header file, this is included by all the applications controlling NM */
#include "Nm.h"
#include "Nm_Priv.h"

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/***************************************************************************************************
 * Function
 ***************************************************************************************************
 ***************************************************************************************************
 * Function Name: Nm_GetVersionInfo
 * Syntax:        void Nm_GetVersionInfo( Std_VersionInfoType* nmVerInfoPtr )
 * Description:   This service returns the version information of this module.
 * Parameter:     nmVerInfoPtr- Pointer to where to store the version information of this module.
 * Return:        None
 ***************************************************************************************************/

/* This function is only available if NM_VERSION_INFO_API is set to TRUE */
#if (NM_VERSION_INFO_API != STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_GetVersionInfo (Std_VersionInfoType * nmVerInfoPtr)
{

    if(NULL_PTR == nmVerInfoPtr)
    {
        /* Report DET : Development Error for Invalid  Pointer */
        NM_DET_REPORT_ERROR((0u), NM_SID_GETVERSIONINFO, NM_E_PARAM_POINTER);
        return;
    }

    nmVerInfoPtr->vendorID = NM_VENDOR_ID;
    nmVerInfoPtr->moduleID = NM_MODULE_ID;
    nmVerInfoPtr->sw_major_version = NM_SW_MAJOR_VERSION;
    nmVerInfoPtr->sw_minor_version = NM_SW_MINOR_VERSION;
    nmVerInfoPtr->sw_patch_version = NM_SW_PATCH_VERSION;

} /*End of Nm_GetVersionInfo( ) */
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#endif

