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
 * This file includes application functions that are called by PduR
 * A dummy implementation is provided here. User has to implement the actual functionality
 * */

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "PduR_Prv.h"

/***************************************************************************************************
 Function name    : PduRAppl_IncompatibleGenerator

 Description      : This is PduRAppl function called when the Incompatible version of the generator
                    is used in comparison to what was originally used.
                    This error is detected during PduR_Init() function call
                    If there is an error, then PduR module will remain in PDUR_UNINIT state.
                    It will not be posisble to use the module functionality, in case of this failure
                    NOTE : If DET is enabled, then most of APIs check if the module is initialised. If DET is disabled
                    most of the APIs will not check for uninitialised state. If the module APIs are called when this
                    error happens , s/w might go to trap.
 Parameter        : void
 Return value     : void
 ***************************************************************************************************
 */
#if (PDUR_CONFIGURATION_VARIANT != PDUR_VARIANT_PRE_COMPILE)

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

void PduRAppl_IncompatibleGenerator(void)
{
    /* User can report DET error / DEM error etc here*/

}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif
