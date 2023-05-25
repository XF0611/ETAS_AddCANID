

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */

#include "Fee_Idx_Cfg.h"

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
#if(defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

#include "Fee_Prv.h"
#include "Fee_Cfg_SchM.h"

# if (STD_ON == FEE_DEV_ERROR_DETECT)
#include "Det.h"
# endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* Protection selectable via Fee_Cfg_SchM.h to save code */
# ifndef FEE_RB_USE_PROTECTION
#define FEE_RB_USE_PROTECTION   (TRUE)
# endif


/* Temporary work around till the feature is made configurable */
# if (defined(FEE_CFG_FEE1X_ENABLED) && (TRUE == FEE_CFG_FEE1X_ENABLED))
/* For Fee1x, there is no delay in background task execution (same as Fee1.0) */
#define FEE_PRV_PROC_MFCALLS_BEFORE_BACKGROUND_ACTION (0u)
# else
/* Debounce active only for Fee2 and Fee3 */
#define FEE_PRV_PROC_MFCALLS_BEFORE_BACKGROUND_ACTION (100u)
# endif

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

#define FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"
static Fee_Prv_Proc_tst            Fee_Prv_Proc_st;             /* Common variables of the proc unit                            */
static Fee_Prv_ProcLock_tst        Fee_Prv_ProcLock_st;         /* Variables related to reentrancy detection                    */
static Fee_Rb_WorkingStateType_ten Fee_Prv_ProcWorkingState_en; /* Fee activity which is currently ongoing (only for debugging) */
#define FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
*/

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"

LOCAL_INLINE void Fee_Prv_ProcMainFunctionIntern(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_Prv_Proc_tst * proc_pst);
LOCAL_INLINE void Fee_Prv_ProcStartExtJob(Fee_Prv_JobDesc_tst const * newExtJob_pcst);
LOCAL_INLINE void Fee_Prv_ProcProcessIdle(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_Prv_Proc_tst * proc_pst);
LOCAL_INLINE void Fee_Prv_ProcProcessBackground(Fee_Prv_Proc_tst * proc_pst);
LOCAL_INLINE void Fee_Prv_ProcProcessExtJob(Fee_Prv_Proc_tst * proc_pst);
LOCAL_INLINE Fee_Rb_WorkingStateType_ten Fee_Prv_ProcCalcWorkingState(Fee_Prv_ProcDriverState_ten driverState_en,
                                                                      boolean stopModeActive_b,
                                                                      Fee_Prv_JobDesc_tst const * currentExtJob_pcst);
LOCAL_INLINE boolean Fee_Prv_ProcTryToGetLock(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_Prv_ProcLock_tst * procLock_pst);
LOCAL_INLINE void Fee_Prv_ProcReleaseLock(Fee_Prv_ProcLock_tst * procLock_pst);
LOCAL_INLINE MemIf_JobResultType Fee_Prv_ProcEnterStopModeDo(Fee_Prv_Proc_tst * proc_pst);

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
 */

/*
 * \brief   Function called during Fee_Init(). Initializes all the variables of this unit.
 */
void Fee_Prv_ProcInit(void)
{
    Fee_Prv_Proc_st.driverState_en              = FEE_PRV_PROC_DRIVERSTATE_UNINIT_E;
    Fee_Prv_Proc_st.currentExtJob_pst           = NULL_PTR;
    Fee_Prv_Proc_st.driverModeActv_en           = MEMIF_MODE_SLOW;
# if (FEE_AR_RELEASE_MAJOR_VERSION == 4) && (FEE_AR_RELEASE_MINOR_VERSION >=4)
    /* From Autosar 4.4 and newer */
    Fee_Prv_Proc_st.driverModeReqd_en           = MEMIF_MODE_SLOW;
# endif
    Fee_Prv_Proc_st.backgroundState_en          = FEE_PRV_PROC_BACKGROUNDSTATE_DISABLED;
    Fee_Prv_Proc_st.backgroundDebounceCntr_u16  = FEE_PRV_PROC_MFCALLS_BEFORE_BACKGROUND_ACTION;
    Fee_Prv_Proc_st.backgroundCancelOngoing_b   = FALSE;
    Fee_Prv_Proc_st.stopModeActive_b            = FALSE;

    Fee_Prv_ProcLock_st.flgUsedSema_vb              = FALSE;
    Fee_Prv_ProcLock_st.dbgReentrantMainFunction_vb = FALSE;

    Fee_Prv_ProcWorkingState_en = FEE_RB_IDLE_E;

    return;
}

/*
 * \brief   Disables execution of background operations.
 */
void Fee_Prv_ProcDisableBackground(void)
{
    Fee_Prv_Proc_st.backgroundState_en = FEE_PRV_PROC_BACKGROUNDSTATE_DISABLED;

    return;
}

/*
 * \brief   Enables execution of background operations.
 */
void Fee_Prv_ProcEnableBackground(void)
{
    Fee_Prv_Proc_st.backgroundState_en = FEE_PRV_PROC_BACKGROUNDSTATE_ENABLED_ACTIVE;

    return;
}

/*
 * \brief   Set the state of the driver.
 *
 * \param   driverState_en  The state which shall be entered
 */
void Fee_Prv_ProcSetDriverState(Fee_Prv_ProcDriverState_ten driverState_en)
{
    Fee_Prv_Proc_st.driverState_en = driverState_en;

    return;
}

/*
 * \brief   Return the current state of the driver.
 *
 * \return  Current state of the driver
 */
Fee_Prv_ProcDriverState_ten Fee_Prv_ProcGetDriverState(void)
{
    return(Fee_Prv_Proc_st.driverState_en);
}

/* \brief   Return the current driver execution mode
 *
 * \return  MEMIF_MODE_SLOW: Driver executes in slow mode
 * \return  MEMIF_MODE_FAST: Driver executes in fast mode
 */
MemIf_ModeType Fee_Prv_ProcGetDriverMode(void)
{
    MemIf_ModeType retVal_en;

    /* According to the prerequisites that RTA-BSW has to run on >= 32 bit controllers and data are always aligned,
     * atomic access is guaranteed by the hardware for 32 bit memory access
     */
    retVal_en = Fee_Prv_Proc_st.driverModeActv_en;

   return(retVal_en);
}

# if(STD_ON == FEE_SET_MODE_SUPPORTED)
/**
 * \brief   Set the mode of the Fee and the underlying flash driver
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   mode                requested mode to set
 */
void Fee_Prv_ProcSetDriverMode(Fee_Rb_DeviceName_ten FeeDeviceName_en, MemIf_ModeType mode)
{
    Fls_Rb_FlsDevice_ten flsDeviceId_en = Fls_Rb_GetDeviceIdFromDrvIndex(Fee_Rb_GetDeviceIndexFromDeviceName(FeeDeviceName_en));
    (void)FeeDeviceName_en;

    Fee_Prv_Lib_Fls_SetMode(flsDeviceId_en, mode);
    Fee_Prv_FsIfSetMode(mode);
    /* According to the prerequisites that RTA-BSW has to run on >= 32 bit controllers and data are always aligned,
     * atomic access is guaranteed by the hardware for 32 bit memory access
     */
    Fee_Prv_Proc_st.driverModeActv_en = mode;

    return;
}

#  if (FEE_AR_RELEASE_MAJOR_VERSION == 4) && (FEE_AR_RELEASE_MINOR_VERSION >= 4)
/* \brief   Set the driver execution mode request
 *
 * \param driverModeReq_en  MEMIF_MODE_SLOW: Driver executes in slow mode
 *                          MEMIF_MODE_FAST: Driver executes in fast mode
 */
void Fee_Prv_ProcSetDriverModeReq(MemIf_ModeType driverModeReq_en)
{
    /* According to the prerequisites that RTA-BSW has to run on >= 32 bit controllers and data are always aligned,
     * atomic access is guaranteed by the hardware for 32 bit memory access
     */
    Fee_Prv_Proc_st.driverModeReqd_en = driverModeReq_en;

    return;
}
#  endif

# endif

/*
 * \brief   Returns the current Fee working state.
 *
 * \return  Current working state of the driver
 */
Fee_Rb_WorkingStateType_ten Fee_Prv_ProcGetWorkingState(void)
{
    return(Fee_Prv_ProcWorkingState_en);
}

/*
 * \brief   Activates the stop mode.
 *
 * \param   proc_pst   Pointer to the common variables of the proc unit
 */
LOCAL_INLINE MemIf_JobResultType Fee_Prv_ProcEnterStopModeDo(Fee_Prv_Proc_tst * proc_pst)
{
    proc_pst->stopModeActive_b = TRUE;

    return(MEMIF_JOB_OK);
}

/*
 * \brief   Starts the job requested from the job unit by triggering the filesystem specific handler function.
 *
 * \param   newExtJob_pcst  Pointer to the job descriptor data
 */
LOCAL_INLINE void Fee_Prv_ProcStartExtJob(Fee_Prv_JobDesc_tst const * newExtJob_pcst)
{
    /* Check pending order type */
    switch(newExtJob_pcst->type_en)
    {
        case FEE_PRV_JOBTYPE_READ_E:              Fee_Prv_FsIfRead(newExtJob_pcst);            break;
        case FEE_PRV_JOBTYPE_WRITE_E:             Fee_Prv_FsIfWrite(newExtJob_pcst);           break;
        case FEE_PRV_JOBTYPE_INVALIDATE_E:        Fee_Prv_FsIfInvalidateBlock(newExtJob_pcst); break;
        case FEE_PRV_JOBTYPE_BLOCKMAINTENANCE_E:  Fee_Prv_FsIfRbMaintainBlock(newExtJob_pcst); break;
        case FEE_PRV_JOBTYPE_TRIGGERREORG_E:      Fee_Prv_FsIfRbTriggerReorg();                break;
        case FEE_PRV_JOBTYPE_ENTER_STOP_MODE_E:
        default:                                  /* Do nothing */                             break;
    }

    return;
}

/*
 * \brief   Processes the current external job based on the selected job descriptor.
 *
 * \param   currentExtJob_pcst  Pointer to the job descriptor data
 */
LOCAL_INLINE void Fee_Prv_ProcProcessExtJob(Fee_Prv_Proc_tst * proc_pst)
{
    MemIf_JobResultType          result_en;
    MemIf_Rb_MigrationResult_ten migrationResult_en;

    switch(proc_pst->currentExtJob_pst->type_en)
    {
        case FEE_PRV_JOBTYPE_READ_E:             result_en = Fee_Prv_FsIfReadDo();                  break;
        case FEE_PRV_JOBTYPE_WRITE_E:            result_en = Fee_Prv_FsIfWriteDo();                 break;
        case FEE_PRV_JOBTYPE_INVALIDATE_E:       result_en = Fee_Prv_FsIfInvalidateBlockDo();       break;
        case FEE_PRV_JOBTYPE_BLOCKMAINTENANCE_E: result_en = Fee_Prv_FsIfRbMaintainBlockDo();       break;
        case FEE_PRV_JOBTYPE_TRIGGERREORG_E:     result_en = Fee_Prv_FsIfRbTriggerReorgDo();        break;
        case FEE_PRV_JOBTYPE_ENTER_STOP_MODE_E:  result_en = Fee_Prv_ProcEnterStopModeDo(proc_pst); break;
        default:                                 result_en = MEMIF_JOB_FAILED;                      break;
    }

    if(MEMIF_JOB_PENDING != result_en)
    {
        /* Reset the environment after the job has been finished */
        proc_pst->driverState_en = FEE_PRV_PROC_DRIVERSTATE_IDLE_E;
        migrationResult_en = Fee_Prv_FsIfRbGetMigrationResult();
        Fee_Prv_JobEnd(proc_pst->currentExtJob_pst, result_en, migrationResult_en);
        proc_pst->currentExtJob_pst = NULL_PTR;

        /* Background enabled but sleeping? */
        if(FEE_PRV_PROC_BACKGROUNDSTATE_ENABLED_SLEEPING == proc_pst->backgroundState_en)
        {
            /* Activate background */
            proc_pst->backgroundState_en = FEE_PRV_PROC_BACKGROUNDSTATE_ENABLED_ACTIVE;
        }

        /* Wind up debounce counter */
        proc_pst->backgroundDebounceCntr_u16 = FEE_PRV_PROC_MFCALLS_BEFORE_BACKGROUND_ACTION;
    }

    return;
}

/*
 * \brief   Covers all actions to be done when the driver state is idle.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   proc_pst            Pointer to the common variables of the proc unit
 */
LOCAL_INLINE void Fee_Prv_ProcProcessIdle(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_Prv_Proc_tst * proc_pst)
{
    MemIf_JobResultType result_en = MEMIF_JOB_PENDING;

# if (FEE_AR_RELEASE_MAJOR_VERSION == 4) && (FEE_AR_RELEASE_MINOR_VERSION >= 4) && (STD_ON == FEE_SET_MODE_SUPPORTED)
    /* From Autosar 4.4 and newer
     * Check if the user has requested a driver mode change
     * According to the prerequisites that RTA-BSW has to run on >= 32 bit controllers and data are always aligned,
     * atomic access is guaranteed by the hardware for 32 bit memory access
     */
    MemIf_ModeType      driverModeReqdLocal_en = proc_pst->driverModeReqd_en;

    if(proc_pst->driverModeActv_en != driverModeReqdLocal_en)
    {
        /* Change driver mode */
        Fee_Prv_ProcSetDriverMode(FeeDeviceName_en, driverModeReqdLocal_en);
    }
# endif
    /* Is a new user job available? */
    if(NULL_PTR != proc_pst->currentExtJob_pst)
    {
        /* Yes -> Start the job processing in the Fs and set the appropriate driver state */
        Fee_Prv_ProcStartExtJob(proc_pst->currentExtJob_pst);
        proc_pst->driverState_en = FEE_PRV_PROC_DRIVERSTATE_EXT_JOB_E;
    }
    else
    {
        /* No -> Check whether any maintenance functions can be executed in the background */
        if(FEE_PRV_PROC_BACKGROUNDSTATE_ENABLED_ACTIVE == proc_pst->backgroundState_en)
        {
            /* Debouncing done? */
            if(0u == proc_pst->backgroundDebounceCntr_u16)
            {
                /* Call Fs-specific background function */
                result_en = Fee_Prv_FsIfBackground();

                /* Fs-specific background function has already completed successfully? */
                if(MEMIF_JOB_OK == result_en)
                {
                    /* Put background operations back to sleep */
                    proc_pst->backgroundState_en = FEE_PRV_PROC_BACKGROUNDSTATE_ENABLED_SLEEPING;
                }
                /* Fs-specific background function not yet complete? */
                else if(MEMIF_JOB_PENDING == result_en)
                {
                    /* Continue the Fs-specific background processing */
                    proc_pst->driverState_en = FEE_PRV_PROC_DRIVERSTATE_BACKGROUND_E;
                }
                /* Fs-specific background function has failed? */
                else
                {
                    /* Do nothing */
                }
            }
            /* Debouncing not yet done? */
            else
            {
                /* Before starting a background action, debounce a little to ensure that it does not start immediately */
                proc_pst->backgroundDebounceCntr_u16--;
            }
        }
        else
        {
            /* FEE_PRV_BACKGROUNDSTATE_DISABLED / FEE_PRV_BACKGROUNDSTATE_ENABLED_SLEEPING : Do nothing */
        }
    }

    return;
}

/*
 * \brief   Covers all background actions.
 *
 * \param   proc_pst   Pointer to the common variables of the proc unit
 */
LOCAL_INLINE void Fee_Prv_ProcProcessBackground(Fee_Prv_Proc_tst * proc_pst)
{
    /* Return variable */
    MemIf_JobResultType result_en;

    /* Is a new job available and background cancel not yet started? */
    if((NULL_PTR != proc_pst->currentExtJob_pst) && (!proc_pst->backgroundCancelOngoing_b))
    {
        /* Cancel background activity */
        Fee_Prv_FsIfCancel();
        proc_pst->backgroundCancelOngoing_b = TRUE;
    }

    /* Call Fs-specific background function */
    result_en = Fee_Prv_FsIfBackground();

    if(MEMIF_JOB_PENDING != result_en)
    {
        proc_pst->backgroundCancelOngoing_b = FALSE;
        /* Background job is finished -> switch the driver state to idle */
        proc_pst->driverState_en = FEE_PRV_PROC_DRIVERSTATE_IDLE_E;
    }

    return;
}

/*
 * \brief   Calculates the current Fee working state.
 *
 * \param   driverState_pen Current driver state
 *
 * \return  Returns the ongoing Fee activity.
 */
LOCAL_INLINE Fee_Rb_WorkingStateType_ten Fee_Prv_ProcCalcWorkingState(Fee_Prv_ProcDriverState_ten driverState_en,
                                                                      boolean stopModeActive_b,
                                                                      Fee_Prv_JobDesc_tst const * currentExtJob_pcst)
{
    Fee_Rb_WorkingStateType_ten stRetVal_en;

    switch(driverState_en)
    {
        case FEE_PRV_PROC_DRIVERSTATE_UNINIT_E:
        case FEE_PRV_PROC_DRIVERSTATE_INIT_E:
        {
            stRetVal_en = FEE_RB_IDLE_E;

        }
        break;

        case FEE_PRV_PROC_DRIVERSTATE_IDLE_E:
        {
            if(stopModeActive_b)
            {
                stRetVal_en = FEE_RB_STOPMODE_E;
            }
            else
            {
                stRetVal_en = FEE_RB_IDLE_E;
            }
        }
        break;

        case FEE_PRV_PROC_DRIVERSTATE_BACKGROUND_E:
        {
            /* Check if background erase operation is ongoing */
            if(Fee_Prv_FsIfRbIsSectorEraseOngoing())
            {
                stRetVal_en = FEE_RB_SECTOR_ERASE_E;
            }
            /* Check if background soft reorganisation operation is ongoing */
            else if (Fee_Prv_FsIfRbIsSoftReorgOngoing())
            {
                stRetVal_en = FEE_RB_SOFT_SECTOR_REORG_MODE_E;
            }
            else
            {
                stRetVal_en = FEE_RB_IDLE_E;
            }
        }
        break;

        case FEE_PRV_PROC_DRIVERSTATE_EXT_JOB_E:
        {
            switch(currentExtJob_pcst->type_en)
            {
                case FEE_PRV_JOBTYPE_READ_E:
                {
                    stRetVal_en = FEE_RB_READ_MODE_E;
                }
                break;

                case FEE_PRV_JOBTYPE_ENTER_STOP_MODE_E:
                {
                    stRetVal_en = FEE_RB_STOPMODE_E;
                }
                break;

                case FEE_PRV_JOBTYPE_WRITE_E:
                case FEE_PRV_JOBTYPE_INVALIDATE_E:
                case FEE_PRV_JOBTYPE_BLOCKMAINTENANCE_E:
                case FEE_PRV_JOBTYPE_TRIGGERREORG_E:
                default:
                {
                    /* Check for hard reorg, because this can happen during a write, invalidate or maintain job */
                    if(Fee_Prv_FsIfRbIsHardReorgOngoing())
                    {
                        stRetVal_en = FEE_RB_HARD_SECTOR_REORG_MODE_E;
                    }
                    else if(Fee_Prv_FsIfRbIsSectorEraseOngoing())
                    {
                        stRetVal_en = FEE_RB_SECTOR_ERASE_E;
                    }
                    else
                    {
                        switch(currentExtJob_pcst->type_en)
                        {
                            case FEE_PRV_JOBTYPE_WRITE_E:
                            {
                                /* All FSx */
                                stRetVal_en = FEE_RB_WRITE_MODE_E;
                            }
                            break;

                            case FEE_PRV_JOBTYPE_INVALIDATE_E:
                            {
                                /* All FSx */
                                stRetVal_en = FEE_RB_INVALIDATE_MODE_E;
                            }
                            break;

                            case FEE_PRV_JOBTYPE_BLOCKMAINTENANCE_E:
                            {
                                /* FS1x + FS3 only */
                                stRetVal_en = FEE_RB_MAINTAIN_MODE_E;
                            }
                            break;

                            /* FEE_PRV_JOBTYPE_TRIGGERREORG_E - Fs1x only */
                            default:
                            {
                                stRetVal_en = FEE_RB_IDLE_E;
                            }
                            break;
                        }
                    }
                }
                break;
            }
        }
        break;

        default:
        {
            stRetVal_en = FEE_RB_IDLE_E;
        }
        break;
    }

    return(stRetVal_en);
}

/*
 * \brief   The internal main function does the actual work of the Fee.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   proc_pst            Pointer to the common variables of the proc unit
 */
LOCAL_INLINE void Fee_Prv_ProcMainFunctionIntern(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_Prv_Proc_tst * proc_pst)
{
    /*
     * Measure main function execution time.
     * Attention: The result is not the actual time the main function really needed since it can be interrupted.
     *            Nevertheless it is a good indicator for debugging purposes.
     */
    Fee_Prv_DbgWatchStart(&Fee_Prv_DbgInfo_st.time_st.procMain_st);

    /* Reset the effort limitation */
    Fee_Prv_LibEffortReset();

    /* Is an external job request already accepted? (condition can only apply in idle or background state) */
    if(NULL_PTR == proc_pst->currentExtJob_pst)
    {
        /* No -> Check for a new request */
        proc_pst->currentExtJob_pst = Fee_Prv_JobNext();
    }

    /* Based on the driver state let the Fs process external or internal jobs */
    switch(proc_pst->driverState_en)
    {
        case FEE_PRV_PROC_DRIVERSTATE_IDLE_E:        Fee_Prv_ProcProcessIdle(FeeDeviceName_en, proc_pst);   break;
        case FEE_PRV_PROC_DRIVERSTATE_BACKGROUND_E:  Fee_Prv_ProcProcessBackground(proc_pst);               break;
        case FEE_PRV_PROC_DRIVERSTATE_EXT_JOB_E:     Fee_Prv_ProcProcessExtJob(proc_pst);                   break;
        default:                                     /* Do nothing */                                       break;
    }

    /* Background enabled? */
    if(FEE_PRV_PROC_BACKGROUNDSTATE_DISABLED != proc_pst->backgroundState_en)
    {
        /* Execute cyclic debugging actions */
        Fee_Prv_DbgMainFunction();
    }

    /* Call working state calculation and set private copy */
    Fee_Prv_ProcWorkingState_en = Fee_Prv_ProcCalcWorkingState(proc_pst->driverState_en,
                                                               proc_pst->stopModeActive_b,
                                                               proc_pst->currentExtJob_pst);

    /* Stop timing measurement of main function */
    Fee_Prv_DbgWatchStop(&Fee_Prv_DbgInfo_st.time_st.procMain_st, FALSE);

    return;
}

/*
 * \brief   Checks global flag for re-entrant invocation of main function, and reports such error to DET if enabled.
 *
 * \param   FeeDeviceName_en    Device instance for which lock has to be acquired
 * \param   procLock_pst        Pointer to the variables related to the reentrancy detection
 *
 * \return  Availability of lock
 * \return  TRUE                if lock is available, i.e. no re-entrant invocation of main function.
 * \return  FALSE               if lock is not available, i.e. re-entrant invocation of main function.
 */
LOCAL_INLINE boolean Fee_Prv_ProcTryToGetLock(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_Prv_ProcLock_tst * procLock_pst)
{
    /* Return variable */
    boolean isLockAvailable_b = FALSE;

# if(FEE_RB_USE_PROTECTION != FALSE)
    /* Disable interrupts */
    SchM_Enter_Fee_Order();
# endif

    /* Is the main function not invoked yet? */
    if(!procLock_pst->flgUsedSema_vb)
    {
        /* Reserve it and allow its execution */
        procLock_pst->flgUsedSema_vb = TRUE;
        isLockAvailable_b = TRUE;
    }

# if(FEE_RB_USE_PROTECTION != FALSE)
    /* Enable interrupts */
    SchM_Exit_Fee_Order();
# endif

    /* Re-entrant invocation of main function? */
    if(!isLockAvailable_b)
    {
        /* Set the debug flag for the detection of reentrant main function */
        procLock_pst->dbgReentrantMainFunction_vb = TRUE;

        Fee_Prv_LibDetReport(FeeDeviceName_en, FEE_SID_MAINFUNCTION, FEE_E_BUSY);
    }

# if(FEE_RB_USE_PROTECTION == FALSE)
    /* Protection disabled, only detection by default */
    isLockAvailable_b = TRUE;
# endif

    return(isLockAvailable_b);
}

/**
 * \brief   Resets the flag having re-entrant information.
 *
 * \param   procLock_pst   Pointer to the variables related to the reentrancy detection
 */
LOCAL_INLINE void Fee_Prv_ProcReleaseLock(Fee_Prv_ProcLock_tst * procLock_pst)
{
    /* Reset the access flag */
    procLock_pst->flgUsedSema_vb = FALSE;

    return;
}

/*
 * \brief   The main function does the actual work of the Fee. The more often it is scheduled the faster the Fee is.
 *
 * \param   FeeDeviceName_en       Device instance for which job has to be performed
 */
void Fee_Rb_Idx_MainFunction(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    // Check parameter
    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_MAINFUNCTION))
    {
// Below code is needed only for multi instance with Fee1.0. To be removed when either Fee1.0 is removed or multi instance support with Fee1.0 is removed.
#  if (defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if (Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            // This request is for device index = 0, this means route this request to Fee1.0
            Fee_MainFunction();
        }
        else
#  endif
        {
            /* Local flag to store the lock accuring status */
            boolean flgUsed_b = FALSE;

            /* Check for re-entrant invocation of main function */
            flgUsed_b = Fee_Prv_ProcTryToGetLock(FeeDeviceName_en, &Fee_Prv_ProcLock_st);

            /* Lock obtained, i.e. no re-entrant invocation of main function? */
            if(flgUsed_b)
            {
                if(!Fee_Prv_Proc_st.stopModeActive_b)
                {
                    /* Main function can be invoked */
                    Fee_Prv_ProcMainFunctionIntern(FeeDeviceName_en, &Fee_Prv_Proc_st);
                }

                /* Release the lock */
                Fee_Prv_ProcReleaseLock(&Fee_Prv_ProcLock_st);
            }
        }
    }
    return;
}

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

#endif
