/*********************************************************************************************************************
*
* Description:     Watchdog function porting.
* FileName:        FBL_WdgM_Port.c
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2019. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
RHT2HC          2020/12/07     modified
**********************************************************************************************************************/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "FBL_WdgM_Port.h"
#include "Wdg_Integration.h"
#include "Os_TimeServices.h"
#include "Gpt_Integration.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
//#define FBL_WDGM_TESTING

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
static uint32  Fbl_WdgTimer = 0;      /* Watchdog timer */

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/
/* [$Satisfies $FAWER_FBL_DD_Wdg 101] */
/**
 * @brief Initialize Watchdog module
 */
void Fbl_Port_InitWdg(void)
{   
    Wdg_Init(&Wdg_ConfigData);
    
    /* Get position start trigger Watchdog */
    (void)OS_Timer32_Start(&Fbl_WdgTimer);
}

/* [$Satisfies $FAWER_FBL_DD_Wdg 102] */
/**
 * @brief Main function of Watchdog to trigger Watchdog
 */
void Fbl_Port_WdgMainFunction(void)
{
#ifdef FBL_WDGM_TESTING
    static volatile boolean enableLoop = 0;
    while(TRUE == enableLoop)
	{

	}
#endif

    /* Trigger watchdog when elapse time greater half of window time */
    if(TRUE == OS_Timer32_IsElapsed(Fbl_WdgTimer, FBL_PORT_WDG_TRIGGER_POINT))
    {
        /* Trigger watchdog */
        Fbl_Port_TriggerWdg();
    }
}

/* [$Satisfies $FAWER_FBL_DD_Wdg 104] */
/**
 * @brief Disable watchdog module
 */
void Fbl_Port_DisableWdg(void)
{
    /* Request to set Watchdog mode to OFF */
    Wdg_SetMode(WDGIF_OFF_MODE);         /* PRQA S 3200 */
}

/* [$Satisfies $FAWER_FBL_DD_Wdg 103] */
/**
 * @brief   Trigger watchdog
 */
void Fbl_Port_TriggerWdg(void)
{
    uint32 Timeout = FBL_PORT_WDG_WINDOW_TIMEOUT;
    /* Trigger watchdog */
    Wdg_SetTriggerCondition((const uint16)Timeout);
	(void)OS_Timer32_Start(&Fbl_WdgTimer);
}

