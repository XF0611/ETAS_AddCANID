

#ifndef DCMDSPOBD_MODE37A_PRIV_H
#define DCMDSPOBD_MODE37A_PRIV_H

/**************************************************************************************************/
/* Included  header files                                                                         */
/**************************************************************************************************/

#include "DcmDspObd_Mode37A_Inf.h"

/**
 ***************************************************************************************************
            OBD service 0x03, 0x07 and 0x0A
 ***************************************************************************************************
 */
#if (DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)
#if(DCM_CFG_DSP_OBDMODE37A_ENABLED != DCM_CFG_OFF)
#define DCM_OBDMODE37A_REQLEN   0x00u

#endif  /* DCM_CFG_DSP_OBDMODE37A_ENABLED */


#endif   /* _DCMDSPOBD_MODE37A_PRIV_H */
#endif
