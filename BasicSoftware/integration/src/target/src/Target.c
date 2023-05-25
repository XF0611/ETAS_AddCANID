/*
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 ************************************************************************************************
 * Component : Target.c
 * Date      : Oct. 14 2021
 * Version   : 1.2
 * Description  : This module implements Timer driver for RTA-OS TASK tick.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Target.h"
#include "Mcal_Integration.h"
#include "alps_dmu.h"
#include "alps.h"
#include "Gpt_Alps.h"



/*
 **********************************************************************************************************************
 * Note: This target(CAL60S244AB) running on xxxMHz external crystal  (hardware dependent)
 *       Os Stopwatch:    
 *       Os Tick Counter: 
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Define/Macros
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Static Variables
 **********************************************************************************************************************
*/
static void Mcu_InitializeClock(void);


/*
 ***************************************************************************************************
 * Functions
 ***************************************************************************************************
 */


/**********************************************************************************
  Function name     : Target_Init
  Description       : initialize target timer at startup.
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks:          It shall be initialized before MCAL modules (e.g., CAN/Fls/ETH)
                    who have dependence with the counter.
***********************************************************************************/
void Target_Init(void)
{
    Mcu_InitializeClock();
}


/**********************************************************************************
  Function name     : Target_Init
  Description       : Initialize target timer at startup.
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks:          It shall be initialized before MCAL modules (e.g., CAN/Fls/ETH)
                    who have dependence with the counter.
***********************************************************************************/
static void Mcu_InitializeClock(void)
{
#define PLL1_OUTPUT_CLOCK_FREQ		(300000000UL)
    /* Initialise the RAM Section */

    /* Set the CPU Clock to the PLL */
    set_current_cpu_freq(PLL1_OUTPUT_CLOCK_FREQ);
    // chip_hw_mdelay(5000);
    // Mcu_Init(&McuConfigData);

    /* Activate the PLL Clock */
    pll_enable_stub();

    /* Distribute MCU Clocks to peripherals */
    system_clock_init();
    Mcu_InitClock(McuConf_McuClockSettingConfig_McuClockSettingConfig_0);
    Mcu_DistributePllClock();
}


/**********************************************************************************
  Function name     : Target_TimerInit
  Description       : Initialize counter for OS Stopwatch
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks:
***********************************************************************************/
void Target_TimerInit(void)
{
    /****************************************************************************
     * The Stopwatch uses the peripheral Timer channel 1 (MCAL Config Timer3)
     ***************************************************************************/
    /* ETAS BIP Porting don't care second parameter when timer works in free counter mode. */
    Gpt_StartTimer(GPT_HW_TIMER3, GPT_MAX_TIMER_RESOLUTION);

    Gpt_StartTimer(GPT_HW_TIMER2, 100000U);
}


/**********************************************************************************
  Function name     : Target_EnableInterrupts
  Description       : Start Timer interrupts for OS counters
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks           : This function is implementation dependent on selection of Gpt
                      channel for OS counter tick.
  Remarks           : If user change Gpt channel for OS counter, this implementation
                      need to be updated by user accordingly.
  Remarks           : Tick interrupt will be enabled only after RTA-OS initialzied 
                      and MCAL initialized.
***********************************************************************************/
void Target_EnableInterrupts(void)
{
    /* ETAS BIP Porting */
    system_tick_start();

    dmu_irq_enable(IRQ_TIMER1,1);
    Gpt_EnableNotification(GPT_HW_TIMER2);
}


/**********************************************************************************
  Function name     : Target_ClearPendingInterrupt
  Description       : Clear source of Millisecond interrupt which for RTE tick measurement
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks:
***********************************************************************************/
#define TARGET_START_SEC_CODE
#include "MemMap.h"
void Target_ClearPendingInterrupt(void)
{
    /* ETAS BIP Porting */
    timer_int_clear(0 /*CHIP_SYS_TIMER_ID*/);
    chip_timer_update(1000 /*CHIP_SYS_TIMER_HZ*/);
}
#define TARGET_STOP_SEC_CODE
#include "MemMap.h"


/**********************************************************************************
  Function name     : Target_GetSysTicks32Bits
  Description       : Get the value of a 32bit free-running hardware counter.
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : uint32 - value of a 32bit counter
  Remarks:
***********************************************************************************/
#define TARGET_START_SEC_CODE
#include "MemMap.h"
uint32 Target_GetSysTicks32Bits(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.5 Target Clock
     * User integration required.
     * Please see the integration guide section for further details.
     */

    /****************************************************************************
     * The Stopwatch uses the peripheral Timer channel 1 (MCAL Config Timer3)
     ***************************************************************************/
    return Gpt_GetTimeElapsed(GPT_HW_TIMER3);
}
#define TARGET_STOP_SEC_CODE
#include "MemMap.h"


/**********************************************************************************
  Function name     : Target_Get1msTicks
  Description       : Provide a software counter which ticks at 1ms.
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : TickType - value of a 32bit counter
  Remarks:
***********************************************************************************/
#define TARGET_START_SEC_CODE
#include "MemMap.h"
TickType Target_Get1msTicks(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.6 Xcp
     * User integration required.
     * Please see the integration guide section for further details.
     * [$Satisfies $BIP_INTG 112]
     */
    TickType tickValue;
    GetCounterValue(Millisecond_TickCounter, &tickValue);
    return (tickValue);
}
#define TARGET_STOP_SEC_CODE
#include "MemMap.h"


/**********************************************************************************
  Function name     : DefaultInterruptHandler
  Description       : Default Interrupt handler for unknown ISR source
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks:
***********************************************************************************/
#define OS_START_SEC_APPL_CODE
#include "Os_MemMap.h"
CAT1_ISR(DefaultInterruptHandler)
{
    for (;;)
    {
        /**
        * @brief ETAS BIP Integration Guide: 6.4.10 Trap Handling
        * User Integration of target dependend traps required.
        * Please see the integration guide section for further details.
        */
    }
}
#define OS_STOP_SEC_APPL_CODE
#include "Os_MemMap.h"


/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
