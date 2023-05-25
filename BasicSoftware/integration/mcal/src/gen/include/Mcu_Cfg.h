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
**  FILENAME  : Mcu_Cfg.h                                                     **
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
**  DESCRIPTION  : MCU configuration generated out of ECU configuration       **
**                 file  (Mcu.bmd)                                            **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

#ifndef MCU_CFG_H
#define MCU_CFG_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/

/* Standard info */

#define MCU_VENDOR_ID		0x8b		        
#define MCU_MODULE_ID       101	
#define MCU_INSTANCE_ID     0	

#define MCU_AR_RELEASE_MAJOR_VERSION	4
#define MCU_AR_RELEASE_MINOR_VERSION	3    
#define MCU_AR_RELEASE_PATCH_VERSION	1 

#define MCU_SW_MAJOR_VERSION    3	
#define MCU_SW_MINOR_VERSION    4	
#define MCU_SW_PATCH_VERSION    0	

/* Required variables */
#define MCU_DEV_ERROR_DETECT          (STD_OFF)
#define MCU_VERSION_INFO_API          (STD_ON)
#define MCU_PERFORM_RESET_API         (STD_ON)
#define MCU_INIT_CLOCK                (STD_ON)
#define MCU_NO_PLL                    (STD_OFF)
#define MCU_GET_RAM_STATE_API         (STD_OFF)


/* Total number of RAM sector configured */
#define MCU_NUM_RAM_SECTORS      (1U)
/* Total number of Clock Setting configured */
#define MCU_NUM_CLOCK_SETTING    (1U)

/* Ram Sector ID */
#define McuConf_McuRamSectorSettingConf_McuRamSectorSettingConf_0   ((Mcu_RamSectionType)0U)

/* Clock Setting ID */
#define McuConf_McuClockSettingConfig_McuClockSettingConfig_0    ((Mcu_ClockType)0U)

/* Mode Setting ID */
#define McuConf_McuModeSettingConf_McuModeSettingConf_0    ((Mcu_ModeType)0U)

#endif
