#ifndef PDUR_DCM_H
#define PDUR_DCM_H

#include "PduR.h"

#include "Dcm.h"
#if(PDUR_ENABLE_INTER_MODULE_CHECKS)
#if (!defined(DCM_AR_RELEASE_MAJOR_VERSION) || (DCM_AR_RELEASE_MAJOR_VERSION != PDUR_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(DCM_AR_RELEASE_MINOR_VERSION) || (DCM_AR_RELEASE_MINOR_VERSION != PDUR_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif /* #if(PDUR_ENABLE_INTER_MODULE_CHECKS) */
#include "Dcm_Cbk.h"

/* Interface translation layers ------------------------------------- */
/**
 * @ingroup PDUR_DCM_H
 *
 *This Macro gets generated through code generation which will be mapped to actual API
 */
#define PduR_RF_Dcm_StartOfReception_Func        Dcm_StartOfReception
#define PduR_RF_Dcm_CopyRxData_Func              Dcm_CopyRxData
#define PduR_RF_Dcm_CopyTxData_Func              Dcm_CopyTxData
#define PduR_RF_Dcm_TpRxIndication_Func          Dcm_TpRxIndication
#define PduR_RF_Dcm_TpTxConfirmation_Func        Dcm_TpTxConfirmation

/**
 * @ingroup PDUR_DCM_H
 *
 *  This function is called by the DCM to request a transmission.
 *\n
 * @param in          id -  ID of DCM I-PDU to be transmitted.\n
 * @param out         const PduInfoType * ptr -   Pointer to a structure with I-PDU related data that shall be transmitted:
 *                                         data length and pointer to I-SDU buffer\n
 *\n
 * @return            E_OK: Transmit request has been accepted.\n
 *                    E_NOT_OK: Transmit request has not been accepted.\n
 */
extern Std_ReturnType PduR_DcmTransmit(PduIdType id, const PduInfoType *ptr);

/* ------------------------------------------------------------------- */
/**
 * @ingroup PDUR_DCM_H
 *      This function is called by the DCM to request a transmission cancellation.\n
 *\n
 * @param in        id: ID of DCM I-PDU to be cancelled the transmission.\n
 *\n
 * @return          E_OK: Cancellation request has been accepted.\n
 *                  E_NOT_OK: Cancellation request has not been accepted.\n
 *
 **************************************************************************************************
 */
extern Std_ReturnType PduR_DcmCancelTransmit(PduIdType id);

#endif /* PDUR_DCM_H    */
