


/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
 
 #define SRVLIBS
 
#include "Mfl.h"

/**
 **********************************************************************************************************************
 * Mfl_GetVersionInfo
 *
 * Macro to get version information including Vendor Id, Module Id and Version Info
 *
 * \param    versionInfo - pointer to structure Std_VersionInfoType
 * \retval   void
 **********************************************************************************************************************
*/
#if (MFL_VERSIONINFOAPI == STD_ON)

    #define MFL_START_SEC_CODE
    #include "Mfl_MemMap.h"
    void Mfl_GetVersionInfo(Std_VersionInfoType* versionInfo)
    {
        versionInfo->vendorID = MFL_VENDOR_ID;
        versionInfo->moduleID = MFL_MODULE_ID;
        versionInfo->sw_major_version = MFL_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = MFL_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = MFL_SW_PATCH_VERSION;
    }
    #define MFL_STOP_SEC_CODE
    #include "Mfl_MemMap.h"

#endif

/*********************************************************************************************************************/


