
#include "PduR_Prv.h"
#if defined(PDUR_TPGATEWAY_SUPPORT) && (PDUR_TPGATEWAY_SUPPORT != STD_OFF)
#include "PduR_Gw.h"

#ifdef PDUR_BUFFERPOOL_SUPPORT
#include "PduR_BufferPool.h"
#endif /* PDUR_BUFFERPOOL_SUPPORT */

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_GF_Tp_TxConfirmation_Func - This function is called by a transport protocol module after the I-PDU has been transmitted
 *                                    on its network, the result will reveal if the transmission was successful or not.
 *
 * \param           PduIdType gwId            - Identification of the transmitted I-PDU.
 *                  Std_ReturnType result    - Result of the transmission of the I-PDU.
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

 void PduR_GF_Tp_TxConfirmation_Func( PduIdType gwId, Std_ReturnType result)
{
    /* Start: Variable declaration */
    PduR_GT_TpPtr      gt;
    /* Fetch the fixed Buffer details */
    const PduR_FIFO_Tp_fixed * fifo;
    /* Fetch the RAM structure details  */
    PduR_TpSessDynPtr  SessionPtr;
    PduR_TpConnDynPtr ConnectionPtr;
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    gt = PDUR_GW_TP_BASE+gwId;
    fifo = (gt->buffer);
    ConnectionPtr = fifo->connectionTable;
    SessionPtr = ConnectionPtr->primarySession;
    /* End: Variable Initialization */

    /* Check if the RX and TX status of the Session Ptr , it shouldn't be IDLE during TxConfirmation */
    if ((SessionPtr->Rx_Status == PDUR_SESSION_IDLE) || (SessionPtr->Tx_Status == PDUR_TX_IDLE))
    {
        return;
    }

    if(gt->numOfLowerLayers == 1u)
    {

        /* FC_VariationPoint_START */
#ifdef PDUR_QUEUEING_SUPPORT
        /* Check the value of activeSessionCount, at this stage it shouldn't be 0 */
        if ( ConnectionPtr->activeSessionCount != 0)
        {
            /* Close the session */
            ConnectionPtr->activeSessionCount--;
            /* Transmission on primary session finished */
            /* Clear the session states */
            PduR_ResetPduR_FIFO_Tp_dynamic_Structure(SessionPtr, gt);
        }
        else
        {
            /* active session count is zero, so return!!*/
            return;
        }
        /* After closing this session, if another active session is found , then switch the sessions */
        if(ConnectionPtr->activeSessionCount == 1)
        {
            /* The present secondary session becomes the primary session */
            ConnectionPtr->primarySession =  ConnectionPtr->secondarySession;
            /* Just closed session, becomes secondary session */
            ConnectionPtr->secondarySession = SessionPtr;

            /* Check for pending operations on the new primary session */
            SessionPtr = ConnectionPtr->primarySession;

            if((SessionPtr->Tx_Status == PDUR_TX_IDLE) && (SessionPtr->numRxLength_auo[SessionPtr->Tx_FifoInstanceUsed_u8] == SessionPtr->SduLength_auo[SessionPtr->Tx_FifoInstanceUsed_u8]))
            {
                PduInfoType readerPduInfo;
                Std_ReturnType Ret_val;

                /* Ensure that the reader's PduInfo object is initialized */
                readerPduInfo.SduDataPtr = SessionPtr->readPtr;
                /* 1st time call, the complete length should be transmitted to lower layer when TpTransmit is called */
                readerPduInfo.SduLength  = SessionPtr->SduLength_auo[SessionPtr->Tx_FifoInstanceUsed_u8];

                /* Change the TxStatus to in-progress and then invoke <LoTp>_Transmit function */
                SessionPtr->Tx_Status = PDUR_TX_INPROGRESS;

                /* Call DestLoTp transmit once the TpThreshold is reached*/
                Ret_val = PduR_loTransmitTable[(gt->loTransmitID)].PduR_loTransmitFunc(gt->loId, &readerPduInfo);

                /* check the return value of the LoTp Transmit API */
                if(Ret_val != E_OK)
                {
                    /* If Dest LoTP Transmit function returns E_NOT_OK, immediately cancel the reception*/
                    ConnectionPtr->activeSessionCount--;
                    /* Clear the session states */
                    PduR_ResetPduR_FIFO_Tp_dynamic_Structure(SessionPtr, gt);
                }
            }

        }
        if(ConnectionPtr->activeSessionCount == 0)
        {
             /* Release the acquired buffers . The below function will release buffer only if buffer pool is used */
            PduR_ReleaseTpBuffer(gt);
        }
#else /* #ifdef PDUR_QUEUEING_SUPPORT */
        /* FC_VariationPoint_END */
#ifdef PDUR_FIFO_TP_SUPPORT
        if(gt->TpQueueDepth_u8 > 1u)
        {
            boolean BreakFlag_b;
            SessionPtr->numTxLength = 0u;
            SessionPtr->Tx_FifoInstanceUsed_u8++;
            BreakFlag_b= TRUE;
            while(BreakFlag_b)
            {
                if(SessionPtr->Tx_FifoInstanceUsed_u8 == gt->TpQueueDepth_u8)
                {
                    /* Reset the Tx_FifoInstanceUsed_u8 */
                    SessionPtr->Tx_FifoInstanceUsed_u8 = 0u;
                    /* Move the ReadPtr to beginning address of the buffer */
                    SessionPtr->readPtr = fifo->begin;
                    SessionPtr->Tx_Status = PDUR_TX_IDLE;
                }
                /* Check if FiFo has data*/
                if(SessionPtr->Tx_FifoInstanceUsed_u8 != SessionPtr->Rx_FifoInstanceUsed_u8)
                {
                    /* Declaration of local variable */
                    PduInfoType readerPduInfo;
                    Std_ReturnType Ret_val;
                    /* Holds the length of the buffer to move the readptr */
                     PduLengthType TxInstanceLength_l;
                    /* Before calling next transmission, set the Tx_Status to IDLE */
                    SessionPtr->Tx_Status = PDUR_TX_IDLE;

                    SessionPtr->readPtr = fifo->begin;
                    TxInstanceLength_l = (PduLengthType)((SessionPtr->Tx_FifoInstanceUsed_u8)*(SessionPtr->bufferLength));
                    /* Move the readPtr to next instance of the buffer */
                    SessionPtr->readPtr += TxInstanceLength_l;


                    /* Ensure that the reader's PduInfo object is initialized */
                    readerPduInfo.SduDataPtr = SessionPtr->readPtr;
                    /* The fifo instance's length should be transmitted to lower layer when TpTransmit is called */
                    readerPduInfo.SduLength  = SessionPtr->SduLength_auo[SessionPtr->Tx_FifoInstanceUsed_u8];

                    /* Call the transmit function to trigger the transmission */
                    Ret_val = PduR_loTransmitTable[(gt->loTransmitID)].PduR_loTransmitFunc(gt->loId, &readerPduInfo);

                    /* With return value E_OK, Set the Tx_Status to TX_INPROGRESS and move the readPtr to next instance of the buffer */
                    if(Ret_val == E_OK)
                    {
                        SessionPtr->Tx_Status = PDUR_TX_INPROGRESS;
                        BreakFlag_b = FALSE;
                    }
                    else
                    {
                        /*[SWS_PduR_00788]*/
                        SessionPtr->numRxLength_auo[SessionPtr->Tx_FifoInstanceUsed_u8] = 0u;
                        SessionPtr->SduLength_auo[SessionPtr->Tx_FifoInstanceUsed_u8] = 0u;
                        /* Returns E_NOT_OK, then I-PDU shall be removed from the FIFO and the next FIFO entry shall be transmitted, if available */
                        /* Move to next instance */
                        SessionPtr->Tx_FifoInstanceUsed_u8++;

                        /*[SWS_PDUR_00807, SWS_PduR_00806]:
                         * When <DstLo>_Transmit() returns E_NOT_OK for a routing path using a FIFO,
                         * the PDU Router shall report PDUR_E_PDU_INSTANCES_LOST to the DET module.
                         */
#if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)
                    PDUR_REPORT_ERROR(PDUR_SID_GATEWAYTPTXCONFIRMATION,PDUR_E_PDU_INSTANCES_LOST);
#endif
                    }
                }
                /* Rx_Status is required to check, otherwise for RxFifoUsed == TxFifoUsed, reset happens, but in between CopyRxData has received, then reset will lead to data lost */
                else if (SessionPtr->Rx_Status != PDUR_RX_INPROGRESS)
                {
                    /* Reset if all the received data is transmitted */
                    PduR_ResetPduR_FIFO_Tp_dynamic_Structure(SessionPtr, gt);
                    /*There is no data available in FiFo, To stop next transmission set breakflag to False */
                    BreakFlag_b = FALSE;
                }
                /* RxFifoUsed == TxFifoUsed && SessionPtr->Rx_Status == PDUR_RX_INPROGRESS */
                else
                {
                    /* Set TxStatus to IDLE to trigger next transmission, when RxFifo = TxFifo and when RxStatus = IN_PROGRESS */
                    SessionPtr->Tx_Status = PDUR_TX_IDLE;
                    /*There is no data available further, To stop next transmission set breakflag to False */
                    BreakFlag_b = FALSE;
                }
            }
        }
        else
        {
               /* FC_VariationPoint_END */
            /* Clear the session states */
            PduR_ResetPduR_FIFO_Tp_dynamic_Structure(SessionPtr, gt);
            /* FC_VariationPoint_START */
        }
#else
        /* Last TxConfirmation received *
         * Release the acquired buffers . The below function will release buffer only if buffer pool is used */
        PduR_ReleaseTpBuffer(gt);
        /* Clear the session states */
        PduR_ResetPduR_FIFO_Tp_dynamic_Structure(SessionPtr, gt);
#endif /* #ifdef PDUR_FIFO_TP_SUPPORT */
#endif
    }
    else
    {
        /* Wait till last TxConfirmation*/
        if(SessionPtr->Tx_E_OK_Count != 0)
        {
           SessionPtr->Tx_E_OK_Count--;
        }
        if(SessionPtr->Tx_E_OK_Count == 0)
        {

#ifdef PDUR_QUEUEING_SUPPORT
            /* Close the session and decremenent activeSessionCount */
            ConnectionPtr->activeSessionCount--;
#endif
             /* Last TxConfirmation received *
             * Release the acquired buffers . The below function will release buffer only if buffer pool is used */
            PduR_ReleaseTpBuffer(gt);
            /* Clear the session states */
            PduR_ResetPduR_FIFO_Tp_dynamic_Structure(SessionPtr, gt);
        }
     }
    (void) result;
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif /* PDUR_TPGATEWAY_SUPPORT */

