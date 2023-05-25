


/* Check for availability of Rte */
#include "rba_MemLib_UseRte.h"

/* If Rte is available then hide internal declarations */
#if defined(RBA_MEMLIB_USE_RTE)
#define MEMIF_RB_HIDE_RTEAPI
#endif

/**************************************************/

#include "MemIf.h"
#include "MemIf_Cfg_SchM.h"

#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
#include "Det.h"
#endif /* MEMIF_DEV_ERROR_DETECT */


/* Special Rte includes if Rte is available (as they are hidden in header) */
#if defined(MEMIF_RB_HIDE_RTEAPI)
#include "SchM_MemIf.h"
#endif


/*
 *********************************************************************
     Macros
 *********************************************************************
 */


/* Protection selectable via MemIf_Cfg_SchM.h to save code */
#ifndef MEMIF_RB_USE_PROTECTION
#define MEMIF_RB_USE_PROTECTION   (TRUE)
#endif


/*
 *********************************************************************
     Variables
 *********************************************************************
 */


#define MEMIF_START_SEC_VAR_INIT_BOOLEAN
#include "MemIf_MemMap.h"

/* Flag to hold the main function access status */
static volatile boolean MemIf_Prv_flgUsedSema_b = FALSE;

/* Flag to hold the debug information, Reentrant MainFunction */
static volatile boolean MemIf_Prv_dbgReentrantMainFunction_b = FALSE;

#define MEMIF_STOP_SEC_VAR_INIT_BOOLEAN
#include "MemIf_MemMap.h"


/*
 *********************************************************************
     Code Area
 *********************************************************************
 */


#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"

/**
 *********************************************************************
 * MemIf_Prv_TryToGetLock(): Detection and protection of re-entrant
 * execution of MainFunction
 *
 * The function check the global flag for re-entrant invokation of
 * Mainfunction and reports if Det enabled
 *
 * \param    none
 * \retval   TRUE : Lock acquired or protection disabled
 * \retval   FALSE: Re-entrant invocation of MainFunction
 * \seealso
 * \usedresources
 *********************************************************************
 */
LOCAL_INLINE boolean MemIf_Prv_TryToGetLock(void)
{
    boolean isLockAvailable_b = FALSE;

#if(MEMIF_RB_USE_PROTECTION != FALSE)
    /* Disable interrupts */
    SchM_Enter_MemIf_Main();
#endif

    /* Is the Main function not invoked yet, then reserve it and allow the execution */
    if(!MemIf_Prv_flgUsedSema_b)
    {
        MemIf_Prv_flgUsedSema_b = TRUE;
        isLockAvailable_b = TRUE;
    }

#if(MEMIF_RB_USE_PROTECTION != FALSE)
    /* Enable interrupts */
    SchM_Exit_MemIf_Main();
#endif

    if(!isLockAvailable_b)
    {
        /* Set the debug flag for the detection of reentrant main function  */
        MemIf_Prv_dbgReentrantMainFunction_b = TRUE;

#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
        /* Multiple invokation of MemIf_MainFunction, report error if det enabled */
        (void)Det_ReportError(MEMIF_MODULE_ID, MEMIF_INSTANCE_ID, MEMIF_RB_SID_MAIN_FUNCTION, MEMIF_RB_E_BUSY);
#endif
    }

#if(MEMIF_RB_USE_PROTECTION == FALSE)
    /* Protection disabled, only detection by default */
    isLockAvailable_b = TRUE;
#endif

    return isLockAvailable_b;
}

/**
 *********************************************************************
 * MemIf_Prv_ReleaseLock(): Reset the flag having reentrant information
 *
 * \param    none
 * \retval   none
 * \seealso
 * \usedresources
 *********************************************************************
 */
LOCAL_INLINE void MemIf_Prv_ReleaseLock(void)
{
    /* Reset the access flag */
    MemIf_Prv_flgUsedSema_b = FALSE;
    return;
}

/**
 *********************************************************************
 * MemIf_Rb_MainFunction(): Calls requiered underlaying
 *                          Mainfunctions.
 *
 * The function calls the required underlaying main functions,
 * depending on which modules are configured.
 *
 * \param    none
 * \retval   none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void MemIf_Rb_MainFunction(void)
{
    /* Local flag to store the lock accuring status */
    boolean flgUsed_b = FALSE;

    /* Check for re-entrant invocation of MainFunction*/
    flgUsed_b = MemIf_Prv_TryToGetLock();

    if(flgUsed_b)
    {

#if (defined(MEMIF_FEE_USED) && (MEMIF_FEE_USED == STD_ON))
        Fee_MainFunction();
        Fls_MainFunction();
#endif
#if (defined(MEMIF_NUM_OF_EA_DEVICES) && (MEMIF_NUM_OF_EA_DEVICES > 0u))
        Ea_MainFunction();
        Eep_MainFunction();
        Spi_MainFunction_Handling();
#endif

        /* Release the lock */
        MemIf_Prv_ReleaseLock();
    }

    return;
}


#if (defined(MEMIF_FEE_AND_EA_USED) && (MEMIF_FEE_AND_EA_USED == STD_ON))
/**
 *********************************************************************
 * MemIf_Rb_BlockMaintenance(): Calls corresponding block maintenance functions.
 *
 * The function calls the required underlying block maintenance functions,
 * depending on which modules are configured.
 *
 * \param    Block number
 * \retval   E_OK:      Order was accepted
 * \retval   E_NOT_OK:  Order was not accepted.
 * \seealso
 * \usedresources
 *********************************************************************
 */
Std_ReturnType MemIf_Rb_BlockMaintenance(uint8 DeviceIndex, uint16 BlockNumber)
{
    Std_ReturnType xRetVal = E_NOT_OK;       /* Default return value */

    #if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
    #endif

# if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_BlockMaintenance(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex),BlockNumber);
    }
#else
    if (DeviceIndex == 0u)
    {
        #if( (!(defined(FEE_RB_MAINTAIN))) || (defined(FEE_RB_MAINTAIN) && (FEE_RB_MAINTAIN != FALSE)) )
        xRetVal = Fee_Rb_BlockMaintenance(BlockNumber);
        #else
        /* Intentionally empty - will return E_NOT_OK */
        #endif
    }
#endif
    else
    {
        xRetVal = Ea_Rb_BlockMaintenance(BlockNumber);
    }
    return(xRetVal);
}
#endif

#if (defined(MEMIF_FEE_AND_EA_USED) && (MEMIF_FEE_AND_EA_USED == STD_ON))
/**
 *********************************************************************
 * MemIf_VarLenRead(): MemIf_VarLenRead interface
 *
 * This function places a read job to the FEE when runtime RAM block
 * configuration feature is enabled and the block length is variable.
 *
 * \param    DeviceIndex:   Index of requeste device
 * \param    Blocknumber:   Nvm index (based on this index the
 *                          corresponding Fee index will be searched
 *                          and placed into the order queue)
 * \param    BlockOffset:   Offset of block
 * \param    DataBufferPtr: Pointer to data buffer
 * \param    Length:        Length of block
 * \return   Function success
 * \retval   E_OK:              Order was successfully placed
 * \retval   E_NOT_OK:          Order was not placed because the
 *                              respective order queue entry is
 *                              not empty or the module has not been
 *                              initialized yet
 * \seealso
 * \usedresources
 *********************************************************************
 */
Std_ReturnType MemIf_Rb_VarLenRead(uint8 DeviceIndex, uint16 BlockNumber, uint16 BlockOffset, uint8 * DataBufferPtr, uint16 Length)
{
    Std_ReturnType xRetVal = E_NOT_OK;       /* Default return value */

#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
#endif

#if (defined(MEMIF_FEE_USED) && (MEMIF_FEE_USED == STD_ON))

#if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_VarLenRead(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex),BlockNumber,BlockOffset,
                                                                                DataBufferPtr,Length);
    }
#else
    if (DeviceIndex == 0u)
    {
        /* Block is configured for Fee (Flash) */
        xRetVal = Fee_Rb_VarLenRead(BlockNumber,BlockOffset,DataBufferPtr,Length);
    }
#endif
#endif

    return(xRetVal);
}
#endif

#if (defined(MEMIF_FEE_AND_EA_USED) && (MEMIF_FEE_AND_EA_USED == STD_ON))
/**
 *********************************************************************
 * MemIf_Rb_VarLenWrite(): MemIf_Rb_VarLenWrite interface
 *
 * This function places a write job to the FEE when runtime RAM block
 * configuration feature is enabled and the block length is variable.
 *
 * \param    DeviceIndex:   Index of requested device
 * \param    Blocknumber:   Nvm index (based on this index the
 *                          corresponding Fee index will be searched
 *                          and placed into the order queue)
 * \param    DataBufferPtr: Pointer Pointer to data buffer
 * \param    Length:        NV block length
 * \return   Function success
 * \retval   E_OK:              Order was successfully placed
 * \retval   E_NOT_OK:          Order was not placed because the
 *                              respective order queue entry is
 *                              not empty or the module has not been
 *                              initialized yet
 * \seealso
 * \usedresources
 *********************************************************************
 */
Std_ReturnType MemIf_Rb_VarLenWrite(uint8 DeviceIndex, uint16 BlockNumber, uint8 * DataBufferPtr, uint16 Length)
{
    Std_ReturnType xRetVal = E_NOT_OK;   /* Default return value */

#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
#endif

#if (defined(MEMIF_FEE_USED) && (MEMIF_FEE_USED == STD_ON))
#if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_VarLenWrite(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex),BlockNumber,
                                                                                DataBufferPtr,Length);
    }
#else
    if (DeviceIndex == 0u)
    {
        /* Block is configured for Fee (Flash) */
        xRetVal = Fee_Rb_VarLenWrite(BlockNumber,DataBufferPtr,Length);
    }
#endif
#endif

    return(xRetVal);
}
#endif

#if (defined(MEMIF_FEE_AND_EA_USED) && (MEMIF_FEE_AND_EA_USED == STD_ON))
/**
 *********************************************************************
 * MemIf_Rb_GetMigrationResult(): MemIf_Rb_GetMigrationResult interface
 *
 * Service returns the migration result of the last data reading
 * during initialisation (calling the API after subsequent read jobs
 * in the same driving cycle does not change the initially returned
 * value anymore).
 *
 * \param    DeviceIndex:   Index of requested device
 * \param    BlockNumber:   Corresponding Fee or Ea block number
 *
 * \retval   MEMIF_RB_MIGRATION_RESULT_INIT_E
 * \retval   MEMIF_RB_MIGRATION_RESULT_NOT_NECESSARY_E
 * \retval   MEMIF_RB_MIGRATION_RESULT_TO_SMALLER_SIZE_E
 * \retval   MEMIF_RB_MIGRATION_RESULT_TO_BIGGER_SIZE_E
 * \retval   MEMIF_RB_MIGRATION_RESULT_NOT_DONE_E
 * \retval   MEMIF_RB_MIGRATION_RESULT_DEACTIVATED_E
 *
 * \seealso
 * \usedresources
 *********************************************************************
 */
MemIf_Rb_MigrationResult_ten MemIf_Rb_GetMigrationResult(uint8 DeviceIndex, uint16 BlockNumber)
{
    MemIf_Rb_MigrationResult_ten xRetVal = MEMIF_RB_MIGRATION_RESULT_INIT_E;  /* Default return value */

#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
#endif

#if (defined(MEMIF_FEE_USED) && (MEMIF_FEE_USED == STD_ON))
#if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_GetMigrationResult(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex),BlockNumber);
    }
#else
    if (DeviceIndex == 0u)
    {
        /* Block is configured for Fee (Flash) */
        xRetVal = Fee_Rb_GetMigrationResult(BlockNumber);
    }
#endif
#endif

    return(xRetVal);
}
#endif

#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"

