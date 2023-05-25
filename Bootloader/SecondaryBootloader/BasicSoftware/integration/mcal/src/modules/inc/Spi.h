/************************************************************************************ 
*  Copyright (c) 2020, Synopsys, Inc. All rights reserved.
*
*  SPI driver is the property of Synopsys, Inc.
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
*  3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
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
*************************************************************************************/

/* Header Protection */
#ifndef SPI_H
#define SPI_H

/* Includes */
#include "Spi_Cfg.h"
#include "Std_Types.h"

/* Macro definitions for API Service ID */
#define SPI_INIT_SID                    0x00u   /* Service Id for Spi_Init API */
#define SPI_DEINIT_SID                  0x01u   /* Service Id for Spi_DeInit API */
#define SPI_WRITEIB_SID                 0x02u   /* Service Id for Spi_WriteIB API */
#define SPI_ASYNCTRANSMIT_SID           0x03u   /* Service Id for Spi_Hw_AsyncTransmit API */
#define SPI_READIB_SID                  0x04u   /* Service Id for Spi_ReadIB API */
#define SPI_SETUPEB_SID                 0x05u   /* Service Id for Spi_SetupEB API */
#define SPI_GETJOBRESULT_SID            0x07u   /* Service Id for Spi_GetJobResult API */
#define SPI_GETSEQUENCERESULT_SID       0x08u   /* Service Id for Spi_GetSequenceResult API */
#define SPI_GETVERSIONINFO_SID          0x09u   /* Service Id for Spi_GetVersionInfo API */
#define SPI_SYNCTRANSMIT_SID            0x0Au   /* Service Id for Spi_SyncTransmit API */
#define SPI_GETHWUNITSTATUS_SID         0x0Bu   /* Service Id for Spi_GetHWUnitStatus API */
#define SPI_CANCEL_SID                  0x0Cu   /* Service Id for Spi_Cancel API */
#define SPI_SETASYNCMODE_SID            0x0Du   /* Service Id for Spi_SetAsyncMode API */

/* Macro definitions for Error Codes specified by AUTOSAR */
#define SPI_E_PARAM_CHANNEL             0x0Au   /* API service called with wrong parameter */
#define SPI_E_PARAM_JOB                 0x0Bu   /* API service called with wrong parameter */
#define SPI_E_PARAM_SEQ                 0x0Cu   /* API service called with wrong parameter */
#define SPI_E_PARAM_LENGTH              0x0Du   /* API service called with wrong parameter */
#define SPI_E_PARAM_UNIT                0x0Eu   /* API service called with wrong parameter */
#define SPI_E_PARAM_POINTER             0x10u   /* APIs called with a Null Pointer */
#define SPI_E_UNINIT                    0x1Au   /* API service used without module initialization */
#define SPI_E_ALREADY_INITIALIZED       0x4Au   /* API SPI_Init service called while the SPI driver has already been initialized */
#define SPI_E_SEQ_PENDING               0x2Au   /* Services called in a wrong sequence */
#define SPI_E_SEQ_IN_PROCESS            0x3Au   /* Synchronous transmission service called at wrong time */

#define SPI_LEVEL_0    0u
#define SPI_LEVEL_1    1u
#define SPI_LEVEL_2    2u

/* Symbolic name to identify the HW SPI Hardware microcontroller peripheral */
#define CSIB0   0u      /* Peripheral named CSIB0 is assigned value 0. */
#define CSIB1   1u      /* Peripheral named CSIB1 is assigned value 1. */
#define CSIB2   2u      /* Peripheral named CSIB2 is assigned value 2. */

/* General defines */
#define ALPS_SPI_HW_CNT 3

/* enum definitions */
typedef enum {
    SPI_EB = 0,        /* External Buffer */
    SPI_IB             /* Internal Buffer */
} Spi_BufferType;

typedef enum {
    SPI_TRANSFER_START_LSB,
    SPI_TRANSFER_START_MSB
} Spi_TransferStartType;

typedef enum {
    SPI_EDGE_LEADING,
    SPI_EDGE_TRAILING
} Spi_EdgeType;

typedef enum {
    SPI_DEVICE, 
    QSPI_DEVICE
}Spi_DeviceType;

typedef enum {
    STD_SPI, 
    QUAD_SPI
}Spi_FrameFormatType;

typedef enum {
    INST_L_0_BIT  = 0x0, 
    INST_L_4_BIT  = 0x1, 
    INST_L_8_BIT  = 0x2, 
    INST_L_16_BIT = 0x3 
}Spi_InstLengthType;

typedef enum {
	CMD_ONLY,
    READ_CMD,
    WRITE_CMD
} Spi_CommandType;

typedef enum {
    ADDR_L_0_BIT  = 0x0, 
    ADDR_L_4_BIT  = 0x1, 
    ADDR_L_8_BIT  = 0x2, 
    ADDR_L_12_BIT = 0x3,
    ADDR_L_16_BIT = 0x4, 
    ADDR_L_20_BIT = 0x5, 
    ADDR_L_24_BIT = 0x6,
    ADDR_L_28_BIT = 0x7, 
    ADDR_L_32_BIT = 0x8, 
    ADDR_L_36_BIT = 0x9, 
    ADDR_L_40_BIT = 0xa,
    ADDR_L_44_BIT = 0xb, 
    ADDR_L_48_BIT = 0xc, 
    ADDR_L_52_BIT = 0xd, 
    ADDR_L_56_BIT = 0xe, 
    ADDR_L_60_BIT = 0xf
}Spi_AddrLengthType;

typedef enum {
    WRITE_READ = 0x0,
    WRITE_ONLY = 0x1,
    READ_ONLY  = 0x2
}Spi_TransferMode;

typedef enum {
    STD_ONLY  = 0x0,
    STD_QUAD  = 0x1,
    QUAD_ONLY = 0x2
}Spi_TransferType;

/* SWS_Spi_00373 */
typedef enum {
    SPI_UNINIT = 0,       /* Status of tThe SPI Handler/Driver which is not initialized or not usable is assigned value 0. */
    SPI_IDLE,             /* Status of tThe SPI Handler/Driver which is not currently transmitting any Job is assigned value 1. */
    SPI_BUSY              /* Status of tThe SPI Handler/Driver which is is performing a SPI Job( transmit ) is assigned value 2. */
} Spi_StatusType;

/* SWS_Spi_00374 */
typedef enum {
    SPI_JOB_OK = 0,     /* Status of a successfully finished job is assigned value 0 */
    SPI_JOB_PENDING,    /* Status of a pending job is assigned value 1 */
    SPI_JOB_FAILED,     /* Status of a failed job is assigned value 2 */
    SPI_JOB_QUEUED      /* Status of a queued job is assigned value 3 */
} Spi_JobResultType;

/* SWS_Spi_00375 */
typedef enum {
    SPI_SEQ_OK = 0,     /* Result of a successfully finished sequence is assigned value 0 */
    SPI_SEQ_PENDING,    /* Result of a pending sequence is assigned value 1 */
    SPI_SEQ_FAILED,     /* Result of an unsuccessfully finished sequence is assigned value 2 */
    SPI_SEQ_CANCELLED   /* Result of a canceled sequence is assigned value 3 */
} Spi_SeqResultType;

/* SWS_Spi_00382 */
typedef enum {
    SPI_POLLING_MODE = 0,
    SPI_INTERRUPT_MODE
} Spi_AsyncModeType;

/* SWS_Spi_00376 */
typedef uint8 Spi_DataBufferType;

/* SWS_Spi_00377 */
typedef uint16 Spi_NumberOfDataType;

/* SWS_Spi_00378 */
typedef uint8 Spi_ChannelType;

/* SWS_Spi_00379 */
typedef uint16 Spi_JobType;

/* SWS_Spi_00380 */
typedef uint8 Spi_SequenceType;

/* SWS_Spi_00381 */
typedef uint8 Spi_HWUnitType;

/* Structure definitions */
typedef struct Spi_ExternalDevice {
    uint32         SpiBaudrate;
    uint32         SpiCsIdentifier;
    Spi_EdgeType   SpiDataShiftEdge;
    uint32         SpiHwUnit;
    uint8          SpiShiftClockIdleLevel;
    uint8          SpiTimeCs2Clk;             /* RX_sample_delay its value cannot exceed 16U; */
    Spi_DeviceType SpiDeviceType;  
} Spi_ExternalDeviceType;

typedef struct Spi_ChannelConfig {
    Spi_ChannelType       SpiChannelId;
    Spi_BufferType        SpiChannelType;
    uint32                SpiDefaultData;
    Spi_NumberOfDataType  SpiEbMaxLength;
    Spi_NumberOfDataType  SpiIbNBuffers;
    Spi_TransferStartType SpiTransferStart;
} Spi_ChannelConfigType;

typedef void (*SpiJobEndNotification_Type)(void);

typedef struct Spi_JobConfig {
    Spi_JobType                  SpiJobId;
    SpiJobEndNotification_Type   SpiJobEndNotification;
    uint8                        SpiDataWidth;
    Spi_FrameFormatType          SpiFrameFormat;
    uint8                        SpiWaitCycles;
    Spi_InstLengthType           SpiInstructionLength;
    uint16                       SpiInstruction;
    Spi_CommandType              SpiCommandType;
    Spi_AddrLengthType           SpiAddressLength;
    uint32                       SpiAddress;
    Spi_TransferType             SpiTransferType;
    Spi_TransferMode             SpiTransferMode;
    uint32                       SpiReadCount;
    uint8                        SpiJobPriority;
    const Spi_ChannelType        *SpiChannelAssignment;
    const Spi_ExternalDeviceType *SpiDeviceAssignment;
} Spi_JobConfigType;

typedef void (*SpiSeqEndNotification_Type)(void);

typedef struct Spi_SequenceConfig {
    uint8                      SpiInterruptibleSequence;
    SpiSeqEndNotification_Type SpiSeqEndNotification;
    Spi_SequenceType           SpiSequenceId;
    uint16                     SpiSeqTotalJobs;   /* consists of the total number of linked jobs in the sequence. */
    const Spi_JobType          *SpiJobAssignment;
} Spi_SequenceConfigType;

typedef struct {
    Spi_ChannelType                 SpiMaxChannel;
    Spi_JobType                     SpiMaxJob;
    Spi_SequenceType                SpiMaxSequence;
    const struct Spi_ChannelConfig  *SpiChannelConfig;
    struct Spi_ExternalDevice *SpiExternalDevice;
    struct Spi_JobConfig      *SpiJobConfig;
    const struct Spi_SequenceConfig *SpiSequenceConfig;  
} Spi_ConfigType;

#define SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
extern const Spi_ConfigType SpiConfigData;
#define SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#define SPI_START_SEC_CODE

/* Public Function Declarations */
/******************************************************************************
*  Service name    : Spi_Init                                                  
*  Syntax          : void Spi_Init(const Spi_ConfigType* ConfigPtr)          
*  Service ID      : 0x00                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : ConfigPtr - Pointer to configuration set                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Service for SPI initialization                            
******************************************************************************/
void Spi_Init(const Spi_ConfigType *ConfigPtr);

/******************************************************************************
*  Service name    : Spi_DeInit                                                
*  Syntax          : Std_ReturnType Spi_DeInit(void)                         
*  Service ID      : 0x01                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: de-initialisation command has been accepted         
*                    E_NOT_OK: de-initialisation command has not been accepted 
*  Description     : Service for SPI de-initialization.                        
******************************************************************************/
Std_ReturnType Spi_DeInit(void);

#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 0U)||(SPI_CHANNEL_BUFFERS_ALLOWED == 2U)) 
/******************************************************************************
*  Service name    : Spi_WriteIB                                               
*  Syntax          : Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel,       
*                                      const Spi_DataBufferType *DataBufferPtr)
*  Service ID      : 0x02                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : Channel - Channel ID.                                     
*                    DataBufferPtr - Pointer to source data buffer.If this     
*                        pointer is null, it is assumed that the data to be    
*                        transmitted is not relevant and the default transmit  
*                        value of this channel will be used instead.           
*  Parameters (out): None                                                      
*  Return value    : E_OK: write command has been accepted                     
*                    E_NOT_OK: write command has not been accepted             
*  Description     : Service for writing one or more data to an IB             
*                    SPI Handler/Driver Channel specified by parameter.        
******************************************************************************/
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, 
                           const Spi_DataBufferType *DataBufferPtr);

/******************************************************************************
*  Service name    : Spi_ReadIB                                                
*  Syntax          : Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel,        
*                                       Spi_DataBufferType *DataBufferPtr)     
*  Service ID      : 0x04                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : Channel - Channel ID.                                     
*  Parameters (out): DataBufferPtr - Pointer to destination data buffer in RAM 
*  Return value    : E_OK: read command has been accepted                      
*                    E_NOT_OK: read command has not been accepted              
*  Description     : Service for reading one or more data to an IB             
*                    SPI Handler/Driver Channel specified by parameter.        
******************************************************************************/
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, 
                          Spi_DataBufferType *DataBufferPtr);
#endif

#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 1U)||(SPI_CHANNEL_BUFFERS_ALLOWED == 2U))
/******************************************************************************
*  Service name        : Spi_SetupEB                                           
*  Syntax              : Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel,   
*                               const Spi_DataBufferType *SrcDataBufferPtr,    
*                               Spi_DataBufferType *DesDataBufferPtr,          
*                               Spi_NumberOfDataType Length)                  
*  Service ID          : 0x05                                                  
*  Sync/Async          : Synchronous                                           
*  Reentrancy          : Reentrant                                             
*  Parameters (in)     : Channel - Channel ID.                                 
*                        SrcDataBufferPtr - Pointer to source data buffer.     
*                        Length - Length (number of data elements) of the data 
*                            to be transmitted from SrcDataBufferPtr and/or    
*                            received from DesDataBufferPtr.                   
*                            Min.: 1                                           
*                            Max.: Specified at configuration for this channel.
*  Parameters (in-out) : DesDataBufferPtr - Pointer to destination data buffer 
*                                           in RAM.                            
*  Return value        : E_OK: read command has been accepted                  
*                        E_NOT_OK: read command has not been accepted          
*  Description         : Service to setup the buffers and the length of data   
*                        for the EB SPI Handler/Driver Channel specified.      
******************************************************************************/
Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel,
                           const Spi_DataBufferType *SrcDataBufferPtr,
                           Spi_DataBufferType *DesDataBufferPtr, 
                           Spi_NumberOfDataType Length);
#endif

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_0) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2) 
/********************************************************************************
*  Service name    : Spi_SyncTransmit                                            
*  Syntax          : Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence)
*  Service ID      : 0x0a                                                        
*  Sync/Async      : Asynchronous                                                
*  Reentrancy      : Reentrant                                                   
*  Parameters (in) : Sequence - Sequence ID.                                     
*  Parameters (out): None                                                        
*  Return value    : E_OK: Transmission command has been accepted.               
*                    E_NOT_OK: Transmission command has not been accepted.       
*  Description     : Service to transmit data on the SPI bus.                    
********************************************************************************/
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);
#endif

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2) 
/********************************************************************************
*  Service name    : Spi_AsyncTransmit                                           
*  Syntax          : Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence)
*  Service ID      : 0x03                                                        
*  Sync/Async      : Asynchronous                                                
*  Reentrancy      : Reentrant                                                   
*  Parameters (in) : Sequence - Sequence ID.                                     
*  Parameters (out): None                                                        
*  Return value    : E_OK: Transmission command has been accepted.               
*                    E_NOT_OK: Transmission command has not been accepted.       
*  Description     : Service to transmit data on the SPI bus.                    
********************************************************************************/
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);
#endif

/*******************************************************************************
*  Service name    : Spi_GetStatus                                              
*  Syntax          : Spi_StatusType Spi_GetStatus(void)                       
*  Service ID      : 0x06                                                       
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : None                                                       
*  Parameters (out): None                                                       
*  Return value    : Spi_StatusType                                             
*  Description     : Service returns the SPI Handler/Driver software module     
*                    status.                                                    
*******************************************************************************/
Spi_StatusType Spi_GetStatus(void);

/*******************************************************************************
*  Service name    : Spi_GetJobResult                                           
*  Syntax          : Spi_JobResultType Spi_GetJobResult(Spi_JobType Job)      
*  Service ID      : 0x07                                                       
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : Job - Job ID. An invalid job ID will return an undefined   
*                          result.                                              
*  Parameters (out): None                                                       
*  Return value    : Spi_JobResultType                                          
*  Description     : This service returns the last transmission result of the   
*                    specified Job.                                             
*******************************************************************************/
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job);

/*******************************************************************************
*  Service name    : Spi_GetSequenceResult                                      
*  Syntax          : Spi_SeqResultType Spi_GetSequenceResult(                   
*                           Spi_SequenceType Sequence)                         
*  Service ID      : 0x08                                                       
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : Sequence - Sequence ID. An invalid sequence ID will return 
*                               an undefined result                             
*  Parameters (out): None                                                       
*  Return value    : Spi_SeqResultType                                          
*  Description     : This service returns the last transmission result of the   
*                    specified Sequence.                                        
*******************************************************************************/
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

#if (SPI_HW_STATUS_API == STD_ON)
/*******************************************************************************
*  Service name    : Spi_GetHWUnitStatus                                        
*  Syntax          : Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit)
*  Service ID      : 0x0b                                                       
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : HWUnit - SPI Hardware microcontroller peripheral (unit) ID.
*  Parameters (out): None                                                       
*  Return value    : Spi_StatusType                                             
*  Description     : This service returns the status of the specified           
*                    SPI Hardware microcontroller pe-ripheral.                  
*******************************************************************************/
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit);
#endif

#if (SPI_CANCEL_API == STD_ON )
/*******************************************************************************
*  Service name    : Spi_Cancel                                                 
*  Syntax          : void Spi_Cancel(Spi_SequenceType Sequence)               
*  Service ID      : 0x0c                                                       
*  Sync/Async      : Asynchronous                                               
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : Sequence - Sequence ID.                                    
*  Parameters (out): None                                                       
*  Return value    : None                                                       
*  Description     : Service cancels the specified on-going sequence            
*                    transmission.                                              
*******************************************************************************/
void Spi_Cancel(Spi_SequenceType Sequence);
#endif

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
/*******************************************************************************
*  Service name    : Spi_SetAsyncMode                                           
*  Syntax          : Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode)  
*  Service ID      : 0x0d                                                       
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Non Reentrant                                              
*  Parameters (in) : Mode - New mode required.                                  
*  Parameters (out): None                                                       
*  Return value    : E_OK: Transmission command has been accepted.              
*                    E_NOT_OK: Transmission command has not been accepted.      
*  Description     : Service to set the asynchronous mechanism mode for SPI     
*                    busses handled asyn-chronously.                            
*******************************************************************************/
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode);
#endif

#if (SPI_VERSION_INFO_API == STD_ON)
/*******************************************************************************
*  Service name    : Spi_GetVersionInfo                                         
*  Syntax          : void Spi_GetVersionInfo(Std_VersionInfoType* VersionInfo)
*  Service ID      : 0x09                                                       
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : None                                                       
*  Parameters (out): VersionInfo -  Pointer to where to store the version       
*                                   information of this module.                 
*  Return value    : None                                                       
*  Description     : This service returns the version information of this module.
*******************************************************************************/
void Spi_GetVersionInfo(Std_VersionInfoType *VersionInfo);
#endif

/******************************************************************************
*  Service name    : Spi_MainFunction_Handling                                 
*  Syntax          : void Spi_MainFunction_Handling(void)                    
*  Service ID      : 0x10                                                      
*  Description     : ----                                                      
******************************************************************************/
void Spi_MainFunction_Handling(void);

#define SPI_STOP_SEC_CODE

#endif /* SPI_H */

