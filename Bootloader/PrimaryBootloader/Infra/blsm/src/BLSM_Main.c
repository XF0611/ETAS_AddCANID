/*********************************************************************************************************************
*
* Description:     BLSM module implementation for Port FBL
* FileName:        BLSM_Main.c
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2019. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
UCN1HC          07/07/2019      modified
**********************************************************************************************************************/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/

#include "BLSM.h"
#include "BLSM_Callouts.h"
#include "ComM.h"
#include "CanSM_ComM.h"
#include "CanSM.h"
#include "Dcm_Cbk.h"
#include "FBL_BootM.h"
#include "Os_TimeServices.h"
#include "EcuMgt.h"
#include "Os.h"
// #include "EthSM.h"

/* ETAS BIP Porting */
#include "arc_exception.h"
#include "alps_clock.h"
#include "alps_emu_reg.h"

/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
static Fbl_BLSM_CmpStateType Fbl_BLSM_CurrentState = FBL_BLSM_CMPSTATE_INIT;

static boolean FBL_BLSM_NvmReadAllEnd = FALSE;

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

/**
 * @brief Initializes bootloader state machine component. This is also the entry main function.
 * @return None.
 *  
 */
Fbl_BLSM_Init                              /* PRQA S 3200 *//* PRQA S 3447 *//* PRQA S 0601 *//* PRQA S 3335 */
{
    raw_writel(REG_EMU_BOOT_DONE, 1);

    /*disbale wdg */

    Fbl_BLSM_CurrentState = FBL_BLSM_CMPSTATE_STARTUP;
    /*  init list One */
    Fbl_BLSM_InitOne();
    /* Init list two */
    Fbl_BLSM_InitTwo();
    Os_Start();
}


/**
 * @brief the BLSM_MainFunction manage the state transaction of FBL system. 
 * @return None.
 */
void Fbl_BLSM_MainFunction(void)                                                                                                /* PRQA S 1503 */
{
	static Fbl_BLSM_CmpStateType Fbl_BLSM_PrevState    = FBL_BLSM_CMPSTATE_INIT;
	ComM_ModeType CurrComMode;
    // ComM_ModeType EthCurrComMode;

    switch (Fbl_BLSM_CurrentState)
    {
        case FBL_BLSM_CMPSTATE_STARTUP:
        {
            if (FBL_BLSM_CMPSTATE_INIT == Fbl_BLSM_PrevState)
            {
                /* Stay in current state */
                Fbl_BLSM_PrevState = FBL_BLSM_CMPSTATE_STARTUP;
                /*Initialize bootloader application*/
                Fbl_BLSM_Initialize_App();
            } else {/* do nothing */}
                    /* Get the current Com mode*/
            if((E_OK == CanSM_GetCurrentComMode(ComMConf_ComMChannel_ComMChannel_Can_Network_Channel, &CurrComMode)))             /* PRQA S 3415 */
            {
                if((TRUE == FBL_BLSM_NvmReadAllEnd) && (COMM_NO_COMMUNICATION == CurrComMode))   /* PRQA S 3415 */
                {
                    /* Once the NvM has been all read, we can initialise
                    the full communication and the DCM and go into the running
                    state */
                    // EthSM_RequestComMode(ComMConf_ComMChannel_ComMChannel_Eth_Network_Channel, COMM_FULL_COMMUNICATION);    /* PRQA S 3200 */ /* MISRA DEVIATION: Don't need return value because there is nothing we can do with it anyway. */
                    CanSM_RequestComMode(ComMConf_ComMChannel_ComMChannel_Can_Network_Channel, COMM_FULL_COMMUNICATION);    /* PRQA S 3200 */ /* MISRA DEVIATION: Don't need return value because there is nothing we can do with it anyway. */
                
                    /* Init Driver list three*/
                    Fbl_BLSM_InitThree();

                    Fbl_BLSM_CurrentState = FBL_BLSM_CMPSTATE_RUN;
                }
            }
            break;
        }

        case FBL_BLSM_CMPSTATE_RUN:
        {
            /*Stay in boot only supports during development time*/
#ifdef DEVELOPMENT
            BLSM_StayInBootMonitorMainFunction();
#endif
            break;
        }
        default:
        {
        	break;
        }
    }
}


/* MISRA RULE 3673: The object addressed by the pointer parameter '%s' is not
 * modified and so the pointer could be of type 'pointer to const'. */
FUNC(void,COMM_CODE) ComM_BusSM_ModeIndication                                                                          /* PRQA S 1503 */
                                       (
                                           VAR(NetworkHandleType,AUTOMATIC ) Channel,
                                           P2VAR(ComM_ModeType,AUTOMATIC,COMM_APPL_DATA) ComMode /* PRQA S 3673 */ /*: The function's protocol is defined by Generated code in BSW layer */
                                       )
{
    if (*ComMode == COMM_FULL_COMMUNICATION)
    {
        Dcm_ComM_FullComModeEntered(Channel);
    }
    else if (*ComMode == COMM_SILENT_COMMUNICATION)
    {
        Dcm_ComM_SilentComModeEntered(Channel);
    }
    else if (*ComMode == COMM_NO_COMMUNICATION)
    {
        Dcm_ComM_NoComModeEntered(Channel);
    }
    else
    {
    	/* Do nothing */
    }
}


/**
 * @brief a Callback function of the NVM which is called whenever Fbl_DataM_MultiBlockJobEndNotification
 * is notified.
 *
 * @param ServiceId Service ID of the NvM
 * @param JobResult Job result
 * @return None
 */
void Fbl_BLSM_NfyDataMJobEnd (uint8 ServiceId, NvM_RequestResultType JobResult)                                                                     /* PRQA S 1503 */
{
    if((NVM_SERVICE_ID_READ_ALL == ServiceId) && ((NvM_RequestResultType)NVM_REQ_OK == JobResult))
    {
        FBL_BLSM_NvmReadAllEnd = TRUE;
    }
}


/**
 * @brief A stay in boot service routine. This will be activated if the data of reception is Data = 04 31 01 F5 18 xx xx xx
 * 
 */
FUNC(Std_ReturnType, DCM_APPL_CODE) Fbl_BLSM_StayInBootCallback(                                                                                        /* PRQA S 1503, 3408 */
                         VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus, /* PRQA S 3206 */ /* This function does not use this parameter */
                          P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_APPL_DATA) ErrorCode)
{
    Std_ReturnType ret = E_OK;
    *ErrorCode = E_OK;
#ifdef DEVELOPMENT
    BLSM_IndicatedStayInBoot();
#endif
    return ret;
}
