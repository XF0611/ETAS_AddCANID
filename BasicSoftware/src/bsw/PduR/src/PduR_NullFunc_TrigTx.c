

#include "PduR_Prv.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_NF_TriggerTransmit_Func
 *      Null function to handle unwanted PduR_xxxIfTriggerTransmit
 *
 * \param           PduIdType gwayId  :ID of N-PDU that shall be received
 *                  uint8*    data    : Data pointer for transmit buffer
 *
 * \retval          E_NOT_OK
 *
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
/* MR12 RULE 8.13 VIOLATION: AUTOSAR specifies that pointer should be a P2VAR as it can be changed during run time*/
Std_ReturnType PduR_NF_TriggerTransmit_Func( PduIdType id,
                                                      PduInfoType * data)
{
    /* data = data; Introduced for MISRA sake. Will be removed by Optimiser */
    (void)id;
    (void)data;

    return E_NOT_OK;
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"


