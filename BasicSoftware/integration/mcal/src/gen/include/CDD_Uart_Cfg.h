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
**  FILENAME   : CDD_Uart_Cfg.h                                                         **
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
**  DESCRIPTION : UART configuration generated out of ECU configuration             **
**                file (CDD_Uart.bmd)                                                   **
**                                                                                 **
**  MAY BE CHANGED BY USER [yes/no]: no                                            **
**                                                                                 **
*************************************************************************************/
#ifndef CDD_UART_CFG_H
#define CDD_UART_CFG_H


#define CDD_UART_VENDOR_ID       0x8b
#define CDD_UART_MODULE_ID       255
#define CDD_UART_INSTANCE_ID     0

#define CDD_UART_AR_RELEASE_MAJOR_VERSION     4
#define CDD_UART_AR_RELEASE_MINOR_VERSION     3
#define CDD_UART_AR_RELEASE_PATCH_VERSION     1

#define CDD_UART_SW_MAJOR_VERSION    3
#define CDD_UART_SW_MINOR_VERSION    4
#define CDD_UART_SW_PATCH_VERSION    0

#define UART_DEV_ERROR_DETECT       (STD_OFF)
#define UART_VERSION_INFO_API       (STD_OFF)


 /* UART Config Set */
#define UART_CONFIG_COUNT    (1U)


#endif /* CDD_UART_CFG_H */
