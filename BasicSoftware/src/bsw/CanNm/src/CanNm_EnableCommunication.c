
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
 Function name    : CanNm_EnableCommunication
 Description      : This AUTOSAR interface is used to enable NM PDU transmission due to a ISO14229
                    Communication Control (28hex) service. This must be called after calling
                    CanNm_DisableCommunication. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
 Return value     : E_OK     - No error
                  : E_NOT_OK - Enabling of NM PDU transmission ability has failed
 ***************************************************************************************************/

/* SWS_CanNm_00264: This API is available only if CANNM_COM_CONTROL_ENABLED is set to TRUE*/
#if (CANNM_COM_CONTROL_ENABLED != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_EnableCommunication(
                                                            NetworkHandleType nmChannelHandle
                                                           )
{
    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Return Value of the API */
    Std_ReturnType RetVal_en;

    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /**** End Of Declarations ****/

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is Invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                                nmChannelHandle, CANNM_SID_ENABLECOMMUNICATION, CANNM_E_INVALID_CHANNEL)


    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                               nmChannelHandle, CANNM_SID_ENABLECOMMUNICATION, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    /* Initialize the pointer to RAM data structure */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    /* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /* TRACE[SWS_CanNm_00177,SWS_CanNm_00295] : API call is valid only if transmission ability should be disabled
       and CanNm is in Network Mode */
    if ((RamPtr_ps->stCanNmComm == FALSE) && (RamPtr_ps->Mode_en == NM_MODE_NETWORK))
    {
        /* Enter critical section */
        SchM_Enter_CanNm_EnableCommunicationNoNest();

        /* TRACE[SWS_CanNm_00176] : Enable NM PDU transmission ability*/
        RamPtr_ps->stCanNmComm = TRUE;

        /* TRACE[SWS_CanNm_00179] : Restart NM Timeout timer */
        CanNm_StartTimer(RamPtr_ps->ctNMTimeoutTimer);

        /* TRACE[SWS_CanNm_00180] : Restart Remote sleep indication timer if Remote sleep indication is enabled */
        #if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
            CanNm_StartTimer(RamPtr_ps->ctRemoteSleepIndTimer);
        #endif

        /* TRACE[SWS_CanNm_00178] : Restart message cycle timer to start the transmission afresh only in states Normal Operation
           and Repeat Message */
        if (RamPtr_ps->State_en != NM_STATE_READY_SLEEP)
        {
            CanNm_StartTransmission(CanNm_NetworkHandle);
        }

        /* Exit Critical Section */
        SchM_Exit_CanNm_EnableCommunicationNoNest();

        /* Return successful operation */
        RetVal_en = E_OK;
    }

        /* Transmission ability is already enabled or CanNm is not in Network mode */
        return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


