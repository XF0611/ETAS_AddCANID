
/* Multicast support functions */

#ifndef PDUR_MC_H
#define PDUR_MC_H
/**
\defgroup PDUR_MC_H    PDUR - Protocol Data Unit Router
*/
#include "PduR_Cfg_Internal.h"
#include "PduR_Types.h"
#if defined(PDUR_MULTICAST_TO_IF_SUPPORT) && PDUR_MULTICAST_TO_IF_SUPPORT == 1
#include "PduR_Multicast_TxConfirmation.h"
#endif
/* ------------------------------------------------------------------------ */
/**
 * @ingroup PDUR_MC_H
 *
 * Begin section for code
 */


#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/*begin */

/**
 * @ingroup PDUR_MC_H
 *
 *  This function is Support function to handle multicast from upper layers to lower layers \n
 *
 *  @param  In:      upToLoTable - This parameter represents Multicast routing table \n
 *  @param  In:      mcId - This parameter represents ID of I-PDU to be transmitted \n
 *  @param  Out:     info - This parameter represents a pointer to PduInfoStructure containing SDU data pointer and
 *                          SDU length of a transmit buffer. \n
 *
 *
 *  @return          E_OK: if the request is accepted\n
 *                   E_NOT_OK: if the request is not accepted  just for testing\n
 *
 */
extern Std_ReturnType PduR_MF_UpToLo( const PduR_MT_UpToLo * upToLoTable,
                                                         PduIdType mcId,
                                                         const PduInfoType * info);

/**
 * @ingroup PDUR_MC_H
 *
 *      Support function to handle Cancel Transmission request for multicast routes \n
 *
 * @param  In:      PduR_MT_UpToLo upToLoTable: Multicast routing table \n
 * @param  In:      PduIdType mcId            : ID of I-PDU to be transmitted \n
 *
 * @return          E_OK: if the request is accepted \n
 *                  E_NOT_OK: if the request is not accepted  just for testing \n
 *
 **************************************************************************************************
 */
extern Std_ReturnType PduR_MF_CancelTransmit_UpToLo(const PduR_MT_UpToLo * upToLoTable,
                                                 PduIdType mcId);

/**
 * @ingroup PDUR_MC_H
 *
 * End section for code
 */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif /* PDUR_MC_H */

