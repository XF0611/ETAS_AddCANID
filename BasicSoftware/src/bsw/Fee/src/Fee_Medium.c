

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Fee_Idx_Cfg.h"

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
#if(defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

#include "Crc.h"
#include "Fee_Prv.h"
#include "rba_MemLib.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

#define FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"
/* Static */
static Fee_Prv_MediumRead_tst               Fee_Prv_MediumRead_st;
static Fee_Prv_MediumCalcCrc_tst            Fee_Prv_MediumCalcCrc_st;
static Fee_Prv_MediumLoad_tst               Fee_Prv_MediumLoad_st;
static Fee_Prv_MediumWrite_tst              Fee_Prv_MediumWrite_st;
static Fee_Prv_MediumCopy_tst               Fee_Prv_MediumCopy_st;
static Fee_Prv_MediumCompareFls2Fls_tst     Fee_Prv_MediumCompareFls2Fls_st;
static Fee_Prv_Medium_tst                   Fee_Prv_Medium_st;
#define FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"

/*
 * In case of all three conditions listed below are fulfilled
 * - DMA is writing into the buffer
 * - data cache controller (CC) is enabled
 * - buffer is linked to a cached memory region
 *
 * Fee_Prv_MediumBuffer_un has to be fully cache line aligned (start address has to be aligned to the start of a
 * cache line and the buffer size has to be multiple of cache line size).
 * The DMA is always bypassing the CC and has to invalidate the cache lines before writing to it,
 * in order to keep cache coherence between DMA (direct writer without CC) and core (reader via CC).
 * Otherwise if the cache line is shared with data of another application, the data may get corrupted.
*/
#define FEE_START_SEC_VAR_CLEARED_SECURED_CACHE_LINE_ALIGNED_UNSPECIFIED
#include "Fee_MemMap.h"
static Fee_Prv_MediumBuffer_tun     Fee_Prv_MediumBuffer_un;
#define FEE_STOP_SEC_VAR_CLEARED_SECURED_CACHE_LINE_ALIGNED_UNSPECIFIED
#include "Fee_MemMap.h"

# if (defined(FLS_RB_ROBUST_PROG_STATUS_API) && (STD_OFF != FLS_RB_ROBUST_PROG_STATUS_API))
# define FEE_START_SEC_VAR_CLEARED_SECURED_8
# include "Fee_MemMap.h"
static uint8 Fee_Prv_MediumRescueBfr_au8[RBA_FLS_WORDLINE_SIZE];
# define FEE_STOP_SEC_VAR_CLEARED_SECURED_8
# include "Fee_MemMap.h"
# endif

/*
 **********************************************************************************************************************
 * Static declarations
 **********************************************************************************************************************
*/

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"

static MemIf_JobResultType Fee_Prv_MediumFlsDo(void);
static void Fee_Prv_MediumSetBfrDirty(void);
static MemIf_JobResultType Fee_Prv_MediumLoad(uint32 adr_u32, uint32 length_u32, uint32 maxPrefetchLen_u32, Fee_Prv_MediumLoadMode_ten loadMode_en);
static void Fee_Prv_MediumProgram(uint32 adr_u32, void const * bfr_pcv, uint32 length_u32);
static MemIf_JobResultType Fee_Prv_MediumProgramDo(void);

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
*/

LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_BlankCheck         (Fee_Prv_MediumWrite_tst * fsm_pst);
#if(FALSE == FEE_RB_WRITE_PADDING_REQUIRED)
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_CopyDataWithoutPadding(Fee_Prv_MediumWrite_tst * fsm_pst);
#define Fee_Prv_MediumWrite_CopyData(fsm_pst)        Fee_Prv_MediumWrite_CopyDataWithoutPadding(fsm_pst)
#else
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_CopyDataWithPadding(Fee_Prv_MediumWrite_tst * fsm_pst);
#define Fee_Prv_MediumWrite_CopyData(fsm_pst)        Fee_Prv_MediumWrite_CopyDataWithPadding(fsm_pst)
#endif
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_Program            (Fee_Prv_MediumWrite_tst * fsm_pst);
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_Compare            (Fee_Prv_MediumWrite_tst * fsm_pst);

LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumCompareFls2Fls_Read   (Fee_Prv_MediumCompareFls2Fls_tst * fsm_pst);
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumCompareFls2Fls_Compare(Fee_Prv_MediumCompareFls2Fls_tst * fsm_pst);

LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumCopy_Read (Fee_Prv_MediumCopy_tst * fsm_pst);
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumCopy_Write(Fee_Prv_MediumCopy_tst * fsm_pst);

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
*/

/**
 * \brief   Check the status of flash jobs.
 *          In synchronous mode call the Fls_MainFunction until the job is not pending any more.
 *          To reduce the number of software paths, MEMIF_JOB_FAILED and E_NOT_OK are both mapped to MEMIF_JOB_FAILED.
 *          This way the higher layers do not know whether an error occurred during job placing or job execution.
 *          In the end the reaction is typically always the same: Retry or abort.
 *          In order to differentiate between hardware failures (with return value MEMIF_JOB_FAILED) and comparison
 *          mismatches of Fls compare jobs, the return value MEMIF_BLOCK_INCONSISTENT is kept for the latter. For
 *          these two situations, different reaction strategies can be required.
 *
 * \retval  MEMIF_JOB_PENDING          Job still pending
 * \retval  MEMIF_JOB_OK               Job finished successfully
 * \retval  MEMIF_JOB_FAILED           Job finished with errors
 * \retval  MEMIF_BLOCK_INCONSISTENT   Compare job finished with mismatch between contents in flash and data buffer
 *                                     Blank Check job has been executed and the result is not blank
*/
static MemIf_JobResultType Fee_Prv_MediumFlsDo(void)
{
    MemIf_JobResultType result_en = MEMIF_JOB_FAILED;
    if(E_OK == Fee_Prv_Medium_st.lastFlsResult_en)
    {
        if(TRUE == Fee_Prv_Medium_st.sync_b)
        {
            do
            {
                /*
                 * The hook function call enables the user to serve tasks, which might be needed to keep the system
                 * stable during the runtime of the loop.
                 */
                FEE_RB_SYNC_LOOP_FLS_MAIN_HOOK();
                Fee_Prv_Lib_Fls_MainFunction(Fee_Prv_Medium_st.flsDevId_en);
                result_en = Fee_Prv_Lib_Fls_GetJobResult(Fee_Prv_Medium_st.flsDevId_en, Fee_Prv_Medium_st.flsAdr_u32);
            } while(MEMIF_JOB_PENDING == result_en);
            /* Evaluation of while loop: This while loop is executed only during init phase, which operates in synchronous
             * mode. In worst case the while loop will be stuck infinitely when the given Fls job is never completed.
             * This can be handled by using the timeout feature of Fls.
             * Also, for longer execution time, projects could use the FeeRbSyncFlsMainLoopHook for safe handling. */
        }
        else
        {
            result_en = Fee_Prv_Lib_Fls_GetJobResult(Fee_Prv_Medium_st.flsDevId_en, Fee_Prv_Medium_st.flsAdr_u32);
        }
        Fee_Prv_DbgFailDump(result_en);
    }
    else
    {
        Fee_Prv_DbgDump();
    }

    return(result_en);
}

/**
 * \brief   Mark the medium buffer dirty
*/
static void Fee_Prv_MediumSetBfrDirty(void)
{
    Fee_Prv_Medium_st.limitLo_u32 = 0u;
    Fee_Prv_Medium_st.limitHi_u32 = 0u;

    return;
}

/**
 * \brief   Check if a block located at flash address <adr_u32> and length <length_u32> is fully loaded into Medium buffer.
 *
 * \param   adr_u32     Start address of the block in the flash
 * \param   length_u32  Length of the block
*/
static boolean Fee_Prv_MediumIsBfrHit(uint32 adr_u32, uint32 length_u32)
{
    boolean result_b;

    if((Fee_Prv_Medium_st.limitLo_u32 <= (adr_u32 + 0u         )) &&
       (Fee_Prv_Medium_st.limitHi_u32 >= (adr_u32 + length_u32 )))
    {
        result_b = TRUE;
    }
    else
    {
        result_b = FALSE;
    }

    return(result_b);
}

/**
 * \brief   Load some data from the flash to the RAM in the medium buffer.
 *          This is useful for buffered access to the flash.
 *
 * \param   adr_u32             Address in the flash to load from
 * \param   length_u32          Amount of bytes to load
 * \param   maxPrefetchLen_u32  Maximum amount of data to load including prefetch
 * \param   loadMode_en         Load direction
 *
 * \retval  MEMIF_JOB_PENDING   Loading still going on
 * \retval  MEMIF_JOB_OK        Loading finished successfully
 * \retval  MEMIF_JOB_FAILED    Loading finished with an error (in this case the content of the buffer is undefined
*/
static MemIf_JobResultType Fee_Prv_MediumLoad(uint32 adr_u32, uint32 length_u32, uint32 maxPrefetchLen_u32, Fee_Prv_MediumLoadMode_ten loadMode_en)
{
    MemIf_JobResultType result_en = MEMIF_JOB_PENDING;

    if(Fee_Prv_MediumIsBfrHit(adr_u32, length_u32))
    {
        result_en = MEMIF_JOB_OK;
    }
    else
    {
        if(FEE_PRV_LOAD_BUSY_E != Fee_Prv_MediumLoad_st.state_en)
        {
            Fee_Prv_MediumLoad_st.nrBytes2Load_u32 = Fee_Prv_LibMin(sizeof(Fee_Prv_MediumBuffer_un.u8), maxPrefetchLen_u32);

            if
            (
                /*Avoid compiler warning for relative addressing where FEE_CFG_EMULATION_START is zero*/
# if(FEE_CFG_EMULATION_START != 0u)
                (FEE_CFG_EMULATION_START <= (adr_u32 + 0u)                          ) && /* Address must be valid DFLASH address                         */
# endif
                (FEE_CFG_EMULATION_END   >= (adr_u32 + length_u32)                  ) && /* Address must be valid DFLASH address                         */
                (length_u32              <= Fee_Prv_MediumLoad_st.nrBytes2Load_u32  ) && /* At least requested amount of data has to be loaded           */
                (0u                      == ((FEE_VIRTUAL_PAGE_SIZE - 1u) & adr_u32))    /* Only allow page aligned accesses to avoid alignment problems */
            )
            {
                if(FEE_PRV_POSTLOAD_E == loadMode_en)
                {
                    Fee_Prv_MediumLoad_st.reloadAdr_u32 = Fee_Prv_LibMin(adr_u32,
                                                                         FEE_CFG_EMULATION_END - Fee_Prv_MediumLoad_st.nrBytes2Load_u32);
                }
                else
                {
                    Fee_Prv_MediumLoad_st.reloadAdr_u32 = Fee_Prv_LibMax((adr_u32 - Fee_Prv_MediumLoad_st.nrBytes2Load_u32) + length_u32,
                                                                         FEE_CFG_EMULATION_START);
                }

                /* Set buffer dirty */
                Fee_Prv_MediumSetBfrDirty();

                /* In case of maxPrefetchLen_u32 and length_u32 is 0, avoid data read and return MEMIF_JOB_OK */
                if(0u < Fee_Prv_MediumLoad_st.nrBytes2Load_u32)
                {
                    /* Start buffer load */
                    /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
                    Fee_Prv_MediumRead(Fee_Prv_MediumLoad_st.reloadAdr_u32, &Fee_Prv_MediumBuffer_un.u8[0u], Fee_Prv_MediumLoad_st.nrBytes2Load_u32);

                    Fee_Prv_MediumLoad_st.state_en = FEE_PRV_LOAD_BUSY_E;
                }
                else
                {
                    result_en = MEMIF_JOB_OK;
                }
            }
            else
            {
                result_en = MEMIF_JOB_FAILED;
            }
        }

        if(MEMIF_JOB_PENDING == result_en)
        {
            result_en = Fee_Prv_MediumReadDo();
        }

        if(MEMIF_JOB_PENDING != result_en)
        {
            Fee_Prv_MediumLoad_st.state_en = FEE_PRV_LOAD_IDLE_E;

            if(MEMIF_JOB_OK == result_en)
            {
                Fee_Prv_Medium_st.limitLo_u32 = Fee_Prv_MediumLoad_st.reloadAdr_u32;
                Fee_Prv_Medium_st.limitHi_u32 = Fee_Prv_MediumLoad_st.reloadAdr_u32 + Fee_Prv_MediumLoad_st.nrBytes2Load_u32;
            }
        }
    }

    return(result_en);
}

/**
 * \brief   Start a programming operation on the flash.
 *          Contains the sole call of Fls_Write in the whole Fee!
 *
 * \param   adr_u32     Address to write to
 * \param   bfr_pcv     Buffer to write from
 * \param   length_u32  Amount of bytes to write
*/
static void Fee_Prv_MediumProgram(uint32 adr_u32, void const * bfr_pcv, uint32 length_u32)
{
    /* For absolute addressing, convert relative to absolute address */
    uint32 adrCnv_u32 = adr_u32 + FEE_PRV_CFG_FLS_BASE_ADR;

    Fee_Prv_MediumSetBfrDirty();

    Fee_Prv_Medium_st.flsAdr_u32 = adrCnv_u32;

    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    Fee_Prv_Medium_st.lastFlsResult_en = Fee_Prv_Lib_Fls_Write(Fee_Prv_Medium_st.flsDevId_en, Fee_Prv_Medium_st.flsAdr_u32, bfr_pcv, length_u32);

    return;
}

/**
 * \brief   Do the work for a program job.
*/
static MemIf_JobResultType Fee_Prv_MediumProgramDo(void)
{
    return(Fee_Prv_MediumFlsDo());
}

/**
 * \brief   Set synchronous (polling) or asynchronous flash access.
 *
 * \param   newSync_b   TRUE = synchronous, FALSE = asynchronous
 *
 * \attention   If the flash access is set to synchronous the Fee_MainFunction can take very long!
 *              Calling this function must consider the scheduling environment.
 *              Typically synchronous flash access is only acceptable in init.
*/
void Fee_Prv_MediumSetSync(boolean newSync_b)
{
    Fee_Prv_Medium_st.sync_b = newSync_b;

    return;
}

/**
 * \brief   initialize buffer limits
 *
 * \param   FeeDeviceName_en    Device instance for which init has to be performed
*/
void Fee_Prv_MediumInit(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    (void)FeeDeviceName_en;

    // setting the limits implicitly sets the buffer dirty --> reload
    Fee_Prv_Medium_st.iterator_u32       = 0u;
    Fee_Prv_Medium_st.limitHi_u32        = 0u;
    Fee_Prv_Medium_st.limitLo_u32        = 0u;
    Fee_Prv_Medium_st.maxPrefetchLen_u32 = 0u;
    Fee_Prv_Medium_st.lastFlsResult_en   = E_NOT_OK;
    Fee_Prv_Medium_st.sync_b             = FALSE;
    Fee_Prv_Medium_st.flsDevId_en     = Fls_Rb_GetDeviceIdFromDrvIndex(Fee_Rb_GetDeviceIndexFromDeviceName(FeeDeviceName_en));
    Fee_Prv_Medium_st.flsAdr_u32     = 0u;

    Fee_Prv_MediumRead_st.adr_pcu8        = NULL_PTR;
    Fee_Prv_MediumRead_st.bfr_pu8         = NULL_PTR;
    Fee_Prv_MediumRead_st.length_u32      = 0u;
    Fee_Prv_MediumRead_st.nrBytesDone_u32 = 0u;
    Fee_Prv_MediumRead_st.isBufferHit_b   = FALSE;

    Fee_Prv_MediumCalcCrc_st.crc_pu32        = NULL_PTR;
    Fee_Prv_MediumCalcCrc_st.firstCall_b     = TRUE;
    Fee_Prv_MediumCalcCrc_st.length_u32      = 0u;
    Fee_Prv_MediumCalcCrc_st.nrBytesDone_u32 = 0u;

    Fee_Prv_MediumLoad_st.reloadAdr_u32    = 0u;
    Fee_Prv_MediumLoad_st.state_en         = FEE_PRV_LOAD_IDLE_E;
    Fee_Prv_MediumLoad_st.nrBytes2Load_u32 = 0u;

    Fee_Prv_MediumWrite_st.state_en          = FEE_PRV_MEDIUM_WRITE_IDLE_E;
    Fee_Prv_MediumWrite_st.entry_b           = TRUE;
    Fee_Prv_MediumWrite_st.data2Program_pcv  = NULL_PTR;
    Fee_Prv_MediumWrite_st.adrDest_u32       = 0u;
    Fee_Prv_MediumWrite_st.bfr_pcv           = NULL_PTR;
    Fee_Prv_MediumWrite_st.bytes2Program_u32 = 0u;
    Fee_Prv_MediumWrite_st.done_u32          = 0u;
    Fee_Prv_MediumWrite_st.length_u32        = 0u;
    Fee_Prv_MediumWrite_st.isMedBfrByp_b     = FALSE;

    Fee_Prv_MediumCopy_st.state_en                  = FEE_PRV_MEDIUM_COPY_IDLE_E;
    Fee_Prv_MediumCopy_st.entry_b                   = TRUE;
    Fee_Prv_MediumCopy_st.adrDest_u32               = 0u;
    Fee_Prv_MediumCopy_st.adrSrc_u32                = 0u;
    Fee_Prv_MediumCopy_st.bytes2Copy_u32            = 0u;
    Fee_Prv_MediumCopy_st.done_u32                  = 0u;
    Fee_Prv_MediumCopy_st.length_u32                = 0u;
    Fee_Prv_MediumCopy_st.writeSeq_en               = FEE_PRV_MEDIUM_SEQ_WRITE_BLNKCHK_COMP_E;

    Fee_Prv_MediumCompareFls2Fls_st.state_en        = FEE_PRV_MEDIUM_COMPAREFLS2FLS_IDLE_E;
    Fee_Prv_MediumCompareFls2Fls_st.entry_b         = TRUE;
    Fee_Prv_MediumCompareFls2Fls_st.adrFls1_u32     = 0u;
    Fee_Prv_MediumCompareFls2Fls_st.adrFls2_u32     = 0u;
    Fee_Prv_MediumCompareFls2Fls_st.length_u32      = 0u;
    Fee_Prv_MediumCompareFls2Fls_st.nrBytesDone_u32 = 0u;

# if (defined(FLS_RB_ROBUST_PROG_STATUS_API) && (STD_OFF != FLS_RB_ROBUST_PROG_STATUS_API))
    rba_MemLib_MemSet(&Fee_Prv_MediumRescueBfr_au8[0], 0u , sizeof(Fee_Prv_MediumRescueBfr_au8));
# endif

    return;
}

/**
 * \brief   Check if data to be read are already cached inside the Medium buffer otherwise start a read operation from the flash.
 *          Contains the sole call of Fls_Read in the whole Fee!
 *
 * \param   adr_u32     Address to read from
 * \param   bfr_pv      Buffer to read to
 * \param   length_u32  Amount of bytes to read
*/
void Fee_Prv_MediumRead(uint32 adr_u32, void * bfr_pv, uint32 length_u32)
{
    uint32 index_u32;
    uint32 adrCnv_u32;

    if(Fee_Prv_MediumIsBfrHit(adr_u32, length_u32))
    {
        /* Data are cached */
        index_u32 = adr_u32 - Fee_Prv_Medium_st.limitLo_u32;

        Fee_Prv_MediumRead_st.adr_pcu8        = &Fee_Prv_MediumBuffer_un.u8[index_u32];
        /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
        Fee_Prv_MediumRead_st.bfr_pu8         = bfr_pv;
        Fee_Prv_MediumRead_st.length_u32      = length_u32;
        Fee_Prv_MediumRead_st.nrBytesDone_u32 = 0u;
        Fee_Prv_MediumRead_st.isBufferHit_b   = TRUE;
    }
    else
    {
        /* For absolute addressing, convert relative to absolute address */
        adrCnv_u32 = adr_u32 + FEE_PRV_CFG_FLS_BASE_ADR;

        Fee_Prv_Medium_st.flsAdr_u32 = adrCnv_u32;

        /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
        Fee_Prv_Medium_st.lastFlsResult_en = Fee_Prv_Lib_Fls_Read(Fee_Prv_Medium_st.flsDevId_en, Fee_Prv_Medium_st.flsAdr_u32, bfr_pv, length_u32);
        Fee_Prv_MediumRead_st.isBufferHit_b = FALSE;
    }

    return;
}

/**
 * \brief   Start a write operation to the flash with the given sequence.
 *          Contains the sole call of Fls_Write in the whole Fee!
 *
 * \param   adr_u32      Address to write to
 * \param   bfr_pcv      Buffer to write from
 * \param   length_u32   Amount of bytes to write
 * \param   sequence_en  Selected write sequence
*/
void Fee_Prv_MediumWrite(uint32 adr_u32, void const * bfr_pcv, uint32 length_u32, Fee_Prv_MediumSeqWrite_ten sequence_en)
{
    Fee_Prv_MediumWrite_ten    stMediumWrite_en;

    /* Check selected write sequence */
    switch(sequence_en)
    {
        /* Note: The first two cases, which rely on the blank check in Fls, could be enclosed by the compiler switch
         * (STD_OFF != FLS_BLANK_CHECK_API) to make the job fail in the first call of Fee_Prv_MediumWriteDo if the
         * functionality is not supported.
         * However, we want to reduce the use of compiler switches as much as possible. Here, this is possible, since
         * the write job will anyway fail in the first call of Fee_Prv_MediumBlankCheckDo (= Fee_Prv_MediumFlsDo) after
         * Fee_Prv_MediumBlankCheck in case the blank check functionality is not supported by Fls. */

        /* Selected sequence is blank check -> copy -> program -> compare ? */
        case FEE_PRV_MEDIUM_SEQ_WRITE_BLNKCHK_COPY_COMP_E:
        {
            /* Start state machine in blank check state without medium buffer bypass */
            stMediumWrite_en = FEE_PRV_MEDIUM_WRITE_BLANKCHECK_E;
            Fee_Prv_MediumWrite_st.isMedBfrByp_b = FALSE;
        }
        break;

        /* Selected sequence is blank check -> program -> compare ? */
        case FEE_PRV_MEDIUM_SEQ_WRITE_BLNKCHK_COMP_E:
        {
            /* Start state machine in blank check state with medium buffer bypass */
            stMediumWrite_en = FEE_PRV_MEDIUM_WRITE_BLANKCHECK_E;
            Fee_Prv_MediumWrite_st.isMedBfrByp_b = TRUE;
        }
        break;

        /* Selected sequence is copy -> program -> compare ? */
        case FEE_PRV_MEDIUM_SEQ_WRITE_COPY_COMP_E:
        {
            /* Start state machine in copy state without medium buffer bypass */
            stMediumWrite_en = FEE_PRV_MEDIUM_WRITE_COPYDATA_E;
            Fee_Prv_MediumWrite_st.isMedBfrByp_b = FALSE;
        }
        break;

        /* Selected sequence is program -> compare ? */
        case FEE_PRV_MEDIUM_SEQ_WRITE_COMP_E:
        {
            /* Start state machine in copy state with medium buffer bypass */
            stMediumWrite_en = FEE_PRV_MEDIUM_WRITE_COPYDATA_E;
            Fee_Prv_MediumWrite_st.isMedBfrByp_b = TRUE;
        }
        break;

        default:
        {
            /* Sequence not (yet) supported; keep state machine idle to cause MEMIF_JOB_FAILED */
            stMediumWrite_en = FEE_PRV_MEDIUM_WRITE_IDLE_E;
        }
        break;
    }

    /* Selected sequence is supported? */
    if(FEE_PRV_MEDIUM_WRITE_IDLE_E != stMediumWrite_en)
    {
        /* Memorize details of request */
        Fee_Prv_MediumWrite_st.adrDest_u32 = adr_u32;
        Fee_Prv_MediumWrite_st.bfr_pcv     = bfr_pcv;
        Fee_Prv_MediumWrite_st.length_u32  = length_u32;
        Fee_Prv_MediumWrite_st.done_u32    = 0u;

        /* Medium buffer shall be by used (i.e. bypass inactive)? */
        if(!Fee_Prv_MediumWrite_st.isMedBfrByp_b)
        {
            /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
            Fee_Prv_MediumWrite_st.data2Program_pcv = &Fee_Prv_MediumBuffer_un.u8[0u];
        }
        else
        {
            /* Fee_Prv_MediumWrite_st.data2Program_pcv is set in state Fee_Prv_MediumWrite_CopyData later */
        }
    }

    /* Reset/restart the state machine */
    FEE_PRV_LIBTRANS(Fee_Prv_MediumWrite_st, stMediumWrite_en);

    return;
}

/**
 * \brief   Erase a part of the flash.
 *          Contains the sole call of Fls_Erase in the whole Fee!
 *
 * \param   adr_u32     Address where the erase shall begin
 * \param   length_u32  Amount of bytes to erase
*/
void Fee_Prv_MediumErase(uint32 adr_u32, uint32 length_u32)
{
    /* For absolute addressing, convert relative to absolute address */
    uint32 adrCnv_u32 = adr_u32 + FEE_PRV_CFG_FLS_BASE_ADR;
    Fee_Prv_Medium_st.flsAdr_u32 = adrCnv_u32;

    Fee_Prv_MediumSetBfrDirty();
    Fee_Prv_Medium_st.lastFlsResult_en = Fee_Prv_Lib_Fls_Erase(Fee_Prv_Medium_st.flsDevId_en, Fee_Prv_Medium_st.flsAdr_u32, length_u32);

    return;
}

/**
 * \brief   Compare the content of a piece of flash with the content of a piece of RAM
 *          Contains the sole call of Fls_Compare in the whole Fee!
 *
 * \param   flsAdr_u32  Address in flash where the compare shall start
 * \param   bfr_pcv     Address in the RAM where the compare shall start
 * \param   length_u32  Amount of bytes to compare
*/
void Fee_Prv_MediumCompareFls2Buf(uint32 adrFls_u32, void const * bfr_pcv, uint32 length_u32)
{
    /* For absolute addressing, convert relative to absolute address */
    uint32 adrCnv_u32 = adrFls_u32 + FEE_PRV_CFG_FLS_BASE_ADR;
    Fee_Prv_Medium_st.flsAdr_u32 = adrCnv_u32;

    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    Fee_Prv_Medium_st.lastFlsResult_en = Fee_Prv_Lib_Fls_Compare(Fee_Prv_Medium_st.flsDevId_en, Fee_Prv_Medium_st.flsAdr_u32, bfr_pcv, length_u32);

    return;
}

/**
 * \brief   Start a compare operation of 2 flash locations.
 *
 * \param   flsAdr1_u32 Address of 1st flash location
 * \param   flsAdr2_u32 Address of 2nd flash location
 * \param   length_u32  Bytes to compare in total
*/
void Fee_Prv_MediumCompareFls2Fls(uint32 adrFls1_u32, uint32 adrFls2_u32, uint32 length_u32)
{
    Fee_Prv_MediumCompareFls2Fls_st.adrFls1_u32       = adrFls1_u32;
    Fee_Prv_MediumCompareFls2Fls_st.adrFls2_u32       = adrFls2_u32;
    Fee_Prv_MediumCompareFls2Fls_st.length_u32        = length_u32;
    Fee_Prv_MediumCompareFls2Fls_st.nrBytesDone_u32   = 0u;

    FEE_PRV_LIBTRANS(Fee_Prv_MediumCompareFls2Fls_st, FEE_PRV_MEDIUM_COMPAREFLS2FLS_READ_E);

    return;
}

/**
 * \brief   Check whether a piece of flash is erased or programmed.
 *          Contains the sole call of Fls_BlankCheck in the whole Fee!
 *
 * \param   adr_u32     Address in flash where the blank check shall start
 * \param   length_u32  Amount of bytes which shall be checked
*/
void Fee_Prv_MediumBlankCheck(uint32 adr_u32, uint32 length_u32)
{
# if (defined(FLS_BLANK_CHECK_API) && (STD_OFF != FLS_BLANK_CHECK_API))

    /* For absolute addressing, convert relative to absolute address */
    uint32 adrCnv_u32 = adr_u32 + FEE_PRV_CFG_FLS_BASE_ADR;

    Fee_Prv_Medium_st.flsAdr_u32 = adrCnv_u32;

    Fee_Prv_Medium_st.lastFlsResult_en = Fee_Prv_Lib_Fls_BlankCheck(Fee_Prv_Medium_st.flsDevId_en, Fee_Prv_Medium_st.flsAdr_u32, length_u32);

# else

    /* Unused parameters */
    (void)adr_u32;
    (void)length_u32;

    /* Memorize result as E_NOT_OK to make the job fail */
    Fee_Prv_Medium_st.lastFlsResult_en = E_NOT_OK;

# endif

    return;
}

/**
 * \brief   Destroy already programmed data by overprogramming with the given value.
 *
 * \param   adr_u32     Address of the data which shall be shredded
 * \param   value_u8    Shredder value (= value used for overprogramming, typically the inverse erase pattern)
 * \param   length_u32  Amount of bytes to shredder (0 is permitted, must fit into the medium buffer, and must be page aligned)
*/
void Fee_Prv_MediumShredder(uint32 adr_u32, uint8 value_u8, uint32 length_u32)
{
    if((length_u32 <= sizeof(Fee_Prv_MediumBuffer_un.u8)) &&
      (Fee_Prv_LibModulo(length_u32, FEE_VIRTUAL_PAGE_SIZE) == 0u))
    {
        rba_MemLib_MemSet(&Fee_Prv_MediumBuffer_un.u8[0], value_u8, length_u32);

        /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
        Fee_Prv_MediumProgram(
            adr_u32,
            &Fee_Prv_MediumBuffer_un.u8[0u],
            length_u32
        );
    }
    else
    {
        Fee_Prv_Medium_st.lastFlsResult_en = E_NOT_OK;
    }

    return;
}

/**
 * \brief   Destroy already programmed data by overprogramming with the given repetitive pattern.
 *
 * \param   adr_u32       Address of the data which shall be shredded
 * \param   patOffs_u32   Offset from where to start in first shredder pattern
 * \param   patSize_u32   Shredder pattern size (in number of bytes, must be greater than patOffs_u32)
 * \param   patBfr_pcv    Pointer to shredder pattern
 * \param   length_u32    Amount of bytes to shredder in total (0 is permitted, must fit into the medium buffer, and must be page aligned)
 *
 * \attention   Refrain from calling this function with a pattern size of one (even though this will work)!
 *              Use Fee_Prv_MediumShredder instead, since the latter is much more efficient for such use-case.
*/
void Fee_Prv_MediumPatShredder(uint32 adr_u32, uint32 patOffs_u32, uint32 patSize_u32, void const * patBfr_pcv, uint32 length_u32)
{
    uint32    nrBytesDone_u32, nrBytesCurrentChunk_u32, nrBytesCurrentOffs_u32;

    if(   (length_u32 <= sizeof(Fee_Prv_MediumBuffer_un.u8))
       && (patSize_u32 > patOffs_u32)
       && (Fee_Prv_LibModulo(length_u32, FEE_VIRTUAL_PAGE_SIZE) == 0u))
    {
        nrBytesDone_u32         = 0uL;
        nrBytesCurrentOffs_u32  = patOffs_u32;
        nrBytesCurrentChunk_u32 = rba_MemLib_Min(patSize_u32 - patOffs_u32, length_u32);

        while(nrBytesCurrentChunk_u32 > 0uL)
        {
            /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
            rba_MemLib_MemCopy(
                    (uint8 const *)patBfr_pcv + nrBytesCurrentOffs_u32,
                    &Fee_Prv_MediumBuffer_un.u8[0] + nrBytesDone_u32,
                    nrBytesCurrentChunk_u32
            );

            nrBytesDone_u32        += nrBytesCurrentChunk_u32;
            nrBytesCurrentOffs_u32  = 0uL;
            nrBytesCurrentChunk_u32 = rba_MemLib_Min(patSize_u32, length_u32 - nrBytesDone_u32);
        };

        /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
        Fee_Prv_MediumProgram(
            adr_u32,
            &Fee_Prv_MediumBuffer_un.u8[0u],
            length_u32
        );
    }
    else
    {
        Fee_Prv_Medium_st.lastFlsResult_en = E_NOT_OK;
    }

    return;
}

/**
 * \brief   Calculate the CRC directly from the flash.
 *
 * \param   adr_u32     Address where the calculation shall start
 * \param   length_u32  Amount of bytes to consider
 * \param   crc_pu32    Point to an uint32 where the result is placed
 * \param   firstCall_b Is this the first call for a chain of CRC calculations or not
*/
void Fee_Prv_MediumCalcCrc(uint32 adr_u32, uint32 length_u32, uint32 * crc_pu32, boolean firstCall_b)
{
    Fee_Prv_MediumCalcCrc_st.nrBytesDone_u32    = 0u;
    Fee_Prv_MediumCalcCrc_st.length_u32         = length_u32;
    Fee_Prv_MediumCalcCrc_st.crc_pu32           = crc_pu32;
    Fee_Prv_MediumCalcCrc_st.firstCall_b        = firstCall_b;

    Fee_Prv_MediumSetIterator(adr_u32, length_u32);

    return;
}

/**
 * \brief   Start a copy operation from one flash location to another (with the given sequence, using internal buffer).
 *
 * \param   adrSrc_u32   Source address in flash
 * \param   adrDest_u32  Target address in flash
 * \param   length_u32   Bytes to copy in total
 * \param   sequence_en  Selected copy sequence
*/
void Fee_Prv_MediumCopy(uint32 adrSrc_u32, uint32 adrDest_u32, uint32 length_u32, Fee_Prv_MediumSeqCopy_ten sequence_en)
{
    Fee_Prv_MediumCopy_ten        stMediumCopy_en;
    Fee_Prv_MediumSeqWrite_ten    seqMediumWrite_en;

    /* Check selected write sequence */
    switch(sequence_en)
    {
        /* Note: The first case, which relies on the blank check in Fls, could be enclosed by the compiler switch
         * (STD_OFF != FLS_BLANK_CHECK_API) to make the job fail in the first call of Fee_Prv_MediumCopyDo if the
         * functionality is not supported.
         * However, we want to reduce the use of compiler switches as much as possible. Here, this is possible, since
         * the copy job will anyway fail in the first call of Fee_Prv_MediumBlankCheckDo (= Fee_Prv_MediumFlsDo) after
         * Fee_Prv_MediumBlankCheck in case the blank check functionality is not supported by Fls. */

        /* Selected sequence is blank check -> copy -> compare ? */
        case FEE_PRV_MEDIUM_SEQ_COPY_BLNKCHK_COMP_E:
        {
            /* Start state machine in read state; the following writing shall be preceded by a blank check */
            stMediumCopy_en   = FEE_PRV_MEDIUM_COPY_READ_E;
            seqMediumWrite_en = FEE_PRV_MEDIUM_SEQ_WRITE_BLNKCHK_COMP_E;
        }
        break;

        /* Selected sequence is copy -> compare ? */
        case FEE_PRV_MEDIUM_SEQ_COPY_COMP_E:
        {
            /* Start state machine in read state; the following writing shall not be preceded by a blank check */
            stMediumCopy_en   = FEE_PRV_MEDIUM_COPY_READ_E;
            seqMediumWrite_en = FEE_PRV_MEDIUM_SEQ_WRITE_COMP_E;
        }
        break;

        default:
        {
            /* Sequence not (yet) supported; keep state machine idle to cause MEMIF_JOB_FAILED */
            stMediumCopy_en = FEE_PRV_MEDIUM_COPY_IDLE_E;
        }
        break;
    }

    /* Selected sequence is supported? */
    if(FEE_PRV_MEDIUM_COPY_IDLE_E != stMediumCopy_en)
    {
        Fee_Prv_MediumCopy_st.adrSrc_u32  = adrSrc_u32;
        Fee_Prv_MediumCopy_st.adrDest_u32 = adrDest_u32;
        Fee_Prv_MediumCopy_st.length_u32  = length_u32;
        Fee_Prv_MediumCopy_st.done_u32    = 0u;
        Fee_Prv_MediumCopy_st.writeSeq_en = seqMediumWrite_en;
    }

    /* Reset/restart the state machine */
    FEE_PRV_LIBTRANS(Fee_Prv_MediumCopy_st, stMediumCopy_en);

    return;
}

/**
 * \brief   Do the work for a read job.
*/
MemIf_JobResultType Fee_Prv_MediumReadDo(void)
{
    MemIf_JobResultType result_en;
    uint32              nrBytesAllowedToProcess_u32;

    if(Fee_Prv_MediumRead_st.isBufferHit_b)
    {
        /* Adapt data size to copy to the available Fee time budget.                                               */
        /* In case of a large data block (max. 64kB) which fits into the Medium buffer, the copy process has to be */
        /* distributed over multiple Fee main calls.                                                               */
        nrBytesAllowedToProcess_u32 = Fee_Prv_LibEffortGet(FEE_PRV_LIMIT_CRCINRAM_CPYRAM_E,
                                                           Fee_Prv_MediumRead_st.length_u32 - Fee_Prv_MediumRead_st.nrBytesDone_u32);

        rba_MemLib_MemCopy(Fee_Prv_MediumRead_st.adr_pcu8 + Fee_Prv_MediumRead_st.nrBytesDone_u32,
                           Fee_Prv_MediumRead_st.bfr_pu8  + Fee_Prv_MediumRead_st.nrBytesDone_u32,
                           nrBytesAllowedToProcess_u32);

        Fee_Prv_MediumRead_st.nrBytesDone_u32 += nrBytesAllowedToProcess_u32;

        if(Fee_Prv_MediumRead_st.length_u32 <= Fee_Prv_MediumRead_st.nrBytesDone_u32)
        {
            result_en = MEMIF_JOB_OK;
        }
        else
        {
            result_en = MEMIF_JOB_PENDING;
        }
    }
    else
    {
        result_en = Fee_Prv_MediumFlsDo();
    }

    return(result_en);
}

/**
 * \brief   Before programming assure that the space which shall be programmed is blank
 *
 * \pre     (BLANKCHECK == state)
 * \post    (MEMIF_JOB_OK      == result) && (PROGRAM       == state) && (TRUE == entry )
 *          (MEMIF_JOB_PENDING == result) && (BLANKCHECK    == state) && (FALSE == entry)
 *          (MEMIF_JOB_FAILED  == result)
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_BlankCheck(Fee_Prv_MediumWrite_tst * fsm_pst)
{
    MemIf_JobResultType result_en;

    if(FEE_PRV_LIBENTRY)
    {
        uint32 nrBytesForBlankcheck_u32;

        // round up the write length to a full virtual page
        // blankchecks are accepted only in multiples of pagesize
        // while the write is accepting aligned smaller pagesize,too
        nrBytesForBlankcheck_u32 = Fee_Prv_LibCeil(fsm_pst->length_u32);

        Fee_Prv_MediumBlankCheck(fsm_pst->adrDest_u32, nrBytesForBlankcheck_u32);
    }

    result_en = Fee_Prv_MediumBlankCheckDo();

    if(MEMIF_JOB_OK == result_en)
    {
        FEE_PRV_LIBSC(FEE_PRV_MEDIUM_WRITE_COPYDATA_E);
    }

    return(result_en);
}

#if(FALSE == FEE_RB_WRITE_PADDING_REQUIRED)
/**
 * \brief   Before programming copy the data to an internal buffer in case the local buffer is not bypassed.
 *          Afterwards start programming using this buffer.
 *
 * \pre     (COPYDATA == state)
 * \post    (MEMIF_JOB_OK      == result) && (PROGRAM       == state) && (TRUE == entry )
 *          (MEMIF_JOB_PENDING == result) && (COPYDATA      == state) && (FALSE == entry)
 *          (MEMIF_JOB_FAILED  == result)
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_CopyDataWithoutPadding(Fee_Prv_MediumWrite_tst * fsm_pst)
{
    uint8 const * startAdr_pcu8;

    /*
     * Need to convert the void const * to a uint8 const * pointer to add the offset of the already done bytes.
     * Pointer arithmetic with void pointers is not meaningful
     */

    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    startAdr_pcu8  = (uint8 const *)fsm_pst->bfr_pcv;
    startAdr_pcu8 += fsm_pst->done_u32;

    if(fsm_pst->isMedBfrByp_b)
    {
        /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
        fsm_pst->data2Program_pcv = (void const * )startAdr_pcu8;
        fsm_pst->bytes2Program_u32 = fsm_pst->length_u32 - fsm_pst->done_u32;
    }
    else
    {
        fsm_pst->bytes2Program_u32 = Fee_Prv_LibMin(sizeof(Fee_Prv_MediumBuffer_un.u8), fsm_pst->length_u32 - fsm_pst->done_u32);

        rba_MemLib_MemCopy(startAdr_pcu8, &Fee_Prv_MediumBuffer_un.u8[0u], fsm_pst->bytes2Program_u32);
    }

    FEE_PRV_LIBSC(FEE_PRV_MEDIUM_WRITE_PROGRAM_E);

    return(MEMIF_JOB_OK);
}

#else
/**
 * \brief   Before programming copy the data to an internal buffer in case the local buffer is not bypassed.
 *          Add padding bytes in case they are needed.
 *          Afterwards start programming using this buffer.
 *
 * \pre     (COPYDATA == state)
 * \post    (MEMIF_JOB_OK      == result) && (PROGRAM       == state) && (TRUE == entry )
 *          (MEMIF_JOB_PENDING == result) && (COPYDATA      == state) && (FALSE == entry)
 *          (MEMIF_JOB_FAILED  == result)
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_CopyDataWithPadding(Fee_Prv_MediumWrite_tst * fsm_pst)
{
    uint8 const * startAdr_pcu8;
    uint8 tempPadding_u8 = 0u;

    /*
     * Need to convert the void const * to a uint8 const * pointer to add the offset of the already done bytes.
     * Pointer arithmetic with void pointers is not meaningful
     */

    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    startAdr_pcu8  = (uint8 const *)fsm_pst->bfr_pcv;
    startAdr_pcu8 += fsm_pst->done_u32;

    if(fsm_pst->isMedBfrByp_b)
    {
        /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
        fsm_pst->data2Program_pcv = (void const * )startAdr_pcu8;
        fsm_pst->bytes2Program_u32 = fsm_pst->length_u32 - fsm_pst->done_u32;

        /* Check if data length is page aligned */
        if(Fee_Prv_LibModulo(fsm_pst->bytes2Program_u32, FEE_VIRTUAL_PAGE_SIZE) != 0u)
        {
            tempPadding_u8 = FEE_VIRTUAL_PAGE_SIZE - Fee_Prv_LibModulo(fsm_pst->bytes2Program_u32, FEE_VIRTUAL_PAGE_SIZE);

            if(fsm_pst->bytes2Program_u32 > FEE_VIRTUAL_PAGE_SIZE)
            {
                /* Round the data to the last complete page */
                fsm_pst->bytes2Program_u32 = fsm_pst->bytes2Program_u32 - (FEE_VIRTUAL_PAGE_SIZE - tempPadding_u8);
            }
            else
            {
                /* Copy the remaining bytes to the local buffer */
                rba_MemLib_MemCopy(startAdr_pcu8, &Fee_Prv_MediumBuffer_un.u8[0u], fsm_pst->bytes2Program_u32);

                /* Write the padding bytes into the local buffer */
                rba_MemLib_MemSet(&Fee_Prv_MediumBuffer_un.u8[fsm_pst->bytes2Program_u32], (uint8)(~FEE_PRV_CFG_ERASE_PATTERN_8BIT), tempPadding_u8);

                /* use the local buffer to write the remaining data + padding */
                Fee_Prv_MediumWrite_st.data2Program_pcv = &Fee_Prv_MediumBuffer_un.u8[0u];

                /* write the remaining bytes + padding */
                fsm_pst->bytes2Program_u32 = FEE_VIRTUAL_PAGE_SIZE;
            }
        }
    }
    else
    {
        fsm_pst->bytes2Program_u32 = Fee_Prv_LibMin(sizeof(Fee_Prv_MediumBuffer_un.u8), fsm_pst->length_u32 - fsm_pst->done_u32);

        rba_MemLib_MemCopy(startAdr_pcu8, &Fee_Prv_MediumBuffer_un.u8[0u], fsm_pst->bytes2Program_u32);

        if(Fee_Prv_LibModulo(fsm_pst->bytes2Program_u32, FEE_VIRTUAL_PAGE_SIZE) != 0u)
        {
            tempPadding_u8 = FEE_VIRTUAL_PAGE_SIZE - Fee_Prv_LibModulo(fsm_pst->bytes2Program_u32, FEE_VIRTUAL_PAGE_SIZE);

            /* Write the padding bytes into the buffer */
            rba_MemLib_MemSet(&Fee_Prv_MediumBuffer_un.u8[fsm_pst->bytes2Program_u32], (uint8)(~FEE_PRV_CFG_ERASE_PATTERN_8BIT), tempPadding_u8);

            /* Updated the number of bytes to be written to be multiple of PAGE_SIZE */
            fsm_pst->bytes2Program_u32 +=  tempPadding_u8;
        }
    }

    FEE_PRV_LIBSC(FEE_PRV_MEDIUM_WRITE_PROGRAM_E);

    return(MEMIF_JOB_OK);
}
#endif

/**
 * \brief   program.
 *
 * \pre     (WRITE == state)
 * \post    (bytes2Program_u32 = min(size(Internal Buffer), length - done))
 * \post    (Internal Buffer == External Buffer)
 * \post    (Dirty(Internal Buffer))
 * \post    (MEMIF_JOB_OK      == result) && (COMPARE == state) && (TRUE == entry )
 *          (MEMIF_JOB_PENDING == result) && (PROGRAM == state) && (FALSE == entry)
 *          (MEMIF_JOB_FAILED  == result)
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_Program(Fee_Prv_MediumWrite_tst * fsm_pst)
{
    MemIf_JobResultType result_en;

    if(FEE_PRV_LIBENTRY)
    {
        Fee_Prv_MediumProgram(fsm_pst->adrDest_u32 + fsm_pst->done_u32, fsm_pst->data2Program_pcv, fsm_pst->bytes2Program_u32);
    }

    result_en = Fee_Prv_MediumProgramDo();

    if(MEMIF_JOB_OK == result_en)
    {
        FEE_PRV_LIBSC(FEE_PRV_MEDIUM_WRITE_COMPARE_E);
    }

    return(result_en);
}

/**
 * \brief   Compare what should have been programmed with what was actually programmed
 *
 * \pre     (COMPARE == state)
 * \post    (MEMIF_JOB_OK      == result) && (PROGRAM   == state) && (TRUE  == entry) && (done = doneOld + bytes2Program) && (done <  length)
 *          (MEMIF_JOB_OK      == result) && (FINAL     == state) && (TRUE  == entry) && (done = doneOld + bytes2Program) && (done >= length)
 *          (MEMIF_JOB_PENDING == result) && (COMPARE   == state) && (FALSE == entry)
 *          (MEMIF_JOB_FAILED  == result)
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumWrite_Compare(Fee_Prv_MediumWrite_tst * fsm_pst)
{
    MemIf_JobResultType result_en;

    if(FEE_PRV_LIBENTRY)
    {
        Fee_Prv_MediumCompareFls2Buf(fsm_pst->adrDest_u32 + fsm_pst->done_u32, fsm_pst->data2Program_pcv, fsm_pst->bytes2Program_u32);
    }

    result_en = Fee_Prv_MediumCompareFls2BufDo();

    if(MEMIF_JOB_OK == result_en)
    {
        fsm_pst->done_u32 += fsm_pst->bytes2Program_u32;
        if(fsm_pst->done_u32 < fsm_pst->length_u32)
        {
            FEE_PRV_LIBSC(FEE_PRV_MEDIUM_WRITE_COPYDATA_E);
        }
        else
        {
            FEE_PRV_LIBSC(FEE_PRV_MEDIUM_WRITE_FINAL_E);
        }
    }
    else if(MEMIF_BLOCK_INCONSISTENT == result_en)
    {
        result_en = MEMIF_JOB_FAILED;
    }
    else
    {
        /* intentionally empty */
    }

    return(result_en);
}

/**
 * \brief   Do the work for a write job.
*/
MemIf_JobResultType Fee_Prv_MediumWriteDo(void)
{
    Fee_Prv_MediumWrite_tst *   fsm_pst;
    MemIf_JobResultType         result_en;

    fsm_pst = &Fee_Prv_MediumWrite_st;

    do
    {
        switch(fsm_pst->state_en)
        {
            case FEE_PRV_MEDIUM_WRITE_BLANKCHECK_E:             result_en = Fee_Prv_MediumWrite_BlankCheck(fsm_pst);            break;
            case FEE_PRV_MEDIUM_WRITE_COPYDATA_E:               result_en = Fee_Prv_MediumWrite_CopyData(fsm_pst);              break;
            case FEE_PRV_MEDIUM_WRITE_PROGRAM_E:                result_en = Fee_Prv_MediumWrite_Program(fsm_pst);               break;
            case FEE_PRV_MEDIUM_WRITE_COMPARE_E:                result_en = Fee_Prv_MediumWrite_Compare(fsm_pst);               break;
            default:                                            result_en = MEMIF_JOB_FAILED;                                   break;
        }
    } while((MEMIF_JOB_OK == result_en) && (FEE_PRV_MEDIUM_WRITE_FINAL_E != fsm_pst->state_en));
    /* Evaluation of while loop: The while loop is required to speed up the state tranisition by one main function call.
     * Since the transition just starts the next step but does not wait till completion, the while loop will not be stuck
     * infinitely. */

    if(MEMIF_JOB_PENDING != result_en)
    {
        FEE_PRV_LIBSC(FEE_PRV_MEDIUM_WRITE_IDLE_E);
    }

    return(result_en);
}

/**
 * \brief   Do the work for an erase job.
*/
MemIf_JobResultType Fee_Prv_MediumEraseDo(void)
{
    return(Fee_Prv_MediumFlsDo());
}

/**
 * \brief   Do the work for a compare job.
*/
MemIf_JobResultType Fee_Prv_MediumCompareFls2BufDo(void)
{
    return(Fee_Prv_MediumFlsDo());
}

/**
 * \brief   Read data from the 1st flash location to the internal buffer.
 * \param   fsm_pst: reference to the statemachine data of the current job
 * \return  return value of the triggered read operation
 *
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumCompareFls2Fls_Read(Fee_Prv_MediumCompareFls2Fls_tst * fsm_pst)
{
    MemIf_JobResultType result_en;

    if(FEE_PRV_LIBENTRY)
    {
        fsm_pst->nrBytesCurrentChunk_u32 = Fee_Prv_LibMin(fsm_pst->length_u32 - fsm_pst->nrBytesDone_u32, sizeof(Fee_Prv_MediumBuffer_un.u8));
        /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
        Fee_Prv_MediumRead(fsm_pst->adrFls1_u32 + fsm_pst->nrBytesDone_u32 , ((void *) &Fee_Prv_MediumBuffer_un.u8[0u]) , fsm_pst->nrBytesCurrentChunk_u32);
        Fee_Prv_MediumSetBfrDirty();
    }

    result_en = Fee_Prv_MediumReadDo();

    if(MEMIF_JOB_OK == result_en)
    {
        FEE_PRV_LIBSC(FEE_PRV_MEDIUM_COMPAREFLS2FLS_COMPARE_E);
    }

    return(result_en);
}

/**
 * \brief   Compare data from the internal buffer to the 2nd flash location
 * \param   fsm_pst: reference to the statemachine data of the current job
 * \return  return value of the triggered compare operation
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumCompareFls2Fls_Compare(Fee_Prv_MediumCompareFls2Fls_tst * fsm_pst)
{
    MemIf_JobResultType result_en;

    if(FEE_PRV_LIBENTRY)
    {
        /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
        Fee_Prv_MediumCompareFls2Buf(fsm_pst->adrFls2_u32 + fsm_pst->nrBytesDone_u32, &Fee_Prv_MediumBuffer_un.u8[0u], fsm_pst->nrBytesCurrentChunk_u32);
    }

    result_en = Fee_Prv_MediumCompareFls2BufDo();

    if(MEMIF_JOB_OK == result_en)
    {
        fsm_pst->nrBytesDone_u32 += fsm_pst->nrBytesCurrentChunk_u32;
        if(fsm_pst->nrBytesDone_u32 < fsm_pst->length_u32)
        {
            FEE_PRV_LIBSC(FEE_PRV_MEDIUM_COMPAREFLS2FLS_READ_E);
        }
        else
        {
            FEE_PRV_LIBSC(FEE_PRV_MEDIUM_COMPAREFLS2FLS_FINAL_E);
        }
    }

    return(result_en);
}

/**
 * \brief   Do the work for the Fls2Fls compare operation.
 * \return job result of the comparison
*/
MemIf_JobResultType Fee_Prv_MediumCompareFls2FlsDo(void)
{
    Fee_Prv_MediumCompareFls2Fls_tst *  fsm_pst;
    MemIf_JobResultType                 result_en;

    fsm_pst = &Fee_Prv_MediumCompareFls2Fls_st;

    do
    {
        switch(fsm_pst->state_en)
        {
            case FEE_PRV_MEDIUM_COMPAREFLS2FLS_READ_E:      result_en = Fee_Prv_MediumCompareFls2Fls_Read(fsm_pst);     break;
            case FEE_PRV_MEDIUM_COMPAREFLS2FLS_COMPARE_E:   result_en = Fee_Prv_MediumCompareFls2Fls_Compare(fsm_pst);  break;
            default:                                        result_en = MEMIF_JOB_FAILED;                               break;
        }
    } while((MEMIF_JOB_OK == result_en) && (FEE_PRV_MEDIUM_COMPAREFLS2FLS_FINAL_E != fsm_pst->state_en));
    /* Evaluation of while loop: The while loop is required to speed up the state tranisition by one main function call.
     * Since the transition just starts the next step but does not wait till completion, the while loop will not be stuck
     * infinitely. */

    if(MEMIF_JOB_PENDING != result_en)
    {
        FEE_PRV_LIBSC(FEE_PRV_MEDIUM_COMPAREFLS2FLS_IDLE_E);
    }

    return(result_en);
}

/**
 * \brief   Do the work for a blank check job.
*/
MemIf_JobResultType Fee_Prv_MediumBlankCheckDo(void)
{
    MemIf_JobResultType result_en;

    result_en = Fee_Prv_MediumFlsDo();

    if(MEMIF_BLOCK_INCONSISTENT == result_en)
    {
        /*
         * Map MEMIF_BLOCK_INCONSISTENT (Blank Check job has been executed and the result is not blank) to
         * MEMIF_BLOCK_FAILED (Job cannot be executed) -> The file system handles both cases in the same way.
         */
        result_en = MEMIF_JOB_FAILED;
    }

    return(result_en);
}

/**
 * \brief   Do the work for a shredder job.
*/
MemIf_JobResultType Fee_Prv_MediumShredderDo(void)
{
    MemIf_JobResultType result_en;

    result_en = Fee_Prv_MediumProgramDo();

    if(MEMIF_JOB_FAILED == result_en)
    {
        /*
         * IFX flash driver returns MEMIF_JOB_OK on overprogramming already programmed pages.
         * JDP/ST flash driver returns MEMIF_JOB_FAILED on overprogramming.
         * In order to equalize the behaviour a FAILED job is interpreted as OK job for overprogramming.
         */
        result_en = MEMIF_JOB_OK;
    }

    return(result_en);
}

/**
 * \brief   The worker function for the CRC calculation.
 *
 * \return                      Job status
 * \retval  MEMIF_JOB_PENDING:  Job pending
 * \retval  MEMIF_JOB_OK:       CRC calculation successful
 * \retval  MEMIF_JOB_FAILED:   CRC calculation not successful (currently not possible)
*/
MemIf_JobResultType Fee_Prv_MediumCalcCrcDo(void)
{
    void const *        ptr_pcv;
    MemIf_JobResultType result_en                   = MEMIF_JOB_PENDING;
    uint32              nrBytesToConsider_u32;
    uint32              nrBytesAllowedToProcess_u32;

    nrBytesToConsider_u32 = Fee_Prv_LibMin(sizeof(Fee_Prv_MediumBuffer_un.u8),
                                           Fee_Prv_MediumCalcCrc_st.length_u32 - Fee_Prv_MediumCalcCrc_st.nrBytesDone_u32);

    /* Adapt data size for CRC calculation to the available Fee time budget.                                      */
    /* In case of a large data block (max. 64kB) which fits into the Medium buffer, the CRC calculation has to be */
    /* distributed over multiple Fee main calls.                                                                  */
    nrBytesAllowedToProcess_u32 = Fee_Prv_LibEffortGet(FEE_PRV_LIMIT_CRCINRAM_CPYRAM_E, nrBytesToConsider_u32);

    if(nrBytesAllowedToProcess_u32 != nrBytesToConsider_u32)
    {
        /* Align to the next lower page border */
        nrBytesAllowedToProcess_u32 = Fee_Prv_LibFloor(nrBytesAllowedToProcess_u32);
    }

    if(0u < nrBytesAllowedToProcess_u32)
    {
        /* Load all data but process only number of data the time budget allows */
        if(MEMIF_JOB_OK == Fee_Prv_MediumNext(nrBytesAllowedToProcess_u32, &ptr_pcv))
        {
            /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
            *Fee_Prv_MediumCalcCrc_st.crc_pu32 = Crc_CalculateCRC32(ptr_pcv,
                                                                    nrBytesAllowedToProcess_u32,
                                                                    *Fee_Prv_MediumCalcCrc_st.crc_pu32,
                                                                    Fee_Prv_MediumCalcCrc_st.firstCall_b);

            Fee_Prv_MediumCalcCrc_st.nrBytesDone_u32 += nrBytesAllowedToProcess_u32;
            Fee_Prv_MediumCalcCrc_st.firstCall_b      = FALSE;
        }
    }

    /* Length should never be smaller, but to be safe consider it */
    if(Fee_Prv_MediumCalcCrc_st.length_u32 <= Fee_Prv_MediumCalcCrc_st.nrBytesDone_u32)
    {
        result_en = MEMIF_JOB_OK;
    }

    return(result_en);
}

/**
 * \brief   Read data from the flash to the internal buffer.
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumCopy_Read(Fee_Prv_MediumCopy_tst * fsm_pst)
{
    MemIf_JobResultType result_en;

    if(FEE_PRV_LIBENTRY)
    {
        fsm_pst->bytes2Copy_u32 = Fee_Prv_LibMin(fsm_pst->length_u32 - fsm_pst->done_u32, sizeof(Fee_Prv_MediumBuffer_un.u8));
        /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
        Fee_Prv_MediumRead(fsm_pst->adrSrc_u32 + fsm_pst->done_u32, &Fee_Prv_MediumBuffer_un.u8[0u], fsm_pst->bytes2Copy_u32);
        Fee_Prv_MediumSetBfrDirty();
    }

    result_en = Fee_Prv_MediumReadDo();

    if(MEMIF_JOB_OK == result_en)
    {
        FEE_PRV_LIBSC(FEE_PRV_MEDIUM_COPY_WRITE_E);
    }

    return(result_en);
}

/**
 * \brief   Write data from the internal buffer to the flash.
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumCopy_Write(Fee_Prv_MediumCopy_tst * fsm_pst)
{
    MemIf_JobResultType result_en;

    if(FEE_PRV_LIBENTRY)
    {
        /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
        Fee_Prv_MediumWrite(fsm_pst->adrDest_u32 + fsm_pst->done_u32, &Fee_Prv_MediumBuffer_un.u8[0u], fsm_pst->bytes2Copy_u32, fsm_pst->writeSeq_en);
    }

    result_en = Fee_Prv_MediumWriteDo();

    if(MEMIF_JOB_OK == result_en)
    {
        fsm_pst->done_u32 += fsm_pst->bytes2Copy_u32;
        if(fsm_pst->done_u32 < fsm_pst->length_u32)
        {
            FEE_PRV_LIBSC(FEE_PRV_MEDIUM_COPY_READ_E);
        }
        else
        {
            FEE_PRV_LIBSC(FEE_PRV_MEDIUM_COPY_FINAL_E);
        }
    }

    return(result_en);
}

/**
 * \brief   Do the work for a copy operation.
*/
MemIf_JobResultType Fee_Prv_MediumCopyDo(void)
{
    Fee_Prv_MediumCopy_tst *   fsm_pst;
    MemIf_JobResultType        result_en;

    fsm_pst = &Fee_Prv_MediumCopy_st;

    do
    {
        switch(fsm_pst->state_en)
        {
            case FEE_PRV_MEDIUM_COPY_READ_E:    result_en = Fee_Prv_MediumCopy_Read(fsm_pst);    break;
            case FEE_PRV_MEDIUM_COPY_WRITE_E:   result_en = Fee_Prv_MediumCopy_Write(fsm_pst);   break;
            default:                            result_en = MEMIF_JOB_FAILED;                    break;
        }
    } while((MEMIF_JOB_OK == result_en) && (FEE_PRV_MEDIUM_COPY_FINAL_E != fsm_pst->state_en));
    /* Evaluation of while loop: The while loop is required to speed up the state tranisition by one main function call.
     * Since the transition just starts the next step but does not wait till completion, the while loop will not be stuck
     * infinitely. */

    if(MEMIF_JOB_PENDING != result_en)
    {
        FEE_PRV_LIBSC(FEE_PRV_MEDIUM_COPY_IDLE_E);
    }

    return(result_en);
}

/**
 * \brief   Set the iterator to a location in the flash.
 *          From there on the iterator can be used to step forwards or backwards through the flash.
 *          Additionally the maximum number of data to be prefetched will be set.
 *
 * \param   adr_u32            Address in flash where the iterator shall point to
 * \param   maxPrefetchLen_u32 Maximum amount of data to load including prefetch
*/
void Fee_Prv_MediumSetIterator(uint32 adr_u32, uint32 maxPrefetchLen_u32)
{
    Fee_Prv_Medium_st.iterator_u32 = adr_u32;
    Fee_Prv_Medium_st.maxPrefetchLen_u32 = maxPrefetchLen_u32;

    return;
}

/**
 * \brief   Get the value of the flash iterator.
 * \seealso Fee_Prv_MediumSetIterator
*/
uint32 Fee_Prv_MediumGetIterator(void)
{
    return(Fee_Prv_Medium_st.iterator_u32);
}

# if (defined(FLS_RB_ROBUST_PROG_STATUS_API) && (STD_OFF != FLS_RB_ROBUST_PROG_STATUS_API))

/**
 * \brief   Get the size of a single wordline.
 *
 * \return  Size of a wordline in the flash in bytes
*/
uint32 Fee_Prv_MediumGetWlSize(void)
{
    return(RBA_FLS_WORDLINE_SIZE);
}
/**
 * \brief   Find out whether the flash lost data on the same word line during last programming action.
 *
 * \return  Word line error occurred YES/NO
*/
boolean Fee_Prv_MediumHasWlError(void)
{
    uint32              dummyErrorAdr_u32;
    uint8           *   dummyBfr_pu8;
    uint32              dummyLength_u32;
    Std_ReturnType      status_en;
    boolean             result_b;

    status_en = Fee_Prv_Lib_Fls_Rb_GetRobustProgStatus(Fee_Prv_Medium_st.flsDevId_en, &dummyErrorAdr_u32, &dummyBfr_pu8, &dummyLength_u32);

    result_b = FALSE;
    if(E_NOT_OK == status_en)
    {
        result_b = TRUE;
    }

    return(result_b);
}

/**
 *  \brief  Get the address of a damaged word line.
 *
 * \return  Address of the damaged word line that was damaged with the last programming action
*/
uint32 Fee_Prv_MediumGetWlErrorAdr(void)
{
    uint32      errorAdr_u32;
    uint8   *   dummyBfr_pu8;
    uint32      dummyLength_u32;

    (void)Fee_Prv_Lib_Fls_Rb_GetRobustProgStatus(Fee_Prv_Medium_st.flsDevId_en, &errorAdr_u32, &dummyBfr_pu8, &dummyLength_u32);

    /* For relative addressing, convert absolute to relative address */
    return(errorAdr_u32 - FEE_PRV_CFG_FLS_BASE_ADR);
}

/**
 *  \brief  Get the amount of data that was rescued before a word line was damaged.
 *
 * \return  Amount of rescued data
*/
uint32 Fee_Prv_MediumGetWlErrorLength(void)
{
    uint32      dummyErrorAdr_u32;
    uint8   *   dummyBfr_pu8;
    uint32      length_u32;

    (void)Fee_Prv_Lib_Fls_Rb_GetRobustProgStatus(Fee_Prv_Medium_st.flsDevId_en, &dummyErrorAdr_u32, &dummyBfr_pu8, &length_u32);

    return(length_u32);
}

/**
 * \brief   Safe the data of the defect wordline from the flash buffer to the Fee buffer.
 *          This is required since the data of the wordline can be thrown away with every flash job.
*/
void Fee_Prv_MediumRescueWlData(void)
{
    uint32              dummyErrorAdr_u32;
    uint8           *   flsBfr_pu8;
    uint32              length_u32;
    Std_ReturnType      status_en;

    status_en = Fee_Prv_Lib_Fls_Rb_GetRobustProgStatus(Fee_Prv_Medium_st.flsDevId_en, &dummyErrorAdr_u32, &flsBfr_pu8, &length_u32);

    if((E_NOT_OK == status_en) && (NULL_PTR != flsBfr_pu8))
    {
        rba_MemLib_MemCopy(flsBfr_pu8, &Fee_Prv_MediumRescueBfr_au8[0u], length_u32);
    }

    return;
}

/**
 * \brief   Get a pointer to the data that was previously rescued with Fee_Prv_MediumRescueWlData().
 *
 * \return  Pointer to previously rescued data
*/
uint8 const * Fee_Prv_MediumGetWlData(void)
{
    return((uint8 const *)&Fee_Prv_MediumRescueBfr_au8[0u]);
}
# endif

/*
 * Those functions are activated if direct read from the Fls is not active.
 * Currently this is not fully supported in the whole Fee so that part must currently always stay OFF.
*/

/**
 * \brief   Set a pointer to the current iterator then increase the iterator for specified length.
 *
 * \param   length_u32 Amount of bytes to step after pointer is set to current iterator
 * \param   ptr_ppcv   Pointer to the pointer which shall be set to the address of the iterator
 *
 * \retval  MEMIF_JOB_PENDING   Job still pending
 * \retval  MEMIF_JOB_OK        Job finished successfully
 * \retval  MEMIF_JOB_FAILED    Job finished with errors
*/
MemIf_JobResultType Fee_Prv_MediumNext(uint32 length_u32, void const * * ptr_ppcv)
{
    MemIf_JobResultType result_en = Fee_Prv_MediumLoad(Fee_Prv_Medium_st.iterator_u32, length_u32, Fee_Prv_Medium_st.maxPrefetchLen_u32, FEE_PRV_POSTLOAD_E);

    if(MEMIF_JOB_OK == result_en)
    {
        uint32 index_u32                = Fee_Prv_Medium_st.iterator_u32 - Fee_Prv_Medium_st.limitLo_u32;
        /* MR12 DIR 1.1, RULE 18.6 VIOLATION: uint8 * can always be converted safely to void *, returns the pointer to a global buffer at a specific offset */
        *ptr_ppcv                      = &Fee_Prv_MediumBuffer_un.u8[index_u32];
        Fee_Prv_Medium_st.iterator_u32  += length_u32;
    }

    return(result_en);
}

/**
 * \brief   Set a pointer to the current iterator then decrease the iterator for specified length.
 *
 * \param   length_u32 Amount of bytes to step after pointer is set to current iterator
 * \param   ptr_ppcv   Pointer to the pointer which shall be set to the address of the iterator
 *
 * \retval  MEMIF_JOB_PENDING   Job still pending
 * \retval  MEMIF_JOB_OK        Job finished successfully
 * \retval  MEMIF_JOB_FAILED    Job finished with errors
*/
MemIf_JobResultType Fee_Prv_MediumPre(uint32 length_u32, void const * * ptr_ppcv)
{
    MemIf_JobResultType result_en = Fee_Prv_MediumLoad(Fee_Prv_Medium_st.iterator_u32, length_u32, Fee_Prv_Medium_st.maxPrefetchLen_u32, FEE_PRV_PRELOAD_E);

    if(MEMIF_JOB_OK == result_en)
    {
        uint32 index_u32                = Fee_Prv_Medium_st.iterator_u32 - Fee_Prv_Medium_st.limitLo_u32;
        /* MR12 DIR 1.1, RULE 18.6 VIOLATION: uint8 * can always be converted safely to void *, returns the pointer to a global buffer at a specific offset */
        *ptr_ppcv                      = &Fee_Prv_MediumBuffer_un.u8[index_u32];
        Fee_Prv_Medium_st.iterator_u32  -= length_u32;
    }

    return(result_en);
}

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
#endif
