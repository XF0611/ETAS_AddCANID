

#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"

/***********************************************************************************************************************
 *    Function Definition
 **********************************************************************************************************************/
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/***********************************************************************************************************************
 Function name    : Dcm_Prv_SetComMState
 Syntax           : Dcm_Prv_SetComMState(NetworkId,ComManagerState)
 Description      : This INLINE function is used to Set whether Network Id is valid or not.
 Parameter        : uint8,Dcm_Dsld_commodeType
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_SetComMState(uint8 NetworkId,Dcm_Dsld_commodeType ComManagerState)
{
    uint8 idxNetwork_u8;

    for(idxNetwork_u8 = 0;idxNetwork_u8<DCM_NUM_COMM_CHANNEL;idxNetwork_u8++)
    {
        if(Dcm_active_commode_e[idxNetwork_u8].ComMChannelId == NetworkId)
        {
            break;
        }
    }

    if (idxNetwork_u8 < DCM_NUM_COMM_CHANNEL)
    {
        Dcm_active_commode_e[idxNetwork_u8].ComMState = ComManagerState;
    }
}

/**
 **********************************************************************************************************************
 * Dcm_ComM_FullComModeEntered - All kind of transmissions shall be enabled immediately. This means that
 * the ResponseOnEvent and PeriodicId. It will be a dummy function in boot as ComM is not available in boot
 * \param           NetworkId
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **********************************************************************************************************************
 */
void Dcm_ComM_FullComModeEntered(uint8 NetworkId)
{
    /*  Communication mode of the corresponding to  network is set to Full Communication mode */
    Dcm_Prv_SetComMState(NetworkId,DCM_DSLD_FULL_COM_MODE);
}


/**
 **************************************************************************************************
 * All kind of transmissions (receive and transmit) shall be stopped . This means that the ResponseOnEvent
 * and PeriodicId and also the transmission of the normal communication (PduR_DcmTransmit) shall be disabled.
 * It will be a dummy function in boot as ComM is not available in boot
 * \param           NetworkId
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_ComM_NoComModeEntered(uint8 NetworkId)
{
    /*  Communication mode of the corresponding to  network is set to No Communication mode */
    Dcm_Prv_SetComMState(NetworkId,DCM_DSLD_NO_COM_MODE);
}

/**
 **************************************************************************************************
 * Dcm_ComM_SilentComModeEntered: All outgoing transmissions shall be stopped immediately. This means that the
 * ResponseOnEvent and PeriodicId and also the transmission of the normal communication (PduR_DcmTransmit)
 * shall be disabled.
 * It will be a dummy function in boot as ComM is not available in boot
 * \param           NetworkId
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_ComM_SilentComModeEntered(uint8 NetworkId)
{
    /*  Communication mode of the corresponding to  network is set to silent  Communication mode */
    Dcm_Prv_SetComMState(NetworkId,DCM_DSLD_SILENT_COM_MODE);
}

/*
***********************************************************************************************************
*  Dcm_SetActiveDiagnostic: The call of this function allows to activate and deactivate the call of ComM_DCM_ActiveDiagnostic() function
*  \param    active:  True Dcm will call ComM_DCM_ActiveDiagnostic() \n
*                           False Dcm shall not call ComM_DCM_ActiveDiagnostic()
*  \retval   E_OK : this value is always returned.
*  \seealso
*************************************************************************************************************
*/

Std_ReturnType Dcm_SetActiveDiagnostic(boolean active)
{
    /* Multicore: No lock necessary as its an atomic operation .
     * Also, the application has to take care of calling Dcm_SetActiveDiagnostic() before Dcm calls CheckActiveDiagnostics from RxIndication. */
    /*Check if the data active parameter is set to True to call ComM_DCM_ActiveDiagnostic()*/
   if(active != FALSE)
   {
       Dcm_ActiveDiagnosticState_en = DCM_COMM_ACTIVE;
   }
   else
   {
       Dcm_ActiveDiagnosticState_en = DCM_COMM_NOT_ACTIVE;
   }
   return(E_OK);
}

/*
***********************************************************************************************************
*  Dcm_CheckActiveDiagnosticStatus: The function checks whether ComM_DCM_ActiveDiagnostic()function needs to be called or not
*  \param    dataNetworkId:  Communication channel network id
*  \retval
*  \seealso
*************************************************************************************************************
*/
void Dcm_CheckActiveDiagnosticStatus(uint8 dataNetworkId)
{
    /* Check the diagnostic state is active, then call the ComM_DCM_ActiveDiagnostic*/
    if(Dcm_ActiveDiagnosticState_en == DCM_COMM_ACTIVE)
    {
        ComM_DCM_ActiveDiagnostic(dataNetworkId);
    }
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
