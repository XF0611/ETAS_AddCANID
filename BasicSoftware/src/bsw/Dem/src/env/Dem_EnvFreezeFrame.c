

#include "Dem_Types.h"
#include "Dem_Array.h"
#include "Dem_EnvFreezeFrame.h"
#include "Dem_Cfg_EnvFreezeFrame.h"
#include "Dem_Cfg_EnvDid.h"

#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"
const uint8 Dem_Cfg_EnvFreezeFrame2Did[] = DEM_CFG_ENVFREEZEFRAME2DID;
/* MR12 RULE 20.7 VIOLATION: Functions like MACROS do not need to be parenthesized.*/
DEM_ARRAY_DEFINE_CONST(Dem_EnvFreezeFrame, Dem_Cfg_EnvFreezeFrame, DEM_CFG_ENVFREEZEFRAME_ARRAYLENGTH, DEM_CFG_ENVFREEZEFRAME);
#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"
