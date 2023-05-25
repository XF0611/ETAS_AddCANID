

#ifndef DCMDSPOBD_MODE9_PRIV_H
#define DCMDSPOBD_MODE9_PRIV_H

/**************************************************************************************************/
/* Included  header files                                                                         */
/**************************************************************************************************/

#include "DcmDspObd_Mode9_Inf.h"

/**
 ***************************************************************************************************
            OBD service 0x09
 ***************************************************************************************************
 */
#if (DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)
#if(DCM_CFG_DSP_OBDMODE9_ENABLED != DCM_CFG_OFF)

#define DCM_OBDMODE09_REQ_LEN_MIN   0x00u     /* Minimum Request Length for OBD MODE 0x09              */
#define DCM_OBDMODE09_REQ_LEN       0x01u     /* Request Length for INFOTYPEs which report data values */
#define DCM_OBDMODE09_REQ_LEN_MAX   0x07u     /* Maximum Request Length for OBD MODE 0x09 is 6 bytes   */
#define DCM_OBDMODE09_SUPPINFOTYPE  0x20u     /* supported INFOTYPES are multiples of 0x20             */
#define DCM_OBDMODE09_INFOTYPE16    0x16u     /* Infotype 16                                           */
#define DCM_OBDMODE09_INFOTYPE29    0x29u     /* Infotype 29                                           */
#define DCM_OBDMODE09_NODIITID16_29 0x01u     /* NODI shall be reported as 1 for  ITID16 till ITID29  */


/* Definitions of States in the service */
typedef enum
{
    DCM_DSP_MODE09_UNINIT,                 /* Uninit state */
    DCM_DSP_MODE09_INIT,                   /* Default state */
    DCM_DSP_MODE09_RUNNING                 /* State to process non-supported Infotypes  */
}Dcm_DspMode09Type_ten;

typedef struct /* To be used in OBD_Mode9 context */
{
    uint8 idxInfoType_u8; /* INFOTYPE index in bit mask configuration list */
    uint8_least  nrInfoTypeChk_qu8; /* Number of INFOTYPEs in the request */
    uint8 adrTmpBuf_au8[DCM_OBDMODE09_REQ_LEN_MAX]; /* Temporary buffer maximum of 6 INFOTYPEs */
    Std_ReturnType dataInfotypeFnResult_u8;
    Dcm_MsgLenType nrReqDataLen_u32; /* Requested data length check */
    uint32 dataInfoTypeMaskVal_u32; /* Bit mask value for supported INFOTYPEs */
}Dcm_ObdMode9ContextType;
#endif


#endif   /* _DCMDSPOBD_MODE9_PRIV_H */
#endif
