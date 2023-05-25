

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "WdgM.h"           /* For visibility of prototype of function WdgM_CheckpointReached() */
#include "WdgM_Bsw_User.h"  /* For visibility of prototype of function WdgM_Rb_Cfg_IndicateCheckPointReached_...() */

/*
**********************************************************************************************************************
* Function Definition
**********************************************************************************************************************
*/

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"

/* Function to indicate check point reached.
 * Make copy of the following function if more than one check point reached is to be indicated.
 * Schedule the function/s in Os.
 * In case it is for the indication of Alive Supervision of a task,it's suggested to schedule it at the top of the task
 */
/* Note: Name of the function "WdgM_Rb_Cfg_IndicateCheckPointReached_<name_corresponding_to_checkpoint>" shall not
 * exceed 60 characters */


#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
