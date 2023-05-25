/* ==========================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
========================================== */

#ifndef GPT_H
#define GPT_H

/* Includes */
#include "Gpt_Cfg.h"
#include "Std_Types.h"

#if ( GPT_WAKEUP_FUNCTIONALITY_API == STD_ON )
#include "EcuM_Cbk.h"
#endif

/* Macro definitions for API Service ID */
#define GPT_GETVERSIONINFO_SID          0x00 /* Service ID for Gpt_GetVersionInfo API */
#define GPT_INIT_SID                    0x01 /* Service ID for Gpt_Init API */
#define GPT_DEINIT_SID                  0x02 /* Service ID for Gpt_DeInit API */
#define GPT_GETTIMEELAPSED_SID          0x03 /* Service ID for Gpt_GetTimeElapsed API */
#define GPT_GETTIMEREMAINING_SID        0x04 /* Service ID for Gpt_GetTimeRemaining API */
#define GPT_STARTTIMER_SID              0x05 /* Service ID for Gpt_StartTimer API */
#define GPT_STOPTIMER_SID               0x06 /* Service ID for Gpt_StopTimer API */
#define GPT_ENABLENOTIFICATION_SID      0x07 /* Service ID for Gpt_EnableNotification API */
#define GPT_DISABLENOTIFICATION_SID     0x08 /* Service ID for Gpt_DisableNotification API */
#define GPT_SETMODE_SID                 0x09 /* Service ID for Gpt_SetMode API */
#define GPT_DISABLEWAKEUP_SID           0x0A /* Service ID for Gpt_DisableWakeup API */
#define GPT_ENABLEWAKEUP_SID            0x0B /* Service ID for Gpt_EnableWakeup API */
#define GPT_CHECKWAKEUP_SID             0x0C /* Service ID for Gpt_CheckWakeup API */
#define GPT_GETPREDEFTIMERVALUE_SID     0x0D /* Service ID for Gpt_GetPredefTimerValue API */

/* Macro definitions for Error Codes specified by AUTOSAR */
/* Development Errors */
#define GPT_E_UNINIT                    0x0A /* API service used without module initialisation */
#define GPT_E_ALREADY_INITIALIZED       0x0D /* API Gpt_Init service called while the GPT driver has already been initialized */
#define GPT_E_INIT_FAILED               0x0E /* API service used when initalisation failed */
#define GPT_E_PARAM_CHANNEL             0x14 /* API service called with wrong parameter */
#define GPT_E_PARAM_VALUE               0x15 /* API service called with wrong parameter */
#define GPT_E_PARAM_POINTER             0x16 /* APIs called with a Null Pointer */
#define GPT_E_PARAM_PREDEF_TIMER        0x17 /* API service called with wrong parameter */
#define GPT_E_PARAM_MODE                0x1F /* API service called with wrong parameter */
/* Runtime Errors */
#define GPT_E_BUSY                      0x0B /* API service called when timer channel is still busy (running) */
#define GPT_E_MODE                      0x0C /* API service called when driver is in wrong mode */

/* General Defines */
#define GPT_MAX_TIMER_RESOLUTION  (uint32)4294967295

/* enum definitions */
/* SWS_Gpt_00360 */
typedef enum {
	GPT_MODE_NORMAL = 0x00,
	GPT_MODE_SLEEP,
	GPT_MODE_UNINITIALIZED
} Gpt_ModeType;

#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
/* SWS_Gpt_00389 */
typedef enum {
	GPT_PREDEF_TIMER_1US_16BIT = 0x00,
	GPT_PREDEF_TIMER_1US_24BIT = 0x01,
	GPT_PREDEF_TIMER_1US_32BIT = 0x02,
	GPT_PREDEF_TIMER_100US_32BIT = 0x03,
} Gpt_PredefTimerType;
#endif

/* Type that holds HW Timers */
typedef enum {
	GPT_HW_TIMER2 = 0x00,
	GPT_HW_TIMER3,
	GPT_HW_TIMER4
} Gpt_HwChannelType;

/* Type that holds Timer Channel modes */
typedef enum {
	GPT_CH_MODE_CONTINUOUS = 0x00,
	GPT_CH_MODE_ONESHOT
} Gpt_ChannelModeType;

/* SWS_Gpt_00358 */
typedef uint8 Gpt_ChannelType;

/* SWS_Gpt_00359 */
typedef uint32 Gpt_ValueType;

/* Function pointer to Callback Function */
typedef void (*Gpt_NotificationType)( void );

/* Structure Definitions */
typedef struct {
	Gpt_ChannelType GptChannelId;
	Gpt_HwChannelType GptHwChannel;
    Gpt_ChannelModeType GptChannelMode;
    Gpt_ValueType GptChannelTickValueMax;  	/* maximum value the load register can hold */
    Gpt_NotificationType GptNotification;
} Gpt_ChannelConfigType;

/* SWS_Gpt_00357 */
typedef struct {
	uint8 NumOfChannel;
    const Gpt_ChannelConfigType *GptChannelConfig;
} Gpt_ConfigType;

#define GPT_START_SEC_CONFIG_DATA_UNSPECIFIED

extern const Gpt_ConfigType GptConfigData;
#define GPT_STOP_SEC_CONFIG_DATA_UNSPECIFIED

/* Public Function Declarations */
#define GPT_START_SEC_CODE

#if (GPT_VERSION_INFO_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_GetVersionInfo                                                
*  Syntax          : void Gpt_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr)                      
*  Service ID      : 0x00                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : VersionInfoPtr : Pointer to where to store the version 
                     information of this module.
*  Description     : This service returns the version information of this module.                     
********************************************************************************/
void Gpt_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr);
#endif

/******************************************************************************
*  Service name    : Gpt_Init                             
*  Syntax          : void Gpt_Init(const Gpt_ConfigType* ConfigPtr)                     
*  Service ID      : 0x01                                                      
*  Sync/Async      : Synchronous                                              
*  Reentrancy      : Non Reentrant                                         
*  Parameters (in) : ConfigPtr - Pointer to a selected configuration struture                                                  
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service Initializes the GPT driver.                    
********************************************************************************/
void Gpt_Init(const Gpt_ConfigType* ConfigPtr);

#if (GPT_DEINIT_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_DeInit                                             
*  Syntax          : void Gpt_DeInit(void)                     
*  Service ID      : 0x02                                                     
*  Sync/Async      : Synchronous                         
*  Reentrancy      : Non Reentrant                                             
*  Parameters (in) : None                                                     
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service Deinitializes the GPT driver.                    
********************************************************************************/
void Gpt_DeInit(void);
#endif

#if (GPT_TIME_ELAPSED_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_GetTimeElapsed                                                
*  Syntax          : Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType Channel)                      
*  Service ID      : 0x03                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None                                                      
*  Return value    : Gpt_ValueType - Elapsed timer value (in number of ticks)
*  Description     : This service returns the time already elapsed.                    
********************************************************************************/
Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType Channel);
#endif

#if ( GPT_TIME_REMAINING_API == STD_ON )
/******************************************************************************
*  Service name    : Gpt_GetTimeRemaining                                                 
*  Syntax          : Gpt_ValueType Gpt_GetTimeRemaining(Gpt_ChannelType Channel)                      
*  Service ID      : 0x04                                                      
*  Sync/Async      : Synchronous                                             
*  Reentrancy      : Reentrant                                  
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None                                                      
*  Return value    : Gpt_ValueType - Remaining timer value (in number of ticks)
*  Description     : This service returns the time remaining until the target time is reached.                    
********************************************************************************/
Gpt_ValueType Gpt_GetTimeRemaining(Gpt_ChannelType Channel);	
#endif

/******************************************************************************
*  Service name    : Gpt_StartTimer                                                
*  Syntax          : void Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value)                       
*  Service ID      : 0x05                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant (but not for the same timer channel)                                             
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.
                     Value   - Target time in number of ticks.                                                     
*  Parameters (out): None                                                     
*  Return value    : None
*  Description     : This service Starts a timer channel.                   
********************************************************************************/
void Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value);

/******************************************************************************
*  Service name    : Gpt_StopTimer                                                 
*  Syntax          : void Gpt_StopTimer(Gpt_ChannelType Channel)                      
*  Service ID      : 0x06                                                      
*  Sync/Async      : Synchronous                                              
*  Reentrancy      : Reentrant (but not for the same timer channel)                                             
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None                                                      
*  Return value    : None 
*  Description     : This service Stops a timer channel.                    
********************************************************************************/
void Gpt_StopTimer(Gpt_ChannelType Channel);

/* SWS_Gpt_00200 */
#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_EnableNotification                                                
*  Syntax          : void Gpt_EnableNotification(Gpt_ChannelType Channel)                     
*  Service ID      : 0x07                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant (but not for the same timer channel)                                             
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service enables the interrupt notification for a channel 
*                    (relevant in normal mode).                    
********************************************************************************/
void Gpt_EnableNotification(Gpt_ChannelType Channel);

/******************************************************************************
*  Service name    : Gpt_DisableNotification                                                
*  Syntax          : void Gpt_DisableNotification(Gpt_ChannelType Channel)                      
*  Service ID      : 0x08                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant (but not for the same timer channel)                                             
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service disables the interrupt notification for a channel 
*                    (relevant in normal mode).                    
********************************************************************************/
void Gpt_DisableNotification(Gpt_ChannelType Channel);
#endif

/* SWS_Gpt_00255 */
/* SWS_Gpt_00201 */
#if (GPT_WAKEUP_FUNCTIONALITY_API == STD_ON)
#if (GPT_REPORT_WAKEUP_SOURCE == STD_ON)
	
/******************************************************************************
*  Service name    : Gpt_SetMode                                                 
*  Syntax          : void Gpt_SetMode(Gpt_ModeType Mode)                      
*  Service ID      : 0x09                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non Reentrant                                              
*  Parameters (in) : Mode - GPT_MODE_NORMAL or GPT_MODE_SLEEP                                                    
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service sets the operation mode of the GPT.                    
********************************************************************************/
void Gpt_SetMode(Gpt_ModeType Mode);

/******************************************************************************
*  Service name    : Gpt_DisableWakeup                                                
*  Syntax          : void Gpt_DisableWakeup(Gpt_ChannelType Channel)                      
*  Service ID      : 0x0A                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant (but not for the same timer channel)                                             
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None
*  Return value    : None
*  Description     : This service disables the wakeup interrupt of a channel 
*                    (relevant in sleep mode).                    
********************************************************************************/
void Gpt_DisableWakeup(Gpt_ChannelType Channel);

/******************************************************************************
*  Service name    : Gpt_EnableWakeup                                                
*  Syntax          : void Gpt_EnableWakeup(Gpt_ChannelType Channel)                      
*  Service ID      : 0x0B                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant (but not for the same timer channel)                                  
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service enables the wakeup interrupt of a channel 
*                    (relevant in sleep mode).                    
********************************************************************************/
void Gpt_EnableWakeup(Gpt_ChannelType Channel);

/******************************************************************************
*  Service name    : Gpt_CheckWakeup                                                
*  Syntax          : void Gpt_CheckWakeup(EcuM_WakeupSourceType WakeupSource)                      
*  Service ID      : 0x0C                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : WakeupSource - Information on wakeup source to be checked.
                     The associated GPT channel can be determined from configuration 
					 data.                                                     
*  Parameters (out): None                                                     
*  Return value    : None
*  Description     : This service checks if a wakeup capable GPT channel is the source 
                     for a wakeup event and calls the ECU state manager service 
					 EcuM_SetWakeupEvent in case of a valid GPT channel wakeup event.                    
********************************************************************************/
void Gpt_CheckWakeup(EcuM_WakeupSourceType WakeupSource);
#endif
#endif

#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_GetPredefTimerValue                                                
*  Syntax          : Std_ReturnType Gpt_GetPredefTimerValue(
                        Gpt_PredefTimerType PredefTimer, uint32* TimeValuePtr)                      
*  Service ID      : 0x0D                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : PredefTimer - GPT Predef Timer                                                     
*  Parameters (out): TimeValuePtr - Pointer to time value destination data in RAM                                                      
*  Return value    : E_OK - no error has been detected 
                     E_NOT_OK - aborted due to errors
*  Description     : This service delivers the current value of the desired GPT Predef Timer.          
********************************************************************************/
Std_ReturnType Gpt_GetPredefTimerValue(Gpt_PredefTimerType PredefTimer, uint32* TimeValuePtr);
#endif

#define GPT_STOP_SEC_CODE

#endif
