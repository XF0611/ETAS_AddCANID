

#include "rba_MemLib.h"
#include "rba_MemLib_Cfg.h"


/**
 *********************************************************************
 * Provide a compile time whether Autosar version support is prototype
 * or series
 *********************************************************************
 */
#if (RBA_MEMLIB_AR_RELEASE_MAJOR_VERSION != 4)
#error Autosar versions different from 4.x are not supported.
#endif

#if (RBA_MEMLIB_AR_RELEASE_MINOR_VERSION > 2)
#if !defined (RBA_MEMLIB_CFG_ENABLE_PROTOTYPE)
#error This Autosar version is only supported as prototype - activate the prototype functionality in the configuration of rba_MemLib
#endif
#endif





#define  RBA_MEMLIB_START_SEC_CODE
#include "rba_MemLib_MemMap.h"

#if(defined(RBA_MEMLIB_VERSION_INFO) && (RBA_MEMLIB_VERSION_INFO != STD_OFF))
/**
 *********************************************************************
 * rba_MemLib_GetVersionInfo(): Return version information
 *
 * This function returns general and SW version information
 *
 * \param   VersioninfoPtr: Pointer to return the version
 *                          information of this module.
 * \return  None
 * \retval
 * \seealso
 * \usedresources
 *********************************************************************
 */
void rba_MemLib_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr)
{
    VersionInfoPtr->vendorID = 6u;      /* RB=6 */
    VersionInfoPtr->moduleID = 255u;    /* not standarized=255 */

    VersionInfoPtr->sw_major_version   = RBA_MEMLIB_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version   = RBA_MEMLIB_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version   = RBA_MEMLIB_SW_PATCH_VERSION;

}
#endif /* MEMLIB_VERSION_INFO_API */

#define  RBA_MEMLIB_STOP_SEC_CODE
#include "rba_MemLib_MemMap.h"
