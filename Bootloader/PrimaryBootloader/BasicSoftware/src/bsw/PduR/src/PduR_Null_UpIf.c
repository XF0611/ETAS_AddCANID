
#include "PduR_Prv.h"


#if defined(PDUR_J1939RM_SUPPORT) && (PDUR_J1939RM_SUPPORT != 0)
#include "PduR_J1939Rm.h"
#include "PduR_J1939Rm_Up.h"
/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_null_J1939RmTxConfirmation
 *        Null functions - to discard any unnecessary notification
 *
 * \param           PduIdType gwayId  :ID of multiplexed I-PDU that has been transmitted.
 *
 * \retval          None
 *
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if (AUTOSAR_VERSION_AR42 == STD_ON)
void PduR_null_J1939RmTxConfirmation( PduIdType id )
#else
/*The parameter 'Std_ReturnType result' is used only when Autosar version is higher than AR42*/
void PduR_null_J1939RmTxConfirmation( PduIdType id , Std_ReturnType result)
#endif
{
    /* Nothing */
    (void)id;  /* To remove warning */
    #if (AUTOSAR_VERSION_AR42 == STD_OFF)
    (void)result;
    #endif

}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif /* PDUR_J1939RM_SUPPORT */

