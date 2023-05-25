

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Inl.h"

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */
#if ((CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)||(CANNM_PN_ENABLED != STD_OFF))

#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

/* Pointer to hold global configuration structure ,used in the module sources */
const CanNm_GlobalConfigType * CanNm_GlobalConfigData_pcs;

#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

#endif

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_Init
 Syntax           : void CanNm_Init( const CanNm_ConfigType* const cannmConfigPtr )
 Description      : This is the AUTOSAR interface for initialization of CanNm called by EcuM.
 Parameter        : cannmConfigPtr - Pointer to a selected configuration structure
 Return value     : None
 ***************************************************************************************************/

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

void CanNm_Init(const CanNm_ConfigType * cannmConfigPtr)
{

    /* Pointer to Configuration Data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Pointer to transmit buffer */
    uint8 * IndexPtr;

    /* Variable to hold channel handle */
    uint8_least Channel_ui;

    /* Variable to hold byte position in the buffer */
    uint8_least Index_ui;


#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)

#if ((CANNM_CONFIGURATION_VARIANT == CANNM_VARIANT_POSTBUILD_LOADABLE) && (CANNM_VERSION_INFO_API != STD_OFF))
    /* variable to read version info from CanNm_GetVersionInfo */
    Std_VersionInfoType versionInfo_s;
#endif

#if(CANNM_DEV_ERROR_DETECT != STD_OFF)
    if(cannmConfigPtr == NULL_PTR)
    {
        (void)cannmConfigPtr;
        /* Report DET : Development Error for Invalid  Config Pointer */
        (void) Det_ReportError(CANNM_MODULE_ID,(0u), CANNM_SID_INIT, CANNM_E_INIT_FAILED);
        return;
    }
#endif

#if ((CANNM_CONFIGURATION_VARIANT == CANNM_VARIANT_POSTBUILD_LOADABLE) && (CANNM_VERSION_INFO_API != STD_OFF))
    /* Read the pre-compile version info */
    CanNm_GetVersionInfo(&versionInfo_s);
    /* Compare pre-compile & post-build version info */
    if(CanNm_CompareVersionInfo(&versionInfo_s, cannmConfigPtr->versionInfo))
    {
        /* versionInfo doesnt match .. Inform application */
        CanNmAppl_IncompatibleGenerator();

        /* Abort further initialisation ..CanNm will remain in NM_STATE_UNINIT state */
        return;
    }
#endif
    /* Initialise global configuration pointer, for later usage in the module */
    /* MR12 RULE 11.5 VIOLATION: Pointer is intialized for Post Build support */
    CanNm_GlobalConfigData_pcs = (const CanNm_GlobalConfigType * ) cannmConfigPtr->CanNm_ConfigData;

#elif(CANNM_PN_ENABLED != STD_OFF)
    /* Initialise global configuration pointer, for later usage in the module */
    CanNm_GlobalConfigData_pcs = &CanNm_GlobalConfigData_cs;
    (void)cannmConfigPtr;

#else
    /* Donot initialise CanNm_GlobalConfigData_pcs if selected CanNm Variant is Pre-Compile and
     * Partial Network feature is disabled */
    (void)cannmConfigPtr;
#endif

#if (CANNM_EIRACALC_ENABLED != STD_OFF)
    CanNm_Init_EIRA_Status();
#endif

#if (CANNM_ERACALC_ENABLED != STD_OFF)
    CanNm_Init_ERA_Status();
#endif

    /* Initialize all the channels which are configured as active */
    for(Channel_ui = 0; Channel_ui < CANNM_NUMBER_OF_CHANNELS; Channel_ui++)
    {
        /* Initialize the channel configuration pointer to proper address */
        ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(Channel_ui);

        /* Check whether the channel can be initialized */
        if(ConfigPtr_pcs->stChannelActive_b != FALSE)
        {
            /* Initialize pointer to RAM structure */
            RamPtr_ps = &CanNm_RamData_s[Channel_ui];

            /* TRACE[SWS_CanNm_00141, SWS_CanNm_00144] : Initialize CANNM mode and network management state */
            RamPtr_ps->Mode_en = NM_MODE_BUS_SLEEP;
            RamPtr_ps->State_en = NM_STATE_BUS_SLEEP;

            /* TRACE[SWS_CanNm_00143] : Initialize CANNM internal global variables */
            RamPtr_ps->NetworkReqState_en = CANNM_NETWORK_RELEASED_E;
            RamPtr_ps->RxCtrlBitVector_u8 = 0x00;
            RamPtr_ps->TxCtrlBitVector_u8 = 0x00;
            RamPtr_ps->ImmediateNmTx_u8 = 0;

#if (CANNM_PN_ENABLED != STD_OFF)
            /*
             * If CanNmPnEnabled is TRUE the CanNm module shall set the value of the transmitted PNI bit to 1.
             * Since this info doesn't change dynamically, set the CBV in the init function itself
             * */
            if(CANNM_GetPnEnabledStatus(ConfigPtr_pcs) != FALSE)
            {
                RamPtr_ps->TxCtrlBitVector_u8 = RamPtr_ps->TxCtrlBitVector_u8 | CANNM_CBV_PNI_STATUS;
            }
            CANNM_ResetPnMsgFilteringStatus(RamPtr_ps);
            RamPtr_ps->PnHandleMultipleNetworkRequests_b = FALSE;
#endif

#if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
            /* Initialise the repeat message request status */
            RamPtr_ps->TxRptMsgStatus_b = FALSE;
#endif

            RamPtr_ps->RxIndication_b = FALSE;
            RamPtr_ps->TxConfirmation_b = FALSE;

            RamPtr_ps->stCanNmComm = TRUE;
            CANNM_RESET_REMOTE_SLEEP();

			/*Initialise Msg timeout time monitoring flag*/
			RamPtr_ps->TxTimeoutMonitoringActive_b = FALSE;

            /* Initialize the free running timer */
            RamPtr_ps->ctSwFrTimer = 0;

            /* Initialize TxPdu data pointer and length. This is the PDU transmitted in CanIf_Transmit */
            RamPtr_ps->Pdu_s.SduDataPtr = &(RamPtr_ps->TxBuffer_au8[0]);
            RamPtr_ps->Pdu_s.SduLength = ConfigPtr_pcs->PduLength_u8;

            /* Copy the node ID from the configuration into TxPdu of RAM data*/
            if (ConfigPtr_pcs->NodeIdPos_u8 != CANNM_PDU_OFF)
            {
                RamPtr_ps->TxBuffer_au8[ConfigPtr_pcs->NodeIdPos_u8] = ConfigPtr_pcs->NodeId_u8;
            }

            /* Set the Control Bit Vector in RAM data to 0x00 */
            if (ConfigPtr_pcs->ControlBitVectorPos_u8 != CANNM_PDU_OFF)
            {
                RamPtr_ps->TxBuffer_au8[ConfigPtr_pcs->ControlBitVectorPos_u8] = 0x00;
            }

            /* Set the user data bytes of RAM data to 0xFF */
            if (ConfigPtr_pcs->UserDataLength_u8 != 0)
            {
                /* Calculate the buffer pointer to set the user bytes */
                IndexPtr = &(RamPtr_ps->TxBuffer_au8[ConfigPtr_pcs->PduLength_u8 - ConfigPtr_pcs->UserDataLength_u8]);

                for(Index_ui = 0; (Index_ui < ConfigPtr_pcs->UserDataLength_u8); Index_ui++)
                {
                    *(IndexPtr) = CANNM_DEFAULT_USER_BYTE;
                    IndexPtr++;
                    RamPtr_ps->UserDataBuffer_au8[Index_ui] = CANNM_DEFAULT_USER_BYTE;
                }
            }

            /* Initialize message transmission status flag */
            RamPtr_ps->MsgTxStatus_b = FALSE;

#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)
            /* Initialize the NmCoordinatorSleepReady bit */
            RamPtr_ps->CoordReadySleepBit_u8 = 0x00;
            /* Initialize CoordReadySleep Indication status */
            RamPtr_ps->stCoordReadySleepInd_b = FALSE;
#endif

#if (CANNM_RETRY_FIRST_MSG_REQ_ENABLED != STD_OFF)
            /* Initialize first transmission failure flag*/
            RamPtr_ps->FirstTransmissionFailure_b = FALSE;
            /* Initialize first message flag */
            RamPtr_ps->FirstMessage_b = FALSE;
#endif
        }
    }

    return;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#if ((CANNM_PN_ENABLED != STD_OFF) && defined(BSWSIM))

/***************************************************************************************************
 Function name    : CanNm_SetPnState
 Syntax           : void CanNm_SetPnState(const NetworkHandleType nmChannelHandle,boolean state)
 Description      : This function sets the PN state in configuaration structure.
 Parameter        : nmChannelHandle - Identification of the NM-channel
                  : state - state of PN Enable sent through test application.
 Return value     : None
 ***************************************************************************************************/
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
void CanNm_SetPnState(const NetworkHandleType nmChannelHandle,boolean state)
{
    /* Variable to hold the Network Handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;
    /* pointer to configuration data */
    CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    ConfigPtr_pcs = (CanNm_ChannelConfigType * ) CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);
    /* Initialize pointer to RAM structure */
   RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    //Change the const directly
    ConfigPtr_pcs->PnEnabled_b = state;
    if(state)
    {
        /* Change the state of the RAM variable as well */
        CANNM_SetPnMsgFilteringStatus(RamPtr_ps);
    }
    else
    {
        /* Change the state of the RAM variable as well */
        CANNM_ResetPnMsgFilteringStatus(RamPtr_ps);
    }
}
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

/***************************************************************************************************
 Function name    : CanNm_SetAllMessageKeepAwake
 Syntax           : void CanNm_SetAllMessageKeepAwake(const NetworkHandleType nmChannelHandle,boolean state)
 Description      : This function sets AllNmMessagesKeepAwake_b in configuaration structure.
 Parameter        : nmChannelHandle - Identification of the NM-channel
                  : state - state of PN Enable sent through test application.
 Return value     : None
 ***************************************************************************************************/
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
void CanNm_SetAllMessageKeepAwake(const NetworkHandleType nmChannelHandle,boolean state)
{
    /* Variable to hold the Network Handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /* pointer to configuration data */
    CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    ConfigPtr_pcs = (CanNm_ChannelConfigType * ) CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    //Change the const directly
    ConfigPtr_pcs->AllNmMessagesKeepAwake_b = state;

}
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

/***************************************************************************************************
 Function name    : CanNm_SetPnHandleMultipleNetworkReqState
 Syntax           : void CanNm_SetPnHandleMultipleNetworkReqState(const NetworkHandleType nmChannelHandle,boolean state)
 Description      : Sets the multiple network request in configuration structure.
 Parameter        : nmChannelHandle - Identification of the NM-channel
                  : state - state of PN Enable sent through test application.
 Return value     : None
 ***************************************************************************************************/
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
void CanNm_SetPnHandleMultipleNetworkReqState(const NetworkHandleType nmChannelHandle,boolean state)
{
        /* Variable to hold the Network Handle of CanNm */
        NetworkHandleType CanNm_NetworkHandle;

        /* pointer to configuration data */
        CanNm_ChannelConfigType * ConfigPtr_pcs;

        /* Receive CanNm Channel Index from the received ComM ChannelID */
        CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

        ConfigPtr_pcs = (CanNm_ChannelConfigType * ) CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

        //Change the const directly
        ConfigPtr_pcs->PnHandleMultipleNetworkRequests_b = state;
}
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif


