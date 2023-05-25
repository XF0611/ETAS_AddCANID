

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

/*
 **********************************************************************************************************************
 * Static declaration
 **********************************************************************************************************************
 */

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"

static Std_ReturnType Fee_Prv_OrderDetCheckModuleInit      (Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8                                    );
static Std_ReturnType Fee_Prv_OrderDetCheckBlkCfg          (Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16                  );
static Std_ReturnType Fee_Prv_OrderDetCheckAdrPtr          (Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, void const * bfr_pcv              );
static Std_ReturnType Fee_Prv_OrderDetCheckBlkLenNull      (Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkLen_u16                 );
static Std_ReturnType Fee_Prv_OrderDetCheckBlkVarLenCfg    (Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16, boolean active_b);
static Std_ReturnType Fee_Prv_OrderDetCheckBlkMigrationCfg (Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16, boolean active_b);

/*
 **********************************************************************************************************************
 * Inline declaration
 **********************************************************************************************************************
 */

# if(STD_ON == FEE_SET_MODE_SUPPORTED)
LOCAL_INLINE Std_ReturnType Fee_Prv_OrderDetCheckModuleIdle (Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8);
# endif
LOCAL_INLINE void Fee_Prv_OrderDetModuleInitFail(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8);
LOCAL_INLINE Std_ReturnType Fee_Prv_OrderDetCheckBlkOfs(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16, uint16 blkOfs_u16);
LOCAL_INLINE Std_ReturnType Fee_Prv_OrderDetCheckBlkLen(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16, uint16 blkOfs_u16, uint16 blkLen_u16);

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
 */

/**
 * \brief   Check whether the Device name (or device index) matches with what is passed.
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 *
 * \retval  E_OK                Device name does exist
 * \retval  E_NOT_OK            Device name does not exist
 */
Std_ReturnType Fee_Prv_OrderDetCheckDeviceName (Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8)
{
    Std_ReturnType result_en = E_OK;
    if(FeeDeviceName_en >= Fee_Rb_Device_Max)
    {
        Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_INVALID_DEVICE_NAME);
        result_en = E_NOT_OK;
    }

    return(result_en);
}
/**
 * \brief   Check whether the Fee is initialized (Fee_Init was called).
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 *
 * \retval  E_OK                Fee is initialized
 * \retval  E_NOT_OK            Fee is not initialized
 */
static Std_ReturnType Fee_Prv_OrderDetCheckModuleInit(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8)
{
    Std_ReturnType result_en = E_OK;
    if(MEMIF_UNINIT == Fee_Rb_Idx_GetStatus(FeeDeviceName_en))
    {
        Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_UNINIT);
        result_en = E_NOT_OK;
    }

    return(result_en);
}

# if(STD_ON == FEE_SET_MODE_SUPPORTED)
/**
 * \brief   Check whether the Fee is currently idle
 *          - Fee_Init was called
 *          - No internal or external action is being processed
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 *
 * \retval  E_OK                Fee_Rb_Idx_GetStatus(FeeDeviceName_en) returns MEMIF_IDLE
 * \retval  E_NOT_OK            Fee_Rb_Idx_GetStatus(FeeDeviceName_en) does not return MEMIF_IDLE
 */
LOCAL_INLINE Std_ReturnType Fee_Prv_OrderDetCheckModuleIdle(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8)
{
    Std_ReturnType result_en;
    MemIf_StatusType status_en;

    status_en = Fee_Rb_Idx_GetStatus(FeeDeviceName_en);
    /* if the driver is busy, a job is requested, but eventually the job is not yet started */
    if(
            /* MR12 RULE 13.5 VIOLATION: Only getter without side effects */
            ((MEMIF_BUSY_INTERNAL == status_en) || (MEMIF_BUSY == status_en)) &&
            (FEE_PRV_PROC_DRIVERSTATE_IDLE_E == Fee_Prv_ProcGetDriverState())
    )
    {
        /* driver is initialised, a job is requested, but the driver is still idle
           -> actions for idle state will be performed, no error will be reported */
        status_en = MEMIF_IDLE;
    }

    switch(status_en)
    {
        case MEMIF_IDLE:
            /* driver is initialised
             * a job (Nvm, Adapter or Debug) may be requested but the driver is still idle */
            result_en = E_OK;
            break;

        case MEMIF_UNINIT:
            Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_UNINIT);
            result_en = E_NOT_OK;
            break;

        case MEMIF_BUSY_INTERNAL:
#  if(FEE_AR_RELEASE_MAJOR_VERSION == 4) && (FEE_AR_RELEASE_MINOR_VERSION >=4)
            /* From Autosar 4.4 and newer */
            result_en = E_OK;
#  else
            Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_BUSY_INTERNAL);
            result_en = E_NOT_OK;
#  endif
            break;

        case MEMIF_BUSY:
            Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_BUSY);
            result_en = E_NOT_OK;
            break;

        default:
            result_en = E_NOT_OK;
            break;
    }

    return(result_en);
}
# endif

/**
 * \brief   Check whether the request is valid for the given block number.
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 * \param   blkNr_u16           The block number that shall be checked
 *
 * \retval  E_OK                The block config is valid
 * \retval  E_NOT_OK            The block config is not valid
 */
static Std_ReturnType Fee_Prv_OrderDetCheckBlkCfg(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16)
{
    Std_ReturnType result_en = E_OK;
    if(FEE_NUM_BLOCKS <= blkNr_u16)
    {
        Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_INVALID_BLOCK_NO);
        result_en = E_NOT_OK;
    }
    else
    {
        if(FEE_SID_RB_MAINTAIN == apiId_u8)
        {
            if( ! Fee_Prv_ConfigIsBlockDoubleStorageActiveByBlockNr(blkNr_u16))
            {
                // reject Maintain orders for non-redundant copies
                Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_INVALID_BLOCK_CFG);
                result_en = E_NOT_OK;
            }
        }
    }
    return(result_en);
}

/**
 * \brief   Check whether a pointer is a NULL_PTR.
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 * \param   bfr_pcv             The pointer that shall be checked
 *
 * \retval  E_OK                The pointer is not a NULL_PTR
 * \retval  E_NOT_OK            The pointer is a NULL_PTR
 */
static Std_ReturnType Fee_Prv_OrderDetCheckAdrPtr(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, void const * bfr_pcv)
{
    Std_ReturnType result_en = E_OK;
    if(NULL_PTR == bfr_pcv)
    {
        Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_PARAM_POINTER);
        result_en = E_NOT_OK;
    }
    return(result_en);
}

/**
 * \brief   Check whether an offset is longer or equal the block length.
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 * \param   blkNr_u16           The block number
 * \param   blkOfs_u16          The offset that shall be checked
 *
 * \retval  E_OK                The offset is smaller than the length of the block
 * \retval  E_NOT_OK            The offset is longer or equal the length of the block
 */
LOCAL_INLINE Std_ReturnType Fee_Prv_OrderDetCheckBlkOfs(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16, uint16 blkOfs_u16)
{
    Std_ReturnType result_en = E_OK;
    if(blkOfs_u16 >= Fee_Prv_ConfigGetBlockLengthByBlockNr(blkNr_u16))
    {
        Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_INVALID_BLOCK_OFS);
        result_en = E_NOT_OK;
    }
    return(result_en);
}

/**
 * \brief   Check that length + offset do not exceed block length.
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 * \param   blkNr_u16           The block number
 * \param   blkOfs_u16          The offset
 * \param   blkLen_u16          The requested length
 *
 * \retval  E_OK                Offset + Length are bigger than the block length
 * \retval  E_NOT_OK            Offset + Length are smaller or equal the block length
 */
LOCAL_INLINE Std_ReturnType Fee_Prv_OrderDetCheckBlkLen(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16, uint16 blkOfs_u16, uint16 blkLen_u16)
{
    Std_ReturnType result_en = E_OK;
    if((blkOfs_u16 + blkLen_u16) > Fee_Prv_ConfigGetBlockLengthByBlockNr(blkNr_u16))
    {
        Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_INVALID_BLOCK_LEN);
        result_en = E_NOT_OK;
    }
    return(result_en);
}

/**
 * \brief   Check that requested block length is not zero.
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 * \param   blkNr_u16           The block number
 * \param   blkLen_u16          The requested length
 *
 * \retval  E_OK                Requested length is bigger than zero
 * \retval  E_NOT_OK            Requested length is zero
 */
static Std_ReturnType Fee_Prv_OrderDetCheckBlkLenNull(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkLen_u16)
{
    Std_ReturnType result_en = E_OK;
    if(0u == blkLen_u16)
    {
        Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_INVALID_BLOCK_LEN);
        result_en = E_NOT_OK;
    }
    return(result_en);
}

/**
 * \brief   Checks that requested block is configured for variable block length.
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 * \param   blkNr_u16           The block number
 * \param   active_b            Under which condition the det error should be set
 *
 * \retval  E_OK                Block is configured for variable block length
 * \retval  E_NOT_OK            Block is not configured for variable block length
 */
static Std_ReturnType Fee_Prv_OrderDetCheckBlkVarLenCfg(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16, boolean active_b)
{
    Std_ReturnType result_en = E_OK;
    if(active_b != Fee_Prv_ConfigIsBlockLengthVariableByBlockNr(blkNr_u16))
    {
        Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_INVALID_VAR_BLK_LEN_CFG);
        result_en = E_NOT_OK;
    }
    return(result_en);
}

/**
 * \brief   Checks that requested block is configured for migration.
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 * \param   blkNr_u16           The block number
 * \param   active_b            Under which condition the det error should be set
 *
 * \retval  E_OK                Block is configured for migration
 * \retval  E_NOT_OK            Block is not configured for migration
 */
static Std_ReturnType Fee_Prv_OrderDetCheckBlkMigrationCfg(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint16 blkNr_u16, boolean active_b)
{
    Std_ReturnType result_en        = E_OK;
    uint16         persistentId_u16 = Fee_Prv_ConfigGetBlockPersistentIdByBlockNr(blkNr_u16);

    if(active_b != Fee_Prv_ConfigIsBlockMigrationActiveByPersistentId(persistentId_u16))
    {
        Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_INVALID_MIGRATION_CFG);
        result_en = E_NOT_OK;
    }
    return(result_en);
}

/**
 * \brief   Raise init fail error (Fee_init fails).
 *
 * \param   FeeDeviceName_en    Device instance for which check has to be performed
 * \param   apiId_u8            The API which requests this check, needed for Det_ReportError in case of failure
 *
 */
LOCAL_INLINE void Fee_Prv_OrderDetModuleInitFail(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8)
{
    Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_INIT_FAILED);
}

/**
 * \brief   Returns if the passed DeviceIndex belongs to Fee or not. The function is required when multi instance of Fee is used
 *
 * \param   DeviceIndex_u8  Device index which has to be evaluated
 *
 * \retval  TRUE            Passed Device Index belongs to Fee
 * \retval  FALSE           Passed Device Index does not belong to Fee
 */
boolean Fee_Rb_IsThisDeviceIndexFee(uint8 DeviceIndex_u8)
{
    boolean isFeeDevice_b;

    if(Fee_Rb_Device_Max != Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex_u8))
    {
        isFeeDevice_b = TRUE;
    }
    else
    {
        isFeeDevice_b = FALSE;
    }

    return(isFeeDevice_b);
}

/**
 * \brief   Get the Device name from the configured FeeRbGeneralDeviceIndex. This DeviceID name is then used while
 * accessing the corresponding instance via the API Fee_Rb_Idx_.
 * When device index is not found, Fee_Rb_FeeDevice_Max is returned -> indicating that the device does not exist.
 *
 * \param   feeDeviceIdx_u8         Device index as configured in paramdef FeeRbGeneralDeviceIndex
 *
 * \retval  Fee_Rb_DeviceName_ten   DeviceID name corresponding to the passed DeviceIndex
 */

Fee_Rb_DeviceName_ten Fee_Rb_GetDeviceNameFromDeviceIndex(uint8 feeDeviceIdx_u8)
{
    uint8 idx;
    Fee_Rb_DeviceName_ten deviceName_en = Fee_Rb_Device_Max;

    for(idx=0u ; idx < (uint8)Fee_Rb_Device_Max; idx++)
    {
        if(Fee_Prv_Idx_DeviceConfig_cast[idx].deviceIdx_u8 == feeDeviceIdx_u8)
        {
            deviceName_en = Fee_Prv_Idx_DeviceConfig_cast[idx].deviceName_en;
            break;
        }
    }

    return (deviceName_en);
}

/**
 * \brief   Get the Device index name from the passed device name.
 * When device name is not found, 255 is returned -> indicating that the device does not exist.
 *
 * \param   FeeDeviceName_en        Device name for which device index has to be returned
 *
 * \retval  Fee_Rb_DeviceName_ten   DeviceID name corresponding to the passed DeviceIndex
 */

uint8 Fee_Rb_GetDeviceIndexFromDeviceName(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    uint8 idx;
    uint8 deviceIdx_u8 = 0xFF;

    for(idx=0u ; idx < (uint8)Fee_Rb_Device_Max; idx++)
    {
        if(Fee_Prv_Idx_DeviceConfig_cast[idx].deviceName_en == FeeDeviceName_en)
        {
            deviceIdx_u8 = Fee_Prv_Idx_DeviceConfig_cast[idx].deviceIdx_u8;
            break;
        }
    }

    return (deviceIdx_u8);
}

/*
 * \brief   Initializes the Fee.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   ConfigPtr:          Reference to postbuild variant. Has to be NULL_PTR because the Fee is not postbuild selectable.
 */
/*MR12 RULE 8.13 VIOLATION: AR4.2 defines this function prototype with a non-constant pointer*/
void Fee_Rb_Idx_Init(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_ConfigType const * ConfigPtr)
{
# if(FEE_AR42_API_COMPLIANCE == STD_ON)
    (void)ConfigPtr;
# endif
    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_INIT))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
#   if(FEE_AR42_API_COMPLIANCE == STD_ON)
            Fee_Init(ConfigPtr);
#   else
            Fee_Init();
#   endif
        }
        else
#  endif
        {
            /* Reset all variables inside Fee common component */
            Fee_Prv_DbgInit();
            Fee_Prv_LibInit();
            Fee_Prv_MediumInit(FeeDeviceName_en);
            Fee_Prv_JobInit();
            Fee_Prv_ProcInit();

            /* Measure the filesystem init time */
            Fee_Prv_DbgWatchStart(&Fee_Prv_DbgInfo_st.time_st.orderInit_st);

            Fee_Prv_ProcSetDriverState(FEE_PRV_PROC_DRIVERSTATE_INIT_E);

            /* Do not limit the effort to spend in the init - full speed is required */
            Fee_Prv_LibUseEffortLimit(FALSE);

            /*
             * Set the medium synchronous.
             * That means the flash is polled and Fls_MainFunction calls are triggered until flash jobs are finished.
             */
            Fee_Prv_MediumSetSync(TRUE);

            /* Initialize the file system - this will take the most time */
            if(E_OK == Fee_Prv_FsIfInit())
            {
                /* Finally set the driver state to IDLE */
                Fee_Prv_ProcSetDriverState(FEE_PRV_PROC_DRIVERSTATE_IDLE_E);

                Fee_Prv_DbgWatchStop(&Fee_Prv_DbgInfo_st.time_st.orderInit_st, FALSE);

                /* If enabled read the debug block */
                Fee_Prv_DbgBlockRead();

                /* Stop the system if a block was lost */
                Fee_Prv_DbgCheckLayout();

                /* Disable sync mode operation */
                Fee_Prv_MediumSetSync(FALSE);
            }
            else
            {
                /* Driver init was not successful --> uninit the Fee */
                Fee_Prv_ProcSetDriverState(FEE_PRV_PROC_DRIVERSTATE_UNINIT_E);

                /* Report DET error */
                Fee_Prv_OrderDetModuleInitFail(FeeDeviceName_en, FEE_SID_INIT);
            }
        }
    }
    return;
}

/*
 * \brief   Mark end of the Fee init. After calling this function background activities are allowed.
 *          Also the Fee is set in a more cooperative mode to make sure task duration constraints are not violated.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 */
void Fee_Rb_Idx_EndInit(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_END_INIT))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            Fee_Rb_EndInit();
        }
        else
#  endif
        {
            if(E_OK == Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_END_INIT))
            {
                Fee_Prv_LibUseEffortLimit(TRUE);
                Fee_Prv_ProcEnableBackground();
            }
        }
    }
    return;
}

/**
 * \brief   Start a read job.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   Blocknumber         Block number as generated in Fee_Cfg.h - This is not the persistent ID!
 * \param   BlockOffset         The offset where the read shall start from in bytes
 * \param   DataBufferPtr       Buffer to read data to
 * \param   Length              Amount of data to read in bytes
 *
 * \retval  E_OK                Order accepted, will be executed asynchronously
 * \retval  E_NOT_OK            Order not accepted
 */
Std_ReturnType Fee_Rb_Idx_Read(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber, uint16 BlockOffset, uint8 * DataBufferPtr, uint16 Length)
{
    Std_ReturnType result_en = E_OK;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_READ))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            result_en = Fee_Read(Blocknumber, BlockOffset, DataBufferPtr, Length);
        }
        else
#  endif
        {
            if(E_OK != Fee_Prv_OrderDetCheckModuleInit (FeeDeviceName_en, FEE_SID_READ))
            {
                result_en = E_NOT_OK;
            }
            /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
            if(E_OK != Fee_Prv_OrderDetCheckAdrPtr(FeeDeviceName_en, FEE_SID_READ, (void const *)DataBufferPtr))
            {
                result_en = E_NOT_OK;
            }
            if(E_OK == Fee_Prv_OrderDetCheckBlkCfg(FeeDeviceName_en, FEE_SID_READ, Blocknumber))
            {
                if(E_OK != Fee_Prv_OrderDetCheckBlkOfs(FeeDeviceName_en, FEE_SID_READ, Blocknumber, BlockOffset))
                {
                    result_en = E_NOT_OK;
                }
                if(E_OK != Fee_Prv_OrderDetCheckBlkLen (FeeDeviceName_en, FEE_SID_READ, Blocknumber, BlockOffset, Length))
                {
                    result_en = E_NOT_OK;
                }
                if(E_OK != Fee_Prv_OrderDetCheckBlkMigrationCfg (FeeDeviceName_en, FEE_SID_READ, Blocknumber, FALSE))
                {
                    result_en = E_NOT_OK;
                }
            }
            else
            {
                result_en = E_NOT_OK;
            }

            if(E_OK == result_en)
            {
                Fee_Prv_JobDesc_tst jobDesc_st;

                /*
                 * Prepare order and try to place it.
                 * If placing the order is not possible E_NOT_OK is returned by Fee_Prv_JobPut.
                 */
                jobDesc_st.type_en                = FEE_PRV_JOBTYPE_READ_E;
                jobDesc_st.bfr_pu8                = DataBufferPtr;
                jobDesc_st.bfr_pcu8               = NULL_PTR;
                jobDesc_st.blockNumber_u16        = Blocknumber;
                jobDesc_st.length_u16             = Length;
                jobDesc_st.offset_u16             = BlockOffset;

                result_en = Fee_Prv_JobPut(FeeDeviceName_en, FEE_SID_READ, &jobDesc_st);
            }
        }
    }
    else
    {
        result_en = E_NOT_OK;
    }

    return(result_en);
}

/**
 * \brief   Starts a read job when block length is variable and calculated on runtime.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   Blocknumber         Block number as generated in Fee_Cfg.h - This is not the persistent ID!
 * \param   BlockOffset         The offset where the read shall start from in bytes
 * \param   DataBufferPtr       Buffer to read data to
 * \param   Length              Amount of data to read in bytes
 *
 * \retval  E_OK                Order accepted, will be executed asynchronously
 * \retval  E_NOT_OK            Order not accepted
 */
Std_ReturnType Fee_Rb_Idx_VarLenRead(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber, uint16 BlockOffset, uint8 * DataBufferPtr, uint16 Length)
{
    Std_ReturnType result_en = E_OK;

    /* Check parameters */
    if(E_OK != Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_VARLENREAD))
    {
        result_en = E_NOT_OK;
    }

    if(E_OK != Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_VARLENREAD))
    {
        result_en = E_NOT_OK;
    }
    /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
    if(E_OK != Fee_Prv_OrderDetCheckAdrPtr(FeeDeviceName_en, FEE_SID_RB_VARLENREAD, (void const *)DataBufferPtr))
    {
        result_en = E_NOT_OK;
    }
    if(E_OK != Fee_Prv_OrderDetCheckBlkLenNull(FeeDeviceName_en, FEE_SID_RB_VARLENREAD, Length))
    {
        result_en = E_NOT_OK;
    }

    if(E_OK == Fee_Prv_OrderDetCheckBlkCfg(FeeDeviceName_en, FEE_SID_RB_VARLENREAD, Blocknumber))
    {
        if(E_OK != Fee_Prv_OrderDetCheckBlkMigrationCfg (FeeDeviceName_en, FEE_SID_RB_VARLENREAD, Blocknumber, TRUE) )
        {
            result_en = E_NOT_OK;
        }
    }
    else
    {
        result_en = E_NOT_OK;
    }

    if(E_OK == result_en)
    {
        Fee_Prv_JobDesc_tst jobDesc_st;

        /*
         * Prepare order and try to place it.
         * If placing the order is not possible E_NOT_OK is returned by Fee_Prv_JobPut.
         */
        jobDesc_st.type_en                = FEE_PRV_JOBTYPE_READ_E;
        jobDesc_st.bfr_pu8                = DataBufferPtr;
        jobDesc_st.bfr_pcu8               = NULL_PTR;
        jobDesc_st.blockNumber_u16        = Blocknumber;
        jobDesc_st.length_u16             = Length;
        jobDesc_st.offset_u16             = BlockOffset;

        result_en = Fee_Prv_JobPut(FeeDeviceName_en, FEE_SID_RB_VARLENREAD, &jobDesc_st);
    }

    return(result_en);
}

/**
 * \brief   Start a write job.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   Blocknumber         Block number as generated in Fee_Cfg.h - This is not the persistent ID!
 * \param   DataBufferPtr       Buffer to write data from
 *
 * \retval  E_OK                Order accepted, will be executed asynchronously
 * \retval  E_NOT_OK            Order not accepted
 */
/* MR12 RULE 8.13 VIOLATION: DataBufferPtr is not const because it is also not const in the public header file */
Std_ReturnType Fee_Rb_Idx_Write(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber, uint8 * DataBufferPtr)
{
    Std_ReturnType result_en = E_OK;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_WRITE))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            result_en = Fee_Write(Blocknumber, DataBufferPtr);
        }
        else
#  endif
        {
            if(E_OK != Fee_Prv_OrderDetCheckModuleInit    (FeeDeviceName_en, FEE_SID_WRITE)) { result_en = E_NOT_OK; }
            /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
            if(E_OK != Fee_Prv_OrderDetCheckAdrPtr        (FeeDeviceName_en, FEE_SID_WRITE, (void const *)DataBufferPtr))
            {
                result_en = E_NOT_OK;
            }

            if(E_OK == Fee_Prv_OrderDetCheckBlkCfg(FeeDeviceName_en, FEE_SID_WRITE, Blocknumber))
            {
                if(E_OK != Fee_Prv_OrderDetCheckBlkVarLenCfg  (FeeDeviceName_en, FEE_SID_WRITE, Blocknumber, FALSE))
                {
                    result_en = E_NOT_OK;
                }
            }
            else
            {
                result_en = E_NOT_OK;
            }

            if(E_OK == result_en)
            {
                Fee_Prv_JobDesc_tst jobDesc_st;

                /*
                 * Prepare order and try to place it.
                 * If placing the order is not possible E_NOT_OK is returned by Fee_Prv_JobPut.
                 */
                jobDesc_st.type_en            = FEE_PRV_JOBTYPE_WRITE_E;
                jobDesc_st.bfr_pu8            = NULL_PTR;
                jobDesc_st.bfr_pcu8           = DataBufferPtr;
                jobDesc_st.blockNumber_u16    = Blocknumber;
                jobDesc_st.length_u16         = Fee_Prv_ConfigGetBlockLengthByBlockNr(Blocknumber);
                jobDesc_st.offset_u16         = 0;

                result_en = Fee_Prv_JobPut(FeeDeviceName_en, FEE_SID_WRITE, &jobDesc_st);
            }
        }
    }
    else
    {
        result_en = E_NOT_OK;
    }

    return(result_en);
}

/**
 * \brief   Starts a write job when block length is variable and calculated on runtime.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   Blocknumber         Block number as generated in Fee_Cfg.h - This is not the persistent ID!
 * \param   DataBufferPtr       Buffer to write data from
 * \param   Length              NV block length
 *
 * \retval  E_OK                Order accepted, will be executed asynchronously
 * \retval  E_NOT_OK            Order not accepted
 */
/* MR12 RULE 8.13 VIOLATION: DataBufferPtr is not const because it is also not const in the public header file */
Std_ReturnType Fee_Rb_Idx_VarLenWrite(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber, uint8 * DataBufferPtr, uint16 Length)
{
    Std_ReturnType result_en = E_OK;

    /* Check parameters */
    if(E_OK != Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_VARLENWRITE))
    {
        result_en = E_NOT_OK;
    }
    if(E_OK != Fee_Prv_OrderDetCheckModuleInit    (FeeDeviceName_en, FEE_SID_RB_VARLENWRITE))
    {
        result_en = E_NOT_OK;
    }
    /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
    if(E_OK != Fee_Prv_OrderDetCheckAdrPtr        (FeeDeviceName_en, FEE_SID_RB_VARLENWRITE, (void const *)DataBufferPtr)    )
    {
        result_en = E_NOT_OK;
    }
    if(E_OK != Fee_Prv_OrderDetCheckBlkLenNull    (FeeDeviceName_en, FEE_SID_RB_VARLENWRITE, Length))
    {
        result_en = E_NOT_OK;
    }

    if(E_OK == Fee_Prv_OrderDetCheckBlkCfg(FeeDeviceName_en, FEE_SID_RB_VARLENWRITE, Blocknumber))
    {
        if(E_OK != Fee_Prv_OrderDetCheckBlkVarLenCfg  (FeeDeviceName_en, FEE_SID_RB_VARLENWRITE, Blocknumber, TRUE))
        {
            result_en = E_NOT_OK;
        }
    }
    else
    {
        result_en = E_NOT_OK;
    }

    if(E_OK == result_en)
    {
        Fee_Prv_JobDesc_tst jobDesc_st;

        /*
         * Prepare order and try to place it.
         * If placing the order is not possible E_NOT_OK is returned by Fee_Prv_JobPut.
         */
        jobDesc_st.type_en            = FEE_PRV_JOBTYPE_WRITE_E;
        jobDesc_st.bfr_pu8            = NULL_PTR;
        jobDesc_st.bfr_pcu8           = DataBufferPtr;
        jobDesc_st.blockNumber_u16    = Blocknumber;
        jobDesc_st.length_u16         = Length;
        jobDesc_st.offset_u16         = 0;

        result_en = Fee_Prv_JobPut(FeeDeviceName_en, FEE_SID_RB_VARLENWRITE, &jobDesc_st);
    }

    return(result_en);
}

/**
 * \brief   Start an invalidate job.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   Blocknumber         Block number as generated in Fee_Cfg.h - This is not the persistent ID!
 *
 * \retval  E_OK        Order accepted, will be executed asynchronously
 * \retval  E_NOT_OK    Order not accepted
 */
Std_ReturnType Fee_Rb_Idx_InvalidateBlock(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber)
{
    Std_ReturnType result_en = E_OK;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_INVALIDATE))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            result_en = Fee_InvalidateBlock(Blocknumber);
        }
        else
#  endif
        {
            if(E_OK != Fee_Prv_OrderDetCheckModuleInit (FeeDeviceName_en, FEE_SID_INVALIDATE))
            {
                result_en = E_NOT_OK;
            }
            if(E_OK != Fee_Prv_OrderDetCheckBlkCfg (FeeDeviceName_en, FEE_SID_INVALIDATE, Blocknumber))
            {
                result_en = E_NOT_OK;
            }

            if(E_OK == result_en)
            {
                Fee_Prv_JobDesc_tst jobDesc_st;

                /*
                 * Prepare order and try to place it.
                 * If placing the order is not possible E_NOT_OK is returned by Fee_Prv_JobPut.
                 */
                jobDesc_st.type_en            = FEE_PRV_JOBTYPE_INVALIDATE_E;
                jobDesc_st.bfr_pu8            = NULL_PTR;
                jobDesc_st.bfr_pcu8           = NULL_PTR;
                jobDesc_st.blockNumber_u16    = Blocknumber;
                jobDesc_st.length_u16         = 0;
                jobDesc_st.offset_u16         = 0;

                result_en = Fee_Prv_JobPut(FeeDeviceName_en, FEE_SID_INVALIDATE, &jobDesc_st);
            }
        }
    }
    else
    {
        result_en = E_NOT_OK;
    }

    return(result_en);
}

/**
 * \brief   Erase immediate data.
 *
 * \param   Blocknumber         Block number as generated in Fee_Cfg.h - This is not the persistent ID!
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 *
 * \retval  E_OK        Order accepted, will be executed asynchronously
 * \retval  E_NOT_OK    Order not accepted
 *
 * \attention   Currently not supported!
 */
Std_ReturnType Fee_Rb_Idx_EraseImmediateBlock(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 BlockNumber)
{
    (void)FeeDeviceName_en;
    (void)BlockNumber;
    return(E_NOT_OK);
}

/**
 * \brief   Cancel running operation.
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 *
 * \attention   Currently not supported!
 */
void Fee_Rb_Idx_Cancel(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    (void)FeeDeviceName_en;
    return;
}

/**
 * \brief   Get the status of the Fee module.
 *
 * \param   FeeDeviceName_en    Device instance for which Fee status has to be returned
 *
 * \retval  MEMIF_UNINIT        The Fee_Init was not called yet
 * \retval  MEMIF_IDLE          The Fee_Init was called, NvM and adapter slots are free, no background operation running
 * \retval  MEMIF_BUSY_INTERNAL a) Adapter job is running
 *                              b) Background operation (maybe erase) is running
 *                              c) Fee_Init is running
 * \retval  MEMIF_BUSY          NvM order is running
 */
MemIf_StatusType Fee_Rb_Idx_GetStatus(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    MemIf_StatusType            result_en = MEMIF_UNINIT;
    Fee_Prv_ProcDriverState_ten driverState_en;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_GETSTATUS))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            result_en = Fee_GetStatus();
        }
        else
#  endif
        {
            driverState_en = Fee_Prv_ProcGetDriverState();
            if(FEE_PRV_PROC_DRIVERSTATE_INIT_E < driverState_en)
            {
                result_en = MEMIF_IDLE;
                if(FEE_PRV_JOBTYPE_NONE_E != Fee_Prv_JobGetActive(FEE_PRV_REQUESTER_NVM_E))
                {
                    result_en = MEMIF_BUSY;
                }
                /* MR12 RULE 13.5 VIOLATION: Only getter without side effects */
                else if((FEE_PRV_JOBTYPE_NONE_E != Fee_Prv_JobGetActive(FEE_PRV_REQUESTER_ADAPTER_E)) ||
                        (FEE_PRV_JOBTYPE_NONE_E != Fee_Prv_JobGetActive(FEE_PRV_REQUESTER_DEBUG_E)))
                {
                    result_en = MEMIF_BUSY_INTERNAL;
                }
                else if(FEE_PRV_PROC_DRIVERSTATE_BACKGROUND_E == driverState_en)
                {
                    result_en = MEMIF_BUSY_INTERNAL;
                }
                else
                {
                    /* intentionally empty */
                }
            }
            else if(FEE_PRV_PROC_DRIVERSTATE_INIT_E == driverState_en)
            {
                result_en = MEMIF_BUSY_INTERNAL;
            }
            else
            {
                /* intentionally empty */
            }
        }
    }
    else
    {
        result_en = MEMIF_IDLE;
    }

    return(result_en);
}

/**
 * \brief   Return the result of the last NvM order.
 *
 * \param   FeeDeviceName_en            Device instance for which job result has to be returned
 *
 * \retval  MEMIF_JOB_OK                Last job executed successfully
 * \retval  MEMIF_JOB_FAILED            Last job failed unexpected
 * \retval  MEMIF_JOB_PENDING           Last job is still running
 * \retval  MEMIF_BLOCK_INCONSISTENT    Last job was a read and
 *                                      a) Not a single instance of all instances has consistent data
 *                                      b) The block is not present at all
 * \retval  MEMIF_BLOCK_INVALIDATED     Last job was a read and the block was invalidated intentionally
 *
 * \attention   Since the job result can change asynchronously a time consuming spin lock is needed!
 *              Please consider this when creating do/while wait loops for the Fee.
 */
MemIf_JobResultType Fee_Rb_Idx_GetJobResult(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    MemIf_JobResultType result_en;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_GETJOBRESULT))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            result_en = Fee_GetJobResult();
        }
        else
#  endif
        {
            if(E_OK != Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_GETJOBRESULT))
            {
                result_en = MEMIF_JOB_FAILED;
            }
            else
            {
                result_en = Fee_Prv_JobGetResult(FEE_PRV_REQUESTER_NVM_E);
            }
        }
    }
    else
    {
        result_en = MEMIF_JOB_FAILED;
    }

    return(result_en);
}

/**
 * \brief   Return the result of the last Adapter order.
 *
 * \param   FeeDeviceName_en            Device instance for which job result has to be returned
 *
 * \retval  MEMIF_JOB_OK                Last job executed successfully
 * \retval  MEMIF_JOB_FAILED            Last job failed unexpected
 * \retval  MEMIF_JOB_PENDING           Last job is still running
 * \retval  MEMIF_BLOCK_INCONSISTENT    Last job was a read and
 *                                      a) Not a single instance of all instances has consistent data
 *                                      b) The block is not present at all
 * \retval  MEMIF_BLOCK_INVALIDATED     Last job was a read and the block was invalidated intentionally
 */
MemIf_JobResultType Fee_Rb_Idx_GetAdapterJobResult(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    MemIf_JobResultType result_en;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_ADAPTERGETJOBRESULT))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            result_en = Fee_Rb_GetAdapterJobResult();
        }
        else
#  endif
        {
            if(E_OK != Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_ADAPTERGETJOBRESULT))
            {
                result_en = MEMIF_JOB_FAILED;
            }
            else
            {
                result_en = Fee_Prv_JobGetResult(FEE_PRV_REQUESTER_ADAPTER_E);
            }
        }
    }
    else
    {
        result_en = MEMIF_JOB_FAILED;
    }
    return(result_en);
}

/**
 * \brief   Returns the migration result of data reading during initialisation
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   Blocknumber         Block number as generated in Fee_Cfg.h
 *
 * \retval  MEMIF_RB_MIGRATION_RESULT_INIT_E               Initialization value, result not yet available
 * \retval  MEMIF_RB_MIGRATION_RESULT_NOT_NECESSARY_E      RAM size == EEPROM size (migration not necessary)
 * \retval  MEMIF_RB_MIGRATION_RESULT_TO_SMALLER_SIZE_E    RAM size < EEPROM size (remaining data discarded)
 * \retval  MEMIF_RB_MIGRATION_RESULT_TO_BIGGER_SIZE_E     RAM size > EEPROM size (remaining data filled with zero)
 * \retval  MEMIF_RB_MIGRATION_RESULT_NOT_DONE_E           Migration not done, because data were not read
 * \retval  MEMIF_RB_MIGRATION_RESULT_DEACTIVATED_E        Migrations is deactivated by configuration
 */
MemIf_Rb_MigrationResult_ten Fee_Rb_Idx_GetMigrationResult(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber)
{
    MemIf_Rb_MigrationResult_ten result_en;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_GETMIGRATIONRESULT))
    {
        result_en = MEMIF_RB_MIGRATION_RESULT_INIT_E;
    }
    if(E_OK != Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_GETMIGRATIONRESULT))
    {
        result_en = MEMIF_RB_MIGRATION_RESULT_INIT_E;
    }
    else if(E_OK != Fee_Prv_OrderDetCheckBlkCfg(FeeDeviceName_en, FEE_SID_RB_GETMIGRATIONRESULT, Blocknumber))
    {
        result_en = MEMIF_RB_MIGRATION_RESULT_INIT_E;
    }
    else
    {
        result_en = Fee_Prv_JobGetMigrationResult(Blocknumber);
    }

    return(result_en);
}

# if((STD_ON == FEE_SET_MODE_SUPPORTED) || (defined(FEE_RB_SET_MODE_API_FS1) && (STD_ON == FEE_RB_SET_MODE_API_FS1)))
/**
 * \brief   Set the mode of the Fee and the underlying flash driver.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   Mode                New mode
 */
void Fee_Rb_Idx_SetMode(Fee_Rb_DeviceName_ten FeeDeviceName_en, MemIf_ModeType Mode)
{
    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_SETMODE))
    {
#  if(defined(FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE == FEE_CFG_FEE_MULTIINSTANCE_ENABLED)) && \
        (defined(FEE_RB_SET_MODE_API_FS1) && (STD_ON == FEE_RB_SET_MODE_API_FS1))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            Fee_SetMode(Mode);
        }
        else
#  endif
#  if(STD_ON == FEE_SET_MODE_SUPPORTED)
        {
            if(E_OK == Fee_Prv_OrderDetCheckModuleIdle(FeeDeviceName_en, FEE_SID_SETMODE))
            {
#   if(FEE_AR_RELEASE_MAJOR_VERSION == 4) && (FEE_AR_RELEASE_MINOR_VERSION >= 4)
                Fee_Prv_ProcSetDriverModeReq(Mode);
#   else
                /* Fee is still idle, so set the driver mode */
                Fee_Prv_ProcSetDriverMode(FeeDeviceName_en, Mode);
#   endif
            }
        }
#  endif
    }
}
# endif

# if((STD_ON == FEE_VERSION_INFO_API) || (defined(FEE_RB_VERSION_INFO_API_FS1) && (STD_ON == FEE_RB_VERSION_INFO_API_FS1)))
/**
 * \brief   Get version information about this Fee.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   VersionInfoPtr      Pointer to a standard version struct that shall be filled
 */
void Fee_Rb_Idx_GetVersionInfo(Fee_Rb_DeviceName_ten FeeDeviceName_en, Std_VersionInfoType * VersionInfoPtr)
{
    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_GETVERSIONINFO))
    {
#  if((defined(FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE == FEE_CFG_FEE_MULTIINSTANCE_ENABLED)) && \
        (defined(FEE_RB_VERSION_INFO_API_FS1) && (STD_ON == FEE_RB_VERSION_INFO_API_FS1)))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            Fee_GetVersionInfo(VersionInfoPtr);
        }
        else
#  endif
        {
#  if(STD_ON == FEE_VERSION_INFO_API)
            /* MR12 DIR 1.1 VIOLATION: uint8 * can always be converted safely to void * */
            if(E_OK != Fee_Prv_OrderDetCheckAdrPtr(FeeDeviceName_en, FEE_SID_GETVERSIONINFO, (void const *)VersionInfoPtr))
            {
                /* Do nothing */
            }
            else
            {
                VersionInfoPtr->vendorID            = FEE_VENDOR_ID;
                VersionInfoPtr->moduleID            = FEE_MODULE_ID;
                VersionInfoPtr->sw_major_version    = FEE_SW_MAJOR_VERSION;
                VersionInfoPtr->sw_minor_version    = FEE_SW_MINOR_VERSION;
                VersionInfoPtr->sw_patch_version    = FEE_SW_PATCH_VERSION;
            }
#  endif
        }
    }
}
# endif

# if((FEE_RB_MAINTAIN != FALSE) || (defined(FEE_RB_MAINTAIN_FS1) && (FEE_RB_MAINTAIN_FS1 != FALSE)))
/**
 * \brief   Make sure that the double storage of a specified block number is intact.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   Blocknumber         The block number as defined in Fee_Cfg.h - This is not the persistent ID!
 *
 * \attention This API is currently not supported by all Fee file systems
 *            For an unsupported job, the job result will be set to failed during the job processing.
 */
Std_ReturnType Fee_Rb_Idx_BlockMaintenance(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber)
{
    Std_ReturnType result_en = E_OK;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_MAINTAIN))
    {
#  if((defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED)) && \
        (defined(FEE_RB_MAINTAIN_FS1) && (FEE_RB_MAINTAIN_FS1 != FALSE)))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            // This request is for device index = 0, this means route this request to Fee1.0
            result_en = Fee_Rb_BlockMaintenance(Blocknumber);
        }
        else
#  endif
        {
#  if(FEE_RB_MAINTAIN != FALSE)
            if(E_OK != Fee_Prv_OrderDetCheckModuleInit (FeeDeviceName_en, FEE_SID_RB_MAINTAIN)             ) { result_en = E_NOT_OK; }
            if(E_OK != Fee_Prv_OrderDetCheckBlkCfg     (FeeDeviceName_en, FEE_SID_RB_MAINTAIN, Blocknumber)) { result_en = E_NOT_OK; }

            if(E_OK == result_en)
            {
                Fee_Prv_JobDesc_tst jobDesc_st;

                /*
                 * Prepare order and try to place it.
                 * If placing the order is not possible E_NOT_OK is returned by Fee_Prv_JobPut.
                 */
                jobDesc_st.type_en            = FEE_PRV_JOBTYPE_BLOCKMAINTENANCE_E;
                jobDesc_st.bfr_pu8            = NULL_PTR;
                jobDesc_st.bfr_pcu8           = NULL_PTR;
                jobDesc_st.blockNumber_u16    = Blocknumber;
                jobDesc_st.length_u16         = Fee_Prv_ConfigGetBlockLengthByBlockNr(Blocknumber);
                jobDesc_st.offset_u16         = 0;

                result_en = Fee_Prv_JobPut(FeeDeviceName_en, FEE_SID_RB_MAINTAIN, &jobDesc_st);
            }
#  endif
        }
    }
    else
    {
        result_en = E_NOT_OK;
    }

    return(result_en);
}
# endif

/**
 * \brief The function when called disables execution of background operations.
 *
 * Note: It does neither suspend nor stop the already ongoing background task but prevents further background operations
 * getting scheduled, i.e.:
 * If a sector reorganization operation has started and one job is currently being transferred,
 *    then the current block operation will be completed and next block will not be transferred.
 * If an erase is ongoing, then it will wait for the erase to
 *    be completed and next background operation will not be started.
 *
 * Background tasks could be resumed again by calling the API Fee_Rb_EnableBackgroundOperation()
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 */
void Fee_Rb_Idx_DisableBackgroundOperation(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_DISABLE_BG))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            Fee_Rb_DisableBackgroundOperation();
        }
        else
#  endif
        {
            if(E_OK == Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_DISABLE_BG))
            {
                Fee_Prv_ProcDisableBackground();
            }
        }
    }
}

/**
 * \brief The function when called enables execution of background operations.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 */
void Fee_Rb_Idx_EnableBackgroundOperation(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_ENABLE_BG))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            Fee_Rb_EnableBackgroundOperation();
        }
        else
#  endif
        {
            if(E_OK == Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_ENABLE_BG))
            {
                Fee_Prv_ProcEnableBackground();
            }
        }
    }
}

# if((FEE_RB_TRIGGER_REORG != FALSE) || (defined(FEE_RB_TRIGGER_REORG_FS1) && (FEE_RB_TRIGGER_REORG_FS1 != FALSE)))
/**
 * \brief The function when called triggers one forced reorganisation.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 *
 * \return Job accepted?
 * \retval E_OK  job is accepted
 * \retval E_NOT_OK job is rejected
 */
Std_ReturnType Fee_Rb_Idx_TriggerReorg(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    Std_ReturnType result_en = E_NOT_OK;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_TRIGGER_REORG))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED)) && \
        (defined(FEE_RB_TRIGGER_REORG_FS1) && (FEE_RB_TRIGGER_REORG_FS1 != FALSE))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            // This request is for device index = 0, this means route this request to Fee1.0
            result_en = Fee_Rb_TriggerReorg();
        }
        else
#  endif
        {
#  if(FEE_RB_TRIGGER_REORG != FALSE)
            result_en = Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_TRIGGER_REORG);

            if(E_OK == result_en)
            {
                Fee_Prv_JobDesc_tst jobDesc_st;

                /*
                 * Prepare order and try to place it.
                 * If placing the order is not possible E_NOT_OK is returned by Fee_Prv_JobPut.
                 */
                jobDesc_st.type_en            = FEE_PRV_JOBTYPE_TRIGGERREORG_E;
                jobDesc_st.bfr_pu8            = NULL_PTR;
                jobDesc_st.bfr_pcu8           = NULL_PTR;
                jobDesc_st.blockNumber_u16    = FEE_NUM_BLOCKS;
                jobDesc_st.length_u16         = 0u;
                jobDesc_st.offset_u16         = 0u;

                result_en = Fee_Prv_JobPut(FeeDeviceName_en, FEE_SID_RB_TRIGGER_REORG, &jobDesc_st);
            }
#  endif
        }
    }

    return(result_en);
}
# endif

# if((FEE_RB_ENTER_STOP_MODE != FALSE) || (defined(FEE_RB_ENTER_STOP_MODE_FS1) && (FEE_RB_ENTER_STOP_MODE_FS1 != FALSE)))
/**
 * \brief The function puts the request for putting the Fee in stop mode at next possible synchronization point.
 * The ongoing job gets completed and only after that Fee would be put to stop mode and no jobs would be executed in
 * this drive cycle.
 *
 * \attention As soon as the Fee enters the stop mode, it cannot be left without reiniting the Fee.
 * The stop mode is able to (and supposed to be used to) alter the Fee emulation area in the flash.
 * Thus the Fee needs to analyse the DFLASH state again.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 */
Std_ReturnType Fee_Rb_Idx_EnterStopMode(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    Std_ReturnType result_en = E_NOT_OK;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_ENTER_STOP_MODE))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED)) && \
        (defined(FEE_RB_ENTER_STOP_MODE_FS1) && (FEE_RB_ENTER_STOP_MODE_FS1 != FALSE))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            result_en = Fee_Rb_EnterStopMode();
        }
        else
#  endif
        {
#  if(FEE_RB_ENTER_STOP_MODE != FALSE)
            result_en = Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_ENTER_STOP_MODE);

            if(E_OK == result_en)
            {
                Fee_Prv_JobDesc_tst jobDesc_st;

                /*
                 * Prepare order and try to place it.
                 * If placing the order is not possible E_NOT_OK is returned by Fee_Prv_JobPut.
                 */
                jobDesc_st.type_en            = FEE_PRV_JOBTYPE_ENTER_STOP_MODE_E;
                jobDesc_st.bfr_pu8            = NULL_PTR;
                jobDesc_st.bfr_pcu8           = NULL_PTR;
                jobDesc_st.blockNumber_u16    = FEE_NUM_BLOCKS;
                jobDesc_st.length_u16         = 0u;
                jobDesc_st.offset_u16         = 0u;

                result_en = Fee_Prv_JobPut(FeeDeviceName_en, FEE_SID_RB_ENTER_STOP_MODE, &jobDesc_st);
            }
#  endif
        }
    }

    return(result_en);
}
# endif

/**
 * \brief   Get the maximum sector change counter. If the Fee is not initialized, the function returns 0.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 *
 * \return  Maximum sector change counter
 */
uint32 Fee_Rb_Idx_GetSectChngCnt(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    uint32 cntrSectChanges_u32;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_GET_SECT_CHNG_CNT))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            cntrSectChanges_u32 = Fee_Rb_GetSectChngCnt();
        }
        else
#  endif
        {
            if(E_OK == Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_GET_SECT_CHNG_CNT))
            {
                cntrSectChanges_u32 = Fee_Prv_FsIfRbGetSectorChgCntr();
            }
            else
            {
                cntrSectChanges_u32 = 0u;
            }
        }
    }
    else
    {
        cntrSectChanges_u32 = 0u;
    }

    return(cntrSectChanges_u32);
}

/**
 * \brief   Get the amount of free space in DFLASH. If the Fee is not initialized, the function returns 0.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 *
 * \return  Amount of free space in bytes
 */
# if((FEE_RB_GET_NR_FREE_BYTES != FALSE) || (defined(FEE_RB_GET_NR_FREE_BYTES_FS1) && (FEE_RB_GET_NR_FREE_BYTES_FS1 != FALSE)))
uint32 Fee_Rb_Idx_GetNrFreeBytes(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    uint32 nrFreeBytes_u32 = 0u;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_GET_NR_FREE_BYTES))
    {
#  if((defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED)) && \
        (defined(FEE_RB_GET_NR_FREE_BYTES_FS1) && (FEE_RB_GET_NR_FREE_BYTES_FS1 != FALSE)))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            nrFreeBytes_u32 = Fee_Rb_GetNrFreeBytes();
        }
        else
#  endif
        {
#  if(FEE_RB_GET_NR_FREE_BYTES != FALSE)
            if(E_OK == Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_GET_NR_FREE_BYTES))
            {
                nrFreeBytes_u32 = Fee_Prv_FsIfRbGetFreeSpace();
            }
#  endif
        }
    }

    return(nrFreeBytes_u32);
}
# endif

/**
 * \brief   Get the amount of free user data space and number of free control data space (FAT entries) in DFLASH before a hard reorganization will be started.
 *          If erase operations are not accepted by the user, free non erased sectors will not be considered for user data space evaluation.
 *          If the Fee is not initialized, the function returns 0.
 *
 * \param   FeeDeviceName_en       Device instance for which job has to be performed
 * \param   nrFreeBytes_pu32       The pointer to the address, where the number of free data bytes information shall be written.
 * \param   nrFreeFatEntries_pu32  The pointer to the address, where the number of free FAT entries information shall be written.
 * \param   isEraseAccepted_b      Information how to handle free non erased sectors.
 *                                 TRUE:  Free non erased sectors shall be considered, erase operations may occur when writing the given number of bytes
 *                                 FALSE: Free non erased sectors shall not be considered, no erase operations when writing the given number of bytes
 */
# if(FEE_RB_GET_NR_FREE_BYTES_AND_FAT_ENTRIES != FALSE)
void Fee_Rb_Idx_GetNrFreeBytesAndFatEntries(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint32 * nrFreeBytes_pu32, uint32 * nrFreeFatEntries_pu32, boolean isEraseAccepted_b)
{
    // Check parameter
    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_GET_NR_FREE_BYTES_AND_FAT_ENTRIES))
    {
        if(E_OK == Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_GET_NR_FREE_BYTES_AND_FAT_ENTRIES))
        {
            Fee_Prv_FsIfRbGetNrFreeBytesAndFatEntries(nrFreeBytes_pu32, nrFreeFatEntries_pu32, isEraseAccepted_b);
        }
        else
        {
            *nrFreeBytes_pu32      = 0u;
            *nrFreeFatEntries_pu32 = 0u;
        }
    }
    else
    {
        *nrFreeBytes_pu32      = 0u;
        *nrFreeFatEntries_pu32 = 0u;
    }
}
# endif

/*
 * \brief   Returns the current Fee working state.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 */
Fee_Rb_WorkingStateType_ten Fee_Rb_Idx_GetWorkingState(Fee_Rb_DeviceName_ten FeeDeviceName_en)
{
    Fee_Rb_WorkingStateType_ten stRetVal_en = FEE_RB_IDLE_E;

    if(E_OK == Fee_Prv_OrderDetCheckDeviceName(FeeDeviceName_en, FEE_SID_RB_GET_WORKING_STATE))
    {
#  if(defined( FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE ==  FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
        if(Fee_Rb_DeviceName == FeeDeviceName_en)
        {
            stRetVal_en = Fee_Rb_GetWorkingState();
        }
        else
#  endif
        {
            if(E_OK == Fee_Prv_OrderDetCheckModuleInit(FeeDeviceName_en, FEE_SID_RB_GET_WORKING_STATE))
            {
                stRetVal_en = Fee_Prv_ProcGetWorkingState();
            }
            else
            {
                stRetVal_en = FEE_RB_IDLE_E;
            }
        }
    }
    return(stRetVal_en);
}

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

#endif
