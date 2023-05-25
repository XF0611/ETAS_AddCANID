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
*************************************************************************************
**                                                                                 **
**  FILENAME   : CDD_Uart_PBCfg.c                                                  **
**                                                                                 **
**                                                                                 **
**  DATE, TIME : 2022-12-14, 16:35:02                                          **
**                                                                                 **
**  GENERATOR  : Build b210315-0853                                              **
**                                                                                 **
**  AUTHOR     : Calterah Semiconductor                                            **
**                                                                                 **
**  VENDOR     : Calterah Semiconductor                                            **
**                                                                                 **
**  DESCRIPTION : UART configuration generated out of ECU configuration            **
**                file (CDD_Uart.bmd)                                              **
**                                                                                 **
**  MAY BE CHANGED BY USER [yes/no]: no                                            **
**                                                                                 **
*************************************************************************************/

/* Include module header file */
#include "CDD_Uart.h"


#define UART_START_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Uart_MemMap.h"

const Uart_CtrlConfigType UartCtrlCfg[] =
{

{
    .HwUnit        = 0,
    .Baudrate      = UART_BAUDRATE_115200,
    .NumCharFrame  = UART_DATALENGTH_8BIT,
    .NumStopBits   = UART_STOPBIT_ONE,
    .ParityType    = UART_PARITY_NO
} 
};


const Uart_ConfigType UartConfigData =
{
    .CtrlCfgPtr = UartCtrlCfg 
};


#define UART_STOP_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Uart_MemMap.h"
