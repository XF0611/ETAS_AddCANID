/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "CanIf_Integration.h"
#include "Std_Types.h"
#include "CanIf_Prv.h"
#if((CANIF_FD_SUPPORT == STD_ON) && (CANIF_WRITE_INTEGRATION_VERSION < 1U))
#include "rba_BswSrv.h" /*for memcpy*/
#endif

/*Below is the sample implementation*/
/*This holds the buffer for L-PDU - should be configured by user. Currently configured as maximum length of CANFD: 64*/
#if((CANIF_FD_SUPPORT == STD_ON) && (CANIF_WRITE_INTEGRATION_VERSION < 1U))

// #define CANFD_PADDING_VALUE             0U

#define CANIF_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
// uint8 CanIf_Pdu_Buffer[64];
#define CANIF_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif

/********************************************************************************************************************
 Function name    : CanIf_Write_Integration

 Description      : This service uses to ensure that L-PDU length is of length different from
                        0-8,12,16,20,24,32,48,64 can be sent to CanDrv in AR 4.0.x.
                    Actual sending of L-PDU to CanDrv is in Can_Write(), which is called at the end of this function.

    For AR versions < 4.1, this API will:
                - Change L-PDU Length to the next higher valid length of 0-8,12,16,20,24,32,48,64 if L-PDU length is
                    different from those valid values.
                - Initialize these unused bytes to the value of the corresponding CanFdPaddingValue
                - Send modified L-PDU to CanDrv
    For AR version > 4.1, this API will:
                - directly send L-PDU to CanDrv
 BSW-9662
 ********************************************************************************************************************
 */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* [$DD_BSWCODE 17054] */ /* [$DD_BSWCODE 17055] */
Can_ReturnType CanIf_Write_Integration(Can_HwHandleType Hth, const Can_PduType* PduInfoPtr)
{
    Can_ReturnType  lCanStatus_en = CAN_NOT_OK;

    /*
    * Below is the sample implementation for DLC remapping preprocessing to ensure only valid L-PDU length is send to CanDrv.
    * Some MCALs also support invalid L-PDU length. Therefore, please check carefully
    * if this preprocessing is required. Otherwise, remove the following section of code.
    */
#if((CANIF_FD_SUPPORT == STD_ON) && (CANIF_WRITE_INTEGRATION_VERSION < 1U))
    // uint8 FdPaddingByte;
    // /*variable used as an index for looping conditions */
    // uint8 index_u8;
    // Can_PduType pduInfo_tmp;

    // /*Initializing the index for the loop */
    // index_u8 = 0;
    // /*Initializing the CanFD padding byte value */
    // FdPaddingByte = CANFD_PADDING_VALUE;


    // pduInfo_tmp.id = PduInfoPtr->id;
    // pduInfo_tmp.length = PduInfoPtr->length;
    // pduInfo_tmp.sdu = (uint8 *)CanIf_Pdu_Buffer;
    // (void)rba_BswSrv_MemCopy(pduInfo_tmp.sdu, PduInfoPtr->sdu, pduInfo_tmp.length);
    // pduInfo_tmp.swPduHandle = PduInfoPtr->swPduHandle;

    // /* Remapping DLC if the data length is larger than 8 bytes */
    // if((PduInfoPtr->length > 8U) && (PduInfoPtr->length <= 12U)) pduInfo_tmp.length = 12U;
    // else if((PduInfoPtr->length > 12U) && (PduInfoPtr->length <= 16U)) pduInfo_tmp.length = 16U;
    // else if((PduInfoPtr->length > 16U) && (PduInfoPtr->length <= 20U)) pduInfo_tmp.length = 20U;
    // else if((PduInfoPtr->length > 20U) && (PduInfoPtr->length <= 24U)) pduInfo_tmp.length = 24U;
    // else if((PduInfoPtr->length > 24U) && (PduInfoPtr->length <= 32U)) pduInfo_tmp.length = 32U;
    // else if((PduInfoPtr->length > 32U) && (PduInfoPtr->length <= 48U)) pduInfo_tmp.length = 48U;
    // else if((PduInfoPtr->length > 48U) && (PduInfoPtr->length <= 64U)) pduInfo_tmp.length = 64U;


    // /* Add CanFdPaddingValue for the padding bytes */
    // for(i = PduInfoPtr->length; i < pduInfo_tmp.length; i++)
    // {
        // pduInfo_tmp.sdu[i] = FdPaddingByte;
    // }

    // /*Set CAN_FD indication bit in case of standard ID for MCAL TC264*/
    // pduInfo_tmp.id |= ((uint32)(1u<<14U));
#endif

#if((CANIF_FD_SUPPORT == STD_ON) && (CANIF_WRITE_INTEGRATION_VERSION < 1U))
    //When DLC remapping is implemented, fix below function call to:
    //lCanStatus_en =  Can_Write(Hth, &pduInfo_tmp);
    lCanStatus_en =  Can_Write(Hth, PduInfoPtr);
#elif ((CANIF_FD_SUPPORT == STD_OFF) && (CANIF_WRITE_INTEGRATION_VERSION < 1U))
    lCanStatus_en =  Can_Write(Hth, PduInfoPtr);
#elif(CANIF_WRITE_INTEGRATION_VERSION >= 1)
    lCanStatus_en =  Can_Write(Hth, PduInfoPtr);
#endif

    return lCanStatus_en;

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

