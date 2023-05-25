

#include "Dem_Client.h"
#include "Dem_ClientBaseHandling.h"
#include "Dem_ClientMachine_Clear.h"
#include "Dem_ClientRequestDispatcher.h"
#include "Dem_Clear.h"
#include "Dem_Nvm.h"
#include "Dem_Lock.h"
#include "Dem_EvMemApi.h"
#include "Dem_Cfg_EvMem.h"
#include "Dem_Cfg_ExtPrototypes.h"
#include "Std_Types.h"
#include "Dem_Dcm_Internal.h"
#include "Dem_Internal.h"
#include "Rte_Dem.h"


#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

DEM_ARRAY_DEFINE (Dem_ClientState, Dem_AllClientsState, DEM_CLIENTID_ARRAYLENGTH_TOTAL);

#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"


static void Dem_Client_SetParameters(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    Dem_AllClientsState[ClientId].data.standard.DTC       = DTC;
    Dem_AllClientsState[ClientId].data.standard.DTCFormat = DTCFormat;
    Dem_AllClientsState[ClientId].data.standard.DTCOrigin = DTCOrigin;
}

static void Dem_Client_SetClientState(Dem_ClientIdType ClientId, uint8 state)
{
    Dem_AllClientsState[ClientId].client_state = state;
}

static uint8 Dem_Client_GetClientState(Dem_ClientIdType ClientId)
{
    return Dem_AllClientsState[ClientId].client_state;
}



void Dem_ClientInit(void)
{
    uint8 ClientId;
    Dem_ClientIdListIterator ClientIdIt;

    for (Dem_Client_ClientIdIteratorNew(&ClientIdIt, DEM_CLIENT_ITERATORTYPE_ALL); Dem_Client_ClientIdIteratorValid(&ClientIdIt); Dem_Client_ClientIdIteratorNext(&ClientIdIt))
    {
        ClientId= Dem_Client_ClientIdIteratorCurrent(&ClientIdIt);
        Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_INIT);
        /*initialization of standard client elements*/
        if (Dem_Client_GetClientType(ClientId)== DEM_CLIENTTYPE_STANDARD)
        {
            Dem_Client_FF_FilterParams[ClientId].FilteredRecordLocIdIterator = DEM_EVMEM_INVALID_LOCID;
        }
        else /*initialization of J1939 client elements*/
        {
#if(DEM_CFG_J1939DCM_CLEAR_SUPPORT != DEM_CFG_J1939DCM_OFF)
        Dem_AllClientsState[ClientId].data.j1939.J1939DTCTypeFilter = DEM_J1939DTC_INVALID_FILTER;
#endif
        }
    }
}

/**************************************************************************************************/
/****************  Dem_SelectDTC ******************************************************************/

#if(DEM_CFG_J1939DCM_CLEAR_SUPPORT != DEM_CFG_J1939DCM_OFF)
static void Dem_Client_SetJ1939Parameters(Dem_J1939DcmSetClearFilterType DTCTypeFilter, uint8 node, uint8 ClientId)
{
    Dem_AllClientsState[ClientId].data.j1939.J1939DTCTypeFilter    = DTCTypeFilter;
    Dem_AllClientsState[ClientId].data.j1939.J1939node             = node;
}

static Std_ReturnType Dem_SelectJ1939Parameters(Dem_J1939DcmSetClearFilterType DTCTypeFilter, uint8 node, uint8 ClientId)
{
    /* Set the Client Parameters */
    if (Dem_Client_GetClientState(ClientId) == DEM_CLIENT_STATE_INIT)
    {
        if (!(Dem_ClientRequestType_isRequestInProgress(ClientId)))
        {
            Dem_Client_SetJ1939Parameters(DTCTypeFilter, node, ClientId);
            Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_PARAMETERS_SET);
            return E_OK;
        }
        else
        {
            return DEM_BUSY;
        }
    }
    /* MR12 RULE 13.5 VIOLATION: Getter function is identified as an expression causing side effect. This warning can be ignored. */
    else if ((Dem_Client_GetClientState(ClientId) == DEM_CLIENT_STATE_PARAMETERS_SET)
            && (! Dem_ClientRequestType_isRequestInProgress(ClientId)))
    {
        Dem_Client_SetJ1939Parameters(DTCTypeFilter, node, ClientId);
        Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_PARAMETERS_SET);
        return E_OK;
    }
    else if (Dem_Client_GetClientState(ClientId) == DEM_CLIENT_STATE_REQUESTED_OPERATION)
    {
        Dem_ClientRequestType_cancelRequest(&Dem_AllClientsState[ClientId].request);
        Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_INIT);
        return DEM_BUSY;
    }
    else
    {
        /* should never occur */
        DEM_ASSERT(Dem_LibGetParamBool(FALSE), DEM_DET_APIID_DEM_SELECTDTC, 0);
        return DEM_BUSY;
    }
}
#endif

Std_ReturnType Dem_SelectDTC(uint8 ClientId, uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
    if (!Dem_isClientIdValid(ClientId))
    {
        return DEM_WRONG_CLIENTID;
    }

    /* Set the Client Parameters */
    if (Dem_Client_GetClientState(ClientId) == DEM_CLIENT_STATE_INIT)
    {
        if (!(Dem_ClientRequestType_isRequestInProgress(ClientId)))
        {
            Dem_Client_SetParameters(ClientId, DTC, DTCFormat, DTCOrigin);
            Dem_ClientSelectionType_invalidateSelectionResult(&Dem_AllClientsState[ClientId].selection);
            Dem_ClientSetED_FFSelectionPending(ClientId, TRUE);
            Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_PARAMETERS_SET);
            return E_OK;
        }
        else
        {
            return DEM_BUSY;
        }
    }
    else if ((Dem_Client_GetClientState(ClientId) == DEM_CLIENT_STATE_PARAMETERS_SET))
    {
        Dem_Client_SetParameters(ClientId, DTC, DTCFormat, DTCOrigin);
        Dem_ClientSetED_FFSelectionPending(ClientId, TRUE);
        Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_PARAMETERS_SET);
        Dem_ClientSelectionType_invalidateSelectionResult(&Dem_AllClientsState[ClientId].selection);
        return E_OK;
    }
    else if (Dem_Client_GetClientState(ClientId) == DEM_CLIENT_STATE_REQUESTED_OPERATION)
    {
        Dem_ClientRequestType_cancelRequest(&Dem_AllClientsState[ClientId].request);
        Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_INIT);
        return DEM_BUSY;
    }
    else
    {
        /* should never occur */
        DEM_ASSERT(Dem_LibGetParamBool(FALSE), DEM_DET_APIID_DEM_SELECTDTC, 0);
        return DEM_BUSY;
    }
}


/**************************************************************************************************/
/****************  ClearDTC **********************************************************************/

Std_ReturnType Dem_ClearDTC(uint8 ClientId)
{
    return Dem_Client_Operation(ClientId, DEM_CLIENT_REQUEST_CLEAR, DEM_DET_APIID_CLEARDTC);
}

#if(DEM_CFG_J1939DCM_CLEAR_SUPPORT != DEM_CFG_J1939DCM_OFF)

static boolean Dem_J1939DcmIsDTCTypeFilterValid(Dem_J1939DcmSetClearFilterType DTCTypeFilter)
{
    return ((DTCTypeFilter == DEM_J1939DTC_CLEAR_ALL) || (DTCTypeFilter == DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE));
}

static boolean Dem_Client_AreJ1939ParametersAlreadyRequested(Dem_J1939DcmSetClearFilterType DTCTypeFilter, uint8 node, uint8 ClientId)
{
    return ((Dem_AllClientsState[ClientId].data.j1939.J1939DTCTypeFilter == DTCTypeFilter) && (Dem_AllClientsState[ClientId].data.j1939.J1939node == node));
}

Std_ReturnType Dem_J1939DcmClearDTC_4_3(Dem_J1939DcmSetClearFilterType DTCTypeFilter, uint8 node, uint8 ClientId)
{
    Std_ReturnType returnSts = E_OK;

    if (Dem_Client_GetClientType(ClientId) != DEM_CLIENTTYPE_J1939)
    {
        return DEM_WRONG_CLIENTID;
    }

    if (!Dem_J1939DcmIsDTCTypeFilterValid(DTCTypeFilter))
    {
        return DEM_CLEAR_FAILED;
    }

    if (!Dem_J1939IsNodeIdValid(node))
    {
        return DEM_CLEAR_FAILED;
    }

    if (!Dem_Client_AreJ1939ParametersAlreadyRequested(DTCTypeFilter, node, ClientId))
    {
        returnSts = Dem_SelectJ1939Parameters(DTCTypeFilter, node, ClientId);
        if (returnSts != E_OK)
        {
            return returnSts;
        }
    }

    if (Dem_Client_GetClientState(ClientId) == DEM_CLIENT_STATE_PARAMETERS_SET)
    {
        Dem_ClientRequestType_setRequest(&Dem_AllClientsState[ClientId].request, DEM_CLIENT_REQUEST_CLEAR);
        Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_REQUESTED_OPERATION);

        return DEM_PENDING;
    }
    /* MR12 RULE 13.5 VIOLATION: Getter function is identified as an expression causing side effect. This warning can be ignored. */
    else if ((Dem_Client_GetClientState(ClientId) == DEM_CLIENT_STATE_REQUESTED_OPERATION)
            || (Dem_Client_GetClientState(ClientId) == DEM_CLIENT_STATE_INIT))
    {
        if (!(Dem_ClientRequestType_isRequestInProgress(ClientId)))
        {
            Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_PARAMETERS_SET);
            return Dem_ClientResultType_getResult(Dem_AllClientsState[ClientId].result);
        }
        else
        {
            return DEM_PENDING;
        }
    }
    else
    {
        /* should never occur */
        DEM_ASSERT(Dem_LibGetParamBool(FALSE), DEM_DET_APIID_DEM_J1939DcmClearDTC, 0);
        return DEM_CLEAR_FAILED;
    }
}

#endif


/* MR12 RULE 2.7 VIOLATION: Based on the config of the macro using the parameter ApiId it might not be used in the function*/
Std_ReturnType Dem_Client_Operation(uint8 ClientId, uint8 requestId, uint8 ApiId)
{
    Std_ReturnType status = E_NOT_OK;

    if (!Dem_isClientIdValid(ClientId))
    {
        DEM_DET(ApiId, DEM_E_WRONG_CONFIGURATION,0u);
        status = DEM_WRONG_CLIENTID;
    }

    else
    {
        switch (Dem_Client_GetClientState(ClientId))
        {
            case DEM_CLIENT_STATE_INIT:
            {
                status = E_NOT_OK;
                DEM_DET(ApiId, DEM_E_WRONG_CONDITION,0u);
                break;
            }
            case DEM_CLIENT_STATE_PARAMETERS_SET:
            {
                if (Dem_Client_IsSelectionResultAvailable(ClientId, requestId))
                {
                    return Dem_ClientSelectionType_getSelectionResult(Dem_AllClientsState[ClientId].selection);
                }
                else
                {
                Dem_ClientRequestType_setRequest(&Dem_AllClientsState[ClientId].request, requestId);
                    Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_REQUESTED_OPERATION);
                    status = DEM_PENDING;
                }

                break;
            }
            case DEM_CLIENT_STATE_REQUESTED_OPERATION:
            {
                if ((Dem_ClientResultType_getRequest(Dem_AllClientsState[ClientId].request)) == requestId)
                {
                    if (!Dem_ClientRequestType_isRequestInProgress(ClientId))
                    {
                        Dem_Client_SetClientState(ClientId, DEM_CLIENT_STATE_PARAMETERS_SET);
                        status = Dem_ClientResultType_getResult(Dem_AllClientsState[ClientId].result);
                    }
                    else
                    {
                        status = DEM_PENDING;
                    }
                }
                else
                {
                    status = DEM_BUSY;
                }
                break;
            }
            default:
            {
                status = E_NOT_OK;
                break;
            }
        }
    }
    return status;
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
