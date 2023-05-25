

#ifndef DCMDSPUDS_CDTCS_PROT_H
#define DCMDSPUDS_CDTCS_PROT_H

#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF) )
#include "Dem.h"
#define DCM_START_SEC_VAR_INIT_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern boolean Dcm_DspCDTCStatus_b;
#define DCM_STOP_SEC_VAR_INIT_BOOLEAN/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/*Function for mode rule check for re-enabling CDTC*/
extern boolean Dcm_ControlDtcSettingModecheck_b (Dcm_NegativeResponseCodeType * NegRespCode_u8);
extern void Dcm_Prv_Cdtcs_Mainfunction(void);

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif
#endif   /* _DCMDSPUDS_CDTCS_PROT_H */

