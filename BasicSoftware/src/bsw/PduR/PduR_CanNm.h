#ifndef PDUR_CANNM_H
#define PDUR_CANNM_H

#include "PduR.h"

#include "CanNm.h"
#if(PDUR_ENABLE_INTER_MODULE_CHECKS)
#if (!defined(CANNM_AR_RELEASE_MAJOR_VERSION) || (CANNM_AR_RELEASE_MAJOR_VERSION != PDUR_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANNM_AR_RELEASE_MINOR_VERSION) || (CANNM_AR_RELEASE_MINOR_VERSION != PDUR_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif /* #if(PDUR_ENABLE_INTER_MODULE_CHECKS) */

/* Interface translation layers ------------------------------------- */

#define PduR_RF_CanNm_Transmit_Func   CanNm_Transmit

/* PduR_CanNmRxIndication  */
extern void PduR_CanNmRxIndication(PduIdType id, const PduInfoType *ptr);

/* PduR_CanNmTxConfirmation  */

extern void PduR_CanNmTxConfirmation(PduIdType id);

/* PduR_CanNmTriggerTransmit  */
extern Std_ReturnType PduR_CanNmTriggerTransmit(PduIdType id, PduInfoType *ptr);

#endif /* PDUR_CANNM_H    */
