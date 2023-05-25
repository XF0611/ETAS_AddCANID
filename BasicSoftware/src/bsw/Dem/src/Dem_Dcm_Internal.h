

#ifndef DEM_DCM_INTERNAL_H
#define DEM_DCM_INTERNAL_H

#include "Dem_Cfg.h"
#include "Dem_Cfg_Client.h"
#include "Platform_Types.h"

#if (DEM_CFG_CHECKAPICONSISTENCY == FALSE) && (DEM_CFG_CLIENT_NUMBER_OF_CLIENTS_USING_RTE > 0)
#define DEM_HIDE_RTE_APIS
#endif
#include "Dem_Dcm.h"


#endif /* DEM_DCM_INTERNAL_H_ */

