

/**
 **********************************************************************************************************************
 * \moduledescription
 *           AUTOSAR FEE
 *           According to Document Version 3.0.0, Part of Release 4.0, Revision 2
 * \scope    INTERN
 **********************************************************************************************************************
 */


/*
 *********************************************************************
 * Includes
 *********************************************************************
 */
#include "Fee.h"
#include "rba_FeeFs1_Prv.h"
#include "Fls.h"
#if (!defined(FLS_AR_RELEASE_MAJOR_VERSION) || (FLS_AR_RELEASE_MAJOR_VERSION != FEE_AR_RELEASE_MAJOR_VERSION))
    #error "AUTOSAR major version undefined or mismatched"
#endif

#if((defined(FEE_PRV_CFG_SELECTED_FS) && (1 == FEE_PRV_CFG_SELECTED_FS)) || (defined(FEE_CFG_FEE1_ENABLED) && (TRUE == FEE_CFG_FEE1_ENABLED)))

#if (FEE_PRESENT == 1)
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
static Fee_stRetVal_ten Fee_LLWrEraseStartFlag(uint8 phySectorIdx_u8);

/**
 *********************************************************************
 * Fee_LLSetEraseSector(): Set sector index of erase order
 *
 * This function sets the sector index of the global erase order
 * to the respective index value of the logical sector
 *
 * \param    EraseLogIdx:   Logical sector index
 * \return
 * \retval   none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void Fee_LLSetEraseSector(uint8 EraseLogIdx)
{
    Fee_LLEraseOrder_st.xPhySectorIdx_u8 = Fee_LLSectorOrder_st[EraseLogIdx].xPhySecIdx_u8;

    Fee_LLSectorOrder_st[EraseLogIdx].SecState_en         = FEE_SECTOR_REQUEST2ERASE_E;
    Fee_LLSectorOrder_st[EraseLogIdx].SecChngCnt_u32      = FEE_SEC_CHANGE_CNT_UNDEF;
}


/**
 *********************************************************************
 * Fee_LLEraseSector(): Erase the selected sector
 *
 * This function erases the formerly selected sector.
 *
 * \param    none
 * \return   Function result
 * \retval   FEE_ORDER_PENDING_E:   Order is still pending
 * \retval   FEE_ABORTED_E:         Order was aborted due to a wrong
 *                                  FLS compare function call
 * \retval   FEE_ERROR_E:           Writing of the erase marker is
 *                                  not possible
 * \retval   FEE_ORDER_FINISHED_E:  Order finished successfully
 *                                  (including write operation for the
 *                                   ERASE marker)
 * \seealso
 * \usedresources
 *********************************************************************
 */
Fee_stRetVal_ten Fee_LLEraseSector(void)
{
    Fee_stRetVal_ten xRetVal = FEE_ORDER_PENDING_E;
    Fls_AddressType    xAdStart_uo;
    Fls_LengthType     xSecSize_uo;
    uint16             xCacheIdx_u16;

    #if (FEE_PRV_DEBUG_MONITOR != FALSE)
    uint32             stDBM_u32 = 0;
    #endif


    /* Switch over state machine */
    switch(Fee_LLEraseOrder_st.EraseState_en)
    {
        /* Idle state */
        case FEE_ERASESEC_IDLE_E:
        {
            ;
        }
        /* No break to save time */

        /* Check if any cache entry points to the sector which should be erased */
        case FEE_ERASESEC_CHECK_CACHE_E:
        {
            /* Check if the sector index is valid */
            if(Fee_LLEraseOrder_st.xPhySectorIdx_u8 < FEE_NUM_FLASH_BANKS_AVAILABLE)
            {
                for (xCacheIdx_u16 = 0; xCacheIdx_u16 < FEE_NUM_BLOCKS; xCacheIdx_u16++)
                {
                    if ((Fee_Cache_au32[xCacheIdx_u16] >= Fee_FlashProp_st[Fee_LLEraseOrder_st.xPhySectorIdx_u8].Fee_PhysStartAddress_u32) &&
                        (Fee_Cache_au32[xCacheIdx_u16] <= Fee_FlashProp_st[Fee_LLEraseOrder_st.xPhySectorIdx_u8].Fee_PhysEndAddress_u32))
                    {
                        /* Set the Cache to FEE_CACHE_TMP_INVALID_VALUE for all blocks that are in the sector which gets erased */
                        Fee_Cache_au32[xCacheIdx_u16] = FEE_CACHE_TMP_INVALID_VALUE;

                        #if (FEE_PRV_DEBUG_MONITOR != FALSE)
                        /* Set status for Debug monitor */
                        stDBM_u32 = 0x1;
                        xRetVal = FEE_ERROR_E;
                        #endif
                    }
                }
            }
            else
            {
                xRetVal = FEE_ABORTED_E;
                break;
            }

            /* Set next state */
            Fee_LLEraseOrder_st.EraseState_en = FEE_ERASESEC_WRITE_ERASESTARTFLAG_E;
        }
        /* No break to save time */

        /* MR12 RULE 16.3 VIOLATION: Time critical position */
        case FEE_ERASESEC_WRITE_ERASESTARTFLAG_E:
        {
            /* Check if the sector index is valid */
            if(Fee_LLEraseOrder_st.xPhySectorIdx_u8 != FEE_NO_SECTOR_TO_ERASE)
            {
                /* Continue with the next step even if writing of flag failed.
                 * There is a risk for interrupted erase issue coming up but the probability is
                 * very low and to continue is better then trying the program operation infinetly */
                if(FEE_ORDER_PENDING_E != Fee_LLWrEraseStartFlag(Fee_LLEraseOrder_st.xPhySectorIdx_u8))
                {
                    /* Set next state */
                    Fee_LLEraseOrder_st.EraseState_en = FEE_ERASESEC_START_E;
                }
            }
            else
            {
                /* No sector currently selected for an erase operation */
                xRetVal = FEE_ABORTED_E;
            }
        }break;

        /* Start the erase procedure */
        case FEE_ERASESEC_START_E:
        {
            /* Check if the sector index is valid */
            if(Fee_LLEraseOrder_st.xPhySectorIdx_u8 < FEE_NUM_FLASH_BANKS_AVAILABLE)
            {
                xAdStart_uo   = Fee_FlashProp_st[Fee_LLEraseOrder_st.xPhySectorIdx_u8].Fee_PhysStartAddress_u32;

                xSecSize_uo  = (Fee_FlashProp_st[Fee_LLEraseOrder_st.xPhySectorIdx_u8].Fee_PhysEndAddress_u32 + 1u) -
                        Fee_FlashProp_st[Fee_LLEraseOrder_st.xPhySectorIdx_u8].Fee_PhysStartAddress_u32;

                /* Erase the respective sector */
                if(Fls_Erase(xAdStart_uo, xSecSize_uo) == E_OK)
                {
                    /* Set next state */
                    Fee_LLEraseOrder_st.EraseState_en = FEE_ERASESEC_DO_E;
                }
                else
                {
                    /* Order was denied due to wrong parameters or an already busy Fls */
                    xRetVal = FEE_ABORTED_E;
                }

                /* Erase the cache status for any block within this sector
                 * --> not necessary */

                #if (FEE_PRV_DEBUGGING != FALSE)
                Fee_Prv_stModuleTest_st.Fee_EraseTimeStart_u32 = rba_MemLib_GetTimerTicks();
                #endif
            }
            else
            {
                xRetVal = FEE_ABORTED_E;
            }

        }
        break;

        /* Wait for the order to be finished */
        case FEE_ERASESEC_DO_E:
        {
            /* switching state from FEE_ERASESEC_DO --> FEE_ERASESEC_END will be done in
             * Fee_JobEndNotification and Fee_JobErrorNotification */

            /* Check whether the OS is currently running or not */
            if(Fee_stMain == FEE_POLLING_MODE_E)
            {
                /* Call the Fls main function manually */
                Fls_MainFunction();
            }

            #if (STD_ON == FEE_POLLING_MODE)
            /* Use the FLS polling mechanism in order to reach the next state */
            Fee_CheckFlsJobResult();
            #endif
        }
        break;

        /* Write the sector ERASED marker */
        case FEE_ERASESEC_WRITE_MARKER_E:
        {
            xRetVal = Fee_LLWriteMarker(Fee_LLEraseOrder_st.xPhySectorIdx_u8, FEE_ERASED_MARKER_ID_E);

            /* Error reaction not necessary
             * --> RAM structure will be set and used for the identification of the next
             *     sector can be used as long as the Fee_Init is not passed again.
             * --> If init is passed, the sector might get erased again */
        }
        break;

        /* Error case */
        case FEE_ERASESEC_ERROR_E:
        default:
            /* Return with error */
            xRetVal = FEE_ERROR_E;
        break;
    }


    /* Check if order was finished with or without success */
    if (xRetVal != FEE_ORDER_PENDING_E)
    {
        #if (FEE_PRV_DEBUG_MONITOR != FALSE)
        /* Report Debug monitor */
        if (stDBM_u32 > 0)
        {
            Fee_DBM_RepError(Fee_LLEraseOrder_st.xPhySectorIdx_u8, FEE_DBM_ERROR_ERASE_FCT_E, stDBM_u32);
            stDBM_u32 = 0;
        }
        #endif

        /* Reset state machine */
        Fee_LLEraseOrder_st.EraseState_en = FEE_ERASESEC_IDLE_E;

        /* Mark sector state */
        Fee_LLEraseOrder_st.xPhySectorIdx_u8  = FEE_NO_SECTOR_TO_ERASE;

        #if (FEE_PRV_DEBUGGING != FALSE)
                Fee_Prv_stModuleTest_st.Fee_EraseTimeEnd_u32 = rba_MemLib_GetTimerTicks();

        Fee_Prv_stModuleTest_st.Fee_EraseTimeDiff_u32 = rba_MemLib_ConvertTimerTicksToUs(
                                Fee_Prv_stModuleTest_st.Fee_EraseTimeEnd_u32 -
                                Fee_Prv_stModuleTest_st.Fee_EraseTimeStart_u32)/1000u;

        if(Fee_Prv_stModuleTest_st.Fee_EraseTimeDiffMax_u32 <
           Fee_Prv_stModuleTest_st.Fee_EraseTimeDiff_u32)
        {
            Fee_Prv_stModuleTest_st.Fee_EraseTimeDiffMax_u32 =
            Fee_Prv_stModuleTest_st.Fee_EraseTimeDiff_u32;
        }
        #endif
    }

    return (xRetVal);
}

/**
 *********************************************************************
 * Fee_LLWrEraseStartFlag(): Sub function to write Erase Start Flag into 12th marker position
 * of the sector being erased.
 *
 *
 * \param    phySectorIdx_u8    :   Physical sector index which is being erased
 * \return   Function result
 * \retval   FEE_ORDER_PENDING_E:   Order is still pending
 * \retval   FEE_ABORTED_E:         Order was aborted due to a wrong
 *                                  FLS compare function call
 * \retval   FEE_ERROR_E:           Writing of the erase marker is
 *                                  not possible
 * \retval   FEE_ORDER_FINISHED_E:  Order finished successfully
 *                                  (including write operation for the
 *                                   ERASE marker)
 * \seealso
 * \usedresources
 *********************************************************************
 */
static Fee_stRetVal_ten Fee_LLWrEraseStartFlag(uint8 phySectorIdx_u8)
{
    Fee_stRetVal_ten    retVal_en = FEE_ORDER_PENDING_E;

    switch(Fee_LLWrEraseStartFlag_st.state_en)
    {
        /* Calculate the address where the Erase Start Flag has to be programmed. */
        case FEE_LLWR_ERASESTARTFLAG_INIT_E:
        {
            Fee_LLWrEraseStartFlag_st.eraseStartFlagAddr_u32 = Fee_FlashProp_st[phySectorIdx_u8].Fee_PhysStartAddress_u32 +
                                                               (((uint8)FEE_ERASE_START_FLAG_ID_E - 1u)  * FEE_PAGE_LEN);

            Fee_LLWrEraseStartFlag_st.state_en = FEE_LLWR_ERASESTARTFLAG_BLK_CHK_E;
        }break;

        /* Check if the erase start flag is already programmed or not */
        case FEE_LLWR_ERASESTARTFLAG_BLK_CHK_E:
        {
            /* Perform a blank check over the respective marker */
            if(Fls_BlankCheck(Fee_LLWrEraseStartFlag_st.eraseStartFlagAddr_u32, FEE_PAGE_LEN) != E_NOT_OK)
            {
                /* Set the next state for finishing the blank check */
                Fee_LLWrEraseStartFlag_st.state_en = FEE_LLWR_ERASESTARTFLAG_BLK_CHK_WAIT_E;
            }
            else
            {
                /* Return an error due to invalid parameters or a busy / not intialized Fls */
                retVal_en = FEE_ABORTED_E;
            }
        }break;

        /* Wait for the blank check to be finished */
        case FEE_LLWR_ERASESTARTFLAG_BLK_CHK_WAIT_E:
        case FEE_LLWR_ERASESTARTFLAG_WRITE_WAIT_E:
        {
            /* switching state will be done in Fee_JobEndNotification and Fee_JobErrorNotification */

            /* Check whether the OS is currently running or not */
            if(Fee_stMain == FEE_POLLING_MODE_E)
            {
                /* Call the Fls main function manually */
                Fls_MainFunction();
            }

            #if (STD_ON == FEE_POLLING_MODE)
            /* Use the FLS polling mechanism in order to reach the next state */
            Fee_CheckFlsJobResult();
            #endif
        }break;

        /* Page is already programmed */
        case FEE_LLWR_ERASESTARTFLAG_ALREADY_PROGRAMMED_E:
        {
            /* This could happen when the erase operation was aborted and restarted
             * -> no further actions required, return job completed sucessfully */
            retVal_en = FEE_ORDER_FINISHED_E;
        }break;

        /* Page is blank */
        case FEE_LLWR_ERASESTARTFLAG_WRITE_E:
        {
            /* Trigger write of the Erase Start Flag with inverse erase pattern */
            Fee_SrvMemSet8((uint8*)&Fee_MarkerBufBytePtr_cpu8[0], (uint8)~FEE_ERASE_PATTERN, (uint32)FEE_PAGE_LEN);

            if(Fls_Write(Fee_LLWrEraseStartFlag_st.eraseStartFlagAddr_u32, (uint8*)&Fee_MarkerBufBytePtr_cpu8[0], FEE_PAGE_LEN) != E_NOT_OK)
            {
                /* Set the next state to wait for the write order to be finished */
                Fee_LLWrEraseStartFlag_st.state_en = FEE_LLWR_ERASESTARTFLAG_WRITE_WAIT_E;
            }
            else
            {
                /* Abort due to wrong parameters or the Fls being busy / not initialized */
                retVal_en = FEE_ABORTED_E;
            }
        }break;

        /* Verify if the page is not blank.
         * Even on failure it is enough to only check if the page is not blank, it doesnt matter if the data is fully programmed or not */
        case FEE_LLWR_ERASESTARTFLAG_WRITE_ERROR_E:
        {
            /* Return error, to keep the flow simple, there is no retry
             * Also, retry might stress the cell as the retry has to be performed on the same page. */
            retVal_en = FEE_ERROR_E;
        }break;

        /* To keep the flow simple, there is no validation if the programing was successful. it is not important if the correct data was programmed,
         * important that the page is no longer blank. */
        case FEE_LLWR_ERASESTARTFLAG_WRITE_FINISHED_E:
        {
            /* Programming was successful. */
            retVal_en = FEE_ORDER_FINISHED_E;
        }break;

        /* Default state */
        default:
        {
            /* Return aborted */
            retVal_en = FEE_ABORTED_E;
        }break;
    }

    /* Reset variables if the job is completed */
    if (FEE_ORDER_PENDING_E != retVal_en)
    {
        Fee_LLWrEraseStartFlag_st.state_en                  = FEE_LLWR_ERASESTARTFLAG_INIT_E;
        Fee_LLWrEraseStartFlag_st.eraseStartFlagAddr_u32    = FEE_NULL;
    }

    return (retVal_en);
}

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
/* FEE_PRESENT */
#endif
/* 1 == FEE_PRV_CFG_SELECTED_FS */
#endif

