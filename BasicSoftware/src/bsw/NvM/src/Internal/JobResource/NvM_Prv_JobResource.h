
#ifndef NVM_PRV_JOBRESOURCE_H
#define NVM_PRV_JOBRESOURCE_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Prv_Job_Types.h"

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
enum
{
    NvM_Prv_idJobResource_MemIf_e,
    NvM_Prv_idJobResource_Crypto_e,
    NvM_Prv_idJobResource_Crc_e,

    NvM_Prv_idJobResource_NrJobResources_e

};

/// Type definition for IDs of job resources used by NvM.
typedef uint8 NvM_Prv_idJobResource_tuo;


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern void NvM_Prv_JobResource_Init(void);

extern void NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_tuo idJobResource_uo,
                                               NvM_Prv_stJob_ten* stJob_pen,
                                               NvM_Prv_JobResult_tst* JobResult_pst,
                                               NvM_Prv_JobData_tst const* JobData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* NVM_PRV_JOBRESOURCE_H */
#endif

