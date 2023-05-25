

#ifndef CANIF_PRV_H
#define CANIF_PRV_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
/* CanIf module header file */
#include "CanIf.h"

#include "CanIf_Cbk.h"





#if (CANIF_J1939NM_ENABLED != STD_OFF)
#include "J1939Nm.h"
#include "J1939Nm_Cbk.h"
#if (!defined(J1939NM_AR_RELEASE_MAJOR_VERSION) || (J1939NM_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(J1939NM_AR_RELEASE_MINOR_VERSION) || (J1939NM_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#if (CANIF_PDUR_ENABLED != STD_OFF)
#include "PduR.h"
#include "PduR_CanIf.h"
#if (!defined(PDUR_AR_RELEASE_MAJOR_VERSION) || (PDUR_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(PDUR_AR_RELEASE_MINOR_VERSION) || (PDUR_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#if (CANIF_CANNM_ENABLED != STD_OFF)
#include "CanNm.h"
#include "CanNm_Cbk.h"
#if (!defined(CANNM_AR_RELEASE_MAJOR_VERSION) || (CANNM_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANNM_AR_RELEASE_MINOR_VERSION) || (CANNM_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#if (CANIF_CANTP_ENABLED != STD_OFF)
#include "CanTp.h"
#include "CanTp_Cbk.h"
#if (!defined(CANTP_AR_RELEASE_MAJOR_VERSION) || (CANTP_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANTP_AR_RELEASE_MINOR_VERSION) || (CANTP_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#if (CANIF_CANTSYN_ENABLED != STD_OFF)
#include "CanTSyn.h"

#if (!defined(CANTSYN_AR_RELEASE_MAJOR_VERSION) || (CANTSYN_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANTSYN_AR_RELEASE_MINOR_VERSION) || (CANTSYN_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#if (CANIF_J1939TP_ENABLED != STD_OFF)
#include "J1939Tp.h"
#include "J1939Tp_Cbk.h"
#if (!defined(J1939TP_AR_RELEASE_MAJOR_VERSION) || (J1939TP_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(J1939TP_AR_RELEASE_MINOR_VERSION) || (J1939TP_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#if (CANIF_XCP_ENABLED != STD_OFF)
#include "Xcp.h"
#include "XcpOnCan_Cbk.h"
#if (!defined(XCP_AR_RELEASE_MAJOR_VERSION) || (XCP_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(XCP_AR_RELEASE_MINOR_VERSION) || (XCP_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#if CANIF_CFG_TRCV_DRV_SUPPORT == STD_ON
#include "CanTrcv.h"
#if (!defined(CANTRCV_AR_RELEASE_MAJOR_VERSION) || (CANTRCV_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANTRCV_AR_RELEASE_MINOR_VERSION) || (CANTRCV_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#if (CANIF_ECUM_ENABLED == STD_ON)
/*EcuM.h required to use EcuM_WakeupSourceType*/
#include "EcuM.h"
#if (!defined(ECUM_AR_RELEASE_MAJOR_VERSION) || (ECUM_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(ECUM_AR_RELEASE_MINOR_VERSION) || (ECUM_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

/* Errors during development will be reported to DET module */
#if (CANIF_PUBLIC_DEV_ERROR_DETECT != STD_OFF)
#include "Det.h"
#if (!defined(DET_AR_RELEASE_MAJOR_VERSION) || (DET_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(DET_AR_RELEASE_MINOR_VERSION) || (DET_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

/* BSW Scheduler for CanIf */
#include "CanIf_Cfg_SchM.h"
#if (!defined(SCHM_AR_RELEASE_MAJOR_VERSION) || (SCHM_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(SCHM_AR_RELEASE_MINOR_VERSION) || (SCHM_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif


/*Version check for ComStack_Types and CanIf*/
#if (!defined(COMTYPE_AR_RELEASE_MAJOR_VERSION) || (COMTYPE_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COMTYPE_AR_RELEASE_MINOR_VERSION) || (COMTYPE_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif


#if (CANIF_CANSM_ENABLED != STD_OFF)

#include "CanSM_Cbk.h"
#if (!defined(CANSM_AR_RELEASE_MAJOR_VERSION) || (CANSM_AR_RELEASE_MAJOR_VERSION != CANIF_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANSM_AR_RELEASE_MINOR_VERSION) || (CANSM_AR_RELEASE_MINOR_VERSION != CANIF_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#endif
/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* FC_VariationPoint_START */

#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

#define CANIF_XCORE_INVALID_PIPE_ID (0xFFu)
#define CANIF_XCORE_RXINDICATION_INVALID_USER_TYPE  (0xFFu)
#define CANIF_XCORE_RXINDICATION_INVALID_CB_IDX     (0xFFu)
#define CANIF_XCORE_RXINDICATION_INVALID_CANID      (0xFFFFFFFFu)

/* The properties of the receiver on the destination core: (BIT0, BIT1) = {0x0 = "POLLING"; 0x1 = "Task"; 0x2 = "SWI"} */
#define CANIF_XCORE_PIPE_FLAG_PROC_POLLING  (0x0u)
#define CANIF_XCORE_PIPE_FLAG_PROC_TASK (0x1u)
#define CANIF_XCORE_PIPE_FLAG_PROC_SWI      (0x2u)
#define CANIF_XCORE_PIPE_GET_FLAG_PROC(x) (x & 0x3u)

/* Properties of the PDUs transmitted over this pipe: (BIT2, BIT3) = {0x0 = "TX"; 0x1 = "RX"; 0x2 = "TXConfirmation"} */
#define CANIF_XCORE_PIPE_FLAG_TYPE_TX  (0x0u << 2u)
#define CANIF_XCORE_PIPE_FLAG_TYPE_RX  (0x1u << 2u)
#define CANIF_XCORE_PIPE_FLAG_TYPE_TXCONF  (0x2u << 2u)
#define CANIF_XCORE_PIPE_GET_FLAG_TYPE(x) (x & (0x3u << 2u))

/* Properties of the PDUs transmitted over this pipe for Buffer allocation: (BIT4) = {1 = VARIABLE SIZE BUFFER;}*/
#define CANIF_XCORE_PIPE_FLAG_VARBUFALLOC (0x1 << 4u)
#define CANIF_XCORE_PIPE_GET_FLAG_ALLOC(x) (x & (0x1u << 4u))


/** PIPE operation defines */
/* PKT Structure:
        BIT0..BIT15 = pduId;
        BIT16..BIT23 = callbackIndex_u8
        BIT24..BIT27 = userType_e
        BIT31="VALID";
*/
#define CANIF_XCORE_PKT_FLAG_INVALID_ENDBUF_PDU  (0x80000000u)
#define CANIF_XCORE_PKT_FLAG_INVALID_PDU  (0x40000000u)
#define CANIF_XCORE_PKT_FLAG_VALID_PDU  (0x0u)
#define CANIF_XCORE_PKT_PDUID_MASK  (0xFFFFu)
#define CANIF_XCORE_PKT_CALLBACKIDX_MASK  (0xFF0000u)
#define CANIF_XCORE_PKT_CALLBACKIDX_SHIFT  (16u)
#define CANIF_XCORE_PKT_USERTYPE_MASK  (0xF000000u)
#define CANIF_XCORE_PKT_USERTYPE_SHIFT  (24u)
#define CANIF_XCORE_PKT_HDR_CALC(pduId, callbackIdx, userType)  (CANIF_XCORE_PKT_FLAG_VALID_PDU | \
                                                                ((pduId) & CANIF_XCORE_PKT_PDUID_MASK) | \
                                                                (((callbackIdx) << CANIF_XCORE_PKT_CALLBACKIDX_SHIFT) & CANIF_XCORE_PKT_CALLBACKIDX_MASK) | \
                                                                (((uint32)(userType) << CANIF_XCORE_PKT_USERTYPE_SHIFT) & CANIF_XCORE_PKT_USERTYPE_MASK) )
#define CANIF_XCORE_PTK_HDR_SIZE_U32W (sizeof(CanIf_XCore_PipePktHdrType_st) / sizeof(uint32))
#endif

/* FC_VariationPoint_END */

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/






#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)

typedef enum
{
    CANIF_TXBUFFER_EMPTY,
    CANIF_TXBUFFER_READY,
    CANIF_TXBUFFER_FULL
}CanIf_Prv_BuffStatus_ten;



/** Ram structure for Buffer  */
typedef struct
{
    uint8 last_index;
    CanIf_Prv_BuffStatus_ten bufferstatus;
}CanIf_Prv_TxBufferStatus_tst;


/** Ram structure for Tx PDU */
typedef struct
{
    boolean pdu_buffered_flag;

}CanIf_Prv_TxPduStatus_tst;

#endif


typedef struct
{

    uint8 Ctrl_Pdu_mode;

#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF)
    CanIf_NotifStatusType       CanIf_TxCnfmStatus;
#endif
    /* this element indicates if partial networking is enabled or disabled per controller [SWS_CANIF_00747]*/
#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    boolean BlPnTxFilter;

#if (CANIF_RB_NODE_CALIBRATION == STD_ON)
    boolean BlPnCtrl_Calib;
#endif
#endif
#if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
    boolean ValidationSts_b;
#endif
}CanIf_ControllerStateType;

/* FC_VariationPoint_START */
 #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

#if (CANIF_XCORE_DEBUG_ACTIVE != STD_OFF)
/* Holds the Debug information of the Pipe. */
typedef struct
{
    uint32 NumSendCalls_u32;        /* Total calls to Pipe Send */
    uint32 NumSendCallsNotOk_u32;   /* Total calls to Pipe send returning OK */
    uint32 NumBytesSendOk_u32;
    uint32 NumReceiveCalls_u32;     /* Total calls to Pipe receive */
    uint32 NumReceiveCallsOk_u32;   /* Total calls to Pipe receive returning OK  */
    uint32 NumBytesReceiveOk_u32;
    PduIdType SendLastAttemptedPduId;
    PduInfoType SendLastAttemptedPduInfo;
    PduIdType ReceiveLastPduId;
    PduInfoType ReceiveLastPduInfo;

} CanIf_XCore_PipeDbgInfoType_st;

#endif
/* Structure holds the runtime information needed by a pipe.
   variables of this type must be placed in non cacheable shared RAM.
*/
typedef struct {
    const CanIf_XCore_PipeConfigType_st *PipeConfig_pcst;   /* ptr to Const Pipe config */
    void *FifoBaseAddr_pv;                                  /* address of the RAM start of the FIFO PIPE */
    void *FifoEndAddr_pv;                                   /* address of the RAM END of the FIFO PIPE */
    volatile uint32 PktCnt_u32;                             /* counter used to determine the amount of iterms WRITTEN by the FIFO data producer */
    volatile uint32 PktCntAck_u32;                          /* counter used to determine the amount of iterms READ by the FIFO data consumer */
    volatile void *WritePtr_pv;                             /* The ptr to write element in FIFO. */
    volatile void *ReadPtr_pv;                              /* The ptr after the last read element in FIFO. Next read operation will read from here */
    #if (CANIF_XCORE_DEBUG_ACTIVE == STD_ON)
    CanIf_XCore_PipeDbgInfoType_st  Dbg_st;                 /* various debug info */
    #endif
} CanIf_XCore_PipeRuntimeInfoType_st;

/**
    The structure of a data element in the FIFO. A "PACKET"
    It has a header. The actual data payload will follow
    PKT Structure:
        BIT0..BIT15 = pduId;
        BIT16..BIT23 = callbackIndex_u8
        BIT24..BIT27 = userType_e
        BIT31="VALID";
*/
typedef struct {
    uint8  CtrlID_u8;
    uint32 PktFlags_u32;
    uint32 PktPayloadLenBytes_u32;
} CanIf_XCore_PipePktHdrType_st;


/**
    This structure role is to wrap all the parameters needed by
    CanIf_XCore_LocalCore_Transmit, CanIf_XCore_LocalCore_TxConfirmation, CanIf_XCore_LocalCore_RxIndication and CanIf_XCore_PipeRead.
    This will allow simpler function declaration and simplification of parameter list.

    The CanIf_XCore_PipeWrite and CanIf_XCore_PipeRead will ensure the serialization and deserialization of these parameters into and from
    the PIPE packet headers.
*/
typedef struct {
    const PduInfoType* PduInfoPtr;
    CanIf_Cfg_UserType_ten userType_e;
    PduIdType PduId;
    uint8 callbackIndex_u8;
    uint8 ControllerId_u8;
} CanIf_XCore_PipeTxRxParams_st;



#endif
/* FC_VariationPoint_END */


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


#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)


#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CanIf_Prv_TxBufferStatus_tst CanIf_Prv_TxBufferRam_ast[];
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"



#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CanIf_Prv_TxPduStatus_tst CanIf_Prv_TxPduRam_ast[];
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"

#endif


#if (CANIF_PUBLIC_SETDYNAMICTXID_API == STD_ON)

#define CANIF_START_SEC_VAR_CLEARED_32
#include "CanIf_MemMap.h"
extern uint32 CanIf_DynTxPduCanId_au32[];
#define CANIF_STOP_SEC_VAR_CLEARED_32
#include "CanIf_MemMap.h"

#endif



#if(CANIF_CFG_TRCV_WAKEUP_SUPPORT== STD_ON)
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
extern const CanIf_Wakeup CanIf_WakeupList_tacst[CANIF_CFG_NO_TRCV_WAKEUP_SUPPORT];
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif

/**
 *
 * Store the PDU Mode of each controller
 */

#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CanIf_ControllerStateType CanIf_Prv_ControllerState_ast[];
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

#define CANIF_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CanIf_XCore_PipeRuntimeInfoType_st CanIf_XCore_PipeRuntimeInfo_ast[CANIF_XCORE_MAX_NUM_PIPES];
#define CANIF_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern boolean CanIf_XCore_EndBuf_InvalidFlag_b;
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern const CanIf_XCore_ConfigType * CanIf_XCore_Config_pcst;
#define CANIF_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern boolean CanIf_XCore_RxClrPipeFlag_b;
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif
/* FC_VariationPoint_END */


/* Flag to check if CANIF is Initialized(TRUE) or Not Initialized (FALSE)*/
#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern boolean CanIf_Prv_InitStatus_b;
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"

#if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)

#define CANIF_START_SEC_VAR_CLEARED_32
#include "CanIf_MemMap.h"
extern uint32 CanIf_DynRxPduCanId_au32[];
#define CANIF_STOP_SEC_VAR_CLEARED_32
#include "CanIf_MemMap.h"

#endif

/**
 * @ingroup CANIF_PRIV_H
 *
 * Callback function Array
 */


/*T:Naming*/
/* BSW-12812 */
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
extern const CanIf_CallbackFuncType CanIf_Callback;
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/* END BSW-12812 */
#include "CanIf_MemMap.h"

/**
 * Store the Notification Status of each CanTxPduId
 */
#if ((CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)&&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))
#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CanIf_NotifStatusType CanIf_Prv_TxNotification_aen[];
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif


#if ((CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API != STD_OFF) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))

/* Store the Notification Status of each Rxpdu  */
#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CanIf_NotifStatusType CanIf_Prv_RxNotification_taen[CANIF_CFG_NUM_CANRXPDUIDS];
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif


#if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)

/* RxBuffer Array */
#define CANIF_START_SEC_VAR_CLEARED_8
#include "CanIf_MemMap.h"
extern uint8 CanIf_Prv_RxBuffer_tau8[CANIF_CFG_RX_BUFFER_SIZE];
#define CANIF_STOP_SEC_VAR_CLEARED_8
#include "CanIf_MemMap.h"
#endif


/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/* API Service IDs */
#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)

/* Service Id of CanIf_Init API */
#define CANIF_INIT_SID                           0x01

/* Service Id of CanIf_DeInit API */
#define CANIF_DEINIT_SID                         0x02

/* Service Id of CanIf_SetControllerMode API */
#define CANIF_SET_CONTROLLER_SID                 0x03

/* Service Id of CanIf_GetControllerMode API */
#define CANIF_GET_CONTROLLER_SID                 0x04

/* Service Id of CanIf_Transmit API */
#define CANIF_TRANSMIT_SID                       0x05

/* Service Id of CanIf_ReadRxPduData API */
#define CANIF_READ_RX_PDU_DATA_SID               0x06

#if (CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)
/* Service Id of CanIf_ReadTxNotifStatus API */
#define CANIF_READ_TX_NOTIF_STATUS_SID           0x07
#endif

/* Service Id of CanIf_ReadRxNotifStatus API */
#define CANIF_READ_RX_NOTIF_STATUS_SID           0x08

/* Service Id of CanIf_SetPduMode API */
#define CANIF_SET_PDU_MODE_SID                   0x09

/* Service Id of CanIf_GetPduMode API */
#define CANIF_GET_PDU_MODE_SID                   0x0A

#if (CANIF_PUBLIC_VERSIONINFO_API != STD_OFF)
/* Service Id of CanIf_GetVersionInfo API */
#define CANIF_GET_VERSION_INFO_SID               0x0B
#endif

#if (CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF)
/* Service Id of CanIf_SetDynamicTxId API */
#define CANIF_SET_DYNAMIC_TXID_SID               0x0C
#endif

/* Service Id of CanIf_SetTrcvMode API */
#define CANIF_SET_TRCV_MODE_SID                  0x0D

/* Service Id of CanIf_GetTrcvMode API */
#define CANIF_GET_TRCV_MODE_SID                  0x0E

/* Service Id of CanIf_GetTrcvWakeupReason API */
#define CANIF_GET_TRCV_WUMODE_REASON_SID         0x0F

/* Service Id of CanIf_SetTrcvWakeupMode API */
#define CANIF_SET_TRCV_WAKEUPMODE_SID            0x10

/* Service Id of CanIf_CheckWakeup API */
#define CANIF_CHECKWAKEUP_SID                    0x11

/* Service Id of CanIf_CheckValidation API */
#define CANIF_CHECKVALIDATION_SID                0x12

/* Service Id of CanIf_TxConfirmation API */
#define CANIF_TX_CONFIRMATION_SID                0x13

/* Service Id of CanIf_RxIndication API */
#define CANIF_RX_INDICATION_SID                  0x14

/* Service Id of CanIf_ReadRxPduData API */
#define CANIF_CONTROLLER_BUSOFF_SID              0x16

/* Service Id of CanIf_ReadRxPduData API */
#define CANIF_CONTROLLER_MODE_IND_SID            0x17

/* Service Id of CanIf_TrcvModeIndication API */
#define CANIF_TRCVMODEINDICATION_SID             0x18


#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF)
/* Service Id of CanIf_GetTxConfirmationState API */
#define CANIF_GET_TX_CONFIRMATION_STATE_SID      0x19
#endif

#define  CANIF_TRIGGER_TRANSMIT_SID              0x41

/* FC_VariationPoint_START */
/* Service Id of CanIf_ResetDynamicApi API */
#define CANIF_RESET_DYNAMIC_TXID_SID             0x23


/* Service Id of CanIf_CheckBaudrate API */
#define CANIF_CHECKBAUDRATE_SID                  0x35

/* Service Id of CanIf_ChangeBaudrate API */
#define CANIF_CHANGEBAUDRATE_SID                 0x1B
/* FC_VariationPoint_END */

/* Service Id of CanIf_SetBaudrate API */
#define CANIF_SETBAUDRATE_SID                    0x27

/* Service Id of CanIf_ClearTrcvWufFlag API */
#define CANIF_CLEARTRCVWUFFLAG_SID               0x1E

/* Service Id of CanIf_CheckTrcvWakeFlag API */
#define CANIF_CHECKTRCVWAKEFLAG_SID              0x1F

/* Service Id of CanIf_ClearTrcvWufFlagIndication API */
#define CANIF_CLEARTRCVWAKEFLAGIND_SID           0x20

/* Service Id of CanIf_CheckTrcvWakeFlagIndication API */
#define CANIF_CHECKTRCVWAKEFLAGIND_SID           0x21

/* Service Id of CanIf_ConfirmPnAvailability API */
#define CANIF_CONFIRMPNAVAILABILITY_SID          0x1a

/* Service Id of CanIf_CancelTransmit API */
#define CANIF_CANCELTRANSMIT_SID                 0x25


/* FC_VariationPoint_START */

/* Service Id of CanIf_ControllerErrorPassive API */
#define CANIF_CONTROLLER_ERRORPASSIVE_SID        0x29

/* Service Id of CanIf_ReturnTxPduId API */
#define CANIF_RETURN_TX_PDU_ID_SID               0x1C

/* Service Id of CanIf_ReturnRxPduId */
#define CANIF_RETURN_RX_PDU_ID_SID               0x2D

/* Service Id of CanIf_ReadRxMsgId */
#define CANIF_READ_RX_MSG_ID_SID                 0x2F

#define CANIF_RB_READ_TXPDU_CANID               0x30

/* Service Id of CanIf_Rb_ReadRxPduCanId */
#define CANIF_RB_READRXPDUCANID_SID              0x31

/* Service Id of CanIf_ReadRxMsgId */
#define CANIF_READ_TX_MSG_ID_SID                 0x32
/* FC_VariationPoint_END */


#if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
/* Service Id of CanIf_Rb_ChangeRxPduCanId API */
#define CANIF_RB_CHANGERXPDUID_SID               0x34
#endif

/* FC_VariationPoint_START */

#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
#define CANIF_XCORE_APIID_INIT_SID                 0x81

#define CANIF_XCORE_APIID_PIPEWRITE_SID            0x82

#define CANIF_XCORE_APIID_PIPEREAD_SID             0x83
#endif
/* FC_VariationPoint_END */

/***********DET ERROR CODE***************/

/* Invalid CanID is reported using following error code */
#define CANIF_E_PARAM_CANID                     10u

/* Invalid Hoh ID is reported using following error code */
#define CANIF_E_PARAM_HOH                       12u


/**
 * @ingroup CANIF_PRIV_H
 * CANIF: DET Error ID'd*
 * CANIF_E_PARAM_HRH: API service called with invalid parameter
 * */
#define CANIF_E_PARAM_HRH                       81u

/**
 * @ingroup CANIF_PRIV_H
 * CANIF: DET Error ID'd*
 * CANIF_E_PARAM_LPDU: API service called with invalid parameter
 * */
#define CANIF_E_PARAM_LPDU                      13u

/**
 * @ingroup CANIF_PRIV_H
 * CANIF: DET Error ID'd*
 * CANIF_E_PARAM_CONTROLLER: API service called with invalid parameter
 * */
#define CANIF_E_PARAM_CONTROLLER                14u

/**
 * @ingroup CANIF_PRIV_H
 * CANIF: DET Error ID'd*
 * CANIF_E_PARAM_CONTROLLERID: API service called with invalid parameter
 * */
#define CANIF_E_PARAM_CONTROLLERID              15u

/**
 * @ingroup CANIF_PRIV_H
 * CANIF: DET Error ID'd*
 * CANIF_E_PARAM_WAKEUPSOURCE: API service called with invalid parameter
 * */
#define CANIF_E_PARAM_WAKEUPSOURCE              16u

/* API service called with invalid parameter */
#define CANIF_E_PARAM_TRCV                      17u

/* API service called with invalid parameter */
#define CANIF_E_PARAM_TRCVMODE                  18u

/**
 * @ingroup CANIF_PRIV_H
 * CANIF: DET Error ID'd*
 * CANIF_E_PARAM_TRCVWAKEUPMODE: API service called with invalid parameter
 * */
#define CANIF_E_PARAM_TRCVWAKEUPMODE            19u

/**
 * @ingroup CANIF_PRIV_H
 * CANIF: DET Error ID'd*
 * CANIF_E_PARAM_POINTER: API service called with invalid pointer
 * */
#define CANIF_E_PARAM_POINTER                   20u

#define CANIF_E_PARAM_CTRLMODE                  21u

#define CANIF_E_PARAM_PDU_MODE                  22u

/* DET error reported when module is unintialized */
#define CANIF_E_UNINIT                          30u


/* DET error reported when PduId provided as input is invalid */
#define CANIF_E_INVALID_RXPDUID                 60u

/**
 * @ingroup CANIF_PRIV_H
 * CANIF: DET Error ID'd*
 * CANIF_E_INVALID_TXPDUID: Transmit PDU ID invalid
 * */
#define CANIF_E_INVALID_TXPDUID                 50u


/**
 * @ingroup CANIF_PRIV_H
 * CANIF: DET Error ID'd*
 *Used in CanIf_Transmit(), Invalid Lenght passed by Upper Layer
 * */
#define CANIF_E_DATA_LENGTH_MISMATCH            62u

/* Invalid Dlc is reported using following error code */
#define CANIF_E_INVALID_DLC                     61u


/*CAN Interface controller mode state machine is in mode CANIF_CS_STOPPED*/
#define CANIF_E_STOPPED                         70u

/*CAN Interface controller mode state machine is not in mode CANIF_CS_SLEEP*/
#define CANIF_E_NOT_SLEEP                         71u

/*Message length was exceeding the maximum length*/
#define CANIF_E_TXPDU_LENGTH_EXCEEDED           90u

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
#define CANIF_XCORE_E_PARAM                 81u

#define CANIF_XCORE_E_WRONGCORE            82u

#endif
/* FC_VariationPoint_END */
#endif

/**********************************************************************************************************************
**                                     Private Macros of CanIf                                                       **
**********************************************************************************************************************/

/* Macro to indicate invalid channel mode for Receive Indication */
#define CANIF_PRV_INVALID_CHANNEL_MODE                0u

/* Macro to indicate valid channel mode for Receive Indication */
#define CANIF_PRV_VALID_CHANNEL_MODE                1u

/* Macro to indicate wakeup mode for Receive Indication */
#define CANIF_PRV_WAKEUP_MODE                       2u

/* Macro to obtain Lower nibble of CanId */
#define CANIF_PRV_CANID_TYPE_MASK                0x0fu

/* Macro for value four */
#define CANIF_PRV_FOUR                           4u

/* Macro for value Six */
#define CANIF_PRV_SIX                             6u

/* Macro for value seven */
#define CANIF_PRV_SEVEN                           7u

/* Macro for value Eight */
#define CANIF_PRV_EIGHT                           8u

/*Invalid Id*/
#define CANIF_INVALID_ID      0xffffu

/* Maximum value a STANDARD CAN ID can have */
#define CANIF_CAN_STD_VAL                       0x7FFu

/* Maximum value a EXTENDED CAN ID can have */
#define CANIF_CAN_XTD_VAL                       0x1FFFFFFFu

/* Values used to come out of the function */
#define CANIF_SET                               TRUE

/*The macro indicates that No UL is configured for the requested rxpdu*/
#define CANIF_PRV_NO_UL                 0u

#if CANIF_FD_SUPPORT == STD_ON
#define CANIF_PRV_MAX_BUFFER_SIZE       64u
#else
#define CANIF_PRV_MAX_BUFFER_SIZE       8u
#endif


/**
 * @ingroup CANIF_PRIV_H
 *
 *  Values used to come out of the function
 */
#define CANIF_RESET                             FALSE


/* * Configuration for reading Rx Notification status only
 */
#define CANIF_READ_NOTIFSTATUS                  0x10


/**
 * @ingroup CANIF_PRIV_H
 *
 * Configuration for reading received data only
 */
#define CANIF_READ_DATA                         0x01


/**
 * @ingroup CANIF_PRIV_H
 *
 * Configuration for reading both notification status and data
 */
#define CANIF_READ_NOTIFSTATUS_DATA             0x11


/**
 * @ingroup CANIF_PRIV_H
 *
 *  Configuration for reading neither notification status nor data
 */
#define CANIF_READ_NONE                         0x00


/**
 * @ingroup CANIF_PRIV_H
 *
 * If PduIdtype is uint8, the value is 255u.
 *  If the PduIdType changes to uint16, the value is changed to 65535u
 */
#define CANIF_STATIC                           (PduIdType)(((uint32)1u << (uint8)((uint8)8u * (uint8)sizeof(PduIdType)))-1u)


/* FC_VariationPoint_START */
/**
 * @ingroup CANIF_PRIV_H
 *
 * Resetting two most significant bit for Extended or standard type
 */

#define CANIF_MSB_BIT_RESET                 0x3FFFFFFFu


/*macro to obtain CanIdType information from CanId*/
#define CANIF_CANID_BIT_SHIFT                           30u

/* macro to obtain Controller Mode information from Ctrl_Pdu_mode*/
#define CANIF_Ctrl_BIT_SHIFT                            4u

/*Bit masking for Controller Mode*/
#define CANIF_BIT_MASK_CTRL_MODE                              0X0Fu

/*Bit masking for Pdu Mode*/
#define CANIF_BIT_MASK_PDU_MODE                               0XF0u


/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for value zero
 */
#define CANIF_ZERO                 0u

/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for value one
 */
#define CANIF_ONE                 1u

/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for value two
 */
#define CANIF_TWO                 2u

/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for value three
 */
#define CANIF_THREE                 3u

/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for value four
 */
#define CANIF_FOUR                  4u



#if (CANIF_METADATA_SUPPORT != STD_OFF)
/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for SHIFT 8 times
 */
#define CANIF_SHIFT_8                     8u

/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for Standard CanId size
 */
#define CANID_SIZE_STD              2u

/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for Extended CanId size
 */
#define CANID_SIZE_XTD              4u

/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for MetaData Mask
 */
#define CANIF_CANID_MASK         0x0000FFFFU

/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for SDU Data buffer size
 */
#define CANIF_SDU_DATA_BUFF_SIZE       68u

#endif /* (CANIF_CFG_METADATA_SUPPORT != STD_OFF) */
/* FC_VariationPoint_END */

/**
 * @ingroup CANIF_PRIV_H
 *
 * Macro for Default CanId mask value
 */
#define CANID_MASK_DEFAULT       0x1FFFFFFFU

/*Fetching 2 MSB's in 31st and 30th position */
#define CANIF_PRV_CANIF_2MSB_MASK                0xC0000000u

/*Resetting 2 MSB's in 31st and 30th position */
#define CANIF_PRV_MSB_BIT_RESET                  0x3FFFFFFFu

 /* Resetting two most significant bit for Extended or standard type*/
#define CANIF_MSB_BIT_RESET_STD                 0x3FFFu


#define CANIF_GET_ONE_BYTE                 0x00ffu

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */









/* Check whether given condition is satisfied, call Det_ReportError() */
#if CANIF_PUBLIC_DEV_ERROR_DETECT != STD_OFF

     /*Macro For functions which just report DET and continue */
    #define CANIF_DET_REPORT_ERROR(CONDITION, API, ERROR)                                                       \
                                   if(CONDITION)                                                                \
                                   {                                                                            \
                                        (void)Det_ReportError((CANIF_MODULE_ID), (CANIF_INSTANCE_ID), (API), (ERROR));\
                                   }
/*Macro For Det with no condition check and return E_NOT_OK */
#define CANIF_DET_REPORT_ERROR_NO_CONDITON(API, ERROR)                                                       \
                                   {                                                                        \
                                    (void)Det_ReportError((CANIF_MODULE_ID), (CANIF_INSTANCE_ID), (API), (ERROR));     \
                                        return E_NOT_OK;                                                             \
                                   }
    /* Macro For Det with return type as  'Std_ReturnType' */
    #define CANIF_DET_REPORT_ERROR_NOT_OK(CONDITION, API, ERROR)                                                   \
                                   if(CONDITION)                                                                \
                                   {                                                                            \
                                       (void)Det_ReportError((CANIF_MODULE_ID), (CANIF_INSTANCE_ID), (API), (ERROR));\
                                        return E_NOT_OK;                                                        \
                                   }

     /**
       * @ingroup CANIF_PRIV_H
       *
       * Macro For functions with return type as 'CanIf_NotifStatusType'
       */

#if ((CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF) || (CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API != STD_OFF)          \
                                                                       ||(CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF))
        #define CANIF_DET_REPORT_ERROR_NO_NOTIF(CONDITION, API, ERROR)                                          \
                                   if(CONDITION)                                                                \
                                   {                                                                            \
                                       (void)Det_ReportError((CANIF_MODULE_ID), (CANIF_INSTANCE_ID), (API), (ERROR));\
                                        return CANIF_NO_NOTIFICATION;                                           \
                                   }
    #endif

       /*Macro for DET which returns from the function on invocation with void return type*/
       #define CANIF_DET_REPORT_ERROR_VOID_RET(CONDITION, API, ERROR)                                                  \
                                   if(CONDITION)                                                                \
                                   {                                                                            \
                                       (void)Det_ReportError((CANIF_MODULE_ID), (CANIF_INSTANCE_ID), (API), (ERROR));\
                                       return;                                                                       \
                                   }

    /**
     * @ingroup CANIF_PRIV_H
     *
     * Macro For functions with return type as 'Std_ReturnType'. Unlocks the interrupts before return.
     */
    #define CANIF_DET_REPORT_ERROR_SCHM_EXIT_NOK(CONDITION, API, ERROR, SCHM_EXIT_FUNC)                               \
                                    if(CONDITION)                                                                     \
                                    {                                                                                 \
                                        (void)Det_ReportError((CANIF_MODULE_ID), (CANIF_INSTANCE_ID), (API), (ERROR));\
                                        /* MR12 RULE 20.7, VIOLATION: This Warning is "Not Critical". The parameter SCHM_EXIT_FUNC is used to function call. */ \
                                        SCHM_EXIT_FUNC();                                                             \
                                        return E_NOT_OK;                                                              \
                                    }

    /* FC_VariationPoint_END */
#define CANIF_DET_REPORT_ERROR_NOR(API, ERROR)  (void)Det_ReportError((CANIF_MODULE_ID), (CANIF_INSTANCE_ID), (API), (ERROR));


#else
/*Dummy macro declaration for Det to avoid compilation error */
#define CANIF_DET_REPORT_ERROR_NOT_OK(CONDITION, API, ERROR)

 #define CANIF_DET_REPORT_ERROR_VOID_RET(CONDITION, API, ERROR)

#define CANIF_DET_REPORT_ERROR(CONDITION, API, ERROR)

#define CANIF_DET_REPORT_ERROR_NOK(CONDITION, API, ERROR)
#define CANIF_DET_REPORT_ERROR_NO_CONDITON(API, ERROR)



#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF || CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API != STD_OFF          \
                                                            || CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)
        #define CANIF_DET_REPORT_ERROR_NO_NOTIF(CONDITION, API, ERROR)
    #endif

    #define CANIF_DET_REPORT_ERROR_SCHM_EXIT_NOK(CONDITION, API, ERROR, SCHM_EXIT_FUNC)
#endif

/*Trcv LUT*/
#if (CANIF_CFG_TRCV_DRV_SUPPORT != STD_OFF)
/**
 * @ingroup CANIF_PRIV_H
 *
 * Transceiver Look up table
 */
#define CANIF_START_SEC_CONST_8
#include "CanIf_MemMap.h"
extern const uint8 CanIf_Trcv_LUT[CANIF_NUM_TRANSCEIVERS];
#define CANIF_STOP_SEC_CONST_8
#include "CanIf_MemMap.h"
#endif

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_Prv_ReadRxBuffer(PduInfoType * PduInfoPtr,
        const CanIf_Cfg_RxPduType_tst * RxPduCfgPtr);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if((CANIF_PUBLIC_TXBUFFERING == STD_ON)&&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern void CanIf_Prv_BufferInit(void);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_Prv_WriteTxBuffer
                                        (PduIdType CanIfTxSduId,
                                         Can_PduType Pdu);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_Prv_ReadTxBuffer(
                                    const CanIf_Cfg_TxBufferConfig_tst * CanIf_TxBufferConfig,
                                    Can_PduType * Pdu);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern void CanIf_Prv_ClearTxChannelBuffer(uint8 ControllerId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif

#if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern void CanIf_Prv_WriteRxBuffer(const uint8 * CanSduPtr,
                                                const CanIf_Cfg_RxPduType_tst * RxPduCfg_pcst,
                                                const PduLengthType CanDlc, const Can_IdType CanId
                                                );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* FC_VariationPoint_START */

#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern void CanIf_XCore_Init(const CanIf_XCore_ConfigType * Config_pcst);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_XCore_PipeWrite(uint16 PipeId, CanIf_XCore_PipeTxRxParams_st* params_pst);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern void CanIf_XCore_PipeRead(uint16 PipeId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern void CanIf_XCore_ClearPipes(uint8 ControllerId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif

/* FC_VariationPoint_END */
#if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_XCore_LocalCore_Transmit(
                                                           PduIdType CanIfTxSduId,
                                                           const PduInfoType * CanIfTxInfoPtr
                                                          );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_XCore_LocalCore_TxConfirmation(const CanIf_Cfg_TxPduConfig_tst * CanIf_TxPduConfig_pst);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern Std_ReturnType CanIf_XCore_LocalCore_RxIndication(const CanIf_Cfg_RxPduType_tst * CanIf_RXPduConfig_pst,
                                                                    const PduInfoType * CanIf_ULPduinfo_pst);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif  /* CANIF_PRV_H */
