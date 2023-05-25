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
**  FILENAME   : Spi_PBCfg.c                                                       **
**                                                                                 **
**                                                                                 **
**  DATE, TIME : 2022-12-08, 18:00:15                                          **
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

/* includes */
#include "Spi.h"
  
#define SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Spi_MemMap.h"

/* External Devices */
Spi_ExternalDeviceType SpiExternalConfigData[] =
{
    {
        .SpiBaudrate             = 1000,
        .SpiCsIdentifier         = 0,
        .SpiHwUnit               = CSIB0,   
        .SpiDataShiftEdge        = SPI_EDGE_TRAILING, 
        .SpiShiftClockIdleLevel  = STD_LOW,
        .SpiTimeCs2Clk           = 3,  /* number of Clk cycles */
        .SpiDeviceType           = SPI_DEVICE, 
    }, 
    {
        .SpiBaudrate             = 4000000,
        .SpiCsIdentifier         = 1,
        .SpiHwUnit               = CSIB1,   
        .SpiDataShiftEdge        = SPI_EDGE_TRAILING, 
        .SpiShiftClockIdleLevel  = STD_LOW,
        .SpiTimeCs2Clk           = 4,  /* number of Clk cycles */
        .SpiDeviceType           = SPI_DEVICE, 
    }, 
    {
        .SpiBaudrate             = 250000,
        .SpiCsIdentifier         = 0,
        .SpiHwUnit               = CSIB2,   
        .SpiDataShiftEdge        = SPI_EDGE_LEADING, 
        .SpiShiftClockIdleLevel  = STD_LOW,
        .SpiTimeCs2Clk           = 0,  /* number of Clk cycles */
        .SpiDeviceType           = QSPI_DEVICE, 
    }, 
    {
        .SpiBaudrate             = 5000000,
        .SpiCsIdentifier         = 0,
        .SpiHwUnit               = CSIB0,   
        .SpiDataShiftEdge        = SPI_EDGE_TRAILING, 
        .SpiShiftClockIdleLevel  = STD_LOW,
        .SpiTimeCs2Clk           = 0,  /* number of Clk cycles */
        .SpiDeviceType           = SPI_DEVICE, 
    }
};

/* Channel Configuration */
const Spi_ChannelConfigType SpiChannelConfigData[] =
{
    {
        .SpiChannelId      = SpiConf_SpiChannel_QSPI_WriteEnable,
        .SpiChannelType    = SPI_IB,
        .SpiIbNBuffers     = 2,
        .SpiEbMaxLength    = 0,
        .SpiDefaultData    = 0,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },     
    {
        .SpiChannelId      = SpiConf_SpiChannel_QSPI_ReadID,
        .SpiChannelType    = SPI_IB,
        .SpiIbNBuffers     = 2,
        .SpiEbMaxLength    = 0,
        .SpiDefaultData    = 0,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },     
    {
        .SpiChannelId      = SpiConf_SpiChannel_QSPI_WriteCfg,
        .SpiChannelType    = SPI_IB,
        .SpiIbNBuffers     = 2,
        .SpiEbMaxLength    = 0,
        .SpiDefaultData    = 0,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },     
    {
        .SpiChannelId      = SpiConf_SpiChannel_QSPI_ReadCfg,
        .SpiChannelType    = SPI_IB,
        .SpiIbNBuffers     = 2,
        .SpiEbMaxLength    = 0,
        .SpiDefaultData    = 0,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },     
    {
        .SpiChannelId      = SpiConf_SpiChannel_QSPI_ReadStatus,
        .SpiChannelType    = SPI_IB,
        .SpiIbNBuffers     = 1,
        .SpiEbMaxLength    = 0,
        .SpiDefaultData    = 0,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },     
    {
        .SpiChannelId      = SpiConf_SpiChannel_QSPI_ErasePage,
        .SpiChannelType    = SPI_IB,
        .SpiIbNBuffers     = 4,
        .SpiEbMaxLength    = 0,
        .SpiDefaultData    = 0,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },     
    {
        .SpiChannelId      = SpiConf_SpiChannel_QSPI_ReadBuffer,
        .SpiChannelType    = SPI_EB,
        .SpiIbNBuffers     = 0,
        .SpiEbMaxLength    = 2048,
        .SpiDefaultData    = 0,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },         {
        .SpiChannelId      = SpiConf_SpiChannel_QSPI_ReadIDBuf,
        .SpiChannelType    = SPI_EB,
        .SpiIbNBuffers     = 0,
        .SpiEbMaxLength    = 30,
        .SpiDefaultData    = 0,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },         {
        .SpiChannelId      = SpiConf_SpiChannel_QSPI_WritePage,
        .SpiChannelType    = SPI_EB,
        .SpiIbNBuffers     = 0,
        .SpiEbMaxLength    = 512,
        .SpiDefaultData    = 0,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },         {
        .SpiChannelId      = SpiConf_SpiChannel_SPI_ReadAds,
        .SpiChannelType    = SPI_EB,
        .SpiIbNBuffers     = 0,
        .SpiEbMaxLength    = 64,
        .SpiDefaultData    = 10,
        .SpiTransferStart  = SPI_TRANSFER_START_MSB
    },          {
		.SpiChannelId      = SpiConf_SpiChannel_SPI_Tcan,
		.SpiChannelType    = SPI_EB,
		.SpiIbNBuffers     = 0,
		.SpiEbMaxLength    = 64,
		.SpiDefaultData    = 10,
		.SpiTransferStart  = SPI_TRANSFER_START_MSB
    }
};

/* Channel link */
Spi_ChannelType SpiJob_0_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_ReadID,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_1_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_ReadIDBuf,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_2_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_WriteEnable,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_3_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_WriteCfg,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_4_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_WriteEnable,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_write_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_WritePage,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_read_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_ReadBuffer,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_7_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_ReadCfg,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_8_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_ReadStatus,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_erase_Channel_link[] =
{
    SpiConf_SpiChannel_QSPI_ErasePage,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_Read_Ads_Channel_link[] =
{
    SpiConf_SpiChannel_SPI_ReadAds,
    SPI_CHANNEL_END_INDICATOR
};

Spi_ChannelType SpiJob_Tcan_Channel_link[] = {
    SpiConf_SpiChannel_SPI_Tcan,
    SPI_CHANNEL_END_INDICATOR
};

/* Job Configuration */
Spi_JobConfigType SpiJobConfigData[] =
{
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_0,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = STD_SPI, 
        .SpiWaitCycles          = 0,
        .SpiTransferType        = STD_ONLY,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 144,
        .SpiCommandType         = READ_CMD,
        .SpiAddressLength       = ADDR_L_24_BIT,
        .SpiAddress             = 0,
        .SpiTransferMode        = WRITE_READ,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_0_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_1,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = STD_SPI, 
        .SpiWaitCycles          = 0,
        .SpiTransferType        = STD_ONLY,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 159,
        .SpiCommandType         = READ_CMD,
        .SpiAddressLength       = ADDR_L_0_BIT,
        .SpiAddress             = 0,
        .SpiTransferMode        = WRITE_READ,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_1_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_2,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = STD_SPI, 
        .SpiWaitCycles          = 0,
        .SpiTransferType        = STD_ONLY,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 6,
        .SpiCommandType         = CMD_ONLY,
        .SpiAddressLength       = ADDR_L_0_BIT,
        .SpiAddress             = 0,
        .SpiTransferMode        = WRITE_READ,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_2_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_3,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = STD_SPI, 
        .SpiWaitCycles          = 8,
        .SpiTransferType        = STD_ONLY,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 1,
        .SpiCommandType         = WRITE_CMD,
        .SpiAddressLength       = ADDR_L_0_BIT,
        .SpiAddress             = 0,
        .SpiTransferMode        = WRITE_READ,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_3_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_4,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = QUAD_SPI, 
        .SpiWaitCycles          = 0,
        .SpiTransferType        = STD_ONLY,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 6,
        .SpiCommandType         = CMD_ONLY,
        .SpiAddressLength       = ADDR_L_0_BIT,
        .SpiAddress             = 0,
        .SpiTransferMode        = WRITE_ONLY,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_4_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_write,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = QUAD_SPI, 
        .SpiWaitCycles          = 0,
        .SpiTransferType        = STD_ONLY,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 50,
        .SpiCommandType         = WRITE_CMD,
        .SpiAddressLength       = ADDR_L_24_BIT,
        .SpiAddress             = 65536,
        .SpiTransferMode        = WRITE_ONLY,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_write_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_read,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = QUAD_SPI, 
        .SpiWaitCycles          = 6,
        .SpiTransferType        = STD_QUAD,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 235,
        .SpiCommandType         = READ_CMD,
        .SpiAddressLength       = ADDR_L_24_BIT,
        .SpiAddress             = 65536,
        .SpiTransferMode        = READ_ONLY,
        .SpiReadCount           = 64,
        .SpiChannelAssignment   = SpiJob_read_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_7,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = STD_SPI, 
        .SpiWaitCycles          = 0,
        .SpiTransferType        = STD_ONLY,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 53,
        .SpiCommandType         = READ_CMD,
        .SpiAddressLength       = ADDR_L_0_BIT,
        .SpiAddress             = 0,
        .SpiTransferMode        = WRITE_READ,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_7_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_8,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = STD_SPI, 
        .SpiWaitCycles          = 0,
        .SpiTransferType        = STD_ONLY,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 5,
        .SpiCommandType         = READ_CMD,
        .SpiAddressLength       = ADDR_L_0_BIT,
        .SpiAddress             = 0,
        .SpiTransferMode        = WRITE_READ,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_8_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_erase,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 8,
        .SpiFrameFormat         = QUAD_SPI, 
        .SpiWaitCycles          = 0,
        .SpiTransferType        = STD_ONLY,
        .SpiInstructionLength   = INST_L_8_BIT,
        .SpiInstruction         = 32,
        .SpiCommandType         = CMD_ONLY,
        .SpiAddressLength       = ADDR_L_24_BIT,
        .SpiAddress             = 65536,
        .SpiTransferMode        = WRITE_ONLY,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_erase_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[2]
    }, 
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_Read_Ads,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 16,
        .SpiFrameFormat         = STD_SPI, 
        .SpiWaitCycles          = 0,
        .SpiTransferType        = QUAD_ONLY,
        .SpiInstructionLength   = INST_L_0_BIT,
        .SpiInstruction         = 0,
        .SpiCommandType         = CMD_ONLY,
        .SpiAddressLength       = ADDR_L_0_BIT,
        .SpiAddress             = 0,
        .SpiTransferMode        = WRITE_READ,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_Read_Ads_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[1]
    },
    {
        .SpiJobId               = SpiConf_SpiJob_SpiJob_Tcan,
        .SpiJobPriority         = 0,
        .SpiJobEndNotification  = NULL_PTR,
        .SpiDataWidth           = 16,
        .SpiFrameFormat         = STD_SPI,
        .SpiWaitCycles          = 0,
        .SpiTransferType        = QUAD_ONLY,
        .SpiInstructionLength   = INST_L_0_BIT,
        .SpiInstruction         = 0,
        .SpiCommandType         = CMD_ONLY,
        .SpiAddressLength       = ADDR_L_0_BIT,
        .SpiAddress             = 0,
        .SpiTransferMode        = WRITE_READ,
        .SpiReadCount           = 0,
        .SpiChannelAssignment   = SpiJob_Tcan_Channel_link,
        .SpiDeviceAssignment    = &SpiExternalConfigData[3]
    }
};

/* Job link */
Spi_JobType SpiSequence_ReadIDBuf_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_0,

    SpiConf_SpiJob_SpiJob_1,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_WriteCfg_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_3,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_WriteData_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_write,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_ReadData_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_read,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_ReadCfg_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_7,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_WriteEnableCfg_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_2,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_WriteEnableData_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_4,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_ReadStatusReg_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_8,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_ErasePage_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_erase,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_ReadAds_Job_link[] =
{

    SpiConf_SpiJob_SpiJob_Read_Ads,
    SPI_JOB_END_INDICATOR
};

Spi_JobType SpiSequence_Tcan_Job_link[] = {
    SpiConf_SpiJob_SpiJob_Tcan,
    SPI_JOB_END_INDICATOR
};

/* Sequence Configuration */
const Spi_SequenceConfigType SpiSequenceConfigData[] =
{
    {
        .SpiSequenceId             = SpiConf_SpiSequence_ReadIDBuf,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 2,
        .SpiJobAssignment          = SpiSequence_ReadIDBuf_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    }, 
    {
        .SpiSequenceId             = SpiConf_SpiSequence_WriteCfg,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 1,
        .SpiJobAssignment          = SpiSequence_WriteCfg_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    }, 
    {
        .SpiSequenceId             = SpiConf_SpiSequence_WriteData,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 1,
        .SpiJobAssignment          = SpiSequence_WriteData_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    }, 
    {
        .SpiSequenceId             = SpiConf_SpiSequence_ReadData,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 1,
        .SpiJobAssignment          = SpiSequence_ReadData_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    }, 
    {
        .SpiSequenceId             = SpiConf_SpiSequence_ReadCfg,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 1,
        .SpiJobAssignment          = SpiSequence_ReadCfg_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    }, 
    {
        .SpiSequenceId             = SpiConf_SpiSequence_WriteEnableCfg,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 1,
        .SpiJobAssignment          = SpiSequence_WriteEnableCfg_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    }, 
    {
        .SpiSequenceId             = SpiConf_SpiSequence_WriteEnableData,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 1,
        .SpiJobAssignment          = SpiSequence_WriteEnableData_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    }, 
    {
        .SpiSequenceId             = SpiConf_SpiSequence_ReadStatusReg,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 1,
        .SpiJobAssignment          = SpiSequence_ReadStatusReg_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    }, 
    {
        .SpiSequenceId             = SpiConf_SpiSequence_ErasePage,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 1,
        .SpiJobAssignment          = SpiSequence_ErasePage_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    }, 
    {
        .SpiSequenceId             = SpiConf_SpiSequence_ReadAds,
        .SpiInterruptibleSequence  = FALSE,
        .SpiSeqTotalJobs           = 1,
        .SpiJobAssignment          = SpiSequence_ReadAds_Job_link,
        .SpiSeqEndNotification     = NULL_PTR,
    },
    {
		.SpiSequenceId             = SpiConf_SpiSequence_Tcan,
		.SpiInterruptibleSequence  = FALSE,
		.SpiSeqTotalJobs           = 1,
		.SpiJobAssignment          = SpiSequence_Tcan_Job_link,
		.SpiSeqEndNotification     = NULL_PTR,
	}
};


const Spi_ConfigType SpiConfigData =
{
    .SpiMaxChannel      = SPI_MAX_CHANNEL,
    .SpiMaxJob          = SPI_MAX_JOB,
    .SpiMaxSequence     = SPI_MAX_SEQUENCE,
    .SpiChannelConfig   = &SpiChannelConfigData[0],
    .SpiSequenceConfig  = &SpiSequenceConfigData[0],
    .SpiJobConfig       = &SpiJobConfigData[0],
    .SpiExternalDevice  = &SpiExternalConfigData[0],
};

#define SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Spi_MemMap.h"
