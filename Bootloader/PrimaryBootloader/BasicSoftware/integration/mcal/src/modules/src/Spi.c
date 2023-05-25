/************************************************************************************ 
*  Copyright (c) 2020, Calterah, Inc. All rights reserved.
*
*  SPI driver is the property of Calterah, Inc.
*  QSPI drvier is property of Calterah Semiconductor, Inc.
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
*************************************************************************************/

/******************************************************************************
*                            Includes                                          
******************************************************************************/
#include "Spi.h"
#include "Spi_Alps.h"
#if(SPI_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#include "SchM_Spi.h"

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/
#ifndef SPI_SW_MAJOR_VERSION
#error "SPI_SW_MAJOR_VERSION is not defined."
#endif

#ifndef SPI_SW_MINOR_VERSION
#error "SPI_SW_MINOR_VERSION is not defined."
#endif

#ifndef SPI_SW_PATCH_VERSION
#error "SPI_SW_PATCH_VERSION is not defined."
#endif

#if(SPI_SW_MAJOR_VERSION != 3U)
#error "SPI_SW_MAJOR_VERSION does not match."
#endif

#if(SPI_SW_MINOR_VERSION != 4U)
#error "SPI_SW_MINOR_VERSION does not match."
#endif

#if(SPI_SW_PATCH_VERSION != 0U)
#error "SPI_SW_PATCH_VERSION does not match."
#endif

/* AUTOSAR Specification File Version Check */
#ifndef SPI_AR_RELEASE_MAJOR_VERSION
#error "SPI_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

/* AUTOSAR Major and Minor release version check  */
#if(SPI_AR_RELEASE_MAJOR_VERSION != 4U)
#error "SPI_AR_RELEASE_MAJOR_VERSION does not match."
#endif

/* Inter Module Check for the correct version usage in the used modules */

/* Version Check */
/* Version check for DET module inclusion */
#if(SPI_DEV_ERROR_DETECT == STD_ON)

#ifndef DET_AR_RELEASE_MAJOR_VERSION
#error "DET_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

#if(DET_AR_RELEASE_MAJOR_VERSION != SPI_AR_RELEASE_MAJOR_VERSION)
#error "DET_AR_RELEASE_MAJOR_VERSION does not match."
#endif

#endif /* SPI_DEV_ERROR_DETECT == STD_ON */

/******************************************************************************
*                       Private Variable Definitions                            
******************************************************************************/
#define SPI_START_SEC_VAR_INIT_8

static boolean SpiInitialized = FALSE;
#define SPI_STOP_SEC_VAR_INIT_8


/******************************************************************************
*                    Private Function Declaration                             
******************************************************************************/

/******************************************************************************
*                         Public Functions                                     
******************************************************************************/
#define SPI_START_SEC_CODE


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
void Spi_Init(const Spi_ConfigType *ConfigPtr)
{
    Std_ReturnType RevtVal = E_OK;

    /* SWS_Spi_00235 or SPI_FSR_00235 */
    Spi_lclParamCheck((boolean)(NULL_PTR == ConfigPtr), 
                      SPI_INIT_SID, 
                      SPI_E_PARAM_POINTER, 
                      &RevtVal);

    /* SWS_Spi_00233 or SPI_FSR_00233 */
    Spi_lclParamCheck((boolean)(TRUE == SpiInitialized),
                      SPI_INIT_SID, 
                      SPI_E_ALREADY_INITIALIZED, 
                      &RevtVal);

    if (E_OK == RevtVal)
    {
        Spi_Hal_Init(ConfigPtr);
    }

    SpiInitialized = TRUE;
}

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
Std_ReturnType Spi_DeInit(void)
{
    Std_ReturnType RevtVal = E_OK;
    
    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_DEINIT_SID, 
                      SPI_E_UNINIT, 
                      &RevtVal);

    if (E_OK == RevtVal)
    {
        RevtVal = Spi_Hal_DeInit();
        if (E_OK == RevtVal)
        {
            SpiInitialized = FALSE;
        }
    }

    return RevtVal;
}

#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 0U)||(SPI_CHANNEL_BUFFERS_ALLOWED == 2U))
/******************************************************************************
*  Service name    : Spi_WriteIB                                               
*  Syntax          : Std_ReturnType Spi_WriteIB(                               
*                           Spi_ChannelType Channel,                           
*                           const Spi_DataBufferType *DataBufferPtr)          
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
                           const Spi_DataBufferType *DataBufferPtr)
{
    Std_ReturnType RevtVal = E_OK;
    
    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_WRITEIB_SID, 
                      SPI_E_UNINIT, 
                      &RevtVal);

    /* SWS_Spi_00031 or SPI_FSR_00031 */
    Spi_lclParamCheck((boolean)(Channel >= SPI_MAX_CHANNEL),
                      SPI_WRITEIB_SID, 
                      SPI_E_PARAM_CHANNEL, 
                      &RevtVal);

    if (E_OK == RevtVal)
    {
        RevtVal = Spi_Hal_WriteIB(Channel, DataBufferPtr);
    }

    return RevtVal;
}

/******************************************************************************
*  Service name    : Spi_ReadIB                                                
*  Syntax          : Std_ReturnType Spi_ReadIB(                                
*                           Spi_ChannelType Channel,                           
*                           Spi_DataBufferType *DataBufferPtr)                
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
                          Spi_DataBufferType *DataBufferPtr)
{
    Std_ReturnType RevtVal = E_OK;
    
    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_READIB_SID, 
                      SPI_E_UNINIT, 
                      &RevtVal);
    /* SPI_FSR_001 */
    Spi_lclParamCheck((boolean)(NULL_PTR == DataBufferPtr),
                      SPI_READIB_SID, 
                      SPI_E_PARAM_POINTER, 
                      &RevtVal);

    /* SWS_Spi_00031 or SPI_FSR_00031 */
    Spi_lclParamCheck((boolean)(Channel >= SPI_MAX_CHANNEL),
                      SPI_READIB_SID, 
                      SPI_E_PARAM_CHANNEL, 
                      &RevtVal);

    if (E_OK == RevtVal)
    {
        RevtVal = Spi_Hal_ReadIB(Channel, DataBufferPtr);
    }

    return RevtVal;
}
#endif

#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 1U)||(SPI_CHANNEL_BUFFERS_ALLOWED == 2U))
/******************************************************************************
*  Service name        : Spi_SetupEB                                           
*  Syntax              : Std_ReturnType Spi_SetupEB(                           
*                               Spi_ChannelType Channel,                       
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
                           Spi_NumberOfDataType Length)
{
    Std_ReturnType RevtVal = E_OK;
    
    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_SETUPEB_SID, 
                      SPI_E_UNINIT, 
                      &RevtVal);

    /* SWS_Spi_00031 or SPI_FSR_00031 */
    Spi_lclParamCheck((boolean)(Channel >= SPI_MAX_CHANNEL),
                      SPI_SETUPEB_SID, 
                      SPI_E_PARAM_CHANNEL, 
                      &RevtVal);

    if (E_OK == RevtVal)
    {
        RevtVal = Spi_Hal_SetupEB(Channel,
                             SrcDataBufferPtr,
                             DesDataBufferPtr,
                             Length);
    }

    return RevtVal;
}
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
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence) 
{
    Std_ReturnType RevtVal = E_OK;
    
    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_SYNCTRANSMIT_SID,
                      SPI_E_UNINIT,
                      &RevtVal);

    /* SWS_Spi_00032 or SPI_FSR_00032 */
    Spi_lclParamCheck((boolean)(Sequence >= SPI_MAX_SEQUENCE),
                      SPI_SYNCTRANSMIT_SID,
                      SPI_E_PARAM_SEQ,
                      &RevtVal);
    
    if (E_OK == RevtVal)
    {
        RevtVal = Spi_Hal_SyncTransmit(Sequence);
    }
    
    return RevtVal;
}
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
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence)
{
    Std_ReturnType RevtVal = E_OK;

    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_ASYNCTRANSMIT_SID,
                      SPI_E_UNINIT,
                      &RevtVal);

    /* SWS_Spi_00032 or SPI_FSR_00032 */
    Spi_lclParamCheck((boolean)(Sequence >= SPI_MAX_SEQUENCE),
                      SPI_ASYNCTRANSMIT_SID,
                      SPI_E_PARAM_SEQ,
                      &RevtVal);

    if (E_OK == RevtVal)
    {
        RevtVal = Spi_Hal_AsyncTransmit(Sequence);
    }
    
    return RevtVal;
}
#endif

/*******************************************************************************
*  Service name    : Spi_GetStatus                                              
*  Syntax          : Spi_StatusType Spi_GetStatus( void )                       
*  Service ID      : 0x06                                                       
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : None                                                       
*  Parameters (out): None                                                       
*  Return value    : Spi_StatusType                                             
*  Description     : Service returns the SPI Handler/Driver software module     
*                    status.                                                    
*******************************************************************************/
Spi_StatusType Spi_GetStatus(void) 
{
    Spi_StatusType Status;
    
    /* SWS_Spi_00320 or SPI_FSR_00320 */
    if (SpiInitialized == FALSE)
    {
        Status = SPI_UNINIT;
    }
    else
    {     
        Status = Spi_Hal_GetStatus();
    }

    return Status;
}

/*******************************************************************************
*  Service name    : Spi_GetJobResult                                           
*  Syntax          : Spi_JobResultType Spi_GetJobResult( Spi_JobType Job )      
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
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job) 
{
    Std_ReturnType RevtVal      = E_OK;
    Spi_JobResultType JobResult = SPI_JOB_FAILED;
    
    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_GETJOBRESULT_SID,
                      SPI_E_UNINIT,
                      &RevtVal);

    /* SWS_Spi_00032 or SPI_FSR_00032 */
    Spi_lclParamCheck((boolean)(Job >= SPI_MAX_JOB),
                      SPI_GETJOBRESULT_SID,
                      SPI_E_PARAM_JOB,
                      &RevtVal);

    if (E_OK == RevtVal)
    {
        JobResult = Spi_Hal_GetJobResult(Job);
    }
    
    return JobResult;
}

/*******************************************************************************
*  Service name    : Spi_GetSequenceResult                                      
*  Syntax          : Spi_SeqResultType Spi_GetSequenceResult(                   
*                                                     Spi_SequenceType Sequence)
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
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence) 
{
    Std_ReturnType RevtVal      = E_OK;
    Spi_SeqResultType SeqResult = SPI_SEQ_FAILED;
    
    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_GETSEQUENCERESULT_SID,
                      SPI_E_UNINIT,
                      &RevtVal);

    /* SWS_Spi_00032 or SPI_FSR_00032 */
    Spi_lclParamCheck((boolean)(Sequence >= SPI_MAX_SEQUENCE),
                      SPI_GETSEQUENCERESULT_SID,
                      SPI_E_PARAM_SEQ,
                      &RevtVal);

    if (E_OK == RevtVal)
    { 
        SeqResult = Spi_Hal_GetSeqResult(Sequence);
    }
    
    return SeqResult;
}

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
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit) 
{
    Std_ReturnType RevtVal      = E_OK;
    Spi_StatusType HwUnitStatus = SPI_UNINIT;
    
    /* SWS_Spi_00143 or SPI_FSR_00143 */
    Spi_lclParamCheck((boolean)(HWUnit >= ALPS_SPI_HW_CNT),
                      SPI_GETHWUNITSTATUS_SID,
                      SPI_E_PARAM_UNIT,
                      &RevtVal);

    if (E_OK == RevtVal)
    {
        if (SpiInitialized == TRUE)
        {
            HwUnitStatus = Spi_Hal_GetHWUnitStatus(HWUnit);
        }
    }

    return HwUnitStatus;
}
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
void Spi_Cancel(Spi_SequenceType Sequence)
{
    Std_ReturnType RevtVal = E_OK;
    
    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_CANCEL_SID,
                      SPI_E_UNINIT,
                      &RevtVal);

    /* SWS_Spi_00032 or SPI_FSR_00032 */
    Spi_lclParamCheck((boolean)(Sequence >= SPI_MAX_SEQUENCE),
                      SPI_CANCEL_SID,
                      SPI_E_PARAM_SEQ,
                      &RevtVal);

    if (E_OK == RevtVal)
    {
        Spi_Hal_Cancel(Sequence);
    }
}
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
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode)
{
    Std_ReturnType RevtVal = E_OK;
    
    /* SWS_Spi_00046 or SPI_FSR_00046 */
    Spi_lclParamCheck((boolean)(FALSE == SpiInitialized),
                      SPI_SETASYNCMODE_SID,
                      SPI_E_UNINIT,
                      &RevtVal);
    
    if (E_OK == RevtVal)
    { 
        RevtVal = Spi_Hal_SetAsyncMode(Mode);
    }
    
    return RevtVal;
}
#endif

#if (SPI_VERSION_INFO_API == STD_ON)
/*******************************************************************************
*  Service name    : Spi_GetVersionInfo                                         
*  Syntax          : void Spi_GetVersionInfo(Std_VersionInfoType *VersionInfo)
*  Service ID      : 0x09                                                       
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : None                                                       
*  Parameters (out): VersionInfo -  Pointer to where to store the version       
*                                   information of this module.                 
*  Return value    : None                                                       
*  Description     : This service returns the version information of this module.
*******************************************************************************/
void Spi_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
    Std_ReturnType RevtVal = E_OK;
    
    /* SWS_Spi_00371 or SPI_FSR_00371 */
    Spi_lclParamCheck((boolean)(NULL_PTR == VersionInfo),
                      SPI_GETVERSIONINFO_SID,
                      SPI_E_PARAM_POINTER,
                      &RevtVal);
    
    if (E_OK == RevtVal)
    {
        VersionInfo->vendorID         = SPI_VENDOR_ID;
        VersionInfo->moduleID         = SPI_MODULE_ID;
        VersionInfo->sw_major_version = SPI_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = SPI_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = SPI_SW_PATCH_VERSION;
    }
}
#endif

/******************************************************************************
*  Service name    : Spi_MainFunction_Handling                                 
*  Syntax          : void Spi_MainFunction_Handling(void)                     
*  Service ID      : 0x10                                                      
*  Description     : ----                                                      
******************************************************************************/
void Spi_MainFunction_Handling(void)
{
    Spi_Hal_MainFunction_Handling();
}

#define SPI_STOP_SEC_CODE


