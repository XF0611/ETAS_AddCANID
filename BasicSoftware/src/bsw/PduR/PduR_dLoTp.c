
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
#include "PduR_LoTp.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/**
 **************************************************************************************************
 * PduR_dCanTpRxIndication - PDU Router CanTpRxIndication.
 *        Function to be invoked DET enable for PduR_CanTpRxIndication
 *
 * This function is called by the CanTp TP.
 * with Result = E_OK after the complete CanTp TP data have successfully been
 * received, i.e. at the very end of the segmented TP receive cycle or after
 * receiving an unsegemented N-PDU.
 *
 * with Result != E_OK if an error (e.g. timeout) has occurred during the TP reception.
 * This enables unlocking of the receive buffer. It is undefined which part of the buffer
 * contains valid data in this case.
 *
 *
 * \param           PduIdType id -  ID of CanTp N-PDU that has been received.
 *                                  Range: 0..(maximum number of N-PDU IDs which may be received by CanTp TP) - 1
 *                  Std_ReturnType  std - Result of the TP reception.
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void PduR_dCanTpRxIndication(PduIdType id, Std_ReturnType std) {
	PDUR_CHECK_STATE_VOID (PDUR_SID_CANTPTPRXIND)

	/*Check for Invalid Id and null pointer */
	if ((id >= PDUR_NR_VALID_CANTPTPRXTOUP_IDS)
			|| (PDUR_CANTP_RXIND_MAP_TAB[(id)] == PDUR_INVALID_ID)
			|| (PduR_iCanTpRxIndicationFunc(id) == NULL_PTR)) {
		PDUR_REPORT_ERROR(PDUR_SID_CANTPTPRXIND, PDUR_E_PDU_ID_INVALID);
		return;
	}

	/*Call the upperlayer RxIndication Function */
	PduR_iCanTpRxIndicationFunc(id)(PduR_iCanTpRxIndicationID(id), std);
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
 * PduR_dCanTpTxConfirmation - Function to be invoked DET enable for PduR_CanTpTxConfirmation
 *
 *                  This function is called by the CanTp Transport Protocol:
 *
 *   with Result  =  E_OK after the complete CanTp TP data have successfully been transmitted,
 *                  i.e. at the very end of the segmented TP transmission cycle.
 *   with Result != E_OK if an error (e.g. timeout) has occurred during the TP transmission.
 *                  This enables unlocking of the transmit buffer.
 *
 *
 * \param           PduIdType id       :ID of CanTp N-PDU that has been transmitted.
 *                  Std_ReturnType std:Result of the TP reception.
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void PduR_dCanTpTxConfirmation(PduIdType id, Std_ReturnType std) {

	PDUR_CHECK_STATE_VOID (PDUR_SID_CANTPTPTXCONF)

	/*Check for Invalid Id and null pointer */
	if ((id >= PDUR_NR_VALID_CANTPTPTXTOUP_IDS)
			|| (PDUR_CANTP_TXCONF_MAP_TAB[(id)] == PDUR_INVALID_ID)
			|| (PduR_iCanTpTxConfirmationFunc(id) == NULL_PTR)) {
		PDUR_REPORT_ERROR(PDUR_SID_CANTPTPTXCONF, PDUR_E_PDU_ID_INVALID);
		return;
	}

	/*Call upper layer TxConfirmation function */
	PduR_iCanTpTxConfirmationFunc(id)(PduR_iCanTpTxConfirmationID(id), std);
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
 * PduR_dCanTpCopyRxData - This service is called by the CanTp TP for requesting a new buffer (pointer to a
 *                             PduInfoStructure containing a pointer to a SDU buffer and the buffer length) for the CanTp
 *                             TP to fill in the received data..
 *                  Function to be invoked DET enable for PduR_CanTpProvideRxBuffer
 *
 * \param           PduIdType id              :ID of CanTp N-PDU that shall be received
 *                  PduLengthType len         :This length identifies the overall number of bytes to be received.
 *                  PduInfoType ** ptr:Pointer to pointer to PduInfoStructure containing SDU data pointer and
 *                                             SDU length of a receive buffer.
 *
 * \retval          BUFREQ_OK       - Buffer request accomplished successful
 *                  BUFREQ_E_BUSY   - Currently no buffer of the requested size is available
 *                  BUFREQ_E_NOT_OK - Buffer request not successful, no buffer provided.
 *
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

BufReq_ReturnType PduR_dCanTpCopyRxData(PduIdType id, const PduInfoType *info,
		PduLengthType *bufferSizePtr) {
	PDUR_CHECK_STATE_RET(PDUR_SID_CANTPTPRXBUF, BUFREQ_E_NOT_OK)

	PDUR_CHECK_PTR_RET(PDUR_SID_CANTPTPRXBUF, info, BUFREQ_E_NOT_OK)

	/*Check for Invalid Id and null pointer */
	if ((id >= PDUR_NR_VALID_CANTPTPRXTOUP_IDS)
			|| (PDUR_CANTP_RXIND_MAP_TAB[(id)] == PDUR_INVALID_ID)
			|| (PduR_iCanTpCopyRxDataFunc(id) == NULL_PTR)) {
		PDUR_REPORT_ERROR(PDUR_SID_CANTPTPRXBUF, PDUR_E_PDU_ID_INVALID);
		return BUFREQ_E_NOT_OK;
	} else {
		/*Call the upper layer CopyRxData Function */
		return PduR_iCanTpCopyRxDataFunc(id)(PduR_iCanTpCopyRxDataID(id), info,
				bufferSizePtr);
	}
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
 * PduR_dCanTpStartOfReception - This service is called by the CanTp TP for requesting a new buffer (pointer to a
 *                             PduInfoStructure containing a pointer to a SDU buffer and the buffer length) for the CanTp
 *                             TP to fill in the received data..
 *                  Function to be invoked DET enable for PduR_CanTpProvideRxBuffer
 *
 * \param           PduIdType id              :ID of CanTp N-PDU that shall be received
 PduInfoType* info         :Pointer to a structure containing content and length of the first frame or 
 single frame including MetaData.
 *                  PduLengthType len         :This length identifies the overall number of bytes to be received.
 *                  PduInfoType ** ptr:Pointer to pointer to PduInfoStructure containing SDU data pointer and
 *                                             SDU length of a receive buffer.
 *
 * \retval          BUFREQ_OK       - Buffer request accomplished successful
 *                  BUFREQ_E_BUSY   - Currently no buffer of the requested size is available
 *                  BUFREQ_E_NOT_OK - Buffer request not successful, no buffer provided.
 *
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
BufReq_ReturnType PduR_dCanTpStartOfReception(PduIdType id,
		const PduInfoType *info, PduLengthType TpSduLength,
		PduLengthType *bufferSizePtr) {
	PDUR_CHECK_STATE_RET(PDUR_SID_CANTPTPSTARTOFRECEP, BUFREQ_E_NOT_OK)

	/*Check for Invalid Id and null pointer */
	if ((id >= PDUR_NR_VALID_CANTPTPRXTOUP_IDS)
			|| (PDUR_CANTP_RXIND_MAP_TAB[(id)] == PDUR_INVALID_ID)
			|| (PduR_iCanTpStartOfReceptionFunc(id) == NULL_PTR)) {
		PDUR_REPORT_ERROR(PDUR_SID_CANTPTPSTARTOFRECEP, PDUR_E_PDU_ID_INVALID);
		return BUFREQ_E_NOT_OK;
	} else {
		/*Call The upper layer StartOfReception function */
		return PduR_iCanTpStartOfReceptionFunc(id)(
				PduR_iCanTpStartOfReceptionID(id), info, TpSduLength,
				bufferSizePtr);
	}
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
 * PduR_dCanTpCopyTxData - This function is called by the CanTp TP for requesting a transmit buffer.
 *                 Function to be invoked DET enable for PduR_CanTpCopyTxData
 *
 * \param(In)       PduIdType id                      :ID of CanTp N-PDU to be transmitted
 *                  PduInfoType * ptr            :Pointer to PduInfoStructure containing SDU data pointer and
 *                                                      SDU length of a transmit buffer.
 *                  RetryInfoType retry              :This parameter is used to retransmit data because
 *                                                   problems occurred in transmitting it the last time.
 *                                                   If the I-PDU is transmitted from a local module (e.g. DCM)
 *                                                   the PDU router module will just forward the parameter value
 *                                                   without check. If the I-PDU is gatewayed from another bus
 *                                                   the PDU Router module will make the following interpretation:
 *                                                   If the transmitted TP I-PDU does not support the retry feature
 *                                                   a NULL_PTR can be provided. Alternatively TpDataState can
 *                                                   indicate TP_NORETRY. Both indicate that the copied transmit
 *                                                   data can be removed from the buffer after it has been copied.
 *                                                   If the retry feature is used by the Tx I-PDU, RetryInfoPtr must
 *                                                   point to a valid RetryInfoType element. If TpDataState
 *                                                   indicates TP_CONFPENDING the previously copied data must
 *                                                   remain in the TP buffer to be available for error recovery.
 *                                                   TP_DATACONF indicates that all data, that have been copied
 *                                                   so far, are confirmed and can be removed from the TP buffer.
 *                                                   Data copied by this API call are excluded and will be confirmed
 *                                                   later. TP_DATARETRY indicates that this API call shall copy
 *                                                   already copied data in order to recover from an error. In
 *                                                   this case TxTpDataCnt specifies the offset of the first byte
 *                                                   to be copied by the API call.
 * \param(Out)      PduLengthType availableDataPtr    :Indicates the remaining number of bytes that are available in the
 *                                                      PduR Tx buffer. AvailableTxDataCntPtr can be used by TP
 *                                                      modules that support dynamic payload lengths (e.g. Iso FrTp)
 *                                                      to determine the size of the following CFs.
 *
 * \retval          BUFREQ_OK       - Buffer request accomplished successful
 *                  BUFREQ_E_BUSY   - Currently no buffer of the requested size is available
 *                  BUFREQ_E_NOT_OK - Buffer request not successful, no buffer provided.
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
BufReq_ReturnType PduR_dCanTpCopyTxData(PduIdType id, const PduInfoType *info,
		RetryInfoType *retry, PduLengthType *availableDataPtr) {
	/* If the PDU Router has not been initialized (PDUR_UNINIT state) all
	 services except PduR_Init() shall report the error PDUR_E_INVALID_REQUEST via the Development Error Tracer (DET)
	 when called.*/
	PDUR_CHECK_STATE_RET(PDUR_SID_CANTPTPTXBUF, BUFREQ_E_NOT_OK)

	PDUR_CHECK_PTR_RET(PDUR_SID_CANTPTPTXBUF, info, BUFREQ_E_NOT_OK)

	/* The PDU identifier shall be within the specified range and shall be
	 configured to be used by the PDU Router for
	 routing according to the post-build routing tables (PDUR_ONLINE state). Otherwise PDUR_E_PDU_ID_INVALID shall be
	 reported to DET.*/

	if ((id >= PDUR_NR_VALID_CANTPTPTXTOUP_IDS)
			|| (PDUR_CANTP_TXCONF_MAP_TAB[(id)] == PDUR_INVALID_ID)
			|| (PduR_iCanTpCopyTxDataFunc(id) == NULL_PTR)) {
		PDUR_REPORT_ERROR(PDUR_SID_CANTPTPTXBUF, PDUR_E_PDU_ID_INVALID);
		return BUFREQ_E_NOT_OK;
	}

	/*Call the upperlayer CopyTxData Function */
	return PduR_iCanTpCopyTxDataFunc(id)(PduR_iCanTpCopyTxDataID(id), info,
			retry, availableDataPtr);
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

