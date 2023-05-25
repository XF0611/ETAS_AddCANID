
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x03_ENABLED != DCM_CFG_OFF)



#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **************************************************************************************************
 * Dcm_Dsp_ReportDTCSnapshotRecordIdentification :
 * Report corresponding DTCs and Record numbers for all FreezeFrame records present in primary memory of ECU (0x03)
 * Response to this subfunction shall include the DTC followed by the Record number for all the records

 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */

Std_ReturnType Dcm_Dsp_ReportDTCSnapshotRecordIdentification(Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType *pMsgContext,\
                                                                        Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint32                          dataDTC_u32;                   /* Variable to store DTC */
    uint16                          nrFiltRec_u16;                 /* Local variable to store the number of filtered records */
    uint16                          idxLoop_qu16 = 0x00u;           /* Local variable for loop counter */
    uint8                           dataRecordNum_u8;              /* Variable to store Record Number */
    Dcm_MsgLenType                  nrResDataLen_u32;              /* Local variable for response length */
    Dcm_MsgLenType                  nrResMaxDataLen_u32;           /* Local variable to store max response length */
    Std_ReturnType                  dataRetGetNextFiltRec_u8     = E_OK;  /* Return Value for Dem_GetNextFilteredRecord */
    Std_ReturnType                  dataRetSetDTCFilterForRec_u8 = E_NOT_OK;  /* Return Value for Dem_SetDTCFilter */
    Std_ReturnType                  dataretVal_u8 = DCM_E_PENDING; /* Variable to store the returnType from subservice API          */

    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);
    *dataNegRespCode_u8 = 0;  /* Initialization to zero - No error */

    /* Initial state */
    if( Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT )
    {
        /* check for request length */
        if(pMsgContext->reqDataLen == DSP_RDTC_03_REQUEST_LENGTH)
        {
            /*TRACE[SWS_Dcm_00298]*/
            /* Set the DTC Filter for records in DEM */
            dataRetSetDTCFilterForRec_u8 = Dem_SetFreezeFrameRecordFilter(ClientId_u8,DEM_DTC_FORMAT_UDS,&nrFiltRec_u16);

            /* If the Filter is accepted by DEM */
            if(E_OK == dataRetSetDTCFilterForRec_u8)
            {
                pMsgContext->resData[0] = DSP_REPORT_DTC_SNAPSHOT_RECORD;
                pMsgContext->resDataLen = 0x01;
                /* Move the state to fill the response */
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;
            }
            /*TRACE[SWS_Dcm_01256]*/
            /* If Filter is not accepted by DEM */
            else
            {
                *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetSetDTCFilterForRec_u8);
                dataretVal_u8=E_NOT_OK;
            }
        }
        /* Request Length not correct */
        else
        {
            /*TRACE[SWS_Dcm_00272]*/
            /* Set Negative response */
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            dataretVal_u8=E_NOT_OK;
        }
    }

    /* State to get the DTC and record number of filtered records from DEM */
    if( Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
    {
        /* Copy the Maximum response length into local variable */
        nrResMaxDataLen_u32 = pMsgContext->resMaxDataLen;

        /* Copy the response data length to local variable */
        nrResDataLen_u32 = pMsgContext->resDataLen;

        /*TRACE[SWS_Dcm_00299]*/
        /* Loop for maximum number of records to read in a cycle */
        while((idxLoop_qu16 < DCM_CFG_RDTC_MAXNUMRECREAD) &&(dataRetGetNextFiltRec_u8 == E_OK)&&(*dataNegRespCode_u8==0))
        {
            idxLoop_qu16++;
            /* Get the  DTC and record number of the filtered record from DEM */
            dataRetGetNextFiltRec_u8 = Dem_GetNextFilteredRecord(ClientId_u8,&dataDTC_u32, &dataRecordNum_u8);

            /* If the record was filtered without any error */
            if(E_OK == dataRetGetNextFiltRec_u8)
            {
                /*TRACE[SWS_Dcm_00300]*/
                /* Check if the response buffer is available for filling the response */
                if((nrResDataLen_u32 + 0x04u) <= nrResMaxDataLen_u32)
                {
                    /* Fill the response buffer with DTC and status */
                    pMsgContext->resData[nrResDataLen_u32] = (uint8)(dataDTC_u32 >> 16u);
                    nrResDataLen_u32++;
                    pMsgContext->resData[nrResDataLen_u32] = (uint8)(dataDTC_u32 >> 8u);
                    nrResDataLen_u32++;
                    pMsgContext->resData[nrResDataLen_u32] = (uint8)(dataDTC_u32);
                    nrResDataLen_u32++;
                    pMsgContext->resData[nrResDataLen_u32] = dataRecordNum_u8;
                    nrResDataLen_u32++;
                }
                else
                {
                    /* Response buffer exceeded - set negative response code */
                    *dataNegRespCode_u8     = DCM_E_RESPONSETOOLONG;
                    dataretVal_u8=E_NOT_OK;
                }
            }
            /* If more time is needed to get the DTC number and record number of the filtered record */
            else if(DEM_PENDING == dataRetGetNextFiltRec_u8)
            {
                /* Nothing to be done */
                dataretVal_u8 = DCM_E_PENDING;
            }
            /*TRACE[SWS_Dcm_01237],
             *TRACE[SWS_Dcm_01238]*/
            /* If all the records are read completely */
            else if(DEM_NO_SUCH_ELEMENT == dataRetGetNextFiltRec_u8)
            {
                /* Reset the status to default state */
                Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFINIT;
                dataretVal_u8=E_OK;
            }
            else
            {
                /* Some problems has occurred in DEM while reading DTC number and record number of filtered record */
                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                dataretVal_u8=E_NOT_OK;
            }
        }

        /* Update the response length */
        pMsgContext->resDataLen = nrResDataLen_u32;
    }
    else
    {
        /* Do nothing */
        dataretVal_u8=E_NOT_OK;
    }

    return dataretVal_u8;
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif

