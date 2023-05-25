

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Inl.h"
#include "CanIf.h"

#if (!defined(CANIF_AR_RELEASE_MAJOR_VERSION) || (CANIF_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANIF_AR_RELEASE_MINOR_VERSION) || (CANIF_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_Transmit
 Description      : Used by PduR to trigger a spontaneous transmission of an NM PDU with the provided
                    NM User Data
 Parameter        : CanNmTxPduId - Handle of the CanNm user data PDU
                  : PduInfoPtr   - Pointer to a structure containing the pointer to the NM user data buffer
                                   and the related DLC.
 Return value     : E_OK     - Transmit request has been accepted
                  : E_NOT_OK - Transmit request has not been accepted (CanNm is not in Repeat Message or
                               Normal Operation State)
 ***************************************************************************************************/

/* TRACE[SWS_CanNm_00330] : This API shall be provided if CanNmComUserDataSupport or CanNmPnEnabled is enabled */
#if((CANNM_COM_USER_DATA_SUPPORT != STD_OFF) || (CANNM_PN_ENABLED != STD_OFF))

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_Transmit(
                                              PduIdType CanNmTxPduId,
                                              const PduInfoType * PduInfoPtr
                                             )
{
    /* pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Pointer to Tx buffer */
    uint8 * TxBufferPtr;

    /* Pointer to user data */
    uint8 * UserDataPtr;

    /* Index of the User bytes in Tx buffer */
    uint8 UserDataOffset;

    /* Return value of the API */
    Std_ReturnType RetVal_en;

    /* Initialize the return value*/
    RetVal_en = E_NOT_OK;

    /********************************* Start: DET *************************************/
    /* Report DET if TxPduId is Invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNmTxPduId >= CANNM_NUMBER_OF_CHANNELS),
                            (uint8)CanNmTxPduId, CANNM_SID_TRANSMIT, CANNM_E_INVALID_PDUID)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNmTxPduId].State_en == NM_STATE_UNINIT),
                            (uint8)CanNmTxPduId, CANNM_SID_TRANSMIT, CANNM_E_NO_INIT)

    /* Report DET if received pointer is a null pointer*/
    CANNM_DET_REPORT_ERROR_NOK(((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR)),
            (uint8)CanNmTxPduId, CANNM_SID_TRANSMIT, CANNM_E_PARAM_POINTER)

    /*********************************  End: DET  *************************************/

    /* Initialize pointer to configuration structure */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNmTxPduId);

    /* Initialize pointer to RAM structure */
    RamPtr_ps = &CanNm_RamData_s[CanNmTxPduId];

#if(CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
	/* TRACE[SWS_CanNm_00333] : Additional transmission of NM PDU with provided user data */
    if ((RamPtr_ps->State_en == NM_STATE_REPEAT_MESSAGE) || (RamPtr_ps->State_en == NM_STATE_NORMAL_OPERATION))
    {

		/* Initialize user data offset */
        UserDataOffset = ConfigPtr_pcs->PduLength_u8 - ConfigPtr_pcs->UserDataLength_u8;

        /* Copy the data provided by upper layer into user data buffer */
        UserDataPtr = &(RamPtr_ps->UserDataBuffer_au8[0]);
        CanNm_CopyBuffer(PduInfoPtr->SduDataPtr,UserDataPtr,(uint8)PduInfoPtr->SduLength);

        /* Check if Control bit vector is enabled and update the CBV in the TxBuffer */
        if(ConfigPtr_pcs->ControlBitVectorPos_u8 != CANNM_PDU_OFF )
        {
            /* Initialize the local TxBuffer */
            TxBufferPtr = &(RamPtr_ps->TxBuffer_au8[0]);

            /* Store the Control bit vector to be transmitted in the TxBuffer */
            TxBufferPtr[ConfigPtr_pcs->ControlBitVectorPos_u8] = RamPtr_ps->TxCtrlBitVector_u8 ;
        }

        /* Update TxBuffer with user data information */
        TxBufferPtr = &(RamPtr_ps->TxBuffer_au8[UserDataOffset]);
        CanNm_CopyBuffer(UserDataPtr,TxBufferPtr,(uint8)PduInfoPtr->SduLength);

        /* Transmit the PDU */
        RetVal_en = CanNm_MessageTransmit(ConfigPtr_pcs,RamPtr_ps);

    }
#endif

    /* TRACE[SWS_CanNm_00333] : CanNm is in Ready Sleep State or Bus Sleep Mode or Prepare Bus Sleep Mode */
    return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


