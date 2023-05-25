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

#ifndef MCU_H
#define MCU_H

/* Includes */
#include "Mcu_Cfg.h"
#include "Std_Types.h"

/* Macro definitions for API Service ID */
#define MCU_INIT_SID                 0x00 /* Service ID for MCU_Init API */
#define MCU_INITRAMSECTION_SID       0x01 /* Service ID for MCU_InitRamSection API */
#define MCU_INITCLOCK_SID            0x02 /* Service ID for MCU_InitClock API */
#define MCU_DISTRIBUTEPLLCLK_SID     0x03 /* Service ID for MCU_DistributePllClock API */
#define MCU_GETPLLSTATUS_SID         0x04 /* Service ID for MCU_GetPllStatus API */
#define MCU_GETRESETREASON_SID       0x05 /* Service ID for MCU_GetResetReason API */
#define MCU_GETRESETRAWVAL_SID       0x06 /* Service ID for MCU_GetResetRawValue API */
#define MCU_PERFORMRESET_SID         0x07 /* Service ID for MCU_PerformReset API */
#define MCU_SETMODE_SID              0x08 /* Service ID for MCU_SetMode API */
#define MCU_GETVERSIONINFO_SID       0x09 /* Service ID for MCU_GetVersionInfo API */
#define MCU_GETRAMSTATE_SID          0x0A /* Service ID for MCU_GetRamState API */
#define MCU_GETPERIPHERALCLKFREQ_SID 0x0B /* Service ID for MCU_GetPeripheralClkFreq API */

/* Macro definitions for Error Codes specified by AUTOSAR */
#define MCU_E_PARAM_CONFIG         0x0A /* API service called with wrong parameter */
#define MCU_E_PARAM_CLOCK          0x0B /* API service called with wrong parameter */
#define MCU_E_PARAM_MODE           0x0C /* API service called with wrong parameter */
#define MCU_E_PARAM_RAMSECTION     0x0D /* API service called with wrong parameter */
#define MCU_E_PLL_NOT_LOCKED       0x0E /* API service called when PLL status is not locked */
#define MCU_E_UNINIT               0x0F /* API service used without module initialisation */
#define MCU_E_PARAM_POINTER        0x10 /* APIs called with a Null Pointer */
#define MCU_E_INIT_FAILED          0x11 /* API service used when initalisation failed */

/* General defines */
#define RESET_VAL_UNDEFINED        0xFF

/* enum definitions */
/* SWS_Mcu_00250 */
typedef enum {
    MCU_PLL_LOCKED,
    MCU_PLL_UNLOCKED,
    MCU_PLL_STATUS_UNDEFINED
} Mcu_PllStatusType;

/* SWS_Mcu_00252 */
typedef enum {
    MCU_POWER_ON_RESET,
    MCU_RESET_UNDEFINED
} Mcu_ResetType;

/* SWS_Mcu_00256 */
#if ( MCU_GET_RAM_STATUS_API == STD_ON )
typedef enum {
    MCU_RAMSTATE_INVALID,
    MCU_RAMSTATE_VALID
} Mcu_RamStateType;
#endif

/* These enums are used as argument to the Mcu_GetPeripheralClkFreq API */
typedef enum {
    MCU_SYSTEM_REF_CLOCK,	
    MCU_UART0_CLOCK,
	MCU_UART1_CLOCK,
	MCU_SPI_M0_CLOCK,
	MCU_SPI_M1_CLOCK,
	MCU_SPI_S_CLOCK,
	MCU_QSPI_CLOCK,
	MCU_GPIO_CLOCK,
	MCU_CAN0_CLOCK,
	MCU_CAN1_CLOCK,
} Mcu_PeriphNameType;

/* SWS_Mcu_00251 */
typedef uint8 Mcu_ClockType;

/* SWS_Mcu_00253 */
typedef uint32 Mcu_RawResetType;

/* SWS_Mcu_00254 */
typedef uint8 Mcu_ModeType;

/* SWS_Mcu_00255 */
typedef uint8 Mcu_RamSectionType;

/* Variable to store Peripheral clock value */
typedef uint32 Mcu_PeripheralClockValType;

/* Structure Definitions */
typedef struct {
    uint32 McuRamSectionBaseAddr;
    uint32 McuRamDefaultData;
    uint32 McuRamSectionSize;
} Mcu_RamSectionSettingConfigType;



typedef struct {
	boolean Can0ExtClockSource;
    boolean Can1ExtClockSource;
	uint32 CanExtClockFreq;
} Mcu_CanClockSourceConfigType;


typedef struct {
     uint8 ApbClkDiv;
     uint8 ApbClkRefDiv;
     uint8 MemClkDiv;
     uint8 AhbClkDiv;
     uint8 CpuClkDiv;
     uint8 Can0ClkDiv;
     uint8 Can1ClkDiv;
	 uint32 McuClockEnableConfig;
	 const Mcu_CanClockSourceConfigType *Mcu_CanClockSourceConfig;
} Mcu_ClockSettingConfigType;


typedef struct {
    uint8 McuRamSector;
    uint8 NumOfClockSetting;
	uint32 McuPeripheralResetConfig;
    const Mcu_RamSectionSettingConfigType *McuRamSettingConfig;
	const Mcu_ClockSettingConfigType      *McuClockSettingConfig;
} Mcu_ConfigType;

#define MCU_START_SEC_CONFIG_DATA_UNSPECIFIED

extern const Mcu_ConfigType McuConfigData;
#define MCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED

/* Public Function Declarations */
#define MCU_START_SEC_CODE
/******************************************************************************
*  Service name    : Mcu_Init                                                  
*  Syntax          : void Mcu_Init( const Mcu_ConfigType *ConfigPtr )          
*  Service ID      : 0x00                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : ConfigPtr - Pointer to MCU driver configuration set                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Service for MCU initialization                            
******************************************************************************/
void Mcu_Init(const Mcu_ConfigType *ConfigPtr);

/******************************************************************************
*  Service name    : Mcu_InitRamSection                                                
*  Syntax          : Std_ReturnType Mcu_InitRamSection( 
                                            Mcu_RamSectionType RamSection)                         
*  Service ID      : 0x01                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : RamSection - Selects RAM memory section provided in 
                                  configuration set.                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted         
*                    E_NOT_OK: command has not been accepted 
*  Description     : Service to initalize the RAM section wise.                        
******************************************************************************/
Std_ReturnType Mcu_InitRamSection(Mcu_RamSectionType RamSection);

#if (MCU_INIT_CLOCK == STD_ON)
/******************************************************************************
*  Service name    : Mcu_InitClock                                                
*  Syntax          : Std_ReturnType Mcu_InitClock( Mcu_ClockType ClockSetting )                         
*  Service ID      : 0x02                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : Clock Setting                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted         
*                    E_NOT_OK: command has not been accepted 
*  Description     : Service to initalize the PLL & other MCU clock options                        
******************************************************************************/
Std_ReturnType Mcu_InitClock(Mcu_ClockType ClockSetting);
#endif

#if (MCU_NO_PLL == STD_OFF)
/******************************************************************************
*  Service name    : Mcu_DistributePllClock                                                
*  Syntax          : Std_ReturnType Mcu_DistributePllClock( void )                         
*  Service ID      : 0x03                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted         
*                    E_NOT_OK: command has not been accepted 
*  Description     : Activates the PLL clock to the MCU clock distribution.                      
******************************************************************************/
Std_ReturnType Mcu_DistributePllClock(void);
#endif

/******************************************************************************
*  Service name    : Mcu_GetPllStatus                                                
*  Syntax          : Mcu_PllStatusType Mcu_GetPllStatus( void )                         
*  Service ID      : 0x04                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_PllStatusType - PLL status 
*  Description     : This service provides the lock status of the PLL.                      
******************************************************************************/
Mcu_PllStatusType Mcu_GetPllStatus(void);

/******************************************************************************
*  Service name    : Mcu_GetResetReason                                                
*  Syntax          : Mcu_ResetType Mcu_GetResetReason( void )                         
*  Service ID      : 0x05                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_ResetType
*  Description     : The service reads the reset type from the hardware.                      
******************************************************************************/
Mcu_ResetType Mcu_GetResetReason(void);

/******************************************************************************
*  Service name    : Mcu_GetResetRawValue                                                
*  Syntax          : Mcu_RawResetType Mcu_GetResetRawValue( void )                       
*  Service ID      : 0x06                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_RawResetType
*  Description     : The service reads the reset type from the hardware register.                      
********************************************************************************/
Mcu_RawResetType Mcu_GetResetRawValue(void);

#if (MCU_PERFORM_RESET_API == STD_ON)
/******************************************************************************
*  Service name    : Mcu_PerformReset                                                
*  Syntax          : void Mcu_PerformReset( void )                       
*  Service ID      : 0x07                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non-reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : The service performs a microcontroller reset.                      
********************************************************************************/
void Mcu_PerformReset(void);
#endif

/******************************************************************************
*  Service name    : Mcu_SetMode                                                
*  Syntax          : void Mcu_SetMode( Mcu_ModeType McuMode )                       
*  Service ID      : 0x08                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : McuMode : Set different MCU power modes configured.                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service activates the MCU power modes.                     
********************************************************************************/
void Mcu_SetMode(Mcu_ModeType McuMode);

#if (MCU_VERSION_INFO_API == STD_ON)
/******************************************************************************
*  Service name    : Mcu_GetVersionInfo                                                
*  Syntax          : void Mcu_GetVersionInfo(Std_VersionInfoType *versioninfo)                      
*  Service ID      : 0x09                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : versioninfo : Pointer to where to store the version 
                     information of this module.
*  Description     : This service returns the version information of this module.                     
********************************************************************************/
void Mcu_GetVersionInfo(Std_VersionInfoType *versioninfo);
#endif

#if (MCU_GET_RAM_STATUS_API == STD_ON)
/******************************************************************************
*  Service name    : Mcu_GetRamState                                                
*  Syntax          : Mcu_RamStateType Mcu_GetRamState( void )                      
*  Service ID      : 0x0A                                                    
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_RamStateType : Status of the Ram Content.
*  Description     : This service provides the actual status of the 
                     microcontroller Ram. (if supported)                     
********************************************************************************/
Mcu_RamStateType Mcu_GetRamState(void);
#endif

/**********************************************************************************
*  Service name    : Mcu_GetPeripheralClkFreq                                                
*  Syntax          : Mcu_PeripheralClockValType Mcu_GetPeripheralClkFreq(Mcu_PeriphNameType PeripheralName) 
*  Parameters (in) : PeripheralName                                                      
*  Parameters (out): None                                                   
*  Return value    : Mcu_PeripheralClockValType
*  Description     : It provides the peripheral clock values configured in MCU module.
************************************************************************************/
Mcu_PeripheralClockValType Mcu_GetPeripheralClkFreq(Mcu_PeriphNameType PeripheralName);


#define MCU_STOP_SEC_CODE

#endif
