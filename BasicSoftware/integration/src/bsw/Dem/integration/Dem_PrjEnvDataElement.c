/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */



/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Read functions for internal data elements
 *
 * Implement read functions for project-specific internal data elements that are referenced by
 * DemInternalDataElementClass/DemDataElementReadFnc
 */

#include "Dem_Prv_InternalAPI.h"
#include "Dem_PrjEnvDataElement.h"
#include "Dem_PrjSpecificExtData.h"
#include "Dem_EventStatus.h"



#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"
uint8 AgedCounter[DEM_EVENTID_ARRAYLENGTH];
uint8 FaultPendingCounter[DEM_EVENTID_ARRAYLENGTH];
#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

void Dem_ExtendedData_Init(void)
{
	uint8 i = 0;
	uint16 eventid = 0;

	for(i = 0; i < DEM_CFG_MAX_NUMBER_EVENT_ENTRY_PRIMARY; i++)
	{

		eventid = Dem_EvMemEventMemory[i].Hdr.Data.EventId;

		if( eventid != 0)
		{
			AgedCounter[eventid] = Dem_EvMemEventMemory[i].Data[2];
			FaultPendingCounter[eventid] = Dem_EvMemEventMemory[i].Data[1];
		}
	}
}

/**
 * \brief Provide the data for an internal data element.
 *
 * Called when the data for the internal data element is needed (e.g. on freeze frame capture or getting the extended
 * data records) and the internal data element was configured to be DEM_PRJ_SPECIFIC_DATA_ON_RETRIEVE or
 * DEM_PRJ_SPECIFIC_DATA_ON_REPORT.
 *
 * Copy the method, change the name into something more meaningful and enter the name in
 * DemInternalDataElementClass\DemRbDataElementReadFnc
 *
 * \param[out] Buffer        The buffer to write the data of the internal data element into. The buffer has the size
 *                           specified in DemInternalDataElementClass/DemDataElementDataSize.
 * \param[in]  internalData  Information about the read request
 */
Std_ReturnType Dem_Get_AgedCounter(uint8* Buffer, const Dem_InternalEnvData* internalData)
{
    /* Please fill your implementation here !
     * see following functions for examples:
     * Dem_ReadEventStatusByte, Dem_ReadAgingCtr, Dem_ReadOvflInd, Dem_ReadMaxFdcDuringCurrentCycle */

	/* This is the counter of the times that a DTC has been aged since the last DTC clearing was carried out.
	 * This value shall be stored in non-volatile memory.
	 * If the value overflows, it shall be held at 255 (FF hex).*/

	if (internalData->evMemLocation != NULL_PTR)
	{
		*Buffer = AgedCounter[(*internalData).evMemLocation->Hdr.Data.EventId];
		internalData->evMemLocation->Data[2] = AgedCounter[(*internalData).evMemLocation->Hdr.Data.EventId]; //position of Aging counter in Extended Data is 2
	}
	else
	{
		*Buffer = 0;
	}
	return E_OK;
}

Std_ReturnType Dem_Get_FaultPendingCounter(uint8* Buffer, const Dem_InternalEnvData* internalData)
{
    /* Please fill your implementation here !
     * see following functions for examples:
     * Dem_ReadEventStatusByte, Dem_ReadAgingCtrUpCnt, Dem_ReadAgingCtrDownCnt, Dem_ReadOvflInd, Dem_ReadMaxFdcDuringCurrentCycle */

	/* This is a nominal value for the number of going into pending fault stage.
	 * It shall be noticed that for each operation cycle, going into pending fault stage only once.
	 * This value must be retained between operation cycles in non-volatile memory.
	 * This counter shall be reset to �0� once �pendingDTC� bit is cleared in no fault operation cycle.
	 * For Non-emissions-related DTC, the threshold of fault pending counter shall be 1.
	 * */
	if (internalData->evMemLocation != NULL_PTR)
	{
		*Buffer = FaultPendingCounter[(*internalData).evMemLocation->Hdr.Data.EventId];
		internalData->evMemLocation->Data[1] = FaultPendingCounter[(*internalData).evMemLocation->Hdr.Data.EventId]; //position of FaultPendingCounter in Extended Data is 1
	}
	else
	{
		*Buffer = 0;
	}
	return E_OK;
}

/*
 * Callback function when DTC Clear service starts
 */
void DemRbClearDTCStart_Fnc(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{

}

/*
 * Callback function when DTC Clear service finishes
 */
void DemRbClearDTCFinish_Fnc(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin)
{
	uint8 i;
	uint16 eventid;
	Dem_DtcIdType dtcid;

	Dem_EvMemEventMemoryType* memEvent;
	for(i = 0; i < DEM_CFG_MAX_NUMBER_EVENT_ENTRY_PRIMARY; i++)
	{
		memEvent = &Dem_EvMemEventMemory[i];
		eventid = memEvent->Hdr.Data.EventId;
		dtcid = Dem_MapEventIdToDtcId[eventid];
		if( Dem_GetDtcCode(dtcid) == DTC)
		{
			AgedCounter[eventid] = 0;
			FaultPendingCounter[eventid] = 0;
			Dem_EvMemEventMemory[i].Data[1] = 0;
			Dem_EvMemEventMemory[i].Data[2] = 0;
		}
	}
}

/*
 * Callback function when DTC status change
 */
void DemCallbackDTCStatusChanged_Fnc(uint32 DTC, uint8 DTCStatusOld, uint8 DTCStatusNew)
{
	uint8 i;
	uint16 eventid;
	Dem_DtcIdType dtcid;
	Dem_EvMemEventMemoryType* memEvent;
	for(i = 0; i < DEM_CFG_MAX_NUMBER_EVENT_ENTRY_PRIMARY; i++)
	{
		memEvent = &Dem_EvMemEventMemory[i];
		eventid = memEvent->Hdr.Data.EventId;
		dtcid = Dem_MapEventIdToDtcId[eventid];
		if( Dem_GetDtcCode(dtcid) == DTC)
		{
			/*
			 * Calculate FaultPendingCounter
			 */
			if( ((DTCStatusNew & 0x04) != 0) && (memEvent->Data[1] != 1))
			{
				FaultPendingCounter[eventid] = 1;

			}
			memEvent->Data[1] = FaultPendingCounter[eventid];

			/*
			 * Calculate AgedCounter
			 */
			if( ( ((DTCStatusOld & 0x08) == 0x08) && ((DTCStatusNew & 0x08) == 0) ) &&  (memEvent->Data[2] < 0xFF) )
			{
				AgedCounter[eventid] +=1;

			}
			memEvent->Data[2] = AgedCounter[eventid];
		}
	}


}

/*
 * Callback function when DTC Clear service starts
 */
void Dem_EndOperationCycle_Callback(void)
{
	uint8 i = 0;
	uint16 eventid = 0;

	for(i = 0; i < DEM_CFG_MAX_NUMBER_EVENT_ENTRY_PRIMARY; i++)
	{

		eventid = Dem_EvMemEventMemory[i].Hdr.Data.EventId;

		if( eventid != 0)
		{
			if ( (Dem_AllEventsStatusByte[eventid] & 0x42) == 0x00 )
			{
				Dem_EvMemEventMemory[i].Data[1] = 0;
			}
		}
	}
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
