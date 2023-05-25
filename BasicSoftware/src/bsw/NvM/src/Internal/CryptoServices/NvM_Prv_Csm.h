
#ifndef NVM_PRV_CSM_H
#define NVM_PRV_CSM_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Prv_Job_Types.h"
#include "NvM_Prv_Crypto_ServiceTypes.h"

#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
/// Type definition of function to start a cryptographic service on the Csm.
typedef NvM_Prv_JobResult_ten (*NvM_Prv_Csm_StartJob_tpfct)(uint32 idJobCsm_uo,
                                                            NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst);

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
# define NVM_START_SEC_CODE
# include "NvM_MemMap.h"

extern boolean NvM_Prv_Csm_GetJobResult(NvM_Prv_Crypto_ServiceResult_tst* JobResult_pst);

extern NvM_Prv_JobResult_ten NvM_Prv_Csm_StartRandomGeneration(uint32 idJobCsm_uo,
                                                               NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst);

extern NvM_Prv_JobResult_ten NvM_Prv_Csm_StartEncryption(uint32 idJobCsm_uo,
                                                         NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst);

extern NvM_Prv_JobResult_ten NvM_Prv_Csm_StartDecryption(uint32 idJobCsm_uo,
                                                         NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst);

extern NvM_Prv_JobResult_ten NvM_Prv_Csm_StartEncryptionAead(uint32 idJobCsm_uo,
                                                             NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst);

extern NvM_Prv_JobResult_ten NvM_Prv_Csm_StartDecryptionAead(uint32 idJobCsm_uo,
                                                             NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst);

extern NvM_Prv_JobResult_ten NvM_Prv_Csm_StartSignatureGeneration(uint32 idJobCsm_uo,
                                                                  NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst);

extern NvM_Prv_JobResult_ten NvM_Prv_Csm_StartSignatureVerification(uint32 idJobCsm_uo,
                                                                    NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst);

# define NVM_STOP_SEC_CODE
# include "NvM_MemMap.h"

#endif

/* NVM_PRV_CSM_H */
#endif

