

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"
/**********************************************************************************************************************
** Function name     : CanIf_SetBaudrate                                                                             **
** Syntax            : Std_ReturnType CanIf_SetBaudrate(uint8 ControllerId, const uint16 BaudRateConfigID)           **
** Service ID[hex]   : 0x27                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant for different ControllerIds. Non reentrant for the same ControllerId.               **
** Parameters (in)   : ControllerId, BaudRateConfigID                                                                **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : This service shall set the baud rate configuration of the CAN controller. Depending on        **
**                     necessary baud rate modifications the controller might have to reset.                         **
**********************************************************************************************************************/
#if (CANIF_SET_BAUDRATE_API == STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

Std_ReturnType CanIf_SetBaudrate(uint8 ControllerId,
        const uint16 BaudRateConfigID)
{

    /* Variable to store the CanStatus returned by Can_SetBaudrate() */
    Std_ReturnType retVal_en = E_NOT_OK;

    /*local pointer to ROM structure of controller */
    const CanIf_Cfg_CtrlConfig_tst * lCtrlConfig_pst;

#if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
    /*local pointer to ROM structure of controller Drv */
    const CanIf_CtrlDrvCfgStruct * lCtrlDrvConfig_pst;
#endif
    /*Local variable to hold the custId for controller */
    uint8 lCtrlCustId_u8;

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_SETBAUDRATE_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_SETBAUDRATE_SID, CANIF_E_PARAM_CONTROLLERID)



    /* Store custId for the requested controller for the selected variant */
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_SETBAUDRATE_SID, CANIF_E_PARAM_CONTROLLERID)

    /* Get configured data of requested controller */
    lCtrlConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + lCtrlCustId_u8;

#if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
    lCtrlDrvConfig_pst = CanIf_Prv_ConfigSet_tpst->CanCtrlDrvCont_Ptr+ (lCtrlConfig_pst->CanDrvIndx);
    /* Return E_NOT_OK when CanDriver does not support baudrate feature*/
   CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlDrvConfig_pst->MultiDrvBln == FALSE), CANIF_SETBAUDRATE_SID, CANIF_E_PARAM_CONTROLLERID)

    /* Call Can_SetBaudrate API */
    if (lCtrlDrvConfig_pst->SetBaudRate!=NULL_PTR)
    {
        retVal_en = lCtrlDrvConfig_pst->SetBaudRate(lCtrlConfig_pst->CtrlCanCtrlRef,BaudRateConfigID);
    }
    else
    {
        retVal_en= E_NOT_OK;
    }
#else
    retVal_en = Can_SetBaudrate((uint8)(lCtrlConfig_pst->CtrlCanCtrlRef), BaudRateConfigID);
#endif

    return(retVal_en);

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

