/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "BswM.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */

#define BSWM_CFG_VERSION_INFO  {    /*Version details*/  \
                    6U, /*Vendor Id*/  \
                    42U, /*Module Id*/  \
                    8U, /*SW Major version*/  \
                    0U, /*SW Minor version*/  \
                    0U /*SW Patch version*/  \
               }

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

#define BSWM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED 
#include "BswM_MemMap.h"

/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
static const BswM_Cfg_ActionListType_tst BswM_Cfg_AL_cast[16];

/* Definition for Rule Structures */

static const BswM_Cfg_RuleType_tst BswM_Cfg_Rule_cast[18] = { { /*   Rule Structure for the configuration container BswM_AR_AppRun2PostRun    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_AppRun2PostRun, &BswM_Cfg_AL_cast[1], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_DISABLE_NM    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_DISABLE_NM, &BswM_Cfg_AL_cast[9], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_DISABLE_RX_ENABLE_TX_NORM    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_ENABLE_TX_NORM,
		&BswM_Cfg_AL_cast[6], /* trueActionList */
		NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_DISABLE_RX_TX_NORM    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_TX_NORM, &BswM_Cfg_AL_cast[8], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_DISABLE_RX_TX_NORM_NM    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_TX_NORM_NM,
		&BswM_Cfg_AL_cast[7], /* trueActionList */
		NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_ENABLE_NM    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_ENABLE_NM, &BswM_Cfg_AL_cast[13], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_ENABLE_RX_DISABLE_TX_NORM    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_DISABLE_TX_NORM,
		&BswM_Cfg_AL_cast[10], /* trueActionList */
		NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_ENABLE_RX_TX_NORM    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_TX_NORM, &BswM_Cfg_AL_cast[11], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_ENABLE_RX_TX_NORM_NM    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_TX_NORM_NM,
		&BswM_Cfg_AL_cast[12], /* trueActionList */
		NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_EcuReset_HARD    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_EcuReset_HARD, &BswM_Cfg_AL_cast[14], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_DCM_EcuReset_SOFT    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_DCM_EcuReset_SOFT, &BswM_Cfg_AL_cast[15], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_PostRun2AppRun    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_PostRun2AppRun, &BswM_Cfg_AL_cast[0], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_PostRun2PrepShutdown    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_PostRun2PrepShutdown, &BswM_Cfg_AL_cast[2], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_Run2AppRun    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_Run2AppRun, &BswM_Cfg_AL_cast[0], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_Run2PrepShutdown    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_Run2PrepShutdown, &BswM_Cfg_AL_cast[2], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_Shutdown    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_PrepShutdown2Shutdown, &BswM_Cfg_AL_cast[4], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_StartupOne2StartupTwo    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_StartupOne2StartupTwo, &BswM_Cfg_AL_cast[5], /* trueActionList */
NULL_PTR }, { /*   Rule Structure for the configuration container BswM_AR_StartupTwo2Run    */
BSWM_FALSE, &BswM_Cfg_LE_BswM_LE_StartupTwo2Run, &BswM_Cfg_AL_cast[3], /* trueActionList */
NULL_PTR } };

/*Array of BswMRule indexes associated with ModeReqPort BswM_MRP_ComM_Can_Network_ETAS. Variant :  __KW_COMMON*/
static const uint16 BswM_Cfg_ListOfRules_BswM_MRP_ComM_Can_Network_ETAS_cau16[] =
		{ 12 };

/* ModeRequestPort structure for MRP BswM_MRP_ComM_Can_Network_ETAS. Variant : __KW_COMMON
 * - ModeInit Value Present flag (isModeInitValuePresent_b) 
 * - NetworkID (idNetwork_u8)
 * - ModeInit Value (dataModeInitValue_en)
 * - RequestProcessing (dataReqProcessing_en)
 * - Number of associated rules (nrAssociatedRules_u16)
 * - Reference to ListOfRules Array (adrRulesRef_pu16)
 */
#define BSWM_CFG_BSWM_MRP_COMM_CAN_NETWORK_ETAS { \
        FALSE,      \
        ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS,       \
        COMM_NO_COMMUNICATION,        \
        BSWM_DEFERRED,      \
        1,       \
        &BswM_Cfg_ListOfRules_BswM_MRP_ComM_Can_Network_ETAS_cau16[0]        \
}

/* Array of ComM Indication MRP structs. Variant : __KW_COMMON */
#define BSWM_CFG_COMMINDICATION_MRPS {   \
       BSWM_CFG_BSWM_MRP_COMM_CAN_NETWORK_ETAS,      \
}

/*Array of BswMRule indexes associated with ModeReqPort BswM_MRP_DCM_ComControl_Can_Network_ETAS. Variant :  __KW_COMMON*/
static const uint16 BswM_Cfg_ListOfRules_BswM_MRP_DCM_ComControl_Can_Network_ETAS_cau16[] =
		{ 1, 2, 3, 4, 5, 6, 7, 8 };

/* ModeRequestPort structure for MRP BswM_MRP_DCM_ComControl_Can_Network_ETAS. Variant : __KW_COMMON
 * - ModeInit Value Present flag (isModeInitValuePresent_b) 
 * - NetworkHandleType (idNetwork_u8)
 * - ModeInit Value (dataModeInitValue_en)
 * - RequestProcessing (dataReqProcessing_en)
 * - Number of associated rules (nrAssociatedRules_u16)
 * - Reference to ListOfRules Array (adrRulesRef_pu16)
 */
#define BSWM_CFG_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS { \
        FALSE,      \
        ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS,       \
        DCM_ENABLE_RX_TX_NORM,        \
        BSWM_IMMEDIATE,      \
        8,       \
        &BswM_Cfg_ListOfRules_BswM_MRP_DCM_ComControl_Can_Network_ETAS_cau16[0]        \
}

/* Array of Dcm Com Mode Request MRP structs. Variant : __KW_COMMON */
#define BSWM_CFG_DCMCOMMODEREQ_MRPS {   \
       BSWM_CFG_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS,      \
}

/*Array of BswMRule indexes associated with ModeReqPort BswM_MRP_WKSRC_PWR. Variant :  __KW_COMMON*/
static const uint16 BswM_Cfg_ListOfRules_BswM_MRP_WKSRC_PWR_cau16[] = { 0, 11,
		12, 13, 14 };

/* ModeRequestPort structure for MRP BswM_MRP_WKSRC_PWR. Variant : __KW_COMMON
 * - Pointer to array containing indexes of associated rules 
 * - Number of rules referring to this MRP
 * - EcuM Wakeup Source reference 
 * - Configured/Default Mode Init Value 
 * - RequestProcessing Enum - Immediate/Deferred 
 * - Mode Init Value present flag
 */
#define BSWM_CFG_BSWM_MRP_WKSRC_PWR { \
    &BswM_Cfg_ListOfRules_BswM_MRP_WKSRC_PWR_cau16[0],  \
    5,          \
    EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_POWER,    \
    0,         \
    BSWM_DEFERRED,       \
    FALSE        \
}

/* Array of EcuM WakeupSource MRP structs. Variant : __KW_COMMON */
#define BSWM_CFG_ECUMWKPSRC_MRPS {   \
    BSWM_CFG_BSWM_MRP_WKSRC_PWR,      \
}

/*Array of BswMRule indexes associated with ModeReqPort BswM_MRP_BSWM_MDG. Variant :  __KW_COMMON*/
static const uint16 BswM_Cfg_ListOfRules_BswM_MRP_BSWM_MDG_cau16[] = { 0, 11,
		12, 13, 14, 15, 16, 17 };

/* ModeRequestPort structure for MRP BswM_MRP_BSWM_MDG. Variant : __KW_COMMON
 * - ModeInit Value Present flag (isModeInitValuePresent_b) 
 * - UserId (idUser_u16)
 * - Max requested mode (dataModeMax_u16)
 * - ModeInit Value (dataModeInitValue_u16)
 * - RequestProcessing (dataReqProcessing_en)
 * - Number of associated rules (nrAssociatedRules_u16)
 * - Reference to ListOfRules Array (adrRulesRef_pu16)
 */
#define BSWM_CFG_BSWM_MRP_BSWM_MDG { \
        FALSE,        \
        0,    \
        65535,                \
        0,         \
        BSWM_IMMEDIATE,    \
        8, \
        &BswM_Cfg_ListOfRules_BswM_MRP_BSWM_MDG_cau16[0]  \
}

/* ModeRequestPort structure for MRP BswM_MRP_DCM_DiagnosticSessionControl. Variant : __KW_COMMON
 * - ModeInit Value Present flag (isModeInitValuePresent_b) 
 * - UserId (idUser_u16)
 * - Max requested mode (dataModeMax_u16)
 * - ModeInit Value (dataModeInitValue_u16)
 * - RequestProcessing (dataReqProcessing_en)
 * - Number of associated rules (nrAssociatedRules_u16)
 * - Reference to ListOfRules Array (adrRulesRef_pu16)
 */
#define BSWM_CFG_BSWM_MRP_DCM_DIAGNOSTICSESSIONCONTROL { \
        TRUE,        \
        1,    \
        65535,                \
        RTE_MODE_DcmDiagnosticSessionControl_DEFAULT_SESSION,  \
        BSWM_IMMEDIATE,    \
        0, \
        NULL_PTR \
}

/*Array of BswMRule indexes associated with ModeReqPort BswM_MRP_DCM_ModeNotification_EcuReset. Variant :  __KW_COMMON*/
static const uint16 BswM_Cfg_ListOfRules_BswM_MRP_DCM_ModeNotification_EcuReset_cau16[] =
		{ 9, 10 };

/* ModeRequestPort structure for MRP BswM_MRP_DCM_ModeNotification_EcuReset. Variant : __KW_COMMON
 * - ModeInit Value Present flag (isModeInitValuePresent_b) 
 * - UserId (idUser_u16)
 * - Max requested mode (dataModeMax_u16)
 * - ModeInit Value (dataModeInitValue_u16)
 * - RequestProcessing (dataReqProcessing_en)
 * - Number of associated rules (nrAssociatedRules_u16)
 * - Reference to ListOfRules Array (adrRulesRef_pu16)
 */
#define BSWM_CFG_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET { \
        TRUE,        \
        2,    \
        65535,                \
        RTE_MODE_DcmEcuReset_NONE,  \
        BSWM_DEFERRED,    \
        2, \
        &BswM_Cfg_ListOfRules_BswM_MRP_DCM_ModeNotification_EcuReset_cau16[0]  \
}

/*Array of BswMRule indexes associated with ModeReqPort BswM_MRP_SWC_AppMode. Variant :  __KW_COMMON*/
static const uint16 BswM_Cfg_ListOfRules_BswM_MRP_SWC_AppMode_cau16[] =
		{ 0, 11 };

/* ModeRequestPort structure for MRP BswM_MRP_SWC_AppMode. Variant : __KW_COMMON
 * - ModeInit Value Present flag (isModeInitValuePresent_b) 
 * - UserId (idUser_u16)
 * - Max requested mode (dataModeMax_u16)
 * - ModeInit Value (dataModeInitValue_u16)
 * - RequestProcessing (dataReqProcessing_en)
 * - Number of associated rules (nrAssociatedRules_u16)
 * - Reference to ListOfRules Array (adrRulesRef_pu16)
 */
#define BSWM_CFG_BSWM_MRP_SWC_APPMODE { \
        FALSE,        \
        3,    \
        65535,                \
        0,         \
        BSWM_DEFERRED,    \
        2, \
        &BswM_Cfg_ListOfRules_BswM_MRP_SWC_AppMode_cau16[0]  \
}

/* Array of GenericRequest MRP structs. Variant : __KW_COMMON */
#define BSWM_CFG_GENERICREQ_MRPS {   \
    BSWM_CFG_BSWM_MRP_BSWM_MDG,      \
    BSWM_CFG_BSWM_MRP_DCM_DIAGNOSTICSESSIONCONTROL,      \
    BSWM_CFG_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET,      \
    BSWM_CFG_BSWM_MRP_SWC_APPMODE,      \
}

/*Array of BswMRule indexes associated with ModeReqPort BswM_MRP_NVM_ReadAllComplete. Variant :  __KW_COMMON*/
static const uint16 BswM_Cfg_ListOfRules_BswM_MRP_NVM_ReadAllComplete_cau16[] =
		{ 17 };

/* ModeRequestPort structure for MRP BswM_MRP_NVM_ReadAllComplete. Variant : __KW_COMMON
 * - Mode Init Value present flag 
 * - Configured NvM Service
 * - Configured/Default Mode Init Value
 * - RequestProcessing Enum - Immediate/Deferred 
 * - Number of rules referring to this MRP 
 * - Pointer to array containing indexes of associated rules
 */
#define BSWM_CFG_BSWM_MRP_NVM_READALLCOMPLETE { \
    TRUE,        \
    BSWM_NVMREADALL,       \
    NVM_REQ_PENDING,         \
    BSWM_IMMEDIATE,       \
    1,          \
    &BswM_Cfg_ListOfRules_BswM_MRP_NVM_ReadAllComplete_cau16[0]  \
}

/* ModeRequestPort structure for MRP BswM_MRP_NVM_WriteAllComplete. Variant : __KW_COMMON
 * - Mode Init Value present flag 
 * - Configured NvM Service
 * - Configured/Default Mode Init Value
 * - RequestProcessing Enum - Immediate/Deferred 
 * - Number of rules referring to this MRP 
 * - Pointer to array containing indexes of associated rules
 */
#define BSWM_CFG_BSWM_MRP_NVM_WRITEALLCOMPLETE { \
    TRUE,        \
    BSWM_NVMWRITEALL,       \
    NVM_REQ_PENDING,         \
    BSWM_IMMEDIATE,       \
    0,          \
    NULL_PTR \
}

/* Array of NvM BlockRequest MRP structs. Variant : __KW_COMMON */
#define BSWM_CFG_NVMJOBMODEIND_MRPS {   \
    BSWM_CFG_BSWM_MRP_NVM_READALLCOMPLETE,      \
    BSWM_CFG_BSWM_MRP_NVM_WRITEALLCOMPLETE,      \
}

/* BswM_Cfg_MRPType_tst : Array of structures for different MRP types. Variant : __KW_COMMON
 */
#define BSWM_CFG_MODEREQPORT {       \
	BSWM_CFG_COMMINDICATION_MRPS,      \
	BSWM_CFG_DCMCOMMODEREQ_MRPS,      \
    BSWM_CFG_ECUMWKPSRC_MRPS,      \
	BSWM_CFG_GENERICREQ_MRPS,      \
	BSWM_CFG_NVMJOBMODEIND_MRPS,      \
            }

/* ModeArbitration Type structure  
 * - ModeRequestPortType structure (BswM_Cfg_MRPType_tst)
 * - Number of rules configured for the chosen variant 
 * - Pointer to base address of array of rules (BswM_Cfg_RuleType_tst)
 */
#define BSWM_CFG_MODEARBITRATION {      \
        BSWM_CFG_MODEREQPORT,     \
        18, \
        &BswM_Cfg_Rule_cast[0] \
}

/*
 **********************************************************************************************************************
 * Arrays of BswMPduGroupSwitch Group References for the variant  with contents: 
 *   # BswMPduEnableGroupSwitch Reference/References
 *   # BswMPduDisableGroupSwitch Reference/References
 **********************************************************************************************************************
 */

static const Com_IpduGroupIdType BswM_Cfg_AC_BswM_AI_EnableRxPduGroup_BswMPduEnableGrpRef_cau16[1] =
		{ ComConf_ComIPduGroup_ComIPduGroup_Rx };

static const Com_IpduGroupIdType BswM_Cfg_AC_BswM_AI_EnableTxPduGroup_BswMPduEnableGrpRef_cau16[1] =
		{ ComConf_ComIPduGroup_ComIPduGroup_Tx };

static const Com_IpduGroupIdType BswM_Cfg_AC_BswM_AI_StartPduGroup_All_BswMPduEnableGrpRef_cau16[2] =
		{ ComConf_ComIPduGroup_ComIPduGroup_Rx,
				ComConf_ComIPduGroup_ComIPduGroup_Tx };

static const Com_IpduGroupIdType BswM_Cfg_AC_BswM_AI_DisableRxPduGroup_BswMPduDisableGrpRef_cau16[1] =
		{ ComConf_ComIPduGroup_ComIPduGroup_Rx };

static const Com_IpduGroupIdType BswM_Cfg_AC_BswM_AI_DisableTxPduGroup_BswMPduDisableGrpRef_cau16[1] =
		{ ComConf_ComIPduGroup_ComIPduGroup_Tx };

static const Com_IpduGroupIdType BswM_Cfg_AC_BswM_AI_StopPduGroup_All_BswMPduDisableGrpRef_cau16[2] =
		{ ComConf_ComIPduGroup_ComIPduGroup_Rx,
				ComConf_ComIPduGroup_ComIPduGroup_Tx };

/*
 **********************************************************************************************************************
 * Array of BswMPduRouterControl Action for the variant  with contents: 
 *   # Reinit 
 *   # No of EnabledPduGroupRef
 *   # No of DisabledPduGroupRef
 *   # Base Addr of EnabledPduGroupRef
 *   # Base Addr of DisabledPduGroupRef
 **********************************************************************************************************************
 */
/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
static const BswM_Cfg_AC_PduGroupSwitchType_tst BswM_Cfg_AC_BswMPduGroupSwitch_cast[BSWM_NO_OF_AC_IPDUGROUPSWITCH] =
		{ {
		/* BswM_AI_DisableRxPduGroup */
		FALSE, 0, 1,

		NULL_PTR,

		&BswM_Cfg_AC_BswM_AI_DisableRxPduGroup_BswMPduDisableGrpRef_cau16[0]

		}, {
		/* BswM_AI_DisableTxPduGroup */
		FALSE, 0, 1,

		NULL_PTR,

		&BswM_Cfg_AC_BswM_AI_DisableTxPduGroup_BswMPduDisableGrpRef_cau16[0]

		}, {
		/* BswM_AI_EnableRxPduGroup */
		FALSE, 1, 0,

		&BswM_Cfg_AC_BswM_AI_EnableRxPduGroup_BswMPduEnableGrpRef_cau16[0],

		NULL_PTR

		}, {
		/* BswM_AI_EnableTxPduGroup */
		FALSE, 1, 0,

		&BswM_Cfg_AC_BswM_AI_EnableTxPduGroup_BswMPduEnableGrpRef_cau16[0],

		NULL_PTR

		}, {
		/* BswM_AI_StartPduGroup_All */
		FALSE, 2, 0,

		&BswM_Cfg_AC_BswM_AI_StartPduGroup_All_BswMPduEnableGrpRef_cau16[0],

		NULL_PTR

		}, {
		/* BswM_AI_StopPduGroup_All */
		FALSE, 0, 2,

		NULL_PTR,

		&BswM_Cfg_AC_BswM_AI_StopPduGroup_All_BswMPduDisableGrpRef_cau16[0]

		} };

/*****************************************************************************************
 * Array of ActionListItems for __KW_COMMON with contents:
 * {
 *     AbortOnFailFlag,
 *     ActionListItemType,
 *     PointerToActionListItem
 * }
 *****************************************************************************************/

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_BswMSwitchAppRun_Items_cast[4] =
		{ {
		/* ActionListItemName  :   BswM_ALI_AllowComm_Can_Network_ETAS */
		FALSE, BSWM_ACTION_COMM_ALLOW_COM,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMComMAllowCom_cast[0] /* BswM_AI_ComMCommAllowed_Can_Network_ETAS */
		}, {
		/* ActionListItemName  :   BswM_ALI_ReqFullComm_Can_Network_ETAS */
		FALSE, BSWM_ACTION_COMM_MODE_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMComMModeSwitch_cast[0] /* BswM_AI_ComMReqFullComm_Can_Network_ETAS */
		}, {
		/* ActionListItemName  :   BswM_ALI_StartPduGroup */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[4] /* BswM_AI_StartPduGroup_All */
		}, {
		/* ActionListItemName  :   BswM_ALI_BswMAppRun */
		FALSE, BSWM_ACTION_SCHM_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMSchMSwitch_cast[0] /* BswM_AI_BswMSwitchAppRun */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_BswMSwitchPostRun_Items_cast[4] =
		{ {
		/* ActionListItemName  :   BswM_ALI_StopPduGroup_All */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[5] /* BswM_AI_StopPduGroup_All */
		}, {
		/* ActionListItemName  :   BswM_ALI_ReqNoComm_Can_Network_ETAS */
		FALSE, BSWM_ACTION_COMM_MODE_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMComMModeSwitch_cast[1] /* BswM_AI_ComMReqNoComm_Can_Network_ETAS */
		}, {
		/* ActionListItemName  :   BswM_ALI_CommNotAllowed_Can_Network_ETAS */
		FALSE, BSWM_ACTION_COMM_ALLOW_COM,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMComMAllowCom_cast[1] /* BswM_AI_ComMCommNotAllowed_Can_Network_ETAS */
		}, {
		/* ActionListItemName  :   BswM_ALI_BswMPostRun */
		FALSE, BSWM_ACTION_SCHM_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMSchMSwitch_cast[1] /* BswM_AI_BswMSwitchPostRun */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_BswMSwitchPrepShutdown_Items_cast[3] =
		{ {
		/* ActionListItemName  :   BswM_ALI_CanTrcv_GotoSleep */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[5] /* BswM_AI_CanTrcv_GotoSleep */
		}, {
		/* ActionListItemName  :   BswM_ALI_SelfHold_Unlock */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[32] /* BswM_AI_SelfHold_Unlock */
		}, {
		/* ActionListItemName  :   BswM_ALI_BswMPrepShutdown */
		FALSE, BSWM_ACTION_SCHM_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMSchMSwitch_cast[2] /* BswM_AI_BswMSwitchPrepShutdown */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_BswMSwitchRun_Items_cast[23] =
		{ {
		/* ActionListItemName  :   BswM_ALl_FimInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[20] /* BswM_AI_FimInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_DemInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[10] /* BswM_AI_DemInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_Dem_EnableStorage */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[12] /* BswM_AI_Dem_EnableStorage */
		}, {
		/* ActionListItemName  :   BswM_ALI_Dem_ExtendedData_Init */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[14] /* BswM_AI_Dem_ExtendedData_Init */
		}, {
		/* ActionListItemName  :   BswM_ALI_Run_CddInitZero */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[30] /* BswM_AI_Run_CddInitZero */
		}, {
		/* ActionListItemName  :   BswM_ALI_Enable_PeripheralSply */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[17] /* BswM_AI_Enable_PeripheralSply */
		}, {
		/* ActionListItemName  :   BswM_ALI_Run_CddInitOne */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[36] /* BswM_Al_Run_CddInitOne */
		}, {
		/* ActionListItemName  :   BswM_ALI_SelfHold */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[31] /* BswM_AI_SelfHold */
		}, {
		/* ActionListItemName  :   BswM_ALI_CanTrcv_Init */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[6] /* BswM_AI_CanTrcv_Init */
		}, {
		/* ActionListItemName  :   BswM_ALI_CanTrcv_GotoNormal */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[4] /* BswM_AI_CanTrcv_GotoNormal */
		}, {
		/* ActionListItemName  :   BswM_ALI_CanIfInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[0] /* BswM_AI_CanIfInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_CanSmInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[2] /* BswM_AI_CanSmInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_PduRInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[25] /* BswM_AI_PduRInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_ComMInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[8] /* BswM_AI_ComMInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_ComInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[7] /* BswM_AI_ComInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_Nm_Init */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[22] /* BswM_AI_Nm_Init */
		}, {
		/* ActionListItemName  :   BswM_ALI_CanNm_Init */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[1] /* BswM_AI_CanNm_Init */
		}, {
		/* ActionListItemName  :   BswM_ALI_DcmInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[9] /* BswM_AI_Dcm_Init */
		}, {
		/* ActionListItemName  :   BswM_ALI_CanTpInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[3] /* BswM_AI_CanTp_Init */
		}, {
		/* ActionListItemName  :   BswM_ALI_XcpInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[35] /* BswM_AI_XcpInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_WdgMInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[34] /* BswM_AI_WdgMInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_RteStart */
		FALSE, BSWM_ACTION_RTE_START,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMRteStart_cast[0] /* BswM_AI_RteStart */
		}, {
		/* ActionListItemName  :   BswM_ALI_BswMRun */
		FALSE, BSWM_ACTION_SCHM_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMSchMSwitch_cast[3] /* BswM_AI_BswMSwitchRun */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_BswMSwitchShutdown_Items_cast[2] =
		{ {
		/* ActionListItemName  :   BswM_ALI_GoDown */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[21] /* BswM_AI_GoDown */
		}, {
		/* ActionListItemName  :   BswM_ALI_BswMShutdown */
		FALSE, BSWM_ACTION_SCHM_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMSchMSwitch_cast[4] /* BswM_AI_BswMSwitchShutdown */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_BswMSwitchStartupTwo_Items_cast[10] =
		{ {
		/* ActionListItemName  :   BswM_ALI_StartupTwo_CddInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[33] /* BswM_AI_StartupTwo_CddInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_Rtm_Fee_ProcessStart */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[27] /* BswM_AI_Rtm_Fee_ProcessStart */
		}, {
		/* ActionListItemName  :   BswM_ALI_FeeInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[18] /* BswM_AI_FeeInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_Rtm_Fee_ProcessEnd */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[26] /* BswM_AI_Rtm_Fee_ProcessEnd */
		}, {
		/* ActionListItemName  :   BswM_ALI_FeeInitEnd */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[19] /* BswM_AI_FeeInitEnd */
		}, {
		/* ActionListItemName  :   BswM_ALI_Rtm_NvM_ProcessStart */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[29] /* BswM_AI_Rtm_NvM_ProcessStart */
		}, {
		/* ActionListItemName  :   BswM_ALI_NvMInit */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[23] /* BswM_AI_NvMInit */
		}, {
		/* ActionListItemName  :   BswM_ALI_Rtm_NvM_ProcessEnd */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[28] /* BswM_AI_Rtm_NvM_ProcessEnd */
		}, {
		/* ActionListItemName  :   BswM_ALI_NvMReadAll */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[24] /* BswM_AI_NvMReadAll */
		}, {
		/* ActionListItemName  :   BswM_ALI_BswMStartupTwo */
		FALSE, BSWM_ACTION_SCHM_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMSchMSwitch_cast[5] /* BswM_AI_BswMSwitchStartupTwo */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_DISABLE_RX_ENABLE_TX_NORM_Items_cast[2] =
		{ {
		/* ActionListItemName  :   BswM_ALI_DisableRxPduGroup */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[0] /* BswM_AI_DisableRxPduGroup */
		}, {
		/* ActionListItemName  :   BswM_ALI_EnableTxPduGroup */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[3] /* BswM_AI_EnableTxPduGroup */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_DISABLE_RX_ENABLE_TX_NORM_NM_Items_cast[3] =
		{ {
		/* ActionListItemName  :   BswM_ALI_DisableRxPduGroup_NORM */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[0] /* BswM_AI_DisableRxPduGroup */
		}, {
		/* ActionListItemName  :   BswM_ALI_DisableTxPduGroup_NORM */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[1] /* BswM_AI_DisableTxPduGroup */
		}, {
		/* ActionListItemName  :   BswM_ALI_DisableRxTxPduGroup_NM */
		FALSE, BSWM_ACTION_NM_CNTRL,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMNMControl_cast[1] /* BswM_AI_NM_Stop */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_DISABLE_RX_TX_NORM_Items_cast[2] =
		{ {
		/* ActionListItemName  :   BswM_ALI_DisableRxPduGroup */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[0] /* BswM_AI_DisableRxPduGroup */
		}, {
		/* ActionListItemName  :   BswM_ALI_DisableTxPduGroup */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[1] /* BswM_AI_DisableTxPduGroup */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_DISABLE_TX_RX_NM_Items_cast[1] =
		{ {
		/* ActionListItemName  :   BswM_ALI_NM_STOP */
		FALSE, BSWM_ACTION_NM_CNTRL,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMNMControl_cast[1] /* BswM_AI_NM_Stop */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_ENABLE_RX_DISABLE_TX_NORM_Items_cast[2] =
		{ {
		/* ActionListItemName  :   BswM_ALI_EnableRxPduGroup */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[2] /* BswM_AI_EnableRxPduGroup */
		}, {
		/* ActionListItemName  :   BswM_ALI_DisableTxPduGroup */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[1] /* BswM_AI_DisableTxPduGroup */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_ENABLE_RX_TX_NORM_Items_cast[2] =
		{ {
		/* ActionListItemName  :   BswM_ALI_EnableRxPduGroup */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[2] /* BswM_AI_EnableRxPduGroup */
		}, {
		/* ActionListItemName  :   BswM_ALI_EnableTxPduGroup */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[3] /* BswM_AI_EnableTxPduGroup */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_ENABLE_RX_TX_NORM_NM_Items_cast[3] =
		{ {
		/* ActionListItemName  :   BswM_ALI_EnableRxPduGroup_NORM */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[2] /* BswM_AI_EnableRxPduGroup */
		}, {
		/* ActionListItemName  :   BswM_ALI_EnableRxTxPduGroup_NM */
		FALSE, BSWM_ACTION_NM_CNTRL,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMNMControl_cast[0] /* BswM_AI_NM_Start */
		}, {
		/* ActionListItemName  :   BswM_ALI_EnableTxPduGroup_NORM */
		FALSE, BSWM_ACTION_PDU_GRP_SWITCH,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMPduGroupSwitch_cast[3] /* BswM_AI_EnableTxPduGroup */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_ENABLE_TX_RX_NM_Items_cast[1] =
		{ {
		/* ActionListItemName  :   BswM_ALI_NM_Start */
		FALSE, BSWM_ACTION_NM_CNTRL,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMNMControl_cast[0] /* BswM_AI_NM_Start */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_EcuReset_HARD_Items_cast[5] =
		{ {
		/* ActionListItemName  :   BswM_ALI_EcuM_SelectShutdownTarget_RESET_IO */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[15] /* BswM_AI_EcuM_SelectShutdownTarget_RESET_IO */
		}, {
		/* ActionListItemName  :   BswM_ALI_Dem_endOperation */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[13] /* BswM_AI_Dem_EndOperation */
		}, {
		/* ActionListItemName  :   BswM_ALI_Dem_Shutdown */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[11] /* BswM_AI_DemShutdown */
		}, {
		/* ActionListItemName  :   BswM_ALI_CanTrcv_GotoSleep */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[5] /* BswM_AI_CanTrcv_GotoSleep */
		}, {
		/* ActionListItemName  :   BswM_ALI_GoDown */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[21] /* BswM_AI_GoDown */
		} };

static const BswM_Cfg_ActionListItemType_tst BswM_Cfg_AL_BswM_AL_DCM_EcuReset_SOFT_Items_cast[2] =
		{ {
		/* ActionListItemName  :   BswM_ALI_EcuM_SelectShutdownTarget_RESET_MCU */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[16] /* BswM_AI_EcuM_SelectShutdownTarget_RESET_MCU */
		}, {
		/* ActionListItemName  :   BswM_ALI_EcuM_GoDown */
		FALSE, BSWM_ACTION_USR_CALLOUT,
		/*MR12 DIR 1.1 VIOLATION: BswM_Prv_Evaluate_Action expects a void pointer to actionlistitem which is then typecast back to the corresponding action type*/
		(const void*) &BswM_Cfg_AC_BswMUserCallout_cast[21] /* BswM_AI_GoDown */
		} };

/*****************************************************************************************
 * Array of ActionLists for __KW_COMMON with contents:
 * {
 *     ExecutionType,
 *     NumberOfActionListItems,
 *     BaseAddressOfActionListItemArray,
 *     Unique Number for ActionList
 * }
 ****************************************************************************************/

static const BswM_Cfg_ActionListType_tst BswM_Cfg_AL_cast[16] = { {
/* ActionListName   :   BswM_AL_BswMSwitchAppRun */
BSWM_TRIGGER, 4, &BswM_Cfg_AL_BswM_AL_BswMSwitchAppRun_Items_cast[0], 0 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_BswMSwitchPostRun */
BSWM_TRIGGER, 4, &BswM_Cfg_AL_BswM_AL_BswMSwitchPostRun_Items_cast[0], 1 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_BswMSwitchPrepShutdown */
BSWM_TRIGGER, 3, &BswM_Cfg_AL_BswM_AL_BswMSwitchPrepShutdown_Items_cast[0], 2 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_BswMSwitchRun */
BSWM_TRIGGER, 23, &BswM_Cfg_AL_BswM_AL_BswMSwitchRun_Items_cast[0], 3 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_BswMSwitchShutdown */
BSWM_TRIGGER, 2, &BswM_Cfg_AL_BswM_AL_BswMSwitchShutdown_Items_cast[0], 4 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_BswMSwitchStartupTwo */
BSWM_CONDITION, 10, &BswM_Cfg_AL_BswM_AL_BswMSwitchStartupTwo_Items_cast[0], 5 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_DISABLE_RX_ENABLE_TX_NORM */
BSWM_TRIGGER, 2,
		&BswM_Cfg_AL_BswM_AL_DCM_DISABLE_RX_ENABLE_TX_NORM_Items_cast[0], 6 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_DISABLE_RX_ENABLE_TX_NORM_NM */
BSWM_TRIGGER, 3,
		&BswM_Cfg_AL_BswM_AL_DCM_DISABLE_RX_ENABLE_TX_NORM_NM_Items_cast[0], 7 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_DISABLE_RX_TX_NORM */
BSWM_TRIGGER, 2, &BswM_Cfg_AL_BswM_AL_DCM_DISABLE_RX_TX_NORM_Items_cast[0], 8 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_DISABLE_TX_RX_NM */
BSWM_TRIGGER, 1, &BswM_Cfg_AL_BswM_AL_DCM_DISABLE_TX_RX_NM_Items_cast[0], 9 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_ENABLE_RX_DISABLE_TX_NORM */
BSWM_TRIGGER, 2,
		&BswM_Cfg_AL_BswM_AL_DCM_ENABLE_RX_DISABLE_TX_NORM_Items_cast[0], 10 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_ENABLE_RX_TX_NORM */
BSWM_TRIGGER, 2, &BswM_Cfg_AL_BswM_AL_DCM_ENABLE_RX_TX_NORM_Items_cast[0], 11 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_ENABLE_RX_TX_NORM_NM */
BSWM_TRIGGER, 3, &BswM_Cfg_AL_BswM_AL_DCM_ENABLE_RX_TX_NORM_NM_Items_cast[0], 12 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_ENABLE_TX_RX_NM */
BSWM_TRIGGER, 1, &BswM_Cfg_AL_BswM_AL_DCM_ENABLE_TX_RX_NM_Items_cast[0], 13 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_EcuReset_HARD */
BSWM_TRIGGER, 5, &BswM_Cfg_AL_BswM_AL_DCM_EcuReset_HARD_Items_cast[0], 14 /* Unique Number for ActionList */
}, {
/* ActionListName   :   BswM_AL_DCM_EcuReset_SOFT */
BSWM_TRIGGER, 2, &BswM_Cfg_AL_BswM_AL_DCM_EcuReset_SOFT_Items_cast[0], 15 /* Unique Number for ActionList */
} };

/* PBAction Type structure Variant : __KW_COMMON
 * - Pointer to base address of array of IPduGroupSwitchType ActionType structure (BswM_Cfg_AC_PduGroupSwitchType_tst)
 */
#define BSWM_CFG_PBACTION                {      \
                &BswM_Cfg_AC_BswMPduGroupSwitch_cast[0],     \
}

/* ModeControl Type structure  
 * - ActionType structure (BswM_Cfg_PBActionType_tst)
 * - Pointer to base address of array of actionlists (BswM_Cfg_ActionListType_tst) Currently NULL_PTR
 */
#define BSWM_CFG_MODECONTROL {      \
        BSWM_CFG_PBACTION,     \
        &BswM_Cfg_AL_cast[0]   \
}

/* BswM_ConfigType  :
 * - ModeArbitration structure (BswM_Cfg_ModeArbitrationType_tst)
 * - ModeControl structure (BswM_Cfg_ModeControlType_tst)
 * - Version info to compare during module initialization
 */
const BswM_ConfigType BswM_Config = {
BSWM_CFG_MODEARBITRATION,
BSWM_CFG_MODECONTROL,
BSWM_CFG_VERSION_INFO };

#define BSWM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED 
#include "BswM_MemMap.h"

#define BSWM_START_SEC_CONFIG_DATA_POSTBUILD_32
#include "BswM_MemMap.h"

const BswM_ConfigType *const BswM_Configurations_capcst[BSWM_NO_OF_CONFIGSETS] =
		{ &BswM_Config };

#define BSWM_STOP_SEC_CONFIG_DATA_POSTBUILD_32
#include "BswM_MemMap.h"
/**********************************************************************************************************************
 *                                                                                                      
 **********************************************************************************************************************/
