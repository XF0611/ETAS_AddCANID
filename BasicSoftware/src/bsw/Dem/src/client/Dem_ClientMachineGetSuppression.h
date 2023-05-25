

#ifndef DEM_CLIENTMACHINEGETSUPPRESSION_H
#define DEM_CLIENTMACHINEGETSUPPRESSION_H

#if ((DEM_CFG_SUPPRESSION == DEM_DTC_SUPPRESSION) || (DEM_CFG_SUPPRESSION == DEM_EVENT_AND_DTC_SUPPRESSION))
DEM_INLINE void Dem_ClientMachineGetSuppression_SetClientRequest(Dem_ClientIdType clientId)
{
    Dem_ClientRequestType tempRequest;
    tempRequest = Dem_AllClientsState[clientId].request;

    if (Dem_ClientSelectionType_isSelectionSingleDTC(Dem_Client_getClient(clientId)->selection))
    {
       Dem_ClientResultType_setResult(&Dem_Client_getClient(clientId)->result,tempRequest,E_OK);
    }
    else
    {
        Dem_ClientResultType_setResult(&Dem_Client_getClient(clientId)->result, tempRequest,DEM_WRONG_DTC);
    }
}
#endif
#endif /* DEM_CLIENTMACHINEGETSUPPRESSION_H */
