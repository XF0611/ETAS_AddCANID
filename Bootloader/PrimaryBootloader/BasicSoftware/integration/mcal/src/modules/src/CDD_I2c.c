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
#include "CDD_I2c.h"
#include "CDD_I2c_Alps.h"

#if (I2C_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/
/*************************version check************************************/
#ifndef CDD_I2c_SW_MAJOR_VERSION
#error "CDD_I2c_SW_MAJOR_VERSION is not defined."
#endif

#ifndef CDD_I2c_SW_MINOR_VERSION
#error "CDD_I2c_SW_MINOR_VERSION is not defined."
#endif

#ifndef CDD_I2c_SW_PATCH_VERSION
#error "CDD_I2c_SW_PATCH_VERSION is not defined."
#endif

#if(CDD_I2c_SW_MAJOR_VERSION != 3U)
#error "CDD_I2c_SW_MAJOR_VERSION does not match."
#endif

#if(CDD_I2c_SW_MINOR_VERSION != 4U)
#error "CDD_I2c_SW_MINOR_VERSION does not match."
#endif

#if(CDD_I2c_SW_PATCH_VERSION != 0U)
#error "CDD_I2c_SW_PATCH_VERSION does not match."
#endif

/* AUTOSAR Secification File Version Check */
#ifndef CDD_I2c_AR_RELEASE_MAJOR_VERSION
#error "CDD_I2c_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

/* AUTOSAR Major release version check  */
#if(CDD_I2c_AR_RELEASE_MAJOR_VERSION != 4U)
#error "CDD_I2c_AR_RELEASE_MAJOR_VERSION does not match."
#endif

#ifndef CDD_I2c_AR_RELEASE_MINOR_VERSION
#error "CDD_I2c_AR_RELEASE_MINOR_VERSION is not defined."
#endif

/* AUTOSAR Minor release version check  */
#if(CDD_I2c_AR_RELEASE_MINOR_VERSION != 3U)
#error "CDD_I2c_AR_RELEASE_MINOR_VERSION does not match."
#endif

#ifndef CDD_I2c_AR_RELEASE_PATCH_VERSION
#error "CDD_I2c_AR_RELEASE_PATCH_VERSION is not defined."
#endif

/* AUTOSAR Minor release version check  */
#if(CDD_I2c_AR_RELEASE_PATCH_VERSION != 1U)
#error "CDD_I2c_AR_RELEASE_PATCH_VERSION does not match."
#endif

/**************************************************************************/



/******************************************************************************
*                       Private Variable Definitions
******************************************************************************/
#define I2C_START_SEC_VAR_INIT_8
/* Initially made I2c UNINIT,once I2c_Init calls it becomes TRUE */
static boolean I2cInitialised = FALSE;

#define I2C_STOP_SEC_VAR_INIT_8


#define I2C_START_SEC_CODE

/******************************************************************************
*                    Private Function Declaration
******************************************************************************/
static void I2c_lclParamCheck(boolean Expr,
                               uint8 ServiceApiId,
                               uint8 ErrorCodeId,
                               Std_ReturnType *ReturnValue);


/******************************************************************************
*                         Public Functions                                    *
******************************************************************************/
/******************************************************************************
*  Service name      : CDD_I2c_Init
*  Syntax            : void CDD_I2c_Init(const I2c_ConfigType* CfgPtr)
*  Service ID[hex]   : 0x00
*  Sync/Async      	 : Synchronous
*  Reentrancy        : Non reentrant
*  Parameters (in)   : CfgPtr - Pointer to configtype structure
*  Parameters(inout) : None
*  Parameters (out)  : None
*  Return value      : None
*  Description       : Service for initialization of I2c
******************************************************************************/
void CDD_I2c_Init(const I2c_ConfigType *CfgPtr)
{
    Std_ReturnType ReturnValue = E_OK;

#if (I2C_DEV_ERROR_DETECT == STD_ON)
    /* I2c_FSR_01 */
    /* Check initial conditions cfgptr is null or not */
    I2c_lclParamCheck((boolean)(CfgPtr == NULL_PTR),
                        I2c_INIT_SERVICE_ID,
                        I2c_E_PARAM_POINTER,
                        &ReturnValue);

    /* I2c_FSR_02 */
    /* Check if I2c_Init initialized already or not */
    I2c_lclParamCheck((boolean)(I2cInitialised == TRUE),
                        I2c_INIT_SERVICE_ID,
                        I2c_E_ALREADY_INITIALIZED,
                        &ReturnValue);
#endif

    if (ReturnValue == E_OK)
    {
        I2c_Hal_Init(CfgPtr);
        I2cInitialised = TRUE;
    }
    else
    {}  //Misra-C


} /* end of CDD_I2c_Init API */


/******************************************************************************
*  Service name    : CDD_I2c_DeInit
*  Syntax          : Std_ReturnType CDD_I2c_DeInit(void)
*  Service ID      : 0x01
*  Sync/Async      : Synchronous
*  Reentrancy      : Non reentrant
*  Parameter (in)  : None
*  Parameters (out): None
*  Return value    : E_OK - De-initialization command accepted
*                    E_NOT_OK - De-initialization command has not been accepted
*  Description     : Service for Deinitialization of I2C
******************************************************************************/
Std_ReturnType CDD_I2c_DeInit(void)
{
    Std_ReturnType ReturnValue = E_OK ;

#if (I2C_DEV_ERROR_DETECT == STD_ON)
    /* Check if I2c_Init initialised already or not */
    I2c_lclParamCheck((boolean)(I2cInitialised == FALSE),
                        I2c_DEINIT_SERVICE_ID,
                        I2c_E_UNINIT,
                        &ReturnValue);
#endif

    /* Once condition is true start I2c_DeInit function in HAL layer */
    if (E_OK == ReturnValue)
    {
        I2c_Hal_DeInit();
        I2cInitialised = FALSE;
    }
    else
    {}
    return ReturnValue;

} /* end of CDD_I2c_DeInit API */

/******************************************************************************
*  Service name       : CDD_I2c_SetSlaveAddress
*  Syntax             : void CDD_I2c_SetSlaveAddress(I2c_AddrType* addr_info  )
*  Service ID         : 0x02
*  Sync/Async      	  : Synchronous
*  Reentrancy         : Non reentrant
*  Parameter (in)     : addr_info - Structure of the I2C address related
*
*  Parameter (inout)  : None
*  Parameters (out)   : None
*  Return value       : E_OK - Transmit command accepted
*                       E_NOT_OK - Transmit command not accepted
*  Description        : Service for I2c Slave address
*******************************************************************************************/
Std_ReturnType CDD_I2c_SetSlaveAddress(I2c_AddrType* addr_info  )
{
	Std_ReturnType  ReturnValue = E_OK;
		 I2c_StatusType Status;

	#if (I2C_DEV_ERROR_DETECT == STD_ON)
	    /* Check if I2c_Init initialised already or not */
	    I2c_lclParamCheck((boolean)(I2cInitialised == FALSE),
	    					I2c_TRANSMIT_SERVICE_ID,
							I2c_E_UNINIT,
	                        &ReturnValue);

	    /* Check TxBuf is null or not */
	    I2c_lclParamCheck((boolean)(addr_info == NULL_PTR),
	    					I2c_TRANSMIT_SERVICE_ID,
							I2c_E_PARAM_POINTER,
	                        &ReturnValue);
	#endif

	    if (ReturnValue == E_OK)
	    {
	        Status = I2c_Hal_GetStatus();
	        if (I2C_STATUS_IDLE == Status)
	        {
	        	I2c_Hal_SetSlaveAddress(addr_info);
	        }
	        else
	        {
	        	ReturnValue = E_NOT_OK;
	        }
	    }
	    else
	    {}

	    return ReturnValue;
}

/******************************************************************************
*  Service name       : CDD_I2c_Transmit
*  Syntax             : Std_ReturnType CDD_I2c_Transmit( uint8 TxBuf, boolean Restart ,boolean Stop_F, boolean CMD, uint8* RxBuf)
*  Service ID         : 0x03
*  Sync/Async      	  : Synchronous
*  Reentrancy         : Non reentrant
*  Parameter (in)     : TxBuf : data to be written
*                       Restart: Restart flag
*                       Stop: Stop flag
*                       CDM: write/read
*  Parameter (inout)  : None
*  Parameters (out)   : Rxbuf: buffer to get data from the slave
*
*  Return value       : E_OK - Receive command accepted
*                       E_NOT_OK - Receive command not accepted
*  Description        : Service for I2C transmit data and receive
*******************************************************************************************/
Std_ReturnType CDD_I2c_Transmit(uint8 TxBuf, I2c_RestartType Restart ,I2c_StopType Stop_F, I2c_CmdType CMD, uint8* RxBuf)
{
	 Std_ReturnType  ReturnValue = E_OK;
	 I2c_StatusType Status;

#if (I2C_DEV_ERROR_DETECT == STD_ON)
    /* Check if I2c_Init initialised already or not */
    I2c_lclParamCheck((boolean)(I2cInitialised == FALSE),
    					I2c_TRANSMIT_SERVICE_ID,
						I2c_E_UNINIT,
                        &ReturnValue);

    /* Check TxBuf is null or not */
    I2c_lclParamCheck((boolean)(RxBuf == NULL_PTR),
    					I2c_TRANSMIT_SERVICE_ID,
						I2c_E_PARAM_POINTER,
                        &ReturnValue);
#endif

    if (ReturnValue == E_OK)
    {
        Status = I2c_Hal_GetStatus();
        if (I2C_STATUS_IDLE == Status)
        {
        	ReturnValue = I2c_Hal_Transmit(TxBuf,  Restart , Stop_F,  CMD, RxBuf);
        }
        else
        {
            ReturnValue = E_NOT_OK;
        }
    }
    return ReturnValue;
}

/******************************************************************************
*                         Private Function Definitions
******************************************************************************/

/**************************************************************************************
*  Service name      : I2c_lclParamCheck
*  Syntax            : I2c_lclParamCheck(boolean Expr,
                                uint8 ServiceApiId,
                                uint8 ErrorCodeId,
                                Std_ReturnType *ReturnValue)
*  Parameters (in)   : Expr - boolean expression to check API conditions
*                      ServiceApiId - Service Id of API
*                      ErrorCodeId - Error Id of Condition
*  Parameters(inout) : ReturnValue - variable to check return type
*  Parameters (out)  : None
*  Return value      : None
*  Description       : Service for local parameter checking of I2c
***************************************************************************************/
static void  I2c_lclParamCheck(boolean Expr,
                                uint8 ServiceApiId,
                                uint8 ErrorCodeId,
                                Std_ReturnType *ReturnValue)
{
    if ((TRUE == Expr) && (*ReturnValue == E_OK))
    {
        #if ( I2C_DEV_ERROR_DETECT == STD_ON )
        (void)Det_ReportError(CDD_I2c_MODULE_ID,
                              CDD_I2c_INSTANCE_ID,
                              ServiceApiId,
                              ErrorCodeId);
        #endif
        *ReturnValue = E_NOT_OK;
    }
} /* end of I2c_lclParamCheck */

#define I2C_STOP_SEC_CODE
