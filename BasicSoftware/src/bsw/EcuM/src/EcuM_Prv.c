



/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "EcuM.h" /*BSW_HeaderInc_002*/

/*todo : This header has to be removed once Rte tool is ready with exclusive area generation*/
#include "EcuM_Cfg_SchM.h"
/*For run time measurement*/
#include "EcuM_RunTime.h"

/*Extern module Headers*/
#include "BswM_EcuM.h"
// #if (!defined(BSWM_AR_RELEASE_MAJOR_VERSION) || (BSWM_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(BSWM_AR_RELEASE_MINOR_VERSION) || (BSWM_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif

#include "Mcu.h"
// #if (!defined(MCU_AR_RELEASE_MAJOR_VERSION) || (MCU_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(MCU_AR_RELEASE_MINOR_VERSION) || ((MCU_AR_RELEASE_MINOR_VERSION != 2) && (MCU_AR_RELEASE_MINOR_VERSION != 3)))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif

/* Switch to either enable or disable call to COMM functions */
#if( ECUM_INCLUDE_COMM == STD_ON )
	#include "ComM_EcuMBswM.h"
	// #if (!defined(COMM_AR_RELEASE_MAJOR_VERSION) || (COMM_AR_RELEASE_MAJOR_VERSION != 4))
	// #error "AUTOSAR major version undefined or mismatched"
	// #endif
	// #if (!defined(COMM_AR_RELEASE_MINOR_VERSION) || (COMM_AR_RELEASE_MINOR_VERSION != 2))
	// #error "AUTOSAR minor version undefined or mismatched"
	// #endif
#endif	 /* ECUM_INCLUDE_COMM */

#include "Os.h"
// #if (!defined(OS_AR_RELEASE_MAJOR_VERSION) || (OS_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(OS_AR_RELEASE_MINOR_VERSION) || (OS_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif

#if(ECUM_DEV_ERROR_DETECT == STD_ON)/*Det included when Det is enabled in EcuM*/
#include "Det.h"
// #if (!defined(DET_AR_RELEASE_MAJOR_VERSION) || (DET_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(DET_AR_RELEASE_MINOR_VERSION) || (DET_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif
#endif/*ECUM_DEV_ERROR_DETECT*/

/*EcuM Private headers*/
#include "EcuM_Callout.h"
#include "EcuM_Prv.h"
/*************************************************************************************************
 Variables
*************************************************************************************************
*/
#define ECUM_START_SEC_VAR_POWER_ON_CLEARED_8
#include "EcuM_MemMap.h"

 /* Variable to store the Selected Boot target*/
EcuM_BootTargetType EcuM_Prv_dataSelectedBootTarget_u8;

/* Variable which stores the informaition if EcuM_Prv_dataSelectedBootTarget_u8 has been set with EcuM_SelectBootTarget
 * This is needed because ECUM_START_SEC_VAR_POWER_ON_INIT_8 does not exist */
uint8 EcuM_Prv_dataSelectedBootTargetInit_u8;

#define ECUM_STOP_SEC_VAR_POWER_ON_CLEARED_8
#include "EcuM_MemMap.h"


#define ECUM_START_SEC_VAR_CLEARED_8
#include "EcuM_MemMap.h"

/*Tells the present Phase of the EcuM*/
uint8 EcuM_Prv_dataPresentPhase_u8;

/* Slave core timeout counter for shutdown synchronization*/
uint8 EcuM_Prv_SlaveCoreTimeoutCtr_u8;
/* Variable to store the selected Shutdown cause*/
EcuM_ShutdownCauseType EcuM_Prv_dataSelectedShutdownCause_u8 = 0;

#if( ECUM_CFG_MODE_HANDLING == STD_ON)
/*Variable used to hold the status of Run Request/Release to notify the BswM as per Run handling protocol*/
uint8 EcuM_Prv_RunRequested_u8;
/*Variable used to hold the status of Post_Run Request/Release to notify the BswM as per Run handling protocol*/
uint8 EcuM_Prv_Post_Run_Requested_u8;
#endif //ECUM_CFG_MODE_HANDLING

#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
/*variable holds the info whether Gopoll called or not*/
EcuM_Prv_SleepType_Enum_ten EcuM_Prv_SleepReqType_en;
/*variable holds the Wakeup Restart Reason*/
EcuM_Prv_WakeupRestartReason_Enum_ten EcuM_Prv_WakeupRestartReason_en;
/*variable used to indicate the wakeup restart event*/
#endif//ECUM_SLEEP_SUPPORT_ENABLE
#define ECUM_STOP_SEC_VAR_CLEARED_8
#include "EcuM_MemMap.h"



#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
/* Variable to store the selected Shutdown target*/
EcuM_ShutdownTargetInfoType EcuM_Prv_dataSelectedShutdownTarget_st;
#if( ECUM_SLEEP_SUPPORT_ENABLE == TRUE )
EcuM_CurrentCoreInfoType EcuM_Prv_CoreStatus_st;
#endif//ECUM_SLEEP_SUPPORT_ENABLE
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VAR_CLEARED_32
#include "EcuM_MemMap.h"
/* Variable to store the Old Pending Wakeup Events*/
EcuM_WakeupSourceType EcuM_Prv_dataOldPendingWakeupEvents_u32;

/* Variable to store the Pending Wakeup Events*/
EcuM_WakeupSourceType EcuM_Prv_dataPendingWakeupEvents_u32;

/* Variable to store the Validated Wakeup Events*/
EcuM_WakeupSourceType EcuM_Prv_dataValidatedWakeupEvents_u32;

/* Variable to store the Expired Wakeup Events*/
EcuM_WakeupSourceType EcuM_Prv_dataExpiredWakeupEvents_u32;

/* Variable to store the Cleared Wakeup Events*/
EcuM_WakeupSourceType EcuM_Prv_dataClrWkpEventsInd_u32;

/* Variable to store the Pending Wakeup Events used for BswM indication in main function for current cycle*/
EcuM_WakeupSourceType EcuM_Prv_dataPndWkpEventsInd_u32;

/* Variable to store the Validated Wakeup Events used for BswM indication in main function for current cycle*/
EcuM_WakeupSourceType EcuM_Prv_dataValWkpEventsInd_u32;

/* Variable to store the Clear Wakeup Events */
EcuM_WakeupSourceType EcuM_Prv_dataExpWkpEventsInd_u32;


#define ECUM_STOP_SEC_VAR_CLEARED_32
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VAR_CLEARED_16
#include "EcuM_MemMap.h"
/* Variable to store the wake up validation Timeout Timer counter */
uint16 EcuM_Prv_WkpSrcValidationTimeoutCtr_u16;

#if (ECUM_SLEEP_SUPPORT_ENABLE== TRUE)
/*Holds the sleep mode for which the EcuM_GoPoll/EcuM_GoHalt called*/
EcuM_ShutdownModeType EcuM_Prv_SleepMode_u16;
#endif//ECUM_SLEEP_SUPPORT_ENABLE
#define ECUM_STOP_SEC_VAR_CLEARED_16
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

/*The ptr to the EcuM whole config pb structure*/
const EcuM_ConfigType * EcuM_Rb_adrConfigPtr_st; /*Pointer to EcuM config Type*/

#if (ECUM_CFG_NVM_BLOCK_CONFIGURED == TRUE)
/* variable to hold the previous shutdown information */
EcuM_ShutdownTargetInfoType EcuM_Prv_dataShutdownInfo_st;
#endif//ECUM_CFG_NVM_BLOCK_CONFIGURED

#define ECUM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VAR_INIT_16
#include "EcuM_MemMap.h"

#if(ECUM_CFG_MODE_HANDLING == STD_ON)
uint16 EcuM_Prv_RunMinDuration_u16 = ECUM_CFG_RUN_SELF_REQUEST_PERIOD;
#endif//ECUM_CFG_MODE_HANDLING

#define  ECUM_STOP_SEC_VAR_INIT_16
#include "EcuM_MemMap.h"


#define ECUM_START_SEC_VAR_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"
/*holds the status of EcuM_Init is called or not*/
boolean EcuM_Prv_flgIsModuleInitialised_b;
/*array used to hold the initalization status of startupTwo*/
boolean EcuM_Prv_flgIsStartupTwoCalled_ab[ECUM_CFG_NUM_OS_CORES];
/* array used to hold the core status to proceed with shutdown*/
boolean EcuM_Prv_flgCoreReady_ab[ECUM_CFG_NUM_OS_CORES];
/*Flag to indicate the updatability of shutdown info*/
boolean EcuM_Prv_flgShutdownInfoDoNotUpdate_b;
#if(ECUM_CFG_MODE_HANDLING == STD_ON)
/*Array of flag to maintain the RUN reuqest status of each user */
boolean EcuM_Prv_userRUNReqStatus_ab[ECUM_CFG_NUM_FLEX_USERS];

/*Array of flag to maintain the POST_RUN reuqest status of each user */
boolean EcuM_Prv_userPostRUNReqStatus_ab[ECUM_CFG_NUM_FLEX_USERS];

/*Variable used to hold the status of EcuM_KillAllRUNRequests*/
boolean EcuM_Prv_KillAllRUNRequests_b = FALSE;

/*Variable used to hold the status of EcuM_KillAllPostRUNRequests*/
boolean EcuM_Prv_KillAllPostRUNRequests_b = FALSE;

#endif//ECUM_CFG_MODE_HANDLING

/*Flag to  validate shutdown from EcuM_GoDown */
boolean EcuM_Prv_flgGoDown_b;

#if (ECUM_CFG_NVM_BLOCK_CONFIGURED == TRUE)
/*flag to determine whether the previous shutdown information has been copied successfully from NV RAM to EcuM
 * prv variable*/
boolean EcuM_Prv_flgNvMReadStatus_b;
#endif//ECUM_CFG_NVM_BLOCK_CONFIGURED

#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
/*variable used to indicate the wakeup restart event*/
boolean EcuM_Prv_flgwakeupRestart_b;
/*Flag to enable clearing the wakeup restart flag*/
boolean EcuM_Prv_flgClearWakeupRestart_b;
/*variable holds the core synchronization info to proceed with polling*/
boolean EcuM_Prv_AllCoreSleepReady_b;
static volatile boolean EcuM_Prv_flgSyncWakeupDetected_b;
#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)
/*array used to hold the sleep status of individual */
volatile boolean EcuM_Prv_flgSleepReady_ab[ECUM_CFG_NUM_OS_CORES];
/*variable result of RAM Integerity check which acts as flag for slave to come out of Halt*/
volatile boolean EcuM_Prv_RAMCheckPassed_b;
/*variable to signal Slave core to continue Polling activity after Master core activity is over*/
volatile boolean EcuM_Prv_PollSlaveCoreContinue_b;
#endif //ECUM_CFG_MULTICORE_ENABLED
#endif//ECUM_SLEEP_SUPPORT_ENABLE
#define ECUM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"


#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
static void EcuM_Prv_HaltingSequence(uint16 dataSleepModeHandle_u16);
static void EcuM_Prv_PollingSequence(uint16 dataSleepModeHandle_u16);
static void EcuM_Prv_McuSetMode( uint8 Mode, EcuM_Prv_OperationType_Enum_ten flgOperation_en);
#endif //ECUM_SLEEP_SUPPORT_ENABLE


#if(ECUM_CFG_MODE_HANDLING == STD_ON)
/***********************************************************************************
  Function name :   EcuM_Prv_IsUserHandlingPossible
  Description   :   EcuM user handling is possible only after the module initialization and the user parameter is valid.
                    This function checks for possiblity of user handling and return TRUE if user handling is possible.
                    This function is called in the following mode handling interfaces EcuM_RequestRUN, EcuM_ReleaseRUN,
                    EcuM_RequestPOST_RUN, EcuM_ReleasePOST_RUN
  Parameter in  :   user , apiid
  Parameter out :   user
  Return value  :   boolean
  Remarks       :
 ************************************************************************************/
boolean EcuM_Prv_IsUserHandlingPossible_b(
        EcuM_UserType * user,
        uint8 apiid)
{

    /* Local Variables */
    boolean return_value = FALSE;
    EcuM_UserType cntrLoop_u16;
    boolean is_validuser_b = FALSE;

    /* Check if the EcuM module is initialised */
    if(FALSE != EcuM_Prv_flgIsModuleInitialised_b)
    {
        for(cntrLoop_u16 = 0;cntrLoop_u16 < ECUM_CFG_NUM_FLEX_USERS;cntrLoop_u16++)
            {
                if(EcuM_Cfg_flexUsers_au16[cntrLoop_u16] == *user)
                {
                    *user = cntrLoop_u16;
                    is_validuser_b = TRUE;
                    break;
                }
            }

        /* Check if the parameter is a valid user */
        if(FALSE != is_validuser_b)
        {
            if (((ECUM_REQUESTRUN_APIID == apiid) && (EcuM_Prv_KillAllRUNRequests_b != FALSE)) ||
                            ((ECUM_REQUESTPOST_RUN_APIID == apiid) && (EcuM_Prv_KillAllPostRUNRequests_b != FALSE)))
            {
                #if (ECUM_DEV_ERROR_DETECT == STD_ON)
                /*DET raised for service called after EcuM_KillAllRUNRequests or EcuM_KillAllPostRUNRequests is called */
                (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, apiid, ECUM_RB_E_ALL_RUN_REQUESTS_KILLED);
                #endif/*ECUM_DEV_ERROR_DETECT*/
            }
            else
            {
                return_value = TRUE;
            }
        }
        else
        {
            #if (ECUM_DEV_ERROR_DETECT == STD_ON)
            /*DET raised for service called with invalid user ID */
            (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, apiid, ECUM_E_INVALID_PAR);
            #endif /*ECUM_DEV_ERROR_DETECT*/
        }
    }
    else
    {
        #if (ECUM_DEV_ERROR_DETECT == STD_ON)
        /*DET raised for service called before EcuM Initialised */
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, apiid, ECUM_E_UNINIT);
        #endif/*ECUM_DEV_ERROR_DETECT*/
    }

    return return_value;

}
#endif /*ECUM_CFG_MODE_HANDLING*/


/***********************************************************************************

  Function name :   EcuM_Prv_WakeupIndication
  Description   :   Check for pending, expired and validated wakeup events and indicates BswM and ComM accordingly
  Parameter in  :   dataPendingWakeupEvents_u32
  Parameter out :
  Return value  :   void
  Remarks       :
 ************************************************************************************/
void EcuM_Prv_WakeupIndication( EcuM_WakeupSourceType dataPendingWakeupEvents_u32 )
{

    /*Local variable declaration*/
    uint8 cntrLoopCtr_u8;
    uint8 ctrLoopPNCNotify_u8;

    /* check for sending Bswm indication for the wakeup sources which is set to pending during set wakeup event */
    if (EcuM_Prv_dataPndWkpEventsInd_u32 != ECUM_NO_WKUP_SRC)
    {
        BswM_EcuM_CurrentWakeup (EcuM_Prv_dataPndWkpEventsInd_u32, ECUM_WKSTATUS_PENDING);
    }

    /* check for sending Bswm indication for the wakeup sources which is set to validated during set wakeup event */
    if (EcuM_Prv_dataValWkpEventsInd_u32 !=ECUM_NO_WKUP_SRC)
    {
        BswM_EcuM_CurrentWakeup (EcuM_Prv_dataValWkpEventsInd_u32, ECUM_WKSTATUS_VALIDATED);


        for(cntrLoopCtr_u8 = 0;cntrLoopCtr_u8 < ECUM_NUM_OF_WKP_SOURCES ; cntrLoopCtr_u8++)
        {
            if (((EcuM_Cfg_idxWakeupSourcesPC_au32 [cntrLoopCtr_u8].WakeupSourceId & EcuM_Prv_dataValWkpEventsInd_u32)
                    != ECUM_NO_WKUP_SRC) &&
                    (EcuM_Cfg_idxWakeupSourcesPC_au32 [cntrLoopCtr_u8].IsComChannelPresent == TRUE))
            {
				/* Switch to either enable or disable call to COMM functions  */
				#if( ECUM_INCLUDE_COMM == STD_ON )
					/*Give ComM Indication*/
					ComM_EcuM_WakeUpIndication (EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].ComChannelReferance);
				#endif   /* ECUM_INCLUDE_COMM */
            }

            /* indicate to ComM module if ComMPNC Channel Reference is associated with the wakeup source */
            if ( ( ( EcuM_Prv_dataValWkpEventsInd_u32 & EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].WakeupSourceId ) != ECUM_NO_WKUP_SRC ) \
                    && ( EcuM_Rb_adrConfigPtr_st->adrWkupPNCRefs[cntrLoopCtr_u8].nbrOfComMPNCRef > 0 ) )
            {

                for ( ctrLoopPNCNotify_u8 = 0; ctrLoopPNCNotify_u8 <  (EcuM_Rb_adrConfigPtr_st->adrWkupPNCRefs[cntrLoopCtr_u8].nbrOfComMPNCRef); ctrLoopPNCNotify_u8++)
                {
					/* Switch to either enable or disable call to COMM functions */
					#if( ECUM_INCLUDE_COMM == STD_ON )
						ComM_EcuM_PNCWakeUpIndication(EcuM_Rb_adrConfigPtr_st->adrWkupPNCRefs[cntrLoopCtr_u8].ListOfPNCRefs[ctrLoopPNCNotify_u8]);
					#endif   /* ECUM_INCLUDE_COMM */
                }

            }
        }
    }

    /* check for sending Bswm indication for the wakeup sources which is cleared  during clear wakeup event */
    if((EcuM_Prv_dataClrWkpEventsInd_u32 != ECUM_NO_WKUP_SRC) && ((EcuM_Prv_dataClrWkpEventsInd_u32 &
            (EcuM_Prv_dataPndWkpEventsInd_u32 | EcuM_Prv_dataValWkpEventsInd_u32|EcuM_Prv_dataExpWkpEventsInd_u32)) ==
                    ECUM_NO_WKUP_SRC))
    {
        BswM_EcuM_CurrentWakeup (EcuM_Prv_dataClrWkpEventsInd_u32, ECUM_WKSTATUS_NONE);
    }

    SchM_Enter_EcuM();  /*Enter Critical Section*/
    EcuM_Prv_dataPndWkpEventsInd_u32 = ECUM_NO_WKUP_SRC;
    EcuM_Prv_dataValWkpEventsInd_u32 = ECUM_NO_WKUP_SRC;
    EcuM_Prv_dataClrWkpEventsInd_u32 = ECUM_NO_WKUP_SRC;
    EcuM_Prv_dataExpWkpEventsInd_u32 = ECUM_NO_WKUP_SRC;
    SchM_Exit_EcuM(); /*leave Critical Sectiont*/

    /* Check whether any new wakeup event has occured. If so start the new wakeupsource using EcuM_StartWakeupSource api */
    if (dataPendingWakeupEvents_u32 != (EcuM_Prv_dataOldPendingWakeupEvents_u32&dataPendingWakeupEvents_u32))
    {
        EcuM_StartWakeupSources (EcuM_Prv_dataOldPendingWakeupEvents_u32 ^ dataPendingWakeupEvents_u32);
    }

    /*Update the EcuM_Prv_dataOldPendingWakeupEvents_u32 with the latest pending wakeup events, so that with
     * the next main cycle these sources are not getting started again*/
    EcuM_Prv_dataOldPendingWakeupEvents_u32 = dataPendingWakeupEvents_u32;

    }


/***********************************************************************************

  Function name :   EcuM_Prv_DecValidationCtr
  Description   :   Decrements the validation timeout counter value by 1 if it is greater than zero
  Parameter in  :   None
  Parameter out :
  Return value  :   void
  Remarks       :
 ************************************************************************************/
void EcuM_Prv_DecValidationCtr( void )
{

    /*This check is to perform to avoid sub zero value for wakeup validation timeout*/
    if (EcuM_Prv_WkpSrcValidationTimeoutCtr_u16 > 0)
    {
        SchM_Enter_EcuM();  /*Enter Critical Section*/
        /*Decrement the validation timeout counter value*/
        EcuM_Prv_WkpSrcValidationTimeoutCtr_u16--;
        SchM_Exit_EcuM(); /*leave Critical Sectiont*/
    }

}

/***********************************************************************************
  Function name :   EcuM_Prv_PBPointerConsistencyCheck
  Description   :   Function to check the consistency of the Post Build pointer. For this
                    purpose the pointer is checked against NULL Pointer, Invalid Pointer
                    and Pre Compile hash, subsequently Error hooks are thrown accordingly.
  Parameter in  :   void
  Parameter out :
  Return value  :   void
  Remarks       :
 ************************************************************************************/

void EcuM_Prv_PBPointerConsistencyCheck(void)
{
        /*local variables*/
        boolean flgupdateValid = FALSE;
        uint8 cntrLoopCtr_u8;
        const uint8 * adrTempPBHashPtr_u8;

        if(EcuM_Rb_adrConfigPtr_st == NULL_PTR)
            {
                EcuM_ErrorHook( ECUM_E_NULL_PTR_RETURNED );
            }

            /*Check the selected post build pointer EcuM_Rb_adrConfigPtr_st, if it is a invalid pointer
             * ie.) a pointer which is not generated from configured EcuM Configsets*/
        for(cntrLoopCtr_u8 = 0;cntrLoopCtr_u8 < ECUM_NO_OF_ECUMCONFIGSETS; cntrLoopCtr_u8++)
        {
            if(EcuM_Rb_adrConfigPtr_st == EcuM_EcuMConfigurations_cpcast[cntrLoopCtr_u8])
            {
                flgupdateValid = TRUE;
                break;
            }

        }
        /*if the selected post build pointer is a invalid then call EcuM_ErrorHook() with ECUM_E_INVALID_PTR_RETURNED as
         * parameter*/
        if(flgupdateValid != TRUE)
        {
            EcuM_ErrorHook( ECUM_E_INVALID_PTR_RETURNED );
        }
        /*
         Checking consistency of configuration data
         The array of Hash values generated at the time of PB ,it's base address is moved to a local ptr
         */
        adrTempPBHashPtr_u8 = &(EcuM_Rb_adrConfigPtr_st->PostBuildHash[0]);
        for (cntrLoopCtr_u8 = 0; cntrLoopCtr_u8 < ECUM_CFG_LENGTH_OF_HASH; cntrLoopCtr_u8++)
        {
            if( EcuM_Cfg_dataPreCompileHash_au8[cntrLoopCtr_u8] != adrTempPBHashPtr_u8[cntrLoopCtr_u8] )
            {
                /*Configuration check is failed and error hook is called here. It is up the
                system integrator to react accordingly.
                This function will never returns.*/
                EcuM_ErrorHook( ECUM_E_CONFIGURATION_DATA_INCONSISTENT );

            }
        }

}

/***********************************************************************************
  Function name :   EcuM_Prv_SetDefinedMcuWakeupSource
  Description   :   The reset reason is obtained from MCU and if it is a defined reset reason
                    the same reason is set as WakeupEvent in this method.
  Parameter in  :   void
  Parameter out :
  Return value  :   void
  Remarks       :
 ************************************************************************************/

void EcuM_Prv_SetDefinedMcuWakeupSource( void )
{

    Mcu_ResetType dataMcuResetReason_e = MCU_RESET_UNDEFINED;
    uint8 cntrLoopCtr_u8;

    dataMcuResetReason_e = Mcu_GetResetReason();

    if (dataMcuResetReason_e != MCU_RESET_UNDEFINED)
    {
        /*loop through all the configured wkup source and find which one is the cause
                for reset by mapping the reset reason that is obtained from the MCU driver*/
        for ( cntrLoopCtr_u8 = 0; cntrLoopCtr_u8 < ECUM_NUM_OF_WKP_SOURCES; cntrLoopCtr_u8++ )
        {
            if ( EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].ResetReason == dataMcuResetReason_e )
            {
                /*Set the wakeup event for  wakeup sources which is mapped to the reset reason which
                 * matches with the Mcu reset reason*/
                EcuM_SetWakeupEvent( EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].WakeupSourceId );

            }

        }
    }
}
#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
/***********************************************************************************
  Function name :   EcuM_Prv_StartSlaveCores
  Description   :   For Multicore configuration, the slave cores are started by the master core, if the core
                    starting failed ErrorHook is called with ECUM_E_START_CORE_FAILED .
  Parameter in  :   void
  Parameter out :
  Return value  :   void
  Remarks       :
 ************************************************************************************/
void EcuM_Prv_StartSlaveCores( void )
{
/*local variables*/
    StatusType dataStatus_chr = E_NOT_OK;
    uint16 cntrLoopCtr_u16;
 /*Starting all the OS Cores in a loop*/


        for( cntrLoopCtr_u16=0; cntrLoopCtr_u16<ECUM_CFG_NUM_OS_CORES ; cntrLoopCtr_u16++ )
        {
            StartCore( cntrLoopCtr_u16, &dataStatus_chr);
            if(dataStatus_chr != E_OK)
            {
                /* StartCore Failed*/

                EcuM_ErrorHook(ECUM_E_START_CORE_FAILED);

            }
        }

}
#endif//ECUM_CFG_MULTICORE_ENABLE

/***********************************************************************************
  Function name :   EcuM_Prv_StartOS
  Description   :   After initialization the control is handed over to OS by the StartOS Macro with
                    the Default AppMode from the PB pointer.
  Parameter in  :   void
  Parameter out :
  Return value  :   void
  Remarks       :
 ************************************************************************************/
void EcuM_Prv_StartOS(void)
{
    StartOS( EcuM_Rb_adrConfigPtr_st->DefaultAppMode )
}

/***********************************************************************************
  Function name :   EcuM_Prv_hasCheckWakeupSourceGetHandle_b
  Description   :   To check whether the source is a configured known WakeupSource
                    to EcuM. If yes returns the ValidityFlag as TRUE.
  Parameter in  :   wakeup sources
  Parameter out :   None
  Return value  :   boolean
  Remarks       :
 ************************************************************************************/
boolean EcuM_Prv_hasCheckWakeupSourceGetHandle_b( EcuM_WakeupSourceType datasource_u32 )
{
    /*Local variable declaration*/
    boolean flgValidity_b = FALSE;

    /* check all the configured wakeup sources with the passed wkup sources */
    if((datasource_u32 != ECUM_NO_WKUP_SRC) && ((ECUM_KNOWN_WAKEUP_SOURCES & datasource_u32) == datasource_u32) )
	{
	   flgValidity_b = TRUE;

	}
	else
	{
	   /*If any of the passed wakeup source(s) is not valid then the flgValidity_b value is ratained as FALSE*/
	}
    return flgValidity_b;
}

#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
/**********************************************************************************************************************
  Function name :   EcuM_Prv_GoSleepSequence
  Description   :   This api is common to EcuM_GoHalt and EcuM_GoPoll.In GoSleep sequence the ECU Manager module
                    configures hardware for the upcoming sleep phase and sets the ECU up for the next wakeup event.
  Parameter in  :   dataSleepModeHandle_u16 which has the index of the selected sleep mode in
                    EcuM_Cfg_idxSleepModes_au32 array
  Parameter out :   None
  Return value  :   None
  Remarks       :
 *********************************************************************************************************************/
void EcuM_Prv_GoSleepSequence(uint16 dataSleepModeHandle_u16)
                {

    /*Local variable declaration*/
    StatusType Os_api_returnvalue = E_NOT_OK;
    EcuM_WakeupSourceType EcuM_dataPendingWakeupEvents_u32 = 0;
    EcuM_WakeupSourceType EcuM_dataValidatedWakeupEvents_u32 = 0;
    EcuM_Prv_SleepType_Enum_ten RequestedSleepType_en;
    uint16 Core_ID = EcuM_Prv_CoreStatus_st.CurrentCoreID;

    if(((EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_MASTER_CORE) && (FALSE != EcuM_Prv_AllCoreSleepReady_b)) ||
            (EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_SINGLE_CORE))
    {
        /*Get a resource for EcuM so that it cannot be interrupted by other Os tasks*/
        Os_api_returnvalue = EcuM_Prv_GetResource (Core_ID);
    }
#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)
    else if((EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_SLAVE_CORE) && (FALSE == EcuM_Prv_flgSleepReady_ab[Core_ID]) &&
            (FALSE != EcuM_Prv_flgSleepReady_ab[ECUM_CFG_STARTUP_CORE]))
    {
        /*Get a resource for EcuM so that it cannot be interrupted by other Os tasks*/
        Os_api_returnvalue = EcuM_Prv_GetResource (Core_ID);
        /*update the core status that gosleep sequence executed successfully*/
         EcuM_Prv_flgSleepReady_ab[Core_ID] = TRUE;
    }
#endif//ECUM_CFG_MULTICORE_ENABLED
    else
    {
        /*nothing to do*/

    }

    if(((Os_api_returnvalue == E_OK) && (EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_MASTER_CORE) &&
            (FALSE != EcuM_Prv_AllCoreSleepReady_b))||
            ((Os_api_returnvalue == E_OK) && (EcuM_Prv_CoreStatus_st.CurrentCore != ECUM_MASTER_CORE)))

    {
        SchM_Enter_EcuM();             /*Enter Critical Section*/
        EcuM_dataPendingWakeupEvents_u32 = EcuM_Prv_dataPendingWakeupEvents_u32;
        EcuM_dataValidatedWakeupEvents_u32 = EcuM_Prv_dataValidatedWakeupEvents_u32;
        SchM_Exit_EcuM();                /*Leave Critical Section*/
        if((ECUM_NO_WKUP_SRC == EcuM_dataPendingWakeupEvents_u32) &&
                (ECUM_NO_WKUP_SRC == EcuM_dataValidatedWakeupEvents_u32))
        {
            /*Proceed with halting sequence in case of successful resource acquiring*/
            /*Getting Sleep Type in critical region*/
            SchM_Enter_EcuM();            /*Enter Critical Section*/
            RequestedSleepType_en = EcuM_Prv_SleepReqType_en;
            SchM_Exit_EcuM();               /*Leave Critical Section*/

            if(ECUM_PRV_GOHALT_E == RequestedSleepType_en)
            {
                EcuM_Prv_HaltingSequence(dataSleepModeHandle_u16);

            }
            else
            {
                if(ECUM_PRV_GOPOLL_E == RequestedSleepType_en)
                 {

                     EcuM_Prv_PollingSequence(dataSleepModeHandle_u16);

                 }
                 else
                 {
                       /* do nothing */
                 }

            }

        }

        else
        {
            EcuM_Prv_WakeupRestartReason_en = ECUM_PRV_WAKEUP_REQUEST_E;

        }
    }
    /*report DET error if resource acquiring failed for all conditions except when core is master core and other cores are all not sleep ready yet*/
    else  if ((!((EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_MASTER_CORE) && (FALSE == EcuM_Prv_AllCoreSleepReady_b)))\
            &&(Os_api_returnvalue != E_OK))
    {
        EcuM_Prv_WakeupRestartReason_en = ECUM_PRV_RESOURCE_NOT_ACQUIRED_E;
#if(ECUM_DEV_ERROR_DETECT == STD_ON)
        /*report error for unsuccessful resource acquiring*/
        (void)Det_ReportError( ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_MAIN_FUNC_APIID, ECUM_E_OS_GET_RESOURCE_FAILED );
#endif//ECUM_DEV_ERROR_DETECT
    }
    else
    {
        //nothing to do
    }

    /*if resource acquiring failed then quit from sleep sequence and
     * execute wakeup restart sequence for the cores where Gosleep sequence executed*/
    if((Os_api_returnvalue != E_OK) || \
            ((ECUM_PRV_NORMAL_E != EcuM_Prv_WakeupRestartReason_en ) && \
                    (ECUM_PRV_CHECK_WAKEUP_PENDING_E != EcuM_Prv_WakeupRestartReason_en )))
    {
        SchM_Enter_EcuM();          /*Enter Critical Section*/
        /*signal for wakeup restart*/
        EcuM_Prv_flgwakeupRestart_b = TRUE;
        /*signal to quit sleep mode*/
        EcuM_Prv_SleepReqType_en = ECUM_PRV_NO_SLEEP_E;

        SchM_Exit_EcuM();             /*Leave Critical Section*/
    }
    return;
}

/**********************************************************************************************************************
  Function name :   EcuM_Prv_HaltingSequence
  Description   :   In Halting sequence, The ECU Manager module shall generate RAM content
                    Hash (callout) and enters into MCU halt mode.
                    In case Multicore, slave cores signals the master just before entering MCU halt mode.
                    Master core waits for all slave core to sleep and generates the Hash for RAM content
                    (system designer has to implement corresponding code for Hash computation).
  Parameter in  :   dataSleepModeHandle_u16 - the index of the selected sleep mode in EcuM_Cfg_idxSleepModes_au32 array
  Parameter out :   None
  Return value  :   None
  Remarks       :
 *********************************************************************************************************************/
static void EcuM_Prv_HaltingSequence(uint16 dataSleepModeHandle_u16)
        {
    EcuM_Prv_OperationType_Enum_ten flgOperation_en = ECUM_PRV_HALT_SEQUENCE_E;
    /*Internal variable which holds the bit mask of wakeup events that are in pending state*/
    EcuM_WakeupSourceType dataPendingWakeupEvents_u32 = ECUM_NO_WKUP_SRC;
    /*Internal variable which holds the bit mask of wakeup events that are in validated state*/
    EcuM_WakeupSourceType dataValidatedWakeupEvents_u32 = ECUM_NO_WKUP_SRC;

    /*update phase variable to indicate sleep mode*/
    EcuM_Prv_dataPresentPhase_u8 = ECUM_PRV_PHASE_SLEEP;

    if ((EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_SINGLE_CORE) || (EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_MASTER_CORE))
    {
        /*Callout (to be implemented by System designer) which calculates the Hash/checksum of RAM contents*/
        EcuM_GenerateRamHash();
    }

    EcuM_Prv_McuSetMode(EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].EquivalentMcuMode, flgOperation_en);

    if( (EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_SINGLE_CORE)||
            (EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_MASTER_CORE))
    {
        if( (uint8)0 == EcuM_CheckRamHash())
        {
            /*Call the EcuM Error Hook, Because the RAM integrity check has failed.*/
            EcuM_ErrorHook(ECUM_E_RAM_CHECK_FAILED);
        }
    }

    /*Check for the occurence of wakeup events*/
    SchM_Enter_EcuM();        /*Enter Critical Section*/
    /*Get the latest PENDING events list*/
    dataPendingWakeupEvents_u32 = EcuM_Prv_dataPendingWakeupEvents_u32;
    /*Get the latest VALIDATED events list*/
    dataValidatedWakeupEvents_u32 = EcuM_Prv_dataValidatedWakeupEvents_u32;
    SchM_Exit_EcuM();           /*Leave Critical Section*/

    if (ECUM_NO_WKUP_SRC != (dataPendingWakeupEvents_u32 &
            EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask))
    {
        /*Indicate wake up status change to BswM as ECUM_WKSTATUS_PENDING*/
        BswM_EcuM_CurrentWakeup((dataPendingWakeupEvents_u32 &
                EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask),ECUM_WKSTATUS_PENDING);

        /*Indication variable may get modified if any pending wakeup event associated with the current sleep mode
         * occurs. If so clear the variable to avoid multiple wakeup status indication to BswM*/
        EcuM_Prv_dataPndWkpEventsInd_u32 ^= (EcuM_Prv_dataPndWkpEventsInd_u32 &
                EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask);
    }

    if(ECUM_NO_WKUP_SRC != (dataValidatedWakeupEvents_u32 &
                                                EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask))
    {
        /*Indicate wake up status change to BswM as ECUM_WKSTATUS_VALIDATED*/
        BswM_EcuM_CurrentWakeup((dataValidatedWakeupEvents_u32 &
                EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask),ECUM_WKSTATUS_VALIDATED);

        /*Indication variable may get modified if any wakeup source associated with the current sleep mode is validated.
         * If so clear the variable to avoid multiple wakeup status indication to BswM*/
        EcuM_Prv_dataValWkpEventsInd_u32 ^= (EcuM_Prv_dataValWkpEventsInd_u32 &
                EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask) ;

    }

#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
    if(EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_MASTER_CORE)
    {
        /*Signal the slave to come out of Halt*/
        EcuM_Prv_RAMCheckPassed_b = TRUE;
    }
    else//if slave core
    {
        /*Waiting for the master core to continue*/
        while(FALSE == EcuM_Prv_RAMCheckPassed_b )
        {
            /*Looping here until master core checks for RAM Integerity*/
        }
    }

#endif//ECUM_CFG_MULTICORE_ENABLED

    /*Proceeding for Wakeup Restart Sequence*/
    EcuM_Prv_WakeupRestartSequence(dataSleepModeHandle_u16);
        }
/***********************************************************************************
  Function name :   EcuM_Prv_PollingSequence
  Description   :   In Polling sequence, The ECU Manager module shall execute the Poll Sequence in sleep modes
                     that reduce the power consumption of the microcontroller but still execute
  Parameter in  :   dataSleepModeHandle_u8 which has the index of the selected sleep mode in  EcuM_Cfg_idxSleepModes_au32 array
  Parameter out :   None
  Return value  :   None
  Remarks       :
 ************************************************************************************/
static void EcuM_Prv_PollingSequence(uint16 dataSleepModeHandle_u16)
        {

#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)
 uint16 Core_ID;
#endif//ECUM_CFG_MULTICORE_ENABLED

    EcuM_Prv_OperationType_Enum_ten flgOperation_en = ECUM_PRV_POLL_SEQUENCE_E;

    /*Internal variable which holds the bit mask of wakeup events that are in pending state*/
    EcuM_WakeupSourceType dataPendingWakeupEvents_u32 = ECUM_NO_WKUP_SRC;

    /*Internal variable which holds the bit mask of wakeup events that are in validated state*/
    EcuM_WakeupSourceType dataValidatedWakeupEvents_u32 = ECUM_NO_WKUP_SRC;

    /*internal variable to store the iteration value*/
    uint8 cntrLoopCtr_u8 =0;

    /*update phase variable to indicate sleep mode*/
    EcuM_Prv_dataPresentPhase_u8 = ECUM_PRV_PHASE_SLEEP;

    EcuM_Prv_McuSetMode(EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].EquivalentMcuMode, flgOperation_en);

    #if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
    /*Get the active core id from Os*/
    Core_ID = GetCoreID();
    if(Core_ID == ECUM_CFG_STARTUP_CORE)
    {
    #endif//ECUM_CFG_MULTICORE_ENABLED

    /*Get the latest PENDING events list*/
    dataPendingWakeupEvents_u32 = EcuM_GetPendingWakeupEvents();

    /*Get the latest VALIDATED events list*/
    dataValidatedWakeupEvents_u32 = EcuM_GetValidatedWakeupEvents();

    /*Stay in a blocking loop until a  wakeup event is detected. The state of the wakeup event can be either pending or validated. */
    while (((dataPendingWakeupEvents_u32 | dataValidatedWakeupEvents_u32) &
                (EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask)) == ECUM_NO_WKUP_SRC)
    {
        /*EcuM_SleepActivity - Callout for integrators */
        EcuM_SleepActivity();

        /*Check  wakeupsources for validation for the wakeup sources that needs polling*/
        /*Loop through the configured wakeupsources*/
        for ( cntrLoopCtr_u8 = 0; cntrLoopCtr_u8 < ECUM_NUM_OF_WKP_SOURCES; cntrLoopCtr_u8++ )
        {
            /*Check whether the wakeup sources assosiated with the sleep mode needs polling or not */
            if (((EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].WakeupSourceId) & (EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask)) != ECUM_NO_WKUP_SRC )
            {


                    /*Check  wakeupsources for validation for the wakeup sources that needs polling*/
                    EcuM_CheckWakeup(EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].WakeupSourceId);

            }

        }
        /*Check for the occurence of wakeup events*/

        /*Get the latest PENDING events list*/
        dataPendingWakeupEvents_u32 = EcuM_GetPendingWakeupEvents();
        /*Get the latest VALIDATED events list*/
        dataValidatedWakeupEvents_u32 = EcuM_GetValidatedWakeupEvents();

    }

    if ((dataPendingWakeupEvents_u32 & EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask) != ECUM_NO_WKUP_SRC)
    {
       /*Indicate wake up status change to BswM as ECUM_WKSTATUS_PENDING*/
        BswM_EcuM_CurrentWakeup((dataPendingWakeupEvents_u32 & EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask),ECUM_WKSTATUS_PENDING);

        /*Indication variable may get modified if any pending wakeup event associated with the current sleep mode occurs. If so clear the variable to avoid multiple wakeup status indication to BswM*/
        EcuM_Prv_dataPndWkpEventsInd_u32 ^= EcuM_Prv_dataPndWkpEventsInd_u32 & EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask;
    }

    if ((dataValidatedWakeupEvents_u32 & EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask) != ECUM_NO_WKUP_SRC)
    {
        /*Indicate wake up status change to BswM as ECUM_WKSTATUS_VALIDATED*/
        BswM_EcuM_CurrentWakeup((dataValidatedWakeupEvents_u32 & EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask),ECUM_WKSTATUS_VALIDATED);

        /*Indication variable may get modified if any wakeup source associated with the current sleep mode is validated. If so clear the variable to avoid multiple wakeup status indication to BswM*/
        EcuM_Prv_dataValWkpEventsInd_u32 ^= EcuM_Prv_dataValWkpEventsInd_u32 & EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask;
    }

    #if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
    }

    if(EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_MASTER_CORE)
    {
        /*Signal the slave to come out of Poll*/
        EcuM_Prv_PollSlaveCoreContinue_b = TRUE;
    }
    else//if slave core
    {
        /*Waiting for the master core to continue*/
        while(FALSE == EcuM_Prv_PollSlaveCoreContinue_b )
        {
            /*Looping here until master core signals slave core to continue*/
        }
    }

    #endif//ECUM_CFG_MULTICORE_ENABLED


        /*Proceeding for Wakeup Restart Sequence*/
        EcuM_Prv_WakeupRestartSequence(dataSleepModeHandle_u16);

 }
/***********************************************************************************
  Function name :   EcuM_Prv_WakeupRestartSequence
  Description   :   It will perform the following operations. Restore Mcu to normal mode,
                    Initialize drivers that needs a restart and release the Os resource so that the other Os task
                    starts running.
  Parameter in  :   dataSleepModeHandle_u16 - the index of the selected sleep mode in EcuM_Cfg_idxSleepModes_au32 array
  Parameter out :   None
  Return value  :   none
  Remarks       :
 ************************************************************************************/
void EcuM_Prv_WakeupRestartSequence ( uint16 dataSleepModeHandle_u16 )
      {
    EcuM_WakeupSourceType dataWakeupSrcMask_u32 = ECUM_NO_WKUP_SRC;
    /*Local variable for storing the return value of os_GetResource()*/
    StatusType Os_api_returnvalue = E_NOT_OK;
    EcuM_Prv_OperationType_Enum_ten flgOperation_en = ECUM_PRV_RESTART_SEQUENCE_E;
#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)
    uint16 cntrLoopOsCore_u16;

    if(FALSE != EcuM_Prv_flgSleepReady_ab[EcuM_Prv_CoreStatus_st.CurrentCoreID])
    {

#endif//ECUM_CFG_MULTICORE_ENABLED

        if (ECUM_PRV_NORMAL_E == EcuM_Prv_WakeupRestartReason_en)
        {
            EcuM_Prv_McuSetMode (ECUM_NORMAL_MCU_MODE, flgOperation_en);
        }

        dataWakeupSrcMask_u32 = EcuM_GetPendingWakeupEvents();
        /*[SWS_EcuM_04084] The ECU Manager module shall derive the wakeup sources to be disabled (and used as the
         *  wakeupSource parameter) from the internal pending events variable (NOT operation). The integration code
         * used for this callout must determine which wakeup sources must be disabled.*/
        dataWakeupSrcMask_u32 = (EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask &
                (~dataWakeupSrcMask_u32));
        /*Disable currently pending wakeup sources */
        EcuM_DisableWakeupSources(dataWakeupSrcMask_u32);

        if (ECUM_PRV_RESOURCE_NOT_ACQUIRED_E != EcuM_Prv_WakeupRestartReason_en)
        {
            if((EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_SINGLE_CORE) || \
                    (EcuM_Prv_CoreStatus_st.CurrentCore == ECUM_MASTER_CORE))
            {
                /*Re initialize the Drivers configured in Driver init restartlist in EcuM*/
                EcuM_AL_DriverRestart (EcuM_Rb_adrConfigPtr_st);
            }
            /*Release the resource for EcuM so that it cannot be interrupted by other Os tasks*/
            Os_api_returnvalue = EcuM_Prv_ReleaseResource (EcuM_Prv_CoreStatus_st.CurrentCoreID);
        }

        if(Os_api_returnvalue != E_OK)
        {
            /*Report error if release resource failed*/
#if(ECUM_DEV_ERROR_DETECT == STD_ON)
            (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_MAIN_FUNC_APIID,
                    ECUM_E_OS_RELEASE_RESOURCE_FAILED);
#endif//ECUM_DEV_ERROR_DETECT

        }
#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)

        SchM_Enter_EcuM();        /*Enter Critical Section*/
        /*clear the sleep ready flag after wakeup restart sequence executed*/
        EcuM_Prv_flgSleepReady_ab[EcuM_Prv_CoreStatus_st.CurrentCoreID] = FALSE;

        SchM_Exit_EcuM();           /*Leave Critical Section*/

    }
    /*clear wakeup restart flag once wakeup restart sequence executed for all the cores */
    for(cntrLoopOsCore_u16 = 0; cntrLoopOsCore_u16 < ECUM_CFG_NUM_OS_CORES ; cntrLoopOsCore_u16++)
    {
        if (FALSE != EcuM_Prv_flgSleepReady_ab[cntrLoopOsCore_u16])
        {
            SchM_Enter_EcuM();        /*Enter Critical Section*/
            EcuM_Prv_flgClearWakeupRestart_b = FALSE;
            SchM_Exit_EcuM();           /*Leave Critical Section*/
            break;
        }
        else
        {
#endif//ECUM_CFG_MULTICORE_ENABLED

            SchM_Enter_EcuM();        /*Enter Critical Section*/
            EcuM_Prv_flgClearWakeupRestart_b = TRUE;
            SchM_Exit_EcuM();           /*Leave Critical Section*/

#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)
        }
    }
    if(TRUE == EcuM_Prv_flgClearWakeupRestart_b)
    {
#endif//ECUM_CFG_MULTICORE_ENABLED

        SchM_Enter_EcuM();        /*Enter Critical Section*/
        /*clear wakeup restart flag once wakeup restart sequence executed for all the cores */
        EcuM_Prv_flgwakeupRestart_b = FALSE;
        EcuM_Prv_flgClearWakeupRestart_b = FALSE;
        /*Set the wakeup Restart reason to Normal for next gopoll or gohalt request*/
        EcuM_Prv_WakeupRestartReason_en = ECUM_PRV_NORMAL_E;
        SchM_Exit_EcuM();           /*Leave Critical Section*/

#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)
    }
#endif//ECUM_CFG_MULTICORE_ENABLED

  flgOperation_en = ECUM_PRV_CLEAR_SLEEP_E;
    /*check for detection of asynchronous wakeup and accordingly, clear or retain sleep type flag,
     * EcuM_Prv_flgShutdownInfoDoNotUpdate_b and sleep mode*/
  EcuM_Prv_McuSetMode( (uint8) 0, flgOperation_en);
  (void)EcuM_Prv_CheckWakeupTimerHandling();
              }

/***********************************************************************************
  Function name :   EcuM_Prv_McuSetMode
  Description   :   It will set the MCU mode to the one mapped with selected sleep mode
  Parameter in  :   Mode, flgOperation_en
  Parameter out :   None
  Return value  :   void
  Remarks       :
 ************************************************************************************/

static void EcuM_Prv_McuSetMode( uint8 Mode, EcuM_Prv_OperationType_Enum_ten flgOperation_en)
                {
    /*Local variable declaration*/
    uint8 ctrLoop1_u8 = 0;



    if(flgOperation_en == ECUM_PRV_HALT_SEQUENCE_E)
    {

        SchM_Enter_EcuM();       /*enter Critical Section*/

        EcuM_Prv_flgSyncWakeupDetected_b = FALSE;

        while(FALSE == EcuM_Prv_flgSyncWakeupDetected_b)
        {
            /* Disable all interrupts before changing the Mcu Mode*/
            DisableAllInterrupts();

            /*Change Mcu mode with the Mcu mode mapped with the selected sleep mode.*/
            Mcu_SetMode(Mode);

            /*Enable All the interrupts*/
            EnableAllInterrupts();

            for( ctrLoop1_u8 = 0;ctrLoop1_u8 < ECUM_NUM_OF_WKP_SOURCES;ctrLoop1_u8++ )
            {
                /*if for any of the newly detected sync wakeup sources the EcuM_SetWakeupEvent was reported or for any async wakeup timer was started*/
                if((EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus== ECUM_SET_WAKEUP_EVENT_E)\
                        || (EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus== ECUM_TIMER_STARTED_E))
                {
                    EcuM_Prv_flgSyncWakeupDetected_b = TRUE;
                    break;
                }
            }
        }
        SchM_Exit_EcuM();        /*Leave Critical Section*/
    }
    /*if halting sequence operation for wakeuprestart sequence for all cores*/
 else if((flgOperation_en == ECUM_PRV_RESTART_SEQUENCE_E) || ( flgOperation_en == ECUM_PRV_POLL_SEQUENCE_E ))
    {
        /* Disable all interrupts before changing the Mcu Mode*/
        DisableAllInterrupts();

        /*Change Mcu mode with the Mcu mode mapped with the selected sleep mode.*/
        Mcu_SetMode(Mode);

        /*Enable All the interrupts*/
        EnableAllInterrupts();
    }
    else
    {
        /*nothing to do*/
    }

    if (flgOperation_en == ECUM_PRV_CLEAR_SLEEP_E)
    {

        for( ctrLoop1_u8 = 0;ctrLoop1_u8 < ECUM_NUM_OF_WKP_SOURCES;ctrLoop1_u8++ )
        {
            if(ECUM_SET_WAKEUP_EVENT_E == EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus)
            {
                break;
            }
            /*if check for occurrence of async wakeups is being done for the first time i.e. during executing wakeuprestartsequence*/
            else if(ECUM_TIMER_STARTED_E == EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus)
            {
                SchM_Enter_EcuM();       /*enter Critical Section*/
                /*Set the wakeup Restart reason to indicate that an asynchronous wakeup has been detected so that no further
                 * gopoll or gohalt request is accepted*/
                EcuM_Prv_WakeupRestartReason_en = ECUM_PRV_CHECK_WAKEUP_PENDING_E;
                /*do not allow the updation of shutdown targets until the wakeup check is complete*/
                EcuM_Prv_flgShutdownInfoDoNotUpdate_b = TRUE;
                /*timer decrementing should be effective only from next mainfunction cycle onwards*/
                EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupTimeout = EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupTimeout + 0x01;
                SchM_Exit_EcuM();        /*Leave Critical Section*/
            }
            else
            {
                /*nothing to be executed here*/
            }
        }
    }
    else
    {
        /*nothing to be executed here*/
    }
    return ;
}

#endif//ECUM_SLEEP_SUPPORT_ENABLE


/***********************************************************************************
  Function name :   EcuM_Prv_ComMWakeupHandling
  Description   :   This service will indicate to ComM module if for a particular wakeup Communication Channel is
                    configured and  Validation is off and whether if ComMPNC Channel Reference is associated with the wakeup source
  Parameter in  :   dataPendingWakeupEvents
  Parameter out :   None
  Return value  :   uint32
  Remarks       :
 ************************************************************************************/
uint32 EcuM_Prv_ComMWakeupHandling (EcuM_WakeupSourceType dataPendingWakeupEvents)
        {
    // Variable used to hold only the wakeup sources with Comm channel reference from the passed sources
    uint32 EcuM_CommchlWkupEvents_u32 = 0;
    uint8 ctrLoop_u8 = 0;
    uint8 ctrLoopPNCNotify_u8 = 0;
    /* splitting the wakeup events which are having comm channel reference */
    for( ctrLoop_u8 = 0;ctrLoop_u8 < ECUM_NUM_OF_WKP_SOURCES;ctrLoop_u8++ )
    {

        /* indicate to ComM module if Communication Channel is configured and  Validation is off */
        if ( ( ( dataPendingWakeupEvents & EcuM_Cfg_idxWakeupSourcesPC_au32[ctrLoop_u8].WakeupSourceId ) != ECUM_NO_WKUP_SRC ) \
                && ( EcuM_Cfg_idxWakeupSourcesPC_au32[ctrLoop_u8].IsComChannelPresent == TRUE ) )
        {

			/* Switch to either enable or disable call to COMM functions */
			#if( ECUM_INCLUDE_COMM == STD_ON )
				ComM_EcuM_WakeUpIndication ( EcuM_Cfg_idxWakeupSourcesPC_au32[ctrLoop_u8].ComChannelReferance );
			#endif  /* ECUM_INCLUDE_COMM */

			EcuM_CommchlWkupEvents_u32 |= EcuM_Cfg_idxWakeupSourcesPC_au32[ctrLoop_u8].WakeupSourceId;

        }

        /* indicate to ComM module if ComMPNC Channel Reference is associated with the wakeup source */
        if ( ( ( dataPendingWakeupEvents & EcuM_Cfg_idxWakeupSourcesPC_au32[ctrLoop_u8].WakeupSourceId ) != ECUM_NO_WKUP_SRC ) \
                && ( EcuM_Rb_adrConfigPtr_st->adrWkupPNCRefs[ctrLoop_u8].nbrOfComMPNCRef > 0 ) )
        {

            for ( ctrLoopPNCNotify_u8 = 0; ctrLoopPNCNotify_u8 <  (EcuM_Rb_adrConfigPtr_st->adrWkupPNCRefs[ctrLoop_u8].nbrOfComMPNCRef); ctrLoopPNCNotify_u8++)
            {
				/* Switch to either enable or disable call to COMM functions */
				#if( ECUM_INCLUDE_COMM == STD_ON )
					ComM_EcuM_PNCWakeUpIndication(EcuM_Rb_adrConfigPtr_st->adrWkupPNCRefs[ctrLoop_u8].ListOfPNCRefs[ctrLoopPNCNotify_u8]);
				#endif  /* ECUM_INCLUDE_COMM */
            }

        }

    }

    return EcuM_CommchlWkupEvents_u32;
        }
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

