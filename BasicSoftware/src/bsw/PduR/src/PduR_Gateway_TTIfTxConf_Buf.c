
#include "PduR_Prv.h"
#if defined(PDUR_IFGATEWAY_SUPPORT) && (PDUR_IFGATEWAY_SUPPORT != STD_OFF)
#include "PduR_Gw.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_GF_TTIf_Buf_TxConfirmation  - Handle TxConfirmation for gatewaying between If layers
 *                  where a single entry buffer is in use
 *
 * \param           PduIdType gwayId:Not used
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if (AUTOSAR_VERSION_AR42 == STD_ON)
void PduR_GF_TTIf_Buf_TxConfirmation_Func( PduIdType gwayId )
#else
/*The parameter 'Std_ReturnType result' is used only when Autosar version is higher than AR42*/
void PduR_GF_TTIf_Buf_TxConfirmation_Func( PduIdType gwayId,Std_ReturnType result)
#endif
{
    (void)gwayId; /*Gateway Id */
    #if (AUTOSAR_VERSION_AR42 == STD_OFF)
    (void)result;
    #endif
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif /* PDUR_IFGATEWAY_SUPPORT */

