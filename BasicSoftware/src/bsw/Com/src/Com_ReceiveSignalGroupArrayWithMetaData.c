

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

#ifdef COM_ECUC_AR_RELEASE_AR45

/* Com_ReceiveSignalGroupArrayWithMetaData() API shall only be available when the configuration switch
 * ComEnableSignalGroupArrayApi and ComMetaDataSupport are enabled */
#if defined(COM_RX_SIGNALGROUP_ARRAY) && defined(COM_METADATA_SUPPORT)

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
 * Functions
 **********************************************************************************************************************
*/
/*
 **********************************************************************************************************************
 * Function name    : Com_ReceiveSignalGroupArrayWithMetaData
 * Description      : The service Com_ReceiveSignalGroupArrayWithMetaData copies the received signal group array
 *                    representation from the PDU to the location designated by SignalGroupArrayPtr and received
 *                    metadata is stored at the position given by MetaDataPtr parameter.
 * Parameters   IN  : idSignalGroup_u16 - Id of signal group to be received.
                OUT : signalGroupArrayPtr_pu8 - reference to the location where the
                                           received signal group array shall be stored
                OUT : MetaDataPtr_pu8 - reference to the location where the
                                           received metadata shall be stored
 * Sync/Async       : Synchronous
 * Reentrancy       : Non Reentrant for the same signal group. Reentrant for different signal groups.
 * Return value     : E_OK                      : service has been accepted
 *                    COM_SERVICE_NOT_AVAILABLE : corresponding I-PDU group was stopped
 *                                                (or service failed due to development error)
 **********************************************************************************************************************
*/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"

/* MR12 RULE 8.13 VIOLATION: A pointer parameter(signalGroupArrayPtr_pu8) in a function prototype should be declared as
   pointer to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature
   is as below, hence suppressed */
uint8 Com_ReceiveSignalGroupArrayWithMetaData(Com_SignalGroupIdType idSignalGroup_u16, uint8 * signalGroupArrayPtr_pu8,
                                                                                                uint8* metaDataPtr_pu8)
{

    Com_RxSigGrpCfg_tpcst       rxSigGrpConstPtr_pcst; /* Local pointer to Rx SignalGroup static configuration */
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    uint8                       returnValue_u8;
    Com_MainFunc_tuo            idRxMainFunc_uo;
    Com_IpduId_tuo              idIpdu_uo;

    /* Return COM_SERVICE_NOT_AVAILABLE code for the below conditions:
     * - DET Error is detected
     * - Corresponding IPduGroup is stopped
     * - SignalGroup is not configured with ArrayAccess */
    returnValue_u8 = COM_SERVICE_NOT_AVAILABLE;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveSignalGroupArray, COM_E_UNINIT);
    }
    else if ((signalGroupArrayPtr_pu8 == NULL_PTR) || (metaDataPtr_pu8 == NULL_PTR))
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveSignalGroupArray, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidRxSignalGroupId(idSignalGroup_u16))
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveSignalGroupArray, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idSignalGroup_u16     = COM_GET_RXSIGNALGRP_ID( idSignalGroup_u16 );

        rxSigGrpConstPtr_pcst = COM_GET_RXSIGGRP_CONSTDATA( idSignalGroup_u16 );

        idIpdu_uo             = rxSigGrpConstPtr_pcst->idComIPdu_uo;

        rxIpduConstPtr_pcst   = COM_GET_RX_IPDU_CONSTDATA(idIpdu_uo);

        /* Fetch the Rx-MainFunction internal Id */
        idRxMainFunc_uo     = (COM_GET_RX_IPDU_CONSTDATA(idIpdu_uo))->idMainFunc_uo;

        /* Check if ArrayAccess is enabled for this Rx-Signal Group */
        if (Com_GetValue(RXIPDU,_IS_METADATAPDU,rxIpduConstPtr_pcst->rxIPduFields_u8) &&
           (Com_GetValue(RXSIGGRP,_ARRAYACCESS,rxSigGrpConstPtr_pcst->rxSignalGrpFields_u8)))
        {
            Com_RxSigGrpArrayCfg_tpcst      rxSigGrpArrayConstPtr_pcst; /* Ptr to Rx SignalGroupArray static config */
            uint8 * rxSigGrpArrayBufRamPtr_pu8;

            rxSigGrpArrayConstPtr_pcst  = COM_GET_RXSIGGRP_ARRAY_CONSTDATA( rxSigGrpConstPtr_pcst->idxSigGrpArray_uo );

            rxSigGrpArrayBufRamPtr_pu8 =
                          &COM_GET_RXSIGGRP_ARRAY_BUFF( idRxMainFunc_uo,rxSigGrpArrayConstPtr_pcst->idxRxSigGrpBuf_uo );

            /**
             * A call to Com_ReceiveSignalGroupArray() on a Rx-SignalGroup that belongs to a stopped I-PDU
             * copy the last known data (or the init value) of the I-PDU to the signalGroupArrayPtr_pu8.
             */
            if ( Com_Prv_CheckRxIPduStatus((PduIdType)idIpdu_uo ) )
            {
                returnValue_u8 = E_OK;
            }

            SchM_Enter_Com_RxSigGrpArrayBuff();

            /**
             * Copy RxSignalGroup Data(uint8[n]) from IPdu Buffer into *signalGroupArrayPtr_pu8
             * Design: Here in Rx-SignalGroup, LastByte is generated as (LastByteNo + 1),
             *         so the Length is derived as just ( LastByte - FirstByte ) instead of ( LastByte - FirstByte + 1 )
             */
            Com_ByteCopy((uint8 *)(signalGroupArrayPtr_pu8),
                        (const uint8 *)(rxSigGrpArrayBufRamPtr_pu8),
                        (uint32)(rxSigGrpConstPtr_pcst->lastByte_uo - rxSigGrpArrayConstPtr_pcst->firstByteNo_uo));

            /* Copy the given RxMetaData internal buffer to Metadata pointer */
            Com_ByteCopy(metaDataPtr_pu8,
                        (rxIpduConstPtr_pcst->rxMetaDataPtr_pcst->RxmetaDataValue_pau8),
                         rxIpduConstPtr_pcst->rxMetaDataPtr_pcst->RxmetaDataLength_u8);

            SchM_Exit_Com_RxSigGrpArrayBuff();
        }
    }
    return returnValue_u8;
}


# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"

#endif /* #if defined(COM_RX_SIGNALGROUP_ARRAY) && defined(COM_METADATA_SUPPORT) */

#endif


