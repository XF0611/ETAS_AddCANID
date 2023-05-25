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
#include "Spi_Alps_RegDef.h"
#include "SchM_Spi.h"
#if(SPI_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#include "Mcu.h"

/******************************************************************************
*                       Macro Definitions
******************************************************************************/
#define SPI_CLEAR_CHANNEL_LOCK      0
#define SPI_SET_CHANNEL_LOCK        1
#define SPI_SW_RX_FIFO_THRESHOLD    25
#define SPI_SW_TX_FIFO_THRESHOLD    10


/******************************************************************************
*                       Private Variables                                      
******************************************************************************/
#define SPI_START_SEC_VAR_INIT_PTR

static Spi_reg_t *Spi_Hw_Reg[] = {(Spi_reg_t *)0xB60000U,
                                  (Spi_reg_t *)0xB70000U,
                                  (Spi_reg_t *)0xB90000U};
#define SPI_STOP_SEC_VAR_INIT_PTR


#define SPI_START_SEC_VAR_INIT_UNSPECIFIED

static Spi_GlobalType SpiGlobal;
#define SPI_STOP_SEC_VAR_INIT_UNSPECIFIED


#define SPI_START_SEC_VAR_INIT_8

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
static boolean FailStatus = FALSE;

static boolean CancelStatus = FALSE;
#endif

#if (SPI_SUPPORT_CONCURRENT_SYNC_TRANSMIT == STD_OFF)
static uint8 SyncLock = 0;
#endif
#define SPI_STOP_SEC_VAR_INIT_8


#if(SPI_CHANNEL_BUFFERS_ALLOWED != 1)
#define SPI_START_SEC_VAR_INIT_32

static uint8 IBBuffer[2U * SPI_MAXIMUM_IB_BUFFER_SIZE]; /* Multiplied by 2 to store both the tx and rx data */
#define SPI_STOP_SEC_VAR_INIT_32

#endif

#define SPI_START_SEC_CODE

/******************************************************************************
*                         Private Function Declaration                         
******************************************************************************/
#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 0U)||(SPI_CHANNEL_BUFFERS_ALLOWED == 2U))
static void Spi_lclSetOrClearChannelLock(Spi_ChannelType Channel, uint8 Cmd);
static uint8 Spi_lclGetChannelLock(Spi_ChannelType Channel);
#endif
static Std_ReturnType Spi_lclUpdateTxSWBuff(Spi_ChannelType Channel, uint8 SpiDataWidth);
static void Spi_lclReadRxSWBuff(Spi_ChannelType Channel, uint8 SpiDataWidth);
//static void Spi_lclTransmit(uint32 HwIdx , Spi_JobType Job);
static void Spi_lclTransmit(uint32 HwIdx);
static void Qspi_lclTransmit(uint32 HwIdx, Spi_JobType Job);
static Std_ReturnType Spi_lclProcessJob(Spi_JobType Job);
static uint32 Spi_lclRotateBits(uint32 Data, uint8 Len);
static uint32 Spi_lclByteSwap(uint32 Data, uint8 DataWidth);
#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
static Std_ReturnType Spi_lclCheckQueueAvailability(uint16 NumOfJobs);
static void Spi_lclQueuePush(Spi_SequenceType Sequence, uint16 JobIdx);
static uint16 Spi_lclGetNextQueueIdx(uint16 Index);
static void Spi_lclQueueCleanUp(void);
#if (SPI_INTERRUPTIBLE_SEQ_ALLOWED == STD_ON)
static uint16 Spi_lclStoreToTempQ(uint16 CurrQIdx, uint16 *JobQ, uint8 *JobLinkedSeqQ);
static void Spi_lclRearrangeQueue(Spi_SequenceType Sequence);
static uint16 Spi_lclGotoIntSeq(void);
#endif
#endif

/******************************************************************************
*                         Public Functions                                     
******************************************************************************/

/******************************************************************************
*  Service name    : Spi_Hal_Init                                              
*  Syntax          : void Spi_Hal_Init (const Spi_ConfigType *ConfigPtr)       
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : ConfigPtr - Pointer to configuration set                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Service for SPI initialization                            
******************************************************************************/
void Spi_Hal_Init (const Spi_ConfigType *ConfigPtr)
{
    uint16 Index;
    uint16 TempNumOfIBChannel;
    uint32 HwIdx;
    const Spi_ExternalDeviceType *ExternalDevPtr;

    #if (SPI_CHANNEL_BUFFERS_ALLOWED != 1)
    uint16 BufIndex;
    #endif
    
    Mcu_PeripheralClockValType ClkVal = 0;

    /* get the SPI system frequency */
    ClkVal = Mcu_GetPeripheralClkFreq(MCU_SPI_M0_CLOCK);
    
    if (ClkVal)
    {   
        SpiGlobal.SpiClk     = ClkVal;
        
        /* store the ConfigPtr in the global structure */
        SpiGlobal.ConfigPtr = ConfigPtr;
        
        /* set all job results to SPI_JOB_OK */
        for (Index = 0; Index < SPI_MAX_JOB; Index++)
        {
            SpiGlobal.JobUnit[Index].JobResult = SPI_JOB_OK;
        }
        /* set all sequence results to SPI_SEQ_OK */
        for (Index = 0; Index < SPI_MAX_SEQUENCE; Index++)
        {
            SpiGlobal.SeqUnit[Index].SeqResult = SPI_SEQ_OK;
            SpiGlobal.SeqUnit[Index].TotalJobs = \
            SpiGlobal.ConfigPtr->SpiSequenceConfig[Index].SpiSeqTotalJobs;
            SpiGlobal.SeqUnit[Index].SentJobs  = 0;
        }

        /* SWS_Spi_00151 */
        #if (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
        /* set the SPI Handler/Driver asynchronous mechanism mode to SPI_POLLING_MODE by default. */
        SpiGlobal.AsyncMode = SPI_POLLING_MODE;
        #endif

        /* set all the Hw modules to SPI_UNINIT */
        for (Index = 0; Index < ALPS_SPI_HW_CNT; Index++)
        {
            SpiGlobal.HwInfo[Index].Status = SPI_UNINIT;
        }
        
        /* loop over all hardware modules */
        for (Index = 0; Index < SPI_MAX_HW_UNIT; Index++)
        {
            ExternalDevPtr = &SpiGlobal.ConfigPtr->SpiExternalDevice[Index];
            /* get the hardware index */
            HwIdx = SpiGlobal.ConfigPtr->SpiExternalDevice[Index].SpiHwUnit;
            /* set hardware status to idle */
            SpiGlobal.HwInfo[HwIdx].Status = SPI_IDLE;
        }
        
        /* SWS_Spi_00082 */
        /* Channel Initialization */
        /* Reset Number of IB Channels  */
        SpiGlobal.NoOfIBChannels = 0; 
        TempNumOfIBChannel = 0;
        Index = 0U;

        #if (SPI_CHANNEL_BUFFERS_ALLOWED != 1)
        BufIndex = 0;
        SpiGlobal.IntBuf.IBBufferPtr = &IBBuffer[0];
        const Spi_ChannelConfigType *ChannelConfigPtr = \
                                            SpiGlobal.ConfigPtr->SpiChannelConfig;
        #endif
        do
        {
            #if (SPI_CHANNEL_BUFFERS_ALLOWED == 2)
            if (ChannelConfigPtr->SpiChannelType == SPI_IB)
            #endif
            #if (SPI_CHANNEL_BUFFERS_ALLOWED != 1)
            {
                /* Store Buffer Offset Position for IB Channels */
                SpiGlobal.IntBuf.IBBufferOffset[Index] = BufIndex;
                BufIndex = BufIndex + ChannelConfigPtr->SpiIbNBuffers;
                /* Store the Maximum IB buffer Channel for the current configuration */
                TempNumOfIBChannel++;
            }
            ChannelConfigPtr++;
            #endif
            Index++;
        } while (Index < SpiGlobal.ConfigPtr->SpiMaxChannel);
        
        /* copy the number of IB buffers in global structure. */
        SpiGlobal.NoOfIBChannels = TempNumOfIBChannel;
        
        #if (SPI_CHANNEL_BUFFERS_ALLOWED != 0)
        /* Initialize Index to the last channel i.e Max Channel */
        Index = SpiGlobal.ConfigPtr->SpiMaxChannel - TempNumOfIBChannel;
        do
        {
            Index--;
            SpiGlobal.ExtBuf[Index].Dest   = NULL_PTR;                /* Set EB Dest Pointer to NULL_PTR  */
            SpiGlobal.ExtBuf[Index].Src    = NULL_PTR;                /* Set EB Source Pointer to NULL_PTR  */
            SpiGlobal.ExtBuf[Index].Length = (Spi_NumberOfDataType)0; /* Set EB Length = 0 */
            SpiGlobal.ExtBuf[Index].Active = 0;                       /* set it to inactive state */
        } while (Index > 0U);
        #endif

        #if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
        /* Queue initialization with a default values */
        for (Index = 0; Index < SPI_MAX_JOB_Q_LENGTH; Index++)
        {
            SpiGlobal.Queue.JobQueue[Index]     = 0xFFFF;
            SpiGlobal.Queue.JobLinkedSeq[Index] = 0xFF;
        }
        SpiGlobal.Queue.JobStartIndex   = 0;
        SpiGlobal.Queue.JobEndIndex     = 0;
        #endif

        /* SWS_Spi_00015 */
        /* set driver state to SPI_IDLE */
        SpiGlobal.DriverStatus = SPI_IDLE;
    }
}

/******************************************************************************
*  Service name    : Spi_Hal_DeInit                                            
*  Syntax          : Std_ReturnType Spi_Hal_DeInit(void)                       
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: de-initialization command has been accepted
*                    E_NOT_OK: de-initialization command has not been accepted
*  Description     : Service for SPI de-initialization.                        
******************************************************************************/
Std_ReturnType Spi_Hal_DeInit(void)
{
	Std_ReturnType ret_val = E_OK;
    uint16 Index;
    uint32 HwIdx;
    
    /* SWS_Spi_00253 or SPI_FSR_00253 */
    if (SPI_BUSY == Spi_Hal_GetStatus())
    {
    	ret_val =  E_NOT_OK;
    }
    else
    {
		/* SWS_Spi_00252 */
		/* loop over all hardware modules */
		for (HwIdx = 0; HwIdx < ALPS_SPI_HW_CNT; HwIdx++)
		{
			if (SpiGlobal.HwInfo[HwIdx].Status == SPI_IDLE)
			{
				/* disable the module */
				Spi_Hw_Reg[HwIdx]->SSIENR        = 0;
				while(Spi_Hw_Reg[HwIdx]->SSIENR != 0)
				{
					;
				}
				/* set value on reset in the respective registers */
				Spi_Hw_Reg[HwIdx]->CTRLR0        = 0x7u;           /* value on reset */
				Spi_Hw_Reg[HwIdx]->BAUDR         = 0u;             /* reset baud register */
				Spi_Hw_Reg[HwIdx]->SER           = 0u;             /* reset slave select register */
				Spi_Hw_Reg[HwIdx]->RX_SAMPLE_DLY = 0u;             /* reset rx_sample delay    */
				SpiGlobal.HwInfo[HwIdx].Status   = SPI_UNINIT;
			}
		}

		#if (SPI_LEVEL_DELIVERED != SPI_LEVEL_0)
		/* Queue de-initialization with a default values */
		for (Index = 0; Index < SPI_MAX_JOB_Q_LENGTH; Index++)
		{
			SpiGlobal.Queue.JobQueue[Index]     = 0xFFFF;
			SpiGlobal.Queue.JobLinkedSeq[Index] = 0xFF;
		}
		SpiGlobal.Queue.JobStartIndex   = 0;
		SpiGlobal.Queue.JobEndIndex     = 0;
		#endif

		SpiGlobal.SpiClk        = 0;
		SpiGlobal.ConfigPtr     = NULL_PTR;

		/* SWS_Spi_00022 */
		/* set driver state to SPI_UNINIT */
		SpiGlobal.DriverStatus = SPI_UNINIT;
    }
    
    return ret_val;
}

/*******************************************************************************
*  Service name    : Spi_Hal_GetStatus                                          
*  Syntax          : Spi_StatusType Spi_Hal_GetStatus(void)                     
*  Parameters (in) : None                                                       
*  Parameters (out): None                                                       
*  Return value    : Spi_StatusType                                             
*  Description     : Service returns the SPI Handler/Driver software module     
*                    status.                                                    
*******************************************************************************/
Spi_StatusType Spi_Hal_GetStatus(void)
{
    uint8 Index;
    Spi_StatusType RetVal = SPI_IDLE;
    
    /* checks if any sequence is SPI_SEQ_PENDING */
    /* if pending returns SPI_BUSY */
    for (Index = 0; Index < SPI_MAX_SEQUENCE; Index++)
    {
        if (SpiGlobal.SeqUnit[Index].SeqResult == SPI_SEQ_PENDING)
        {
            RetVal = SPI_BUSY;
        }
    }
    return RetVal;
}

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
Spi_JobResultType Spi_Hal_GetJobResult(Spi_JobType Job)
{
    /* return the corresponding job result. */
    return SpiGlobal.JobUnit[Job].JobResult;
}

/*******************************************************************************
*  Service name    : Spi_Hal_GetSeqResult                                       
*  Syntax          : Spi_SeqResultType Spi_Hal_GetSeqResult(                    
*                                                    Spi_SequenceType Sequence) 
*  Parameters (in) : Sequence - Sequence ID. An invalid sequence ID will return 
*                               an undefined result                             
*  Parameters (out): None                                                       
*  Return value    : Spi_SeqResultType                                          
*  Description     : This service returns the last transmission result of the   
*                    specified Sequence.                                        
*******************************************************************************/
Spi_SeqResultType Spi_Hal_GetSeqResult(Spi_SequenceType Sequence)
{
    /* return the corresponding sequence result. */
    return SpiGlobal.SeqUnit[Sequence].SeqResult;
}

#if (SPI_HW_STATUS_API == STD_ON)
/*******************************************************************************
*  Service name    : Spi_Hal_GetHWUnitStatus                                    
*  Syntax          : Spi_StatusType Spi_Hal_GetHWUnitStatus(Spi_HWUnitType HWUnit)
*  Parameters (in) : HWUnit - SPI Hardware microcontroller peripheral (unit) ID.
*  Parameters (out): None                                                       
*  Return value    : Spi_StatusType                                             
*  Description     : This service returns the status of the specified           
*                    SPI Hardware microcontroller peripheral.
*******************************************************************************/
Spi_StatusType Spi_Hal_GetHWUnitStatus(Spi_HWUnitType HWUnit)
{
    /* return the corresponding hw status from the unit structure */
    return SpiGlobal.HwInfo[HWUnit].Status;
}
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
                               Spi_NumberOfDataType Length)
{
    const Spi_ChannelConfigType *ChannelConfigPtr;
    Std_ReturnType  RetVal      = E_OK;
    uint16 TempChannel          = 0;
    
    ChannelConfigPtr = &SpiGlobal.ConfigPtr->SpiChannelConfig[Channel];

    #if (SPI_CHANNEL_BUFFERS_ALLOWED != 0U)
    Spi_lclParamCheck((boolean)(SPI_EB != ChannelConfigPtr->SpiChannelType),
                      SPI_SETUPEB_SID,
                      SPI_E_PARAM_CHANNEL,
                      &RetVal);
    #endif

    /* SWS_Spi_00060 or SPI_FSR_00060 */
    Spi_lclParamCheck((boolean)(Length > ChannelConfigPtr->SpiEbMaxLength),
                      SPI_SETUPEB_SID,
                      SPI_E_PARAM_LENGTH,
                      &RetVal);
    
    /* assign the source and destination pointer to corresponding channel EB allocated. */
    if (RetVal == E_OK)
    {
        TempChannel                          = Channel - SpiGlobal.NoOfIBChannels;
        SpiGlobal.ExtBuf[TempChannel].Src    = SrcDataBufferPtr;
        SpiGlobal.ExtBuf[TempChannel].Dest   = DesDataBufferPtr;
        SpiGlobal.ExtBuf[TempChannel].Length = Length;
        SpiGlobal.ExtBuf[TempChannel].Active = 1;
    }

    return RetVal;
}
#endif

#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 0U)||(SPI_CHANNEL_BUFFERS_ALLOWED == 2U))
/******************************************************************************
*  Service name    : Spi_Hal_ReadIB                                            
*  Syntax          : Std_ReturnType Spi_Hal_ReadIB(Spi_ChannelType Channel,    
*                                                  Spi_DataBufferType *DataBufferPtr)
*  Parameters (in) : Channel - Channel ID.                                     
*  Parameters (out): DataBufferPtr - Pointer to destination data buffer in RAM 
*  Return value    : E_OK: read command has been accepted                      
*                    E_NOT_OK: read command has not been accepted              
*  Description     : Service for reading one or more data to an IB             
*                    SPI Handler/Driver Channel specified by parameter.        
******************************************************************************/
Std_ReturnType Spi_Hal_ReadIB(Spi_ChannelType Channel,
                              Spi_DataBufferType *DataBufferPtr)
{
    const Spi_ChannelConfigType *ChannelConfigPtr;
    uint16 IBStartIndex, IBEndIndex, Index;
    Std_ReturnType RetVal = E_OK;
        
    /* get the channel configuration pointer */
    ChannelConfigPtr = &SpiGlobal.ConfigPtr->SpiChannelConfig[Channel];
    
    /* get the offset where the received data is stored */
    IBStartIndex = (SpiGlobal.IntBuf.IBBufferOffset[Channel]\
                                                    + SPI_MAXIMUM_IB_BUFFER_SIZE);

    /* get the end index till where it should be read */
    IBEndIndex = ChannelConfigPtr->SpiIbNBuffers;
    
    for (Index = 0; Index != IBEndIndex; Index++)
    {
        DataBufferPtr[Index] = SpiGlobal.IntBuf.IBBufferPtr[IBStartIndex];
        IBStartIndex++;
    }

    return RetVal;
}

/******************************************************************************
*  Service name    : Spi_Hal_WriteIB                                           
*  Syntax          : Std_ReturnType Spi_Hal_WriteIB(                           
*                           Spi_ChannelType Channel,                           
*                           const Spi_DataBufferType* DataBufferPtr )          
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
                               const Spi_DataBufferType *DataBufferPtr)
{
    const Spi_ChannelConfigType *ChannelConfigPtr;
    uint16                      IBStartIndex;
    uint16                      IBEndIndex;
    uint16                      Index;

    Std_ReturnType RetVal = E_OK;
     
    /* get the channel configuration pointer */
    ChannelConfigPtr = &SpiGlobal.ConfigPtr->SpiChannelConfig[Channel];
    
    /* check for channel write lock */
    RetVal = E_NOT_OK;

    /* disable OS interrupts */
    SchM_Enter_Spi_WriteIB();

    if (Spi_lclGetChannelLock(Channel) == 0)
    {
        RetVal = E_OK;

        /* set channel lock */
        Spi_lclSetOrClearChannelLock(Channel, SPI_SET_CHANNEL_LOCK);

        /* enable the OS interrupts */
        SchM_Exit_Spi_WriteIB();

        /* get the offset where the data should be written */
        IBStartIndex = (SpiGlobal.IntBuf.IBBufferOffset[Channel]);

        /* get the end index till where it should be written */
        IBEndIndex = ChannelConfigPtr->SpiIbNBuffers;

        /* if DataBufferPtr is null pointer copy the default data */
        if (DataBufferPtr == NULL_PTR)
        {
            SpiGlobal.IntBuf.NullParamStat[Channel] = TRUE;
        }
        else
        {
            SpiGlobal.IntBuf.NullParamStat[Channel] = FALSE;
            for (Index = 0; Index != IBEndIndex ; Index++)
            {
                SpiGlobal.IntBuf.IBBufferPtr[IBStartIndex] = DataBufferPtr[Index];
                IBStartIndex++;
            }
        }

        /* disable OS interrupts */
        SchM_Enter_Spi_WriteIB();
        
        /* clear channel lock */
        Spi_lclSetOrClearChannelLock(Channel, SPI_CLEAR_CHANNEL_LOCK);
    }

    /* enable the OS interrupts */
    SchM_Exit_Spi_WriteIB();

    return RetVal;
}
#endif

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_0) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
/********************************************************************************
*  Service name    : Spi_Hal_SyncTransmit                                        
*  Syntax          : Std_ReturnType Spi_Hal_SyncTransmit(Spi_SequenceType Sequence)
*  Parameters (in) : Sequence - Sequence ID.                                     
*  Parameters (out): None                                                        
*  Return value    : E_OK: Transmission command has been accepted.               
*                    E_NOT_OK: Transmission command has not been accepted.       
*  Description     : Service to transmit data on the SPI bus.                    
********************************************************************************/
Std_ReturnType Spi_Hal_SyncTransmit(Spi_SequenceType Sequence)
{
    const Spi_SequenceConfigType *SeqConfigPtr1, *SeqConfigPtr2;
    const Spi_JobConfigType      *JobConfigPtr;
    Spi_JobType                  JobIdx1, JobIdx2, JobIdx;
    Spi_JobType                  JobId1, JobId2, JobId;
    uint32                       HwIdx;

    Std_ReturnType RetVal = E_OK;
    boolean FailStaus     = FALSE;

    /* check if the sequence is already pending */
    if (SPI_SEQ_PENDING == Spi_Hal_GetSeqResult(Sequence))
    {
        return E_NOT_OK;
    }
    
    #if (SPI_SUPPORT_CONCURRENT_SYNC_TRANSMIT == STD_OFF)
    if (SyncLock == 1)
    {
        /* SWS_Spi_00135 or SPI_FSR_00135 */
        Spi_lclParamCheck(TRUE, SPI_SYNCTRANSMIT_SID, SPI_E_SEQ_IN_PROCESS, &RetVal);
        return RetVal;
    }
    #endif
    
    SeqConfigPtr1 = &SpiGlobal.ConfigPtr->SpiSequenceConfig[Sequence];
    
    /* check for sharing jobs or hardware */
    for (uint8 SeqIdx = 0; SeqIdx < SPI_MAX_SEQUENCE; SeqIdx++)
    {
        if (SeqIdx == Sequence)
        {
            continue;
        }
        else if (SPI_SEQ_PENDING == Spi_Hal_GetSeqResult(SeqIdx))
        {    
		    /* Disable the OS interrupts */
			SchM_Enter_Spi_SyncTransmit();
            SeqConfigPtr2 = &SpiGlobal.ConfigPtr->SpiSequenceConfig[SeqIdx];
            for (JobIdx1 = 0; SeqConfigPtr1->SpiJobAssignment[JobIdx1] != \
                                               SPI_JOB_END_INDICATOR; JobIdx1++)
            {
                JobId1 = SeqConfigPtr1->SpiJobAssignment[JobIdx1];
                for (JobIdx2 = 0; SeqConfigPtr2->SpiJobAssignment[JobIdx2] != \
                                                SPI_JOB_END_INDICATOR; JobIdx2++)
                {
                    JobId2 = SeqConfigPtr2->SpiJobAssignment[JobIdx2];
                    /* check for shared jobs */
                    if (JobId1 == JobId2)
                    {
                        if (SPI_JOB_PENDING == Spi_Hal_GetJobResult(JobId1))
                        {
                            /* enable the OS interrupts */
                            SchM_Exit_Spi_SyncTransmit();

                            /* return E_NOT_OK if it has some shared jobs which are pending. */
                            return E_NOT_OK;
                        }
                    }
                }
                #if (SPI_SUPPORT_CONCURRENT_SYNC_TRANSMIT == STD_ON)
                /* SWS_Spi_00114 */
                JobConfigPtr = &SpiGlobal.ConfigPtr->SpiJobConfig[JobId1];
                HwIdx        = JobConfigPtr->SpiDeviceAssignment->SpiHwUnit;
                if (SpiGlobal.HwInfo[HwIdx].Status == SPI_BUSY) 
                {
                    /* enable the OS interrupts */
                    SchM_Exit_Spi_SyncTransmit();

                    /* SWS_Spi_00135 */
                    /* Hardware Bus is not available */
                    Spi_lclParamCheck(TRUE,
                                      SPI_SYNCTRANSMIT_SID,
                                      SPI_E_SEQ_IN_PROCESS,
                                      &RetVal);
                    return RetVal;
                }
                #endif
            }
        }
        else
        {
        	/*compliant to misra-c*/
        }
    }

    #if (SPI_SUPPORT_CONCURRENT_SYNC_TRANSMIT == STD_OFF)
    /* turn ON the SyncLock */
    SyncLock = 1;
    #endif
    
    /* SWS_Spi_00134 */
    /* set the SPI Handler/driver status to busy  */
    SpiGlobal.DriverStatus = SPI_BUSY;
    
    /* SWS_Spi_00285 */
    /* set sequence result to sequence pending. */
    SpiGlobal.SeqUnit[Sequence].SeqResult = SPI_SEQ_PENDING;
   
    /* SWS_Spi_00286 */
    /* set all the job results to job pending and hardware unit status to busy */
    for (JobIdx = 0; SeqConfigPtr1->SpiJobAssignment[JobIdx] != \
                                        SPI_JOB_END_INDICATOR; JobIdx++)
    {
        JobId = SeqConfigPtr1->SpiJobAssignment[JobIdx];
        SpiGlobal.JobUnit[JobId].JobResult = SPI_JOB_PENDING;
        JobConfigPtr = &SpiGlobal.ConfigPtr->SpiJobConfig[JobId];
        HwIdx = JobConfigPtr->SpiDeviceAssignment->SpiHwUnit;
        SpiGlobal.HwInfo[HwIdx].Status = SPI_BUSY;
    }
    
    /* CallType is not being used for now. Required for interrupt mode */
    SpiGlobal.CallType = SPI_SYNC_CALL;
    
    /* set the sent jobs variable to 0 */
    SpiGlobal.SeqUnit[Sequence].SentJobs = 0;

    /* loop over all the jobs to process jobs */
    for (JobIdx = 0; SeqConfigPtr1->SpiJobAssignment[JobIdx] != \
                                        SPI_JOB_END_INDICATOR; JobIdx++)
    {
        RetVal = Spi_lclProcessJob(SeqConfigPtr1->SpiJobAssignment[JobIdx]);
        if (RetVal == E_NOT_OK)
        {
          FailStaus = TRUE;
          break;
        }  
        SpiGlobal.SeqUnit[Sequence].SentJobs++;
    }
    
    if (TRUE == FailStaus)
    {
        /* make all the pending job status as failed and hardware unit status to idle */
        for(JobIdx = 0; SeqConfigPtr1->SpiJobAssignment[JobIdx] != \
                                        SPI_JOB_END_INDICATOR ; JobIdx++)
        {
            JobId = SeqConfigPtr1->SpiJobAssignment[JobIdx];
            if (SPI_JOB_PENDING == Spi_Hal_GetJobResult(JobId))
            {
                SpiGlobal.JobUnit[JobId].JobResult = SPI_JOB_FAILED;
                JobConfigPtr = &SpiGlobal.ConfigPtr->SpiJobConfig[JobId];
                /* Get the hw_idx */
                HwIdx = JobConfigPtr->SpiDeviceAssignment->SpiHwUnit;
                /* set hardware unit status to SPI_IDLE */
                SpiGlobal.HwInfo[HwIdx].Status = SPI_IDLE;
                /* call job end notification */
                if (JobConfigPtr->SpiJobEndNotification != NULL_PTR)
                {
                    JobConfigPtr->SpiJobEndNotification();
                }
            }
        }

        SpiGlobal.SeqUnit[Sequence].SeqResult = SPI_SEQ_FAILED;
        /* SWS_Spi_00120 */
        /* call sequence end notification */
        /* Disable the OS interrupts */
	    SchM_Enter_Spi_SyncTransmit();
        if (SeqConfigPtr1->SpiSeqEndNotification != NULL_PTR)
        {
            SeqConfigPtr1->SpiSeqEndNotification();
        }

        /* enable the OS interrupts */
        SchM_Exit_Spi_SyncTransmit();

        return RetVal;
    }
    
    /* set sequence result to sequence SPI_SEQ_OK. */
    SpiGlobal.SeqUnit[Sequence].SeqResult = SPI_SEQ_OK;
    
    /* set the SPI Handler/driver status to busy  */
    SpiGlobal.DriverStatus = SPI_IDLE;
    
    #if (SPI_SUPPORT_CONCURRENT_SYNC_TRANSMIT == STD_OFF)
    /* turn OFF the SyncLock */
    SyncLock = 0;
    #endif
   
   /* SWS_Spi_00120 */
    /* Disable the OS interrupts */
	SchM_Enter_Spi_SyncTransmit();
    if (SeqConfigPtr1->SpiSeqEndNotification != NULL_PTR)
    {
        SeqConfigPtr1->SpiSeqEndNotification();
    }
    
    /* enable the OS interrupts */
    SchM_Exit_Spi_SyncTransmit();
   
   return RetVal;
}
#endif

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
/********************************************************************************
*  Service name    : Spi_Hal_AsyncTransmit                                       
*  Syntax          : Std_ReturnType Spi_Hal_AsyncTransmit(Spi_SequenceType Sequence)
*  Parameters (in) : Sequence - Sequence ID.                                     
*  Parameters (out): None                                                        
*  Return value    : E_OK: Transmission command has been accepted.               
*                    E_NOT_OK: Transmission command has not been accepted.       
*  Description     : Service to transmit data on the SPI bus.                    
********************************************************************************/
Std_ReturnType Spi_Hal_AsyncTransmit(Spi_SequenceType Sequence)
{
     const Spi_SequenceConfigType *SeqConfigPtr1, *SeqConfigPtr2;
     Spi_JobType                  JobIdx1, JobIdx2, JobIdx;
     Spi_JobType                  JobId1, JobId2, JobId;
     Spi_JobResultType            JobResult;

     Std_ReturnType RetVal = E_OK;

    /* Disable the OS interrupts */
    SchM_Enter_Spi_AsyncTransmit();
    
    /* check if the sequence is already pending */
    if (SPI_SEQ_PENDING == Spi_Hal_GetSeqResult(Sequence))
    {
        /* enable the OS interrupts */
        SchM_Exit_Spi_AsyncTransmit();

        /* SWS_Spi_00081 */
        Spi_lclParamCheck(TRUE, SPI_ASYNCTRANSMIT_SID, SPI_E_SEQ_PENDING, &RetVal);
        return RetVal;
    }
    
    SeqConfigPtr1 = &SpiGlobal.ConfigPtr->SpiSequenceConfig[Sequence];
    
    /* check for sharing jobs */
    for (uint8 SeqIdx = 0; SeqIdx < SPI_MAX_SEQUENCE ; SeqIdx++)
    {
        if (SeqIdx == Sequence)
        {
            continue;
        }
        else if (SPI_SEQ_PENDING == Spi_Hal_GetSeqResult(SeqIdx))
        {
            SeqConfigPtr2 = &SpiGlobal.ConfigPtr->SpiSequenceConfig[SeqIdx];
            for (JobIdx1 = 0; SeqConfigPtr1->SpiJobAssignment[JobIdx1] != \
                                            SPI_JOB_END_INDICATOR; JobIdx1++)
            {
                JobId1 = SeqConfigPtr1->SpiJobAssignment[JobIdx1];
                for (JobIdx2 = 0; SeqConfigPtr2->SpiJobAssignment[JobIdx2] != \
                                               SPI_JOB_END_INDICATOR; JobIdx2++)
                {
                    JobId2 = SeqConfigPtr2->SpiJobAssignment[JobIdx2];

                    /* check for shared jobs */
                    if (JobId1 == JobId2)
                    {
                        JobResult = Spi_Hal_GetJobResult(JobId1);
                        if (SPI_JOB_PENDING == JobResult || SPI_JOB_QUEUED == JobResult)
                        {
                            /* enable the OS interrupts */
                            SchM_Exit_Spi_AsyncTransmit();

                            /* return E_NOT_OK if it has some shared jobs which are pending. */
                            /* SWS_Spi_00086 */
                            Spi_lclParamCheck(TRUE,
                                              SPI_ASYNCTRANSMIT_SID,
                                              SPI_E_SEQ_PENDING,
                                              &RetVal);
                            return RetVal;
                        } /* end if(SPI_JOB_PENDING == Spi_Hal_GetJobResult(JobId1)) */
                    } /* end if (JobId1 == JobId2) */
                } /* end for(JobIdx2 = 0; SeqConfigPtr2->SpiJobAssignment[JobIdx2] != ...) */
            } /* end for(JobIdx1 = 0; SeqConfigPtr1->SpiJobAssignment[JobIdx1] != ...)  */
        } /* end else if (SPI_SEQ_PENDING == Spi_Hal_GetSeqResult(SeqIdx))     */
        else
        {
        	/*compliant to misra-c*/
        }
    } /* end for(uint8 SeqIdx = 0; SeqIdx < SPI_MAX_SEQUENCE ; SeqIdx++) */
    
    /* queue cleanup */
    if (FailStatus || CancelStatus)
    {
        Spi_lclQueueCleanUp();
        FailStatus = FALSE;
        #if (SPI_CANCEL_API == STD_ON)
        CancelStatus = FALSE;
        #endif
    }
    
    /* check for queue availability. */
    RetVal = Spi_lclCheckQueueAvailability(SpiGlobal.SeqUnit[Sequence].TotalJobs);
    if (RetVal == E_NOT_OK)
    {
        /* enable the OS interrupts */
        SchM_Exit_Spi_AsyncTransmit();
        return E_NOT_OK;
    }
    
    /* CallType is not being used for now. Required for interrupt mode */
    SpiGlobal.CallType = SPI_ASYNC_CALL;
    
    /* SWS_Spi_00020 */
    /* set sequence result to sequence pending. */
    SpiGlobal.SeqUnit[Sequence].SeqResult = SPI_SEQ_PENDING;
        
    /* SWS_Spi_00194 */
    /* set all the job results to SPI_JOB_QUEUED  */
    for (JobIdx = 0; SeqConfigPtr1->SpiJobAssignment[JobIdx] != \
                                            SPI_JOB_END_INDICATOR; JobIdx++)
    {
        JobId = SeqConfigPtr1->SpiJobAssignment[JobIdx];
        SpiGlobal.JobUnit[JobId].JobResult = SPI_JOB_QUEUED;
    }
    
    /* set the sent jobs variable to 0 */
    SpiGlobal.SeqUnit[Sequence].SentJobs = 0;

    #if (SPI_INTERRUPTIBLE_SEQ_ALLOWED == STD_OFF)
    /* push all the jobs to the queue */
    Spi_lclQueuePush(Sequence,0);
   
    #else
    /* rearrange the queue based on job priority */
    Spi_lclRearrangeQueue(Sequence);
    #endif
	 /* Disable the OS interrupts */
    SchM_Enter_Spi_SyncTransmit();
    /* if driver is idle start transmission of first job */
    if (SpiGlobal.DriverStatus == SPI_IDLE)
    {
      SpiGlobal.DriverStatus = SPI_BUSY;
      Spi_Hal_MainFunction_Handling();
    }

    /* enable the OS interrupts */
    SchM_Exit_Spi_AsyncTransmit();

    return RetVal;
}
#endif

#if (SPI_CANCEL_API == STD_ON )
/*******************************************************************************
*  Service name    : Spi_Hal_Cancel                                             
*  Syntax          : void Spi_Hal_Cancel( Spi_SequenceType Sequence )           
*  Parameters (in) : Sequence - Sequence ID.                                    
*  Parameters (out): None                                                       
*  Return value    : None                                                       
*  Description     : Service cancels the specified on-going sequence            
*                    transmission.                                              
*******************************************************************************/
void Spi_Hal_Cancel(Spi_SequenceType Sequence)
{
    const Spi_SequenceConfigType *SeqConfigPtr;
    Spi_JobType                  JobIdx, JobId;
    const Spi_JobConfigType      *JobConfigPtr;
    Spi_JobResultType            JobRes;
    
    SeqConfigPtr = &SpiGlobal.ConfigPtr->SpiSequenceConfig[Sequence];
    #if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
    CancelStatus = TRUE;
    #endif
    
    /* set the queued job result to job failed */
    for (JobIdx = 0; SeqConfigPtr->SpiJobAssignment[JobIdx] != \
                                              SPI_JOB_END_INDICATOR; JobIdx++)
    {
        JobId = SeqConfigPtr->SpiJobAssignment[JobIdx];
        JobRes = Spi_Hal_GetJobResult(JobId);

        if (SPI_JOB_QUEUED == JobRes)
        {
            SpiGlobal.JobUnit[JobId].JobResult = SPI_JOB_FAILED;
            JobConfigPtr = &SpiGlobal.ConfigPtr->SpiJobConfig[JobId];

            /* SWS_Spi_00119 */
            if (JobConfigPtr->SpiJobEndNotification != NULL_PTR)
            {
                JobConfigPtr->SpiJobEndNotification();
            }
        }
    }
    SpiGlobal.SeqUnit[Sequence].SeqResult = SPI_SEQ_CANCELLED;

    /* SWS_Spi_00120 */
    if (SeqConfigPtr->SpiSeqEndNotification != NULL_PTR)
    {
        SeqConfigPtr->SpiSeqEndNotification();
    }
    
}
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
*                    busses handled asynchronously.
*******************************************************************************/
Std_ReturnType Spi_Hal_SetAsyncMode(Spi_AsyncModeType Mode)
{
    return E_NOT_OK;
}
#endif

/******************************************************************************
*  Service name    : Spi_Hal_MainFunction_Handling                             
*  Syntax          : void Spi_MainFunction_Handling(void)                      
*  Description     : ----                                                      
******************************************************************************/
void Spi_Hal_MainFunction_Handling(void)
{ 
#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
    if(FailStatus || CancelStatus)
    {
        Spi_lclQueueCleanUp();
        FailStatus = FALSE;
        #if (SPI_CANCEL_API == STD_ON)
        CancelStatus = FALSE;  
        #endif 
    }
    
    if (SpiGlobal.Queue.JobStartIndex != SpiGlobal.Queue.JobEndIndex)
    {
        const Spi_SequenceConfigType *SeqConfigPtr;
        const Spi_JobConfigType      *JobConfigPtr;
        Spi_JobType                  JobId, StartIdx, JobIdx;
        Spi_JobResultType            JobRes;
        Spi_SequenceType             SeqId;
        Std_ReturnType               RetVal;
                
        StartIdx                      = SpiGlobal.Queue.JobStartIndex;   
        JobId                         = SpiGlobal.Queue.JobQueue[StartIdx];
        SeqId                         = SpiGlobal.Queue.JobLinkedSeq[StartIdx];
        SeqConfigPtr                  = &SpiGlobal.ConfigPtr->SpiSequenceConfig[SeqId];
        SpiGlobal.Queue.JobStartIndex = Spi_lclGetNextQueueIdx(StartIdx);
        
        /* Disable OS interrupts */
        SchM_Enter_Spi_MainFunction();
        
        RetVal = Spi_lclProcessJob(JobId);
        
        /* Enable OS interrupts */
        SchM_Exit_Spi_MainFunction();
        
        if (RetVal == E_NOT_OK)
        {
            /* set the pending job result to SPI_JOB_FAILED. */
            for (JobIdx = 0; SeqConfigPtr->SpiJobAssignment[JobIdx] != \
                       SPI_JOB_END_INDICATOR ; JobIdx++)
            {
                JobId = SeqConfigPtr->SpiJobAssignment[JobIdx];
                JobRes = Spi_Hal_GetJobResult(JobId);
                if ((SPI_JOB_PENDING == JobRes) || (SPI_JOB_QUEUED == JobRes))
                {
                    SpiGlobal.JobUnit[JobId].JobResult = SPI_JOB_FAILED;
                    JobConfigPtr = &SpiGlobal.ConfigPtr->SpiJobConfig[JobId];

                    /* SWS_Spi_00119 */
                    if (JobConfigPtr->SpiJobEndNotification != NULL_PTR)
                    {
                       JobConfigPtr->SpiJobEndNotification();
                    }
                }
            }
            SpiGlobal.SeqUnit[SeqId].SeqResult = SPI_SEQ_FAILED;

            /* SWS_Spi_00120 */
            if (SeqConfigPtr->SpiSeqEndNotification != NULL_PTR)
            {
                SeqConfigPtr->SpiSeqEndNotification();
            }
            FailStatus = TRUE;
        }
        else
        {
            SpiGlobal.SeqUnit[SeqId].SentJobs++;
            SpiGlobal.Queue.JobQueue[StartIdx] = 0xFFFF;
            SpiGlobal.Queue.JobLinkedSeq[StartIdx] = 0xFF;
            
            if (SpiGlobal.SeqUnit[SeqId].SentJobs == SpiGlobal.SeqUnit[SeqId].TotalJobs)
            {
                /* set sequence result to sequence SPI_SEQ_OK. */
                SpiGlobal.SeqUnit[SeqId].SeqResult = SPI_SEQ_OK;

                /* SWS_Spi_00120 */
                if (SeqConfigPtr->SpiSeqEndNotification != NULL_PTR)
                {
                    SeqConfigPtr->SpiSeqEndNotification();
                }          
            }
        }
    }
    else
    {
        SpiGlobal.DriverStatus = SPI_IDLE;
        SpiGlobal.Queue.JobStartIndex = 0;
        SpiGlobal.Queue.JobEndIndex = 0;
    }

#endif
}

/******************************************************************************
*                         Private Functions                                     
******************************************************************************/

/*******************************************************************************
*  Service name     : Spi_lclParamCheck                                            
*  Syntax           : void Spi_lclParamCheck(boolean Expr,                         
*                                           uint8 ApiId,                          
*                                           uint8 ErrId,                          
*                                           Std_ReturnType *RetVal)                    
*  Parameters (in)  : Expr - TRUE or FALSE                                       
*                     ApiId - API ID                                            
*                     ErrId - ERROR ID                                          
*  Parameter(in-out): RetVal
*  Return value     : None                                                       
*  Description      : Reports DET error if SPI_DEV_ERROR_DETECT == STD_ON.
*******************************************************************************/
void Spi_lclParamCheck(boolean Expr, uint8 ApiId, uint8 ErrId, Std_ReturnType *RetVal)
{
    /* checks if the expr is TRUE */
    if ((TRUE == Expr) && (E_OK == *RetVal))
    {
        #if (SPI_DEV_ERROR_DETECT == STD_ON)
        /* calls Det_ReportError from Det module */
        (void)Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, ApiId, ErrId);
        #endif

        *RetVal = E_NOT_OK;
    }
}

#if ((SPI_CHANNEL_BUFFERS_ALLOWED == 0U)||(SPI_CHANNEL_BUFFERS_ALLOWED == 2U))
/*******************************************************************************
*  Service name    : Spi_lclGetChannelLock                                      
*  Syntax          : uint8 Spi_lclGetChannelLock(Spi_ChannelType Channel)       
*  Parameters (in) : Channel - Channel ID                                       
*  Parameter (out) : --                                                         
*  Return value    : LockStatus                                                
*  Description     : Returns the lock status of the specified channel           
*******************************************************************************/
static uint8 Spi_lclGetChannelLock(Spi_ChannelType Channel)
{
    uint16 Index;
    uint8  LockStatus;

    /* get the index where the lock status for this channel
       is set in the ChannelLock array */ 
    Index = Channel >> 3;

    /* get the lock status */
    LockStatus = ((SpiGlobal.IntBuf.ChannelLock[Index]) & \
                                                   (1u << ((Channel) & 0x7)));

    return LockStatus;
}

/*******************************************************************************
*  Service name    : Spi_lclSetOrClearChannelLock                               
*  Syntax          : void Spi_lclSetOrClearChannelLock(Spi_ChannelType Channel, 
*                                                      uint8 Cmd)              
*  Parameters (in) : Channel - Channel ID                                       
*                    Cmd - command SPI_SET_CHANNEL_LOCK or SPI_CLEAR_CHANNEL_LOCK       
*  Parameter (out) : --                                                         
*  Return value    : --                                                         
*  Description     : sets or clears the channel lock for a specified channel    
*******************************************************************************/
static void Spi_lclSetOrClearChannelLock(Spi_ChannelType Channel, uint8 Cmd)
{
    uint16 Index;
    uint8  ShiftVal;
    
    /* get the index where the lock status for this channel
       is set in the ChannelLock array */ 
    Index = Channel >> 3;
    ShiftVal = 0x7;

    /* check for the command. If it is set or clear channel lock */
    if (SPI_SET_CHANNEL_LOCK == Cmd)
    {
        ShiftVal  =  Channel & ShiftVal;
        SpiGlobal.IntBuf.ChannelLock[Index] |= (1u <<  ShiftVal);
    }
    else if (SPI_CLEAR_CHANNEL_LOCK == Cmd)
    {
        SpiGlobal.IntBuf.ChannelLock[Index] &= (~(1u << (Channel & ShiftVal)));
    }
    else
    {
    	/*compliant to misra-c*/
    }
}
#endif

/*******************************************************************************
*  Service name    : Spi_lclProcessJob                                          
*  Syntax          : Std_ReturnType Spi_lclProcessJob(Spi_JobType Job)          
*  Parameters (in) : Job - Job ID                                               
*  Parameter (out) : --                                                         
*  Return value    : Std_ReturnTypes                                            
*  Description     : Processes a specified Job.                                 
*******************************************************************************/
static Std_ReturnType Spi_lclProcessJob(Spi_JobType Job)
{
    const Spi_ExternalDeviceType *ExternalDevPtr;
    Spi_ChannelType              ChannelIdx;
    uint32                       HwIdx;
    uint32                       TempCtrlr0Reg;

    Std_ReturnType RetVal                 = E_OK;    
    const Spi_JobConfigType *JobConfigPtr = &SpiGlobal.ConfigPtr->SpiJobConfig[Job];
    
    /* Set job status to SPI_JOB_PENDING , hardware unit status to SPI_BUSY */
    SpiGlobal.JobUnit[Job].JobResult = SPI_JOB_PENDING;
    ExternalDevPtr                   = JobConfigPtr->SpiDeviceAssignment;
    HwIdx                            = ExternalDevPtr->SpiHwUnit;
    SpiGlobal.HwInfo[HwIdx].Status   = SPI_BUSY;
    SpiGlobal.SWBuffer.SpiBuffIdx    = 0;
    
    if (!((ExternalDevPtr->SpiDeviceType == QSPI_DEVICE) && 
          (JobConfigPtr->SpiTransferMode == READ_ONLY)))
    {
        /* update the software TX buffer with TX data. */
        for (ChannelIdx = 0; JobConfigPtr->SpiChannelAssignment[ChannelIdx] != \
                                           SPI_CHANNEL_END_INDICATOR; ChannelIdx++)
        {
            RetVal = Spi_lclUpdateTxSWBuff(JobConfigPtr->SpiChannelAssignment[ChannelIdx],
                                       JobConfigPtr->SpiDataWidth);
            if (RetVal == E_NOT_OK)
            {
                SpiGlobal.HwInfo[HwIdx].Status = SPI_IDLE;
                return RetVal;
            }
        }
    }
    
    /* Do hardware initialization */
    /* poll until and SPI is free */
    while (((Spi_Hw_Reg[HwIdx]->SR) & 1) != 0)
	{
    	;
	}

    /* disable the module */
    Spi_Hw_Reg[HwIdx]->SSIENR = 0;
    while (Spi_Hw_Reg[HwIdx]->SSIENR != 0)
    {
    	;
    }

    /* set to control register to default value */
    Spi_Hw_Reg[HwIdx]->CTRLR0 = 0x0070000;
    
    /* set the clock phase */
    Spi_Hw_Reg[HwIdx]->CTRLR0 |= ((ExternalDevPtr->SpiDataShiftEdge == \
                                       SPI_EDGE_LEADING) ? 0 : 1u) << 6u;
    /* set the clock polarity */
    Spi_Hw_Reg[HwIdx]->CTRLR0 |= ((ExternalDevPtr->SpiShiftClockIdleLevel == \
                                                STD_LOW) ? 0 : 1u) << 7u;
    
    /* set the baud rate */
    Spi_Hw_Reg[HwIdx]->BAUDR = (SpiGlobal.SpiClk/ExternalDevPtr->SpiBaudrate);

    /* set Rx SAMPLE Delay */
    Spi_Hw_Reg[HwIdx]->RX_SAMPLE_DLY = ExternalDevPtr->SpiTimeCs2Clk;

    /* write the data width */
    /* store the ctrlr0 register in Temp variable */
    TempCtrlr0Reg = Spi_Hw_Reg[HwIdx]->CTRLR0;
    TempCtrlr0Reg &= ~(0x7F0000u);                       /* clear the dfs field first */

    if (ExternalDevPtr->SpiDeviceType == QSPI_DEVICE)
    {
        if (JobConfigPtr->SpiFrameFormat == QUAD_SPI)
        {
            TempCtrlr0Reg |= ((uint32)0x2 << 21u);       /* Set the Quad SPI Format */
        }
        else
        {}
        if (JobConfigPtr->SpiTransferMode == WRITE_READ)
        {
            TempCtrlr0Reg |= ((uint32)0x0 << 8);         /* Set WriteNRead mode in TMOD bit */
        }
        else if (JobConfigPtr->SpiTransferMode == WRITE_ONLY)
        {
            TempCtrlr0Reg |= ((uint32)0x1 << 8);         /* Set Write Only mode in TMOD bit */
        }
        else if (JobConfigPtr->SpiTransferMode == READ_ONLY)
        {
            TempCtrlr0Reg |= ((uint32)0x3 << 8);         /* Set EEPROM Read mode in TMOD bit */
        }
        else
        {
        	/*compliant to misra-c*/
        }
    }

    TempCtrlr0Reg |= ((uint32)(JobConfigPtr->SpiDataWidth-1) << 16u);   /* write the data width. */
    Spi_Hw_Reg[HwIdx]->CTRLR0 = TempCtrlr0Reg;

    TempCtrlr0Reg = 0;
    if (ExternalDevPtr->SpiDeviceType == QSPI_DEVICE)
    {
        /* store the spi_ctrlr0 register in Temp variable */
        TempCtrlr0Reg = Spi_Hw_Reg[HwIdx]->SPI_CTRLR0;

        /* clear the WaitCycles, instruction length, address length, and transfer type fields. */
        TempCtrlr0Reg &= ~(0xFB3Fu);

        TempCtrlr0Reg |= ((uint32)JobConfigPtr->SpiWaitCycles << 11u);       /* Set the WaitCycles */
        TempCtrlr0Reg |= ((uint32)JobConfigPtr->SpiInstructionLength << 8u); /* Set the Instruction Length */
        TempCtrlr0Reg |= ((uint32)JobConfigPtr->SpiAddressLength << 2u);     /* Set the Address Length */
        TempCtrlr0Reg |= ((uint32)JobConfigPtr->SpiTransferType << 0u);      /* Set the SPI Mode to send both instruction and address length. */
        
        /* store the Temp variable back to spi_ctrlr0 register */
        Spi_Hw_Reg[HwIdx]->SPI_CTRLR0 = TempCtrlr0Reg;
    }

    /* enable the SPI module */
    Spi_Hw_Reg[HwIdx]->SSIENR = 1;
    while (Spi_Hw_Reg[HwIdx]->SSIENR != 1)
    {
    	;
    }

    if (ExternalDevPtr->SpiDeviceType != QSPI_DEVICE)
    {
        /* set the chips elect identifier */
        Spi_Hw_Reg[HwIdx]->SER = (1 << ExternalDevPtr->SpiCsIdentifier);
        /* transmit data */
        //Spi_lclTransmit(HwIdx,Job);
        Spi_lclTransmit(HwIdx);
        /* reset the chip select identifier */
        Spi_Hw_Reg[HwIdx]->SER &= (~(1u << ExternalDevPtr->SpiCsIdentifier));
    }
    else
    {
        /* transmit data */
        Qspi_lclTransmit(HwIdx, Job);
    }

    if (!((ExternalDevPtr->SpiDeviceType == QSPI_DEVICE) && 
          (JobConfigPtr->SpiTransferMode == WRITE_ONLY)))
    {
        /* update the software receive buffer with received  data. */
        SpiGlobal.SWBuffer.SpiBuffIdx = 0;
        for (ChannelIdx = 0; JobConfigPtr->SpiChannelAssignment[ChannelIdx] != \
                                               SPI_CHANNEL_END_INDICATOR; ChannelIdx++)
        {
            Spi_lclReadRxSWBuff(JobConfigPtr->SpiChannelAssignment[ChannelIdx],
                                JobConfigPtr->SpiDataWidth);
        }
    }

    /* set to job status to SPI_JOB_OK and hardware unit status to SPI_IDLE */
    SpiGlobal.JobUnit[Job].JobResult = SPI_JOB_OK;
    SpiGlobal.HwInfo[HwIdx].Status = SPI_IDLE;
    
    if (JobConfigPtr->SpiJobEndNotification != NULL_PTR)
    {
        JobConfigPtr->SpiJobEndNotification();
    }

    return RetVal;
}

/*********************************************************************************
*  Service name    : Spi_lclUpdateTxSWBuff                                        
*  Syntax          : Std_ReturnType Spi_lclUpdateTxSWBuff(Spi_ChannelType Channel,
*                                                          uint32 SpiDataWidth)   
*  Parameters (in) : Channel - Channel ID                                         
*                    SpiDataWidth - The data width used for this channel.
*  Parameter (out) : --                                                           
*  Return value    : Std_ReturnTypes                                              
*  Description     : Updates the software TX buffer with the channel data.
**********************************************************************************/
static Std_ReturnType Spi_lclUpdateTxSWBuff(Spi_ChannelType Channel, uint8 SpiDataWidth)
{
    const Spi_ChannelConfigType *ChannelConfigPtr;

    Std_ReturnType RetVal      = E_OK;
    const uint8 *SrcDataPtr8   = NULL_PTR; 
    const uint16 *SrcDataPtr16 = NULL_PTR;
    const uint32 *SrcDataPtr32 = NULL_PTR;
    uint16 DataLenOfChannel    = 0;
    uint16 TxLen               = 0;
    uint32 TxData              = 0;
    
    ChannelConfigPtr = &SpiGlobal.ConfigPtr->SpiChannelConfig[Channel];
       
    /* get the buffer pointers */
    #if (SPI_CHANNEL_BUFFERS_ALLOWED != 1)
    if (ChannelConfigPtr->SpiChannelType == SPI_IB)
    {
        uint16 SrcStartIndex;
        SrcStartIndex = (SpiGlobal.IntBuf.IBBufferOffset[Channel]);
        if (SpiDataWidth > 8)
        {
            if (SpiDataWidth > 16)
            {
                if (SpiGlobal.IntBuf.NullParamStat[Channel] == FALSE)
                {
                    /* PRQA S 3305 ++*/ 
                    SrcDataPtr32 = (const uint32 *)(&SpiGlobal.IntBuf.IBBufferPtr[SrcStartIndex]); /* Need this 3305*/
                }
                DataLenOfChannel = (ChannelConfigPtr->SpiIbNBuffers / 4);
            }
            else
            {
                if (SpiGlobal.IntBuf.NullParamStat[Channel] == FALSE)
                {
                    SrcDataPtr16 = (const uint16 *)(&SpiGlobal.IntBuf.IBBufferPtr[SrcStartIndex]);
                }
                DataLenOfChannel = (ChannelConfigPtr->SpiIbNBuffers / 2);
            }
        }
        else
        {
            if (SpiGlobal.IntBuf.NullParamStat[Channel] == FALSE)
            {
                SrcDataPtr8 = (&SpiGlobal.IntBuf.IBBufferPtr[SrcStartIndex]);
            }
            DataLenOfChannel = ChannelConfigPtr->SpiIbNBuffers;
        }
    }
    #endif
    #if (SPI_CHANNEL_BUFFERS_ALLOWED != 0)
    if (ChannelConfigPtr->SpiChannelType == SPI_EB)
    {
        uint16 EBChannelIndex;
        EBChannelIndex = Channel - SpiGlobal.NoOfIBChannels;
        if (SpiGlobal.ExtBuf[EBChannelIndex].Active == 0)
        {
            return E_NOT_OK;
        }
        DataLenOfChannel = SpiGlobal.ExtBuf[EBChannelIndex].Length;
        if (SpiDataWidth > 8)
        {
            if (SpiDataWidth > 16)
            {
                SrcDataPtr32 = (const uint32 *)(SpiGlobal.ExtBuf[EBChannelIndex].Src);
            }
            else
            {
                SrcDataPtr16 = (const uint16 *)(SpiGlobal.ExtBuf[EBChannelIndex].Src);
            }
        }
        else
        {
            SrcDataPtr8 = SpiGlobal.ExtBuf[EBChannelIndex].Src;
        }
    }
    #endif
    while (TxLen != DataLenOfChannel)
    {        
        if (SpiDataWidth > 8)
        {
            if (SpiDataWidth > 16)
            {
                if (SrcDataPtr32 == NULL_PTR)
                {
                    TxData = ChannelConfigPtr->SpiDefaultData;
                }
                else
                {
                    TxData = (*SrcDataPtr32);
                    TxData = Spi_lclByteSwap(TxData, SpiDataWidth);
                    SrcDataPtr32++;
                }
            }
            else
            {
                if (SrcDataPtr16 == NULL_PTR)
                {
                    TxData = ChannelConfigPtr->SpiDefaultData;
                }
                else
                {
                    TxData = (uint32) (*SrcDataPtr16);
                    TxData = Spi_lclByteSwap(TxData, SpiDataWidth);
                    SrcDataPtr16++;
                }
            }
        }
        else
        {
            if (SrcDataPtr8 == NULL_PTR)
            {
                TxData = ChannelConfigPtr->SpiDefaultData;
            }
            else
            {
                TxData = (uint32) (*SrcDataPtr8);
                SrcDataPtr8++;
            }
        }

        if (ChannelConfigPtr->SpiTransferStart == SPI_TRANSFER_START_LSB)
        {
            TxData = Spi_lclRotateBits(TxData, SpiDataWidth);
        }

        /* write into software TX buffer */
        SpiGlobal.SWBuffer.SpiTxSWBuff[SpiGlobal.SWBuffer.SpiBuffIdx] = TxData;

        TxLen++;
        SpiGlobal.SWBuffer.SpiBuffIdx++;
    }

    return RetVal;
}

/*********************************************************************************
*  Service name    : Spi_lclReadRxSWBuff                                        
*  Syntax          : void Spi_lclReadRxSWBuff(Spi_ChannelType Channel,
*                                               uint32 SpiDataWidth)
*  Parameters (in) : Channel - Channel ID                                         
*                    SpiDataWidth - The data width used for this channel.
*  Parameter (out) : --                                                           
*  Return value    : --                                              
*  Description     : Reads the data from the software receive buffer and updates the
*                    internal/external buffer based on configuration of the channel.
**********************************************************************************/
static void Spi_lclReadRxSWBuff(Spi_ChannelType Channel, uint8 SpiDataWidth)
{
    const Spi_ChannelConfigType *ChannelConfigPtr;

    uint16 DataLenOfChannel = 0;
    uint8 *DestDataPtr8     = NULL_PTR;
    uint16 *DestDataPtr16   = NULL_PTR;
    uint32 *DestDataPtr32   = NULL_PTR;
    uint32 RxData           = 0;
    uint16 RxLen            = 0;

    /* get the channel configuration pointer */
    ChannelConfigPtr = &SpiGlobal.ConfigPtr->SpiChannelConfig[Channel];
    
    /* get the buffer pointers */
    #if (SPI_CHANNEL_BUFFERS_ALLOWED != 1)
    if (ChannelConfigPtr->SpiChannelType == SPI_IB)
    {
        uint16 DestStartIndex;
        DestStartIndex = (SpiGlobal.IntBuf.IBBufferOffset[Channel] \
                                                + SPI_MAXIMUM_IB_BUFFER_SIZE);
        if (SpiDataWidth > 8)
        {
            if (SpiDataWidth > 16)
            {
                DestDataPtr32 = (uint32 *)(&SpiGlobal.IntBuf.IBBufferPtr[DestStartIndex]);
                DataLenOfChannel = (ChannelConfigPtr->SpiIbNBuffers / 4);
            }
            else
            {
                DestDataPtr16 = (uint16 *)(&SpiGlobal.IntBuf.IBBufferPtr[DestStartIndex]);
                DataLenOfChannel = (ChannelConfigPtr->SpiIbNBuffers / 2);
            }
        }
        else
        {
            DestDataPtr8 = (&SpiGlobal.IntBuf.IBBufferPtr[DestStartIndex]);
            DataLenOfChannel = ChannelConfigPtr->SpiIbNBuffers;
        }
    }
    #endif
    #if (SPI_CHANNEL_BUFFERS_ALLOWED != 0)
    if (ChannelConfigPtr->SpiChannelType == SPI_EB)
    {
        uint16 EBChannelIndex;
        EBChannelIndex = Channel - SpiGlobal.NoOfIBChannels;
        DataLenOfChannel = SpiGlobal.ExtBuf[EBChannelIndex].Length;
        if (SpiDataWidth > 8)
        {
            if (SpiDataWidth > 16)
            {
                DestDataPtr32 = (uint32 *)(SpiGlobal.ExtBuf[EBChannelIndex].Dest);
            }
            else
            {
                DestDataPtr16 = (uint16 *)(SpiGlobal.ExtBuf[EBChannelIndex].Dest);
            }
        }
        else
        {
            DestDataPtr8 = SpiGlobal.ExtBuf[EBChannelIndex].Dest;
        }
    }
    #endif
    
    while (RxLen != DataLenOfChannel)
    {
        RxData = SpiGlobal.SWBuffer.SpiRxSWBuff[SpiGlobal.SWBuffer.SpiBuffIdx];
        if (SpiDataWidth > 8)
        {
            if(SpiDataWidth > 16) 
            {
                if (DestDataPtr32 != NULL_PTR)
                {
                    RxData = Spi_lclByteSwap(RxData, SpiDataWidth);
                    *DestDataPtr32 = RxData;
                    DestDataPtr32++;
                }
            }
            else
            {
                if (DestDataPtr16 != NULL_PTR)
                {
                    RxData = Spi_lclByteSwap(RxData, SpiDataWidth);
                    *DestDataPtr16 = (uint16)RxData;
                    DestDataPtr16++;
                }
            }
        }
        else
        {
            if (DestDataPtr8 != NULL_PTR)
            {
                *DestDataPtr8 = (uint8)RxData;
                DestDataPtr8++;
            }
        }
        RxLen++;
        SpiGlobal.SWBuffer.SpiBuffIdx++;
    }
}

/*******************************************************************************
*  Service name    : Spi_lclTransmit                                           
*  Syntax          : void Spi_lclTransmit(uint32 HwIdx , Spi_JobType Job)
*  Parameters (in) : HwIdx -  The hardware unit used to send the data.
*  Parameter (out) : --                                                        
*  Return value    : --
*  Description     : Transmits the data.                                       
*******************************************************************************/
#if 0
static void Spi_lclTransmit(uint32 HwIdx , Spi_JobType Job)
{
    uint32 TxIdx = 0, RxIdx = 0;
    uint32 TempRxflrReg     = 0;
    uint32 TempTxflrReg     = 0;
    uint32 TempLoopCounter  = 0;
    uint16 InstData         = 0;
    uint32 AddrData         = 0;
    uint8  i 				= 0;
    uint8  Temp				= 0;
    uint32 DiscardedData    = 0;

    const Spi_JobConfigType *JobConfigPtr = &SpiGlobal.ConfigPtr->SpiJobConfig[Job];
    const Spi_ExternalDeviceType *ExternalDevPtr = JobConfigPtr->SpiDeviceAssignment;

    /* Writing Instruction in the local variable */
    InstData = JobConfigPtr->SpiInstruction;
    /* Writing Address in the local variable */
    AddrData = JobConfigPtr->SpiAddress;

	if (JobConfigPtr->SpiAddressLength != 0)
	{
		Temp = (4u * JobConfigPtr->SpiAddressLength)/(JobConfigPtr->SpiDataWidth);
		/* Writing Address in the FIFO */
		while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0){;}

		Spi_Hw_Reg[HwIdx]->DR = InstData;
		for(i=0; i<Temp ;i++)
		{
			Spi_Hw_Reg[HwIdx]->DR = (AddrData >>(JobConfigPtr->SpiDataWidth*(Temp-i-1u)));
		}
	}

	Spi_Hw_Reg[HwIdx]->SER = (1 << ExternalDevPtr->SpiCsIdentifier);

    while (TxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
    {
        /* poll if TX FIFO is full */
        while (((Spi_Hw_Reg[HwIdx]->SR) & (2)) != 2)
        {
        	;
        }

        Spi_Hw_Reg[HwIdx]->DR = SpiGlobal.SWBuffer.SpiTxSWBuff[TxIdx];
        TxIdx++;
        
        if (JobConfigPtr->SpiTransferMode == WRITE_ONLY)
        {
        	//nothing to do
        }
        else
        {
			/* receive data if RX FIFO has more than 25 entries to avoid overflow */
			TempRxflrReg = Spi_Hw_Reg[HwIdx]->RXFLR;
			if (TempRxflrReg >= SPI_SW_RX_FIFO_THRESHOLD)
			{
				TempLoopCounter = 0;
				/* copy data units of RX FIFO */
				while (TempLoopCounter != TempRxflrReg)
				{
					TempTxflrReg = Spi_Hw_Reg[HwIdx]->TXFLR;
					if (TempTxflrReg <= SPI_SW_TX_FIFO_THRESHOLD)
					{
						break;
					}
					SpiGlobal.SWBuffer.SpiRxSWBuff[RxIdx] = Spi_Hw_Reg[HwIdx]->DR;
					RxIdx++;
					TempLoopCounter++;
				}
			}
        }
    }
    /* wait till TX FIFO gets empty or till device busy! */
    while (((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_EMPTY) == 0) || (Spi_Hw_Reg[HwIdx]->SR & SPI_DEVICE_BUSY)){ ; }


    /* Clear the RX FIFO */
    if (JobConfigPtr->SpiTransferMode == WRITE_ONLY)
    {
		while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY))
		{
			DiscardedData = Spi_Hw_Reg[HwIdx]->DR;
		}
    }
    else		/* receive the remaining data */
    {
		while (RxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
		{
			TempRxflrReg = Spi_Hw_Reg[HwIdx]->RXFLR;
			if (TempRxflrReg != 0)
			{
				SpiGlobal.SWBuffer.SpiRxSWBuff[RxIdx] = Spi_Hw_Reg[HwIdx]->DR;
				RxIdx++;
			}
		}
    }

    /* reset the chip select identifier */
    Spi_Hw_Reg[HwIdx]->SER &= (~(1u << ExternalDevPtr->SpiCsIdentifier));
}
#endif

static void Spi_lclTransmit(uint32 HwIdx)
{
    uint32 TxIdx = 0, RxIdx = 0;
    uint32 TempRxflrReg     = 0;
    uint32 TempTxflrReg     = 0;
    uint32 TempLoopCounter  = 0;

    while (TxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
    {
        /* poll if TX FIFO is full */
        while (((Spi_Hw_Reg[HwIdx]->SR) & (2)) != 2);

        Spi_Hw_Reg[HwIdx]->DR = SpiGlobal.SWBuffer.SpiTxSWBuff[TxIdx];
        TxIdx++;

        /* receive data if RX FIFO has more than 25 entries to avoid overflow */
        TempRxflrReg = Spi_Hw_Reg[HwIdx]->RXFLR;
        if (TempRxflrReg >= SPI_SW_RX_FIFO_THRESHOLD)
        {
            TempLoopCounter = 0;
            /* copy data units of RX FIFO */
            while (TempLoopCounter != TempRxflrReg)
            {
                TempTxflrReg = Spi_Hw_Reg[HwIdx]->TXFLR;
                if (TempTxflrReg <= SPI_SW_TX_FIFO_THRESHOLD)
                {
                    break;
                }
                SpiGlobal.SWBuffer.SpiRxSWBuff[RxIdx] = Spi_Hw_Reg[HwIdx]->DR;
                RxIdx++;
                TempLoopCounter++;
            }
        }
    }

    /* receive the remaining data */
    while (RxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
    {
        TempRxflrReg = Spi_Hw_Reg[HwIdx]->RXFLR;
        if (TempRxflrReg != 0)
        {
            SpiGlobal.SWBuffer.SpiRxSWBuff[RxIdx] = Spi_Hw_Reg[HwIdx]->DR;
            RxIdx++;
        }
    }
}
/******************************************************************************
*  Service name    : Qspi_lclTransmit                                          
*  Syntax          : void Qspi_lclTransmit(uint32 HwIdx, Spi_JobType Job)
*  Parameters (in) : HwIdx -  The hardware unit used to send the data.
*  Parameters (in) : Job -  The JobID to be processed.
*  Parameter (out) : --                                                        
*  Return value    : --
*  Description     : Transmits the data.                                       
******************************************************************************/
static void Qspi_lclTransmit(uint32 HwIdx, Spi_JobType Job)
{
    uint32 TxIdx = 0, RxIdx = 0;
    uint32 RxLoopVar        = 0;
    uint32 OneReadCount     = 32u;
    uint32 ReadCount        = 0;
    uint16 InstData         = 0;
    uint32 AddrData         = 0;
    uint32 OriAddrData      = 0;
    uint32 DiscardedData    = 0;
    uint32 WaitCycle        = 0;

    const Spi_JobConfigType *JobConfigPtr        = &SpiGlobal.ConfigPtr->SpiJobConfig[Job];
    const Spi_ExternalDeviceType *ExternalDevPtr = JobConfigPtr->SpiDeviceAssignment;

    /* Writing Instruction in the local variable */
    InstData = JobConfigPtr->SpiInstruction;
    /* Writing Address in the local variable */
    AddrData = JobConfigPtr->SpiAddress;

    if (JobConfigPtr->SpiTransferMode == WRITE_READ)
    {
        uint32 WRTxIdx = 0, WRRxIdx = 0;
        uint32 DiscardedDataLen = 1;

        if (JobConfigPtr->SpiCommandType == READ_CMD)
        {
            /* Clear the RX FIFO */
            while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY))
            {
                DiscardedData = Spi_Hw_Reg[HwIdx]->DR;
            }
        }

        /* Writing Instruction in the FIFO */
        while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0)
        {
        	;
        }
        Spi_Hw_Reg[HwIdx]->DR = InstData;

        if (JobConfigPtr->SpiAddressLength != 0)
        {
            /* Writing Address in the FIFO */
            while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0)
            {
            	;
            }
            Spi_Hw_Reg[HwIdx]->DR = AddrData;
            DiscardedDataLen +=3;
        }

        if (JobConfigPtr->SpiCommandType == READ_CMD)
        {
            WaitCycle = JobConfigPtr->SpiWaitCycles/8;
            DiscardedDataLen += WaitCycle;

            while (WaitCycle)
            {
                while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0)
                {
                	;
                }
                Spi_Hw_Reg[HwIdx]->DR = 0x0;
                WaitCycle--;
            }
        }
        
        if (JobConfigPtr->SpiCommandType != CMD_ONLY)
        {
            while ((Spi_Hw_Reg[HwIdx]->SR) & (SPI_TX_FIFO_NOT_FULL))
            {
                /* poll if TX FIFO is full */
                if (WRTxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
                {
                    Spi_Hw_Reg[HwIdx]->DR = SpiGlobal.SWBuffer.SpiTxSWBuff[WRTxIdx];
                    WRTxIdx++;
                }
                else
                {
                    break;
                }
            }
        }

        /* set the chip select identifier */
        Spi_Hw_Reg[HwIdx]->SER = (1 << ExternalDevPtr->SpiCsIdentifier);

        if (JobConfigPtr->SpiCommandType == READ_CMD)
        {
            /* remove unused data. */
            while (DiscardedDataLen)
            {
                while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY) == 0)
                {
                	;
                }
                DiscardedData = Spi_Hw_Reg[HwIdx]->DR;
                Spi_Hw_Reg[HwIdx]->DR = 0;
                DiscardedDataLen--;
            }

            while (WRTxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
            {
                while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY) == 0)
                {
                	;
                }
                SpiGlobal.SWBuffer.SpiRxSWBuff[WRRxIdx] = Spi_Hw_Reg[HwIdx]->DR;
                WRRxIdx++;
                Spi_Hw_Reg[HwIdx]->DR = SpiGlobal.SWBuffer.SpiTxSWBuff[WRTxIdx];
                WRTxIdx++;
            }
            while (WRRxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
            {
                while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY) == 0)
                {
                	;
                }
                SpiGlobal.SWBuffer.SpiRxSWBuff[WRRxIdx] = Spi_Hw_Reg[HwIdx]->DR;
                WRRxIdx++;
            }
        }

        if (JobConfigPtr->SpiCommandType == WRITE_CMD)
        {
            while (WRTxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
            {
                while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY) == 0)
                {
                	;
                }
                DiscardedData = Spi_Hw_Reg[HwIdx]->DR;
                while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0)
                {
                	;
                }
                Spi_Hw_Reg[HwIdx]->DR = SpiGlobal.SWBuffer.SpiTxSWBuff[WRTxIdx];
            }
        }

        /* wait till TX FIFO gets empty or till device busy */
        while (((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_EMPTY) == 0) || ((Spi_Hw_Reg[HwIdx]->SR & SPI_DEVICE_BUSY) == 0x1 ))
        {
        	;
        }

        /* reset the chip select identifier */
        Spi_Hw_Reg[HwIdx]->SER &= (~(1u << ExternalDevPtr->SpiCsIdentifier));

        /* Clear the RX FIFO */
        while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY))
        {
            DiscardedData = Spi_Hw_Reg[HwIdx]->DR;
        }
    }

    if (JobConfigPtr->SpiTransferMode == WRITE_ONLY)
    {
        /* Writing Instruction in the FIFO */
        while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0)
        {
        	;
        }
        Spi_Hw_Reg[HwIdx]->DR = InstData;

        if (JobConfigPtr->SpiAddressLength != 0)
        {
            /* Writing Address in the FIFO */
            while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0)
            {
            	;
            }
            Spi_Hw_Reg[HwIdx]->DR = AddrData ;
        }

        if (JobConfigPtr->SpiCommandType != CMD_ONLY)
        {
            while ((Spi_Hw_Reg[HwIdx]->SR) & (SPI_TX_FIFO_NOT_FULL))
            {
                /* poll if TX FIFO is full */
                if (TxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
                {
                    Spi_Hw_Reg[HwIdx]->DR = SpiGlobal.SWBuffer.SpiTxSWBuff[TxIdx];
                    TxIdx++;
                }
                else 
                {
                    break;
                }
            }
         }

        /* set the chip select identifier */
        Spi_Hw_Reg[HwIdx]->SER = (1 << ExternalDevPtr->SpiCsIdentifier);
        
        if (JobConfigPtr->SpiCommandType != CMD_ONLY)
        {
            while (TxIdx != SpiGlobal.SWBuffer.SpiBuffIdx)
            {
                while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0)
                {
                	;
                }
                Spi_Hw_Reg[HwIdx]->DR = SpiGlobal.SWBuffer.SpiTxSWBuff[TxIdx];
                TxIdx++;
            }
        }

        /* wait till TX FIFO gets empty or till device busy! */
        while (((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_EMPTY) == 0) || (Spi_Hw_Reg[HwIdx]->SR & SPI_DEVICE_BUSY))
        {
        	;
        }

        /* Clear the RX FIFO */
        while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY))
        {
            DiscardedData = Spi_Hw_Reg[HwIdx]->DR;
        }

        /* reset the chip select identifier */
        Spi_Hw_Reg[HwIdx]->SER &= (~(1u << ExternalDevPtr->SpiCsIdentifier));
    }

    if (JobConfigPtr->SpiTransferMode == READ_ONLY)
    {
        /* Clear the RX FIFO */
        while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY))
        {
            DiscardedData = Spi_Hw_Reg[HwIdx]->DR;
        }

       /* Writing Address in the local variable */
        OriAddrData = JobConfigPtr->SpiAddress;

        ReadCount = JobConfigPtr->SpiReadCount;
        while (ReadCount)
        {
            if (ReadCount < OneReadCount)
            {
                OneReadCount = ReadCount;
            }

            /* disable the module */
            Spi_Hw_Reg[HwIdx]->SSIENR = 0;
            while (Spi_Hw_Reg[HwIdx]->SSIENR != 0)
            {
            	;
            }

            /* Set number of frames to be read */
            Spi_Hw_Reg[HwIdx]->CTRLR1 = OneReadCount - 1;
            
            /* enable the SPI module */
            Spi_Hw_Reg[HwIdx]->SSIENR = 1;
            while (Spi_Hw_Reg[HwIdx]->SSIENR != 1)
            {
            	;
            }

            /* Writing Instruction in the local variable */
            InstData = JobConfigPtr->SpiInstruction;
            AddrData = OriAddrData;

            /* Writing Instruction in the FIFO */
            while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0)
            {
            	;
            }
            Spi_Hw_Reg[HwIdx]->DR = InstData;

            if (JobConfigPtr->SpiAddressLength != 0)
            {
                /* Writing Address in the FIFO */
                while ((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_NOT_FULL) == 0)
                {
                	;
                }
                Spi_Hw_Reg[HwIdx]->DR = AddrData;
            }

            RxLoopVar = OneReadCount;

            /* set the chip select identifier */
            Spi_Hw_Reg[HwIdx]->SER = (1 << ExternalDevPtr->SpiCsIdentifier);

            while (RxLoopVar)
            {
                while((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY) == 0)
                {
                	;
                }
                SpiGlobal.SWBuffer.SpiRxSWBuff[RxIdx] = Spi_Hw_Reg[HwIdx]->DR;
                RxIdx++;
                RxLoopVar--;
            }
            OriAddrData += (OneReadCount * (JobConfigPtr->SpiDataWidth/8));
            ReadCount = ReadCount - OneReadCount;

            /* wait till TX FIFO gets empty or till device busy */
            while (((Spi_Hw_Reg[HwIdx]->SR & SPI_TX_FIFO_EMPTY) == 0) || (Spi_Hw_Reg[HwIdx]->SR & SPI_DEVICE_BUSY))
			{
            	;
			}

            /* reset the chip select identifier */
            Spi_Hw_Reg[HwIdx]->SER &= (~(1u << ExternalDevPtr->SpiCsIdentifier));
        }
        
        /* Clear the RX FIFO */
        while ((Spi_Hw_Reg[HwIdx]->SR & SPI_RX_FIFO_NOT_EMPTY))
        {
            DiscardedData = Spi_Hw_Reg[HwIdx]->DR;
        }
    }
}

/*******************************************************************************
*  Service name    : Spi_lclByteSwap                                            
*  Syntax          : uint32 Spi_lclByteSwap(uint32 Data, uint8 DataWidth)       
*  Parameters (in) : Data - the data whose byte is to be swapped  
*                  : DataWidth - the length to be rotated.                      
*  Parameter (out) : --                                                         
*  Return value    : Swapped data                                               
*  Description     : Takes the data and swaps byte.                             
*******************************************************************************/
static uint32 Spi_lclByteSwap(uint32 Data, uint8 DataWidth)
{
    uint32 Value = Data;

    if (DataWidth > 16)
    {
        Value = ((Value & 0x000000FF) << 24) |
                ((Value & 0x0000FF00) <<  8) |
                ((Value & 0x00FF0000) >>  8) |
                ((Value & 0xFF000000) >> 24);
    }
    else
    {
        Value = ((Value & 0x00FF) << 8) | ((Value & 0xFF00) >> 8);
    }

    return Value;
}

/*******************************************************************************
*  Service name    : Spi_lclRotateBits                                          
*  Syntax          : uint32 Spi_lclRotateBits(uint32 Data , uint8 Len)          
*  Parameters (in) : Data - the data whose bits are to be rotated,              
*                  : Len - the length to be rotated.                            
*  Parameter (out) : --                                                         
*  Return value    : Data with rotated bits                                      
*  Description     : Takes the data and rotates the bits, used in LSB-first transfer.
*******************************************************************************/
static uint32 Spi_lclRotateBits(uint32 Data , uint8 Len)
{
    uint32 Res     = 0;
    uint8 TempData = 0;
    
    for (uint8 Idx = 0; Idx < Len; Idx++)
    {
        TempData = (uint8)((Data >> Idx) & 1);
        Res |= TempData << (Len-Idx-1);
    }
    return Res;
}

#if (SPI_LEVEL_DELIVERED == SPI_LEVEL_1) || (SPI_LEVEL_DELIVERED == SPI_LEVEL_2)
/*******************************************************************************
*  Service name    : Spi_lclCheckQueueAvailability                              
*  Syntax          : Std_ReturnType Spi_lclCheckQueueAvailability(uint16 NumOfJobs)
*  Parameters (in) : NumOfJobs - the number of jobs spaces to be empty          
*  Parameter (out) : --                                                         
*  Return value    : Std_ReturnType                                             
*  Description     : Checks if the queue has enough space to fit the NumOfJobs  
*******************************************************************************/
static Std_ReturnType Spi_lclCheckQueueAvailability(uint16 NumOfJobs)
{
    Std_ReturnType RetVal;
    uint16         StartIdx;
    uint16         EndIdx;
    uint16         AvailableSlots;
    
    StartIdx = SpiGlobal.Queue.JobStartIndex;
    EndIdx   = SpiGlobal.Queue.JobEndIndex;

    if (StartIdx == EndIdx)
    {
        AvailableSlots = SPI_MAX_JOB_Q_LENGTH;
    }
    else if (StartIdx < EndIdx)
    {
        AvailableSlots = (SPI_MAX_JOB_Q_LENGTH - EndIdx) + (StartIdx - 1);
    }
    else
    {
        AvailableSlots = StartIdx - EndIdx;
        AvailableSlots = AvailableSlots - 1;
    }

    if (AvailableSlots > NumOfJobs)
    {
        RetVal = E_OK;
    }
    else
    {
        RetVal = E_NOT_OK;
    }
    return RetVal;
}

/*******************************************************************************
*  Service name    : Spi_lclGetNextQueueIdx                                     
*  Syntax          : uint16 Spi_lclGetNextQueueIdx(uint16 Index)                
*  Parameters (in) : Index - The present index                                  
*  Parameter (out) : --                                                         
*  Return value    : returns the Next Index                                     
*  Description     : Increments the present index and returns the next valid index.
*******************************************************************************/
static uint16 Spi_lclGetNextQueueIdx(uint16 Index)
{
    uint16 TempIndex = Index;
    
    TempIndex++;
    if (TempIndex == (SPI_MAX_JOB_Q_LENGTH))
    {
        TempIndex = 0;
    }
    return TempIndex;
}

/*********************************************************************************
*  Service name    : Spi_lclQueuePush                                             
*  Syntax          : void Spi_lclQueuePush(Spi_SequenceType Sequence,             
*                                          uint16 JobIdx)                        
*  Parameters (in) : Sequence - Sequence ID                                       
*                    JobIdx - The index in SpiJobAssignment from where the JobId 
*                              will be pushed to queue.                           
*  Parameter (out) : --                                                           
*  Return value    : --                                                           
*  Description     : Pushes the JobId and SeqId to queue.
**********************************************************************************/
static void Spi_lclQueuePush(Spi_SequenceType Sequence, uint16 JobIdx)
{
    const Spi_SequenceConfigType *SeqConfigPtr;

    uint16 QueueIdx     = SpiGlobal.Queue.JobEndIndex;
    uint16 TempJobIndex = JobIdx;

    SeqConfigPtr = &SpiGlobal.ConfigPtr->SpiSequenceConfig[Sequence];

    /* add jobs to Queue from the specified TempJobIndex */
    while (SeqConfigPtr->SpiJobAssignment[TempJobIndex] != SPI_JOB_END_INDICATOR)
    {
        SpiGlobal.Queue.JobQueue[QueueIdx] = SeqConfigPtr->SpiJobAssignment[TempJobIndex];
        SpiGlobal.Queue.JobLinkedSeq[QueueIdx] = Sequence;
        QueueIdx = Spi_lclGetNextQueueIdx(QueueIdx);
        TempJobIndex++;
    }
    SpiGlobal.Queue.JobEndIndex = QueueIdx;
}

/*******************************************************************************
*  Service name    : Spi_lclQueueCleanUp                                        
*  Syntax          : void Spi_lclQueueCleanUp(void)                             
*  Parameters (in) : --                                                         
*  Parameter (out) : --                                                         
*  Return value    : --                                                        
*  Description     : Cleans the queue, removes all failed or cancelled sequence
*                    and jobs from queue.
*******************************************************************************/
static void Spi_lclQueueCleanUp(void)
{
    uint16      TempQStartIdx;
    uint16      TempQNextIdx;
    uint16      TempQEndIdx;
    Spi_JobType JobId;

    uint16 QStartIdx = SpiGlobal.Queue.JobStartIndex;
    uint16 QEndIdx   = SpiGlobal.Queue.JobEndIndex;

    TempQEndIdx      = SpiGlobal.Queue.JobEndIndex;

    /* loops through the queue and removes the jobs and relevant
       sequence which are canceled or failed and rearranges the queue */
    while (QStartIdx != QEndIdx)
    {
        JobId = SpiGlobal.Queue.JobQueue[QStartIdx];
        if ((Spi_Hal_GetJobResult(JobId) == SPI_JOB_FAILED))
        {
            TempQStartIdx = QStartIdx;
            TempQNextIdx = QStartIdx;
            while (TempQNextIdx != QEndIdx)
            {
                TempQNextIdx = Spi_lclGetNextQueueIdx(TempQNextIdx);
                SpiGlobal.Queue.JobQueue[TempQStartIdx] = \
                                            SpiGlobal.Queue.JobQueue[TempQNextIdx];
                SpiGlobal.Queue.JobLinkedSeq[TempQStartIdx] = \
                                         SpiGlobal.Queue.JobLinkedSeq[TempQNextIdx];
                TempQEndIdx   = TempQStartIdx;
                TempQStartIdx = Spi_lclGetNextQueueIdx(TempQStartIdx);
            }

            /* set the end index with default values */
            SpiGlobal.Queue.JobQueue[TempQStartIdx]     = 0xFFFF;
            SpiGlobal.Queue.JobLinkedSeq[TempQStartIdx] = 0xFF;

            /* update the new end index */
            QEndIdx = TempQEndIdx;
        }
        else
        {
            QStartIdx = Spi_lclGetNextQueueIdx(QStartIdx);
        }
    }

    SpiGlobal.Queue.JobEndIndex = TempQEndIdx;
}

#if (SPI_INTERRUPTIBLE_SEQ_ALLOWED == STD_ON)
/**********************************************************************************
*  Service name    : Spi_lclRearrangeQueue                                      
*  Syntax          : void Spi_lclRearrangeQueue(Spi_SequenceType Sequence)      
*  Parameters (in) : Sequence - Sequence Id                                     
*  Parameter (out) : --                                                         
*  Return value    : --                                                         
*  Description     : Arranges the global queue based on interrupt priority of Jobs.
**********************************************************************************/
static void Spi_lclRearrangeQueue(Spi_SequenceType Sequence)
{
    const Spi_SequenceConfigType *SeqConfigPtr;
    uint16 QueueIdx, JobIdx, TempStoreIdx;
    uint16 JobIdxInQ, JobId, CurrIdx;
    uint8  QueuePrio, CurrJobPrio;
    uint16 TempQueueSize;
    uint16 Counter;
    uint16 TempJobQ[SPI_MAX_JOB_Q_LENGTH]          = {0};
    uint8  TempJobLinkedSeqQ[SPI_MAX_JOB_Q_LENGTH] = {0};

    SeqConfigPtr = &SpiGlobal.ConfigPtr->SpiSequenceConfig[Sequence];

    /* Move till the first interruptible sequence */
    QueueIdx = Spi_lclGotoIntSeq();
    if (QueueIdx == SpiGlobal.Queue.JobEndIndex)
    {
        /* have already reached the end of Queue, 
           just fill in the new jobs of sequence to queue*/
        Spi_lclQueuePush(Sequence,0);
    }
    else 
    {
        JobIdx = 0;
        TempStoreIdx = QueueIdx;
        if (SeqConfigPtr->SpiInterruptibleSequence == TRUE)
        {
            do {
                TempStoreIdx = QueueIdx;

                /* extract job id from the current sequence and its priority */
                JobId = SeqConfigPtr->SpiJobAssignment[JobIdx];
                CurrJobPrio = SpiGlobal.ConfigPtr->SpiJobConfig[JobId].SpiJobPriority;
                
                /* extract job id from queue and its priority */
                JobIdxInQ = SpiGlobal.Queue.JobQueue[QueueIdx];
                QueuePrio = SpiGlobal.ConfigPtr->SpiJobConfig[JobIdxInQ].SpiJobPriority;

                /* compare priorities */
                if (CurrJobPrio > QueuePrio)
                {
                    CurrIdx = QueueIdx;
                    /* store it in temp array */
                    TempQueueSize = Spi_lclStoreToTempQ(CurrIdx, 
                                                        &TempJobQ[0],
                                                        &TempJobLinkedSeqQ[0]);

                    /* Place the Job in Queue where priority is lower,denoted by TempStoreIdx */
                    SpiGlobal.Queue.JobQueue[TempStoreIdx]     = JobId;
                    SpiGlobal.Queue.JobLinkedSeq[TempStoreIdx] = Sequence;

                    /* Increment TempStoreIdx  */
                    TempStoreIdx = Spi_lclGetNextQueueIdx(TempStoreIdx);

                    /* from TempStoreIdx, fill the Queue with remaining jobs,
                       previously stored in a temp array */
                    CurrIdx = TempStoreIdx;
                    for (Counter = 0U; Counter < TempQueueSize; Counter++)
                    {
                        SpiGlobal.Queue.JobQueue[CurrIdx] = TempJobQ[Counter];
                        SpiGlobal.Queue.JobLinkedSeq[CurrIdx] = \
                                                TempJobLinkedSeqQ[Counter];
                        CurrIdx = Spi_lclGetNextQueueIdx(CurrIdx);
                    }
                    /* Move end index by 1 as one extra job is placed */
                    SpiGlobal.Queue.JobEndIndex = \
                                     Spi_lclGetNextQueueIdx(SpiGlobal.Queue.JobEndIndex);
                    JobIdx++;
                }
                else
                {
                    /* Increment the queue index  */
                    QueueIdx = Spi_lclGetNextQueueIdx(QueueIdx);
                }
            } while ((QueueIdx != SpiGlobal.Queue.JobEndIndex) && \
                     ((SeqConfigPtr->SpiJobAssignment[JobIdx]) != \
                       SPI_JOB_END_INDICATOR));

            /* Push the sequence jobs at the end of queue */
            Spi_lclQueuePush(Sequence,JobIdx);
        }
        else
        {
            CurrIdx = QueueIdx;
            TempQueueSize = Spi_lclStoreToTempQ(CurrIdx,
                                                &TempJobQ[0], 
                                                &TempJobLinkedSeqQ[0]);
            do {
                SpiGlobal.Queue.JobQueue[TempStoreIdx] = \
                                    SeqConfigPtr->SpiJobAssignment[JobIdx];
                SpiGlobal.Queue.JobLinkedSeq[TempStoreIdx] = Sequence;
                TempStoreIdx = Spi_lclGetNextQueueIdx(TempStoreIdx);
                JobIdx++;
                /* New entry to the Queue, increment jobEndIndex */
                SpiGlobal.Queue.JobEndIndex = Spi_lclGetNextQueueIdx(SpiGlobal.Queue.JobEndIndex);
            } while (SeqConfigPtr->SpiJobAssignment[JobIdx] != SPI_JOB_END_INDICATOR);
            
            CurrIdx = TempStoreIdx;
            for (Counter = 0U; Counter < TempQueueSize; Counter++)
            {
                SpiGlobal.Queue.JobQueue[CurrIdx]     = TempJobQ[Counter];
                SpiGlobal.Queue.JobLinkedSeq[CurrIdx] = TempJobLinkedSeqQ[Counter];
                CurrIdx = Spi_lclGetNextQueueIdx(CurrIdx);
            }
        }
    }
}

/**********************************************************************************
*  Service name    : Spi_lclGotoIntSeq                                             
*  Syntax          : uint16 Spi_lclGotoIntSeq(void)                                
*  Parameters (in) : --                                                            
*  Parameter (out) : --                                                            
*  Return value    : Queue index of the first interruptible  sequence              
*  Description     : Finds the first interruptible sequence index in queue and     
*                    returns the index .                                           
**********************************************************************************/
static uint16 Spi_lclGotoIntSeq(void)
{
    const Spi_SequenceConfigType *SeqConfigPtr;
    Spi_SequenceType             SeqId;

    uint16 QStartIdx = SpiGlobal.Queue.JobStartIndex;
    uint16 QEndIdx   = SpiGlobal.Queue.JobEndIndex;
    
    /* loop through the queue and find the first interruptible sequence in queue */
    while (QStartIdx != QEndIdx)
    {
        SeqId = SpiGlobal.Queue.JobLinkedSeq[QStartIdx];
        SeqConfigPtr = &SpiGlobal.ConfigPtr->SpiSequenceConfig[SeqId];
        if (SeqConfigPtr->SpiInterruptibleSequence == TRUE)
        {
            return QStartIdx;
        }
        QStartIdx = Spi_lclGetNextQueueIdx(QStartIdx);
    }
    return QStartIdx;
}

/**********************************************************************************
*  Service name     : Spi_lclStoreToTempQ
*  Syntax           : uint16 Spi_lclStoreToTempQ(uint16 CurrQIdx,
*                                                uint16 *JobQ,                      
*                                                uint8 *JobLinkedSeqQ)
*  Parameters (in)  : CurrQIdx - the index in queue
*  Parameter(in-out): JobQ - temp job queue
*                     JobLinkedSeqQ  - temp sequence queue
*  Return value     : number of data units copied.
*  Description      : copy the data from queue to temp queue and return the number
*                    of entries copied.                                            
**********************************************************************************/
static uint16 Spi_lclStoreToTempQ(uint16 CurrQIdx, uint16 *JobQ, uint8 *JobLinkedSeqQ)
{
    uint16 Counter;
    uint16 Current_QIdx = CurrQIdx;

    Counter = 0;

    /* stores the values of JobQueue and JobLinkedSeq queue
       from current index to end index in
       queue pointed by JobQ and JobLinkedSeqQ */
    while (Current_QIdx != SpiGlobal.Queue.JobEndIndex)
    {
        JobQ[Counter] = SpiGlobal.Queue.JobQueue[Current_QIdx];
        JobLinkedSeqQ[Counter] = SpiGlobal.Queue.JobLinkedSeq[Current_QIdx];
        Counter++;
        Current_QIdx = Spi_lclGetNextQueueIdx(Current_QIdx);
    }
    return Counter;
}
#endif
#endif

#define SPI_STOP_SEC_CODE

