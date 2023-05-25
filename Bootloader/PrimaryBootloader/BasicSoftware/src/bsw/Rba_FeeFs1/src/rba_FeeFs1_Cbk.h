


#ifndef  RBA_FEEFS1_CBK_H
#define  RBA_FEEFS1_CBK_H

/**
 **********************************************************************************************************************
 * \moduledescription
 *           AUTOSAR FEE
 *           According to Document Version 3.0.0, Part of Release 4.0, Revision 2
 * \scope    INTERN
 **********************************************************************************************************************
 */


/*
 ****************************************************************************************************
 * Includes
 ****************************************************************************************************
 */

#if((defined(FEE_PRV_CFG_SELECTED_FS) && (1 == FEE_PRV_CFG_SELECTED_FS)) || (defined(FEE_CFG_FEE1_ENABLED) && (TRUE == FEE_CFG_FEE1_ENABLED)))

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
/*
 ****************************************************************************************************
 * Global Function Declarations
 ****************************************************************************************************
 */

/*******************************************************************************
** Syntax : void Fee_JobEndNotification(void)                                 **
**                                                                            **
** Service ID: 16                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : FEE051: This routine shall be called by the underlying flash **
** driver to report the successful end of an asynchronous operation.          **
*******************************************************************************/
#if (FEE_POLLING_MODE == TRUE)
extern void Fee_SwitchJobEndNotification (void);
#endif

/*******************************************************************************
** Syntax : void Fee_JobErrorNotification(void)                               **
**                                                                            **
** Service ID: 17                                                             **
**                                                                            **
** Sync/Async:  Synchronous                                                   **
**                                                                            **
** Reentrancy:  Non reentrant                                                 **
**                                                                            **
** Parameters (in) :  None                                                    **
**                                                                            **
** Parameters (out):  None                                                    **
**                                                                            **
** Return value    :  None                                                    **
**                                                                            **
** Description : FEE053: This routine shall be called by the underlying flash **
** driver to report the failure of an asynchronous operation.                 **
*******************************************************************************/
#if (FEE_POLLING_MODE == TRUE)
extern void Fee_SwitchJobErrorNotification (void);
#endif

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

#endif  /* RBA_FEEFS1_CBK_H */
/* 1 == FEE_PRV_CFG_SELECTED_FS */
#endif

