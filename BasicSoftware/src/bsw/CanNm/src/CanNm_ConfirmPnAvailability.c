

#include "CanNm_Prv.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_ConfirmPnAvailability
 Description      : This is the AUTOSAR interface to enable PN filter functionality on the indicated
                    This API shall be called by CanSM.
                    NM channel
 Parameter        : nmChannelHandle - Identification of the NM-channel
 Return value     : None
 ***************************************************************************************************
 */

/* TRACE[SWS_CanNm_00346] : API is only available if CANNM_PN_ENABLED is set to TRUE*/
#if (CANNM_PN_ENABLED != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle)
{
    /* Variable to hold the network handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /* Pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Receive CanNm channel index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_CONFIRMPNAVAILABILITY, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                            nmChannelHandle, CANNM_SID_CONFIRMPNAVAILABILITY, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    /* Initialize pointer to RAM structure */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    /* TRACE[SWS_CanNm_00404] : If PN is enabled for the channel, then activate the PN message filtering only for that channel */
    if (CANNM_GetPnEnabledStatus(ConfigPtr_pcs) != FALSE)
    {
        CANNM_SetPnMsgFilteringStatus(RamPtr_ps);
    }
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


