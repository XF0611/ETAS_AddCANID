


#ifndef BFX_H
#define BFX_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Std_Types.h"
#include "Bfx_Types.h"
#include "Bfx_Cfg.h"

/* Patch to speed up the run of QAC tool */
#if (!defined(__QACDF__) || (defined(__QACDF__) && defined(SRVLIBS)))

/*
 * SRVLIBS_CPP_BUILD - This macro will be enabled only for PS-EC Cplusplus project
 * for vcup c++ only extern declarations should be visible header files should not be included.
 */
#if(!defined(SRVLIBS_CPP_BUILD))

#if ((BFX_CFG_OPTIMIZATION == BFX_CFG_TCCOMMON) || (BFX_CFG_OPTIMIZATION == BFX_CFG_TC27XX))
    #include "rba_BfxTCCommon_Replacements.h"
#endif
#if ((BFX_CFG_OPTIMIZATION == BFX_CFG_MPCCOMMON) || (BFX_CFG_OPTIMIZATION == BFX_CFG_MPCCUT2))
    #include "rba_BfxMPCCommon_Replacements.h"
#endif
#if (BFX_CFG_OPTIMIZATION == BFX_CFG_ARMV8X32)
    #include "rba_BfxARMV8X32_Replacements.h"
#endif

#include "Bfx_Bit_Inl.h"
#if ((BFX_CFG_OPTIMIZATION == BFX_CFG_TCCOMMON) || (BFX_CFG_OPTIMIZATION == BFX_CFG_TC27XX))
    #include "rba_BfxTCCommon_Bit_Inl.h"
#endif
#if ((BFX_CFG_OPTIMIZATION == BFX_CFG_MPCCOMMON) || (BFX_CFG_OPTIMIZATION == BFX_CFG_MPCCUT2))
    #include "rba_BfxMPCCommon_Bit_Inl.h"
#endif
#if (BFX_CFG_OPTIMIZATION == BFX_CFG_ARMV8X32)
    #include "rba_BfxARMV8X32_Bit_Inl.h"
#endif

/* End if for "#if(!defined(SRVLIBS_CPP_BUILD))" */
#endif

/* End if for "Speed up the run of QAC tool" */
#endif


/*
 **********************************************************************************************************************
 * Module Version Information
 **********************************************************************************************************************
 */
#define BFX_MODULE_ID                   205
#define BFX_VENDOR_ID                   6
#define BFX_SW_MAJOR_VERSION            9
#define BFX_SW_MINOR_VERSION            0
#define BFX_SW_PATCH_VERSION            0
#define BFX_AR_RELEASE_MAJOR_VERSION    4
#define BFX_AR_RELEASE_MINOR_VERSION    2
#define BFX_AR_RELEASE_REVISION_VERSION 2


/*
 **********************************************************************************************************************
 * GetVersionInfo
 **********************************************************************************************************************
 */
#if (BFX_VERSIONINFOAPI == STD_ON)
    #define BFX_START_SEC_CODE
    #include "Bfx_MemMap.h"
    extern void Bfx_GetVersionInfo(Std_VersionInfoType* versionInfo);
    #define BFX_STOP_SEC_CODE
    #include "Bfx_MemMap.h"
#endif

#ifdef __cplusplus
/* extern "C" { */
}
#endif



/* BFX_H */
#endif
