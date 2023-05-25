
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
 Function name    : CanNm_DisableCommunication
 Description      : This AUTOSAR interface is used to disable NM PDU transmission ability temporarily,
                    due to a ISO14229 Communication Control (28hex) service. It should be ensured that
                    CanNm_EnableCommunication is called after this API call. This API shall be called
                    by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
 Return value     : E_OK     - No error
                  : E_NOT_OK - Disabling of NM PDU transmission ability has failed
 ***************************************************************************************************/

/* TRACE[SWS_CanNm_00262] : This API is available only if CANNM_COM_CONTROL_ENABLED is set to TRUE*/
#if (CANNM_COM_CONTROL_ENABLED != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_DisableCommunication(
                                                             NetworkHandleType nmChannelHandle
                                                            )
{
    /* Return value of the API */
    Std_ReturnType RetVal_en ;

    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /**** End Of Declarations ****/

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                                nmChannelHandle, CANNM_SID_DISABLECOMMUNICATION, CANNM_E_INVALID_CHANNEL)


    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                            nmChannelHandle, CANNM_SID_DISABLECOMMUNICATION, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    /* TRACE[SWS_CanNm_00172] : This API call is allowed only within Network mode */
    if (CanNm_RamData_s[CanNm_NetworkHandle].Mode_en == NM_MODE_NETWORK)
    {
        /* TRACE[SWS_CanNm_00170] : Disable Network Management PDU transmission ability*/
        CanNm_RamData_s[CanNm_NetworkHandle].stCanNmComm = FALSE;

        /* Return successful operation */
        RetVal_en = E_OK;
    }

    /* CanNm is not in Network mode */
    return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


