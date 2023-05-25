

#include "MemIf.h"
#include "MemIf_Cfg_SchM.h"

#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
#include "Det.h"
#endif

#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"

#if (defined(MEMIF_FEE_AND_EA_USED) && (MEMIF_FEE_AND_EA_USED == STD_ON))

/**
 *********************************************************************
 * MemIf_Cancel(): MemIf_Cancel interface
 *
 * This function cancels an order.
 *
 * \param    DeviceIndex: Index of requeste device
 * \return   none
 * \retval   none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void MemIf_Cancel(uint8 DeviceIndex)
{
#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
#endif
# if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        Fee_Rb_Idx_Cancel(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex));
    }
#else
    if (DeviceIndex == 0)
    {
        Fee_Cancel();
    }
# endif
    else
    {
        Ea_Cancel();
    }
}

/**
 *********************************************************************
 * MemIf_Write(): MemIf_Write interface
 *
 * This function places a write job to the FEE or EA.
 *
 * \param    DeviceIndex:   Index of requeste device
 * \param    Blocknumber:   Nvm index (based on this index the
 *                          corresponding Fee index will be searched
 *                          and placed into the order queue)
 * \param    DataBufferPtr: Pointer to data buffer
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
Std_ReturnType MemIf_Write(uint8 DeviceIndex, uint16 BlockNumber, uint8* DataBufferPtr)
{
    Std_ReturnType xRetVal = E_NOT_OK;       /* Default return value */

#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
#endif

# if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_Write(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex), BlockNumber, DataBufferPtr);
    }
#else
    if (DeviceIndex == 0)
    {
        xRetVal = Fee_Write(BlockNumber,DataBufferPtr);
    }
#endif
    else
    {
        xRetVal = Ea_Write(BlockNumber,DataBufferPtr);
    }
    return(xRetVal);
}

/**
 *********************************************************************
 * MemIf_Read(): MemIf_Read interface
 *
 * This function places a read job to the FEE or EA.
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
Std_ReturnType MemIf_Read(uint8 DeviceIndex, uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length)
{
    Std_ReturnType xRetVal = E_NOT_OK;       /* Default return value */

    #if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
    #endif

# if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_Read(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex),BlockNumber,BlockOffset,DataBufferPtr,Length);
    }
#else
    if (DeviceIndex == 0)
    {
        xRetVal = Fee_Read(BlockNumber,BlockOffset,DataBufferPtr,Length);
    }
#endif
    else
    {
        xRetVal = Ea_Read(BlockNumber,BlockOffset,DataBufferPtr,Length);
    }
    return(xRetVal);
}

/**
 *********************************************************************
 * MemIf_InvalidateBlock(): MemIf_InvalidateBlock interface
 *
 * This function places a read job to the FEE or EA.
 *
 * \param    DeviceIndex:   Index of requeste device
 * \param    Blocknumber:   Nvm index (based on this index the
 *                          corresponding Fee index will be searched
 *                          and placed into the order queue)
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
Std_ReturnType MemIf_InvalidateBlock(uint8 DeviceIndex, uint16 BlockNumber)
{
    Std_ReturnType xRetVal = E_NOT_OK;       /* Default return value */

    #if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
    #endif

# if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_InvalidateBlock(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex),BlockNumber);
    }
#else
    if (DeviceIndex == 0)
    {
        xRetVal = Fee_InvalidateBlock(BlockNumber);
    }
#endif
    else
    {
        xRetVal = Ea_InvalidateBlock(BlockNumber);
    }
    return(xRetVal);
}

/**
 *********************************************************************
 * MemIf_GetStatus(): MemIf_GetStatus interface
 *
 * This function returns the status of a a device.
 *
 * \param    DeviceIndex: Index of requeste device
 * \return   none
 * \retval   none
 * \seealso
 * \usedresources
 *********************************************************************
 */
MemIf_StatusType MemIf_GetStatus(uint8 DeviceIndex)
{
    MemIf_StatusType xRetVal = MEMIF_IDLE;       /* Default return value */

    #if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
    #endif

# if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_GetStatus(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex));
    }
#else
    if (DeviceIndex == 0)
    {
        xRetVal = Fee_GetStatus();
    }
#endif
    else
    {
        xRetVal = Ea_GetStatus();
    }
    return(xRetVal);
}

/**
 *********************************************************************
 * MemIf_GetJobResult(): MemIf_GetJobResult interface
 *
 * This function returns the status of a a job.
 *
 * \param    DeviceIndex: Index of requeste device
 * \return   none
 * \retval   none
 * \seealso
 * \usedresources
 *********************************************************************
 */
MemIf_JobResultType MemIf_GetJobResult(uint8 DeviceIndex)
{
    MemIf_JobResultType xRetVal = MEMIF_JOB_FAILED;       /* Default return value */

    #if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
    #endif

# if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_GetJobResult(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex));
    }
#else
    if (DeviceIndex == 0)
    {
        xRetVal = Fee_GetJobResult();
    }
#endif
    else
    {
        xRetVal = Ea_GetJobResult();
    }
    return(xRetVal);
}

/**
 *********************************************************************
 * MemIf_EraseImmediateBlock(): MemIf_EraseImmediateBlock interface
 *
 * This function places a read job to the FEE or EA.
 *
 * \param    DeviceIndex:   Index of requeste device
 * \param    Blocknumber:   Nvm index (based on this index the
 *                          corresponding Fee index will be searched
 *                          and placed into the order queue)
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
Std_ReturnType MemIf_EraseImmediateBlock(uint8 DeviceIndex, uint16 BlockNumber)
{
    Std_ReturnType xRetVal = E_NOT_OK;       /* Default return value */

    #if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
    MemIf_CheckDeviceIndex(DeviceIndex);
    #endif

# if (defined (MEMIF_RB_NUM_OF_FEE_DEVICES) && (MEMIF_RB_NUM_OF_FEE_DEVICES > 1u))
    if (Fee_Rb_IsThisDeviceIndexFee(DeviceIndex) == TRUE)
    {
        xRetVal = Fee_Rb_Idx_EraseImmediateBlock(Fee_Rb_GetDeviceNameFromDeviceIndex(DeviceIndex),BlockNumber);
    }
#else
    if (DeviceIndex == 0)
    {
        xRetVal = Fee_EraseImmediateBlock(BlockNumber);
    }
#endif
    else
    {
        xRetVal = Ea_EraseImmediateBlock(BlockNumber);
    }
    return(xRetVal);
}

#if (defined(MEMIF_DEV_ERROR_DETECT) && (MEMIF_DEV_ERROR_DETECT == STD_ON))
/**
 *********************************************************************
 * MemIf_CheckDeviceIndex(): MemIf_CheckDeviceIndex interface
 *
 * This function checks the device index.
 *
 * \param    DeviceIndex: Index of requeste device
 * \return   none
 * \retval   none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void MemIf_CheckDeviceIndex(uint8 DeviceIndex)
{
    if (DeviceIndex >= MEMIF_MAX_NUM_DEVICES)
    {
        (void)Det_ReportError(MEMIF_MODULE_ID, MEMIF_INSTANCE_ID, MEMIF_SID_CHECK_INDEX, MEMIF_E_PARAM_DEVICE);
    }

}
#endif
#endif

#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"

