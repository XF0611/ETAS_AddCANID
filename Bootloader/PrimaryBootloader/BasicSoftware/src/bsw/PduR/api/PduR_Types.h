
/*PDU Router type definitions */

/*PDU Router type definitions */

/**
\defgroup PDUR_TYPES_H    PDUR - Protocol Data Unit Router
*/

#ifndef PDUR_TYPES_H
#define PDUR_TYPES_H

#include "PduR.h"
#include "PduR_Cfg_Internal.h"
#include "ComStack_Types.h"

/* ------------------------------------------------------------------------ */


/**
 * @ingroup PDUR_TYPES_H
 * this enum is used for PduR state type \n
 *
 *  typedef enum  { \n
 *   PDUR_UNINIT,               PduR uninitialized state\n
 *   PDUR_REDUCED,              PduR reduced state \n
 *   PDUR_ONLINE                PduR Online state \n
 *  }PduR_StateType;
 */

typedef enum {
   PDUR_UNINIT,
   PDUR_REDUCED,
   PDUR_ONLINE
} PduR_StateType;                                    /**/
/* PDUR_RTA-BSW_end 174 */

/* Routing Path Group */
/**
 * @ingroup PDUR_TYPES_H
 *
 * Routing path group for IF path type
 */
#define PDUR_FIFO_IF    0u

/**
 * @ingroup PDUR_TYPES_H
 *
 * Routing path group for Unicast FIFO TP Path Type
 */
#define PDUR_UCFIFO_TP  1u

/**
 * @ingroup PDUR_TYPES_H
 *
 * Routing path group for Unicast SingleEntryBuffer Path Type
 */
#define PDUR_UCSEB_TP   2u

/**
 * @ingroup PDUR_TYPES_H
 *
 * Multicast SingleEntryBuffer Path Type
 */
#define PDUR_MCSEB_TP   3u

/**
 * @ingroup PDUR_TYPES_H
 *
 * Invalid PDU id handlers
 */

#define PduR_IH_CancelTransmit_Func    PduR_invId_UpCancelTransmit

/**
 * @ingroup PDUR_TYPES_H
 *
 * Invalid PDU id handlers for Cancel Receive API
 */

#define PduR_IH_CancelReceive_Func    PduR_invId_UpCancelReceive

/**
 * @ingroup PDUR_TYPES_H
 *
 * Invalid PDU id handlers for TriggerTransmit API
 */

#define PduR_IH_TriggerTransmit_Func    PduR_invId_IfTriggerTransmit


/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate PduR_LayerType
 */

typedef uint8 PduR_LayerType;       /* to hold one of PDUR_LAYER_xxx */

/* Corresponding defines PDUR_LAYER_xxx can be found in pdur.h file */

/* ------------------------------------------------------------------------ */

/**
 * @ingroup PDUR_TYPES_H
 * This structure is used for Routing Path Group feature related definitions \n
 * typedef struct \n
 *  { \n
 *      uint8  bufferLock;          this variable is used for buffer lock \n
 *      uint8  lowerLayers;         this variable is used for loweer layers \n
 * } PduR_LockType ;
 */

typedef struct
{
   uint8    bufferLock;
   uint8    lowerLayers;
}PduR_LockType;
#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR Buffer lock type.\n
 */
typedef PduR_LockType   PduR_BuffLockType;
#else
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR Buffer lock type.\n
 */
typedef uint8           PduR_BuffLockType;
#endif /* #if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0) */

/* ------------------------------------------------------------------------ */
/* Routing table data structures */

/***
 Below functions can assignde to function pointer PduR_loTransmitFunc:
PduR_MF_Com_Transmit_Func
PduR_MF_Dcm_Transmit_Func
PduR_MF_IpdumUp_Transmit_Func
PduR_MF_"+uplModName+"_Transmit_Func

PduR_RF_CanIf_Transmit_Func
PduR_RF_FrNm_Transmit_Func
PduR_RF_FrIf_Transmit_Func
PduR_RF_LinIf_Transmit_Func
PduR_RF_IpdumLo_Transmit_Func
PduR_RF_CanTp_Transmit_Func
PduR_RF_FrTp_Transmit_Func
PduR_RF_LinTp_Transmit_Func

PDUR_IH_Com_Transmit_Func

PDUR_IH_Dcm_Transmit_Func
PDUR_IH_IpdumUp_Transmit_Func
PDUR_IH_"+uplModName+"_Transmit_Func

PduR_GF_TTIf_FIFO_Transmit_Func
PduR_GF_DDIf_FIFO_Transmit_Func
PduR_GF_TTIf_Buf_Transmit_Func
PduR_GF_DDIf_Buf_Transmit_Func
PduR_GF_Com_Transmit_Func
PduR_GF_Dcm_Transmit_Func
PduR_GF_IpdumUp_Transmit_Func
PduR_GF_"+uplIfNameOfRefRxPdu(dst)+"_Transmit_Func
PduR_GF_"+uplTpNameOfRefRxPdu(dst)+"_Transmit_Func

PduR_GwMcTx_Transmit_Func

NULL_PTR_LOTRANSMIT
***/

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR lower layer transmit function type.\n
 *typedef struct \n
 *{\n
 *  Std_ReturnType (*PduR_loTransmitFunc) ( PduIdType id, const PduInfoType * info );\n
 *}PduR_loTransmitFuncType;\n
 */
typedef struct
{
   Std_ReturnType (*PduR_loTransmitFunc) ( PduIdType id, const PduInfoType * info );
}PduR_loTransmitFuncType;


/***
 Below functions can assignde to function pointer PduR_loCancelTransmitFunc
PDUR_IH_CancelTransmit_Func

PduR_MF_CancelTransmit_Func

PduR_RF_CanTp_CancelTransmit_Func
PduR_RF_FrTp_CancelTransmit_Func
***/

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR lower layer cancel transmit function type.\n
 *typedef struct \n
 *{\n
 *  Std_ReturnType (*PduR_loCancelTransmitFunc) ( PduIdType id );\n
 *}PduR_loCancelTransmitFuncType;\n
 */
typedef struct
{
   Std_ReturnType (*PduR_loCancelTransmitFunc) ( PduIdType id );
}PduR_loCancelTransmitFuncType;


/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR lower layer cancel receive function type.\n
 *typedef struct \n
 *{\n
 *  Std_ReturnType (*PduR_loCancelReceiveFunc) ( PduIdType id );\n
 *}PduR_loCancelReceiveFuncType;\n
 */
typedef struct
{
   Std_ReturnType (*PduR_loCancelReceiveFunc) ( PduIdType id );
}PduR_loCancelReceiveFuncType;


/***
 Below functions can assignde to function pointer PduR_upIfRxIndicationFunc
PduR_RF_Com_RxIndication_Func
PduR_RF_IpdumUp_RxIndication_Func
PduR_RF_"+uplIfNameOfRefRxPdu(routingPath.destPdu())+"_RxIndication_Func

PduR_GF_If_RxIndication_Func

PDUR_IH_CanIfRx_RxIndication_Func
PDUR_IH_FrIfRx_RxIndication_Func
PDUR_IH_IpdumLoRx_RxIndication_Func
PDUR_IH_LinIfRx_RxIndication_Func
***/
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR upper layer interface receive indication function type.\n
 *typedef struct \n
 *{\n
 *  void (*PduR_upIfRxIndicationFunc) ( PduIdType gwayId, const PduInfoType * data );\n
 *}PduR_upIfRxIndicationFuncType;\n
 */
typedef struct
{
   void (*PduR_upIfRxIndicationFunc) ( PduIdType gwayId, const PduInfoType * data );
}PduR_upIfRxIndicationFuncType;


/***
 Below functions can assignde to function pointer PduR_upIfTxConfirmationFunc
PduR_RF_Com_TxConfirmation_Func
PduR_RF_IpdumUp_TxConfirmation_Func
PduR_RF_"+canifdest.uplNameFromDestPdu()+"_TxConfirmation_Func
PduR_RF_"+frifdest.uplNameFromDestPdu()+"_TxConfirmation_Func
PduR_RF_"+getTheUpLIfName(destPdu)+"_TxConfirmation_Func
PduR_RF_"+linifdest.uplNameFromDestPdu()+"_TxConfirmation_Func

PduR_null_ComTxConfirmation_Func
PduR_null_IpdumUpTxConfirmation_Func
PduR_null_"+canifdest.uplNameFromDestPdu()+"TxConfirmation_Func

PduR_GF_DDIf_NoBuf_TxConfirmation_Func
PduR_GF_DDIf_Buf_TxConfirmation_Func
PduR_GF_DDIf_FIFO_TxConfirmation_Func
PduR_GF_TTIf_Buf_TxConfirmation_Func
PduR_GF_TTIf_FIFO_TxConfirmation_Func


PDUR_IH_CanIfTx_TxConfirmation_Func
PDUR_IH_FrIfTx_TxConfirmation_Func
PDUR_IH_LinIfTx_TxConfirmation_Func
PDUR_IH_IpdumLoTx_TxConfirmation_Func
***/

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR upper layer interface transmit confirmation function type.\n
 *typedef struct \n
 *{\n
 *    void (*PduR_upIfTxConfirmationFunc) ( PduIdType id );\n
 *}PduR_upIfTxConfirmationFuncType;\n
 */
typedef struct
{
    #if (AUTOSAR_VERSION_AR42 == STD_ON)
    void (*PduR_upIfTxConfirmationFunc) ( PduIdType id );
    #else
    void (*PduR_upIfTxConfirmationFunc) ( PduIdType id, Std_ReturnType result );
    #endif

}PduR_upIfTxConfirmationFuncType;


/***
 Below functions can assignde to function pointer PduR_upIfTriggerTxFunc
PduR_RF_Com_TriggerTransmit_Func
PduR_RF_IpdumUp_TriggerTransmit_Func
PduR_RF_"+frifdest.uplNameFromDestPdu()+"_TriggerTransmit_Func
PduR_RF_"+linifdest.uplNameFromDestPdu()+"_TriggerTransmit_Func

PduR_NF_TriggerTransmit_Func

PduR_GF_TTIf_Buf_TriggerTransmit_Func
PduR_GF_TTIf_FIFO_TriggerTransmit_Func

PDUR_IH_FrIfTx_TriggerTransmit_Func
PDUR_IH_LinIfTx_TriggerTransmit_Func
***/

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR upper layer interface trigger transmit function type.\n
 *typedef struct \n
 *{\n
 *  Std_ReturnType (*PduR_upIfTriggerTxFunc) ( PduIdType gwayId, PduInfoType * dataPtr );\n
 *}PduR_upIfTriggerTxFuncType;\n
 */
typedef struct
{
   Std_ReturnType (*PduR_upIfTriggerTxFunc) ( PduIdType gwayId, PduInfoType * dataPtr );
}PduR_upIfTriggerTxFuncType;


/***
 Below functions can assignde to function pointer PduR_upTpProvideRxBufFunc
PduR_RF_Dcm_ProvideRxBuffer_Func
PduR_RF_"+uplTpNameOfRefRxPdu(routingPath.destPdu())+"_ProvideRxBuffer_Func

PduR_GF_CanTp_FIFO_ProvideRxBuffer_Func
PduR_GF_CanTp_Buf_ProvideRxBuffer_Func
PduR_GF_FrTp_FIFO_ProvideRxBuffer_Func
PduR_GF_FrTp_Buf_ProvideRxBuffer_Func
PduR_GF_LinTp_FIFO_ProvideRxBuffer_Func
PduR_GF_LinTp_Buf_ProvideRxBuffer_Func

PDUR_IH_CanTpRx_ProvideRxBuffer_Func
PDUR_IH_FrTpRx_ProvideRxBuffer_Func
PDUR_IH_LinTpRx_ProvideRxBuffer_Func
***/

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR upper layer copy receive data function type.\n
 *typedef struct \n
 *{\n
 *  BufReq_ReturnType (*PduR_upTpCopyRxDataFunc) ( PduIdType gwId, const PduInfoType * info, PduLengthType* bufferSizePtr );\n
 *}PduR_upTpCopyRxDataFuncType;\n
 */
typedef struct
{
   BufReq_ReturnType (*PduR_upTpCopyRxDataFunc) ( PduIdType gwId, const PduInfoType * info, PduLengthType* bufferSizePtr );
}PduR_upTpCopyRxDataFuncType;

/***
 Below functions can assignde to function pointer PduR_upTpProvideRxBufFunc
PduR_RF_Dcm_StartOfReception_Func
PduR_RF_"+uplTpNameOfRefRxPdu(routingPath.destPdu())+"_StartOfReception_Func

PDUR_IH_CanTpRx_StartOfReception_Func
PDUR_IH_FrTpRx_StartOfReception_Func
PDUR_IH_LinTpRx_StartOfReception_Func
***/

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR upper layer Start of reception function type.\n
 *typedef struct \n
 *{\n
 *  BufReq_ReturnType (*PduR_upTpStartOfReceptionFunc) ( PduIdType gwId,const PduInfoType * info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr );\n
 *}PduR_upTpStartOfReceptionFuncType;\n
 */
typedef struct
{
   BufReq_ReturnType (*PduR_upTpStartOfReceptionFunc) ( PduIdType gwId,const PduInfoType * info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr );
}PduR_upTpStartOfReceptionFuncType;


/***
 Below functions can assignde to function pointer PduR_upTpRxIndicationFunc
PduR_RF_Dcm_RxIndication_Func
PduR_RF_"+uplTpNameOfRefRxPdu(routingPath.destPdu())+"_RxIndication_Func

PduR_GF_CanTp_FIFO_RxIndication_Func
PduR_GF_CanTp_Buf_RxIndication_Func
PduR_GF_FrTp_FIFO_RxIndication_Func
PduR_GF_FrTp_Buf_RxIndication_Func
PduR_GF_LinTp_FIFO_RxIndication_Func
PduR_GF_LinTp_Buf_RxIndication_Func

PDUR_IH_CanTpRx_RxIndication_Func
PDUR_IH_FrTpRx_RxIndication_Func
PDUR_IH_LinTpRx_RxIndication_Func
***/

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR upper layer receive indication function type.\n
 *typedef struct \n
 *{\n
 *  void (*PduR_upTpRxIndicationFunc) ( PduIdType, Std_ReturnType );\n
 *}PduR_upTpRxIndicationFuncType;\n
 */
typedef struct
{
   void (*PduR_upTpRxIndicationFunc) ( PduIdType gwId, Std_ReturnType result );
}PduR_upTpRxIndicationFuncType;


/***
 Below functions can assignde to function pointer PduR_upTpProvideTxBufFunc
PduR_RF_Dcm_ProvideTxBuffer_Func
PduR_RF_"+uplTpNameOfRefTxPdu(routingPath.destPdu())+"_ProvideTxBuffer_Func

PduR_GF_CanTp_FIFO_ProvideTxBuffer_Func
PduR_GF_CanTp_Buf_ProvideTxBuffer_Func
PduR_GF_FrTp_FIFO_ProvideTxBuffer_Func
PduR_GF_FrTp_Buf_ProvideTxBuffer_Func
PduR_GF_LinTp_FIFO_ProvideTxBuffer_Func
PduR_GF_LinTp_Buf_ProvideTxBuffer_Func

PDUR_IH_CanTpTx_ProvideTxBuffer_Func
PDUR_IH_FrTpTx_ProvideTxBuffer_Func
PDUR_IH_LinTpTx_ProvideTxBuffer_Func

***/

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR upper layer copy transmit data function type.\n
 *typedef struct \n
 *{\n
 *  BufReq_ReturnType (*PduR_upTpCopyTxDataFunc) ( PduIdType gwId, const PduInfoType * info, RetryInfoType* retry, PduLengthType* availableDataPtr);\n
 *}PduR_upTpCopyTxDataFuncType;\n
 */
typedef struct
{
/*   BufReq_ReturnType (*PduR_upTpProvideTxBufFunc) ( PduIdType, PduInfoType * , RetryInfoType*,PduLengthType*);*/
    BufReq_ReturnType (*PduR_upTpCopyTxDataFunc) ( PduIdType gwId, const PduInfoType * info, RetryInfoType* retry, PduLengthType* availableDataPtr);
}PduR_upTpCopyTxDataFuncType;


/***
 Below functions can assignde to function pointer PduR_upTpTxConfirmationFunc
PduR_RF_Dcm_TxConfirmation_Func
PduR_RF_"+uplTpNameOfRefTxPdu(routingPath.destPdu())+"_TxConfirmation_Func

PduR_GF_CanTp_FIFO_TxConfirmation_Func
PduR_GF_CanTp_Buf_TxConfirmation_Func
PduR_GF_FrTp_FIFO_TxConfirmation_Func
PduR_GF_FrTp_Buf_TxConfirmation_Func
PduR_GF_LinTp_FIFO_TxConfirmation_Func
PduR_GF_LinTp_Buf_TxConfirmation_Func

PDUR_IH_CanTpRx_TxConfirmation_Func
PDUR_IH_FrTpRx_TxConfirmation_Func
PDUR_IH_LinTpRx_TxConfirmation_Func

***/

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is used to indicate the PduR upper layer transmit confirmation function type.\n
 *typedef struct \n
 *{\n
 *  void (*PduR_upTpTxConfirmationFunc) ( PduIdType gwId, Std_ReturnType result);\n
 *}PduR_upTpTxConfirmationFuncType;\n
 */
typedef struct
{
   void (*PduR_upTpTxConfirmationFunc) ( PduIdType gwId, Std_ReturnType result);
}PduR_upTpTxConfirmationFuncType;


/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR lower layer transmit function pointer.\n
 */
typedef uint8 PduR_loTransmitFP;
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR lower layer cancel transmit function pointer.\n
 */
typedef uint8 PduR_loCancelTransmitFP;
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR upper interface layer receive indication function pointer.\n
 */
typedef uint8 PduR_upIfRxIndicationFP;
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR upper interface layer transmit confirmation function pointer.\n
 */
typedef uint8 PduR_upIfTxConfirmationFP;
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR upper interface layer trigger transmit function pointer.\n
 */
typedef uint8 PduR_upIfTriggerTxFP;
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR upper transport layer start of reception function pointer.\n
 */
typedef uint8 PduR_upTpStartOfReceptionFP;
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR upper transport layer provide receive buffer function pointer.\n
 */
typedef uint8 PduR_upTpProvideRxBufFP;
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is upper transport layer receive indication function pointer.\n
 */
typedef uint8 PduR_upTpRxIndicationFP;
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR upper transport layer provide transmit buffer function pointer.\n
 */
typedef uint8 PduR_upTpProvideTxBufFP;
/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR upper transport layer transmit confirmation function pointer.\n
 */
typedef uint8 PduR_upTpTxConfirmationFP;


/**
 * @ingroup PDUR_TYPES_H
 * This structure represents ID of the lower layer ot transmit data. \n
 * typedef struct \n
 *  { \n
 *      PduIdType                    loId;                      lower layer ID \n
 *      PduR_loTransmitFP            loTransmitID;              lower layer transmit ID \n
 *      PduR_loCancelTransmitFP      loCancelTransmitID;        lower layer cancel transmit ID\n
 * } PduR_RT_UpToLo ;
 */
typedef struct
{
    PduIdType                    loId;
    PduR_loTransmitFP            loTransmitID;
    PduR_loCancelTransmitFP      loCancelTransmitID;
} PduR_RT_UpToLo;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents ID of the upper layer to give receive indication. \n
 * typedef struct \n
 *  { \n
 *      PduIdType                    upId;                      upper layer ID\n
 *      PduR_upIfRxIndicationFP      upRxIndicationID;          upper layer receive indication ID \n
 * } PduR_RT_LoIfRxToUp ;
 */
typedef struct
{
    PduIdType                    upId;
    PduR_upIfRxIndicationFP      upRxIndicationID;
} PduR_RT_LoIfRxToUp;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents ID of the upper layer to give transmit confirmation. \n
 * typedef struct \n
 *  { \n
 *      PduIdType                    upId;                      upper layer ID\n
 *      PduR_upIfTxConfirmationFP    upTxConfirmationID;        upper layer transmit confirmation ID \n
 * } PduR_RT_LoIfTxToUp ;
 */
typedef struct
{
    PduIdType                    upId;
    PduR_upIfTxConfirmationFP    upTxConfirmationID;
} PduR_RT_LoIfTxToUp;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents ID of the upper layer to give trigger transmit confirmation. \n
 * typedef struct \n
 *  { \n
 *      PduIdType                    upId;                      upper layer ID\n
 *      PduR_upIfTriggerTxFP         upTriggerTxID;             upper layer trigger transmit ID \n
 *      PduR_upIfTxConfirmationFP    upTxConfirmationID;        upper layer transmit confirmation ID \n
 * } PduR_RT_LoTtIfTxToUp ;
 */
typedef struct
{
    PduIdType                    upId;
    PduR_upIfTriggerTxFP         upTriggerTxID;
    PduR_upIfTxConfirmationFP    upTxConfirmationID;
 } PduR_RT_LoTtIfTxToUp;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents ID of the upper layer to give receive indication. \n
 * typedef struct \n
 *  { \n
 *      PduIdType                    upId;                  upper layer ID\n
 *      PduR_upTpStartOfReceptionFP  upStartOfReceptionID;  upper layer start of reception node ID \n
 *      PduR_upTpProvideRxBufFP      upProvideRxBufID;      upper layer receive buffer ID \n
 *      PduR_upTpRxIndicationFP      upRxIndicationID;      upper layer receive indication ID\n
 * } PduR_RT_LoTpRxToUp ;
 */
typedef struct
{
   PduIdType                    upId;
   PduR_upTpStartOfReceptionFP  upStartOfReceptionID;
   PduR_upTpProvideRxBufFP      upProvideRxBufID;
   PduR_upTpRxIndicationFP      upRxIndicationID;
} PduR_RT_LoTpRxToUp;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents ID of the upper layer to give transmit confirmation. \n
 * typedef struct \n
 *  { \n
 *      PduIdType                    upId;                      upper layer ID\n
 *      PduR_upTpProvideTxBufFP      upProvideTxBufID;          upper layer transmit buffer ID \n
 *      PduR_upTpTxConfirmationFP    upTxConfirmationID;        upper layer transmit confirmation ID \n
 * } PduR_RT_LoTtIfTxToUp ;
 */
typedef struct
{
    PduIdType                    upId;
    PduR_upTpProvideTxBufFP      upProvideTxBufID;
    PduR_upTpTxConfirmationFP    upTxConfirmationID;
} PduR_RT_LoTpTxToUp;

/* ------------------------------------------------------------------------ */
/* Multicast routing table data structures */
/**
 * @ingroup PDUR_TYPES_H
 *  This structure represents multicatsing from upper layer to lower layer.\n
 * typedef struct\n
 *  {\n
 *  PduIdType                    upId;      upper layer ID \n
 *  PduR_RT_UpToLo               upToLo;    lower layer name \n
 *  #if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0) \n
 *  PduR_RoutingPathGroupIdType  routingCntrl_Idx;      Index to routing enable/disable status flag of RPG \n
 *  #endif \n
 *  } PduR_MT_UpToLo;
 */
typedef struct
{
   PduR_RT_UpToLo               upToLo;
    #if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)
   /* Index to routing enable/disable status flag of RPG */
   PduR_RoutingPathGroupIdType  routingCntrl_Idx;
   #endif /* #if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0) */
   boolean                      isTpModule;
   PduIdType                    upId;
   PduIdType                    upLayerSymName;
   uint8                        upSrcLayerName;
} PduR_MT_UpToLo;

/* ------------------------------------------------------------------------ */
/* Gateway routing table data structures */
/**
 * @ingroup PDUR_TYPES_H
 * This structure represents gateway transmission. \n
 * typedef struct \n
 *  { \n
 *      PduLengthType                length;            Data length \n
 *      PduIdType                    loId;              lower layer ID\n
 *      PduR_loTransmitFP            transmitID;        Transmit ID \n
 * } PduR_GT_IfTx ;
 */
typedef struct
{
   PduLengthType                length;
   PduIdType                    loId;
   PduR_loTransmitFP            transmitID;
} PduR_GT_IfTx;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents gateway interface. \n
 * typedef struct \n
 *  { \n
 *     void * buffer;    Points to either a PduInfoType (for a single entry buffer) or
                                                        a PduR_Buf_If_dynamic (for IF Single Buffer) or
                                                        a PduR_FIFO_fixed  \n
 *      PduIdType                    loId;              lower layer ID \n
 *      PduR_loTransmitFP            transmitID;        Transmit ID \n
 *      boolean                        isFifoType;        Fifo or Non-Fifo type \n
 * } PduR_GT_If ;
 */
typedef struct
{
   void * buffer;
   PduIdType                    loId;
   PduR_loTransmitFP            transmitID;
   boolean                      isFifoType;
} PduR_GT_If;


/**
 * @ingroup PDUR_TYPES_H
 * This structure represents fifo buffer \n
 * typedef struct \n
 *  { \n
 *   uint8 * bufferPtr;    Start address of the buffer, will not be modified \n
 *   uint8 * readPtr;      Start address of read pointer  \n
 *  uint8 * writePtr;     Start address of write pointer  \n
 *  #if defined(PDUR_MULTICAST_FROM_TP_SUPPORT) && (PDUR_MULTICAST_FROM_TP_SUPPORT != 0)
 *  PduInfoType * infoPtr;  Store info of Pdu at the time of StartOfreception\n
 *  #endif  \n
 *  PduLengthType            numRxLength_auo[PDUR_MAX_QUEUEDEPTH];                Number of bytes recieved                \n  Number of bytes received from source TP
 *  PduLengthType            numTxLength;                Number of bytes transmitted             \n  NUmber of bytes transmitted to Des TP
 *  PduLengthType            SduLength_auo[PDUR_MAX_QUEUEDEPTH];                  contains the number of bytes valid in the read tp buffer \n
 *  PduLengthType            bufferLength;               Store configured bufferLength
 *  #ifdef PDUR_STORE_AND_FORWARD
 *  PduLengthType                        remainingBuffLength;   Store remaining buffer Length in case of PDUR_STORE_AND_FORWARD feature enabled
 *  PduLengthType                        RxChunkLength;         Store the length of the received chunks
 *  PduLengthType                       TxChunkLength;         Store the length of the transmitted chunks
 *  #endif  \n
 *  uint8                                Rx_Status;     Represents the status of the reception \n
 *  uint8                                Tx_Status;     Represents the status of the transmission \n
 *  uint8                                Tx_E_OK_Count;   Count of Successful Transmission of Pdu \n
 *  #ifdef PDUR_FIFO_TP_SUPPORT
 * uint8                                Rx_FifoInstanceUsed_u8; Represents the fifo instances used. Required at reception path \n
 * uint8                                Tx_FifoInstanceUsed_u8; Represents the fifo instances used. Required at transmission path \n
#endif
 * } PduR_TpSession_Dynamic ;
 */
typedef struct
{
    uint8 * bufferPtr;
    uint8 * readPtr;
    uint8 * writePtr;
 #if defined(PDUR_MULTICAST_FROM_TP_SUPPORT) && (PDUR_MULTICAST_FROM_TP_SUPPORT != 0)
    const PduInfoType * infoPtr;
 #endif
    PduLengthType                        numRxLength_auo[PDUR_MAX_QUEUEDEPTH];
    PduLengthType                        numTxLength;
    PduLengthType                        SduLength_auo[PDUR_MAX_QUEUEDEPTH];
    PduLengthType                        bufferLength;
 #ifdef PDUR_STORE_AND_FORWARD
    PduLengthType                        remainingBuffLength;
    PduLengthType                        RxChunkLength;
    PduLengthType                        TxChunkLength;
 #endif
    uint8                                Rx_Status;
    uint8                                Tx_Status;
    uint8                                Tx_E_OK_Count;
    uint8                                Rx_FifoInstanceUsed_u8;
    uint8                                Tx_FifoInstanceUsed_u8;

 } PduR_TpSession_Dynamic;

/**
 * @ingroup PDUR_TYPES_H
 *
 * pointer to structure PduR_TpSession_Dynamic
 */
typedef PduR_TpSession_Dynamic * PduR_TpSessDynPtr;


/*@ingroup PDUR_TYPES_H
 * This structure holds list of all sessions \n
 * typedef struct
 *{
 *PduR_TpSession_Dynamic* GwTpSessionPtr;
 *}PduR_GwTp_SessionListType;*/

typedef struct
{
    PduR_TpSession_Dynamic * GwTpSessionPtr;
}PduR_GwTp_SessionListType;

/**
 * @ingroup PDUR_TYPES_H
 *
 * pointer to PduR_TpSession_Dynamic structure.
 */
typedef const PduR_GwTp_SessionListType * PduR_GwTp_SessionListPtr;

/*@ingroup PDUR_TYPES_H
 * typedef struct
*{
*    uint8 * PduR_UpIfTxConf_ConfigPtr;
*}PduR_UpIfTxConf_Config;*/

typedef struct
{
    uint8 * PduR_UpIfTxConf_ConfigPtr;
}PduR_UpIfTxConf_Config;
/**
 * @ingroup PDUR_TYPES_H
 * This is RAM structure representing TP Queueing .\n
 * typedef struct \n
 *  { \n
 *  PduR_TpSession_Dynamic * primarySession;              Points to either a PduR_FIFO_If_dynamic or a PduR_TpSession_Dynamic \n
 *  PduR_TpSession_Dynamic * secondarySession;          Pdu length \n
 *  uint8                             activeSessionCount;       activeSessionCount \n
 * } PduR_TpConnection_Dynamic ;
 */
typedef struct
{
   PduR_TpSession_Dynamic * primarySession;
   /* FC_VariationPoint_START */
#ifdef PDUR_QUEUEING_SUPPORT
   PduR_TpSession_Dynamic * secondarySession;
   uint8                                                 activeSessionCount;
#endif
   /* FC_VariationPoint_END */
} PduR_TpConnection_Dynamic;

typedef PduR_TpConnection_Dynamic * PduR_TpConnDynPtr;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents fixed fifo data frames.\n
 * typedef struct \n
 *  { \n
 *  uint8 * begin;            starting address \n
 *  PduR_TpConnection_Dynamic * connectionTable \n             Points to PduR_TpConnection_Dynamic \n
 *  PduLengthType                     eltSize;          Pdu length \n
 *  uint16                         buffPoolId;       Buffer Pool id \n
 * } PduR_FIFO_Tp_fixed ;
 */
typedef struct
{
   uint8 * begin;
   PduR_TpConnection_Dynamic * connectionTable;
   PduLengthType                     eltSize;
   uint16                            buffPoolId;
} PduR_FIFO_Tp_fixed;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents gateway transport. \n
 * typedef struct \n
 *  { \n
 *      const PduR_FIFO_Tp_fixed * buffer;    Points to either a PduR_SingleEntryBuffer or a PduR_FIFO_fixed \n
 *      PduLengthType                numOfLowerLayers;         No.Of lower layers configured(To Check if its 1:n gateway multicast)
 *       FC_VariationPoint_START
 *      PduLengthType                TpGwQueueSize;        Size of the Queue for a TP gateway if it is referring to a buffer pool\n
 *      FC_VariationPoint_END
 *      PduIdType                    loId;              lower layer ID \n
 *      uint16                       TpThreshold;        PduR TpThreshold \n
 *      PduR_loTransmitFP            loTransmitID;        Transmit ID \n
 *      #ifdef PDUR_FIFO_TP_SUPPORT
 *      uint8                       TpQueueDepth_u8;    Represents the QueueDepth of FIFO Tp.
 *      #endif
 * } PduR_GT_Tp ;
 */
typedef struct
{
   const PduR_FIFO_Tp_fixed*   buffer;
   PduLengthType               numOfLowerLayers;
   /* FC_VariationPoint_START */
   PduLengthType               TpGwQueueSize;
   /* FC_VariationPoint_END */
   PduIdType                   loId;
   PduIdType                   upId;
   uint16                      TpThreshold;
   PduR_loTransmitFP           loTransmitID;
#ifdef PDUR_FIFO_TP_SUPPORT
   uint8                       TpQueueDepth_u8;
#endif
} PduR_GT_Tp;

/**
 * @ingroup PDUR_TYPES_H
 *
 * pointer to PduR_GT_Tp structure.
 */
typedef const PduR_GT_Tp * PduR_GT_TpPtr;

/**
 * @ingroup PDUR_TYPES_H
* typedef struct \n
* {\n
*    PduR_RT_LoTpRxToUp loTpRxToUp; LoTp to UpTp Rx mapping table\n
*    PduR_RoutingPathGroupIdType  routingCntrl_Idx; Routing path Index \n
*}PduR_RPG_LoTpRxToUp;
*/

typedef struct
{
    PduR_RT_LoTpRxToUp loTpRxToUp;
    PduR_RoutingPathGroupIdType  routingCntrl_Idx;
}PduR_RPG_LoTpRxToUp;

/**
 * @ingroup PDUR_TYPES_H
 *
 * pointer to PduR_RPG_LoTpRxToUp structure.
 */
typedef const PduR_RPG_LoTpRxToUp *PduR_RPG_TpRxToUpPtr;

/* ------------------------------------------------------------------------ */

/**
 * @ingroup PDUR_TYPES_H
 *
 * This typedef is PduR lower layer Cancel Receive function pointer.\n
 */
typedef uint8 PduR_loCancelReceiveFP;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents the lower layer TP Pdu to call cancel receive request. \n
 * typedef struct \n
 *  { \n
 *      PduIdType                    LoTpRxId;     lower layer ID. As LoTp do not have any Rx Id, PduRSrcPduHandleId is used here \n
 *      PduR_loCancelReceiveFP       CancelRxFP;   Function Pointer for lower layer Cancel Rx API \n
 * } PduR_RT_UpTpRxToLoTp ;
 */
typedef struct
{
   PduIdType                    LoTpRxId;
/*#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)
   PduR_RoutingPathGroupIdType  RpgIdx;
#endif*/
   PduR_loCancelReceiveFP       CancelRxFP;
} PduR_RT_UpTpRxToLoTp;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents the lower layer TP Pdu to call cancel receive request. \n
 * typedef struct \n
 *  { \n
 *      const PduR_RT_UpTpRxToLoTp * UpTpToLoTp;             Pointer to PduR_RT_UpTpRxToLoTp structure \n
 *      PduIdType                                                 UpTpToLoTp_NrEntries;   No. of Routing paths per upper module \n
 * } PduR_UpTpToLoTpRxConfig;
 */
typedef struct
{
   const PduR_RT_UpTpRxToLoTp * UpTpToLoTp;
   const PduIdType*           UpTpToLoTpMapTable;
   PduIdType                                                 UpTpToLoTp_NrEntries;
} PduR_UpTpToLoTpRxConfig;

/* PDUR480 */
/**
 * @ingroup PDUR_TYPES_H
 *
 */
typedef uint8 PduR_ParameterValueType;  /* Range: 0x00 - 0x7F and 0xF1 - 0xF9 */

/* ------------------------------------------------------------------------ */
/* Buffer types */
/**
 * @ingroup PDUR_TYPES_H
 * This structure represents single entry buffer type \n
 * typedef struct \n
 *  { \n
 *  PduInfoType                                     info;               Info pointer for pdu length \n
 *  PduR_BuffLockType * lockPtr;            To lock the info pointer \n
 *  PduInfoType * readerPduInfo;      To store and forward the pduinfo pointer per routing path \n
 * } PduR_SingleEntryBuffer ;
 */
typedef struct
{
    PduInfoType                       info;
    PduR_BuffLockType * lockPtr;
    PduInfoType * readerPduInfo;
} PduR_SingleEntryBuffer;

/**
 * @ingroup PDUR_TYPES_H
 *
 * pointer to PduR_SingleEntryBuffer structure.
 */
typedef const PduR_SingleEntryBuffer * PduR_SingleEntryBufferPtr;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents fifo buffer \n
 * typedef struct \n
 *  { \n
 *  uint8 * readPtr;           Start address of read pointer \n
 *  uint8 * writePtr;          Start address of write pointer  \n
 *  PduLengthType * actLen;    Length of data which needs to be transmitted \n
 *  uint8                             used;              Number of Fifo elements used \n
 *  uint8                             txConfPending;     Transmit pending \n
 *  uint8                                readIndex;      Tx Pdu Index entry value
 *  uint8                                writeIndex;     Rx Pdu Index entry value
 * } PduR_FIFO_If_dynamic ;
 */
typedef struct
{
   uint8*                                  readPtr;       /*: note that TxIdx (requested by the spec) is
                                                         logically equivalent to our readPtr */
   uint8*                                  writePtr;
   PduLengthType                           actLen[PDUR_FIFO_IF_MAXSIZE];
   uint8                                   used;
   uint8                                   txConfPending; /**/
   uint8                                   readIndex;
   uint8                                   writeIndex;
  } PduR_FIFO_If_dynamic;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents Default values of the GW buffer of type LAST_IS_BEST.\n
 * typedef struct \n
 *  { \n
 *  uint8                    defaultValue;    Value at the corresponding byte\n
 *  uint8                    BytePosition;    Byte Position of the buffer \n
 * } PduR_DefaultValues_fixed ;
 */
typedef struct
{
   uint8                    byteDefaultValue;
   uint8                    bytePosition;
} PduR_DefaultValues_fixed;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents configured buffer \n
 * typedef struct \n
 *  { \n
 * PduRInfoType * gwBuf;       Points to PduRInfoType structure \n
 * uint8 * begin;             starting address \n
 * const PduR_DefaultValues_fixed * defaultValueElement;  default values of buffer\n
 * uint8                                   defaultValueSize; Total number of byte positions to be filled \n
 * uint8                                   confLen;     Configured Buffer Length \n
 * uint8                                   Tx_Status;   status of the transmitted Pdu
 * } PduR_Buf_If_dynamic ;
 */
typedef struct
{
    PduInfoType * gwBuf;
    uint8 * begin;
    const PduR_DefaultValues_fixed * defaultValueElement;
    uint8                                   defaultValueSize;
    uint8                                   confLen;
    uint8                                   Tx_Status;
} PduR_Buf_If_dynamic;

/**
 * @ingroup PDUR_TYPES_H
 *
 * pointer to structure  PduR_Buf_If_dynamic
 */
typedef PduR_FIFO_If_dynamic * PduR_FifoDynIfPtr;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents fixed fifo data frames.\n
 * typedef struct \n
 *  { \n
 *  uint8 * begin;    starting address \n
 *  void * dyn;      Points to either a PduR_FIFO_If_dynamic or a PduR_TpSession_Dynamic \n
 *  PduLengthType                     eltSize;  Pdu length \n
 *  uint8                             nrElts;   size \n
 * } PduR_FIFO_fixed ;
 */
typedef struct
{
   uint8 * begin;
   void * dyn;
   PduLengthType                     eltSize;
   uint8                             nrElts;
} PduR_FIFO_fixed;

/**
 * @ingroup PDUR_TYPES_H
 *
 * pointer to PduR_FIFO_fixed structure.
 */
typedef const PduR_FIFO_fixed * PduR_FifoType;



/**
 * @ingroup PDUR_TYPES_H
 * This structure represents configured cdd data frames.\n
 * typedef struct \n
 *  { \n
 *  const PduR_RT_UpToLo * CddToLo;       lower layer address \n
 *  const PduR_MT_UpToLo * CddToLoMc;     multicast lower layer address \n
 *  PduIdType                                             CddToLo_NrEntries;  number of entries \n
 * } PduR_CddConfig ;
 */
typedef struct
{
   const PduR_RT_UpToLo * CddToLo;
   const PduR_MT_UpToLo * CddToLoMc;
   const PduIdType*           CddToLoMapTable;
   PduIdType                                             CddToLo_NrEntries;
} PduR_CddConfig;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents configured lower TP data frames.\n
 * typedef struct \n
 *  { \n
   const PduR_RT_LoTpRxToUp * LoTpRxToUp;            lower Tp receive address \n
   const PduR_RT_LoTpTxToUp * LoTpTxToUp;            lower Tp transmit address \n
   PduIdType                                                 LoTpRxToUp_NrEntries;  number of receive entries \n
   PduIdType                                                 LoTpTxToUp_NrEntries;  number of transmit entries \n
 * } PduR_LoTpConfig ;
 */
typedef struct
{
   const PduR_RT_LoTpRxToUp * LoTpRxToUp;
   const PduR_RT_LoTpTxToUp * LoTpTxToUp;
   const PduIdType*           LoTpTxToUpMapTable;
   const PduIdType*           LoTpRxToUpMapTable;
   PduIdType                                                 LoTpRxToUp_NrEntries;
   PduIdType                                                 LoTpTxToUp_NrEntries;
} PduR_LoTpConfig;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents configured Upper TP data frames.\n
 * typedef struct\n
 * { \n
 *  const PduR_RT_UpToLo * UpToLo; \n
 *  const PduR_MT_UpToLo * UpToLoMc; \n
 *  PduIdType                                                 UpToLo_NrEntries; \n
 *  } PduR_UpConfig; \n
 */
typedef struct
{
   const PduR_MT_UpToLo * UpToLoMc;
   const PduR_RT_UpToLo * UpToLo;
   const PduIdType*           UpToLoMapTable;
   PduIdType                                                 UpToLo_NrEntries;
} PduR_UpConfig;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents configured lower If data frames.\n
 * typedef struct \n
 *  { \n
   const PduR_RT_LoIfRxToUp * LoIfRxToUp;            lower If receive address \n
   const PduR_RT_LoIfTxToUp * LoIfTxToUp;            lower If transmit address \n
   PduIdType                                                 LoIfRxToUp_NrEntries;  number of receive entries \n
   PduIdType                                                 LoIfTxToUp_NrEntries;  number of transmit entries \n
 * } PduR_LoIfDConfig ;
 */
typedef struct
{
   const PduR_RT_LoIfRxToUp * LoIfRxToUp;
   const PduR_RT_LoIfTxToUp * LoIfTxToUp;
   const PduIdType*           LoIfTxToUpMapTable;
   const PduIdType*           LoIfRxToUpMapTable;
   PduIdType                                                 LoIfRxToUp_NrEntries;
   PduIdType                                                 LoIfTxToUp_NrEntries;
} PduR_LoIfDConfig;
/**
 * @ingroup PDUR_TYPES_H
 * This structure represents configured trigger transmit data frames.\n
 * typedef struct \n
 *  { \n
   const PduR_RT_LoIfRxToUp * LoIfRxToUp;            lower If receive address \n
   const PduR_RT_LoIfTxToUp * LoIfTxToUp;            lower If transmit address \n
   PduIdType                                                 LoIfRxToUp_NrEntries;  number of receive entries \n
   PduIdType                                                 LoIfTxToUp_NrEntries;  number of transmit entries \n
 * } PduR_LoIfTTConfig ;
 */
typedef struct
{
    const PduR_RT_LoIfRxToUp * LoIfRxToUp;
    const PduR_RT_LoTtIfTxToUp * LoIfTxToUp;
    const PduIdType*           LoIfTxToUpMapTable;
    const PduIdType*           LoIfRxToUpMapTable;
    PduIdType                                                 LoIfRxToUp_NrEntries;
    PduIdType                                                 LoIfTxToUp_NrEntries;
} PduR_LoIfTTConfig;


/* ------------------------------------------------------------------------ */

/* Routing Path Group feature related definitions. */

/**
 * @ingroup PDUR_TYPES_H
 *
 * Void pointer used in RPG PduR_TargetInfoType structure \n
 */
typedef void * PduR_RPGVoidPtr;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents target type.  \n
 * typedef struct \n
 *  { \n
 *      PduR_RPGVoidPtr                             destInfo;           Pointer to Dest info table within the RPG  \n
 *      uint8                                       pathType;           For which PathType this Dest belongs to \n
 * } PduR_TargetInfoType ;
 */
typedef struct
{
    PduR_RPGVoidPtr             destInfo;
    uint8                       pathType;
} PduR_TargetInfoType;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents routing path group type.  \n
 * typedef struct \n
 *  { \n
 *      uint8 * begin;      starting address of the destination GW buffer \n
 *      const PduR_DefaultValues_fixed * defaultValueElement;        default \n
 *      values of buffer\n
 *      uint8                                   defaultValueSize;        Total number of byte positions to be filled\n
 * } PduR_DefaultValue_Info ;
 */
typedef struct
{
    uint8 * begin;
    const PduR_DefaultValues_fixed * defaultValueElement;
    uint8                                   defaultValueSize;
}PduR_DefaultValue_Info;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents routing path group type.  \n
 * typedef struct \n
 *  { \n
 *      const PduR_TargetInfoType * targetInfo;    Pointer to target info table for this RPG \n
 *      const PduR_DefaultValue_Info * defaultValue_info; Information on Default values of \n
 *      each Dest PDU in RPG. \n
 *      PduR_RoutingPathGroupIdType          rpg_Idx;        Index to routing enable/disable status flag of RPG  \n
 *      uint8                                cntDestOfFIFOType;  No. of DestPdus of FIFO type referred in this RPG\n
 *      uint8                                cntOfDestWithDefValue; No. of Dest pdu having default values configured. \n
 * } PduR_RPGInfoType ;
 */
typedef struct
{
    const PduR_TargetInfoType * targetInfo;
    const PduR_DefaultValue_Info * defaultValue_info;
    PduR_RoutingPathGroupIdType                         rpg_Idx;
    uint8                                               cntDestOfTypeFIFO;
    uint8                                               cntOfDestWithDefValue;
} PduR_RPGInfoType;

/**
 * @ingroup PDUR_TYPES_H
 * This structure represents Post build config types. \n
 * typedef struct\n
 *  {\n
 *   const PduR_CddConfig * cddConf;        CDD config \n
 *   const PduR_LoTpConfig * LoTpConf;       Lower Tp config \n
 *   const PduR_LoIfDConfig * LoIfDConf;      Lower If config \n
 *   const PduR_LoIfTTConfig * LoIfTTConf;     Lower if trigger transmit config \n
 *   const PduR_UpConfig * UpIfConf;       Upper layer If config
 *   const PduR_UpConfig * UpTpConf;;      Upper layer Tp config \n
 *   const PduR_MT_UpToLo * gwToLoMc;       gateway to lower layer multicasting \n
*   const PduR_GT_IfTx * gwIfTx;         gateway if transmission \n
 *   const PduR_GT_If * gwIf;           gateway If \n
 *   const PduR_GT_Tp * gwTp;           gateway Tp \n
 *   const PduR_RPG_LoTpRxToUp *rpgRxTp    RPG for Tp Rx RP\n
 *
 *  #if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)\n
 *   const PduR_RPGInfoType * rpg_ConfigInfo;     config info \n
 *   boolean * rpg_EnRoutingInfo; \n
 *   PduR_RoutingPathGroupIdType                            rpg_NrEntries;          Number of RPGs\n
 *  #endif \n
 *   const PduR_UpTpToLoTpRxConfig * PduR_UpTpToLoTpRxCfg;   To Support Cancel Receive API \n
 *   uint16                                                 configId;       config ID \n
 * } PduR_PBConfigType;
*/
typedef struct 
{
    const PduR_CddConfig * cddConf;
    const PduR_LoTpConfig * LoTpConf;
    const PduR_LoIfDConfig * LoIfDConf;
    const PduR_LoIfTTConfig * LoIfTTConf;
    const PduR_UpConfig * UpIfConf;
    const PduR_UpConfig * UpTpConf;
    const PduR_MT_UpToLo * gwToLoMc;

    const PduR_GT_IfTx * gwIfTx;
    const PduR_GT_If * gwIf;
    const PduR_GT_Tp * gwTp;
    const PduR_RPG_LoTpRxToUp * rpgRxTp;
#if defined(PDUR_TPGATEWAY_SUPPORT) && (PDUR_TPGATEWAY_SUPPORT != STD_OFF)
    const PduR_GwTp_SessionListType * gwTp_SessionListPtr;
#endif
#if defined(PDUR_MULTICAST_TO_IF_SUPPORT) && (PDUR_MULTICAST_TO_IF_SUPPORT != 0)
    const PduR_UpIfTxConf_Config * UpIfTxConf_Config;
#endif
#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)
    const PduR_RPGInfoType * rpg_ConfigInfo;
    const boolean * rpg_EnRoutingInfo;
    boolean * pduR_Rpg_Status;
    const PduR_RoutingPathGroupIdType*                           rpgId_MappingTab;
    PduR_RoutingPathGroupIdType                            rpg_NrEntries; /* Number of RPGs.*/
#endif
    const PduR_UpTpToLoTpRxConfig * PduR_UpTpToLoTpRxCfg; /*To Support Cancel Receive API*/
    uint16                                                 configId;
    uint16                                                 totalNoOfGwTpRps; /*Total no of GwTp routingPaths*/
    uint16                                                 totalNoOfGwIfRps; /*Total no of GwIf routingPaths*/
} PduR_PBConfigType;


typedef struct
{
    Dem_EventStatusType initFailedErrorStatus;
    Dem_EventStatusType instanceLostErrorStatus;
    boolean initFailedErrorFlag;
    boolean instanceLostErrorFlag;
}PduR_DsmReInitType;


/**
 * @ingroup PDUR_TYPES_H
 *
 * PduR internal return type,typedef to Uint8
 */
typedef uint8 PduR_ReturnType;

/**
 * @ingroup PDUR_TYPES_H
 *
 * PduR internal Gw If buffer Type,Defined to Boolean value
 */
#define PduR_GwIfBufType_FIFO TRUE

/**
 * @ingroup PDUR_TYPES_H
 *
 * PduR internal Gw If buffer Type,Defined to Boolean value
 */
#define PduR_GwIfBufType_NonFIFO FALSE

/**
 * @ingroup PDUR_TYPES_H
 *
 * PduR internal Tp Module, Defined to Boolean value
 */
#define PduR_TpModule TRUE

/**
 * @ingroup PDUR_TYPES_H
 *
 * PduR internal If Module, Defined to Boolean value
 */
#define PduR_NonTpModule FALSE

#endif /* PDUR_TYPES_H */

