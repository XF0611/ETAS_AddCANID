

#ifndef DEM_CLIENTMACHINE_DTCSTATUS_H
#define DEM_CLIENTMACHINE_DTCSTATUS_H

#include "Dem_DTCStatusByte.h"
#include "Dem_DTCs.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"



DEM_INLINE void  Dem_ClientMachineGetStatus_SetClientRequest(Dem_ClientIdType clientId)
{
    boolean DtcStatusIsValid;
    uint8 newDTCStatus;
    Dem_ClientState *client = Dem_Client_getClient(clientId);

    if (Dem_ClientSelectionType_isSelectionSingleDTC(client->selection)
            && Dem_DtcUsesOrigin(Dem_ClientSelectionType_getSelectionDtcIndex(client->selection), client->data.standard.DTCOrigin))
    {

        newDTCStatus = Dem_DtcStatusByteRetrieveWithOrigin(Dem_ClientSelectionType_getSelectionDtcIndex(client->selection),
                client->data.standard.DTCOrigin, &DtcStatusIsValid) & ((uint8) DEM_CFG_DTCSTATUS_AVAILABILITYMASK);

        if (!DtcStatusIsValid)
        {
            /* MR12 RULE 13.2 VIOLATION: The order of access of volatile objects is not significant. Therefore this warning can be ignored.*/
            Dem_ClientResultType_setResult(&client->result, client->request, DEM_NO_SUCH_ELEMENT);
        }
        else
        {
            client->data.standard.DTCStatus = newDTCStatus;
            /* MR12 RULE 13.2 VIOLATION: The order of access of volatile objects is not significant. Therefore this warning can be ignored.  */
            Dem_ClientResultType_setResult(&client->result, client->request, E_OK);
        }
    }
    else
    {   /* MR12 RULE 13.2 VIOLATION: The order of access of volatile objects is not significant. Therefore this warning can be ignored.  */
        Dem_ClientResultType_setResult(&client->result, client->request, DEM_WRONG_DTC);
    }
}


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


#endif /* DEM_CLIENTMACHINE_DTCSTATUS_H */
