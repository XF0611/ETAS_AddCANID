/*********************************************************************************************************************
*
* Description:     BootM module implementation
* FileName:        FBL_BootM.c
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
*/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Os.h"
#include "FBL_WdgM.h"
#include "FBL_BootM.h"
#include "CanSM_ComM.h"
#include "Dcm.h"
#include "FBL_DataM.h"
#include "EcuMgt.h"
#include "BLSM_CallOuts.h"
#include "ImageM.h"
#include "ImageM_ImageUpdateStatus.h"
#include "FBL_BootMCfg.h"
// #include "EthSM.h"
#include "Can.h"
#include "arc_builtin.h"
#include "arc_cache.h"
#include "OsTargetTypes.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define IMAGEM_APPLICATION_REGION_START_ADDRESS_ARC ((uint32)0x380004) //zco7sgh: equal to address of "_arc_start" (EXT_ROM + 4)

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/**
 * @brief
 *
 */
typedef enum
{
    FBL_BOOTM_CMPSTATE_NOT_INIT,
    FBL_BOOTM_CMPSTATE_INIT
} Fbl_BootM_CmpStateType;

/**
 * @brief
 *
 */
typedef enum
{
    FBL_BOOTM_SYSTEM_STATE_RUNNING,
    FBL_BOOTM_SYSTEM_STATE_PRE_RUNNING
} Fbl_BootM_SystemStateType;

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
/* Component status */
static Fbl_BootM_CmpStateType Fbl_BootM_CmpState = FBL_BOOTM_CMPSTATE_NOT_INIT;
/* Main function state variables */
static Fbl_BootM_SystemStateType Fbl_BootM_CurrentSystemState = FBL_BOOTM_SYSTEM_STATE_PRE_RUNNING;
static boolean Fbl_BootM_DefaultSessionSwitched = FALSE;
/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/
static void Fbl_BootM_DcmSessionMonitor(void);
static Std_ReturnType Fbl_BootM_ClearReprogrammingRequestFlag(void);
/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/
/* Implement Bootloader Session transition in accordance with ISO14229 */
static void Fbl_BootM_DcmSessionMonitor(void)
{
    Dcm_SesCtrlType currSession;
    Fbl_BootM_ReprogrammingRequestFlagStateType reprogrammingRequestFlagState;
    static Dcm_SesCtrlType Fbl_BootM_PreviousSession = DCM_DEFAULT_SESSION;

    (void)Fbl_BootM_ReadReprogrammingRequestFlag(&reprogrammingRequestFlagState);
    /* There is no need to check the return value because Dcm_GetSesCtrlType always returns E_OK*/
    (void)Dcm_GetSesCtrlType(&currSession);

    if (((currSession != DCM_PROGRAMMING_SESSION) && (Fbl_BootM_PreviousSession == DCM_PROGRAMMING_SESSION)) ||
        ((currSession == DCM_DEFAULT_SESSION) && (Fbl_BootM_DefaultSessionSwitched == TRUE)))
    {
        /*Reset ECu when non-default session has been timeout or after a ROLLBACK process has done!*/
        EcuMgt_PerformReset();
    }

    if (FBL_BOOTM_REPROGRAMMING_REQUEST_FLAG_STATE_ON == reprogrammingRequestFlagState)
    {
        (void)Fbl_BootM_ClearReprogrammingRequestFlag();
    }
    Fbl_BootM_DefaultSessionSwitched = FALSE;
    Fbl_BootM_PreviousSession = currSession;
}


/**
 * @brief clear the reporgramming flag. 
 *        It will change this flag to FBL_BOOTM_REPROGRAMMING_REQUEST_FLAG_STATE_OFF
 * 
 * @return Std_ReturnType result is E_OK        - succeeded.
 *                                  E_NOT_OK    - Failed.
 */
static Std_ReturnType Fbl_BootM_ClearReprogrammingRequestFlag(void)
{
    Std_ReturnType ret = E_NOT_OK;
    Fbl_BootM_ReprogrammingRequestFlagStateType flag = FBL_BOOTM_REPROGRAMMING_REQUEST_FLAG_STATE_OFF;
    ret = FBL_DataM_WriteNvmReprogrammingRequestFlag((const uint8 *)&flag);                                                 /* PRQA S 0310 */
    return ret;
}
/*********************************************************************************************************************
* Export Functions Implementation
*********************************************************************************************************************/

/**
 * @brief Initialize bootM component
 * @return None
 */
void Fbl_BootM_Init(void)                                                                                                   /* PRQA S 1503 */
{
    Fbl_BootM_CmpState = FBL_BOOTM_CMPSTATE_INIT;
    Fbl_BootM_CurrentSystemState = FBL_BOOTM_SYSTEM_STATE_PRE_RUNNING;
}

/**
 * @brief De-initialize BootM component
 * @return None
 */
void Fbl_BootM_DeInit(void)                                                                                                 /* PRQA S 1503 */
{
    Fbl_BootM_CmpState = FBL_BOOTM_CMPSTATE_NOT_INIT;
}


/**
 * @brief Service is monitor bootloader sesstion transition.
 * @return None
 */
void Fbl_BootM_MainFunction(void)                                                                                            /* PRQA S 1503 */
{
    ComM_ModeType Can_ComMStatus;

    if (Fbl_BootM_CmpState == FBL_BOOTM_CMPSTATE_INIT)
    {
        switch (Fbl_BootM_CurrentSystemState)
        {
        case FBL_BOOTM_SYSTEM_STATE_PRE_RUNNING:
            if(E_OK == CanSM_GetCurrentComMode(ComMConf_ComMChannel_ComMChannel_Can_Network_Channel, &Can_ComMStatus))
            {
                if (COMM_FULL_COMMUNICATION == Can_ComMStatus)
                {
                    /*BootM transition state to FBL_BOOTM_SYSTEM_STATE_RUNNING */
                    Fbl_BootM_CurrentSystemState = FBL_BOOTM_SYSTEM_STATE_RUNNING;
                }
            }
            break;
        case FBL_BOOTM_SYSTEM_STATE_RUNNING:
            /*Call session monitor service*/
            Fbl_BootM_DcmSessionMonitor();
            break;
        default:
            break;
        }
    }
}

/**
 * @brief Callback from DCM to notify whenever the session change.
 * 
 */
FUNC(void, DCM_APPL_CODE)
DcmAppl_Switch_DcmDiagnosticSessionControl(VAR(Dcm_SesCtrlType, AUTOMATIC) SessionMode)                                                 /* PRQA S 1503 */
{
    static boolean Fbl_BootM_FirstEntrySession = TRUE;
    if (TRUE == Fbl_BootM_FirstEntrySession)
    {
        Fbl_BootM_FirstEntrySession = FALSE;
        Fbl_BootM_DefaultSessionSwitched = FALSE;
    }
    else
    {
        if (SessionMode == DCM_DEFAULT_SESSION)
        {
            Fbl_BootM_DefaultSessionSwitched = TRUE;
        }
    }
}

void Fbl_BootM_CheckEcuState(void)                                                                                                      /* PRQA S 1503 */
{    
    ImageM_ApplicationValidFlagType ApplicationValidate_Flag;
    Fbl_BootM_ReprogrammingRequestFlagStateType reprogrammingRequestState;
    
    (void)ImageM_ImageStatus_ReadApplicationValidFlag((uint8 *)&ApplicationValidate_Flag);               /* PRQA S 0310 */
    (void)Fbl_BootM_ReadReprogrammingRequestFlag(&reprogrammingRequestState);

    /* zco7shg: flag shall be checked as per configurable images/cores later */
    if((FBL_BOOTM_VALIDITY_TRUE_ARC == ApplicationValidate_Flag) && (FBL_BOOTM_REPROGRAMMING_REQUEST_FLAG_STATE_ON != reprogrammingRequestState))
    {
        // Perform JUMP to Application
        Fbl_JumpToApplication();
    }
}

void Fbl_JumpToApplication(void)
{
    void (*appStartAdd)(void);
    Can_DeInit();
    Fbl_WdgM_Disable();
    Os_DisableAllInterrupts();
    
    // appStartAdd = (void (*)(void))(IMAGEM_APPLICATION_REGION_START_ADDRESS);   /* PRQA S 0305, 0306 */ /* MISRA DEVIATION: Cast between a pointer to object or pointer to function and an integral type is necessary. */
    appStartAdd = (void (*)(void))(IMAGEM_APPLICATION_REGION_START_ADDRESS_ARC);
    _arc_aux_write(0x4B, 1);
    while (_arc_aux_read(0x48) & 0x100);
    icache_invalidate();
    appStartAdd();
}
