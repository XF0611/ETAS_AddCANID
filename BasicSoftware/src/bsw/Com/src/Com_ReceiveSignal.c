

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
 Function name    : Com_ReceiveSignal
 Description      : Service for receiving the signal, This API copies the data to the application buffer
 Parameter        : idSignal_u16        -> Id of the Signal
                  : signalDataPtr_pv    -> Pointer to the address where signal data needs to be copied.
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(signalDataPtr_pv) in a function prototype should be declared as pointer
   to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is as
   below, hence suppressed */
uint8 Com_ReceiveSignal(Com_SignalIdType idSignal_u16, void * signalDataPtr_pv)
{

    /* Local pointer holds the address of Rx signal static configuration */
    Com_RxSigCfg_tpcst          rxSigConstPtr_pcst;
    uint8                       status_u8;  /* Return status */
    /* Local variable holds the signal type */
    uint8                       sigType_u8;

    status_u8 = COM_SERVICE_NOT_AVAILABLE;

#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveSignal, COM_E_UNINIT);
    }
    else if (signalDataPtr_pv == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveSignal, COM_E_PARAM_POINTER);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    if(Com_Prv_IsValidRxSignalId(idSignal_u16))
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idSignal_u16        = COM_GET_RXSIGNAL_ID(idSignal_u16);

        rxSigConstPtr_pcst  = COM_GET_RXSIG_CONSTDATA(idSignal_u16);

        sigType_u8          = Com_GetValue(GEN,_TYPE,rxSigConstPtr_pcst->general_u8);

        /*Check if Ipdu Group is started*/
        if(Com_Prv_CheckRxIPduStatus((PduIdType)rxSigConstPtr_pcst->idComIPdu_uo))
        {
            status_u8 = E_OK;
        }

        if((sigType_u8 % COM_TWO) == COM_ZERO)
        {
            /* Copy Data of Unsigned Signal Types */
            Com_Prv_CopyData_Sig_UnsignedType(idSignal_u16, signalDataPtr_pv);
        }
        else
        {
            /* Copy Data of Signed Signal Types */
            Com_Prv_CopyData_Sig_SignedType(idSignal_u16, signalDataPtr_pv);
        }
    }
#ifdef COM_RX_SIGNALGROUP
    else if(Com_Prv_IsValidRxGroupSignalId(idSignal_u16))
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idSignal_u16    = COM_GET_RXGRPSIGNAL_ID(idSignal_u16);

        Com_Prv_ReceiveShadowSignal(idSignal_u16, signalDataPtr_pv);
        status_u8 = E_OK;
    }
#endif
    else
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveSignal, COM_E_PARAM);
    }
    return(status_u8);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CopyData_Sig_UnsignedType
 Description      : Service to copy the data for UnSigned Signal type
 Parameter        : idSignal_u16     -> Id of the signal
                  : signalDataPtr_pv -> Pointer to the the signal data
 Return value     : None
 **********************************************************************************************************************
*/
/* MR12 RULE 8.13 VIOLATION: This is a false positive warning, 'signalDataPtr_pv' is modified and so the pointer cannot
 * be changed to type 'pointer to const'. Hence warning is suppressed. */
void Com_Prv_CopyData_Sig_UnsignedType(Com_SignalIdType idSignal_u16, void * signalDataPtr_pv)
{

    Com_RxSigCfg_tpcst          rxSigConstPtr_pcst;
    Com_SigBuffIndex_tuo        idxSigBuff_uo;
    Com_MainFunc_tuo            idRxMainFunc_uo;
    uint8                       sigType_u8;

    rxSigConstPtr_pcst      = COM_GET_RXSIG_CONSTDATA(idSignal_u16);
    idxSigBuff_uo           = rxSigConstPtr_pcst->idxSigBuff_uo;
    sigType_u8              = Com_GetValue(GEN,_TYPE,rxSigConstPtr_pcst->general_u8);
    idRxMainFunc_uo         = (COM_GET_RX_IPDU_CONSTDATA(rxSigConstPtr_pcst->idComIPdu_uo))->idMainFunc_uo;

    /* Copy the SigBuffer data into signalDataPtr_pv*/
    switch(sigType_u8)
    {
        /* MR12 RULE 11.5 VIOLATION: For signalDataPtr_pv is defined as void pointer in the
           AUTOSAR specification, so suppressing warning "Cast from a pointer to void to a pointer to object". */
        /* Locks are required here, as the signal buffer may be updated in Com_RxIndication(),
           which may be called in interrupt context. */
        case COM_UINT8:
            {
                *(uint8*)signalDataPtr_pv = COM_GET_SIGTYPEUINT8_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
            }
            break;
        case COM_UINT16:
            {
#if (CPU_TYPE == CPU_TYPE_8)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
#endif
                *(uint16*)signalDataPtr_pv = COM_GET_SIGTYPEUINT16_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
#if (CPU_TYPE == CPU_TYPE_8)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
#endif
            }
            break;
        case COM_UINT32:
            {
#if (CPU_TYPE != CPU_TYPE_32)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
#endif
                *(uint32*)signalDataPtr_pv = COM_GET_SIGTYPEUINT32_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
#if (CPU_TYPE != CPU_TYPE_32)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
#endif
            }
            break;
#ifdef COM_RXSIG_INT64
        case COM_UINT64:
            {
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                *(uint64*)signalDataPtr_pv = COM_GET_SIGTYPEUINT64_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
            }
            break;
#endif /* #ifdef COM_RXSIG_INT64 */
        case COM_BOOLEAN:
            {
                *(boolean*)signalDataPtr_pv = (COM_GET_SIGTYPEUINT8_BUFF(idRxMainFunc_uo, idxSigBuff_uo) != COM_ZERO);
            }
            break;
        case COM_UINT8_N:
            {
                /*Copy each byte into the signalDataPtr_pv from Signal buffer  */
                /* The locks inside the function Com_ByteCopy() are removed.
                 * Hence locks are used here
                 */
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                Com_ByteCopy((uint8*)signalDataPtr_pv,&COM_GET_SIGTYPEUINT8_BUFF(idRxMainFunc_uo, idxSigBuff_uo),
                             (uint32)rxSigConstPtr_pcst->bitSize_uo);
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
            }
            break;

/* FC_VariationPoint_START */
#ifdef COM_FLOAT32SUPP
        case COM_FLOAT32:
            {
                uint32 rxNewVal_u32;

#if (CPU_TYPE != CPU_TYPE_32)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
#endif
                rxNewVal_u32 = COM_GET_SIGTYPEUINT32_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
#if (CPU_TYPE != CPU_TYPE_32)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
#endif
                /* Convert the 32 bit stream  into float value */
                /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
                   But the void pointer is always deferenced to a type, based on the pointers alignment.
                   Hence the alignment warning can safely be ignored. */
                (void)rba_BswSrv_MemCopy( signalDataPtr_pv, (const void *)&rxNewVal_u32, COM_F32_SIZE_IN_BYTES );

            }
            break;
#endif /* #ifdef COM_FLOAT32SUPP */

#ifdef COM_RXSIG_FLOAT64SUPP
        case COM_FLOAT64:
            /* FLOAT64 signal */
            {
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                /* MR12 RULE 11.5 VIOLATION: For signalDataPtr_pv is defined as void pointer in the AUTOSAR specification,
                   so suppressing warning "Cast from a pointer to void to a pointer to object". */
                *(float64*)signalDataPtr_pv = COM_GET_SIGTYPEFLOAT64_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);

            }
            break;
#endif
/* FC_VariationPoint_END */

        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
            break;
    }
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CopyData_Sig_SignedType
 Description      : Service to copy the data for Signed Signal type
 Parameter        : idSignal_u16      -> Id of the signal
                  : signalDataPtr_pv  -> Pointer to the the signal data
 Return value     : None
 **********************************************************************************************************************
*/
/* MR12 RULE 8.13 VIOLATION: This is a false positive warning, 'signalDataPtr_pv' is modified and so the pointer cannot
 * be changed to type 'pointer to const'. Hence warning is suppressed. */
void Com_Prv_CopyData_Sig_SignedType(Com_SignalIdType idSignal_u16, void * signalDataPtr_pv)
{

    Com_RxSigCfg_tpcst          rxSigConstPtr_pcst;
    Com_SigBuffIndex_tuo        idxSigBuff_uo;
    /* Local variable holds internal Rx-MainFunction Id */
    Com_MainFunc_tuo            idRxMainFunc_uo;
    /* Local variable holds the signal type */
    uint8                       sigType_u8;

    rxSigConstPtr_pcst          = COM_GET_RXSIG_CONSTDATA(idSignal_u16);
    idxSigBuff_uo               = rxSigConstPtr_pcst->idxSigBuff_uo;
    sigType_u8                  = Com_GetValue(GEN,_TYPE,rxSigConstPtr_pcst->general_u8);
    idRxMainFunc_uo             = (COM_GET_RX_IPDU_CONSTDATA(rxSigConstPtr_pcst->idComIPdu_uo))->idMainFunc_uo;

    /* Copy the SigBuffer data into signalDataPtr_pv*/
    switch(sigType_u8)
    {
        /* MR12 RULE 11.5 VIOLATION: For signalDataPtr_pv is defined as void pointer in the
           AUTOSAR specification, so suppressing warning "Cast from a pointer to void to a pointer to object". */
        /* Locks are required here, as the signal buffer may be updated in Com_RxIndication(),
           which may be called in interrupt context. */
        case COM_SINT8:
            {
                *(sint8*)signalDataPtr_pv = (sint8)COM_GET_SIGTYPEUINT8_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
            }
            break;
        case COM_SINT16:
            {
#if (CPU_TYPE == CPU_TYPE_8)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
#endif
                *(sint16*)signalDataPtr_pv = (sint16)COM_GET_SIGTYPEUINT16_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
#if (CPU_TYPE == CPU_TYPE_8)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
#endif
            }
            break;
        case COM_SINT32:
            {
#if (CPU_TYPE != CPU_TYPE_32)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
#endif
                *(sint32*)signalDataPtr_pv = (sint32)COM_GET_SIGTYPEUINT32_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
#if (CPU_TYPE != CPU_TYPE_32)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
#endif
            }
            break;
#ifdef COM_RXSIG_INT64
        case COM_SINT64:
            {
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                *(sint64*)signalDataPtr_pv = (sint64)COM_GET_SIGTYPEUINT64_BUFF(idRxMainFunc_uo, idxSigBuff_uo);
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
            }
            break;
#endif /* #ifdef COM_RXSIG_INT64 */

        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
            break;
    }
}
