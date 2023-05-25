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
**  FILENAME  : Mcu_PBCfg.c                                                   **
**                                                                            **
**                                                                            **
**  DATE, TIME: 2022-12-11, 15:30:19                                      **
**                                                                            **
**  GENERATOR : Build b210315-0853                                          **
**                                                                            **
**  AUTHOR    : Calterah Semiconductor Inc                                    **
**                                                                            **
**  VENDOR    : Calterah Semiconductor                                        **
**                                                                            **
**  DESCRIPTION  : MCU configuration generated out of ECU configuration       **
**                 file  (Mcu.bmd)                                            **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                       Includes                                             **
*******************************************************************************/
/* Include module header file */
#include "Mcu.h"

#define MCU_START_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Mcu_MemMap.h"

const Mcu_RamSectionSettingConfigType McuRamSectionConfigData[] =
{
    {
	.McuRamSectionBaseAddr = 0x770000, 
	.McuRamDefaultData     = 0x0, 
    .McuRamSectionSize     = 0x100               
  }
};


const Mcu_CanClockSourceConfigType McuCanClockSourceConfigData[] =
{
  {
		
	.Can0ExtClockSource = FALSE,
	.Can1ExtClockSource = FALSE,
	.CanExtClockFreq    = 0U
	
  }
};


const Mcu_ClockSettingConfigType McuClockSettingConfigData[] =
{
   {
	        
      /* Divider values */
	  .AhbClkDiv       = 1U, 
	  .ApbClkDiv       = 3U,
	  .ApbClkRefDiv    = 0U,
	  .Can0ClkDiv      = 9U,
	  .Can1ClkDiv      = 9U,
	  .CpuClkDiv       = 0U,
	  .MemClkDiv       = 0U,
	              
    
    .McuClockEnableConfig = 0x01f6ffff,
        .Mcu_CanClockSourceConfig = &McuCanClockSourceConfigData[0]
   }
};

const Mcu_ConfigType McuConfigData =
{
    .McuRamSector = 1U,
  
    .NumOfClockSetting = 1U,   

    
    .McuPeripheralResetConfig = 0x003fffff,
    
    .McuRamSettingConfig = &McuRamSectionConfigData[0],    
    .McuClockSettingConfig = &McuClockSettingConfigData[0]      
};



#define MCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Mcu_MemMap.h"
