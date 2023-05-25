
/* StartOfReception, CopyRxData and RxIndication to support gatewaying between TP layers, when using a FIFO */

#include "PduR_Prv.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */
#ifdef PDUR_RPG_FOR_RXTP_SUPPORT

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_RPG_Tp_StartOfReception_Func - Handle StartOfReception for TP reception when the dest PDU is referred under RPG
 *
 * \param           PduIdType rpgId  :ID of N-PDU that has been received
 *                  PduLengthType  len:This length identifies the overall number of bytes to be received.
 *                  PduR_TpRxPduInfoPtrPtr pduInfoPtr:  Pointer to pointer to PduInfoStructure containing SDU data
 *                  pointer and SDU length of a receive buffer.
 *
 * \retval          BufReq_ReturnType r
 *                  BUFREQ_OK - Buffer request accomplished successful.
 *                  BUFREQ_E_OVFL - Receiver is not able to receive number of TpSduLength bytes; no buffer provided.
 *                  BUFREQ_E_NOT_OK - Buffer request not successful, no buffer provided.
 * \seealso
 * \usedresources   PDUR172
 **************************************************************************************************
 */

/* MR12 RULE 8.13 VIOLATION: A pointer parameter in a function prototype should be declared as pointer to const if the pointer is not used to modify the addressed object.
 But AUTOSAR SWS specification signature is as below, Hence suppressed */
BufReq_ReturnType PduR_RPG_Tp_StartOfReception_Func(PduIdType rpgId, const PduInfoType* info,PduLengthType TpSduLength, PduLengthType* bufferSizePtr)
{
    /* Start: Variable declaration */
    PduR_RPG_TpRxToUpPtr rpgt;
    BufReq_ReturnType rtn;
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    rpgt = PDUR_RX_TP_RPG_BASE+rpgId;
    rtn = BUFREQ_E_NOT_OK;
    /* End: Variable Initialization */

    /* Below check is to perform routing if it is configured and enabled. */
    if (PDUR_RPG_ENROUTING_INFO(rpgt->routingCntrl_Idx) != FALSE )   /* Routing is enabled or not */
    {
       rtn = PduR_upTpStartOfReceptionTable[(rpgt->loTpRxToUp.upStartOfReceptionID)].PduR_upTpStartOfReceptionFunc(rpgt->loTpRxToUp.upId,info, TpSduLength, bufferSizePtr);
    }
    return rtn;
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_RPG_Tp_CopyRxData_Func - Handle CopyRxData for TP reception when the dest PDU is referred under RPG
 *
 * \param           PduIdType rpgId  :ID of N-PDU that has been received
 *                  PduLengthType  len:This length identifies the overall number of bytes to be received.
 *                  PduR_TpRxPduInfoPtrPtr pduInfoPtr:  Pointer to pointer to PduInfoStructure containing SDU data
 *                  pointer and SDU length of a receive buffer.
 *
 * \retval          BufReq_ReturnType r
 *                  BUFREQ_OK - Buffer request accomplished successful.
 *                  BUFREQ_E_OVFL - Receiver is not able to receive number of TpSduLength bytes; no buffer provided.
 *                  BUFREQ_E_NOT_OK - Buffer request not successful, no buffer provided.
 * \seealso
 * \usedresources   PDUR172
 **************************************************************************************************
 */

/* MR12 RULE 8.13 VIOLATION: A pointer parameter in a function prototype should be declared as pointer to const if the pointer is not used to modify the addressed object.
 But AUTOSAR SWS specification signature is as below, Hence suppressed */
BufReq_ReturnType PduR_RPG_Tp_CopyRxData_Func( PduIdType rpgId, const PduInfoType* info, PduLengthType* bufferSizePtr)
{
    /* Start: Variable declaration */
    PduR_RPG_TpRxToUpPtr rpgt;
    BufReq_ReturnType rtn;
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    rpgt = PDUR_RX_TP_RPG_BASE+rpgId;
    rtn = BUFREQ_E_NOT_OK;
    /* End: Variable Initialization */

    /* Below check is to perform routing if it is configured and enabled. */
    if (PDUR_RPG_ENROUTING_INFO(rpgt->routingCntrl_Idx) != FALSE )   /* Routing is enabled or not */
    {
       rtn = PduR_upTpCopyRxDataTable[(rpgt->loTpRxToUp.upProvideRxBufID)].PduR_upTpCopyRxDataFunc(rpgt->loTpRxToUp.upId,info, bufferSizePtr);
    }
    return rtn;
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_RPG_Tp_RxIndication_Func - Called by the transport protocol module after an I-PDU has been received successfully or
 *                                   when an error occurred. It is also used to confirm cancellation of an I-PDU.
 *
 * \param           PduIdType rpgId              :ID of  N-PDU that has been received
 *                  Std_ReturnType  result     : Result of the reception.
 *                  E_OK in case TP reception completed successfully
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

void PduR_RPG_Tp_RxIndication_Func( PduIdType rpgId, Std_ReturnType result)
{
    /* Start: Variable declaration */
    PduR_RPG_TpRxToUpPtr rpgt;
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    rpgt = PDUR_RX_TP_RPG_BASE+rpgId;
    /* End: Variable Initialization */

    /* Tp RxIndication shouldnt be controlled using RPG. So calling directly to upper layer.*/
    /*As per note mentioned for req SWS_PDUR_00805*/
    PduR_upTpRxIndicationTable[(rpgt->loTpRxToUp.upRxIndicationID)].PduR_upTpRxIndicationFunc(rpgt->loTpRxToUp.upId,result);
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif /*PDUR_RPG_FOR_RXTP_SUPPORT*/
