

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Inl.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_SetUserData
 Description      : This is the AUTOSAR interface to user data for NM PDUs transmitted next on the
                    bus. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
                  : nmUserDataPtr   - Pointer where the user data for the next transmitted NM PDU
                                      shall be copied from
 Return value     : E_OK - No error
                  : E_NOT_OK - Setting of user data has failed
 ***************************************************************************************************
 */

/* TRACE[SWS_CanNm_00266, SWS_CanNm_00327] : API available only if CANNM_USER_DATA_ENABLED is set to TRUE,
   CANNM_PASSIVE_MODE_ENABLED and CANNM_COM_USER_DATA_SUPPORT are disabled */
#if((CANNM_USER_DATA_ENABLED != STD_OFF) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && \
      (CANNM_COM_USER_DATA_SUPPORT == STD_OFF))

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_SetUserData(
                                                    NetworkHandleType nmChannelHandle,
                                                    const uint8 * nmUserDataPtr
                                                   )
{
    /* Variable to hold the length of user data */
    uint8 UserDataLen;

    /* Pointer to global buffer holding user data */
    uint8 * BufferPtr;

    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /* Pointer to access channel configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /**** End Of Declarations ****/

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /* set the pointer to Configuration structure of Channel CanNm_NetworkHandle */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_SETUSERDATA, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                                nmChannelHandle, CANNM_SID_SETUSERDATA, CANNM_E_NO_INIT)

    /* Report DET if received pointer is a null pointer */
    CANNM_DET_REPORT_ERROR_NOK((nmUserDataPtr == NULL_PTR),
                            nmChannelHandle, CANNM_SID_SETUSERDATA, CANNM_E_PARAM_POINTER)

    /*********************************  End: DET  *************************************/

    /* Get the length of user data to be transmitted next on the bus */
    UserDataLen = ConfigPtr_pcs->UserDataLength_u8;

    /* Fetch the address of the buffer where the received user data shall be stored*/
    BufferPtr = &(CanNm_RamData_s[CanNm_NetworkHandle].UserDataBuffer_au8[0]);

    /* Suspend interrupts to provide Data consistency */
    SchM_Enter_CanNm_SetUserDataNoNest();

    /* This is an internal function with predictable run-time; hence doesn't affect interrupt lock */
    /* TRACE[SWS_CanNm_00159] : Copy the user data from the passed pointer into buffer*/
    CanNm_CopyBuffer(nmUserDataPtr,BufferPtr,UserDataLen);

    /* Enable interrupts */
    SchM_Exit_CanNm_SetUserDataNoNest();

    /* Return successful operation */
    return E_OK;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


