
#ifndef NVM_PRV_QUEUE_H
#define NVM_PRV_QUEUE_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Prv_Queue_Types.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern void NvM_Prv_Queue_Initialize(NvM_Prv_Queue_tst* Queue_pst);

extern boolean NvM_Prv_Queue_IsEmpty(NvM_Prv_Queue_tst const* Queue_pcst);

extern boolean NvM_Prv_Queue_IsFull(NvM_Prv_Queue_tst const* Queue_pcst);

extern uint16 NvM_Prv_Queue_GetFreeEntries(NvM_Prv_Queue_tst const* Queue_pcst);

extern uint16 NvM_Prv_Queue_GetEntries(NvM_Prv_Queue_tst const* Queue_pcst);

extern uint16 NvM_Prv_Queue_Enqueue(NvM_Prv_Queue_tst* Queue_pst,
                                    uint8* QueueBuffer_pu8,
                                    uint8 const* ElementToEnqueue_pcu8,
                                    uint32 ElementSize_u32);

extern void NvM_Prv_Queue_Dequeue(NvM_Prv_Queue_tst* Queue_pst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* NVM_PRV_QUEUE_H */
#endif

