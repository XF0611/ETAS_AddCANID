
#include "Dcm_Cfg_Prot.h"
#include "DcmDspObd_Mode37A_Priv.h"
#include "Rte_Dcm.h"

#if (DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)
#if(DCM_CFG_DSP_OBDMODE37A_ENABLED != DCM_CFG_OFF)

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **********************************************************************************************************************
 * Dcm_Prv_CheckDTCFilteredStatus :
 * This function is used to check the filtered DTC's and extract the Emission related DTCs.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
static Std_ReturnType Dcm_Prv_CheckDTCFilteredStatus (
        Dcm_MsgLenType  nrResMaxDataLen_u32,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint8                            ClientID_u8;  /* Variable to store the DEM Client ID */
    uint32                           dataMode37ADtc_u32;       /* Variable to store DTC */
    uint8                            stMode37ADTC_u8;           /* Variable to store the DTC status */
    Dcm_MsgLenType                   nrResDTCDataLen_u32;     /* Local variable number of DTCs and response length */
    uint32_least                     cntrLoop_qu32;            /* variable for loop counter */
    Dem_ReturnGetNextFilteredDTCType dataRetGetNextFilt_u8; /*Return type for Dem_GetNextFilteredDTC*/
    Std_ReturnType                   dataRetVal_u8;      /* variable for Return value */

    dataRetVal_u8 = E_OK;

    /* Filter mask accepted by DEM */
    /* Response length will be at least 1 byte if no matched DTCs are found */
    nrResDTCDataLen_u32 = 1;

    /* Restrict the for loop for Max number of DTC than can be fit in with the available buffer size */
    nrResMaxDataLen_u32 = nrResMaxDataLen_u32>>1u;

    for(cntrLoop_qu32 = 0x0u; ((cntrLoop_qu32 < nrResMaxDataLen_u32) && (*dataNegRespCode_u8 == 0x0u)); cntrLoop_qu32++)
    {
        ClientID_u8 = Dcm_Prv_GetDemClientId(DCM_OBDCONTEXT); /* Store the Client ID */

        /* Get the filtered DTC */
        dataRetGetNextFilt_u8 = Dem_GetNextFilteredDTC(ClientID_u8,
                                                       &dataMode37ADtc_u32,
                                                       &stMode37ADTC_u8);
        if(dataRetGetNextFilt_u8 == E_OK)
        {
            /* Fill the response buffer with DTC and status */
            pMsgContext->resData[nrResDTCDataLen_u32] = (uint8)(dataMode37ADtc_u32 >> 16uL);
            nrResDTCDataLen_u32++;
            pMsgContext->resData[nrResDTCDataLen_u32] = (uint8)(dataMode37ADtc_u32 >> 8uL);
            nrResDTCDataLen_u32++;
        }
        else if(dataRetGetNextFilt_u8 == DEM_NO_SUCH_ELEMENT)
        {
            /* All the matched DTCs are read */
            /* Fill first byte of response buffer with the number of DTCs */
            /* one DTC is of length 2 bytes */
            pMsgContext->resData[0] = (uint8)nrResDTCDataLen_u32 >> 1uL;
            /* break loop operation if no further matching DTCs are found */
            break;
        }
        else
        {
            /* NRC 0x12 is sent, problems has occurred in DEM while reading DTC */
           *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
            dataRetVal_u8      = E_NOT_OK;
        }
    }
    /* Update the response length */
    pMsgContext->resDataLen = nrResDTCDataLen_u32;

    return(dataRetVal_u8);
}
/**
 **********************************************************************************************************************
 * Dcm_DcmOBDMODE37A :
 * This OBD service is used to obtain all emission related confirmed, pending and permanent diagnostic information.
 * OBD service 0x03 is used to obtain confirmed DTCs.
 * OBD service 0x07 is used to obtain pending DTCs.
 * OBD service 0x0A is used to obtain permanent DTCs.
 * This function is called by DSD, when SID is 0x03 or 0x07 or 0x0A,
 * this service is provided in DSP module and configured in DCM identifier table.
 * DEM function Dem_SetDTCFilte is used to set the filter mask and function Dem_GetNextFilteredDTC is used
 * to extract corresponding Emission related DTCs.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
Std_ReturnType Dcm_DcmObdMode37A (
        Dcm_SrvOpStatusType OpStatus,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint8                   ClientID_u8;  /* Variable to store the DEM Client ID */
    Dcm_MsgLenType          nrResMaxDataLen_u32;     /* Variable to store max response length */
    uint8                   dataDTCStatusMask_u8;      /* variable for DTCStatusMask for different modes */
    Dem_DTCOriginType       dataDTCOrigin_u16;     /* variable for DTCOrigin for different modes */
    Dem_ReturnSetFilterType dataRetSetDTCFilter_u8;   /* Return type for Dem_SetDTCFilter */
    Std_ReturnType          dataRetVal_u8;      /* variable for Return value */

    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);
    *dataNegRespCode_u8 = 0x0u;                  /* Initialisation of local variables */

    /* Initilization of DTC status mask and DTC origin for OBD service 0x03 */
    dataDTCStatusMask_u8  = 0x08;
    dataDTCOrigin_u16      = DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY;
    dataRetVal_u8         = E_OK;

    /* Check if the request Data Length is equal to 0 */
    if(pMsgContext->reqDataLen == DCM_OBDMODE37A_REQLEN)
    {
        /* Copy the Maximum response length into local variable */
        nrResMaxDataLen_u32 = pMsgContext->resMaxDataLen;

        switch(pMsgContext->idContext)
        {
            case 0x03u:
            {
                /* Request for confirmed emission related DTCs */
                /* Set the DTC Filter for OBD service 0x03 */
                /* DTC status mask and DTC origin for OBD service 0x08 */
                dataDTCStatusMask_u8 = 0x08;

                break;
            }
            case 0x07u:
            {
                /* Request for confirmed emission related DTCs */
                /* Set the DTC Filter for OBD service 0x07 */
                /* DTC status mask and DTC origin for OBD service 0x04 */
                dataDTCStatusMask_u8 = 0x04;

                break;
            }
            case 0x0Au:
            {
                /* Request for confirmed emission related DTCs */
                /* Set the DTC Filter for OBD service 0x0A */
                /* DTC status mask and DTC origin for OBD service 0x00 */
                dataDTCStatusMask_u8 = 0x00;
                dataDTCOrigin_u16     = DEM_DTC_ORIGIN_PERMANENT_MEMORY;
                break;
            }
            default:
            {
                /* E_NOT_OK is sent if the SID is other than 0x03 or 0x07 or 0x0A */
                *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
                dataRetVal_u8       = E_NOT_OK;
                break;
            }
        }
        /* Stop processing the request in case of any Negative response code */
        if((pMsgContext->idContext == 0x03u)||
           (pMsgContext->idContext == 0x07u)||
           (pMsgContext->idContext == 0x0Au))
        {
            ClientID_u8 = Dcm_Prv_GetDemClientId(DCM_OBDCONTEXT); /* Store the Client ID */

            dataRetSetDTCFilter_u8 = Dem_SetDTCFilter(ClientID_u8,
                                                      dataDTCStatusMask_u8,
                                                      DEM_DTC_FORMAT_OBD,
                                                      dataDTCOrigin_u16,
                                                      FALSE,
                                                      DEM_SEVERITY_NO_SEVERITY,
                                                      FALSE);
            if(dataRetSetDTCFilter_u8 == E_OK)
            {
                dataRetVal_u8 = Dcm_Prv_CheckDTCFilteredStatus(nrResMaxDataLen_u32,
                                                               pMsgContext,
                                                               dataNegRespCode_u8);
            }
            else
            {
                /* E_NOT_OK is sent if the filter mask is not accepted DEM */
                *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetSetDTCFilter_u8);
                dataRetVal_u8       = E_NOT_OK;
            }
        }
    }
    /* Request Length not correct */
    else
    {
        /* NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing */
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
         dataRetVal_u8      = E_NOT_OK;
    }
    return dataRetVal_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif
