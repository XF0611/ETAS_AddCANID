
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
#include "PduR_UpIf.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/**
 **************************************************************************************************
 * PduR_dComTransmit - Function to be invoked if DET is enable for PduR_ComTransmit
 *      This function is called by the Com to request a transmission.
 *
 * \param           PduIdType id: ID of Com I-PDU to be transmitted.
 *                  const PduInfoType * ptr: Pointer to a structure with I-PDU related data that shall be transmitted:
 *                                         data length and pointer to I-SDU buffer
 *
 * \retval          E_OK Transmit request has been accepted
 *                  E_NOT_OK Transmit request has not been accepted
 *
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
Std_ReturnType PduR_dComTransmit(PduIdType id, const PduInfoType *ptr) {

	/* Returns the state */
	PDUR_CHECK_STATE_RET(PDUR_SID_COMTX, E_NOT_OK)

	/* Checks for invalid pointer */
	PDUR_CHECK_PTR_RET(PDUR_SID_COMTX, ptr, E_NOT_OK)

	/* If the PDU identifier is not within the specified range then PDUR_E_PDU_ID_INVALID is reported via DET.*/
	if ((id >= PDUR_NR_VALID_COM_IDS)
			|| (PDUR_COM_TX_MAP_TAB[(id)] == PDUR_INVALID_ID)
			|| (PduR_iComTransmitFunc(id) == NULL_PTR)) {
		PDUR_REPORT_ERROR(PDUR_SID_COMTX, PDUR_E_PDU_ID_INVALID);
		return E_NOT_OK;
	}

	/*Calls the Lower layer TransmitFunction */

	return PduR_iComTransmitFunc(id)(PduR_iComTransmitID(id), ptr);

}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

