

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
 * Function    : CanNm_GetNodeIdentifier
 * Description : This is the AUTOSAR interface to get node identifier out of most recently received
 *               NM PDU. This API shall be called by Nm.
 * Parameter   : nmChannelHandle - Identification of the NM-channel
 *               nmNodeIdPtr     - Pointer to the location where the node identifier out of the most
 *                                 recently received NM PDU shall be copied to
 * Return value: E_OK     - No error
 *               E_NOT_OK - Getting of the node identifier out of the most recently received NM PDU
 *                          has failed or is not configured for this network handle
 ***************************************************************************************************/
#if (CANNM_NODE_ID_ENABLED != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_GetNodeIdentifier(
                                                          NetworkHandleType nmChannelHandle,
                                                          uint8 * nmNodeIdPtr
                                                         )
{
    /* pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Return Value of the API */
    Std_ReturnType RetVal_en;

    NetworkHandleType CanNm_NetworkHandle;

    /**** End of declarations ****/

    /* Receive the Internal CanNmChannel structure index from the received ComM NetworkHandle*/
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                                    nmChannelHandle, CANNM_SID_GETNODEIDENTIFIER, CANNM_E_INVALID_CHANNEL)

    /* SWS_CanNm_00269 : Report DET if CANNM is uninitialised */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                                nmChannelHandle, CANNM_SID_GETNODEIDENTIFIER, CANNM_E_NO_INIT)

    /* Report DET if received pointer is a null pointer*/
    CANNM_DET_REPORT_ERROR_NOK((nmNodeIdPtr == NULL_PTR),
                                nmChannelHandle, CANNM_SID_GETNODEIDENTIFIER, CANNM_E_PARAM_POINTER)

    /*********************************  End: DET  *************************************/

    /* Initialise the pointer to RAM data structure */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    /* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /* Initialise the pointer to configuration data structure of channel NetworkHandle */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    /* Allow copying of Node Id only when message has been received, Node ID is enabled for the channel
     * and node ID is not OFF */
    if ((FALSE != ConfigPtr_pcs->NodeIdEnabled_b) && (FALSE != RamPtr_ps->RxStatus_b) && \
            (CANNM_PDU_OFF != ConfigPtr_pcs->NodeIdPos_u8))
    {
        /* TRACE[SWS_CanNm_00132] : Copy the Node Id from the last received message into the passed pointer */
        *nmNodeIdPtr = RamPtr_ps->RxNodeId_u8;

        /* Return successful operation */
        RetVal_en = E_OK;
    }

    /* No message is received */
    return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif

