

#ifndef DCMDSPUDS_IOCBI_PROT_H
#define DCMDSPUDS_IOCBI_PROT_H


/**
 ***************************************************************************************************
            Input Output Control By Identifier (IOCBI) service
 ***************************************************************************************************
 */

#if (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF)

/* Definitions of states of IOCBI service */
typedef enum
{
    DCM_IOCBI_IDLESTATE,              		        /* Idle state */
    DCM_IOCBI_FCS_ACTIVE,						/* Freeze current state for a DID is active */
    DCM_IOCBI_FCS_PENDING,						/* Freeze current state for a DID is pending */
    DCM_IOCBI_RTD_ACTIVE,						/* Reset to default state for a DID is active */
    DCM_IOCBI_RTD_PENDING,         				/* Reset to default state for a DID is pending*/
    DCM_IOCBI_STA_ACTIVE,          				/* Short term adjustment for a DID is active */
    DCM_IOCBI_STA_PENDING,             			/* Short term adjustment for a DID is pending */
    DCM_IOCBI_RCE_ACTIVE,              			/* Return Control To Ecu for a DID is active*/
    DCM_IOCBI_RCE_PENDING             			/* Return Control To Ecu for a DID is pending*/
  }Dcm_Dsp_IocbiDidStatus_ten;

/*Structure to store the index of IOCBI DIDs and their status*/
typedef struct
{
	uint16    	idxindex_u16;
	Dcm_Dsp_IocbiDidStatus_ten IocbiStatus_en;
} Dcm_Dsp_IocbiStatusType_tst;


/**
 **********************************************************************************************************************
		Array for storing Active IOCBI DIDs and their active status
 **********************************************************************************************************************
**/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_Dsp_IocbiStatusType_tst DcmDsp_IocbiStatus_array[DCM_CFG_NUM_IOCBI_DIDS];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_ResetActiveIoCtrl(uint32 dataSessionMask_u32,
												 uint32 dataSecurityMask_u32,
												 boolean flgSessChkReqd_b);
extern void Dcm_Prv_DspIOCBIConfirmation(
	Dcm_IdContextType dataIdContext_u8,
	PduIdType dataRxPduId_u8,
	uint16 dataSourceAddress_u16,
	Dcm_ConfirmationStatusType status_u8
													    );
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif   /* _DCMDSPUDS_IOCBI_PROT_H */

