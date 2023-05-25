
#ifndef NVM_PRV_QUEUE_TYPES_H
#define NVM_PRV_QUEUE_TYPES_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
/**
 * \brief
 * Definition of the structure which defines a queue implemented by a circular buffer.
 * \details
 * This structure contains only administrative data required for a circular buffer.
 * The user of a queue has to provide a buffer to store the elements.
 */
typedef struct
{
    /// Index of the first element (head) in the queue.
    /// The head of the queue is the oldest element so it will be dequeued as first.
    uint16          idxHead_u16;
    /// Counter of free entries currently available.
    uint16          cntrFreeEntries_u16;
    /// Maximum number of entries the queue can maintain.
    uint16 const    size_cu16;

} NvM_Prv_Queue_tst;

/* NVM_PRV_QUEUE_TYPES_H */
#endif

