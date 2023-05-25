
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


/********************************************************************************************************
 * Function
 ********************************************************************************************************
 ********************************************************************************************************
 * Function Name: Nm_GetUserData

 * Description:   This is the AUTOSAR interface for getting the user data out of the most recently received
 *                NM message, this API calls either CanNm_GetUserData or FrNm_GetUserData based on the
 *                channel handle.
 * Parameter1:     NetworkHandle   - Identification of the NM-channel
 * Parameter2:     nmUserDataPtr   - Pointer where user data out of the last successfully received
 *                                        NM message shall be copied to.
 * Return:         Std_ReturnType
 * Retval:         E_OK         - No error
 * Retval:         E_NOT_OK     - Getting of user data has failed
********************************************************************************************************/

/* This function is only available if NM_USER_DATA_ENABLED is set to TRUE */
#if(NM_USER_DATA_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
Std_ReturnType Nm_GetUserData(NetworkHandleType NetworkHandle, uint8 * nmUserDataPtr)
{
    const Nm_ConfigType * ConfDataPtr_pcst;         /* Configuration pointer holds referrence of configuration data */
    const Nm_BusNmApiType_tst * FuncPtr_pcst;       /* Pointer to Bus specific APIs */
    Std_ReturnType RetVal_u8;                       /* Return value */
    NetworkHandleType Nm_NetworkHandle_u8;          /* Network handle is received from ComM */
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    /* process only if the channel handle is within allowed range and the data pointer is valid */
    if ((Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS) && (nmUserDataPtr != NULL_PTR))
    {
        /* Configuration pointer initialization */
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
        /* Holds the reference to Bus specific APIs */
        FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

        /* Calls the Bus specific API*/
        RetVal_u8 = (*FuncPtr_pcst->BusNm_GetUserData_pfct)(NetworkHandle, nmUserDataPtr);
    }
    else
    {
        if (!(Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS))
        {
            /* Report to DET as the network handle parameter is not a configured network handle */
#if (NM_AR45_VERSION != STD_OFF) 
             NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_GETUSERDATA, NM_E_INVALID_CHANNEL);
#else
             NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_GETUSERDATA, NM_E_HANDLE_UNDEF);
#endif				 
        }
        else
        {
            //do nothing
        }
        if (nmUserDataPtr == NULL_PTR)
        {
            /* Report to DET as the parameter passed is  a null pointer*/
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_GETUSERDATA, NM_E_PARAM_POINTER);
        }
        else
        {
            //do nothing
        }


        /* Reject the call: either Nm_NetworkHandle_u8 is not within allowed range or data pointer is invalid */
        RetVal_u8 = E_NOT_OK;
    }


    return(RetVal_u8);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

