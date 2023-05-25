/*******************************************************************************
**                                                                            **
** Copyright (C) 2020, Calterah Semiconductor, Inc.                           **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Calterah       **
** Semiconductor, Inc. Passing on and copying of this document, and           **
** communication of its contents is not permitted without prior written       **
** authorization.                                                             **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME   : Port_Cfg.h                                                   **
**                                                                            **
**                                                                            **
**  DATE, TIME : 2022-12-08, 18:00:15                                     **
**                                                                            **
**  GENERATOR  : Build b210315-0853                                         **
**                                                                            **
**  AUTHOR     : Calterah Semiconductor                                       **
**                                                                            **
**  VENDOR     : Calterah Semiconductor                                       **
**                                                                            **
**  DESCRIPTION : PORT configuration generated out of ECU configuration       **
**                file (Port.bmd)                                             **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H



#define PORT_VENDOR_ID		0x8b		        
#define PORT_MODULE_ID       124	
#define PORT_INSTANCE_ID     0	

#define PORT_AR_RELEASE_MAJOR_VERSION	4
#define PORT_AR_RELEASE_MINOR_VERSION	3    
#define PORT_AR_RELEASE_PATCH_VERSION	1 

#define PORT_SW_MAJOR_VERSION    3	
#define PORT_SW_MINOR_VERSION    4	
#define PORT_SW_PATCH_VERSION    0

#define NOT_VALID 0xFFu

#define PORT_DEV_ERROR_DETECT       (STD_OFF)
#define PORT_VERSION_INFO_API       (STD_OFF)
#define PORT_SET_PIN_MODE_API       (STD_OFF)
#define PORT_SET_PIN_DIRECTION_API  (STD_OFF)

#endif /* PORT_CFG_H */

