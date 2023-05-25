

/****************************************************************************************************/
/*                                  Include Section                                                 */
/****************************************************************************************************/

#include "CanIf_Prv.h"

/***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

#if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)

/* RxBuffer Array */
#define CANIF_START_SEC_VAR_CLEARED_8
#include "CanIf_MemMap.h"
uint8 CanIf_Prv_RxBuffer_tau8[CANIF_CFG_RX_BUFFER_SIZE];
#define CANIF_STOP_SEC_VAR_CLEARED_8
#include "CanIf_MemMap.h"
#endif


#if ((CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API != STD_OFF) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))
/* Store the Notification Status of each CanRxPduId  */
#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
CanIf_NotifStatusType CanIf_Prv_RxNotification_taen[CANIF_CFG_NUM_CANRXPDUIDS];
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif


/*Trcv LUT*/
#if (CANIF_CFG_TRCV_DRV_SUPPORT != STD_OFF)
#define CANIF_START_SEC_CONST_8
#include "CanIf_MemMap.h"
const uint8 CanIf_Trcv_LUT[CANIF_NUM_TRANSCEIVERS] = {0};
#define CANIF_STOP_SEC_CONST_8
#include "CanIf_MemMap.h"
#endif

/************************************************************************/

/*Tx global variable definition*/

#if((CANIF_PUBLIC_TXBUFFERING == STD_ON) && (CANIF_CFG_TX_FEATURE_ENABLED== STD_ON))
CanIf_Prv_TxBufferStatus_tst CanIf_Prv_TxBufferRam_ast[CANIF_TOTAL_TXBUFFERS];
CanIf_Prv_TxPduStatus_tst CanIf_Prv_TxPduRam_ast[CANIF_TOTAL_TXPDUS];
#endif

#if (CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF)
uint32 CanIf_DynTxPduCanId_au32[CANIF_TOTAL_DYNAMIC_PDUS];
#endif


#if ((CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)&& (CANIF_CFG_TX_FEATURE_ENABLED== STD_ON))
CanIf_NotifStatusType CanIf_Prv_TxNotification_aen[CANIF_TOTAL_TXPDUS];
#endif

/*Rx global variable definition*/

#if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
uint32 CanIf_DynRxPduCanId_au32[CANIF_RX_DYNAMIC_PDUS];
#endif
