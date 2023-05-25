

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


#ifdef COM_RX_SIGNALGROUP
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

LOCAL_INLINE void Com_Prv_CopyData_GrpSig_UnsignedType(
                                            Com_SignalIdType        idIntSignal_u16,
                                            void *                  signalDataPtr_pv,
                                            Com_MainFunc_tuo        idRxMainFunc_uo
                                               );

LOCAL_INLINE void Com_Prv_CopyData_GrpSig_SignedType(
                                            Com_SignalIdType        idIntSignal_u16,
                                            void *                  signalDataPtr_pv,
                                            Com_MainFunc_tuo        idRxMainFunc_uo
                                             );


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
 Function name    : Com_ReceiveShadowSignal
 Description      : Service for receiving the shadow Signal
 Parameter        : idSignal_u16        -> Id of the signal
                  : signalDataPtr_pv    -> Pointer to the the signal data
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(signalDataPtr_pv) in a function prototype should be declared as pointer
   to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is as
   below, hence suppressed */
void Com_ReceiveShadowSignal(Com_SignalIdType idSignal_u16, void * signalDataPtr_pv)
{

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveShadowSignal, COM_E_UNINIT);
    }
    else if (signalDataPtr_pv == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveShadowSignal, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidRxGroupSignalId(idSignal_u16))
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveShadowSignal, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idSignal_u16    = COM_GET_RXGRPSIGNAL_ID(idSignal_u16);

        Com_Prv_ReceiveShadowSignal(idSignal_u16, signalDataPtr_pv);
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ReceiveShadowSignal
 Description      : Service for receiving the shadow Signal
 Parameter        : idSignal_u16      -> Id of the signal
                  : signalDataPtr_pv  -> Pointer to the the signal data
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(signalDataPtr_pv) in a function prototype should be declared as pointer
   to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is as
   below, hence suppressed */
void Com_Prv_ReceiveShadowSignal(Com_SignalIdType idSignal_u16, void * signalDataPtr_pv)
{

    /* Local Variables */
    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst;
    uint8                           sigType_u8;
    Com_MainFunc_tuo                idRxMainFunc_uo;

    rxGrpSigConstPtr_pcst   = COM_GET_RXGRPSIG_CONSTDATA(idSignal_u16);

    sigType_u8              = Com_GetValue(RXGRPSIG,_TYPE,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8);

    /* Fetch the Rx-MainFunction internal Id */
    idRxMainFunc_uo = (COM_GET_RX_IPDU_CONSTDATA((COM_GET_RXSIGGRP_CONSTDATA(rxGrpSigConstPtr_pcst->idSigGrp_uo))->idComIPdu_uo))->idMainFunc_uo;

    /* The last known value has to be given back even if IPDU Group is stopped
     * So No need of checking Ipdu Group Status */
    if((sigType_u8 % COM_TWO) == COM_ZERO)
    {
        /* Copy Data of Unsigned Signal Types */
        Com_Prv_CopyData_GrpSig_UnsignedType(idSignal_u16, signalDataPtr_pv, idRxMainFunc_uo);
    }
    else
    {
        /* Copy Data of Signed Signal Types */
        Com_Prv_CopyData_GrpSig_SignedType(idSignal_u16, signalDataPtr_pv, idRxMainFunc_uo);
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CopyData_GrpSig_UnsignedType
 Description      : Service to copy the data for UnSigned Signal type
 Parameter        : idIntSignal_u16    -> Id of the signal
                    signalDataPtr_pv   -> Pointer to the the signal data
                    idRxMainFunc_uo    -> MainFunction Id to access GroupSignal buffer
 Return value     : None
 **********************************************************************************************************************
*/
/* MR12 RULE 8.13 VIOLATION: This is a false positive warning, 'signalDataPtr_pv' is modified and so the pointer cannot be
   changed to type 'pointer to const'. Hence warning is suppressed. */
LOCAL_INLINE void Com_Prv_CopyData_GrpSig_UnsignedType(
                                            Com_SignalIdType        idIntSignal_u16,
                                            void *                  signalDataPtr_pv,
                                            Com_MainFunc_tuo        idRxMainFunc_uo
                                               )
{

    /* Local Variables */
    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst;
    Com_RxGrpSigBuffIndex_tuo       idxGrpSigBuff_uo;
    uint8                           sigType_u8;

    rxGrpSigConstPtr_pcst   = COM_GET_RXGRPSIG_CONSTDATA(idIntSignal_u16);

    sigType_u8              = Com_GetValue(RXGRPSIG,_TYPE,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8);
    idxGrpSigBuff_uo        = rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo;

    /* Copy GroupSignal data from shadow buffer into SignalDataPtr_pv */
    switch( sigType_u8 )
    {
        case COM_UINT8:
        {
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(uint8*)signalDataPtr_pv = COM_GET_RXGRPSIGTYPEUINT8_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);

        }
        break;
        case COM_UINT16:
        {
#if (CPU_TYPE == CPU_TYPE_8)
            SchM_Enter_Com_RxGrpSigBuff();
#endif
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(uint16*)signalDataPtr_pv = COM_GET_RXGRPSIGTYPEUINT16_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);

#if (CPU_TYPE == CPU_TYPE_8)
            SchM_Exit_Com_RxGrpSigBuff();
#endif
        }
        break;
        case COM_UINT32:
        {
#if (CPU_TYPE != CPU_TYPE_32)
            SchM_Enter_Com_RxGrpSigBuff();
#endif
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(uint32*)signalDataPtr_pv = COM_GET_RXGRPSIGTYPEUINT32_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);

#if (CPU_TYPE != CPU_TYPE_32)
            SchM_Exit_Com_RxGrpSigBuff();
#endif
        }
        break;

#ifdef COM_RXGRPSIG_INT64
        case COM_UINT64:
        {
            SchM_Enter_Com_RxGrpSigBuff();
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification,
             * to support different groupsignal types. On call to this api, application has to pass a valid datatype
             * pointer address. Hence this warning can safely be ignored.
             */
            *(uint64*)signalDataPtr_pv = COM_GET_RXGRPSIGTYPEUINT64_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);

            SchM_Exit_Com_RxGrpSigBuff();
        }
        break;
#endif /* #ifdef COM_RXGRPSIG_INT64 */

        case COM_BOOLEAN:
        {
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification,
             * to support different groupsignal types. On call to this api, application has to pass a valid datatype
             * pointer address. Hence this warning can safely be ignored.
             */
            *(boolean*)signalDataPtr_pv = (COM_GET_RXGRPSIGTYPEUINT8_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo) != COM_ZERO);

        }
        break;

/* FC_VariationPoint_START */
#ifdef COM_GRPSIGFLOAT32SUPP
        case COM_FLOAT32:
        {
            uint32 rxGrpSigNewVal_u32;

#if (CPU_TYPE != CPU_TYPE_32)
            SchM_Enter_Com_RxGrpSigBuff();
#endif
            rxGrpSigNewVal_u32 = COM_GET_RXGRPSIGTYPEUINT32_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);
#if (CPU_TYPE != CPU_TYPE_32)
            SchM_Exit_Com_RxGrpSigBuff();
#endif
            /* Convert the 32 bit stream into float value */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
             * But the void pointer is always deferenced to a type, based on the pointers alignment.
             * Hence the alignment warning can safely be ignored.
             */
           (void)rba_BswSrv_MemCopy(signalDataPtr_pv,(const void *)&rxGrpSigNewVal_u32,COM_F32_SIZE_IN_BYTES);

        }
        break;
#endif /* COM_GRPSIGFLOAT32SUPP */

#ifdef COM_RXGRPSIG_FLOAT64SUPP
        case COM_FLOAT64:
        {
            SchM_Enter_Com_RxGrpSigBuff();
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification,
             * to support different groupsignal types. On call to this api, application has to pass a valid datatype
             * pointer address. Hence this warning can safely be ignored.
             */
            *(float64*)signalDataPtr_pv = COM_GET_RXGRPSIGTYPEFLOAT64_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);

            SchM_Exit_Com_RxGrpSigBuff();
        }
        break;
#endif
/* FC_VariationPoint_END */
       case COM_UINT8_N:
        {
            SchM_Enter_Com_RxGrpSigBuff();

            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification,
             * to support different groupsignal types. On call to this api, application has to pass a valid datatype
             * pointer address. Hence this warning can safely be ignored. */
            Com_ByteCopy( (uint8*)signalDataPtr_pv, &(COM_GET_RXGRPSIGTYPEUINT8_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo)),
                          (uint32)rxGrpSigConstPtr_pcst->bitSize_uo);

            SchM_Exit_Com_RxGrpSigBuff();
        }
        break;
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
 Function name    : Com_Prv_CopyData_GrpSig_SignedType
 Description      : Service to copy the data for Signed Signal type
 Parameter        : idIntSignal_u16  -> Id of the signal
                    signalDataPtr_pv -> Pointer to the the signal data
                    idRxMainFunc_uo  -> MainFunction Id to access GroupSignal buffer
 Return value     : None
 **********************************************************************************************************************
*/
/* MR12 RULE 8.13 VIOLATION: This is a false positive warning, 'signalDataPtr_pv' is modified and so the pointer cannot be
   changed to type 'pointer to const'. Hence warning is suppressed. */
LOCAL_INLINE void Com_Prv_CopyData_GrpSig_SignedType(
                                          Com_SignalIdType      idIntSignal_u16,
                                          void *                signalDataPtr_pv,
                                          Com_MainFunc_tuo      idRxMainFunc_uo
                                             )
{

    /* Local Variables */
    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst;
    Com_RxGrpSigBuffIndex_tuo       idxGrpSigBuff_uo;
    uint8                           sigType_u8;

    rxGrpSigConstPtr_pcst = COM_GET_RXGRPSIG_CONSTDATA(idIntSignal_u16);

    sigType_u8          = Com_GetValue(RXGRPSIG,_TYPE,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8);
    idxGrpSigBuff_uo    = rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo;

    /* Copy GroupSignal data from shadow buffer into signalDataPtr_pv */
    switch( sigType_u8 )
    {
        case COM_SINT8:
        {
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored. */
            *(sint8*)signalDataPtr_pv = (sint8)COM_GET_RXGRPSIGTYPEUINT8_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);

        }
        break;
        case COM_SINT16:
        {
#if (CPU_TYPE == CPU_TYPE_8)
            SchM_Enter_Com_RxGrpSigBuff();
#endif
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(sint16*)signalDataPtr_pv = (sint16)COM_GET_RXGRPSIGTYPEUINT16_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);

#if (CPU_TYPE == CPU_TYPE_8)
            SchM_Exit_Com_RxGrpSigBuff();
#endif
        }
        break;
        case COM_SINT32:
        {
#if (CPU_TYPE != CPU_TYPE_32)
            SchM_Enter_Com_RxGrpSigBuff();
#endif
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(sint32*)signalDataPtr_pv = (sint32)COM_GET_RXGRPSIGTYPEUINT32_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);

#if (CPU_TYPE != CPU_TYPE_32)
            SchM_Exit_Com_RxGrpSigBuff();
#endif
        }
        break;

#ifdef COM_RXGRPSIG_INT64
        case COM_SINT64:
        {
            SchM_Enter_Com_RxGrpSigBuff();
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pv is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(sint64*)signalDataPtr_pv = (sint64)COM_GET_RXGRPSIGTYPEUINT64_BUFF(idRxMainFunc_uo,idxGrpSigBuff_uo);

            SchM_Exit_Com_RxGrpSigBuff();
        }
        break;
#endif /* #ifdef COM_RXGRPSIG_INT64 */

        default:
       /**
         * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
         * to be configured, hence default case is intentionally left empty.
         */
       break;
    }
}
#endif /* #ifdef COM_RX_SIGNALGROUP */

