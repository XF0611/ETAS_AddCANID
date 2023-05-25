

#ifndef NVM_CBK_H
#define NVM_CBK_H

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern void NvM_Csm_Callback(uint32 jobId, Std_ReturnType result);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* NVM_CBK_H */
#endif

