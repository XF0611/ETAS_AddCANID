

#ifndef DEM_CLIENTMACHINE_SELECT_H
#define DEM_CLIENTMACHINE_SELECT_H


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"


DEM_INLINE void Dem_ClientMachineSelectionResult_SetClientRequest(Dem_ClientIdType clientId)
{
    Dem_ClientResultType_setResult(&Dem_AllClientsState[clientId].result, Dem_AllClientsState[clientId].request, E_OK);
}

DEM_INLINE void Dem_ClientMachineSelectionResult4Clear_SetClientRequest(Dem_ClientIdType clientId)
{
          Dem_ClientResultType_setResult(&Dem_AllClientsState[clientId].result, Dem_AllClientsState[clientId].request, E_OK);
}


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif /* DEM_CLIENTMACHINE_SELECT_H */
