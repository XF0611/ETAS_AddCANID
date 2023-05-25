
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
 * Function Name: Nm_DisableCommunication

 * Description:   This is the interface to disable NM transmission
 * Parameter:     NetworkHandle- Identification of the NM-channel
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Disabling of NM PDU transmission ability has failed.
 *************************************************************************************************************/

/* This function is only available if NM_COM_CONTROL_ENABLED is set to TRUE */
#if (NM_COM_CONTROL_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
Std_ReturnType Nm_DisableCommunication(NetworkHandleType NetworkHandle)
{
    const Nm_ConfigType * ConfDataPtr_pcst;         /* Configuration pointer holds referrence of configuration data */
    const Nm_BusNmApiType_tst * FuncPtr_pcst;       /* Pointer to Bus specific APIs */
    Std_ReturnType RetVal_u8;                       /* Return value */
    NetworkHandleType Nm_NetworkHandle_u8;          /* Network handle is received from ComM */
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    RetVal_u8 = E_NOT_OK;
    /* Process only if the channel handle is within allowed range */
    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        /* Configuration pointer initialization */
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
#if (NM_AR45_VERSION != STD_OFF)		
        if(ConfDataPtr_pcst->NmPassiveModeEnabled == FALSE)
        {
#endif		
        /* Holds the reference to Bus specific APIs */
        FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

        /* Calls the Bus specific API*/
        RetVal_u8 = (*FuncPtr_pcst->BusNm_DisableCommunication_pfct)(NetworkHandle);
#if (NM_AR45_VERSION != STD_OFF)			
        }
        else
        {
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_DISABLECOMMUNICATION, NM_E_INVALID_CHANNEL);
        }
#endif		
    }
    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
#if (NM_AR45_VERSION != STD_OFF) 
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_DISABLECOMMUNICATION, NM_E_INVALID_CHANNEL);
#else
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_DISABLECOMMUNICATION, NM_E_HANDLE_UNDEF);
#endif
    }


    return(RetVal_u8);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

