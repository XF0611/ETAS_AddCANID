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
**  FILENAME  : Fls_Cfg.h                                                     **
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
**  DESCRIPTION  : Fls configuration include file                             **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/
#ifndef FLS_CFG_H
#define FLS_CFG_H


#include "MemIf_Types.h"




#define FLS_VENDOR_ID       0x8bU     			 /* FLS Vendor Id */
#define FLS_MODULE_ID       0x5cU                 /* FLS Module Id */
#define FLS_INSTANCE_ID     0x0U	             /* FLS Instance Id */

#define FLS_AR_RELEASE_MAJOR_VERSION	4
#define FLS_AR_RELEASE_MINOR_VERSION	3    
#define FLS_AR_RELEASE_PATCH_VERSION	1 

#define FLS_SW_MAJOR_VERSION    3	
#define FLS_SW_MINOR_VERSION    4	
#define FLS_SW_PATCH_VERSION    0	

/*support flash list*/
#define S25FL128_Manu_ID  0x01U            //Manufacture ID
#define S25FL128_Device_ID  0x17U          //Device ID

#define W25Q32JV_Manu_ID  0xEFU            //Manufacture ID
#define W25Q32JV_Device_ID  0x15U          //Device ID

#define MX25L3233F_Manu_ID  0xC2U            //Manufacture ID
#define MX25L3233F_Device_ID  0x15U          //Device ID

#define IS25WP032D_Manu_ID  0x9DU            //Manufacture ID
#define IS25WP032D_Device_ID  0x15U          //Device ID

#define IS25WP016D_Manu_ID  0x9DU            //Manufacture ID
#define IS25WP016D_Device_ID  0x14U          //Device ID

#define GD25Q32C_Manu_ID   0xC8U            //Manufacture ID
#define GD25Q32C_Device_ID  0x15U           //Device ID


#define FlsBaseAddress   0x10000U         //Flash base address
#define FLS_ENDADDRESS   0x20000U	             //Flash end address
#define FLS_PAGESIZE     0x100u                   								  //Flash Page size 256 byte

/* Pre-Compile parameters */
#define FLS_DEV_ERROR_DETECT            (STD_OFF)

#define FlsEraseVerificationEnabled      (STD_ON)

#define FlsWriteVerificationEnabled  	 (STD_ON)


/*SWS_Fls_00272  SWS_Fls_00359 SWS_Fls_00360 SWS_Fls_00362  */
#define FlsTimeoutSupervisionEnabled  	(STD_OFF)

#define FlsCancelApi     		(STD_OFF)
#define FlsGetStatusApi     	(STD_ON)
#define FlsGetJobResultApi      (STD_ON)
#define FlsCompareApi           (STD_ON)
#define FlsSetModeApi           (STD_OFF)
#define FlsBlankCheckApi        (STD_ON) //ETAS BIP Porting

/*SPI related*/               
#define QSPI_ID     2
/*channel related*/
#define  QSPI_CHANNEL_WRITEENABLE   		SpiConf_SpiChannel_QSPI_WriteEnable
#define  QSPI_CHANNEL_READID  		 		SpiConf_SpiChannel_QSPI_ReadID
//#define  QSPI_CHANNEL_WRITECFG   			SpiConf_SpiChannel_QSPI_WriteCfg
//#define  QSPI_CHANNEL_READCFG   			SpiConf_SpiChannel_QSPI_ReadCfg
#define  QSPI_CHANNEL_READSTATUS   			SpiConf_SpiChannel_QSPI_ReadStatus
#define  QSPI_CHANNEL_ERASEPAGE   			SpiConf_SpiChannel_QSPI_ErasePage
#define  QSPI_CHANNEL_READBUFFER   			SpiConf_SpiChannel_QSPI_ReadBuffer
#define  QSPI_CHANNEL_READIDBUF   			SpiConf_SpiChannel_QSPI_ReadIDBuf
#define  QSPI_CHANNEL_WRITEPAGE   			SpiConf_SpiChannel_QSPI_WritePage

/*job related*/
#define  QSPI_ERASEJOB    		SpiConf_SpiJob_SpiJob_erase
#define  QSPI_WRITEJOB    		SpiConf_SpiJob_SpiJob_write
#define  QSPI_READJOB     		SpiConf_SpiJob_SpiJob_read
 

/*sequence related*/
#define  FLS_READIDBUF    		SpiConf_SpiSequence_ReadIDBuf
//#define  FLS_WRITECFG     		SpiConf_SpiSequence_WriteCfg
#define  FLS_WRITEDATA    		SpiConf_SpiSequence_WriteData
#define  FLS_READDATA	  		SpiConf_SpiSequence_ReadData
//#define  FLS_READCFG      	    SpiConf_SpiSequence_ReadCfg
#define  FLS_WRITEENABLECFG     SpiConf_SpiSequence_WriteEnableCfg
#define  FLS_WRITEENABLEDATA	SpiConf_SpiSequence_WriteEnableData
#define  FLS_READSTATUSCFG      SpiConf_SpiSequence_ReadStatusReg
#define  FLS_ERASEPAGE 		    SpiConf_SpiSequence_ErasePage

#endif
