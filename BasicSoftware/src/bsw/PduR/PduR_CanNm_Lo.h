#ifndef PDUR_CANNM_LO_H
#define PDUR_CANNM_LO_H

#include "PduR_Prv.h"

/* Ensure that the function declarations in this header are located in the code section. */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------- */
#if defined(PDUR_CONFIG_SINGLE_IFRX)
#define PduR_iCanNmRxIndicationID(id)        (id)
#define PduR_iCanNmRxIndicationFunc(id)        (PDUR_CONFIG_SINGLE_IFRX(RxIndication))
#else
#define PduR_iCanNmRxIndicationID(id)        (PDUR_CANNM_RXIND_BASE[PDUR_CANNM_RXIND_MAP_TAB[(id)]].upId)
#define PduR_iCanNmRxIndicationFunc(id)      (PduR_upIfRxIndicationTable[(PDUR_CANNM_RXIND_BASE[PDUR_CANNM_RXIND_MAP_TAB[(id)]].upRxIndicationID)].PduR_upIfRxIndicationFunc)

#endif /* PDUR_CONFIG_SINGLE_IFRX */

extern void PduR_dCanNmRxIndication(PduIdType id, const PduInfoType *ptr);

#if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)
#define PduR_aCanNmRxIndication(id, ptr)     PduR_dCanNmRxIndication((id), (ptr))
#else
#define PduR_aCanNmRxIndication(id, ptr)     PduR_iCanNmRxIndicationFunc(id)(PduR_iCanNmRxIndicationID(id), (ptr))
#endif /* PDUR_DEV_ERROR_DETECT */

#define PduR_rCanNmRxIndication(id, ptr)      PduR_aCanNmRxIndication((id), (ptr))

/* ------------------------------------------------------------------- */
#if defined(PDUR_CONFIG_SINGLE_IFTX_UP)
#define PduR_iCanNmTxConfirmationID(id)      (id)
#define PduR_iCanNmTxConfirmationFunc(id)      (PDUR_CONFIG_SINGLE_IFTX_UP(TxConfirmation))
#else
#define PduR_iCanNmTxConfirmationID(id)      (PDUR_CANNM_TXCONF_BASE[PDUR_CANNM_TXCONF_MAP_TAB[(id)]].upId)
#define PduR_iCanNmTxConfirmationFunc(id)    (PduR_upIfTxConfirmationTable[(PDUR_CANNM_TXCONF_BASE[PDUR_CANNM_TXCONF_MAP_TAB[(id)]].upTxConfirmationID)].PduR_upIfTxConfirmationFunc)

#endif /* PDUR_CONFIG_SINGLE_IFTX_UP */

extern void PduR_dCanNmTxConfirmation(PduIdType id);

#if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)
#define PduR_aCanNmTxConfirmation(id)        PduR_dCanNmTxConfirmation(id)
#else
#define PduR_aCanNmTxConfirmation(id)        PduR_iCanNmTxConfirmationFunc(id)(PduR_iCanNmTxConfirmationID(id))
#endif /* PDUR_DEV_ERROR_DETECT */

#define PduR_rCanNmTxConfirmation(id)         PduR_aCanNmTxConfirmation(id)

/* ------------------------------------------------------------------- */
#if defined(PDUR_CONFIG_SINGLE_IFTX_UP)
#define PduR_iCanNmTriggerTransmitID(id)     (id)
#define PduR_iCanNmTriggerTransmitFunc(id)     (PDUR_CONFIG_SINGLE_IFTX_UP(TriggerTransmit))
#else
#define PduR_iCanNmTriggerTransmitID(id)     (PDUR_CANNM_TXCONF_BASE[PDUR_CANNM_TXCONF_MAP_TAB[(id)]].upId)
#define PduR_iCanNmTriggerTransmitFunc(id)   (PduR_upIfTriggerTxTable[(PDUR_CANNM_TXCONF_BASE[PDUR_CANNM_TXCONF_MAP_TAB[(id)]].upTriggerTxID)].PduR_upIfTriggerTxFunc)

#endif /* PDUR_CONFIG_SINGLE_IFTX_UP */

extern Std_ReturnType PduR_dCanNmTriggerTransmit(PduIdType id,
		PduInfoType *ptr);

#if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)
#define PduR_aCanNmTriggerTransmit(id, ptr)  PduR_dCanNmTriggerTransmit((id), (ptr))
#else
#define PduR_aCanNmTriggerTransmit(id, ptr)  PduR_iCanNmTriggerTransmitFunc(id)(PduR_iCanNmTriggerTransmitID(id), (ptr))
#endif /* PDUR_DEV_ERROR_DETECT */

#define PduR_rCanNmTriggerTransmit(id, ptr)   PduR_aCanNmTriggerTransmit((id), (ptr))

/* Invalid PDU id handlers ------------------------------------------ */
#define PDUR_IH_CanNmRx_RxIndication_Func     PDUR_DET_API(PduR_invId_IfRxIndication)
#define PDUR_IH_CanNmTx_TxConfirmation_Func   PDUR_DET_API(PduR_invId_IfTxConfirmation)
#define PDUR_IH_CanNmTx_TriggerTransmit_Func  PDUR_DET_API(PduR_invId_IfTriggerTransmit)

/* Anything after this point will not be placed in the code section. */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif /* PDUR_CANNM_LO_H */

