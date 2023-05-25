
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"
#include "Rte_Dcm.h"


#if(DCM_CFG_DSP_READDTCINFORMATION_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint32 Dsp_RdtcDTC_u32;       /* Variable to store DTC */
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_DspRDTCSubFuncStates_ten Dcm_DspRDTCSubFunc_en;  /* State variable for subfunction state */
#if(DCM_CFG_DSP_RDTCEXTENDEDDATA_OR_FREEZEFRAME_SUBFUNC_ENABLED != DCM_CFG_OFF)
/* State variable to indicate the state of ExtendedData and SnapshotRecord data subfunctions */
Dcm_DspRDTCSubFunctionState_en RDTCExtendedandSnapshotDataState_en;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint8 Dcm_idxDspRDTCTab_u8;         /* Variable for Table index  */
uint8 ClientId_u8;                                       /* Variable to store the DEM Client ID */
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if(DCM_CFG_DSP_RDTCEXTENDEDDATA_OR_FREEZEFRAME_SUBFUNC_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint8 Dcm_DspRDTCRecordNum_u8; /* variable to store the record number */
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if(DCM_CFG_RDTCPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_MsgLenType Dcm_DspRDTCMaxRespLen_u32;    /* Maximum response length */
Dcm_MsgLenType Dcm_DspRDTCFilledRespLen_u32;/* Response length filled */
Dcm_MsgLenType Dcm_DspTotalRespLenFilled_u32;/* Variable to store total response length filled */
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_DspNumFltDTC_u16;         /* Number of filtered DTC's */
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
boolean Dcm_DspFirstPageSent_b;       /* Whether first page is sent or not? */
boolean Dcm_DspFillZeroes_b;          /* Variable to check if zero's needs to be filled in page */
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_MsgType Dcm_DspRDTCRespBufPtr_u8;     /* Pointer to the response buffer */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 ***********************************************************************************************************************
 * Dcm_Dsp_ReadDTCInfo_Ini : In this function initialisation of RDTC service is done.
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 *
 ***********************************************************************************************************************
 */

void Dcm_Dsp_ReadDTCInfo_Ini (void)
{
    /* Reset RDTC Service state machine */
    Dcm_SrvOpstatus_u8 = DCM_INITIAL;
#if(DCM_CFG_DSP_RDTCEXTENDEDDATA_OR_FREEZEFRAME_SUBFUNC_ENABLED != DCM_CFG_OFF)
    /* Occurs when Enable DTC has not successfully finished, but already pending count
        is exhausted and general reject is sent */
    if(DCM_RDTC_SUBFUNCTION_REENABLERECORDUPDATE == RDTCExtendedandSnapshotDataState_en)
    {
        //log DET as this should not happen
        DCM_DET_ERROR(DCM_RDTC_ID,DCM_E_ENABLEDTCRECORD_FAILED);
    }
#endif

#if(DCM_CFG_RDTCPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
    /* If init function is called , update page function pointer is
       configured as NULL_PTR */
    Dcm_adrUpdatePage_pfct = NULL_PTR;
#endif
}


/**
 ***********************************************************************************************************************
 * Dcm_Prv_DspReadDTCInfoConfirmation : API used for confirmation of response
 *                                      sent for ReadDTCInformation (0x19) Service.
 * \param           dataIdContext_u8        Service Id
 * \param           dataRxPduId_u8          PDU Id on which request is Received
 * \param           dataSourceAddress_u16   Tester Source address id
 * \param           status_u8               Status of Tx confirmation function
 *
 * \retval          None
 * \seealso
 *
 ***********************************************************************************************************************
 */
void Dcm_Prv_DspReadDTCInfoConfirmation(Dcm_IdContextType dataIdContext_u8,
                                                        PduIdType dataRxPduId_u8,
                                                        uint16 dataSourceAddress_u16,
                                                        Dcm_ConfirmationStatusType status_u8)
{
#if(DCM_CFG_RDTCPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
				/* Set the state of main state machine to Initialized state */
    Dcm_Dsp_ReadDTCInfo_Ini();
#endif
    DcmAppl_DcmConfirmation(dataIdContext_u8, dataRxPduId_u8,dataSourceAddress_u16, status_u8);
}


/**
 ***********************************************************************************************************************
 * Dcm_DcmReadDTCInformation :
 *  This service allows a client to read the status of server resident Diagnostic Trouble Code (DTC) information
 *  from any server, or group of servers within a vehicle. Unless otherwise required by the particular subfunction,
 *  the server shall return all DTC information.
 *  This service allows the client to do the following:
 *  - Retrieve the number of DTCs matching a client defined DTC status mask,
 *  - Retrieve the list of all DTCs matching a client defined DTC status mask,
 *  - Retrieve the list of DTCs within a particular functional group matching a client defined DTC status mask,
 *  - Retrieve all DTCs with "permanent DTC" status.
 *
 * \param           OpStatus           : Output status of service.
 *                  pMsgContext        : Pointer to message structure.
 *                                       (input : RequestLength, Response buffer size, request bytes)
 *                                       (output: Response bytes and Response length )
 *                  dataNegRespCode_u8 : Pointer to a Byte in which to store a negative Response code
 *                                       in case of detection of an error in the request.
 *
 * \retval          E_OK               : Request was successful.
 *                  DCM_E_PENDING      : Request is not yet finished.
 *                  E_NOT_OK           : Request was not successful.
 * \seealso
 *
 ***********************************************************************************************************************
 */
/* TRACE[SWS_Dcm_00248] */
Std_ReturnType Dcm_DcmReadDTCInformation (Dcm_SrvOpStatusType OpStatus,
                                     Dcm_MsgContextType * pMsgContext,
                                     Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    uint8 dataRDTCSubFunc_u8; /* Local variable to store the subfunction */
    Std_ReturnType dataServRetval_u8;  /* Local varaible to store the return value of the service */
    /* local pointer to sub-service configuration structure*/
    const Dcm_Dsld_SubServiceType * adrSubservice_pcst;

    /* Initialization to zero - No error */
    *dataNegRespCode_u8 = 0x00;
    dataServRetval_u8 = DCM_E_PENDING;

    ClientId_u8 = Dcm_Prv_GetDemClientId(DCM_UDSCONTEXT);

    /* If OpStatus is set to DCM_CANCEL then call the ini function for resetting */
    if(OpStatus == DCM_CANCEL)
    {
        /* Call the Ini Function */
        Dcm_Dsp_ReadDTCInfo_Ini();
        /* Set the return value to E_OK as the Ini function will always be serviced  */
        dataServRetval_u8 = E_OK;
    }
    else
    {
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
        if(pMsgContext->msgAddInfo.sourceofRequest == DCM_ROE_SOURCE)
        {

            adrSubservice_pcst = Dcm_Dsld_Conf_cs.sid_table_pcs[Dcm_DsldRoe2_pcst->servicetable_Id_u8].ptr_service_table_pcs[Dcm_DsldSrvIndex_u8].ptr_subservice_table_pcs;
        }
        else
#endif
        {
            /* get the active service configuration structure */
            adrSubservice_pcst = (Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8]).ptr_subservice_table_pcs;
        }
        /* RDTC State machine */
        /* Initialization state */
        if(Dcm_SrvOpstatus_u8 == DCM_INITIAL)
        {
            /* Copy the subfunction parameter to local variable */
            dataRDTCSubFunc_u8 = pMsgContext->reqData[DSP_RDTC_POSSUBFUNC];

            /* loop through the configured subfunctions */
			Dcm_idxDspRDTCTab_u8=0;
            while((Dcm_idxDspRDTCTab_u8 < DCM_CFG_NUMRDTCSUBFUNC) )
            {
                /* If the subfunction sent from tester if found in configuration */
                if(adrSubservice_pcst[Dcm_idxDspRDTCTab_u8].subservice_id_u8 == dataRDTCSubFunc_u8)
                {
                    /* Sub function configured */
                    break;
                }
            Dcm_idxDspRDTCTab_u8++;
            }

            /* Switch the RDTC state to call service only for Dsp RDTC functions*/
            /*Check if call function for the configured subfunction is Dsp RDTC function or not */
            if(adrSubservice_pcst[Dcm_idxDspRDTCTab_u8].isDspRDTCSubFnc_b != FALSE)
            {
                /* Initialize the RDTC Subfunction state */
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;

                #if(DCM_CFG_DSP_RDTCEXTENDEDDATA_OR_FREEZEFRAME_SUBFUNC_ENABLED != DCM_CFG_OFF)
                    RDTCExtendedandSnapshotDataState_en = DCM_RDTC_SUBFUNCTION_INITIAL;
                #endif

                /* Move the RDTC state to next state where the service is called */
                Dcm_SrvOpstatus_u8 = DCM_PROCESSSERVICE;
            }
        }

        /* State to call the service for the subfunction */
        /*If RDTC state is next state where the service handler is called and
          function for call service is Dsp RDTC function or not*/
        if(Dcm_SrvOpstatus_u8 == DCM_PROCESSSERVICE)
        {
            /* Call the function for the sub function */
            dataServRetval_u8 = (*adrSubservice_pcst[Dcm_idxDspRDTCTab_u8].SubFunc_fp)(OpStatus,
                                                                                       pMsgContext,
                                                                                       dataNegRespCode_u8);
        }
        if(adrSubservice_pcst[Dcm_idxDspRDTCTab_u8].isDspRDTCSubFnc_b == FALSE)
        {
                    /* Call the function for the sub function */
                    dataServRetval_u8 = (*adrSubservice_pcst[Dcm_idxDspRDTCTab_u8].SubFunc_fp)(Dcm_ExtSrvOpStatus_u8,
                            pMsgContext,
                            dataNegRespCode_u8);
                    if(dataServRetval_u8 == DCM_E_PENDING)
                    {
                        Dcm_ExtSrvOpStatus_u8 = DCM_PENDING;
                    }
                    else
                    {
                        Dcm_ExtSrvOpStatus_u8 = DCM_INITIAL;
                    }
         }

        /* If Negative response code is set */
        /*Check if the function for call subfunction is Dsp RDTC function or not*/
        if((*dataNegRespCode_u8 != 0) && (adrSubservice_pcst[Dcm_idxDspRDTCTab_u8].isDspRDTCSubFnc_b != FALSE))
        {
            /* Initialize the RDTC Subfunction state */
            Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;

            /* Reset RDTC Service state machine */
            Dcm_SrvOpstatus_u8 = DCM_INITIAL;
        }
    }
    return dataServRetval_u8;
}


#if(DCM_CFG_RDTCPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
/**
 ***********************************************************************************************************************
 * Dcm_Dsp_RDTCUpdatePage : This function is used to update the Page length and the Response buffer pointer .
 *
 * \param           PageBufPtr    Pointer to response buffer
 *                  PageLen       Page length which can be filled
 *
 * \retval          None
 * \seealso
 *
 ***********************************************************************************************************************
 */

void Dcm_Dsp_RDTCUpdatePage(Dcm_MsgType PageBufPtr,
                                           Dcm_MsgLenType PageLen)
{
    /* If the RDTC state is DSP_RDTC_SFTXPAGE */
    if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFTXPAGE)
    {
        /* Check if the next page should be filled with DTC information or zero's */

        Dcm_DspRDTCSubFunc_en = (Dcm_DspFillZeroes_b == FALSE) ? DSP_RDTC_SFFILLRESP : DSP_RDTC_SFFILLZERO;

    }
    /* Update the Response length and the Response buffer pointer */
    Dcm_DspRDTCMaxRespLen_u32 = PageLen;
    Dcm_DspRDTCRespBufPtr_u8 = PageBufPtr;

}



/**
 ***********************************************************************************************************************
 * Dcm_Dsp_RDTCFillZero : This function is used in paged buffering to fill the  free space in  page buffer with Zeros.
 *
 * \param           RemTotalResLen    Remaining response bytes
 *
 * \retval          None
 * \seealso
 *
 ***********************************************************************************************************************
 */
/* TRACE[SWS_Dcm_00588] */

void Dcm_Dsp_RDTCFillZero(Dcm_MsgLenType RemTotalResLen )
{
    uint32_least nrResLen_qu32 ;      /* Variable pointing to location to fill zeroes */
    uint32_least dataRemBuffer_qu32;    /* Variable to store available free space in buffer */
    /*Initialize Dcm_DspFillZeroes_b is to TRUE*/
    Dcm_DspFillZeroes_b = TRUE;
    /*Calculate the remaining available buffer */
    dataRemBuffer_qu32 = Dcm_DspRDTCMaxRespLen_u32 - Dcm_DspRDTCFilledRespLen_u32;

    /* If buffer is not full */
    if(RemTotalResLen != 0u)
    {
        /* Check if the remaining response bytes is greater than the available page size */
        if(RemTotalResLen > dataRemBuffer_qu32)
        {
            RemTotalResLen = dataRemBuffer_qu32;
        }
        dataRemBuffer_qu32 = RemTotalResLen + Dcm_DspRDTCFilledRespLen_u32;

        /* Filling the page with zero's till the page is full or the total response is filled */
        for(nrResLen_qu32=Dcm_DspRDTCFilledRespLen_u32; nrResLen_qu32< dataRemBuffer_qu32; nrResLen_qu32++)
        {
            Dcm_DspRDTCRespBufPtr_u8[nrResLen_qu32] = 0x0;
        }

        /* Updating the total response length filled */
        Dcm_DspTotalRespLenFilled_u32 += RemTotalResLen;
        /* Updating the filled response length in the page */
        Dcm_DspRDTCFilledRespLen_u32 += RemTotalResLen;
    }
}


/**
 ***********************************************************************************************************************
 * Dcm_IsProtocolIPCanFD : API called to get active protocol on which the paged buffer request is received.
 * The API returns TRUE in case of CANFD/IP/SUPPLIER SPECIFIC PROTOCOLS and FALSE in other cases
 *
 * \param           None
 *
 *
 * \retval          Boolean
 * \seealso
 * \usedresources
 ***********************************************************************************************************************
 */
boolean Dcm_IsProtocolIPCanFD(void)
{
    Dcm_ProtocolType activeProtocol_u8 = DCM_NO_ACTIVE_PROTOCOL;

    boolean retValue_b = TRUE;
    /* Get the running protocol */
    (void)Dcm_GetActiveProtocol(&activeProtocol_u8);

#if(DCM_CFG_CANFD_ENABLED==DCM_CFG_OFF)
    /* Return TRUE if active protocol is over CANFD */
    if((activeProtocol_u8==DCM_UDS_ON_CAN)||(activeProtocol_u8==DCM_ACEA_ON_CAN)|| \
            /* FC_VariationPoint_START */
       (activeProtocol_u8==DCM_KWP_ON_CAN)|| (activeProtocol_u8==DCM_OBD_ON_CAN))
                                                           /* FC_VariationPoint_END */
    {
        /*Set the value for retValue_b is to FALSE*/
        retValue_b= FALSE;
    }
    else

#endif
    {
        if((activeProtocol_u8==DCM_UDS_ON_FLEXRAY)||(activeProtocol_u8==DCM_ACEA_ON_FLEXRAY)|| \
                /* FC_VariationPoint_START */
           (activeProtocol_u8==DCM_OBD_ON_FLEXRAY))
                /* FC_VariationPoint_END */
        {
            /*Set the value for retValue_b is to FALSE*/
            retValue_b= FALSE;
        }
    }
    return (retValue_b);
}

#endif
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif
#endif

