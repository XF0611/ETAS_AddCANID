/* *****************************************************************************
 * BEGIN: Banner
 *-----------------------------------------------------------------------------
 *                                 ETAS GmbH
 *                      D-70469 Stuttgart, Borsigstr. 14
 *-----------------------------------------------------------------------------
 *    Administrative Information (automatically filled in by ISOLAR)         
 *-----------------------------------------------------------------------------
 * Name: 
 * Description:
 * Version: 1.0
 *-----------------------------------------------------------------------------
 * END: Banner
 ******************************************************************************

 * Project : ETAS_BIP_PT
 * Component: /ETAS_BIP/WdgMUT
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.1
 * Author: HUR4HC
 * Date : Fri May 20 15:49:04 2022
 ****************************************************************************/
#ifndef WDGMUT_H
#define WDGMUT_H
/*
 **********************************************************************************************************************
 * Type Define
 **********************************************************************************************************************
 */
typedef enum Wdg_Test_t{
	WDG_NONE,
	ALIVE_OK_LACKOF_ONE_CP_ALIVE_SUPERVISION_ENTITY,
	ALIVE_NOTOK_LACKOF_TWO_ALIVE_SUPERVISION_ENTITY,
	ALIVE_NOTOK_ADDMORE_ALIVE_SUPERVISION_ENTITY,
	DEADLINE_OK_ONTIME_CP_WDG_DEADLINE_ENTITY,
	DEADLINE_NOTOK_LATE_CP_WDG_DEADLINE_ENTITY,
	LOGICAL_OK_VALID_CP,
	LOGICAL_NOTOK_INVALID_INITIAL_CP,
	LOGICAL_NOTOK_INVALID_FINAL_CP,
}Wdg_Test_t;

typedef enum WdgMMode_Test_t{
	WDGM_FASTMODE,
	WDGM_SLOWMODE,
}WdgMMode_Test_t;


#endif /* WDGMUT_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
