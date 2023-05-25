

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
 Function name    : CanNm_RequestBusSynchronization
 Description      : This is the AUTOSAR interface to trigger non-periodic NM PDU transmission which
                    is used to request bus synchronization. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
 Return value     : E_OK     - No error
                  : E_NOT_OK - Requesting of bus synchronization has failed
 ***************************************************************************************************/

/* TRACE[SWS_CanNm_00280] : API is available only if CANNM_BUS_SYNCHRONIZATION_ENABLED is set to TRUE and
   CANNM_PASSIVE_MODE_ENABLED is off */

#if ((CANNM_BUS_SYNCHRONIZATION_ENABLED != STD_OFF) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF))

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_RequestBusSynchronization(
                                                                  NetworkHandleType nmChannelHandle
                                                                 )
{
    /* Pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Return value of the API */
    Std_ReturnType RetVal_en;

    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /**** End Of Declarations ****/

    /* Receive CanNm channel index from the received ComM channelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

	/* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_REQUESTBUSSYNCHRONIZATION, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                            nmChannelHandle, CANNM_SID_REQUESTBUSSYNCHRONIZATION, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    /* Set the pointer to RAM structure of channel CanNm_NetworkHandle */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    /* TRACE[SWS_CanNm_00181,SWS_CanNm_00187] : Check if CanNm is in Network Mode and if transmission ability is enabled */
    if ((RamPtr_ps->Mode_en == NM_MODE_NETWORK) && (RamPtr_ps->stCanNmComm != FALSE))
    {
        /* Initialize pointer to configuration structure */
        ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

        /* Copy the User data */
        CanNm_UpdateTxPdu(ConfigPtr_pcs,RamPtr_ps);

        /* SWS_CanNm_00130: Trigger transmission of single PDU */
        RetVal_en = CanNm_MessageTransmit(ConfigPtr_pcs,RamPtr_ps);

        /* Check if transmission triggering is successful */
        if(RetVal_en == E_OK)
        {
            /* Return successful operation */
            RetVal_en = E_OK;
        }
    }

    /* TRACE[SWS_CanNm_00181,SWS_CanNm_00187] : API call is allowed in Network Mode and if transmission ability enabled */
    return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


