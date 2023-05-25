

#ifndef CANNM_PRV_H
#define CANNM_PRV_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm.h"

#include "CanNm_Cbk.h"


#include "CanNm_Cfg_SchM.h"

#if (CANNM_DEV_ERROR_DETECT != STD_OFF)
#include "Det.h"
#if (!defined(DET_AR_RELEASE_MAJOR_VERSION) || (DET_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(DET_AR_RELEASE_MINOR_VERSION) || (DET_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#include "CanSM_TxTimeoutException.h"
#if (!defined(CANSM_AR_RELEASE_MAJOR_VERSION) || (CANSM_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANSM_AR_RELEASE_MINOR_VERSION) || (CANSM_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#if (( CANNM_COM_USER_DATA_SUPPORT != STD_OFF) || ( CANNM_PN_ENABLED != STD_OFF) )
#include "PduR_CanNm.h"
#if (!defined(PDUR_AR_RELEASE_MAJOR_VERSION) || (PDUR_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(PDUR_AR_RELEASE_MINOR_VERSION) || (PDUR_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* Get current Sw Free Running time */
#define CanNm_GetCurrFRTime()                                  (RamPtr_ps->ctSwFrTimer)

/* Check whether a certain timer has timed-out or not */
#define CanNm_TimerExpired(PrevTimeStamp, Timeout)             ((CanNm_GetCurrFRTime() - (PrevTimeStamp)) >= (Timeout))

/* Start the timer Timer */
#define CanNm_StartTimer(Timer)                                ((Timer) = (CanNm_GetCurrFRTime()))

/* Definition for variables to hold the Pdu position */

/* Macro when Byte 0 is used */
#define CANNM_PDU_BYTE_0                                       0x00U

/* Macro when Byte 1 is used */
#define CANNM_PDU_BYTE_1                                       0x01U

/* macro when Node Identification is not used */
#define CANNM_PDU_OFF                                          0xFFU

/* macro used for Default user byte */
#define CANNM_DEFAULT_USER_BYTE                                0xFFU

/* Used for invalid handle ids */
#define CANNM_INVALID_HANDLE                                   0xFFU

/*Used to read the Repeat Message bit */
#define CANNM_READ_RPTMSG_MASK                                 0x01U

/* Used to reset the Repeat Message Bit inside Control Bit Vector */
#define CANNM_RESET_RPTMSG_MASK                                0xFEU

/* Used to reset the Control Bit Vector */
#define CANNM_RESET_CONTROL_BIT_VECTOR_MASK                    0x00U

/* Used to read the NM Coord bits */
#define CANNM_READ_NMCOORD_MASK                                0x06U

/* Position of the PNI status in CBV */
#define CANNM_CBV_PNI_STATUS                                   0x40U

/* Position of the active wakeup bit position in CBV */
#define CANNM_ACTIVE_WAKEUP_BIT_POS                            (0x10U)


/* Service IDs of CanNm APIs */
/* Indicates the Api Id passed in the call of Det_ReportError for corresponding API's*/

#define CANNM_SID_INIT                                                    (0)
#define CANNM_SID_PASSIVESTARTUP                                          (0x01)
#define CANNM_SID_NETWORKREQUEST                                          (0x02)
#define CANNM_SID_NETWORKRELEASE                                          (0x03)
#define CANNM_SID_DISABLECOMMUNICATION                                    (0x0C)
#define CANNM_SID_ENABLECOMMUNICATION                                     (0x0D)
#define CANNM_SID_SETUSERDATA                                             (0x04)
#define CANNM_SID_GETUSERDATA                                             (0x05)
#define CANNM_SID_GETNODEIDENTIFIER                                       (0x06)
#define CANNM_SID_GETLOCALNODEIDENTIFIER                                  (0x07)
#define CANNM_SID_REPEATMESSAGEREQUEST                                    (0x08)
#define CANNM_SID_GETPDUDATA                                              (0x0A)
#define CANNM_SID_GETSTATE                                                (0x0B)
#define CANNM_SID_GETVERSIONINFO                                          (0xF1)
#define CANNM_SID_REQUESTBUSSYNCHRONIZATION                               (0xC0)
#define CANNM_SID_CHECKREMOTESLEEPINDICATION                              (0xD0)
#define CANNM_SID_TRANSMIT                                                (0x14)
#define CANNM_SID_TXCONFIRMATION                                          (0x40)
#define CANNM_SID_RXINDICATION                                            (0x42)
#define CANNM_SID_MAINFUNCTION                                            (0x13)
#define CANNM_SID_CONFIRMPNAVAILABILITY                                   (0x16)
#define CANNM_SID_SETSLEEPREADYBIT                                        (0x17)
#define CANNM_COORD_READY_SLEEP_BIT_MASK                                  (0x08u)

#if (CANNM_DEV_ERROR_DETECT != STD_OFF)

/* Macros For functions with return type as 'void' and with no return statement */
#define CANNM_DET_REPORT_ERROR_NO_RET(CONDITION,InstanceId, ApiId, ErrorId)                                            \
                                 if(CONDITION)                                                                \
                                   {                                                                            \
                                        (void)Det_ReportError((CANNM_MODULE_ID),(InstanceId),(ApiId),(ErrorId));      \
                                   }

/* Macros For functions with return type as 'void'*/
#define CANNM_DET_REPORT_ERROR(CONDITION,InstanceId, ApiId, ErrorId)                                            \
                                 if(CONDITION)                                                                \
                                   {                                                                            \
                                       (void)Det_ReportError((CANNM_MODULE_ID),(InstanceId),(ApiId),(ErrorId));      \
                                        return;                                                                 \
                                   }
/* Macros For functions with return type as 'Std_ReturnType' */
#define CANNM_DET_REPORT_ERROR_NOK(CONDITION,InstanceId, ApiId, ErrorId)                                        \
                                 if(CONDITION)                                                                \
                                   {                                                                            \
                                        (void)Det_ReportError((CANNM_MODULE_ID),(InstanceId),(ApiId),(ErrorId));      \
                                        return E_NOT_OK;                                                     \
                                   }

#else

#define CANNM_DET_REPORT_ERROR(CONDITION,InstanceId, ApiId, ErrorId)           do{}while(0);
#define CANNM_DET_REPORT_ERROR_NOK(CONDITION,InstanceId, ApiId, ErrorId)       do{}while(0);
#define CANNM_DET_REPORT_ERROR_NO_RET(CONDITION,InstanceId, ApiId, ErrorId)    do{}while(0);

#endif

#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
/* Macro For accessing channel configuration data with Network handle when post build variant is selected  */
#define CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle)                     (&CanNm_GlobalConfigData_pcs->ChannelConfig_pcs[CanNm_NetworkHandle])
#else
/* Macro For accessing channel configuration data with Network handle when precompile variant is selected */
#define CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle)                     (&CanNm_ChannelConfigData_cs[CanNm_NetworkHandle])
#endif

#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
#define CANNM_GET_NUMBER_OF_PN_CONFIGURED()                               (CanNm_GlobalConfigData_pcs->NumOfPnsConfigured_u8)
#else
#define CANNM_GET_NUMBER_OF_PN_CONFIGURED()                               (CANNM_NUMBER_OF_PNS)
#endif


#define CANNM_REMOTE_SLEEP_DETECTED()                                     ((RamPtr_ps->stRemoteSleepInd) != (FALSE))
#define CANNM_RESET_REMOTE_SLEEP()                                        ((RamPtr_ps->stRemoteSleepInd) = (FALSE))
#define CANNM_SET_REMOTE_SLEEP()                                          ((RamPtr_ps->stRemoteSleepInd) = (TRUE))

/***** GETTERS for config parameters ******/

#define CANNM_GetPnEnabledStatus(ConfigPtr_pcs)                           ((ConfigPtr_pcs)->PnEnabled_b)
#define CANNM_GetAllNmMessagesKeepAwake(ConfigPtr_pcs)                    ((ConfigPtr_pcs)->AllNmMessagesKeepAwake_b)
#if (CANNM_ERACALC_ENABLED != STD_OFF)
#define CANNM_GetEraEnabledStatus(ConfigPtr_pcs)                          ((ConfigPtr_pcs)->EraCalcStatus_b)
#endif

/***** GETTERS & SETTERS for RAM parameters ******/

#define CANNM_GetPnMsgFilteringStatus(RamPtr_ps)                          ((RamPtr_ps)->PnMsgFilteringEnabled_b)
#define CANNM_SetPnMsgFilteringStatus(RamPtr_ps)                          ((RamPtr_ps)->PnMsgFilteringEnabled_b = (TRUE))
#define CANNM_ResetPnMsgFilteringStatus(RamPtr_ps)                        ((RamPtr_ps)->PnMsgFilteringEnabled_b = (FALSE))
#define CANNM_GetStartIndex_ERAStatus(ChannelOffset)                      ((ChannelOffset) * CANNM_PN_INFOLENGTH)
#define CANNM_GetStartIndex_ERATimer(ChannelOffset)                       ((ChannelOffset) * (CANNM_GET_NUMBER_OF_PN_CONFIGURED()))

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/* Type definition for variables to hold Network States */
typedef enum
{
    CANNM_NETWORK_RELEASED_E = 0,
    CANNM_NETWORK_REQUESTED_E

} CanNm_NetworkStateType;

typedef struct
{
    PduInfoType                                     Pdu_s; /* To store Tx PDU information */
    CanNm_TimerType                                 ctSwFrTimer; /* Free running Timer */
    CanNm_TimerType                                 MsgCyclePeriod; /* To hold periodicity of NM PDUs */
    CanNm_TimerType                                 PrevMsgCycleTimestamp; /*To monitor the message cycle timer */
    CanNm_TimerType                                 PrevMsgTimeoutTimestamp; /* To monitor the Msg timeout timer */
    CanNm_TimerType                                 ctRepeatMessageTimer; /* To monitor Repeat Message Time */
    CanNm_TimerType                                 ctNMTimeoutTimer; /* To monitor NM-Timeout Time */
    CanNm_TimerType                                 ctWaitBusSleepTimer; /* To monitor Wait Bus Sleep */
    CanNm_TimerType                                 ctRemoteSleepIndTimer; /* To monitor Remote Sleep indication time */
    uint8                                           RxBuffer_au8[CANNM_PDU_LENGTH_MAX]; /* To store the received Pdu */
    uint8                                           TxBuffer_au8[CANNM_PDU_LENGTH_MAX]; /* To store the Tx Pdu */
    Nm_StateType                                    State_en; /* Stores current NM state */
    CanNm_NetworkStateType                          NetworkReqState_en; /* Stores current Network Request state */
    uint8                                           UserDataBuffer_au8[CANNM_PDU_LENGTH_MAX]; /* To store the received User bytes */
    Nm_ModeType                                     Mode_en; /* Stores current NM mode */
    uint8                                           RxNodeId_u8; /*To store the received Node Id*/
uint8                                               RxCtrlBitVector_u8; /*To store the received CBV*/
    uint8                                           TxCtrlBitVector_u8; /*To store CBV to be transmitted*/
    uint8                                           ImmediateNmTx_u8; /* To store number of Immediate Nm Pdus to be transmitted*/
#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)
    uint8                                           CoordReadySleepBit_u8; /*Stores NmCoordinatorSleepReady bit status*/
#endif
#if (CANNM_PN_ENABLED != STD_OFF)
    boolean                                         PnMsgFilteringEnabled_b; /*Is PN active for this channel, at a given point of time*/
    boolean                                         PnHandleMultipleNetworkRequests_b; /* Flag to store status */
#endif
    boolean                                         MsgTxStatus_b; /*To maintain the status of message tranmsission */
#if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
    boolean                                         TxRptMsgStatus_b; /* To maintain the status of repeat message bit */
#endif
    boolean                                         RxIndication_b; /* Stores Rx indication */
    boolean                                         TxConfirmation_b; /* Stores Tx confirmation */
    boolean                                         RxStatus_b; /* Holds reception status for one NM Sleep-Cycle */
    boolean                                         stCanNmComm; /* Stores CanNm communication status */
    boolean                                         stRemoteSleepInd; /* Stores Remote Sleep Indication status*/
	boolean											TxTimeoutMonitoringActive_b; /* Msgtimeouttime monitoring flag */
#if (CANNM_RETRY_FIRST_MSG_REQ_ENABLED != STD_OFF)
	boolean                                         FirstMessage_b; /* Flag to indicate first message after transition from Bus Sleep Mode to repeat Message State*/
    boolean                                         FirstTransmissionFailure_b; /* To store transmission failure from CanIf */
#endif
#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)
    boolean                                         stCoordReadySleepInd_b; /*Stores CoordReadySleep indication status*/
#endif
} CanNm_RamType;

/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */

/* Global Config data structure to access Channel related information */
#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
/*CanNm_ChannelConfigData_cs made a  variable for SIM testing purpose only*/
#if defined(BSWSIM)
extern CanNm_ChannelConfigType CanNm_ChannelConfigData_cs[];
#else
extern const CanNm_ChannelConfigType CanNm_ChannelConfigData_cs[];
#endif
#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"

#if ((CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)||(CANNM_PN_ENABLED != STD_OFF))

/*  Global RAM data structure to access PN related information */
#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
extern const CanNm_GlobalConfigType CanNm_GlobalConfigData_cs;
#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"

/* Pointer to constant structure which holds the global config data main related to Partial Networking */
#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"
extern const CanNm_GlobalConfigType * CanNm_GlobalConfigData_pcs;
#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

#endif

/* RAM data structure */
#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"
extern CanNm_RamType CanNm_RamData_s[];
#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

#if(CANNM_COMM_LINEAR!=STD_ON)
/* CanNm Internal Handle Table */
#define CANNM_START_SEC_CONST_UNSPECIFIED
#include "CanNm_MemMap.h"
extern const NetworkHandleType CanNm_HandleTable[];
#define CANNM_STOP_SEC_CONST_UNSPECIFIED
#include "CanNm_MemMap.h"
#endif

/* Timer for each PN  */
#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"
extern CanNm_TimerType CanNm_PNTimer_au32[];
#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"
/* Array to hold the EIRA Global and current status of all relevant PN  */
extern uint8 CanNm_EIRAGlobalStatus_au8[];
extern uint8 CanNm_EIRACurrentStatus_au8[];
#define CANNM_STOP_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"

/* Timer for each PN across the channels for ERA calculation */
#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"
extern CanNm_TimerType CanNm_PNTimer_ERA_au32[];
#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"
/*Array to hold the ERA Global and current status of all relevant PN */
extern uint8 CanNm_ERAGlobalStatus_au8[];
extern uint8 CanNm_ERACurrentStatus_au8[];
#define CANNM_STOP_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_ChangeState(
                                                  CanNm_RamType * RamPtr_ps,
                                                  Nm_StateType nmState_en,
                                                  Nm_ModeType nmMode_en
                                               );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_StartTransmission( const NetworkHandleType CanNm_NetworkHandle );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#if ((CANNM_CONFIGURATION_VARIANT == CANNM_VARIANT_POSTBUILD_LOADABLE) && (CANNM_VERSION_INFO_API != STD_OFF))
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNmAppl_IncompatibleGenerator(void);
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#endif /* CANNM_PRV_H */


