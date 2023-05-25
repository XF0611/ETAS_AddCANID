

/***************************************************************************************************
*                              Include files                                                       *
***************************************************************************************************/

#include "CanIf_Prv.h"

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */


/****************************************************************************************************
 *                              Function Definition                                                 *
 ****************************************************************************************************/

/**********************************************************************************************************************
** Function name     : CanIf_CheckWakeup                                                                             **
** Syntax            : Std_ReturnType CanIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource)                          **
** Service ID[hex]   : 0x11                                                                                          **
** Sync/Async        : Asynchronous                                                                                  **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : WakeupSource                                                                                  **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : This service checks, whether an underlying CAN driver or a CAN transceiver driver already     **
**                     signals a wakeup event.                                                                       **
**********************************************************************************************************************/
/*[SWS_CANIF_00180] The function is pre-compile time enabled if CanIfWakeupSupport is enabled */
#if (CANIF_WAKEUP_SUPPORT == STD_ON)&&(CANIF_CFG_TRCV_WAKEUP_SUPPORT == STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_CheckWakeup (EcuM_WakeupSourceType WakeupSource)
{
    /*Local pointer to CanIf_Prv_WakeUp_Event_ta */
    const CanIf_Wakeup * WkpEvent_tpcst;
    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * lControllerState_p;

    /* Flag to know whether a wakeup device was found*/
    boolean WkpDeviceFound_b= FALSE;

    /* Variable to loop through transceiver */
    uint8_least LoopVar_tqu8;

    /* Variable to loop through transceiver */
    uint8_least WakeupId_tqu8= 0;

    /*Return value*/
    Std_ReturnType Ret_trcv_t = E_NOT_OK;

    WkpEvent_tpcst = CanIf_WakeupList_tacst;
    /*[SWS_CANIF_00401] If CAN Interface is uninitialized, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_CHECKWAKEUP_SID, CANIF_E_UNINIT)

    /*Loop through to know the wakeup device*/
    for(LoopVar_tqu8 = 0u; LoopVar_tqu8 < CANIF_CFG_NO_TRCV_WAKEUP_SUPPORT; LoopVar_tqu8++)
    {
        /*If the WakeupSource is one of the Transceivers */
        if((WkpEvent_tpcst +LoopVar_tqu8)->WakeupSourceId == WakeupSource)
        {
            /*Update the Flag*/
            WkpDeviceFound_b = TRUE;

            WakeupId_tqu8= LoopVar_tqu8;

            /*Stop searching the LUT*/
            break;
        }
    }


    /*[SWS_CANIF_00398]Report DET if no wakeup devices were found*/
    CANIF_DET_REPORT_ERROR_NOT_OK((WkpDeviceFound_b == FALSE), CANIF_CHECKWAKEUP_SID, CANIF_E_PARAM_WAKEUPSOURCE)

    /*Get configured data of requested controller*/
    lControllerState_p = CanIf_Prv_ControllerState_ast + (WkpEvent_tpcst +WakeupId_tqu8)->ControllerId;

    /*[SWS_CANIF_00679] Report DET if Controller mode is not equal to CANIF_CS_SLEEP */
    CANIF_DET_REPORT_ERROR_NOT_OK(((((lControllerState_p->Ctrl_Pdu_mode)& CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT) !=(uint8)CANIF_CS_SLEEP), CANIF_CHECKWAKEUP_SID, CANIF_E_NOT_SLEEP)

    if(((((lControllerState_p->Ctrl_Pdu_mode)& CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT) == (uint8)CANIF_CS_SLEEP) && (WkpDeviceFound_b == TRUE))
    {
        /*[SWS_CANIF_00720] Invoke Wakeup functions */
        Ret_trcv_t = CanTrcv_CheckWakeup((WkpEvent_tpcst +LoopVar_tqu8)->TransceiverId);
    }


    return(Ret_trcv_t);

}/* End of CanIf_CheckWakeup() */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif/*end of #if (CANIF_WAKEUP_SUPPORT == STD_ON)&&(CANIF_CFG_TRCV_WAKEUP_SUPPORT == STD_ON)*/


/**********************************************************************************************************************
** Function name     : CanIf_CheckValidation                                                                         **
** Syntax            : Std_ReturnType CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource)                      **
** Service ID[hex]   : 0x12                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : WakeupSource                                                                                  **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : This service is performed to validate a previous wakeup event.                                **
**********************************************************************************************************************/
/*Doubts:
 * 1. In the old code the wakeup event is resent on the invocation of CanIf_CheckValidation(). I am not able to find the requirement for the same.
 *  */
/*[SWS_CANIF_00226]This API is available only if wakeup is supported in transceivers and Wakeupcheckvalidationsupport is enabled*/
#if (CANIF_CFG_TRCV_WAKEUP_SUPPORT == STD_ON && CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_CheckValidation (EcuM_WakeupSourceType WakeupSource)
{
    /*Local pointer to CanIf_Prv_WakeUp_Event_ta */
    const CanIf_Wakeup * WkpEvent_tpcst;

    const CanIf_CallbackFuncType * CallBackFunc_ps;

    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * ControllerState_ps;

    /* Variable to loop through transceiver */
    uint8_least LoopVar_tqu8;

    /* Variable to loop through transceiver */
    uint8_least WakeupId_tqu8= 0;

    /* Flag to know whether a wakeup device was found*/
    Std_ReturnType WkpDeviceFound_b= E_NOT_OK;

    WkpEvent_tpcst = CanIf_WakeupList_tacst;

    /*Initializing callback pointer */
    CallBackFunc_ps = &CanIf_Callback;

    /*Report to DET if CanIF was not initialized and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_CHECKVALIDATION_SID, CANIF_E_UNINIT)

    /*Loop through the Wakeup LUT to know the device*/
    for(LoopVar_tqu8 = 0; LoopVar_tqu8 < CANIF_CFG_NO_TRCV_WAKEUP_SUPPORT; LoopVar_tqu8++)
    {
        /*If the WakeupSource is a valid one...*/
        if((WkpEvent_tpcst +LoopVar_tqu8)->WakeupSourceId == WakeupSource)
        {

            /*Wakeup source is valid*/
            WkpDeviceFound_b = E_OK;

            WakeupId_tqu8 = LoopVar_tqu8;

            /*Stop searching the LUT*/
            break;
        }
    }

    /*[SWS_CANIF_00404] Report DET if no wakeup devices were found*/
    CANIF_DET_REPORT_ERROR_NOT_OK((WkpDeviceFound_b == E_NOT_OK), CANIF_CHECKVALIDATION_SID, CANIF_E_PARAM_WAKEUPSOURCE)

    /*Fetching controller Information to local variable */
    ControllerState_ps= CanIf_Prv_ControllerState_ast + (WkpEvent_tpcst +WakeupId_tqu8)->ControllerId;

    if(((((ControllerState_ps->Ctrl_Pdu_mode)& CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT) == (uint8)CANIF_CS_STARTED) && (WkpDeviceFound_b ==E_OK))
    {
        /*[SWS_CANIF_00179] Indicate application about Wakeup condition */
        if((NULL_PTR != CallBackFunc_ps->User_ValidateWakeupEvent) && (ControllerState_ps->ValidationSts_b == CANIF_SET))
        {
            CallBackFunc_ps->User_ValidateWakeupEvent(WakeupSource);
        }
    }

    return(WkpDeviceFound_b);
}/* End of CanIf_CheckValidation() */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif/*End of #if (CANIF_CFG_TRCV_WAKEUP_SUPPORT == STD_ON && CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)*/
