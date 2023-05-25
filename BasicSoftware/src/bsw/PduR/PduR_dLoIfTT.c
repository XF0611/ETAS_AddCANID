
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: PduR  / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#include "PduR_Prv.h"

#include "PduR_Cfg.h"
/* Appropriate header files are included to declare the prototypes
 */
#include "PduR_LoIfTT.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_CanNmRxIndication
 *    This function is called by the CanNm after a FlexRay L-PDU has been received.
 *
 *
 * \param           PduIdType Id    : ID of FlexRay L-PDU that has been received.
 *                  const uint8 *ptr: Pointer to FlexRay SDU (buffer of received payload)
 *
 * \retval          None
 * \seealso         PDUR195
 * \usedresources
 **************************************************************************************************
 */

void PduR_CanNmRxIndication(PduIdType id, const PduInfoType *ptr) {
	PduR_rCanNmRxIndication((id), (ptr));
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_CanNmTxConfirmation - This function is called by the CanNm after the PDU has been transmitted on the
 *                           network.
 *
 *
 * \param           PduIdType id -  ID of  L-PDU that has been transmitted.
 *                                  Range: 0..(maximum number of L-PDU IDs which may be transmitted by
 *                                             Interface) - 1
 *
 *
 * \retval          None
 * \seealso         PDUR196
 * \usedresources
 **************************************************************************************************
 */
void PduR_CanNmTxConfirmation(PduIdType id) {
	PduR_rCanNmTxConfirmation(id);
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_CanNmTriggerTransmit - This function is called by the CanNm for sending out a  frame.
 * The trigger transmit is initiated by the  schedule. Whether this function is called or not is statically
 * configured for each PDU.
 *
 *
 * \param           PduIdType id -  ID of FlexRay L-PDU that is requested to be transmitted.
 *                                  Range: 0..(maximum number of L-PDU IDs which may be transmitted by FlexRay
 *                                             Interface) - 1
 *
 *                  uint8 *  ptr - Pointer to place inside the transmit buffer of the L-PDU where data shall be copied
 *                                 to.
 *
 * \retval          None
 * \seealso         PDUR199
 * \usedresources
 **************************************************************************************************
 */

Std_ReturnType PduR_CanNmTriggerTransmit(PduIdType id, PduInfoType *ptr) {
	return ((Std_ReturnType) PduR_rCanNmTriggerTransmit((id), (ptr)));
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_dCanNmRxIndication - Function to be invoked DET enable for PduR_CanNmRxIndication
 *    This function is called by the CanNm after a  L-PDU has been received.
 *
 *
 * \param           PduIdType Id    : ID of FlexRay L-PDU that has been received.
 *                  const uint8 *ptr: Pointer to FlexRay SDU (buffer of received payload)
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void PduR_dCanNmRxIndication(PduIdType id, const PduInfoType *ptr) {

	PDUR_CHECK_STATE_VOID (PDUR_SID_CANNMRXIND)

	PDUR_CHECK_PTR_VOID(PDUR_SID_CANNMRXIND, ptr)

	/*Check for Invalid Id and null pointer */
	if ((id >= PDUR_NR_VALID_CANNMIFRXTOUP_IDS)
			|| (PDUR_CANNM_RXIND_MAP_TAB[(id)] == PDUR_INVALID_ID)
			|| (PduR_iCanNmRxIndicationFunc(id) == NULL_PTR)) {
		PDUR_REPORT_ERROR(PDUR_SID_CANNMRXIND, PDUR_E_PDU_ID_INVALID);
		return;
	}

	/*Call the upper layer Rxindication function */
	PduR_iCanNmRxIndicationFunc(id)(PduR_iCanNmRxIndicationID(id), ptr);
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_dCanNmTriggerTransmit - Function to be invoked DET enable for PduR_CanNmTriggerTransmit
 * This function is called by the CanNm for sending out a  frame.
 * The trigger transmit is initiated by the schedule. Whether this function is called or not is statically
 * configured for each PDU.
 *
 *
 * \param           PduIdType id -  ID of FlexRay L-PDU that is requested to be transmitted.
 *                                  Range: 0..(maximum number of L-PDU IDs which may be transmitted by FlexRay
 *                                             Interface) - 1
 *
 *                  uint8 *  ptr - Pointer to place inside the transmit buffer of the L-PDU where data shall be copied
 *                                 to.
 *
 * \retval          None
 * \seealso         PDUR199
 * \usedresources
 **************************************************************************************************
 */
Std_ReturnType PduR_dCanNmTriggerTransmit(PduIdType id, PduInfoType *ptr) {

	PDUR_CHECK_STATE_RET(PDUR_SID_CANNMTRIGTX, E_OK)

	PDUR_CHECK_PTR_RET(PDUR_SID_CANNMTRIGTX, ptr, E_OK)

	/*Check for Invalid Id and null pointer */
	if ((id >= PDUR_NR_VALID_CANNMIFTXTOUP_IDS)
			|| (PDUR_CANNM_TXCONF_MAP_TAB[(id)] == PDUR_INVALID_ID)
			|| (PduR_iCanNmTriggerTransmitFunc(id) == NULL_PTR)) {
		PDUR_REPORT_ERROR(PDUR_SID_CANNMTRIGTX, PDUR_E_PDU_ID_INVALID);
		return E_OK;
	}

	/*Call the upper layer trigger Transmit function */
	return ((Std_ReturnType) PduR_iCanNmTriggerTransmitFunc(id)(
			PduR_iCanNmTriggerTransmitID(id), ptr));
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_dCanNmTxConfirmation - Function to be invoked DET enable for PduR_CanNmTxConfirmation
 *    This function is called by the CanNm after the PDU has been transmitted on the
 *    network.
 *
 *
 * \param           PduIdType id -  ID of FlexRay L-PDU that has been transmitted.
 *                                  Range: 0..(maximum number of L-PDU IDs which may be transmitted by FlexRay
 *                                             Interface) - 1
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void PduR_dCanNmTxConfirmation(PduIdType id) {
	/*begin */
	PDUR_CHECK_STATE_VOID (PDUR_SID_CANNMTXCONF)
	/*end */

	/*begin */
	/*begin */
	if ((id >= PDUR_NR_VALID_CANNMIFTXTOUP_IDS)
			|| (PDUR_CANNM_TXCONF_MAP_TAB[(id)] == PDUR_INVALID_ID)
			|| (PduR_iCanNmTxConfirmationFunc(id) == NULL_PTR)) {
		PDUR_REPORT_ERROR(PDUR_SID_CANNMTXCONF, PDUR_E_PDU_ID_INVALID);
		return;
	}
	/*end */
	/*end */

	PduR_iCanNmTxConfirmationFunc(id)(PduR_iCanNmTxConfirmationID(id));
}
/* ------------------------------------------------------------------------ */
/* End section for code */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

