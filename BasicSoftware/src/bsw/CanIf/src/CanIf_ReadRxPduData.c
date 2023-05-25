

/*
**********************************************************************************************************************
* Includes
**********************************************************************************************************************
*/

#include "CanIf_Prv.h"

/**********************************************************************************************************************
**                                    Function Definition                                                            **
**********************************************************************************************************************/

/**********************************************************************************************************************
** Function name     : CanIf_ReadRxPduData                                                                           **
** Syntax            : Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId, PduInfoType* CanIfRxInfoPtr)       **
** Service ID[hex]   : 0x06                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Non Reentrant                                                                                 **
** Parameters (in)   : CanIfRxSduId                                                                                  **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : CanIfRxInfoPtr                                                                                **
** Return value      : Std_ReturnType                                                                                **
** Description       : This service provides the CAN DLC and the received data of the requested CanIfRxSduId to the  **
**                     calling upper layer.                                                                          **
**********************************************************************************************************************/

#if ((CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON) && (CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId,
        PduInfoType * CanIfRxInfoPtr)
{
    /* Pointer to HRH configuration */
    const CanIf_Cfg_Hrhtype_tst * HrhCfg_pcst;

    /* Pointer to Rx Pdu configuration */
    const CanIf_Cfg_RxPduType_tst * RxPduCfg_pcst;

    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * ControllerState_ps;

    /* Return Value of the API */
    uint8_least Status;


    /* [SWS_CANIF_00661] If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_READ_RX_PDU_DATA_SID, CANIF_E_UNINIT)

    /* Initialization */
    Status = E_NOT_OK;


    /* [SWS_CANIF_00325] If CanRxPduId is invalid, report to DET and return E_NOT_OK
     * first check of the pduid obtained is correct then check if the pdu can be buffered*/
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIfRxSduId>= CANIF_CFG_NUM_CANRXPDUIDS), \
                                                                CANIF_READ_RX_PDU_DATA_SID, CANIF_E_INVALID_RXPDUID)
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanIfRxSduId]== CANIF_INVALID_ID), \
                                                                   CANIF_READ_RX_PDU_DATA_SID, CANIF_E_INVALID_RXPDUID)

    /* Get the pointer to desired Rx pdu */
    RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanIfRxSduId];

    CANIF_DET_REPORT_ERROR_NOT_OK(((RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_NOTIFSTATUS)  || \
                                (RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_NONE)), \
                                            CANIF_READ_RX_PDU_DATA_SID, CANIF_E_INVALID_RXPDUID)

    /* [SWS_CANIF_00326] If the PduInfoPtr is null, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == CanIfRxInfoPtr), CANIF_READ_RX_PDU_DATA_SID, CANIF_E_PARAM_POINTER)

    /* Initialize pointer to HRH configuration structure */
    HrhCfg_pcst = (CanIf_Prv_ConfigSet_tpst->HrhConfig_pcst) + RxPduCfg_pcst->Hrhref_t;


    /* Initialize pointer to Controller State */
    #if (CANIF_RB_NODE_CALIBRATION == STD_ON)
    ControllerState_ps = CanIf_Prv_ControllerState_ast + (CanIf_CanCtrl_CanIfCtrl_MappingTable_acst[HrhCfg_pcst->getRxNode()]);
    #else
    ControllerState_ps = CanIf_Prv_ControllerState_ast + (HrhCfg_pcst->ControllerId_u8);
    #endif




    /* [SWS_CANIF_00324] Check whether the data can be read, the controller mode and the channel mode */
    if(((uint8)CANIF_CS_STARTED == (((ControllerState_ps->Ctrl_Pdu_mode)& CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT)) && (((uint8)CANIF_ONLINE == ((ControllerState_ps->Ctrl_Pdu_mode)& CANIF_BIT_MASK_CTRL_MODE)) ||
                ((uint8)CANIF_TX_OFFLINE == ((ControllerState_ps->Ctrl_Pdu_mode)& CANIF_BIT_MASK_CTRL_MODE))))

    {
        /* Read the data from the Rx buffer */
        CanIf_Prv_ReadRxBuffer(CanIfRxInfoPtr, RxPduCfg_pcst);

        /*Request for L-SDU data has been accepted */
        Status = E_OK;
    }

    return((Std_ReturnType)Status);
}  /* End of CanIf_ReadRxPduData() */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif


