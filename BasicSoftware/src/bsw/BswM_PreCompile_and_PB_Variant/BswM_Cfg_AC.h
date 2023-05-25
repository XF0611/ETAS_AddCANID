#ifndef BSWM_CFG_AC_H
#define BSWM_CFG_AC_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Std_Types.h"
#include "ComStack_Types.h"
/*
 **********************************************************************************************************************
 * Enum definitions
 **********************************************************************************************************************
 */

/* Enumeration for ActionList Execution Type */
typedef enum {
	BSWM_TRIGGER, BSWM_CONDITION
} BswM_ActionListExecutionType_ten;

/* Enumeration macros for all the available Actions and ActionList */
typedef enum {
	BSWM_ACTIONLIST,
	BSWM_ACTION_COMM_ALLOW_COM,
	BSWM_ACTION_COMM_MODE_LMTN,
	BSWM_ACTION_COMM_MODE_SWITCH,
	BSWM_ACTION_DEADLINE_MNT_CTRL,
	BSWM_ACTION_ECUM_STATE_SWITCH,
	BSWM_ACTION_ECUM_DRIVER_INIT_LIST_BSWM,
	BSWM_ACTION_ETHIF_MODESWITCH,
	BSWM_ACTION_J1939DCM_STATE_SWITCH,
	BSWM_ACTION_J1939RM_STATE_SWITCH,
	BSWM_ACTION_LIN_SCHDL_SWITCH,
	BSWM_ACTION_NM_CNTRL,
	BSWM_ACTION_PDU_GRP_SWITCH,
	BSWM_ACTION_PDU_ROUTER_CNTRL,
	BSWM_ACTION_RTE_MODE_REQUEST,
	BSWM_ACTION_RTE_START,
	BSWM_ACTION_RTE_STOP,
	BSWM_ACTION_RTE_SWITCH,
	BSWM_ACTION_SCHM_SWITCH,
	BSWM_ACTION_SD_CLNT_SERV_MODE_REQ,
	BSWM_ACTION_SD_CNSMD_EVNT_GRP_MODE_REQ,
	BSWM_ACTION_SD_SERVR_SERV_MODE_REQ,
	BSWM_ACTION_SWITCH_IPDU_MODE,
	BSWM_ACTION_TIMER_CONTROL,
	BSWM_ACTION_TRIG_IPDU_SEND,
	BSWM_ACTION_USR_CALLOUT,
	BSWM_ACTION_RB_SWC_USR_CALLOUT,
	BSWM_ACTIONLIST_SIZE
} BswM_ActionListItemType_ten;

/* Enumeration for Pdu Router Action */
typedef enum {
	BSWM_PDUR_DISABLE, BSWM_PDUR_ENABLE
} BswM_PduRouterControlActionType_ten;

/* Enumeration for ComM Mode Switch Action */
typedef enum {
	BSWM_FULL_COM, BSWM_NO_COM
} BswM_ComMModeSwitchActionType_ten;

/* Enumeration for EthIf SwitchPortGroup Action */
typedef enum {
	BSWM_ETHTRCV_MODE_ACTIVE, BSWM_ETHTRCV_MODE_DOWN
} BswM_EthIfModeSwitch_ten;

/* Enumeration for Nm Control Action */
typedef enum {
	BSWM_NM_DISABLE, BSWM_NM_ENABLE
} BswM_NmControlActionType_ten;

/* Enumeration for J1939Dcm State Switch Action */
typedef enum {
	BSWM_J1939DCM_STATE_ONLINE, BSWM_J1939DCM_STATE_OFFLINE
} BswM_J1939DcmStateSwitchActionType_ten;

/* Enumeration for J1939Rm State Switch Action */
typedef enum {
	BSWM_J1939RM_STATE_ONLINE, BSWM_J1939RM_STATE_OFFLINE
} BswM_J1939RmStateSwitchActionType_ten;

/* Enumeration for SdClientSrvM*/
typedef enum {
	BSWM_SD_CLIENT_SERVICE_RELEASED, BSWM_SD_CLIENT_SERVICE_REQUESTED
} BswM_Sd_ClientServiceSetStateType_ten;

/* Enumeration for SdConsumedeEvntGrp Action */
typedef enum {
	BSWM_SD_CONSUMED_EVENTGROUP_RELEASED, BSWM_SD_CONSUMED_EVENTGROUP_REQUESTED
} BswM_Sd_ConsumedEventGroupSetStateType_ten;

/* Enumeration for SdServerSrvM Action */
typedef enum {
	BSWM_SD_SERVER_SERVICE_DOWN, BSWM_SD_SERVER_SERVICE_AVAILABLE
} BswM_Sd_ServerServiceSetStateType_ten;

/* Enumeration for BswMTImerControl Action */
typedef enum {
	BSWM_TIMER_STOP, BSWM_TIMER_START
} BswM_TimerControl_ten;

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */

/* BswMComMAllowCom Action Type Definition 
 * - Flag to represent Allow or Block communication for a ComM Channel
 * - A reference to the ComM Channel for which communication shall be allowed or blocked
 */
typedef struct {
	boolean isComAllowed_b;
	NetworkHandleType idNetworkReference_u8;
} BswM_Cfg_AC_ComMAllowComType_tst;

/* BswMComMModeSwitch Action Type Definition 
 * - Specifies if the requested communication mode is BSWM_FULL_COM/BSWM_NO_COM
 * - A reference to the ComM User that is associated to the Communication channel 
 */
typedef struct {
	BswM_ComMModeSwitchActionType_ten dataComMRequestedMode_en;
	ComM_UserHandleType idComMUserReference_u8;
} BswM_Cfg_AC_ComMModeSwitchType_tst;

/* BswMNMControl Action Type Definition 
 * - Specifies if the requested action is BSWM_NM_DISABLE/BSWM_NM_ENABLE
 * - A reference to the Com channel that is associated to the network 
 */
typedef struct {
	BswM_NmControlActionType_ten dataNMRequestedAction_en;
	NetworkHandleType idChannel_u8;
} BswM_Cfg_AC_NmControlType_tst;

/* BswMSchMSwitch Action Type Definition 
 * - ID for each BswMSchMSwitch actions
 */
typedef struct {
	uint16 idSchMSwitch_u16;
} BswM_Cfg_AC_SchMSwitchType_tst;

/* BswMUserCallout Action Type Definition 
 * - UserID for each usercallout actions
 */
typedef struct {
	uint16 idUserCallout_u16;
} BswM_Cfg_AC_UserCalloutType_tst;

/* BswMRteStart Action Type Definition 
 * - Dummy variable
 */
typedef struct {
	uint16 idRteStart_u16; //Need to check it once and then add in Prv_AC.c and beloe MemMap section as well 
} BswM_Cfg_AC_RteStartType_tst;

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */
#define BSWM_NO_OF_AC_COMM_ALLOW_COM           2u    
#define BSWM_NO_OF_AC_COMM_MODE_LIMITATION     0u
#define BSWM_NO_OF_AC_COMM_MODE_SWITCH         2u
#define BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL   0u
#define BSWM_NO_OF_AC_ECUMSTATESWITCH          0u
#define BSWM_NO_OF_AC_ECUMDRIVERINITLISTBSWM   0u   
#define BSWM_NO_OF_AC_ETHIFMODESWITCH          0u
#define BSWM_NO_OF_AC_J1939DCM_STATE_SWITCH    0u
#define BSWM_NO_OF_AC_J1939RM_STATE_SWITCH     0u
#define BSWM_NO_OF_AC_LINSM_SCHEDULESWITCH     0u
#define BSWM_NO_OF_AC_NM_CNTRL                 2u
#define BSWM_NO_OF_AC_IPDUGROUPSWITCH          6u
#define BSWM_NO_OF_AC_PDUROUTERCONTROL         0u
#define BSWM_NO_OF_AC_RTE_MODE_REQUEST         0u
#define BSWM_NO_OF_AC_RTE_START                1u
#define BSWM_NO_OF_AC_RTE_STOP                 0u
#define BSWM_NO_OF_AC_RTE_SWITCH               0u
#define BSWM_NO_OF_AC_SCHM_SWITCH               7u
#define BSWM_NO_OF_AC_SWITCHIPDUMODE           0u
#define BSWM_NO_OF_AC_BSWMTIMERCONTROL         0u   
#define BSWM_NO_OF_AC_TRIGIPDUSEND             0u
#define BSWM_NO_OF_AC_USERCALLOUT              37u
#define BSWM_NO_OF_AC_RB_SWC_USERCALLOUT              0u

#define BSWM_NO_OF_AC_SDCLIENT_SRVMR            0u
#define BSWM_NO_OF_AC_SDCONSMD_EVNTGRPMR     0u
#define BSWM_NO_OF_AC_SDSRVR_SRVMR     0u

#define BswM_Cfg_AC_BswMComMAllowCom_cast                       BswM_Cfg_AC_BswMComMAllowCom_cast

#define BswM_Cfg_AC_BswMComMModeSwitch_cast                     BswM_Cfg_AC_BswMComMModeSwitch_cast

#define BswM_Cfg_AC_BswMNMControl_cast                          BswM_Cfg_AC_BswMNMControl_cast

#define BswM_Cfg_AC_BswMSchMSwitch_cast                          BswM_Cfg_AC_BswMSchMSwitch_cast

#define BswM_Cfg_AC_BswMUserCallout_cast                        BswM_Cfg_AC_BswMUserCallout_cast

#define BswM_Cfg_AC_BswMRteStart_cast                        BswM_Cfg_AC_BswMRteStart_cast

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

#define BSWM_START_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"

/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
extern const BswM_Cfg_AC_ComMAllowComType_tst BswM_Cfg_AC_BswMComMAllowCom_cast[BSWM_NO_OF_AC_COMM_ALLOW_COM];

/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
extern const BswM_Cfg_AC_ComMModeSwitchType_tst BswM_Cfg_AC_BswMComMModeSwitch_cast[BSWM_NO_OF_AC_COMM_MODE_SWITCH];

/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
extern const BswM_Cfg_AC_NmControlType_tst BswM_Cfg_AC_BswMNMControl_cast[BSWM_NO_OF_AC_NM_CNTRL];

/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
extern const BswM_Cfg_AC_SchMSwitchType_tst BswM_Cfg_AC_BswMSchMSwitch_cast[BSWM_NO_OF_AC_SCHM_SWITCH];

/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
extern const BswM_Cfg_AC_UserCalloutType_tst BswM_Cfg_AC_BswMUserCallout_cast[BSWM_NO_OF_AC_USERCALLOUT];

/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
extern const BswM_Cfg_AC_RteStartType_tst BswM_Cfg_AC_BswMRteStart_cast[BSWM_NO_OF_AC_RTE_START];

#define BSWM_STOP_SEC_CONST_UNSPECIFIED 
#include "BswM_MemMap.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

extern void BswM_Cfg_EcuMDriverInitListBswMFunction(
		uint16 idBswMEcuMDriverInitListBswM_u16);
extern void BswM_Cfg_ProcessUserCalloutFunction(uint16 idUserCallout_u16);
extern Std_ReturnType BswM_Cfg_ProcessRteModeRequestFunction(
		uint16 idRteModeRequest_u16);
extern Std_ReturnType BswM_Cfg_ProcessRteSwitchFunction(uint16 idRteSwitch_u16);
extern Std_ReturnType BswM_Cfg_ProcessSchMSwitchFunction(
		uint16 idSchMSwitch_u16);
extern void BswM_Cfg_ProcessRbSwcUserCalloutFunction(
		uint16 idRbSwcUserCallout_u16);
extern void BswM_Cfg_BswMTimerControlFunction(
		BswM_TimerControl_ten BswMTimerControlState,
		float32 BswMTimerValue_f32);

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#endif /* BSWM_CFG_AC_H */
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/
