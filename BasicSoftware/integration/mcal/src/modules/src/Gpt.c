/* ==========================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
========================================== */

/******************************************************************************
*                            Includes                                          
******************************************************************************/
#include "Gpt.h"
#include "Gpt_Alps.h"

#if(GPT_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#include "SchM_Gpt.h"

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/
#ifndef GPT_SW_MAJOR_VERSION
#error "GPT_SW_MAJOR_VERSION is not defined."
#endif

#ifndef GPT_SW_MINOR_VERSION
#error "GPT_SW_MINOR_VERSION is not defined."
#endif

#ifndef GPT_SW_PATCH_VERSION
#error "GPT_SW_PATCH_VERSION is not defined."
#endif

#if(GPT_SW_MAJOR_VERSION != 3U)
#error "GPT_SW_MAJOR_VERSION does not match."
#endif

#if(GPT_SW_MINOR_VERSION != 4U)
#error "GPT_SW_MINOR_VERSION does not match."
#endif

#if(GPT_SW_PATCH_VERSION != 0U)
#error "GPT_SW_PATCH_VERSION does not match."
#endif

/* AUTOSAR Secification File Version Check */
#ifndef GPT_AR_RELEASE_MAJOR_VERSION
#error "GPT_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

/* AUTOSAR Major and Minor release version check  */
#if(GPT_AR_RELEASE_MAJOR_VERSION != 4U)
#error "GPT_AR_RELEASE_MAJOR_VERSION does not match."
#endif

/* Inter Module Check for the correct version usage in the used modules */

/* Version Check */
/* Version check for DET module inclusion */
#if(GPT_DEV_ERROR_DETECT == STD_ON)

#ifndef DET_AR_RELEASE_MAJOR_VERSION
#error "DET_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

#if (DET_AR_RELEASE_MAJOR_VERSION != GPT_AR_RELEASE_MAJOR_VERSION)
#error "DET_AR_RELEASE_MAJOR_VERSION does not match."
#endif

#endif /* GPT_DEV_ERROR_DETECT == STD_ON */

/******************************************************************************
*                       Private Variable Definitions                            
******************************************************************************/
#define GPT_START_SEC_VAR_INIT_8

static boolean GptInitialized = FALSE;
#define GPT_STOP_SEC_VAR_INIT_8


#define GPT_START_SEC_VAR_INIT_UNSPECIFIED

extern Gpt_GlobalType GptGlobal;
#define GPT_STOP_SEC_VAR_INIT_UNSPECIFIED


#define GPT_START_SEC_CODE

							  
/******************************************************************************
*                         Public Functions                                     
******************************************************************************/

#if (GPT_VERSION_INFO_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_GetVersionInfo                                                
*  Syntax          : void Gpt_GetVersionInfo( Std_VersionInfoType* VersionInfoPtr )                      
*  Service ID      : 0x00                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : VersionInfoPtr : Pointer to where to store the version 
                     information of this module.
*  Description     : This service returns the version information of this module.                     
********************************************************************************/
void Gpt_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr)
{
	Std_ReturnType RetVal = E_OK;
    
	/* SWS_Gpt_00338 */
    Gpt_lclParamCheck((boolean)(NULL_PTR != VersionInfoPtr),
                      GPT_GETVERSIONINFO_SID,
                      GPT_E_PARAM_POINTER,
                      &RetVal);
    
    if (E_OK == RetVal)
	{
        VersionInfoPtr->vendorID = GPT_VENDOR_ID;
        VersionInfoPtr->moduleID = GPT_MODULE_ID;
        VersionInfoPtr->sw_major_version = GPT_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = GPT_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = GPT_SW_PATCH_VERSION;
    }
}
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
void Gpt_Init(const Gpt_ConfigType* ConfigPtr)
{
	Std_ReturnType RetVal = E_OK;
	
	/* SWS_Gpt_00307 */
	Gpt_lclParamCheck((boolean)(GptInitialized == FALSE), GPT_INIT_SID,
			           GPT_E_ALREADY_INITIALIZED, &RetVal);
					   
	Gpt_lclParamCheck((boolean)(NULL_PTR != ConfigPtr), GPT_INIT_SID,
                       GPT_E_PARAM_POINTER, &RetVal);
					   
	if(RetVal == E_OK)
    {
        Gpt_Hal_Init(ConfigPtr);
    }
	
	GptInitialized = TRUE;
}

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
void Gpt_DeInit(void)
{
	Std_ReturnType RetVal = E_OK;
	
	/* SWS_Gpt_00220 */
	Gpt_lclParamCheck((boolean)(GptInitialized == TRUE), GPT_DEINIT_SID,
					   GPT_E_UNINIT, &RetVal);
					   
	if(RetVal == E_OK)
    {
        Gpt_Hal_DeInit();
		/* SWS_Gpt_00363 */	
	    GptInitialized = FALSE;
    }
}
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
Gpt_ValueType Gpt_GetTimeElapsed(Gpt_ChannelType Channel)
{
	Std_ReturnType RetVal = E_OK;
	Gpt_ValueType  ElapsedVal = 0x00;
	
	/* SWS_Gpt_00222 */
	Gpt_lclParamCheck((boolean)(GptInitialized == TRUE), 
                       GPT_GETTIMEELAPSED_SID, 
                       GPT_E_UNINIT,
                       &RetVal);
	
	/* SWS_Gpt_00210 */
	Gpt_lclParamCheck((boolean)(Channel < GPT_CHANNEL_CNT), 
	                   GPT_GETTIMEELAPSED_SID, 
					   GPT_E_PARAM_CHANNEL,
					   &RetVal);
					   
	if(RetVal == E_OK)
	{
		ElapsedVal = Gpt_Hal_GetTimeElapsed(Channel);
	}
	
	return ElapsedVal;
}
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
Gpt_ValueType Gpt_GetTimeRemaining(Gpt_ChannelType Channel)
{
	Std_ReturnType RetVal = E_OK;
	Gpt_ValueType  RemainingVal = 0x00;
	
	/* SWS_Gpt_00223 */
	Gpt_lclParamCheck((boolean)(GptInitialized == TRUE), 
                       GPT_GETTIMEREMAINING_SID, 
                       GPT_E_UNINIT,
                       &RetVal);
					   
	/* SWS_Gpt_00211 */
	Gpt_lclParamCheck((boolean)(Channel < GPT_CHANNEL_CNT), 
	                   GPT_GETTIMEREMAINING_SID, 
					   GPT_E_PARAM_CHANNEL,
					   &RetVal);
					   
	if(RetVal == E_OK)
	{
		RemainingVal = Gpt_Hal_GetTimeRemaining(Channel);
	}
	
	return RemainingVal;
}
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
void Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value)
{
	Std_ReturnType RetVal = E_OK;
	
	/* SWS_Gpt_00224 */
	Gpt_lclParamCheck((boolean)(GptInitialized == TRUE), 
                       GPT_STARTTIMER_SID, 
                       GPT_E_UNINIT,
                       &RetVal);
					   
	/* SWS_Gpt_00212 */
	Gpt_lclParamCheck((boolean)(Channel < GPT_CHANNEL_CNT), 
	                   GPT_STARTTIMER_SID, 
					   GPT_E_PARAM_CHANNEL,
					   &RetVal);
					   
	
	/* SWS_Gpt_00218 */
	Gpt_lclParamCheck((boolean)((0 < Value ) && (Value <= (uint32)GPT_MAX_TIMER_RESOLUTION)),
	                   GPT_STARTTIMER_SID, 
					   GPT_E_PARAM_VALUE,
					   &RetVal);
					   
	if(RetVal == E_OK)
	{
		Gpt_Hal_StartTimer(Channel, Value);
	}
}

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
void Gpt_StopTimer(Gpt_ChannelType Channel)
{
	Std_ReturnType RetVal = E_OK;
	
	/* SWS_Gpt_00225 */
	Gpt_lclParamCheck((boolean)(GptInitialized == TRUE), 
                       GPT_STOPTIMER_SID, 
                       GPT_E_UNINIT,
                       &RetVal);
					   
	/* SWS_Gpt_00213 */
	Gpt_lclParamCheck((boolean)(Channel < GPT_CHANNEL_CNT), 
	                   GPT_STOPTIMER_SID, 
					   GPT_E_PARAM_CHANNEL,
					   &RetVal);
					   
	if(RetVal == E_OK)
	{
		Gpt_Hal_StopTimer(Channel);
	}
}

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
void Gpt_EnableNotification(Gpt_ChannelType Channel)
{
	Std_ReturnType RetVal = E_OK;
	
	/* SWS_Gpt_00226 */
	Gpt_lclParamCheck((boolean)(GptInitialized == TRUE), 
                       GPT_ENABLENOTIFICATION_SID, 
                       GPT_E_UNINIT,
                       &RetVal);
					   
	/* SWS_Gpt_00214 */
	Gpt_lclParamCheck((boolean)(Channel < GPT_CHANNEL_CNT), 
	                   GPT_ENABLENOTIFICATION_SID, 
					   GPT_E_PARAM_CHANNEL,
					   &RetVal);
					   
	/* SWS_Gpt_00377 */
	Gpt_lclParamCheck((boolean)(GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptNotification != NULL_PTR), 
	                   GPT_ENABLENOTIFICATION_SID, 
					   GPT_E_PARAM_CHANNEL,
					   &RetVal);
					   
	if(RetVal == E_OK)
	{
		/* SWS_Gpt_00014 */
		GptGlobal.Gpt_NotifEnable[Channel] = 1;
	}
}

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
void Gpt_DisableNotification(Gpt_ChannelType Channel)
{
	Std_ReturnType RetVal = E_OK;
	
	/* SWS_Gpt_00227 */
	Gpt_lclParamCheck((boolean)(GptInitialized == TRUE), 
                       GPT_DISABLENOTIFICATION_SID, 
                       GPT_E_UNINIT,
                       &RetVal);
					   
	/* SWS_Gpt_00217 */
	Gpt_lclParamCheck((boolean)(Channel < GPT_CHANNEL_CNT), 
	                   GPT_DISABLENOTIFICATION_SID, 
					   GPT_E_PARAM_CHANNEL,
					   &RetVal);
					   
	/* SWS_Gpt_00379 */
	Gpt_lclParamCheck((boolean)(GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptNotification != NULL_PTR), 
	                   GPT_DISABLENOTIFICATION_SID, 
					   GPT_E_PARAM_CHANNEL,
					   &RetVal);
					   
	if(RetVal == E_OK)
	{
		/* SWS_Gpt_00015 */
		GptGlobal.Gpt_NotifEnable[Channel] = 0;
	}
}
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
void Gpt_SetMode(Gpt_ModeType Mode)
{
	/* Not Supported */
}

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
void Gpt_DisableWakeup(Gpt_ChannelType Channel)
{
	/* Not Supported */
}

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
void Gpt_EnableWakeup(Gpt_ChannelType Channel)
{
	/* Not Supported */
}

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
void Gpt_CheckWakeup(EcuM_WakeupSourceType WakeupSource)
{
	/* Not Supported */
}
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
Std_ReturnType Gpt_GetPredefTimerValue(Gpt_PredefTimerType PredefTimer, uint32* TimeValuePtr)
{
	Std_ReturnType RetVal = E_OK;
	
	/* SWS_Gpt_00398 */
	Gpt_lclParamCheck((boolean)(GptInitialized == TRUE), 
                       GPT_GETPREDEFTIMERVALUE_SID, 
                       GPT_E_UNINIT,
                       &RetVal);
					   
	/* SWS_Gpt_00399 */
	/* SWS_Gpt_00400 */
	Gpt_lclParamCheck((boolean)(PredefTimer == GPT_PREDEF_TIMER_100US_32BIT), 
                       GPT_GETPREDEFTIMERVALUE_SID, 
                       GPT_E_PARAM_PREDEF_TIMER,
                       &RetVal);
					   
	/* SWS_Gpt_00403 */
	Gpt_lclParamCheck((boolean)(NULL_PTR != TimeValuePtr), 
	                   GPT_GETPREDEFTIMERVALUE_SID,
                       GPT_E_PARAM_POINTER,
					   &RetVal);
					   
	if (RetVal == E_OK)
	{
		RetVal = Gpt_Hal_GetPredefTimerValue(PredefTimer, TimeValuePtr);
	}
	
	return RetVal;
}
#endif
    
#define GPT_STOP_SEC_CODE

