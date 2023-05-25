/* ==========================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
========================================== */

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
**  FILENAME  : Gpt_Cfg.h                                                     **
**                                                                            **
**                                                                            **
**  DATE, TIME: 2022-12-08, 18:00:15                                      **
**                                                                            **
**  GENERATOR : Build b210315-0853                                          **
**                                                                            **
**  AUTHOR    : Calterah Semiconductor Inc                                    **
**                                                                            **
**  VENDOR    : Calterah Semiconductor                                        **
**                                                                            **
**  DESCRIPTION  : GPT configuration generated out of ECU configuration       **
**                 file  (Gpt.bmd)                                            **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

#ifndef GPT_CFG_H
#define GPT_CFG_H

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/

/* Standard info */
#define GPT_VENDOR_ID		0x8b		        
#define GPT_MODULE_ID       100	
#define GPT_INSTANCE_ID     0	

#define GPT_AR_RELEASE_MAJOR_VERSION	4
#define GPT_AR_RELEASE_MINOR_VERSION	3    
#define GPT_AR_RELEASE_PATCH_VERSION	1 

#define GPT_SW_MAJOR_VERSION    3	
#define GPT_SW_MINOR_VERSION    4	
#define GPT_SW_PATCH_VERSION    0	

/* Required variables */
#define GPT_DEV_ERROR_DETECT                  (STD_OFF)
#define GPT_VERSION_INFO_API                  (STD_OFF)
#define GPT_DEINIT_API                        (STD_ON)
#define GPT_TIME_ELAPSED_API                  (STD_ON)
#define GPT_TIME_REMAINING_API                (STD_ON)
#define GPT_ENABLE_DISABLE_NOTIFICATION_API   (STD_ON)
#define GPT_WAKEUP_FUNCTIONALITY_API          (STD_OFF)
#define GPT_REPORT_WAKEUP_SOURCE              (STD_OFF)
#define GPT_PREDEFTIMER_FUNCTIONALITY_API      (STD_OFF)


/* Total no of channels configured */
#define GPT_CHANNEL_CNT               (2U)

/* Symbolic names */
#define GptConf_GptChannelConfiguration_GptChannelConfiguration_0    ((Gpt_ChannelType)0U)
#define GptConf_GptChannelConfiguration_GptChannelConfiguration_1    ((Gpt_ChannelType)1U)

#endif
