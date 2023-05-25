
#ifndef NVM_PRV_PROCESSMULTIBLOCK_H
#define NVM_PRV_PROCESSMULTIBLOCK_H

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
typedef boolean (*NvM_Prv_Multi_Cancel_tpfct)(void);

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern void NvM_Prv_Multi_Initialize(void);
extern boolean NvM_Prv_Multi_IsInProgress(void);
extern void NvM_Prv_Multi_SetResult(NvM_RequestResultType Result_uo);
extern NvM_RequestResultType NvM_Prv_Multi_GetResult(void);
extern void NvM_Prv_Multi_Cancel(NvM_Prv_Multi_Cancel_tpfct Cancel_pfct);
extern void NvM_Prv_Multi_GoToNextBlock(NvM_Prv_QueueEntry_tst* QueueEntry_pst);
extern NvM_Prv_idJob_ten NvM_Prv_Multi_Process(NvM_Prv_QueueEntry_tst* QueueEntry_pst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#endif /* NVM_PRV_PROCESSMULTIBLOCK_H */
