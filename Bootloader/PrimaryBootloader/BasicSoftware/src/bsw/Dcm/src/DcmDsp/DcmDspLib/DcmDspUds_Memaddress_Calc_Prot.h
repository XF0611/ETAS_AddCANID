

#ifndef DCMDSPUDS_MEMADDRESS_CALC_PROT_H
#define DCMDSPUDS_MEMADDRESS_CALC_PROT_H


/**
 *********************************************************************************************************************
           Used by Read Memory By Address (RMBA)/Write Memory By Address (RMBA) service/ RequestUpload Service (0x35)/TransferData Service (0x36)
 *********************************************************************************************************************
 */

#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && ((DCM_CFG_DSP_READMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF) || \
													   (DCM_CFG_DSP_WRITEMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF)|| \
                                                       (DCM_CFG_DSP_REQUESTUPLOAD_ENABLED != DCM_CFG_OFF)       || \
                                                       (DCM_CFG_DSP_TRANSFERDATA_ENABLED !=DCM_CFG_OFF)))

#if ((DCM_CFG_DSP_REQUESTUPLOAD_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_REQUESTDOWNLOAD_ENABLED != DCM_CFG_OFF))
typedef struct
{
    uint32 dataMemoryAddress_u32;          /* Memory address requested by RequestUpload/RequestDownload service from where the data has to be read */
    uint32 dataMemorySize_u32;             /* Memory size requested bu RequestUpload/RequestDownload service */
    uint32 dataMaxBlockLength_u32;         /* The max block length returned for RequestUpload/RequestDownload service from ECU */
    uint32 nrMaxBlockSequenceNum_u32;
    uint8  dataMemoryIdentifier_u8;        /* Memory identifier requested by RequestUpload/RequestDownload service */
    boolean isUploadStarted_b;             /* Flag to indicate whether RequestUpload request is received before TransferData service request */
    boolean isDownloadStarted_b;           /* Flag to indicate whether RequestDownload request is received before TransferData service request */
    uint8  compressionMethod_u8;
} Dcm_DataTransfer_tst;
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_DataTransfer_tst Dcm_DataTransfer_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if(DCM_CFG_DSP_TRANSFERDATA_ENABLED != DCM_CFG_OFF)
typedef struct
{
    uint32 dataUploadedDataLength_u32;             /* The variable to hold the total length of data uploaded to the client */
    uint32 nrCompletedTransfer_u32;                 /* Number of transfer data calls completed */
    uint32 cntCurrentBlockCount;
    uint32 dataTransferedDataSize_u32;
} Dcm_TransferData_tst;
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_TransferData_tst Dcm_TransferData_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern boolean Dcm_GetRequestUploadStatus(void);
extern boolean Dcm_GetRequestDownloadStatus(void);
extern void Dcm_DspDeactivateRequestUploadDownloadPermission(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&& ((DCM_CFG_DSP_REQUESTUPLOAD_ENABLED != DCM_CFG_OFF)|| \
                                                     (DCM_CFG_DSP_REQUESTDOWNLOAD_ENABLED != DCM_CFG_OFF)))
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_UpdateTransferRequestCount(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_Prv_DspReqUploadConfirmation(
	Dcm_IdContextType dataIdContext_u8,
	PduIdType dataRxPduId_u8,
	uint16 dataSourceAddress_u16,
	Dcm_ConfirmationStatusType status_u8
														);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_GetMemoryInfo(uint8 dataSize_u8,
											 const uint8 * adrReqBuf_pcu8,
											 uint32 * adrMemAddrLength_pu32);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif

#if( DCM_CFG_DSP_REQUESTTRANSFEREXIT_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_Prv_DspReqTrfExitConfirmation(
	Dcm_IdContextType dataIdContext_u8,
	PduIdType dataRxPduId_u8,
	uint16 dataSourceAddress_u16,
	Dcm_ConfirmationStatusType status_u8
										                   );
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if( DCM_CFG_DSP_REQUESTDOWNLOAD_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_Prv_DspReqDownloadConfirmation(
	Dcm_IdContextType dataIdContext_u8,
	PduIdType dataRxPduId_u8,
	uint16 dataSourceAddress_u16,
	Dcm_ConfirmationStatusType status_u8
														     );
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#endif   /* _DCMDSPUDS_MEMADDRESS_CALC_PROT_H */

