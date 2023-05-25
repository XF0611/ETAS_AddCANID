


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

#if((defined(FEE_PRV_CFG_SELECTED_FS) && (1 == FEE_PRV_CFG_SELECTED_FS)) || (defined(FEE_CFG_FEE1_ENABLED) && (TRUE == FEE_CFG_FEE1_ENABLED)))

#if (FEE_PRESENT == 1)
/**
 *********************************************************************
 * Fee_EraseImmediateBlock(): Service to erase a logical block
 *
 * The functionality of this interface is not supported. This is a
 * dummy function to ensure compatability with the MemIf and Ea.
 *
 * \param    BlockNumber:   Number of logical block(persistent ID)
 * \return   Function success
 * \retval   E_OK:          Order was successfully placed
 * \retval   E_NOT_OK:      Order was not placed because the
 *                          respective order queue entry is
 *                          not empty or the module has not been
 *                          initialized yet.
 * \seealso
 * \usedresources
 *********************************************************************
 */
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
Std_ReturnType Fee_EraseImmediateBlock(uint16 BlockNumber)
{
    Std_ReturnType xRetVal = E_NOT_OK;      /* Default return value */

    (void)BlockNumber;

    return (xRetVal);
}
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
/* FEE_PRESENT == 1 */
#endif
/* 1 == FEE_PRV_CFG_SELECTED_FS */
#endif

