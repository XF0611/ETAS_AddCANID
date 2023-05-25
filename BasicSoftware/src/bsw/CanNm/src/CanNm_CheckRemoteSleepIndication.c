

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
 Function name    : CanNm_CheckRemoteSleepIndication
 Description      : This AUTOSAR interface checks if remote sleep indication has taken place or not
                    This API shall be called by Nm.
 Parameter        : nmChannelHandle     - Identification of the NM-channel
                  : nmRemoteSleepIndPtr - Pointer where check result of remote sleep indication shall
                                          be copied to
 Return value     : E_OK     - No error
                  : E_NOT_OK - Checking of remote sleep indication bits has failed
 ***************************************************************************************************/

/* Detection of remote sleep indication shall be statically configurable with use of the
 * CANNM_REMOTE_SLEEP_IND_ENABLED switch  */
#if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_CheckRemoteSleepIndication(
                                                                   NetworkHandleType nmChannelHandle,
                                                                   boolean * nmRemoteSleepIndPtr)
{
    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Return Value of the API */
    Std_ReturnType RetVal_en;

    /* Variable to hold the Network Handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /**** End Of Declarations ****/

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_CHECKREMOTESLEEPINDICATION, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                              nmChannelHandle, CANNM_SID_CHECKREMOTESLEEPINDICATION, CANNM_E_NO_INIT)

    /* Report DET if received pointer is a null pointer*/
    CANNM_DET_REPORT_ERROR_NOK((nmRemoteSleepIndPtr == NULL_PTR),
                                nmChannelHandle, CANNM_SID_CHECKREMOTESLEEPINDICATION, CANNM_E_PARAM_POINTER)

    /*********************************  End: DET  *************************************/

    /* Initialize the pointer to RAM data structure */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

	/* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /* SWS_CanNm_00154 : Checking of remote sleep indication allowed either in Normal Opearation or Ready Sleep State */
    if ((RamPtr_ps->State_en == NM_STATE_NORMAL_OPERATION) || (RamPtr_ps->State_en == NM_STATE_READY_SLEEP))
    {
        /* Update the passed pointer with current remote sleep indication status */
        *nmRemoteSleepIndPtr = RamPtr_ps->stRemoteSleepInd;

        /* Return successful operation */
        RetVal_en = E_OK;
    }


    /* CanNm is in Bus-Sleep Mode, Prepare Bus-Sleep Mode or Repeat Message State */
    return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


