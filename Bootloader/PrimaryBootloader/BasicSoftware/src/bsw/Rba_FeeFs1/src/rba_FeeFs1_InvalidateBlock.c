


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
 * Fee_InvalidateBlock(): Place an invalidation order
 *
 * This function places an invalidation order to the FEE order queue.
 * If Development Error Detection is enabled, the function checks
 * the FEE module status additionaly.
 *
 * \param    Blocknumber:   Auto generated Fee block number which
 *                          corresponds to the respective persistent ID
 *
 * \return   Function success
 * \retval   E_OK:              Order was successfully placed
 * \retval   E_NOT_OK:          Order was not placed because the
 *                              respective order queue entry is
 *                              not empty
 * \seealso
 * \usedresources
 *********************************************************************
 */
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
Std_ReturnType Fee_InvalidateBlock(uint16 Blocknumber)
{
    Std_ReturnType xRetVal = E_NOT_OK;          /* Default return value */
    Std_ReturnType retModuleState_u8 , retBlockCfg_u8;
    /* Check the FEE module status and the user's block number */
    /* MR12 RULE 13.5 VIOLATION: Same error must be reported either if module is not idle or incorrect id is passed. */

    retModuleState_u8 = Fee_CheckModuleSt(FEE_SID_INVALIDATE);
    retBlockCfg_u8    = Fee_CheckBlockCfg(FEE_SID_INVALIDATE, Blocknumber);
    if((retModuleState_u8 != E_OK) ||
       (retBlockCfg_u8 != E_OK))
    {
        /* Return with an error */
        return (E_NOT_OK);
    }

    /* Hint: if the queue entry is not empty, E_NOT_OK will be returned independent from the DET setting */
    xRetVal = Fee_HLPlaceOrder(Blocknumber, 0, NULL_PTR, 0, FEE_INVALIDATE_ORDER);

    return (xRetVal);
}
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
/* FEE_PRESENT */
#endif
/* 1 == FEE_PRV_CFG_SELECTED_FS */
#endif

