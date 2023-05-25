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
#ifndef SPI_ALPS
#define SPI_ALPS

typedef enum {
    SPI_ASYNC_CALL,
    SPI_SYNC_CALL,
} Spi_CallType;

/* Structure definitions */
typedef struct {
    Spi_JobResultType   JobResult;
} Spi_JobUnitType;

typedef struct {
    Spi_SeqResultType   SeqResult;
    uint16              TotalJobs;    /* total number of contained jobs */
    uint16              SentJobs;     /* number of sent jobs. */
} Spi_SeqUnitType;

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
typedef struct {
    Spi_JobType       JobStartIndex;
    Spi_JobType       JobEndIndex;
    Spi_JobType       JobQueue[SPI_MAX_JOB_Q_LENGTH];
    Spi_SequenceType  JobLinkedSeq[SPI_MAX_JOB_Q_LENGTH];  /* This variable holds the sequence id of the job that is currently been used for transmission. */  
}Spi_QueueType;
#endif

typedef struct {
    Spi_StatusType  Status; 
}Spi_HwInfoType;

typedef struct {
    uint32 SpiTxSWBuff[SPI_MAX_BUFFER];
    uint32 SpiRxSWBuff[SPI_MAX_BUFFER];
    uint32 SpiBuffIdx;
}Spi_SWBuffType;
    
#if(SPI_CHANNEL_BUFFERS_ALLOWED != 0)
typedef struct {
    const Spi_DataBufferType    *Src;    
    Spi_DataBufferType          *Dest;   
    Spi_NumberOfDataType        Length; 
    boolean                     Active; /* Set if the buffer is being used. */
} Spi_EbType;
#endif

#if(SPI_CHANNEL_BUFFERS_ALLOWED != 1)
typedef struct {
    uint8   *IBBufferPtr;
    uint16  IBBufferOffset[SPI_MAXIMUM_IB_CHANNELS];
    uint8   ChannelLock[SPI_WRITELOCK_INDEX];
    boolean NullParamStat[SPI_MAXIMUM_IB_CHANNELS];
}Spi_IBBufferAccessType;
#endif

typedef struct {
    const Spi_ConfigType    *ConfigPtr;
    #if(SPI_CHANNEL_BUFFERS_ALLOWED != 0)
    Spi_EbType              ExtBuf[SPI_MAXIMUM_EB_CHANNELS];
    #endif
    Spi_AsyncModeType       AsyncMode;
    #if(SPI_CHANNEL_BUFFERS_ALLOWED != 1)
    Spi_IBBufferAccessType  IntBuf;
    #endif
    Spi_HwInfoType          HwInfo[ALPS_SPI_HW_CNT];
    Spi_SeqUnitType         SeqUnit[SPI_MAX_SEQUENCE];
    Spi_JobUnitType         JobUnit[SPI_MAX_JOB];
    uint16                  NoOfIBChannels;
    Spi_StatusType          DriverStatus;
    Spi_CallType            CallType;
    #if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
    Spi_QueueType           Queue;
    #endif
    Spi_SWBuffType          SWBuffer;
    uint32                  SpiClk;
} Spi_GlobalType;

#define SPI_START_SEC_CODE


/* Public Function Declarations */
/******************************************************************************
*  Service name    : Spi_Hal_Init                                              
*  Syntax          : void Spi_Hal_Init(const Spi_ConfigType* ConfigPtr)        
*  Parameters (in) : ConfigPtr - Pointer to configuration set                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Service for SPI initialization                            
******************************************************************************/
void Spi_Hal_Init(const Spi_ConfigType *ConfigPtr);

/******************************************************************************
*  Service name    : Spi_Hal_DeInit                                            
*  Syntax          : Std_ReturnType Spi_Hal_DeInit(void)                       
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: de-initialisation command has been accepted         
*                    E_NOT_OK: de-initialisation command has not been accepted 
*  Description     : Service for SPI de-initialization.                        
******************************************************************************/
Std_ReturnType Spi_Hal_DeInit(void);

#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 0U)||(SPI_CHANNEL_BUFFERS_ALLOWED == 2U)) 
/******************************************************************************
*  Service name    : Spi_Hal_WriteIB                                           
*  Syntax          : Std_ReturnType Spi_Hal_WriteIB(                           
*                           Spi_ChannelType Channel,                           
*                           const Spi_DataBufferType *DataBufferPtr )          
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
Std_ReturnType Spi_Hal_WriteIB(Spi_ChannelType Channel, 
                       const Spi_DataBufferType *DataBufferPtr);

/******************************************************************************
*  Service name    : Spi_Hal_ReadIB                                            
*  Syntax          : Std_ReturnType Spi_Hal_ReadIB(                            
*                           Spi_ChannelType Channel,                           
*                           Spi_DataBufferType *DataBufferPtr)                
*  Parameters (in) : Channel - Channel ID.                                     
*  Parameters (out): DataBufferPtr - Pointer to destination data buffer in RAM 
*  Return value    : E_OK: read command has been accepted                      
*                    E_NOT_OK: read command has not been accepted              
*  Description     : Service for reading one or more data to an IB             
*                    SPI Handler/Driver Channel specified by parameter.        
******************************************************************************/
Std_ReturnType Spi_Hal_ReadIB(Spi_ChannelType Channel,
                       Spi_DataBufferType *DataBufferPtr);
#endif

#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 1U)||(SPI_CHANNEL_BUFFERS_ALLOWED == 2U))
/******************************************************************************
*  Service name        : Spi_Hal_SetupEB                                       
*  Syntax              : Std_ReturnType Spi_Hal_SetupEB(                       
*                               Spi_ChannelType Channel,                       
*                               const Spi_DataBufferType *SrcDataBufferPtr,    
*                               Spi_DataBufferType *DesDataBufferPtr,          
*                               Spi_NumberOfDataType Length)                  
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
Std_ReturnType Spi_Hal_SetupEB(Spi_ChannelType Channel,
                               const Spi_DataBufferType *SrcDataBufferPtr,
                               Spi_DataBufferType *DesDataBufferPtr, 
                               Spi_NumberOfDataType Length);
#endif

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_0) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
/********************************************************************************
*  Service name    : Spi_Hal_SyncTransmit                                        
*  Syntax          : Std_ReturnType Spi_Hal_SyncTransmit(
*                                           Spi_SequenceType Sequence)
*  Parameters (in) : Sequence - Sequence ID.                                     
*  Parameters (out): None                                                        
*  Return value    : E_OK: Transmission command has been accepted.               
*                    E_NOT_OK: Transmission command has not been accepted.       
*  Description     : Service to transmit data on the SPI bus.                    
********************************************************************************/
Std_ReturnType Spi_Hal_SyncTransmit(Spi_SequenceType Sequence);
#endif

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
/********************************************************************************
*  Service name    : Spi_Hal_AsyncTransmit                                       
*  Syntax          : Std_ReturnType Spi_Hal_AsyncTransmit(
*                                           Spi_SequenceType Sequence)
*  Parameters (in) : Sequence - Sequence ID.                                     
*  Parameters (out): None                                                        
*  Return value    : E_OK: Transmission command has been accepted.               
*                    E_NOT_OK: Transmission command has not been accepted.       
*  Description     : Service to transmit data on the SPI bus.                    
********************************************************************************/
Std_ReturnType Spi_Hal_AsyncTransmit(Spi_SequenceType Sequence);
#endif

/*******************************************************************************
*  Service name    : Spi_Hal_GetStatus                                          
*  Syntax          : Spi_StatusType Spi_Hal_GetStatus(void)                     
*  Parameters (in) : None                                                       
*  Parameters (out): None                                                       
*  Return value    : Spi_StatusType                                             
*  Description     : Service returns the SPI Handler/Driver software module     
*                    status.                                                    
*******************************************************************************/
Spi_StatusType Spi_Hal_GetStatus(void);

/*******************************************************************************
*  Service name    : Spi_Hal_GetJobResult                                       
*  Syntax          : Spi_JobResultType Spi_Hal_GetJobResult(Spi_JobType Job)    
*  Parameters (in) : Job - Job ID. An invalid job ID will return an undefined   
*                          result.                                              
*  Parameters (out): None                                                       
*  Return value    : Spi_JobResultType                                          
*  Description     : This service returns the last transmission result of the   
*                    specified Job.                                             
*******************************************************************************/
Spi_JobResultType Spi_Hal_GetJobResult(Spi_JobType Job);

/*******************************************************************************
*  Service name    : Spi_Hal_GetSeqResult                                      
*  Syntax          : Spi_SeqResultType Spi_Hal_GetSeqResult(                   
*                           Spi_SequenceType Sequence)                         
*  Parameters (in) : Sequence - Sequence ID. An invalid sequence ID will return 
*                               an undefined result                             
*  Parameters (out): None                                                       
*  Return value    : Spi_SeqResultType                                          
*  Description     : This service returns the last transmission result of the   
*                    specified Sequence.                                        
*******************************************************************************/
Spi_SeqResultType Spi_Hal_GetSeqResult(Spi_SequenceType Sequence);

#if (SPI_HW_STATUS_API == STD_ON)
/*******************************************************************************
*  Service name    : Spi_Hal_GetHWUnitStatus                                    
*  Syntax          : Spi_StatusType Spi_Hal_GetHWUnitStatus(Spi_HWUnitType HWUnit)
*  Parameters (in) : HWUnit - SPI Hardware microcontroller peripheral (unit) ID.
*  Parameters (out): None                                                       
*  Return value    : Spi_StatusType                                             
*  Description     : This service returns the status of the specified           
*                    SPI Hardware microcontroller pe-ripheral.                  
*******************************************************************************/
Spi_StatusType Spi_Hal_GetHWUnitStatus(Spi_HWUnitType HWUnit);
#endif

#if (SPI_CANCEL_API == STD_ON )
/*******************************************************************************
*  Service name    : Spi_Hal_Cancel                                             
*  Syntax          : void Spi_Hal_Cancel(Spi_SequenceType Sequence)             
*  Parameters (in) : Sequence - Sequence ID.                                    
*  Parameters (out): None                                                       
*  Return value    : None                                                       
*  Description     : Service cancels the specified on-going sequence            
*                    transmission.                                              
*******************************************************************************/
void Spi_Hal_Cancel(Spi_SequenceType Sequence);
#endif

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
/*******************************************************************************
*  Service name    : Spi_Hal_SetAsyncMode                                       
*  Syntax          : Std_ReturnType Spi_Hal_SetAsyncMode(Spi_AsyncModeType Mode)
*  Parameters (in) : Mode - New mode required.                                  
*  Parameters (out): None                                                       
*  Return value    : E_OK: Transmission command has been accepted.              
*                    E_NOT_OK: Transmission command has not been accepted.      
*  Description     : Service to set the asynchronous mechanism mode for SPI     
*                    busses handled asyn-chronously.                            
*******************************************************************************/
Std_ReturnType Spi_Hal_SetAsyncMode(Spi_AsyncModeType Mode);
#endif

/******************************************************************************
*  Service name    : Spi_Hal_MainFunction_Handling                             
*  Syntax          : void Spi_MainFunction_Handling(void)                      
*  Description     : ----                                                      
******************************************************************************/
void Spi_Hal_MainFunction_Handling(void); 

/*******************************************************************************
*  Service name    : Spi_lclParamCheck                                          
*  Syntax          : void Spi_lclParamCheck(boolean Expr,                       
*                                           uint8 ApiId,                        
*                                           uint8 ErrId,                        
*                                           Std_ReturnType *RetVal)             
*  Parameters (in) : Expr - TRUE or FALSE                                       
*                    ApiId - API ID                                             
*                    ErrId - ERROR ID                                           
*  Parameter(inout): RetVal                                                     
*  Return value    : None                                                       
*  Description     : Reports Det error if SPI_DEV_ERROR_DETECT == STD_ON.       
*******************************************************************************/
void Spi_lclParamCheck(boolean Expr, uint8 ApiId,
                       uint8 ErrId, Std_ReturnType *RetVal);

#define SPI_STOP_SEC_CODE


#endif /* SPI_ALPS */

