/******************************************************************************
**                                                                           **
*******************************************************************************
**                                                                           **
**  $FILENAME   : BSLM.h $                                                   **
**                                                                           **
**  $CC VERSION : \main\6 $                                                  **
**                                                                           **
**  $DATE       : 2019-08-07 $                                               **
**                                                                           **
**  AUTHOR      : RBVH-AUTOSAR-Engineering                                   **
**                                                                           **
**                                                                           **
**  DESCRIPTION  : Bootloader state machine File                             **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: yes                                     **
**                                                                           **
******************************************************************************/

#ifndef     BLSM_H
#define     BLSM_H

/*******************************************************************************
**                      Includes
*******************************************************************************/
#include "BLSMCfg.h"
#include "NvM.h"
#include "Os.h"
/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/

#define Fbl_BLSM_Init           OS_MAIN()
#define BLSM_MainFunction   Fbl_BLSM_MainFunction


/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
typedef enum
{
  FBL_BLSM_CMPSTATE_INIT,
  FBL_BLSM_CMPSTATE_STARTUP,
  FBL_BLSM_CMPSTATE_RUN
}Fbl_BLSM_CmpStateType;
/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/

/**
 * @brief the BLSM_MainFunction manage the state transaction of FBL system. 
 * @return None.
 */
extern void Fbl_BLSM_MainFunction(void);

/**
 * @brief a Callback function of the NVM which is called whenever Fbl_DataM_MultiBlockJobEndNotification
 * is notified.
 *
 * @param ServiceId Service ID of the NvM
 * @param JobResult Job result
 * @return None
 */
extern void Fbl_BLSM_NfyDataMJobEnd (uint8 ServiceId, NvM_RequestResultType JobResult);

#ifdef STAY_IN_BOOT_SUPPORTED

/**
 * @brief notify to BLSM whenever Stay-In-Boot Frame was received!
 * @return none
 */
extern void Fbl_Port_BLSM_StayInBootNotification(void);
#endif
/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/
#endif

