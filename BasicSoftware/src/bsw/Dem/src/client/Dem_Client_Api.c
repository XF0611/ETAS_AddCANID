


#include "Dem_ClientBaseHandling.h"
#include "Dem_Dcm.h"
#include "Dem_Cfg_Clear.h"


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"


/**************************************************************************************************/
/****************  GetDTCSelectionResult **********************************************************************/

Std_ReturnType Dem_GetDTCSelectionResult(uint8 ClientId)
{
    return Dem_Client_Operation(ClientId, DEM_CLIENT_REQUEST_TRIGGERSELECTIONRESULT,
            DEM_DET_APIID_DEM_GETDTCSELECTIONRESULT);
}

Std_ReturnType Dem_GetDTCSelectionResultForClearDTC(uint8 ClientId)
{
    Std_ReturnType RetVal = E_NOT_OK;

    RetVal = Dem_Client_Operation(ClientId, DEM_CLIENT_REQUEST_GETSELECTIONRESULT4CLEAR,
            DEM_DET_APIID_DEM_GETDTCSELECTIONRESULTFORCLEAR);
    if(RetVal == E_OK)
    {
        if((Dem_LibGetParamUI8(DEM_CFG_CLEAR_DTC_LIMITATION) == Dem_LibGetParamUI8(DEM_CFG_CLEAR_DTC_LIMITATION_ONLY_CLEAR_ALL_DTCS)) &&
           (Dem_ClientSelectionType_getTypeOfSelection(Dem_AllClientsState[ClientId].selection) != DEM_CLIENT_SELECTION_TYPE_ALL_DTCS))
        {
            Dem_ClientResultType_setResult(&Dem_AllClientsState[ClientId].result, Dem_AllClientsState[ClientId].request, DEM_WRONG_DTC);
            RetVal = DEM_WRONG_DTC;
        }
    }
    return RetVal;
}


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
