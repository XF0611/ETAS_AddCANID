/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 

#ifndef CANIF_INTEGRATION_H
#define CANIF_INTEGRATION_H


/*
 *  Set the MCAL minor release version here.
 *  Example:
 *  For AR 4.0.3, the minor version is 0U
 *  For AR 4.2.2, the minor version is 2U
 *  For AR 4.3.1, the minor version is 3U
 *  Default is 2U for compliance with AR 4.2.2
*/
#define CANIF_RX_INDICATION_VERSION (3U)

#define CANIF_WRITE_INTEGRATION_VERSION (3U)

#define CANIF_CONTROLLERMODEINDICATION_VERSION (3U)

#define CANIF_SETCONTROLLERMODE_INTEGRATION_VERSION (3U)


#include "CanIf.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

/* BSW-415 */
#if(CANIF_RX_INDICATION_VERSION < 1)
extern FUNC(void, CANIF_CODE) CanIf_RxIndication(
                                                 VAR(Can_HwHandleType, AUTOMATIC) Hrh,
                                                 VAR(Can_IdType, AUTOMATIC) CanId,
                                                 VAR(uint8, AUTOMATIC) CanDlc,
                                                 P2CONST (uint8, AUTOMATIC, CANIF_APPL_DATA) CanSduPtr
                                                );
#elif(CANIF_RX_INDICATION_VERSION >= 1)
extern FUNC(void, CANIF_CODE) CanIf_RxIndication(
                                            P2CONST (Can_HwType, AUTOMATIC, CANIF_APPL_DATA) Mailbox,
                                            P2CONST (PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr
                                         );
#endif

/* BSW-9662 */ /* [$DD_BSWCODE 17054] */ /* [$DD_BSWCODE 17055] */
extern Can_ReturnType CanIf_Write_Integration(Can_HwHandleType Hth,const Can_PduType* PduInfoPtr);

/* BSW-10854 */
#if(CANIF_CONTROLLERMODEINDICATION_VERSION > 2)
extern void CanIf_ControllerModeIndication( uint8 ControllerId, Can_ControllerStateType ControllerMode);
#elif(CANIF_CONTROLLERMODEINDICATION_VERSION <= 2)
extern void CanIf_ControllerModeIndication( uint8 ControllerId, CanIf_ControllerModeType ControllerMode);
#endif

/* BSW-10855 */ /* [$DD_BSWCODE 25562] */
Can_ReturnType CanIf_SetControllerMode_Integration( uint8 Controller, Can_StateTransitionType Transition);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif /* CANIF_INTEGRATION_H */
