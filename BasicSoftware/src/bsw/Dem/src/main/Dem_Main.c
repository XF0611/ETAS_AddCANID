

#include "Dem_Internal.h"
#include "SchM_Dem.h"
#include "Rte_Dem.h"

#include "Dem_Main.h"

#include "Dem_Prv_J1939Dcm.h"
#include "Dem_ClientRequestDispatcher.h"
#include "Dem_Cfg_AssertionChk.h"
#include "Dem_IndicatorAttributes.h"
#include "Dem_OperationCycle.h"
#include "Dem_Clear.h"
#include "Dem_Events.h"
#include "Dem_Deb.h"
#include "Dem_EventFHandling.h"
#include "Dem_EventStatus.h"
#include "Dem_EventRecheck.h"
#include "Dem_EvBuff.h"
#include "Dem_EvBuffDistributor.h"
#include "Dem_DTCStatusByte.h"
#include "Dem_DTCFilter.h"
#include "Dem_EventStatus.h"
#include "Dem_EvMem.h"
#include "Dem_EvMemGen.h"
#include "Dem_Nvm.h"
#if (DEM_CFG_FIM_USED == DEM_CFG_FIM_ON)
#include "FiM.h"
#endif
#include "Dem_EvBuffDistributor.h"
#include "Dem_DisturbanceMemory.h"
#include "Dem_Obd.h"

#include "Dem_Bfm.h"

#if(DEM_CFG_SUPPORT_PROJECTSPECIFIC_CONSISTENCYCHECK == DEM_CFG_CONSISTENCY_CHECK_ON)
#include "Dem_PrjSpecificConsistencyCheck.h"
#endif
#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"

struct Dem_ConfigType
{
    boolean debouncerAlternativeParameters;
    boolean dtcAlternativeParameters;
    /* FC_VariationPoint_START */
    #if (DEM_BFM_ENABLED == DEM_BFM_ON)
    rba_DemBfm_ConfigDataType const * demBfmConfigPointer;
    #endif
    /* FC_VariationPoint_END */
};

/* FC_VariationPoint_START */
#if(DEM_BFM_ENABLED == DEM_BFM_OFF)
/* FC_VariationPoint_END */
const Dem_ConfigType Dem_ConfigAlternativeParameters = { TRUE, TRUE };
/* FC_VariationPoint_START */
#else
const Dem_ConfigType Dem_ConfigAlternativeParameters = { TRUE, TRUE, &rba_DemBfm_ConfigParam_cs };
#endif
/* FC_VariationPoint_END */

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"

#if (DEM_CFG_VERSION_INFO_API == DEM_CFG_VERSION_INFO_API_ON)
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
void Dem_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    if(versioninfo == NULL_PTR)
    {
        DEM_DET(DEM_DET_APIID_GETVERSIONINFO, DEM_E_PARAM_POINTER,0u);
    }
    else
    {
        versioninfo->vendorID = DEM_VENDOR_ID;
        versioninfo->moduleID = DEM_MODULE_ID;
        versioninfo->sw_major_version = DEM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = DEM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = DEM_SW_PATCH_VERSION;
    }
}
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif


#define DEM_START_SEC_VAR_INIT
#include "Dem_MemMap.h"

Dem_OpMoStateType Dem_OpMoState = DEM_OPMO_STATE_NOTINITIALIZED;
Dem_FimStateType Dem_FimState = DEM_FIM_STATE_NOTINITIALIZED;
Dem_boolean_least Dem_TestFailedStatusInitialized = FALSE;

static boolean Dem_OpMoWasInitialized = FALSE;

#define DEM_STOP_SEC_VAR_INIT
#include "Dem_MemMap.h"


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

void Dem_PreInit(const Dem_ConfigType* ConfigPtr)
{
    DEM_ASSERT(Dem_OpMoState==DEM_OPMO_STATE_NOTINITIALIZED, DEM_DET_APIID_PREINIT, 0);
    /*Dem_AllEventsState is a global variable, so this variable is already set to 0*/
    Dem_EvtPreInitEvents();

    Dem_PreInitErrorQueue();

    Dem_ReportErrorStatusEnableQueue();
    if (ConfigPtr != NULL_PTR)
    {
        if (ConfigPtr->debouncerAlternativeParameters) {
            Dem_DebSwitchToAlternativeParameters();
        }
#if (DEM_CFG_ALTERNATIVEDTC == DEM_CFG_ALTERNATIVEDTC_ON)
        if(ConfigPtr->dtcAlternativeParameters) {
            Dem_SwitchToAlternativeDtc();
        }
#endif
    }

/* FC_VariationPoint_START */
	#if (DEM_BFM_ENABLED == DEM_BFM_ON)
	if( ConfigPtr != NULL_PTR )
	{
		rba_DemBfm_PreInit( ConfigPtr->demBfmConfigPointer );
	}
	else
	{
		rba_DemBfm_PreInit( NULL_PTR );
	}
	#endif
/* FC_VariationPoint_END */

	Dem_OpMoState = DEM_OPMO_STATE_PREINITIALIZED;
}

/*API for locking all failure info*/
Std_ReturnType Dem_LockAllFailureInfo (void)
{
#if(DEM_CFG_LOCK_ALLFAILUREINFO == DEM_CFG_LOCK_ALLFAILUREINFO_ON)
    Dem_OpMoState = DEM_OPMO_STATE_LOCK_ALL_FAILURE_INFO;
    (void)Dem_LockEventMemory(TRUE);
    return E_OK;
#else
    return E_NOT_OK;
#endif
}

#if(DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
DEM_INLINE void Dem_ClearAllNvmBlocks(void)
{
    if(Dem_NvMIsInvalidateAllNVMBlocksRequested())
    {
        Dem_DistMemClearNvm();
#if (DEM_CFG_FFPRESTORAGESUPPORT == DEM_CFG_FFPRESTORAGESUPPORT_ON) && (DEM_CFG_FFPRESTORAGE_NONVOLATILE == DEM_CFG_FFPRESTORAGE_NONVOLATILE_ON)
        Dem_EvBuffClearNvm();
#endif
        Dem_EvMemClearNvm();
        Dem_EvMemGenClearNVM();
        Dem_EvtHistoryStatusClearNVM();
        Dem_EvtStClearNvm();
        Dem_IndicatorAttributesClearNvm();
    }
}
#endif

void Dem_Init(void)
{

#if(DEM_CFG_LOCK_ALLFAILUREINFO == DEM_CFG_LOCK_ALLFAILUREINFO_ON)
    if(!Dem_OpMoIsAllFailureInfoLocked())
#endif
    {
        DEM_ASSERT(Dem_OpMoState==DEM_OPMO_STATE_PREINITIALIZED, DEM_DET_APIID_INIT, 0);

        if (Dem_OpMoState == DEM_OPMO_STATE_PREINITIALIZED)
        {
            /*Call-out point for Project Specific Consistency Check*/
#if(DEM_CFG_SUPPORT_PROJECTSPECIFIC_CONSISTENCYCHECK == DEM_CFG_CONSISTENCY_CHECK_ON)
            Dem_ProjectSpecificConsistencyCheck();
#endif
            /* Dem_NvMInit needs to be called as first thing in init function! */
            Dem_NvMInit();

            Dem_ClientInit();

            Dem_FilterInit();

#if (DEM_CFG_J1939DCM_READ_DTC_SUPPORT)
            Dem_J1939DcmDtcFilterInit();
#endif
#if(DEM_CFG_J1939DCM_FREEZEFRAME_SUPPORT || DEM_CFG_J1939DCM_EXPANDED_FREEZEFRAME_SUPPORT)
            Dem_J1939DcmFreezeFrameFilterInit();
#endif
            if (!Dem_OpMoWasInitialized)
            {
#if(DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
                //Should always have the first position in this if statement for clearing NVM Blocks
                Dem_ClearAllNvmBlocks();
#endif

                Dem_OperationCycleInitCheckNvm();

                //Set the event status to 0 for the events which are currently not available or suppressed
                Dem_EvtInitEvents();

                Dem_EventStatusInitCheckNvM();

#if(DEM_CFG_ALLOW_HISTORY == DEM_CFG_ALLOW_HISTORY_ON)
                Dem_EvtHistoryStatusInitCheckNvM();
#endif

                Dem_IndicatorAttributeInitCheckNvM();


                Dem_EvMemInit();

                /* Process eventStatusByte:  restore faultLevel from testFailed;
                * clear test failed if !DemStatusBitStorageTestFailed and !DemRbEventStatusBitStorageTestFailed */
                Dem_InitEventStatusTestFailed();

                /* Reconstruct Graph */
                Dem_DependencyInit();

                /* Required to move the indicator attribute init below Event Memory Init because of consistency check
                 * introduced for Indicator attributes in Event Memory Init */
                Dem_IndicatorAttributeInit();

/* FC_VariationPoint_START */
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
                rba_DemObdBasic_InitCheckNvm();
#endif
/* FC_VariationPoint_END */

#if (DEM_CFG_FFPRESTORAGESUPPORT == DEM_CFG_FFPRESTORAGESUPPORT_ON)
                Dem_PreStoredFFInitCheckNvM();
#endif

#if (DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)
                Dem_DistMemInitCheckNvM();
#endif

#if( DEM_BFM_ENABLED == DEM_BFM_ON )
                rba_DemBfm_InitCheckNvM();
                rba_DemBfm_InitCausality();
#endif
                Dem_EvMemInitCausality();

#if(DEM_CFG_FIM_USED == DEM_CFG_FIM_ON)
                Dem_FimState = DEM_FIM_STATE_IS_IN_INITIALIZATION;

                /* This call will resynchronise the Fim Status with Dem Status.
                 * Note: If a new operation cycle is already started, then Fim will either get updates via Status change callbacks or
                 * Fim will update it as part of its Main function routine.*/
                FiM_DemInit();

                Dem_FimState = DEM_FIM_STATE_INITIALIZED;
#endif /* (DEM_CFG_FIM_USED == DEM_CFG_FIM_ON) */
            }

            /* FC_VariationPoint_START */
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
            rba_DemObdBasic_InitCheckPlausability();
#endif
            /* FC_VariationPoint_END */

            Dem_OperationCycleInit();

            /* FC_VariationPoint_START */
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
            rba_DemObd_Init();
#endif
            /* FC_VariationPoint_END */

#if (DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)
            Dem_DistMemInit();
#endif

            /* FC_VariationPoint_START */
#if( DEM_BFM_ENABLED == DEM_BFM_ON )
            rba_DemBfm_Init();
#endif
            /* FC_VariationPoint_END */
            Dem_TestFailedStatusInitialized = TRUE; /* should only be set directly before disabling the queue */
            Dem_ReportErrorStatusDisableQueue();

            Dem_OpMoWasInitialized = TRUE;
            Dem_OpMoState = DEM_OPMO_STATE_INITIALIZED;

        }
    }
}

DEM_INLINE void Dem_HandleImmediateTrigger(void)
{
#if(DEM_CFG_TRIGGER_TO_STORE_NVM_SUPPORTED)
    if(Dem_NvMIsImmediateStorageRequested())
    {
    /* FC_VariationPoint_START */
#if( DEM_BFM_ENABLED == DEM_BFM_ON )
        (void)rba_DemBfm_TriggerStoreToNvm();
#endif
    /* FC_VariationPoint_END */
    }
#endif //(DEM_CFG_TRIGGER_TO_STORE_NVM_SUPPORTED)
}


void Dem_MainFunction(void)
{
    if (Dem_OpMoState == DEM_OPMO_STATE_INITIALIZED)
    {
        Dem_HandleImmediateTrigger();

        if(!Dem_OperationCyclesMainFunction())
        {
            /* Only do processing of Enable Condition callbacks if no operation cycle change this cycle (performance reasons) */
#if (DEM_CFG_ENABLECONDITION == DEM_CFG_ENABLECONDITION_ON)
            Dem_EnCoCallbacks();
#endif
        }

        /* The Dispatcher function shall be called before clearmainfunction(). Do not change the sequence */
        Dem_ClientRequestDispatcher_main();

        Dem_DebMainFunction();

        /* The Dem_IndicatorAttributeMainFunction() needs to be called before Dem_EvtMainFunction(),
         * because the rba_DemIndicatorLitePS component uses this function to update WIR bit and these values need to be updated for NvM write */
        Dem_IndicatorAttributeMainFunction();

        /* FC_VariationPoint_START */
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
        rba_DemObd_MainFunction();
#endif
        /* FC_VariationPoint_END */

        Dem_EvBuffMainFunction();
        /* FC_VariationPoint_START */
#if( DEM_BFM_ENABLED == DEM_BFM_ON )
        rba_DemBfm_MainFunction();
#endif
        /* FC_VariationPoint_END */
        Dem_DependencyMainFunction();

        Dem_StoCoMainFunction();

        Dem_EvBuffDistributorMainFunction();

#if(DEM_CFG_DTCFILTER_EXTERNALPROCESSING == DEM_CFG_DTCFILTER_EXTERNALPROCESSING_OFF)
        Dem_DTCFilterMainFunction();
#endif

        Dem_EvMemMainFunction();

#if (DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)
        Dem_DistMemMainFunction();
#endif
#if((DEM_CFG_CALLBACK_INIT_MON_FOR_EVENT_SUPPORTED == TRUE)||(DEM_CFG_EVT_GLOBALINITMONITORINGCALLBACK == TRUE))
        Dem_InitMonitorForEFnc();
#endif
        Dem_NvMMainFunction();
    }
#if(DEM_CFG_LOCK_ALLFAILUREINFO == DEM_CFG_LOCK_ALLFAILUREINFO_ON)
    if (Dem_OpMoIsAllFailureInfoLocked())
    {
#if(DEM_CFG_DTCFILTER_EXTERNALPROCESSING == DEM_CFG_DTCFILTER_EXTERNALPROCESSING_OFF)
        Dem_DTCFilterMainFunction();
#endif
    }
#endif
}


void Dem_Shutdown(void)
{
#if(DEM_CFG_LOCK_ALLFAILUREINFO == DEM_CFG_LOCK_ALLFAILUREINFO_ON)
    if(!Dem_OpMoIsAllFailureInfoLocked())
#endif
    {
        DEM_ASSERT(Dem_OpMoState==DEM_OPMO_STATE_INITIALIZED, DEM_DET_APIID_SHUTDOWN, 0);

        if (Dem_OpMoState == DEM_OPMO_STATE_INITIALIZED)
        {
            /* FC_VariationPoint_START */
            //DEM102 : Dem_Shutdown shall finalize all pending operations in the Dem module
            //DEM341 : For individual non-volatile blocks the API NvM_WriteBlock shall be called within the API Dem_Shutdown.

#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
            rba_DemObdBasic_Shutdown();
#endif
            /* FC_VariationPoint_END */
            Dem_ReportErrorStatusEnableQueue();
            Dem_OpMoState = DEM_OPMO_STATE_PREINITIALIZED;
            Dem_TestFailedStatusInitialized = FALSE;

            Dem_EvBuffDistributorMainFunction();
            /* FC_VariationPoint_START */
#if( DEM_BFM_ENABLED == DEM_BFM_ON )
            rba_DemBfm_Shutdown();
#endif
            /* FC_VariationPoint_END */

 /* The Dem_IndicatorAttributeShutdown() is a function hook, so that the project specific indicator handling
  * can define its own shutdown behavior */
            Dem_IndicatorAttributeShutdown();

            Dem_NvMShutdown();
        }
    }
}


#if (DEM_CFG_ERASE_ALL_DATA_NOT_EQUAL_TO_ERASE_ALL_OFF)
Std_ReturnType Dem_GetEraseAllNvMDataResult(Dem_EraseAllStatusType* EraseResult)
{
    /*Set Erase Result to return Value of Query function*/
    *EraseResult = Dem_NvMGetInvalidateAllNvMBlocksStatus();

    return E_OK;
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
