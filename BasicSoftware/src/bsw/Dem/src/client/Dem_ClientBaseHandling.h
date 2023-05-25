

#ifndef DEM_CLIENTBASEHANDLING_H
#define DEM_CLIENTBASEHANDLING_H

#include "Dem_ClientHandlingTypes.h"
#include "Dem_EnvExtendedData.h"
#include "Dem_Client.h"
#include "Dem_Lib.h"
#include "Rte_Dem_Type.h"



#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/**************************************************************************************************/
/*********  MASKS : Dem_ClientRequestType, Dem_ClientResultType & Dem_ClientSelectionType *********/

#define DEM_CLIENT_TYPE_MASK     0xFFu

/**************************************************************************************************/
/****************  MASKS : Dem_ClientRequestType & Dem_ClientResultType ***************************/

#define DEM_CLIENT_REQUEST_RESULT_IDMASK       0xFF00u

/**************************************************************************************************/
/****************  Dem_ClientSelectED_FFDataType  *******************************************************/

// EvMem Copy Valid
DEM_INLINE void Dem_ClientSelectED_FFDataType_SetEvMemCopyValid(Dem_ClientIdType ClientId, boolean IsCopyValid )
{
    Dem_AllClientsState[ClientId].data.standard.SelectED_FFData.IsCopyValid = IsCopyValid;
}

DEM_INLINE boolean Dem_ClientSelectED_FFDataType_IsEvMemCopyValid( Dem_ClientIdType ClientId )
{
    return (Dem_AllClientsState[ClientId].data.standard.SelectED_FFData.IsCopyValid);
}

//DTC Record Update
DEM_INLINE void Dem_Client_SetDTCRecordUpdateDisabled(Dem_ClientIdType ClientId, boolean Disabled )
{
    Dem_AllClientsState[ClientId].data.standard.IsDTCRecordUpdateDisabled = Disabled;
}

DEM_INLINE boolean Dem_Client_IsDTCRecordUpdateDisabled( Dem_ClientIdType ClientId )
{
    return (Dem_AllClientsState[ClientId].data.standard.IsDTCRecordUpdateDisabled);
}

//EvMem Reader Copy
DEM_INLINE void Dem_ClientSelectED_FFDataType_SaveEvMemCopy( Dem_ClientIdType ClientId, const Dem_EvMemEventMemoryType* EvMem )
{
    Dem_AllClientsState[ClientId].data.standard.SelectED_FFData.EvMemCopy = *EvMem ;
}

DEM_INLINE Dem_EvMemEventMemoryType* Dem_ClientSelectED_FFDataType_GetEvMemCopy(Dem_ClientIdType ClientId)
{
    return ( &Dem_AllClientsState[ClientId].data.standard.SelectED_FFData.EvMemCopy );
}

DEM_INLINE void Dem_ClientSetED_FFSelectionPending(Dem_ClientIdType ClientId, boolean SetBit)
{
    Dem_AllClientsState[ClientId].data.standard.IsED_FFSelectionPending = SetBit;
}

DEM_INLINE boolean Dem_ClientIsED_FFSelectionPending(Dem_ClientIdType ClientId)
{
    return Dem_AllClientsState[ClientId].data.standard.IsED_FFSelectionPending;
}

DEM_INLINE void Dem_ClientSetGetNextDataCalled(Dem_ClientIdType ClientId, boolean SetBit)
{
    Dem_AllClientsState[ClientId].data.standard.IsGetNextDataCalled = SetBit;
}

DEM_INLINE boolean Dem_ClientIsGetNextDataCalled(Dem_ClientIdType ClientId)
{
    return Dem_AllClientsState[ClientId].data.standard.IsGetNextDataCalled;
}

/**************************************************************************************************/
/****************  Dem_ClientResultType  **********************************************************/

DEM_INLINE void Dem_ClientResultType_setResult(volatile Dem_ClientResultType *result, Dem_ClientRequestType request, Std_ReturnType newResult)
{
    Dem_ClientResultType tempResult;

    /* MR12 RULE 14.3 VIOLATION: The operand depends on configuration and is not a constant. */
    DEM_STATIC_ASSERT(((DEM_SIZEOF_TYPE(Dem_ClientResultType)) >= 2u), DEM_CLIENTRSULTTYPE_SHALL_BE_UINT16);

    tempResult = (request & DEM_CLIENT_REQUEST_RESULT_IDMASK);
    tempResult |= newResult;
    *result = tempResult;
}

DEM_INLINE Std_ReturnType Dem_ClientResultType_getResult(Dem_ClientResultType result)
{
    return ((Std_ReturnType)(result & DEM_CLIENT_TYPE_MASK));
}

DEM_INLINE Std_ReturnType Dem_ClientResultType_getRequest(Dem_ClientRequestType request)
{
    return ((Std_ReturnType)(request & DEM_CLIENT_TYPE_MASK));
}

DEM_INLINE void Dem_ClientResultType_confirmCancel(volatile Dem_ClientResultType *result, Dem_ClientRequestType request)
{
    Dem_ClientResultType tempResult;

    /* MR12 RULE 14.3 VIOLATION: The operand depends on configuration and is not a constant. */
    DEM_STATIC_ASSERT(((DEM_SIZEOF_TYPE(Dem_ClientResultType)) >= 2u), DEM_CLIENTRSULTTYPE_SHALL_BE_UINT16);

    tempResult = (request & DEM_CLIENT_REQUEST_RESULT_IDMASK);
    tempResult |= E_NOT_OK;
    *result = tempResult;
}

/**************************************************************************************************/
/****************  Dem_ClientRequestType  *********************************************************/

#define DEM_CLIENT_REQUEST_TYPE_CANCEL_MASK     0x01u

DEM_INLINE boolean Dem_ClientRequestType_isRequestInProgress(Dem_ClientIdType clientId)
{
    return ((Dem_AllClientsState[clientId].request & DEM_CLIENT_REQUEST_RESULT_IDMASK)
            != (Dem_AllClientsState[clientId].result & DEM_CLIENT_REQUEST_RESULT_IDMASK));
}

DEM_INLINE uint8 Dem_ClientRequestType_getMachineIndex(Dem_ClientRequestType request)
{
    uint8 machineIndex;
    machineIndex = (((uint8)(request & DEM_CLIENT_TYPE_MASK)) >> 1u);
    return machineIndex;
}

DEM_INLINE void Dem_ClientRequestType_setRequest(volatile Dem_ClientRequestType *request, uint8 newRequest)
{
    Dem_ClientRequestType tempRequest;

    /* MR12 RULE 14.3 VIOLATION: The operand depends on configuration and is not a constant. */
    DEM_STATIC_ASSERT(((DEM_SIZEOF_TYPE(Dem_ClientRequestType) >= 2u)), DEM_CLIENTREQUESTTYPE_SHALL_BE_UINT16);

    tempRequest = *request;
    tempRequest += 0x0100u;
    tempRequest = (tempRequest & DEM_CLIENT_REQUEST_RESULT_IDMASK) | newRequest;
    *request = tempRequest;
}


DEM_INLINE void Dem_ClientRequestType_cancelRequest(volatile Dem_ClientRequestType *request)
{
    Dem_ClientRequestType_setRequest(request,1);
}


DEM_INLINE boolean Dem_ClientRequestType_isCancelRequested(Dem_ClientRequestType request)
{
    return ((request & DEM_CLIENT_REQUEST_TYPE_CANCEL_MASK) == 1u);
}

/**************************************************************************************************/
/****************  Dem_ClientSelectionType  *******************************************************/

#define DEM_CLIENT_SELECTION_DTCINDEXMASK                0xFFFFu
#define DEM_CLIENT_INVALIDATE_SELECTION_IDMASK           0x00FFFFFFu
#define DEM_CLIENT_SELECTION_TYPE_BITPOS_SELECTION_TYPE  16u
#define DEM_CLIENT_SELECTION_TYPE_BITPOS_RESULT          24u

DEM_INLINE uint16 Dem_ClientSelectionType_getSelectionDtcIndex (Dem_ClientSelectionType selection)
{
    return ((uint16)(selection & DEM_CLIENT_SELECTION_DTCINDEXMASK));
}

DEM_INLINE void Dem_ClientSelectionType_setSelection (Dem_ClientSelectionType *selection, uint16 dtcId, uint8 typeOfSelection, Std_ReturnType selectionResult)
{
    Dem_ClientSelectionType tempSelection;

    /* MR12 RULE 14.3 VIOLATION: The operand depends on configuration and is not a constant. */
    DEM_STATIC_ASSERT(((DEM_SIZEOF_TYPE(Dem_ClientSelectionType) >= 4u)), DEM_CLIENTSELECTIONTYPE_SHALL_BE_UINT32);

    tempSelection = (((uint32)dtcId) | (((uint32)typeOfSelection) << DEM_CLIENT_SELECTION_TYPE_BITPOS_SELECTION_TYPE) | (((uint32)selectionResult) << DEM_CLIENT_SELECTION_TYPE_BITPOS_RESULT));
    *selection = tempSelection;
}

DEM_INLINE boolean Dem_ClientSelectionType_isSelectionSingleDTC (Dem_ClientSelectionType selection)
{
    uint8 tempSelectionType;
    tempSelectionType = (uint8)((selection >> DEM_CLIENT_SELECTION_TYPE_BITPOS_SELECTION_TYPE) & DEM_CLIENT_TYPE_MASK);

    return (tempSelectionType == DEM_CLIENT_SELECTION_TYPE_SINGLE_DTC);
}

DEM_INLINE boolean Dem_ClientSelectionType_isSelectionDTCGroupAll (Dem_ClientSelectionType selection)
{
    uint8 tempSelectionType;
    tempSelectionType = (uint8)((selection >> DEM_CLIENT_SELECTION_TYPE_BITPOS_SELECTION_TYPE) & DEM_CLIENT_TYPE_MASK);

    return (tempSelectionType == DEM_CLIENT_SELECTION_TYPE_ALL_DTCS);
}

DEM_INLINE boolean Dem_ClientSelectionType_isSelectionDTCGroup (Dem_ClientSelectionType selection)
{
    uint8 tempSelectionType;
    tempSelectionType = (uint8)((selection >> DEM_CLIENT_SELECTION_TYPE_BITPOS_SELECTION_TYPE) & DEM_CLIENT_TYPE_MASK);

    return (tempSelectionType == DEM_CLIENT_SELECTION_TYPE_GROUP_DTC);
}

DEM_INLINE boolean Dem_ClientSelectionType_isSelectionPending (Dem_ClientSelectionType selection)
{
    uint8 tempSelectionResult;
    tempSelectionResult = (uint8)((selection >> DEM_CLIENT_SELECTION_TYPE_BITPOS_RESULT) & DEM_CLIENT_TYPE_MASK);

    return (tempSelectionResult == DEM_PENDING);
}

DEM_INLINE void Dem_ClientSelectionType_invalidateSelectionResult (Dem_ClientSelectionType *selection)
{
    Std_ReturnType tempresult = DEM_PENDING;
    Dem_ClientSelectionType tempSelection;

    /* MR12 RULE 14.3 VIOLATION: The operand depends on configuration and is not a constant. */
    DEM_STATIC_ASSERT(((DEM_SIZEOF_TYPE(Dem_ClientSelectionType) >= 4u)), DEM_CLIENTSELECTIONTYPE_SHALL_BE_UINT32);

    tempSelection = (*selection & DEM_CLIENT_INVALIDATE_SELECTION_IDMASK);
    tempSelection |= (((uint32)tempresult) << DEM_CLIENT_SELECTION_TYPE_BITPOS_RESULT);
    *selection = tempSelection;
}

DEM_INLINE Std_ReturnType Dem_ClientSelectionType_getSelectionResult (Dem_ClientSelectionType selection)
{
    return ((Std_ReturnType)((selection >> DEM_CLIENT_SELECTION_TYPE_BITPOS_RESULT) & DEM_CLIENT_TYPE_MASK));
}

DEM_INLINE uint8 Dem_ClientSelectionType_getTypeOfSelection (Dem_ClientSelectionType selection)
{
    return ((uint8)((selection >> DEM_CLIENT_SELECTION_TYPE_BITPOS_SELECTION_TYPE) & DEM_CLIENT_TYPE_MASK));
}

DEM_INLINE boolean Dem_Client_IsSelectionResultAvailable(uint8 ClientId, uint8 requestId)
{
    return (((requestId == DEM_CLIENT_REQUEST_TRIGGERSELECTIONRESULT) || (requestId == DEM_CLIENT_REQUEST_DISABLERECORDUPDATE) ||
         (requestId == DEM_CLIENT_REQUEST_ENABLERECORDUPDATE) || (requestId == DEM_CLIENT_REQUEST_GETSELECTIONRESULT4CLEAR) ||
         (requestId == DEM_CLIENT_REQUEST_GET_SUPPRESSION))
        && (Dem_ClientSelectionType_getSelectionResult(Dem_AllClientsState[ClientId].selection) != DEM_PENDING));
}

/**************************************************************************************************/

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
