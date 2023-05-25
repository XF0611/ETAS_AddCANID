

#ifndef CANNM_CBK_H
#define CANNM_CBK_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

/* APIs called by Can Interface module */

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_RxIndication(
                                                    PduIdType RxPduId,
                                                    const PduInfoType * PduInfoPtr
                                                );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_TxConfirmation( PduIdType TxPduId );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#endif /* CANNM_CBK_H */


