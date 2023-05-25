


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComM_Priv.h"
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */
/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */
#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)
LOCAL_INLINE void ComM_Prv_UpdateNvMInhibitionStatus(uint8 ChannelId_u8, uint8 InhibitionStatus_u8,
                                                                                    uint8 * NvM_InhibitionArray_pu8);
#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"


/*******************************************************************************
 * Function name    : ComM_DeInit
 * Syntax           : void ComM_DeInit(void)
 * Description      : De-initializes the AUTOSAR Communication Manager.
 * Parameters       : void
 * Return value     : void
 ******************************************************************************/
void ComM_DeInit(void)
{
    /* Local Variables Declaration */
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    #endif  /*  #if (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    uint8                           ChannelIndexCtr_u8;
    ComM_GlobalVarType_tst *        globalRamPtr_pst;

#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)
    Std_ReturnType                  Ret_NvM_Writeval_tu8;
    uint8                           ChannelId_u8;
    ComM_ChannelVarType_tst *       channelRamPtr_pst;
#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */

    globalRamPtr_pst = &ComM_GlobalStruct;

    /* Local variable initialization */
    /* DET check for communication manger initialization */
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    /* check if communication manager in initialized. Else report DET */
    if (/* check if communication manager in initialized.
        Else report DET */
    globalRamPtr_pst->ComM_InitStatus_en != COMM_INIT)
    {
        /* Report DET with ComM not initialised status*/
        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_DEINIT_APIID,COMM_E_NOT_INITED);
        return;
    }
    else
    {
        /* do nothing */
    }
    #endif  /*  #if (COMM_DEV_ERROR_DETECT != STD_OFF)  */

    /* Set the init status as Uninit */
    /* De-initializes (terminates) the AUTOSAR Communication Manager*/
    globalRamPtr_pst->ComM_InitStatus_en = COMM_UNINIT;
    for (ChannelIndexCtr_u8 = C_ZERO ; ChannelIndexCtr_u8 < COMM_NO_OF_CHANNELS ; ChannelIndexCtr_u8++)
    {
        /* loop thro all the ComM channel,and if any of the channel is found to
        be not in NO_COMMUNICATION state then deinit call is ignored */
        /* Check if the current channel mode of all the channel is No communication .
        If yes change the INIT status as Uninit */
        if (ComM_ChannelStruct[ChannelIndexCtr_u8].ChannelState_e != COMM_NO_COM_NO_PENDING_REQUEST)
        {
            /*  Initialize the status if any one of the channel is not in No communication mode and break it */
            globalRamPtr_pst->ComM_InitStatus_en = COMM_INIT;
            break;
        }
    }

#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)

        /*Storing ComMEcuGroupClassification and  Inhibit counter values on to NvM*/
        ComM_NvMblock.ComMEcuGroupClassification_u8 = ComM_GlobalStruct.ComM_EcuGroupClassification_u8;
        ComM_NvMblock.Inhibitioncounter_u16 = ComM_GlobalStruct.ComM_InhibitCounter_u16;

        /*Loop through all Channel to store the ComMNoWakeup value to NvM */
        /* MR12 RULE 14.2 VIOLATION: The variable incremented in the third expression of this 'for' statement is actually the variable identified as the 'loop control variable.
               * Hence it is an incorrect warning reported by Misra */

        for (ChannelId_u8 = C_ZERO; ChannelId_u8 < COMM_NO_OF_CHANNELS ;++ChannelId_u8)
        {
            /*MR12 RULE 10.3 VIOLATION:  An expression of 'essentially Boolean' type (_Bool) is being converted to unsigned type, 'unsigned char' on assignment.
            * This is a Incorrect Misra Warning. This is taken care in review.*/
            channelRamPtr_pst = &ComM_ChannelStruct[ChannelId_u8];
            ComM_Prv_UpdateNvMInhibitionStatus(ChannelId_u8, COMM_GET_PREVENT_WAKEUP(channelRamPtr_pst->InhibitionReqStatus_u8), ComM_NvMblock.ComMNoWakeup_u8);
        }

        /*MR12 DIR 1.1 VIOLATION: NvM_WriteBlock's NvM_SrcPtr return value should be void as per Autosar guidelines.*/
        Ret_NvM_Writeval_tu8 = NvM_WriteBlock(COMM_NVM_BLOCK_REFERENCE,&ComM_NvMblock);

        /* Control shouldn't come here.If at all happens, report to DET */
        #if (COMM_DEV_ERROR_DETECT != STD_OFF)
        if(Ret_NvM_Writeval_tu8 != E_OK)
        {
            (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_NVM_REFERENCE_BLOCK,COMM_E_NVM_WRITE_FAILED);
            return;
        }
        #endif   /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */

#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */

        return;


}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

/*****************************************************************************************************
 * Function name    : ComM_Prv_UpdateNvMInhibitionStatus
 * Syntax           : ComM_Prv_UpdateNvMInhibitionStatus
 *                    (
 *                      uint8 ChannelId, uint8 InhibitionStatus_u8, uint8 * NvM_InhibitionArray_pu8
 *                    )
 * Description      : Internal function to store the NvM ComMNoWakeup value in an array.
 *                                          (Each bit constitutes the value of ComMNoWakeup)
 * Parameters       : ChannelId -> Channel handle Id
 *                    InhibitionStatus_u8 -> Staus taken from the config structure.
 *                  : NvM_InhibitionArray_pu8 -> Array which stores ComMNoWakeup value .
 * Return value     : void
 *
 ****************************************************************************************************/
#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)

LOCAL_INLINE void ComM_Prv_UpdateNvMInhibitionStatus(uint8 ChannelId_u8, uint8 InhibitionStatus_u8,
                                                                            uint8 * NvM_InhibitionArray_pu8)

{
    /* Local variable declaration*/
    uint8 mask_u8;
    uint8 byteIndex_u8;
    uint8 bitPosition_u8;

    /* Local variable initialization*/
    byteIndex_u8 = (ChannelId_u8) >> 3;
    bitPosition_u8 = (ChannelId_u8) % 8 ;

    if(InhibitionStatus_u8 == 0x01)
    {
        /*If the InhibitionStautus is 1 then set the bit*/
         mask_u8 = ((uint8)1 << bitPosition_u8);
        *(NvM_InhibitionArray_pu8+byteIndex_u8) = (uint8)(*(NvM_InhibitionArray_pu8+byteIndex_u8) | (mask_u8)) ;
    }
    else
    {
        /*If the InhibitionStautus is 0 then reset the bit*/
         mask_u8 = (~ ((uint8)1 << bitPosition_u8));
        *(NvM_InhibitionArray_pu8+byteIndex_u8) = (uint8)(*(NvM_InhibitionArray_pu8+byteIndex_u8) & (mask_u8));
    }

}
#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */



