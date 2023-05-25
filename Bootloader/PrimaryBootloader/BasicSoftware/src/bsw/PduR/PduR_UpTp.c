
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
#include "PduR_UpTp.h"

#include "PduR_Dcm.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_DcmTransmit
 *      This function is called by the DCM to request a transmission.
 *
 * \param           PduIdType id: ID of DCM I-PDU to be transmitted.
 *                  const PduInfoType * ptr: Pointer to a structure with I-PDU related data that shall be transmitted:
 *                                         data length and pointer to I-SDU buffer
 *
 * \retval          E_OK Transmit request has been accepted
 *                  E_NOT_OK Transmit request has not been accepted
 *
 * \seealso         PDUR202, PDUR206
 * \usedresources
 **************************************************************************************************
 */

Std_ReturnType PduR_DcmTransmit(PduIdType id, const PduInfoType *ptr) {
	return (PduR_rDcmTransmit((id), (ptr)));
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
 * PduR_DcmCancelTransmit
 *      This function is called by the DCM to request a transmission cancellation
 *
 * \param           PduIdType id: ID of DCM I-PDU to be cancelled the transmission.
 *
 * \retval          E_OK Cancellation request has been accepted
 *                  E_NOT_OK Cancellation request has not been accepted
 *
 **************************************************************************************************
 */

Std_ReturnType PduR_DcmCancelTransmit(PduIdType id) {
	Std_ReturnType r;

#if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)
    /* DET Checks if DET is enabled */
    PDUR_CHECK_STATE_RET(PDUR_SID_DCMCNCLTX, E_NOT_OK)

    if ( ( id >= PDUR_NR_VALID_DCM_IDS ) || (PDUR_DCM_TX_MAP_TAB[(id)] == PDUR_INVALID_ID) || ( PduR_iDcmCancelTransmitFunc(id) == NULL_PTR ))
    {
        PDUR_REPORT_ERROR(PDUR_SID_DCMCNCLTX, PDUR_E_PDU_ID_INVALID);
        return  E_NOT_OK;
    }
#endif /* PDUR_DEV_ERROR_DETECT */

	r = PduR_iDcmCancelTransmitFunc(id)(PduR_iDcmCancelTransmitID(id));

	return r;
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

