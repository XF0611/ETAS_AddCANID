

/****************************************************************************************************/
/* Preprocessor includes                                                                            */
/****************************************************************************************************/

#include "EcuM.h" /*BSW_HeaderInc_002*/

#include "EcuM_Callout.h"

/*todo : This header has to be removed once Rte tool is ready with exclusive area generation*/
#include "EcuM_Cfg_SchM.h"

#if(ECUM_CFG_ECUC_RB_RTE_IN_USE != FALSE)
#include "SchM_EcuM.h"
#endif//ECUM_CFG_ECUC_RB_RTE_IN_USE

/*Extern module Headers*/
#include "BswM_EcuM.h"
// #if (!defined(BSWM_AR_RELEASE_MAJOR_VERSION) || (BSWM_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(BSWM_AR_RELEASE_MINOR_VERSION) || (BSWM_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif

#include "Rte_Main.h" /*For SchM_DeInit*/

#if(ECUM_DEV_ERROR_DETECT == STD_ON)/*Det included when Det is enabled in EcuM*/
#include "Det.h"
// #if (!defined(DET_AR_RELEASE_MAJOR_VERSION) || (DET_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(DET_AR_RELEASE_MINOR_VERSION) || (DET_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif
#endif/*ECUM_DEV_ERROR_DETECT*/

#include "Os.h"
// #if (!defined(OS_AR_RELEASE_MAJOR_VERSION) || (OS_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(OS_AR_RELEASE_MINOR_VERSION) || (OS_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif

/*EcuM Private headers*/
#include "EcuM_Prv.h"
/****************************************************************************************************/
/* Type definitions
 ***************************************************************************************************
 */
/****************************************************************************************************/
/* Variables                                                                                        */
/****************************************************************************************************/

#if (ECUM_CFG_MULTICORE_ENABLED == STD_ON)

#define ECUM_START_SEC_VAR_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"

 static boolean EcuM_Prv_ShutDownflgCoresStopped;
 static boolean EcuM_Prv_ShutDownflgCoresTimeout;

#define ECUM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"

#endif //ECUM_CFG_MULTICORE_ENABLED


#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

static void EcuM_Prv_ProceedShutdown( void );
static void EcuM_Prv_WakeupValidation(void);
static Std_ReturnType EcuM_Prv_ShutdownSynchronization( void );

#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
static void EcuM_Prv_ProceedSleep( uint16 dataSleepModeHandle_u16 );
#endif//ECUM_SLEEP_SUPPORT_ENABLE

#if(ECUM_CFG_MODE_HANDLING == STD_ON)

static void EcuM_Prv_RunHandling( void );
static void EcuM_Prv_PostRunArbitration( void );
static void EcuM_Prv_RunArbitration( void );

#endif//ECUM_CFG_MODE_HANDLING

#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)

static void EcuM_Prv_ReportCheckWakeupError ( void );

#endif//ECUM_SLEEP_SUPPORT_ENABLE

/***************************************************************************************************
 Function name    : void EcuM_MainFunction(void)
 Syntax           : void void EcuM_MainFunction(void)
 Description      : EcuM sheduled functions which is called by the BSW sheduler in a cyclic interval defined by the
                    configuration parameter EcuM_MainFunctionPeriod.Carries out all the functionality of the
                    EcuM when the OS is running.
 Parameter        : None
 Return value     : None
 ***************************************************************************************************/

void EcuM_MainFunction(void)
{

    if (FALSE ==  EcuM_Prv_flgIsModuleInitialised_b)
    {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        /*DET raised for service called before EcuM Initialised */
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_MAIN_FUNC_APIID, ECUM_E_UNINIT);
#endif//ECUM_DEV_ERROR_DETECT
    }
    else if (E_NOT_OK ==  EcuM_Prv_ShutdownSynchronization())
    {
#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
        if(EcuM_Prv_WakeupRestartReason_en != ECUM_PRV_CHECK_WAKEUP_PENDING_E)
        {
#endif//ECUM_SLEEP_SUPPORT_ENABLE
        /*update the phase variable to ECUM_PRV_PHASE_UP - EcuM is now in Up phase*/
        EcuM_Prv_dataPresentPhase_u8 = ECUM_PRV_PHASE_UP;

        /*RUN and POST_RUN requests and releases has to be arbitrated in EcuM MainFunction when ECUM_CFG_MODE_HANDLING
         * enabled in the system(Run Handling Protocol).*/

#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)
 /*Get the active core id from Os*/
 if(GetCoreID() == ECUM_CFG_STARTUP_CORE)
 {
#endif//ECUM_CFG_MULTICORE_ENABLED

#if( ECUM_CFG_MODE_HANDLING == STD_ON)
            EcuM_Prv_RunHandling();
#endif//ECUM_CFG_MODE_HANDLING

        /*If shutdown flag is not set, proceed to the wakeup source validations*/
        EcuM_Prv_WakeupValidation();
#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)
            /*If shutdown flag is not set, proceed to the wakeup source validations*/
        }
#endif//ECUM_CFG_MULTICORE_ENABLED
#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
        }
#endif//ECUM_SLEEP_SUPPORT_ENABLE

    }
    else
    {
        //nothing to be executed here
    }
    return;
}

#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
/***********************************************************************************
  Function name :   EcuM_Prv_CheckWakeupTimerHandling
  Description   :   This service will decrement the timer for the wakeups configured with timer value and will
                    update the flag to indicate the status of the wakeup source check
  Parameter in  :   none
  Parameter out :   None
  Return value  :   none
  Remarks       :
 ************************************************************************************/
boolean EcuM_Prv_CheckWakeupTimerHandling ( void )
         {
     uint8 ctrLoop1_u8 = 0;
     EcuM_Prv_SleepStatus_ten flgExitSleep_u8 = ENTER_SLEEP_E;
     EcuM_Cfg_CheckWakeupSrc_tst dataCheckWakeupTimer_ast[ECUM_NUM_OF_WKP_SOURCES];
     boolean flgReturnValue_b = FALSE;
     /*Local variable to hold sleep mode and intiliazed with invalid value*/
     EcuM_ShutdownModeType dataSleepModeHandle_u16 = 0xFF;
     SchM_Enter_EcuM();           /*Enter Critical Section*/
     /*sleep mode for which EcuM_GoPoll/EcuM_GoHalt was called*/
     dataSleepModeHandle_u16 = EcuM_Prv_SleepMode_u16;
	 /* Added to remove warning "variable 'dataSleepModeHandle_u16' was set but never used" when Sleep is enabled */
	 (void)dataSleepModeHandle_u16;

     /*get the status and the timer value for all wakeups*/
     for( ctrLoop1_u8 = 0;ctrLoop1_u8 < ECUM_NUM_OF_WKP_SOURCES;ctrLoop1_u8++ )
     {
     dataCheckWakeupTimer_ast[ctrLoop1_u8] = EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8];
     }
     SchM_Exit_EcuM();        /*Leave Critical Section*/

     for( ctrLoop1_u8 = 0;ctrLoop1_u8 < ECUM_NUM_OF_WKP_SOURCES;ctrLoop1_u8++ )
     {
         /* If EcuM_SetWakeupEvent was called for any wakeup then exit sleep and complete wakeup*/
         if(ECUM_SET_WAKEUP_EVENT_E == dataCheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus)
         {
           flgExitSleep_u8 = EXIT_SLEEP_E;//exit sleep
           flgReturnValue_b = FALSE;
           SchM_Enter_EcuM();           /*Enter Critical Section*/
           /*allow the updation of shutdown targets*/
           EcuM_Prv_flgShutdownInfoDoNotUpdate_b = FALSE;
           /*update the wakeuprestart reason to normal for next GoPoll/GoHalt*/
           EcuM_Prv_WakeupRestartReason_en = ECUM_PRV_NORMAL_E;
           EcuM_Prv_SleepReqType_en = ECUM_PRV_NO_SLEEP_E;
           /*update phase variable to indicate that sleep phase has been exited*/
           EcuM_Prv_dataPresentPhase_u8 = ECUM_PRV_PHASE_UP;
           SchM_Exit_EcuM();            /*Exit Critical Section*/

             break;
         }
         /*Check if any timer needs to be decremented for any wakeup*/
         else  if((ECUM_TIMER_STARTED_E == dataCheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus)\
                 && (dataCheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupTimeout > 0))
         {
             SchM_Enter_EcuM();           /*Enter Critical Section*/
             /*Decrement the timer */
             EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupTimeout=EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupTimeout - 1;
             /*If timeout occurs, update the status for the corresponding wakeup to timer expired*/
             if(EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupTimeout == 0)
             {
                 EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus = ECUM_TIMER_EXPIRED_E;
             }
             else
             {
                 /*signal that EcuM needs to continue waiting for check of wakeup sources to finish*/
                 flgExitSleep_u8 = CONTINUE_WAIT;
                 flgReturnValue_b = TRUE;
             }
             SchM_Exit_EcuM();        /*Leave Critical Section*/
           }

         else
         {
             /*nothing to do*/
         }

         }

     if (flgExitSleep_u8 == ENTER_SLEEP_E)
         {
            EcuM_Prv_ReportCheckWakeupError();
            /*update the wakeuprestart reason to normal for next execution of GoSleep sequence*/
            EcuM_Prv_WakeupRestartReason_en = ECUM_PRV_NORMAL_E;
            /*  since EcuM_SetWakeupEvent was not reported for any wakeupevent and CheckWakeupTimer for all of the async
             *  wakeups that had been detected has either expired or been reset by the call of EcuM_EndCheckWakeup,
             *  the system has to re-enter sleep. The EcuM_Prv_dataPresentPhase_u8 is still sleep,
             *  EcuM_Prv_flgShutdownInfoDoNotUpdate_b is FALSE, EcuM_Prv_WakeupRestartReason_en is ECUM_PRV_NORMAL_E
             *  the EcuM_Prv_SleepReqType_en is EcuM_GoPoll/EcuM_GoHalt depending on whichever was called and
             *  EcuM_Prv_SleepMode_u16 has the selected mode. So simply returning FALSE to EcuM_Prv_ShutdownSynchronization
             *  will result in a call of EcuM_Prv_ProceedSleep and hence sleep phase can be re-entered.*/
         }
     else if(flgExitSleep_u8 == EXIT_SLEEP_E)
     {
         SchM_Enter_EcuM();           /*Enter Critical Section*/
         /*get the status and the timer value for all wakeups*/
         for( ctrLoop1_u8 = 0;ctrLoop1_u8 < ECUM_NUM_OF_WKP_SOURCES;ctrLoop1_u8++ )
         {
             /*reset the CheckWakeup flag and CheckWakeup Timer for the next sleep phase*/
         EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus = ECUM_FLAG_RESET_E;
         EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupTimeout = 0;
         }
         SchM_Exit_EcuM();        /*Leave Critical Section*/
     }
     else
     {
     /*nothing to do*/
     }
    return flgReturnValue_b;
}


/***********************************************************************************
  Function name :   EcuM_Prv_ReportCheckWakeupError
  Description   :   This service will report DET error for either of two cases:
                    1) if check for all asynchronous wakeups returned a negative result, i.e EcuM_EndCheckWakeup was
                    reported.
                    2) if CheckWakeupTimer expired for all asynchronous wakeups and hence check was unsuccessful
                    3) if either CheckWakeupTimer expired or EcuM_EndCheckWakeup was reported for all asynchronous wakeups and hence check was unsuccessful
  Parameter in  :   None
  Parameter out :   None
  Return value  :   None
  Remarks       :
 ************************************************************************************/
static void EcuM_Prv_ReportCheckWakeupError ( void )
        {
    /*Local variable to hold sleep mode and intiliazed with invalid value*/
    EcuM_Prv_SleepType_Enum_ten dataSleepType_en = ECUM_PRV_NO_SLEEP_E;
    uint8 dataApiId_u8 = 0x00;
    uint8 ctrLoop1_u8 = 0x00;
    uint8 cntrEnd_u8 = 0x00;
    uint8 cntrExp_u8 = 0x00;
    uint8 cntrReset_u8 = 0x00;
    EcuM_Cfg_CheckWakeupSrc_tst dataCheckWakeupTimer_ast[ECUM_NUM_OF_WKP_SOURCES];

    SchM_Enter_EcuM();           /*Enter Critical Section*/
    /*sleep mode for which EcuM_GoPoll/EcuM_GoHalt was called*/
    dataSleepType_en = EcuM_Prv_SleepReqType_en;
    for( ctrLoop1_u8 = 0;ctrLoop1_u8 < ECUM_NUM_OF_WKP_SOURCES;ctrLoop1_u8++ )
    {
    dataCheckWakeupTimer_ast[ctrLoop1_u8] = EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8];
    }
    SchM_Exit_EcuM();        /*Leave Critical Section*/
        if (dataSleepType_en != ECUM_PRV_NO_SLEEP_E)
        {
            if(dataSleepType_en == ECUM_PRV_GOHALT_E)

            {
                dataApiId_u8 = ECUM_GOHALT_APIID;
            }

            else
            {
                dataApiId_u8 = ECUM_GOPOLL_APIID;
            }

        for( ctrLoop1_u8 = 0;ctrLoop1_u8 < ECUM_NUM_OF_WKP_SOURCES;ctrLoop1_u8++ )
        {
            if(dataCheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus == ECUM_END_CHECK_WAKEUP_E)
            {
                /*get the number of wakeups for which check of the wakeup returned a negative result*/
                cntrEnd_u8++;
                /*reset the CheckWakeup flag and CheckWakeup Timer for the next sleep phase*/
                SchM_Enter_EcuM();           /*Enter Critical Section*/


            EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus = ECUM_FLAG_RESET_E;
            EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupTimeout = 0x00;

                SchM_Exit_EcuM();
            }
            else if(dataCheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus == ECUM_TIMER_EXPIRED_E)
            {
                /*get the number of wakeups for which check failed due to timeout*/
                cntrExp_u8++;
                /*reset the CheckWakeup flag and CheckWakeup Timer for the next sleep phase*/
                SchM_Enter_EcuM();           /*Enter Critical Section*/


            EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupStatus = ECUM_FLAG_RESET_E;
            EcuM_Cfg_CheckWakeupTimer_ast[ctrLoop1_u8].CheckWakeupTimeout = 0x00;

                SchM_Exit_EcuM();
            }

            else
            {
                /*get the number of wakeups for which timer was not started*/
                cntrReset_u8++;
            }
        }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
            if( (cntrEnd_u8 != 0x00) && (ECUM_NUM_OF_WKP_SOURCES==(cntrEnd_u8 + cntrReset_u8)) )

            {
                /*report error if all wake up sources call EcuM_EndCheckWakeup()*/
                (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, dataApiId_u8, ECUM_E_WAKEUP_NO_SOURCE_FOUND);
            }
            else if( (cntrExp_u8 != 0x00) && (ECUM_NUM_OF_WKP_SOURCES==(cntrExp_u8 + cntrReset_u8)) )
            {
                /*report error if for all wake up sources CheckWakeupTimer expired and hence check was unsuccessful*/
                (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, dataApiId_u8, ECUM_E_WAKEUP_TIMEOUT);
            }
            else if(ECUM_NUM_OF_WKP_SOURCES != cntrReset_u8)
            {
                /*report error if  some wakeups report EcuM_EndCheckWakeup and CheckWakeupTimer expires for all other wakeups*/
                (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, dataApiId_u8, ECUM_E_WAKEUP_NO_SOURCE_FOUND);
            }
            else
            {
             /*nothing to do*/
            }
#endif//ECUM_DEV_ERROR_DETECT
        }
            else
            {
                /*Note: this function is called only if EcuM_SetWakeupEvent was not reported for any wakeupevent and
                 * CheckWakeupTimer for all of the async wakeups that had been detected has either expired or been reset
                 * by the call of EcuM_EndCheckWakeup. In this case EcuM_Prv_SleepReqType_en will never be ECUM_PRV_NO_SLEEP_E*/
            }

    return;
        }


/**********************************************************************************************************************
  Function name :   EcuM_Prv_ProceedSleep
  Description   :   In Multicore Environment, Core synchronization is done and Sleep request is proceed to Go sleep
                    sequence.
  Parameter in  :   dataSleepModeHandle_u16 - the index of the selected sleep mode in EcuM_Cfg_idxSleepModes_au32 array
  Parameter out :   None
  Return value  :   None
  Remarks       :
 *********************************************************************************************************************/

static void EcuM_Prv_ProceedSleep( uint16 dataSleepModeHandle_u16 )
                {
    /*Local variable declaration*/
#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)
    uint16 cntrLoopOsCore_u16 = 0;
    /*Get the active core id from Os*/
    EcuM_Prv_CoreStatus_st.CurrentCoreID = GetCoreID();
    if(FALSE == EcuM_Prv_flgSleepReady_ab[EcuM_Prv_CoreStatus_st.CurrentCoreID])
    {
#endif//ECUM_CFG_MULTICORE_ENABLED
        /*GoSleep sequence*/

        /*Clear the wakeup sources */
        SchM_Enter_EcuM();   /*Enter Critical Section*/

        /*Clear the enabled wakeup source from internal pending variable*/
        EcuM_Prv_dataPendingWakeupEvents_u32  ^= (EcuM_Prv_dataPendingWakeupEvents_u32
                & EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask);

        /*Clear the enabled wakeup source from internal validated variable*/
        EcuM_Prv_dataValidatedWakeupEvents_u32  ^= (EcuM_Prv_dataValidatedWakeupEvents_u32
                & EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask);

        /*Clear the enabled wakeup source from internal expired variable*/
        EcuM_Prv_dataExpiredWakeupEvents_u32  ^= (EcuM_Prv_dataExpiredWakeupEvents_u32
                & EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask);

        SchM_Exit_EcuM();      /*leave Critical Section*/

        /*Inidicate the wakeup status (NONE) to BswM*/
        BswM_EcuM_CurrentWakeup(EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask,
                                                                                                   ECUM_WKSTATUS_NONE);


        /*Enable the wakeup sources configured in Wakeupsourcemask of the selected sleep mode*/
        EcuM_EnableWakeupSources (EcuM_Cfg_idxSleepModes_au32[dataSleepModeHandle_u16].WakeupSourceMask);


        #if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
    }

    if( EcuM_Prv_CoreStatus_st.CurrentCoreID == ECUM_CFG_STARTUP_CORE)
    {
        EcuM_Prv_CoreStatus_st.CurrentCore = ECUM_MASTER_CORE;

        EcuM_Prv_flgSleepReady_ab[ECUM_CFG_STARTUP_CORE] = TRUE;

        /*Loop to check core ready flag for all cores. If one core is not ready don't proceed with sleep */
        for(cntrLoopOsCore_u16 = 0; cntrLoopOsCore_u16 < ECUM_CFG_NUM_OS_CORES ; cntrLoopOsCore_u16++)
        {
            if ( FALSE == EcuM_Prv_flgSleepReady_ab[cntrLoopOsCore_u16])
            {
                /*Set the flag to FALSE if any of the cores are not ready and stopped*/
                EcuM_Prv_AllCoreSleepReady_b = FALSE;
                break;
            }
            else
            {
                /*Set the flag to TRUE if all the cores are ready and stopped */
                EcuM_Prv_AllCoreSleepReady_b = TRUE;
            }
        }
    }
    else
    {
        EcuM_Prv_CoreStatus_st.CurrentCore= ECUM_SLAVE_CORE;
    }


#else//ECUM_CFG_MULTICORE_ENABLED

    EcuM_Prv_CoreStatus_st.CurrentCore= ECUM_SINGLE_CORE;
    EcuM_Prv_CoreStatus_st.CurrentCoreID=0xFF;//initialise with invalid value
#endif//ECUM_CFG_MULTICORE_ENABLED
#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
if(EcuM_Prv_flgSleepReady_ab[ECUM_CFG_STARTUP_CORE] == TRUE)
{
#endif//ECUM_CFG_MULTICORE_ENABLED
    EcuM_Prv_GoSleepSequence(dataSleepModeHandle_u16);
#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
}
#endif//ECUM_CFG_MULTICORE_ENABLED
                }

#endif//ECUM_SLEEP_SUPPORT_ENABLE
/***********************************************************************************************
  Function name :   EcuM_Prv_Shutdown
  Description   :   Contains the shutdown functionalities for Multicore Shutdown Synchronization
  Parameter in  :   None
  Parameter out :   None
  Return value  :   None
  Remarks       :
 *************************************************************************************************/
static void EcuM_Prv_ProceedShutdown(void)
{

#if(ECUM_CFG_MULTICORE_ENABLED  ==  STD_ON)

    /*Local variable declaration*/
       uint16 cntrLoopOsCore_u16;
       CoreIdType EcuM_activeCoreid;

    EcuM_Prv_ShutDownflgCoresStopped = FALSE;
    EcuM_Prv_ShutDownflgCoresTimeout = FALSE;
    /*Get the active core id from Os*/
    EcuM_activeCoreid = GetCoreID();

    if(EcuM_activeCoreid == ECUM_CFG_STARTUP_CORE)
    {

        /*Increment the core ready timeout for slave cores*/
        EcuM_Prv_SlaveCoreTimeoutCtr_u8++;

        /* Set MasterCore also to finished in case ECUM_CFG_STARTUP_CORE is unequal to zero */
        EcuM_Prv_flgCoreReady_ab[ECUM_CFG_STARTUP_CORE] = TRUE;


         /*Loop to check core ready flag for all cores. If one core is not ready don't proceed with shutdown */
        for(cntrLoopOsCore_u16 = 0; cntrLoopOsCore_u16 < ECUM_CFG_NUM_OS_CORES ; cntrLoopOsCore_u16++)
        {
            if (EcuM_Prv_flgCoreReady_ab[cntrLoopOsCore_u16] == FALSE )
            {
                /*Set the flag to FALSE if any of the cores are not ready and stopp */
                EcuM_Prv_ShutDownflgCoresStopped = FALSE;
                break;
            }
            else
            {
                EcuM_Prv_ShutDownflgCoresStopped = TRUE;
            }
        }

        /*Check for timeout of Slave core readiness*/
        if(EcuM_Prv_SlaveCoreTimeoutCtr_u8 > ECUM_CFG_SLAVE_CORE_TIMEOUT)
        {
            EcuM_Prv_ShutDownflgCoresTimeout = TRUE;

#if(ECUM_DEV_ERROR_DETECT == STD_ON)

            /*Call to Det_ReportError for CoreReady timeout error*/
            (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_MAIN_FUNC_APIID, ECUM_E_COREREADY_TIMEOUT_EXPIRED);

#endif /* ECUM_DEV_ERROR_DETECT */

        }

        /*Call ShutdownAllCores if all cores have stopped or a timeout has occured  */
        if ((EcuM_Prv_ShutDownflgCoresStopped == TRUE) || (EcuM_Prv_ShutDownflgCoresTimeout == TRUE))
        {
            EcuM_OnGoOffOne();

            /* De-Initialization of BSW Mode Manager*/
            BswM_Deinit();

            /* De-Initialization of Schedule Manager*/
            SchM_Deinit();

            /*Unsetting the Shutdown Flag is specified by AUTOSAR*/
            EcuM_Prv_flgGoDown_b =   FALSE;

            /*Call ShutdownAllCores*/

			/*If CCAS Initialization switch is configured,then Cores Deinitialization is disabled*/
			#if( ECUM_CFG_CCAS_INIT == STD_OFF )
				ShutdownAllCores(E_OS_SYS_NO_RESTART);
			#endif/*ECUM_CFG_CCAS_INIT*/
        }
    }
        else
        {

        /* Check if EcuM_GoDown is called and CoreReady Flag is not set for the Specific core */
        if((FALSE == EcuM_Prv_flgCoreReady_ab[EcuM_activeCoreid]) && (EcuM_Prv_flgGoDown_b  == TRUE))
        {

            EcuM_OnGoOffOne();

            /* De-Initialization of Schedule Manager*/
            SchM_Deinit();

            /*Set the core ready flag*/
            EcuM_Prv_flgCoreReady_ab[EcuM_activeCoreid] = TRUE;

            return;
        }

    }
#else//ECUM_CFG_MULTICORE_ENABLED

             EcuM_OnGoOffOne();

              /* De-Initialization of BSW Mode Manager*/
              BswM_Deinit();

              /* De-Initialization of Schedule Manager*/
              SchM_Deinit();

			/*If CCAS Initialization switch is configured,then OS Deinitialization is disabled*/
			#if( ECUM_CFG_CCAS_INIT == STD_OFF )
						/*Call of ShutdownOS function for Single core system*/
						ShutdownOS(E_OS_SYS_NO_RESTART);
			#endif/*ECUM_CFG_CCAS_INIT*/

#endif//ECUM_CFG_MULTICORE_ENABLED

}


/***********************************************************************************************
  Function name :   EcuM_Prv_WakeupValidation
  Description   :   Contains the Wakeup validation functionality for the wakeup sources
  Parameter in  :   None
  Parameter out :   None
  Return value  :   None
  Remarks       :
 *************************************************************************************************/
static void EcuM_Prv_WakeupValidation(void)
        {
    /*Local variable declaration*/
    uint8 cntrLoopCtr_u8;
    EcuM_WakeupSourceType dataPendingWakeupEvents_u32;
    uint16 datawkpSrcValidationTimeoutCtr_u16;

        SchM_Enter_EcuM();  /*Enter Critical Section*/
        /*Get the latest PENDING events list*/
        dataPendingWakeupEvents_u32 = EcuM_Prv_dataPendingWakeupEvents_u32;
        /*Get the Validation timeout value with the current main cycle*/
        datawkpSrcValidationTimeoutCtr_u16 = EcuM_Prv_WkpSrcValidationTimeoutCtr_u16;
        SchM_Exit_EcuM(); /*leave Critical Sectiont*/

        EcuM_Prv_WakeupIndication(dataPendingWakeupEvents_u32);

        if (EcuM_Prv_dataOldPendingWakeupEvents_u32 != ECUM_NO_WKUP_SRC )
        {
            /*Iterate the configured wakeup sources*/
            for (cntrLoopCtr_u8 = 0;cntrLoopCtr_u8 < ECUM_NUM_OF_WKP_SOURCES ; cntrLoopCtr_u8++)
            {
                if ((EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].WakeupSourceId & EcuM_Prv_dataOldPendingWakeupEvents_u32) != ECUM_NO_WKUP_SRC)
                {
                    /* if configured wakeup source state is EcuM wakeupsource pending (or not validated) and the validation timeout is not expired */
                    /*call EcuM_CheckValidation with the wakeup source as the parameter */
                    EcuM_CheckValidation (EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].WakeupSourceId);
                }

                /*if any wakeup is validated in between the corresponding pending wakeup event will get cleared.
                 * if so update the Pending wakeup event variable corresponding to this main cycle with the latest pending wakeup event */
                if ((EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].WakeupSourceId &
                        EcuM_Prv_dataValidatedWakeupEvents_u32) != ECUM_NO_WKUP_SRC)
                {
                    SchM_Enter_EcuM();  /*Enter Critical Section*/
                    EcuM_Prv_dataOldPendingWakeupEvents_u32 = EcuM_Prv_dataPendingWakeupEvents_u32;
                     SchM_Exit_EcuM(); /*leave Critical Sectiont*/
                }
            }
        }


    /* Check whether validation timeout counter is expired AND is there still any Pending Wakeup events
     * that is evaluated in the current main cycle, then make the wakeupsource state as Expired */
    if ((datawkpSrcValidationTimeoutCtr_u16 == 0) &&
            ( (EcuM_Prv_dataOldPendingWakeupEvents_u32 & dataPendingWakeupEvents_u32) != ECUM_NO_WKUP_SRC))
    {
        /*Update the Expired wakeup event variable */
        SchM_Enter_EcuM();  /*Enter Critical Section*/
        EcuM_Prv_dataExpiredWakeupEvents_u32 ^= EcuM_Prv_dataOldPendingWakeupEvents_u32 ;
        EcuM_Prv_dataPendingWakeupEvents_u32 ^= EcuM_Prv_dataOldPendingWakeupEvents_u32;
        /*Update the EcuM_Prv_dataOldPendingWakeupEvents_u32 to Zero so that with the next cycle new sources
         * will be evaluated for Validation*/
        EcuM_Prv_dataOldPendingWakeupEvents_u32 = ECUM_NO_WKUP_SRC;
        SchM_Exit_EcuM(); /*leave Critical Sectiont*/

        /* Give wakeup state indication to BswM*/
        BswM_EcuM_CurrentWakeup ( EcuM_Prv_dataExpiredWakeupEvents_u32 , ECUM_WKSTATUS_EXPIRED);

        /*Stop the validation of all sources that got expired with the current main cycle*/
        EcuM_StopWakeupSources (EcuM_Prv_dataExpiredWakeupEvents_u32 );
    }

    EcuM_Prv_DecValidationCtr();

    return;

}
#if( ECUM_CFG_MODE_HANDLING == STD_ON)
/***********************************************************************************************
  Function name :   EcuM_Prv_RunHandling
  Description   :   Contains the activities of EcuM Run Handling Protocol
  Parameter in  :   None
  Parameter out :   None
  Return value  :   None
  Remarks       :
 *************************************************************************************************/
static void EcuM_Prv_RunHandling(void)
{

    /*To ensure  the warm-up time to prepare the RTE and application software users as well as  the proper termination after
     * a idle time The Minimum RUN duration feature has been provided. It is a configurable value.*/
    if( EcuM_Prv_RunMinDuration_u16 > 0 )
    {
        EcuM_Prv_RunMinDuration_u16 = EcuM_Prv_RunMinDuration_u16 - 1;
    }
     else
    {

    }
    /* Fixed Mode Handling Arbitration for All the EcuM-Flex User Run Request will be handled from EcuM_Prv_RunArbitration and RUN Status
     * Requested/Released will be notified to BswM through BswM_EcuM_RequestedState Api.*/
    EcuM_Prv_RunArbitration( );

    /* Fixed Mode Handling Arbitration for All the EcuM-Flex User Post_Run Request will be handled from EcuM_Prv_RunArbitration and POSTRUN Status
      * Requested/Released will be notified to BswM through BswM_EcuM_RequestedState Api.*/
    EcuM_Prv_PostRunArbitration( );

    return;
}

/***********************************************************************************************
  Function name :   EcuM_Prv_RunArbitration
  Description   :   Contains the activities Arbitrating and notifying the User Run Request status
                    of EcuM Run Handling Protocol
  Parameter in  :   None
  Parameter out :   None
  Return value  :   None
  Remarks       :
 *************************************************************************************************/
static void EcuM_Prv_RunArbitration( void )
{
    /*iterator variable for the users*/
    uint8 cntrLoopCtr_u8;
    /*Local value used to ensure either RUN Request is in progress or not*/
    boolean UserRUNReqPending_b = FALSE;

    /*Loop for handling the RUN requests from the Users*/
        for( cntrLoopCtr_u8 = 0; cntrLoopCtr_u8 <ECUM_CFG_NUM_FLEX_USERS;cntrLoopCtr_u8++ )
        {
            /*Check for the Presence of a particular user request for RUN state*/
            if( EcuM_Prv_userRUNReqStatus_ab[ cntrLoopCtr_u8 ] == TRUE )
            {
                /*If it is the first user request then notify the BswM about the status ECUM_RUNSTATUS_REQUESTED*/
                if( ( EcuM_Prv_RunRequested_u8 == ECUM_RUNSTATUS_RELEASED ) || ( EcuM_Prv_RunRequested_u8 == ECUM_RUNSTATUS_UNKNOWN ) )
                {
                    BswM_EcuM_RequestedState( ECUM_STATE_APP_RUN,ECUM_RUNSTATUS_REQUESTED );
                    EcuM_Prv_RunRequested_u8 = ECUM_RUNSTATUS_REQUESTED;

                }
                /*If atleast one user request is pending update the given local variable else donot update.
                 * based on that status ECUM_RUNSTATUS_RELEASED notification to BswM will be decided */
                else
                {

                }
                UserRUNReqPending_b = TRUE;
                break;
            }
            else
            {

            }
        }
        /*BswM Notification Status Released to be triggered, when all the user requests are released. Run minimum duration check
            is to ensure the idle time as well as warm-up time to prepare the RTE and application software users.*/
        if( ( EcuM_Prv_RunMinDuration_u16 == 0 ) && ( UserRUNReqPending_b == FALSE )
                && ( ( EcuM_Prv_RunRequested_u8 == ECUM_RUNSTATUS_REQUESTED ) ||  ( EcuM_Prv_RunRequested_u8 == ECUM_RUNSTATUS_UNKNOWN ) ) )
        {
            BswM_EcuM_RequestedState( ECUM_STATE_APP_RUN,ECUM_RUNSTATUS_RELEASED );
            EcuM_Prv_RunRequested_u8 = ECUM_RUNSTATUS_RELEASED;
        }
        else
        {

        }

        return;
}
/***********************************************************************************************
  Function name :   EcuM_Prv_PostRunArbitration
  Description   :   Contains the activities Arbitrating and notifying the User PostRun Request status
                    of EcuM Run Handling Protocol
  Parameter in  :   None
  Parameter out :   None
  Return value  :   None
  Remarks       :
 *************************************************************************************************/
static void EcuM_Prv_PostRunArbitration( void )
{
    /*iterator variable for the users*/
    uint8 cntrLoopCtr_u8;
    /*Local value used to ensure either POSTRUN Request is in progress or not*/
    boolean UserPostRUNReqPending_b = FALSE;

    /*Loop for handling the POSTRUN requests from the Users*/
        for( cntrLoopCtr_u8 = 0; cntrLoopCtr_u8 < ECUM_CFG_NUM_FLEX_USERS;cntrLoopCtr_u8++ )
        {
            /*Check for the Presence of a particular user request for POSTRUN state*/
            if( EcuM_Prv_userPostRUNReqStatus_ab[ cntrLoopCtr_u8 ] == TRUE )
            {
                /*If it is the first user request then notify the BswM about the status ECUM_RUNSTATUS_REQUESTED for POSTRUN*/
                if( ( EcuM_Prv_Post_Run_Requested_u8 == ECUM_RUNSTATUS_RELEASED ) || ( EcuM_Prv_Post_Run_Requested_u8 == ECUM_RUNSTATUS_UNKNOWN ) )
                {
                    BswM_EcuM_RequestedState( ECUM_STATE_APP_POST_RUN,ECUM_RUNSTATUS_REQUESTED  );
                    EcuM_Prv_Post_Run_Requested_u8 = ECUM_RUNSTATUS_REQUESTED;
                }
                /*If atleast one user request is pending update the given local variable else donot update.
                 * based on that status ECUM_RUNSTATUS_RELEASED notification to BswM will be decided */
                else
                {

                }
                UserPostRUNReqPending_b = TRUE;
                break;
            }
            else
            {

            }
        }
        /*BswM Notification Status Released to be triggered, when all the user requests are released. Run minimum duration check
        is to ensure the idle time as well as warm-up time to prepare the RTE and application software users.*/
        if( ( EcuM_Prv_RunMinDuration_u16 == 0 ) && ( UserPostRUNReqPending_b == FALSE )
                && ( ( EcuM_Prv_Post_Run_Requested_u8 == ECUM_RUNSTATUS_REQUESTED ) || ( EcuM_Prv_Post_Run_Requested_u8 == ECUM_RUNSTATUS_UNKNOWN ) ) )
        {
            BswM_EcuM_RequestedState( ECUM_STATE_APP_POST_RUN,ECUM_RUNSTATUS_RELEASED );
            EcuM_Prv_Post_Run_Requested_u8 = ECUM_RUNSTATUS_RELEASED;
        }
        else
        {

        }

        return;
}
#endif//ECUM_CFG_MODE_HANDLING

/***********************************************************************************************
  Function name :   EcuM_Prv_ShutdownSynchronization
  Description   :   Handles the phase transitions for sleep and shutdown phases
  Parameter in  :   None
  Parameter out :   None
  Return value  :   Std_ReturnType
  Remarks       :
 *************************************************************************************************/
static Std_ReturnType EcuM_Prv_ShutdownSynchronization( void )
 {

    Std_ReturnType return_value = E_NOT_OK;
#if (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)


    EcuM_Prv_SleepType_Enum_ten id_RequestedSleepType_en;
    boolean flgWakeRestartStatus_b = FALSE;
    /*Local variable to hold sleep mode and intiliazed with invalid value*/
    EcuM_ShutdownModeType dataSleepModeHandle_u16 = 0xFF;
    boolean flgContinueWakeupCheck_b = FALSE;
    EcuM_Prv_WakeupRestartReason_Enum_ten stWakeupRestartReason_en = ECUM_PRV_NORMAL_E;

    SchM_Enter_EcuM();           /*Enter Critical Section*/
    stWakeupRestartReason_en = EcuM_Prv_WakeupRestartReason_en;
    SchM_Exit_EcuM();           /*Enter Critical Section*/

    /*if wakeup occurred because an asynchronous wakeup was detected and its check is pending, then call the timer variuable handling function*/

    if(ECUM_PRV_CHECK_WAKEUP_PENDING_E == stWakeupRestartReason_en)
    {
        flgContinueWakeupCheck_b = EcuM_Prv_CheckWakeupTimerHandling();
    }

    /*if wakeup check is complete*/
    if(flgContinueWakeupCheck_b == FALSE)
    {
    SchM_Enter_EcuM();           /*Enter Critical Section*/

    /*sleep mode for which EcuM_GoPoll/EcuM_GoHalt was called*/
    dataSleepModeHandle_u16 = EcuM_Prv_SleepMode_u16;
    /*Getting the Sleep type in critical region*/
    id_RequestedSleepType_en = EcuM_Prv_SleepReqType_en;

    SchM_Exit_EcuM();            /*Exit Critical Section*/

    if( (ECUM_PRV_GOHALT_E == id_RequestedSleepType_en) || ( ECUM_PRV_GOPOLL_E == id_RequestedSleepType_en ))
    {
        EcuM_Prv_ProceedSleep(dataSleepModeHandle_u16);
    }
    else
    {

        /* do nothing*/

    }

    /*Getting the WakeupRestart flag in critical region*/
    SchM_Enter_EcuM();         /*Enter Critical Section*/

    flgWakeRestartStatus_b  = EcuM_Prv_flgwakeupRestart_b;

    SchM_Exit_EcuM();            /*leave Critical Section*/

    if ( FALSE != flgWakeRestartStatus_b)
    {
        EcuM_Prv_WakeupRestartSequence(dataSleepModeHandle_u16);
    }

    if((((ECUM_PRV_GOPOLL_E != id_RequestedSleepType_en) &&
            (ECUM_PRV_GOHALT_E != id_RequestedSleepType_en)) && (TRUE != flgWakeRestartStatus_b)))
    {
#endif//ECUM_SLEEP_SUPPORT_ENABLE

        /*Check for GoDown flag is set from EcuM_GoDown*/
        if(FALSE != EcuM_Prv_flgGoDown_b)
        {
            /*Call to Shutdown operations if GoDown flag is set*/
            EcuM_Prv_ProceedShutdown();
            return_value = E_OK;
        }
        else
        {

        }
#if  (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
    }
    }
    else
    {
        /*if waiting for timer to expire, return E_OK*/
        return_value = E_OK;
    }
#endif//ECUM_SLEEP_SUPPORT_ENABLE
    return return_value;
}

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
