/*=============================================================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
==============================================================================*/
/*******************************************************************************
**                                                                            **
** Copyright (c) 2020, Calterah Semiconductor, Inc.            				  **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to 				  **
** Calterah Semiconductor Technology. Passing on and copying of this document,** 
** and communication of its contents is not permitted without prior written   **
** authorization.      														  **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME  : Can_Cfg.h							                          **
**                                                                            **
**                                                                            **
**  DATE, TIME: 2022-12-08, 18:00:15                                      **
**                                                                            **
**  GENERATOR : Build b210315-0853                                          **
**                                                                            **
**  AUTHOR    : Calterah Semiconductor										  **
**                                                                            **
**  VENDOR    : Calterah Semiconductor                                        **
**                                                                            **
**  DESCRIPTION  : CAN configuration generated out of ECU configuration       **
**                 file  (Can.bmd)                                            **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

#ifndef CAN_CFG_H
#define CAN_CFG_H


   
#define CAN_MODULE_ID       0x50U    /* CAN Module Id */
#define CAN_INSTANCE_ID     0x0U  /* CAN Instance Id */
#define CAN_VENDOR_ID       0x8bU    /* CAN Vendor Id */

#define CAN_AR_RELEASE_MAJOR_VERSION    4U  /* CAN Module Autosar Major Version Number */
#define CAN_AR_RELEASE_MINOR_VERSION    2U  /* CAN Module Autosar Minor Version Number */ //zco7sgh
#define CAN_AR_RELEASE_PATCH_VERSION    1U  /* CAN Module Autosar Patch Version Number */

#define CAN_SW_MAJOR_VERSION    3U  /* Software Major Version Number */
#define CAN_SW_MINOR_VERSION    4U  /* CAN Module Software Minor Version Number */
#define CAN_SW_PATCH_VERSION    0U  /* CAN Module Software Patch Version Number */

/* Pre-Compile parameters */
#define CAN_DEV_ERROR_DETECT      (STD_OFF)
#define CAN_VERSION_INFO_API      (STD_OFF)
#define CAN_SET_BAUDRATE_API      (STD_ON)
#define CAN_MAINFUNCTION_BUSOFF_PERIOD  (STD_OFF)
#define CAN_MAINFUNCTION_MODE_PERIOD    (STD_OFF)
#define SUPPORT_CAN_STATISTICS          (STD_OFF)

#define CAN_MAIN_FUNCTION_WRITE_POLLING_ALLOWED  (STD_ON)


#define CAN_MAIN_FUNCTION_READ_POLLING_ALLOWED   (STD_ON)
#define CAN_MAIN_FUNCTION_BUSOFF_POLLING_ALLOWED   (STD_ON)
/* Total Number Of CAN Controllers Configured */
#define CAN_ALPS_CTRL_CONFIG_COUNT      (1U)
 
/* Total Number Of HRH Configured*/
#define CAN_NUM_OF_HRHS 		 (7U)

/* Total Number Of HTH Configured*/
#define CAN_NUM_OF_HTHS  		 (6U)

#define CAN_TRIGGER_TRANSMIT_USED (STD_OFF)

/* Support CAN/CAN FD*/
#define CAN_FD_SUPPORTED      (STD_OFF)

/* Can Controller ID */
#define CanConf_CanController_ETAS_CAN   ((CanControllerIdType)0U)

/* Can Hardware Object ID */
#define CanConf_ETAS_CAN_Rx_Std_MailBox_1      0         
#define CanConf_ETAS_CAN_Rx_Std_MailBox_2      1         
#define CanConf_ETAS_CAN_Rx_Std_MailBox_3      2         
#define CanConf_ETAS_CAN_Rx_Std_MailBox_4      3         
#define CanConf_ETAS_CAN_Rx_Std_MailBox_5      4         
#define CanConf_ETAS_CAN_Rx_Std_MailBox_6      5   
#define CanConf_ETAS_CAN_Rx_Std_MailBox_7      6      
#define CanConf_ETAS_CAN_Tx_Std_MailBox_1      7         
#define CanConf_ETAS_CAN_Tx_Std_MailBox_2      8         
#define CanConf_ETAS_CAN_Tx_Std_MailBox_3      9         
#define CanConf_ETAS_CAN_Tx_Std_MailBox_4      10         
#define CanConf_ETAS_CAN_Tx_Std_MailBox_5      11   
#define CanConf_ETAS_CAN_Tx_Std_MailBox_6      12      

/* BaudRate Config ID */
#define CanConf_CanController_ETAS_CAN_ETAS_CAN_BR_0   ((uint32)0U)

/*initialization parameter definition*/
extern const Can_ConfigType CanConfigData;


/* CAN ID index array number */#define CANID_STD_NUM 				8U
#define CANID_EXT_NUM	 			1U  

#endif
