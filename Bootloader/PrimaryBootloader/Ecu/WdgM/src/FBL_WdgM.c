/*********************************************************************************************************************
*
* Description:     DataM module private configuration implementation
* FileName:        FBL_WdgM.c
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
**********************************************************************************************************************/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/

#include "FBL_WdgM_Port.h"
#include "FBL_WdgM.h"
#include "FBL_WdgMCfg.h"

/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables Implementation
**********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
**********************************************************************************************************************/
/* [$Satisfies $FAWER_FBL_DD_Wdg 105] */
/*
 * @brief Initialize Watchdog and SMU module
 * */
void Fbl_WdgM_init(void)
{
    Fbl_Port_InitWdg();
}

/* [$Satisfies $FAWER_FBL_DD_Wdg 106] */
/*
 * @brief Watchdog main function
 * */
void Fbl_WdgM_Mainfunction(void)
{
    Fbl_Port_WdgMainFunction();
}

/* [$Satisfies $FAWER_FBL_DD_Wdg 107] */
/**
 * @brief Disable watchdog module
 */
void Fbl_WdgM_Disable(void)
{
    Fbl_Port_DisableWdg();
}

/* [$Satisfies $FAWER_FBL_DD_Wdg 108] */
/**
 * @brief Trigger watchdog
 */
void Fbl_WdgM_TriggerWdg(void)
{
    Fbl_Port_TriggerWdg();
}
