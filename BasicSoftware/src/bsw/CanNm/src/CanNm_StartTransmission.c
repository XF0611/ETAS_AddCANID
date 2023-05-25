

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
 Function name    : CanNm_StartTransmission
 Syntax           : void CanNm_StartTransmission( const NetworkHandleType CanNm_NetworkHandle )
 Description      : This is the internal function of CanNm for starting transmission of Nm messages on CAN.
                    This function is called by the CanNm Main Function, CanNm_EnableCommunication and
                    CanNm_PassiveStartUp. This function will be called only if the Passive Mode is disabled.
 Parameter        : CanNm_NetworkHandle - Identification of the NM-channel
 Return value     : None
 ***************************************************************************************************/

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

void CanNm_StartTransmission(const NetworkHandleType CanNm_NetworkHandle)
{
    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Pointer to access channel configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /**** End Of Declarations ****/

    /* Initialize pointer to RAM structure */
    RamPtr_ps  = &CanNm_RamData_s[CanNm_NetworkHandle];

    /* Set the pointer to configuration structure of channel CanNm_NetworkHandle */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    /* Start transmission */
    RamPtr_ps->MsgTxStatus_b = TRUE;

    /* Message transmit confirmation not received */
    RamPtr_ps->TxConfirmation_b = FALSE;

    if(RamPtr_ps->ImmediateNmTx_u8 == 0)
    {
        /* Get value of message cycle offset time */
        RamPtr_ps->MsgCyclePeriod = ConfigPtr_pcs->MsgCycleOffset;
    }
    else
    {
        /* First Pdu should be transmitted as soon as possible */
        RamPtr_ps->MsgCyclePeriod = 0;
    }
    /* Start monitoring message cycle offset time */
    RamPtr_ps->PrevMsgCycleTimestamp = RamPtr_ps->ctSwFrTimer;

    return;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif

