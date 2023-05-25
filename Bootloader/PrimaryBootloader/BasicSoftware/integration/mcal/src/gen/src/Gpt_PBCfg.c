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
**  FILENAME  : Gpt_PBCfg.c                                                   **
**                                                                            **
**                                                                            **
**  DATE, TIME: 2022-12-23, 16:22:51                                      **
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

/*******************************************************************************
**                       Includes                                             **
*******************************************************************************/
/* Include module header file */
#include "Gpt.h"

extern void NotifCh0 (void);
extern void NotifCh1 (void);

#define GPT_START_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Gpt_MemMap.h"	

const Gpt_ChannelConfigType GptChannelConfigData[] = 
{
	{	

		.GptChannelId = 0,
		.GptHwChannel = GPT_HW_TIMER2,
		.GptChannelMode = GPT_CH_MODE_ONESHOT,
		.GptChannelTickValueMax = 4294967295,
		.GptNotification = NotifCh0
	}, 
	{	

		.GptChannelId = 1,
		.GptHwChannel = GPT_HW_TIMER3,
		.GptChannelMode = GPT_CH_MODE_CONTINUOUS,
		.GptChannelTickValueMax = 4294967295,
		.GptNotification = NotifCh1
	}
};


const Gpt_ConfigType GptConfigData = 
{
	.NumOfChannel = 2,
	.GptChannelConfig = &GptChannelConfigData[0]
};

	
#define GPT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Gpt_MemMap.h"
