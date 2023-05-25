/************************************************************************************ 
*  Copyright (c) 2020, Calterah, Inc. All rights reserved.
*
*  SPI driver is the property of Calterah, Inc.
*  QSPI driver is property of Calterah Semiconductor, Inc.
*
*  Redistribution and use in source and binary forms, with or without modification,
*  are permitted provided that the following conditions are met:
*
*  1) Redistributions of source code must retain the above copyright notice, this 
*     list of conditions and the following disclaimer.
*
*  2) Redistributions in binary form must reproduce the above copyright notice, this
*     list of conditions and the following disclaimer in the documentation and/or 
*     other materials provided with the distribution.
*
*  3) Neither the name of the Calterah, Inc., nor the names of its contributors may
*     be used to endorse or promote products derived from this software without 
*     specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
*  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
*  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
*  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
*  OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************************
**                                                                                 **
**  FILENAME   : Spi_Cfg.h                                                         **
**                                                                                 **
**                                                                                 **
**  DATE, TIME : 2022-12-23, 16:22:51                                          **
**                                                                                 **
**  GENERATOR  : Build b210315-0853                                              **
**                                                                                 **
**  AUTHOR     : Calterah Semiconductor                                            **
**                                                                                 **
**  VENDOR     : Calterah Semiconductor                                            **
**                                                                                 **
**  DESCRIPTION : SPI configuration generated out of ECU configuration             **
**                file (Spi.bmd)                                                   **
**                                                                                 **
**  MAY BE CHANGED BY USER [yes/no]: no                                            **
**                                                                                 **
*************************************************************************************/

#ifndef SPI_CFG_H
#define SPI_CFG_H


#define SPI_VENDOR_ID       0x8bU     /* SPI Vendor Id */
#define SPI_MODULE_ID       83U                   /* SPI Module Id */
#define SPI_INSTANCE_ID     0U                 /* SPI Instance Id */


#define SPI_AR_RELEASE_MAJOR_VERSION    4U  /* SPI Module Autosar Major Version Number for ALPS */
#define SPI_AR_RELEASE_MINOR_VERSION    3U  /* SPI Module Autosar Minor Version Number for ALPS */
#define SPI_AR_RELEASE_PATCH_VERSION    1U  /* SPI Module Autosar Patch Version Number for ALPS */

#define SPI_SW_MAJOR_VERSION    3U          /* Software Major Version Number for ALPS */
#define SPI_SW_MINOR_VERSION    4U          /* SPI Module Software Minor Version Number for ALPS */
#define SPI_SW_PATCH_VERSION    0U          /* SPI Module Software Patch Version Number for ALPS */

/* Pre-Compile parameters */
#define SPI_DEV_ERROR_DETECT                    (STD_OFF)
#define SPI_CANCEL_API                          (STD_ON)
#define SPI_VERSION_INFO_API                    (STD_ON)
#define SPI_HW_STATUS_API                       (STD_OFF)
#define SPI_INTERRUPTIBLE_SEQ_ALLOWED           (STD_OFF)
#define SPI_SUPPORT_CONCURRENT_SYNC_TRANSMIT    (STD_OFF)

#define SPI_LEVEL_DELIVERED             2U
#define SPI_CHANNEL_BUFFERS_ALLOWED     2U
    
#define SPI_MAX_JOB_Q_LENGTH           5U
#define SPI_MAX_JOB                    11U 
#define SPI_MAX_CHANNEL                10U 
#define SPI_MAX_SEQUENCE               10U
#define SPI_MAX_HW_UNIT                4U

/* Buffer config data */
#define SPI_MAXIMUM_IB_CHANNELS        6U
#define SPI_MAXIMUM_IB_BUFFER_SIZE     13U
#define SPI_WRITELOCK_INDEX            2U 

#define SPI_MAXIMUM_EB_CHANNELS        4U
#define SPI_MAX_BUFFER                 1131UL

#define SPI_CHANNEL_END_INDICATOR      (Spi_ChannelType)0xFFU
#define SPI_JOB_END_INDICATOR          (Spi_JobType)0xFFFFU

/* Channel ID */
#define SpiConf_SpiChannel_QSPI_WriteEnable   ((Spi_ChannelType)0U)
#define SpiConf_SpiChannel_QSPI_ReadID   ((Spi_ChannelType)1U)
#define SpiConf_SpiChannel_QSPI_WriteCfg   ((Spi_ChannelType)2U)
#define SpiConf_SpiChannel_QSPI_ReadCfg   ((Spi_ChannelType)3U)
#define SpiConf_SpiChannel_QSPI_ReadStatus   ((Spi_ChannelType)4U)
#define SpiConf_SpiChannel_QSPI_ErasePage   ((Spi_ChannelType)5U)
#define SpiConf_SpiChannel_QSPI_ReadBuffer   ((Spi_ChannelType)6U)
#define SpiConf_SpiChannel_QSPI_ReadIDBuf   ((Spi_ChannelType)7U)
#define SpiConf_SpiChannel_QSPI_WritePage   ((Spi_ChannelType)8U)
#define SpiConf_SpiChannel_SPI_ReadAds   ((Spi_ChannelType)9U)

/* Job ID */
#define SpiConf_SpiJob_SpiJob_0   ((Spi_JobType)0U)
#define SpiConf_SpiJob_SpiJob_1   ((Spi_JobType)1U)
#define SpiConf_SpiJob_SpiJob_2   ((Spi_JobType)2U)
#define SpiConf_SpiJob_SpiJob_3   ((Spi_JobType)3U)
#define SpiConf_SpiJob_SpiJob_4   ((Spi_JobType)4U)
#define SpiConf_SpiJob_SpiJob_write   ((Spi_JobType)5U)
#define SpiConf_SpiJob_SpiJob_read   ((Spi_JobType)6U)
#define SpiConf_SpiJob_SpiJob_7   ((Spi_JobType)7U)
#define SpiConf_SpiJob_SpiJob_8   ((Spi_JobType)8U)
#define SpiConf_SpiJob_SpiJob_erase   ((Spi_JobType)9U)
#define SpiConf_SpiJob_SpiJob_Read_Ads   ((Spi_JobType)10U)

/* Sequence ID */
#define SpiConf_SpiSequence_ReadIDBuf   ((Spi_SequenceType)0U)
#define SpiConf_SpiSequence_WriteCfg   ((Spi_SequenceType)1U)
#define SpiConf_SpiSequence_WriteData   ((Spi_SequenceType)2U)
#define SpiConf_SpiSequence_ReadData   ((Spi_SequenceType)3U)
#define SpiConf_SpiSequence_ReadCfg   ((Spi_SequenceType)4U)
#define SpiConf_SpiSequence_WriteEnableCfg   ((Spi_SequenceType)5U)
#define SpiConf_SpiSequence_WriteEnableData   ((Spi_SequenceType)6U)
#define SpiConf_SpiSequence_ReadStatusReg   ((Spi_SequenceType)7U)
#define SpiConf_SpiSequence_ErasePage   ((Spi_SequenceType)8U)
#define SpiConf_SpiSequence_ReadAds   ((Spi_SequenceType)9U)

#endif /*SPI_CFG_H*/
