

#ifndef DCMDSPOBD_MODE2_PRIV_H
#define DCMDSPOBD_MODE2_PRIV_H

/**************************************************************************************************/
/* Included  header files                                                                         */
/**************************************************************************************************/

#include "DcmDspObd_Mode2_Inf.h"

/**
 ***************************************************************************************************
            OBD MODE 02 (To read freeze frame data )
 ***************************************************************************************************
 */
#if (DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)
#if(DCM_CFG_DSP_OBDMODE2_ENABLED != DCM_CFG_OFF)

#define DCM_DSPMODE02_PID02   0x02u       /*PID 02 number which reports DTC number */
#define MAX_PID_LENGTH_DATA   0xffu       /*Maximum length of data bytes associated with PID */
#define DCM_OBDMODE02_SUPPPID 0x20u     /* Supported PIDS are multiples of 0x20*/

typedef struct /* To be used in Dcm_Prv_specialPID02 context */
{
    boolean flgGetDTCNum_b;               /* if TRUE, call DEM API to get DTC,else don't call DEM API */
    boolean isPIDFound_b;                 /* If PID is found to be configured */
    uint8 nrMultiple_u8;                  /* Number of special PIDS requested are multiples of 0x20 */
    uint8 idxPID_u8;                      /* Pid index in bit mask configuration list */
    uint8 nrPid_u8;                       /* Number of PIDs needs to be searched in PID configuration */
    uint8 nrPIDValid_qu8;                 /* Number of valid Pids in temp request buffer */
    uint8 nrPIDChk_qu8;                   /* Number of Pids in request format */
    uint8 idxPIDStart_qu8;                /* Start index in PID configuration */
    Std_ReturnType dataRetGet_u8;         /* Return type of API DcmStub_Dem_DcmReadDataOfOBDFreezeFrame */
    uint32 dataPIDBitMask_u32;            /* For supported PIDs,each PID is bit coded value of 4bytes */
    uint32 dataCalBitMask_u32;            /* Calculated Bit mask based on PID value */
    uint32 nrDTC_u32;                     /* DTC number for PID 02 */
    Dcm_MsgLenType nrResMaxDataLen_u32;   /* Maximum available response buffer */
    Dcm_MsgLenType nrResDataLen_u32;      /* Response data length */
}Dcm_ObdContextType;

#endif /*DCM_CFG_DSP_OBDMODE2_ENABLED*/

#endif   /* _DCMDSPOBD_MODE2_PRIV_H */
#endif
