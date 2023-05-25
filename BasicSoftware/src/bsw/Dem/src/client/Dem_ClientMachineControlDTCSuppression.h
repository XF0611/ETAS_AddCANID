

#ifndef DEM_CLIENTMACHINECONTROLDTCSUPPRESSION_H
#define DEM_CLIENTMACHINECONTROLDTCSUPPRESSION_H

#if ((DEM_CFG_SUPPRESSION == DEM_DTC_SUPPRESSION) || (DEM_CFG_SUPPRESSION == DEM_EVENT_AND_DTC_SUPPRESSION))
DEM_INLINE void Dem_ClientMachineControlDTCSuppression(uint8 ClientId, boolean SupressionStatus)
{
    Dem_ClientRequestType tempRequest;
    tempRequest = Dem_AllClientsState[ClientId].request;

    if (Dem_ClientSelectionType_isSelectionSingleDTC(Dem_Client_getClient(ClientId)->selection))
    {
        Dem_DtcIdType dtcId = Dem_DtcIdFromDtcCode(Dem_Client_getClient(ClientId)->data.standard.DTC);
       if(Dem_IsEventMemoryEntryExistForDTC(dtcId))
       {
           Dem_ClientResultType_setResult(&Dem_Client_getClient(ClientId)->result,tempRequest,E_NOT_OK);
       }
       else
       {
           Dem_DtcSuppressionApply(dtcId, SupressionStatus);
           Dem_ClientResultType_setResult(&Dem_Client_getClient(ClientId)->result,tempRequest,E_OK);
       }
    }
    else
    {
        Dem_ClientResultType_setResult(&Dem_Client_getClient(ClientId)->result, tempRequest,
                                        DEM_WRONG_DTC);
    }
}

DEM_INLINE void Dem_ClientMachineEnableDTCSuppression_SetClientRequest(Dem_ClientIdType ClientId)
{
            Dem_ClientMachineControlDTCSuppression(ClientId, TRUE);
}

DEM_INLINE void Dem_ClientMachineDisableDTCSuppression_SetClientRequest(Dem_ClientIdType ClientId)
{
            Dem_ClientMachineControlDTCSuppression(ClientId, FALSE);
}
#endif

#endif /* DEM_CLIENTMACHINECONTROLDTCSUPPRESSION_H */
