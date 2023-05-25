
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
 * Function Name: Nm_RepeatMessageRequest

 * Description:   This AUTOSAR interface causes Repeat Message Request to be transmitted next on the bus
 * Parameter:     NetworkHandle- Identification of the NM-channel
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Repeat Message Request has failed
 *************************************************************************************************************/
#if (NM_AR45_VERSION != STD_OFF) 

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
Std_ReturnType Nm_RepeatMessageRequest(NetworkHandleType NetworkHandle)
{
    const Nm_ConfigType * ConfDataPtr_pcst; /* Configuration pointer holds referrence of configuration data */
    const Nm_BusNmApiType_tst * FuncPtr_pcst;     /* Pointer to Bus specific APIs */
    Std_ReturnType RetVal_u8 = E_NOT_OK;                            /* Return value */
    NetworkHandleType Nm_NetworkHandle_u8;              /* Network handle is received from ComM */

    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    /* process only if the channel handle is within allowed range */
    if ((Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS))
    {
        /* Configuration pointer initialization */
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
        if(ConfDataPtr_pcst->NmPassiveModeEnabled == FALSE)
        {
        /* Holds the reference to Bus specific APIs */

	        FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

	        /* Calls the Bus specific API */
	        RetVal_u8 = (*FuncPtr_pcst->BusNm_RepeatMessageRequest_pfct)(NetworkHandle);
        }
        else
        {
            /* reject the call: NmPassiveModeEnabled is true Report DET and Return value is E_NOT_OK*/
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_REPEATMESSAGEREQUEST, NM_E_INVALID_CHANNEL);
        }
    }
    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
        /* reject the call: Nm_NetworkHandle_u8 is not within allowed range with Return value E_NOT_OK */
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_REPEATMESSAGEREQUEST, NM_E_INVALID_CHANNEL);
    }


    return(RetVal_u8);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#else

/* This function is only available if NmNodeDetectionEnabled is TRUE */
#if (NM_NODE_DETECTION_ENABLED != STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
Std_ReturnType Nm_RepeatMessageRequest(NetworkHandleType NetworkHandle)
{
    const Nm_ConfigType * ConfDataPtr_pcst; /* Configuration pointer holds referrence of configuration data */
    const Nm_BusNmApiType_tst * FuncPtr_pcst;     /* Pointer to Bus specific APIs */
    Std_ReturnType RetVal_u8 = E_NOT_OK;                            /* Return value */
    NetworkHandleType Nm_NetworkHandle_u8;              /* Network handle is received from ComM */

    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    /* process only if the channel handle is within allowed range */
    if ((Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS))
    {
        /* Configuration pointer initialization */
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
        /* Holds the reference to Bus specific APIs */
        if(ConfDataPtr_pcst->NmNodeDetectionEnabled)
        {
	        FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

	        /* Calls the Bus specific API */
	        RetVal_u8 = (*FuncPtr_pcst->BusNm_RepeatMessageRequest_pfct)(NetworkHandle);
        }
    }
    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_REPEATMESSAGEREQUEST, NM_E_HANDLE_UNDEF);


        /* reject the call: Nm_NetworkHandle_u8 is not within allowed range */
        RetVal_u8 = E_NOT_OK;
    }


    return(RetVal_u8);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

#endif

