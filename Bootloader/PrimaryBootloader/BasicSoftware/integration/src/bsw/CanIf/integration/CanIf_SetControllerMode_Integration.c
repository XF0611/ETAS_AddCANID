/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 

#include "CanIf_Integration.h"


/***************************************************************************************************
 Function name    : CanIf_SetControllerMode_Integration

 Description      : This service uses to ensure that Can_SetControllerMode request works well across AR versions.
    The main reason is from AR 4.3, Can_SetControllerMode function prototype is changed to used Can_ControllerStateType parameter

    For AR version <= 4.2, this API will:
                - directly route the request to Can Driver
    For AR version > 4.2, this API will:
                - Mapping Transition of Can_StateTransitionType into requestedState of Can_ControllerStateType
                - call the 4.3 AR Can_SetControllerMode API.
 BSW-10855
 ***************************************************************************************************
 */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

/* [$DD_BSWCODE 25562] */
Can_ReturnType CanIf_SetControllerMode_Integration( uint8 Controller, Can_StateTransitionType Transition)
{
    Can_ReturnType retVal = CAN_NOT_OK;

#if(CANIF_SETCONTROLLERMODE_INTEGRATION_VERSION > 2)
    Can_ControllerStateType requestedState;

    /* AR 4.3.1 and AR 4.2.2 defines Can_SetControllerMode function prototype differently in the second parameter
     * AR 4.2.2 uses Can_StateTransitionType; however, 4.3.1 uses Can_ControllerStateType
     * Below code are the mapping for them.
     */
    switch (Transition)
    {
        case CAN_T_START:
            requestedState = CAN_CS_STARTED;
            break;
        case CAN_T_STOP:
            requestedState = CAN_CS_STOPPED;
            break;
        case CAN_T_SLEEP:
            requestedState = CAN_CS_SLEEP;
            break;
        case CAN_T_WAKEUP:
            requestedState = CAN_CS_STOPPED;
            break;
        default: //CAN_T_MAXTRANSITION is not used by CanIf anywhere.
            requestedState = CAN_CS_UNINIT;
    }
    // enum definition of Can_ReturnType are of the same value and meaning with Std_ReturnType.
    // typecast to Can_ReturnType makes no change in term of meaning and functionality.
    retVal = (Can_ReturnType)Can_SetControllerMode(Controller, requestedState);
#elif(CANIF_SETCONTROLLERMODE_INTEGRATION_VERSION <= 2)
    retVal = Can_SetControllerMode(Controller, Transition);
#endif

    return retVal;
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
