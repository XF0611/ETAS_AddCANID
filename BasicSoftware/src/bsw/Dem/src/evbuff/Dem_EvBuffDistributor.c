

#include "Dem_EvBuff.h"
#include "Dem_EvBuffDistributor.h"
#include "Dem_Bfm.h"
#include "Dem_EvMem.h"
#include "Dem_EnvMain.h"
#include "Dem_Clear.h"

#if (DEM_CFG_SUPPORT_PROJECTSPECIFIC_FAILUREMEMORY == TRUE)
#include "Dem_PrjSpecificFailureMemory.h"
#endif

#define DEM_START_SEC_VAR_INIT
#include "Dem_MemMap.h"
static boolean Dem_isDistributionRunning = FALSE;
#define DEM_STOP_SEC_VAR_INIT
#include "Dem_MemMap.h"

/* MR12 RULE 8.13 VIOLATION: Parameter fe not made const, as it is modified in subfunction */
DEM_INLINE void Dem_EvBuffDistributorCallReceivers (Dem_EvBuffEvent* fe)
{

    const Dem_EvBuffEvent_MemData* memData_ptr;

#if (DEM_CFG_EVBUFF_STORES_ENVDATA == DEM_CFG_EVBUFF_STORES_ENVDATA_ON)

    memData_ptr = &(fe->memData);

#else

    Dem_EvBuffEvent_MemData memData;
    Dem_EvBuffCaptureAllEnvData(fe, &memData);
    memData_ptr = &memData;

#endif


    if (   (fe->eventType == C_EVENTTYPE_SET)
            || (fe->eventType == C_EVENTTYPE_SET_RESET)
#if (DEM_CFG_STOREWAITINGFORMONITORINGEVENT == TRUE)
            || (fe->eventType == C_EVENTTYPE_SET_WAITINGFORMONITORING)
#endif
    )
    {
        Dem_EvMemSetEventFailedAllMem(fe->eventId, memData_ptr->envData.Buffer);
    }

    if (   (fe->eventType == C_EVENTTYPE_RESET)
            || (fe->eventType == C_EVENTTYPE_SET_RESET)
    )
    {
        Dem_EvMemSetEventPassedAllMem(fe->eventId, memData_ptr->envData.Buffer);
    }

    if (fe->eventType == C_EVENTTYPE_UNROBUST)
    {

        Dem_EvMemSetEventUnRobustAllMem(fe->eventId, memData_ptr->envData.Buffer);
    }

    /* FC_VariationPoint_START */
#if(DEM_BFM_ENABLED == DEM_BFM_ON)
    rba_DemBfm_EnvReport(fe, &(memData_ptr->DemRbBfmFailureMemoryData));
#endif
    /* FC_VariationPoint_END */

#if(DEM_CFG_SUPPORT_PROJECTSPECIFIC_FAILUREMEMORY == TRUE)
    Dem_ProjectSpecificFailureMemoryReport(fe, &(memData_ptr->DemRbProjectSpecificFailureMemoryData));
#endif
}

#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"
static Dem_EvBuffEvent tmpEvBuffLocation;
#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/**
 * Get all failure events from SFB and distribute them
 */
void Dem_EvBuffDistributorMainFunction(void)
{
	Dem_boolean_least performDistribution = FALSE;
	uint32 idx;
	const Dem_EvBuffEvent* fEvent;

	/* FC_VariationPoint_START */
	#if( DEM_BFM_ENABLED == DEM_BFM_ON )
	if( rba_DemBfm_DeleteAllIsInProgress() )
	{
		return;
	}
	#endif
	/* FC_VariationPoint_END */


	DEM_ENTERLOCK_MON();
	if (!Dem_isDistributionRunning)
	{
		performDistribution = TRUE;
		Dem_isDistributionRunning = TRUE;
	}
	DEM_EXITLOCK_MON();

	while (performDistribution)
	{
		DEM_ENTERLOCK_MON();
		/* getting next event from evBuff and isDistributionRunning must be
		 * atomic action, to ensure correct processing in case of concurrent
		 * call to DistributorProcess (on flush).
		 */
		fEvent = Dem_EvBuffGetEvent(&idx);

		if (fEvent == NULL_PTR)
		{
			performDistribution = FALSE;
			Dem_isDistributionRunning = FALSE;
		}
		else
		{
			DEM_MEMCPY(&tmpEvBuffLocation, fEvent, DEM_SIZEOF_VAR(tmpEvBuffLocation));

            /*The Flag NextReportRelevantForMemories is reset here to make sure it is reset only after the distribution to the memories is done*/
            Dem_EvtSetNextReportRelevantForMemories(fEvent->eventId, FALSE);

			Dem_EvBuffRemoveEvent(idx);
		}
		DEM_EXITLOCK_MON();

		if (performDistribution)
		{
			Dem_EvBuffDistributorCallReceivers(&tmpEvBuffLocation);
		}
	}
}


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
