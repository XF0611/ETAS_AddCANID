
/* Provides a single buffer for gatewaying to an IF layer which uses trigger transmit */

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
 * PduR_GF_TTIf_Buf_Transmit_Func - Provides a single buffer for gatewaying to an IF layer which uses trigger transmit
 *                  Forward the PDU to the configured lower layer
 *
 * \param           PduIdType gwayId - Gateway ID
 *                  const PduInfoType * info - pointer to tx buffer
 *
 *
 * \retval          Std_ReturnType - E_OK
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
/* The signature needs to be compatible with <Lo>_Transmit */
Std_ReturnType PduR_GF_TTIf_Buf_Transmit_Func(PduIdType gwayId,
                                                                 const PduInfoType * info)
{
    const PduR_GT_If * gt;
    PduR_Buf_If_dynamic   *dyn;
    PduInfoType * infoBuf;
    Std_ReturnType rtn;


    uint16 i;
    gt      = PDUR_GW_IF_BASE + gwayId;
    /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.
     * Hence Supressed.*/
    dyn     = (PduR_Buf_If_dynamic *) gt->buffer;
    infoBuf = dyn->gwBuf;
    rtn = E_NOT_OK;

    /*This should be NoNest critical section*/
    SchM_Enter_PduR_LockIFGatewayTriggerTx(); /* enter critical area */
    /*check if sdulength is greater than conf length*/
    if(dyn->Tx_Status == PDUR_TX_IDLE)
    {
        if(info->SduLength > dyn->confLen)
        {
            infoBuf->SduLength = dyn->confLen;
        }
        else
        {
            infoBuf->SduLength = info->SduLength;
        }

        /* Store the PDU in the buffer, overwrite the current contents if necessary. */
        for(i = 0 ; i < infoBuf->SduLength ; i++)
        {
            *((infoBuf->SduDataPtr)+i) = *((info->SduDataPtr)+i);
        }

        /* Forward the PDU to the configured lower layer. */
        /* Ignore the result of this function call. */
        (void)PduR_loTransmitTable[(gt->transmitID)].PduR_loTransmitFunc( gt->loId, infoBuf );

        dyn->Tx_Status = PDUR_TX_INPROGRESS;
        rtn = E_OK;
    }
    else
    {
        rtn = E_NOT_OK;
    }

    SchM_Exit_PduR_LockIFGatewayTriggerTx(); /* exit critical area */

    /* This return code will always be discarded by one of our callers. */
    return rtn;
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif /* PDUR_IFGATEWAY_SUPPORT */

