
/*<VersionHead>
 ***************************************************************************************************
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Nm / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 ***************************************************************************************************
 </VersionHead>*/

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
/* Nm interface main header file */
#include "Nm.h"
/* Nm interface private header file */
#include "Nm_Priv.h"

/*start of declarations*/

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static Std_ReturnType Nm_DummyRequestBusSynchronization(
		const NetworkHandleType nmChannelHandle);

static Std_ReturnType Nm_DummyRequestBusSynchronization(
		const NetworkHandleType nmChannelHandle) {
	(void) nmChannelHandle;
	/* [REQ] Return E_NOT_OK without executing any functionality */
	return (E_NOT_OK);

}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static Std_ReturnType Nm_DummyCheckRemoteSleepIndication(
		const NetworkHandleType nmChannelHandle,
		boolean *const nmRemoteSleepIndPtr);

static Std_ReturnType Nm_DummyCheckRemoteSleepIndication(
		const NetworkHandleType nmChannelHandle,
		boolean *const nmRemoteSleepIndPtr) {
	(void) nmChannelHandle;
	*nmRemoteSleepIndPtr = *nmRemoteSleepIndPtr;

	/* [REQ] Return E_NOT_OK without executing any functionality */
	return (E_NOT_OK);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static Std_ReturnType Nm_DummySetUserData(
		const NetworkHandleType nmChannelHandle,
		const uint8 *const nmUserDataPtr);

static Std_ReturnType Nm_DummySetUserData(
		const NetworkHandleType nmChannelHandle,
		const uint8 *const nmUserDataPtr) {
	(void) nmChannelHandle;
	(void) nmUserDataPtr;

	/* [REQ] Return E_NOT_OK without executing any functionality */
	return (E_NOT_OK);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static Std_ReturnType Nm_DummySetSleepReadyBit(
		const NetworkHandleType nmChannelHandle, const boolean setBit);

static Std_ReturnType Nm_DummySetSleepReadyBit(
		const NetworkHandleType nmChannelHandle, const boolean setBit) {
	(void) nmChannelHandle;
	(void) setBit;

	/* [REQ] Return E_NOT_OK without executing any functionality */
	return (E_NOT_OK);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
const Nm_ConfigType Nm_ConfData_cs[NM_NUMBER_OF_CHANNELS] = {
// Channel 0
		{ 0,             // Shutdown delay time
#if (NM_STATE_REPORT_ENABLED != STD_OFF)
        NM_NMS_REPORT_DISABLED,                            // NMS report not used for the channel
#endif
				NM_BUS_TYPE_CAN,     // BusNm Type
				NM_ACTIVE_CHANNEL,     // Channel Type
				0xFF,      // Index of the cluster this channel belongs to
				0,               // Handle of the ComM channel
				FALSE,     //ChannelSleepMaster   
				FALSE,     //SynchronizingNetwork
				TRUE,     // Node detection enabled
				TRUE,     // Node Id enabled
				FALSE,     // Repeat message id enabled      
		} };
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
Nm_NetworkRamType_tst Nm_ChannelData_s[NM_NUMBER_OF_CHANNELS];
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"

#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
Nm_GlobalRamType_tst Nm_GlobalData_s[NM_NUMBER_OF_CLUSTERS];
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"

#define NM_START_SEC_VAR_CLEARED_32
#include "Nm_MemMap.h"
Nm_TimerType SwFRTimer;
#define NM_STOP_SEC_VAR_CLEARED_32
#include "Nm_MemMap.h"

#endif

/* APIs of BusNm */

#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
const Nm_BusNmApiType_tst Nm_BusNmApi[] = { { &CanNm_NetworkRelease,
		&CanNm_NetworkRequest, &CanNm_PassiveStartUp,
		&Nm_DummyRequestBusSynchronization, &Nm_DummyCheckRemoteSleepIndication,
		&CanNm_DisableCommunication, &CanNm_EnableCommunication,
		&CanNm_GetLocalNodeIdentifier, &CanNm_GetNodeIdentifier,
		&CanNm_GetPduData, &CanNm_GetState, &CanNm_GetUserData,
		&CanNm_RepeatMessageRequest, &Nm_DummySetUserData,
		&Nm_DummySetSleepReadyBit } };
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

/* Call-back functions called by Nm, defined by User application */

#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
const Nm_UserCallbackType_tst Nm_UserCallbacks = {
/* PduRxIndication */
&Nm_RxIndicationCallback,

//RemoteSleepCancel
		&TestNm_RemoteSleepCancellation,

		//RemoteSleepInd
		&TestNm_RemoteSleepIndication,

		//StateChangeInd
		&Nm_StateChangeIndication

};
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

