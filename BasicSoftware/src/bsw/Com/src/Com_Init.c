

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_Init_Receive(void);

LOCAL_INLINE void Com_Prv_Init_RxSignal(PduIdType idIpdu_uo);

#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE void Com_Prv_Init_RxSignalGroup(PduIdType idIpdu_uo);
#endif

LOCAL_INLINE void Com_Prv_Init_Send(void);

LOCAL_INLINE void Com_Prv_Init_TxSignal(PduIdType idIpdu_uo);

#ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE void Com_Prv_Init_TxSignalGroup(PduIdType idIpdu_uo);

LOCAL_INLINE void Com_Prv_Init_GroupSignal(
                                    Com_TxIpduCfg_tpcst     txIpduConstPtr_pcst,
                                    Com_TxSigGrpCfg_tpcst   txSigGrpConstPtr_pcst
                                          );
#endif /* COM_TX_SIGNALGROUP */

#ifdef COM_IPDU_WITHOUT_IPDUGROUP_EXISTS
LOCAL_INLINE void Com_Prv_Init_RxIpduWoGroupRef(PduIdType idIpdu_uo);
LOCAL_INLINE void Com_Prv_Init_TxIpduWoGroupRef(PduIdType idIpdu_uo);
#endif /* COM_IPDU_WITHOUT_IPDUGROUP_EXISTS */

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"

Com_StatusType  Com_InitStatus_en;

#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)

/* Note: pointer variable should always be under Pre-Compile MemMap section,
 * or else it can lead to RAM corruption */
const Com_ConfigData_tst *      Com_Prv_Config_pcst;
const Com_CommonData_tst *      Com_Prv_Common_pcst;
const Com_ConfigType *          Com_Variant_pcst;

#endif

#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 Function name    : Com_Init
 Description      : Service for initializing COM module
 Parameter        : config -> Pointer to Com module's configuration data
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

void Com_Init(const Com_ConfigType * config_pcst)
{

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
    Com_RxGwQueueRam_tpst       rxGwQueuePtr_pst;
#endif
#ifdef COM_F_ONEEVERYN
    uint16_least                index_qu16;
#endif

#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)

    if (config_pcst == NULL_PTR)
    {
        /* Repot to DET in case input configuration pointer is not valid */
        COM_DET_REPORT_ERROR(COMServiceId_Init, COM_E_INIT_FAILED);
        return;
    }
    else
    {
        Com_Variant_pcst = config_pcst;

        /* MR12 RULE 11.5 VIOLATION: On assignment, target pointer will reference to the memory address, and there are
         * no side effects. Hence suppressed the warning. */
        Com_Prv_Config_pcst = (const Com_ConfigData_tst * )Com_Variant_pcst->configData_pcv;
        Com_Prv_Common_pcst = (const Com_CommonData_tst * )Com_Variant_pcst->commonData_pcv;

    }
#else
    (void)config_pcst;
#endif /* (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
    rxGwQueuePtr_pst                    = COM_GET_RXGWQUEUE_S;
    rxGwQueuePtr_pst->rxGwQueueRead_uo  = COM_ZERO;
    rxGwQueuePtr_pst->rxGwQueueWrite_uo = COM_ZERO;
    rxGwQueuePtr_pst->rxGwQueuePtr_puo  = &COM_GET_RXGWQUEUE(0);
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

    /* clear IpduGroupVector's */
    rba_BswSrv_MemSet8(Com_IpduGrpVector_au8, COM_ZERO, COM_NUM_ARR_IPDUGRPVECT);

#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
    rba_BswSrv_MemSet8(Com_IpduGrpVector_DM_au8, COM_ZERO, COM_NUM_ARR_IPDUGRPVECT);
#endif

/* FC_VariationPoint_START */
#if (COM_COMMON_TX_PERIOD == STD_ON)
    /* Reset common IPDU Tx period feature, if active */
    Com_ReducedPeriod_u16 = COM_ZERO;
#endif /* COM_COMMON_TX_PERIOD */
/* FC_VariationPoint_END */

#ifdef COM_F_ONEEVERYN
    /* Initialize RAM structure Com_OneEveryN */
    for (index_qu16 = COM_ZERO; index_qu16 < COM_GET_NUM_SIG_GRPSIG_WITH_ONE_EVERY_N_FILTER; index_qu16++)
    {
        COM_GET_ONEEVERYN( index_qu16 ).period_u32      = COM_GET_ONEEVERYN_CONST( index_qu16 ).period_u32;
        COM_GET_ONEEVERYN( index_qu16 ).offset_u32      = COM_GET_ONEEVERYN_CONST( index_qu16 ).offset_u32;
        COM_GET_ONEEVERYN( index_qu16 ).occurrence_u8   = COM_GET_ONEEVERYN_CONST( index_qu16 ).occurrence_u8;
    }
#endif /* #ifdef COM_F_ONEEVERYN */

    /* Initialize configured Receive Signals, Signal Groups */
    Com_Prv_Init_Receive();

    /* Initialize configured transmission Signals, Signal Groups */
    Com_Prv_Init_Send();

/* FC_VariationPoint_START */
#ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
    /* Initial value for all the TxIpdus should be set for IpduControl Via Rba_Nds_EcuVariant feature */
    rba_BswSrv_MemSet8(Com_TxIpduControlVector_au8, COM_MAX_U8_VALUE, COM_NUM_ARR_TXIPDUCONTROLVECTOR);
#endif

#ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
    /* Initial value for all the RxIpdus should be set for IpduControl Via Rba_Nds_EcuVariant feature */
    rba_BswSrv_MemSet8(Com_RxIpduControlVector_au8, COM_MAX_U8_VALUE, COM_NUM_ARR_RXIPDUCONTROLVECTOR);
#endif
/* FC_VariationPoint_END */

    /* Set Com module initialization status as complete. */
    Com_InitStatus_en = COM_INIT;
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_Init_Receive
 Description      : Service to initialize all configured signals, signalGroup at the receiver side
 Parameter        : None
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_Init_Receive(void)
{
    Com_RxIpduRam_tpst          rxIpduRamPtr_pst;
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    uint16_least                idxIdIpdu_qu16;

    /* Start: Reset all Rx Flags */
    rxIpduRamPtr_pst    = &COM_GET_RXPDURAM_S(0);

    rxIpduConstPtr_pcst = COM_GET_RX_IPDU_CONSTDATA(0);

    for(idxIdIpdu_qu16 = COM_ZERO; idxIdIpdu_qu16 < COM_GET_NUM_RX_IPDU; idxIdIpdu_qu16++)
    {
        /*Reset all the Rx Ipdu flags*/
        rxIpduRamPtr_pst->rxFlags_u8      = COM_ZERO;
        rxIpduRamPtr_pst->rxIPduLength_uo = COM_ZERO;

#if defined(COM_RxIPduTimeout) && defined(COM_RX_DUAL_CYCLE_SUPPPORT)
        rxIpduRamPtr_pst->rxDualCycTimeout_u16 = COM_ZERO;
#endif

#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
        rxIpduRamPtr_pst->dynRxGrpSigLength_u16 = COM_ZERO;
#endif

#ifdef COM_IPDU_WITHOUT_IPDUGROUP_EXISTS
        if (Com_GetValue(RXIPDU,_PDUWITHPDUGRP,rxIpduConstPtr_pcst->rxIPduFields_u8 ) == COM_FALSE)
        {
            /* Initialize Ipdu which does not belongs to any IpduGroup */
            Com_Prv_Init_RxIpduWoGroupRef((PduIdType)idxIdIpdu_qu16);
        }
        else
#endif /* #ifdef COM_IPDU_WITHOUT_IPDUGROUP_EXISTS */
        {
            COM_GET_IPDUCOUNTER_S(idxIdIpdu_qu16)  = COM_ZERO;

# if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
            COM_GET_IPDUCOUNTER_DM(idxIdIpdu_qu16) = COM_ZERO;
# endif
        }

        /* Call function to initialize configured reception signals */
        Com_Prv_Init_RxSignal((PduIdType)idxIdIpdu_qu16);

#ifdef COM_RX_SIGNALGROUP
        /* Call function to initialize configured reception signalgroup */
        Com_Prv_Init_RxSignalGroup((PduIdType)idxIdIpdu_qu16);
#endif  /* #ifdef COM_RX_SIGNALGROUP */

        rxIpduRamPtr_pst++;
        rxIpduConstPtr_pcst++;
    }
    /* End: Reset all Rx Falgs */
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_Init_RxSignal
 Description      : Service to initialize all configured signal at the receiver side
 Parameter        : idIpdu_uo  -> Rx Ipdu
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_Init_RxSignal(PduIdType idIpdu_uo)
{
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    Com_RxSigCfg_tpcst          rxSigConstPtr_pcst;
    Com_RxSigRam_tpst           rxSigRamPtr_pst;
    uint16_least                idxSig_qu16;
    Com_SigMax_tuo              rxSigVal_uo;
    Com_MainFunc_tuo            idRxMainFunc_uo;

#if defined( COM_RXSIG_INT64 ) || defined( COM_RXSIG_FLOAT64SUPP )
    uint8                       type_u8;
#endif

    rxIpduConstPtr_pcst     = COM_GET_RX_IPDU_CONSTDATA(idIpdu_uo);

    rxSigConstPtr_pcst      = COM_GET_RXSIG_CONSTDATA(rxIpduConstPtr_pcst->idRxSig_uo);
    rxSigRamPtr_pst         = &COM_GET_RXSIGNALFLAG(rxIpduConstPtr_pcst->idRxSig_uo);

    /* Fetch the Rx-MainFunction internal Id */
    idRxMainFunc_uo    = rxIpduConstPtr_pcst->idMainFunc_uo;

    /* AUTOSAR COM shall initialize all signals on receiver side with the configured init values */
    for (idxSig_qu16 = rxIpduConstPtr_pcst->numOfSig_u16; idxSig_qu16 != COM_ZERO; idxSig_qu16--)
    {
        /* Reset all the flags in the rxSigRAMFields */
        rxSigRamPtr_pst->rxSigRAMFields_u8 = COM_ZERO;
        /* Get the init value of the signal */
        /* Sign extension to be performed on Init value
         * But when a signed value is assigned to a uint32 variable,it is sign extended by default */
        rxSigVal_uo=Com_Prv_GetRxSigInitValue(rxSigConstPtr_pcst);

#if defined( COM_RXSIG_INT64 ) || defined( COM_RXSIG_FLOAT64SUPP )
        /* Get the signal type_u8 */
        type_u8        = Com_GetValue(GEN,_TYPE,rxSigConstPtr_pcst->general_u8);
#endif /* #if defined( COM_RXSIG_INT64 ) || defined( COM_RXSIG_FLOAT64SUPP ) */

/* FC_VariationPoint_START */
#ifdef COM_RXSIG_FLOAT64SUPP

        if(type_u8 == COM_FLOAT64)
        {
            /* For float64 signals, rxSigConstPtr_pcst->Init_Val holds index to float array holding init values.*/
            COM_GET_SIGTYPEFLOAT64_BUFF(idRxMainFunc_uo,rxSigConstPtr_pcst->idxSigBuff_uo) =
                                                     Com_getfloat64InitValue(rxSigVal_uo);
        }
        else
#endif /* #ifdef COM_RXSIG_FLOAT64SUPP */
/* FC_VariationPoint_END */
        {
#ifdef COM_RXSIG_INT64
            /* TRACE[SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue) of a signal
             * to the size of its ComSignalType.
             * Init_Val by default will be sign extended only for 32bits. Currently for COM_SINT64 signal types,
             * if the signal is of Negative, it is required to perform the sign extension till 64bits explicitly. */
            if ( type_u8 == COM_SINT64 )
            {
                rxSigVal_uo = (Com_SigMax_tuo)((sint64)((sint32)rxSigVal_uo));
            }
            else
            {
                /* do nothing for other signal types */
            }
#endif /* #ifdef COM_RXSIG_INT64 */

            Com_Prv_UpdateRxSignalBuffer(rxSigConstPtr_pcst, rxSigVal_uo, idRxMainFunc_uo);
        }

#if defined(COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)

        /* Check the filter algorithm */
        if(Com_GetValue(RXSIG,_FILTRALG,rxSigConstPtr_pcst->rxSignalFields_u8) == (uint8)COM_MASKED_NEW_DIFFERS_MASKED_OLD)
        {
            /* TRACE[SWS_Com_00603] The AUTOSAR COM module shall set the old_value of the filtering mechanisms for
             * each signal to the ComSignalInitValue during start-up. */
            COM_GET_F_OLDVALUE(rxSigConstPtr_pcst->idxFilter_uo) = (uint32)rxSigVal_uo;
        }
#endif /* #if defined(COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD) */

        rxSigRamPtr_pst++;
        rxSigConstPtr_pcst++;
    }
}


#ifdef COM_RX_SIGNALGROUP

/*
 **********************************************************************************************************************
 Function name    : Com_Prv_Init_RxSignalGroup
 Description      : Service to initialize all configured signal group at the receiver side
 Parameter        : idIpdu_uo  -> Rx Ipdu
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_Init_RxSignalGroup(PduIdType idIpdu_uo)
{
    Com_RxIpduCfg_tpcst             rxIpduConstPtr_pcst;
    Com_RxSigGrpCfg_tpcst           rxSigGrpConstPtr_pcst;
    Com_RxSigGrpRam_tpst            rxSigGrpRamPtr_pst;
    uint16_least                    idxSigGrp_qu16;
    Com_MainFunc_tuo                idRxMainFunc_uo;

    rxIpduConstPtr_pcst     = COM_GET_RX_IPDU_CONSTDATA(idIpdu_uo);
    rxSigGrpConstPtr_pcst   = COM_GET_RXSIGGRP_CONSTDATA(rxIpduConstPtr_pcst->idFirstRxSigGrp_uo);
    rxSigGrpRamPtr_pst      = &COM_GET_RXSIGGRPFLAG(rxIpduConstPtr_pcst->idFirstRxSigGrp_uo);
    idRxMainFunc_uo         = rxIpduConstPtr_pcst->idMainFunc_uo;

    /* AUTOSAR COM shall initialize all Rx-SignalGroup Array(if any) and Secondary GroupSignal Buffers
     * on receiver side with the configured init values */
    for (idxSigGrp_qu16 = rxIpduConstPtr_pcst->numOfSigGrp_u16; idxSigGrp_qu16 != COM_ZERO; idxSigGrp_qu16--)
    {
        Com_Prv_SigGrpReplaceToInit( rxSigGrpConstPtr_pcst, idRxMainFunc_uo );

        rxSigGrpRamPtr_pst->rxSigGrpRAMFields_u8 = (uint8)COM_ZERO;

        rxSigGrpRamPtr_pst++;
        rxSigGrpConstPtr_pcst++;
    }
}

#endif /* #ifdef COM_RX_SIGNALGROUP */

/*
 **********************************************************************************************************************
 Function name    : Com_Prv_Init_Send
 Description      : Service to initialize all configured signals, signalGroup at the sender side
 Parameter        : None
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_Init_Send(void)
{
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    uint16_least                idxIdIpdu_qu16;

    txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(0);
    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(0);

    for (idxIdIpdu_qu16 = COM_ZERO; idxIdIpdu_qu16 < COM_GET_NUM_TX_IPDU; idxIdIpdu_qu16++)
    {
        /*Reset all the Tx Flags*/
        txIpduRamPtr_pst ->txFlags_u16      = COM_ZERO;
#if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT)
        txIpduRamPtr_pst->dynLength_u16     = COM_ZERO;
#endif
#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
        txIpduRamPtr_pst->dynTxGrpSigLength_u16 = COM_ZERO;
#endif
        txIpduRamPtr_pst->currentTxModePtr_pcst  = txIpduConstPtr_pcst->tMConstPtr_pcst;

        /*Update the IPDU buffer with Padding Bytes/Unused Area default*/
        rba_BswSrv_MemSet8(txIpduConstPtr_pcst->buffPtr_pu8, txIpduConstPtr_pcst->paddingByte_u8, COM_GET_TXIPDU_SIZE(idxIdIpdu_qu16));

#ifdef COM_IPDU_WITHOUT_IPDUGROUP_EXISTS
        /* Start: Reset all Tx Flags */
        if (Com_GetValue(TXIPDU,_PDUWITHPDUGRP,txIpduConstPtr_pcst->txIPduFields_u16) == COM_FALSE)
        {
            /* Call function to initialize Ipdu which does not belongs to any IpduGroup */
            Com_Prv_Init_TxIpduWoGroupRef((PduIdType)idxIdIpdu_qu16);
        }
        else
#endif /* #ifdef COM_IPDU_WITHOUT_IPDUGROUP_EXISTS */
        {
            COM_GET_IPDUCOUNTER_S(idxIdIpdu_qu16 + COM_GET_NUM_RX_IPDU) = COM_ZERO;

            /* Set the TxIPdu-Mode to most probable case */
            Com_SetRamValue(TXIPDU,_TXMODESTATE,txIpduRamPtr_pst->transMode_u8,COM_TRUE);

            Com_SetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8,(uint8)COM_TXMODE_NONE);
            /* FC_VariationPoint_START */
#ifdef COM_MIXEDPHASESHIFT
            /* Disable MixedPhaseShift by default */
            Com_SetRamValue(TXIPDU,_MIXEDPHASESHIFT,txIpduRamPtr_pst->transMode_u8,COM_FALSE);
#endif /* #ifdef COM_MIXEDPHASESHIFT */
            /* FC_VariationPoint_END */
        }

        /*Reset all other Tx Global variables including Min Delays*/
#ifdef COM_TxIPduTimeOut
        txIpduRamPtr_pst->cntrTxTimeout_u16         = COM_ZERO;
#endif /* #ifdef COM_TxIPduTimeOut */
#ifdef COM_TxFilters
        txIpduRamPtr_pst->cntrTrueTMC_u16           = COM_ZERO;
#endif /* #ifdef COM_TxFilters */
        txIpduRamPtr_pst->cntrRepetitions_u8        = COM_ZERO;

        txIpduRamPtr_pst->cntrMinDelayTime_u16      = COM_ZERO; /*Minimum Delay Tick for the Tx IPDU's */

        /* Call function to initialize configured transmission signals */
        Com_Prv_Init_TxSignal((PduIdType)idxIdIpdu_qu16);

#ifdef COM_TX_SIGNALGROUP
        /* Call function to initialize configured transmission signalgroup */
        Com_Prv_Init_TxSignalGroup((PduIdType)idxIdIpdu_qu16);
#endif

#ifdef COM_TX_IPDUCOUNTER
        /* Check if IPduCounter is configured */
        if (txIpduConstPtr_pcst->idxTxIpduCntr_uo != COM_TXIPDU_CNTR_INV_IDX)
        {
            /* TRACE[SWS_Com_00687] The AUTOSAR COM module shall set the I-PDU counter to 0, after the I-PDU was
             * initialized by Com_Init or reinitialized by Com_IpduGroupStart. */
            Com_Prv_TxIpduInitializeCounter( (PduIdType)idxIdIpdu_qu16 );
        }
        else
        {
            /* do nothing: counter is not configured */
        }
#endif /* #ifdef COM_TX_IPDUCOUNTER */

        txIpduConstPtr_pcst++;
        txIpduRamPtr_pst++;

    } /* end of for loop */
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_Init_TxSignal
 Description      : Service to initialize all configured signals at the sender side
 Parameter        : idIpdu_uo -> Tx Ipdu
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_Init_TxSignal(PduIdType idIpdu_uo)
{

    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
    Com_SigMax_tuo              txSigNewVal_uo;
    uint16_least                idxSig_qu16;
#if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) || defined(COM_EffectiveSigTOC)
    uint16                      constByteValue_u16;
#endif
#ifdef COM_EffectiveSigTOC
    uint16                      txSigTp_u16; /* Variable to hold the Transfer Property of Signal */
#endif
#ifdef COM_TxSigUpdateBit
    PduLengthType               byteNo_uo;
#endif
    uint8                       constByteValue_u8;
    uint8                       type_u8;

    txIpduConstPtr_pcst      = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);

    /* AUTOSAR COM shall initialize all signals on sender side with
    the configured init values */
    txSigConstPtr_pcst       = COM_GET_TXSIG_CONSTDATA(txIpduConstPtr_pcst->idTxSig_uo);

    for (idxSig_qu16 = txIpduConstPtr_pcst->numOfSig_u16; idxSig_qu16 != COM_ZERO; idxSig_qu16--)
    {
        constByteValue_u8    = txSigConstPtr_pcst->general_u8;
        /* Get the signal type */
        type_u8              = Com_GetValue(GEN,_TYPE,constByteValue_u8);

        /* Sign extension to be performed on Init value
        * But when a signed value is assigned to a uint32 variable,it is sign extended by default*/
        txSigNewVal_uo         = Com_Prv_GetTxSigInitValue(txSigConstPtr_pcst);

#ifdef COM_TXSIG_INT64
        /* TRACE[SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue) of a signal
        * to the size of its ComSignalType.
        * Currently for COM_SINT64 signal type, ComSignalInitValue is supported for maximum 32bits.
        * If negative values are configured, by default these values will be sign extended only till 32bits.
        * so, for COM_SINT64, it is required to perform the sign extension till 64bits explicitly. */
        if ( type_u8 == COM_SINT64 )
        {
            txSigNewVal_uo = (Com_SigMax_tuo)((sint64)((sint32)txSigNewVal_uo));
        }
        else
        {
            /* do nothing for other signal types */
        }
#endif /* #ifdef COM_TXSIG_INT64 */

        Com_Prv_InitializePduBuffWithSignalInitValue( txIpduConstPtr_pcst->buffPtr_pu8,
                                                      txSigNewVal_uo,
                                                      txSigConstPtr_pcst->bitPos_uo,
                                                      txSigConstPtr_pcst->bitSize_uo, type_u8,
                                                      Com_GetValue(GEN,_ENDIANESS,constByteValue_u8)  );

#if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) || defined(COM_EffectiveSigTOC)
        constByteValue_u16 = txSigConstPtr_pcst->txSignalFields_u16;
#endif

#if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD))
        if(Com_GetValue(TXSIG,_FILTRALG,constByteValue_u16) == (uint16)COM_MASKED_NEW_DIFFERS_MASKED_OLD)
        {
            /* TRACE[SWS_Com_00603] The AUTOSAR COM module shall set the old_value of the filtering mechanisms for
             * each signal to the ComSignalInitValue during start-up. */
            COM_GET_F_OLDVALUE(txSigConstPtr_pcst->idxFilter_uo) = (uint32)txSigNewVal_uo;
        }
#endif /* #if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) */

#ifdef COM_EffectiveSigTOC
        /* Get Signal Transfer Property */
        txSigTp_u16 = Com_GetValue(TXSIG,_TP,constByteValue_u16);

        /* Assign Init value as Old Value, if Signal's Transfer property is TriggeredOnChange or
         * TriggeredOnChangeWithoutRepetition */
        if (COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(txSigTp_u16))
        {
            COM_GET_OLDVALTRIGONCHANGE(txSigConstPtr_pcst->idxOldValue_uo) = (Com_OldValTrigOnChng_tauo)txSigNewVal_uo;
        }
#endif /* #ifdef COM_EffectiveSigTOC */

#ifdef COM_TxSigUpdateBit

        /* Check the clear update bit */
        if(Com_GetValue(GEN,_UPDBITCONF,constByteValue_u8))
        {
            /*Find the Byte No*/
            byteNo_uo = (PduLengthType)(txSigConstPtr_pcst->updateBitPos_uo >> 3u);

            /*Update the Bit in IPDU Buffer*/
            Com_ClearUpdateBitValue(txIpduConstPtr_pcst, txSigConstPtr_pcst->updateBitPos_uo, byteNo_uo)
        }
#endif/*COM_TxSigUpdateBit*/
        txSigConstPtr_pcst++;
    }
}

#ifdef COM_TX_SIGNALGROUP
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_Init_TxSignalGroup
 Description      : Service to initialize all configured signal Group at the sender side
 Parameter        : idIpdu_uo  -> Tx Ipdu
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_Init_TxSignalGroup(PduIdType idIpdu_uo)
{
    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
    uint16_least                    idxIdSigGrp_qu16;
    uint16_least                    idFirstSigGrp_qu16;
    uint16_least                    idMaxSigGrp_qu16;
#ifdef COM_TxSigGrpUpdateBit
    PduLengthType                   byteNo_uo;
#endif

    txIpduConstPtr_pcst   = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);

    idFirstSigGrp_qu16    = txIpduConstPtr_pcst->idFirstTxSigGrp_uo;
    idMaxSigGrp_qu16      = idFirstSigGrp_qu16 + txIpduConstPtr_pcst->numOfSigGroups_u16;

    txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(idFirstSigGrp_qu16);

    for (idxIdSigGrp_qu16 = idFirstSigGrp_qu16; idxIdSigGrp_qu16 < idMaxSigGrp_qu16; idxIdSigGrp_qu16++)
    {
        /* Call function to initialize configured transmission group signals */
        Com_Prv_Init_GroupSignal(txIpduConstPtr_pcst, txSigGrpConstPtr_pcst);

#if defined(COM_EffectiveSigGrpTOC) || defined (COM_SIGNALGROUPGATEWAY)
        COM_GET_TXSIGGRPFLAG(idxIdSigGrp_qu16).txSigGrpRAMFields_u8 = COM_ZERO;
#endif

#ifdef COM_TX_SIGNALGROUP_ARRAY
        /* Check if SignalGroupArray is enabled to copy the data to IPDU buffer else, do nothing
         * GroupSignal's SignalInitValues are already updated into the IPduBuffer in
         * Com_Prv_Init_GroupSignal() */
        if (Com_GetValue(TXSIGGRP,_ARRAYACCESS,txSigGrpConstPtr_pcst->txSignalGrpFields_u8) == COM_FALSE)
#endif /* #ifdef COM_TX_SIGNALGROUP_ARRAY */
        {
            /* Copy the data which is present in Shadow buffer to IPDU buffer */
            Com_Prv_ShadowBuffToIpduBuff((Com_SignalGroupIdType)idxIdSigGrp_qu16);
        }

#ifdef COM_TxSigGrpUpdateBit

        /* Clear Update bits*/
        if(Com_GetValue(TXSIGGRP,_UPDBITCONF,txSigGrpConstPtr_pcst->txSignalGrpFields_u8))
        {
            /*Find the Byte No*/
            byteNo_uo = (PduLengthType)(txSigGrpConstPtr_pcst->updateBitPos_uo >> 3u);

            /*Update the Bit in IPDU Buffer*/
            Com_ClearUpdateBitValue(txIpduConstPtr_pcst, txSigGrpConstPtr_pcst->updateBitPos_uo, byteNo_uo)
        }
#endif  /*COM_TxSigGrpUpdateBit*/
        txSigGrpConstPtr_pcst++;
    }
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_Init_GroupSignal
 Description      : Service to initialize all configured Groupsignal at the sender side
 Parameter        : txIpduConstPtr_pcst      -> Address to Tx IPdu
                    txSigGrpConstPtr_pcst    -> Address to SignalGroup
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_Init_GroupSignal(Com_TxIpduCfg_tpcst      txIpduConstPtr_pcst,
                                           Com_TxSigGrpCfg_tpcst    txSigGrpConstPtr_pcst)
{
    /* Variable declaration */
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
    Com_SigMax_tuo                  txGrpSigNewVal_uo;
    uint16_least                    idxIdGrpSig_qu16;
    uint16_least                    idMaxGrpSig_qu16;
    uint16                          constByteValue_u16;
    uint8                           type_u8;
    Com_MainFunc_tuo                idTxMainFunc_uo;
#ifdef COM_TX_SIGNALGROUP_ARRAY
    boolean                         isSigGrpArrayAccess_b;
    isSigGrpArrayAccess_b = Com_GetValue(TXSIGGRP,_ARRAYACCESS,txSigGrpConstPtr_pcst->txSignalGrpFields_u8);
#endif /* #ifdef COM_TX_SIGNALGROUP_ARRAY */

    idTxMainFunc_uo     = txIpduConstPtr_pcst->idMainFunc_uo;

    idxIdGrpSig_qu16    = txSigGrpConstPtr_pcst->idFirstGrpSig_uo;
    idMaxGrpSig_qu16    = idxIdGrpSig_qu16 + txSigGrpConstPtr_pcst->numOfGrpSig_uo;

    txGrpSigConstPtr_pcst    = COM_GET_TXGRPSIG_CONSTDATA(idxIdGrpSig_qu16);

    for ( ; idxIdGrpSig_qu16 < idMaxGrpSig_qu16; idxIdGrpSig_qu16++)
    {
        /* Shadowbuffer of a signal Group(group signal buffers) on sender-side shall be initialized*/
        /* Sign extension to be performed on Init valu, when a signed value is assigned to a uint32 variable,
         * it is sign extended by default*/
        txGrpSigNewVal_uo=Com_Prv_GetTxGrpSigInitValue(txGrpSigConstPtr_pcst);

        /* Get the value of the ComSignaltype bit field */
        constByteValue_u16   = txGrpSigConstPtr_pcst->txGrpSigFields_u16;
        /* Get type of the signal */
        type_u8 = (uint8) Com_GetValue(TXGRPSIG,_TYPE,constByteValue_u16);

#ifdef COM_TXGRPSIG_INT64
        /* TRACE[SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue) of a
         * signal to the size of its ComSignalType.
         * Currently for COM_SINT64 signal type, ComSignalInitValue is supported for maximum 32bits.
         * If negative values are configured, by default these values will be sign extended only till 32bits.
         * so, for COM_SINT64, it is required to perform the sign extension till 64bits explicitly. */
        if ( type_u8 == COM_SINT64 )
        {
            txGrpSigNewVal_uo = (Com_SigMax_tuo)((sint64)((sint32)txGrpSigNewVal_uo));
        }
        else
        {
            /* do nothing for other signal types */
        }
#endif /* #ifdef COM_TXGRPSIG_INT64 */

#ifdef COM_TX_SIGNALGROUP_ARRAY
        if ( isSigGrpArrayAccess_b )
        {
            Com_Prv_InitializePduBuffWithSignalInitValue( txIpduConstPtr_pcst->buffPtr_pu8,
                                                          txGrpSigNewVal_uo,
                                                          txGrpSigConstPtr_pcst->bitPos_uo,
                                                          txGrpSigConstPtr_pcst->bitSize_uo, type_u8,
                                                          Com_GetValue(TXGRPSIG,_ENDIANESS,constByteValue_u16) );
        }
        else
#endif /* #ifdef COM_TX_SIGNALGROUP_ARRAY */
#ifdef COM_TXGRPSIG_FLOAT64SUPP
        if(type_u8 == COM_FLOAT64)
        {
            /* FLOAT64 signal */
            /* For float64 signals, TxGrpSig_NewVal holds index to float array holding init values.*/
            COM_GET_TXGRPSIGTYPEFLOAT64_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo) =
                                                                             Com_getfloat64InitValue(txGrpSigNewVal_uo);
        }
        else
#endif /* #ifdef COM_TXGRPSIG_FLOAT64SUPP */
        if(type_u8 == COM_UINT8_N) /* UINT8_N signal */
        {
            Com_ByteCopyInit(&COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo),
                            (uint32)txGrpSigNewVal_uo,(uint32)txGrpSigConstPtr_pcst->bitSize_uo);
        }
        else
        {
            Com_Prv_CopyTxGroupSignalData( (Com_SignalIdType)(idxIdGrpSig_qu16), txGrpSigNewVal_uo, idTxMainFunc_uo );
        }

#if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD))
        if(txGrpSigConstPtr_pcst->filterAlgo_u8 == (uint8)COM_MASKED_NEW_DIFFERS_MASKED_OLD)
        {
            /* TRACE[SWS_Com_00603] The AUTOSAR COM module shall set the old_value of the filtering mechanisms for
             * each signal to the ComSignalInitValue during start-up. */
            COM_GET_F_OLDVALUE(txGrpSigConstPtr_pcst->idxFilter_uo) = (uint32)txGrpSigNewVal_uo;
        }
#endif /* #if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) */
#ifdef COM_EffectiveSigGrpTOC
        /* Get transfer property of the group signal */
        if(Com_GetValue(TXGRPSIG,_TP,constByteValue_u16))
        {
            COM_GET_OLDVALTRIGONCHANGE(txGrpSigConstPtr_pcst->idxOldValue_uo) = (Com_OldValTrigOnChng_tauo)txGrpSigNewVal_uo;
        }
#endif /* #ifdef COM_EffectiveSigGrpTOC */
        txGrpSigConstPtr_pcst++;
    }
}
#endif  /*COM_TX_SIGNALGROUP*/


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_Init_RxIpduWoGroupRef
 Description      : Service for initializing COM module for the reception
 Parameter        : idIpdu_uo -> Rx Ipdu
 Return value     : None
 **********************************************************************************************************************
*/
#ifdef COM_IPDU_WITHOUT_IPDUGROUP_EXISTS
LOCAL_INLINE void Com_Prv_Init_RxIpduWoGroupRef(PduIdType idIpdu_uo)
{
    Com_RxIpduRam_tpst        rxIpduRamPtr_pst;

    rxIpduRamPtr_pst = &COM_GET_RXPDURAM_S(idIpdu_uo);

    /* TRACE[SWS_Com_00840] If an I-PDU is not assigned to any I-PDU group, the AUTOSAR COM shall start this I-PDU
     * within Com_Init, such an I-PDU can never be stopped */
    COM_GET_IPDUCOUNTER_S(idIpdu_uo) = COM_ONE;

    Com_SetRamValue(RXIPDU,_PDUSTATUS,rxIpduRamPtr_pst->rxFlags_u8,COM_START);

#if defined(COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout) || defined(COM_RxSigGrpUpdateTimeout)
    /* TRACE[SWS_Com_00224] If the reception deadline monitoring state of an I-PDU is enabled(if an I-PDU is not
     * assigned to any I-PDU group), the AUTOSAR COM module shall set the reception deadline monitoring timer for the
     * included signals and signal groups to the configured ComFirstTimeout value */
    if (Com_Prv_EnableRxDeadlineMonitoring((Com_IpduId_tuo)idIpdu_uo))
    {
        /* TRACE[SWS_Com_00772] The reception deadline monitoring of an I PDU is enabled,
         * if an I-PDU is not assigned to any I-PDU group */
        COM_GET_IPDUCOUNTER_DM(idIpdu_uo) = COM_ONE;

        Com_SetRamValue(RXIPDU,_DMSTATUS,rxIpduRamPtr_pst->rxFlags_u8,COM_START);
    }
    else
    {
        COM_GET_IPDUCOUNTER_DM(idIpdu_uo) = COM_ZERO;
    }

#endif /* #if defined(COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout) || defined(COM_RxSigGrpUpdateTimeout) */

#ifdef COM_RX_IPDUCOUNTER
    /* Set the flag until first successful reception of an Rx IPDU with counter */
    Com_SetRamValue(RXIPDU,_IS_FIRST_RECEPTION,rxIpduRamPtr_pst->rxFlags_u8,COM_TRUE);
#endif
}
#endif /* #ifdef COM_IPDU_WITHOUT_IPDUGROUP_EXISTS */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_Init_TxIpduWoGroupRef
 Description      : Service for initializing COM module for the transmission
 Parameter        : idIpdu_uo -> Tx Ipdu
 Return value     : None
 **********************************************************************************************************************
*/
#ifdef COM_IPDU_WITHOUT_IPDUGROUP_EXISTS
LOCAL_INLINE void Com_Prv_Init_TxIpduWoGroupRef(PduIdType idIpdu_uo)
{
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    uint8                       latestTransMode_u8;
    boolean                     defaultTMSStatus_b;

    /*Update the bytes with Initial Value*/
    txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(idIpdu_uo);

    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);

    /* Fetch the default TMS status which is evaluated during the code generation time
     * for the ComIpdu's which are not part of any ComIpduGroup  */
    defaultTMSStatus_b = Com_GetValue(TXIPDU,_DEFAULTTMSSTATUS,txIpduConstPtr_pcst->txIPduFields_u16);

    /* TRACE[SWS_Com_00840] If an I-PDU is not assigned to any I-PDU group, the I-PDU shall be initially started
     * (and never been stopped) */
    COM_GET_IPDUCOUNTER_S(idIpdu_uo + COM_GET_NUM_RX_IPDU) = COM_ONE;

    Com_SetRamValue(TXIPDU,_PDUSTATUS,txIpduRamPtr_pst->txFlags_u16,COM_START);

    /* TRACE[SWS_Com_00877]: If an I-PDU is not part of any I-PDU Group, it is started during the initialization of COM.
     * Its starting transmission mode shall be evaluated according to the
     * ComSignalInitValue of the signals contributing to its TMS.
     * Set the Tx Mode state as DefaultTMSStatus. */
    Com_SetRamValue(TXIPDU,_TXMODESTATE,txIpduRamPtr_pst->transMode_u8,defaultTMSStatus_b);
    /* Set accordingly the TxModeMode as per DefaultTMSStatus */
    Com_Prv_SetCurrentTxModePtr(txIpduRamPtr_pst,txIpduConstPtr_pcst,defaultTMSStatus_b);
    /* Set the latest mode as per current mode */
    latestTransMode_u8 = txIpduRamPtr_pst->currentTxModePtr_pcst->mode_u8;
    Com_SetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8,latestTransMode_u8);

    if (COM_TX_MODE_IS_CYCLIC(latestTransMode_u8))
    {
        /* I.e Load the TxTimeOffset for cyclic type Pdu's(PERIODIC or MIXED) */
        txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEOFFSET(idIpdu_uo, txIpduRamPtr_pst->currentTxModePtr_pcst);
#ifdef COM_MIXEDPHASESHIFT
        /* Set the MixedPhaseShift Mode */
        Com_SetRamValue(TXIPDU,_MIXEDPHASESHIFT,txIpduRamPtr_pst->transMode_u8,txIpduRamPtr_pst->currentTxModePtr_pcst->mixedPhaseShift_b);
#endif /* #ifdef COM_MIXEDPHASESHIFT */
    }
}
#endif /* #ifdef COM_IPDU_WITHOUT_IPDUGROUP_EXISTS */

