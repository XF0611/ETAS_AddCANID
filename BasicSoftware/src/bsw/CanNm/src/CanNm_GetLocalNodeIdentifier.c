

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
 Function name    : CanNm_GetLocalNodeIdentifier
 Description      : This function is used to get node identifier configured for the local node.
                    This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
                    nmNodeIdPtr     - Pointer where node identifier of the local node shall be copied to
 Return value     : E_OK     - No error
                  : E_NOT_OK - Getting of the node identifier of the local node has failed or is not
                               configured for this network handle
 ***************************************************************************************************/

#if (CANNM_NODE_ID_ENABLED != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_GetLocalNodeIdentifier(
                                                               NetworkHandleType nmChannelHandle,
                                                               uint8 * nmNodeIdPtr
                                                              )
{

    /* pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Return Value of the API */
    Std_ReturnType RetVal_en;

    NetworkHandleType CanNm_NetworkHandle;
    /**** End of declarations ****/

    /* Receive the Internal CanNmChannel structure index from the received ComM NetworkHandle*/
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if Network Handle is Invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                                   nmChannelHandle, CANNM_SID_GETLOCALNODEIDENTIFIER, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is Uninitialised */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                                nmChannelHandle, CANNM_SID_GETLOCALNODEIDENTIFIER, CANNM_E_NO_INIT)

    /* Report DET if received Pointer is a Null Pointer*/
    CANNM_DET_REPORT_ERROR_NOK((nmNodeIdPtr == NULL_PTR),
                                nmChannelHandle, CANNM_SID_GETLOCALNODEIDENTIFIER, CANNM_E_PARAM_POINTER)

   /*********************************  End: DET  *************************************/

   /* Initialise the pointer to configuration data structure of channel NetworkHandle */
   ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

   	/* Initialize the return value*/
	RetVal_en = E_NOT_OK;

   /* Check if Node ID is enabled for the channel and if its not configured OFF */
   if((FALSE != ConfigPtr_pcs->NodeIdEnabled_b) && (CANNM_PDU_OFF != ConfigPtr_pcs->NodeIdPos_u8))
   {
       /* TRACE[SWS_CanNm_00133] : Copy the Node Id of local host node into the passed pointer */
       *nmNodeIdPtr = ConfigPtr_pcs->NodeId_u8;

       /* Return successful operation */
       RetVal_en = E_OK;
   }

   /* API rejected as source node id is not configured */
   return RetVal_en;

}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


