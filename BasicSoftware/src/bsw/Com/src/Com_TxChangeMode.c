

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
#ifdef COM_TxFilters
LOCAL_INLINE void Com_Prv_TxChangeMode_Signal            (Com_IpduId_tuo idTxIpdu_uo);

# ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE void Com_Prv_TxChangeMode_SignalGroup       (Com_IpduId_tuo idTxIpdu_uo);
LOCAL_INLINE boolean Com_Prv_TxChangeMode_GroupSignal    (Com_IpduId_tuo idTxIpdu_uo,
                                                          Com_TxGrpSigCfg_tpcst txGrpSigConstPtr_pcst);
# endif
#endif /* #ifdef COM_TxFilters */

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


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
 Function name    : Com_Prv_TxChangeMode
 Description      : Internal function used for Transmission Mode Selector
 Parameter        : idTxIpdu_uo
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_TxChangeMode(Com_IpduId_tuo idTxIpdu_uo)
{
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    uint8                       latestTransMode_u8;
    boolean                     mode_b;

    /* Initialise Mode as TRUE, it could be the most probable case */
    mode_b                  = COM_TRUE;

    txIpduConstPtr_pcst     = COM_GET_TX_IPDU_CONSTDATA(idTxIpdu_uo);
    txIpduRamPtr_pst        = &COM_GET_TXPDURAM_S(idTxIpdu_uo);

    /* Call Filter for all signals and Group Signals in the IPDU */
#ifdef COM_TxFilters
    txIpduRamPtr_pst->cntrTrueTMC_u16 = COM_ZERO;

    /* Check if Transmission Mode switch is possible by signal filter mechanism */
    if (Com_GetValue(TXIPDU,_FILTEREVALREQ,txIpduConstPtr_pcst->txIPduFields_u16))
    {
        Com_Prv_TxChangeMode_Signal(idTxIpdu_uo);

#ifdef COM_TX_SIGNALGROUP
        Com_Prv_TxChangeMode_SignalGroup(idTxIpdu_uo);
#endif

        if (txIpduRamPtr_pst->cntrTrueTMC_u16 != COM_ZERO)
        {
            /* If any signal/signal group evaluates filter as TRUE, then TM TRUE is selected for IPDU */
            mode_b = COM_TRUE;
        }
        else
        {
            /* If all signal/signal group evaluates filter as FALSE, then TM FALSE is selected for IPDU */
            mode_b = COM_FALSE;
        }
    }
#endif /*COM_TxFilters*/

    /* If a change of the TMS causes a change of the transmission mode for one I-PDU, the timer for the cycle
    time of the periodic and the Mixed Tx Mode shall be retstarted */

    /* Update the TxModeState with the Mode */
    Com_SetRamValue(TXIPDU,_TXMODESTATE,txIpduRamPtr_pst->transMode_u8,mode_b);

    /* Update the CurrentTxModePtr */
    Com_Prv_SetCurrentTxModePtr(txIpduRamPtr_pst,txIpduConstPtr_pcst,mode_b);

    /* Update the Com_LatestTransMode[idTxIpdu_uo] */
    latestTransMode_u8 = txIpduRamPtr_pst->currentTxModePtr_pcst->mode_u8;
    Com_SetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8,latestTransMode_u8);

    /* Reset the timers */
    if (COM_TX_MODE_IS_CYCLIC(latestTransMode_u8))
    {
        /* The function may be interrupted by Com_MainFunctionTx().This will lead to inconsistant data in
         * Com_MainFunctionTx(), So Interrupt is locked here. */
        txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEPERIOD(idTxIpdu_uo, txIpduRamPtr_pst->currentTxModePtr_pcst);

        /*Below Initialize is done if Com_Prv_TxChangeMode() is called by Ipdu_GroupStart()*/
        txIpduRamPtr_pst->cntrRepPeriod_u16 = COM_ZERO;
        /*This means there is no n Transmission, This will be updated in Com_SendSignal()and Com_SendSignalGroup()*/
        txIpduRamPtr_pst->cntrRepetitions_u8 = COM_ZERO;
        /* FC_VariationPoint_START */
#ifdef COM_MIXEDPHASESHIFT
        Com_SetRamValue(TXIPDU,_MIXEDPHASESHIFT,txIpduRamPtr_pst->transMode_u8,
                                                txIpduRamPtr_pst->currentTxModePtr_pcst->mixedPhaseShift_b);
#endif /* #ifdef COM_MIXEDPHASESHIFT */
        /* FC_VariationPoint_END */
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#ifdef COM_TxFilters
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_TxChangeMode_Signal
 Description      : Internal function used for Transmission Mode Selector
 Parameter        : idTxIpdu_uo
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_TxChangeMode_Signal(Com_IpduId_tuo idTxIpdu_uo)
{
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
    Com_TxSigRam_tpst           txSigRamPtr_pst;
    uint16_least                index_qu16;
    Com_SigMax_tuo              txNewVal_uo;
    uint8                       filterAlg_u8;  /* Filter type */
    uint8                       constByteValue_u8;
    uint8                       sigType_u8;
    boolean                     filterVal_b;

    txIpduConstPtr_pcst     = COM_GET_TX_IPDU_CONSTDATA(idTxIpdu_uo);
    txIpduRamPtr_pst        = &COM_GET_TXPDURAM_S(idTxIpdu_uo);
    txSigConstPtr_pcst      = COM_GET_TXSIG_CONSTDATA(txIpduConstPtr_pcst->idTxSig_uo);
    txSigRamPtr_pst         = &COM_GET_TXSIGNALFLAG(txIpduConstPtr_pcst->idTxSig_uo);

    for (index_qu16 = txIpduConstPtr_pcst->numOfSig_u16; index_qu16 != COM_ZERO; index_qu16--)
    {
        filterAlg_u8 = (uint8)Com_GetValue(TXSIG,_FILTRALG,txSigConstPtr_pcst->txSignalFields_u16);

        /* Check if the filter is configured, NOTE: If filter is not configured, then the Value will be COM_ALWAYS
            in the configuration and this means that the signal will not contribute to TMS calculation */
        if (filterAlg_u8 != (uint8)COM_NOTCONFIGURED)
        {
            constByteValue_u8 = txSigConstPtr_pcst->general_u8;
            sigType_u8 = Com_GetValue(GEN,_TYPE,constByteValue_u8);

            if (sigType_u8 != (uint8)COM_UINT8_N)
            {
                /* Locks are required here,to protect Tx Ipdu buffer due to the signal updates as per below requirement.
                 * TRACE[SWS_Com_00334] By a call to the functions: Com_SendSignal, Com_SendSignalGroup, or
                 * Com_InvalidateSignal, the AUTOSAR COM module shall update the values of its internal buffers even
                 * for stopped I-PDUs. */
                SchM_Enter_Com_TxIpduProtArea(IPDUGROUPSTART);

                txNewVal_uo = Com_Prv_UnpackSignal( Com_GetValue(GEN,_ENDIANESS,constByteValue_u8),
                                             txSigConstPtr_pcst->bitPos_uo,
                                             txSigConstPtr_pcst->bitSize_uo,
                                             txIpduConstPtr_pcst ->buffPtr_pu8,
                                             ((sigType_u8 & COM_ONE) != COM_ZERO));

                SchM_Exit_Com_TxIpduProtArea(IPDUGROUPSTART);
            }
            else
            {
                txNewVal_uo = COM_ZERO;
                /* This variable will not be checked in Com_Prv_Filter, as the Algorithm for Uint8[n] will be only
                 * ALWAYS or NEVER */
            }
            /* The last parameter is Information from where the call to Com_Prv_Filter happens */
            /* NOTE on last parameter:
             * 1.Currently the values for the parameter CallContext_b are COM_TRUE/COM_FALSE
             * 2.It is used only for filter ONE_EVERYN */
            filterVal_b = Com_Prv_Filter((uint32)txNewVal_uo,filterAlg_u8, txSigConstPtr_pcst->idxFilter_uo, COM_FALSE);

            if (filterVal_b)
            {
                /* Increment TRUE counter , based on filter condition */
                txIpduRamPtr_pst->cntrTrueTMC_u16++;
            }
            /* Store the latest filter state of the signal */
            Com_SetRamValue(TXSIG,_SIGCHNGMODE,txSigRamPtr_pst->txSigRAMFields_u8,filterVal_b);
        }
        txSigConstPtr_pcst++;
        txSigRamPtr_pst++;
    }
}


#ifdef COM_TX_SIGNALGROUP

/*
 **********************************************************************************************************************
 Function name    : Com_Prv_TxChangeMode_SignalGroup
 Description      : Internal function used for Transmission Mode Selector
 Parameter        : idTxIpdu_uo
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_TxChangeMode_SignalGroup(Com_IpduId_tuo idTxIpdu_uo)
{
    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
    Com_TxGrpSigRam_tpst            txGrpSigRamPtr_pst;
    uint16_least                    idxSigGrp_qu16;
    uint16_least                    idxGrpSig_qu16;
    boolean                         filterVal_b;

    txIpduConstPtr_pcst   = COM_GET_TX_IPDU_CONSTDATA(idTxIpdu_uo);

    /* Secondly check for Group Signals */
    /* For Selection of the transmission mode the Group signals shall be treated as normal signals */
    txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(txIpduConstPtr_pcst->idFirstTxSigGrp_uo);

    for(idxSigGrp_qu16 = txIpduConstPtr_pcst->numOfSigGroups_u16; idxSigGrp_qu16 > COM_ZERO; idxSigGrp_qu16--)
    {
        if(Com_GetValue(TXSIGGRP,_FILTEREVALREQ,txSigGrpConstPtr_pcst->txSignalGrpFields_u8))
        {
            /* Now check for each Group Signal of this signal group */
            txGrpSigRamPtr_pst = &COM_GET_TXGRPSIGFLAG(txSigGrpConstPtr_pcst->idFirstGrpSig_uo);

            txGrpSigConstPtr_pcst = COM_GET_TXGRPSIG_CONSTDATA(txSigGrpConstPtr_pcst->idFirstGrpSig_uo);

            for(idxGrpSig_qu16 = txSigGrpConstPtr_pcst->numOfGrpSig_uo; idxGrpSig_qu16 > COM_ZERO; idxGrpSig_qu16--)
            {
                /* Check if the filter is configured, NOTE: If filter is not configured,
                then the Value will be COM_ALWAYS in the configuration */
                if(txGrpSigConstPtr_pcst->filterAlgo_u8 != (uint8)COM_NOTCONFIGURED)
                {
                    filterVal_b = Com_Prv_TxChangeMode_GroupSignal(idTxIpdu_uo, txGrpSigConstPtr_pcst);

                    /* Store the latest filter state of the signal */
                    Com_SetRamValue(TXGRPSIG,_SIGCHNGMODE,txGrpSigRamPtr_pst->txGrpSigRAMFields_u8,filterVal_b);
                }
                txGrpSigConstPtr_pcst++;
                txGrpSigRamPtr_pst++;
            }
        }
        txSigGrpConstPtr_pcst++;
    }
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_TxChangeMode_GroupSignal
 Description      : Internal function used for Transmission Mode Selector
 Parameter        : idTxIpdu_uo         - TX Pdu Id
                    txGrpSigConstPtr_pcst - Group signal constant pointer
 Return value     : Filter_Val - COM_TRUE/COM_FALSE
 **********************************************************************************************************************
*/

LOCAL_INLINE boolean Com_Prv_TxChangeMode_GroupSignal(Com_IpduId_tuo idTxIpdu_uo, Com_TxGrpSigCfg_tpcst txGrpSigConstPtr_pcst)
{
    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
    Com_TxIpduRam_tpst              txIpduRamPtr_pst;
# ifdef COM_TX_SIGNALGROUP_ARRAY
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
#endif
    Com_SigMax_tuo                  txNewVal_uo;
    Com_TxGrpSigBuffIndex_tuo       idxGrpSigBuff_uo;
    Com_MainFunc_tuo                idTxMainFunc_uo;
    uint16                          constByteValue_u16;
    uint16                          sigType_u16;
    boolean                         filterVal_b;

    txIpduConstPtr_pcst     = COM_GET_TX_IPDU_CONSTDATA(idTxIpdu_uo);
    txIpduRamPtr_pst        = &COM_GET_TXPDURAM_S(idTxIpdu_uo);
    idTxMainFunc_uo         = txIpduConstPtr_pcst->idMainFunc_uo;
    txNewVal_uo             = COM_ZERO;

    constByteValue_u16 = txGrpSigConstPtr_pcst->txGrpSigFields_u16;
    sigType_u16 = Com_GetValue(TXGRPSIG,_TYPE,constByteValue_u16);
# ifdef COM_TX_SIGNALGROUP_ARRAY
    txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(txGrpSigConstPtr_pcst->idSigGrp_uo);
# endif
    if(sigType_u16 != COM_UINT8_N)
    {
# ifdef COM_TX_SIGNALGROUP_ARRAY
        if (Com_GetValue(TXSIGGRP,_ARRAYACCESS,txSigGrpConstPtr_pcst->txSignalGrpFields_u8))
        {
            /* Locks are required here, to protect Tx Ipdu buffer due to the signal updates as per below requirement.
             * TRACE[SWS_Com_00334] By a call to the functions: Com_SendSignal, Com_SendSignalGroup, or
             * Com_InvalidateSignal, the AUTOSAR COM module shall update the values of its internal buffers even
             * for stopped I-PDUs. */
            SchM_Enter_Com_TxIpduProtArea(IPDUGROUPSTART);

            /* Unpack GroupSignal data from SignalGroup with ArrayAccess */
            txNewVal_uo = Com_Prv_UnpackSignal( Com_GetValue(TXGRPSIG,_ENDIANESS,constByteValue_u16),
                                         txGrpSigConstPtr_pcst->bitPos_uo,
                                         txGrpSigConstPtr_pcst->bitSize_uo,
                                         txIpduConstPtr_pcst->buffPtr_pu8,
                                         ((sigType_u16 & COM_ONE) != COM_ZERO));

            SchM_Exit_Com_TxIpduProtArea(IPDUGROUPSTART);
        }
        else
# endif /* # ifdef COM_TX_SIGNALGROUP_ARRAY */
        {
            idxGrpSigBuff_uo = txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo;

            switch(sigType_u16 >> 1u)
            {
            case 0x00u: /* SINT8 signal, UINT8 signal */
            case 0x03u: /*Boolean Signal*/
                txNewVal_uo = COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo);
                break;

            case 0x01u:
                /* SINT16 signal, UINT16 signal */
#if (CPU_TYPE == CPU_TYPE_8)
                Com_Get_SchM_Enter_Com_TxGrpSigBuffer
#endif
                txNewVal_uo = COM_GET_TXGRPSIGTYPEU16_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo);
#if (CPU_TYPE == CPU_TYPE_8)
                Com_Get_SchM_Exit_Com_TxGrpSigBuffer
#endif
                break;

            case 0x02u:
#if (CPU_TYPE != CPU_TYPE_32)
                Com_Get_SchM_Enter_Com_TxGrpSigBuffer
#endif
                /* SINT32 signal, UINT32 signal */
                txNewVal_uo = COM_GET_TXGRPSIGTYPEU32_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo);
#if (CPU_TYPE != CPU_TYPE_32)
                Com_Get_SchM_Exit_Com_TxGrpSigBuffer
#endif
                break;

# ifdef COM_TXGRPSIG_INT64
            case 0x08u:
                Com_Get_SchM_Enter_Com_TxGrpSigBuffer
                /* SINT64 signal, UINT64 signal */
                txNewVal_uo = COM_GET_TXGRPSIGTYPEU64_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo);
                Com_Get_SchM_Exit_Com_TxGrpSigBuffer
                break;
# endif /* #ifdef COM_TXGRPSIG_INT64 */

            default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal
             * types are allowed to be configured, hence default case is intentionally left empty.
             */
                break;
            }
        }
    }
    else
    {
        /* For GroupSignal of array types(UINT8[n]), signal values are not used for
         * filter(ALWAYS, NEVER, ONE_EVERY_N) evaluation */
        /* txNewVal_uo is already initialized to 0u */
    }
    /* The last parameter is Information from where the call to Com_Prv_Filter happens */
    /* NOTE on last parameter:
    * 1.Currently the values for the parameter CallContext_b are COM_TRUE/COM_FALSE
    * 2.It is used only for filter ONE_EVERYN */
    filterVal_b = Com_Prv_Filter((uint32)txNewVal_uo, txGrpSigConstPtr_pcst->filterAlgo_u8,
                                              txGrpSigConstPtr_pcst->idxFilter_uo, COM_FALSE);

    if (filterVal_b)
    {
        /* Increment TRUE counter , based on filter condition */
        txIpduRamPtr_pst->cntrTrueTMC_u16++;
    }
    return filterVal_b;
}
#endif /* #ifdef COM_TX_SIGNALGROUP */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_SigTxChangeMode
 Description      : This function evaluates the current TMS of the IPDU, based on the corresponding signal
                    which is updated.
 Parameter        : idSignal_u16 -> Id of the signal
                  : txNewVal_u32 -> New signal value
 Return value     : COM_TRUE/COM_FALSE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
boolean Com_Prv_SigTxChangeMode(Com_SignalIdType idSignal_u16, uint32 txNewVal_u32)
{
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
    Com_TxSigRam_tpst           txSigRamPtr_pst;
    Com_IpduId_tuo              idTxIpdu_uo;
    boolean                     sigTMSState_b;
    boolean                     returnValue_b;

    txSigConstPtr_pcst      = COM_GET_TXSIG_CONSTDATA(idSignal_u16);
    txSigRamPtr_pst         = &COM_GET_TXSIGNALFLAG(idSignal_u16);
    idTxIpdu_uo             = txSigConstPtr_pcst->idComIPdu_uo;

    txIpduRamPtr_pst        = &COM_GET_TXPDURAM_S(idTxIpdu_uo);

    SchM_Enter_Com_TxIpduProtArea(SIGTXCHANGEMODE);

    /* Check the configured filter algorithm */
    if (Com_GetValue(TXSIG,_FILTRALG,txSigConstPtr_pcst->txSignalFields_u16) > (uint16)COM_NEVER)
    {
        /* Evaluate current filter state of the signal */
        /* The last parameter is Information from where the call to Com_Prv_Filter happens */
        /* NOTE on last parameter:
         * 1.Currently the values for the parameter CallContext_b are COM_TRUE/COM_FALSE
         * 2.It is used only for filter ONE_EVERYN */
        sigTMSState_b = Com_Prv_Filter(txNewVal_u32,
                           (uint8)Com_GetValue(TXSIG,_FILTRALG,txSigConstPtr_pcst->txSignalFields_u16),
                                  txSigConstPtr_pcst->idxFilter_uo, COM_TRUE);

        /* Update TMS TRUE counter, only if filter state of the signal changes.
         * Otherwise, TMS TRUE counter value remains unchanged. */
        if (Com_GetRamValue(TXSIG,_SIGCHNGMODE,txSigRamPtr_pst->txSigRAMFields_u8) != sigTMSState_b)
        {
            if (sigTMSState_b)
            {
                /* Signal TMC has changed from FALSE to TRUE, hence increment TMS TRUE counter */
                txIpduRamPtr_pst->cntrTrueTMC_u16++;
            }
            else
            {
                /* Signal TMC has changed from TRUE to FALSE, hence decrement TMS TRUE counter */
                txIpduRamPtr_pst->cntrTrueTMC_u16--;
            }

            /* Update the latest filter state of the signal, if the signal filter state changes */
            Com_SetRamValue(TXSIG,_SIGCHNGMODE,txSigRamPtr_pst->txSigRAMFields_u8,sigTMSState_b);
        }
    }

    /* Call function to check for the mode changes */
    /* If TMS was changed explicitly by call to Com_SwitchIpduTxMode,
     * then call to Com_SendSignal or Com_SendSignalGroup must switch back to actual TMS for I-PDU. */
    returnValue_b = Com_Prv_CheckModeChange(idTxIpdu_uo);

    SchM_Exit_Com_TxIpduProtArea(SIGTXCHANGEMODE);

    return(returnValue_b);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#ifdef COM_TX_SIGNALGROUP
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_SigGrpTxChangeMode
 Description      : This function evaluates the current TMS of the IPDU, based on the corresponding signal or
                    signal group which is updated.
 Parameter        : idSigGroup_u16 -> Id of the SignalGroup
 Return value     : COM_TRUE/COM_FALSE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
boolean Com_Prv_SigGrpTxChangeMode(Com_SignalGroupIdType idSigGroup_u16)
{

    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
    Com_TxIpduRam_tpst              txIpduRamPtr_pst;
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
    Com_TxGrpSigRam_tpst            txGrpSigRamPtr_pst;

    Com_SigMax_tuo                  txNewValGrpSig_uo;
    Com_TxGrpSigBuffIndex_tuo       idxGrpSigBuff_uo;
    uint16_least                    idxGrpSig_qu16;
    Com_IpduId_tuo                  idTxIpdu_uo;
    uint16                          constByteValue_u16;
    uint16                          sigType_u16;
    Com_MainFunc_tuo                idTxMainFunc_uo;
    boolean                         sigTMSState_b;
    boolean                         returnValue_b;
#ifdef COM_TX_SIGNALGROUP_ARRAY
    boolean                         isSigGrpArrayAccess_b;
#endif

    txSigGrpConstPtr_pcst   = COM_GET_TXSIGGRP_CONSTDATA(idSigGroup_u16);
    idTxIpdu_uo             = txSigGrpConstPtr_pcst->idComIPdu_uo;

    txIpduConstPtr_pcst     = COM_GET_TX_IPDU_CONSTDATA(idTxIpdu_uo);
    txIpduRamPtr_pst        = &COM_GET_TXPDURAM_S(idTxIpdu_uo);

    txGrpSigConstPtr_pcst   = COM_GET_TXGRPSIG_CONSTDATA(txSigGrpConstPtr_pcst->idFirstGrpSig_uo);
    txGrpSigRamPtr_pst      = &COM_GET_TXGRPSIGFLAG(txSigGrpConstPtr_pcst->idFirstGrpSig_uo);

    idTxMainFunc_uo         = txIpduConstPtr_pcst->idMainFunc_uo;

#ifdef COM_TX_SIGNALGROUP_ARRAY
    isSigGrpArrayAccess_b  = Com_GetValue(TXSIGGRP,_ARRAYACCESS,txSigGrpConstPtr_pcst->txSignalGrpFields_u8);
#endif

    /* Call Filter for all Group Signals in the IPDU */
    SchM_Enter_Com_TxIpduProtArea(SIGGRPTXCHANGEMODE);

    for(idxGrpSig_qu16 = txSigGrpConstPtr_pcst->numOfGrpSig_uo; idxGrpSig_qu16 > COM_ZERO; idxGrpSig_qu16--)
    {
        /* Check if the filter is configured, NOTE: If filter is not configured,
           then the Value will be COM_ALWAYS in the configuration */
        if((txGrpSigConstPtr_pcst->filterAlgo_u8 != (uint8)COM_NOTCONFIGURED) && (txGrpSigConstPtr_pcst->filterAlgo_u8 > COM_NEVER))
        {
            constByteValue_u16  = txGrpSigConstPtr_pcst->txGrpSigFields_u16;
            sigType_u16         = (Com_GetValue(TXGRPSIG,_TYPE,constByteValue_u16));

            txNewValGrpSig_uo   = COM_ZERO; /* initialize local variable for every GroupSignal */

            if(txGrpSigConstPtr_pcst->filterAlgo_u8 != COM_ONE_EVERY_N)
            {
#ifdef COM_TX_SIGNALGROUP_ARRAY
                if (isSigGrpArrayAccess_b)
                {
                    /* Unpack GroupSignal data from SignalGroup with ArrayAccess */
                    txNewValGrpSig_uo = Com_Prv_UnpackSignal( Com_GetValue(TXGRPSIG,_ENDIANESS,constByteValue_u16),
                                                              txGrpSigConstPtr_pcst->bitPos_uo,
                                                              txGrpSigConstPtr_pcst->bitSize_uo,
                                                              txIpduConstPtr_pcst->buffPtr_pu8,
                                                              ((sigType_u16 & COM_ONE) != COM_ZERO));
                }
                else
#endif /* # ifdef COM_TX_SIGNALGROUP_ARRAY */
                {
                    idxGrpSigBuff_uo = txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo;

                    switch(sigType_u16 >> 1u)
                    {
                    case 0x00u:  /* UINT8   Signal */
                    case 0x03u:  /* Boolean Signal */
                        txNewValGrpSig_uo = COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo);
                        break;

                    case 0x01u:
                        /* UINT16 signal */
                        txNewValGrpSig_uo = COM_GET_TXGRPSIGTYPEU16_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo);
                        break;

                    case 0x02u:
                        /* UINT32 signal */
                        txNewValGrpSig_uo = COM_GET_TXGRPSIGTYPEU32_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo);
                        break;

#ifdef COM_TXGRPSIG_INT64
                    case 0x08u:
                        /* UINT64 signal */
                        txNewValGrpSig_uo = COM_GET_TXGRPSIGTYPEU64_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo);
                        break;
#endif /* #ifdef COM_TXGRPSIG_INT64 */

                    default:
                        /**
                         * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal
                         * types are allowed to be configured, hence default case is intentionally left empty.
                         */
                        break;
                    }
                }
            }
            else
            {
                /* For GroupSignal of array types(UINT8[n],FLOAT), signal values are not used for
                 * filter(ALWAYS, NEVER, ONE_EVERY_N) evaluation */
                /* txNewValGrpSig_uo is already initialized to 0 */
            }

            /* The last parameter is Information from where the call to Com_Prv_Filter happens */
            /* NOTE on last parameter:
             * 1.Currently the values for the parameter CallContext_b are COM_TRUE/COM_FALSE
             * 2.It is used only for filter ONE_EVERYN */
            sigTMSState_b = Com_Prv_Filter((uint32)txNewValGrpSig_uo, txGrpSigConstPtr_pcst->filterAlgo_u8,
                                                               txGrpSigConstPtr_pcst->idxFilter_uo, COM_TRUE);

            /* Update TMS TRUE counter , only if filter state of the signal changes.
             * Otherwise, TMS TRUE counter value remains unchanged.
             */
            if (Com_GetRamValue(TXGRPSIG,_SIGCHNGMODE,txGrpSigRamPtr_pst->txGrpSigRAMFields_u8) != sigTMSState_b)
            {
                if (sigTMSState_b)
                {
                    /* Signal TMC has changed from FALSE to TRUE, hence increment TMS TRUE counter */
                    txIpduRamPtr_pst->cntrTrueTMC_u16++;
                }
                else
                {
                    /* Signal TMC has changed from TRUE to FALSE, hence decrement TMS TRUE counter */
                    txIpduRamPtr_pst->cntrTrueTMC_u16--;
                }

                Com_SetRamValue(TXGRPSIG,_SIGCHNGMODE,txGrpSigRamPtr_pst->txGrpSigRAMFields_u8,sigTMSState_b);
            }
        }
        txGrpSigConstPtr_pcst++;
        txGrpSigRamPtr_pst++;
    }

    /* Call function to check for the mode changes */
    /* If TMS was changed explicitly by call to Com_SwitchIpduTxMode,
     * then call to Com_SendSignal or Com_SendSignalGroup must switch back to actual TMS for I-PDU. */
    returnValue_b = Com_Prv_CheckModeChange(idTxIpdu_uo);

    SchM_Exit_Com_TxIpduProtArea(SIGGRPTXCHANGEMODE);

    return(returnValue_b);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_TX_SIGNALGROUP */

#endif /* #ifdef COM_TxFilters */

