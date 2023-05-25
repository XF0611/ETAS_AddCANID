


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanIf_Prv.h"



/*Validations to be considered: If TxBuffering is on, at least one buffer should have size greater than 0*/

#if((CANIF_PUBLIC_TXBUFFERING == STD_ON) && (CANIF_CFG_TX_FEATURE_ENABLED== STD_ON))

static void CanIf_Prv_GetFreeTxBufferIndex
                                            (
                                                uint8 * lFreeIndex,
                                                const CanIf_Cfg_TxBufferConfig_tst * TxBufferConfig_pst
                                            );

static void CanIf_Prv_SortTxBuffer(uint32 BufferId,
        CanIf_Cfg_CanIdBuffer_tst * CanIdBuf_p, uint8 BufferSize
                                            );





/**********************************************************************************************************************
 * Function name     : CanIf_Prv_WriteTxBuffer
 * Syntax            : void CanIf_Prv_WriteTxBuffer(PduIdType CanIfTxSduId,
 *                                                          Can_PduType Pdu)
 * Service ID[hex]   : --
 * Sync/Async        : --
 * Reentrancy        : --
 * Parameters (in)   : CanIfTxSduId, Pdu
 * Parameters (inout): None
 * Parameters (out)  : None
 * Return value      : Std_ReturnType
 * Description       : Function is called by CanIf_Transmit() which writes PDU to corresponding TxBuffer.                                                                                          **
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_Prv_WriteTxBuffer(
                                                            PduIdType CanIfTxSduId,
                                                            Can_PduType Pdu
                                                        )
{
    uint8 * lDataBuf_p;

    CanIf_Cfg_CanIdBuffer_tst * lCanIdBuf_p;

    const CanIf_Cfg_TxPduConfig_tst * lTxPduConfig_pst;

    const CanIf_Cfg_TxBufferConfig_tst * lTxBufferConfig_pst;

    uint32 lBufferId;

    uint8 lBufferSize;

    uint8 lDataMaxLen;

    uint8 lDataLen;

    CanIf_Cfg_CanHandleType_ten lCanHandleType;

    uint16 lIndex;

    uint8 lSduWriteIndex = 0xFF;

    uint16 lCanIdWriteIndex = 0;

    uint8 lDataIndex;

    uint8 * lSduPtr;

    Std_ReturnType lRetValue;
/* variable to store the position of TxPduId in the mapping table(CustId)*/
    uint16 ltxPduCustId_t;

/* to get the position of the TxPduId in the mapping table*/
    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanIfTxSduId];

    /*Get the config structure for the requested TxPdu*/
    lTxPduConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;

    /*Get TxBuffer config structure for the requested TxPdu*/
    lTxBufferConfig_pst = lTxPduConfig_pst->CanIf_TxBufferConfigPtr;

    /*Get the array where Sdu is stored*/
    lDataBuf_p          = lTxBufferConfig_pst->DataBuf;

    /*Get the array where CanId, Sdulength and swHandle are stored*/
    lCanIdBuf_p         = lTxBufferConfig_pst->CanIdBuf;

    /*Get the buffer parameters*/
    lBufferId           = (uint32)lTxBufferConfig_pst->CanIfBufferId;
    lBufferSize         = lTxBufferConfig_pst->CanIfBufferSize;
    lDataMaxLen         = lTxBufferConfig_pst->CanIfBufferMaxDataLength;

    /*Get the Hth type*/
    lCanHandleType      = lTxBufferConfig_pst->CanIf_HthConfigPtr->CanHandleType;

    /*[SWS_CANIF_00837], [SWS_CANIF_00835]*/

    if (    /*  Buffer is full AND incoming PDU is not already buffered */
            ((CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus == CANIF_TXBUFFER_FULL)
                    && (CanIf_Prv_TxPduRam_ast[CanIfTxSduId].pdu_buffered_flag != TRUE))

            /*  CanIfTxBuffer is assigned to a FullCAN HTH  */
            ||(lCanHandleType == CANIF_FULL)
       )
    {
        lRetValue = E_NOT_OK;                    /*PDU not buffered*/
    }
    else                                   /*Buffer is ready to store the incoming PDU*/
    {
        /*If queue is initially empty */
        if (CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus == CANIF_TXBUFFER_EMPTY)
        {
            CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus = CANIF_TXBUFFER_READY;
        }

        /*Incoming PDU is already buffered*/
        if(CanIf_Prv_TxPduRam_ast[CanIfTxSduId].pdu_buffered_flag == TRUE)                       /*[SWS_CANIF_00068]*/
        {
            /*Get the index of both CanId and SDU buffers*/
            for (lIndex = 0; lIndex <= CanIf_Prv_TxBufferRam_ast[lBufferId].last_index; lIndex++)
            {
                /*Optimization may be possible for searching TxPduId using pdu_buffered_flag. To be taken care if going for optimization*/
                if ((lCanIdBuf_p+lIndex)->swPduHandle == lTxPduConfig_pst->TxPduId)
                {
                    lSduWriteIndex   = (lCanIdBuf_p + lIndex)->BufferIndex;
                    lCanIdWriteIndex = lIndex;
                    break;
                }
            }
        }
        else    /*[SWS_CANIF_00836]*/
        {
            /*Search for free position in SDU Buffer*/
            CanIf_Prv_GetFreeTxBufferIndex(&lSduWriteIndex, lTxBufferConfig_pst);
            /*Get the next index of the CanId queue buffer*/

            CanIf_Prv_TxBufferRam_ast[lBufferId].last_index = (((CanIf_Prv_TxBufferRam_ast[lBufferId].last_index) + 1)&(0xFFu));

            lCanIdWriteIndex                               = CanIf_Prv_TxBufferRam_ast[lBufferId].last_index;
            /*If queue is full, set the buffer status to CANIF_TXBUFFER_FULL*/
            if(lCanIdWriteIndex == ((lBufferSize - 1)&0xFFu))
            {
                CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus = CANIF_TXBUFFER_FULL;
            }
            /*Store the SDU buffer index where data is to be written*/
            (lCanIdBuf_p + lCanIdWriteIndex)->BufferIndex  = lSduWriteIndex;
            /*Set the flag which represents SDU buffer index is already occupied with a PDU*/
            *(lDataBuf_p + ((lSduWriteIndex)*(lDataMaxLen+1)) + 0)          = 0x00;
            /*Set the flag this PDU is already buffered*/
            CanIf_Prv_TxPduRam_ast[CanIfTxSduId].pdu_buffered_flag = TRUE;

         }
        /*Store the transmit request*/
        (lCanIdBuf_p + lCanIdWriteIndex)->CanId       = Pdu.id;
        (lCanIdBuf_p + lCanIdWriteIndex)->swPduHandle = Pdu.swPduHandle;


/*For trigger transmit pdu, no need to store data*/
#if(CANIF_TRIGGERTRANSMIT_SUPPORT == STD_ON)
        if(lTxPduConfig_pst->TxPduTriggerTransmit != TRUE)
#endif
        {
            /**/
            lSduPtr = Pdu.sdu;

            if(Pdu.length > lDataMaxLen)
            {
                lDataLen = lDataMaxLen;
                (lCanIdBuf_p + lCanIdWriteIndex)->SduLength = lDataMaxLen;
            }
            else
            {
                lDataLen = Pdu.length;
                (lCanIdBuf_p + lCanIdWriteIndex)->SduLength = Pdu.length;
            }

            /*No NULL check is needed for lSduPtr as it is already done in CanIf_Tranmsit() for non-triggertransmit case*/

            for(lDataIndex=1; lDataIndex < (lDataLen + 1); lDataIndex++)   /*lDataLen is excluded of flag index*/
            {
                /*Copy the data to SDU buffer*/
                *(lDataBuf_p + ((lSduWriteIndex)*(lDataMaxLen+1)) + lDataIndex) = *(lSduPtr+(lDataIndex-1));
            }

        }

        /*[SWS_CANIF_00070] Sort buffer based on CanId*/
        CanIf_Prv_SortTxBuffer(lBufferId, lCanIdBuf_p, lBufferSize);
        lRetValue = E_OK;
    }
    return(lRetValue);
} /*End of CanIf_Prv_WriteTxBuffer()*/

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/**********************************************************************************************************************
** Function name     : CanIf_Prv_SortTxBuffer                                                                        **
** Syntax            : void CanIf_Prv_SortTxBuffer(uint32 BufferId, CanIf_Cfg_CanIdBuffer_tst* CanIdBuf_p, uint8     **
**                                                                                                  BufferSize)      **
** Service ID[hex]   : --                                                                                            **
** Sync/Async        : --                                                                                            **
** Reentrancy        : --                                                                                            **
** Parameters (in)   : BufferId                                                                                      **
** Parameters (inout): CanIdBuf_p                                                                                    **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : Function sorts the TxBuffer based on CanId.                                                   **                                                                              **
***********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
static void CanIf_Prv_SortTxBuffer(uint32 BufferId,
        CanIf_Cfg_CanIdBuffer_tst * CanIdBuf_p, uint8 BufferSize)
{
    uint16 lIndex1;

    uint8 lIndex2;

    Can_IdType lCanId;

    uint8 lBufIndex;

    PduIdType lswPduHandle;

    uint8 lSduLength ;

        /*Sort (QSort) CanId buffer based on CanId. */
    for (lIndex1 = 0; lIndex1 <= CanIf_Prv_TxBufferRam_ast[BufferId].last_index; lIndex1++)
    {
        for (lIndex2 = 0; (lIndex2 <= ((CanIf_Prv_TxBufferRam_ast[BufferId].last_index)-lIndex1)) && ((lIndex2+1)<BufferSize); lIndex2++)
        {
            if ((CanIdBuf_p+lIndex2)->CanId < (CanIdBuf_p+(lIndex2+1))->CanId)
            {
                lCanId       = (CanIdBuf_p+lIndex2)->CanId;
                lBufIndex    = (CanIdBuf_p+lIndex2)->BufferIndex;
                lswPduHandle = (CanIdBuf_p+lIndex2)->swPduHandle;
                lSduLength   = (CanIdBuf_p+lIndex2)->SduLength;

                CanIdBuf_p[lIndex2].CanId = CanIdBuf_p[lIndex2+1].CanId;
                CanIdBuf_p[lIndex2].BufferIndex = CanIdBuf_p[lIndex2+1].BufferIndex;
                CanIdBuf_p[lIndex2].swPduHandle = CanIdBuf_p[lIndex2+1].swPduHandle;
                CanIdBuf_p[lIndex2].SduLength = CanIdBuf_p[lIndex2+1].SduLength;

                CanIdBuf_p[lIndex2+1].CanId         = lCanId;
                CanIdBuf_p[lIndex2+1].BufferIndex   = lBufIndex;
                CanIdBuf_p[lIndex2+1].swPduHandle   = lswPduHandle;
                CanIdBuf_p[lIndex2+1].SduLength     = lSduLength;

            }
        }
    }
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/**********************************************************************************************************************
** Function name     : CanIf_Prv_ClearTxChannelBuffer                                                                **
** Syntax            : void CanIf_Prv_ClearTxChannelBuffer(uint8 ControllerId)                                        **
** Service ID[hex]   : --                                                                                            **
** Sync/Async        : --                                                                                            **
** Reentrancy        : --                                                                                            **
** Parameters (in)   : ControllerId                                                                                  **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : Function clears all the Tx buffers which are indirectly linked to a particular controller.     **                                                                              **
***********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

void CanIf_Prv_ClearTxChannelBuffer(uint8 ControllerId)
{
    uint8 * lDataBuf_p;
    CanIf_Cfg_CanIdBuffer_tst * lCanIdBuf_p;
    uint32 lBufferId;
    uint32 lTxPduId;
    uint8 lBufferSize;
    uint8 lDataIndex;
    uint8 BufferIndex;
    uint8 lDataMaxLen;
    const CanIf_Cfg_TxBufferConfig_tst * lBufferConfig_pst;
#if ( CANIF_RB_NODE_CALIBRATION != STD_ON )
    uint32 * BufferId_ptr;
    uint32 * TxPduId_ptr;
    uint32 lBufferCustId;
    uint32 BufferIdArrayIndex;
    uint32 TxPduIdArrayIndex;
    const CanIf_Cfg_CtrlConfig_tst * lCtrlConfig_pst;
#endif
#if ( CANIF_RB_NODE_CALIBRATION == STD_ON )
	uint8 lCalibCtrlId;
    uint32 lBufferArrayIndex;
    uint32 lHthTxPduMapTblArryIndex;
#endif

#if ( CANIF_RB_NODE_CALIBRATION != STD_ON )
    /*Get the configuration of request controller*/
    lCtrlConfig_pst           = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + ControllerId;
    BufferId_ptr              = lCtrlConfig_pst->BufferIdPtr;
    TxPduId_ptr               = lCtrlConfig_pst->TxPduIdPtr;
    /*Get the address of Buffer config structure*/
    lBufferConfig_pst         = CanIf_Prv_ConfigSet_tpst->CanIf_TxBufferConfigPtr;

    /*For all buffers linked to this controller*/
    for(BufferIdArrayIndex = 0; BufferIdArrayIndex < (lCtrlConfig_pst->TotalBufferCount); BufferIdArrayIndex++)
    {
        /*Get the Buffer Id*/
        lBufferCustId   = *(BufferId_ptr + BufferIdArrayIndex);
        /*Get the Buffer address corresponding to the buffer Id*/
        lDataBuf_p    = (lBufferConfig_pst + lBufferCustId)->DataBuf;

        lCanIdBuf_p  = (lBufferConfig_pst + lBufferCustId)->CanIdBuf;
        lDataMaxLen  = (lBufferConfig_pst + lBufferCustId)->CanIfBufferMaxDataLength;
        /*Get the size of Buffer*/
        lBufferSize = (lBufferConfig_pst + lBufferCustId)->CanIfBufferSize;
        lBufferId   = (uint32)(lBufferConfig_pst + lBufferCustId)->CanIfBufferId;

        if (CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus != CANIF_TXBUFFER_EMPTY)       /*Buffer is not empty*/
        {
            for(BufferIndex = 0 ; BufferIndex < lBufferSize ; BufferIndex++ )
            {
                /* Clear the Data in DataBuf*/
                *(lDataBuf_p + ((BufferIndex)*(lDataMaxLen+1)) + 0)    = 0xFF; /*Free*/
                for(lDataIndex=1; lDataIndex < (lDataMaxLen + 1); lDataIndex++)
                {
                    *(lDataBuf_p + ((BufferIndex)*(lDataMaxLen + 1)) + lDataIndex) = 0;
                }

                (lCanIdBuf_p + BufferIndex)->BufferIndex = 0;
                (lCanIdBuf_p + BufferIndex)->CanId = 0;
                (lCanIdBuf_p + BufferIndex)->SduLength = 0;
                (lCanIdBuf_p + BufferIndex)->swPduHandle = 0;
            }
            CanIf_Prv_TxBufferRam_ast[lBufferId].last_index   = 0xFF;                  /*Important. Don't change line of code*/
            CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus  = CANIF_TXBUFFER_EMPTY;
        }
    } /* end of for(BufferIdArrayIndex = 0; BufferIdArrayIndex < (lCtrlConfig_pst->TotalBufferCount); BufferIdArrayIndex++) */

    /*Clear the TxPdu already buffered flag */
    for(TxPduIdArrayIndex = 0 ; TxPduIdArrayIndex < (lCtrlConfig_pst->TotalTxPduCount) ; TxPduIdArrayIndex++ )
    {
     lTxPduId = *(TxPduId_ptr + TxPduIdArrayIndex);
     CanIf_Prv_TxPduRam_ast[lTxPduId].pdu_buffered_flag = FALSE;

    }
#else /* CANIF_RB_CALIBRATION = STD_ON */
    /* Loop through all the BufferConfig */
	for ( lBufferArrayIndex=0; lBufferArrayIndex < CANIF_TOTAL_TXBUFFERS; lBufferArrayIndex++ )
	{
	    lBufferConfig_pst = CanIf_Prv_ConfigSet_tpst->CanIf_TxBufferConfigPtr + lBufferArrayIndex;
	    lCalibCtrlId = lBufferConfig_pst->CanIf_HthConfigPtr->getCtrlCfg();
	    if( ( CanIf_CanCtrl_CanIfCtrl_MappingTable_acst[lCalibCtrlId]  == ControllerId ) &&
	        ( lBufferConfig_pst->CanIf_HthConfigPtr->CanHandleType == CANIF_BASIC  ) )
	    {
	        lDataBuf_p  = lBufferConfig_pst->DataBuf;
            lCanIdBuf_p = lBufferConfig_pst->CanIdBuf;
            lDataMaxLen = lBufferConfig_pst->CanIfBufferMaxDataLength;
            lBufferSize = lBufferConfig_pst->CanIfBufferSize;
            lBufferId   = (uint32)lBufferConfig_pst->CanIfBufferId;

            /* Check the Buffer is not empty */
            if ( CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus != CANIF_TXBUFFER_EMPTY )
            {
                for( BufferIndex = 0 ; BufferIndex < lBufferSize ; BufferIndex++ )
                {
                    /* Clear the Data in DataBuf*/
                    *(lDataBuf_p + ((BufferIndex)*(lDataMaxLen+1)) + 0) = 0xFF; /*Free*/
                    for( lDataIndex=1; lDataIndex < (lDataMaxLen + 1); lDataIndex++ )
                    {
                       *(lDataBuf_p + ((BufferIndex)*(lDataMaxLen + 1)) + lDataIndex) = 0;
                    }

                    (lCanIdBuf_p + BufferIndex)->BufferIndex = 0;
                    (lCanIdBuf_p + BufferIndex)->CanId       = 0;
                    (lCanIdBuf_p + BufferIndex)->SduLength   = 0;
                    (lCanIdBuf_p + BufferIndex)->swPduHandle = 0;

                }
                CanIf_Prv_TxBufferRam_ast[lBufferId].last_index    = 0xFF;
                CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus  = CANIF_TXBUFFER_EMPTY;
            } /* end of if ( CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus != CANIF_TXBUFFER_EMPTY ) */

            for( lHthTxPduMapTblArryIndex = 0;
                 lHthTxPduMapTblArryIndex < lBufferConfig_pst->CanIf_HthConfigPtr->NumTxPdus;
                 lHthTxPduMapTblArryIndex++ )
            {
                lTxPduId = lBufferConfig_pst->CanIf_HthConfigPtr->HthTxPduMapTblPtr[lHthTxPduMapTblArryIndex];
                CanIf_Prv_TxPduRam_ast[lTxPduId].pdu_buffered_flag = FALSE;
            }
	    }

	} /* end of for ( lBufferArrayIndex=0; lBufferArrayIndex < CANIF_TOTAL_TXBUFFERS; lBufferArrayIndex++ ) */

#endif
} /*End of CanIf_Prv_ClearTxChannelBuffer()*/

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/**********************************************************************************************************************
** Function name     : CanIf_Prv_BufferInit                                                                          **
** Syntax            : void CanIf_Prv_BufferInit(void)                                                               **
** Service ID[hex]   : --                                                                                            **
** Sync/Async        : --                                                                                            **
** Reentrancy        : --                                                                                            **
** Parameters (in)   : None                                                                                          **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : void                                                                                          **
** Description       : Function called by API CanIf_Init() to clear all the buffers                                  **
**                     can be stored.                                                                                **
***********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

void CanIf_Prv_BufferInit(void)
{
    uint8 * lDataBuf_p;

    CanIf_Cfg_CanIdBuffer_tst * lCanIdBuf_p;

    uint32 lBufferCustId;
    uint32 lBufferId;
    const CanIf_Cfg_TxBufferConfig_tst* lCanIf_TxBufferConfig_p;

    uint8 lBufferSize;
    uint8 lDataIndex;
    uint8 BufferIndex;
    uint8 lDataMaxLen;
    uint32 TxPduId;

    lCanIf_TxBufferConfig_p = CanIf_Prv_ConfigSet_tpst->CanIf_TxBufferConfigPtr;

    /*For all buffers*/
     for(lBufferCustId = 0; lBufferCustId < CanIf_Prv_ConfigSet_tpst->NumOfTxBuffers; lBufferCustId++)
     {

         /*Get the Buffer address corresponding to the buffer Id*/
         lDataBuf_p    = (lCanIf_TxBufferConfig_p + lBufferCustId)->DataBuf;

         lCanIdBuf_p  = (lCanIf_TxBufferConfig_p + lBufferCustId)->CanIdBuf;
         lDataMaxLen  = (lCanIf_TxBufferConfig_p + lBufferCustId)->CanIfBufferMaxDataLength;
         /*Get the size of Buffer*/
         lBufferSize = (lCanIf_TxBufferConfig_p + lBufferCustId)->CanIfBufferSize;
         lBufferId = (uint32)(lCanIf_TxBufferConfig_p + lBufferCustId)->CanIfBufferId;
         for(BufferIndex = 0 ; BufferIndex < lBufferSize ; BufferIndex++ )
         {
             /* Clear the Data in DataBuf*/
             *(lDataBuf_p + (BufferIndex*(lDataMaxLen + 1)) + 0)    = 0xFF;         /*Free*/
              for(lDataIndex=1; lDataIndex < (lDataMaxLen + 1); lDataIndex++)
              {
                  *(lDataBuf_p + (BufferIndex*(lDataMaxLen + 1)) + lDataIndex) = 0;
              }

             (lCanIdBuf_p + BufferIndex)->BufferIndex = 0;
             (lCanIdBuf_p + BufferIndex)->CanId = 0;
             (lCanIdBuf_p + BufferIndex)->SduLength = 0;
             (lCanIdBuf_p + BufferIndex)->swPduHandle = 0;
         }
         CanIf_Prv_TxBufferRam_ast[lBufferId].last_index   = 0xFF;                  /*Important. Don't change line of code*/
         CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus  = CANIF_TXBUFFER_EMPTY;
     }
     /*Clear the TxPdu buffered flag */
     for(TxPduId = 0 ; TxPduId < (CanIf_Prv_ConfigSet_tpst->NumOfTxPdus) ; TxPduId++ )
     {
         CanIf_Prv_TxPduRam_ast[TxPduId].pdu_buffered_flag = FALSE;
     }
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/**********************************************************************************************************************
** Function name     : CanIf_Prv_ReadTxBuffer                                                                        **
** Syntax            : Std_ReturnType CanIf_Prv_ReadTxBuffer(const CanIf_Cfg_TxBufferConfig_tst* CanIf_TxBufferConfig,
**                                                                                                  Can_PduType* Pdu)**
** Service ID[hex]   : --                                                                                            **
** Sync/Async        : --                                                                                            **
** Reentrancy        : --                                                                                            **
** Parameters (in)   : CanIf_TxBufferConfig                                                                          **
** Parameters (inout): Pdu                                                                                           **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : Function called by API CanIf_TxConfirmation() to read the high priority PDU from buffer       **
**                     can be stored.                                                                                **
***********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_Prv_ReadTxBuffer(const CanIf_Cfg_TxBufferConfig_tst* CanIf_TxBufferConfig, Can_PduType* Pdu)
{


    uint8 * lDataBuf_p;
    CanIf_Cfg_CanIdBuffer_tst * lCanIdBuf_p;
    uint32 lBufferId;
    uint8 lDataMaxLen;
    uint8 lDataLen;
    uint8 lDataIndex;
    uint8 * lSduPtr;
    uint8 lSduReadIndex;
    uint8 lCanIdReadIndex;
    Std_ReturnType lRetValue;

#if(CANIF_TRIGGERTRANSMIT_SUPPORT == STD_ON)
    const CanIf_Cfg_TxPduConfig_tst * lTxPduConfig_pst;
    uint16 ltxPduCustId_t;
#endif

    /*Get the buffer parameters*/
    lDataBuf_p  = CanIf_TxBufferConfig->DataBuf;
    lCanIdBuf_p = CanIf_TxBufferConfig->CanIdBuf;
    lBufferId   = (uint32)CanIf_TxBufferConfig->CanIfBufferId;
    lDataMaxLen = CanIf_TxBufferConfig->CanIfBufferMaxDataLength;

    lCanIdReadIndex = CanIf_Prv_TxBufferRam_ast[lBufferId].last_index;
    lSduReadIndex   = lCanIdBuf_p[lCanIdReadIndex].BufferIndex;


    /*Check is done by caller. Remove during optimization*/
    /*[SWS_CANIF_00386]*/  /*Buffer is empty*/
    if (CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus  == CANIF_TXBUFFER_EMPTY)
    {
        lRetValue =  E_NOT_OK;

    }
    else                                         /*Buffer is NOT empty*/
    {
        /*Read CanId and TxPduId from CanId queue buffer*/
          Pdu->id          = (lCanIdBuf_p + lCanIdReadIndex)->CanId;
          Pdu->swPduHandle = (lCanIdBuf_p + lCanIdReadIndex)->swPduHandle;

          lSduPtr = Pdu->sdu;

#if(CANIF_TRIGGERTRANSMIT_SUPPORT == STD_ON)
          /*Get TxPdu config structure for the requested TxPdu*/
          ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[Pdu->swPduHandle];
          lTxPduConfig_pst    = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;
          /*if trigger transmit pdu, set sdu pointer to NULL amd sdulength as 0*/
          if(lTxPduConfig_pst->TxPduTriggerTransmit == TRUE)

          {
              lSduPtr     = NULL_PTR;
              Pdu->length = 0;
          }
          else
#endif
          {
              Pdu->length      = (lCanIdBuf_p + lCanIdReadIndex)->SduLength;

              if(Pdu->length > lDataMaxLen)
              {
                  lDataLen = lDataMaxLen;
              }
              else
              {
                  lDataLen = Pdu->length;
              }

              /*For non-trigger transmit pdu, read the data from the corresponding index and sdulength*/
              for(lDataIndex=1; lDataIndex < (lDataLen + 1); lDataIndex++)
              {
                  *(lSduPtr + (lDataIndex-1)) = *(lDataBuf_p + ((lSduReadIndex)*(lDataMaxLen+1)) + lDataIndex);
              }

          }

          lRetValue = E_OK;
    }
    return(lRetValue);

} /*End of CanIf_BufferRead() */
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/**********************************************************************************************************************
** Function name     : CanIf_Prv_GetFreeTxBufferIndex                                                                  **
** Syntax            : static void CanIf_Prv_GetFreeTxBufferIndex
                                            (
                                            uint8 * lFreeIndex,
                                            uint8 BufferSize,
                                            uint8 * DataBuf
                                            )
** Service ID[hex]   : --                                                                                            **
** Sync/Async        : --                                                                                            **
** Reentrancy        : --                                                                                            **
** Parameters (in)   : BufferSize, DataBuf                                                                           **
** Parameters (inout): lFreeIndex                                                                                    **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : Static function called by API CanIf_Prv_WriteTxBuffer() to get free TxBuffer index where      **
**                     new PDU can be stored.                                                                        **
***********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"


static void CanIf_Prv_GetFreeTxBufferIndex
                                                (
                                                uint8 * lFreeIndex,
                                                const CanIf_Cfg_TxBufferConfig_tst * TxBufferConfig_pst
                                                )

{
    uint8 Index;
    uint8 lBufferSize;
    uint8 lDataMaxLen;
    uint8 * lDataBuf_p;

    lBufferSize = TxBufferConfig_pst->CanIfBufferSize;
    lDataMaxLen = TxBufferConfig_pst->CanIfBufferMaxDataLength;
    lDataBuf_p  = TxBufferConfig_pst->DataBuf;


    for(Index = 0; Index < lBufferSize; Index++)
    {
        if(*(lDataBuf_p + ((Index)*(lDataMaxLen+1)) + 0) == 0xFF)
        {
            *lFreeIndex = Index;
            break;
        }
    }
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif


/***********************************************************************************************************************
** Function name     : CanIf_Prv_WriteRxBuffer                                                                        **
** Syntax            : void CanIf_Prv_WriteRxBuffer(Const uint8* CanSduPtr,Const CanIf_Cfg_RxPduType_tst* RxPduCfgPtr,**
**                     Const CanDlc,Const CanId)                                                                      **
** Service ID[hex]   : --                                                                                             **
** Sync/Async        : --                                                                                             **
** Reentrancy        : --                                                                                             **
** Parameters (in)   : CanSduPtr, RxPduCfgPtr                                                                         **
** Parameters (inout): None                                                                                           **
** Parameters (out)  : None                                                                                           **
** Return value      : None                                                                                           **
** Description       : Internal function called by API CanIf_RxIndication to write data to receive buffer             **
**********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)
void CanIf_Prv_WriteRxBuffer(const uint8 * CanSduPtr,
                                              const CanIf_Cfg_RxPduType_tst * RxPduCfg_pcst,
                                              const PduLengthType CanDlc, const Can_IdType CanId
                                               )
{

    /* Byte position in the Rx buffer */
    uint8_least Index_u8;

    /*Tx-buffer pointer*/
    uint8 * RxTemBuff_pu8;

    /*varaiable to hold Sdu length*/
    uint8 SduLength_u8 = 0;

    /*Hold dlc without metadata */
    PduLengthType Dlc_u8 = 0;

    /* Holds Configured/Calibrated Dlc value and used to compare against DLC given by CanDrv */
    PduLengthType ConfigRxPduDlc_u8;

    #if(CANIF_METADATA_SUPPORT == STD_ON)
    /*store canid in the local variable removing Idtype information */
    Can_IdType CanIdNoIdType_t;

    /*Fetch CanId without CanIdType*/
    CanIdNoIdType_t = CanId & CANIF_PRV_MSB_BIT_RESET;
   #endif



    /* Check if the buffer is allocated */
    if(RxPduCfg_pcst->RxBuffer_u16 != (uint16)0xFFFFu)
    {
        RxTemBuff_pu8 = &CanIf_Prv_RxBuffer_tau8[RxPduCfg_pcst->RxBuffer_u16];

#if ( CANIF_RB_CALIBRATION == STD_ON )
        if( RxPduCfg_pcst->getRxPduDlc != NULL_PTR )
        {
            ConfigRxPduDlc_u8 = RxPduCfg_pcst->getRxPduDlc();
        }
        else
#endif
        {
            ConfigRxPduDlc_u8 = RxPduCfg_pcst->RxPduDlc_u8;
        }

        if( CanDlc < ConfigRxPduDlc_u8 )
        {
            Dlc_u8 = CanDlc;
            /* Get the SduLength */
            #if(CANIF_METADATA_SUPPORT == STD_ON)
            SduLength_u8 = (uint8)(CanDlc +RxPduCfg_pcst->MetadataLength_u8);
            #else
            SduLength_u8 = (uint8)(CanDlc);
            #endif /* #if(CANIF_METADATA_SUPPORT == STD_ON) */
        }
        else
        {
            Dlc_u8 = ConfigRxPduDlc_u8;
            /* Get the SduLength */
            #if(CANIF_METADATA_SUPPORT == STD_ON)
            SduLength_u8 = (uint8)(ConfigRxPduDlc_u8 + RxPduCfg_pcst->MetadataLength_u8);
            #else
            SduLength_u8 = (uint8)(ConfigRxPduDlc_u8);
            #endif /* #if(CANIF_METADATA_SUPPORT == STD_ON) */
        }
        /*Enter Critical Section */
        SchM_Enter_CanIf_RxBufAccessNoNest();

        /* Copy the SduLength */
        *(RxTemBuff_pu8) = SduLength_u8;

        /* Copy the Data */
        for(Index_u8 = 1; Index_u8 <= Dlc_u8; Index_u8++)
        {
            *(RxTemBuff_pu8 + Index_u8) = *CanSduPtr;
            CanSduPtr++;
        }

        #if(CANIF_METADATA_SUPPORT == STD_ON)
        if(RxPduCfg_pcst->MetadataLength_u8 >= 1u)
        {
            /*[SWS_CANIF_00844]copy the received canid bytes into SduDataBuffer with LSB byte first after the payload data*/
            for(Index_u8 = Dlc_u8+1u; Index_u8 <= SduLength_u8 ; Index_u8++)
            {
                *(RxTemBuff_pu8 + Index_u8) = (uint8)(CanIdNoIdType_t & 0x00ffu);
                CanIdNoIdType_t = (CanIdNoIdType_t>>CANIF_SHIFT_8);
            }
        }
        #endif /* #if(CANIF_METADATA_SUPPORT == STD_ON) */

        /* Leave Critical Section */
        SchM_Exit_CanIf_RxBufAccessNoNest();
    }
    /*to avoid compiler warning*/
    (void)CanId;
}
#endif
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/**********************************************************************************************************************
** Function name     : CanIf_Prv_ReadRxBuffer                                                                        **
** Syntax            : void CanIf_Prv_ReadRxBuffer(uint8* CanSduPtr, Const CanIf_Cfg_RxPduType_tst* RxPduCfgPtr)     **
** Service ID[hex]   : --                                                                                            **
** Sync/Async        : --                                                                                            **
** Reentrancy        : --                                                                                            **
** Parameters (in)   : CanSduPtr, RxPduCfgPtr                                                                        **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : Internal function called by API CanIf_ReadRxPduData to read data from receive buffer          **
**********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)
void CanIf_Prv_ReadRxBuffer(PduInfoType * PduInfoPtr,
                                            const CanIf_Cfg_RxPduType_tst * RxPduCfgPtr)
{
    /* Pointer to data */
    uint8 * PduData_p;

    /* Byte position in the Rx buffer */
    uint16_least Index_u16;

    /*Rx-buffer pointer*/
    uint8 * RxBuffPtr_Temp;


    /* Get the Data Pointer */
    PduData_p = PduInfoPtr->SduDataPtr;

    /* Check if the buffer is allocated */
    if(RxPduCfgPtr->RxBuffer_u16 != 0xFFFFu)
    {
        /* Get the buffer Pointer */
        RxBuffPtr_Temp = &CanIf_Prv_RxBuffer_tau8[RxPduCfgPtr->RxBuffer_u16];

        /*Enter Critical Section */
        SchM_Enter_CanIf_RxBufAccessNoNest();

        /* Copy the Dlc */
        PduInfoPtr->SduLength = *(RxBuffPtr_Temp);

        /* Copy the data */
        for(Index_u16 = 1; Index_u16 <= PduInfoPtr->SduLength; Index_u16++ )
        {
            *PduData_p = *(RxBuffPtr_Temp + Index_u16);
            PduData_p++;
        }

        /* Leave Critical Section */
        SchM_Exit_CanIf_RxBufAccessNoNest();
    }
}
#endif
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
