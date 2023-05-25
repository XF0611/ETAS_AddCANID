
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
 * Function Name: Nm_NetworkRequest

 * Description:   This is the AUTOSAR interface to request Network. This API calls either CanNm_NetworkRequest
 *                or FrNm_NetworkRequest based on the configuration
 * Parameter:     NetworkHandle- Identification of the NM-channel
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Request for bus communication has failed
 *************************************************************************************************************/
#if (NM_AR45_VERSION != STD_OFF)  
 
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle)
{
    const Nm_ConfigType * ConfDataPtr_pcst;     /* Configuration pointer holds referrence of configuration data */
    const Nm_BusNmApiType_tst * FuncPtr_pcst;   /* Pointer to Bus specific APIs */

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
        if(ConfDataPtr_pcst->NmPassiveModeEnabled == FALSE)
        {
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF)
        /* Holds the reference to Bus specific APIs */
        FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

        RetVal_u8 = (*FuncPtr_pcst->BusNm_NetworkRequest_pfct)(NetworkHandle);
#else
        if(ConfDataPtr_pcst->ClusterIdx != 0xFF)
        {
            /*call internal network request to call bus Specific network request and to handle  channel state change accordingly*/
            RetVal_u8 = Nm_Prv_NetworkRequest(TRUE,NetworkHandle,NM_EXTERNAL_REQUEST);
        }
        /*channel is not a part of any cluster*/
        else
        {
            FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];
            RetVal_u8 = (*FuncPtr_pcst->BusNm_NetworkRequest_pfct)(NetworkHandle);
        }
#endif
        }
        else
        {
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_NETWORKREQUEST, NM_E_INVALID_CHANNEL);
        }
    }

    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_NETWORKREQUEST, NM_E_INVALID_CHANNEL);
    }

    return(RetVal_u8);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#else
 
/* The API is available only if the Passive Mode is Disabled */
#if(NM_PASSIVE_MODE_ENABLED == STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle)
{
    const Nm_ConfigType * ConfDataPtr_pcst;     /* Configuration pointer holds referrence of configuration data */
    const Nm_BusNmApiType_tst * FuncPtr_pcst;   /* Pointer to Bus specific APIs */

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
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF)
        /* Holds the reference to Bus specific APIs */
        FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

        RetVal_u8 = (*FuncPtr_pcst->BusNm_NetworkRequest_pfct)(NetworkHandle);
#else
        if(ConfDataPtr_pcst->ClusterIdx != 0xFF)
        {
            /*call internal network request to call bus Specific network request and to handle  channel state change accordingly*/
            RetVal_u8 = Nm_Prv_NetworkRequest(TRUE,NetworkHandle,NM_EXTERNAL_REQUEST);
        }
        /*channel is not a part of any cluster*/
        else
        {
            FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];
            RetVal_u8 = (*FuncPtr_pcst->BusNm_NetworkRequest_pfct)(NetworkHandle);
        }
#endif
    }

    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_NETWORKREQUEST, NM_E_HANDLE_UNDEF);
    }

    return(RetVal_u8);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif /* Passive Mode Switch */

#endif

