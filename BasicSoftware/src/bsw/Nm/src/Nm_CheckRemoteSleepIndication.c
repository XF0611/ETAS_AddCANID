
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"

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
 * Function Name: Nm_CheckRemoteSleepIndication

 * Description:   This AUTOSAR interface gets the status of remote sleep detection
 * Parameter1:    NetworkHandle- Identification of the NM-channel
 * Parameter2:    nmRemoteSleepIndPtr - Pointer to the location where the check result of remote sleep
 *                                      indication shall be copied.
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Checking of remote sleep indication bits has failed
 *************************************************************************************************************/


/* This function is only available if NmRemoteSleepIndEnabled is set to TRUE */
#if (NM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
Std_ReturnType Nm_CheckRemoteSleepIndication(NetworkHandleType NetworkHandle, boolean * nmRemoteSleepIndPtr)
{
    const Nm_ConfigType * ConfDataPtr_pcst;
    const Nm_BusNmApiType_tst * FuncPtr_pcst;
    Std_ReturnType RetVal_u8;
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);
    RetVal_u8 = E_NOT_OK;
    /* process only if the channel handle is within allowed range and the nmRemoteSleepIndPtr pointer is valid */
    if ((Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS) && (nmRemoteSleepIndPtr != NULL_PTR))
    {
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
#if (NM_AR45_VERSION != STD_OFF)		
        if(ConfDataPtr_pcst->NmPassiveModeEnabled == FALSE)
        {
#endif		
        FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

        /* Calls the Bus specific API */
        RetVal_u8 = (*FuncPtr_pcst->BusNm_CheckRemoteSleepIndication_pfct)(NetworkHandle, nmRemoteSleepIndPtr);
#if (NM_AR45_VERSION != STD_OFF)			
        }
        else
        {
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_CHECKREMOTESLEEPINDICATION, NM_E_INVALID_CHANNEL);
        }
#endif		
    }
    else
    {
        if (!(Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS))
        {
            /* Report to DET as the network handle parameter is not a configured network handle */
#if (NM_AR45_VERSION != STD_OFF) 
             NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_CHECKREMOTESLEEPINDICATION, NM_E_INVALID_CHANNEL);
#else
             NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_CHECKREMOTESLEEPINDICATION, NM_E_HANDLE_UNDEF);
#endif			
        }
        else
        {
            //do nothing
        }
        if (nmRemoteSleepIndPtr == NULL_PTR)
        {
            /* Report to DET as the parameter passed is  a null pointer*/
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_CHECKREMOTESLEEPINDICATION, NM_E_PARAM_POINTER);
        }
        else
        {
            //do nothing
        }

    }


    return(RetVal_u8);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

