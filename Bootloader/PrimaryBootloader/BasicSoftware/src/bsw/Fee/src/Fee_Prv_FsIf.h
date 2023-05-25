

#ifndef FEE_PRV_FSIF_H
#define FEE_PRV_FSIF_H

/* The FsIf (Filesystem-Interace) unit is the connection from the main function to the actual file system.
 * Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
# if (defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
#include "rba_FeeFs1x.h"
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
#include "rba_FeeFs2.h"
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
#include "rba_FeeFs3.h"
#  endif


/*
 **********************************************************************************************************************
 * Inline Declarations
 **********************************************************************************************************************
*/
LOCAL_INLINE Std_ReturnType    Fee_Prv_FsIfInit (void);

LOCAL_INLINE void    Fee_Prv_FsIfSetMode (MemIf_ModeType mode_en);

LOCAL_INLINE void    Fee_Prv_FsIfWrite               (Fee_Prv_JobDesc_tst const * order_pcst);
LOCAL_INLINE void    Fee_Prv_FsIfRead                (Fee_Prv_JobDesc_tst const * order_pcst);
LOCAL_INLINE void    Fee_Prv_FsIfInvalidateBlock     (Fee_Prv_JobDesc_tst const * order_pcst);
LOCAL_INLINE void    Fee_Prv_FsIfRbMaintainBlock     (Fee_Prv_JobDesc_tst const * order_pcst);
LOCAL_INLINE void    Fee_Prv_FsIfEraseImmediateBlock (Fee_Prv_JobDesc_tst const * order_pcst);

LOCAL_INLINE void    Fee_Prv_FsIfCancel (void);

LOCAL_INLINE void    Fee_Prv_FsIfRbTriggerReorg (void);

LOCAL_INLINE MemIf_JobResultType    Fee_Prv_FsIfWriteDo               (void);
LOCAL_INLINE MemIf_JobResultType    Fee_Prv_FsIfReadDo                (void);
LOCAL_INLINE MemIf_JobResultType    Fee_Prv_FsIfInvalidateBlockDo     (void);
LOCAL_INLINE MemIf_JobResultType    Fee_Prv_FsIfRbMaintainBlockDo     (void);
LOCAL_INLINE MemIf_JobResultType    Fee_Prv_FsIfEraseImmediateBlockDo (void);
LOCAL_INLINE MemIf_JobResultType    Fee_Prv_FsIfBackground            (void);
LOCAL_INLINE MemIf_JobResultType    Fee_Prv_FsIfRbTriggerReorgDo      (void);

LOCAL_INLINE uint32    Fee_Prv_FsIfRbGetSectorChgCntr (void);

LOCAL_INLINE uint32    Fee_Prv_FsIfRbGetFreeSpace (void);

LOCAL_INLINE void    Fee_Prv_FsIfRbGetNrFreeBytesAndFatEntries (
        uint32 * nrFreeBytes_pu32, uint32 * nrFreeFatEntries_pu32, boolean isEraseAccepted_b);

LOCAL_INLINE boolean    Fee_Prv_FsIfRbIsSoftReorgOngoing   (void);
LOCAL_INLINE boolean    Fee_Prv_FsIfRbIsHardReorgOngoing   (void);
LOCAL_INLINE boolean    Fee_Prv_FsIfRbIsSectorEraseOngoing (void);

LOCAL_INLINE MemIf_Rb_MigrationResult_ten    Fee_Prv_FsIfRbGetMigrationResult (void);


/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
*/

/**
 * \brief   Initialize a file system.
*/
LOCAL_INLINE Std_ReturnType Fee_Prv_FsIfInit(void)
{
    Std_ReturnType stRetVal_en;
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    stRetVal_en = rba_FeeFs1x_Init();
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    stRetVal_en = rba_FeeFs2_Init();
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    stRetVal_en = rba_FeeFs3_Ini();
#  else
    stRetVal_en = E_NOT_OK;
#  endif

    return(stRetVal_en);
}

/**
 * \brief   Set the execution mode.
 *
 * \param   mode_en:  fast or slow mode
*/
LOCAL_INLINE void Fee_Prv_FsIfSetMode(MemIf_ModeType mode_en)
{
    /* In Fs1x and Fs3, the execution mode is retrieved via Fee_Prv_ProcGetDriverMode.
     * In Fs2, the feature is not (yet) supported. */
    (void)mode_en;

    return;
}

/**
 * \brief   Let the file system start a write job.
 *
 * \param   order_pcst  Order which contains all information about the write job.
*/
LOCAL_INLINE void Fee_Prv_FsIfWrite(Fee_Prv_JobDesc_tst const * order_pcst)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    rba_FeeFs1x_Write(order_pcst);
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    rba_FeeFs2_Write(order_pcst);
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    rba_FeeFs3_Write(order_pcst);
#  endif

    return;
}

/**
 * \brief   Let the file system start a read job.
 *
 * \param   order_pcst  Order which contains all information about the read job.
*/
LOCAL_INLINE void Fee_Prv_FsIfRead(Fee_Prv_JobDesc_tst const * order_pcst)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    rba_FeeFs1x_Read(order_pcst);
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    rba_FeeFs2_Read(order_pcst);
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    rba_FeeFs3_Read(order_pcst);
#  endif

    return;
}

/**
 * \brief   Let the file system start an invalidate job.
 *
 * \param   order_pcst  Order which contains all information about the invalidate job.
*/
LOCAL_INLINE void Fee_Prv_FsIfInvalidateBlock(Fee_Prv_JobDesc_tst const * order_pcst)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    rba_FeeFs1x_Invalidate(order_pcst);
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    rba_FeeFs2_InvalidateBlock(order_pcst);
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    rba_FeeFs3_Invalidate(order_pcst);
#  endif

    return;
}

/**
 * \brief Let the file system start a maintain job.
 *
 * \param   order_pcst  Order which contains all information about the maintain job.
*/
LOCAL_INLINE void Fee_Prv_FsIfRbMaintainBlock(Fee_Prv_JobDesc_tst const * order_pcst)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    rba_FeeFs1x_Rb_Maintain(order_pcst);
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    rba_FeeFs2_BlockMaintenance(order_pcst);
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    rba_FeeFs3_Maintain(order_pcst);
#  endif

    return;
}

/**
 * \brief   Let the file system start an erase immediate job.
 *
 * \param   order_pcst  Order which contains all information about the erase immediate job.
 *
 * \attention   Currently no file system supports erase immediate.
*/
LOCAL_INLINE void Fee_Prv_FsIfEraseImmediateBlock(Fee_Prv_JobDesc_tst const * order_pcst)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    // not a valid Fs1x job
    (void) *order_pcst;
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    rba_FeeFs2_EraseImmediateBlock(order_pcst);
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    rba_FeeFs3_Erase(order_pcst);
#  endif

    return;
}

/**
 * \brief   Let the file system cancel a job.
 *          Can also be called when a background maintenance action shall be called.
 *          This is for example used to abort a running soft reorganization.
 *
*/
LOCAL_INLINE void Fee_Prv_FsIfCancel(void)
{
#  if(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    rba_FeeFs2_Cancel();
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    rba_FeeFs3_Cancel();
#  endif

    return;
}

/**
 * \brief   Let the file system start a triggered reorg job.
 *
 * \param   order_pcst  Order which contains all information about the triggered reorg job.
*/
LOCAL_INLINE void Fee_Prv_FsIfRbTriggerReorg(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    rba_FeeFs1x_TriggerReorg();
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    /* Feature currently not supported */
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    /* Feature currently not supported */
#  endif

    return;
}

/**
 * \brief   Do the work of a write job.
 *
 * \retval  MEMIF_JOB_PENDING   Write is not yet finished
 * \retval  MEMIF_JOB_OK        Write finished successfully
 * \retval  MEMIF_JOB_FAILED    Write failed
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_FsIfWriteDo(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_BlockJobDo());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_WriteDo());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(rba_FeeFs3_WriteDo());
#  else
    return(MEMIF_JOB_FAILED);
#  endif
}

/**
 * \brief   Do the work of a read job.
 *
 * \retval  MEMIF_JOB_PENDING           Read is not yet finished
 * \retval  MEMIF_JOB_OK                Read finished successfully
 * \retval  MEMIF_BLOCK_INCONSISTENT    Block was not found or the data was not consistent
 * \retval  MEMIF_BLOCK_INVALID         Block was previously invalidated with an invalidate order
 * \retval  MEMIF_JOB_FAILED            Read failed
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_FsIfReadDo(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_BlockJobDo());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_ReadDo());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(rba_FeeFs3_ReadDo());
#  else
    return(MEMIF_JOB_FAILED);
#  endif
}

/**
 * \brief   Do the work of an invalidate job.
 *
 * \retval  MEMIF_JOB_PENDING   Invalidate is not yet finished
 * \retval  MEMIF_JOB_OK        Invalidate finished successfully
 * \retval  MEMIF_JOB_FAILED    Invalidate failed
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_FsIfInvalidateBlockDo(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_BlockJobDo());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_InvalidateBlockDo());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(rba_FeeFs3_InvalidateDo());
#  else
    return(MEMIF_JOB_FAILED);
#  endif
}

/**
 * \brief   Do the work of an Maintain job.
 *
 * \retval  MEMIF_JOB_PENDING   Maintain is not yet finished
 * \retval  MEMIF_JOB_OK        Maintain finished successfully
 * \retval  MEMIF_JOB_FAILED    Maintain failed
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_FsIfRbMaintainBlockDo(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_BlockJobDo());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_BlockMaintenanceDo());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(rba_FeeFs3_MaintainDo());
#  else
    return(MEMIF_JOB_FAILED);
#  endif
}

/**
 * \brief   Do the work of an erase immediate job.
 *
 * \retval  MEMIF_JOB_PENDING   Erase immediate is not yet finished
 * \retval  MEMIF_JOB_OK        Erase immediate finished successfully
 * \retval  MEMIF_JOB_FAILED    Erase immediate failed
 *
 * \attention   Currently no file system supports erase immediate.
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_FsIfEraseImmediateBlockDo(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(MEMIF_JOB_FAILED);
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_EraseImmediateBlockDo());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(rba_FeeFs3_EraseDo());
#  else
    return(MEMIF_JOB_FAILED);
#  endif
}

/**
 * \brief   Let the file system perform some background actions.
 *          This can for example mean erasing a sector in the background.
 *
 * \retval  MEMIF_JOB_PENDING   A background operation is currently running
 * \retval  MEMIF_JOB_OK        No background operation is currently running
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_FsIfBackground(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_Background());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_Background());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(rba_FeeFs3_Background());
#  else
    return(MEMIF_JOB_FAILED);
#  endif
}

/**
 * \brief   Do the work of a triggered reorg job.
 *
 * \retval  MEMIF_JOB_PENDING   Triggered reorg is not yet finished
 * \retval  MEMIF_JOB_OK        Triggered reorg is finished successfully
 * \retval  MEMIF_JOB_FAILED    Triggered reorg failed
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_FsIfRbTriggerReorgDo(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_TriggerReorgDo());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(MEMIF_JOB_FAILED);
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(MEMIF_JOB_FAILED);
#  else
    return(MEMIF_JOB_FAILED);
#  endif
}

/**
 * \brief   Function to return sector change counter
 *
 * \retval  sector change counter
*/
LOCAL_INLINE uint32 Fee_Prv_FsIfRbGetSectorChgCntr(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_GetSectorChgCntr());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_GetSectorChgCntr());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    /* not supported */
    return(0u);
#  else
    return(0u);
#  endif
}

/**
 * \brief   Get the amount of free space in DFLASH
 *
 * \return  Amount of free space in bytes
*/
LOCAL_INLINE uint32 Fee_Prv_FsIfRbGetFreeSpace(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_GetFreeSpace());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(0u);
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(rba_FeeFs3_GetFreeSpace());
#  else
    return(0u);
#  endif
}

/**
 * \brief   Get the amount of free user data space and number of free control data space (FAT entries) in DFLASH before a hard reorganization will be started.
 *          If erase operations are not accepted by the user, free non erased sectors will not be considered for user data space evaluation.
 *
 * \param   nrFreeBytes_pu32       The pointer to the address, where the number of free data bytes information shall be written.
 * \param   nrFreeFatEntries_pu32  The pointer to the address, where the number of free FAT entries information shall be written.
 * \param   isEraseAccepted_b      Information how to handle free non erased sectors.
 *                                 TRUE:  Free non erased sectors shall be considered, erase operations may occur when writing the given number of bytes
 *                                 FALSE: Free non erased sectors shall not be considered, no erase operations when writing the given number of bytes
 */
LOCAL_INLINE void Fee_Prv_FsIfRbGetNrFreeBytesAndFatEntries(uint32 * nrFreeBytes_pu32, uint32 * nrFreeFatEntries_pu32, boolean isEraseAccepted_b)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    *nrFreeBytes_pu32      = 0u;
    *nrFreeFatEntries_pu32 = 0u;
    (void)isEraseAccepted_b;
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    rba_FeeFs2_GetNrFreeBytesAndFatEntries(nrFreeBytes_pu32, nrFreeFatEntries_pu32, isEraseAccepted_b);
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    *nrFreeBytes_pu32      = 0u;
    *nrFreeFatEntries_pu32 = 0u;
    (void)isEraseAccepted_b;
#  else
    *nrFreeBytes_pu32      = 0u;
    *nrFreeFatEntries_pu32 = 0u;
    (void)isEraseAccepted_b;
#  endif
}

/**
 * \brief   Get the status of soft reorg (only when in driverstate background!)
 *
 * \return  Status of soft reorg
*/
LOCAL_INLINE boolean Fee_Prv_FsIfRbIsSoftReorgOngoing(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_IsSoftReorgOngoing());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_IsReorgOngoing());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(rba_FeeFs3_IsSoftDefragOngoing());
#  else
    return(FALSE);
#  endif
}

/**
 * \brief   Get the status of hard reorg
 *
 * \return  Status of hard reorg
*/
LOCAL_INLINE boolean Fee_Prv_FsIfRbIsHardReorgOngoing(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_IsHardReorgOngoing());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_IsReorgOngoing());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(rba_FeeFs3_IsHardDefragOngoing());
#  else
    return(FALSE);
#  endif
}

/**
 * \brief   Get the status of sector erase
 *
 * \return  Status of sector erase
*/
LOCAL_INLINE boolean Fee_Prv_FsIfRbIsSectorEraseOngoing(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(rba_FeeFs1x_IsSectorEraseOngoing());
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_IsSectorEraseOngoing());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(FALSE);
#  else
    return(FALSE);
#  endif
}

/**
 * \brief   Get the migration result of the latest read operation
 *
 * \return  Migration result
*/
LOCAL_INLINE MemIf_Rb_MigrationResult_ten Fee_Prv_FsIfRbGetMigrationResult(void)
{
#  if(defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
    return(MEMIF_RB_MIGRATION_RESULT_INIT_E);
#  elif(defined(FEE_CFG_FEE2_ENABLED) && (TRUE == FEE_CFG_FEE2_ENABLED))
    return(rba_FeeFs2_GetMigrationResult());
#  elif(defined(FEE_CFG_FEE3_ENABLED) && (TRUE == FEE_CFG_FEE3_ENABLED))
    return(MEMIF_RB_MIGRATION_RESULT_INIT_E);
#  else
    return(MEMIF_RB_MIGRATION_RESULT_NOT_DONE_E);
#  endif
}

# endif

/* FEE_PRV_FSIF_H */
#endif
