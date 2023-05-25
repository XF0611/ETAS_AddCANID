
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "rba_MemLib.h"

#include "NvM_Prv_Queue.h"

 /*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/**
 * \brief
 * This function initializes given queue.
 *
 * \param[in] Queue_pst
 * Pointer to the queue to initialize
 */
void NvM_Prv_Queue_Initialize(NvM_Prv_Queue_tst* Queue_pst)
{
    Queue_pst->idxHead_u16 = 0;
    Queue_pst->cntrFreeEntries_u16 = Queue_pst->size_cu16;
}

/**
 * \brief
 * This function returns whether the given queue is empty.
 *
 * \param[in] Queue_pcst
 * Pointer to the queue to be checked
 *
 * \return
 * - true = given queue is empty
 * - false = given queue has at least one element enqueued
 */
boolean NvM_Prv_Queue_IsEmpty(NvM_Prv_Queue_tst const* Queue_pcst)
{
    return (Queue_pcst->size_cu16 == Queue_pcst->cntrFreeEntries_u16);
}

/**
 * \brief
 * This function returns whether the given queue is full.
 *
 * \param[in] Queue_pcst
 * Pointer to the queue to be checked
 *
 * \return
 * - true = given queue is full
 * - false = given queue is not full, i.e. at least one element can be equeued
 */
boolean NvM_Prv_Queue_IsFull(NvM_Prv_Queue_tst const* Queue_pcst)
{
    return (0u == Queue_pcst->cntrFreeEntries_u16);
}

/**
 * \brief
 * This function returns number of free entries in the given queue.
 *
 * \param[in] Queue_pcst
 * Pointer to the queue for which number of free entries will be returned
 *
 * \return
 * Number of free entries
 */
uint16 NvM_Prv_Queue_GetFreeEntries(NvM_Prv_Queue_tst const* Queue_pcst)
{
    return Queue_pcst->cntrFreeEntries_u16;
}

/**
 * \brief
 * This function returns number of entries in the given queue.
 *
 * \param[in] Queue_pcst
 * Pointer to the queue for which number of entries will be returned
 *
 * \return
 * Number of entries
 */
uint16 NvM_Prv_Queue_GetEntries(NvM_Prv_Queue_tst const* Queue_pcst)
{
    return (Queue_pcst->size_cu16 - Queue_pcst->cntrFreeEntries_u16);
}

/**
 * \brief
 * This function enqueues an element into the given queue.
 *
 * \param[in] Queue_pst
 * Pointer to the queue to enqueue an element
 * \param[inout] QueueBuffer_pu8
 * Pointer to the queue buffer to enqueue an element
 * \param[in] ElementToEnqueue_pcu8
 * Constant pointer to the element to enqueue
 * \param[in] ElementSize_u32
 * Size of the element to enqueue in bytes
 *
 * \return
 * Index in the queue buffer where the element has been enqueued
 */
uint16 NvM_Prv_Queue_Enqueue(NvM_Prv_Queue_tst* Queue_pst,
                             uint8* QueueBuffer_pu8,
                             uint8 const* ElementToEnqueue_pcu8,
                             uint32 ElementSize_u32)
{
    uint16 cntrEntries_u16 = Queue_pst->size_cu16 - Queue_pst->cntrFreeEntries_u16;
    uint16 idxEnd_u16 = (Queue_pst->idxHead_u16 + cntrEntries_u16) % Queue_pst->size_cu16;

    rba_MemLib_MemCopy(ElementToEnqueue_pcu8,
                       &QueueBuffer_pu8[ElementSize_u32 * idxEnd_u16],
                       ElementSize_u32);

    --Queue_pst->cntrFreeEntries_u16;

    return idxEnd_u16;
}

/**
 * \brief
 * This function dequeues first element (head) from the given queue.
 * \details
 * This function only updates queue administrative data. The enqueued element remains in the queue and
 * will be automatically overwritten at further enqueuing actions.
 *
 * \param[in] Queue_pst
 * Pointer to the queue to dequeue an element
 */
void NvM_Prv_Queue_Dequeue(NvM_Prv_Queue_tst* Queue_pst)
{
    Queue_pst->idxHead_u16 = (Queue_pst->idxHead_u16 + 1u) % Queue_pst->size_cu16;
    ++Queue_pst->cntrFreeEntries_u16;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

