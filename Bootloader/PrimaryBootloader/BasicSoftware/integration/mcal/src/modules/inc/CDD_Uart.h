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

#ifndef CDD_UART_H
#define CDD_UART_H

/* Includes */
#include "CDD_Uart_Cfg.h"
#include "Std_Types.h"


/* Service API Id's */
#define UART_INIT_SERVICE_ID                  0x01u    /* Service id for initialization of UART.*/
#define UART_DEINIT_SERVICE_ID                0x02u    /* Service id for Deinitialization of UART */
#define UART_TRANSMIT_SERVICE_ID              0x03u    /* Service id for UART Transmission */
#define UART_RECEIVE_SERVICE_ID               0x04u    /* Service id for UART Reception */
#define UART_GETSTATUS_SERVICE_ID             0x05u    /* Service id to get status of the UART */
#define UART_DATAAVAILABLE_SERVICE_ID         0x06u    /* Service id to check data availability in receive data register */
#define UART_GETERRORSTATUS_SERVICE_ID        0x07u    /* Service id  to indicate type of error in module operation */
#define UART_SETBAUDARTE_SERVICE_ID           0x08u    /* Service id to set the baud rate for UART. */
#define UART_GETVERSIONINFO_SERVICE_ID        0x09u    /* Service Id for Uart_GetVersionInfo API */


/* Error codes Id's */
#define UART_E_UNINIT                        0x01u            /* API service used without module initialization */
#define UART_E_ALREADY_INITIALIZED           0x02u            /* API Uart_Init service called while the Uart driver has already been initialized */
#define UART_E_PARAM_POINTER                 0x03u            /* API Service used with a null pointer */
#define UART_E_PARAM_BAUDRATE                0x04u            /* API Parameter baud rate has an invalid value */
#define UART_E_PARAM_HWUNIT                  0x05u            /* API service called with wrong parameter */
#define UART_E_PARAM_NUMCHARFRAME            0x06u            /* API service called with wrong parameter */
#define UART_E_PARAM_NUMSTOPBITS             0x07u            /* API service called with wrong parameter */
#define UART_E_PARAM_PARITY                  0x08u            /* API service called with wrong parameter */

#define UART_CONTROLLER_COUNT                2u

/* Enum definations */
typedef enum
{
    UART_STATUS_UNINIT,
    UART_STATUS_IDLE,
    UART_STATUS_BUSY
}Uart_StatusType;

typedef enum 
{
    UART_DATALENGTH_5BIT,
    UART_DATALENGTH_6BIT,
    UART_DATALENGTH_7BIT ,
    UART_DATALENGTH_8BIT
}Uart_NumCharFrameType;

typedef enum
{

    UART_STOPBIT_ONE, 
    UART_STOPBIT_ONE_AND_HALF,
    UART_STOPBIT_TWO
}Uart_NumStopBitsType;

/* Enum for parity checking */
typedef enum
{
    UART_PARITY_NO,
    UART_PARITY_EVEN,
    UART_PARITY_ODD,
    UART_PARITY_FORCED_0,
    UART_PARITY_FORCED_1
}Uart_ParityType;


/* Enum for standard baudrates */
typedef enum 
{ 
    /* Uart serial communication baudrate */
    UART_BAUDRATE_600,
    UART_BAUDRATE_1200,
    UART_BAUDRATE_2400,
    UART_BAUDRATE_4800,
    UART_BAUDRATE_9600,
    UART_BAUDRATE_14400,
    UART_BAUDRATE_19200,
    UART_BAUDRATE_28800,
    UART_BAUDRATE_38400,
    UART_BAUDRATE_56000,
    UART_BAUDRATE_57600,
    UART_BAUDRATE_115200,
    UART_BAUDRATE_128000,
    UART_BAUDRATE_256000,
}Uart_BaudrateType;

/* Enum for data availability in fifo */
typedef enum
{
    UART_RECEIVE_FIFO_EMPTY,
    UART_RECEIVE_FIFO_NOT_EMPTY
}Uart_FIFOStatusType;

/* Structure for Uart Packet config type */
typedef struct 
{
    uint8                 HwUnit;
    Uart_NumCharFrameType NumCharFrame;
    Uart_NumStopBitsType  NumStopBits;
    Uart_ParityType       ParityType;
    Uart_BaudrateType     Baudrate;
}Uart_CtrlConfigType;

/* Cofiguration Structure for UART */
typedef struct 
{
    const Uart_CtrlConfigType *CtrlCfgPtr;
}Uart_ConfigType;

#define UART_START_SEC_CONFIG_DATA_UNSPECIFIED

extern const Uart_ConfigType UartConfigData;

#define UART_STOP_SEC_CONFIG_DATA_UNSPECIFIED

/* API Function prototypes */
#define UART_START_SEC_CODE

void Uart_Init(const Uart_ConfigType *CfgPtr);
Std_ReturnType Uart_DeInit(void);
Std_ReturnType Uart_Transmit(uint8 HwUnit, const uint8 *TxBuf, uint16 Length);
Std_ReturnType Uart_Receive(uint8 HwUnit, uint8 *RxBuf, uint16 *Length);
Uart_StatusType Uart_GetStatus(uint8 HwUnit);
Std_ReturnType Uart_DataAvailable(uint8 HwUnit, Uart_FIFOStatusType *Status);
void Uart_GetErrorStatus(uint8 HwUnit, uint8 *ErrorInfo);
Std_ReturnType Uart_SetBaudrate(uint8 HwUnit, Uart_BaudrateType Baudrate);
void Uart_GetVersionInfo(Std_VersionInfoType* VersionInfo);
 
#define UART_STOP_SEC_CODE

#endif /* CDD_UART_H */
