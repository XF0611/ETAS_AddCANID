

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
 Function name    : CanNm_GetState
 Description      : This is the AUTOSAR interface which returns state and the mode of the network
                    management. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
                  : nmStatePtr - Pointer where the state of the network management shall be copied to
                  : nmModePtr  - Pointer where the mode of the network management shall be copied to
 Return value     : E_OK - No error
                  : E_NOT_OK - Getting of NM state has failed
 ***************************************************************************************************/

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_GetState(
                                                 NetworkHandleType nmChannelHandle,
                                                 Nm_StateType * nmStatePtr,
                                                 Nm_ModeType * nmModePtr
                                                )
{
    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /**** End Of Declarations ****/

    /* Receive CanNm channel index from the received ComM channelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_GETSTATE, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                            nmChannelHandle, CANNM_SID_GETSTATE, CANNM_E_NO_INIT)

    /* Report DET if received pointer is a null pointer*/
    CANNM_DET_REPORT_ERROR_NOK(((nmStatePtr == NULL_PTR) || (nmModePtr == NULL_PTR)),
                            nmChannelHandle, CANNM_SID_GETSTATE, CANNM_E_PARAM_POINTER)


    /*********************************  End: DET  *************************************/

    /* Suspend interrupts to provide data consistency */
    SchM_Enter_CanNm_GetStateNoNest();

    /* Copy the current state and mode of the network into passed pointers*/
    *nmStatePtr = CanNm_RamData_s[CanNm_NetworkHandle].State_en;
    *nmModePtr = CanNm_RamData_s[CanNm_NetworkHandle].Mode_en;

    /* Enable interrupts */
    SchM_Exit_CanNm_GetStateNoNest();

    /* Return successful operation */
    return E_OK;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


