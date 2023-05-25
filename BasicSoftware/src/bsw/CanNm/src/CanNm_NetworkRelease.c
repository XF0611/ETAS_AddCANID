

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
 Function name    : CanNm_NetworkRelease
 Description      : This is the AUTOSAR interface to release the network, since ECU doesn't have to
                    communicate on the bus. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
 Return value     : E_OK - No error
                  : E_NOT_OK - Releasing of network has failed
 ***************************************************************************************************/

/* TRACE[SWS_CanNm_00260] : The API is available only if CANNM_PASSIVE_MODE_ENABLED is disabled */
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_NetworkRelease( NetworkHandleType nmChannelHandle )
{
    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /* Return value of the API */
    Std_ReturnType RetVal_en;

    /**** End Of Declarations ****/

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_NETWORKRELEASE, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                            nmChannelHandle, CANNM_SID_NETWORKRELEASE, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    /* Check if NM PDU transmission ability is enabled */
    if (CanNm_RamData_s[CanNm_NetworkHandle].stCanNmComm != FALSE)
    {
        /* TRACE[SWS_CanNm_00105] : Set network state to released */
        CanNm_RamData_s[CanNm_NetworkHandle].NetworkReqState_en = CANNM_NETWORK_RELEASED_E;

        /* Return successful operation */
        RetVal_en = E_OK;
    }

    return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif

