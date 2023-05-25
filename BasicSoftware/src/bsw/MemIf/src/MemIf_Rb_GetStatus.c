

#include "MemIf.h"
#include "MemIf_Cfg_SchM.h"

#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"

/**
 *********************************************************************
 * MemIf_Rb_GetStatus(): Return module status of MemIf
 *
 * The function returns the status of underlaying
 * FEE and of EA module, depending if Fee and EA are
 * configured.
 *
 * \param
 * MEMIF_UNINIT        :  MemIf (Fee/EA) is uninitialized
 * MEMIF_IDLE          :  MemIf (Fee/EA) is currently idle
 * MEMIF_BUSY          :  Memif (Fee/EA) is currently busy in executing order
 * MEMIF_BUSY_INTERNAL :  Memif (Fee/EA) is currently busy with an internal
 *                        management operation.
 * \retval
 * Status of MemIf and underlaying main functions.
 * \seealso
 * \usedresources
 *********************************************************************
 */
MemIf_StatusType MemIf_Rb_GetStatus(void)
{
    /* Local variables*/
    MemIf_StatusType status  = MEMIF_IDLE;
    MemIf_StatusType xRetVal = MEMIF_IDLE;

    /* Avoid unused variable compiler warning if neither Fee nor Ea are used */
    (void)status;

    /* Only if Fee is used */
    #if (defined(MEMIF_FEE_USED) && (MEMIF_FEE_USED == STD_ON))
    status = Fee_GetStatus();
    if ((status == MEMIF_BUSY) || (status == MEMIF_BUSY_INTERNAL))
    {
        xRetVal = MEMIF_BUSY;
    }
    #endif

    /* Only if EA is used */
    #if (defined(MEMIF_NUM_OF_EA_DEVICES) && (MEMIF_NUM_OF_EA_DEVICES > 0))
    status = Ea_GetStatus();
    if ((status == MEMIF_BUSY) || (status == MEMIF_BUSY_INTERNAL))
    {
        xRetVal = MEMIF_BUSY;
    }
    #endif

    /* Return result */
    return (xRetVal);
}

#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"

