


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/


/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
/*
 * If COM_DontUseExternalSymbolicNames is defined before including Com_Cfg.h file, then external symbolic names will
 * not be visible.
 * Com_PBcfg.c file should not use external symbolic names.
 * This mechanism is used to prevent the accidental usage of external symbolic names in this file
 * This file should use only internal symbolic name defined in  Com_PBcfg_InternalSymbolicNames.h
 */
#define COM_DontUseExternalSymbolicNames
#include "Com_Prv.h"
#include "Com_PBcfg_Common.h"


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

/* START: I-PDU Buffers */

    /* Tx-Ipdu Local Buffers */

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    uint8 Com_dIP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_TxByte[8];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    uint8 Com_dIP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_TxByte[6];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    uint8 Com_dIP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_TxByte[7];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    uint8 Com_dIP_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_TxByte[6];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    /* Rx-Ipdu Local Buffers */

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    uint8 Com_dIP_ComRx_0x261_Can_Network_ETAS_Channel_CAN_RxByte[1];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    uint8 Com_dIP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_RxByte[8];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    uint8 Com_dIP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_RxByte[8];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* END: I-PDU Buffers */

#ifdef COM_F_MASKEDNEWDIFFERSOLD

/* Begin section for RAM variables of uint32 type */
#define COM_START_SEC_VAR_CLEARED_32
#include "Com_MemMap.h"
uint32 Com_F_OldVal_au32[1];

/* End section for RAM variables of uint32 type */
#define COM_STOP_SEC_VAR_CLEARED_32
#include "Com_MemMap.h"
#endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */

#if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC)

/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
Com_OldValTrigOnChng_tauo Com_OldValTrigOnChng_auo[1];

/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* #if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC) */

#ifdef COM_F_ONEEVERYN

/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
Com_OneEveryN_tst Com_OneEveryN_ast[1];

/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* #ifdef COM_F_ONEEVERYN */


/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#ifdef COM_TP_IPDUTYPE
PduLengthType Com_TpTxIpduLength_auo[ COM_MAX_NUM_OF_TXIPDU_IN_ANY_VARIANT ];
#endif

Com_TxIpduRamData_tst Com_TxIpduRam_ast[ COM_MAX_NUM_OF_TXIPDU_IN_ANY_VARIANT ];

Com_RxIpduRamData_tst Com_RxIpduRam_ast[ COM_MAX_NUM_OF_RXIPDU_IN_ANY_VARIANT ];

Com_TxSignalFlagType_tst Com_TxSignalFlag_ast[ COM_MAX_NUM_OF_TXSIG_IN_ANY_VARIANT ];

Com_RxSignalFlagType_tst Com_RxSignalFlag_ast[ COM_MAX_NUM_OF_RXSIG_IN_ANY_VARIANT ];

/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"


/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

uint8 Com_IpduCounter_au8[ COM_MAX_NUM_OF_TXIPDU_IN_ANY_VARIANT + COM_MAX_NUM_OF_RXIPDU_IN_ANY_VARIANT ];

# if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
uint8 Com_IpduCounter_DM_au8[ COM_MAX_NUM_OF_RXIPDU_IN_ANY_VARIANT ];
# endif

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"


#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)

/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
PduIdType Com_RxGwQueue_auo[0u + 1u];

Com_RxGwQueueRAMType_tst Com_RxGwQueue_st;

/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */


#ifdef COM_RX_SIGNALGROUP

/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
Com_RxSignalGrpFlagType_tst Com_RxSignalGrpFlag_ast[ COM_MAX_NUM_OF_RXSIGGRP_IN_ANY_VARIANT ];

/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#endif

#ifdef COM_TX_SIGNALGROUP
#if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY)

/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
Com_TxSignalGrpFlagType_tst Com_TxSignalGrpFlag_ast[ COM_MAX_NUM_OF_TXSIGGRP_IN_ANY_VARIANT ];

/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* #if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY) */


/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
Com_TxGrpSignalFlagType_tst Com_TxGrpSignalFlag_ast[ COM_MAX_NUM_OF_TXGRPSIG_IN_ANY_VARIANT ];

/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#endif

#ifdef COM_RxIPduDeferredProcessing

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    /* Buffer to hold data for deferred signal processing */
static uint8 Com_DeferredBuffer_au8[ 8 ];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

#endif /* #ifdef COM_RxIPduDeferredProcessing */






/* Signal Buffers */
/* Type: UINT8, SINT8, UINT8_N, BOOLEAN */

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

uint8 Com_SigType_au8[16];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"


/* Type: UINT16, SINT16 */

/* Begin section for RAM variables of uint16 type */
#define COM_START_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"

uint16 Com_SigType_au16[2];

/* End section for RAM variables of uint16 type */
#define COM_STOP_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"


/* Type: UINT32, SINT32, FLOAT32 */
#define Com_SigType_au32          ( NULL_PTR )

/* Type: UINT64, SINT64 */
#ifdef COM_RXSIG_INT64
#define Com_SigType_au64          ( NULL_PTR )
#endif /* #ifdef COM_RXSIG_INT64 */


#ifdef COM_RXSIG_FLOAT64SUPP
#define Com_SigType_af64          ( NULL_PTR )
#endif /* #ifdef COM_RXSIG_FLOAT64SUPP */

/* Type: UINT8_DYN */
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
#define Com_SigType_dyn_au8          ( NULL_PTR )
#endif


#ifdef COM_RX_SIGNALGROUP_ARRAY
#define Com_RxSigGrpArrayBuf_au8  ( NULL_PTR )
#endif /* #ifdef COM_RX_SIGNALGROUP_ARRAY */

#ifdef COM_RX_SIGNALGROUP
/* Rx Group Signal Buffers */
/* The function Com_ReceiveSignalGroup() is not interrupt safe, hence there is need to
 * to generate second buffer for all the group signals
 */
/* Type: UINT8, SINT8, UINT8_N, BOOLEAN */
#define Com_RxGrpSigType_au8             ( NULL_PTR )
#define Com_SecondRxGrpSigType_au8       ( NULL_PTR )

/* Type: UINT16, SINT16 */
#define Com_RxGrpSigType_au16            ( NULL_PTR )
#define Com_SecondRxGrpSigType_au16      ( NULL_PTR )

/* Type: UINT32, SINT32 */
#define Com_RxGrpSigType_au32            ( NULL_PTR )
#define Com_SecondRxGrpSigType_au32      ( NULL_PTR )

/* Type: UINT64, SINT64 */
#ifdef COM_RXGRPSIG_INT64
#define Com_RxGrpSigType_au64            ( NULL_PTR )
#define Com_SecondRxGrpSigType_au64      ( NULL_PTR )
#endif /* #ifdef COM_RXGRPSIG_INT64 */

#ifdef COM_RXGRPSIG_FLOAT64SUPP
#define Com_RxGrpSigType_af64            ( NULL_PTR )
#define Com_SecondRxGrpSigType_af64      ( NULL_PTR )
#endif /* #ifdef COM_RXGRPSIG_FLOAT64SUPP */

#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
#define Com_RxGrpSigType_dyn_au8         ( NULL_PTR )
#define Com_SecondRxGrpSigType_dyn_au8   ( NULL_PTR )
#endif /* #ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT */

#endif /* #ifdef COM_RX_SIGNALGROUP */


#ifdef COM_TX_SIGNALGROUP


/* Tx Group Signal Buffers  */
/* Type: UINT8, SINT8, UINT8_N, BOOLEAN */
#define Com_TxGrpSigType_au8          ( NULL_PTR )

/* Type: UINT16, SINT16 */
#define Com_TxGrpSigType_au16         ( NULL_PTR )

/* Type: UINT32, SINT32, FLOAT32 */
#define Com_TxGrpSigType_au32         ( NULL_PTR )

/* Type: UINT64, SINT64 */
#ifdef COM_TXGRPSIG_INT64
#define Com_TxGrpSigType_au64         ( NULL_PTR )
#endif /* #ifdef COM_TXGRPSIG_INT64 */


/* Type: FLOAT64 */
#ifdef COM_TXGRPSIG_FLOAT64SUPP
#define Com_TxGrpSigType_af64         ( NULL_PTR )
#endif /* #ifdef COM_TXGRPSIG_FLOAT64SUPP */


/* Type: UINT8_DYN */
#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
#define Com_TxGrpSigType_dyn_au8         ( NULL_PTR )
#endif /* #ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT */


#endif /* end of #ifdef COM_TX_SIGNALGROUP */



/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
*/





#ifdef COM_INITVALOPTIMIZATION

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_32
#include "Com_MemMap.h"

const uint32 Com_Prv_UniqueInitVal_acu32[7] =
{
    0x0uL 
    ,    0x1FFuL 
    ,    0x2000uL 
    ,    0x28uL 
    ,    0x4000uL 
    ,    0x7FFFuL 
    ,    0xFFEuL 
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_32
#include "Com_MemMap.h"

#endif /* end of #ifdef COM_INITVALOPTIMIZATION */



/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_Prv_xRxRamBuf_tst Com_Prv_xRxRamBuf_acst[ 1u ] =
{
    {
    (uint8 * )              Com_SigType_au8

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
    ,(uint8 * )             Com_SigType_dyn_au8
#endif

    ,(uint16 * )            Com_SigType_au16

    ,(uint32 * )            Com_SigType_au32

#ifdef COM_RXSIG_INT64
    ,(uint64 * )            Com_SigType_au64
#endif

#ifdef COM_RXSIG_FLOAT64SUPP
    ,(float64 * )           Com_SigType_af64
#endif

#ifdef COM_RX_SIGNALGROUP
    ,(uint8 * )             Com_RxGrpSigType_au8

#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
    ,(uint8 * )             Com_RxGrpSigType_dyn_au8
    ,(uint8 * )             Com_SecondRxGrpSigType_dyn_au8
#endif

    ,(uint16 * )            Com_RxGrpSigType_au16

    ,(uint32 * )            Com_RxGrpSigType_au32

#ifdef COM_RXGRPSIG_INT64
    ,(uint64 * )            Com_RxGrpSigType_au64
#endif

#ifdef COM_RXGRPSIG_FLOAT64SUPP
    ,(float64 * )           Com_RxGrpSigType_af64
#endif

    ,(uint8 * )             Com_SecondRxGrpSigType_au8

    ,(uint16 * )            Com_SecondRxGrpSigType_au16

    ,(uint32 * )            Com_SecondRxGrpSigType_au32

#ifdef COM_RXGRPSIG_INT64
    ,(uint64 * )            Com_SecondRxGrpSigType_au64
#endif

#ifdef COM_RXGRPSIG_FLOAT64SUPP
    ,(float64 * )           Com_SecondRxGrpSigType_af64
#endif

#ifdef COM_RX_SIGNALGROUP_ARRAY
    ,(uint8 * )             Com_RxSigGrpArrayBuf_au8
#endif

#endif /* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_RxIPduDeferredProcessing
    ,(uint8 * )             Com_DeferredBuffer_au8
#endif
    }

};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"



#ifdef COM_TX_SIGNALGROUP


/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_Prv_xTxSigGrpRamBuf_tst Com_Prv_xTxSigGrpRamBuf_acst[ 1u ] =
{
    {
    (uint8 * )              Com_TxGrpSigType_au8

#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
    ,(uint8 * )             Com_TxGrpSigType_dyn_au8
#endif

    ,(uint16 * )             Com_TxGrpSigType_au16

    ,(uint32 * )             Com_TxGrpSigType_au32

#ifdef COM_TXGRPSIG_INT64
    ,(uint64 * )            Com_TxGrpSigType_au64
#endif

#ifdef COM_TXGRPSIG_FLOAT64SUPP
    ,(float64 * )           Com_TxGrpSigType_af64
#endif

    }

};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"


#endif/* #ifdef COM_TX_SIGNALGROUP */








