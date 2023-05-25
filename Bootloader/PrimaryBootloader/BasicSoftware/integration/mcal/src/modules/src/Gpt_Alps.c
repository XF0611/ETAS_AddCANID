/* ==========================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
========================================== */

/******************************************************************************
*                            Includes                                          
******************************************************************************/
#include "Gpt.h"
#include "Gpt_Alps.h"
#include "Gpt_Alps_RegDef.h"

/******************************************************************************
*                       Private Variables                                      
******************************************************************************/
#define GPT_START_SEC_VAR_INIT_PTR

static Gpt_Reg_t *Gpt_Reg = (Gpt_Reg_t *)0xB10000U;
#define GPT_STOP_SEC_VAR_INIT_PTR


#define GPT_START_SEC_VAR_INIT_UNSPECIFIED

Gpt_GlobalType GptGlobal;
#define GPT_STOP_SEC_VAR_INIT_UNSPECIFIED


#define GPT_START_SEC_CODE

/******************************************************************************
*                         Private Function Declaration                         
******************************************************************************/
static void Gpt_lclInterruptClear(Gpt_HwChannelType Channel);

ISR(Gpt1_Isr) { Gpt_Isr(0); }
ISR(Gpt2_Isr) { Gpt_Isr(1); }
ISR(Gpt3_Isr) { Gpt_Isr(2); }

/******************************************************************************
*                         Public Functions                                     
******************************************************************************/

/******************************************************************************
*  Service name    : Gpt_Hal_Init                             
*  Syntax          : void Gpt_Hal_Init(const Gpt_ConfigType* ConfigPtr)                     
*  Parameters (in) : ConfigPtr - Pointer to a selected configuration struture                                                  
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service Initializes the GPT driver.                    
********************************************************************************/
void Gpt_Hal_Init(const Gpt_ConfigType* ConfigPtr)
{
	Gpt_HwChannelType TimerChnl;
	Gpt_ChannelModeType ChannelMode;
	GptGlobal.ConfigPtr = ConfigPtr;
	
	for (uint8 ChnlIdx = 0; ChnlIdx < GPT_CHANNEL_CNT; ChnlIdx++)
	{
		/* SWS_Gpt_00068 */
		TimerChnl = GptGlobal.ConfigPtr->GptChannelConfig[ChnlIdx].GptHwChannel;
		
		ChannelMode = GptGlobal.ConfigPtr->GptChannelConfig[ChnlIdx].GptChannelMode;
		
		switch (TimerChnl)
		{
			case GPT_HW_TIMER2:
			    /* Clear any pending Interrupts on read - HW TIMER 2*/
				Gpt_lclInterruptClear(GPT_HW_TIMER2);
				
				/* Disable Timer - HW TIMER 2 */
				Gpt_Reg->TIMER2_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				
				/* Set Channel mode - HW TIMER 2 */
				if (ChannelMode == GPT_CH_MODE_CONTINUOUS)
				{
		            Gpt_Reg->TIMER2_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_MODE_BIT_LPOS));
				} else {
					/* One-Shot Mode */
					Gpt_Reg->TIMER2_CONTROLREG |= (1 << GPT_TIMER_MODE_BIT_LPOS);
				}
				break;

			case GPT_HW_TIMER3:
			    /* Clear any pending Interrupts on read - HW TIMER 3*/
				Gpt_lclInterruptClear(GPT_HW_TIMER3);

				/* Disable Timer - HW TIMER 3 */
				Gpt_Reg->TIMER3_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));

				/* Set Channel mode - HW TIMER 3 */
				if (ChannelMode == GPT_CH_MODE_CONTINUOUS)
				{
		            Gpt_Reg->TIMER3_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_MODE_BIT_LPOS));
				} else {
					/* One-Shot Mode */
					Gpt_Reg->TIMER3_CONTROLREG |= (1 << GPT_TIMER_MODE_BIT_LPOS);
				}
				break;

			case GPT_HW_TIMER4:
			    /* Clear any pending Interrupts on read - HW TIMER 4*/
				Gpt_lclInterruptClear(GPT_HW_TIMER4);

				/* Disable Timer - HW TIMER 4 */
				Gpt_Reg->TIMER4_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));

				/* Set Channel mode - HW TIMER 4 */
				if (ChannelMode == GPT_CH_MODE_CONTINUOUS)
				{
		            Gpt_Reg->TIMER4_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_MODE_BIT_LPOS));
				} else {
					/* One-Shot Mode */
					Gpt_Reg->TIMER4_CONTROLREG |= (1 << GPT_TIMER_MODE_BIT_LPOS);
				}
				break;
			default:
				/* compliant to misra-c */
				break;
		}
		
		GptGlobal.Gpt_ChannelState[ChnlIdx] = GPT_CH_STATE_INITIALIZED;
		/* SWS_Gpt_00107 */
        GptGlobal.Gpt_NotifEnable[ChnlIdx] = 0;
	}
	
	/* SWS_Gpt_00339 */
	GptGlobal.OperationMode = GPT_MODE_NORMAL;
	
#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
	/* Set HW Timer1 to Continuous mode */
	Gpt_Reg->TIMER1_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_MODE_BIT_LPOS));
	/* Load maximum count value to HW Timer1 register */
	Gpt_Reg->TIMER1_LOADCOUNT  = GPT_MAX_TIMER_VAL;
	/* Enable HW Timer1 */
	Gpt_Reg->TIMER1_CONTROLREG |= (1 << GPT_TIMER_ENABLE_BIT_LPOS);
#endif
}

#if (GPT_DEINIT_API == STD_ON)
/******************************************************************************
*  Service name    : Gpt_Hal_DeInit                                             
*  Syntax          : void Gpt_Hal_DeInit(void)                                          
*  Parameters (in) : None                                                     
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service Deinitializes the GPT driver.                    
********************************************************************************/
void Gpt_Hal_DeInit(void)
{
	Std_ReturnType RetVal = E_OK;
	Gpt_HwChannelType TimerChnl;
	
	for (uint8 ChnlIdx = 0; ChnlIdx < GPT_CHANNEL_CNT; ChnlIdx++)
	{
		/* SWS_Gpt_00068 */
		TimerChnl = GptGlobal.ConfigPtr->GptChannelConfig[ChnlIdx].GptHwChannel;
		
		/* SWS_Gpt_00234 */
		Gpt_lclParamCheck((boolean)(GptGlobal.Gpt_ChannelState[ChnlIdx] != GPT_CH_STATE_RUNNING),
                      GPT_DEINIT_SID,
                      GPT_E_BUSY,
                      &RetVal);
		
		 /* SWS_Gpt_00162 */
		switch (TimerChnl)
		{
			case GPT_HW_TIMER2:
			    /* Clear any pending Interrupts on read - HW TIMER 2*/
				Gpt_lclInterruptClear(GPT_HW_TIMER2);
				/* Disable Timer - HW TIMER 2 */
				Gpt_Reg->TIMER2_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				break;
				
			case GPT_HW_TIMER3:
			    /* Clear any pending Interrupts on read - HW TIMER 3*/
				Gpt_lclInterruptClear(GPT_HW_TIMER3);

				/* Disable Timer - HW TIMER 3 */
				Gpt_Reg->TIMER3_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				break;
				
			case GPT_HW_TIMER4:
			    /* Clear any pending Interrupts on read - HW TIMER 4*/
				Gpt_lclInterruptClear(GPT_HW_TIMER4);

				/* Disable Timer - HW TIMER 4 */
				Gpt_Reg->TIMER4_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				break;
			default:
				/*compliant to misra-c*/
				break;
		}
		
		/* SWS_Gpt_00105 */
        GptGlobal.Gpt_NotifEnable[ChnlIdx] = 0;
	}
	
	/* SWS_Gpt_00363 */
	GptGlobal.OperationMode = GPT_MODE_UNINITIALIZED;
	
#if(GPT_PREDEFTIMER_FUNCTIONALITY_API == STD_ON)
	/* Disable HW Timer1 */
	Gpt_Reg->TIMER1_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
#endif
}
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
Gpt_ValueType Gpt_Hal_GetTimeElapsed(Gpt_ChannelType Channel)
{
	Gpt_ValueType TimeElapsed = 0;
	uint32 TempCurrentTimerVal;
	Gpt_HwChannelType TimerChnl;
	
	TimerChnl = GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptHwChannel;
	
	 /* SWS_Gpt_00295 */
	if (GptGlobal.Gpt_ChannelState[Channel] == GPT_CH_STATE_INITIALIZED)
    {
		/*compliant to misra-c*/
    }
	/* SWS_Gpt_00299 */
	else if ((GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptChannelMode == GPT_CH_MODE_ONESHOT) &&
	   (GptGlobal.Gpt_ChannelState[Channel] == GPT_CH_STATE_EXPIRED))
	{
		TimeElapsed = GptGlobal.Gpt_TargetVal[Channel];
    }
	
	/* SWS_Gpt_00010 */ /* Running */
    /* SWS_Gpt_00297 */ /* Stopped */
    /* SWS_Gpt_00361 */ /* Continuous mode */
	else
	{
		switch(TimerChnl)
		{
			case GPT_HW_TIMER2:
				TempCurrentTimerVal = Gpt_Reg->TIMER2_CURRENTVALUE;
				TimeElapsed         = GptGlobal.Gpt_TargetVal[Channel] - TempCurrentTimerVal;
				break;
			case GPT_HW_TIMER3:
				TempCurrentTimerVal = Gpt_Reg->TIMER3_CURRENTVALUE;
				TimeElapsed         = GptGlobal.Gpt_TargetVal[Channel] - TempCurrentTimerVal;
				break;
			case GPT_HW_TIMER4:
				TempCurrentTimerVal = Gpt_Reg->TIMER4_CURRENTVALUE;
				TimeElapsed         = GptGlobal.Gpt_TargetVal[Channel] - TempCurrentTimerVal;
				break;
			default:
				/*compliant to misra-c*/
				break;
		}
	}
	return TimeElapsed;
}
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
Gpt_ValueType Gpt_Hal_GetTimeRemaining(Gpt_ChannelType Channel)
{
	Gpt_ValueType TimeRemaining = 0;
	uint32 TempTimeRemaining;
	Gpt_HwChannelType TimerChnl;
	
	TimerChnl = GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptHwChannel;
	
	/* SWS_Gpt_00301 */
	if (GptGlobal.Gpt_ChannelState[Channel] == GPT_CH_STATE_INITIALIZED)
    {
       /*compliant to misra-c*/
    }
	
	/* SWS_Gpt_00305 */
	else if ((GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptChannelMode == GPT_CH_MODE_ONESHOT) &&
	   (GptGlobal.Gpt_ChannelState[Channel] == GPT_CH_STATE_EXPIRED))
	{
		/*compliant to misra-c*/
    }
	
	/* SWS_Gpt_00083 */ /* Running */
    /* SWS_Gpt_00303 */ /* Stopped */
	else
	{
		switch(TimerChnl)
		{
			case GPT_HW_TIMER2:
				TempTimeRemaining = Gpt_Reg->TIMER2_CURRENTVALUE;
				TimeRemaining     = TempTimeRemaining;
				break;
			case GPT_HW_TIMER3:
				TempTimeRemaining = Gpt_Reg->TIMER3_CURRENTVALUE;
				TimeRemaining     = TempTimeRemaining;
				break;
			case GPT_HW_TIMER4:
				TempTimeRemaining = Gpt_Reg->TIMER4_CURRENTVALUE;
				TimeRemaining     = TempTimeRemaining;
				break;
			default:
				/*compliant to misra-c*/
				break;
		}
	}
	
	return TimeRemaining;
}
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
void Gpt_Hal_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value)
{
	Std_ReturnType RetVal = E_OK;
	Gpt_HwChannelType TimerChnl;
	Gpt_ChannelModeType ChannelMode;
	
	TimerChnl = GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptHwChannel;
	ChannelMode = GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptChannelMode;
	
	GptGlobal.Gpt_TargetVal[Channel] = Value;
	
	/* SWS_Gpt_00084 */
	Gpt_lclParamCheck((boolean)(GptGlobal.Gpt_ChannelState[Channel] != GPT_CH_STATE_RUNNING),
                      GPT_STARTTIMER_SID,
                      GPT_E_BUSY,
                      &RetVal);
					  
	/* SWS_Gpt_00274 */
	/* SWS_Gpt_00275 */

	/* Load the configured value to register TimerNLoadCount */
	/* Unmask interrupt for the configured Timers */
	/* Enable Timer */
	
	if (RetVal == E_OK)
	{
	    switch(TimerChnl)
	    {
		    case GPT_HW_TIMER2:
			    if (ChannelMode == GPT_CH_MODE_ONESHOT)
			    {
		            Gpt_Reg->TIMER2_LOADCOUNT = Value;
				} else {
					/* Continuous mode */
					Gpt_Reg->TIMER2_LOADCOUNT = GPT_MAX_TIMER_VAL;
				}
			    Gpt_Reg->TIMER2_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_INTERRUPT_MASK_BIT_LPOS));
			    Gpt_Reg->TIMER2_CONTROLREG |= (1 << GPT_TIMER_ENABLE_BIT_LPOS);
			    break;
		    case GPT_HW_TIMER3:
		        if (ChannelMode == GPT_CH_MODE_ONESHOT)
			    {
		            Gpt_Reg->TIMER3_LOADCOUNT = Value;
				} else {
					/* Continuous mode */
					Gpt_Reg->TIMER3_LOADCOUNT = GPT_MAX_TIMER_VAL;
				}
			    Gpt_Reg->TIMER3_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_INTERRUPT_MASK_BIT_LPOS));
			    Gpt_Reg->TIMER3_CONTROLREG |= (1 << GPT_TIMER_ENABLE_BIT_LPOS);
			    break;
		    case GPT_HW_TIMER4:
		        if (ChannelMode == GPT_CH_MODE_ONESHOT)
			    {
		            Gpt_Reg->TIMER4_LOADCOUNT = Value;
				} else {
					/* Continuous mode */
					Gpt_Reg->TIMER4_LOADCOUNT = GPT_MAX_TIMER_VAL;
				}
			    Gpt_Reg->TIMER4_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_INTERRUPT_MASK_BIT_LPOS));
			    Gpt_Reg->TIMER4_CONTROLREG |= (1 << GPT_TIMER_ENABLE_BIT_LPOS);
			    break;
		    default:
		    	/*compliant to misra-c*/
		    	break;
	    }
	    /* SWS_Gpt_00364 */
	    GptGlobal.Gpt_ChannelState[Channel] = GPT_CH_STATE_RUNNING;
	}
	else
	{
		/*compliant to misra-c*/
	}

}

/******************************************************************************
*  Service name    : Gpt_Hal_StopTimer                                                 
*  Syntax          : void Gpt_Hal_StopTimer(Gpt_ChannelType Channel)
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                     
*  Parameters (out): None                                                      
*  Return value    : None 
*  Description     : This service Stops a timer channel.                    
********************************************************************************/
void Gpt_Hal_StopTimer(Gpt_ChannelType Channel)
{
	Gpt_HwChannelType TimerChnl;
	
	TimerChnl = GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptHwChannel;
	
	/* SWS_Gpt_00099 */
    /* SWS_Gpt_00344 */
	if (GptGlobal.Gpt_ChannelState[Channel] != GPT_CH_STATE_RUNNING)
	{
		/*compliant to misra-c*/
	}
	else
	{
	/* Clear any pending interrupts */
	/* Mask Interrupt */
	/* Disable timer */
	/* SWS_Gpt_00013 */
		switch(TimerChnl)
		{
			case GPT_HW_TIMER2:
				Gpt_lclInterruptClear(GPT_HW_TIMER2);
				Gpt_Reg->TIMER2_CONTROLREG |= (1 << GPT_TIMER_INTERRUPT_MASK_BIT_LPOS);
				Gpt_Reg->TIMER2_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				break;
			case GPT_HW_TIMER3:
				Gpt_lclInterruptClear(GPT_HW_TIMER3);
				Gpt_Reg->TIMER3_CONTROLREG |= (1 << GPT_TIMER_INTERRUPT_MASK_BIT_LPOS);
				Gpt_Reg->TIMER3_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				break;
			case GPT_HW_TIMER4:
				Gpt_lclInterruptClear(GPT_HW_TIMER4);
				Gpt_Reg->TIMER4_CONTROLREG |= (1 << GPT_TIMER_INTERRUPT_MASK_BIT_LPOS);
				Gpt_Reg->TIMER4_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				break;
			default:
				/*compliant to misra-c*/
				break;
		}
		/* SWS_Gpt_00343 */
		GptGlobal.Gpt_ChannelState[Channel] = GPT_CH_STATE_STOPPED;
	}
}

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
Std_ReturnType Gpt_Hal_GetPredefTimerValue(Gpt_PredefTimerType PredefTimer, uint32* TimeValuePtr)
{
	/* This is a Down-counter Timer */
	*TimeValuePtr = Gpt_Reg->TIMER1_CURRENTVALUE;
	
	return E_OK;
}
#endif


void Gpt_lclParamCheck(boolean Expr, uint8 ApiId, uint8 ErrId, Std_ReturnType *RetVal)   
{
    if ((E_OK == *RetVal) && (Expr == FALSE)) {
    #if (GPT_DEV_ERROR_DETECT == STD_ON)
        /* calls Det_ReportError from Det module */
        (void)Det_ReportError(GPT_MODULE_ID, GPT_INSTANCE_ID, ApiId, ErrId);
    #endif
        *RetVal = E_NOT_OK;
    }
}

/******************************************************************************
*                         Private Function Definitions                                 
******************************************************************************/

/******************************************************************************
*  Service name    : Gpt_lclInterruptClear                                                
*  Syntax          : void Gpt_lclInterruptClear(Gpt_ChannelType Channel)
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                    
*  Parameters (out): None
*  Return value    : None
*  Description     : This service clears interrupts.
********************************************************************************/
void Gpt_lclInterruptClear(Gpt_HwChannelType Channel)
{
	uint32 ReadNClearIntSts;
	
	switch(Channel)
	{
		case GPT_HW_TIMER2:
		    /* Check if Interrupt is Active */
			if ((Gpt_Reg->TIMER2_INTSTATUS & 0x01) == 0x01)
			{
				/* Clear interrupt */
			    ReadNClearIntSts = Gpt_Reg->TIMER2_EOI;
			}
			else
			{
				/*compliant to misra-c*/
			}
			break;
		case GPT_HW_TIMER3:
		    /* Check if Interrupt is Active */
			if ((Gpt_Reg->TIMER3_INTSTATUS & 0x01) == 0x01)
			{
				/* Clear interrupt */
			    ReadNClearIntSts = Gpt_Reg->TIMER3_EOI;
			}
			else
			{
				/*compliant to misra-c*/
			}
			break;
		case GPT_HW_TIMER4:
		    /* Check if Interrupt is Active */
			if ((Gpt_Reg->TIMER4_INTSTATUS & 0x01) == 0x01)
			{
				/* Clear interrupt */
			    ReadNClearIntSts = Gpt_Reg->TIMER4_EOI;
			}
			else
			{
				/*compliant to misra-c*/
			}
			break;
		default:
			/*compliant to misra-c*/
			break;
	}
}


/******************************************************************************
*  Service name    : Gpt_Isr                                                
*  Syntax          : void Gpt_Isr( uint32 channel)
*  Parameters (in) : Channel - Numeric identifier of the GPT channel.                                                    
*  Parameters (out): None
*  Return value    : None
*  Description     : Interrupt Service Routine.
********************************************************************************/
void Gpt_Isr(uint8 Channel)
{
    Gpt_HwChannelType TimerChnl;
	Gpt_ChannelModeType ChannelMode;
	TimerChnl = GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptHwChannel;
	ChannelMode = GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptChannelMode;

	switch(Channel)
	{
		case GPT_HW_TIMER2:
		    /* Clear any pending Interrupts on read - HW TIMER 2 */
			Gpt_lclInterruptClear(GPT_HW_TIMER2);
			/* One-shot Mode */
			if (ChannelMode == GPT_CH_MODE_ONESHOT)
			{
				// /* Disable Timer */
				// Gpt_Reg->TIMER2_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				// /* Change the channel state to Expired */
				// GptGlobal.Gpt_ChannelState[Channel] = GPT_CH_STATE_EXPIRED;
			}
			break;
		case GPT_HW_TIMER3:
		    /* Clear any pending Interrupts on read - HW TIMER 3 */
			Gpt_lclInterruptClear(GPT_HW_TIMER3);
			/* One-shot Mode */
			if (ChannelMode == GPT_CH_MODE_ONESHOT)
			{
				/* Disable Timer */
				Gpt_Reg->TIMER3_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				/* Change the channel state to Expired */
				GptGlobal.Gpt_ChannelState[Channel] = GPT_CH_STATE_EXPIRED;
			}
			break;
		case GPT_HW_TIMER4:
			/* Clear any pending Interrupts on read - HW TIMER 4 */
			Gpt_lclInterruptClear(GPT_HW_TIMER4);
			/* One-shot Mode */
			if (ChannelMode == GPT_CH_MODE_ONESHOT)
			{
				/* Disable Timer */
				Gpt_Reg->TIMER4_CONTROLREG &= (uint32)(~(1 << GPT_TIMER_ENABLE_BIT_LPOS));
				/* Change the channel state to Expired */
				GptGlobal.Gpt_ChannelState[Channel] = GPT_CH_STATE_EXPIRED;
			}
			break;
		default :
			/*compliant to misra-c*/
		    break;
	}

#if (GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON)
	/* Notify */
	if (GptGlobal.Gpt_NotifEnable[Channel] == 1 )
	{
		/* SWS_Gpt_00086 */
        GptGlobal.ConfigPtr->GptChannelConfig[Channel].GptNotification();
	}
#endif  
}

#define GPT_STOP_SEC_CODE

