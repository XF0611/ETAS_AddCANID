
#ifndef NVM_PRV_CRYPTO_H
#define NVM_PRV_CRYPTO_H
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

extern void NvM_Prv_Crypto_DoStateMachine(NvM_Prv_stJob_ten* stJob_pen,
                                          NvM_Prv_JobResult_tst* JobResult_pst,
                                          NvM_Prv_JobData_tst const* JobData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* NVM_PRV_CRYPTO_H */
#endif

