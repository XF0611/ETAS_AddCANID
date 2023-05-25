

#include "PduR_Prv.h"
/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_invId_IfRxIndication
 *                Functions to be invoked when an invalid ID is encountered
 *
 * \param           PduIdType Id:     ID of L-PDU that has been received.
 *                  const uint8 *ptr: Pointer to L-SDU (buffer of received payload)
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

void PduR_invId_IfRxIndication(PduIdType id, const PduInfoType * ptr)
{
    /* nothing - DET not enabled */
    /* to remove warning */
    (void)id;
    (void)ptr;
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
 * PduR_FrIfTriggerTransmit
 *            Functions to be invoked when an invalid ID is encountered
 *
 *
 * \param          PduIdType id:ID of L-PDU that is requested to be transmitted.
 *                 uint8 *  ptr:Pointer to place inside the transmit buffer of the L-PDU where data shall be copied to.
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
/* MR12 RULE 8.13 VIOLATION: As per AUTOSAR SWS specification signature is as made as P2VAR. So this warning can be ignored.*/
Std_ReturnType PduR_invId_IfTriggerTransmit( PduIdType id, PduInfoType * data )
{
    /* nothing - DET not enabled */
    /* to remove warning */
     /* data = data; Introduced for MISRA sake. Will be removed by Optimiser anyway */
    (void)id;
    (void)data;
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
 * PduR_invId_IfTxConfirmation
 *           Functions to be invoked when an invalid ID is encountered.
 *
 *
 * \param           PduIdType id -  ID of FlexRay L-PDU that has been transmitted.
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if (AUTOSAR_VERSION_AR42 == STD_ON)
void PduR_invId_IfTxConfirmation( PduIdType id)
#else
/*The parameter 'Std_ReturnType result' is used only when Autosar version is higher than AR42*/
void PduR_invId_IfTxConfirmation( PduIdType id, Std_ReturnType result)
#endif
{
    /* nothing - DET not enabled */
    /* to remove warning */
    (void)id;
    #if (AUTOSAR_VERSION_AR42 == STD_OFF)
    (void)result;
    #endif

}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

