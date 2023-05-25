/************************************************************************************
*  Copyright (c) 2020, Calterah, Inc. All rights reserved.
*
*  Flash driver is External Flash driver, will use QSPI driver of Alps Inc.
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
#include "Fls.h"
#include "Fls_Alps.h"
//#include "SchM_Fls.h"

#define FLS_START_SEC_VAR_NO_INIT_UNSPECIFIED


#define FLS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED


#if(FLS_DEV_ERROR_DETECT == STD_ON)
/* SWS_Fls_00260 SWS_Fls_00261 */
#include "Det.h"
#endif

#define FLS_START_SEC_VAR_INIT_8

static boolean FlsInitialized = FALSE;
#define FLS_STOP_SEC_VAR_INIT_8


#define FLS_START_SEC_CODE


/*************************version check************************************/
#ifndef FLS_SW_MAJOR_VERSION
#error "FLS_SW_MAJOR_VERSION is not defined."
#endif

#ifndef FLS_SW_MINOR_VERSION
#error "FLS_SW_MINOR_VERSION is not defined."
#endif

#ifndef FLS_SW_PATCH_VERSION
#error "FLS_SW_PATCH_VERSION is not defined."
#endif

#if(FLS_SW_MAJOR_VERSION != 3U)
#error "FLS_SW_MAJOR_VERSION does not match."
#endif

#if(FLS_SW_MINOR_VERSION != 4U)
#error "FLS_SW_MINOR_VERSION does not match."
#endif

#if(FLS_SW_PATCH_VERSION != 0U)
#error "FLS_SW_PATCH_VERSION does not match."
#endif

/* AUTOSAR Secification File Version Check */
#ifndef FLS_AR_RELEASE_MAJOR_VERSION
#error "FLS_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

/* AUTOSAR Major release version check  */
#if(FLS_AR_RELEASE_MAJOR_VERSION != 4U)
#error "FLS_AR_RELEASE_MAJOR_VERSION does not match."
#endif

#ifndef FLS_AR_RELEASE_MINOR_VERSION
#error "FLS_AR_RELEASE_MINOR_VERSION is not defined."
#endif

/* AUTOSAR Minor release version check  */
#if(FLS_AR_RELEASE_MINOR_VERSION != 3U)
#error "FLS_AR_RELEASE_MINOR_VERSION does not match."
#endif

#ifndef FLS_AR_RELEASE_PATCH_VERSION
#error "FLS_AR_RELEASE_PATCH_VERSION is not defined."
#endif

/* AUTOSAR Minor release version check  */
#if(FLS_AR_RELEASE_PATCH_VERSION != 1U)
#error "FLS_AR_RELEASE_PATCH_VERSION does not match."
#endif

/**************************************************************************/

/******************************************************************************
*  Service name    : Fls_Init
*  Syntax          : void Fls_Init(const Fls_ConfigType* ConfigPtr)
*  Service ID      : 0x00
*  Sync/Async      : Synchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : ConfigPtr - Pointer to flash driver configuration set
*  Parameters (out): None
*  Return value    : None
*  Description     : Initializes the Flash Driver.
******************************************************************************/
void Fls_Init(const Fls_ConfigType* ConfigPtr)
{
	Std_ReturnType RevtVal = E_OK;

    /* SWS_Fls_00015 */
	Fls_lclParamCheck((boolean)(NULL_PTR == ConfigPtr),
    					FLS_INIT_SID,
						FLS_E_PARAM_POINTER,
                        &RevtVal);

    if (E_OK == RevtVal)
    {
        if(FlsInitialized == FALSE)
        {
        	Fls_Hal_Init(ConfigPtr);
        }
        else
        {
        	/*compliant to misra-c*/
        }
        FlsInitialized = TRUE ;
    }
    else
    {}
}

/******************************************************************************
*  Service name    : Fls_Erase
*  Syntax          : Std_ReturnType Fls_Erase(Fls_AddressType TargetAddress,Fls_LengthType Length)
*  Service ID      : 0x01
*  Sync/Async      : Asynchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : TargetAddress : Target address in flash memory. This address offset will be added to the flash memory base address.
 	 	 	 	 	 Length: Number of bytes to erase
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : Erases flash sector(s)
******************************************************************************/
Std_ReturnType Fls_Erase(Fls_AddressType TargetAddress,Fls_LengthType Length)
{
	Std_ReturnType RevtVal = E_OK;

	RevtVal = Fls_Hal_Erase( TargetAddress, Length);

	return RevtVal;
}

Std_ReturnType Fls_Erase_FBL(Fls_AddressType TargetAddress,Fls_LengthType Length)
{
	Std_ReturnType RevtVal = E_OK;

	RevtVal = Fls_Hal_Erase_FBL( TargetAddress, Length);

	return RevtVal;
}

/******************************************************************************
*  Service name    : Fls_Write
*  Syntax          : Std_ReturnType Fls_Write(Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length)
*  Service ID      : 0x02
*  Sync/Async      : Asynchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : TargetAddress : Target address in flash memory. This address offset will be added to the flash memory base address.
*  					 SourceAddressPtr: Pointer to the data to be written to the Flash
 	 	 	 	 	 Length: Number of bytes to write
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : Writes one or more complete flash pages.
******************************************************************************/
Std_ReturnType Fls_Write(Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length)
{
	Std_ReturnType RevtVal = E_OK;

	/*SWS_Fls_00157*/
	Fls_lclParamCheck((boolean)(NULL_PTR == SourceAddressPtr),
						FLS_WRITE_SID,
						FLS_E_PARAM_DATA,
                        &RevtVal);


    if (E_OK == RevtVal)
    {
    	RevtVal = Fls_Hal_Write( TargetAddress, SourceAddressPtr, Length);
    }
    else
    {}

	return RevtVal;
}

Std_ReturnType Fls_Write_FBL(Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length)
{
	Std_ReturnType RevtVal = E_OK;

	/*SWS_Fls_00157*/
	Fls_lclParamCheck((boolean)(NULL_PTR == SourceAddressPtr),
						FLS_WRITE_SID,
						FLS_E_PARAM_DATA,
                        &RevtVal);


    if (E_OK == RevtVal)
    {
    	RevtVal = Fls_Hal_Write_FBL( TargetAddress, SourceAddressPtr, Length);
    }
    else
    {}

	return RevtVal;
}

/*SWS_Fls_00183*/
#if (FlsCancelApi == STD_ON)
/******************************************************************************
*  Service name    : Fls_Cancel
*  Syntax          : void Fls_Cancel(void)
*  Service ID      : 0x03
*  Sync/Async      : Synchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : Cancels an ongoing job.
******************************************************************************/
void Fls_Cancel(void)
{
	//not supported by the hardware
}
#endif

/*SWS_Fls_00184*/
#if (FlsGetStatusApi == STD_ON)
/******************************************************************************
*  Service name    : Fls_GetStatus
*  Syntax          : MemIf_StatusType Fls_GetStatus(void)
*  Service ID      : 0x04
*  Sync/Async      : Synchronous
*  Reentrancy      : Reentrant
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : MemIf_StatusType
*  Description     : Returns the driver state.
******************************************************************************/
MemIf_StatusType Fls_GetStatus(void)
{
	MemIf_StatusType RevtVal;

	RevtVal = Fls_Hal_GetStatus();

	return RevtVal;
}
#endif

/*SWS_Fls_00185*/
#if (FlsGetJobResultApi == STD_ON)
/******************************************************************************
*  Service name    : Fls_GetJobResult
*  Syntax          : MemIf_JobResultType Fls_GetJobResult(void)
*  Service ID      : 0x05
*  Sync/Async      : Synchronous
*  Reentrancy      : Reentrant
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : MemIf_JobResultType
*  Description     : Returns the result of the last job.
******************************************************************************/
MemIf_JobResultType Fls_GetJobResult(void)
{
	MemIf_JobResultType RevtVal;

	/* SWS_Fls_00358*/
	#if (FLS_DEV_ERROR_DETECT == STD_ON)
	if(FlsInitialized == FALSE)
	{
		(void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_GETJOBRESULT_SID, FLS_E_UNINIT);
		RevtVal =  MEMIF_JOB_FAILED;
	}
	else
	{
	#endif
		RevtVal = Fls_Hal_GetJobResult();
	#if (FLS_DEV_ERROR_DETECT == STD_ON)
	}
	#endif

	return RevtVal;
}
#endif

/******************************************************************************
*  Service name    : Fls_Read
*  Syntax          : Std_ReturnType Fls_Read(Fls_AddressType SourceAddress,uint8* TargetAddressPtr,Fls_LengthType Length)
*  Service ID      : 0x07
*  Sync/Async      : Asynchronous
*  Reentrancy      : Non Reentrant
*  Parameters (in) : SourceAddress:Source address in flash memory. This address offset will be added to the flash memory base address.
*  					 Length:Number of bytes to read
*
*  Parameters (out): TargetAddressPtr : Pointer to target data buffer.
*  Return value    : Std_ReturnType
*  Description     : Reads from flash memory.
******************************************************************************/
Std_ReturnType Fls_Read(Fls_AddressType SourceAddress,uint8* TargetAddressPtr,Fls_LengthType Length)
{
	Std_ReturnType RevtVal = E_OK;

	/* SWS_Fls_00158 */
	Fls_lclParamCheck((boolean)(NULL_PTR == TargetAddressPtr),
						FLS_READ_SID,
						FLS_E_PARAM_DATA,
                        &RevtVal);

    if (E_OK == RevtVal)
    {
    	RevtVal = Fls_Hal_Read(SourceAddress,TargetAddressPtr,Length);
    }
    else
    {}

	return RevtVal;
}

Std_ReturnType Fls_Read_FBL(Fls_AddressType SourceAddress,uint8* TargetAddressPtr,Fls_LengthType Length)
{
	Std_ReturnType RevtVal = E_OK;

	/* SWS_Fls_00158 */
	Fls_lclParamCheck((boolean)(NULL_PTR == TargetAddressPtr),
						FLS_READ_SID,
						FLS_E_PARAM_DATA,
                        &RevtVal);

    if (E_OK == RevtVal)
    {
    	RevtVal = Fls_Hal_Read_FBL(SourceAddress,TargetAddressPtr,Length);
    }
    else
    {}

	return RevtVal;
}

/*SWS_Fls_00186*/
#if (FlsCompareApi == STD_ON)
/******************************************************************************
*  Service name    : Fls_Compare
*  Syntax          : Std_ReturnType Fls_Compare(Fls_AddressType SourceAddress,const uint8* TargetAddressPtr,Fls_LengthType Length)
*  Service ID      : 0x08
*  Sync/Async      : Asynchronous
*  Reentrancy      : Non Reentrant
*  Parameters (in) : SourceAddress:Source address in flash memory. This address offset will be added to the flash memory base address.
*  					 TargetAddressPtr:Pointer to target data buffer
*  					 Length:Number of bytes to read
*
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : Compares the contents of an area of flash memory with that of an application data buffer.
******************************************************************************/
Std_ReturnType Fls_Compare(Fls_AddressType SourceAddress,const uint8* TargetAddressPtr,Fls_LengthType Length)
{
	Std_ReturnType RevtVal = E_OK;

	/* SWS_Fls_00152 */
	#if (FLS_DEV_ERROR_DETECT == STD_ON)
	if(FlsInitialized == FALSE)
	{
		(void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANKCHECK_SID, FLS_E_UNINIT);
		RevtVal =  E_NOT_OK;
	}
	else
	{
	#endif
		/* SWS_Fls_00273 */
		Fls_lclParamCheck((boolean)(NULL_PTR == TargetAddressPtr),
							FLS_COMPARE_SID,
							FLS_E_PARAM_DATA,
							&RevtVal);

		if (E_OK == RevtVal)
		{
			RevtVal = Fls_Hal_Compare( SourceAddress, TargetAddressPtr, Length);
		}
		else
		{}
	#if (FLS_DEV_ERROR_DETECT == STD_ON)
	}
	#endif

	return RevtVal;
}
#endif

/*SWS_Fls_00187*/
#if (FlsSetModeApi == STD_ON)
/******************************************************************************
*  Service name    : Fls_SetMode
*  Syntax          : void Fls_SetMode(MemIf_ModeType Mode)
*  Service ID      : 0x09
*  Sync/Async      : Synchronous
*  Reentrancy      : Non Reentrant
*  Parameters (in) : Mode:MEMIF_MODE_SLOW: Slow read access / normal SPI access.
						  MEMIF_MODE_FAST: Fast read access / SPI burst access.
*
*  Parameters (out): None
*  Return value    : None
*  Description     : Sets the flash driver's operation mode.
******************************************************************************/
void Fls_SetMode(MemIf_ModeType Mode)
{
	 Fls_Hal_SetMode(Mode);
}
#endif

/******************************************************************************
*  Service name    : Fls_GetVersionInfo
*  Syntax          : void Fls_GetVersionInfo(Std_VersionInfoType* VersioninfoPtr)
*  Service ID      : 0x10
*  Sync/Async      : Synchronous
*  Reentrancy      : Reentrant
*  Parameters (in) : None
*  Parameters (out): VersioninfoPtr:Pointer to where to store the version information of this module.
*  Return value    : None
*  Description     : Returns the version information of this module.
******************************************************************************/
void Fls_GetVersionInfo(Std_VersionInfoType* VersioninfoPtr)
{
	Std_ReturnType RevtVal = E_OK;

	/* SWS_Fls_00363 */
	Fls_lclParamCheck((boolean)(NULL_PTR == VersioninfoPtr),
						FLS_GETVERSIONINFO_SID,
						FLS_E_PARAM_POINTER,
                        &RevtVal);
    if (E_OK == RevtVal)
    {
    	VersioninfoPtr->vendorID 		 = FLS_VENDOR_ID ;
    	VersioninfoPtr->moduleID 		 = FLS_MODULE_ID ;
    	VersioninfoPtr->sw_major_version = FLS_SW_MAJOR_VERSION ;
		VersioninfoPtr->sw_minor_version = FLS_SW_MINOR_VERSION ;
		VersioninfoPtr->sw_patch_version = FLS_SW_PATCH_VERSION ;
    }
    else
    {}
}

/******************************************************************************
*  Service name    : Fls_BlankCheck
*  Syntax          : Std_ReturnType Fls_BlankCheck(Fls_AddressType TargetAddress,Fls_LengthType Length)
*  Service ID      : 0x0a
*  Sync/Async      : Asynchronous
*  Reentrancy      : Non Reentrant
*  Parameters (in) : TargetAddress:Address in flash memory from which the blank check should be started.
*  					 Length:Number of bytes to be checked for erase pattern.
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : The function Fls_BlankCheck shall verify, whether a given memory area has been
					 erased but not (yet) programmed. The function shall limit the maximum number of
				     checked flash cells per main function cycle to the configured value
					 FlsMaxReadNormalMode or FlsMaxReadFastMode respectively.
******************************************************************************/
Std_ReturnType Fls_BlankCheck(Fls_AddressType TargetAddress,Fls_LengthType Length)
{
	Std_ReturnType RevtVal = E_OK;
	/*SWS_Fls_00382*/
	#if (FLS_DEV_ERROR_DETECT == STD_ON)
	if(FlsInitialized == FALSE)
	{
		(void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANKCHECK_SID, FLS_E_UNINIT);
		RevtVal =  E_NOT_OK;
	}
	else
	{
	#endif

	   RevtVal = Fls_Hal_BlankCheck( TargetAddress, Length);

	#if (FLS_DEV_ERROR_DETECT == STD_ON)
	}
	#endif

	return RevtVal;
}

/* SWS_Fls_00255 SWS_Fls_00235 */
/******************************************************************************
*  Service name    : Fls_MainFunction
*  Syntax          : void Fls_MainFunction(void)
*  Service ID      : 0x06
*  Description     : Performs the processing of jobs.
******************************************************************************/
void Fls_MainFunction(void)
{
	/* SWS_Fls_00117 */
	#if (FLS_DEV_ERROR_DETECT == STD_ON)
	if(FlsInitialized == FALSE)
	{
		(void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_MAINFUNCTION_SID, FLS_E_UNINIT);
	}
	else
	{
	#endif

		/*SWS_Fls_00220 , SWS_Fls_00225 , SWS_Fls_00238, SWS_Fls_00243 , SWS_Fls_00378 */
		Fls_Hal_MainFunction_Handling();

	#if (FLS_DEV_ERROR_DETECT == STD_ON)
	}
	#endif
}

#define FLS_STOP_SEC_CODE

