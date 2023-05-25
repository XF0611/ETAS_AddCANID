

#ifndef DEM_CLIENTMACHINE_SELECTED_FFDATA_H
#define DEM_CLIENTMACHINE_SELECTED_FFDATA_H

#include "Dem_Client.h"
#include "Dem_EvMem.h"

/******************************************************************************************/
/****************  SelectED_FFMachineStates  *********************************************************/
#define DEM_SELECT_ED_FF_STATE_INIT                 0x00u
#define DEM_SELECT_ED_STATE_SELECTION_INVALID       0x01u
#define DEM_SELECT_ED_ITERATION_IN_PROGRESS         0x02u
#define DEM_SELECT_FF_STATE_SELECTION_INVALID       0x03u
#define DEM_SELECT_FF_ITERATION_IN_PROGRESS         0x04u

/******************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

DEM_INLINE void Dem_ClientMachine_SelectED_FFData_SetClientRequest( Dem_ClientIdType ClientId, volatile Dem_ClientResultType *result, Dem_ClientRequestType request )
{
    if(Dem_ClientSelectionType_isSelectionSingleDTC(Dem_Client_getClient(ClientId)->selection))
    {
        uint16_least LocId;
        Dem_DtcIdType DtcId;

        DtcId = Dem_ClientSelectionType_getSelectionDtcIndex(Dem_Client_getClient(ClientId)->selection);
        LocId = Dem_EvMemGetMemoryLocIdOfDtcAndOriginWithVisibility(DtcId, Dem_Client_getClient(ClientId)->data.standard.DTCOrigin, Dem_EvMemGetShadowVisibility());
        if (Dem_EvMemIsEventMemLocIdValid(LocId))
        {
            Dem_ClientSelectED_FFDataType_SetEvMemCopyValid(ClientId, TRUE );
            Dem_ClientSelectED_FFDataType_SaveEvMemCopy( ClientId , &Dem_EvMemEventMemory[LocId] );
        }
        else
        {
            Dem_ClientSelectED_FFDataType_SetEvMemCopyValid(ClientId, FALSE );
        }
        Dem_ClientResultType_setResult(result, request, E_OK);
    }
    else
    {
        Dem_ClientResultType_setResult(result, request, DEM_WRONG_DTC);
    }
}

DEM_INLINE void Dem_ClientMachine_SelectED_FFData_SetState(uint8 ClientId, uint8 NewState)
{
    Dem_Client_getClient(ClientId)->data.standard.SelectED_FF_MachineState = NewState;
}

DEM_INLINE uint8 Dem_ClientMachine_SelectED_FFData_GetState(uint8 ClientId)
{
    return Dem_Client_getClient(ClientId)->data.standard.SelectED_FF_MachineState;
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif /* DEM_CLIENTMACHINE_SELECTED_FFDATA_H */
