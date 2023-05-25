

#ifndef CANIF_H
#define CANIF_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/* Type definitions of CAN Interface */
#include "CanIf_Types.h"

/* CanIf configuration header file */
#include "CanIf_Cfg.h"

#if(CANIF_CONFIGURATION_VARIANT == CANIF_CFG_VAR_PBS)
/* CanIf configuration header file */
#include "CanIf_PBcfg.h"
#endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/




/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/



/*This hold the current config set details*/
#define CANIF_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern const CanIf_ConfigType * CanIf_Prv_ConfigSet_tpst;
#define CANIF_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"


/* Declaration of CanIf_Init() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern void CanIf_Init(const CanIf_ConfigType * ConfigPtr);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/* Declaration of CanIf_SetControllerMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId,
        CanIf_ControllerModeType ControllerMode);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_GetControllerMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId,
        CanIf_ControllerModeType * ControllerModePtr);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_Transmit() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_Transmit(PduIdType CanIfTxSduId,
        const PduInfoType * CanIfTxInfoPtr);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_CancelTransmit() API */
#if (STD_ON == CANIF_PUBLIC_CANCEL_TRANSMIT_SUPPORT)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_CancelTransmit(PduIdType CanIfTxSduId);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* Declaration of CanIf_SetPduMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_SetPduMode(uint8 ControllerId,
        CanIf_PduModeType PduModeRequest);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* FC_VariationPoint_START */
/* Declaration of CanIf_DeInit() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern void CanIf_DeInit(const CanIf_ConfigType * ConfigPtr);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_Get_DirectHw_Info() API */
#if (CANIF_DIRECT_HW_WRITE == STD_ON)



#define CANIF_START_SEC_CODE_FAST
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_Get_DirectHw_Info (PduIdType CanTxPduId,
        rba_CanDirectHWInfo_o * hw_info_po);

#define CANIF_STOP_SEC_CODE_FAST
#include "CanIf_MemMap.h"

/* Declaration of CanIf_DirectHw_Write() API */

#define CANIF_START_SEC_CODE_FAST
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_DirectHw_Write(const uint32 * p_tx_data_buff_u32
        , uint32 data_len_u32, const rba_CanDirectHWInfo_o * hw_info_po);

#define CANIF_STOP_SEC_CODE_FAST
#include "CanIf_MemMap.h"
#endif
/* FC_VariationPoint_END */


/* Declaration of CanIf_GetPduMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_GetPduMode(
                                                         uint8 ControllerId,
                                                         CanIf_PduModeType * PduModePtr
                                                        );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if ((CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON) && (CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))
/* Declaration of CanIf_ReadRxPduData() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId,
                                                        PduInfoType * CanIfRxInfoPtr);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


#if ((CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF) && (CANIF_CFG_TX_FEATURE_ENABLED== STD_ON))
/* Declaration of CanIf_ReadTxNotifStatus() API */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern CanIf_NotifStatusType CanIf_ReadTxNotifStatus(
                                                                      PduIdType CanIfTxSduId
                                                                      );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if ((CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API != STD_OFF) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))
/* Declaration of CanIf_ReadRxNotifStatus() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


#if (CANIF_PUBLIC_VERSIONINFO_API != STD_OFF)
/* Declaration of CanIf_GetVersionInfo() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern void CanIf_GetVersionInfo (Std_VersionInfoType * VersionInfo);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if (CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF)
/* Declaration of CanIf_SetDynamicTxId() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern void CanIf_SetDynamicTxId(
                                                  PduIdType CanTxPduId,
                                                  Can_IdType CanId
                                                   );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


/* FC_VariationPoint_START */
/* Declaration of CanIf_ResetDynamicTxId() API */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern void CanIf_ResetDynamicTxId(PduIdType CanTxPduId);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/* FC_VariationPoint_END */



#if ((CANIF_WAKEUP_SUPPORT != STD_OFF) &&(CANIF_CFG_TRCV_WAKEUP_SUPPORT == STD_ON))
/* Declaration of CanIf_CheckWakeup() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_CheckWakeup (EcuM_WakeupSourceType WakeupSource);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if ((CANIF_CFG_TRCV_WAKEUP_SUPPORT == STD_ON) && (CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON))
/* Declaration of CanIf_CheckValidation() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_CheckValidation (EcuM_WakeupSourceType WakeupSource);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* Declaration of CanIf_SetTrcvMode() API */
#if CANIF_CFG_TRCV_DRV_SUPPORT ==STD_ON
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_SetTrcvMode(
                                            uint8 TransceiverId,
                                            CanTrcv_TrcvModeType TransceiverMode
                                                          );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if CANIF_CFG_TRCV_DRV_SUPPORT ==STD_ON
/* Declaration of CanIf_GetTrcvMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_GetTrcvMode(
                                            CanTrcv_TrcvModeType * TransceiverModePtr,
                                            uint8 TransceiverId
                                                           );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if CANIF_CFG_TRCV_DRV_SUPPORT!=STD_OFF
/* Declaration of CanIf_GetTrcvWakeupReason() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_GetTrcvWakeupReason(
                                            uint8 TransceiverId,
                                            CanTrcv_TrcvWakeupReasonType * TrcvWuReasonPtr
                                                                 );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/* Declaration of CanIf_SetTrcvWakeupMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_SetTrcvWakeupMode(
                                                uint8 TransceiverId,
                                                CanTrcv_TrcvWakeupModeType TrcvWakeupMode
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* FC_VariationPoint_START */

#if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
/* Declaration of CanIf_ReturnRxPduId() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern Std_ReturnType CanIf_ReturnRxPduId(
                                                     PduIdType CanIfRxTargetPduId,
                                                    PduIdType * CanIfRxPduIdPtr
                                                    );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
/* Declaration of CanIf_ReturnTxPduId() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_ReturnTxPduId(
                                                     PduIdType CanIfTxTargetPduId,
                                                    PduIdType * CanIfTxPduIdPtr
                                                    );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
/* FC_VariationPoint_END */



#if (CANIF_SET_BAUDRATE_API == STD_ON)
/* Declaration of CanIf_SetBaudrate() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_SetBaudrate(
                                                uint8 ControllerId,
                                                const uint16 BaudRateConfigID
                                                );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF)

/* Declaration of CanIf_GetTxConfirmationState() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern CanIf_NotifStatusType CanIf_GetTxConfirmationState(
                                       uint8 ControllerId
                                               );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


#if((CANIF_PUBLIC_PN_SUPPORT == STD_ON) && (CANIF_CFG_TRCV_DRV_SUPPORT==STD_ON))

/* Declaration of CanIf_ClearTrcvWufFlag() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_ClearTrcvWufFlag(
                                              uint8 TransceiverId
                                                               );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_CheckTrcvWakeFlag() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_CheckTrcvWakeFlag(
                                              uint8 TransceiverId
                                                               );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif

/* FC_VariationPoint_START */
#if ((CANIF_RB_READMSGID_API == STD_ON) && (CANIF_CFG_TX_FEATURE_ENABLED== STD_ON))
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_ReadTxMsgId() API */
extern uint32 CanIf_ReadTxMsgId(
                                                  uint16 msgHandleTx_u16
                                                 );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if ((CANIF_RB_READMSGID_API == STD_ON) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* Declaration of CanIf_ReadRxMsgId() API */
extern uint32 CanIf_ReadRxMsgId(uint16 msgHandleRx_u16);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif

#if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* Declaration of CanIf_Rb_ReadTxPduCanId() API */
extern Std_ReturnType CanIf_Rb_ReadTxPduCanId(
                                                    PduIdType CanIfTxPduId,
                                                    uint32 * TxPduCanId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_Rb_ReadRxPduCanId() API */
extern Std_ReturnType CanIf_Rb_ReadRxPduCanId( PduIdType CanIfRxPduId,
        uint32 * RxPduCanId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif



#if ((CANIF_RB_CHANGERXPDUID_API != STD_OFF) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))
/* Declaration of CanIf_Rb_ChangeRxPduCanId() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_Rb_ChangeRxPduCanId(PduIdType CanRxPduId, Can_IdType CanId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* FC_VariationPoint_END */


/*
 **********************************************************************************************************************
 * Global Variables                                                                                                   *
 **********************************************************************************************************************
*/

#endif   /* CANIF_H */
