

 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/


/*
 * If COM_DontUseExternalSymbolicNames is defined before including Com_Cfg.h file, then external symbolic names will
 * not be visible.
 * Com_PBcfg.c file should not use external symbolic names.
 * This mechanism is used to prevent the accidental usage of external symbolic names in this file
 * This file should use only internal symbolic name defined in  Com_PBcfg_InternalSymbolicNames.h
 */
#define COM_DontUseExternalSymbolicNames
#include "Com_Prv.h"
#include "Com_Cbk.h"
#include "PduR_Com.h"
#include "Com_PBcfg_Common.h"
#include "Com_PBcfg_Variant.h"

/*
 * The file Com_PBcfg_InternalSymbolicNames.h defines internal symbolic names
 * These names should be used in the tables generated in this file
 * Regular symbolic names should not be used here
 */
#define COM_PBCFG_INCLUDE_INT_SYM_NAMES
#include "Com_PBcfg_InternalSymbolicNames.h"



/* START: Tx Signal Details  */

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_Prv_xTxSigCfg_tst Com_Prv_xTxSigCfg_acst[COM_NUM_TX_SIGNALS] =
{
        
    {  /* S_ETAS03_Sig10_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        10,                                                /*BitPosition*/

        1,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS03_Sig11_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        4,                                                /*BitPosition*/

        4,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS03_Sig12_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        2,                                                /*BitPosition*/

        2,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS03_Sig13_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0xFFEuL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = NEW_IS_WITHIN
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x2C,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        43,                                                /*BitPosition*/

        13,/*BitSize*/

#ifdef COM_TxFilters
        0x00,                                                  /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        6u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT16
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x22                                               /*General*/

    },
        
    {  /* S_ETAS03_Sig14_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        15,                                                /*BitPosition*/

        1,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS03_Sig15_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x7FFFuL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = NEW_IS_WITHIN
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x2C,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        24,                                                /*BitPosition*/

        16,/*BitSize*/

#ifdef COM_TxFilters
        0x01,                                                  /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        5u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT16
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x22                                               /*General*/

    },
        
    {  /* S_ETAS03_Sig16_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        14,                                                /*BitPosition*/

        1,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS03_Sig17_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        11,                                                /*BitPosition*/

        2,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS03_Sig18_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        13,                                                /*BitPosition*/

        1,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS03_Sig19_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        0,                                                /*BitPosition*/

        2,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS04_Sig20_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x1FFuL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = NEW_IS_WITHIN
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x2C,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        32,                                                /*BitPosition*/

        10,/*BitSize*/

#ifdef COM_TxFilters
        0x02,                                                  /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        1u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT16
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x22                                               /*General*/

    },
        
    {  /* S_ETAS04_Sig21_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x28uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = NEW_IS_WITHIN
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x2C,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        40,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0x03,                                                  /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        3u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS04_Sig22_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = NEW_IS_WITHIN
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x2C,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        13,                                                /*BitPosition*/

        11,/*BitSize*/

#ifdef COM_TxFilters
        0x04,                                                  /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT16
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x22                                               /*General*/

    },
        
    {  /* S_ETAS04_Sig23_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = NEW_IS_WITHIN
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x2C,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        20,                                                /*BitPosition*/

        9,/*BitSize*/

#ifdef COM_TxFilters
        0x05,                                                  /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT16
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x22                                               /*General*/

    },
        
    {  /* S_ETAS04_Sig24_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x28uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = NEW_IS_WITHIN
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x2C,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        48,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0x06,                                                  /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        3u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS04_Sig25_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = NEW_IS_WITHIN
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x2C,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        26,                                                /*BitPosition*/

        10,/*BitSize*/

#ifdef COM_TxFilters
        0x07,                                                  /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT16
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x22                                               /*General*/

    },
        
    {  /* S_ETAS05_Sig26_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        0,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS05_Sig27_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        8,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS05_Sig28_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        16,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS05_Sig29_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        24,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS05_Sig30_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        32,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS05_Sig31_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        40,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS05_Sig32_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        48,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_ETAS05_Sig33_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = ALWAYS
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x4,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        56,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* S_NM_Tx_Sig_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_Tx */
#ifdef COM_TxInvalid
        0x0uL,             /* DataInvalid_Val*/
#endif
#ifndef COM_INITVALOPTIMIZATION
        0x0uL,             /* Init_Val*/
#endif

        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = COM_NOTCONFIGURED
            DataInvalidType : 1;    = false
            TimeOutEnabled  : 1;    = false
        }Com_TxSigPropType;    */
        0x54,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        16,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        COM_MAX_U8_VALUE,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        COM_MAX_U8_VALUE,                                              /*OldVal_Index*/
#endif

        (Com_IpduId_tuo)ComIPdu_Internal_IP_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_Tx,             /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                               /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1; = false
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    }

};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

/* END: Tx Signal Details  */

/* START: Rx Signal Details  */

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_Prv_xRxSigCfg_tst Com_Prv_xRxSigCfg_acst[COM_NUM_RX_SIGNALS] =
{    
        
    {  /* S_ETAS02_Sig09_ComRx_0x261_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS02_Sig09_ComRx_0x261_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       0,                         /*BitPosition*/
       0,                /* Signal Buffer Index */
       
      1, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x261_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS06_Sig34_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS06_Sig34_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       0,                         /*BitPosition*/
       1,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS06_Sig35_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS06_Sig35_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       8,                         /*BitPosition*/
       2,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS06_Sig36_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS06_Sig36_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       16,                         /*BitPosition*/
       3,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS06_Sig37_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS06_Sig37_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       24,                         /*BitPosition*/
       4,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS06_Sig38_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS06_Sig38_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       32,                         /*BitPosition*/
       5,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS06_Sig39_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS06_Sig39_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       40,                         /*BitPosition*/
       6,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS06_Sig40_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS06_Sig40_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       48,                         /*BitPosition*/
       7,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS06_Sig41_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS06_Sig41_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       56,                         /*BitPosition*/
       8,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS01_Sig00_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS01_Sig00_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       8,                         /*BitPosition*/
       9,                /* Signal Buffer Index */
       
      2, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS01_Sig01_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS01_Sig01_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       37,                         /*BitPosition*/
       10,                /* Signal Buffer Index */
       
      1, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS01_Sig02_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS01_Sig02_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       56,                         /*BitPosition*/
       11,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS01_Sig03_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS01_Sig03_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       48,                         /*BitPosition*/
       12,                /* Signal Buffer Index */
       
      4, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS01_Sig04_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS01_Sig04_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       36,                         /*BitPosition*/
       13,                /* Signal Buffer Index */
       
      1, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS01_Sig05_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS01_Sig05_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       24,                         /*BitPosition*/
       14,                /* Signal Buffer Index */
       
      1, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS01_Sig06_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS01_Sig06_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x0uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       38,                         /*BitPosition*/
       15,                /* Signal Buffer Index */
       
      2, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        0u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS01_Sig07_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS01_Sig07_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x4000uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       25,                         /*BitPosition*/
       0,                /* Signal Buffer Index */
       
      15, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        4u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT16
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x22,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* S_ETAS01_Sig08_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        &Rte_COMCbk_S_ETAS01_Sig08_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       0x0uL,              /* DataInvalid_Val */
#endif
#ifndef COM_INITVALOPTIMIZATION
       0x2000uL,              /* Init_Val */
#endif

#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif
       10,                         /*BitPosition*/
       1,                /* Signal Buffer Index */
       
      14, /*BitSize*/

#ifdef COM_RxFilters
        COM_MAX_U8_VALUE,                           /*Filter_Index*/
#endif

       (Com_IpduId_tuo)ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,                       /*Ipdu Reference*/
#ifdef COM_INITVALOPTIMIZATION
        2u,                                                             /* Init_Val_Index*/
#endif
        /*
        {
            Type:5;        = UINT16
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = false
            Not_Used:1;
        }Com_GeneralType; */
        0x22,       /*General*/

        /*
        {
            DataInvalidType:2;   = NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = false
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    

};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

/* END: Rx Signal Details  */











/* START: TMS Details  */


/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

/* IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx has a TMS switch */
static const Com_TransModeInfo_tst Com_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx_TransModeInfo_acst[] =
{
    /* True Mode configuration */
    {
        1, /* TimePeriod */
        
        1, /* TimeOffset */
        
        0, /* RepetitionPeriod */
        
        0,  /* NumRepetitions */
#ifdef COM_MIXEDPHASESHIFT
        COM_TXMODE_PERIODIC, /* Mode */
        COM_FALSE    /* MixedPhaseShift status */
#else
        COM_TXMODE_PERIODIC /* Mode */
#endif /* #ifdef COM_MIXEDPHASESHIFT */
    }
,
    /* False Mode configuration */
    {
        1, /* TimePeriod */
        
        1, /* TimeOffset */
        
        0, /* RepetitionPeriod */
        
        0,  /* NumRepetitions */

#ifdef COM_MIXEDPHASESHIFT
        COM_TXMODE_PERIODIC, /* Mode */
        COM_FALSE    /* MixedPhaseShift status */
#else
        COM_TXMODE_PERIODIC /* Mode */
#endif /* #ifdef COM_MIXEDPHASESHIFT */

    }
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"



/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

/* IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx has a TMS switch */
static const Com_TransModeInfo_tst Com_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx_TransModeInfo_acst[] =
{
    /* True Mode configuration */
    {
        5, /* TimePeriod */
        
        1, /* TimeOffset */
        
        0, /* RepetitionPeriod */
        
        0,  /* NumRepetitions */
#ifdef COM_MIXEDPHASESHIFT
        COM_TXMODE_PERIODIC, /* Mode */
        COM_FALSE    /* MixedPhaseShift status */
#else
        COM_TXMODE_PERIODIC /* Mode */
#endif /* #ifdef COM_MIXEDPHASESHIFT */
    }
,
    /* False Mode configuration */
    {
        5, /* TimePeriod */
        
        1, /* TimeOffset */
        
        0, /* RepetitionPeriod */
        
        0,  /* NumRepetitions */

#ifdef COM_MIXEDPHASESHIFT
        COM_TXMODE_PERIODIC, /* Mode */
        COM_FALSE    /* MixedPhaseShift status */
#else
        COM_TXMODE_PERIODIC /* Mode */
#endif /* #ifdef COM_MIXEDPHASESHIFT */

    }
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"



/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

/* IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx has a TMS switch */
static const Com_TransModeInfo_tst Com_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx_TransModeInfo_acst[] =
{
    /* True Mode configuration */
    {
        10, /* TimePeriod */
        
        1, /* TimeOffset */
        
        0, /* RepetitionPeriod */
        
        0,  /* NumRepetitions */
#ifdef COM_MIXEDPHASESHIFT
        COM_TXMODE_PERIODIC, /* Mode */
        COM_FALSE    /* MixedPhaseShift status */
#else
        COM_TXMODE_PERIODIC /* Mode */
#endif /* #ifdef COM_MIXEDPHASESHIFT */
    }
,
    /* False Mode configuration */
    {
        10, /* TimePeriod */
        
        1, /* TimeOffset */
        
        0, /* RepetitionPeriod */
        
        0,  /* NumRepetitions */

#ifdef COM_MIXEDPHASESHIFT
        COM_TXMODE_PERIODIC, /* Mode */
        COM_FALSE    /* MixedPhaseShift status */
#else
        COM_TXMODE_PERIODIC /* Mode */
#endif /* #ifdef COM_MIXEDPHASESHIFT */

    }
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"



/* END: TMS Details  */






/* START: Tx IPDU Details  */

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_Prv_xTxIpduInfoCfg_tst Com_Prv_xTxIpduCfg_acst[COM_NUM_TX_IPDU] =
{
    {   /*Ipdu: IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx*/

        Com_dIP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_TxByte,              /*Pointer to the Ipdu Buffer*/

        Com_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx_TransModeInfo_acst,
        

        #ifdef COM_TxIPduCallOuts
        /* Ipdu Callout Function*/
        NULL_PTR,
        #endif
        #ifdef COM_TxIPduNotification
        /* Ipdu Notification Function*/

        NULL_PTR,
        #endif

        #ifdef COM_ERRORNOTIFICATION
        /* Error Notification part */

        NULL_PTR,
        #endif

        #ifdef COM_TxIPduTimeOutNotify
        /* Timeout Notification part*/

        NULL_PTR,
        #endif


        #ifdef COM_METADATA_SUPPORT
        NULL_PTR,               /* Pointer to the MetaData Ipdu Buffer */
        #endif

        6                                      /*Size in Bytes*/,

        #ifdef COM_TxIPduTimeOut
        1,               /*Timeout Fact*/
        #endif
        0, /*MinDelay Time factor*/

        10,                    /*No Of Signals present in the IPDU*/
        #ifdef COM_TX_SIGNALGROUP

        0,               /*No of Signal Groups present in the IPDU*/
        #endif

        PduRConf_PduRSrcPdu_ComTx_0x55_Com2PduR_Can_Network_ETAS_Channel_CAN,              /* PduR Id */


        (Com_TxIntSignalId_tuo)ComSignal_Internal_S_ETAS03_Sig10_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx,     /*Index to First Signal within this Ipdu*/
        #ifdef COM_TX_SIGNALGROUP

        0,                            /*This IPDU does not contain any Signal Groups*/
        #endif

        /*
        {
            Signal_Processing:1;          = IMMEDIATE
            TMSCalclation:2;              = MODE_VALID
            NoneModeTimeOut:1;            = COM_FALSE
            ClearUpdBit:2                 = CLEAR_UPDATEBIT_NOT_APPLICABLE
            FilterEvalReq:1               = true
            IsDynPdu:1;                   = false
            IsGwDestPdu:1;                = false
            IsCalloutFrmTrigTrans:1;      = false
            isLargeDataPdu:1;             = false
            isCancelTransmitSupported:1;  = false
            ipduPartOfIpduGrp:1;          = true
            defaultTMSStatus:1;           = false
            Is_MetaDataPdu:1;             = false
            Not_Used:1;
        }Com_TxIpduFlagType;
        */
        0x1040,  /*Transmission Type*/


#ifdef COM_TX_IPDUCOUNTER
        COM_TXIPDU_CNTR_INV_IDX,      /* Index to TxIPduCounter */
#endif

        /* Com_MainFunctionTx() */
        ComMainFunction_Internal_MainFunctionTx,

        0xFF               /*Padding Byte*/

    },
    {   /*Ipdu: IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx*/

        Com_dIP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_TxByte,              /*Pointer to the Ipdu Buffer*/

        Com_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx_TransModeInfo_acst,
        

        #ifdef COM_TxIPduCallOuts
        /* Ipdu Callout Function*/
        NULL_PTR,
        #endif
        #ifdef COM_TxIPduNotification
        /* Ipdu Notification Function*/

        NULL_PTR,
        #endif

        #ifdef COM_ERRORNOTIFICATION
        /* Error Notification part */

        NULL_PTR,
        #endif

        #ifdef COM_TxIPduTimeOutNotify
        /* Timeout Notification part*/

        NULL_PTR,
        #endif


        #ifdef COM_METADATA_SUPPORT
        NULL_PTR,               /* Pointer to the MetaData Ipdu Buffer */
        #endif

        7                                      /*Size in Bytes*/,

        #ifdef COM_TxIPduTimeOut
        5,               /*Timeout Fact*/
        #endif
        0, /*MinDelay Time factor*/

        6,                    /*No Of Signals present in the IPDU*/
        #ifdef COM_TX_SIGNALGROUP

        0,               /*No of Signal Groups present in the IPDU*/
        #endif

        PduRConf_PduRSrcPdu_ComTx_0x98_Com2PduR_Can_Network_ETAS_Channel_CAN,              /* PduR Id */


        (Com_TxIntSignalId_tuo)ComSignal_Internal_S_ETAS04_Sig20_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx,     /*Index to First Signal within this Ipdu*/
        #ifdef COM_TX_SIGNALGROUP

        0,                            /*This IPDU does not contain any Signal Groups*/
        #endif

        /*
        {
            Signal_Processing:1;          = IMMEDIATE
            TMSCalclation:2;              = MODE_VALID
            NoneModeTimeOut:1;            = COM_FALSE
            ClearUpdBit:2                 = CLEAR_UPDATEBIT_NOT_APPLICABLE
            FilterEvalReq:1               = true
            IsDynPdu:1;                   = false
            IsGwDestPdu:1;                = false
            IsCalloutFrmTrigTrans:1;      = false
            isLargeDataPdu:1;             = false
            isCancelTransmitSupported:1;  = false
            ipduPartOfIpduGrp:1;          = true
            defaultTMSStatus:1;           = false
            Is_MetaDataPdu:1;             = false
            Not_Used:1;
        }Com_TxIpduFlagType;
        */
        0x1040,  /*Transmission Type*/


#ifdef COM_TX_IPDUCOUNTER
        COM_TXIPDU_CNTR_INV_IDX,      /* Index to TxIPduCounter */
#endif

        /* Com_MainFunctionTx() */
        ComMainFunction_Internal_MainFunctionTx,

        0xFF               /*Padding Byte*/

    },
    {   /*Ipdu: IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx*/

        Com_dIP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_TxByte,              /*Pointer to the Ipdu Buffer*/

        Com_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx_TransModeInfo_acst,
        

        #ifdef COM_TxIPduCallOuts
        /* Ipdu Callout Function*/
        NULL_PTR,
        #endif
        #ifdef COM_TxIPduNotification
        /* Ipdu Notification Function*/

        NULL_PTR,
        #endif

        #ifdef COM_ERRORNOTIFICATION
        /* Error Notification part */

        NULL_PTR,
        #endif

        #ifdef COM_TxIPduTimeOutNotify
        /* Timeout Notification part*/

        NULL_PTR,
        #endif


        #ifdef COM_METADATA_SUPPORT
        NULL_PTR,               /* Pointer to the MetaData Ipdu Buffer */
        #endif

        8                                      /*Size in Bytes*/,

        #ifdef COM_TxIPduTimeOut
        10,               /*Timeout Fact*/
        #endif
        0, /*MinDelay Time factor*/

        8,                    /*No Of Signals present in the IPDU*/
        #ifdef COM_TX_SIGNALGROUP

        0,               /*No of Signal Groups present in the IPDU*/
        #endif

        PduRConf_PduRSrcPdu_ComTx_0x123_Com2PduR_Can_Network_ETAS_Channel_CAN,              /* PduR Id */


        (Com_TxIntSignalId_tuo)ComSignal_Internal_S_ETAS05_Sig26_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx,     /*Index to First Signal within this Ipdu*/
        #ifdef COM_TX_SIGNALGROUP

        0,                            /*This IPDU does not contain any Signal Groups*/
        #endif

        /*
        {
            Signal_Processing:1;          = IMMEDIATE
            TMSCalclation:2;              = MODE_VALID
            NoneModeTimeOut:1;            = COM_FALSE
            ClearUpdBit:2                 = CLEAR_UPDATEBIT_NOT_APPLICABLE
            FilterEvalReq:1               = true
            IsDynPdu:1;                   = false
            IsGwDestPdu:1;                = false
            IsCalloutFrmTrigTrans:1;      = false
            isLargeDataPdu:1;             = false
            isCancelTransmitSupported:1;  = false
            ipduPartOfIpduGrp:1;          = true
            defaultTMSStatus:1;           = false
            Is_MetaDataPdu:1;             = false
            Not_Used:1;
        }Com_TxIpduFlagType;
        */
        0x1040,  /*Transmission Type*/


#ifdef COM_TX_IPDUCOUNTER
        COM_TXIPDU_CNTR_INV_IDX,      /* Index to TxIPduCounter */
#endif

        /* Com_MainFunctionTx() */
        ComMainFunction_Internal_MainFunctionTx,

        0xFF               /*Padding Byte*/

    },
    {   /*Ipdu: IP_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_Tx*/

        Com_dIP_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_TxByte,              /*Pointer to the Ipdu Buffer*/

        &Com_NONE_TransModeInfo_cst,

        #ifdef COM_TxIPduCallOuts
        /* Ipdu Callout Function*/
        NULL_PTR,
        #endif
        #ifdef COM_TxIPduNotification
        /* Ipdu Notification Function*/

        NULL_PTR,
        #endif

        #ifdef COM_ERRORNOTIFICATION
        /* Error Notification part */

        NULL_PTR,
        #endif

        #ifdef COM_TxIPduTimeOutNotify
        /* Timeout Notification part*/

        NULL_PTR,
        #endif


        #ifdef COM_METADATA_SUPPORT
        NULL_PTR,               /* Pointer to the MetaData Ipdu Buffer */
        #endif

        6                                      /*Size in Bytes*/,

        #ifdef COM_TxIPduTimeOut
        0,               /*Timeout Fact*/
        #endif
        0, /*MinDelay Time factor*/

        1,                    /*No Of Signals present in the IPDU*/
        #ifdef COM_TX_SIGNALGROUP

        0,               /*No of Signal Groups present in the IPDU*/
        #endif

        PduRConf_PduRSrcPdu_NM_Tx_0x600_Com2PduR_Can_Network_ETAS_Channel_CAN,              /* PduR Id */


        (Com_TxIntSignalId_tuo)ComSignal_Internal_S_NM_Tx_Sig_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_Tx,     /*Index to First Signal within this Ipdu*/
        #ifdef COM_TX_SIGNALGROUP

        0,                            /*This IPDU does not contain any Signal Groups*/
        #endif

        /*
        {
            Signal_Processing:1;          = IMMEDIATE
            TMSCalclation:2;              = MODE_INVALID
            NoneModeTimeOut:1;            = COM_FALSE
            ClearUpdBit:2                 = CLEAR_UPDATEBIT_NOT_APPLICABLE
            FilterEvalReq:1               = false
            IsDynPdu:1;                   = false
            IsGwDestPdu:1;                = false
            IsCalloutFrmTrigTrans:1;      = false
            isLargeDataPdu:1;             = false
            isCancelTransmitSupported:1;  = false
            ipduPartOfIpduGrp:1;          = true
            defaultTMSStatus:1;           = false
            Is_MetaDataPdu:1;             = false
            Not_Used:1;
        }Com_TxIpduFlagType;
        */
        0x1006,  /*Transmission Type*/


#ifdef COM_TX_IPDUCOUNTER
        COM_TXIPDU_CNTR_INV_IDX,      /* Index to TxIPduCounter */
#endif

        /* Com_MainFunctionTx() */
        ComMainFunction_Internal_MainFunctionTx,

        0xFF               /*Padding Byte*/

    }
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

/* END: Tx IPDU Details  */

/* START : Time out information structure for signals with update-bits */
#ifdef COM_RxSigUpdateTimeout
#endif /* #ifdef COM_RxSigUpdateTimeout */
/* END : Time out information structure for signals with update-bits */

/* START : Time out information structure for signal groups with update-bits */
#ifdef COM_RxSigGrpUpdateTimeout
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */
/* END : Time out information structure for signal groups with update-bits */



/* START: Rx IPDU Details  */

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"


const Com_Prv_xRxIpduInfoCfg_tst Com_Prv_xRxIpduCfg_acst[COM_NUM_RX_IPDU] =
{
    {   /*Ipdu: IP_ComRx_0x261_Can_Network_ETAS_Channel_CAN_Rx*/
        

        Com_dIP_ComRx_0x261_Can_Network_ETAS_Channel_CAN_RxByte,              /*Pointer to the Local Ipdu Buffer*/
        
        #ifdef COM_RxSigUpdateTimeout

        NULL_PTR,                       /* Pointer to timeout information structure for signals with update-bits, within the IPdu */
        #endif /* #ifdef COM_RxSigUpdateTimeout */
        #ifdef COM_RxSigGrpUpdateTimeout

        NULL_PTR,                       /* Pointer to timeout information structure for signal groups with update-bits, within the IPdu */
        #endif /* #ifdef COM_RxSigGrpUpdateTimeout */

        #ifdef COM_RxIPduCallOuts
        /* Ipdu Callout */

        NULL_PTR,
        #endif

        #ifdef COM_RxIPduTimeoutNotify
        /* Timeout Notification part*/

        NULL_PTR,
        #endif /* COM_RxIPduTimeoutNotify */

        #ifdef COM_RxIPduNotification
        /* Ipdu Notification Function*/

        NULL_PTR,  /* Rx IPdu notification callback */
        #endif

        #ifdef COM_METADATA_SUPPORT
        NULL_PTR,               /* Pointer to the MetaData Rx Ipdu Buffer */
        #endif

        1                                  /*Size in Bytes*/,
        #ifdef COM_RxIPduTimeout

        0,        /* First time out value after IPDU group start */

        0,              /* Support Rx IPDU Timeout */
        #endif /* #ifdef COM_RxIPduTimeout */

        1,                /*No Of Signals present in the IPDU*/

        #ifdef COM_RX_SIGNALGROUP

        0,           /*No of Signal Groups present in the IPDU*/
        #endif

        (Com_RxIntSignalId_tuo)ComSignal_Internal_S_ETAS02_Sig09_ComRx_0x261_Can_Network_ETAS_Channel_CAN_Rx,   /*Index to First Signal within this Ipdu*/
         #ifdef COM_RX_SIGNALGROUP
        0,                            /*This IPDU does not contain any Signal Groups*/
        #endif
#ifdef COM_SIGNALGATEWAY
        0,       /* Number of signals with gateway */
#endif
#ifdef COM_SIGNALGROUPGATEWAY
        0,   /* Number of signal groups with gateway */
#endif

#ifdef COM_RX_IPDUCOUNTER
        COM_RXIPDU_CNTR_INV_IDX,      /* Index to RxIPduCounter */
#endif

        /* Com_MainFunctionRx() */
        ComMainFunction_Internal_MainFunctionRx,

        /*
        {
            Signal_Processing:1;  = IMMEDIATE
            Notify_Cbk:1;         = true
            IsGwIPdu:1;           = false
            ipduPartOfIpduGrp:1;  = true
            IS_TP_TYPE:1;         = false
            TP_INV_CFG:1;         = false
            Is_MetaDataPdu:1;     = false
            Not_Used:1;
        } Com_RxIpduFlagType;
        */
        0xA                 /* Reception Type */


    },
    {   /*Ipdu: IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx*/
        

        Com_dIP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_RxByte,              /*Pointer to the Local Ipdu Buffer*/
        
        #ifdef COM_RxSigUpdateTimeout

        NULL_PTR,                       /* Pointer to timeout information structure for signals with update-bits, within the IPdu */
        #endif /* #ifdef COM_RxSigUpdateTimeout */
        #ifdef COM_RxSigGrpUpdateTimeout

        NULL_PTR,                       /* Pointer to timeout information structure for signal groups with update-bits, within the IPdu */
        #endif /* #ifdef COM_RxSigGrpUpdateTimeout */

        #ifdef COM_RxIPduCallOuts
        /* Ipdu Callout */

        NULL_PTR,
        #endif

        #ifdef COM_RxIPduTimeoutNotify
        /* Timeout Notification part*/

        NULL_PTR,
        #endif /* COM_RxIPduTimeoutNotify */

        #ifdef COM_RxIPduNotification
        /* Ipdu Notification Function*/

        NULL_PTR,  /* Rx IPdu notification callback */
        #endif

        #ifdef COM_METADATA_SUPPORT
        NULL_PTR,               /* Pointer to the MetaData Rx Ipdu Buffer */
        #endif

        8                                  /*Size in Bytes*/,
        #ifdef COM_RxIPduTimeout

        0,        /* First time out value after IPDU group start */

        0,              /* Support Rx IPDU Timeout */
        #endif /* #ifdef COM_RxIPduTimeout */

        8,                /*No Of Signals present in the IPDU*/

        #ifdef COM_RX_SIGNALGROUP

        0,           /*No of Signal Groups present in the IPDU*/
        #endif

        (Com_RxIntSignalId_tuo)ComSignal_Internal_S_ETAS06_Sig34_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,   /*Index to First Signal within this Ipdu*/
         #ifdef COM_RX_SIGNALGROUP
        0,                            /*This IPDU does not contain any Signal Groups*/
        #endif
#ifdef COM_SIGNALGATEWAY
        0,       /* Number of signals with gateway */
#endif
#ifdef COM_SIGNALGROUPGATEWAY
        0,   /* Number of signal groups with gateway */
#endif

#ifdef COM_RX_IPDUCOUNTER
        COM_RXIPDU_CNTR_INV_IDX,      /* Index to RxIPduCounter */
#endif

        /* Com_MainFunctionRx() */
        ComMainFunction_Internal_MainFunctionRx,

        /*
        {
            Signal_Processing:1;  = IMMEDIATE
            Notify_Cbk:1;         = true
            IsGwIPdu:1;           = false
            ipduPartOfIpduGrp:1;  = true
            IS_TP_TYPE:1;         = false
            TP_INV_CFG:1;         = false
            Is_MetaDataPdu:1;     = false
            Not_Used:1;
        } Com_RxIpduFlagType;
        */
        0xA                 /* Reception Type */


    },
    {   /*Ipdu: IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx*/
        

        Com_dIP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_RxByte,              /*Pointer to the Local Ipdu Buffer*/
        
        #ifdef COM_RxSigUpdateTimeout

        NULL_PTR,                       /* Pointer to timeout information structure for signals with update-bits, within the IPdu */
        #endif /* #ifdef COM_RxSigUpdateTimeout */
        #ifdef COM_RxSigGrpUpdateTimeout

        NULL_PTR,                       /* Pointer to timeout information structure for signal groups with update-bits, within the IPdu */
        #endif /* #ifdef COM_RxSigGrpUpdateTimeout */

        #ifdef COM_RxIPduCallOuts
        /* Ipdu Callout */

        NULL_PTR,
        #endif

        #ifdef COM_RxIPduTimeoutNotify
        /* Timeout Notification part*/

        NULL_PTR,
        #endif /* COM_RxIPduTimeoutNotify */

        #ifdef COM_RxIPduNotification
        /* Ipdu Notification Function*/

        NULL_PTR,  /* Rx IPdu notification callback */
        #endif

        #ifdef COM_METADATA_SUPPORT
        NULL_PTR,               /* Pointer to the MetaData Rx Ipdu Buffer */
        #endif

        8                                  /*Size in Bytes*/,
        #ifdef COM_RxIPduTimeout

        0,        /* First time out value after IPDU group start */

        0,              /* Support Rx IPDU Timeout */
        #endif /* #ifdef COM_RxIPduTimeout */

        9,                /*No Of Signals present in the IPDU*/

        #ifdef COM_RX_SIGNALGROUP

        0,           /*No of Signal Groups present in the IPDU*/
        #endif

        (Com_RxIntSignalId_tuo)ComSignal_Internal_S_ETAS01_Sig00_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,   /*Index to First Signal within this Ipdu*/
         #ifdef COM_RX_SIGNALGROUP
        0,                            /*This IPDU does not contain any Signal Groups*/
        #endif
#ifdef COM_SIGNALGATEWAY
        0,       /* Number of signals with gateway */
#endif
#ifdef COM_SIGNALGROUPGATEWAY
        0,   /* Number of signal groups with gateway */
#endif

#ifdef COM_RX_IPDUCOUNTER
        COM_RXIPDU_CNTR_INV_IDX,      /* Index to RxIPduCounter */
#endif

        /* Com_MainFunctionRx() */
        ComMainFunction_Internal_MainFunctionRx,

        /*
        {
            Signal_Processing:1;  = IMMEDIATE
            Notify_Cbk:1;         = true
            IsGwIPdu:1;           = false
            ipduPartOfIpduGrp:1;  = true
            IS_TP_TYPE:1;         = false
            TP_INV_CFG:1;         = false
            Is_MetaDataPdu:1;     = false
            Not_Used:1;
        } Com_RxIpduFlagType;
        */
        0xA                 /* Reception Type */


    }
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

/* END: Rx IPDU Details  */



/* START: IPDU Group Details  */

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_Prv_xIpduGrpInfoCfg_tst Com_Prv_xIpduGrpCfg_acst[2] =
{
    /* "Index of First IPdu"               "No of Rx-Ipdus" */

    /* ComIPduGroup_Rx */
    { 0, 3 },
    /* ComIPduGroup_Tx */
    { 3, 0 }

};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

/* END: IPDU Group Details  */

/* Reference to Ipdus belonging to the Ipdu Groups */

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"


const Com_IpduId_tuo Com_Prv_xIPduGrp_IpduRefCfg_acuo[7] =
{

    /* ComIPduGroup_Rx */

    ComIPdu_Internal_IP_ComRx_0x261_Can_Network_ETAS_Channel_CAN_Rx,
    ComIPdu_Internal_IP_ComRx_0x456_Can_Network_ETAS_Channel_CAN_Rx,
    ComIPdu_Internal_IP_ComRx_0x81_Can_Network_ETAS_Channel_CAN_Rx,
    /* ComIPduGroup_Tx */

    (COM_NUM_RX_IPDU + (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x123_Can_Network_ETAS_Channel_CAN_Tx),
    (COM_NUM_RX_IPDU + (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx),
    (COM_NUM_RX_IPDU + (Com_IpduId_tuo)ComIPdu_Internal_IP_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx),
    (COM_NUM_RX_IPDU + (Com_IpduId_tuo)ComIPdu_Internal_IP_NM_Tx_0x600_Can_Network_ETAS_Channel_CAN_Tx)
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"








#ifdef COM_F_ONEEVERYN

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
const Com_OneEveryN_tst Com_OneEveryN_Const_acst[1] =
{
    /* Period   Offset  Occurence*/


    {    1,    5, 1    }    /*  DummyForMisra    */
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#endif /* #ifdef COM_F_ONEEVERYN */


#if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX)

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_MaskX_tst Com_MaskX_acst[1] =
{
    /*Mask       X*/



    {    1,    5    }    /*  DummyForMisra    */
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"


#endif /* #if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX) */

#ifdef COM_F_MASKEDNEWDIFFERSOLD

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_32
#include "Com_MemMap.h"

const uint32 Com_Mask_acu32[1] =
{
    /*Mask*/
    1    /* DummyForMisra */
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_32
#include "Com_MemMap.h"

#endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */

#if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS)

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_POSMinMax_tst Com_POSMinMax_acst[8]=
{
    /*  Min      Max */



    /* Signals */
    
    {    0,    8189    }    /*  S_ETAS03_Sig13_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx    */

    ,
    {    0,    65534    }    /*  S_ETAS03_Sig15_ComTx_0x55_Can_Network_ETAS_Channel_CAN_Tx    */

    ,
    {    0,    1022    }    /*  S_ETAS04_Sig20_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx    */

    ,
    {    0,    254    }    /*  S_ETAS04_Sig21_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx    */

    ,
    {    0,    2046    }    /*  S_ETAS04_Sig22_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx    */

    ,
    {    0,    510    }    /*  S_ETAS04_Sig23_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx    */

    ,
    {    0,    254    }    /*  S_ETAS04_Sig24_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx    */

    ,
    {    0,    1022    }    /*  S_ETAS04_Sig25_ComTx_0x98_Can_Network_ETAS_Channel_CAN_Tx    */

    





};


/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#endif /* #if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS) */

#if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG)

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_NEGMinMax_tst Com_NEGMinMax_acst[1] =
{
    /*  Min      Max */


    {    1,    5    }    /*  DummyForMisra    */
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#endif /* #if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG) */


/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

const Com_MainFunctionCfgType_tst Com_MainFunctionCfg_acst[ COM_NUM_OF_MAINFUNCTION ] =
{
    {
        /* Com_MainFunctionRx() - cylce time Rx: 0.01 s */
        0u,  /* Start RxIPdu-Id */
        3u,  /* Num of RxIpdus */
        10u /* TimeBase in ms */
    }
    ,{
        /* Com_MainFunctionTx() - cylce time Tx: 0.01 s */
        0u,  /* Start TxIPdu-Id */
        4u,  /* Num of TxIpdus */
        10u /* TimeBase in ms */
    }
};

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"





