
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Interface header file for declaration of callback notifications,
 * this is included by the <Bus>Nm modules */
#include "Nm_Cbk.h"
/* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"
/* Included to access ComM APIs related with NM */
#include "ComM.h"
#if(NM_ENABLE_INTER_MODULE_CHECKS)
# if (!defined(COMM_AR_RELEASE_MAJOR_VERSION) || (COMM_AR_RELEASE_MAJOR_VERSION != NM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
# if (!defined(COMM_AR_RELEASE_MINOR_VERSION) || (COMM_AR_RELEASE_MINOR_VERSION != NM_AR_RELEASE_MINOR_VERSION))
#  error "AUTOSAR minor version undefined or mismatched"
# endif
#endif /* #if(NM_ENABLE_INTER_MODULE_CHECKS) */


/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_NetworkStartIndication

 * Description:   This is the NM call-back interface for notification of Network Start,
 *                this function in turn notifies ComM.
 * Parameter:     NetworkHandle - Identification of the NM-channel
 * Return:        void
 *************************************************************************************************************/


#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_NetworkStartIndication(NetworkHandleType NetworkHandle)
{

    NetworkHandleType Nm_NetworkHandle_u8;  /* Network handle is received from ComM */
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    /* process only if the network handle is within allowed range */
    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {

        /* notify Communication Manager that NM has been started for the channel Nm_NetworkHandle_u8 */
        ComM_Nm_NetworkStartIndication(NetworkHandle);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

