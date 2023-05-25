

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/

#include "EcuM.h" /*BSW_HeaderInc_002*/

#include "EcuM_Callout.h"

/*todo : This header has to be removed once Rte tool is ready with exclusive area generation*/
#include "EcuM_Cfg_SchM.h"

/*Extern module Headers*/
#include "Rte_Main.h" /*For SchM_Init*/

#include "Os.h" /* specified in SWS. required for StartOS */
// #if (!defined(OS_AR_RELEASE_MAJOR_VERSION) || (OS_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(OS_AR_RELEASE_MINOR_VERSION) || (OS_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif

#include "BswM.h"
// #if (!defined(BSWM_AR_RELEASE_MAJOR_VERSION) || (BSWM_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(BSWM_AR_RELEASE_MINOR_VERSION) || (BSWM_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif

/*Extern module Headers*/
#if (ECUM_CFG_NVM_BLOCK_CONFIGURED == TRUE)
#include "NvM.h"                    /*for setramblockstatus api*/
// #if (!defined(NVM_AR_RELEASE_MAJOR_VERSION) || (NVM_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(NVM_AR_RELEASE_MINOR_VERSION) || (NVM_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif
#endif//ECUM_CFG_NVM_BLOCK_CONFIGURED

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
#include "EcuM_Prv.h"

/**************************************************************************************************/
/* static global variables                                                                          */
/**************************************************************************************************/
#define ECUM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
#include "EcuM_MemMap.h"
#if (ECUM_CFG_NVM_BLOCK_CONFIGURED == TRUE)
/* Structure array block of shutdown info. This is stored in RAM */
EcuM_ShutdownTargetInfoType EcuM_Rb_dataShutdownInfo_st;
#endif//ECUM_CFG_NVM_BLOCK_CONFIGURED
#define ECUM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
#include "EcuM_MemMap.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/***********************************************************************************

 Function name:   void EcuM_Init (void)
 Description:     Initialise EcuM and Carries out STARTUP_ONE
 Parameter in:    None
 Parameter out:   None
 \return    void
 \nRemarks:       Initializes the ECU state manager and carries out the startup one
 procedure ,which is done before the OS is started.At the end of
 this service OS is started and the contol is handed over to OS.
 ************************************************************************************/
void EcuM_Init(void)
{


#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
    /*Get the active core id from Os*/
    if(GetCoreID() == ECUM_CFG_STARTUP_CORE)
    {
#endif//ECUM_CFG_MULTICORE_ENABLED

        /*EcuM STARTUP ONE is carried out when the EcuM_Init is called and here
        EcuM cuurent state is set as ECUM_PRV_PHASE_START_PREOS*/
        EcuM_Prv_dataPresentPhase_u8 = ECUM_PRV_PHASE_START_PREOS;

#if      (ECUM_INTERRUPTS_ENABLE == TRUE)
        /*EcuM_AL_SetProgrammableInterrupts shall set the interrupts on ECUs with programmable interrupts.*/
        EcuM_AL_SetProgrammableInterrupts();
#endif//ECUM_INTERRUPTS_ENABLE

        /*This callout initialize BSW modules that do not use post-build
        configuration parameters. The callout not only contain driver initialization
        but any kind of pre-OS, low level initialization code*/
        EcuM_AL_DriverInitZero();

        /*
         This callout is expected to return a pointer to a fully initialized
         EcuM_ConfigType structure containing the post-build configuration data for
         EcuM and all other BSW modules
         */

        EcuM_Rb_adrConfigPtr_st = EcuM_DeterminePbConfiguration();

        /* EcuM need to check the consistency of the Post Build pointer. For this
           purpose the pointer is checked against NULL Pointer, Invalid Pointer
           and Error hooks are thrown accordingly. Also it is compared with the
           Pre Compile hash for the consistency check*/
        EcuM_Prv_PBPointerConsistencyCheck();

#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
	/*If CCAS Initialization switch is configured,then Core initialization is disabled*/
	#if(ECUM_CFG_CCAS_INIT == STD_OFF)
        /*If slave cores are configured to start earlier, The slave cores are started just after master validates post build pointer */
		#if(ECUM_CFG_SLAVECORE_EARLIER_START == STD_ON)
                /* All slave cores are started. */
                EcuM_Prv_StartSlaveCores();

		#endif//ECUM_CFG_SLAVECORE_EARLIER_START
	#endif/*ECUM_CFG_CCAS_INIT*/

    }
#endif//ECUM_CFG_MULTICORE_ENABLED

    /*The callout not only contain driver initialization but any kind of pre-OS,
        low level initialization code. The Base Address of the EcuM PB init structure obtained
        by the above call to EcuM_DeterminePbConfiguration is passed as the parameter*/
    EcuM_AL_DriverInitOne( EcuM_Rb_adrConfigPtr_st );

#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
    /*Get the active core id from Os*/
    if(GetCoreID() == ECUM_CFG_STARTUP_CORE)
    {
#endif//ECUM_CFG_MULTICORE_ENABLED

        /* The reset reason is obtained from MCU and if it is a defined reset reason
           the same reason is set as WakeupEvent. */
        EcuM_Prv_SetDefinedMcuWakeupSource();
        /*
     Default shutdown target is selected
         */
        EcuM_Prv_dataSelectedShutdownTarget_st.ShutdownTarget = EcuM_Rb_adrConfigPtr_st->DefaultShutdownTarget.ShutdownTarget;

        EcuM_Prv_dataSelectedShutdownTarget_st.mode = EcuM_Rb_adrConfigPtr_st->DefaultShutdownTarget.mode;

        EcuM_Prv_dataSelectedShutdownTarget_st.ShutdownCause = EcuM_Rb_adrConfigPtr_st->DefaultShutdownTarget.ShutdownCause;
#if (ECUM_STARTUP_DURATION == TRUE)
        EcuM_Prv_dataSelectedShutdownTarget_st.TimeMeasured = EcuM_Rb_adrConfigPtr_st->DefaultShutdownTarget.TimeMeasured;
#endif//ECUM_STARTUP_DURATION



        /* As per reqirement EcuM2601: If hardware cannot detect a specific wakeup source, then
            the ECU State Manager shall report a ECUM_WKSOURCE_RESET instead. */
        if ((EcuM_Prv_dataPendingWakeupEvents_u32 | EcuM_Prv_dataValidatedWakeupEvents_u32) == ECUM_NO_WKUP_SRC)
        {
            EcuM_Prv_dataValidatedWakeupEvents_u32 |= EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_RESET;
            EcuM_Prv_dataValWkpEventsInd_u32 |= EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_RESET;
        }


        /* The StartCore function will be called on all slave cores if Multicore is enabled*/
#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)

	/*If CCAS Initialization switch is configured,then Core initialization is disabled*/
	#if( ECUM_CFG_CCAS_INIT == STD_OFF )

		/*If slave cores are NOT configured to start earlier, start the cores as per AutoSar SWS*/
		#if(ECUM_CFG_SLAVECORE_EARLIER_START == STD_OFF)
        /* The slave cores are started. */
        EcuM_Prv_StartSlaveCores();
		#endif//ECUM_CFG_SLAVECORE_EARLIER_START

	#endif/*ECUM_CFG_CCAS_INIT*/
#endif/*ECUM_CFG_MULTICORE_ENABLED*/

        /*  Before starting the OS EcuM is made as Initialised at the end of this API */
        EcuM_Prv_flgIsModuleInitialised_b  = TRUE;

#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
    }
#endif//ECUM_CFG_MULTICORE_ENABLED

    /*This callout will be called from Master and Slave core Init api's */
    EcuM_SwitchOsAppMode();

    /* Finally at the end of startup one OS is started with the default APP mode
       and by this control is handed over to OS from EcuM */

    /*If CCAS Initialization switch is configured,then OS initialization is disabled*/
#if(ECUM_CFG_CCAS_INIT == STD_OFF)
    EcuM_Prv_StartOS();
#endif/*ECUM_CFG_CCAS_INIT*/
}


/***********************************************************************************

  Function name:  void EcuM_StartupTwo(void )
  Description:    Carries ou statup two sequence.
  \nRemarks:      The integrator can configure the OS to activate the
                  EcuM_StartupTwo task by any mechanism, as long as it
                  is started immediately after StartOS is called. The task
                  can also be activated from within another task and this
                  other task could be an autostart task.
 ************************************************************************************/

void EcuM_StartupTwo(void)
        {
    /*Check if module was initialized*/
    if ( EcuM_Prv_flgIsModuleInitialised_b == TRUE )
      {

    /* Check for Multicore system */
#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)

    CoreIdType EcuM_activeCoreid;
    /*Get the active core id from Os*/
    EcuM_activeCoreid = GetCoreID();

    /*EcuM_StartupTwo should NOT be called more than one time in each core*/
    if( FALSE == EcuM_Prv_flgIsStartupTwoCalled_ab[EcuM_activeCoreid] )
    {

        if(EcuM_activeCoreid == ECUM_CFG_STARTUP_CORE)
        {
            /*Updating the EcuM State (In Multi Core scenario: only in master core)*/
            EcuM_Prv_dataPresentPhase_u8 = ECUM_PRV_PHASE_START_POSTOS;
        }
        else
        {
            //nothing to be executed here
        }

        /*Setting the core specific flag that EcuM_StartupTwo is called*/
        EcuM_Prv_flgIsStartupTwoCalled_ab[EcuM_activeCoreid] = TRUE;

        /* BSW-15945 */ /* [$DD_BSWCODE 37921] */
        SchM_Start_Integration();
        /* END BSW-15945 */
        
        if(EcuM_activeCoreid == ECUM_CFG_STARTUP_CORE)
        {

            /*Initialize the BswM Module with the Post-Build Configuration pointer.
             * NOTE : For RTA-BSW BswM module PB is not supported. So a pointer to this will raise Det error. As EcuM
             * is designed basicall for RTA-BSW, the default implementation is with a NULL_PTR to BswM. If the project
             * is with a BswM module then EcuM will generate the valid pointers by using a non-AR boolean parameter
             * in EcuMGeneral.*/

            /*Initialize the BswM Module. (In Multi Core scenario: only in master core)*/
            BswM_Init(EcuM_Rb_adrConfigPtr_st->ModuleInitPtrPB.BswM_InitConfigPtr_cpst);

        }
        else
        {

            //nothing to be executed here
        }
        
        /* BSW-15945 */ /* [$DD_BSWCODE 37921] */
        /*Initialize the Schedule Manager in All cores.*/
        SchM_Init();
        
        SchM_StartTiming_Integration();       
        /* END BSW-15945 */                
    }

#else /*ECUM_CFG_MULTICORE_ENABLED*/

    /*EcuM_StartupTwo should NOT be called more than one time*/
    if( FALSE == EcuM_Prv_flgIsStartupTwoCalled_ab[0] )
    {
        /*Updating the EcuM State (In Multi Core scenario: only in master core)*/
        EcuM_Prv_dataPresentPhase_u8 = ECUM_PRV_PHASE_START_POSTOS;

        /*Setting the flag that EcuM_StartupTwo is called.[In Single core scenario array size is one that is
             the flag variable */
        EcuM_Prv_flgIsStartupTwoCalled_ab[0] = TRUE;

        /*BSW-15945*/ /* [$DD_BSWCODE 37921] */
        SchM_Start_Integration();
        /* END BSW-15945 */        

        /*Initialize the BswM Module with the Post-Build Configuration pointer.
         * NOTE : For RTA-BSW BswM module PB is not supported. So a pointer to this will raise Det error. As EcuM
         * is designed basicall for RTA-BSW, the default implementation is with a NULL_PTR to BswM. If the project
         * is with a BswM module then EcuM will generate the valid pointers by using a non-AR boolean parameter
         * in EcuMGeneral.*/

        BswM_Init(EcuM_Rb_adrConfigPtr_st->ModuleInitPtrPB.BswM_InitConfigPtr_cpst);

        /* BSW-15945 */ /* [$DD_BSWCODE 37921] */
        /*Initialize the Schedule Manager in All cores.*/
        SchM_Init();
        
        SchM_StartTiming_Integration();
        /* END BSW-15945 */            

    }
    else
    {
        //nothing to be executed here
    }

#endif /* ECUM_CFG_MULTICORE_ENABLED */


        }
    else
    {
#if( ECUM_DEV_ERROR_DETECT == STD_ON )
         (void)Det_ReportError( ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_START_TWO_APIID, ECUM_E_UNINIT );
 #endif/*ECUM_DEV_ERROR_DETECT*/

         return;
     }
        }



#if (ECUM_CFG_NVM_BLOCK_CONFIGURED == TRUE)
/***********************************************************************************

  Function name :   Std_ReturnType EcuM_Rb_NvMSingleBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult)
  Description   :   This function is invoked by NvM  on termination of each access to the EcuM configured NvM block.
  Parameter in  :   ServiceId, JobResult
  Parameter out :   None
  Return value  :   Std_ReturnType
  Remarks       :
 ************************************************************************************/

Std_ReturnType EcuM_Rb_NvMSingleBlockCallbackFunction(
        uint8 ServiceId,
        uint8 JobResult)
        {
    Std_ReturnType returnvalue_u8 = E_NOT_OK;

    /*Check to get the status of NvM_ReadALL*/
    if((ServiceId==0x0c) && (JobResult==NVM_REQ_OK))
    {
        /*set flag to TRUE if ReadAll was successful*/
        EcuM_Prv_flgNvMReadStatus_b=TRUE;
        /*copy shutdown info from NvM to prv variable*/
        EcuM_Prv_dataShutdownInfo_st=EcuM_Rb_dataShutdownInfo_st;
        /*update RAM block with current selected shutdown target and mode*/
        EcuM_Rb_dataShutdownInfo_st=EcuM_Prv_dataSelectedShutdownTarget_st;

       /*Setting RAM block status for shutdown info block of NvM */
       /*The return value of NvM_SetRamBlockStatus is not considered with the assumption that failed cases are taken
        * care in NvM. The sevice fails in cases like uninitialized NvM or passed Id  being present already in Queue or
        * passed id being out of range. In all these cases corresponding DET will be raised directly from NvM module.*/
        (void)NvM_SetRamBlockStatus(NvMConf_NvMBlockDescriptor_ECUM_CFG_NVM_BLOCK,TRUE);
         returnvalue_u8   =   E_OK;
    }
    /*Check to get the status of NvM_WriteALL*/
    else if((ServiceId==0x13) && (JobResult==NVM_REQ_OK))
    {
        returnvalue_u8   =   E_OK;
    }
    else
    {
        //nothing to be executed here
    }
    return returnvalue_u8;
        }
#endif//ECUM_CFG_NVM_BLOCK_CONFIGURED

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
