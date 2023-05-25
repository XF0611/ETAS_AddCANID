
#ifndef NVM_PRV_EXPLICITSYNCHRONIZATION_H
#define NVM_PRV_EXPLICITSYNCHRONIZATION_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Prv_BlockData.h"
/* BSW-8494 */
#include "NvM_Prv_SizeRamMirror.h"
/* END BSW-8494 */
/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern boolean NvM_Prv_ExplicitSync_CheckBlockSizes(void);
extern NvM_Prv_JobResult_ten NvM_Prv_ExplicitSync_CopyData(NvM_Prv_ExplicitSync_Copy_tpfct CopyFunction_pfct,
                                                           NvM_BlockIdType idBlock_uo,
                                                           uint8 const* cntrExpSyncOperations_pcu8,
                                                           uint8* RamBlock_pu8);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* NVM_PRV_EXPLICITSYNCHRONIZATION_H */
#endif
