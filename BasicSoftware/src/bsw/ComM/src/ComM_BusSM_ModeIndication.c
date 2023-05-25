

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "ComM_Priv.h"
#include "EcuM.h"
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */
/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/* ComM_BusSM_ModeIndication */
/*******************************************************************************
 * Function name    : ComM_BusSM_ModeIndication
 * Syntax           : void ComM_BusSM_ModeIndication
 *                    (
 *                      NetworkHandleType Channel, ComM_ModeType* ComMode
 *                      )
 * Description      : Indication of the actual bus mode by the corresponding Bus State Manager.
 * Parameters       : Channel
 *                    ComMode
 * Return value     : void
 ******************************************************************************/
/* MR12 RULE 8.13 VIOLATION:Violated since SWS gives function definition in the given way */
#if (COMM_AR45_VERSION_USED != STD_OFF)
void ComM_BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode)
#else
void ComM_BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType * ComMode)
#endif
{
    /* Local variable initialization */
    /**********************************************DET CHECKS STARTS HERE****(void)**********************/
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    /* DET check for communication manager initialization */
    /* Check if ComManager is initialized */
    if (ComM_GlobalStruct.ComM_InitStatus_en != COMM_INIT)
    {
        /* Report DET with ComM not initialized*/
        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_BUSSM_MODEINDICATION,COMM_E_NOT_INITED);
        return;
    }
    #if (COMM_AR45_VERSION_USED != STD_ON)
    /* DET check for pointer passed to be a NULL pointer */
    if (ComMode == NULL_PTR)
    {
        /* Report DET with NULL parameter*/
        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_BUSSM_MODEINDICATION,COMM_E_WRONG_PARAMETERS);
        return;
    }
    #endif
    /* Check if the channel is present in the configured channels */
    if (Channel >= COMM_NO_OF_CHANNELS)
    {
        /* Report DET with Invalid channel parameter*/

        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_BUSSM_MODEINDICATION,COMM_E_WRONG_PARAMETERS);
        return;
    }
    #endif  /*  #if (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    /*****************************************DET ENDS HERE*************************************************/
    (void) Channel;
    (void) ComMode;
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

