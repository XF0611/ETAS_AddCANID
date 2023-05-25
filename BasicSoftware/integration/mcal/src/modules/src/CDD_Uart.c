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
#include "Uart.h"
#include "CDD_Uart_Alps.h"

#if ( UART_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/
#ifndef CDD_UART_SW_MAJOR_VERSION
#error "CDD_UART_SW_MAJOR_VERSION is not defined."
#endif

#ifndef CDD_UART_SW_MINOR_VERSION
#error "CDD_UART_SW_MINOR_VERSION is not defined."
#endif

#ifndef CDD_UART_SW_PATCH_VERSION
#error "CDD_UART_SW_PATCH_VERSION is not defined."
#endif

#if(CDD_UART_SW_MAJOR_VERSION != 3U)
#error "CDD_UART_SW_MAJOR_VERSION does not match."
#endif

#if(CDD_UART_SW_MINOR_VERSION != 4U)
#error "UART_SW_MINOR_VERSION does not match."
#endif

#if(CDD_UART_SW_PATCH_VERSION != 0U)
#error "CDD_UART_SW_PATCH_VERSION does not match."
#endif

/* AUTOSAR Secification File Version Check */
#ifndef CDD_UART_AR_RELEASE_MAJOR_VERSION
#error "CDD_UART_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

/* AUTOSAR Major and Minor release version check  */
#if(CDD_UART_AR_RELEASE_MAJOR_VERSION != 4U)
#error "CDD_UART_AR_RELEASE_MAJOR_VERSION does not match."
#endif

/* Inter Module Check for the correct version usage in the used modules */

/* Version Check */
/* Version check for DET module inclusion */
#if(UART_DEV_ERROR_DETECT == STD_ON)

#ifndef DET_AR_RELEASE_MAJOR_VERSION
#error "DET_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

#if (DET_AR_RELEASE_MAJOR_VERSION != CDD_UART_AR_RELEASE_MAJOR_VERSION)
#error "DET_AR_RELEASE_MAJOR_VERSION does not match."
#endif

#endif /* UART_DEV_ERROR_DETECT == STD_ON */

/******************************************************************************
*                       Private Variable Definitions
******************************************************************************/
#define UART_START_SEC_VAR_INIT_8

/* Initially made uart UNINIT,once Uart_Init calls it becomes TRUE */
static boolean UartInitialised = FALSE;
#define UART_STOP_SEC_VAR_INIT_8

#define UART_START_SEC_CODE
/******************************************************************************
*                    Private Function Declaration
******************************************************************************/
static void Uart_lclParamCheck(boolean Expr,
                               uint8 ServiceApiId,
                               uint8 ErrorCodeId,
                               Std_ReturnType *ReturnValue);


/******************************************************************************
*                         Public Functions                                    *
******************************************************************************/ 

/******************************************************************************
*  Service name      : Uart_Init
*  Syntax            : void Uart_Init(const Uart_ConfigType* CfgPtr)
*  Service ID[hex]   : 0x01
*  Parameters (in)   : CfgPtr - Pointer to configtype structure
*  Parameters(inout) : None
*  Parameters (out)  : None
*  Return value      : None
*  Description       : Service for initialization of UART
******************************************************************************/
void Uart_Init(const Uart_ConfigType *CfgPtr)
{
    Std_ReturnType ReturnValue = E_OK;

    /* Uart_FSR_01 */
    /* Check initial conditions cfgptr is null or not */
    Uart_lclParamCheck((boolean)(CfgPtr == NULL_PTR),
                        UART_INIT_SERVICE_ID,
                        UART_E_PARAM_POINTER,
                        &ReturnValue);

    /* UART_FSR_02 */
    /* Check if Uart_Init initialized already or not */
    Uart_lclParamCheck((boolean)(UartInitialised == TRUE),
                        UART_INIT_SERVICE_ID,
                        UART_E_ALREADY_INITIALIZED,
                        &ReturnValue);

    if (ReturnValue == E_OK)
    {
        Uart_Hal_Init(CfgPtr);
    }

    UartInitialised = TRUE;

} /* end of Uart_Init API */

/******************************************************************************
*  Service name    : Uart_DeInit
*  Syntax          : Std_ReturnType Uart_DeInit(void)
*  Service ID      : 0x02
*  Parameter (in)  : None
*  Parameters (out): None
*  Return value    : E_OK - De-initialization command accepted
*                    E_NOT_OK - De-initialization command has not been accepted
*  Description     : Service for Deinitialization of UART
******************************************************************************/
Std_ReturnType Uart_DeInit(void)
{
    Std_ReturnType ReturnValue = E_OK ;

    /* UART_FSR_04 */
    /* Check if Uart_Init initialised already or not */
    Uart_lclParamCheck((boolean)(UartInitialised == FALSE),
                        UART_DEINIT_SERVICE_ID,
                        UART_E_UNINIT,
                        &ReturnValue);

    /* Once condition is true start Uart_DeInit function in HAL layer */
    if (E_OK == ReturnValue)
    {
        ReturnValue = Uart_Hal_DeInit();
        if (E_OK == ReturnValue)
        {
            UartInitialised = FALSE;
        }
    }
    
    return ReturnValue;
    
} /* end of Uart_DeInit API */

/******************************************************************************
*  Service name       : Uart_Transmit
*  Syntax             : Std_ReturnType Uart_Transmit(uint8 HwUnit,
*                                                    const uint8 *TxBuf,
*                                                    uint16 Length)
*  Service ID         : 0x03
*  Parameter (in)     : HwUnit - Module ID of the Hardware unit to transmit
*                       TxBuf  - Pointer to the data to be transmitted
*                       Length - Number of bytes to be transmitted. Range: 0 - 65535
*  Parameter (inout)  : None
*  Parameters (out)   : None
*  Return value       : E_OK - Transmit command accepted
*                       E_NOT_OK - Transmit command not accepted
*  Description        : Service for UART Transmission
*******************************************************************************************/
Std_ReturnType Uart_Transmit(uint8 HwUnit, const uint8 *TxBuf, uint16 Length)
{
    Std_ReturnType  ReturnValue = E_OK;
    Uart_StatusType Status;

    /* UART_FSR_04 */
    /* Check if Uart_Init initialised already or not */
    Uart_lclParamCheck((boolean)(UartInitialised == FALSE),
                        UART_TRANSMIT_SERVICE_ID,
                        UART_E_UNINIT,
                        &ReturnValue);

    /* UART_FSR_05 */
    /* Check HwUnit value lessthan total count value or not */
    Uart_lclParamCheck((boolean)(HwUnit >= UART_CONTROLLER_COUNT),
                        UART_TRANSMIT_SERVICE_ID,
                        UART_E_PARAM_HWUNIT,
                        &ReturnValue);

    /* UART_FSR_06 */
    /* Check TxBuf is null or not */
    Uart_lclParamCheck((boolean)(TxBuf == NULL_PTR),
                        UART_TRANSMIT_SERVICE_ID,
                        UART_E_PARAM_POINTER,
                        &ReturnValue);

    if (ReturnValue == E_OK)
    {
        Status = Uart_GetStatus(HwUnit);
        if (UART_STATUS_UNINIT == Status)
        {
            Uart_lclParamCheck(TRUE,
                               UART_TRANSMIT_SERVICE_ID,
                               UART_E_UNINIT,
                               &ReturnValue);
        }
        else if (UART_STATUS_BUSY == Status)
        {
            ReturnValue = E_NOT_OK;
        }
        else
        {            
            ReturnValue = Uart_Hal_Transmit(HwUnit, TxBuf, Length);
        }
    }
    return ReturnValue;
    
}/* end of Uart_Transmit API */

/******************************************************************************
*  Service name       : Uart_Receive
*  Syntax             : Std_ReturnType Uart_Receive(uint8 HwUnit, uint8 *RxBuf,
*                                                   uint16 *Length)
*  Service ID         : 0x04
*  Parameter (in)     : HwUnit - Module ID of the Hardware unit to receive
*                       RxBuf  - Buffer to store received data
*  Parameter (inout)  : None
*  Parameters (out)   : Length - Number of bytes which were read
*  Return value       : E_OK - Receive command accepted
*                       E_NOT_OK - Receive command not accepted
*  Description        : Service for UART Reception
*******************************************************************************************/
Std_ReturnType Uart_Receive(uint8 HwUnit, uint8 *RxBuf, uint16 *Length)
{
    Std_ReturnType  ReturnValue = E_OK;
    Uart_StatusType Status;

    /* UART_FSR_04 */
    /* Check if Uart_Init initialised already or not */
    Uart_lclParamCheck((boolean)(UartInitialised == FALSE),
                        UART_RECEIVE_SERVICE_ID,
                        UART_E_UNINIT,
                        &ReturnValue);

    /* UART_FSR_05 */
    /* Check HwUnit value lessthan total count value or not */
    Uart_lclParamCheck((boolean)(HwUnit >= UART_CONTROLLER_COUNT),
                        UART_RECEIVE_SERVICE_ID,
                        UART_E_PARAM_HWUNIT,
                        &ReturnValue );

    /* UART_FSR_06 */
    /* Check RxBuf is null or not */
    Uart_lclParamCheck((boolean)(RxBuf == NULL_PTR),
                        UART_RECEIVE_SERVICE_ID,
                        UART_E_PARAM_POINTER,
                        &ReturnValue);

    /* UART_FSR_06 */
    /* Check Length is null or not */
    Uart_lclParamCheck((boolean)(Length == NULL_PTR),
                        UART_RECEIVE_SERVICE_ID,
                        UART_E_PARAM_POINTER,
                        &ReturnValue);

    if (ReturnValue == E_OK)
    {
        Status = Uart_GetStatus(HwUnit);
        if (UART_STATUS_UNINIT == Status)
        {
            Uart_lclParamCheck(TRUE,
                               UART_RECEIVE_SERVICE_ID,
                               UART_E_UNINIT,
                               &ReturnValue);
        }
        else
        {
            ReturnValue = Uart_Hal_Receive(HwUnit, RxBuf, Length);
        }
    }

    return ReturnValue;

}/* end of Uart_Receive */

/******************************************************************************
*  Service name    : Uart_GetStatus
*  Syntax          : Uart_StatusType Uart_GetStatus(uint8 HwUnit)
*  Service ID      : 0x05
*  Parameter (in)  : HwUnit -  Module ID of the Hardware unit to get status
*  Parameters (out): None
*  Return value    : UART_STATUS_UNINIT - UART is uninitialized
*                    UART_STATUS_IDLE - UART is in idle state
*                    UART_STATUS_BUSY - UART is in busy state
*  Description     : Service to get status of the UART
******************************************************************************/
Uart_StatusType Uart_GetStatus(uint8 HwUnit)
{
    Uart_StatusType UartStatus  = UART_STATUS_UNINIT;
    Std_ReturnType  ReturnValue = E_OK;
    
    /* UART_FSR_05 */
    /* Check HwUnit value lessthan total count value or not */
    Uart_lclParamCheck((boolean)(HwUnit >= UART_CONTROLLER_COUNT),
                        UART_GETSTATUS_SERVICE_ID,
                        UART_E_PARAM_HWUNIT,
                        &ReturnValue );

    if (ReturnValue == E_OK)
    {
        if (UartInitialised == FALSE)
        {
            UartStatus = UART_STATUS_UNINIT;
        }
        else
        {
            UartStatus = Uart_Hal_GetStatus(HwUnit);
        }
    }

    return UartStatus;

} /* end of Uart_GetStatus */

/******************************************************************************************
*  Service name       : Uart_DataAvailable
*  Syntax             : Std_ReturnType Uart_DataAvailable(uint8 HwUnit,
*                                                         Uart_FIFOStatusType *Status)
*  Service ID         : 0x06
*  Parameter (in)     : HwUnit -  Module ID of the Hardware unit to know data availability.
*  Parameter (inout)  : Status -  UART_RECEIVE_FIFO_EMPTY - Data is not available.
*                                 UART_RECEIVE_FIFO_NOT_EMPTY - Data is available.
*  Parameters (out)   : None
*  Return value       : E_OK - DataAvailable command accepted
*                       E_NOT_OK - DataAvailable command not accepted
*  Description        : Service to check data availability in receive data register FIFO
*******************************************************************************************/
Std_ReturnType Uart_DataAvailable(uint8 HwUnit, Uart_FIFOStatusType *Status)
{
    Std_ReturnType  ReturnValue = E_OK;
    Uart_StatusType HwStatus;
    
    /* UART_FSR_04 */
    /* Check if Uart_Init initialised already or not */
    Uart_lclParamCheck((boolean)(UartInitialised == FALSE),
                        UART_DATAAVAILABLE_SERVICE_ID,
                        UART_E_UNINIT,
                        &ReturnValue);

    /* UART_FSR_05 */
    Uart_lclParamCheck((boolean)(HwUnit >= UART_CONTROLLER_COUNT),
                        UART_DATAAVAILABLE_SERVICE_ID,
                        UART_E_PARAM_HWUNIT,
                        &ReturnValue);

    /* UART_FSR_06 */
    Uart_lclParamCheck((boolean)(Status == NULL_PTR),
                        UART_DATAAVAILABLE_SERVICE_ID,
                        UART_E_PARAM_POINTER,
                        &ReturnValue);

    if (ReturnValue == E_OK)
    {
        HwStatus = Uart_GetStatus(HwUnit);
        if (UART_STATUS_UNINIT==HwStatus)
        {
            Uart_lclParamCheck(TRUE,
                               UART_DATAAVAILABLE_SERVICE_ID,
                               UART_E_UNINIT,
                               &ReturnValue );
        }
        else
        {
            ReturnValue = Uart_Hal_DataAvailable(HwUnit, Status);
        }
    }
    
    return ReturnValue;
    
}/* end of Uart_DataAvailable API */

/******************************************************************************
*  Service name       : Uart_GetErrorStatus
*  Syntax             : void Uart_GetErrorStatus(uint8 HwUnit, uint8 *ErrorInfo)
*  Service ID         : 0x07
*  Parameter (in)     : HwUnit -  Module ID of the Hardware unit to get status
*  Parameter (inout)  : ErrorInfo - bit encoded as follows
*                       bit 0 : UART Overrun error
*                       bit 1 : UART Framing error
*                       bit 2 : UART Rx FIFO error
*                       bit 3 : UART Parity error
*  Parameters (out)   : None
*  Return value       : None
*  Description        : Service to indicate type of error in module operation
******************************************************************************/
void Uart_GetErrorStatus(uint8 HwUnit, uint8 *ErrorInfo)
{
    Std_ReturnType  ReturnValue = E_OK;
    Uart_StatusType Status;
    
    /* UART_FSR_04 */
    /* Check if Uart_Init initialised already or not */
    Uart_lclParamCheck((boolean)(UartInitialised == FALSE),
                        UART_GETERRORSTATUS_SERVICE_ID,
                        UART_E_UNINIT,
                        &ReturnValue);

    /* UART_FSR_05 */
    /* Check HwUnit value lessthan total count value or not */
    Uart_lclParamCheck((boolean)(HwUnit >= UART_CONTROLLER_COUNT),
                        UART_GETERRORSTATUS_SERVICE_ID,
                        UART_E_PARAM_HWUNIT,
                        &ReturnValue);

    /* UART_FSR_06 */
    /* Check ErrorInfo is null pointer or not */
    Uart_lclParamCheck((boolean)(ErrorInfo == NULL_PTR),
                        UART_GETERRORSTATUS_SERVICE_ID,
                        UART_E_PARAM_POINTER,
                        &ReturnValue);
  
    if (ReturnValue == E_OK)
    {
        Status = Uart_GetStatus(HwUnit);
        if (UART_STATUS_UNINIT == Status)
        {
            Uart_lclParamCheck(TRUE,
                               UART_GETERRORSTATUS_SERVICE_ID,
                               UART_E_UNINIT,
                               &ReturnValue);
        }
        else
        {
            Uart_Hal_GetErrorStatus(HwUnit, ErrorInfo);
        }
    }

} /* end of Uart_GetErrorStatus */

/******************************************************************************
*  Service name    : Uart_SetBaudrate
*  Syntax          : Std_ReturnType Uart_SetBaudrate(unit8 HwUnit,
.....................................................Uart_BaudrateType Baudrate)
*  Service ID      : 0x08
*  Parameter (in)  : HwUnit   - Hardware unit for which baud rate need to set
*                    Baudrate - Baud rate needs to be set
*  Parameters (out): None
*  Return value    : E_OK - Set baudrate command accepted.
*                    E_NOT_OK - Set baudrate command not accepted.
*  Description     : This service shall set the baud rate for UART
******************************************************************************/
Std_ReturnType Uart_SetBaudrate(uint8 HwUnit, Uart_BaudrateType Baudrate)
{
    Std_ReturnType  ReturnValue = E_OK;
    Uart_StatusType Status;

    /* UART_FSR_04 */
    /* Check if Uart_Init initialised already or not */
    Uart_lclParamCheck((boolean)(UartInitialised == FALSE),
                        UART_SETBAUDARTE_SERVICE_ID,
                        UART_E_UNINIT,
                        &ReturnValue);

    /* UART_FSR_05 */
    /* Check HwUnit value lessthan total count value or not */
    Uart_lclParamCheck((boolean)(HwUnit >= UART_CONTROLLER_COUNT),
                        UART_SETBAUDARTE_SERVICE_ID,
                        UART_E_PARAM_HWUNIT,
                        &ReturnValue);

    if (ReturnValue == E_OK)
    {
        Status = Uart_GetStatus(HwUnit);
        if (UART_STATUS_UNINIT == Status)
        {
            Uart_lclParamCheck(TRUE,
                               UART_SETBAUDARTE_SERVICE_ID,
                               UART_E_UNINIT,
                               &ReturnValue);
        }
        else
        {            
            /* Set appropriate baudrate to the module */
            ReturnValue = Uart_Hal_SetBaudrate(HwUnit, Baudrate);
        }
    }

    return ReturnValue;
} /* end of Uart_SetBaudrate */

#if (UART_VERSION_INFO_API == STD_ON)
/******************************************************************************
*  Service name     : Uart_GetVersionInfo                                      
*  Syntax           : void UART_GetVersionInfo(Std_VersionInfoType* VersionInfo)
*  Service ID       : 0x09                                                     
*  Sync/Async       : Synchronous                                              
*  Reentrancy       : Reentrant                                             
*  Parameters (in)  : VersionInfo: Pointer to where to store the version 
*                     information of this module              
*  Parameters (out) : None                                                     
*  Return value     : None                                                     
*  Description      : Returns the version information of this module.
******************************************************************************/
void Uart_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
    Std_ReturnType ReturnValue = E_OK;

    /* UART_FSR_06 */
    Uart_lclParamCheck((boolean)(NULL_PTR == VersionInfo),
                        UART_GETVERSIONINFO_SERVICE_ID,
                        UART_E_PARAM_POINTER,
                        &ReturnValue);

    if (E_OK == ReturnValue)
    {
        VersionInfo->vendorID         = CDD_UART_VENDOR_ID;
        VersionInfo->moduleID         = CDD_UART_MODULE_ID;
        VersionInfo->sw_major_version = CDD_UART_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = CDD_UART_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = CDD_UART_SW_PATCH_VERSION;
    }
}
#endif

/******************************************************************************
*                         Private Function Definitions
******************************************************************************/

/**************************************************************************************
*  Service name      : Uart_lclParamCheck
*  Syntax            : Std_ReturnType Uart_lclParamCheck(boolean Expr,
*                                                        uint8 ServiceApiId,
*                                                        uint8 ErrorCodeId,
*                                                        Std_ReturnType* ReturnValue);
*  Parameters (in)   : Expr - boolean expression to check API conditions
*                      ServiceApiId - Service Id of API
*                      ErrorCodeId - Error Id of Condition
*  Parameters(inout) : ReturnValue - variable to check return type
*  Parameters (out)  : None
*  Return value      : None
*  Description       : Service for local parameter checking of UART
***************************************************************************************/
static void  Uart_lclParamCheck(boolean Expr,
                                uint8 ServiceApiId,
                                uint8 ErrorCodeId,
                                Std_ReturnType *ReturnValue)
{
    if ((TRUE == Expr) && (*ReturnValue == E_OK))
    {
        #if ( UART_DEV_ERROR_DETECT == STD_ON )
        (void)Det_ReportError(CDD_UART_MODULE_ID,
                              CDD_UART_INSTANCE_ID,
                              ServiceApiId,
                              ErrorCodeId);
        #endif
        *ReturnValue = E_NOT_OK;
    }
} /* end of Uart_lclParamCheck */

#define UART_STOP_SEC_CODE
