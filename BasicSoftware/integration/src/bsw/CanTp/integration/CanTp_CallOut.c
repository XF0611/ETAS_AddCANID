/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 

 
#include "ComStack_Types.h"
#include "CanTp.h"

#define CANTP_START_SEC_FDCALLOUT_CODE
#include "CanTp_MemMap.h"
/***********************************************************************************************************************
 Function name    : CanTp_XX_YY_ExternalFdSupportCallback
 Syntax           : CanTp_XX_YY_ExternalFdSupportCallback(RxPduId, PduInfoPtr, FdEnabled)
 Description      : Function will be as a final call just before starting the functionality.
 Parameter        : PduIdType, PduInfoType*, boolean
 Return value     : Std_ReturnType
***********************************************************************************************************************/
Std_ReturnType CanTp_XX_YY_ExternalFdSupportCallback(PduIdType RxPduId, const PduInfoType *PduInfoPtr,
                                                     const boolean FdEnabled)
{
    (void)RxPduId;
    (void)PduInfoPtr;
    (void)FdEnabled;

    return E_OK;
}

#define CANTP_STOP_SEC_FDCALLOUT_CODE
#include "CanTp_MemMap.h"
