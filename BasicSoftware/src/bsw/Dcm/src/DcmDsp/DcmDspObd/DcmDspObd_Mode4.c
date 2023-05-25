
#include "Dcm_Cfg_Prot.h"
#include "DcmDspObd_Mode4_Priv.h"
#include "Rte_Dcm.h"

#if (DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_OBDMODE4_ENABLED  != DCM_CFG_OFF)

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_DspObdMode04Type_ten Dcm_stDspObdMode04State_en;   /* Variable to store the state of OBD
                                                                              service 0X04 */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**
 **********************************************************************************************************************
 * Dcm_DcmObdMode04_Ini : Initialisaton function for OBD Mode 0x04 service
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **********************************************************************************************************************
 */
void Dcm_DcmObdMode04_Ini(void)
{
    /* Initialisation of OBD Service 0x04 state to INIT state  */
    Dcm_stDspObdMode04State_en = DCM_DSP_MODE04_INIT;
}
/**
 **********************************************************************************************************************
 * Dcm_Prv_DcmObdMode04CheckInitState :
 * This function is used to check for clearing all emission related diagnostic information.
 * This function is called by Dcm_DcmObdMode04, when SID is 0x04..
 *
 * \param           dataNegRespCode_u8
 *                  pMsgContext
 *
 * \retval          dataNegRespCode_u8
 * \seealso
 *
 **********************************************************************************************************************
 */
static void Dcm_Prv_DcmObdMode04CheckInitState (
        const Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType stObdMode04Appl_u8;            /* check conditions for OBD Mode 04 */

    /* Check if the request Data Length is equal to 0 */
    if(pMsgContext->reqDataLen == DCM_OBDMODE04_REQ_LEN)
    {
        /* check whether the conditions like ignition is ON, engine is not running are satisfied
        this check is done by APPL function call DcmAppl_OBD_Mode04() */
        stObdMode04Appl_u8 = DcmAppl_OBD_Mode04();

        /* if the above conditions are satisfied then application function DcmAppl_OBD_Mode04() returns E_OK
        if the conditions are not satisfied then application function returns E_NOT_OK*/
        if(stObdMode04Appl_u8 == E_OK)
        {
            /* Update the OBD MODE 04 state to call DEM */
            Dcm_stDspObdMode04State_en = DCM_DSP_MODE04_CLEAR;
        }
        else
        {
            /*Set Negative response code */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
        }
    }
    else
    {
        /*Set Negative response code Request Length Invalid
        NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing*/
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
    }
}
/**
 **********************************************************************************************************************
 * Dcm_Prv_DcmObdMode04ClearDTC :
 * This function is used to clear all emission related diagnostic information.
 * This function is called by DSD, when SID is 0x04,
 * this service is provided in DSP module and configured in DCM identifier table.
 * When the DSP receives such a request it executes DEM function Dem_ClearDTC with
 * parameter DTCGroup Emission related DTCs and all related information from the fault memory.
 *
 * \param           dataNegRespCode_u8
 *
 * \retval          dataRetVal_u8
 * \seealso
 *
 **********************************************************************************************************************
 */
static Std_ReturnType Dcm_Prv_DcmObdMode04ClearDTC (Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint8          ClientID_u8;                 /* Variable to store the DEM Client ID */
    Std_ReturnType dataDemClrRetVal_u8;/*for temporary storage of the DEM return values (enum)*/
    Std_ReturnType dataRetVal_u8;      /* check conditions for OBD Mode 04 */

    dataDemClrRetVal_u8 = E_OK;
    dataRetVal_u8       = E_NOT_OK;

    ClientID_u8 = Dcm_Prv_GetDemClientId(DCM_OBDCONTEXT);      /* Store the Client ID */

    /* Request DEM to clear all emission related diagnostic information
     Parameter to Dem_ClearDTC are DTCGROUP is group of emission related DTCS,
     DTCFORMATTYPE is the format of DTCS
     DTCORIGIN is DTCS in primary memory*/
    dataDemClrRetVal_u8 = Dem_ClearDTC(ClientID_u8);

    switch(dataDemClrRetVal_u8)
    {
        /* if the clearing of emission related diagnostic information is successful */
        case E_OK:
        {
            /* reset the state machine */
            Dcm_stDspObdMode04State_en = DCM_DSP_MODE04_INIT;
            dataRetVal_u8              = E_OK;
            break;
        }
        /* If DEM needs more time to clear the information */
        case DEM_PENDING:
        {
            /* Remain in the same state */
            dataRetVal_u8 = DCM_E_PENDING;
            break;
        }
        default:
        {
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT; /* Set NRC 0x22*/
            dataRetVal_u8 = E_NOT_OK;
            break;
        }
    }
    return (dataRetVal_u8);
}
/**
 **********************************************************************************************************************
 * Dcm_DcmOBDMODE04 :
 * This OBD service is used to clear all emission related diagnostic information.
 * This function is called by DSD, when SID is 0x04,
 * this service is provided in DSP module and configured in DCM identifier table.
 * When the DSP receives such a request it executes DEM function Dem_ClearDTC with
 * parameter DTCGroup Emission related DTCs and all related information from the fault memory.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
Std_ReturnType Dcm_DcmObdMode04 (
        Dcm_SrvOpStatusType OpStatus,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint8 ClientID_u8; /* Variable to store the DEM Client ID */
    Std_ReturnType dataRetVal_u8 = E_NOT_OK; /* check conditions for OBD Mode 04 */

    *dataNegRespCode_u8 = 0x0u; /* Initialisation of local variables */

    /* If OpStatus is set to DCM_CANCEL then call the ini function for resetting */
    if (OpStatus == DCM_CANCEL)
    {
        /* Call the Ini Function */
        Dcm_DcmObdMode04_Ini();
        /* Set the return value to E_OK as the Ini function will always be serviced  */
        dataRetVal_u8 = E_OK;
    }
    else
    {
        if(Dcm_stDspObdMode04State_en == DCM_DSP_MODE04_INIT)
        {
            Dcm_Prv_DcmObdMode04CheckInitState (pMsgContext,
                    dataNegRespCode_u8);
            ClientID_u8 = Dcm_Prv_GetDemClientId(DCM_OBDCONTEXT); /* Store the Client ID */

            if(Dcm_stDspObdMode04State_en == DCM_DSP_MODE04_CLEAR)
            {
                dataRetVal_u8 = Dem_SelectDTC(ClientID_u8,DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_FORMAT_OBD, DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY);
                if(dataRetVal_u8 == E_OK)
                {
                    dataRetVal_u8 = Dcm_Prv_DcmObdMode04ClearDTC(dataNegRespCode_u8);
                }

                else
                {
                    *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetVal_u8);
                }
            }
        }
        else if (Dcm_stDspObdMode04State_en == DCM_DSP_MODE04_CLEAR)
        {
            dataRetVal_u8 = Dcm_Prv_DcmObdMode04ClearDTC(dataNegRespCode_u8);
        }
        else
        {
            /*should not happen, possible DET issue*/
        }

        if(Dcm_stDspObdMode04State_en == DCM_DSP_MODE04_UNINIT)
        {
            /*in order to avoid the Dcm layer to call this service handler once again,
             send negative response 0x12 and due to functional addressing this negative response
             is suppressed hence no response is sent by ECU */
            /* Call function to set Negative response code */
            *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED; /* Set NRC 0x12*/;
        }
    }

    /* If negative response code is set */
    if(*dataNegRespCode_u8 != 0x0u)
    {
        /* reset the state machine */
        Dcm_stDspObdMode04State_en = DCM_DSP_MODE04_INIT;
        dataRetVal_u8 = E_NOT_OK;
    }
    if(Dcm_stDspObdMode04State_en == DCM_DSP_MODE04_INIT)
    {
        pMsgContext->resDataLen = 0x0u; /* Set response length to 0 */

    }
    return dataRetVal_u8;
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif

