


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#ifndef COM_PBCFG_COMMON_H
#define COM_PBCFG_COMMON_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

#define COM_UPDATE_MAX      COM_MAX_U8_VALUE    /* max ipdu size */



/* In all the variants */
# define COM_NUM_OF_TXIPDU_IN_ALL_VARIANTS              4u
# define COM_NUM_OF_RXIPDU_IN_ALL_VARIANTS              3u

# define COM_NUM_OF_TXSIG_IN_ALL_VARIANTS               25u
# define COM_NUM_OF_RXSIG_IN_ALL_VARIANTS               18u

# define COM_NUM_OF_TXSIGGRP_IN_ALL_VARIANTS            0u
# define COM_NUM_OF_RXSIGGRP_IN_ALL_VARIANTS            0u

# define COM_NUM_OF_TXGRPSIG_IN_ALL_VARIANTS            0u
# define COM_NUM_OF_RXGRPSIG_IN_ALL_VARIANTS            0u

# define COM_NUM_OF_IPDUGRP_IN_ALL_VARIANTS             2u

# define COM_NUM_OF_TX_SIG_GRPSIG_IN_ALL_VARIANTS       (COM_NUM_OF_TXSIG_IN_ALL_VARIANTS + COM_NUM_OF_TXGRPSIG_IN_ALL_VARIANTS)
# define COM_NUM_OF_RX_SIG_GRPSIG_IN_ALL_VARIANTS       (COM_NUM_OF_RXSIG_IN_ALL_VARIANTS + COM_NUM_OF_RXGRPSIG_IN_ALL_VARIANTS)

/* max in any of the variant */
# define COM_MAX_NUM_OF_TXIPDU_IN_ANY_VARIANT           4u
# define COM_MAX_NUM_OF_RXIPDU_IN_ANY_VARIANT           3u

# define COM_MAX_NUM_OF_TXSIG_IN_ANY_VARIANT            25u
# define COM_MAX_NUM_OF_RXSIG_IN_ANY_VARIANT            18u

# define COM_MAX_NUM_OF_TXSIGGRP_IN_ANY_VARIANT         1u
# define COM_MAX_NUM_OF_RXSIGGRP_IN_ANY_VARIANT         1u

# define COM_MAX_NUM_OF_TXGRPSIG_IN_ANY_VARIANT         1u
# define COM_MAX_NUM_OF_RXGRPSIG_IN_ANY_VARIANT         1u




/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/


/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"

#ifdef COM_TP_IPDUTYPE
/* Used to store PduLength of the Tx-Ipdu just befor PduR_ComTransmit() is called. */
extern PduLengthType                                        Com_TpTxIpduLength_auo[];
#endif /* #ifdef COM_TP_IPDUTYPE */

#ifdef COM_RX_SIGNALGROUP
/* Array of Rx Signal group flags */
extern Com_RxSignalGrpFlagType_tst                          Com_RxSignalGrpFlag_ast[];
#endif /* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_TX_SIGNALGROUP
#if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY)

/* Array of Tx Signal group flags */
extern Com_TxSignalGrpFlagType_tst                          Com_TxSignalGrpFlag_ast[];
#endif /* #if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY) */

/* Array of Tx  group Signal flags */
extern Com_TxGrpSignalFlagType_tst                          Com_TxGrpSignalFlag_ast[];
#endif /* #ifdef COM_TX_SIGNALGROUP */

/* Array of Rx Ipdu Ram flags */
extern Com_RxIpduRamData_tst                                Com_RxIpduRam_ast[];

/* Array of Tx Ipdu Ram flags */
extern Com_TxIpduRamData_tst                                Com_TxIpduRam_ast[];

/* Array of Tx signal flags */
extern Com_TxSignalFlagType_tst                             Com_TxSignalFlag_ast[];

/* Array of Rx signal flags */
extern Com_RxSignalFlagType_tst                             Com_RxSignalFlag_ast[];

/* Gw Rx Ipdu queue */
extern PduIdType                                            Com_RxGwQueue_auo[];

/* Structure for Gw Rx queue */
extern Com_RxGwQueueRAMType_tst                             Com_RxGwQueue_st;


#ifdef COM_F_ONEEVERYN
extern Com_OneEveryN_tst                                    Com_OneEveryN_ast[];
#endif

#if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC)
extern Com_OldValTrigOnChng_tauo                            Com_OldValTrigOnChng_auo[];
#endif


/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"


#ifdef COM_F_MASKEDNEWDIFFERSOLD

/* Begin section for RAM variables of uint32 type */
#define COM_START_SEC_VAR_CLEARED_32
#include "Com_MemMap.h"
extern uint32 Com_F_OldVal_au32[];

/* End section for RAM variables of uint32 type */
#define COM_STOP_SEC_VAR_CLEARED_32
#include "Com_MemMap.h"
#endif


/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Counter for every Ipdu which gives number of Started Ipdu groups, this Ipdu is part of. */
extern uint8 Com_IpduCounter_au8[];

#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
/* Counter for every Ipdu which gives number of Deadline monitoring enbaled Rx Ipdu groups, this Ipdu is part of. */
extern uint8 Com_IpduCounter_DM_au8[];
#endif /* #if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout) */

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"


/* START: I-PDU Buffers */

    /* Tx-Ipdu Local Buffers */

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
extern uint8 Com_dIP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_TxByte[];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
extern uint8 Com_dIP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_TxByte[];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
extern uint8 Com_dIP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_TxByte[];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
extern uint8 Com_dIP_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_TxByte[];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
    /* Rx-Ipdu Local Buffers */

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
extern uint8 Com_dIP_ComRx_0x261_Can_Network_ETAS_Channel_CAN_RxByte[];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
extern uint8 Com_dIP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_RxByte[];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
extern uint8 Com_dIP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_RxByte[];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

/* END: I-PDU Buffers */



/*Start: Signal Buffer -----> uint8/sint8/boolean/uint8[n]*/

/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
extern uint8 Com_SigType_au8[];

/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
/*End: Signal Buffer -----> uint8/sint8/boolean/uint8[n]*/
/*Start: Signal Buffer -----> uint16/sint16*/

/* Begin section for RAM variables of uint16 type */
#define COM_START_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
extern uint16 Com_SigType_au16[];

/* End section for RAM variables of uint16 type */
#define COM_STOP_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
/*End: Signal Buffer -----> uint16/sint16*/









/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"


#if defined (COM_TXSIG_FLOAT64SUPP ) || defined(COM_TXGRPSIG_FLOAT64SUPP) || defined(COM_RXGRPSIG_FLOAT64SUPP) || defined(COM_RXSIG_FLOAT64SUPP)
extern const float64 Com_FloatValArray_acf64[];
#endif

/* Set of Rx-buffers generated for each ComMainFunction */
extern const Com_Prv_xRxRamBuf_tst                          Com_Prv_xRxRamBuf_acst[];

#ifdef COM_TX_SIGNALGROUP

/* Set of Tx-GroupSignal shadow buffers generated for each ComMainFunction */
extern const Com_Prv_xTxSigGrpRamBuf_tst                    Com_Prv_xTxSigGrpRamBuf_acst[];
#endif /* #ifdef COM_TX_SIGNALGROUP */


/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"



#ifdef COM_INITVALOPTIMIZATION

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_32
#include "Com_MemMap.h"

extern const uint32                                         Com_Prv_UniqueInitVal_acu32[];


/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_32
#include "Com_MemMap.h"
#endif /* end of #ifdef COM_INITVALOPTIMIZATION */





#endif   /* end of COM_PBCFG_COMMON_H */


