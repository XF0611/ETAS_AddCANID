

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
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
  Function name    : CanNm_SetSleepReadyBit
  Description      : This is the AUTOSAR interface to set the NM SleepReady bit in Control Bit Vector
                     for messages transmitted next on the bus. This API shall be called by Nm.
  Parameter        : nmChannelHandle - Identification of the NM-channel
                   : nmSleepReadyBit - Value written to ReadySleep Bit in CBV which is recieved from
                     NM.
  Return value     : E_OK - No error
                   : E_NOT_OK - Writing of remote sleep indication bit has failed.
  ***************************************************************************************************/

#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_SetSleepReadyBit(
                                                         NetworkHandleType nmChannelHandle,
                                                         boolean nmSleepReadyBit
                                                        )
{
    /* pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Variable to hold the Network Handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /* Return Value of the API */
    Std_ReturnType RetVal_en;

    /**** End Of Declarations ****/

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);


    /********************************* Start: DET *************************************/

    /* Report DET if Network Handle is Invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_SETSLEEPREADYBIT, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is Uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                            nmChannelHandle, CANNM_SID_NETWORKRELEASE, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    /* Set the pointer to RAM structure of Channel CanNm_NetworkHandle */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    if (nmSleepReadyBit == TRUE)
    {

        /* Set the NM Coordination Sleep Ready bits in the Control bit vector */
        CanNm_RamData_s[CanNm_NetworkHandle].TxCtrlBitVector_u8 = CanNm_RamData_s[CanNm_NetworkHandle].TxCtrlBitVector_u8 | (CANNM_COORD_READY_SLEEP_BIT_MASK);

    }

    else
    {

        /* Reset the NM Coordination Sleep Ready bits in the Control bit vector */
        CanNm_RamData_s[CanNm_NetworkHandle].TxCtrlBitVector_u8 = CanNm_RamData_s[CanNm_NetworkHandle].TxCtrlBitVector_u8 & (~(CANNM_COORD_READY_SLEEP_BIT_MASK));

    }

    /* Initialize pointer to configuration structure */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    /* Copy the User data */
    CanNm_UpdateTxPdu(ConfigPtr_pcs,RamPtr_ps);

    /* Transmit the PDU */
    RetVal_en = CanNm_MessageTransmit(ConfigPtr_pcs,RamPtr_ps);

    /* Check if transmission triggering is successful */
    if(RetVal_en == E_OK)
    {
        /* Return successful operation */
        return E_OK;
    }

    return E_NOT_OK;

}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif

