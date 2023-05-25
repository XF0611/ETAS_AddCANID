
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Seca_Inf.h"
#include "Rte_Dcm.h"
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Seca_Prv.h"

#if(DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 *******************************************************************************
 * Dcm_Prv_DspSecurityConfirmation : API used for confirmation of response sent
 *								              for SecurityAccess (0x27) service.
 * \param           dataIdContext_u8      Service Id
 * \param           dataRxPduId_u8        PDU Id on which request is Received
 * \param           dataSourceAddress_u16 Tester Source address id
 * \param           status_u8                Status of Tx confirmation function
 *
 * \retval          None
 * \seealso
 *
 *******************************************************************************
 */
 void Dcm_Prv_DspSecurityConfirmation(
	Dcm_IdContextType dataIdContext_u8,
	PduIdType dataRxPduId_u8,
	uint16 dataSourceAddress_u16,
	Dcm_ConfirmationStatusType status_u8)
{
	uint8 dataSecLevel_u8;

/* The security level requested is activated for positive as well as negative
   confirmations of a positive response.This is to avoid a situation of
   instability if the Application has already prepared for a change in
   security,but the Tx confirmation of response to tester is negative*/
/* Check if POS response sent successfully and Dcm_DspChgSecLevelis set to
   TRUE */
    if (((status_u8 == DCM_RES_POS_OK) || (status_u8 == DCM_RES_POS_NOT_OK))
											&& (Dcm_DspChgSecLevel_b != FALSE))
    {
        /* Calculate the security level */
        dataSecLevel_u8 = (uint8)((Dcm_DspSecAccType_u8 + 1u)>>1u);

        /* Unlock security level */
        Dcm_Prv_SetSecurityLevel (dataSecLevel_u8);
        /* Resetting Stored AccessType */
        Dcm_DspSecAccType_u8 = 0x0;
        /* Resetting Security Index */
        Dcm_DspSecTabIdx_u8  = 0x0;
    }
    /* Resetting Change Security */

    Dcm_DspChgSecLevel_b   = FALSE;
DcmAppl_DcmConfirmation(dataIdContext_u8,dataRxPduId_u8,dataSourceAddress_u16,
																    status_u8);
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif

