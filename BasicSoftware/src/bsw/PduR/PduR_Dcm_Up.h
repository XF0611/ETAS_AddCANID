#ifndef PDUR_DCM_UP_H
#define PDUR_DCM_UP_H

#include "PduR_Prv.h"

/**
 * @ingroup PDUR_DCM_H
 *
 * To ensure that the function declarations in this header are located in the code section.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

#if defined(PDUR_CONFIG_SINGLE_TPTX_LO)
#define PduR_iDcmTransmitID(id)           (id)                                   
#define PduR_iDcmTransmitFunc(id)           (PDUR_CONFIG_SINGLE_TPTX_LO(Transmit)) 
#else
#define PduR_iDcmTransmitID(id)           (PDUR_DCM_TX_BASE[PDUR_DCM_TX_MAP_TAB[(id)]].loId)          
#define PduR_iDcmTransmitFunc(id)           (PduR_loTransmitTable[(PDUR_DCM_TX_BASE[PDUR_DCM_TX_MAP_TAB[(id)]].loTransmitID)].PduR_loTransmitFunc)
#endif /* PDUR_CONFIG_SINGLE_TPTX_LO */

/**
 * @ingroup PDUR_DCM_H
 *      This function is called by the DCM to request a transmission. \n
 * \n
 * @param in        id: ID of DCM I-PDU to be transmitted. \n
 *                  const PduInfoType * ptr: Pointer to a structure with I-PDU related data that shall be transmitted:
 *                                         data length and pointer to I-SDU buffer. \n
 * \n
 * @return          E_OK: Transmit request has been accepted.\n
 *                  E_NOT_OK: Transmit request has not been accepted.\n
 */
extern Std_ReturnType PduR_dDcmTransmit(PduIdType id, const PduInfoType *ptr);

#if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)
 #define PduR_aDcmTransmit(id, ptr)        PduR_dDcmTransmit((id), (ptr))
#else

#define PduR_aDcmTransmit(id, ptr)        PduR_iDcmTransmitFunc(id)(PduR_iDcmTransmitID(id), (ptr))
#endif /* PDUR_DEV_ERROR_DETECT */

#define PduR_rDcmTransmit(id, ptr)         PduR_aDcmTransmit((id), (ptr))

/* Multicast mapping ------------------------------------------------ */

/**
 * @ingroup PDUR_DCM_H
 *      Support function to handle multicast from PduR_DcmTransmit.\n
 *
 * @param in        id                      : multicast ID to be transmitted.\n
 * @param in        const PduInfoType * info  : Pointer to a structure with PDU related data that shall be transmitted:
 *                                            data length and pointer to buffer.\n
 * \n
 * @return          E_OK: Transmit request has been accepted.\n
 *                  E_NOT_OK: Transmit request has not been accepted.\n
 */
extern Std_ReturnType PduR_MF_Dcm_Transmit_Func(PduIdType id,
		const PduInfoType *info);

/* Invalid PDU id handlers ------------------------------------------ */
#define PDUR_IH_Dcm_Transmit_Func    PDUR_DET_API(PduR_invId_UpTransmit)

/* PDUR481: PduR Cancel Transmit Feature */

#if defined(PDUR_CONFIG_SINGLE_TPCANCELTX_LO)
#define PduR_iDcmCancelTransmitID(id)           (id)
#define PduR_iDcmCancelTransmitFunc(id)         (PDUR_CONFIG_SINGLE_TPCANCELTX_LO(CancelTransmit))
#else
#define PduR_iDcmCancelTransmitID(id)           (PDUR_DCM_TX_BASE[PDUR_DCM_TX_MAP_TAB[(id)]].loId)
#define PduR_iDcmCancelTransmitFunc(id)         (PduR_loCancelTransmitTable[(PDUR_DCM_TX_BASE[PDUR_DCM_TX_MAP_TAB[(id)]].loCancelTransmitID)].PduR_loCancelTransmitFunc)
#endif /* PDUR_CONFIG_SINGLE_TPCANCELTX_LO */

/**
 * @ingroup PDUR_DCM_H
 *
 * Anything after this point will not be placed in the code section.
 */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif /* PDUR_DCM_UP_H */

