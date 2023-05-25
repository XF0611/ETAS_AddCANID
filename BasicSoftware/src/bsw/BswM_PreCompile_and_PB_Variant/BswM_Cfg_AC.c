
/***********************************************************************************************************************
 * Include files
 **********************************************************************************************************************/
#include "BswM.h"

#if (BSWM_RTE_IN_USE == TRUE)
#include "Rte_BswM.h"
#include "SchM_BswM.h"
#include "Rte_Main.h"
#endif
#include "BswM_Prv.h"

/***********************************************************************************************************************
 * Type Definition
 **********************************************************************************************************************/
typedef void (*BswM_Cfg_vFunPtrv_t)(void);
typedef Std_ReturnType (*BswM_Cfg_u8FunPtrv_t)(void);

/* User Include Files */
#include "BswM_PBDefine.h"

/***********************************************************************************************************************
 * BswMComMAllowCom Action/s Object.
 * # ComMAllowed
 * # NetworkReference
 ***********************************************************************************************************************/
#define BSWM_START_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"

const BswM_Cfg_AC_ComMAllowComType_tst BswM_Cfg_AC_BswMComMAllowCom_cast[BSWM_NO_OF_AC_COMM_ALLOW_COM] =
		{ {
		/* BswM_AI_ComMCommAllowed_Can_Network_ETAS */
		TRUE, ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS },
				{
				/* BswM_AI_ComMCommNotAllowed_Can_Network_ETAS */
				FALSE,
						ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS } };

#define BSWM_STOP_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"    

/***********************************************************************************************************************
 * BswMComMModeSwitch Action/s Object.
 * # ComMRequestMode
 * # ComUserReference
 ***********************************************************************************************************************/
#define BSWM_START_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"

const BswM_Cfg_AC_ComMModeSwitchType_tst BswM_Cfg_AC_BswMComMModeSwitch_cast[BSWM_NO_OF_AC_COMM_MODE_SWITCH] =
		{
				{
				/* BswM_AI_ComMReqFullComm_Can_Network_ETAS */
				BSWM_FULL_COM,
						ComMConf_ComMUser_ComMUser_Can_Network_ETAS_Channel_Can_Network_ETAS },
				{
				/* BswM_AI_ComMReqNoComm_Can_Network_ETAS */
				BSWM_NO_COM,
						ComMConf_ComMUser_ComMUser_Can_Network_ETAS_Channel_Can_Network_ETAS } };

#define BSWM_STOP_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"    

/***********************************************************************************************************************
 * BswMNMControl Action/s Object.
 * # NMControl Action Type
 * # NetworkHandleRef
 ***********************************************************************************************************************/
#define BSWM_START_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"

const BswM_Cfg_AC_NmControlType_tst BswM_Cfg_AC_BswMNMControl_cast[BSWM_NO_OF_AC_NM_CNTRL] =
		{
				{
				/* BswM_AI_NM_Start */
				BSWM_NM_ENABLE,
						ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS },
				{
				/* BswM_AI_NM_Stop */
				BSWM_NM_DISABLE,
						ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS } };

#define BSWM_STOP_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"    

/***********************************************************************************************************************
 * BswMRteStart Action/s Object.
 * # Dummy variable
 ***********************************************************************************************************************/
#define BSWM_START_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"

const BswM_Cfg_AC_RteStartType_tst BswM_Cfg_AC_BswMRteStart_cast[BSWM_NO_OF_AC_RTE_START] =
		{
		/* BswMRteStart */
		0 };

#define BSWM_STOP_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"    

/***********************************************************************************************************************
 * IDs for SchMSwitch Functions.
 * # ID
 ***********************************************************************************************************************/
#define BSWM_START_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"

const BswM_Cfg_AC_SchMSwitchType_tst BswM_Cfg_AC_BswMSchMSwitch_cast[BSWM_NO_OF_AC_SCHM_SWITCH] =
		{ {
		/* BswM_AI_BswMSwitchAppRun */
		0 }, {
		/* BswM_AI_BswMSwitchPostRun */
		1 }, {
		/* BswM_AI_BswMSwitchPrepShutdown */
		2 }, {
		/* BswM_AI_BswMSwitchRun */
		3 }, {
		/* BswM_AI_BswMSwitchShutdown */
		4 }, {
		/* BswM_AI_BswMSwitchStartupTwo */
		5 }, {
		/* BswM_AI_SwitchAppMode */
		6 } };

#define BSWM_STOP_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"    

#define BSWM_START_SEC_CODE 
#include "BswM_MemMap.h"                
/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_SchMSwitch_0_BswM_AI_BswMSwitchAppRun
 * Description     :       SchMSwitch Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       Std_ReturnType
 * Remarks         :   
 *****************************************************************************************/
static Std_ReturnType BswM_Cfg_AC_SchMSwitch_0_BswM_AI_BswMSwitchAppRun(void) {
	Std_ReturnType dataRet_u8 = BSWM_NO_RET_VALUE;
	Std_ReturnType action_RetVal_pu8 = BSWM_NO_RET_VALUE;

	/*
	 SchM_Switch_p_o( mode )
	 P = Port name
	 O = Mode Group within the ModeSwitchInterface
	 */

	dataRet_u8 =
			SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE(
					RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_APP_RUN);

	/* return whether the action was sucessful or not */
	switch (dataRet_u8) {
	case RTE_E_OK:
		action_RetVal_pu8 = E_OK;
		break;
	case RTE_E_LIMIT:
		action_RetVal_pu8 = E_NOT_OK;
		break;
	default:
		action_RetVal_pu8 = BSWM_NO_RET_VALUE;
		break;
	}

	return action_RetVal_pu8;
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_SchMSwitch_1_BswM_AI_BswMSwitchPostRun
 * Description     :       SchMSwitch Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       Std_ReturnType
 * Remarks         :   
 *****************************************************************************************/
static Std_ReturnType BswM_Cfg_AC_SchMSwitch_1_BswM_AI_BswMSwitchPostRun(void) {
	Std_ReturnType dataRet_u8 = BSWM_NO_RET_VALUE;
	Std_ReturnType action_RetVal_pu8 = BSWM_NO_RET_VALUE;

	/*
	 SchM_Switch_p_o( mode )
	 P = Port name
	 O = Mode Group within the ModeSwitchInterface
	 */

	dataRet_u8 =
			SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE(
					RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_POST_RUN);

	/* return whether the action was sucessful or not */
	switch (dataRet_u8) {
	case RTE_E_OK:
		action_RetVal_pu8 = E_OK;
		break;
	case RTE_E_LIMIT:
		action_RetVal_pu8 = E_NOT_OK;
		break;
	default:
		action_RetVal_pu8 = BSWM_NO_RET_VALUE;
		break;
	}

	return action_RetVal_pu8;
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_SchMSwitch_2_BswM_AI_BswMSwitchPrepShutdown
 * Description     :       SchMSwitch Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       Std_ReturnType
 * Remarks         :   
 *****************************************************************************************/
static Std_ReturnType BswM_Cfg_AC_SchMSwitch_2_BswM_AI_BswMSwitchPrepShutdown(
		void) {
	Std_ReturnType dataRet_u8 = BSWM_NO_RET_VALUE;
	Std_ReturnType action_RetVal_pu8 = BSWM_NO_RET_VALUE;

	/*
	 SchM_Switch_p_o( mode )
	 P = Port name
	 O = Mode Group within the ModeSwitchInterface
	 */

	dataRet_u8 =
			SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE(
					RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_PREP_SHUTDOWN);

	/* return whether the action was sucessful or not */
	switch (dataRet_u8) {
	case RTE_E_OK:
		action_RetVal_pu8 = E_OK;
		break;
	case RTE_E_LIMIT:
		action_RetVal_pu8 = E_NOT_OK;
		break;
	default:
		action_RetVal_pu8 = BSWM_NO_RET_VALUE;
		break;
	}

	return action_RetVal_pu8;
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_SchMSwitch_3_BswM_AI_BswMSwitchRun
 * Description     :       SchMSwitch Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       Std_ReturnType
 * Remarks         :   
 *****************************************************************************************/
static Std_ReturnType BswM_Cfg_AC_SchMSwitch_3_BswM_AI_BswMSwitchRun(void) {
	Std_ReturnType dataRet_u8 = BSWM_NO_RET_VALUE;
	Std_ReturnType action_RetVal_pu8 = BSWM_NO_RET_VALUE;

	/*
	 SchM_Switch_p_o( mode )
	 P = Port name
	 O = Mode Group within the ModeSwitchInterface
	 */

	dataRet_u8 =
			SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE(
					RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_RUN);

	/* return whether the action was sucessful or not */
	switch (dataRet_u8) {
	case RTE_E_OK:
		action_RetVal_pu8 = E_OK;
		break;
	case RTE_E_LIMIT:
		action_RetVal_pu8 = E_NOT_OK;
		break;
	default:
		action_RetVal_pu8 = BSWM_NO_RET_VALUE;
		break;
	}

	return action_RetVal_pu8;
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_SchMSwitch_4_BswM_AI_BswMSwitchShutdown
 * Description     :       SchMSwitch Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       Std_ReturnType
 * Remarks         :   
 *****************************************************************************************/
static Std_ReturnType BswM_Cfg_AC_SchMSwitch_4_BswM_AI_BswMSwitchShutdown(
		void) {
	Std_ReturnType dataRet_u8 = BSWM_NO_RET_VALUE;
	Std_ReturnType action_RetVal_pu8 = BSWM_NO_RET_VALUE;

	/*
	 SchM_Switch_p_o( mode )
	 P = Port name
	 O = Mode Group within the ModeSwitchInterface
	 */

	dataRet_u8 =
			SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE(
					RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_SHUTDOWN);

	/* return whether the action was sucessful or not */
	switch (dataRet_u8) {
	case RTE_E_OK:
		action_RetVal_pu8 = E_OK;
		break;
	case RTE_E_LIMIT:
		action_RetVal_pu8 = E_NOT_OK;
		break;
	default:
		action_RetVal_pu8 = BSWM_NO_RET_VALUE;
		break;
	}

	return action_RetVal_pu8;
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_SchMSwitch_5_BswM_AI_BswMSwitchStartupTwo
 * Description     :       SchMSwitch Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       Std_ReturnType
 * Remarks         :   
 *****************************************************************************************/
static Std_ReturnType BswM_Cfg_AC_SchMSwitch_5_BswM_AI_BswMSwitchStartupTwo(
		void) {
	Std_ReturnType dataRet_u8 = BSWM_NO_RET_VALUE;
	Std_ReturnType action_RetVal_pu8 = BSWM_NO_RET_VALUE;

	/*
	 SchM_Switch_p_o( mode )
	 P = Port name
	 O = Mode Group within the ModeSwitchInterface
	 */

	dataRet_u8 =
			SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE(
					RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_STARTUP_TWO);

	/* return whether the action was sucessful or not */
	switch (dataRet_u8) {
	case RTE_E_OK:
		action_RetVal_pu8 = E_OK;
		break;
	case RTE_E_LIMIT:
		action_RetVal_pu8 = E_NOT_OK;
		break;
	default:
		action_RetVal_pu8 = BSWM_NO_RET_VALUE;
		break;
	}

	return action_RetVal_pu8;
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_SchMSwitch_6_BswM_AI_SwitchAppMode
 * Description     :       SchMSwitch Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       Std_ReturnType
 * Remarks         :   
 *****************************************************************************************/
static Std_ReturnType BswM_Cfg_AC_SchMSwitch_6_BswM_AI_SwitchAppMode(void) {
	Std_ReturnType dataRet_u8 = BSWM_NO_RET_VALUE;
	Std_ReturnType action_RetVal_pu8 = BSWM_NO_RET_VALUE;

	/*
	 SchM_Switch_p_o( mode )
	 P = Port name
	 O = Mode Group within the ModeSwitchInterface
	 */

	dataRet_u8 =
			SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE(
					RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_POST_RUN);

	/* return whether the action was sucessful or not */
	switch (dataRet_u8) {
	case RTE_E_OK:
		action_RetVal_pu8 = E_OK;
		break;
	case RTE_E_LIMIT:
		action_RetVal_pu8 = E_NOT_OK;
		break;
	default:
		action_RetVal_pu8 = BSWM_NO_RET_VALUE;
		break;
	}

	return action_RetVal_pu8;
}

#define BSWM_STOP_SEC_CODE 
#include "BswM_MemMap.h"            

/***********************************************************************************************************************
 * Array of Functions Pointers
 * # FunctionNames
 ***********************************************************************************************************************/
#define BSWM_START_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"             
static const BswM_Cfg_u8FunPtrv_t BswM_Cfg_AC_BswMSchMSwitch_FunPtr_cafct[BSWM_NO_OF_AC_SCHM_SWITCH] =
		{ &BswM_Cfg_AC_SchMSwitch_0_BswM_AI_BswMSwitchAppRun,
				&BswM_Cfg_AC_SchMSwitch_1_BswM_AI_BswMSwitchPostRun,
				&BswM_Cfg_AC_SchMSwitch_2_BswM_AI_BswMSwitchPrepShutdown,
				&BswM_Cfg_AC_SchMSwitch_3_BswM_AI_BswMSwitchRun,
				&BswM_Cfg_AC_SchMSwitch_4_BswM_AI_BswMSwitchShutdown,
				&BswM_Cfg_AC_SchMSwitch_5_BswM_AI_BswMSwitchStartupTwo,
				&BswM_Cfg_AC_SchMSwitch_6_BswM_AI_SwitchAppMode };

#define BSWM_STOP_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"         

/*****************************************************************************************
 * Function name   :       BswM_Cfg_ProcessSchMSwitchFunction
 * Description     :       Determine which SchMSwitch Function to be invoked.
 * Parameter       :       SchMSwitchId
 * Return value    :       Std_ReturnType
 * Remarks         :   
 *****************************************************************************************/
#define BSWM_START_SEC_CODE 
#include "BswM_MemMap.h"
Std_ReturnType BswM_Cfg_ProcessSchMSwitchFunction(uint16 idSchMSwitch_u16) {
	Std_ReturnType RetVal = BSWM_NO_RET_VALUE;

	/* Check if the idSchMSwitch_u16 is within the array boundary */
	if (idSchMSwitch_u16
			< (sizeof(BswM_Cfg_AC_BswMSchMSwitch_FunPtr_cafct)
					/ sizeof(BswM_Cfg_u8FunPtrv_t))) {
		RetVal = BswM_Cfg_AC_BswMSchMSwitch_FunPtr_cafct[idSchMSwitch_u16]();
	} else {
		/* Do Nothing */
	}
	return RetVal;
}
#define BSWM_STOP_SEC_CODE 
#include "BswM_MemMap.h"

/***********************************************************************************************************************
 * UserIDs for UserCallout Functions.
 * # UserID
 ***********************************************************************************************************************/
#define BSWM_START_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"

const BswM_Cfg_AC_UserCalloutType_tst BswM_Cfg_AC_BswMUserCallout_cast[BSWM_NO_OF_AC_USERCALLOUT] =
		{ {
		/* BswM_AI_CanIfInit */
		0 }, {
		/* BswM_AI_CanNm_Init */
		1 }, {
		/* BswM_AI_CanSmInit */
		2 }, {
		/* BswM_AI_CanTp_Init */
		3 }, {
		/* BswM_AI_CanTrcv_GotoNormal */
		4 }, {
		/* BswM_AI_CanTrcv_GotoSleep */
		5 }, {
		/* BswM_AI_CanTrcv_Init */
		6 }, {
		/* BswM_AI_ComInit */
		7 }, {
		/* BswM_AI_ComMInit */
		8 }, {
		/* BswM_AI_Dcm_Init */
		9 }, {
		/* BswM_AI_DemInit */
		10 }, {
		/* BswM_AI_DemShutdown */
		11 }, {
		/* BswM_AI_Dem_EnableStorage */
		12 }, {
		/* BswM_AI_Dem_EndOperation */
		13 }, {
		/* BswM_AI_Dem_ExtendedData_Init */
		14 }, {
		/* BswM_AI_EcuM_SelectShutdownTarget_RESET_IO */
		15 }, {
		/* BswM_AI_EcuM_SelectShutdownTarget_RESET_MCU */
		16 }, {
		/* BswM_AI_Enable_PeripheralSply */
		17 }, {
		/* BswM_AI_FeeInit */
		18 }, {
		/* BswM_AI_FeeInitEnd */
		19 }, {
		/* BswM_AI_FimInit */
		20 }, {
		/* BswM_AI_GoDown */
		21 }, {
		/* BswM_AI_Nm_Init */
		22 }, {
		/* BswM_AI_NvMInit */
		23 }, {
		/* BswM_AI_NvMReadAll */
		24 }, {
		/* BswM_AI_PduRInit */
		25 }, {
		/* BswM_AI_Rtm_Fee_ProcessEnd */
		26 }, {
		/* BswM_AI_Rtm_Fee_ProcessStart */
		27 }, {
		/* BswM_AI_Rtm_NvM_ProcessEnd */
		28 }, {
		/* BswM_AI_Rtm_NvM_ProcessStart */
		29 }, {
		/* BswM_AI_Run_CddInitZero */
		30 }, {
		/* BswM_AI_SelfHold */
		31 }, {
		/* BswM_AI_SelfHold_Unlock */
		32 }, {
		/* BswM_AI_StartupTwo_CddInit */
		33 }, {
		/* BswM_AI_WdgMInit */
		34 }, {
		/* BswM_AI_XcpInit */
		35 }, {
		/* BswM_Al_Run_CddInitOne */
		36 } };

#define BSWM_STOP_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"    

#define BSWM_START_SEC_CODE 
#include "BswM_MemMap.h"                
/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_0_BswM_AI_CanIfInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_0_BswM_AI_CanIfInit(void) {
	CanIf_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_1_BswM_AI_CanNm_Init
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_1_BswM_AI_CanNm_Init(void) {
	CanNm_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_2_BswM_AI_CanSmInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_2_BswM_AI_CanSmInit(void) {
	CanSM_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_3_BswM_AI_CanTp_Init
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_3_BswM_AI_CanTp_Init(void) {
	CanTp_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_4_BswM_AI_CanTrcv_GotoNormal
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_4_BswM_AI_CanTrcv_GotoNormal(void) {
	BswM_CanTrcv_GotoNormal();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_5_BswM_AI_CanTrcv_GotoSleep
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_5_BswM_AI_CanTrcv_GotoSleep(void) {
	BswM_CanTrcv_GotoSleep();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_6_BswM_AI_CanTrcv_Init
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_6_BswM_AI_CanTrcv_Init(void) {
	CanTrcv_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_7_BswM_AI_ComInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_7_BswM_AI_ComInit(void) {
	Com_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_8_BswM_AI_ComMInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_8_BswM_AI_ComMInit(void) {
	ComM_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_9_BswM_AI_Dcm_Init
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_9_BswM_AI_Dcm_Init(void) {
	Dcm_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_10_BswM_AI_DemInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_10_BswM_AI_DemInit(void) {
	Dem_Init();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_11_BswM_AI_DemShutdown
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_11_BswM_AI_DemShutdown(void) {
	Dem_Shutdown();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_12_BswM_AI_Dem_EnableStorage
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_12_BswM_AI_Dem_EnableStorage(void) {
	Dem_SetEnableCondition(0, TRUE);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_13_BswM_AI_Dem_EndOperation
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_13_BswM_AI_Dem_EndOperation(void) {
	Dem_SetOperationCycleState(0, DEM_CYCLE_STATE_END);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_14_BswM_AI_Dem_ExtendedData_Init
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_14_BswM_AI_Dem_ExtendedData_Init(void) {
	Dem_ExtendedData_Init();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_15_BswM_AI_EcuM_SelectShutdownTarget_RESET_IO
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_15_BswM_AI_EcuM_SelectShutdownTarget_RESET_IO(
		void) {
	EcuM_SelectShutdownTarget(ECUM_SHUTDOWN_TARGET_RESET, ECUM_RESET_IO);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_16_BswM_AI_EcuM_SelectShutdownTarget_RESET_MCU
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_16_BswM_AI_EcuM_SelectShutdownTarget_RESET_MCU(
		void) {
	EcuM_SelectShutdownTarget(ECUM_SHUTDOWN_TARGET_RESET, ECUM_RESET_MCU);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_17_BswM_AI_Enable_PeripheralSply
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_17_BswM_AI_Enable_PeripheralSply(void) {
	BswM_Enable_PeripheralSply();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_18_BswM_AI_FeeInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_18_BswM_AI_FeeInit(void) {
	Fee_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_19_BswM_AI_FeeInitEnd
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_19_BswM_AI_FeeInitEnd(void) {
	Fee_Rb_EndInit();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_20_BswM_AI_FimInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_20_BswM_AI_FimInit(void) {
	FiM_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_21_BswM_AI_GoDown
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_21_BswM_AI_GoDown(void) {
	EcuM_GoDown(0);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_22_BswM_AI_Nm_Init
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_22_BswM_AI_Nm_Init(void) {
	Nm_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_23_BswM_AI_NvMInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_23_BswM_AI_NvMInit(void) {
	NvM_Init();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_24_BswM_AI_NvMReadAll
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_24_BswM_AI_NvMReadAll(void) {
	NvM_Integration_ReadAll();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_25_BswM_AI_PduRInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_25_BswM_AI_PduRInit(void) {
	PduR_Init (&PduR_Config);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_26_BswM_AI_Rtm_Fee_ProcessEnd
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_26_BswM_AI_Rtm_Fee_ProcessEnd(void) {
	Rtm_ProcessEnd (Rtm_Mon_Cfg_ProcessId_Fee_Init);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_27_BswM_AI_Rtm_Fee_ProcessStart
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_27_BswM_AI_Rtm_Fee_ProcessStart(void) {
	Rtm_ProcessStart (Rtm_Mon_Cfg_ProcessId_Fee_Init);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_28_BswM_AI_Rtm_NvM_ProcessEnd
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_28_BswM_AI_Rtm_NvM_ProcessEnd(void) {
	Rtm_ProcessEnd (Rtm_Mon_Cfg_ProcessId_NvM_Init);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_29_BswM_AI_Rtm_NvM_ProcessStart
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_29_BswM_AI_Rtm_NvM_ProcessStart(void) {
	Rtm_ProcessStart (Rtm_Mon_Cfg_ProcessId_NvM_Init);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_30_BswM_AI_Run_CddInitZero
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_30_BswM_AI_Run_CddInitZero(void) {
	BswM_Run_CddInitZero();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_31_BswM_AI_SelfHold
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_31_BswM_AI_SelfHold(void) {
	BswM_PerformSelfHold();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_32_BswM_AI_SelfHold_Unlock
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_32_BswM_AI_SelfHold_Unlock(void) {
	BswM_SelfHold_Unlock();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_33_BswM_AI_StartupTwo_CddInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_33_BswM_AI_StartupTwo_CddInit(void) {
	BswM_StartupTwo_CddInit();
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_34_BswM_AI_WdgMInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_34_BswM_AI_WdgMInit(void) {
	WdgM_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_35_BswM_AI_XcpInit
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_35_BswM_AI_XcpInit(void) {
	Xcp_Init (NULL_PTR);
}

/*****************************************************************************************
 * Function name   :       BswM_Cfg_AC_UserCallout_36_BswM_Al_Run_CddInitOne
 * Description     :       UserCallout Function is invoked for Action.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
static void BswM_Cfg_AC_UserCallout_36_BswM_Al_Run_CddInitOne(void) {
	BswM_Run_CddInitOne();
}

#define BSWM_STOP_SEC_CODE 
#include "BswM_MemMap.h"            

/***********************************************************************************************************************
 * Array of Functions Pointers
 * # FunctionNames
 ***********************************************************************************************************************/
#define BSWM_START_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"             
static const BswM_Cfg_vFunPtrv_t BswM_Cfg_AC_BswMUserCallout_FunPtr_cafct[BSWM_NO_OF_AC_USERCALLOUT] =
		{ &BswM_Cfg_AC_UserCallout_0_BswM_AI_CanIfInit,
				&BswM_Cfg_AC_UserCallout_1_BswM_AI_CanNm_Init,
				&BswM_Cfg_AC_UserCallout_2_BswM_AI_CanSmInit,
				&BswM_Cfg_AC_UserCallout_3_BswM_AI_CanTp_Init,
				&BswM_Cfg_AC_UserCallout_4_BswM_AI_CanTrcv_GotoNormal,
				&BswM_Cfg_AC_UserCallout_5_BswM_AI_CanTrcv_GotoSleep,
				&BswM_Cfg_AC_UserCallout_6_BswM_AI_CanTrcv_Init,
				&BswM_Cfg_AC_UserCallout_7_BswM_AI_ComInit,
				&BswM_Cfg_AC_UserCallout_8_BswM_AI_ComMInit,
				&BswM_Cfg_AC_UserCallout_9_BswM_AI_Dcm_Init,
				&BswM_Cfg_AC_UserCallout_10_BswM_AI_DemInit,
				&BswM_Cfg_AC_UserCallout_11_BswM_AI_DemShutdown,
				&BswM_Cfg_AC_UserCallout_12_BswM_AI_Dem_EnableStorage,
				&BswM_Cfg_AC_UserCallout_13_BswM_AI_Dem_EndOperation,
				&BswM_Cfg_AC_UserCallout_14_BswM_AI_Dem_ExtendedData_Init,
				&BswM_Cfg_AC_UserCallout_15_BswM_AI_EcuM_SelectShutdownTarget_RESET_IO,
				&BswM_Cfg_AC_UserCallout_16_BswM_AI_EcuM_SelectShutdownTarget_RESET_MCU,
				&BswM_Cfg_AC_UserCallout_17_BswM_AI_Enable_PeripheralSply,
				&BswM_Cfg_AC_UserCallout_18_BswM_AI_FeeInit,
				&BswM_Cfg_AC_UserCallout_19_BswM_AI_FeeInitEnd,
				&BswM_Cfg_AC_UserCallout_20_BswM_AI_FimInit,
				&BswM_Cfg_AC_UserCallout_21_BswM_AI_GoDown,
				&BswM_Cfg_AC_UserCallout_22_BswM_AI_Nm_Init,
				&BswM_Cfg_AC_UserCallout_23_BswM_AI_NvMInit,
				&BswM_Cfg_AC_UserCallout_24_BswM_AI_NvMReadAll,
				&BswM_Cfg_AC_UserCallout_25_BswM_AI_PduRInit,
				&BswM_Cfg_AC_UserCallout_26_BswM_AI_Rtm_Fee_ProcessEnd,
				&BswM_Cfg_AC_UserCallout_27_BswM_AI_Rtm_Fee_ProcessStart,
				&BswM_Cfg_AC_UserCallout_28_BswM_AI_Rtm_NvM_ProcessEnd,
				&BswM_Cfg_AC_UserCallout_29_BswM_AI_Rtm_NvM_ProcessStart,
				&BswM_Cfg_AC_UserCallout_30_BswM_AI_Run_CddInitZero,
				&BswM_Cfg_AC_UserCallout_31_BswM_AI_SelfHold,
				&BswM_Cfg_AC_UserCallout_32_BswM_AI_SelfHold_Unlock,
				&BswM_Cfg_AC_UserCallout_33_BswM_AI_StartupTwo_CddInit,
				&BswM_Cfg_AC_UserCallout_34_BswM_AI_WdgMInit,
				&BswM_Cfg_AC_UserCallout_35_BswM_AI_XcpInit,
				&BswM_Cfg_AC_UserCallout_36_BswM_Al_Run_CddInitOne };

#define BSWM_STOP_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"         

/*****************************************************************************************
 * Function name   :       BswM_Cfg_ProcessUserCalloutFunction
 * Description     :       Determine which UserCallout Function to be invoked.
 * Parameter       :       UserId
 * Return value    :       void
 * Remarks         :   
 *****************************************************************************************/
#define BSWM_START_SEC_CODE 
#include "BswM_MemMap.h"
void BswM_Cfg_ProcessUserCalloutFunction(uint16 idUserCallout_u16) {
	/* Check if the idUserCallout_u16 is within the array boundary */
	if (idUserCallout_u16
			< (sizeof(BswM_Cfg_AC_BswMUserCallout_FunPtr_cafct)
					/ sizeof(BswM_Cfg_vFunPtrv_t))) {
		BswM_Cfg_AC_BswMUserCallout_FunPtr_cafct[idUserCallout_u16]();
	} else {
		/* Do Nothing */
	}
}
#define BSWM_STOP_SEC_CODE 
#include "BswM_MemMap.h"

/**********************************************************************************************************************
 * End of file                                                                                                     
 **********************************************************************************************************************/
