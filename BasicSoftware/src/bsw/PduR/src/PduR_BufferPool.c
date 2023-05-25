

#include "PduR_Prv.h"
#include "PduR.h"
#include "PduR_BufferPool.h"

#if defined(PDUR_BUFFERPOOL_SUPPORT)

const PduR_BufferPool_ConfigType * PduR_Global_BufferPoolPtr;

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

void PduR_BufferPoolInit(const PduR_BufferPool_ConfigType * BuffPoolConfigPtr)
{
    /* Start: Variable declaration */
    PduR_BufferPool_FixedPtr BufferPoolConstPtr;
    uint8 BufferPool_Idx;
    PduRBufferPoolLengthType PoolBuffer_Idx;    /* Local variable which holds each buffer's index inside the buffer pool */
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    PduR_Global_BufferPoolPtr = BuffPoolConfigPtr;
    BufferPoolConstPtr = PDUR_GW_TP_BASE_BUFFERPOOL_FIXED;
    /* End: Variable Initialization */

    /* Loop through all the buffer pools */
    for(BufferPool_Idx = 0; BufferPool_Idx < PDUR_NUM_BUFFERPOOL; BufferPool_Idx++)
    {
        /* Start: Variable declaration */
        PduR_BufferPool_DynamicPtr BufferPoolDynPtr;
        PduR_PoolBuffer_DynamicPtr PoolBufferDynPtr;
        PduR_PoolBuffer_FixedPtr PoolBufferFixedPtr;
        /* End: Variable declaration */

        /* Start: Variable Initialization */
        BufferPoolDynPtr = BufferPoolConstPtr[BufferPool_Idx].bufferPoolDyn;
        PoolBufferDynPtr = BufferPoolConstPtr[BufferPool_Idx].poolBufferDyn;
        PoolBufferFixedPtr = BufferPoolConstPtr[BufferPool_Idx].poolBufferConst;
        /* End: Variable Initialization */

        /* Loop through all the buffers inside individual buffer pool */
        for(PoolBuffer_Idx = 0; PoolBuffer_Idx < BufferPoolConstPtr[BufferPool_Idx].totalBuffersInPool; PoolBuffer_Idx++)
            {
            /* Assign the index value to the first byte of the buffer,
             * which will be used in PduR_BufferPoolRelease API to release
             */
                PoolBufferFixedPtr[PoolBuffer_Idx].poolBufferPtr[0] = (uint8)PoolBuffer_Idx;
                PoolBufferDynPtr[PoolBuffer_Idx].poolBufferStatus = TRUE;
            }
        /* Update the no.of free buffers with the total no.of buffers inside the buffer pool */
        BufferPoolDynPtr->freeBuffersInPool = BufferPoolConstPtr[BufferPool_Idx].totalBuffersInPool;
    }

}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

BufReq_ReturnType PduR_BufferPoolRequest(uint16 BuffPoolId, PduLengthType TpSduLength, PduLengthType * PduRBufferSizePtr, uint8 * *bufferPtr)
{
    /* Start: Variable declaration */
    PduR_BufferPool_FixedPtr BufferPoolConstPtr;
    PduR_BufferPool_DynamicPtr BufferPoolDynPtr;
    PduRBufferPoolLengthType    FreeBuffersInPool;
    BufReq_ReturnType rtn;
    PduRBufferPoolLengthType PoolBuffer_Idx;    /* Local variable which holds each buffer's index inside the buffer pool */
    boolean BufferPoolReqFlag;
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    BufferPoolConstPtr = PDUR_GW_TP_BASE_BUFFERPOOL_FIXED + BuffPoolId;
    BufferPoolDynPtr = BufferPoolConstPtr->bufferPoolDyn;
    FreeBuffersInPool = BufferPoolDynPtr->freeBuffersInPool;
    rtn = BUFREQ_OK;
    BufferPoolReqFlag = FALSE;
    /* End: Variable Initialization */

    /* check if the requested length is less than maximum buffer length in the pool */
    if(TpSduLength <= BufferPoolConstPtr->maxBufferSize)
    {
        BufferPoolReqFlag = TRUE;
    }/*end: if(TpSduLength <= FixedPtr->maxBufferSize) */
    else
    {
#ifdef PDUR_STORE_AND_FORWARD
        /* TpSduLength is greater than bufferLength and LimitedBuffer feature is enabled*/
        BufferPoolReqFlag = TRUE;
#else
        rtn =  BUFREQ_E_OVFL;
#endif
    }

    if(BufferPoolReqFlag != FALSE)
    {
        /* Request the buffer pool for the buffer if there is atleast one free buffer available */
        if(FreeBuffersInPool > 0)
        {
            /*Loop through all the free buffers inside the buffer pool*/
            for(PoolBuffer_Idx = 0; PoolBuffer_Idx < BufferPoolConstPtr->totalBuffersInPool; PoolBuffer_Idx++)
            {

                /*Linear search for the buffer inside the buffer pool*/
                /* First step: To check with requested length with buffer length and buffer status has to be checked*/
                if((BufferPoolConstPtr->poolBufferDyn[PoolBuffer_Idx].poolBufferStatus == TRUE)
			    #ifndef PDUR_STORE_AND_FORWARD
                /*This check is only needed if limitedBuffer feature is disabled*/
				&& (TpSduLength <= BufferPoolConstPtr->poolBufferConst[PoolBuffer_Idx].poolBufferLength)
				#endif
				 )
                {

                    /* Copy the allocated buffer's length into PduRBufferSizePtr out parameter */
                    *PduRBufferSizePtr = BufferPoolConstPtr->poolBufferConst[PoolBuffer_Idx].poolBufferLength;
                    /* Copy the allocated buffer's pointer into bufferPtr out parameter, this is the actual buffer to be used for gatewaying */
                    *bufferPtr = &(BufferPoolConstPtr->poolBufferConst[PoolBuffer_Idx].poolBufferPtr[1]);
                    /* Reset the buffer's status flag */
                    BufferPoolConstPtr->poolBufferDyn[PoolBuffer_Idx].poolBufferStatus = FALSE;
                    /* Decrement the count of free buffer's after allocating */
                    FreeBuffersInPool--;
                    BufferPoolDynPtr->freeBuffersInPool = FreeBuffersInPool;
                    /* Requested length is allocated, hence return BUFREQ_OK*/
                    rtn = BUFREQ_OK;
                    /* Once we get the buffer, exit from the loop */
                    break;
               }/*end: (TpSduLength <= FixedPtr->poolBufferConst[index].poolBufferLength #ifndef PDUR_STORE_AND_FORWARD && FixedPtr->poolBufferDyn[index].poolBufferStatus == TRUE #endif )*/
               else
               {
                   /*The requested SduLength buffer is not available in the buffer pool*/
                   rtn = BUFREQ_E_NOT_OK;
               }
           }/*end: for(index = 0; index < FreeBuffersInPool; index++)*/
      }/* end: if(FreeBuffersInPool > 0) */
      else
      {
          /* All the buffer's inside the buffer pool are occupied */
               rtn = BUFREQ_E_NOT_OK;
       }
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

void PduR_BufferPoolRelease(uint16 BuffPoolId, const uint8 * bufferPtr)
{
    /* Start: Variable declaration */
    PduR_BufferPool_FixedPtr BufferPoolConstPtr;
    PduR_BufferPool_DynamicPtr BufferPoolDynPtr;
    PduRBufferPoolLengthType    FreeBuffersInPool;
    PduRBufferPoolLengthType PoolBuffer_Idx;    /* Local variable which holds each buffer's index inside the buffer pool */
    /* End: Variable declaration */

    /* Start: Variable Initialization */
    BufferPoolConstPtr = PDUR_GW_TP_BASE_BUFFERPOOL_FIXED + BuffPoolId;
    BufferPoolDynPtr = BufferPoolConstPtr->bufferPoolDyn;
    FreeBuffersInPool = BufferPoolDynPtr->freeBuffersInPool;
    /* End: Variable Initialization */

    /* This check is done, in case if an unexpected TxConfirmation is called,
     * Then buffer release has to happen only if it is acquired */
    if(bufferPtr != NULL_PTR)
        {
            /* Copy the first byte of the buffer to find the index of the buffer inside the buffer pool */
            bufferPtr--;
            PoolBuffer_Idx = bufferPtr[0];

            /* Reset the buffer's status flag */
            BufferPoolConstPtr->poolBufferDyn[PoolBuffer_Idx].poolBufferStatus = TRUE;
            /*Increment the free buffer counter*/
            FreeBuffersInPool++;
            BufferPoolDynPtr->freeBuffersInPool = FreeBuffersInPool;
        }

}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif /* End: #if defined(PDUR_BUFFERPOOL_SUPPORT) */

