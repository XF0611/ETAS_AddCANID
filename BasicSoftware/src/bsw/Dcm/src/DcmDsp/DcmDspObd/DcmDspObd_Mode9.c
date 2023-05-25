
#include "Dcm_Cfg_Prot.h"
#include "DcmDspObd_Mode9_Priv.h"
#include "Rte_Dcm.h"

#if (DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_OBDMODE9_ENABLED != DCM_CFG_OFF)

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint8 Dcm_dataDspInfoType_u8; /* INFOTYPE which report data value */
static uint8 Dcm_idxDspInfoType_u8; /* Index of the INFOTYPE which report data value */
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint16 Dcm_idxDspVehData_u16;/* Index to iterate through VehDataInfo for a VehInfo */
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_DspMode09Type_ten Dcm_stDspMode09_en; /* Variable to store the state of Mode9 service */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**********************************************************************************************************************
 * Dcm_Prv_SupportInfoTypecheck :
 * The purpose of this service is to read vehicle-specific information such as VIN, calibration IDs etc.
 * Some of this information may be required by regulations and some may be desired to be reported in a
 * standard format if supported by the vehicle manufacturer.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
#if (DCM_CFG_INFOTYPE_SUPPORT != DCM_CFG_OFF)
void Dcm_Prv_SupportInfoTypecheck(void)
{
    boolean resetlastbit;
    uint8 infotype;
    uint8 idxInfoType_u8;
    uint8 infotypecount;
    Std_ReturnType l_returnstatus;
    uint8 restbitindex;
    l_returnstatus=E_OK;
    resetlastbit=FALSE;

    for(infotypecount=0u; infotypecount<DCM_CFG_NUMOF_INFOTYPE;infotypecount++)
    {
        if((Dcm_Dsp_VehInfoArray_acs[infotypecount].GetVehInfotypeSupport_pf.GetVehInfotypeSupport_pf1) != NULL_PTR)
        {
            infotype=Dcm_Dsp_VehInfoArray_acs[infotypecount].Infotype_u8;
            /*call the configured support function*/
            l_returnstatus=((*(Dcm_Dsp_VehInfoArray_acs[infotypecount].GetVehInfotypeSupport_pf.GetVehInfotypeSupport_pf1))\
                    (infotype));

            /*check if the call back function retutns E_NOT_OK from application*/
            if(l_returnstatus==E_NOT_OK)
            {
                idxInfoType_u8=infotype/DCM_OBDMODE09_SUPPINFOTYPE;
                /*update the bitmask value and numInfotype in veh info array*/
                Dcm_Dsp_Mode9Bitmask_acs[idxInfoType_u8].BitMask_u32 &= ~(0x01u<<((uint32)32u - ((infotype) % 0x20u)));
                resetlastbit=TRUE;
            }
        }
    }
    /*update the last bit of the bit mask value as per the updated vech info array Dcm_Dsp_Mode9Bitmask_acs*/
    if(TRUE==resetlastbit)
    {
        for(restbitindex=0x07u; restbitindex!=0x00u; restbitindex--)
        {
            if((restbitindex==0x07u)&&(Dcm_Dsp_Mode9Bitmask_acs[restbitindex].BitMask_u32==0x00u))
            {
                Dcm_Dsp_Mode9Bitmask_acs[restbitindex-0x01u].BitMask_u32 &= ~(0x01u);
            }
            else
            {
                if((Dcm_Dsp_Mode9Bitmask_acs[restbitindex].BitMask_u32==0x00u)||(Dcm_Dsp_Mode9Bitmask_acs[restbitindex].BitMask_u32==0x01u))
                {
                    Dcm_Dsp_Mode9Bitmask_acs[restbitindex-1u].BitMask_u32 &= ~(0x01u);
                }
                else
                {
                    break;
                }
            }
        }
    }
}
#endif
/**
 **********************************************************************************************************************
 * Dcm_DcmObdMode09_Ini : Initialisation function for Mode 09 Service
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **********************************************************************************************************************
 */
void Dcm_DcmObdMode09_Ini(void)
{
    /* Set Mode 09 state machine to DCM_DSP_MODE09_INIT */
    if(Dcm_stDspMode09_en == DCM_DSP_MODE09_RUNNING)
    {
        Dcm_stDspMode09_en = DCM_DSP_MODE09_INIT;

        /* If the Veh Info data API last accessed is a RTE API */
        if(Dcm_Dsp_VehInfoData_acs[Dcm_idxDspVehData_u16].InfoType_APIType_e == OBD_MODE9_RTE_FNC)
        {
            /* Fill the parameters DataValueBuffer and DataalueBuffersize pass NULL_PTR as the API is invoked to cancel
               the operation */
           (void)((*(Dcm_Dsp_VehInfoData_acs[Dcm_idxDspVehData_u16].GetInfotypeValueData_pf.GetInfotypeValueData_pf1))\
                    (DCM_CANCEL,NULL_PTR,NULL_PTR));
        }
    }
    Dcm_stDspMode09_en = DCM_DSP_MODE09_INIT;
    Dcm_SrvOpstatus_u8 = DCM_INITIAL;
}
/**********************************************************************************************************************
 * Dcm_Prv_Availability09Pid :
 * The purpose of this function is to check for the Avaliablilty PIDs of OBD mode9 service.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
static void Dcm_Prv_Availability09Pid (
        Dcm_ObdMode9ContextType *ModeContext,
        uint8* adrRespBuf_pu8,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint16_least idxInfotypeResBuf_qu16; /* Index to response buffer for valid INFOTYPEs */

    /* Initialization of local variables */
    idxInfotypeResBuf_qu16 = 0x0u;

    /* Check for INFOTYPEs which are requested by tester */
    for(ModeContext->nrInfoTypeChk_qu8 = 0;(ModeContext->nrInfoTypeChk_qu8 < ModeContext->nrReqDataLen_u32);\
    ModeContext->nrInfoTypeChk_qu8++)
    {
        /* Index for Dcm_Dsp_Mode9Bitmask_acs is calculated based on INFOTYPE number */
        /* Index = INFOTYPE number/0x20 */
        ModeContext->idxInfoType_u8 =
                (uint8)((ModeContext->adrTmpBuf_au8[ModeContext->nrInfoTypeChk_qu8]) / DCM_OBDMODE09_SUPPINFOTYPE);

        /* Access the INFOTYPE mask value corresponding to index in Dcm_Dsp_Mode9Bitmask_acs */
        ModeContext->dataInfoTypeMaskVal_u32 = Dcm_Dsp_Mode9Bitmask_acs[ModeContext->idxInfoType_u8].BitMask_u32;

        /* Each supported INFOTYPE provides bit coded value for the range of INFOTYPEs
         which are supported */
        /* For eg: INFOTYPE 00 provides bit coded value for the range of INFOTYPE $01 to $20 */
        /* For each supported INFOTYPE, 4bytes (32 INFOTYPES) are allocated in response format */

        /* Copy the INFOTYPE number and INFOTYPEMask value only if the accessed INFOTYPEMask
         value is not equal to 00 00 00 00*/
        if(ModeContext->dataInfoTypeMaskVal_u32 > 0u)
        {
            /* Check if response is sufficient to hold the data and info type */
            if((pMsgContext->resMaxDataLen - idxInfotypeResBuf_qu16) > 0x4u)
            {
                /* Copy INFOTYPE to response buffer */
                adrRespBuf_pu8[idxInfotypeResBuf_qu16] = ModeContext->adrTmpBuf_au8[ModeContext->nrInfoTypeChk_qu8];
                idxInfotypeResBuf_qu16++;

                /* For supported INFOTYPE, 4bytes are allocated in response format */
                /* Copy bit mask to message buffer */
                adrRespBuf_pu8[idxInfotypeResBuf_qu16] = (uint8)(ModeContext->dataInfoTypeMaskVal_u32 >> 24u);
                idxInfotypeResBuf_qu16++;
                adrRespBuf_pu8[idxInfotypeResBuf_qu16] = (uint8)(ModeContext->dataInfoTypeMaskVal_u32 >> 16u);
                idxInfotypeResBuf_qu16++;
                adrRespBuf_pu8[idxInfotypeResBuf_qu16] = (uint8)(ModeContext->dataInfoTypeMaskVal_u32 >> 8u);
                idxInfotypeResBuf_qu16++;
                adrRespBuf_pu8[idxInfotypeResBuf_qu16] = (uint8) ModeContext->dataInfoTypeMaskVal_u32;
                idxInfotypeResBuf_qu16++;

                /* Send positive response code */
                *dataNegRespCode_u8 = 0x0u;
            }
            else
            {
                /* Report development error "Buffer overflow" to DET module if the DET module is enabled */
                DCM_DET_ERROR(DCM_OBDMODE09_ID, DCM_E_INTERFACE_BUFFER_OVERFLOW)
                /* Set Negative response code 0x12 */
                /* NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing */
                *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

                /* Update index to request data length to exit out of the for loop */
                break;
            }
        }
    }
    /* Response length = INFOTYPE number + 4 data bytes for each requested INFOTYPE  */
    pMsgContext->resDataLen = idxInfotypeResBuf_qu16;
}
/**********************************************************************************************************************
 * Dcm_Prv_CheckBitMask :
 * The purpose of this function is to check the bit mask value.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
static void Dcm_Prv_CheckBitMask (
        Dcm_ObdMode9ContextType ModeContext,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint8_least nrInfoTypeMaxLoop_qu8;  /* No of iteration to search INFOTYPE in configuration */
    uint8_least idxinfoTypeStart_qu8;   /* Start index in INFOTYPE configuration */
    uint8_least nrInfotype_qu8;         /* Number of INFOTYPEs to be searched in INFOTYPE configuration*/

    /* Initialization of local variables */
    idxinfoTypeStart_qu8 = 0x0u;
    nrInfotype_qu8       = 0x0u;

    /* Requested INFOTYPE is supported in ECU. */
    /* Get starting index of the INFOTYPE to be searched in Dcm_Dsp_VehInfoArray_acs*/
    idxinfoTypeStart_qu8 = Dcm_Dsp_Mode9Bitmask_acs[ModeContext.idxInfoType_u8].StartIndex_u8;

    /* Get the number of INFOTYPES for which below loop has to be executed */
    /* eg:if only 5INFOTYPEs are supported in the range 01 to 1F, and if INFOTYPE 01 is */
    /* requested by tester then its sufficient to search INFOTYPE 01 in the range 01-1F */
    /* with only 5 iterations since only 5 INFOTYPEs are configured in that range */
    nrInfoTypeMaxLoop_qu8 = idxinfoTypeStart_qu8+(Dcm_Dsp_Mode9Bitmask_acs[ModeContext.idxInfoType_u8].NumInfotypes_u8);

    for(nrInfotype_qu8 = idxinfoTypeStart_qu8;
        nrInfotype_qu8 < nrInfoTypeMaxLoop_qu8;
        nrInfotype_qu8++)
    {
        /* If requested VehInfoType is found to be configured */
        if(Dcm_dataDspInfoType_u8 == (Dcm_Dsp_VehInfoArray_acs[nrInfotype_qu8].Infotype_u8))
        {
            /* Store the index of the requested INFOTYPE */
            Dcm_idxDspInfoType_u8 = (uint8)nrInfotype_qu8;

            /* Update the state machine to call software component API */
            Dcm_stDspMode09_en = DCM_DSP_MODE09_RUNNING;

            /* Get the starting index of the VehInfoData in Dcm_Dsp_VehInfoData_acs array */
            Dcm_idxDspVehData_u16 = Dcm_Dsp_VehInfoArray_acs[Dcm_idxDspInfoType_u8].InfoDatatypeIndex_u16;

            /* Copy INFOTYPE to response buffer */
            pMsgContext->resData[0] = Dcm_dataDspInfoType_u8;

            /*Check Whether number of dataitems is configured as False*/
            if(Dcm_Dsp_VehInfoArray_acs[Dcm_idxDspInfoType_u8].Is_NODI_Enabled_b != TRUE)
            {
                /*For Vehicle tracking data NODI value (ITIDs 0x16 - 0x29) shall be reported as 1 */
                if((Dcm_dataDspInfoType_u8 >= DCM_OBDMODE09_INFOTYPE16)&&
                  (Dcm_dataDspInfoType_u8 <= DCM_OBDMODE09_INFOTYPE29))
                {
                    /* For (ITIDs 0x16 - 0x29) NODI value is reported as 1 */
                    pMsgContext->resData[1] = DCM_OBDMODE09_NODIITID16_29;
                }
                else
                {
                    /* Store the number of Data elements  corresponding to the VehInfotype in the response buffer */
                    pMsgContext->resData[1] = (Dcm_Dsp_VehInfoArray_acs[Dcm_idxDspInfoType_u8].NumOfInfoData_u8);
                }
                    /* Update the response length by 2 bytes*/
                    pMsgContext->resDataLen = 2;
            }
            else
            {
                pMsgContext->resDataLen = 1;
            }

            Dcm_SrvOpstatus_u8 = DCM_INITIAL;
            /* Send positive response code */
            *dataNegRespCode_u8 = 0x0u;
            /* INFOTYPE found Stop the search */
            break;
        }
    }
}
/**********************************************************************************************************************
 * Dcm_Prv_ProcessDataOfOneInfotype :
 * The purpose of this function is to process data of one infotype.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
static void Dcm_Prv_ProcessDataOfOneInfotype (
        Dcm_ObdMode9ContextType *ModeContext,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint32 dataCalInfotypeBitMask_u32; /* Calculated Bit mask based on INFOTYPE value */

    /* Initialization of local variables */
    dataCalInfotypeBitMask_u32 = 0x0u; /* To remove warning */

    /* Requests for INFOTYPEs which request data value should contain only one INFOTYPE */
    /* Else NRC 0x12 will be sent */
    if(ModeContext->nrReqDataLen_u32 == DCM_OBDMODE09_REQ_LEN)
    {
        /* INFOTYPE which report data value */
        Dcm_dataDspInfoType_u8 = ModeContext->adrTmpBuf_au8[0];

        /* Index for INFOTYPE which report data value */
        /* Calculated based on INFOTYPE number. index number = INFOTYPE number/0x20 */
        ModeContext->idxInfoType_u8 = (uint8)(Dcm_dataDspInfoType_u8 / DCM_OBDMODE09_SUPPINFOTYPE);

        /* Access the INFOTYPE mask corresponding to INFOTYPE index in Dcm_Dsp_Mode9Bitmask_acs */
        ModeContext->dataInfoTypeMaskVal_u32 = Dcm_Dsp_Mode9Bitmask_acs[ModeContext->idxInfoType_u8].BitMask_u32;

        /* Based on requested INFOTYPE value, calculate the corresponding INFOTYPE Bit mask */
        /* eg:for INFOTYPE 01 BIT mask will be 80 00 00 00, as BIT 31 is mapped to INFOTYPE 01 */
        dataCalInfotypeBitMask_u32 = ((uint32)1u << ((uint32)32u - ((Dcm_dataDspInfoType_u8) % 0x20)));

        /* If required bit in the BIT mask is set */
        if((ModeContext->dataInfoTypeMaskVal_u32 & dataCalInfotypeBitMask_u32) > 0u)
        {
            Dcm_Prv_CheckBitMask(*ModeContext,
                                  pMsgContext,
                                  dataNegRespCode_u8);
        }
    }
    else
    {
        /* Invalid Request Length send NRC 0x12 */
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        DCM_DET_ERROR(DCM_OBDMODE09_ID, DCM_E_INVALID_LENGTH)
    }
}

/**********************************************************************************************************************
 * Dcm_Prv_Mode09Init :
 * The purpose of this function is to check if it is multiple of 0x20 or non multiple of 0x20 Infotypes.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
static Dcm_NegativeResponseCodeType Dcm_Prv_Mode09Init (
        Dcm_ObdMode9ContextType *ModeContext,
        Dcm_MsgContextType *pMsgContext,
        Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint8* adrRespBuf_pu8; /* Pointer to Response buffer */
    uint8* adrReqBuf_pu8; /* Pointer to Request buffer */
    uint8  nrMultiple_u8; /* Number of special INFOTYPES requested */

    /* Initialization of local variables */
    nrMultiple_u8 = 0x0u;

    /* Check if the request Data Length is greater than 0 and less than 7 */
    if((ModeContext->nrReqDataLen_u32 > DCM_OBDMODE09_REQ_LEN_MIN) &&
       (ModeContext->nrReqDataLen_u32 < DCM_OBDMODE09_REQ_LEN_MAX))
    {
        /* Pointer to request buffer */
        adrReqBuf_pu8 = pMsgContext->reqData;

        /* Check if it is multiple of 0x20 or non multiple of 0x20 Infotypes */
        for(ModeContext->nrInfoTypeChk_qu8 = 0; (ModeContext->nrInfoTypeChk_qu8 < ModeContext->nrReqDataLen_u32); \
        ModeContext->nrInfoTypeChk_qu8++)
        {
            /* Check the multiple of 0x20 */
            if((adrReqBuf_pu8[ModeContext->nrInfoTypeChk_qu8] & 0x1Fu) == 0u)
            {
                /* It is pure multiple of 0x20 */
                nrMultiple_u8++;
            }

            /* Copy INFOTYPE to temporary buffer */
            ModeContext->adrTmpBuf_au8[ModeContext->nrInfoTypeChk_qu8] = adrReqBuf_pu8[ModeContext->nrInfoTypeChk_qu8];
        }

        /* The request may have INFOTYPE mix of multiple and non multiple of 0x20 */
        /* If only availability INFOTYPE is requested then the counter value = requested data length */
        /* If only INFOTYPE which report data value is requested then the counter value = 0 */
        if((nrMultiple_u8 == 0) || (nrMultiple_u8 == ModeContext->nrReqDataLen_u32))
        {
            /* Calculate buffer index to be passed for the data */
            adrRespBuf_pu8 = pMsgContext->resData;

            /* Set Negative response code 0x12 */
            /* NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing */
            *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

            /* Processes availability INFOTYPEs */
            if(nrMultiple_u8 != 0)
            {
                Dcm_Prv_Availability09Pid (ModeContext,
                                           adrRespBuf_pu8,
                                           pMsgContext,
                                           dataNegRespCode_u8);
            }
            else
            {
                Dcm_Prv_ProcessDataOfOneInfotype (ModeContext,
                                                  pMsgContext,
                                                  dataNegRespCode_u8);
            }
        }
        else
        {
            /* Set Negative response code 0x12 for the combination of both supported and Non supported */
            /* INFOTYPEs in single request */
            *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
            DCM_DET_ERROR(DCM_OBDMODE09_ID, DCM_E_MIXED_MODE)
        }
    }
    else
    {
        /* Invalid Request Length send NRC 0x12 */
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        DCM_DET_ERROR(DCM_OBDMODE09_ID, DCM_E_INVALID_LENGTH)
    }
    return(*dataNegRespCode_u8);
}
/**********************************************************************************************************************
 * Dcm_Prv_Mode09Pending :
 * The purpose of this function is to check and set the pending state.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
static Std_ReturnType Dcm_Prv_Mode09Pending (
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType dataRetVal_u8;

    dataRetVal_u8 = E_OK;

    /* WaitPend response  is allowed only for InfoType 0x06 */
    if(Dcm_dataDspInfoType_u8 == 0x06)
    {
        /* Set OpStatus to DCM_PENDING */
        dataRetVal_u8      = DCM_E_PENDING;
        Dcm_SrvOpstatus_u8 = DCM_PENDING;
    }
    else
    {
        if(Dcm_Dsp_VehInfoData_acs[Dcm_idxDspVehData_u16].InfoType_APIType_e == OBD_MODE9_RTE_FNC)
        {
            /* Pass the parameters DataValueBuffer and DataalueBuffersize as NULL_PTR , as the API is invoked to
               cancel the operation */
            (void)(*(Dcm_Dsp_VehInfoData_acs[Dcm_idxDspVehData_u16].GetInfotypeValueData_pf.GetInfotypeValueData_pf1))
                    (DCM_CANCEL,NULL_PTR,NULL_PTR);
        }

        /* NRC 0x12 is suppressed in DCM module */
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        /* Error code for DCM_E_PENDING */
        DCM_DET_ERROR(DCM_OBDMODE09_ID, DCM_E_RET_E_PENDING)
        /* Set OpStatus to DCM_INITIAL */
        Dcm_SrvOpstatus_u8 = DCM_INITIAL;
    }

    return (dataRetVal_u8);
}
/**********************************************************************************************************************
 * Dcm_Prv_GetVehInfoData :
 * The purpose of this function is to Call the function to get the vehinfo data.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
static Std_ReturnType Dcm_Prv_GetVehInfoData (
        Dcm_ObdMode9ContextType *ModeContext,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint8          DataValueBufferSize_u8; /* Pointer to DataValueBufferSize - Used by GetInfoType API*/
    Std_ReturnType dataRetVal_u8;

    dataRetVal_u8 = E_OK;
    /* BSW-12744 - Checking of buffer exceeds 255u is removed */
    /* BSW-12744 */
    /* [$DD_BSWCODE 30190] */
    /* Update the parameter DataValueBufferSize with the configured DcmDspVehInfoDataSize */
    DataValueBufferSize_u8 = Dcm_Dsp_VehInfoData_acs[Dcm_idxDspVehData_u16].Infodatatype_size_u8;
    /* END BSW-12744 */
    
    /* Call the function to get the vehinfo data */
    ModeContext->dataInfotypeFnResult_u8 =
            (*(Dcm_Dsp_VehInfoData_acs[Dcm_idxDspVehData_u16].GetInfotypeValueData_pf.GetInfotypeValueData_pf1))
            (Dcm_SrvOpstatus_u8,&pMsgContext->resData[pMsgContext->resDataLen],&DataValueBufferSize_u8);

    /* Data available from the software component */
    if( ModeContext->dataInfotypeFnResult_u8 == E_OK)
    {
        /* Update the result data length with the DataBufferSize returned by the API GetInfoTypeValueData */
        pMsgContext->resDataLen += (uint32)DataValueBufferSize_u8;
        /* Increment the index to the VehInfo data array */
        Dcm_idxDspVehData_u16++;
        Dcm_SrvOpstatus_u8 = DCM_INITIAL;
    }
    else if(ModeContext->dataInfotypeFnResult_u8 == DCM_E_PENDING)
    {
        dataRetVal_u8 = Dcm_Prv_Mode09Pending(dataNegRespCode_u8);
    }
    else
    {
        dataRetVal_u8 = E_NOT_OK;
        /* NRC 0x12 is suppressed in DCM module */
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        /* Set OpStatus to DCM_INITIAL */
        Dcm_SrvOpstatus_u8 = DCM_INITIAL;
        /* Error code for E_NOT_OK */
        DCM_DET_ERROR(DCM_OBDMODE09_ID, DCM_E_RET_E_NOT_OK)
    }
    return(dataRetVal_u8);
}
/**********************************************************************************************************************
 * Dcm_Prv_Mode09Running :
 * The purpose of this function is to loop through  all the data elements and read the data corresponding
 * to the veh infotype by calling the configured API.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
static Std_ReturnType Dcm_Prv_Mode09Running (
        Dcm_ObdMode9ContextType *ModeContext,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType dataReturnVal_u8 = E_NOT_OK;

    /*Loop through the data elements and read the data corresponding to the VehInfoType by calling the configured API*/
    do
    {
        /* Check if response is sufficient to hold the data and info type */
        if((pMsgContext->resMaxDataLen - pMsgContext->resDataLen) >=
                (uint32)(Dcm_Dsp_VehInfoData_acs[Dcm_idxDspVehData_u16].Infodatatype_size_u8))
        {
            dataReturnVal_u8 = Dcm_Prv_GetVehInfoData (ModeContext, pMsgContext, dataNegRespCode_u8);
        }
        else
        {
            /* NRC 0x12 is suppressed in DCM module */
            *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
            /* Set return type to E_NOT_OK to break out of the do-while loop */
            ModeContext->dataInfotypeFnResult_u8 = E_NOT_OK;
            /* Report development error "Invalid length" to DET module if the DET module is enabled */
            DCM_DET_ERROR(DCM_OBDMODE09_ID, DCM_E_INTERFACE_BUFFER_OVERFLOW)
        }
    }

    while ((Dcm_idxDspVehData_u16 <
            (Dcm_Dsp_VehInfoArray_acs[Dcm_idxDspInfoType_u8].InfoDatatypeIndex_u16+ \
                    Dcm_Dsp_VehInfoArray_acs[Dcm_idxDspInfoType_u8].NumOfInfoData_u8)) && \
            (ModeContext->dataInfotypeFnResult_u8==E_OK));

    /* If all the API for reading VehInfo data returns E_OK */
    if(ModeContext->dataInfotypeFnResult_u8 == E_OK)
    {
        /* Set the state to DCM_DSP_MODE09_INIT */
        Dcm_stDspMode09_en = DCM_DSP_MODE09_INIT;
        dataReturnVal_u8   = E_OK;
    }

    /* Update the response length */
    return(dataReturnVal_u8);
}
/**********************************************************************************************************************
 * Dcm_DcmObdMode09 :
 * The purpose of this service is to read vehicle-specific information such as VIN, calibration IDs etc.
 * Some of this information may be required by regulations and some may be desired to be reported in a
 * standard format if supported by the vehicle manufacturer.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **********************************************************************************************************************
 */
Std_ReturnType Dcm_DcmObdMode09 (
        Dcm_SrvOpStatusType OpStatus,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType          dataRetVal_u8;
    Dcm_ObdMode9ContextType ModeContext;

    /* Initialization of local variables */
    *dataNegRespCode_u8                 = 0x0u;
    ModeContext.adrTmpBuf_au8[0]        = 0x0u; /* To remove Misra warning */
    ModeContext.nrReqDataLen_u32        = (pMsgContext->reqDataLen); /* copy the requested data length
                                                                        into local variable */
    ModeContext.dataInfotypeFnResult_u8 = E_NOT_OK;
    dataRetVal_u8                       = E_OK;

    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
    removed and the one passed by the central statemachine needs to be used */
    if (OpStatus == DCM_CANCEL)
    {
        Dcm_DcmObdMode09_Ini();
    }
    else
    {
        if(Dcm_stDspMode09_en == DCM_DSP_MODE09_INIT)
        {
            *dataNegRespCode_u8 = Dcm_Prv_Mode09Init (&ModeContext,
                                                      pMsgContext,
                                                      dataNegRespCode_u8);
        }

        /* If the state is DCM_DSP_MODE09_RUNNING - Info type is found  */
        if(Dcm_stDspMode09_en == DCM_DSP_MODE09_RUNNING)
        {
            dataRetVal_u8 = Dcm_Prv_Mode09Running (&ModeContext,
                                                   pMsgContext,
                                                   dataNegRespCode_u8);
        }

        /* If negative response code is set */
        if(*dataNegRespCode_u8 != 0x0u)
        {
            dataRetVal_u8      = E_NOT_OK;
            Dcm_stDspMode09_en = DCM_DSP_MODE09_INIT;
        }
    }
    return dataRetVal_u8;
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif
