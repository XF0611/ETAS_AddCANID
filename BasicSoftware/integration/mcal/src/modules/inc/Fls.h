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


#ifndef INC_MCAL_FLS_H_
#define INC_MCAL_FLS_H_

/* Includes */
#include "Fls_Cfg.h"
#include "Std_Types.h"

#define FLS_INIT_SID                    0x00u   /* Service Id for Fls_Init API */
#define FLS_ERASE_SID 				    0x01u   /* Service Id for Fls_Erase API */
#define FLS_WRITE_SID 				    0x02u   /* Service Id for Fls_Write API */
#define FLS_CANCEL_SID 				    0x03u   /* Service Id for Fls_Cancel API */
#define FLS_GETSTATUS_SID 				0x04u   /* Service Id for Fls_GetStatus API */
#define FLS_GETJOBRESULT_SID 			0x05u   /* Service Id for Fls_GetJobResult API */
#define FLS_MAINFUNCTION_SID 		    0x06u   /* Service Id for Fls_MainFunction API */
#define FLS_READ_SID 				    0x07u   /* Service Id for Fls_Read API */
#define FLS_COMPARE_SID 				0x08u   /* Service Id for Fls_Compare API */
#define FLS_SETMODE_SID 			    0x09u   /* Service Id for Fls_SetMode API */
#define FLS_GETVERSIONINFO_SID 			0x10u   /* Service Id for Fls_GetVersionInfo API */
#define FLS_BLANKCHECK_SID 				0x0au   /* Service Id for Fls_BlankCheck API */


/* Macro definitions for Error Codes specified by AUTOSAR */
/*Development errors*/
#define FLS_E_PARAM_CONFIG             0x01u   /* API service called with wrong parameter*/
#define FLS_E_PARAM_ADDRESS            0x02u   /* API service called with wrong parameter*/
#define FLS_E_PARAM_LENGTH             0x03u   /* API service called with wrong parameter*/
#define FLS_E_PARAM_DATA               0x04u   /* API service called with wrong parameter*/
#define FLS_E_UNINIT                   0x05u   /* API service called without module initialization*/
#define FLS_E_BUSY                     0x06u   /* API service called while driver still busy*/
#define FLS_E_PARAM_POINTER            0x0au   /* API service called with NULL pointer*/

/*Transient Fault*/
#define FLS_E_ERASE_FAILED             0x01u   /* Flash erase failed (HW)*/
#define FLS_E_WRITE_FAILED             0x02u   /* Flash write failed (HW)*/
#define FLS_E_READ_FAILED              0x03u   /* Flash read failed (HW)*/
#define FLS_E_COMPARE_FAILED           0x04u   /* Flash compare failed (HW)*/
#define FLS_E_UNEXPECTED_FLASH_ID      0x05u   /* Expected hardware ID not matched*/

/*Runtime Errors*/
#define FLS_E_VERIFY_ERASE_FAILED      0x07u   /* Erase verification (blank check) failed */
#define FLS_E_VERIFY_WRITE_FAILED      0x08u   /* Write verification (compare) failed */
#define FLS_E_TIMEOUT              	   0x09u   /* Timeout exceeded */

/*ETAS BIP Porting*/
#define FLS_ERASE_PATTERN              255u

/* pointer to function type*/
typedef void (*FUNPOINTER)(void);

/* SWS_Fls_00369 */
typedef uint32 Fls_AddressType;

/* SWS_Fls_00370 */
typedef uint32 Fls_LengthType;


typedef struct {
	uint16 FlsNumberOfSectors;				/* Number of continuous sectors with identical values for FlsSectorSize and FlsPageSize. The parameter FlsSectorStartAddress denotes the start address of the first sector. */
	Fls_LengthType FlsPageSize;             /* Size of one page of this sector */
	Fls_LengthType FlsSectorSize;			/* Size of this sector */
	Fls_AddressType FlsSectorStartaddress;  /* Start address of this sector */
}FlsSector;

typedef uint8 FlsExternalDriver;

/*SWS_Fls_00352 , SWS_Fls_00353,  SWS_Fls_00368, SWS_Fls_00354 */
typedef struct {
	uint32 FlsAcErase;   					/* Address offset in RAM to which the erase flash access code shall be loaded */

	uint32 FlsAcWrite;          			/* Address offset in RAM to which the write flash access code shall be loaded */

	uint32 FlsCallCycle;                	/* Cycle time of calls of the flash driver's main function (in seconds) Only relevant if deadline monitoring for internal functionality has to be done in software */

	MemIf_ModeType FlsDefaultMode;			/* Default FLS device mode after initialization */

	FUNPOINTER FlsJobEndNotification;   		/* Mapped to the job end notification routine provided by some upper layer module, typically the Fee module.*/

	FUNPOINTER FlsJobErrorNotification;			/* Mapped to the job error notification routine provided by some upper layer module, typically the Fee module.*/

	uint32 FlsMaxReadFastMode;              /* The maximum number of bytes to read or compare in one cycle of the flash driver's job processing function in fast mode. */

	uint32 FlsMaxReadNormalMode;			/* The maximum number of bytes to read or compare in one cycle of the flash driver's job processing function in normal mode. */

	uint32 FlsMaxWriteFastMode;             /* The maximum number of bytes to write in one cycle of the flash driver's job processing function in fast mode. */

	uint32 FlsMaxWriteNormalMode;           /* The maximum number of bytes to write in one cycle of the flash driver's job processing function in normal mode. */

	uint32 FlsProtection;                   /* Erase/write protection settings. Only relevant if supported by hardware */

	FlsExternalDriver* ExternalDriver ;      /* This container is present for external Flash drivers only.  */

	FlsSector* SectorList;				/* List of flash-able sectors and pages */

	/*SWS_Fls_00355*/

} Fls_ConfigType;



extern const Fls_ConfigType  Fls_Config;



/* Public Function Declarations */
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
void Fls_Init(const Fls_ConfigType* ConfigPtr);

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
Std_ReturnType Fls_Erase(Fls_AddressType TargetAddress,Fls_LengthType Length);

/******************************************************************************
*  Service name    : Fls_Write
*  Syntax          : Std_ReturnType Fls_Write(Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length)
*  Service ID      : 0x02
*  Sync/Async      : Asynchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : TargetAddress : Target address in flash memory. This address offset will be added to the flash memory base address.
 	 	 	 	 	 Length: Number of bytes to write
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : Writes one or more complete flash pages.
******************************************************************************/
Std_ReturnType Fls_Write(Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length);

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
void Fls_Cancel(void);
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
MemIf_StatusType Fls_GetStatus(void);
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
MemIf_JobResultType Fls_GetJobResult(void);
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
Std_ReturnType Fls_Read(Fls_AddressType SourceAddress,uint8* TargetAddressPtr,Fls_LengthType Length);

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
Std_ReturnType Fls_Compare(Fls_AddressType SourceAddress,const uint8* TargetAddressPtr,Fls_LengthType Length);
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
void Fls_SetMode(MemIf_ModeType Mode);
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
void Fls_GetVersionInfo(Std_VersionInfoType* VersioninfoPtr);

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
Std_ReturnType Fls_BlankCheck(Fls_AddressType TargetAddress,Fls_LengthType Length);


/******************************************************************************
*  Service name    : Fls_MainFunction
*  Syntax          : void Fls_MainFunction(void)
*  Service ID      : 0x06
*  Description     : Performs the processing of jobs.
******************************************************************************/
void Fls_MainFunction(void);






#endif /* INC_MCAL_FLS_H_ */
