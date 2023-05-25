
#include "PduR_Prv.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"


/**
 **************************************************************************************************
 * PduR_invId_UpTransmit
 *      Functions to be invoked when an invalid ID is encountered.
 *
 * \param           PduIdType id         :ID of I-PDU to be transmitted.
 *                  const PduInfoType * ptr:Pointer to pointer to PduInfoStructure containing SDU data pointer and
 *                                        SDU length of a transmit buffer.
 *
 * \retval          E_NOT_OK
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
Std_ReturnType PduR_invId_UpTransmit( PduIdType id, const PduInfoType * ptr )
{
    /* to remove warning */
    (void)id;
    (void)ptr;
    /* nothing - DET not enabled */
    return E_NOT_OK;
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
 * PduR_invId_UpCancelTransmit
 *      Functions to be invoked when an invalid ID is encountered.
 *
 * \param           PduIdType id: ID of DCM I-PDU to be cancelled the transmission.
 *
 *
 * \retval          E_NOT_OK
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
/* MR12 RULE 8.13 VIOLATION: AUTOSAR specifies that pointer should be a P2VAR as it can be changed during run time*/
Std_ReturnType PduR_invId_UpCancelTransmit(PduIdType id)
{
    /* to remove warning */
    (void)id;
    /* nothing - DET not enabled */
    return E_NOT_OK;
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

