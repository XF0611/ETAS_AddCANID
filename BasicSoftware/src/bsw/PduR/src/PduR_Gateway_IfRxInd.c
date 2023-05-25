
#include "PduR_Prv.h"
#if defined(PDUR_IFGATEWAY_SUPPORT) && (PDUR_IFGATEWAY_SUPPORT != STD_OFF)
#include "PduR_Gw.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_GF_If_RxIndication_Func
 *    Performs signature translation between RxIndication() and Transmit() during interface-to-interface gatewaying.
 *    The signature needs to be compatible with <Up>_RxIndication
 *
 * \param           PduIdType Id    : ID of gateway PDU that has been received.
 *                  const uint8 *ptr: Pointer to rx SDU (buffer of received payload)
 *
 * \retval          None
 * \seealso         PDUR195
 * \usedresources
 **************************************************************************************************
 */

void PduR_GF_If_RxIndication_Func( PduIdType gwayId, const PduInfoType * data)
{
    const PduR_GT_IfTx * gt = PDUR_GW_IF_TX_BASE + gwayId;

    /* Transmit function may point to one of:
     * - PduR_GwMcTx_Transmit_Func() for multicast
     * - Lower layer, for DD non-FIFO transmission.
     * - PduR_GF_DDIf_FIFO_Transmit_Func() for DD-FIFO gateway.
     * - PduR_GF_TTIf_FIFO_Transmit_Func() for TT-FIFO gateway.
     * - PduR_GF_TTIf_Buf_Transmit_Func() for single-buffer TT gateway. */
    /*Ignore the result of this function call. */
    (void)PduR_loTransmitTable[(gt->transmitID)].PduR_loTransmitFunc(gt->loId, data);
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif /* PDUR_IFGATEWAY_SUPPORT */

