

#include "Dem_ClientBaseHandling.h"
#include "Dem_ClientRequestDispatcher.h"
#include "Dem_ClientMachine_Clear.h"
#include "Dem_ClientMachine_DTCRecordUpdate.h"
#include "Dem_ClientMachine_SelectED_FFData.h"
#include "Dem_EvMemApi.h"
#include "Dem_DTCGroup.h"
#include "Dem_DTCs.h"
#include "Dem_ClientMachine_Select.h"
#include "Dem_ClientMachine_DtcStatus.h"
#include "Dem_ClientMachineControlDTCSuppression.h"
#include "Dem_ClientMachineGetSuppression.h"


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

static void Dem_ClientRequestDispatcher_transferSelectionErrorToResult(Dem_ClientIdType ClientId, Dem_ClientRequestType request)
{
    Dem_ClientResultType_setResult(&Dem_AllClientsState[ClientId].result, request,
            Dem_ClientSelectionType_getSelectionResult(Dem_AllClientsState[ClientId].selection));
}

static void Dem_ClientRequestDispatcher_calculateSelection (Dem_ClientIdType ClientId)
{
    if (Dem_ClientSelectionType_isSelectionPending(Dem_Client_getClient(ClientId)->selection))
    {
        Dem_DtcIdType dtcId = DEM_DTCID_INVALID;
        uint8 typeOfSelection = DEM_CLIENT_SELECTION_TYPE_INVALID;
        Std_ReturnType selectionResult;

        if (!Dem_EvMemIsDtcOriginValid(&Dem_Client_getClient(ClientId)->data.standard.DTCOrigin))
        {
            selectionResult = DEM_WRONG_DTCORIGIN;
        }
        /* MR12 RULE 13.5 VIOLATION: Getter function is identified as an expression causing side effect. This warning can be ignored. */
        else if( (Dem_Client_getClient(ClientId)->data.standard.DTCFormat != DEM_DTC_FORMAT_OBD) && (Dem_Client_getClient(ClientId)->data.standard.DTCFormat != DEM_DTC_FORMAT_UDS) && (Dem_Client_getClient(ClientId)->data.standard.DTCFormat != DEM_DTC_FORMAT_J1939))
        {
            selectionResult = DEM_WRONG_DTC;
        }
        else
        {
            if (Dem_Client_getClient(ClientId)->data.standard.DTC == DEM_DTC_GROUP_ALL_DTCS)
            {
                typeOfSelection = DEM_CLIENT_SELECTION_TYPE_ALL_DTCS;
                selectionResult = E_OK;
            }
            else
            {
                dtcId = Dem_DtcGroupIdFromDtcGroupCode(Dem_Client_getClient(ClientId)->data.standard.DTC);
                if (dtcId != DEM_DTCGROUPID_INVALID)
                {
                    typeOfSelection = DEM_CLIENT_SELECTION_TYPE_GROUP_DTC;
                    selectionResult = E_OK;
                }
                else
                {
                    dtcId = Dem_DtcIdFromDtcCode(Dem_Client_getClient(ClientId)->data.standard.DTC);
                    /* MR12 RULE 13.5 VIOLATION: Getter function is identified as an expression causing side effect. This warning can be ignored. */
                    if ((!Dem_isDtcIdValid(dtcId)) || (Dem_Client_getClient(ClientId)->data.standard.DTCFormat == DEM_DTC_FORMAT_OBD))
                    {
                        selectionResult = DEM_WRONG_DTC;
                    }
                    else
                    {
                        typeOfSelection = DEM_CLIENT_SELECTION_TYPE_SINGLE_DTC;
                        selectionResult = E_OK;
                    }
                }
            }
        }
        Dem_ClientSelectionType_setSelection(&Dem_Client_getClient(ClientId)->selection, dtcId, typeOfSelection, selectionResult);
    }
}

void Dem_ClientRequestDispatcher_main(void)
{
    Dem_ClientIdListIterator ClientIdIt;
    uint8 ClientId;
    Dem_ClientRequestType tempRequest;
    boolean execute;

    for (Dem_Client_ClientIdIteratorNew(&ClientIdIt, DEM_CLIENT_ITERATORTYPE_ALL); Dem_Client_ClientIdIteratorValid(&ClientIdIt); Dem_Client_ClientIdIteratorNext(&ClientIdIt))
    {
        ClientId = Dem_Client_ClientIdIteratorCurrent(&ClientIdIt);
        tempRequest = Dem_AllClientsState[ClientId].request;

        if (Dem_ClientRequestType_isRequestInProgress(ClientId))
        {
            if ((Dem_ClientRequestType_isCancelRequested(tempRequest))
                    && ((Dem_ClientClearMachine_GetMachineActiveClient() != ClientId) || (Dem_ClientClearMachine_GetMachineState() == DEM_CLEAR_DTC_MACHINE_STATE_IDLE)))
            {
                Dem_ClientResultType_confirmCancel(&Dem_AllClientsState[ClientId].result, tempRequest);
            }
            else
            {
                execute = TRUE;
#if(DEM_CFG_J1939DCM != DEM_CFG_J1939DCM_OFF)
                if (Dem_Client_GetClientType(ClientId) != DEM_CLIENTTYPE_J1939)
#endif
                {
                    Dem_ClientRequestDispatcher_calculateSelection(ClientId);

                    if (Dem_ClientSelectionType_getSelectionResult(Dem_Client_getClient(ClientId)->selection) != E_OK)
                    {
                        Dem_ClientRequestDispatcher_transferSelectionErrorToResult(ClientId, tempRequest);
                        execute = FALSE;
                    }
                    else if(Dem_DtcIsSuppressed(Dem_ClientSelectionType_getSelectionDtcIndex (Dem_Client_getClient(ClientId)->selection)) &&
                            ((Dem_ClientRequestType_getMachineIndex(tempRequest) != (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_ENABLE_SUPPRESSION))) &&
                             (Dem_ClientRequestType_getMachineIndex(tempRequest) != (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_DISABLE_SUPPRESSION))) &&
                             (Dem_ClientRequestType_getMachineIndex(tempRequest) != (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_GET_SUPPRESSION)))))
                    {
                           Dem_ClientResultType_setResult(&Dem_Client_getClient(ClientId)->result, tempRequest, DEM_WRONG_DTC);
                           execute = FALSE;
                    }
                    else
                    {
                        /* nothing to do, just needed to sarisfy MISRA */
                    }
                }
                if (execute)
                {
                    if (Dem_ClientRequestType_getMachineIndex(tempRequest)
                            == (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_CLEAR)))
                    {
                        Dem_ClientMachineClear_SetClientRequest(ClientId);
                    }
#if ((DEM_CFG_SUPPRESSION == DEM_DTC_SUPPRESSION) || (DEM_CFG_SUPPRESSION == DEM_EVENT_AND_DTC_SUPPRESSION))
                    else if (Dem_ClientRequestType_getMachineIndex(tempRequest)
                            == (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_ENABLE_SUPPRESSION)))
                    {
                        Dem_ClientMachineEnableDTCSuppression_SetClientRequest(ClientId);
                    }
                    else if (Dem_ClientRequestType_getMachineIndex(tempRequest)
                            == (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_DISABLE_SUPPRESSION)))
                    {
                        Dem_ClientMachineDisableDTCSuppression_SetClientRequest(ClientId);
                    }
                    else if (Dem_ClientRequestType_getMachineIndex(tempRequest)
                            == (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_GET_SUPPRESSION)))
                    {
                        Dem_ClientMachineGetSuppression_SetClientRequest(ClientId);
                    }
#endif
                    else if ((Dem_ClientRequestType_getMachineIndex(tempRequest)
                            == (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_TRIGGERSELECTIONRESULT))))
                    {
                        Dem_ClientMachineSelectionResult_SetClientRequest(ClientId);
                    }
                    else if ((Dem_ClientRequestType_getMachineIndex(tempRequest)
                            == (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_GETSELECTIONRESULT4CLEAR))))
                    {
                        Dem_ClientMachineSelectionResult4Clear_SetClientRequest(ClientId);
                    }
                    else if ((Dem_ClientRequestType_getMachineIndex(tempRequest)
                            == (Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_GETSTATUS))))
                    {
                        Dem_ClientMachineGetStatus_SetClientRequest(ClientId);
                    }
                    else if(Dem_ClientRequestType_getMachineIndex(tempRequest) == Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_SELECT_ED_FF_DATA))
                    {
                        Dem_ClientMachine_SelectED_FFData_SetClientRequest(ClientId, &Dem_Client_getClient(ClientId)->result, tempRequest );
                    }
                    else if(Dem_ClientRequestType_getMachineIndex(tempRequest) == Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_DISABLERECORDUPDATE))
                    {
                        Dem_ClientMachine_DisableDTCRecordUpdate_SetClientRequest(&Dem_Client_getClient(ClientId)->result, tempRequest );
                    }
                    else if(Dem_ClientRequestType_getMachineIndex(tempRequest) == Dem_ClientRequestType_getMachineIndex(DEM_CLIENT_REQUEST_ENABLERECORDUPDATE))
                    {
                        Dem_ClientMachine_EnableDTCRecordUpdate_SetClientRequest(&Dem_Client_getClient(ClientId)->result, tempRequest );
                    }
					else
					{
						/* Code will not reach this part*/
					}

                }
            }
        }
    }

    Dem_ClearMainFunction();
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
