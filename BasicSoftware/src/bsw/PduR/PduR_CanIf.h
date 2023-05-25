#ifndef PDUR_CANIF_H
#define PDUR_CANIF_H

#include "PduR.h"

#include "CanIf.h"
#if(PDUR_ENABLE_INTER_MODULE_CHECKS)
#if (!defined(CANIF_AR_RELEASE_MAJOR_VERSION) || (CANIF_AR_RELEASE_MAJOR_VERSION != PDUR_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANIF_AR_RELEASE_MINOR_VERSION) || (CANIF_AR_RELEASE_MINOR_VERSION != PDUR_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif /* #if(PDUR_ENABLE_INTER_MODULE_CHECKS) */

/* Interface translation layers ------------------------------------- */
#define PduR_RF_CanIf_Transmit_Func   CanIf_Transmit

/* PduR_CanIfRxIndication  */
extern void PduR_CanIfRxIndication(PduIdType id, const PduInfoType *ptr);
/* PduR_CanIfTxConfirmation  */
extern void PduR_CanIfTxConfirmation(PduIdType id);

#endif /* PDUR_CANIF_H    */
