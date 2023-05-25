


#include "PduR_Prv.h"
#if defined(PDUR_IFGATEWAY_SUPPORT) && (PDUR_IFGATEWAY_SUPPORT != STD_OFF)
#include "PduR_Gw.h"
#include "PduR.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_GF_TTIf_Buf_TriggerTransmit_Func - Handle gatewaying between If layers where the destination layer
 *      uses Trigger Transmit data provision, and is configured to use  a FIFO
 *      Note that PDUR254 specifies that an item of state called TxIdx shall be
 *      associated with each FIFO. This is logically equivalent to the readPtr which
 *      we maintain for the FIFO anyway and so is not supplied separately.
 *
 *
 * \param           PduIdType gwayId - GatewayID that is requested to be transmitted.
 *                  uint8 *  dataPtr - Pointer to place inside the transmit buffer of the L-PDU where data shall be
 *                                     copied to.
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
/* The signature needs to be compatible with <Up>_TriggerTransmit */
Std_ReturnType PduR_GF_TTIf_FIFO_TriggerTransmit_Func( PduIdType gwayId,
                                                                PduInfoType * dataPtr)
{
    const PduR_GT_If * gt = PDUR_GW_IF_BASE + gwayId;
    /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.
     * Hence Supressed.*/
    PduR_FifoType         fifo = (PduR_FifoType)(gt->buffer);
    PduR_FifoDynIfPtr     dyn  = (PduR_FifoDynIfPtr)(fifo->dyn);
    Std_ReturnType        retVal = E_NOT_OK;


    uint16 i;

    SchM_Enter_PduR_LockIFGatewayTriggerTxNoNest(); /* enter the critical area */

    /* [SWS_PduR_00662] : If FIFO has entry then copy the data into destination buffer,
     * If FIFO is empty then return E_NOT_OK*/
    /*  [SWS_PduR_00819] : Check the lower layer buffer size,
     *  if lower layer buffer size is less than the stored Pdu Data length then return E_NOT_OK
     *  Stored PduData Length <=  lowerLayer Buffer Length */
    if((dyn->actLen[dyn->readIndex] > 0u) && (dyn->actLen[dyn->readIndex] <=  dataPtr->SduLength))
    {

         /* copy the data into the buffer */

        for(i = 0 ; i < dyn->actLen[dyn->readIndex] ; i++)
        {
            *((dataPtr->SduDataPtr)+i) = *((dyn->readPtr)+i);
         }

        /* Copy actual Length to SduLength */
        dataPtr->SduLength = dyn->actLen[dyn->readIndex];
        retVal = E_OK;
    }

    SchM_Exit_PduR_LockIFGatewayTriggerTxNoNest();  /* exit the critical area */
    return retVal;
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"


#endif /* PDUR_IFGATEWAY_SUPPORT */

