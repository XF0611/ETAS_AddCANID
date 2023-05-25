

#include "Dem_Dcm_Internal.h"
#include "Dem_DTCFilter.h"
#include "Dem_Cfg_DTC_DataStructures.h"
#include "Dem_Cfg_DTCs.h"
#include "Dem_BitArray.h"
#include "Dem_Obd.h"
#include "Dem_ChronoSort.h"

typedef struct
{
	/* state machine */
	volatile boolean start;
	volatile boolean exec;
	/* output parameter */
	volatile uint8   DTCStatus;
	/* input parameter */
	volatile Dem_DTCOriginType DTCOrigin;
	volatile Dem_DtcIdType DtcId;
} Dem_DTCFilterSyncState;

#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"
	DEM_BITARRAY_DEFINE(Dem_DTCFilterMatching[DEM_CLIENTID_ARRAYLENGTH_STD],DEM_DTCID_ARRAYLENGTH);
	static Dem_DTCFilterState Dem_DTCFilter[DEM_CLIENTID_ARRAYLENGTH_STD];
	#if DEM_CFG_EVMEM_MIRROR_MEMORY_DTC_STATUS_STORED
	static Dem_DTCFilterSyncState Dem_DTCFilterSync;
	#endif
#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"



#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/* called in DCM task context */
DEM_INLINE void Dem_DtcFilterSyncInit(void)
{
#if DEM_CFG_EVMEM_MIRROR_MEMORY_DTC_STATUS_STORED

	Dem_DTCFilterSync.start  = TRUE;
#endif
}


DEM_INLINE boolean Dem_DtcFilterSyncGetDtcStatusByte (Dem_DtcIdType dtcId, Dem_DTCOriginType DTCOrigin, uint8* DTCStatus)
{
	DEM_UNUSED_PARAM(DTCOrigin);

#if DEM_CFG_EVMEM_MIRROR_MEMORY_DTC_STATUS_STORED
	/* Sync currently is only necessary to retrieve the stored DTC status in MIRROR Memory */
	if (DTCOrigin == DEM_DTC_ORIGIN_MIRROR_MEMORY)
	{
		/* sync job is running: just wait till results are available */
		if (Dem_DTCFilterSync.exec)
		{
			return FALSE;
		}
		/* start/init a new sync job */
		if (Dem_DTCFilterSync.start)
		{
			Dem_DTCFilterSync.DtcId = dtcId;
			Dem_DTCFilterSync.DTCOrigin = DTCOrigin;
			Dem_DTCFilterSync.start = FALSE;
			Dem_DTCFilterSync.exec = TRUE;
			return FALSE;
		}
		/* take the results of a finished sync job*/
		*DTCStatus = Dem_DTCFilterSync.DTCStatus;
		Dem_DTCFilterSync.start = TRUE;
		return TRUE;
	}
#endif

	/* retrieve of current DTC status */
	*DTCStatus = Dem_DtcStatusByteRetrieve(dtcId);
	return TRUE;
}

/* called in DEM task context */
DEM_INLINE void Dem_DtcFilterSyncCyclic(void)
{
#if DEM_CFG_EVMEM_MIRROR_MEMORY_DTC_STATUS_STORED
	if (Dem_DTCFilterSync.exec)
	{
		boolean DtcStatusIsValid;

		/* this is the sync action */
		/* MR12 RULE 13.2 VIOLATION: The order of access of volatile objects is not significant. Therefore this warning can be ignored.  */
		Dem_DTCFilterSync.DTCStatus = Dem_DtcStatusByteRetrieveWithOrigin(Dem_DTCFilterSync.DtcId, Dem_DTCFilterSync.DTCOrigin, &DtcStatusIsValid);
		Dem_DTCFilterSync.exec = FALSE;
	}
#endif
}

void Dem_FilterInit(void)
{
    uint8 clientId;
    Dem_ClientIdListIterator ClientIdIt;

    for (Dem_Client_ClientIdIteratorNew(&ClientIdIt, DEM_CLIENT_ITERATORTYPE_STD); Dem_Client_ClientIdIteratorValid(&ClientIdIt);
            Dem_Client_ClientIdIteratorNext(&ClientIdIt))
    {
        clientId = Dem_Client_ClientIdIteratorCurrent(&ClientIdIt);
        Dem_DtcFilterInit(clientId);
    }
}

void Dem_DtcFilterInit(uint8 ClientId)
{
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
    rba_DemObdBasic_Dtc_DtcFilterInit(&Dem_DTCFilter[ClientId]);
#endif

#if (DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED)
    Dem_ChronoSortDTCFilterInit(ClientId);
#endif
        Dem_DtcIdIteratorInvalidate(&Dem_DTCFilter[ClientId].searchIt);
        Dem_DtcIdIteratorInvalidate(&Dem_DTCFilter[ClientId].retrieveIt);
        Dem_DTCFilter[ClientId].isNewFilterCriteria = TRUE;
}

void Dem_SetDTCFilterstartIterator(uint8 ClientId)
{
    Dem_DtcIdIteratorNew(&(Dem_DTCFilter[ClientId].searchIt));
    Dem_DtcIdIteratorNew(&(Dem_DTCFilter[ClientId].retrieveIt));

    Dem_BitArrayClearAll(Dem_DTCFilterMatching[ClientId], DEM_DTCID_ARRAYLENGTH);
}

Std_ReturnType Dem_SetDTCFilter(uint8 ClientId,
                                         uint8 DTCStatusMask,
                                          Dem_DTCFormatType DTCFormat,
                                          Dem_DTCOriginType DTCOrigin,
                                          boolean FilterWithSeverity,
                                          Dem_DTCSeverityType DTCSeverityMask,
                                          boolean FilterForFaultDetectionCounter)
{
    Std_ReturnType returnVal = E_OK;

   /* parameter check: ClientId*/
    if (Dem_Client_GetClientType(ClientId) != DEM_CLIENTTYPE_STANDARD)
        {
            DEM_DET(DEM_DET_APIID_SETDTCFILTER, DEM_E_WRONG_CONFIGURATION,0u);
            return DEM_WRONG_CLIENTID;
        }
    /* Call the FilterInit to reinit the iterators as it will be more robust when a running filter operation is interrupted by a new call to setDTCFilter */
    Dem_DtcFilterInit(ClientId);

    /* parameter check: DTCOrigin */
    if (!(Dem_EvMemIsDtcOriginValid(&DTCOrigin)
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
    || (DTCOrigin == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
#endif
    ))
    {
        /*[SWS_Dem_01067]*/
        DEM_DET(DEM_DET_APIID_SETDTCFILTER, DEM_E_WRONG_CONFIGURATION,0u);
        returnVal = E_NOT_OK;
    }

    /* parameter check: DTCFormat */
#if(DEM_CFG_OBD != DEM_CFG_OBD_ON)
    if (DTCFormat == DEM_DTC_FORMAT_OBD)
    {
        /* [SWS_Dem_01066]:The Det error DEM_E_WRONG_CONFIGURATION shall be reported
             if the function Dem_SetDTCFilter is called with a value of the parameter
              DTCFormat that is not supported per configuration*/
        DEM_DET(DEM_DET_APIID_SETDTCFILTER, DEM_E_WRONG_CONFIGURATION,0u);
        returnVal = E_NOT_OK;
    }
#endif

    if (returnVal != E_NOT_OK)
    {
        Dem_DTCFilter[ClientId].DTCStatusMask = (uint8) (DTCStatusMask & DEM_CFG_DTCSTATUS_AVAILABILITYMASK);
        Dem_DTCFilter[ClientId].DTCFormat = DTCFormat;
        Dem_DTCFilter[ClientId].DTCOrigin = DTCOrigin;
        Dem_DTCFilter[ClientId].FilterWithSeverity = FilterWithSeverity;
        Dem_DTCFilter[ClientId].DTCSeverityMask = DTCSeverityMask;
        Dem_DTCFilter[ClientId].FilterForFaultDetectionCounter = FilterForFaultDetectionCounter;

        Dem_DTCFilter[ClientId].numberOfMatchingDTCs = 0;
        Dem_DTCFilter[ClientId].filterSet = TRUE;

        Dem_DtcFilterSyncInit();

        if (Dem_DTCFilter[ClientId].DTCFormat == DEM_DTC_FORMAT_OBD)
        {
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
            rba_DemObdBasic_Dtc_SetDTCFilter(&Dem_DTCFilter[ClientId], DTCOrigin);
#endif
        }
        else
        {
#if (DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED)
            if (!Dem_ChronoSortSetDTCFilter(ClientId, &Dem_DTCFilter[ClientId])) /** To determine whether the filtering is performed by chrono or standard algorithm. */
#endif
            {
                Dem_SetDTCFilterstartIterator(ClientId);
            }
        }
    }
    return returnVal;
}


DEM_INLINE Dem_boolean_least Dem_DTCFilterMatchesStatus(const Dem_DTCFilterState* dtcFilter_p, uint8 DtcStatus, sint8 FaultDetectionCounter)
{
    Dem_boolean_least matches = TRUE;

    if (dtcFilter_p->DTCStatusMask != 0)
    {
        matches = matches && ((dtcFilter_p->DTCStatusMask & (DtcStatus & DEM_CFG_DTCSTATUS_AVAILABILITYMASK)) != 0u);
    }

    if (dtcFilter_p->FilterForFaultDetectionCounter)
    {
        matches = matches && ((FaultDetectionCounter > 0) && (FaultDetectionCounter < 127));
    }

    return matches;
}

Dem_boolean_least Dem_DTCFilterMatches(const Dem_DTCFilterState* dtcFilter_p, Dem_DtcIdType dtcId)
{
    uint8 DtcStatus = 0;
    Dem_boolean_least matches = TRUE;
    sint8 FaultDetectionCounter = 0;

    boolean DtcStatusIsValid;

    if (dtcFilter_p->DTCStatusMask != 0)
    {
        DtcStatus = Dem_DtcStatusByteRetrieveWithOrigin(dtcId, dtcFilter_p->DTCOrigin, &DtcStatusIsValid);
    }

    if (!Dem_DtcUsesOrigin(dtcId, dtcFilter_p->DTCOrigin))
    {
        matches = FALSE;
    }

    if (dtcFilter_p->FilterWithSeverity)
    {
        matches = matches && ((Dem_Cfg_Dtc_GetSeverity(dtcId) & dtcFilter_p->DTCSeverityMask) != 0);
    }

    if (dtcFilter_p->FilterForFaultDetectionCounter)
    {
        FaultDetectionCounter = Dem_DtcFaultDetectionRetrieve(dtcId);
    }
    /* MR12 RULE 13.5 VIOLATION: Boolean returning function is identified as an expression causing side effect. This warning can be ignored. */
    matches = matches && Dem_DTCFilterMatchesStatus(dtcFilter_p, DtcStatus, FaultDetectionCounter);

#if(DEM_CFG_OBD != DEM_CFG_OBD_OFF)
   /* MR12 RULE 13.5 VIOLATION: Boolean returning function is identified as an expression causing side effect. This warning can be ignored. */
   matches = matches && rba_DemObdBasic_Dtc_DTCFilterMatches(dtcId, dtcFilter_p);
#endif

    return matches;
}

void Dem_DTCFilterStandardMainFunction(uint8 ClientId)
{
    const sint32 epc = (sint32) DEM_DTC_FILTER_NUMBER_OF_EVENTS_PER_CYCLE; //to avoid MISRA warnings
    sint32 i = epc;
    Dem_DtcIdIterator searchItCopy; /* local copy of Dem_DTCFilter.searchIt to reduce lock time */
    Dem_DtcIdType dtcId;
    Dem_boolean_least matches;
    sint32 numberOfEvents;

    DEM_ENTERLOCK_DCM();
    Dem_DTCFilter[ClientId].isNewFilterCriteria = FALSE;
    searchItCopy = Dem_DTCFilter[ClientId].searchIt;
    DEM_EXITLOCK_DCM();

    Dem_DtcFilterSyncCyclic();

    if (Dem_DTCFilter[ClientId].filterSet == TRUE)
    {
        if (Dem_DtcIdIteratorIsValid(&searchItCopy))
        {
            while (i > 0)
            {
                /* check filter-rules for current dtcId */
                dtcId = Dem_DtcIdIteratorCurrent(&searchItCopy);

                if (!Dem_isDtcIdValid(dtcId))
                {
                    return;
                }

                if (!Dem_DtcIsSuppressed(dtcId))
                {
                    numberOfEvents = (sint32) Dem_DtcIdGetNumberOfEvents(dtcId);
                    /* only execute if number of events of current DTC does not exceed the total number of events allowed in this cycle or it is the first DTC in this cycle */
                    if ((numberOfEvents > i) && (i != epc))
                    {
                        break;
                    }
                    i = i - numberOfEvents;
                    matches = Dem_DTCFilterMatches(&Dem_DTCFilter[ClientId], dtcId);
                }
                else
                {
                    i = i - 1;
                    matches = FALSE;
                }

                DEM_ENTERLOCK_DCM();
                if (!Dem_DTCFilter[ClientId].isNewFilterCriteria)
                {
                    if (matches)
                    {
                        Dem_BitArraySetBit(Dem_DTCFilterMatching[ClientId], dtcId);
                        Dem_DTCFilter[ClientId].numberOfMatchingDTCs++;
                    }

                    /* advance iterator to next dtcId */
                    Dem_DtcIdIteratorNext(&Dem_DTCFilter[ClientId].searchIt);
                    searchItCopy = Dem_DTCFilter[ClientId].searchIt;
                }
                else
                {
                    i = 0;
                }
                DEM_EXITLOCK_DCM();
            }
        }
    }
}

boolean Dem_IsStandardFilterFinished(const Dem_DtcIdIterator *it)
{
    return (!Dem_DtcIdIteratorIsValid(it));
}

void Dem_DTCFilterMainFunction(void)
{
    uint8 clientId;
    Dem_ClientIdListIterator ClientIdIt;

    /*loop through all the clients which are configured- std clients*/
    for (Dem_Client_ClientIdIteratorNew(&ClientIdIt, DEM_CLIENT_ITERATORTYPE_STD); Dem_Client_ClientIdIteratorValid(&ClientIdIt);
            Dem_Client_ClientIdIteratorNext(&ClientIdIt))
    {
        clientId= Dem_Client_ClientIdIteratorCurrent(&ClientIdIt);
#if DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED
        if (Dem_IsChronoSortEnabled(clientId))
        {
            Dem_ChronoSortMainFunction(clientId, &Dem_DTCFilter[clientId]);
        }
        else
#endif
        {
            Dem_DTCFilterStandardMainFunction(clientId);
        }
    }
#if (DEM_CFG_J1939DCM_READ_DTC_SUPPORT)
            Dem_J1939DcmDTCFilterMainFunction();
#endif
}

Std_ReturnType Dem_GetNextFilteredStandardDTCID(uint8 ClientId, Dem_DtcIdType* dtcId, uint32* DTC, uint8* DTCStatus, sint8* DTCFaultDetectionCounter)
{
    uint16_least i = DEM_DTC_FILTER_RETRIEVE_NUMBER_OF_DTCS;
    uint8 localDTCStatus;
    sint8 FaultDetectionCounter = 0;

    while (i > 0u)
    {
        if (!Dem_DtcIdIteratorIsValid(&Dem_DTCFilter[ClientId].retrieveIt))
        {
            return DEM_NO_SUCH_ELEMENT;
        }

        if (Dem_DTCFilter[ClientId].retrieveIt == Dem_DTCFilter[ClientId].searchIt)
        {
            return DEM_PENDING;
        }

        *dtcId = Dem_DtcIdIteratorCurrent(&(Dem_DTCFilter[ClientId].retrieveIt));

        if (Dem_BitArrayIsBitSet(Dem_DTCFilterMatching[ClientId], *dtcId))
        {
            if (!Dem_DtcFilterSyncGetDtcStatusByte(*dtcId, Dem_DTCFilter[ClientId].DTCOrigin, &localDTCStatus))
            {
                return DEM_PENDING;
            }

            if (Dem_DTCFilter[ClientId].FilterForFaultDetectionCounter)
            {
                FaultDetectionCounter = Dem_DtcFaultDetectionRetrieve(*dtcId);
            }

            if (Dem_DTCFilterMatchesStatus(&Dem_DTCFilter[ClientId], localDTCStatus, FaultDetectionCounter))
            {
                if (DTCStatus != NULL_PTR)
                {
                    *DTCStatus = (uint8)(localDTCStatus & DEM_CFG_DTCSTATUS_AVAILABILITYMASK);
                }

                if (DTCFaultDetectionCounter != NULL_PTR)
                {
                    *DTCFaultDetectionCounter = FaultDetectionCounter;
                }

                *DTC = Dem_GetDtcCode(*dtcId);

                Dem_DtcIdIteratorNext(&Dem_DTCFilter[ClientId].retrieveIt);
                return E_OK;
            }
        }
        i--;
        Dem_DtcIdIteratorNext(&Dem_DTCFilter[ClientId].retrieveIt);
    }
    return DEM_PENDING;
}

static Std_ReturnType Dem_GetNextFilteredDTCID(uint8 ClientId, Dem_DtcIdType* dtcId, uint32* DTC, uint8* DTCStatus, sint8* DTCFaultDetectionCounter)
{
#if DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED
    if (Dem_IsChronoSortEnabled(ClientId))
    {
        return Dem_ChronoSortGetNextFilteredDTCID(ClientId, dtcId, DTC, DTCStatus);
    }
    else
#endif
    {
        return Dem_GetNextFilteredStandardDTCID(ClientId, dtcId, DTC, DTCStatus, DTCFaultDetectionCounter);
    }
}

Std_ReturnType Dem_GetNextFilteredDTC(uint8 ClientId, uint32* DTC, uint8* DTCStatus)
{
    Dem_DtcIdType dtcId;
    Std_ReturnType retVal = DEM_NO_SUCH_ELEMENT;

    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(DTC, DEM_DET_APIID_GETNEXTFILTEREDDTC, E_NOT_OK);
    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(DTCStatus, DEM_DET_APIID_GETNEXTFILTEREDDTC, E_NOT_OK);

    /* parameter check: ClientId*/
    if (Dem_Client_GetClientType(ClientId) != DEM_CLIENTTYPE_STANDARD)
    {
        DEM_DET(DEM_DET_APIID_GETNEXTFILTEREDDTC, DEM_E_WRONG_CONFIGURATION,0u);
        return DEM_WRONG_CLIENTID;
    }

    /*As per [SWS_Dem_01300] and [SWS_Dem_01266]*/
    if (Dem_DTCFilter[ClientId].filterSet != TRUE)
    {
        DEM_DET(DEM_DET_APIID_GETNEXTFILTEREDDTC, DEM_E_WRONG_CONDITION,0u);
        return E_NOT_OK;
    }

#if DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED
    if(Dem_IsChronoSortEnabled(ClientId) || (Dem_DtcIdIteratorIsValid(&Dem_DTCFilter[ClientId].retrieveIt)))
#else
    if (Dem_DtcIdIteratorIsValid(&Dem_DTCFilter[ClientId].retrieveIt))
#endif
    {
        retVal = Dem_GetNextFilteredDTCID(ClientId, &dtcId, DTC, DTCStatus, NULL_PTR);
    }
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
    else
    {
        retVal = rba_DemObdBasic_Dtc_GetNextFilteredDTC(&Dem_DTCFilter[ClientId], DTC, DTCStatus);
    }
#endif

    return retVal;
}

Std_ReturnType Dem_GetNextFilteredDTCAndFDC(uint8 ClientId, uint32* DTC, sint8* DTCFaultDetectionCounter)
{
    Dem_DtcIdType dtcId;
    Std_ReturnType retVal;

    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(DTC, DEM_DET_APIID_GETNEXTFILTEREDDTCANDFDC, E_NOT_OK);
    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(DTCFaultDetectionCounter, DEM_DET_APIID_GETNEXTFILTEREDDTCANDFDC, E_NOT_OK);

    /* parameter check: ClientId*/
    if (Dem_Client_GetClientType(ClientId) != DEM_CLIENTTYPE_STANDARD)
    {
        DEM_DET(DEM_DET_APIID_GETNEXTFILTEREDDTCANDFDC, DEM_E_WRONG_CONFIGURATION,0u);
        return DEM_WRONG_CLIENTID;
    }

    /*As per [SWS_Dem_01300] and [SWS_Dem_01266]*/
    if (Dem_DTCFilter[ClientId].filterSet != TRUE)
    {
        DEM_DET(DEM_DET_APIID_GETNEXTFILTEREDDTCANDFDC, DEM_E_WRONG_CONDITION,0u);
        return E_NOT_OK;
    }

    retVal = Dem_GetNextFilteredDTCID(ClientId, &dtcId, DTC, NULL_PTR, DTCFaultDetectionCounter);

    return retVal;
}


Std_ReturnType Dem_GetNextFilteredDTCAndSeverity (uint8 ClientId, uint32* DTC, uint8* DTCStatus, Dem_DTCSeverityType* DTCSeverity, uint8* DTCFunctionalUnit)
{
    Dem_DtcIdType dtcId;
    Std_ReturnType retVal;

    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(DTC,DEM_DET_APIID_GETNEXTFILTEREDDTCANDSEVERITY,E_NOT_OK);
    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(DTCStatus,DEM_DET_APIID_GETNEXTFILTEREDDTCANDSEVERITY,E_NOT_OK);
    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(DTCSeverity,DEM_DET_APIID_GETNEXTFILTEREDDTCANDSEVERITY,E_NOT_OK);
    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(DTCFunctionalUnit,DEM_DET_APIID_GETNEXTFILTEREDDTCANDSEVERITY,E_NOT_OK);

    /* parameter check: ClientId*/
    if (Dem_Client_GetClientType(ClientId) != DEM_CLIENTTYPE_STANDARD)
    {
        DEM_DET(DEM_DET_APIID_GETNEXTFILTEREDDTCANDSEVERITY, DEM_E_WRONG_CONFIGURATION,0u);
        return DEM_WRONG_CLIENTID;
    }
    /*As per [SWS_Dem_01300] and [SWS_Dem_01266]*/
    if (Dem_DTCFilter[ClientId].filterSet != TRUE)
    {
        DEM_DET(DEM_DET_APIID_GETNEXTFILTEREDDTCANDSEVERITY, DEM_E_WRONG_CONDITION,0u);
        return E_NOT_OK;
    }

    retVal = Dem_GetNextFilteredDTCID(ClientId, &dtcId, DTC, DTCStatus, NULL_PTR);
    if (retVal == E_OK)
    {
        *DTCSeverity = Dem_Cfg_Dtc_GetSeverity(dtcId);
        *DTCFunctionalUnit = Dem_Cfg_Dtc_GetFunc_Unit(dtcId);
    }

    return retVal;
}

Std_ReturnType Dem_GetNumberOfStandardFilteredDTC(uint8 ClientId, uint16* NumberOfFilteredDTC)
{
    *NumberOfFilteredDTC = Dem_DTCFilter[ClientId].numberOfMatchingDTCs;
    return E_OK;
}

Std_ReturnType Dem_GetNumberOfFilteredDTC(uint8 ClientId, uint16* NumberOfFilteredDTC)
{
    Std_ReturnType retVal;

    DEM_ENTRY_CONDITION_CHECK_NOT_NULL_PTR(NumberOfFilteredDTC,DEM_DET_APIID_GETNUMBEROFFILTEREDDTC,E_NOT_OK);

    /* parameter check: ClientId*/
    if (Dem_Client_GetClientType(ClientId) != DEM_CLIENTTYPE_STANDARD)
    {
        DEM_DET(DEM_DET_APIID_GETNUMBEROFFILTEREDDTC, DEM_E_WRONG_CONFIGURATION,0u);
        return DEM_WRONG_CLIENTID;
    }

    /*As per [SWS_Dem_01300] and [SWS_Dem_01266]*/
    if(Dem_DTCFilter[ClientId].filterSet!=TRUE)
    {
        DEM_DET(DEM_DET_APIID_GETNUMBEROFFILTEREDDTC, DEM_E_WRONG_CONDITION,0u);
        return E_NOT_OK;
    }

    if (Dem_DtcIdIteratorIsValid(&Dem_DTCFilter[ClientId].searchIt))
    {
        return DEM_PENDING;
    }

#if (DEM_CFG_REPORT_CHRONOLOGICAL_ORDER_HOOKS_ENABLED)
    if (Dem_IsChronoSortEnabled(ClientId))
    {
        retVal = Dem_ChronoSortGetNumberOfFilteredDTC(ClientId, NumberOfFilteredDTC);
    }
    else
#endif
    {
        retVal = Dem_GetNumberOfStandardFilteredDTC(ClientId, NumberOfFilteredDTC);
    }

    return retVal;
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
