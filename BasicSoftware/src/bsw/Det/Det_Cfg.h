

/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/


#ifndef DET_CFG_H
#define DET_CFG_H

#include "Det_Cfg_Version.h"
/* Configuration status of GetVersionInfo API */
#define DET_CFG_VERSIONINFO_SUPPORTED           FALSE
/* Configuration status of Dlt forwarding */
#define DET_CFG_FORWARDTODLT_SUPPORTED          FALSE
/* Configuration status of API consistency check */
#define DET_CFG_CHECK_API_CONSISTENCY           TRUE

/* Configuration status of runtime error callout */
#define DET_CFG_RUNTIMECALLOUT_SUPPORTED        TRUE

/* Configuration status of transient fault callout */
#define DET_CFG_TRANSIENTCALLOUT_SUPPORTED      TRUE

/* Configuration status of Error Hooks */
#define DET_CFG_HOOKCOUNT                       1

/* Configuration status of Error Buffer */
#define DET_CFG_ERRORBUFFER_ENABLED             TRUE
#define DET_CFG_ERRORBUFFERAPI_SUPPORTED        TRUE
#define DET_CFG_ERRORBUFFERSIZE                 100
#define DET_CFG_ERRORBUFFER_FILTER_DUPLICATES   TRUE



#if (DET_CFG_RUNTIMECALLOUT_SUPPORTED)
extern void Det_ReportRuntimeErrorHook(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);
#define DET_CFG_CALLRUNTIMECALLOUT(ModuleId, InstanceId, ApiId, ErrorId) Det_ReportRuntimeErrorHook((ModuleId), (InstanceId), (ApiId), (ErrorId))
#endif /* #if (DET_CFG_RUNTIMECALLOUT_SUPPORTED) */

#if (DET_CFG_TRANSIENTCALLOUT_SUPPORTED)
extern Std_ReturnType Det_ReportTransientFaultHook(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 FaultId);
#define DET_CFG_CALLTRANSIENTCALLOUT(ModuleId, InstanceId, ApiId, FaultId) Det_ReportTransientFaultHook((ModuleId), (InstanceId), (ApiId), (FaultId))
#endif /* #if (DET_CFG_TRANSIENTCALLOUT_SUPPORTED) */

#if (DET_CFG_HOOKCOUNT > 0)
#define DET_START_SEC_CODE
#include "Det_MemMap.h"
extern void Det_ReportErrorHook(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"

#define DET_CFG_CALLERRORHOOKS(ModuleId, InstanceId, ApiId, ErrorId) \
do{ \
Det_ReportErrorHook((ModuleId), (InstanceId), (ApiId), (ErrorId)); \
}while (0)
#endif /* #if  (DET_CFG_HOOKCOUNT > 0) */

#endif	/* DET_CFG_H */

