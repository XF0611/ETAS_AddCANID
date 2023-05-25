#ifndef PDUR_CANTP_H
#define PDUR_CANTP_H

#include "PduR.h"

#include "CanTp.h"
#if(PDUR_ENABLE_INTER_MODULE_CHECKS)
#if (!defined(CANTP_AR_RELEASE_MAJOR_VERSION) || (CANTP_AR_RELEASE_MAJOR_VERSION != PDUR_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANTP_AR_RELEASE_MINOR_VERSION) || (CANTP_AR_RELEASE_MINOR_VERSION != PDUR_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif /* #if(PDUR_ENABLE_INTER_MODULE_CHECKS) */

/* Interface translation layers ------------------------------------- */
#define PduR_RF_CanTp_Transmit_Func             CanTp_Transmit
#define PduR_RF_CanTp_CancelTransmit_Func       CanTp_CancelTransmit

/* PduR_CanTpRxIndication  */
extern void PduR_CanTpRxIndication(PduIdType id, Std_ReturnType std);

/* PduR_CanTpTxConfirmation  */
extern void PduR_CanTpTxConfirmation(PduIdType id, Std_ReturnType std);

extern BufReq_ReturnType PduR_CanTpProvideTxBuffer(PduIdType id,
		PduInfoType **ptr, uint16 len);

extern BufReq_ReturnType PduR_CanTpProvideRxBuffer(PduIdType id,
		PduLengthType len, PduInfoType **ptr);

/* PduR_CanTpCopyRxData  */
extern BufReq_ReturnType PduR_CanTpCopyRxData(PduIdType id,
		const PduInfoType *info, PduLengthType *bufferSizePtr);

/* PduR_CanTpStartOfReception */
extern BufReq_ReturnType PduR_CanTpStartOfReception(PduIdType id,
		const PduInfoType *info, PduLengthType TpSduLength,
		PduLengthType *bufferSizePtr);
/* PduR_CanTpCopyTxData  */
extern BufReq_ReturnType PduR_CanTpCopyTxData(PduIdType id,
		const PduInfoType *info, RetryInfoType *retry,
		PduLengthType *availableDataPtr);

#endif /* PDUR_CANTP_H    */

