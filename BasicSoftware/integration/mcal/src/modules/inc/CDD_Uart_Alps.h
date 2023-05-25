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

#ifndef CDD_UART_ALPS_H
#define CDD_UART_ALPS_H

#define UART_LSR_OVRRUN_MSK           (0x00000002UL)
#define UART_LSR_PARITY_MSK           (0x00000004UL)
#define UART_LSR_FRAMING_MSK          (0x00000008UL)
#define UART_LSR_RECEIVEFIFO_MSK      (0x00000080UL)

#define UART_START_SEC_CODE

void Uart_Hal_Init(const Uart_ConfigType *CfgPtr);
Std_ReturnType Uart_Hal_DeInit(void);
Std_ReturnType Uart_Hal_Transmit(uint8 HwUnit, const uint8 *TxBuf, uint16 Length);
Std_ReturnType Uart_Hal_Receive(uint8 HwUnit, uint8 *RxBuf, uint16 *Length);
Std_ReturnType Uart_Hal_DataAvailable(uint8 HwUnit, Uart_FIFOStatusType *Status);
Uart_StatusType Uart_Hal_GetStatus(uint8 HwUnit);
void Uart_Hal_GetErrorStatus(uint8 HwUnit, uint8 *ErrorInfo);
Std_ReturnType Uart_Hal_SetBaudrate(uint8 HwUnit, Uart_BaudrateType Baudrate);

#define UART_STOP_SEC_CODE

#endif /* CDD_UART_ALPS_H */
