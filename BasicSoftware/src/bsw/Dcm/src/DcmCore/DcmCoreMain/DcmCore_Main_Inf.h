

#ifndef DCMCORE_MAIN_INF_H
#define DCMCORE_MAIN_INF_H

/**
 ***************************************************************************************************
 * \moduledescription
 *                      Interfaces header file for DcmCore DSLDSD
 *
 ***************************************************************************************************
 */


/*
 ***************************************************************************************************
 * Public Includes
 ***************************************************************************************************
 */

#include "Dcm.h"
#include "ComM_Dcm.h"


#include "Dcm_Cfg_SchM.h"
#if(DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF)
#include "SchM_Dcm.h"
#endif
#if (DCM_CFG_DET_SUPPORT_ENABLED != DCM_CFG_OFF)
#include "Det.h"
#endif

/*
 ***************************************************************************************************
 * Protected Includes (package wide includes)
 ***************************************************************************************************
 */
#include "DcmCore_DslDsd_Prot.h"
#include "Dcm_Dsl_Priv.h"

#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Roe_Inf.h"
#endif

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Uds_Prot.h"
#include "DcmDspUds_Rc_Prot.h"

#if((DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_DDDISTORINGTONVRAM_ENABLED != DCM_CFG_OFF))
#include "DcmDspUds_Dddi_Prot.h"
#endif

#if(DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Iocbi_Prot.h"
#endif

#if(DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Cdtcs_Prot.h"
#endif

#endif
#include "DcmAppl.h"

/* _DCMCORE_MAIN_INF_H                                                                          */
#endif

