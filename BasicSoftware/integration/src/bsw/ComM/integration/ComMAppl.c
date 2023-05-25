/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



/*
 * This file includes application functions that are called by CanNm
 * A dummy implementation is provided here. User has to implement the actual functionality
 * */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComM_Priv.h"
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */
/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */



/***************************************************************************************************
 Function name    : ComMAppl_IncompatibleGenerator

 Description      : This is ComMAppl function called when the Incompatible version of the generator
                    is used in comparison to what was originally used.
                    This error is detected during ComM_Init() function call
                    If there is an error, then ComM module will remain in COMM_STATE_UNINIT.
                    It will not be posisble to use the module functionality, in case of this failure
                    NOTE : If DET is enabled, then most of APIs check if the module is initialised. If DET is disabled
                    most of the APIs will not check for uninitialised state. If the module APIs are called when this
                    error happens , s/w might go to trap.
                    Hence user has to ensure that when this error is detected, module APIs are not called. Ex : Dont
                    start OS
                    This funtion is called only when post-build loadable feature is used.
 Parameter        : void
 Return value     : void
 ***************************************************************************************************
 */
#if(COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

void ComMAppl_IncompatibleGenerator(void)
{
    // User can report DET error / DEM error etc here

}


#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif



