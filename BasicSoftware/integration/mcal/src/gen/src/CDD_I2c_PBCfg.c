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
**  FILENAME  : CDD_I2c_PBCfg.c                                                   **
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
**  DESCRIPTION  : I2C configuration file                                     **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

/* includes */
#include "CDD_I2c.h"

#define I2C_START_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "I2c_MemMap.h"


const I2c_ConfigType I2C_CfgData = 
{
		.I2C_Speed = I2C_STANDARD,
	.I2C_addrmode = I2C_7Bit_Addr,
	.I2C_SCL_highcnt = 1950U,
	.I2C_SCL_lowcnt = 1980U,
	.I2C_rxholdtime = 0U,
	.I2C_txholdtime = 1U,
	.I2C_suppressionlimit = 5U
};

#define I2C_STOP_SEC_CONFIG_DATA_UNSPECIFIED //ETAS BIP Porting
//ETAS BIP Porting TODO #include "I2c_MemMap.h"
