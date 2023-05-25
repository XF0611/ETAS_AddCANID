

/**********************************************************************************************************************
** Includes                                                                                                          **
**********************************************************************************************************************/
#include "CanIf_Prv.h"
#if CANIF_CFG_TRCV_DRV_SUPPORT ==STD_ON
#include "CanTrcv.h"
#endif

/**********************************************************************************************************************
** used functions                                                                                                    **
**********************************************************************************************************************/

/**********************************************************************************************************************
** Function name     : CanIf_SetTrcvMode                                                                             **
** Syntax            : Std_ReturnType CanIf_SetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)   **
** Service ID[hex]   : 0x0d                                                                                          **
** Sync/Async        : Asynchronous                                                                                  **
** Reentrancy        : Non Reentrant                                                                                 **
** Parameters (in)   : TransceiverId, TransceiverMode                                                                **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : This service changes the operation mode of the tansceiver TransceiverId, via calling the      **
**                     corresponding CAN Transceiver Driver service.                                                 **
**********************************************************************************************************************/
/*[SWS_CANIF_00XXX]The function is pre-compile time enabled if Tranceiver is configured */
#if CANIF_CFG_TRCV_DRV_SUPPORT ==STD_ON
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_SetTrcvMode(uint8 TransceiverId,
        CanTrcv_TrcvModeType TransceiverMode)
{
    /*Return value*/
    Std_ReturnType Status_t;

    /*Report to DET if CanIF was not initialized and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_SET_TRCV_MODE_SID, CANIF_E_UNINIT)

    /* [SWS_CANIF_00538] Report to DET if Transceiver Id is invalid and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_SET_TRCV_MODE_SID, CANIF_E_PARAM_TRCV)

    /* [SWS_CANIF_00648] Report to DET if invalid TransceiverMode ie other than STANDBY, NORMAL, SLEEP 0,1,2*/
    CANIF_DET_REPORT_ERROR_NOT_OK(((TransceiverMode!=CANTRCV_TRCVMODE_NORMAL)&&(TransceiverMode!=CANTRCV_TRCVMODE_SLEEP)&&(TransceiverMode!=CANTRCV_TRCVMODE_STANDBY)), CANIF_SET_TRCV_MODE_SID, CANIF_E_PARAM_TRCVMODE)

    /*[SWS_CANIF_00358] Call the Transceiver driver function */
    Status_t = CanTrcv_SetOpMode(TransceiverId,TransceiverMode);

    /*return value*/
     return (Status_t);

}/*End of CanIf_SetTrcvMode()*/


#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
/**********************************************************************************************************************
** Function name     : CanIf_GetTrcvMode                                                                             **
** Syntax            : Std_ReturnType CanIf_GetTrcvMode(CanTrcv_TrcvModeType* TransceiverModePtr,uint8 TransceiverId)**
** Service ID[hex]   : 0x0e                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Non Reentrant                                                                                 **
** Parameters (in)   : TransceiverId                                                                                 **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : TransceiverModePtr                                                                            **
** Return value      : Std_ReturnType                                                                                **
** Description       : This function invokes CanTrcv_GetOpMode and updates the parameter TransceiverModePtr with     **
**                     the value OpMode provided by CanTrcv.                                                         **
**********************************************************************************************************************/
/*[SWS_CANIF_00XXX]The function is pre-compile time enabled if Tranceiver is configured */
#if CANIF_CFG_TRCV_DRV_SUPPORT ==STD_ON
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_GetTrcvMode(CanTrcv_TrcvModeType * TransceiverModePtr,
                                       uint8 TransceiverId)

{
    /*Return value*/
    Std_ReturnType Status_t;

    /*Report to DET if CanIF was not initialized and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_GET_TRCV_MODE_SID, CANIF_E_UNINIT)

    /* [SWS_CANIF_00364] Report to DET if Transceiver Id is invalid and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_GET_TRCV_MODE_SID, CANIF_E_PARAM_TRCV)

    /*[SWS_CANIF_00650] Report to DET if invalid TransceiverModePtr ie if a NULL pointer*/
    CANIF_DET_REPORT_ERROR_NOT_OK((TransceiverModePtr== NULL_PTR), CANIF_GET_TRCV_MODE_SID, CANIF_E_PARAM_POINTER)

    /*[SWS_CANIF_00363] Call the Transceiver driver function */
    Status_t =CanTrcv_GetOpMode(TransceiverId, TransceiverModePtr);

    /*return value*/
    return (Status_t);

}/*CanIf_GetTrcvMode() ends*/

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/**********************************************************************************************************************
** Function name     : CanIf_GetTrcvWakeupReason                                                                     **
** Syntax            : Std_ReturnType CanIf_GetTrcvWakeupReason(uint8 TransceiverId,                                 **
**                     CanTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr)                                                **
** Service ID[hex]   : 0x0f                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Non Reentrant                                                                                 **
** Parameters (in)   : TransceiverId                                                                                 **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : TrcvWuReasonPtr                                                                               **
** Return value      : Std_ReturnType                                                                                **
** Description       : This service returns the reason for the wake up of the transceiver TransceiverId, via calling **
**                     the corresponding CAN Transceiver Driver service.                                             **
**********************************************************************************************************************/
/*[SWS_CANIF_00371]The function is pre-compile time enabled if Tranceiver is configured */
#if CANIF_CFG_TRCV_DRV_SUPPORT==STD_ON
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_GetTrcvWakeupReason( uint8 TransceiverId,
        CanTrcv_TrcvWakeupReasonType * TrcvWuReasonPtr)
{
    /*Return value*/
    Std_ReturnType Status_t;

    /*[SWS_CANIF_00XXX] Report to DET if CanIF was not initialised and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_GET_TRCV_WUMODE_REASON_SID, CANIF_E_UNINIT)

    /* [SWS_CANIF_00537] Report to DET if Transceiver Id is invalid and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_GET_TRCV_WUMODE_REASON_SID, CANIF_E_PARAM_TRCV)

    /*[SWS_CANIF_00649] Report to DET if invalid TransceiverModePtr ie if a NULL pointer*/
    CANIF_DET_REPORT_ERROR_NOT_OK((TrcvWuReasonPtr== NULL_PTR), CANIF_GET_TRCV_WUMODE_REASON_SID, CANIF_E_PARAM_POINTER)

    /*[SWS_CANIF_00368] Get the wakeup reason for the Transceiver */
    Status_t =CanTrcv_GetBusWuReason(TransceiverId,TrcvWuReasonPtr);

    /*return value*/
    return (Status_t);

}/*CanIf_GetTrcvWakeupReason() ends*/
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/**********************************************************************************************************************
** Function name     : CanIf_SetTrcvWakeupMode                                                                       **
** Syntax            : Std_ReturnType CanIf_SetTrcvWakeupMode(uint8 TransceiverId,                                   **
**                     CanTrcv_TrcvWakeupModeType* TrcvWakeupMode)                                                   **
** Service ID[hex]   : 0x10                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Non Reentrant                                                                                 **
** Parameters (in)   : TransceiverId, TrcvWakeupMode                                                                 **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : This function shall call CanTrcv_SetTrcvWakeupMode.                                           **
**********************************************************************************************************************/
/*[SWS_CANIF_00XXX]The function is pre-compile time enabled if Tranceiver is configured */
#if CANIF_CFG_TRCV_DRV_SUPPORT==STD_ON
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_SetTrcvWakeupMode(uint8 TransceiverId,
        CanTrcv_TrcvWakeupModeType TrcvWakeupMode)
{
    /*Return value*/
    Std_ReturnType Status_t;

    /*[SWS_CANIF_00XXX] Report to DET if CanIF was not initialised and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_SET_TRCV_WAKEUPMODE_SID, CANIF_E_UNINIT)

    /* [SWS_CANIF_00535] Report to DET if Transceiver Id is invalid and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_SET_TRCV_WAKEUPMODE_SID, CANIF_E_PARAM_TRCV)

    /* [SWS_CANIF_00536] Report to DET if invalid TrcvWakeupMode ie other than ENABLE, DISABLE, CLEAR */
    CANIF_DET_REPORT_ERROR_NOT_OK(((TrcvWakeupMode!=CANTRCV_WUMODE_ENABLE)&&(TrcvWakeupMode!=CANTRCV_WUMODE_DISABLE)&&(TrcvWakeupMode!=CANTRCV_WUMODE_CLEAR)), CANIF_SET_TRCV_WAKEUPMODE_SID, CANIF_E_PARAM_TRCVWAKEUPMODE)

    /* [SWS_CANIF_00372] Enables, disables or clears wake-up events of the Transceiver according to TrcvWakeupMode*/
    Status_t =CanTrcv_SetWakeupMode(TransceiverId, TrcvWakeupMode);

    return (Status_t);

}/*CanIf_SetTrcvWakeupMode() ends*/
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/**********************************************************************************************************************
** Function name     : CanIf_ClearTrcvWufFlag                                                                        **
** Syntax            : Std_ReturnType CanIf_ClearTrcvWufFlag(uint8 TransceiverId)                                    **
** Service ID[hex]   : 0x1e                                                                                          **
** Sync/Async        : Asynchronous                                                                                  **
** Reentrancy        : Reentrant for different CAN transceivers                                                      **
** Parameters (in)   : TransceiverId                                                                                 **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : Requests the CanIf module to clear the WUF flag of the designated CAN transceiver.            **
**********************************************************************************************************************/
/*[SWS_CANIF_00771]The function is pre-compile time enabled using the parameter CANIF_PUBLIC_PN_SUPPORT */
#if((CANIF_PUBLIC_PN_SUPPORT == STD_ON) && (CANIF_CFG_TRCV_DRV_SUPPORT==STD_ON))
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_ClearTrcvWufFlag(uint8 TransceiverId)
{
    /*Return value*/
    Std_ReturnType Status_t;

#if (CANIF_CFG_TRCV_PN_SUPPORT == STD_OFF)
    /*Initialse the return value*/
     Status_t = E_NOT_OK;
#endif

    /* [SWS_CANIF_00708] Report to DET if CanIF was not initialized and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_CLEARTRCVWUFFLAG_SID, CANIF_E_UNINIT)

    /* [SWS_CANIF_00706] Report to DET if Transceiver Id is invalid and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_CLEARTRCVWUFFLAG_SID, CANIF_E_PARAM_TRCV)

    /*[SWS_CANIF_00766] Call the CanTrcv function*/
#if (CANIF_CFG_TRCV_PN_SUPPORT == STD_ON)
    Status_t = CanTrcv_ClearTrcvWufFlag(TransceiverId);
#endif

    /*return value*/
    return(Status_t);
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/**********************************************************************************************************************
** Function name     : CanIf_TrcvModeIndication                                                                      **
** Syntax            : void CanIf_TrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)      **
** Service ID[hex]   : 0x22                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : TransceiverId, TransceiverMode                                                                **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : This service indicates a transceiver state transition referring to the corresponding CAN      **
**                     transceiver with the abstract CanIf TransceiverId.                                            **
**********************************************************************************************************************/
/*[SWS_CANIF_00730]The function is pre-compile time enabled if Tranceiver is configured */
#if CANIF_CFG_TRCV_DRV_SUPPORT==STD_ON
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_TrcvModeIndication(uint8 TransceiverId,
        CanTrcv_TrcvModeType TransceiverMode)
{
    /*Pointer to the structure CanIf_CallbackFuncType */
    const CanIf_CallbackFuncType * CallBackPtr_Temp;

    /*Initializing callback pointer */
    CallBackPtr_Temp = &CanIf_Callback;

    /*[SWS_CANIF_00708] Report to DET if CanIF was not initialised and exit the function*/
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_TRCVMODEINDICATION_SID, CANIF_E_UNINIT)

    /* [SWS_CANIF_00706] Report to DET if Transceiver Id is invalid and exit the function*/
    CANIF_DET_REPORT_ERROR_VOID_RET(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_TRCVMODEINDICATION_SID, CANIF_E_PARAM_TRCV)

    /* Indicate upper layer*/
    if(NULL_PTR != CallBackPtr_Temp->User_TransceiverModeIndication)
    {
        CallBackPtr_Temp->User_TransceiverModeIndication(TransceiverId, TransceiverMode);
    }
}/*end of CanIf_TrcvModeIndication()*/
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
/**********************************************************************************************************************
** Function name     : CanIf_CheckTrcvWakeFlag                                                                       **
** Syntax            : Std_ReturnType CanIf_CheckTrcvWakeFlag(uint8 TransceiverId)                                   **
** Service ID[hex]   : 0x1f                                                                                          **
** Sync/Async        : Asynchronous                                                                                  **
** Reentrancy        : Reentrant for different CAN transceivers                                                      **
** Parameters (in)   : TransceiverId                                                                                 **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : Requests the CanIf module to check the Wake flag of the designated CAN transceiver.           **
**********************************************************************************************************************/
/*[SWS_CANIF_00813]The function is pre-compile time enabled using the parameter CANIF_PUBLIC_PN_SUPPORT */
#if((CANIF_PUBLIC_PN_SUPPORT == STD_ON) && (CANIF_CFG_TRCV_DRV_SUPPORT==STD_ON))
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_CheckTrcvWakeFlag(uint8 TransceiverId)
{
    /*Return value*/
    Std_ReturnType Status_t;

    /* [SWS_CANIF_00XXX] Report to DET if CanIF was not initialized and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_CHECKTRCVWAKEFLAG_SID, CANIF_E_UNINIT)

    /* [SWS_CANIF_00770] Report to DET if Transceiver Id is invalid and exit the function*/
    CANIF_DET_REPORT_ERROR_NOT_OK(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_CHECKTRCVWAKEFLAG_SID, CANIF_E_PARAM_TRCV)

    /*[SWS_CANIF_00765] Call the CanTrcv function*/
    Status_t = CanTrcv_CheckWakeFlag(TransceiverId);

    /*return value*/
    return(Status_t);
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/**********************************************************************************************************************
** Function name     : CanIf_ClearTrcvWufFlagIndication                                                              **
** Syntax            : Void CanIf_ClearTrcvWufFlagIndication(uint8 TransceiverId)                                    **
** Service ID[hex]   : 0x20                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : TransceiverId                                                                                 **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : This service indicates that the transceiver has cleared the WufFlag referring to the          **
**                     corresponding CAN transceiver with the abstract CanIf TransceiverId.                          **
**********************************************************************************************************************/
/*[SWS_CANIF_00808]The function is pre-compile time enabled using the parameter CANIF_PUBLIC_PN_SUPPORT */
#if(CANIF_PUBLIC_PN_SUPPORT == STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_ClearTrcvWufFlagIndication(uint8 TransceiverId)
{
        /*Pointer to the structure CanIf_CallbackFuncType */
        const CanIf_CallbackFuncType * CallBackPtr_Temp;

        /* [SWS_CANIF_00806] Report to DET if CanIF was not initialized and exit the function*/
        CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_CLEARTRCVWAKEFLAGIND_SID, CANIF_E_UNINIT)

        /* [SWS_CANIF_00805] Report to DET if Transceiver Id is invalid and exit the function*/
        CANIF_DET_REPORT_ERROR_VOID_RET(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_CLEARTRCVWAKEFLAGIND_SID, CANIF_E_PARAM_TRCV)

    /*Initializing callback pointer */
    CallBackPtr_Temp = &CanIf_Callback;

        /*This check can be removed as User_ConfirmPnAvailability element is always generated when PN is enabled
         * If CanIfDispatchUserConfirmPnAvailabilityUL == CanSM then CanIfDispatchUserConfirmPnAvailabilityName need not be configured
         * If CanIfDispatchUserConfirmPnAvailabilityUL == CDD then CanIfDispatchUserConfirmPnAvailabilityName should be configured else validation should be invoked
         * 2. If a macro can be added as suggested in point 4 section transceiver functionality*/
        if(CallBackPtr_Temp->User_ClearTrcvWufFlagIndication != NULL_PTR)
        {
            /* [SWS_CANIF_00759] Invoke User_CheckTrcvWakeFlagIndication */
            CallBackPtr_Temp->User_ClearTrcvWufFlagIndication(TransceiverId);
        }
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/**********************************************************************************************************************
** Function name     : CanIf_CheckTrcvWakeFlagIndication                                                             **
** Syntax            : Void CanIf_CheckTrcvWakeFlagIndication(uint8 TransceiverId)                                   **
** Service ID[hex]   : 0x21                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : TransceiverId                                                                                 **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : This service indicates that the check of the transceivers wake-up flag has been finished by  **
**                     the corresponding CAN transceiver with the abstract CanIf TransceiverId. This indication is   **
**                     used to cope with the asynchronous transceiver communication.                                 **
**********************************************************************************************************************/
/*[SWS_CANIF_00812]The function is pre-compile time enabled using the parameter CANIF_PUBLIC_PN_SUPPORT */
#if(CANIF_PUBLIC_PN_SUPPORT == STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_CheckTrcvWakeFlagIndication(uint8 TransceiverId)
{
    /*Pointer to the structure CanIf_CallbackFuncType */
    const CanIf_CallbackFuncType * CallBackPtr_Temp;

    /* [SWS_CANIF_00810] Report to DET if CanIF was not initialized and exit the function*/
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_CHECKTRCVWAKEFLAGIND_SID, CANIF_E_UNINIT)

    /* [SWS_CANIF_00809] Report to DET if Transceiver Id is invalid and exit the function*/
    CANIF_DET_REPORT_ERROR_VOID_RET(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_CHECKTRCVWAKEFLAGIND_SID, CANIF_E_PARAM_TRCV)

    /*Initializing callback pointer */
    CallBackPtr_Temp = &CanIf_Callback;


    /*This check can be removed as User_ConfirmPnAvailability element is always generated when PN is enabled
     * If CanIfDispatchUserConfirmPnAvailabilityUL == CanSM then CanIfDispatchUserConfirmPnAvailabilityName need not be configured
     * If CanIfDispatchUserConfirmPnAvailabilityUL == CDD then CanIfDispatchUserConfirmPnAvailabilityName should be configured else validation should be invoked
     * 2. If a macro can be added as suggested in point 4 section transceiver functionality*/
    if(CallBackPtr_Temp->User_CheckTrcvWakeFlagIndication != NULL_PTR)
    {
        /* [SWS_CANIF_00759] Invoke User_CheckTrcvWakeFlagIndication */
        CallBackPtr_Temp->User_CheckTrcvWakeFlagIndication(TransceiverId);
    }
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/**********************************************************************************************************************
** Function name     : CanIf_ConfirmPnAvailability                                                                   **
** Syntax            : Void CanIf_ConfirmPnAvailability(uint8 TransceiverId)                                         **
** Service ID[hex]   : 0x1a                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : TransceiverId                                                                                 **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : This service indicates that the transceiver is running in PN communication mode referring to  **
**                     the corresponding CAN transceiver with the abstract CanIf TransceiverId.                      **
**********************************************************************************************************************/
/*[SWS_CANIF_00754]The function is pre-compile time enabled using the parameter CANIF_PUBLIC_PN_SUPPORT */
#if(CANIF_PUBLIC_PN_SUPPORT == STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_ConfirmPnAvailability(uint8 TransceiverId)
{
    /*Pointer to the structure CanIf_CallbackFuncType */
    const CanIf_CallbackFuncType * CallBackPtr_Temp;

    /* [SWS_CANIF_00817] Report to DET if CanIF was not initialized and exit the function*/
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_CONFIRMPNAVAILABILITY_SID, CANIF_E_UNINIT)

    /* [SWS_CANIF_00816] Report to DET if Transceiver Id is invalid and exit the function*/
    CANIF_DET_REPORT_ERROR_VOID_RET(TransceiverId >=CANIF_CFG_NUM_TRCVS, CANIF_CONFIRMPNAVAILABILITY_SID, CANIF_E_PARAM_TRCV)

    /*Initializing callback pointer */
    CallBackPtr_Temp = &CanIf_Callback;

    /*This check can be removed as User_ConfirmPnAvailability element is always generated when PN is enabled
     * If CanIfDispatchUserConfirmPnAvailabilityUL == CanSM then CanIfDispatchUserConfirmPnAvailabilityName need not be configured
     * If CanIfDispatchUserConfirmPnAvailabilityUL == CDD then CanIfDispatchUserConfirmPnAvailabilityName should be configured else validation should be invoked
     * 2. If a macro can be added as suggested in point 4 section transceiver functionality*/
     if(CallBackPtr_Temp->User_ConfirmPnAvailability != NULL_PTR)
     {
         /* [SWS_CANIF_00753] Invoke User_ConfirmPnAvailability */
         CallBackPtr_Temp->User_ConfirmPnAvailability(TransceiverId);
     }
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
