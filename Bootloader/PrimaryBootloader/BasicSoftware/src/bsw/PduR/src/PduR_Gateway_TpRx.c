
/* StartOfReception, CopyRxData and RxIndication to support gatewaying between TP layers, when using a FIFO */

#include "PduR_Prv.h"
#include "Dem.h"             /* For Dem_ReportErrorStatus */
#if(PDUR_ENABLE_INTER_MODULE_CHECKS)
#if (!defined(DEM_AR_RELEASE_MAJOR_VERSION) || (DEM_AR_RELEASE_MAJOR_VERSION != PDUR_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(DEM_AR_RELEASE_MINOR_VERSION) || (DEM_AR_RELEASE_MINOR_VERSION != PDUR_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif /* #if(PDUR_ENABLE_INTER_MODULE_CHECKS) */


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
 * PduR_GF_Tp_StartOfReception_Func - Handle StartOfReception for TP gatewaying, for those cases where a
 *                                    FIFO buffer is in use
 *
 * Note that the v2 PDUR specification:
 * - does not appear to allow a FIFO to be configured for store-and-forward TP
 * gatewaying (i.e. for concurrent operation of a TP gateway).
 * - does not specify that a FIFO must be provided for
 * store-and-forward TP gatewaying. Therefore, although our implementation allows a FIFO to be used
 * for store-and- forward TP gatewaying, this cannot actually be configured and will never occur
 * Store-and-forward gatewaying will always use a single buffer.
 * A FIFO will only be used for routing-on-the-fly, when the multiple chunks of a PDU must be stored
 * separately (i.e. each in its own FIFO entry).
 *
 * \param           PduIdType gwId  :ID of N-PDU that has been received
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
BufReq_ReturnType PduR_GF_Tp_StartOfReception_Func(PduIdType gwId, const PduInfoType* info,PduLengthType TpSduLength, PduLengthType* bufferSizePtr)
{

    /* Start: Variable declaration */
    PduR_GT_TpPtr gt;
    const PduR_FIFO_Tp_fixed * fifo;
    BufReq_ReturnType rtn;
    PduR_TpSessDynPtr SessionPtr;
    PduR_TpConnDynPtr ConnectionPtr;
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    gt = PDUR_GW_TP_BASE+gwId;
    fifo = (gt->buffer);
    rtn = BUFREQ_OK;
    ConnectionPtr = fifo->connectionTable;
    /* Working on primary session by default*/
    SessionPtr = ConnectionPtr->primarySession;
    /* End: Variable Initialization */


    /* FC_VariationPoint_START */
#ifdef PDUR_QUEUEING_SUPPORT
    if (ConnectionPtr->activeSessionCount == 0)
    {
        /* The below function handles requesting two buffers in case of queuing is enabled */
        rtn = PduR_GetTpBuffer(gt,TpSduLength);
        if (rtn != BUFREQ_OK)
        {
            return rtn;
        }

    }
    /* One session is already in progress */
    else if((ConnectionPtr->activeSessionCount == 1) && (gt->TpGwQueueSize == 2u))

    {
        /* Second reception has to be accepted, only if the first reception is completed
         * There is no need to request the buffer here, as it is already requested
         * Actually the check for Reception finished on primary session not required here, as TP itself wouldnt accept
         * the second request on the same handle
         * If there is no queue configured for the connection , then there is no point in considering the backup
         * session
         * If total PDU length of second SOR is more than allocated buffer, return with buffer overflow. */
        if(SessionPtr->Rx_Status != PDUR_RX_FINISHED)
        {
            return BUFREQ_E_NOT_OK;
        }
        if(TpSduLength > fifo->connectionTable->secondarySession->bufferLength)
        {
#ifdef PDUR_STORE_AND_FORWARD
            return BUFREQ_OK;
#else
            return BUFREQ_E_OVFL;
#endif
        }
            /* Working on secondary session now */
            SessionPtr = ConnectionPtr->secondarySession;
    }
    else
    {
        /* More than 2 reception requests will not be handled, hence E_NOT_OK is returned to the TP
         * Or if the first session's reception was not completed yet */
        return BUFREQ_E_NOT_OK;
    }
    /* Increment active session count as a new session is acquired */
    ConnectionPtr->activeSessionCount++;

#else
    /* FC_VariationPoint_END */
    /* Primary condition to accept, Rx_Status == PDUR_SESSION_IDLE else check if FIFO is enabled */
    if (SessionPtr->Rx_Status != PDUR_SESSION_IDLE)
    {
#ifdef PDUR_FIFO_TP_SUPPORT
        if (gt->TpQueueDepth_u8 > 1u)
            {
            /* In case of FIFO enabled, check if there are any free instance where the new reception can be accommodated */
                if(!((SessionPtr->Rx_Status == PDUR_RX_FINISHED) &&
                    (SessionPtr->Rx_FifoInstanceUsed_u8 != SessionPtr->Tx_FifoInstanceUsed_u8)))
                {
                    /* No instances of the FIFO are free, hence reject this session request */
                    return BUFREQ_E_NOT_OK;
                }
            }
        else
#endif /* #ifdef PDUR_FIFO_TP_SUPPORT */
        {
            /* Reject this request as one instance of the reception is in-progress */
            return BUFREQ_E_NOT_OK;
        }
    }
    rtn = PduR_GetTpBuffer(gt,TpSduLength);
    /* Check if requested buffer is successfully allocated */
    if (rtn != BUFREQ_OK)
    {
        return rtn;
    }
    /* FC_VariationPoint_START */
#endif /* #ifdef PDUR_QUEUEING_SUPPORT */
    /* FC_VariationPoint_END */
    /* Buffer is allocated, initialize the session pointer variables */

#ifdef PDUR_FIFO_TP_SUPPORT
    /* Incase of FIFO, only update the readPtr and writePtr for the first instance(i.e First StartOfReception call) */
    if (SessionPtr->Rx_Status == PDUR_SESSION_IDLE)
#endif
    {
       /* Update read ptr and write ptr to the buffer pointer */
        SessionPtr->readPtr = SessionPtr->bufferPtr;
        SessionPtr->writePtr = SessionPtr->bufferPtr;
    }
    /* Update the alloted buffer size to buffer size pointer  */
    *bufferSizePtr = SessionPtr->bufferLength;
    /* Store the Complete Message Length Received */
    SessionPtr->SduLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] = TpSduLength;

#ifdef PDUR_STORE_AND_FORWARD
            SessionPtr->remainingBuffLength = SessionPtr->bufferLength;
#endif
#if defined(PDUR_MULTICAST_FROM_TP_SUPPORT) && (PDUR_MULTICAST_FROM_TP_SUPPORT != 0)
    {
       /* Check if Info pointer is not null
	   Stored the info fwd to the upper Layer*/
       if( info != NULL_PTR)
       {
           /* Store valid buffer address */
           SessionPtr->infoPtr = info;
       }
       else
       {
           /* Assign NuLL_PTR */
           SessionPtr->infoPtr = NULL_PTR;
       }

    }
#else
    {
        /* This PduInfoType info is not used, because the same pointer maybe provided in the first CopyRxData call
             * Hence ignored  */
         (void) info ;
    }
#endif
    /* Set a flag indicating that the reception request has been accepted */
    SessionPtr->Rx_Status = PDUR_RX_INPROGRESS;
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
 * PduR_GF_Tp_CopyRxData_Func - Handle CopyRxData for TP gatewaying, for those cases where a
 *                                    FIFO buffer is in use
 *
 * Note that the v2 PDUR specification:
 * - does not appear to allow a FIFO to be configured for store-and-forward TP
 * gatewaying (i.e. for concurrent operation of a TP gateway).
 * - does not specify that a FIFO must be provided for
 * store-and-forward TP gatewaying. Therefore, although our implementation allows a FIFO to be used
 * for store-and- forward TP gatewaying, this cannot actually be configured and will never occur
 * Store-and-forward gatewaying will always use a single buffer.
 * A FIFO will only be used for routing-on-the-fly, when the multiple chunks of a PDU must be stored
 * separately (i.e. each in its own FIFO entry).
 *
 * \param           PduIdType gwId  :ID of N-PDU that has been received
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
BufReq_ReturnType PduR_GF_Tp_CopyRxData_Func( PduIdType gwId, const PduInfoType* info, PduLengthType* bufferSizePtr)
{
    /* Start: Variable declaration */
    PduR_GT_TpPtr gt;
    const PduR_FIFO_Tp_fixed * fifo;
    PduR_TpSessDynPtr SessionPtr;
    PduR_TpConnDynPtr ConnectionPtr;
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    gt = PDUR_GW_TP_BASE+gwId;
    fifo = (gt->buffer);
    ConnectionPtr = fifo->connectionTable;

    /* End: Variable Initialization */

    PduR_LoadActiveSession(&SessionPtr,ConnectionPtr);

    /* This check ensures that StartOfReception API is called before CopyRxData call*/
    if ((SessionPtr == NULL_PTR) || (SessionPtr->Rx_Status != PDUR_RX_INPROGRESS) || (info->SduLength > (SessionPtr->SduLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] - SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8])))
    {
        return BUFREQ_E_NOT_OK;
    }

    if ( info->SduLength == 0u )
    {
        *bufferSizePtr = PduR_UpdateBuffSizePtr(SessionPtr);
        return BUFREQ_OK;
    }

#ifdef PDUR_STORE_AND_FORWARD

    if((SessionPtr->RxChunkLength == SessionPtr->bufferLength) && (((SessionPtr->RxChunkLength - SessionPtr->TxChunkLength) != 0u) || (SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] == SessionPtr->numTxLength)))
    {
        /* Received data length is equal to bufferLength
         * reset writePtr ,RxChunlkLength and
         * Update remainingBuffLength with transmitted data Length */
        SessionPtr->writePtr = SessionPtr->bufferPtr;
        SessionPtr->RxChunkLength = 0;
        SessionPtr->remainingBuffLength = SessionPtr->TxChunkLength;
    }

    if(info->SduLength > SessionPtr->remainingBuffLength)
    {
        /*if CopyRxData called with sduLength greater than available bufferLength buffer*/
        return BUFREQ_E_NOT_OK;
    }
    else if(SessionPtr->TxChunkLength > SessionPtr->RxChunkLength)
    {
        /* Transmited chunk Length is greter than received chunk length
         * Copy the data into the receive buffer and update the write pointer */
        rba_BswSrv_MemCopy8(SessionPtr->writePtr,info->SduDataPtr,info->SduLength);
        /* Increment the write pointer to the length of sdulength */
        SessionPtr->writePtr += info->SduLength;
        /* Update the received Chunk Length of data  */
        SessionPtr->RxChunkLength += info->SduLength;
        /* Update the received total Length of data  */
        SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] += info->SduLength;
        /* Update remaining buffer size info */
        SessionPtr->remainingBuffLength = SessionPtr->TxChunkLength - SessionPtr->RxChunkLength;
    }
    /*In case of first Time CopyRxData 1st write is going on*/
    else if((SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] == SessionPtr->numTxLength) || (SessionPtr->RxChunkLength > SessionPtr->TxChunkLength))
    {
        PduLengthType availableBuffLength_l;
        availableBuffLength_l = SessionPtr->bufferLength - SessionPtr->RxChunkLength;
        if(info->SduLength > availableBuffLength_l)
        {
            /* Total received length is equal to transmitted length or received chunk length is greater than transmitted chunk length
              * Copy the data into the receive buffer and update the write pointer */
              rba_BswSrv_MemCopy8(SessionPtr->writePtr,info->SduDataPtr,availableBuffLength_l);
              /* Reset the writePtr and transmitted chunk length */
              SessionPtr->writePtr = SessionPtr->bufferPtr;
              SessionPtr->RxChunkLength = 0;
              rba_BswSrv_MemCopy8(SessionPtr->writePtr,(info->SduDataPtr + availableBuffLength_l),(info->SduLength - availableBuffLength_l));
              SessionPtr->writePtr += (info->SduLength - availableBuffLength_l);
              SessionPtr->RxChunkLength += (info->SduLength - availableBuffLength_l);
              /* Update remaining buffer size info */
              SessionPtr->remainingBuffLength = SessionPtr->TxChunkLength - SessionPtr->RxChunkLength;
        }
        else
        {
            rba_BswSrv_MemCopy8(SessionPtr->writePtr,info->SduDataPtr,info->SduLength);
            /* Increment the write pointer to the length of sdulength */
            SessionPtr->writePtr += info->SduLength;
             /* Update the received Chunk Length of data  */
            SessionPtr->RxChunkLength += info->SduLength;
            /* Update remaining buffer size info */
            SessionPtr->remainingBuffLength = ((SessionPtr->bufferLength - SessionPtr->RxChunkLength) + SessionPtr->TxChunkLength);

        }

        /* Length of data received */
        SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] += info->SduLength;

    }
    else
    {
        /* To remove Misra warning
         * do nothing*/
    }
    /* Update remaining buffer size info to underlying TP layer */
    *bufferSizePtr = SessionPtr->remainingBuffLength;
#else
        /* Copy the data into the receive buffer and update the write pointer */
        rba_BswSrv_MemCopy8(SessionPtr->writePtr,info->SduDataPtr,info->SduLength);

        /* Increment the write pointer to the length of sdulength */
        SessionPtr->writePtr += info->SduLength;
        /* Load the Length of data received */
        SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] += info->SduLength;

        /* Update remaining buffer size info to underlying TP layer */
        *(bufferSizePtr) = SessionPtr->bufferLength - SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8];
               /* FC_VariationPoint_START */
#endif

#ifdef PDUR_QUEUEING_SUPPORT
    /* The transmission for primary session is ongoing, So do not trigger the transmission for the secondary session
     * Just copy the secondary session's data into the buffer and return */
    if(SessionPtr == ConnectionPtr->secondarySession)
    {
        return BUFREQ_OK;
    }
#endif /* PDUR_QUEUEING_SUPPORT */
    /* FC_VariationPoint_END */

    /*Unicast as well as Multicast Gateway */
    /* Check if TpTransmit API is called or not */
    /* Call LoTp transmit if received length in PduRTp buffer is more than or equal to TpThreshold
     * If TpThreshold is not configured then call LoTp Transmit when Received data length is equal to bufferlength  */
    if(!((SessionPtr->Tx_Status == PDUR_TX_IDLE) &&
            ((SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] >= gt->TpThreshold) ||
             ((gt->TpThreshold == PDUR_TPTHRESHOLD_DEFVALUE) &&
               (SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] == SessionPtr->bufferLength)))))
     {
            /* Either the LoTp Transmit is already called or
             * the PduR doesn't have enough data to trigger for the transmission */
            return BUFREQ_OK;
     }
     else
     {

        PduInfoType readerPduInfo;
        Std_ReturnType Ret_val = E_OK;

         /* Ensure that the reader's PduInfo object is initialized */
          readerPduInfo.SduDataPtr = SessionPtr->readPtr;
         /*At the first time the complete length should be transmitted to lower layer when TpTransmit is called */
          readerPduInfo.SduLength = SessionPtr->SduLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8];

         /* Change the TxStatus to in-progress and then invoke <LoTp>_Transmit function */
         SessionPtr->Tx_Status = PDUR_TX_INPROGRESS;

         /* Call DestLoTp transmit once the TpThreshold is reached*/
         Ret_val = PduR_loTransmitTable[(gt->loTransmitID)].PduR_loTransmitFunc(gt->loId, &readerPduInfo);

         /* check the return value of the LoTp Transmit API */
         if(Ret_val != E_OK)
         {
             /* Change the TxStatus back to TxIdle if <LoTp>_Transmit function returns E_NOT_OK */
             SessionPtr->Tx_Status = PDUR_TX_IDLE;

             /* If Dest LoTP Transmit function returns E_NOT_OK, immediately cancel the ongoing reception*/
              return BUFREQ_E_NOT_OK;
          }
       }

    return BUFREQ_OK;
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
 * PduR_GF_Tp_RxIndication_Func - Called by the transport protocol module after an I-PDU has been received successfully or
 *                                   when an error occurred. It is also used to confirm cancellation of an I-PDU.
 *
 * \param           PduIdType gwId              :        ID of  N-PDU that has been received
 *                  Std_ReturnType  result     : Result of the reception.
 *                  E_OK in case TP reception completed successfully
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

void PduR_GF_Tp_RxIndication_Func( PduIdType gwId, Std_ReturnType result)
{
    /* Start: Variable declaration */
    PduR_GT_TpPtr gt;
    const PduR_FIFO_Tp_fixed * fifo;
    PduR_TpSessDynPtr SessionPtr;
    PduR_TpConnDynPtr ConnectionPtr;
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    gt = PDUR_GW_TP_BASE+gwId;
    fifo = (gt->buffer);
    ConnectionPtr = (PduR_TpConnDynPtr)(fifo->connectionTable);
    /* End: Variable Initialization */

    PduR_LoadActiveSession(&SessionPtr,ConnectionPtr);
    /*At this stage Session ptr Rx status should be in PDUR_RX_INPROGRESS */
    if ((SessionPtr == NULL_PTR) || (SessionPtr->Rx_Status != PDUR_RX_INPROGRESS))
    {
        /* Session is not found */
        return;
    }

    if (result == E_OK)
    {
        /*If Rx Indication result is OK, then set Session ptr TRX status to FINISHED*/
         SessionPtr->Rx_Status = PDUR_RX_FINISHED;
         /*Check TxStatus always for primarySession to Transmit  */
         if(ConnectionPtr->primarySession->Tx_Status == PDUR_TX_IDLE)
         {
             VAR(PduInfoType,AUTOMATIC) readerPduInfo;
             /* Ensure that the reader's PduInfo object is initialized */
             readerPduInfo.SduDataPtr = SessionPtr->readPtr;
             /*At the first time the complete length should be transmitted to lower layer when TpTransmit is called */
             readerPduInfo.SduLength = SessionPtr->SduLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8];
             /* Change the TxStatus to in-progress and then invoke <LoTp>_Transmit function */
             SessionPtr->Tx_Status = PDUR_TX_INPROGRESS;

             /* Call the transmit api in case of multicast gateway */
             PduR_loTransmitTable[(gt->loTransmitID)].PduR_loTransmitFunc(gt->loId, &readerPduInfo);

         }
#ifdef PDUR_FIFO_TP_SUPPORT
        if(gt->TpQueueDepth_u8 > 1u)
        {
            /* Move the WritePtr and point to the next instance of the buffer */
            SessionPtr->Rx_FifoInstanceUsed_u8++;
            SessionPtr->writePtr = gt->buffer->begin;

            /* Check if FIFO is FULL or not. If not FULL, move the writePtr to next instance, else reset back the pointer to begin of the buffer in TxConfirmation */
            if(SessionPtr->Rx_FifoInstanceUsed_u8 != gt->TpQueueDepth_u8)
            {
                /* Holds the length of the buffer to move the writeptr */
                PduLengthType RxInstanceLength_l;
                RxInstanceLength_l = (PduLengthType)((SessionPtr->Rx_FifoInstanceUsed_u8)*(SessionPtr->bufferLength));
                /* Move writePtr to next instance */
                SessionPtr->writePtr += RxInstanceLength_l;
            }
            else
            {
                /* All the instances have been used, hence moving it to the start of the FIFO */
                SessionPtr->Rx_FifoInstanceUsed_u8 = 0;
            }
         }
#endif /* #ifdef PDUR_FIFO_TP_SUPPORT */
    }
    /* result is E_NOT_OK */
    else
    {
#ifdef PDUR_FIFO_TP_SUPPORT
        /* Rx_FifoInstanceUsed_u8 and WritePtr is not required to move to next instance when Tp_RxIndication result is E_NOT_OK */
        if(gt->TpQueueDepth_u8 > 1u)
        {
            /* Holds the length of the buffer to move the writeptr */
            PduLengthType RxInstanceLength_l;
            /* To move the writePtr to next instance, this is brought back to begin and moved */
            SessionPtr->writePtr = gt->buffer->begin;
            RxInstanceLength_l = (PduLengthType)((SessionPtr->Rx_FifoInstanceUsed_u8)*(SessionPtr->bufferLength));
            /* Move writePtr to next instance */
            SessionPtr->writePtr += RxInstanceLength_l;

            /* Reset the received data in CopyRxData() and in StartOfReception() of the same instance, instead of overwriting it */
            SessionPtr->numRxLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8] = 0u;
            SessionPtr->SduLength_auo[SessionPtr->Rx_FifoInstanceUsed_u8]   = 0u;

            /* Check if in case FIFO is FULL / empty, ie. RxFifoInstanceUsed = 0 and TxFifoIntanceUsed = 0
             * RxIndication come for first instance of the FiFo*/
            if((SessionPtr->Rx_FifoInstanceUsed_u8 == 0u) && (SessionPtr->Tx_FifoInstanceUsed_u8 == 0u))
            {
                /* If RxIndication returns with E_NOT_OK with first reception, then to accept next reception, reset Rx_Status to IDLE */
                SessionPtr->Rx_Status = PDUR_SESSION_IDLE;
            }
            else
            {
                /* If RxIndication returns with E_NOT_OK with first reception, then to accept next reception, reset Rx_Status to FINISHED */
                SessionPtr->Rx_Status = PDUR_RX_FINISHED;
            }

            /* Transmission shall be aborted which was triggered from the CopyRxData().
             * Use Case: Two successful reception is done and two TxConfirmation is received, assume during next reception's CopyRxData,
             * transmission started, after that TpRxIndication() received with E_NOT_OK, then abort the transmission
             * Check if Reception and transmission for the same instance */
            if((SessionPtr->Rx_FifoInstanceUsed_u8 == SessionPtr->Tx_FifoInstanceUsed_u8) && (SessionPtr->Tx_Status == PDUR_TX_INPROGRESS))
            {
                SessionPtr->Tx_Status = PDUR_TX_IDLE;

            }
             else
            {
                /* Do Nothing */
            }
        }
        else
#endif /* PDUR_FIFO_TP_SUPPORT */
        {
        /* FC_VariationPoint_START */
        /*If Rx Indication result is NOT OK, then release and reset respective buffer */
#ifdef PDUR_QUEUEING_SUPPORT
        /* Close the current session as it has reported error during reception */
        if(ConnectionPtr->activeSessionCount != 0)
        {
            ConnectionPtr->activeSessionCount--;
            /* If there are no active sessions , then release the buffers */
            if(ConnectionPtr->activeSessionCount == 0)
            {
                PduR_ReleaseTpBuffer(gt);
            }
        }
#else
        /* FC_VariationPoint_END */
        /* Release the acquired buffers . The below function will release buffer only if buffer pool is used */
        PduR_ReleaseTpBuffer(gt);
        /* FC_VariationPoint_START */
#endif
        /* FC_VariationPoint_END */
        /* Clear the session states */
        PduR_ResetPduR_FIFO_Tp_dynamic_Structure(SessionPtr, gt);
        }
    }

}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif /* PDUR_TPGATEWAY_SUPPORT */

