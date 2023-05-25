/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



/*****************************************************************************************
 * Header Includes *
 *****************************************************************************************/

#include "Std_Types.h"
#include "BswM.h"
#include "BswM_Prv.h"

/***********************************************************
* Function name: void BswM_Appl_IncompatibleGenerator( void )
* Description: Function called by BswM in case of version incompatibility in the post-build generate.
* This an empty function. User will write the error handler code here.
* Return value: None
* Remarks:
***********************************************************/

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

void BswM_Appl_IncompatibleGenerator( void )
{
    /* Fill the error handling code to handle the version mis-match in the post-build structure */
    return;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"


/*
 **********************************************************************************************************************
*/
