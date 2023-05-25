


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"

#if (CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

void CanIf_SetDynamicTxId(PduIdType CanTxPduId, Can_IdType CanId)
{
#if (CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
     /* Pointer to Tx Pdu configuration */
    const CanIf_Cfg_TxPduConfig_tst * lTxPduConfig_pst;
    uint16 ltxPduCustId_t;
    Can_IdType * DynIdPtr_Temp;



#if CANIF_PUBLIC_DEV_ERROR_DETECT != STD_OFF
    /* Local variable to store two MSB's */
     uint8 canifMSBbits_u8;
#endif

    /* If CAN Interface is uninitialized, report to DET and return */
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_SET_DYNAMIC_TXID_SID, CANIF_E_UNINIT)

     /* If CanTxPduId is invalid, report to DET and return  */
     CANIF_DET_REPORT_ERROR_VOID_RET(CanTxPduId>=CANIF_TOTAL_TXPDUS, CANIF_SET_DYNAMIC_TXID_SID,CANIF_E_INVALID_TXPDUID)

    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanTxPduId];

    /* If CanTxPduId is invalid, report to DET and return  */
    CANIF_DET_REPORT_ERROR_VOID_RET((ltxPduCustId_t == 0xFFFFu), CANIF_SET_DYNAMIC_TXID_SID,CANIF_E_INVALID_TXPDUID)

    lTxPduConfig_pst = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;

    /* If PDU is of type STATIC(CanTxPduId is invalid), report to DET and return */
    CANIF_DET_REPORT_ERROR_VOID_RET((CANIF_STATIC == (lTxPduConfig_pst->TxPduType)), CANIF_SET_DYNAMIC_TXID_SID,CANIF_E_INVALID_TXPDUID)

    DynIdPtr_Temp = CanIf_DynTxPduCanId_au32;


#if CANIF_PUBLIC_DEV_ERROR_DETECT != STD_OFF
    /*shifting the canid bits to access two MSB's */
    canifMSBbits_u8= (uint8)(CanId>>30u);

    /* If CanId is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_VOID_RET((((canifMSBbits_u8 == CANIF_ZERO)|| \
            (canifMSBbits_u8 == CANIF_ONE)) && ((CanId & CANIF_MSB_BIT_RESET) > CANIF_CAN_STD_VAL)),
            CANIF_SET_DYNAMIC_TXID_SID, CANIF_E_PARAM_CANID)

    CANIF_DET_REPORT_ERROR_VOID_RET((((canifMSBbits_u8 == CANIF_TWO)||(canifMSBbits_u8 == CANIF_THREE)) && \
            ((CanId & CANIF_MSB_BIT_RESET)  > CANIF_CAN_XTD_VAL)), CANIF_SET_DYNAMIC_TXID_SID, CANIF_E_PARAM_CANID)
#endif

    /* Check if the CanTxPduId is configured as Dynamic */
    if(lTxPduConfig_pst->TxPduType != CANIF_STATIC)
    {

        /* Update the new CanId in the global array */
        *(DynIdPtr_Temp + (lTxPduConfig_pst->TxPduType)) = CanId;

    }
#endif
} /* End of CanIf_SetDynamicTxId() */
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
