

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Prv.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_NetworkRequest
 Description      : This is the AUTOSAR interface to request the network, since ECU needs to
                    communicate on the bus. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
 Return value     : E_OK - No error
                  : E_NOT_OK - Requesting of network has failed
 ***************************************************************************************************/

/* TRACE[SWS_CanNm_00257] : The API is available only if CANNM_PASSIVE_MODE_ENABLED is disabled */
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_NetworkRequest( NetworkHandleType nmChannelHandle )
{
    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

#if (CANNM_PN_ENABLED != STD_OFF)
    /* Pointer to access channel configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;
#endif

    /**** End Of Declarations ****/

    /* Receive CanNm channel index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

#if (CANNM_PN_ENABLED != STD_OFF)
    /* Set the pointer to configuration structure of channel CanNm_NetworkHandle */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);
#endif
    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_NETWORKREQUEST, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                            nmChannelHandle, CANNM_SID_NETWORKREQUEST, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    /* TRACE[SWS_CanNm_00255, SWS_CanNm_00104] : Set network state to requested */
    CanNm_RamData_s[CanNm_NetworkHandle].NetworkReqState_en = CANNM_NETWORK_REQUESTED_E;

#if (CANNM_PN_ENABLED != STD_OFF)
    /* TRACE[SWS_CanNm_00444] : Check the condition for spontaneous transmission of Nm PDU */
    if ((CanNm_RamData_s[CanNm_NetworkHandle].Mode_en == NM_MODE_NETWORK) &&
            (ConfigPtr_pcs->PnHandleMultipleNetworkRequests_b != FALSE))
    {
        /* Set PnHandleMultipleNetworkRequests flag in RAM structure */
        CanNm_RamData_s[CanNm_NetworkHandle].PnHandleMultipleNetworkRequests_b = TRUE;
    }

#endif

    return E_OK;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif

