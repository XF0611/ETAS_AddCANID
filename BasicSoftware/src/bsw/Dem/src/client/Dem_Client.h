

#ifndef DEM_CLIENT_H
#define DEM_CLIENT_H

#include "Dem_ClientHandlingTypes.h"
#include "Dem_Array.h"
#include "Dem_EvMemTypes.h"
#include "Dem_EnvRecordIterator.h"
#include "Dem_Lib.h"

/******************************************************************************************/
/**************** Dem Clients  ***************************************************************/
typedef struct
{
    boolean IsCopyValid;    // Indicates that Evmem Copy was taken or not(Eg: Because of DTC not being stored)
    Dem_EvMemEventMemoryType EvMemCopy;
} Dem_ClientSelectED_FFDataType;

typedef struct
{
    Dem_DTCFormatType DTCFormat;
    Dem_DTCOriginType DTCOrigin;
    boolean IsDTCRecordUpdateDisabled;
    uint8 SelectED_FF_MachineState;
    boolean IsED_FFSelectionPending;
    boolean IsGetNextDataCalled;
    volatile uint8 DTCStatus;
    uint32 DTC;
    Dem_ClientSelectED_FFDataType SelectED_FFData;
    Dem_EnvRecordIteratorType SelectED_FFIt;
} Dem_ClientState_Standard;

typedef struct
{
#if(DEM_CFG_J1939DCM_CLEAR_SUPPORT != DEM_CFG_J1939DCM_OFF)
    Dem_J1939DcmSetClearFilterType J1939DTCTypeFilter;
    uint8 J1939node;
#else
    uint8 Dem_Dummy;     /* dummy variable to avoid empty structure error in some compilers in case J1939 is not supported*/
#endif
} Dem_ClientState_J1939;

typedef struct
{
    volatile uint8 client_state;
    volatile Dem_ClientRequestType request;
    volatile Dem_ClientResultType result;
    Dem_ClientSelectionType selection;
    union
    {
        Dem_ClientState_Standard standard;
        Dem_ClientState_J1939 j1939;
    } data;

} Dem_ClientState;

#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

DEM_ARRAY_DECLARE(Dem_ClientState, Dem_AllClientsState, DEM_CLIENTID_ARRAYLENGTH_TOTAL);

#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

typedef struct {
    Dem_ClientIdType it;
    Dem_ClientIdType end;
} Dem_ClientIdListIterator;

#define DEM_CLIENT_ITERATORTYPE_STD   0x01
#define DEM_CLIENT_ITERATORTYPE_ALL   0x02


DEM_INLINE void Dem_Client_ClientIdIteratorNew(Dem_ClientIdListIterator *ClientIdIt, uint8 iteratortype)
{

    if (iteratortype == DEM_CLIENT_ITERATORTYPE_STD)/*loop through Standard clients*/
    {
        ClientIdIt->it = 1;
        ClientIdIt->end = DEM_CLIENTID_ARRAYLENGTH_STD;
    }
    else if (iteratortype == DEM_CLIENT_ITERATORTYPE_ALL)/*loop through Standard and J1939 clients*/
    {
        ClientIdIt->it = 1;
        ClientIdIt->end = DEM_CLIENTID_ARRAYLENGTH_TOTAL;
    }
    /* Note: Below code snippet can be enabled or uncommented for J1939 functionality
     *  to loop over only J1939 clients with DEM_CLIENT_ITERATORTYPE_J1939 as 0x03
     * This shall be adapted when J1939 related changes are done as per AR4.3.1
     */
    /*else if(loop == DEM_CLIENT_ITERATORTYPE_J1939)loop through J1939 clients
     {
     ClientIdIt->it = DEM_CLIENTID_ARRAYLENGTH_STD;
     ClientIdIt->end = DEM_CLIENTID_ARRAYLENGTH_TOTAL;
     }*/
    else
    {
        /* MR12 RULE 14.3 VIOLATION: MACRO used to throw DET error on entering unintended else part.*/
        DEM_ASSERT(FALSE, DEM_DET_APIID_CLIENTIDITERATOR, 0);
    }
}

DEM_INLINE Dem_boolean_least Dem_Client_ClientIdIteratorValid(const Dem_ClientIdListIterator *ClientIdIt)
{
    return ((Dem_boolean_least)(ClientIdIt->it < ClientIdIt->end ));
}

DEM_INLINE void Dem_Client_ClientIdIteratorNext(Dem_ClientIdListIterator *ClientIdIt)
{
    (ClientIdIt->it)++;
}

DEM_INLINE Dem_ClientIdType Dem_Client_ClientIdIteratorCurrent(const Dem_ClientIdListIterator *ClientIdIt)
{
   return (Dem_ClientIdType)(ClientIdIt->it);
}

/* ClientId Validation check*/
DEM_INLINE Dem_boolean_least Dem_isClientIdValid(Dem_ClientIdType clientId)
{
   return ((clientId != DEM_CLIENTID_INVALID) && (clientId < DEM_CLIENTID_ARRAYLENGTH_TOTAL));
}

void Dem_ClientInit(void);

DEM_INLINE Dem_ClientState* Dem_Client_getClient (Dem_ClientIdType ClientId)
{
    return (&Dem_AllClientsState[ClientId]);
}

DEM_INLINE uint8 Dem_Client_GetClientType (Dem_ClientIdType ClientId)
{
    return((Dem_isClientIdValid(ClientId))?((ClientId <= DEM_CLIENTS_COUNT_STANDARD)?DEM_CLIENTTYPE_STANDARD:DEM_CLIENTTYPE_J1939):DEM_CLIENTTYPE_INVALID);
}

Std_ReturnType Dem_Client_Operation(uint8 ClientId, uint8 requestId, uint8 ApiId);

#if(DEM_CFG_J1939DCM_CLEAR_SUPPORT != DEM_CFG_J1939DCM_OFF)
Std_ReturnType Dem_J1939DcmClearDTC_4_3 (Dem_J1939DcmSetClearFilterType DTCTypeFilter , uint8 node, uint8 ClientId);
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
