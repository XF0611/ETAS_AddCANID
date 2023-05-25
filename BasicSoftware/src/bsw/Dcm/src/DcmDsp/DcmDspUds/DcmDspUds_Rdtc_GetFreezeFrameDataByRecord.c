

#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Rdtc_Inf.h"
#include "Rte_Dcm.h"
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Rdtc_Priv.h"

#if(DCM_CFG_DSP_RDTCSUBFUNC_0x05_ENABLED != DCM_CFG_OFF)

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **************************************************************************************************
 * Dcm_Dsp_GetFreezeFrameDataByRecord :
 * The  function implements the RDTC subfunction 0x05.
 * This function will invoke the Dem APIs Dem_DcmGetDTCOfOBDFreezeFrame,Dem_DcmReadDataOfOBDFreezeFrame and
 * Dem_GetStatusOfDTC to get the freeze frame data,DTC and DTC status related to the record  number send by the tester.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 *
 **************************************************************************************************
 */
Std_ReturnType Dcm_Dsp_GetFreezeFrameDataByRecord (Dcm_SrvOpStatusType OpStatus,\
                                                   Dcm_MsgContextType *pMsgContext,\
                                                   Dcm_NegativeResponseCodeType *dataNegRespCode_u8)
{
    uint8   dataRDTCSubFunc_u8;                 /* Local variable to store sub function*/
    uint8   dataRecordNum_u8;                   /* Variable to store Record Number */
    uint8   dataRetGetDTCOfOBDFreezeFrame_u8;   /* Local Var to store return value of API Dem_DcmGetDTCOfOBDFreezeFrame*/
    uint8   dataRetGetSelectDTC_u8;             /* Local Var to store return valuse of API Dem_SelectDTC*/
    uint8   dataretReadDataOfOBDFreezeFrame_u8 = E_OK; /* Local Var to store return valuse of API Dem_DcmReadDataOfOBDFreezeFrame*/
    uint8   nrPid_u8;                           /* Number of PIDs needs to be searched in PID configuration  */


    uint16  idxPIDData_qu16;                    /* Start of data index associated with each PID */
    uint16  dataAvailableBuffLen_u16;           /* Local variable to store and send available buffer size for Dem api Dem_DcmReadDataOfOBDFreezeFrame*/
    uint16  idxDataSource_u16;                  /* Index to the data element of a PID  */
    uint16  datafillBufLen_u16;

    Dcm_MsgLenType  dataFillRespLen_u32;        /* Local variable to store the response length filled*/
    Dcm_MsgLenType  dataRemainBuffer_u32;       /* Local variable for maximum response buffer available*/
    Std_ReturnType  dataRetGetStatusOfDTC_u8;   /* Return type of Dem_GetStatusOfDTC */
    Std_ReturnType retVal_u8 = DCM_E_PENDING;                   /* Local var to to store return value*/

    uint16  DidNumber_u16;                       /* Local Var to store DID number*/
    static uint32  dataDTC_u32;                  /* Variable to store DTC */
    static uint8   stDTCStatus_u8;               /* Variable to store the DTC status */
    boolean databreakLoop_b = FALSE;


    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);
    *dataNegRespCode_u8         = 0u;


    /*Copy subfunction value from the request to local variable*/
    dataRDTCSubFunc_u8 = pMsgContext->reqData[DSP_RDTC_POSSUBFUNC];

    /*Check if the subfunction received is RDTC subfunction 0x05*/
    if(DSP_REPORT_DTC_STORED_DATA_BY_RECORD_NUMBER == dataRDTCSubFunc_u8 )
    {
        /* Check if request length is correct(max request length ) */
        if(DSP_RDTC_05_REQLEN == pMsgContext->reqDataLen)
        {
            /*Copy record number from request to local variable*/
            dataRecordNum_u8 = pMsgContext->reqData[DSP_RDTC_POSSUBFUNC +1u];

            /*store the subfunction value to response buffer and update the request length*/
            pMsgContext->resData[0] = DSP_REPORT_DTC_STORED_DATA_BY_RECORD_NUMBER;
            pMsgContext->resDataLen = 0x01;

            /*Reserve 5 bytes for storing Record number(1 byte),DTC(3 bytes)and DTC status(1 byte) in the response
                         *  buffer before passing the buffer to Dem API to fill the DataRecord*/
            dataFillRespLen_u32 = 6;

            /* TRACE [SWS_Dcm_00574] */
            /*Check if requested record number is 0x00 or not*/
            if(dataRecordNum_u8 == 0x00u)
            {
                if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFINIT)
                {
                    /* TRACE [SWS_Dcm_01193] */
                    /*Get DTC value for FrameNumber 0*/
                    dataRetGetDTCOfOBDFreezeFrame_u8 = Dem_DcmGetDTCOfOBDFreezeFrame(dataRecordNum_u8,
                                                                                     &dataDTC_u32,
                                                                                     DEM_DTC_FORMAT_UDS);
                    /*Check if DTC is read successfully*/
                    if(E_OK == dataRetGetDTCOfOBDFreezeFrame_u8)
                    {
                        /*Call Dem API with DTC for further Operations*/
                        dataRetGetSelectDTC_u8 = Dem_SelectDTC(ClientId_u8,
                                                               dataDTC_u32,
                                                               DEM_DTC_FORMAT_UDS,
                                                               DEM_DTC_ORIGIN_PRIMARY_MEMORY);

                        /*Check if DTC sent is accepted by DEM*/
                        if(E_OK == dataRetGetSelectDTC_u8)
                        {
                            Dcm_DspRDTCSubFunc_en = DSP_RDTC_GETDTCSTATUS;
                        }
                        else
                        {
                            *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetGetSelectDTC_u8);
                            retVal_u8 = E_NOT_OK;
                        }
                    }
                    /*If E_NOT_OK is returned by API Dem_DcmGetDTCOfOBDFreezeFrame */
                    else
                    {
                        /* Set Negative response */
                        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetGetDTCOfOBDFreezeFrame_u8);
                        retVal_u8 = E_NOT_OK;
                    }
                }

                /*State to get the status of selected DTC*/
                if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_GETDTCSTATUS)
                {
                    /* TRACE [SWS_Dcm_00389] */
                    dataRetGetStatusOfDTC_u8 = Dem_GetStatusOfDTC(ClientId_u8,&stDTCStatus_u8);

                    /* If the available status mask is read successfully */
                    if(E_OK == dataRetGetStatusOfDTC_u8)
                    {
                        Dcm_DspRDTCSubFunc_en = DSP_RDTC_SFFILLRESP;
                    }
                    /* If the DTC sent by the tester is not correct */
                    else if(DEM_NO_SUCH_ELEMENT == dataRetGetStatusOfDTC_u8)
                    {
                        /*When DEM_STATUS_FAILED Dcm should send an positive response with SID and subfunction alone*/
                        pMsgContext->resData[0x00] = pMsgContext->reqData[0x00];
                        /* Update the Response data length information */
                        pMsgContext->resDataLen = 0x01;
                        retVal_u8 = E_OK;
                    }
                    else if(DEM_PENDING == dataRetGetStatusOfDTC_u8)
                    {
                        /* Do nothing - remain in the same state and call the API again */
                        retVal_u8 = DCM_E_PENDING;
                    }
                    else
                    {
                        /* Set Negative response */
                        *dataNegRespCode_u8 = Dcm_Prv_SetErrorCodeForDemOperation(dataRetGetStatusOfDTC_u8);
                        retVal_u8 = E_NOT_OK;
                    }
                }

                /*State to get the Freeze frame information*/
                if(Dcm_DspRDTCSubFunc_en == DSP_RDTC_SFFILLRESP)
                {
                    /*Dem_GetStatusOfDTCreturned ok.Now fill the record number,DTC and DTC status in the response buffer*/
                    pMsgContext->resData[dataFillRespLen_u32-5u] = dataRecordNum_u8;
                    pMsgContext->resData[dataFillRespLen_u32-4u] = (uint8)(dataDTC_u32>>16u);
                    pMsgContext->resData[dataFillRespLen_u32-3u] = (uint8)(dataDTC_u32>>8u);
                    pMsgContext->resData[dataFillRespLen_u32-2u] = (uint8)(dataDTC_u32);
                    pMsgContext->resData[dataFillRespLen_u32-1u] = stDTCStatus_u8;
                    pMsgContext->resData[dataFillRespLen_u32]    = DCM_CFG_DSP_NUMPIDSUPP_MODE2;

                    /*Update response data length*/
                    pMsgContext->resDataLen = dataFillRespLen_u32;

                    dataFillRespLen_u32++;

                    for(nrPid_u8 = 0; nrPid_u8 < DCM_CFG_DSP_NUMPIDSUPP_MODE2; nrPid_u8++)
                    {
                        DidNumber_u16 = (uint16)(Dcm_Dsp_Mode2PidArray_acs[nrPid_u8].Pid_Id_u8 + 0xF400u);

                        pMsgContext->resData[dataFillRespLen_u32] = (uint8)((DidNumber_u16 >> 0x08u) & 0x00FFu);
                        dataFillRespLen_u32++;

                        pMsgContext->resData[dataFillRespLen_u32] = (uint8)(DidNumber_u16 & 0x00FFu);
                        dataFillRespLen_u32++;

                        /* Access data index of data structure Dcm_Dsp_Mode1DataPid_acs associated with each requested PID */
                        idxPIDData_qu16 = Dcm_Dsp_Mode2PidArray_acs[nrPid_u8].DataSourcePid_ArrayIndex_u16;

                        /* Loop through each of the data elements and call the configured function to read PID data */
                        for(idxDataSource_u16 = (uint16)idxPIDData_qu16;((idxDataSource_u16<(idxPIDData_qu16 + Dcm_Dsp_Mode2PidArray_acs[nrPid_u8].Num_DataSourcePids_u8)) && (dataretReadDataOfOBDFreezeFrame_u8 == E_OK));idxDataSource_u16++)
                        {
                            /* Calculate the remaining buffer length available */
                            if (pMsgContext->resMaxDataLen > dataFillRespLen_u32)
                            {
                                dataRemainBuffer_u32 = (pMsgContext->resMaxDataLen - dataFillRespLen_u32);

                                /* Get the maximum length of the data element configured */
                                dataAvailableBuffLen_u16 = \
                                        (uint8)Dcm_Dsp_Mode2DataSourcePid_acs[idxDataSource_u16].Length_data_u8;

                                /* TRACE [SWS_Dcm_00248] */
                                if(dataAvailableBuffLen_u16 > dataRemainBuffer_u32)
                                {
                                    databreakLoop_b = TRUE;
                                    *dataNegRespCode_u8 = DCM_E_RESPONSETOOLONG;
                                    retVal_u8 = E_NOT_OK;
                                }
                            }
                            /* TRACE [SWS_Dcm_00248] */
                            else
                            {
                                databreakLoop_b = TRUE;
                                *dataNegRespCode_u8 = DCM_E_RESPONSETOOLONG;
                                retVal_u8 = E_NOT_OK;
                            }



                            if(databreakLoop_b == FALSE)
                            {
                                /* TRACE [SWS_Dcm_00388] */
                                /* call the API to get the data and pass the pointer to the buffer location */
                                dataretReadDataOfOBDFreezeFrame_u8 = \
                                        Dem_DcmReadDataOfOBDFreezeFrame(Dcm_Dsp_Mode2PidArray_acs[nrPid_u8].Pid_Id_u8,\
                                                                        (uint8)(idxDataSource_u16-idxPIDData_qu16), \
                                                                        &pMsgContext->resData[dataFillRespLen_u32], \
                                                                        &dataAvailableBuffLen_u16);

                                /*Check if data is read successfully*/
                                if(E_OK == dataretReadDataOfOBDFreezeFrame_u8)
                                {
                                    /*Check if no record found*/
                                    if(dataAvailableBuffLen_u16 == 0u)
                                    {
                                        for(datafillBufLen_u16 = 0; datafillBufLen_u16 < Dcm_Dsp_Mode2DataSourcePid_acs[idxDataSource_u16].Length_data_u8 ; datafillBufLen_u16++)
                                        {
                                            pMsgContext->resData[dataFillRespLen_u32] = 0x00;
                                            dataFillRespLen_u32++;
                                            dataAvailableBuffLen_u16++;
                                        }
                                        pMsgContext->resDataLen = dataFillRespLen_u32 + dataAvailableBuffLen_u16;
                                    }
                                    else
                                    {
                                        /*Update response length with the filled data size by (Dem_DcmReadDataOfOBDFreezeFrame)
                                         *  and already filled resp length (dataFillRespLen_u32) */
                                        pMsgContext->resDataLen = dataFillRespLen_u32 + dataAvailableBuffLen_u16;

                                        /*Update index of resData (dataFillRespLen_u32) where the next data can be written*/
                                        dataFillRespLen_u32 = dataFillRespLen_u32 + dataAvailableBuffLen_u16;
                                    }

                                    retVal_u8 = E_OK;
                                }
                                /*Check if error occcured while reading the data*/
                                else
                                {
                                    *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                                    retVal_u8 = E_NOT_OK;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }

                        if(databreakLoop_b != FALSE)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                /*TRACE[SWS_Dcm_00632]*/
                /*Incorrect request message length*/
                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
            }

        }
        else
        {
            /*Incorrect request message length*/
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }
    }

    if(0x0 != (*dataNegRespCode_u8))
    {
        retVal_u8=E_NOT_OK;
    }

    return retVal_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif
