
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_Main_Inf.h"
#include  "DcmDspUds_Seca_Prv.h"
#include "DcmDspUds_Roe_Inf.h"
#include "Rte_Dcm.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/* definition of pointers */
const Dcm_Dsld_connType * Dcm_DsldConnTable_pcst;
const Dcm_Dsld_protocol_tableType * Dcm_DsldProtocol_pcst;
const uint8 * Dcm_DsldRxTable_pcu8;
const uint8 * Dcm_DsldSessionTable_pcu8;
Dcm_MsgContextType Dcm_DsldMsgContext_st;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
Dcm_MsgContextType Dcm_OBDMsgContext_st; /* MsgContext Structure for OBD */
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF )
const Dcm_ConfigType * Dcm_ActiveConfigSet_Ptr;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/* definition of structure containing the global variables */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_DsldInternalStructureType_tst Dcm_DsldGlobal_st;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
Dcm_OBDInternalStructureType_tst Dcm_OBDGlobal_st;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"

/* Global variables to handle timer using OS Timer */
#if (DCM_CFG_OSTIMER_USE != FALSE)
#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
uint32 Dcm_P2OrS3StartTick_u32;    /* Value of ticks when request is received */
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
uint32 Dcm_OBDP2OrS3StartTick_u32;
#endif
uint32 Dcm_CurrTick_u32;    /* Current CPU ticks value */
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
uint32 Dcm_PagedBufferStartTick_u32;    /* Starting tick value for paged buffer timer */
#endif
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
uint32 Dcm_TimerStartTick_u32;    /* Starting tick value for Roe timer */
#endif
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
#if ((DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)||(DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF))
#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
uint32 Dcm_SysPrevTick_u32;  /* Previous tick value */
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
uint32 Dcm_SysCurrTick_u32;  /* Current tick value */
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
#endif

#endif

#if(DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
uint32 Dcm_Dsp_SecaGlobaltimer_u32;
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
#endif


#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF )
uint8 Dcm_ActiveConfiguration_u8;       /*GLobal variable which will hold the current active configuration*/
#endif

uint8 Dcm_CurProtocol_u8;   /* Variable to Indicate the currently active protocol- used for StopProtocol in DslStateMachine */
Dcm_SesCtrlType Dcm_CC_ActiveSession_u8; /* Global Varible to hold CC Acctivesession*/

#if(DCM_CFG_VIN_SUPPORTED != DCM_CFG_OFF)
static uint8 s_VinWaitPendingCounter_u8 ;   /*Indicate pending response is returned while reading VIN DID */
static Std_ReturnType s_VinBufferInitStatus_u8 = E_OK; /*Indicate return status for reading VIN DID  */
#endif
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"



#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_Dsld_activediagnostic_ten Dcm_ActiveDiagnosticState_en;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#if (DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
boolean Dcm_ReadyForBoot_b;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
boolean Dcm_ObdSendTxConfirmation_b;
#endif
boolean Dcm_isInitialised_b;
boolean Dcm_acceptRequests_b;
boolean Dcm_isCancelTransmitInvoked_b; /* Variable used for checking cancel transmit status */
boolean Dcm_isStopProtocolInvoked_b; /* Flag used to check invocation of StopProtocol */
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

void Dcm_Prv_DSL_Mainfunction(void);


/***********************************************************************************************************************
 Function name    : Dcm_Prv_OBDMainFunction
 Description      : Function to manage OBD Portion when parallel processing is enabled
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_OBDMainFunction(void)
{
    /* TxConfirmation was received. Inform the application */
    if(Dcm_ObdSendTxConfirmation_b == TRUE)
    {
       Dcm_Prv_ConfirmationToOBDApl();
       Dcm_ObdSendTxConfirmation_b = FALSE;
    }
    /* Check OBD P2/P2* timer and further process it */
    Dcm_Prv_OBDTimerProcessing();
    // State Machine for OBD request processing
    Dcm_Prv_OBDStateMachine();
}
#endif

/**
 *************************************************************************************************
 * Dcm_Prv_Statemachine_Init :  Internal function to Main Function to call DSL and DSD state
 *                              machines
 * \param           None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
static void Dcm_Prv_Statemachine_Init (void)
{
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_OBDMainFunction();
#endif
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    if(Dcm_Prv_GetDslPreemptionState()   != DSL_SUBSTATE_STOP_PROTOCOL_RECEIVING)
#endif
    {
        Dcm_Prv_DSL_Mainfunction();
        Dcm_Prv_DsdStateMachine();
    }
}

/**
 *************************************************************************************************
 * Dcm_Prv_MainVINinit :  Internal function to Main Function to initialise VIN buffer
 * \param           None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

#if(DCM_CFG_VIN_SUPPORTED != DCM_CFG_OFF)
static void Dcm_Prv_MainVINinit (void)
{
    /* Read VIN DID During Startup */
          /* Check if VIN DID is already read */
          if(Dcm_VinReceived_b != TRUE)
          {
                /*Check if Dcm_VinBuffer_Init return status is E_OK or DCM_E_PENDING */
              if((s_VinBufferInitStatus_u8 == E_OK) || ((s_VinBufferInitStatus_u8 == DCM_E_PENDING) && (s_VinWaitPendingCounter_u8 < DCM_CFG_WAIT_FOR_VIN)))
              {
                  s_VinBufferInitStatus_u8 = Dcm_VinBuffer_Init();
                   /*Increament counter value if server needs more time to read */
                  if(s_VinBufferInitStatus_u8 == DCM_E_PENDING)
                  {
                      s_VinWaitPendingCounter_u8++;
                  }
                  if(Dcm_VinReceived_b == TRUE)
                  {
                      /* Update the VIN wait pend counter with zero when VIN data is available */
                      s_VinWaitPendingCounter_u8 = 0x00;
                  }
              }
          }
}
#endif

/**
 *************************************************************************************************
 * Dcm_Prv_UdsMainFunction :  Internal function to Main Function for checking CDTC mode status,
 *                          call CommunicationControl Main function and Response On Event Main
 *                          function
 * \param           None
 *
 *
 * \retval          None
 * \seealsook
 * \usedresources
 **************************************************************************************************
 */
static void Dcm_Prv_UdsMainFunction(void)
{
#if(DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF)
    /* Call state machine for CC and CDTCS related mode check*/
    Dcm_Prv_Cdtcs_Mainfunction();
#endif

#if ( ( DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF ) && ( DCM_CFG_DSP_COMMUNICATIONCONTROL_ENABLED != DCM_CFG_OFF ) )
    /* Call CC Mainfunction*/
    Dcm_Prv_CC_Mainfunction();
#endif

#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_ROE_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF))
    /* Call Roe Mainfunction*/
    Dcm_Prv_RoeMainFunction();
#endif
}

/**
 *************************************************************************************************
 * Dcm_Prv_DSL_Mainfunction :  Internal function to Main Function to check for Default session request
 *                           from the application, call RDPI main function, and then call the DSL
 *                           statemachine.
 * \param           None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_Prv_DSL_Mainfunction(void)
{
    /* check DSD is in DSD_TX_CONFIRMATION state */
    if(Dcm_Prv_GetDsdState() == DSD_SENDTXCONF_APPL )
    {
        /* call DSD state machine to give confirmation to Application */
        Dcm_Prv_DsdSendTxConfirmation();
    }

    /* Check if the application has requested for switching to the default session */
    Dcm_Prv_ProcessResetToDefaultSession();

#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
    Dcm_RdpiMainFunction();
#endif
    /* call DSL state machine */
    Dcm_Prv_DslStateMachine();
}

/**
 *************************************************************************************************
 * Dcm_Prv_Main_Warmstart :  Internal function to Main Function to check whether warmstart is required.
 *
 * \param           None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

#if (DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_Main_Warmstart(void)
{

    Std_ReturnType stGetPermTxWarmResp_u8; /* Variable to get return from DcmAppl API */


    /* Check for No Communication mode in DCM */

    if(Dcm_ProgConditions_st.StoreType != DCM_NOTVALID_TYPE)
    {

       if(Dcm_ProgConditions_st.StoreType == DCM_WARMINIT_TYPE)
       {


           /* No warm response needs to be transmitted, move to FULL COMM mode */
           Dcm_DslDsdWarmStart();



           Dcm_ReadyForBoot_b = FALSE;
       }

       else
       {
           if(DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_GetActiveConnectionIdx_u8()].channel_idx_u8))
           {
               /* Call the application API to get the permissions for transmission */
               stGetPermTxWarmResp_u8 = DcmAppl_DcmGetPermTxWarmResp();

               switch(stGetPermTxWarmResp_u8)
               {
                   /* If the permission is given by application to transmit the warm response */
                   case E_OK :
                   {
                     Dcm_DslDsdWarmStart();
                     /* Reset the variable to FALSE to prevent re-entry to the loop as permission is received for transmitting response */\
                     Dcm_ReadyForBoot_b = FALSE;
                   }
                     break;

                   case DCM_E_PENDING:
                     break;

                   default :
                   {
                     /* Move to FULL COMM mode as the warm response cannot be sent */
                     /* Reset the Programming table contents */
                     Dcm_ProgConditions_st.StoreType = DCM_NOTVALID_TYPE;
                     /* Reset the variable to FALSE to prevent re-entry to the loop as permission is not received for transmitting response */
                     Dcm_ReadyForBoot_b = FALSE;
                   }
                    break;
               }

           }
       }
    }
    else
    {
     /* State transition to FULL-COMM will happen via Dcm call-back to ComM. Nothing to be done here */
     /* Reset the variable to FALSE as the StoreType is invalid */

     Dcm_ReadyForBoot_b = FALSE;
    }

}
#endif



/**
 *************************************************************************************************
 * Dcm_MainFunction :  DCM cyclic function called by the OS
 * \param           None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_MainFunction(void)
{

    /*Check if DCM is initialized?*/

    if(Dcm_acceptRequests_b != FALSE)
    {

#if (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)
        if(Dcm_Dsp_SecaGlobaltimer_u32 < 0xFFFFFFFFu)
        {
        /* If the SECA service is present in DSP */
        Dcm_Dsp_SecaGlobaltimer_u32++;
        }

#if(DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)
        if(Dcm_GetattemptCounterWaitCycle_u8 <= DCM_DSP_SECURITY_MAX_ATTEMPT_COUNTER_READOUT_TIME)
        {
        /* Call DSP function to load the Delay count values of Security levels */
        Dcm_Dsp_RestoreDelayCount();
        }
#endif

#endif

#if (DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
        if(Dcm_ReadyForBoot_b != FALSE)
        {
         Dcm_Prv_Main_Warmstart();
        }

    /*Check for No Communication mode in DCM*/
        if(Dcm_ReadyForBoot_b == FALSE)
        {
        /*Dcm is initialized*/
        Dcm_isInitialised_b = TRUE;
        }
#else
       /*initialized the variable*/
        Dcm_isInitialised_b = TRUE;
#endif
        Dcm_Prv_Statemachine_Init();


        Dcm_Prv_UdsMainFunction();

#if(DCM_CFG_VIN_SUPPORTED != DCM_CFG_OFF)
        Dcm_Prv_MainVINinit ();
#endif

#if((DCM_CFG_DSPOBDSUPPORT_ENABLED!= DCM_CFG_OFF)&&(DCM_CFG_DSP_OBDMODE9_ENABLED != DCM_CFG_OFF)&&\
        (DCM_CFG_INFOTYPE_SUPPORT != DCM_CFG_OFF))
        if(Dcm_SupportInfotype_b==TRUE)
        {
            Dcm_Prv_SupportInfoTypecheck();
            Dcm_SupportInfotype_b=FALSE;
        }
#endif

    }
}

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

