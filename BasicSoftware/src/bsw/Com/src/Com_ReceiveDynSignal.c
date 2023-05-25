

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


#if defined(COM_RX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT)
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

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
LOCAL_INLINE uint8  Com_Prv_ReceiveDynSignal(Com_SignalIdType idSignal_u16, void * signalDataPtr_pv, uint16 * lengthPtr_pu16);
LOCAL_INLINE uint16 Com_Prv_RetrieveDynamicSignalLength(Com_RxSigCfg_tpcst rxSigConstPtr_pcst);
#endif

#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
LOCAL_INLINE uint8  Com_Prv_ReceiveDynGroupSignal(Com_SignalIdType idSignal_u16, void * signalDataPtr_pv, uint16 * lengthPtr_pu16);
#endif

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
 Function name    : Com_ReceiveDynSignal
 Description      : Service for receiving the signal, This API copies the data to the application buffer
 Parameter        : idSignal_u16            -> Id of the Signal
                  : signalDataPtr_pv        -> Pointer to the address where signal data needs to be copied.
                  : lengthPtr_pu16          -> Pointer to length of the recieved length.
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(SignalDataPtr) in a function prototype should be declared as pointer
   to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is
   as below, hence suppressed */
uint8 Com_ReceiveDynSignal(Com_SignalIdType idSignal_u16, void * signalDataPtr_pv, uint16 * lengthPtr_pu16)
{

    uint8                       status_u8;        /* Return status */

    /* Returns with COM_SERVICE_NOT_AVAILABLE when IPDU Group is Stopped or
     * DET error occurs or when Type of the signal is not configured with UINT8_DYN */
    status_u8 = COM_SERVICE_NOT_AVAILABLE;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveDynSignal, COM_E_UNINIT);
    }
    else if ((signalDataPtr_pv == NULL_PTR) || (lengthPtr_pu16 == NULL_PTR))
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveDynSignal, COM_E_PARAM_POINTER);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
    /* Check for dynamic signal */
    if(Com_Prv_IsValidRxSignalId(idSignal_u16))
    {
        status_u8 = Com_Prv_ReceiveDynSignal(idSignal_u16, signalDataPtr_pv, lengthPtr_pu16);
    }
    else
#endif
#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
    /* Check for dynamic group signal */
    if(Com_Prv_IsValidRxGroupSignalId(idSignal_u16))
    {
        status_u8 = Com_Prv_ReceiveDynGroupSignal(idSignal_u16, signalDataPtr_pv, lengthPtr_pu16);
    }
    else
#endif
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveDynSignal, COM_E_PARAM);
    }

    return(status_u8);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ReceiveDynSignal
 Description      : Service for receiving the dynamic signal, This API copies the data to the application buffer
 Parameter        : idSignal_u16        -> Id of the Signal
                  : signalDataPtr_pv    -> Pointer to the address where signal data needs to be copied.
                  : lengthPtr_pu16      -> Pointer to length of the received length.
 Return value     : E_OK/E_NOT_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(signalDataPtr_pv) in a function prototype should be declared as pointer
   to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is
   as below, hence suppressed */
LOCAL_INLINE uint8 Com_Prv_ReceiveDynSignal(Com_SignalIdType idSignal_u16, void* signalDataPtr_pv, uint16 * lengthPtr_pu16)
{

    Com_RxSigCfg_tpcst          rxSigConstPtr_pcst; /* Local pointer holds the address of Rx signal static configuration */
    uint16                      dynLength_u16;     /* Received Dynamic Signal Length  */
    Com_SigBuffIndex_tuo        idxSigBuff_uo;
    uint8                       type_u8;          /* Local variable holds the signal type */
    uint8                       status_u8;        /* Return status */

    /* Initialize variable */
    status_u8  = COM_SERVICE_NOT_AVAILABLE;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
     * to internal Id which is generated through configuration
     * If PC variant is selected, then no mapping table will be used. */
    idSignal_u16        = COM_GET_RXSIGNAL_ID(idSignal_u16);

    rxSigConstPtr_pcst  = COM_GET_RXSIG_CONSTDATA(idSignal_u16);
    idxSigBuff_uo       = rxSigConstPtr_pcst->idxSigBuff_uo;
    type_u8             = Com_GetValue(GEN,_TYPE,rxSigConstPtr_pcst->general_u8);

    /* Check if Ipdu Group is started */
    if (Com_Prv_CheckRxIPduStatus((PduIdType)rxSigConstPtr_pcst->idComIPdu_uo) && (COM_UINT8_DYN == type_u8))
    {
        /* Retrieve dynamic signal length */
        dynLength_u16 = Com_Prv_RetrieveDynamicSignalLength(rxSigConstPtr_pcst);

        /* If the requested length is less than the received length, then return E_NOT_OK */
        if (*lengthPtr_pu16 >= dynLength_u16)
        {
            /*Copy each byte into the SignalDataPtr from Signal buffer  */
            /* The locks inside the function Com_ByteCopy() are removed.
             * Hence locks are used here
             */
            SchM_Enter_Com_RxSigDynBuff();
            * lengthPtr_pu16 = dynLength_u16;
            if (COM_ZERO != dynLength_u16)
            {
                Com_MainFunc_tuo idRxMainFunc_uo;

                /* Fetch the Rx-MainFunction internal Id */
                idRxMainFunc_uo = (COM_GET_RX_IPDU_CONSTDATA(rxSigConstPtr_pcst->idComIPdu_uo))->idMainFunc_uo;

                /* MR12 RULE 11.5 VIOLATION: For SignalDataPtr is defined as void pointer in the AUTOSAR
                 * specification, so suppressing warning "Cast from a pointer to void to a pointer to object" */
                Com_ByteCopy((uint8*)signalDataPtr_pv,
                                &COM_GET_SIGTYPEUINT8DYN_BUFF(idRxMainFunc_uo,idxSigBuff_uo),dynLength_u16);

            }
            SchM_Exit_Com_RxSigDynBuff();

            /* Positive condition hence status_u8 as E_OK */
            status_u8 = E_OK;
        }
        else
        {
            status_u8 = E_NOT_OK;
        }
    }
    return(status_u8);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT */

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT

/*
 **********************************************************************************************************************
 Function name    : Com_Prv_RetrieveDynamicSignalLength
 Description      : Service for retrieve dynamic signal length
 Return value     : COM_ZERO
                    Dynamic signal length
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
LOCAL_INLINE uint16 Com_Prv_RetrieveDynamicSignalLength(Com_RxSigCfg_tpcst rxSigConstPtr_pcst)
{
    PduLengthType                   startByteNo_uo;   /* Start Byte Number of Dynamic Signal */
    uint16                          dynLength_u16;     /* Received Dynamic Signal Length  */

    dynLength_u16 = COM_ZERO;

    /* Calculate Rx Dynamic Signal Start Position */
    startByteNo_uo = ( rxSigConstPtr_pcst->bitPos_uo >> 3u );

    /* Check if Dynamic Signal has been received with atleast one byte */
    if (COM_GET_RXPDURAM_S(rxSigConstPtr_pcst->idComIPdu_uo).rxIPduLength_uo > startByteNo_uo)
    {
        /* Calculate its received length */
        dynLength_u16 = (uint16)(COM_GET_RXPDURAM_S(rxSigConstPtr_pcst->idComIPdu_uo).rxIPduLength_uo - startByteNo_uo);
    }
    else
    {
        /* Report it as 0, if it is not received yet */
    }

    return dynLength_u16;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT */

#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ReceiveDynGroupSignal
 Description      : Service for receiving the dynamic group signal, This API copies the data to the application buffer
 Parameter        : idSignal_u16        -> Id of the Signal
                  : signalDataPtr_pv    -> Pointer to the address where group signal data needs to be copied.
                  : lengthPtr_pu16      -> Pointer to length of the received length.
 Return value     : E_OK/E_NOT_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(signalDataPtr_pv) in a function prototype should be declared as pointer
   to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is
   as below, hence suppressed */
LOCAL_INLINE uint8 Com_Prv_ReceiveDynGroupSignal(Com_SignalIdType idSignal_u16, void * signalDataPtr_pv, uint16 * lengthPtr_pu16)
{

    /* Local pointer holds the address of Rx group signal static configuration */
    Com_RxIpduRam_tpst              rxIpduRamPtr_pst;
    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst;
    uint16                          dynLength_u16;     /* Received Dynamic Signal Length  */
    PduIdType                       idIpdu_uo;
    Com_RxGrpSigBuffIndex_tuo       idxRxGrpSignalBuff_uo;
    uint8                           type_u8;          /* Local variable holds the signal type */
    uint8                           status_u8;        /* Return status */

    /* Initialize variable */
    status_u8               = COM_SERVICE_NOT_AVAILABLE;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
     * to internal Id which is generated through configuration
     * If PC variant is selected, then no mapping table will be used. */
    idSignal_u16            = COM_GET_RXGRPSIGNAL_ID(idSignal_u16);
    rxGrpSigConstPtr_pcst   = COM_GET_RXGRPSIG_CONSTDATA(idSignal_u16);
    idxRxGrpSignalBuff_uo   = rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo;
    idIpdu_uo               = COM_GET_RXSIGGRP_CONSTDATA(rxGrpSigConstPtr_pcst->idSigGrp_uo)->idComIPdu_uo;
    type_u8                 = Com_GetValue(RXGRPSIG,_TYPE,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8);
    rxIpduRamPtr_pst        = &COM_GET_RXPDURAM_S(idIpdu_uo);

    /* Check if Ipdu Group is started */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
     * to store and read here, may read obsolete value. Evaluation(or no evaluation) of 2nd operand
     * shall not impact the system behaviour. Hence suppressed. */
    if (Com_Prv_CheckRxIPduStatus(idIpdu_uo) && (COM_UINT8_DYN == type_u8) &&
        Com_GetValue(RXGRPSIG,_ISDYNGRPSIG,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8))
    {
        /* Retrieve dynamic group signal length */
        dynLength_u16 = rxIpduRamPtr_pst->dynRxGrpSigLength_u16;

        /* If the requested length is less than the received length, then return E_NOT_OK */
        if (*lengthPtr_pu16 >= dynLength_u16)
        {
            /*Copy each byte into the SignalDataPtr from Signal buffer  */
            /* The locks inside the function Com_ByteCopy() are removed.
             * Hence locks are used here
             */
            SchM_Enter_Com_RxGrpSigDynBuff();
            * lengthPtr_pu16 = dynLength_u16;
            if (COM_ZERO != dynLength_u16)
            {
                Com_MainFunc_tuo idRxMainFunc_uo;

                /* Fetch the Rx-MainFunction internal Id */
                idRxMainFunc_uo = (COM_GET_RX_IPDU_CONSTDATA((COM_GET_RXSIGGRP_CONSTDATA(rxGrpSigConstPtr_pcst->idSigGrp_uo))->idComIPdu_uo))->idMainFunc_uo;

                /* MR12 RULE 11.5 VIOLATION: For signalDataPtr_pv is defined as void pointer in the AUTOSAR
                 * specification, so suppressing warning "Cast from a pointer to void to a pointer to object" */
                Com_ByteCopy((uint8*)signalDataPtr_pv,
                              &COM_GET_RXGRPSIGTYPEUINT8DYN_BUFF(idRxMainFunc_uo,idxRxGrpSignalBuff_uo),dynLength_u16);

            }
            SchM_Exit_Com_RxGrpSigDynBuff();

            /* Positive condition hence status as E_OK */
            status_u8 = E_OK;
        }
        else
        {
            status_u8 = E_NOT_OK;
        }
    }
    return(status_u8);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT */

#endif /* #if defined(COM_RX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT) */

