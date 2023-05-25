
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
 * Function Name: Nm_PassiveStartUp

 * Description:   This is the AUTOSAR interface for triggering passive start up of NM, this API calls either
 *                CanNm_PassiveStartUp or FrNm_PassiveStartUp based on the configuration
 * Parameter:     NetworkHandle- Identification of the NM-channel
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Passive start of network management has failed
 *************************************************************************************************************/


#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
Std_ReturnType Nm_PassiveStartUp(NetworkHandleType NetworkHandle)
{
    const Nm_ConfigType * ConfDataPtr_pcst;
    const Nm_BusNmApiType_tst * FuncPtr_pcst;


    Std_ReturnType RetVal_u8;
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    RetVal_u8 = E_NOT_OK;
    /* process only if the channel handle is within allowed range */
    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        /* Configuration pointer initialization */
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
        /* Holds the reference to Bus specific APIs */
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF)
        FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

        /* Calls the Bus specific API*/
        RetVal_u8 = (*FuncPtr_pcst->BusNm_PassiveStartUp_pfct)(NetworkHandle);
#else
        if(ConfDataPtr_pcst->ClusterIdx != 0xFF)
        {
            /*call internal network request to call bus specific passive startup and to handle channel state change accordingly*/
            RetVal_u8 = Nm_Prv_NetworkRequest(FALSE,NetworkHandle,NM_EXTERNAL_REQUEST);
        }
        /*channel is not a part of any cluster*/
        else
        {
            FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];
            RetVal_u8 = (*FuncPtr_pcst->BusNm_PassiveStartUp_pfct)(NetworkHandle);
        }
        #endif
    }

    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
#if (NM_AR45_VERSION != STD_OFF) 
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_PASSIVESTARTUP, NM_E_INVALID_CHANNEL);
#else
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_PASSIVESTARTUP, NM_E_HANDLE_UNDEF);
#endif		
    }

    return(RetVal_u8);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

