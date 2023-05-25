
/*
 ***************************************************************************************************
 *                                   Includes
 ***************************************************************************************************
 */
#include "EcuM.h" /*BSW_HeaderInc_002*/

#include "Mcu.h"
// #if (!defined(MCU_AR_RELEASE_MAJOR_VERSION) || (MCU_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(MCU_AR_RELEASE_MINOR_VERSION) || ((MCU_AR_RELEASE_MINOR_VERSION != 2) && (MCU_AR_RELEASE_MINOR_VERSION != 3)))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif

/* Switch to either enable or disable call to COMM functions */
#if( ECUM_INCLUDE_COMM == STD_ON )
#include "ComM.h"
// #if (!defined(COMM_AR_RELEASE_MAJOR_VERSION) || (COMM_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(COMM_AR_RELEASE_MINOR_VERSION) || (COMM_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif
#endif
/****************************************************************************************************
 *                                   CONFIGURATION PARAMETERS
 ***************************************************************************************************/

#define ECUM_START_SEC_CONST_8
#include "EcuM_MemMap.h"

/*************************************************************************************************
 Container Name    : EcuMWakeupSource
 Description       : Contains the list of PNC references linked to each WakeupSource
 ****************************************************************************************************/

#define ECUM_STOP_SEC_CONST_8
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CONST_UNSPECIFIED
#include "EcuM_MemMap.h"

/*************************************************************************************************
 Container Name    : EcuMWakeupSource
 Description       : Contains the list of all the PNC references related to the WakeupSource container as a whole.
 ****************************************************************************************************/
const EcuM_Cfg_dataWkupPNCRefStruct_tst EcuM_Cfg_dataWkupPNCRef_cast[ECUM_NUM_OF_WKP_SOURCES] =
		{ { 0, NULL_PTR }, { 0, NULL_PTR }, { 0, NULL_PTR }, { 0, NULL_PTR }, {
				0, NULL_PTR }, { 0, NULL_PTR }, { 0, NULL_PTR } };
#define ECUM_STOP_SEC_CONST_UNSPECIFIED
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_CONST_UNSPECIFIED
#include "EcuM_MemMap.h"

/*The structure remains same as EcuM3.x implementation. ResetReason has to be acquired from Mcu configuration*/
/*************************************************************************************************
Container Name    : EcuMWakeupSource
Description       : Contains all the Pre Compile Parameters related to WakeupSource container.
****************************************************************************************************/
const EcuM_Cfg_dataWkupSrcStruct_tst EcuM_Cfg_idxWakeupSourcesPC_au32[ECUM_NUM_OF_WKP_SOURCES] =
{/*IsComChannelPresent,       ComChannelReferance,                     ResetReason,             ValidationTimeout,   IsWakeupSourcePolling,                WakeupSourceId,                     CheckWakeupTimeout                  */
		{ FALSE, 255, McuConf_McuResetReasonConf_McuResetReasonConf, 0, TRUE,
				EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_POWER, 0 }, { FALSE,
				255, McuConf_McuResetReasonConf_McuResetReasonConf_001, 0, TRUE,
				EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_RESET, 0 }, { FALSE,
				255, McuConf_McuResetReasonConf_McuResetReasonConf_002, 0, TRUE,
				EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_RESET, 0 },
				{ FALSE, 255, McuConf_McuResetReasonConf_McuResetReasonConf_003,
						0, TRUE,
						EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_WDG, 0 },
				{ FALSE, 255, McuConf_McuResetReasonConf_McuResetReasonConf_003,
						0, TRUE,
						EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_EXTERNAL_WDG, 0 },
				{ TRUE,
						ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS,
						McuConf_McuResetReasonConf_McuResetReasonConf, 0, FALSE,
						EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_CAN, 0 },
				{ FALSE, 255, McuConf_McuResetReasonConf_McuResetReasonConf, 0,
						FALSE,
						EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_GPT, 0 } };

#if  (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)
/*********************************************************************************************************************
 Container Name    : EcuMSleepMode
 Description       : Contains all the Pre Compile Parameters related to SleepModes container
 *********************************************************************************************************************/

const EcuM_Cfg_dataSleepModeStruct_tst EcuM_Cfg_idxSleepModes_au32[ECUM_CFG_NUM_SLEEP_MODES] =
		{/*    IsCPUSuspend   ,   EquivalentMcuMode  ,     WakeupSourceMask */

		{ FALSE, McuConf_McuModeSettingConf_McuModeSettingConf_0, 0x1fu } };
#endif//ECUM_SLEEP_SUPPORT_ENABLE

#define ECUM_STOP_SEC_CONST_UNSPECIFIED
#include "EcuM_MemMap.h"

#if  (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)

#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
/*The array of structure containing a list of wakeupsources configured with checkwakeuptimeout and flag to indicate status of wakeup check*/
/*************************************************************************************************
 Container Name    : EcuMWakeupSource
 Description       : Contains all the Pre Compile Parameters related to WakeupSource container.
 ****************************************************************************************************/
volatile EcuM_Cfg_CheckWakeupSrc_tst EcuM_Cfg_CheckWakeupTimer_ast[ECUM_NUM_OF_WKP_SOURCES] =
		{/* WakeupSourceId,                                 CheckWakeupTimeout,               CheckWakeupStatus                  */

		{ EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_POWER, 0, ECUM_FLAG_RESET_E },
				{ EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_RESET, 0,
						ECUM_FLAG_RESET_E }, {
						EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_RESET,
						0, ECUM_FLAG_RESET_E }, {
						EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_WDG, 0,
						ECUM_FLAG_RESET_E }, {
						EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_EXTERNAL_WDG, 0,
						ECUM_FLAG_RESET_E }, {
						EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_CAN, 0,
						ECUM_FLAG_RESET_E }, {
						EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_GPT, 0,
						ECUM_FLAG_RESET_E } };
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

#endif//ECUM_SLEEP_SUPPORT_ENABLE

#define ECUM_START_SEC_CONST_16
#include "EcuM_MemMap.h"

/*************************************************************************************************
 Container Name    : EcuMGoDownAllowedUsers
 Description       : Contains Module ID's configured for calling EcuM_GoDown
 ****************************************************************************************************/
const uint16 EcuM_Cfg_idxGoDownValidCallerArr_au16[ECUM_CFG_GODOWN_CALLER_ARR_LEN] =
		{ 0 };

#define ECUM_STOP_SEC_CONST_16
#include "EcuM_MemMap.h"

