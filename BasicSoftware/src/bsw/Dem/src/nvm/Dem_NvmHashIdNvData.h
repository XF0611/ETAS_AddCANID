

#ifndef DEM_NVMHASHIDNVDATA_H
#define DEM_NVMHASHIDNVDATA_H

#include "Platform_Types.h"
#include "Dem_Array.h"
#include "Dem_Cfg.h"
#include "Dem_Cfg_Nvm.h"

typedef uint8 Dem_NvmHashIdType;

#define DEM_START_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"

#if (DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
DEM_ARRAY_DECLARE(Dem_NvmHashIdType, Dem_HashIdInNvm, DEM_NVM_HASH_ID_SIZE);
#endif

#define DEM_STOP_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"



#endif /* DEM_NVMHASHIDNVDATA_H */
