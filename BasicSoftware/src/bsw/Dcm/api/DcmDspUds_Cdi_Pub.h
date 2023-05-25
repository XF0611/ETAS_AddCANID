

#ifndef DCMDSPUDS_CDI_PUB_H
#define DCMDSPUDS_CDI_PUB_H

/**
 ***************************************************************************************************
            Clear diagnostics information (CDI) service
 ***************************************************************************************************
 */
#if(DCM_CFG_DSP_CLEARDIAGNOSTICINFORMATION_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_CDI_CONDITIONCHECK_ENABLED!=DCM_CFG_OFF)
#define DCM_START_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Pointer to the function which checks whether the mode rule is failed or not.
 */
extern boolean (*Dcm_CdiModeRuleRef_pf) (Dcm_NegativeResponseCodeType * Nrc_u8);
extern Std_ReturnType (*Dcm_CDIUserConditionCheck_pf) (uint32 GoDTC,Dcm_NegativeResponseCodeType * ErrorCode);

#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif
#endif   /* _DCMDSPUDS_CDI_PUB_H */

