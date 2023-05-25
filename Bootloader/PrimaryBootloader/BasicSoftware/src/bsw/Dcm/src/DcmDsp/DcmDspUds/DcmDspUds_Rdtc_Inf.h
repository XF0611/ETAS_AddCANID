

#ifndef DCMDSPUDS_RDTC_INF_H
#define DCMDSPUDS_RDTC_INF_H


/*
 ***************************************************************************************************
 * Public Includes
 ***************************************************************************************************
 */

#include "Dcm.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_READDTCINFORMATION_ENABLED != DCM_CFG_OFF)
#include "Dem.h"
#include "DcmAppl.h"
#include "DcmDsp_Lib_Prv.h"


/*
 ***************************************************************************************************
 * Protected Includes (package wide includes)
 ***************************************************************************************************
 */

#include "DcmCore_DslDsd_Prot.h"
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Roe_Inf.h"
#endif

/*
 ***************************************************************************************************
 * Other Inline Functions
 ***************************************************************************************************
 */
#endif

/* _DCMDSPUDS_RDTC_INF_H                                                                          */
#endif

