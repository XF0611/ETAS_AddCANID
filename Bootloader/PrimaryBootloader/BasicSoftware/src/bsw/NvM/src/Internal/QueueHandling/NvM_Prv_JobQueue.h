
#ifndef NVM_PRV_JOBQUEUE_H
#define NVM_PRV_JOBQUEUE_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Prv_Job_Types.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern void NvM_Prv_JobQueue_Initialize(void);

extern void NvM_Prv_JobQueue_Enqueue(NvM_Prv_Job_tst const* Job_pcst);

extern void NvM_Prv_JobQueue_Dequeue(void);

extern boolean NvM_Prv_JobQueue_IsFull(void);

extern boolean NvM_Prv_JobQueue_IsEmpty(void);

extern NvM_Prv_Job_tst* NvM_Prv_JobQueue_GetEntry(uint16 idxJob_u16);

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
extern uint8* NvM_Prv_JobQueue_GetUsedIntBfr(NvM_BlockIdType idBlock_uo, NvM_Prv_idService_tuo idService_uo);
extern NvM_Prv_JobData_tst* NvM_Prv_JobQueue_GetJobData(NvM_BlockIdType idBlock_uo,
                                                        NvM_Prv_idService_tuo idService_uo);
#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* NVM_PRV_JOBQUEUE_H */
#endif

