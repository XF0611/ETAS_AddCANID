


#ifndef DCMDSPUDS_CDI_PRV_H
#define DCMDSPUDS_CDI_PRV_H

#include "Dcm.h"

#if (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_CLEARDIAGNOSTICINFORMATION_ENABLED != DCM_CFG_OFF)

#include "DcmCore_DslDsd_Prot.h"
#include "DcmDsp_Lib_Prv.h"


#define DCM_CDI_REQ_LEN              0x03u     /* Request length for CDI */
#define DCM_POS_DTC_HIGHBYTE         0x00u     /* High byte number in request */
#define DCM_POS_DTC_MIDDLEBYTE       0x01u     /* Middle byte number in request */
#define DCM_POS_DTC_LOWBYTE          0x02u     /* Low byte number in request */
#define DCM_POS_DTC_BIT16            16u       /*shift DTC to 16 bits */
#define DCM_POS_DTC_BIT8             8u         /*shift DTC to 8  bits */
#define DCM_DTC_GRP_MASK             0x00FFFFFFu


#if (DCM_CFG_DSP_CDI_CONDITIONCHECK_ENABLED!=DCM_CFG_OFF)
typedef boolean (*Cdi_ModeRuleRef_pf) (Dcm_NegativeResponseCodeType* Nrc_u8);
typedef Std_ReturnType (*CDIUserConditionCheck_pf)(uint32 GoDTC,Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#endif
#endif

#endif   /* _DCMDSPUDS_CDI_PRV_H */

