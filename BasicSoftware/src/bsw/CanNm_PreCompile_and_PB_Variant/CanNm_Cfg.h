
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanNm / AR42.10.0.0             Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1               Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 </VersionHead>*/

#ifndef CANNM_CFG_H
#define CANNM_CFG_H

/* CanNm module and vendor id according to AUTOSAR BSW module list  */

#define CANNM_MODULE_ID                                                   31u
#define CANNM_VENDOR_ID                                                   6u

/* Major, minor and revision version of AUTOSAR CanNm specification */

#define CANNM_AR_RELEASE_MAJOR_VERSION                                    4u
#define CANNM_AR_RELEASE_MINOR_VERSION                                    2u
#define CANNM_AR_RELEASE_REVISION_VERSION                                 2u

/* Major, minor and patch version of AUTOSAR CanNm implementation */

#define CANNM_SW_MAJOR_VERSION                                            10u
#define CANNM_SW_MINOR_VERSION                                            0u
#define CANNM_SW_PATCH_VERSION                                            0u

#include "CanNm_Cfg_Internal.h"
/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */

/* Definition of supported module variants  */
#define CANNM_VARIANT_PRE_COMPILE                                         0
#define CANNM_VARIANT_LINK_TIME                                           1
#define CANNM_VARIANT_POSTBUILD_LOADABLE                                  2
#define CANNM_VARIANT_POSTBUILD_SELECTABLE                                3

#define CANNM_CONFIGURATION_VARIANT  CANNM_VARIANT_PRE_COMPILE

#if(CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
/* The below macros are required in case CanNm Init call is triggered with any of following */

#define CanNm_Config CanNmGlobalConfig





#endif

/* Macros for Symbolic Name Reference */

#define CanNmConf_CanNmUserDataTxPdu_NM_Tx_0x600_UserData_Can_Network_ETAS_Channel        0

#define CanNmConf_CanNmRxPdu_NM_Rx_0x677_Can_Network_ETAS_Channel        0

#define CanNmConf_CanNmTxPdu_NM_Tx_0x600_Can_Network_ETAS_Channel        0

/* Type definition for CanNm Timers */
typedef uint32 CanNm_TimerType;

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
/* Configuration data structure */
typedef struct {
	CanNm_TimerType MsgCycleTime; /* Timer for message cycle */
	CanNm_TimerType MsgCycleOffset; /* Timer for message cycle offset */
	CanNm_TimerType MsgTimeoutTime; /* Timer for message timeout time */
	CanNm_TimerType MsgReducedTime; /* Timer for message reduced time */
	CanNm_TimerType ImmediateNmCycleTime; /* Timer for Immediate Nm cycle */
	CanNm_TimerType NMTimeoutTime; /* Network Timeout for NM-Messages */
	CanNm_TimerType RepeatMessageTime; /* Timeout for Repeat Message State */
	CanNm_TimerType RemoteSleepIndTime; /* Timeout for Remote Sleep Indication */
	CanNm_TimerType WaitBusSleepTime; /* Timeout for bus calm down phase*/
	PduIdType TxPduId; /* Holds the CanIf transmit PDU Id */
#if (CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
    PduIdType                                PduRId; /* Holds Handle Id of PduR for TxConfimation and TriggerTransmit */
#endif
	NetworkHandleType NetworkHandle; /* Holds the Handle Id of the ComM channel */
	uint8 PduLength_u8; /* Holds the length of Pdu data */
	uint8 NodeIdPos_u8; /* Holds the position of Node Id in the Pdu */
	uint8 ControlBitVectorPos_u8; /* Holds the position of CBV in the PDU */
	boolean NodeDetectionEnabled_b; /* Node detection enabled*/
	boolean NodeIdEnabled_b; /* Node ID enabled*/
	boolean RepeatMsgIndEnabled_b; /* Repeat Message indication enabled*/
	uint8 NodeId_u8; /* Holds the Node Id */
	uint8 UserDataLength_u8; /* Holds the length of user data */
	uint8 ImmediateNmTransmissions_u8; /* Holds the number of Immediate Nm Pdus */
#if (CANNM_ERACALC_ENABLED != STD_OFF)
    uint8                                    ERA_ComPduId_u8; /* PDU id of ERA referred PDU in COM */
    uint8                                    ChannelOffset_u8;/* Offset in list of channels with ERA configured */
    boolean                                  EraCalcStatus_b; /* ERA status for each channel */
    uint8                                    Era_PduLength_u8;/* ERA PDU length */
#endif
	boolean stChannelActive_b; /* To configure which channel has to be initialized */
	boolean stBusLoadReductionActive_b; /*Indicates if Bus load reduction is enabled */
#if (CANNM_PN_ENABLED != STD_OFF)
//todo : All the booleans can be clubbed into bitmask
    boolean                                  PnEnabled_b; /* Indicates if PN is enabled for the channel */
    boolean                                  AllNmMessagesKeepAwake_b; /* Keep awake on any message */
    boolean                                  PnHandleMultipleNetworkRequests_b; /*Indicates if this feature is enabled*/
#endif
#if (CANNM_RETRY_FIRST_MSG_REQ_ENABLED != STD_OFF)
    boolean                                  RetryFirstMessageRequest_b; /* RetryFirstMessageRequest feature support */
#endif
#if (CANNM_CAR_WAKEUP_RX_ENABLED != STD_OFF)
    boolean                                  stCanNmCarWakeUpRxEnabled_b;
#if (CANNM_CAR_WAKEUP_FILTER_ENABLED != STD_OFF)
    boolean                                  stCarWakeUpFilterEnabled_b;
    uint8                                    stCarWakeUpFilterNodeId_u8;
#endif
    uint8                                    stCarWakeUpBytePosition_u8;
    uint8                                    stCarWakeUpBitPosition_u8;
#endif
	boolean ActiveWakeupBitEnabled_b; /* Handling of the Active Wakeup Bit enabled? */
} CanNm_ChannelConfigType;

/* CanNm configuration structure for each Pn */
typedef struct {
	uint8 ByteNum; /* The Byte which the PN is part of */
	uint8 BitMask; /* Corresponding Filter Mask to extract the PN info from the byte */

} CanNm_PNConfigType;

#if ((CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)||(CANNM_PN_ENABLED != STD_OFF))

typedef struct
{
#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
    /* Pointer to hold the address of channel configuration data  */
    const CanNm_ChannelConfigType * ChannelConfig_pcs;
#endif
#if (CANNM_PN_ENABLED != STD_OFF)
    /*Pointer to PN Filter Mask. This mask holds the info about which PN is relevant for the ECU */
    const uint8 * PnFilterMask_pcu8;
#endif
#if (CANNM_EIRACALC_ENABLED != STD_OFF) || (CANNM_ERACALC_ENABLED != STD_OFF)
    /*Pointer to Pn Info table  */
    const CanNm_PNConfigType * PnInfo_pcs;
    /*Timer for the reset of PN requests in the EIRA and ERA. */
    CanNm_TimerType PnResetTime;
#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
    /*Variable to hold number of PN configured  */
    uint8 NumOfPnsConfigured_u8;
#endif
#endif
} CanNm_GlobalConfigType;

#endif

/* type definition for variables to hold base configuration structure */

typedef struct {
	const void *CanNm_ConfigData;
	/* Pointer to version info of the module , generated post-build time */
	const Std_VersionInfoType *versionInfo;
} CanNm_ConfigType;

#endif /* CANNM_CFG_H */

