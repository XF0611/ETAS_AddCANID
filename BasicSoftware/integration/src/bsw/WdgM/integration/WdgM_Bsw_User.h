


#ifndef WDGM_BSW_USER_H
#define WDGM_BSW_USER_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"   /* For visibility of definition of "FUNC" and "WDGM_CODE" */
/*
#if (!defined(STD_TYPES_AR_RELEASE_MAJOR_VERSION) || (STD_TYPES_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(STD_TYPES_AR_RELEASE_MINOR_VERSION) || (STD_TYPES_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
*/

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"

/* Function to indicate check point reached.
 * Make copy of the following function prototype if more than one check point reached is to be indicated.
 * Schedule the function/s in Os.
 * In case it is for the indication of Alive Supervision of a task,it's suggested to schedule it at the top of the task
 */
/* Note: Name of the function "WdgM_Rb_Cfg_IndicateCheckPointReached_<name_corresponding_to_checkpoint>" shall not
 * exceed 60 characters */


#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#endif /* WDGM_BSW_USER_H */
