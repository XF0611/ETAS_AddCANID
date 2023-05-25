/************************************************************************************ 
*  Copyright (c) 2020, Calterah, Inc. All rights reserved.
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
#include "CDD_Uart.h"
#include "Uart_Alps_RegDef.h"
#include "CDD_Uart_Alps.h"

#if ( UART_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif

#include "Mcu.h"

/******************************************************************************
*                      Global variable  Definitions
******************************************************************************/
#define UART_START_SEC_CONST_32

static const Uart_ConfigType* LclCfgPtr;
#define UART_STOP_SEC_CONST_32

#define UART_START_SEC_VAR_INIT_PTR

/* Getting base address of the module  */
static Uart_RegType *UartHwReg[] = {( Uart_RegType*) 0xB30000UL,
                                    ( Uart_RegType*) 0xB40000UL
                                   };
#define UART_STOP_SEC_VAR_INIT_PTR

                   
#define UART_START_SEC_VAR_INIT_32

static Uart_StatusType UartStatus[UART_CONTROLLER_COUNT];
#define UART_STOP_SEC_VAR_INIT_32

/*--------------------------- Hal Function definitions-----------------------------*/
#define UART_START_SEC_CODE

/************************************************************************************
*     Uart_Hal_Init                                                             *
************************************************************************************/
void Uart_Hal_Init(const Uart_ConfigType *CfgPtr)
{
    uint8  HwUnit;
    uint32 RegValue     = 0;
    uint32 TempValue    = 0;
    uint32 LowBitValue  = 0;
    uint32 HighBitValue = 0;
    Mcu_PeripheralClockValType UartSysClk = 0;
    
    /* Get UART System Clock */
    UartSysClk = Mcu_GetPeripheralClkFreq(MCU_UART0_CLOCK);
    if (UartSysClk)
    {  
        /* Assigning to local CfgPtr to access main Uart_ConfigType */
        LclCfgPtr = CfgPtr;
        
        for (uint8 i=0; i<UART_CONTROLLER_COUNT; i++)
        {
            UartStatus[i] = UART_STATUS_UNINIT;
        }
        for (uint8 i=0; i<UART_CONFIG_COUNT; i++)
        {
            TempValue    = 0;
            LowBitValue  = 0;
            HighBitValue = 0;
            HwUnit = LclCfgPtr->CtrlCfgPtr[i].HwUnit;

            uint8 Flag                  = 1;
            uint8 NumCharFrameValue     = LclCfgPtr->CtrlCfgPtr[i].NumCharFrame;
            uint8 NumStopBitsValue      = LclCfgPtr->CtrlCfgPtr[i].NumStopBits;
            uint8 ParityValue           = LclCfgPtr->CtrlCfgPtr[i].ParityType;
            uint8 BaudRate              = LclCfgPtr->CtrlCfgPtr[i].Baudrate;
                    
            /* Data packet configuration  for Data frame i.e number of bits needs to transfer */
            switch (NumCharFrameValue)
            {
                case UART_DATALENGTH_5BIT    :  TempValue |=  (0x00 << 0);
                                                break;
                
                case UART_DATALENGTH_6BIT    :  TempValue |=  (0x01 << 0);
                                                break;
                
                case UART_DATALENGTH_7BIT    :  TempValue |=  (0x02 << 0);
                                                break;
                
                case UART_DATALENGTH_8BIT    :  TempValue |=  (0x03 << 0);
                                                break;
                
                default                      : /* UART_FSR_07 */
                                                Flag = 0;
                                                #if ( UART_DEV_ERROR_DETECT == STD_ON )
                                                Det_ReportError(CDD_UART_MODULE_ID,
                                                                CDD_UART_INSTANCE_ID,
                                                                UART_INIT_SERVICE_ID,
                                                                UART_E_PARAM_NUMCHARFRAME);
                                                #endif
                                                break;
                
            }
            
            /* Data packet configuration for  stop bits either 1 or 2 stop bits */
            switch (NumStopBitsValue)
            {
                case UART_STOPBIT_ONE           :  TempValue |= (0x00 << 2);
                                                   break;

                case UART_STOPBIT_ONE_AND_HALF  :  if((TempValue & 0x3) == 0)
                								   {
                									  TempValue |= (0x01 << 2);
                								   }
                                                   break;

                case UART_STOPBIT_TWO           :  if((TempValue & 0x03)!= 0 )
												   {
														TempValue |= (0x01 << 2);
												   }
                                                   break;

                default                         :  /* UART_FSR_08 */
                                                   Flag = 0;
                                                   #if ( UART_DEV_ERROR_DETECT == STD_ON )
                                                   Det_ReportError(CDD_UART_MODULE_ID,
                                                                   CDD_UART_INSTANCE_ID,
                                                                   UART_INIT_SERVICE_ID,
                                                                   UART_E_PARAM_NUMSTOPBITS);
                                                   #endif
                                                   break;
            }
            /* Data packet configuration for  parity set up */
            switch (ParityValue)
            {
                case UART_PARITY_NO        :  TempValue |= ((0x00 << 3) | (0x00 << 4) | (0x00 << 5));
                                              break;

                case UART_PARITY_EVEN      :  TempValue |= ((0x01 << 3) | (0x01 << 4) | (0x00 << 5));
                                              break;
                                                 
                case UART_PARITY_ODD       :  TempValue |= ((0x01 << 3) | (0x00 << 4) | (0x00 << 5));
                                              break;

                case UART_PARITY_FORCED_0  :  TempValue |= ((0x01 << 3) | (0x01 << 4) | (0x01 << 5));
                                              break;

                case UART_PARITY_FORCED_1  :  TempValue |= ((0x01 << 3) | (0x00 << 4) | (0x01 << 5));
                                              break;

                default                    :  /* UART_FSR_09 */
                                              Flag = 0;
                                              #if ( UART_DEV_ERROR_DETECT == STD_ON )
                                              Det_ReportError(CDD_UART_MODULE_ID,
                                                              CDD_UART_INSTANCE_ID,
                                                              UART_INIT_SERVICE_ID,
                                                              UART_E_PARAM_PARITY);
                                              #endif
                                              break;
                
            }
            switch (BaudRate)
            {
                case UART_BAUDRATE_600     :  RegValue     =  (UartSysClk / (16 * 600));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                              break;

                case UART_BAUDRATE_1200    :  RegValue     =  (UartSysClk / (16 * 1200));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                              break;
                    
                case UART_BAUDRATE_2400    :  RegValue     =  (UartSysClk / (16 * 2400));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                              break;
                    
                case UART_BAUDRATE_4800    :  RegValue     =  (UartSysClk / (16 * 4800));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF );
                                              break;
                    
                case UART_BAUDRATE_9600    :  RegValue     =  (UartSysClk / (16 * 9600));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF );
                                              break;
                    
                case UART_BAUDRATE_14400   :  RegValue     =  (UartSysClk / (16 * 14400));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF );
                                              break;
                    
                case UART_BAUDRATE_19200   :  RegValue     =  (UartSysClk / (16 * 19200));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF );
                                              break;
                    
                case UART_BAUDRATE_28800   :  RegValue     =  (UartSysClk / (16 * 28800));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF );
                                              break;
                    
                case UART_BAUDRATE_38400   :  RegValue     =  (UartSysClk / (16 * 38400));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                              break;
                    
                case UART_BAUDRATE_56000   :  RegValue     =  (UartSysClk / (16 * 56000));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                              break;
                    
                case UART_BAUDRATE_57600   :  RegValue     =  (UartSysClk / (16 * 57600));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                              break;
                   
                case UART_BAUDRATE_115200  :  RegValue     =  (UartSysClk / (16 * 115200));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                              break;
                    
                case UART_BAUDRATE_128000  :  RegValue     =  (UartSysClk / (16 * 128000));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                              break;
                  
                case UART_BAUDRATE_256000  :  RegValue     =  (UartSysClk / (16 * 256000));
                                              LowBitValue  =  (RegValue & 0xFF);
                                              HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                              break;
                    
                default                    :  /* UART_FSR_10 */
                                              Flag = 0;
                                              #if ( UART_DEV_ERROR_DETECT == STD_ON )
                                              Det_ReportError(CDD_UART_MODULE_ID,
                                                              CDD_UART_INSTANCE_ID,
                                                              UART_INIT_SERVICE_ID,
                                                              UART_E_PARAM_BAUDRATE);
                                              #endif
                                              break;
                
            }
            if (Flag == 1)
            {
                /* Set Divisor Latch Access Bit(DLAB) bit high */
                TempValue                               |= (0x01 << 7);
                UartHwReg[HwUnit]->LCR                   = TempValue;
                UartHwReg[HwUnit]->Uart_0x00_Reg.DLL     = LowBitValue;
                UartHwReg[HwUnit]->Uart_0x04_Reg.DLH     = HighBitValue;
                UartHwReg[HwUnit]->LCR                  &= ~(0x01u << 7);

                /* Enable FIFO */
                UartHwReg[HwUnit]->Uart_0x08_Reg.FCR     = (0x01 << 0);
                UartStatus[HwUnit] = UART_STATUS_IDLE;
            }
        }
    }

} /* end of Uart_Hal_Init */

/************************************************************************************
*     Uart_Hal_DeInit                                                               *
************************************************************************************/
Std_ReturnType Uart_Hal_DeInit(void)
{
    Std_ReturnType ReturnValue = E_OK;
    uint8 i;
    uint32 RegLSRClear;
    uint32 RegUSRClear;
    
    for (i=0; i<UART_CONTROLLER_COUNT; i++)
    {
        if (UartStatus[i] == UART_STATUS_BUSY)
        {
        	break;
        }
    }
    if (i < UART_CONTROLLER_COUNT)
    {
        ReturnValue = E_NOT_OK;
    }
    else
    /* Reset all the control, status, general purpose registers */
    {
        for (i=0; i<UART_CONFIG_COUNT; i++)
        {
            uint8 HwUnit                            = LclCfgPtr->CtrlCfgPtr[i].HwUnit;
            UartHwReg[HwUnit]->LCR                  = 0;
            UartHwReg[HwUnit]->Uart_0x00_Reg.DLL    = 0;
            UartHwReg[HwUnit]->Uart_0x04_Reg.DLH    = 0;
            RegLSRClear                             = UartHwReg[HwUnit]->LSR;
            RegUSRClear                             = UartHwReg[HwUnit]->USR;
            UartHwReg[HwUnit]->Uart_0x08_Reg.FCR    = 0;
            UartStatus[HwUnit]                      = UART_STATUS_UNINIT;
        }
    }
    
    return ReturnValue;
    
}   /* end of Uart_Hal_DeInit */

/************************************************************************************
*  Uart_Hal_Transmit                                                    *
************************************************************************************/
Std_ReturnType Uart_Hal_Transmit(uint8 HwUnit,
                                 const uint8 *TxBuf,
                                 uint16 Length)
{
    Std_ReturnType ReturnValue = E_OK;
    uint16 ByteIdx;
    UartStatus[HwUnit] = UART_STATUS_BUSY;
    
    for (ByteIdx=0; ByteIdx<Length; ByteIdx++)
    {
        UartHwReg[HwUnit]->Uart_0x00_Reg.THR = TxBuf[ByteIdx];
        while (!(UartHwReg[HwUnit]->LSR & (0x01 << 5)))
        {
        	;
        }
    }
    
    UartStatus[HwUnit] = UART_STATUS_IDLE;
    
    return ReturnValue;
    
} /* end of Uart_Hal_Transmit */

/************************************************************************************
*  Uart_Hal_Receive                                                 *
************************************************************************************/
Std_ReturnType  Uart_Hal_Receive(uint8 HwUnit,
                                 uint8 *RxBuf,
                                 uint16 *Length)
{
    Uart_FIFOStatusType FIFOStatus;
    Std_ReturnType ReturnValue = E_OK;
    uint8 ByteCount;

    *Length = 0;
    Uart_Hal_DataAvailable(HwUnit, &FIFOStatus);
    
    if (FIFOStatus == UART_RECEIVE_FIFO_NOT_EMPTY)
    {
        ByteCount = 0;
        do
        {
            RxBuf[ByteCount] = (uint8)UartHwReg[HwUnit]->Uart_0x00_Reg.RBR;
            ByteCount++;
            
        }while(UartHwReg[HwUnit]->LSR & (0x01));
        
        *Length = ByteCount;
     }
     else
     {
    	 ReturnValue = E_NOT_OK;
     }

    return ReturnValue;

} /* end of Uart_Hal_Receive */


/************************************************************************************
*     Uart_Hal_GetStatus                                                                *
************************************************************************************/
Uart_StatusType Uart_Hal_GetStatus(uint8 HwUnit)
{
    return(UartStatus[HwUnit]);

} /* end of Uart_Hal_GetStatus*/

/************************************************************************************
*    Uart_Hal_DataAvailable                                                     *
************************************************************************************/
Std_ReturnType Uart_Hal_DataAvailable(uint8 HwUnit, Uart_FIFOStatusType *Status)
{

    /* Check whether Receiver FIFO contain data or not */
    *Status = ((UartHwReg[HwUnit]->USR & ( 0x01 << 3 )) == 8) ? \
               (UART_RECEIVE_FIFO_NOT_EMPTY) : (UART_RECEIVE_FIFO_EMPTY);
    
    return E_OK;
    
} /* end of Uart_Hal_DataAvailable */

/************************************************************************************
*    Uart_Hal_GetErrorStatus                                                            *
************************************************************************************/
void  Uart_Hal_GetErrorStatus(uint8 HwUnit, uint8 *ErrorInfo)
{
    *ErrorInfo      = 0;
    uint32 RegValue = UartHwReg[HwUnit]->LSR;
    
    if (0x2UL == (RegValue & UART_LSR_OVRRUN_MSK))
    {
        *ErrorInfo |= (0x01<<0);
    }
    else
    {
    	/*compliant to misra-c*/
    }
    if (0x8UL == (RegValue & UART_LSR_FRAMING_MSK))
    {
    	*ErrorInfo |= (0x01<<1);
    }
    else
    {
    	/*compliant to misra-c*/
    }
    if (0x80UL == (RegValue & UART_LSR_RECEIVEFIFO_MSK))
    {
    	*ErrorInfo |= (0x01<<2);
    }
    else
    {
    	/*compliant to misra-c*/
    }
    
    if (0x4UL == (RegValue & UART_LSR_PARITY_MSK))
    {
    	*ErrorInfo |= (0x01<<3);
    }
    else
    {
    	/*compliant to misra-c*/
    }
        
} /* end of Uart_Hal_GetErrorStatus */

/************************************************************************************
*     Uart_Hal_SetBaudrate                                                          *
************************************************************************************/
Std_ReturnType Uart_Hal_SetBaudrate (uint8 HwUnit, Uart_BaudrateType Baudrate)
{
    uint8  Flag         = 1;
    uint32 RegValue     = 0;
    uint32 LowBitValue  = 0;
    uint32 HighBitValue = 0;
    Std_ReturnType ReturnValue   = E_OK;
    Mcu_PeripheralClockValType UartSysClk = 0;
    
    /* Get UART System Clock */
    UartSysClk = Mcu_GetPeripheralClkFreq(MCU_UART0_CLOCK);
    if (UartSysClk)
    {    
        switch (Baudrate)
        {
            case UART_BAUDRATE_600     :  RegValue     =  (UartSysClk / (16 * 600));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
            
            case UART_BAUDRATE_1200    :  RegValue     =  (UartSysClk / (16 * 1200));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
            
            case UART_BAUDRATE_2400    :  RegValue     =  (UartSysClk / (16 * 2400));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_4800    :  RegValue     =  (UartSysClk / (16 * 4800));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_9600    :  RegValue     =  (UartSysClk / (16 * 9600));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_14400   :  RegValue     =  (UartSysClk / (16 * 14400));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_19200   :  RegValue     =  (UartSysClk / (16 * 19200));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_28800   :  RegValue     =  (UartSysClk / (16 * 28800));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_38400   :  RegValue     =  (UartSysClk / (16 * 38400));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_56000   :  RegValue     =  (UartSysClk / (16 * 56000));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_57600   :  RegValue     =  (UartSysClk / (16 * 57600));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_115200  :  RegValue     =  (UartSysClk / (16 * 115200));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_128000  :  RegValue     =  (UartSysClk / (16 * 128000));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            case UART_BAUDRATE_256000  :  RegValue     =  (UartSysClk / (16 * 256000));
                                          LowBitValue  =  (RegValue & 0xFF);
                                          HighBitValue =  ((RegValue >> 0x08) & 0xFF);
                                          break;
                
            default                    :  /* UART_FSR_10 */
                                          Flag = 0;
                                          #if ( UART_DEV_ERROR_DETECT == STD_ON )
                                          Det_ReportError(CDD_UART_MODULE_ID,
                                                          CDD_UART_INSTANCE_ID,
                                                          UART_SETBAUDARTE_SERVICE_ID,
                                                          UART_E_PARAM_BAUDRATE);
                                          #endif
                                          break;
        }
        if (Flag == 1)
        {
            /* Set Divisor Latch Access Bit(DLAB) bit high */
            UartHwReg[HwUnit]->LCR                 |= (0x01<<7);
            UartHwReg[HwUnit]->Uart_0x00_Reg.DLL    = LowBitValue;
            UartHwReg[HwUnit]->Uart_0x04_Reg.DLH    = HighBitValue;
            UartHwReg[HwUnit]->LCR                 &= ~(0x01u << 7);
        }
    }
    
    return ReturnValue;
}/* end of Uart_Hal_SetBaudrate */

#define UART_STOP_SEC_CODE
