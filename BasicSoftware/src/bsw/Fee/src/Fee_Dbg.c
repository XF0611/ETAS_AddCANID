

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Fee_Idx_Cfg.h"
/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
#if(defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

#include "Fee_Prv.h"

# if (defined(FEE_PRV_CFG_DBG)) && (TRUE == FEE_PRV_CFG_DBG)

#  include "rba_MemLib.h"

#  if (!defined(RBA_MEMLIB_CFG_ENABLE_PROTOTYPE))
#   error Fee debugging features have been enabled but prototypes were not enabled for the Mem stack in rba_MemLib. \
          The debugging features were enabled under BCT container FeeRbDebug.                                       \
          Please enable the BCT switch rba_MemLibCentralEnablePrototype - for more details see rba_MemLib docu.     \
          Fee debugging features shall not be used in series environment.                                           \
          For quick debugging you can also delete these lines losing series maturity.
#  endif

#  if (TRUE == FEE_PRV_CFG_DBG_TIME)
#include "Mcu.h"
#  endif

#  if (TRUE == FEE_PRV_CFG_DBG_DUMP)
#include <stdio.h>
#include "rba_FlsVecu_Vecu.h"
#  endif

#  if (TRUE == FEE_PRV_CFG_DBG_CHECKFATENTRIES)
#include "rba_FeeFs2.h"
#include "rba_FeeFs2_Prv.h"
#  endif


/*
 **********************************************************************************************************************
 * Local macros
 **********************************************************************************************************************
*/
#define FEE_PRV_DBG_OFFSET_FOR_READ_VALIDATION  0u
#define FEE_PRV_DBG_LENGTH_FOR_READ_VALIDATION  1u
// Currently the unit is designed to work only for single instance so its ok to use the macro directly
#define FEE_PRV_DBG_INSTANCE_BEING_TESTED       FEE_INSTANCE_ID

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

#define FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"
Fee_Prv_DbgInfo_tst Fee_Prv_DbgInfo_st;
static Fee_Prv_Dbg_tst Fee_Prv_Dbg_st;
#define FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"

#  if (TRUE == FEE_PRV_CFG_DBG_CHECKLAYOUT)
#define FEE_START_SEC_VAR_CLEARED_8
#include "Fee_MemMap.h"
static uint8 Fee_Prv_DbgCheckLayoutBfr_au8[1u];
#define FEE_STOP_SEC_VAR_CLEARED_8
#include "Fee_MemMap.h"
#  endif

#  if (TRUE == FEE_PRV_CFG_DBG_BLOCK)
#define FEE_START_SEC_VAR_CLEARED_8
#include "Fee_MemMap.h"
uint8 Fee_Prv_DbgBfr_au8[FEE_PRV_CFG_DBG_BLOCK_SIZE];
#define FEE_STOP_SEC_VAR_CLEARED_8
#include "Fee_MemMap.h"
#  endif


/*
 **********************************************************************************************************************
 * Static declarations
 **********************************************************************************************************************
*/

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
#  if ( (TRUE == FEE_PRV_CFG_DBG_TIME) || (TRUE == FEE_PRV_CFG_DBG_ERASECOUNTERS) )
static void Fee_Prv_DbgHistInsert(uint32 * hist_pu32, uint32 value_u32, uint32 histSize_u32);
#  endif
#  if ( (TRUE == FEE_PRV_CFG_DBG_TIME) && (TRUE == FEE_PRV_CFG_DBG_BLOCK) )
static void Fee_Prv_DbgTeMerge(Fee_Prv_DbgTimeEntry_tst * t1_pst, Fee_Prv_DbgTimeEntry_tst const * t2_pcst);
#  endif


/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
*/
/*
 * \brief init function for the variables used in the Dbg unit
 */
void Fee_Prv_DbgInit(void)
{
    /* The contents of Fee_Prv_DbgInfo_st depends on compiler switch settings,
     * i.e. an initialization with rba_MemLib_MemSet is easiest. */
    rba_MemLib_MemSet((uint8 *)(&Fee_Prv_DbgInfo_st), 0u, sizeof(Fee_Prv_DbgInfo_st));

    /* The contents of Fee_Prv_Dbg_st doesn't depend on compiler switch settings, i.e. can be intialized directly. */
    Fee_Prv_Dbg_st.nrDump_u32 = 0u;
    Fee_Prv_Dbg_st.nrDbgBlockWrites_u32 = 0u;
    Fee_Prv_Dbg_st.isDebugBlockWriteRequested_b = FALSE;
    Fee_Prv_Dbg_st.freeze_b = FALSE;

#  if (TRUE == FEE_PRV_CFG_DBG_CHECKLAYOUT)
    rba_MemLib_MemSet(Fee_Prv_DbgCheckLayoutBfr_au8, 0u, sizeof(Fee_Prv_DbgCheckLayoutBfr_au8));
#  endif

#  if (TRUE == FEE_PRV_CFG_DBG_BLOCK)
    rba_MemLib_MemSet(Fee_Prv_DbgBfr_au8, 0u, sizeof(Fee_Prv_DbgBfr_au8));
#  endif

    return;
}

#  if ( (TRUE == FEE_PRV_CFG_DBG_TIME) || (TRUE == FEE_PRV_CFG_DBG_ERASECOUNTERS) )
/**
 * \brief   Insert a value into a histogram.
 *          The histogram is in logarithmic scale with basis of 2.
 *          The intervals of the baskets in the histogram are
 *          [0, 1[
 *          [1, 2[
 *          [2, 4[
 *          [4, 8[
 *          ...
 *          [log(2,histSize_u32 - 1),infinity[
 *
 * \param   hist_pu32       Pointer to the histogram
 * \param   value_u32       Value that shall be inserted into the histogram
 * \param   histSize_u32    Size of the histogram
*/
static void Fee_Prv_DbgHistInsert(uint32 * hist_pu32, uint32 value_u32, uint32 histSize_u32)
{
    /* Local variable */
    uint32 basket_u32 = 0u;

    /* Loop over intervals of histogram */
    /* Evaluation of while loop: The while loop will be exited after a maximum of histSize_u32 calls, which is a derived
     * from paramdef FeeRbDebugTimeHistogramSize - 1.
     * This configured value will be limited due to available RAM space and hence, the below loop will not be stuck infinitely.
     * Moreover, this code is used only for debugging purpose and is not part of the productive software.  */
    while( (value_u32 > (1u << basket_u32)) && (basket_u32 < (histSize_u32 - 1u)) )
    {
        /* Next interval */
        basket_u32++;
    }

    /* Insert value */
    hist_pu32[basket_u32]++;

    return;
}
#  endif

#  if ( (TRUE == FEE_PRV_CFG_DBG_TIME) && (TRUE == FEE_PRV_CFG_DBG_BLOCK) )
/**
 * \brief   Merge histograms and maximum values of two time entries.
 *
 * \param   t1_pst      First time entry, will also be the target of the merge
 * \param   t2_pcst     Second time entry
*/
static void Fee_Prv_DbgTeMerge(Fee_Prv_DbgTimeEntry_tst * t1_pst, Fee_Prv_DbgTimeEntry_tst const * t2_pcst)
{
    /* Local variable */
    uint32 a_u32;

    /* Loop over source histogram */
    for(a_u32 = 0; a_u32 < FEE_PRV_CFG_DBG_TIME_HISTOGRAM_BASKETNUM; a_u32++)
    {
        /* Merge source into target histogram */
        t1_pst->hist_au32[a_u32] += t2_pcst->hist_au32[a_u32];
    }

    /* If maximum value in source histogram is greater ... */
    if(t1_pst->tiMax_u32 < t2_pcst->tiMax_u32)
    {
        /* ... merge this value into target histogram */
        t1_pst->tiMax_u32 = t2_pcst->tiMax_u32;
    }

    return;
}
#  endif

/**
 * \brief   Main function of the debug unit to execute cyclic debugging actions.
*/
void Fee_Prv_DbgMainFunction(void)
{
#  if (TRUE == FEE_PRV_CFG_DBG_BLOCK)

    /* Local variables */
    MemIf_StatusType    status_en = Fee_GetStatus();
    uint32              packAdr_u32;

    /*
     * Only start a write of the debug block under the following conditions:
     * - A write of the debug block is requested
     * - Fee_Rb_EndInit was called (Fee_Prv_EraseEnabled_b = TRUE)
     * - No effort was spent already
     * - The Fee is currently idle
     */
    if(    (TRUE                                       == Fee_Prv_Dbg_st.isDebugBlockWriteRequested_b)
        && (Fee_Prv_LibEffortMeasure_st.effortCtr_u32  == Fee_Prv_ConfigMaxEffort_cu32               )
        && (MEMIF_IDLE                                 == status_en                                  ) )
    {
        packAdr_u32 = 0u;


#   if (TRUE == FEE_PRV_CFG_DBG_TIME)
        /* Space sufficient for the timing measurements? */
        if(sizeof(Fee_Prv_DbgBfr_au8) >= (packAdr_u32 + sizeof(Fee_Prv_DbgInfo_st.time_st)))
        {
            /* Pack timing measurements of debug struct into buffer */
            rba_MemLib_MemCopy(
                (uint8 const *)(&Fee_Prv_DbgInfo_st.time_st),
                &Fee_Prv_DbgBfr_au8[packAdr_u32],
                sizeof(Fee_Prv_DbgInfo_st.time_st)
            );
            packAdr_u32 += sizeof(Fee_Prv_DbgInfo_st.time_st);
        }
#   endif
#   if (TRUE == FEE_PRV_CFG_DBG_ERASECOUNTERS)
        /* Space (also) sufficient for the erase counters? */
        if(sizeof(Fee_Prv_DbgBfr_au8) >= (packAdr_u32 + sizeof(Fee_Prv_DbgInfo_st.eraseCounters_st)))
        {
            /* Pack erase counters of debug struct into buffer */
            rba_MemLib_MemCopy(
                (uint8 const *)(&Fee_Prv_DbgInfo_st.eraseCounters_st),
                &Fee_Prv_DbgBfr_au8[packAdr_u32],
                sizeof(Fee_Prv_DbgInfo_st.eraseCounters_st)
            );
            packAdr_u32 += sizeof(Fee_Prv_DbgInfo_st.eraseCounters_st);
        }
#   endif

        /* Place a write-order at the Fee */
        if(E_OK == Fee_Write(FeeConf_FeeBlockConfiguration_Fee_DebugBlock, &Fee_Prv_DbgBfr_au8[0u]))
        {
            /* Track the number of debug block write for debugging purposes */
            Fee_Prv_Dbg_st.nrDbgBlockWrites_u32++;

            /* Reset write request for debug block */
            Fee_Prv_Dbg_st.isDebugBlockWriteRequested_b = FALSE;
        }
    }
#  endif  /* FEE_PRV_CFG_DBG_BLOCK */

    return;
}

#  if (TRUE == FEE_PRV_CFG_DBG_TIME)
/**
 * \brief   Start a timing measurement.
 *
 * \param   time_pst    Time entry which shall be used for measuring
*/
void Fee_Prv_DbgWatchStart(Fee_Prv_DbgTimeEntry_tst * time_pst)
{
    /* Take start-timestamp */
    time_pst->tiTmp_u32 = Mcu_Rb_GetSysTicks();

    return;
}

/**
 * \brief   Stop a previously started timing measurement and evaluate the result.
 *
 * \param   time_pst            Time entry which was used for starting the measurement
 * \param   updateDebugBlock_b  In case the time is a new maximum trigger storing the debug block
 *
 * \seealso Fee_Prv_DbgWatchStart
*/
void Fee_Prv_DbgWatchStop(Fee_Prv_DbgTimeEntry_tst * time_pst, boolean updateDebugBlock_b)
{
    /* Take stop-timestamp to update last ... */
    time_pst->tiLast_u32 = MCU_RB_TICKS_TO_US(Mcu_Rb_GetSysTicks() - time_pst->tiTmp_u32);

    /* ... and accumulated timing measurement */
    time_pst->tiAcc_u32 += time_pst->tiLast_u32;

    /* Last timing measurement exceeds the maximum? */
    if(time_pst->tiMax_u32 < time_pst->tiLast_u32)
    {
        /* Update maximum timing measurement */
        time_pst->tiMax_u32 = time_pst->tiLast_u32;

        /* If requested, trigger storing the debug info struct */
        if(TRUE == updateDebugBlock_b)
        {
            Fee_Prv_Dbg_st.isDebugBlockWriteRequested_b = TRUE;
        }
    }

    /* Insert new timing measurement into histogram */
    Fee_Prv_DbgHistInsert(&time_pst->hist_au32[0], time_pst->tiLast_u32, FEE_PRV_CFG_DBG_TIME_HISTOGRAM_BASKETNUM);

    return;
}
#  endif  /* FEE_PRV_CFG_DBG_TIME */

#  if (TRUE == FEE_PRV_CFG_DBG_ERASECOUNTERS)
/**
 * \brief   Track the erase of a sector in the debug block.
 *
 * \param   ID of the sector that shall be tracked
*/
void Fee_Prv_DbgEraseCountersTrack(rba_FeeFs2_Prv_Sector_tst const * sector_pcst)
{
    /* Local variable */
    uint32 id_u32 = rba_FeeFs2_Prv_SectorGetId(sector_pcst);

    /* Increment the sector specific erase counter */
    Fee_Prv_DbgInfo_st.eraseCounters_st.sectorDeleteAbs_au32[id_u32]++;

    /*
     * Find out the erase delta and remember it in the histogram.
     * Do not track the delta if the sector counter is 0.
     * In case the sector counter is 0 the sector was in undefined state on startup.
     */
    if(0u < sector_pcst->counter_u32)
    {
        uint32 delta_u32 = rba_FeeFs2_Prv_SectorGetMaxCounter() - sector_pcst->counter_u32;

        /* Insert erase counter deltas into histogram */
        Fee_Prv_DbgHistInsert(
            &Fee_Prv_DbgInfo_st.eraseCounters_st.sectorDeleteDelta_aau32[id_u32][0u],
            delta_u32,
            FEE_PRV_CFG_DBG_ERASECOUNTERS_HISTOGRAM_BASKETNUM
        );
    }

    /* Trigger storing the debug info struct */
    Fee_Prv_Dbg_st.isDebugBlockWriteRequested_b = TRUE;

    return;
}
#  endif  /* FEE_PRV_CFG_DBG_ERASECOUNTERS */

#  if (TRUE == FEE_PRV_CFG_DBG_CHECKLAYOUT)
/**
 * \brief   Check that all blocks in a list are present - if not, go in a endless loop.
 *          This function is useful for reset tests.
 *          When a block gets lost this function prevents that the dump is modified further.
*/
void Fee_Prv_DbgCheckLayout(void)
{
    /* Local variables */
    uint32                         a_u32;
    Fee_Prv_ConfigRequester_ten    requester_en;
    MemIf_JobResultType            result_en;
    Fls_Rb_FlsDevice_ten           flsDeviceId_en     = Fls_Rb_GetDeviceIdFromDrvIndex(FEE_PRV_DBG_INSTANCE_BEING_TESTED);
    Fee_Rb_DeviceName_ten          feeDeviceName_en   = Fee_Rb_GetDeviceNameFromDeviceIndex(FEE_PRV_DBG_INSTANCE_BEING_TESTED);

    /* Loop over all blocks, which are configured to be checked */
    /* MR12 RULE 15.4 VIOLATION: Using 2 breaks has no side effects, first break is reached when Fee_Read() fails. and second when block has not been found */
    for(a_u32 = 0; a_u32 < (sizeof(Fee_Prv_DbgCheckLayoutBlocklist_au16)/sizeof(uint16)); a_u32++)
    {
        /* Place a read-order at the Fee */
        if(E_OK == Fee_Rb_Idx_Read(feeDeviceName_en,
                                   Fee_Prv_DbgCheckLayoutBlocklist_au16[a_u32],
                                   FEE_PRV_DBG_OFFSET_FOR_READ_VALIDATION,
                                   &Fee_Prv_DbgCheckLayoutBfr_au8[0],
                                   FEE_PRV_DBG_LENGTH_FOR_READ_VALIDATION))
        {
            /* Check block configuration to find out the block's requester */
            requester_en = Fee_Prv_ConfigGetBlockRequesterByBlockNr(Fee_Prv_DbgCheckLayoutBlocklist_au16[a_u32]);

            /* Call Fee and Fls until order is completely processed */
            do
            {
                Fee_Rb_Idx_MainFunction(feeDeviceName_en);
                FEE_RB_SYNC_LOOP_FLS_MAIN_HOOK();
                Fee_Prv_Lib_Fls_MainFunction(flsDeviceId_en);

                /* Query job status depending on the block's requester */
                result_en = Fee_Prv_OrderGetJobResult(feeDeviceName_en, requester_en);

            } while(MEMIF_JOB_PENDING == result_en);
            /* Evaluation of while loop: In worst case the while loop will be stuck infinitely when the given Fls job is
             * not completed. This can be handled by using the timeout feature of Fls.
             * Moreover, this code is used only for debugging purpose and is not part of the productive software.  */

            /* Block lost (i.e. MEMIF_BLOCK_INCONSISTENT, MEMIF_JOB_PENDING/MEMIF_JOB_CANCELED are impossible here)? */
            if( (MEMIF_JOB_OK != result_en) && (MEMIF_BLOCK_INVALID != result_en) )
            {
                /* Block was lost - freeze the system */
                Fee_Prv_Dbg_st.freeze_b = TRUE;
                break;
            }
        }
        else
        {
            /* Something strange is going on - freeze the system */
            Fee_Prv_Dbg_st.freeze_b = TRUE;
            break;
        }
    }

    /* Endless loop to prevent that the dump is further modified */
    /* Evaluation of while loop: The endless loop is done intentionally to be able to debug why the block read failed.
     * This code is used only for debugging purpose and is not part of the productive software. Hence, the loop is safe.  */
    while(TRUE == Fee_Prv_Dbg_st.freeze_b)
    {
        Fee_Prv_DbgCheckLayoutBfr_au8[0]++;
    }

    return;
}
#  endif  /* FEE_PRV_CFG_DBG_CHECKLAYOUT */

#  if (TRUE == FEE_PRV_CFG_DBG_CHECKFATENTRIES)
/**
 * \brief   Check that all blocks in a list are present after a FAT sector change in the new FAT sector.
 *          If not, go in a endless loop.
 *          This function is useful for reset tests to preserve the history of the old FAT sector.
*/
void Fee_Prv_DbgCheckFatEntries(void)
{
    /* Local variables */
    rba_FeeFs2_Prv_FatEntry_tst const * fe_pcst;
    rba_FeeFs2_Prv_Sector_tst   const * fatSec_pcst;
    uint32                              found_au32[sizeof(Fee_Prv_DbgCheckLayoutBlocklist_au16)/sizeof(uint16)];
    uint32                              a_u32;

    /* Get new FAT sector */
    fatSec_pcst = rba_FeeFs2_Prv_SectorGetLastFatActive();

    /* Set all to 0 */
    rba_MemLib_MemSet((uint8 *)found_au32, 0u, sizeof(found_au32));

    /* Loop over all entries inside new FAT sector */
    /* MR12 RULE 11.4 VIOLATION: Cast necessary for direct read from FAT sector for debugging purpose */
    for(    fe_pcst = (rba_FeeFs2_Prv_FatEntry_tst const *)(rba_FeeFs2_Prv_SectorGetDataBegin(fatSec_pcst)  );
            fe_pcst < (rba_FeeFs2_Prv_FatEntry_tst const *)(fatSec_pcst->adrEnd_u32 + 1u                    );
            fe_pcst++                                                                                       )
    {
        /* Loop over all blocks, which are configured to be checked */
        for(a_u32 = 0u; a_u32 < (sizeof(Fee_Prv_DbgCheckLayoutBlocklist_au16)/sizeof(uint16)); a_u32++)
        {
            /* FAT entry exists for block to be checked? */
            if(fe_pcst->persistentId_u16 == Fee_Prv_ConfigGetBlockPersistentIdByBlockNr(Fee_Prv_DbgCheckLayoutBlocklist_au16[a_u32]))
            {
                /* Memorize hit and continue with next FAT entry */
                found_au32[a_u32] = 1u;
                break;
            }
        }
    }

    /* Loop over all blocks expected in the FAT */
    for(a_u32 = 0u; a_u32 < (sizeof(found_au32)/sizeof(uint32)); a_u32++)
    {
        /* FAT entry missing for block? */
        if(0u == found_au32[a_u32])
        {
            /* Something strange is going on - freeze the system */
            Fee_Prv_Dbg_st.freeze_b = TRUE;
        }
    }

    /* Endless loop to prevent that the dump is further modified */
    /* Evaluation of while loop: The endless loop is done intentionally to be able to debug why the blocks are lost.
     * This code is used only for debugging purpose and is not part of the productive software. Hence, the loop is safe.  */
    while(TRUE == Fee_Prv_Dbg_st.freeze_b)
    {
        Fee_Prv_DbgCheckLayoutBfr_au8[0]++;
    }

    return;
}
#  endif  /* FEE_PRV_CFG_DBG_CHECKFATENTRIES */

#  if (TRUE == FEE_PRV_CFG_DBG_DUMP)
/**
 * \brief   Create a dump in the Fee development simulation.
 *          This is only active during development.
*/
void Fee_Prv_DbgDump(void)
{
    /* Local variable */
    char filename_ac[50];

    /* Create numbered dump */
    sprintf(filename_ac, "Fee_Dump%u", Fee_Prv_Dbg_st.nrDump_u32);

    char Path_achr[RBA_BSWDLT_PRV_FILEPATH_LEN];
    strcpy(Path_achr, "_log\\");
    strcat(Path_achr, filename_ac);
    rba_FlsVecu_Vecu_SaveDump_Data(
            Fls_Rb_FlsDevice,
            FEE_PRV_CFG_FLS_BASE_ADR + FEE_PHYS_SEC_START0,
            FEE_PRV_CFG_FLS_SIZE,
            Path_achr,
            RBA_FLSVECU_FILEFORMAT_TEXT );

    /* Prepare for next dump */
    Fee_Prv_Dbg_st.nrDump_u32++;

    return;
}

/**
 * \brief   Create a dump if the job result is failed.
 *          Useful for finding unexpected errors.
 *          This is only active during development.
 *
 * \param   Job result
*/
void Fee_Prv_DbgFailDump(MemIf_JobResultType result_en)
{
    /* Job has failed? */
    if(MEMIF_JOB_FAILED == result_en)
    {
        /* Create a dump */
        Fee_Prv_DbgDump();
    }

    return;
}
#  endif  /* FEE_PRV_CFG_DBG_DUMP */

#  if (TRUE == FEE_PRV_CFG_DBG_BLOCK)
void Fee_Prv_DbgBlockRead(void)
{
    /* Local variables */
    Fee_Prv_JobDesc_tst             order_st;
    MemIf_JobResultType             jobResult_en;
    MemIf_Rb_MigrationResult_ten    dummyMigrationResult_en;
    uint32                          packAdr_u32;
#   if (TRUE == FEE_PRV_CFG_DBG_TIME)
    Fee_Prv_DbgTimeEntry_tst        init_st;
#   endif /* FEE_PRV_CFG_DBG_TIME */
    Fls_Rb_FlsDevice_ten            flsDeviceId_en     = Fls_Rb_GetDeviceIdFromDrvIndex(FEE_PRV_DBG_INSTANCE_BEING_TESTED);
    Fee_Rb_DeviceName_ten           feeDeviceName_en   = Fee_Rb_GetDeviceNameFromDeviceIndex(FEE_PRV_DBG_INSTANCE_BEING_TESTED);

    /* Setup order */
    order_st.type_en                = FEE_PRV_JOBTYPE_READ_E;
    order_st.bfr_pu8                = (uint8 *)(&Fee_Prv_DbgBfr_au8[0u]);
    order_st.blockNumber_u16        = FeeConf_FeeBlockConfiguration_Fee_DebugBlock;
    order_st.length_u16             = FEE_PRV_CFG_DBG_BLOCK_SIZE;
    order_st.offset_u16             = 0u;
    order_st.migrationResult_pen    = &dummyMigrationResult_en;

    /* Perform the read */
    if(E_OK == Fee_Rb_Idx_Read(feeDeviceName_en,
                               FeeConf_FeeBlockConfiguration_Fee_DebugBlock,
                               FEE_PRV_DBG_OFFSET_FOR_READ_VALIDATION,
                               &Fee_Prv_DbgBfr_au8[0],
                               FEE_PRV_CFG_DBG_BLOCK_SIZE))
    {
        do
        {
            Fee_Rb_Idx_MainFunction(feeDeviceName_en);
            FEE_RB_SYNC_LOOP_FLS_MAIN_HOOK();
            Fee_Prv_Lib_Fls_MainFunction(flsDeviceId_en);

            /* Query job status depending on the block's requester */
            jobResult_en = Fee_Rb_Idx_GetJobResult(feeDeviceName_en);
        } while(MEMIF_JOB_PENDING == jobResult_en);
        /* Evaluation of while loop: In worst case the while loop will be stuck infinitely when the given Fls job is
         * never completed. This can be handled by using the timeout feature of Fls.
         * Moreover, this code is used only for debugging purpose and is not part of the productive software.  */
    }
    else
    {
        /* Software should not reach here. anyways treat this as a read error. could also be changed to MEMIF_BLOCK_INCONSISTENT */
        jobResult_en = MEMIF_JOB_FAILED;
    }

    /* Read has failed? */
    if(MEMIF_JOB_OK != jobResult_en)
    {
        /* Set all to 0 */
        rba_MemLib_MemSet(Fee_Prv_DbgBfr_au8, 0u, sizeof(Fee_Prv_DbgBfr_au8));
    }

    packAdr_u32 = 0u;

#   if (TRUE == FEE_PRV_CFG_DBG_TIME)
    /*
     * This function shall only be called at the end of Fee_Init.
     * At this place a value for the init time is already available.
     * This measurement shall not be overwritten but merged with the already available data.
     */
    init_st = Fee_Prv_DbgInfo_st.time_st.orderInit_st;

    /* Space sufficient for the timing measurements? */
    if(sizeof(Fee_Prv_DbgBfr_au8) >= (packAdr_u32 + sizeof(Fee_Prv_DbgInfo_st.time_st)))
    {
        /* Copy the read timing measurements into debug struct */
        rba_MemLib_MemCopy(
            &Fee_Prv_DbgBfr_au8[packAdr_u32],
            (uint8 *)(&Fee_Prv_DbgInfo_st.time_st),
            sizeof(Fee_Prv_DbgInfo_st.time_st)
        );
        packAdr_u32 += sizeof(Fee_Prv_DbgInfo_st.time_st);
    }

    /* Merge read and taken (init) time measurement */
    Fee_Prv_DbgTeMerge(&Fee_Prv_DbgInfo_st.time_st.orderInit_st, &init_st);
#   endif /* FEE_PRV_CFG_DBG_TIME */

#   if (TRUE == FEE_PRV_CFG_DBG_ERASECOUNTERS)
    /* Space (also) sufficient for the erase counters? */
    if(sizeof(Fee_Prv_DbgBfr_au8) >= (packAdr_u32 + sizeof(Fee_Prv_DbgInfo_st.eraseCounters_st)))
    {
        /* Copy the read erase counters into debug struct */
        rba_MemLib_MemCopy(
            &Fee_Prv_DbgBfr_au8[packAdr_u32],
            (uint8 *)(&Fee_Prv_DbgInfo_st.eraseCounters_st),
            sizeof(Fee_Prv_DbgInfo_st.eraseCounters_st)
        );
        packAdr_u32 += sizeof(Fee_Prv_DbgInfo_st.eraseCounters_st);
    }
#   endif  /* FEE_PRV_CFG_DBG_ERASECOUNTERS */

    return;
}
#  endif  /* FEE_PRV_CFG_DBG_BLOCK */

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

# endif  /* FEE_PRV_CFG_DBG */

#endif
