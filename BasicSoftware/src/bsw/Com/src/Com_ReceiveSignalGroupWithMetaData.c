

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

#ifdef COM_ECUC_AR_RELEASE_AR45

#if defined(COM_RX_SIGNALGROUP) && defined(COM_METADATA_SUPPORT)

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
 Function name    : Com_ReceiveSignalGroupWithMetaData
 Description      : Service used to copy the SignalGroup content from IpduBuffer to Shadowbuffer and copy the metadata
                    into position specified by the MetaDataPtr parameter
 Parameter        : idSignalGroup_u16 -> Id of the SignalGroup
                  : metaDataPtr_pv    -> Pointer to the address where metadata data needs to be copied.
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

/* MR12 RULE 8.13 VIOLATION: A pointer parameter(signalDataPtr_pv) in a function prototype should be declared as pointer
   to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is as
   below, hence suppressed */
uint8 Com_ReceiveSignalGroupWithMetaData(Com_SignalGroupIdType idSignalGroup_u16, uint8* metaDataPtr_pu8)
{

    Com_RxSigGrpCfg_tpcst       rxSigGrpConstPtr_pcst;
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    uint8                       status_u8;

    status_u8 = COM_SERVICE_NOT_AVAILABLE;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveSignalGroup, COM_E_UNINIT);
    }
    else if (metaDataPtr_pu8 == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveSignalGroup, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidRxSignalGroupId(idSignalGroup_u16))
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceiveSignalGroup, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        if(idSignalGroup_u16 < COM_GET_NUM_RX_SIGNALGRP)
        {
            /* If PB variant is selected, then PduId which is passed to this function will be changed
             * to internal Id which is generated through configuration
             * If PC variant is selected, then no mapping table will be used. */
            idSignalGroup_u16     = COM_GET_RXSIGNALGRP_ID(idSignalGroup_u16);

            rxSigGrpConstPtr_pcst = COM_GET_RXSIGGRP_CONSTDATA(idSignalGroup_u16);

            rxIpduConstPtr_pcst   = COM_GET_RX_IPDU_CONSTDATA(rxSigGrpConstPtr_pcst->idComIPdu_uo);

            /* Check if received signal group is part of metadata I-PDU
             * If IPDU Group is Started, then update the status variable
             */
            if(Com_GetValue(RXIPDU,_IS_METADATAPDU,rxIpduConstPtr_pcst->rxIPduFields_u8) &&
              (Com_Prv_CheckRxIPduStatus((PduIdType)(rxSigGrpConstPtr_pcst->idComIPdu_uo))))
            {
                Com_Prv_ReceiveSignalGroup(idSignalGroup_u16);

                /* Copy the given RxMetaData internal buffer to Metadata pointer */
                Com_ByteCopy(metaDataPtr_pu8,
                            (rxIpduConstPtr_pcst->rxMetaDataPtr_pcst->RxmetaDataValue_pau8),
                             rxIpduConstPtr_pcst->rxMetaDataPtr_pcst->RxmetaDataLength_u8);

                status_u8 = E_OK;
            }
        }
        else
        {

        }
    }
    return (status_u8);
}


#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if defined(COM_RX_SIGNALGROUP) && defined(COM_METADATA_SUPPORT) */

#endif


