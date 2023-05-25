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
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Std_Types.h"
#include "CanIf_Cbk.h"
#include "CanIf_Integration.h"


/***************************************************************************************************
 Function name    : CanIf_ControllerModeIndication

 Description      : Service indicates a mode change is successful for the particular
                      CAN controller. This service is called by the CAN driver. The actual handling
                      is implemented in CanIf_ControllerModeIndication_Internal(), which is called
                      at the end of this function.
                    The function prototype can be changed using CANIF_CONTROLLERMODEINDICATION_VERSION.
    For AR versions <= 4.2, this API is used:
       void CanIf_ControllerModeIndication( uint8 ControllerId, CanIf_ControllerModeType ControllerMode)

    For AR versions > 4.2, this API is used:
       void CanIf_ControllerModeIndication( uint8 ControllerId, Can_ControllerStateType ControllerMode)
 BSW-10854
 ***************************************************************************************************
 */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

#if(CANIF_CONTROLLERMODEINDICATION_VERSION > 2)
void CanIf_ControllerModeIndication( uint8 ControllerId,
        Can_ControllerStateType ControllerMode )
{
    VAR(CanIf_ControllerModeType, AUTOMATIC) ControllerMode_tmp = CANIF_CS_UNINIT;

    switch (ControllerMode)
    {
        case CAN_CS_SLEEP:
            ControllerMode_tmp = CANIF_CS_SLEEP;
            break;
        case CAN_CS_STARTED:
            ControllerMode_tmp = CANIF_CS_STARTED;
            break;
        case CAN_CS_STOPPED:
            ControllerMode_tmp = CANIF_CS_STOPPED;
            break;
        case CAN_CS_UNINIT:
            ControllerMode_tmp = CANIF_CS_UNINIT;
            break;
        default:
            break;
    }

    CanIf_ControllerModeIndication_Internal(ControllerId, ControllerMode_tmp);

#elif(CANIF_CONTROLLERMODEINDICATION_VERSION <= 2)
void CanIf_ControllerModeIndication( uint8 ControllerId,
        CanIf_ControllerModeType ControllerMode )
{

    CanIf_ControllerModeIndication_Internal(ControllerId, ControllerMode);
#endif

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

