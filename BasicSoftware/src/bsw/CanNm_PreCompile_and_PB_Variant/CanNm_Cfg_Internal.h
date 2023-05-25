
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanNm / AR42.10.0.0             Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1               Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 </VersionHead>*/

#ifndef CANNM_CFG_INTERNAL_H
#define CANNM_CFG_INTERNAL_H

/**
 *******************************************************************************
 * \module description
 *                      Can NM configuration header file. This file is generated
 by CUBEC
 *
 *                      This is included by the cannm.h file. So, all the
 parameters are exported.
 * \scope
 *******************************************************************************
 */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/*
 ******************************************************************************
 * Defines
 ******************************************************************************
 */

/* Check if channel mapping between CanNm and ComM is linear */
#define CANNM_COMM_LINEAR                                                 STD_ON

/* Enable/disable the development error detection */
#define CANNM_DEV_ERROR_DETECT                                            STD_ON

/* Enable/disable the function CanNm_GetVersionInfo */
#define CANNM_VERSION_INFO_API                                            STD_OFF

/* Enable/disable the user data support */
#define CANNM_USER_DATA_ENABLED                                           STD_ON

/* Enable/disable the COM user data support */
#define CANNM_COM_USER_DATA_SUPPORT                                       STD_ON

/* Enable/disable the NM Coordination Sync Feature */
#define CANNM_COORDINATOR_SYNC_SUPPORT                                    STD_OFF

/* Enable support of Remote Sleep Indication */
#define CANNM_REMOTE_SLEEP_IND_ENABLED                                    STD_OFF

/* Enable support of Bus Synchronization */
#define CANNM_BUS_SYNCHRONIZATION_ENABLED                                 STD_OFF

/* Enable/disable the node detection functionality */
#define CANNM_NODE_DETECTION_ENABLED                                      STD_ON

/* Enable/disable the bus load reduction mechanism */
#define CANNM_BUS_LOAD_REDUCTION_ENABLED                                  STD_OFF

/* Enable support of source Node Identifier */
#define CANNM_NODE_ID_ENABLED                                             STD_ON

/* Enable support of Immediate Restart in PrepareBusSleep */
#define CANNM_IMMEDIATE_RESTART_ENABLED                                   STD_OFF

/* Enable/disable the passive mode */
#define CANNM_PASSIVE_MODE_ENABLED                                        STD_OFF

/* Enable support of indication of received PDUs */
#define CANNM_PDU_RX_INDICATION_ENABLED                                   STD_ON

/* Enable support of indication of state changes */
#define CANNM_STATE_CHANGE_IND_ENABLED                                    STD_ON

/* Enable support of ISO i-14229 Communication Control service */
#define CANNM_COM_CONTROL_ENABLED                                         STD_ON

/* Enable/disable the immediate transmit confirmation */
#define CANNM_IMMEDIATE_TXCONF_ENABLED                                    STD_OFF

/* Enable/disable the notification that a RepeatMessageRequest bit has been received */
#define CANNM_REPEAT_MSG_IND_ENABLED                                      STD_OFF

/* Enables or disables support of CarWakeUp bit evaluation in received NM PDUs */
#define CANNM_CAR_WAKEUP_RX_ENABLED                                       STD_OFF

/* Enables or disables support of CarWakeUp bit evaluation in received NM PDUs */
#define CANNM_CAR_WAKEUP_FILTER_ENABLED                                   STD_OFF

/* Enable/Disable support of CanNmRetryFirstMessageRequest feature */
#define CANNM_RETRY_FIRST_MSG_REQ_ENABLED				                  STD_OFF

/* Enable/disable Partial Networking extensions */
#define CANNM_PN_ENABLED                                                  STD_OFF

/* Enable/disable EIRA calculation */
#define CANNM_EIRACALC_ENABLED                                            STD_OFF

/* Enable/disable ERA calculation */
#define CANNM_ERACALC_ENABLED                                             STD_OFF

/* Defines the number of CanNm channels */
#define CANNM_NUMBER_OF_CHANNELS                                          1U

/* Length, in bytes, of the Pn Information in User Data */
#define CANNM_PN_INFOLENGTH                                               0U

/* Get the Length of EIRA PDU */
#define CANNM_EIRA_PDU_LENGTH                                             0U

/* Get the maximum Length of all ERA PDUs */
#define CANNM_ERA_PDU_LENGTH_MAX                                          0U

/*RTE Enabled or Disabled*/
#define CANNM_ECUC_RB_RTE_IN_USE 				         STD_ON

/* Number of Maximum PN's configurable during post build time: Useful for EIRA and ERA calculation */
#define CANNM_MAX_NUMBER_OF_PNS                                           0U

/* Offset of the PN request information in the NM message */
#define CANNM_PN_INFO_OFFSET                                              0U

#if (CANNM_EIRACALC_ENABLED != STD_OFF)

/* Pdu id of EIRA referred pdu in Com */
#define CANNM_PNEIRA_RXPDU_ID                                             0U

#endif

#if (CANNM_ERACALC_ENABLED != STD_OFF)

/* Number of CanNm channels with ERA enabled */
#define CANNM_NUMBER_OF_ERA_CHANNELS                                      0U

/* Size of the ERA array */
#define CANNM_ERA_ARRAYSIZE                                               (CANNM_PN_INFOLENGTH * CANNM_NUMBER_OF_ERA_CHANNELS)

#endif

/* Maximum Pdu length */
#define CANNM_PDU_LENGTH_MAX                                              8U

/* Defines the scheduling raster in micro-seconds for all channels */
#define CANNM_MAINFUNCTION_PERIOD       10000U

/* Size of the Mapping Table based on the maximum value of ChannelID refered by CanNm in ComM */
#define CANNM_MAPPING_TABLE_SIZE                                          (1)

/* Macro which receives ComM Network Handle and returns the matching CanNm Network Handle */
#define CANNM_GET_HANDLE(nmChannelHandle)     (nmChannelHandle) 

/*
 *******************************************************************************
 * Type definitions
 *******************************************************************************
 */

/*
 *******************************************************************************
 * Extern declarations
 *******************************************************************************
 */

/*
 *******************************************************************************
 * Prototypes
 *******************************************************************************
 */

#endif /* CANNM_CFG_INTERNAL_H */
