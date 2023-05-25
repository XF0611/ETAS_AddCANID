


#ifndef MFL_H
#define MFL_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Std_Types.h"
#include "Mfl_Types.h"
#include "Mfl_Cfg.h"

/* Patch to support the old name of the interface. Change of Mfl_ArcSin_f32 in Rev3 SWS.
   Autosar service name is different to expected name. */
#define  Mfl_arcSin_f32 Mfl_ArcSin_f32

/* Patch to support the old name of the interface. Change of Mfl_ArcCos_f32 in Rev3 SWS.
   Autosar service name is different to expected name. */
#define  Mfl_arcCos_f32 Mfl_ArcCos_f32

/* Patch to support the old name of the interface. Change of Mfl_ArcTan_f32 in Rev3 SWS.
   Autosar service name is different to expected name. */
#define  Mfl_arcTan_f32 Mfl_ArcTan_f32

/* Patch to support the old name of the interface. Change of Mfl_ArcTan2_f32 in Rev3 SWS.
   Autosar service name is different to expected name. */
#define  Mfl_arcTan2_f32 Mfl_ArcTan2_f32

/* Patch to support the old name of the interface. Change of Mfl_CtrlSetLimit in Rev3 SWS.
   Autosar service name is different to expected name. */
#define Mfl_CtrlSetLimits Mfl_CtrlSetLimit

/* Patch to support the old name of the interface. Change of Mfl_DebounceSetParam in Rev3 SWS.
   Autosar service name is different to expected name. */
#define  Mfl_DebounceSetparam Mfl_DebounceSetParam

/* Patch to support the old name of the interface. Change of Mfl_RampCalcSwitch in Rev3 SWS.
   Autosar service name is different to expected name. */
#define  Mfl_RampCalcSwitch_f32 Mfl_RampCalcSwitch

/* Patch to support the old name of the interface. Change of Mfl_ArrayAverage_f32_f32 in Rev3 SWS.
   Autosar service name is different to expected name. */
#define  Mfl_Array_Average_f32_f32 Mfl_ArrayAverage_f32_f32

/* Patch to speed up the run of QAC tool */
#if (!defined(__QACDF__) || (defined(__QACDF__) && defined(SRVLIBS)))

/*
 * SRVLIBS_CPP_BUILD - This macro will be enabled only for PS-EC Cplusplus project
 * for vcup c++ only extern declarations should be visible header files should not be included.
 */
#if(!defined(SRVLIBS_CPP_BUILD))

#include "Mfl_Control_Inl.h"
#include "Mfl_Controller_Inl.h"
#include "Mfl_Stat_Inl.h"
#include "Mfl_Limit_Inl.h"
#include "Mfl_Trig_Inl.h"
#include "Mfl_Conversion_Inl.h"

#if ((MFL_CFG_OPTIMIZATION == MFL_CFG_TCCOMMON) || (MFL_CFG_OPTIMIZATION == MFL_CFG_TC27XX))
    #include "rba_MflTCCommon_Conversion_Inl.h"
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
#define MFL_MODULE_ID                   210
#define MFL_VENDOR_ID                   6
#define MFL_SW_MAJOR_VERSION            9
#define MFL_SW_MINOR_VERSION            0
#define MFL_SW_PATCH_VERSION            0
#define MFL_AR_RELEASE_MAJOR_VERSION    4
#define MFL_AR_RELEASE_MINOR_VERSION    2
#define MFL_AR_RELEASE_REVISION_VERSION 2


/*
 **********************************************************************************************************************
 * GetVersionInfo
 **********************************************************************************************************************
 */
#if (MFL_VERSIONINFOAPI == STD_ON)
    #define MFL_START_SEC_CODE
    #include "Mfl_MemMap.h"
    extern void Mfl_GetVersionInfo(Std_VersionInfoType* versionInfo);
    #define MFL_STOP_SEC_CODE
    #include "Mfl_MemMap.h"
#endif

#ifdef __cplusplus
/* extern "C" { */
}
#endif



/* MFL_H */
#endif
