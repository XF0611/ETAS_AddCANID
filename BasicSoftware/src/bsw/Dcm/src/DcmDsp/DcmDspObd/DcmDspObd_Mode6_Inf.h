

#ifndef DCMDSPOBD_MODE6_INF_H
#define DCMDSPOBD_MODE6_INF_H

/*
 ***************************************************************************************************
 * Public Includes
 ***************************************************************************************************
 */

#include "Dcm.h"

#if (DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)

#if (DCM_CFG_DET_SUPPORT_ENABLED != DCM_CFG_OFF)
#include "Det.h"
#endif

/* BSW-8793 */
#if((DCM_CFG_DSP_OBDMODE6_ENABLED != DCM_CFG_OFF) && (DCM_DSP_OBDMID_DTR_RB_SUPPORT == DCM_CFG_OFF))
#include "Dem.h"
#endif
/* END BSW-8793 */

/*
 ***************************************************************************************************
 * Protected Includes (package wide includes)
 ***************************************************************************************************
 */

#include "DcmCore_DslDsd_Prot.h"



/*
 ***************************************************************************************************
 * Other Inline Functions
 ***************************************************************************************************
 */

#endif
/* _DCMDSPOBD_MODE6_INF_H                                                                          */
#endif

