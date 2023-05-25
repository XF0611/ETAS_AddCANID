/* ==========================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
========================================== */

#ifndef GPT_ALPS_H
#define GPT_ALPS_H

#include "Gpt.h"

/* Macro definitions */
#define GPT_TIMER_ENABLE_BIT_LPOS            0x00
#define GPT_TIMER_MODE_BIT_LPOS              0x01
#define GPT_TIMER_INTERRUPT_MASK_BIT_LPOS    0x02
#define GPT_MAX_TIMER_VAL                 0xFFFFFFFF
#define IRQ_TIMER0                           35 /* This is predef Timer */
#define IRQ_TIMER1                           36
#define IRQ_TIMER2                           37
#define IRQ_TIMER3                           38
#define GPT_ISR_PRIORITY                      0

//#define ISR(_func_name_)    void _func_name_ (uint8 id)
#define ISR(x) void Os_Entry_##x(void) /* ETAS BIP Porting for OS macro ISR() */


/* Enum Definitions */
/* Type that holds all states of the Gpt channels */
typedef enum {
	GPT_CH_STATE_STOPPED = 0x00,
	GPT_CH_STATE_RUNNING,
	GPT_CH_STATE_EXPIRED,
	GPT_CH_STATE_INITIALIZED
} Gpt_ChannelStateType;


/* Structure definitions */
typedef struct {
	const Gpt_ConfigType *ConfigPtr;
	Gpt_ChannelStateType Gpt_ChannelState[GPT_CHANNEL_CNT];
	Gpt_ValueType Gpt_TargetVal[GPT_CHANNEL_CNT];
	boolean Gpt_NotifEnable[GPT_CHANNEL_CNT];
	Gpt_ModeType OperationMode;
} Gpt_GlobalType;


/* Public Function Declarations */
#define GPT_START_SEC_CODE


/* Function for Parameter Validation */
void Gpt_lclParamCheck(boolean Expr,
                              uint8 ApiId, 
                              uint8 ErrId, 
                              Std_ReturnType *RetVal);

/* Interrupt Service routine for all the timer channels */
void Gpt_Isr( uint8 channel);

/* Interrupt handlers to be registeres */
ISR(Gpt1_Isr);
ISR(Gpt2_Isr);
ISR(Gpt3_Isr);

/******************************************************************************
*  Service name    : Gpt_Hal_Init                             
*  Syntax          : void Gpt_Hal_Init(const Gpt_ConfigType* ConfigPtr)                     
*  Parameters (in) : ConfigPtr - Pointer to a selected configuration struture                                                  
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service Initializes the GPT driver.                    
********************************************************************************/
void Gpt_Hal_Init(const Gpt_ConfigType* ConfigPtr);

#if (GPT_DEINIT_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_Hal_DeInit                                             
*  Syntax          : void Gpt_Hal_DeInit(void)                                          
*  Parameters (in) : None                                                     
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service Deinitializes the GPT driver.                    
********************************************************************************/
void Gpt_Hal_DeInit(void);
#endif

#if (GPT_TIME_ELAPSED_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_Hal_GetTimeElapsed                                                
*  Syntax          : Gpt_ValueType Gpt_Hal_GetTimeElapsed(Gpt_ChannelType Channel)
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None                                                      
*  Return value    : Gpt_ValueType - Elapsed timer value (in number of ticks)
*  Description     : This service returns the time already elapsed.                    
********************************************************************************/
Gpt_ValueType Gpt_Hal_GetTimeElapsed(Gpt_ChannelType Channel);
#endif

#if ( GPT_TIME_REMAINING_API == STD_ON )
/******************************************************************************
*  Service name    : Gpt_Hal_GetTimeRemaining                                                 
*  Syntax          : Gpt_ValueType Gpt_Hal_GetTimeRemaining(Gpt_ChannelType Channel)
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None                                                      
*  Return value    : Gpt_ValueType - Remaining timer value (in number of ticks)
*  Description     : This service returns the time remaining until the target time is reached.                    
********************************************************************************/
Gpt_ValueType Gpt_Hal_GetTimeRemaining(Gpt_ChannelType Channel);	
#endif

/******************************************************************************
*  Service name    : Gpt_Hal_StartTimer                                                
*  Syntax          : void Gpt_Hal_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value)                                             
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.
                     Value   - Target time in number of ticks.                                                     
*  Parameters (out): None                                                     
*  Return value    : None
*  Description     : This service Starts a timer channel.                   
********************************************************************************/
void Gpt_Hal_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value);

/******************************************************************************
*  Service name    : Gpt_Hal_StopTimer                                                 
*  Syntax          : void Gpt_Hal_StopTimer(Gpt_ChannelType Channel)
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None                                                      
*  Return value    : None 
*  Description     : This service Stops a timer channel.                    
********************************************************************************/
void Gpt_Hal_StopTimer(Gpt_ChannelType Channel);

#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_Hal_GetPredefTimerValue                                                
*  Syntax          : Std_ReturnType Gpt_Hal_GetPredefTimerValue(
                        Gpt_PredefTimerType PredefTimer, uint32* TimeValuePtr)
*  Parameters (in) : PredefTimer - GPT Predef Timer                                                     
*  Parameters (out): TimeValuePtr - Pointer to time value destination data in RAM                                                      
*  Return value    : E_OK - no error has been detected 
                     E_NOT_OK - aborted due to errors
*  Description     : This service delivers the current value of the desired GPT Predef Timer.          
********************************************************************************/
Std_ReturnType Gpt_Hal_GetPredefTimerValue(Gpt_PredefTimerType PredefTimer, uint32* TimeValuePtr);
#endif



#define GPT_STOP_SEC_CODE


#endif
