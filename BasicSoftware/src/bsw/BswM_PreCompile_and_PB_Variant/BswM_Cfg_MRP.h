#ifndef BSWM_CFG_MRP_H
#define BSWM_CFG_MRP_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Std_Types.h"
#include "ComStack_Types.h"
#include "BswM_Types.h"

/*
 **********************************************************************************************************************
 * Enum definitions
 **********************************************************************************************************************
 */

/*
 BswM Request Processing type Enum--
 Defines when the processing of a mode arbitration request is done :
 a. Deferred : In the context of the BswM main function
 b. Immediate : In the context of the MRP itself
 */

typedef enum {
	BSWM_DEFERRED, BSWM_IMMEDIATE
} BswM_ReqProcessing_ten;

/* Enumeration literal to represent the BswM Rule State */
typedef enum {
	BSWM_FALSE, BSWM_TRUE, BSWM_UNDEFINED
} BswM_RuleStateType_ten;

/* Enumeration literal to represent the BswM Timer MRP State */
typedef enum {
	BSWM_TIMER_STARTED, BSWM_TIMER_EXPIRED, BSWM_TIMER_STOPPED
} BswM_TimerStateType_ten;

/*
 BswM Mode Request Port type Enum--
 Defines the various types of mode request sources
 Default value = 0xFF has been defined to avoid MISRA warnings when 
 assigning a default value for variables of this enum type 
 (in particular, when clearing the interrupt queue, the MRPType member 
 is assigned with the default value defined in this enum) 
 */

typedef enum {
	/* TODO : verify for all MRP */
	BSWM_MRP_BSW_MODE_NOTIFICATION,
	BSWM_MRP_CANSM_IND,
	BSWM_MRP_COMM_IND,
	BSWM_MRP_COMM_INITIATE_RESET,
	BSWM_MRP_COMM_PNC_REQST,
	BSWM_MRP_DCM_APPL_UPDATE_IND,
	BSWM_MRP_DCM_COM_MODE_REQST,
	BSWM_MRP_ECUM_IND,
	BSWM_MRP_ECUM_RUN_REQST_IND,
	BSWM_MRP_ECUM_WKUP_SOURCE,
	BSWM_MRP_ETHIF_PORTFGROUP,
	BSWM_MRP_ETHSM_IND,
	BSWM_MRP_FRSM_IND,
	BSWM_MRP_GENERIC_REQST,
	BSWM_MRP_J1939_DCM_BROADCAST_STATS,
	BSWM_MRP_J1939_NM_IND,
	BSWM_MRP_LINSM_IND,
	BSWM_MRP_LINSM_SCHEDULE_IND,
	BSWM_MRP_LINTP_MODE_REQST,
	BSWM_MRP_NM_CAR_WKUP_IND,
	BSWM_MRP_NVM_JOB_MODE_IND,
	BSWM_MRP_NVM_REQST,
	BSWM_MRP_SD_CLNT_SERV_CURR_STATE,
	BSWM_MRP_SD_CNSMD_EVNT_GRP_CURR_STATE,
	BSWM_MRP_SD_EVNT_HNDLR_CURR_STATE,
	BSWM_MRP_SWC_MODE_NOTIFICATION,
	BSWM_MRP_SWC_MODE_REQUEST,
	BSWM_MRP_TIMER,
	BSWM_MRP_DEFAULT = 0xFF
} BswM_Cfg_MRPType_ten;

/*
 BswM No Source Status type Enum--
 Defines requested mode type for MRPs with No source :
 a. BSWM_NO_SOURCE_STATUS_FALSE : MRP Initial Mode Type
 b. BSWM_NO_SOURCE_STATUS_TRUE  : MRP Requested Mode Type
 */
typedef enum {
	BSWM_NO_SOURCE_STATUS_FALSE, BSWM_NO_SOURCE_STATUS_TRUE
} BswM_Cfg_NoSourceType_en;

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */

/*Count of configured ports for each Mode Request Source type*/

#define BSWM_NO_OF_MRP_CANSM_INDICATION             0u
#define BSWM_NO_OF_MRP_COMM_INDICATION              1u
#define BSWM_NO_OF_MRP_COMM_INITIATE_RESET          0u
#define BSWM_NO_OF_MRP_COMMPNCS_REQUEST             0u
#define BSWM_NO_OF_MRP_DCM_APP_UPDATE_INDICATION    0u
#define BSWM_NO_OF_MRP_DCM_COM_MODE_REQ             1u
#define BSWM_NO_OF_ECUM_INDICATION                  0u
#define BSWM_NO_OF_ECUMRUNREQ_STATES                0u
#define BSWM_NO_OF_ECUMWKP_SOURCES                  1u
#define BSWM_NO_OF_MRP_ETHIF_PORTFGROUP             0u
#define BSWM_NO_OF_MRP_ETHSM_INDICATION             0u
#define BSWM_NO_OF_MRP_FRSM_INDICATION              0u
#define BSWM_NO_OF_GENREQS                          0u
#define BSWM_NO_OF_BSWMODENOTIFICATION              3u 
#define BSWM_NO_OF_J1939DCM_CHANNELS                0u
#define BSWM_NO_OF_J1939NM_CHANNELS                 0u
#define BSWM_NO_OF_LINSM_CHANNELS                   0u
#define BSWM_NO_OF_LINSMSCH_CHANNELS                0u
#define BSWM_NO_OF_LINTP_CHANNELS                   0u
#define BSWM_NO_OF_NM_CHANNELS                      0u
#define BSWM_NO_OF_NVM_REQUESTS                     0u
#define BSWM_NO_OF_NVM_SERVICE_IDS                  2u
#define BSWM_NO_OF_SDCLIENTSRV_CS                   0u    
#define BSWM_NO_OF_SDCNSEVGRP_HANDLE_CS            0u 
#define BSWM_NO_OF_SDEVNTHNDLR_HANDLE_CS           0u 
#define BSWM_NO_OF_TIMER                           0u

#define BSWM_NO_OF_MRP_SWCMODENOTIF                 1u
#define BSWM_NO_OF_MRP_SWCMODEREQ                   0u
#define BSWM_NO_OF_GENREQ_TOTAL                     (BSWM_NO_OF_GENREQS + BSWM_NO_OF_BSWMODENOTIFICATION + BSWM_NO_OF_MRP_SWCMODENOTIF + BSWM_NO_OF_MRP_SWCMODEREQ)
#define BSWM_MRP_WITH_NO_CHANNEL_SOURCE             0u

#define BSWM_NO_OF_MRP_DEFERREDREQ                  4

/* BswMBswModeNotification - IDs of BswMBswModeNotification */
#define BSWM_CFG_USERID_BSWM_MRP_BSWM_MDG    0

#define BSWM_CFG_USERID_BSWM_MRP_DCM_DIAGNOSTICSESSIONCONTROL    1

#define BSWM_CFG_USERID_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET    2

/* BswMSwcModeNotification - IDs of BswMSwcModeNotification */
#define BSWM_CFG_USERID_BSWM_MRP_SWC_APPMODE    3

/* MRP array indexes for ComM Indication MRPs */
/* ComM Indication -- Indexes of the ComM Indication referenced by BswM -- BswMModeRequesterId*/
#define BSWM_IDX_BSWM_MRP_COMM_CAN_NETWORK_ETAS         0

/* MRP array indexes for Dcm Com Mode Request MRPs */
/* Dcm Com Mode Request -- Indexes of the Dcm Com Mode Request referenced by BswM -- BswMModeRequesterId*/
#define BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS         0

/* EcuM Wakeup source requests -- Indexes of the EcuM wakeup source referenced by BswM */
#define BSWM_IDX_BSWM_MRP_WKSRC_PWR         0

/* BswMBswModeNotification - Indexes of BswMBswModeNotification */
#define BSWM_IDX_BSWM_MRP_BSWM_MDG         0

#define BSWM_IDX_BSWM_MRP_DCM_DIAGNOSTICSESSIONCONTROL         1

#define BSWM_IDX_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET         2

/* BswMSwcModeNotification - Indexes of BswMSwcModeNotification */
#define BSWM_IDX_BSWM_MRP_SWC_APPMODE         3

/* NvM Job Mode Notification -- Indexes of the NvM Job Mode Notification referenced by BswM */
#define BSWM_IDX_BSWM_MRP_NVM_READALLCOMPLETE         0
#define BSWM_IDX_BSWM_MRP_NVM_WRITEALLCOMPLETE         1

/* NvM service IDs to process the mode notifications */
#define BSWM_NVMREADALL                 (0x0c)
#define BSWM_NVMWRITEALL                (0x0d)
#define BSWM_NVMRBFIRSTINITALL          (0xfa)
#define BSWM_NVMRBREMOVENONRESISTANT    (0xfe)

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */

/* Structure to hold the mode value of ComM Indication MRPs */
typedef struct {
	boolean isValidModePresent_b;
	ComM_ModeType dataMode_u8;
} BswM_Cfg_MRP_PC_ComMIndicationType_tst;

/* Structure to hold the mode value of Dcm Com Mode Request MRPs */
typedef struct {
	boolean isValidModePresent_b;
	Dcm_CommunicationModeType dataMode_u8;
} BswM_Cfg_MRP_PC_DcmComModeRequestType_tst;

/* Structure to hold the state value of EcuM Wakeup Source MRPs */
typedef struct {
	boolean isValidModePresent_b;
	EcuM_WakeupStatusType dataState;
} BswM_Cfg_PCEcuMWkpSrcType_tst;

/* Structure to hold the mode value of Generic Request MRPs */
typedef struct {
	boolean isValidModePresent_b;
	BswM_ModeType dataMode_u16;
} BswM_Cfg_MRP_PC_GenReqType_tst;

/* Structure to hold the state value of NvM Job Mode Notification MRPs */
typedef struct {
	boolean isValidModePresent_b;
	NvM_RequestResultType dataMode_en;
} BswM_Cfg_PCNvMJobModeType_tst;

/* Structure to hold the Deferred Requested for MRPs */
typedef struct {
	const BswM_Cfg_MRPType_ten dataMRPType_en;
	const uint32 dataChannelID_u32;
	const uint16 *adrAssociatedRules_pu16;
	uint16 nrAssociatedRules_u16;
	boolean isDeferredReqstPresent_b;
} BswM_Cfg_MRP_PC_DeferredReqst_tst;

/*
 **********************************************************************************************************************
 * Extern Variables
 **********************************************************************************************************************
 */

#define BSWM_START_SEC_VAR_INIT_UNSPECIFIED
#include "BswM_MemMap.h"

extern BswM_Cfg_MRP_PC_ComMIndicationType_tst BswM_Cfg_ComMIndicationModeInfo_ast[BSWM_NO_OF_MRP_COMM_INDICATION];

extern BswM_Cfg_MRP_PC_DcmComModeRequestType_tst BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_NO_OF_MRP_DCM_COM_MODE_REQ];

extern BswM_Cfg_PCEcuMWkpSrcType_tst BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_NO_OF_ECUMWKP_SOURCES];

extern BswM_Cfg_MRP_PC_GenReqType_tst BswM_Cfg_GenericReqModeInfo_ast[BSWM_NO_OF_GENREQ_TOTAL];

extern BswM_Cfg_PCNvMJobModeType_tst BswM_Cfg_NvMJobModeInfo_ast[BSWM_NO_OF_NVM_SERVICE_IDS];

extern BswM_Cfg_MRP_PC_DeferredReqst_tst BswM_Prv_DeferredReqstInfo_ast[BSWM_NO_OF_MRP_DEFERREDREQ];

#define BSWM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "BswM_MemMap.h"

#endif /* BSWM_CFG_MRP_H */
/*********************************************************************************************************************/

