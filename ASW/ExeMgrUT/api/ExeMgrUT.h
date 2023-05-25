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
 * Component: /ETAS_BIP/ExeMgr
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.1
 * Author: HUR4HC
 * Date : Fri Jul 20 15:49:04 2022
 ****************************************************************************/
#ifndef EXEMGRUT_H
#define EXEMGRUT_H

/*
 **********************************************************************************************************************
 * Type Define
 **********************************************************************************************************************
 */

typedef enum ShutdownTarget_type{
	SHUTDOWN_TARGET_SLEEP,
	SHUTDOWN_TARGET_RESET,
	SHUTDOWN_TARGET_OFF,
}ShutdownTarget_type;

typedef enum ShutdownMode_type{
	RESET_MCU,
	RESET_WDGM,
	RESET_IO,
}ShutdownMode_type;

typedef enum ShutdownCause_type{
	CAUSE_UNKNOWN,
	CAUSE_ECU_STATE,
	CAUSE_WDGM,
	CAUSE_DCM,
}ShutdownCause_type;

typedef enum Testing_t{
	INVALID_WAKEUP_SOURCE,
	VALID_WAKEUP_SOURCE,
	LAST_SHUTDOWN_TARGET_INTERFACE,
	SHUTDOWN_TARGET_INTERFACE,
	APP_MODE_REQUEST_APP_RUN,
	APP_MODE_REQUEST_POST_RUN,
	SHUTDOWN_CAUSE_INTERFACE,
	TESTING_NONE,
}Testing_t;

#endif /* EXEMGRUT_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */