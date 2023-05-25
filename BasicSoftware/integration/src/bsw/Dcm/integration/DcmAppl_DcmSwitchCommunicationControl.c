/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */



#include "DcmCore_DslDsd_Inf.h"
#include "DcmCore_DslDsd_Pub.h"
#include "Com.h"
#include "Nm.h"
/*TESTCODE-START
#include "DcmTest.h"
TESTCODE-END*/
#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && ( DCM_CFG_DSP_COMMUNICATIONCONTROL_ENABLED != DCM_CFG_OFF) )

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCM_TPL
 * DcmAppl_DcmSwitchCommunicationControl :-\n
 * This api will be called after sending the confirmation for the UDS service Communication control(0x28).\n
 * Dcm will pass the network ID of the channel and the new requested Communication mode to this api.Application can perform required actions if any.
 * @param[in]  NetworkID        : Channel Id for which the communication mode is requested.
 * @param[in]  RequestedMode    : Requested CommunicationMode
 * @retval     None
 */
/*zco7sgh: after session timeout, communicaton restore to default status*/
Com_IpduGroupVector BswM_GddIpduGroupVector;
#define BSWM_MAX_IPDUGROUP_INDEX 2u
void DcmAppl_DcmSwitchCommunicationControl( uint8 NetworkID ,Dcm_CommunicationModeType RequestedMode)
{
	(void)(NetworkID);
	(void)(RequestedMode);
	VAR(uint8, AUTOMATIC) idxIndex_u8;
	/*TESTCODE-START
	DcmTest_DcmAppl_DcmSwitchCommunicationControl(NetworkID,RequestedMode);
	TESTCODE-END*/

	if (DCM_ENABLE_RX_TX_NORM_NM == RequestedMode)
	{
		for(idxIndex_u8=0; idxIndex_u8 < BSWM_MAX_IPDUGROUP_INDEX; idxIndex_u8++ )
		{
			Com_SetIpduGroup(BswM_GddIpduGroupVector, idxIndex_u8, TRUE);
		}
		Com_IpduGroupControl(BswM_GddIpduGroupVector, FALSE);

		for(idxIndex_u8=0; idxIndex_u8 < (uint8)NM_NUMBER_OF_CHANNELS; idxIndex_u8++ )
		{
			(void)CanNm_EnableCommunication((NetworkHandleType)idxIndex_u8);
		}
	}
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif /* (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF) */
