

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
#ifdef COM_TxInvalid
LOCAL_INLINE uint8 Com_Prv_InternalInvalidateSignal(Com_SignalIdType idSignal_u16);
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
 Function name    : Com_InvalidateSignal
 Description      : Service for Invalidating the signal
 Parameter        : idSignal_u16 -> Id of the signal
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
uint8 Com_InvalidateSignal(Com_SignalIdType idSignal_u16)
{
#if defined(COM_TxInvalid) || defined(COM_TxGrpSigInvalid)
    uint8                       status_u8;

    status_u8 = COM_SERVICE_NOT_AVAILABLE;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_InvalidateSignal, COM_E_UNINIT);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
# ifdef COM_TxInvalid
    if(Com_Prv_IsValidTxSignalId(idSignal_u16))
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idSignal_u16    = COM_GET_TXSIGNAL_ID(idSignal_u16);

        status_u8       = Com_Prv_InternalInvalidateSignal(idSignal_u16);
    }
    else
# endif
# ifdef COM_TxGrpSigInvalid
    if(Com_Prv_IsValidTxGroupSignalId(idSignal_u16))
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idSignal_u16    = COM_GET_TXGRPSIGNAL_ID(idSignal_u16);

        status_u8       = Com_Prv_InternalInvalidateShadowSignal(idSignal_u16);
    }
    else
# endif
    {
        COM_DET_REPORT_ERROR(COMServiceId_InvalidateSignal, COM_E_PARAM);
    }

    return (status_u8);
#else
    (void)idSignal_u16;
    return( COM_SERVICE_NOT_AVAILABLE );
#endif /* #if defined(COM_TxInvalid) || defined(COM_TxGrpSigInvalid) */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/*
 **********************************************************************************************************************
 Function name    : Com_Prv_InternalInvalidateSignal
 Description      : Service for Invalidating the signal
 Parameter        : idSignal_u16 -> Id of the signal
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#ifdef COM_TxInvalid
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
LOCAL_INLINE uint8 Com_Prv_InternalInvalidateSignal(Com_SignalIdType idSignal_u16)
{
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    PduIdType                   idIpdu_uo;
    PduLengthType               byteNo_uo;
    uint8                       status_u8;
    /* BSW-10917 */ /* [$DD_BSWCODE 26612] */
    Com_SigMax_tuo   txSigInvValuePerType = 0u; //create the longest variable to hold the data to be send to Com_InternalSendSignal
    Com_SigMax_tuo   txSigInvValue;
    uint32 txSigInvValue_32;
    void *SignalDataPtr = (void *)&txSigInvValuePerType; /*PRQA S 0314 EOF # Cast from a pointer to object type to a pointer to void*/
    /* END BSW-10917 */

    status_u8           = COM_SERVICE_NOT_AVAILABLE;

    txSigConstPtr_pcst  = COM_GET_TXSIG_CONSTDATA(idSignal_u16);

    idIpdu_uo           = txSigConstPtr_pcst->idComIPdu_uo;

    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);

    /* The AUTOSAR COM module shall only process Com_InvalidateSignal only if a ComSignalDataInvalidValue is
     * configured for the signal with the given SignalId.
     * Copy the Invalid value to all the Bytes of this signal in Ipdu buffer */
    if (Com_GetValue(TXSIG,_INVACTION,txSigConstPtr_pcst->txSignalFields_u16)) /*PRQA S 3344 EOF # Controlling expression is not an 'essentially Boolean' expression*/
    {
#ifdef COM_TP_IPDUTYPE
        /* Proceed only if Large Data tranmission is not in Progress */
        if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,COM_GET_TXPDURAM_S(idIpdu_uo).txFlags_u16))
        {
            /* The TxIPdu transmission of large Data IPdu is in progress, hence no signal update is allowed until
             * the transmission is completed. */
            status_u8 = COM_BUSY;
        }
        else
#endif /*#ifdef COM_TP_IPDUTYPE*/
        {
            uint8 type_u8;

            type_u8 = Com_GetValue(GEN,_TYPE,txSigConstPtr_pcst->general_u8);

            /* After invaliding the signal data, this function shall perform a call of Com_SendSignal internally.
             * But as per design, Com_Prv_InternalSendSignal() is called with the signal Id and pass Address of Data.
             * Checking if the IpduGroup is started is checked in Com_Prv_InternalSendSignal() so no need of checking
             * it here. */
            if(type_u8 != (uint8)COM_UINT8_N)
            {
                /* BSW-10917 */ /* [$DD_BSWCODE 26612] */
                if(type_u8 != (uint8)COM_SINT64)
                {
                txSigInvValue = (Com_SigMax_tuo)txSigConstPtr_pcst->dataInvalidVal_u32;
                    switch (type_u8) {
                    case COM_UINT8:
                        {
                            *(uint8*) SignalDataPtr = (uint8) txSigInvValue; /*PRQA S 0316 EOF # Cast from a pointer to void to a pointer to object type*/
                        }
                        break;
                    case COM_UINT16:
                        {
                            *(uint16*) SignalDataPtr = (uint16) txSigInvValue;
                        }
                        break;
                    case COM_UINT32:
                        {
                            *(uint32*) SignalDataPtr = (uint32) txSigInvValue;
                        }
                        break;
#ifdef COM_TXSIG_INT64
                    case COM_UINT64:
                        {
                            *(uint64*)SignalDataPtr = (uint64) txSigInvValue;
                        }
                        break;
#endif /* #ifdef COM_TXSIG_INT64 */
                    case COM_BOOLEAN:
                        {
                            *(boolean*) SignalDataPtr = (boolean) (txSigInvValue != 0u); /*PRQA S 4304 # An expression of 'essentially Boolean' type (%1s) is being cast to unsigned type '%2s'*/
                        }
                        break;
                    case COM_SINT8:
                        {
                            *(sint8*)SignalDataPtr =  (sint8)txSigInvValue;
                        }
                        break;
                    case COM_SINT16:
                        {
                            *(sint16*)SignalDataPtr =  (sint16)txSigInvValue;
                        }
                        break;
                    case COM_SINT32:
                        {
                            *(sint32*)SignalDataPtr =  (sint32)txSigInvValue;
                        }
                        break;
#ifdef COM_FLOAT32SUPP
                    case COM_FLOAT32:
                        {
                            txSigInvValue_32 = (uint32)txSigInvValue;
                            (void)rba_BswSrv_MemCopy( SignalDataPtr, (const void *) &txSigInvValue_32, COM_F32_SIZE_IN_BYTES ); /*PRQA S 0314 # Cast from a pointer to object type to a pointer to void*/
                        }
                        break;
#endif /* #ifdef COM_FLOAT32SUPP */
                        case COM_FLOAT64:                       
                        default: /*PRQA S 2016 # This 'switch' statement 'default' clause is empty */
                            break; 
                        }
                }
                else
                {
                    /*Type COM_SINT64*/
                    *(sint64*)SignalDataPtr =  (sint64) ((sint32) txSigConstPtr_pcst->dataInvalidVal_u32);  
                }
                /* END BSW-10917 */

                /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_Prv_InternalSendSignal()
                 * definition.
                 * But the void pointer is always deferenced to a same/lesser integer type & it is 32bit aligned.
                 * Hence the alignment warning can safely be ignored. */
                /* BSW-10917 */
                status_u8 = Com_Prv_InternalSendSignal( idSignal_u16, (const void *)SignalDataPtr );

            }
            else
            {
                /*Find the byteNo_uo of the IPDU buffer*/
                byteNo_uo = (PduLengthType)(txSigConstPtr_pcst->bitPos_uo >> 3u);
                /* Enter exclusive area here */
                SchM_Enter_Com_TxIpduProtArea(INVALIDATESIGNAL);

                Com_ByteCopyInit((txIpduConstPtr_pcst->buffPtr_pu8 + byteNo_uo), txSigConstPtr_pcst->dataInvalidVal_u32,
                                                                     txSigConstPtr_pcst->bitSize_uo);

                SchM_Exit_Com_TxIpduProtArea(INVALIDATESIGNAL);
                /* Exit exclusive area here */

                /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_Prv_InternalSendSignal()
                 * definition.
                 * But the void pointer is always deferenced to a same integer type. Hence the alignment
                 * warning can safely be ignored. */
                status_u8 = Com_Prv_InternalSendSignal( idSignal_u16, (const void *)(txIpduConstPtr_pcst->buffPtr_pu8 + byteNo_uo) );

            }
        }
    }
    else
    {
        /* Return COM_SERVICE_NOT_AVAILABLE if ComSignalDataInvalidValue is not configured with the given SignalId*/
    }

    /* Note: if IPdu Group is stopped, then the Com_InternalSendSignal( ) will return COM_SERVICE_NOT_AVAILABLE */
    return (status_u8);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif

