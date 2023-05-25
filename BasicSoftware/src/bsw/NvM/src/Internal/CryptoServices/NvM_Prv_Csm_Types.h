
#ifndef NVM_PRV_CSM_TYPES_H
#define NVM_PRV_CSM_TYPES_H
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
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

/**
 * Definition of the structure for a Csm job result.
 */
typedef struct
{
    /// Status of the Csm job result:
    /// - true = result is valid, i.e. the Csm has set the result via callback
    /// - false = result is invalid, i.e. the Csm has not yet updated it or valid result has been already evaluated
    boolean         isResultValid_b;
    /// ID of the finished Csm job.
    /// During result evaluation this ID is used to check whether the finished Csm job is the one
    /// which has been started before.
    uint32          idJobCallback_uo;
    /// Actual Csm job result.
    Std_ReturnType  Result_uo;

} NvM_Prv_Csm_JobResult_tst;

#endif

/* NVM_PRV_CSM_TYPES_H */
#endif

