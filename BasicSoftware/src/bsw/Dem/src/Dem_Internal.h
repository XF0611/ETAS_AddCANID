

#ifndef DEM_INTERNAL_H
#define DEM_INTERNAL_H

#include "Dem_Cfg.h"
#include "Platform_Types.h"
#include "Rte_Dem_Type.h"

#if (DEM_CFG_CHECKAPICONSISTENCY == FALSE)
    #ifdef RTE_TYPE_H
    #define DEM_HIDE_RTE_APIS
    #endif
#else
    #ifndef RTE_TYPE_H
    #error "If the RTE is not used, then /Dem/DemGeneral/DemRbGeneral/DemRbCheckApiConsistency has to be set to False."
    #endif /* RTE_TYPE_H */
#endif /* DEM_HIDE_RTE_APIS */

#include "Dem.h"

#endif /* DEM_INTERNAL_H */
