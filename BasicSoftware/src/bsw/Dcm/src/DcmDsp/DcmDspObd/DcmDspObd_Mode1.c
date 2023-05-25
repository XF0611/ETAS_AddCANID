

#include "DcmDspObd_Mode1_Inf.h"

#if (DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_OBDMODE1_ENABLED != DCM_CFG_OFF)
#include "DcmDspObd_Mode1_Priv.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/****************************************************************************************************
* Dcm_DcmObdMode01 :
* The purpose of this service is to allow read the current emission related data values,
* including analog inputs and outputs, digital inputs and outputs and system information.
* Request information includes PID value that indicates to the on board system the
* specific information requested and supported PIDs provided bit coded value for the range of PIDs.
*
* \param           pMsgContext    Pointer to message structure
*
* \retval          None
* \seealso
*
**************************************************************************************************
*/
Std_ReturnType Dcm_DcmObdMode01 (Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType * pMsgContext,Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    uint8 * adrRespBuf_pu8; /* Pointer to Response buffer                                */
    uint8 * adrReqBuf_pu8;  /* Pointer to Request buffer                                 */
#if (DCM_CFG_DSP_OBDMODE1_SINT32_SR_ENABLED != DCM_CFG_OFF)
    sint32 dataSignal_s32;
#endif
    uint32 dataPIDMaskVal_u32;      /* For supported PIDs,each PID is bit coded value of 4 bytes */
    uint32 dataCalPIDBitMask_u32;   /* Calculated Bit mask based on PID value                    */
    uint32 dataPidRunTimeMask_u32; /* For supported PIDs,each PID is bit coded value of 4 bytes */
    Dcm_MsgLenType nrResMaxDataLen_u32;   /* Maximum available response buffer                         */
    Dcm_MsgLenType nrReqDataLen_u32;      /* Request data length                                       */
    uint16_least idxPIDRes_qu16;     /* Index to response buffer for valid PIDs                   */
    uint16 idxSupportInfo_u16;/* Variable to store the index to fill support information   */
    uint16 idxDataSource_u16;/* Index to the data element of a PID                        */
#if (DCM_CFG_DSP_OBDMODE1_UINT16_SR_ENABLED != DCM_CFG_OFF)
    uint16 dataSignal_u16;
#endif
#if (DCM_CFG_DSP_OBDMODE1_SR_ENABLED != DCM_CFG_OFF)
    uint16 nrElements_u16;
    uint32 dataSignal_u32;
    uint16 idxLoop_u16;
#endif
    uint16 posnPidData_u16; /* Variable to store data position of PID to be passed to application API */
#if (DCM_CFG_DSP_OBDMODE1_SINT16_SR_ENABLED != DCM_CFG_OFF)
    sint16 dataSignal_s16;
#endif
    uint8_least nrPid_qu8;      /* Number of PIDs needs to be searched in PID configuration  */
    uint8_least idxPIDStart_qu8;      /* Start index in PID configuration                          */
    uint8_least idxPIDData_qu8;       /* Start of data index associated with each PID              */
    uint8_least nrPIDChk_qu8;      /* No: of PIDS in request to be searched in config structure */
    uint8 adrTmpBuf_au8[DCM_OBDMODE01_REQ_LEN_MAX]; /* Temporary buffer for maximum of 6 PIDs */
    uint8 idxPID_u8;         /* Pid index in bit mask configuration list                  */
    uint8 nrMultiple_u8;       /* Number of special PIDS requested are multiples of 0x20    */
    uint8 dataSupportInfoByte_u8;  /* Byte to store the support information                     */
    Std_ReturnType dataGetPIDRetVal_u8;       /* Return value for function to read PID data                */
    Std_ReturnType dataReturnValue_u8;
    boolean stPIDFound_b;                /* Flag set to 1 if requested PID is found in configuration  */
#if (DCM_CFG_DSP_OBDMODE1_UINT8_SR_ENABLED != DCM_CFG_OFF)
    uint8 dataSignal_u8;
#endif
#if (DCM_CFG_DSP_OBDMODE1_SINT8_SR_ENABLED != DCM_CFG_OFF)
    sint8 dataSignal_s8;
#endif
#if (DCM_CFG_DSP_OBDMODE1_BOOL_SR_ENABLED != DCM_CFG_OFF)
    boolean dataSignal_b;
#endif
    boolean stInfrastructureError_b;

    /* Initialisation of local variables */
#if (DCM_CFG_DSP_OBDMODE1_SINT32_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_s32 = 0x0;
#endif
#if (DCM_CFG_DSP_OBDMODE1_UINT16_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_u16 = 0x0u;
#endif
#if (DCM_CFG_DSP_OBDMODE1_SINT16_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_s16 = 0x0;
#endif
#if (DCM_CFG_DSP_OBDMODE1_UINT8_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_u8  = 0x0u;
#endif
#if (DCM_CFG_DSP_OBDMODE1_SINT8_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_s8  = 0x0;
#endif
#if (DCM_CFG_DSP_OBDMODE1_BOOL_SR_ENABLED != DCM_CFG_OFF)
    dataSignal_b   = FALSE;
#endif
#if (DCM_CFG_DSP_OBDMODE1_SR_ENABLED != DCM_CFG_OFF)
    nrElements_u16 = 0x0u;
    dataSignal_u32 = 0x0u;
    idxLoop_u16  = 0x0u;
#endif
    /* Initialisation of local variables */
    *dataNegRespCode_u8       = 0x00u;
    dataGetPIDRetVal_u8     = E_OK;
    idxPIDRes_qu16   = 0x00u;
    nrPid_qu8    = 0x00u;
    idxPIDStart_qu8    = 0x00u;
    idxPIDData_qu8     = 0x00u;
    dataCalPIDBitMask_u32 = 0x00u;                     /* To remove warning */
    adrTmpBuf_au8[0]    = 0x00u;                     /* To remove Misra warning */
    /*Initialise PIDFound value with False*/
    stPIDFound_b      = FALSE;
    nrMultiple_u8     = 0x00u;
    /* Initialise InfrastructureError value with false*/
    stInfrastructureError_b = FALSE;
    dataReturnValue_u8=E_NOT_OK;

    /*TRACE[SWS_Dcm_00243]*/
    /* Copy the requested data length into local variable */
    nrReqDataLen_u32 = (pMsgContext->reqDataLen);
    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);
    /* Check if the request Data Length is greater than 0 and less than 7 */
    if((nrReqDataLen_u32 > DCM_OBDMODE01_REQ_LEN_MIN) && (nrReqDataLen_u32 < DCM_OBDMODE01_REQ_LEN_MAX))
    {
        /* Pointer to request buffer */
        adrReqBuf_pu8 = pMsgContext->reqData;

        /* Check if it is pure multiple of 0x20 or non multiple of 0x20 */
        for(nrPIDChk_qu8=0;(nrPIDChk_qu8 < nrReqDataLen_u32);nrPIDChk_qu8++)
        {
            /* Check the multiple of 0x20 */
            if((adrReqBuf_pu8[nrPIDChk_qu8] & 0x1Fu)== 0u)
            {
                /* Increment counter if PID is pure multiple of 0x20 */
                nrMultiple_u8++;
            }

            /* Copy PID to temporary buffer */
            adrTmpBuf_au8[nrPIDChk_qu8] = adrReqBuf_pu8[nrPIDChk_qu8];
        }

        /* Request should not be a  mix of  availability and data PIDs. If only availability PID is
           requested then the counter value should be requested data length.If only PID which report
           data value is requested then the counter value should be equal to 0. */
        if((nrMultiple_u8 == 0u) || (nrMultiple_u8 == nrReqDataLen_u32 ))
        {
            /* Calculate buffer index to be passed for the data */
            adrRespBuf_pu8 = pMsgContext->resData;

            /* Set Negative response code 0x12 */
            /* NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing */
            *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

            /* Get the maximum response data length in response buffer */
            nrResMaxDataLen_u32 = pMsgContext->resMaxDataLen;

            /* Check for PIDs which are requested by tester */
            for(nrPIDChk_qu8 = 0;(nrPIDChk_qu8 < nrReqDataLen_u32);nrPIDChk_qu8++)
            {
                dataPidRunTimeMask_u32 = 0x0; /*Initialize the local variable to 0 which is to calculate the support for PIDS in the specified range*/
                /* Index for Dcm_Dsp_Mode1Bitmask_acs is calculated based on PID number index number = PID number/0x20*/
                idxPID_u8 = (uint8)((adrTmpBuf_au8[nrPIDChk_qu8]) / DCM_OBDMODE01_SUPPPID);

                /* Based on PID number,access the PID mask value corresponding to index in Dcm_Dsp_Mode1Bitmask_acs*/
                dataPIDMaskVal_u32 = Dcm_Dsp_Mode1Bitmask_acs[idxPID_u8].BitMask_u32;

                /* If only availability PID is requested then the counter value should be requested data length.
                   If only PID which report data value is requested then the counter value is equal to 0*/
                /*TRACE[SWS_Dcm_00407]*/
                if(nrMultiple_u8 != 0)
                {
                    /* Check if request buffer is sufficient */
                    if(nrResMaxDataLen_u32>=0x05u)
                    {
                        /* In case supported PIDs, based on PID number calculate the index and get masked value from
                        configuration structure Dcm_Dsp_BitMask_acs.Each supported PID provides bit coded value for
                        the range of PIDS which are supported or not.Eg: PID 00 provides bit coded value for range
                        of PID $01 to $20.For each supported PID, 4 bytes(32 PIDS) are allocated in response format */

                        /* Copy the PID number and PIDMask value only if accessed PIDMask value is not 00 00 00 00*/
                        if(dataPIDMaskVal_u32 > 0u)
                        {
                            dataPidRunTimeMask_u32=dataPIDMaskVal_u32;
                            /*This additional check is required if post build is enabled and to copy the PID number and PID mask value
                              only when it is not 00 00 00 00*/
                            if(dataPidRunTimeMask_u32 > 0u)
                            {
                                /* Copy PID to response buffer */
                                adrRespBuf_pu8[idxPIDRes_qu16] = adrTmpBuf_au8[nrPIDChk_qu8];
                                idxPIDRes_qu16++;

                                /* Increment the response buffer index by 1 since PID number is already filled */
                                /* Copy bit mask to response buffer of service */
                                adrRespBuf_pu8[idxPIDRes_qu16] = (uint8)(dataPidRunTimeMask_u32 >> 24u);
                                idxPIDRes_qu16++;
                                adrRespBuf_pu8[idxPIDRes_qu16] = (uint8)(dataPidRunTimeMask_u32 >> 16u);
                                idxPIDRes_qu16++;
                                adrRespBuf_pu8[idxPIDRes_qu16] = (uint8)(dataPidRunTimeMask_u32 >> 8u);
                                idxPIDRes_qu16++;
                                adrRespBuf_pu8[idxPIDRes_qu16] = (uint8) dataPidRunTimeMask_u32;
                                idxPIDRes_qu16++;


                                /* Send positive response code */
                                *dataNegRespCode_u8 = 0x0u;
                                /* Update the response maximum data length */
                                nrResMaxDataLen_u32 = nrResMaxDataLen_u32-0x05uL;
                            }
                        }
                    }
                    else
                    {
                        /* Report development error "Invalid length" to DET module if the DET module is enabled */
                        DCM_DET_ERROR(DCM_OBDMODE01_ID, DCM_E_INTERFACE_BUFFER_OVERFLOW)

                        /* Set Negative response code 0x12 */
                        /* NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing */
                        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

                        break;
                    }
                }

                /*TRACE[SWS_Dcm_00408]*/
                else
                {
                    /* Based on requested PID value, calculate the corresponding PID Bit mask
                    for the range 01-1F,21-3F ... For eg: PID 01 is requested then generate BIT mask
                    as 80 00 00 00 since BIT 31 is mapped to PID 01 */
                    dataCalPIDBitMask_u32 = ((uint32)1u << ((uint32)32u - ((adrTmpBuf_au8[nrPIDChk_qu8]) % 0x20)));

                    if((dataPIDMaskVal_u32 & dataCalPIDBitMask_u32) > 0u)
                    {
                        /* Requested PID is supported in ECU. Based on the index value in Dcm_Dsp_Mode1Bitmask_acs,
                        get the starting index,number of PIDS to be searched in
                        Dcm_Dsp_PidArray_acs( PID configurations) */
                        idxPIDStart_qu8 = Dcm_Dsp_Mode1Bitmask_acs[idxPID_u8].StartIndex_u8;
                        /*Update PIDFound to False*/
                        stPIDFound_b = FALSE;

                        dataGetPIDRetVal_u8 = E_OK;
                        /* Get the number of PIDS(its number of iterations) for which below for loop has to be
                        looped for. Eg: If in the range PID 01 to 1F, only 5 PIDs are supported and PID 01 is
                        requested by tester then its sufficient to search PID 01 in the range pid 01 to 1F
                        with only 5 iterations since only 5 PIDs are configured in that range*/
                        /*Check whether PIDFound is false*/
                        for(nrPid_qu8 = idxPIDStart_qu8; ((nrPid_qu8 <(idxPIDStart_qu8 +(Dcm_Dsp_Mode1Bitmask_acs[idxPID_u8].NumPids_u8))) && (stPIDFound_b == FALSE)); nrPid_qu8++)
                        {
                            if( adrTmpBuf_au8[nrPIDChk_qu8] == (Dcm_Dsp_Mode1PidArray_acs[nrPid_qu8].Pid_Id_u8))

                            {
                                /* Set the PIDFound to TRUE. This indicates PID is found in configuration structure and
                                    not required to continue executing the above for loop*/
                                /*Update PIDFound to True*/
                                stPIDFound_b = TRUE;
                                if(nrResMaxDataLen_u32>=(uint32)(Dcm_Dsp_Mode1PidArray_acs[nrPid_qu8].Pid_Size_u8+1))
                                {

                                    /* Access data index of data structure Dcm_Dsp_Mode1DataPid_acs associated with each requested PID */
                                    idxPIDData_qu8 = Dcm_Dsp_Mode1PidArray_acs[nrPid_qu8].DataSourcePid_ArrayIndex_u16;

                                    /*TRACE[SWS_Dcm_00623]*/
                                    /*TRACE[SWS_Dcm_00944]*/
                                    /* Fill the number of bytes corresponding to length of the PID with 0x00 */
                                    for(idxDataSource_u16 = (uint16)idxPIDRes_qu16;
                                            idxDataSource_u16 <(idxPIDRes_qu16 + Dcm_Dsp_Mode1PidArray_acs[nrPid_qu8].Pid_Size_u8+1u);
                                            idxDataSource_u16++)
                                    {
                                        adrRespBuf_pu8[idxDataSource_u16] = 0x0;
                                    }


                                    /* Loop through each of the data elements and call the configured function to read PID data */
                                    for(idxDataSource_u16= (uint16)idxPIDData_qu8;
                                            ((idxDataSource_u16<(idxPIDData_qu8+Dcm_Dsp_Mode1PidArray_acs[nrPid_qu8].Num_DataSourcePids_u8))&&(dataGetPIDRetVal_u8 == E_OK));
                                            idxDataSource_u16++)
                                    {

                                        /* If the configured function has a synchronous behaviour */
                                        if((Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].PidUsePort == OBD_USE_DATA_SYNCH_FNC)||
                                                (Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].PidUsePort == OBD_USE_DATA_SYNCH_CLIENT_SERVER))
                                        {
                                            /* Calculate the position where the PID data needs to be filled for the current PID data source */
                                            posnPidData_u16=(uint16)(idxPIDRes_qu16+(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Pos_data_u16/(uint16)8)+(uint16)1);
                                            /* Increment the response buffer by one since PID number of 1byte is stored */
                                            /* Call corresponding function to read the PID data and pass the pointer to response buffer from where data is to be filled for the PID */
                                            /* The application has to take care of filling the data( correct number of bits) in the correct positions configured */
                                            dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue1_pf))
                                                                                                       (&adrRespBuf_pu8[posnPidData_u16]);

                                        }
                                        else
                                        {
#if (DCM_CFG_DSP_OBDMODE1_SR_ENABLED != DCM_CFG_OFF)
                                            /* Check if the data is available via Sender receiver interface */
                                            if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].PidUsePort == OBD_USE_DATA_SENDER_RECEIVER)
                                            {
                                                /* Signal is of uint8 type */
                                                if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8 == DCM_UINT8)
                                                {
#if (DCM_CFG_DSP_OBDMODE1_UINT8_SR_ENABLED != DCM_CFG_OFF)
                                                    nrElements_u16 = (uint16)((uint32)Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16/8u);

                                                    if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16 == 8u)
                                                    {   /* Read uint8 signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue1_pf))(&dataSignal_u8);
                                                        Dcm_ObdArraySignal_au8[0] = dataSignal_u8;
                                                    }
                                                    else
                                                    {   /* Read uint8 array of signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue1_pf))(Dcm_ObdArraySignal_au8);
                                                    }
                                                    /* Copy the signals to Global buffer */
                                                    for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(dataGetPIDRetVal_u8 == E_OK)); idxLoop_u16++)
                                                    {
                                                        dataSignal_u32 = (uint32)Dcm_ObdArraySignal_au8[idxLoop_u16];
                                                        /* Store the read signal before proceeding to next signal */
                                                        /*TRACE[SWS_Dcm_00718]*/
                                                        Dcm_StoreSignal(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8,
                                                                (uint16)(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Pos_data_u16+(uint16)(idxLoop_u16*8u)),
                                                                &adrRespBuf_pu8[idxPIDRes_qu16 + 1u], dataSignal_u32,
                                                                Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataEndianness_u8);
                                                    }
#endif
                                                }
#if (DCM_CFG_DSP_OBDMODE1_UINT16_SR_ENABLED != DCM_CFG_OFF)
                                                /* Signal is of uint16 type */
                                                else if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8 == DCM_UINT16)
                                                {
                                                    nrElements_u16 = (uint16)(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16/16);

                                                    if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16 == 16)
                                                    {   /* Read uint16 signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue2_pf))(&dataSignal_u16);
                                                        Dcm_ObdArraySignal_au16[0] = dataSignal_u16;
                                                    }
                                                    else
                                                    {   /* Read uint16 array of signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue2_pf))(Dcm_ObdArraySignal_au16);
                                                    }
                                                    /* Copy the signals to Global buffer */
                                                    for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(dataGetPIDRetVal_u8 == E_OK)); idxLoop_u16++)
                                                    {
                                                        dataSignal_u32 = (uint32)Dcm_ObdArraySignal_au16[idxLoop_u16];
                                                        /* Store the read signal before proceeding to next signal */
                                                        Dcm_StoreSignal(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8,
                                                                (uint16)((Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Pos_data_u16+(uint16)(idxLoop_u16*16u))),
                                                                &adrRespBuf_pu8[idxPIDRes_qu16 + 1u], dataSignal_u32,
                                                                Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataEndianness_u8);
                                                    }
                                                }
#endif
#if (DCM_CFG_DSP_OBDMODE1_UINT32_SR_ENABLED != DCM_CFG_OFF)
                                                /* Signal is of uint32 type */
                                                else if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8 == DCM_UINT32)
                                                {
                                                    nrElements_u16 = (uint16)(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16/32u);

                                                    if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16 == 32u)
                                                    {   /* Read uint32 signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue3_pf))(&dataSignal_u32);
                                                        Dcm_ObdArraySignal_au32[0] = dataSignal_u32;
                                                    }
                                                    else
                                                    {   /* Read uint32 array of signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue3_pf))(Dcm_ObdArraySignal_au32);
                                                    }
                                                    /* Copy the signals to Global buffer */
                                                    for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(dataGetPIDRetVal_u8 == E_OK)); idxLoop_u16++)
                                                    {
                                                        dataSignal_u32 = Dcm_ObdArraySignal_au32[idxLoop_u16];
                                                        /* Store the read signal before proceeding to next signal */
                                                        Dcm_StoreSignal(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8,
                                                                (uint16)((Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Pos_data_u16+(uint16)(idxLoop_u16*32u))),
                                                                &adrRespBuf_pu8[idxPIDRes_qu16 + 1u], dataSignal_u32,
                                                                Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataEndianness_u8);
                                                    }
                                                }
#endif
#if (DCM_CFG_DSP_OBDMODE1_SINT8_SR_ENABLED != DCM_CFG_OFF)
                                                /* Signal is of Sint8 type */
                                                else if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8 == DCM_SINT8)
                                                {
                                                    nrElements_u16 = (uint16)(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16/8u);

                                                    if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16 == 8u)
                                                    {   /* Read sint8 signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue4_pf))(&dataSignal_s8);
                                                        Dcm_ObdArraySignal_as8[0] = dataSignal_s8;
                                                    }
                                                    else
                                                    {   /* Read sint8 array of signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue4_pf))(Dcm_ObdArraySignal_as8);
                                                    }
                                                    /* Copy the signals to Global buffer */
                                                    for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(dataGetPIDRetVal_u8 == E_OK)); idxLoop_u16++)
                                                    {
                                                        dataSignal_u32 = (uint32)Dcm_ObdArraySignal_as8[idxLoop_u16];
                                                        /* Store the read signal before proceeding to next signal */
                                                        Dcm_StoreSignal(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8,
                                                                (uint16)((Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Pos_data_u16+(uint16)(idxLoop_u16*8u))),
                                                                &adrRespBuf_pu8[idxPIDRes_qu16 + 1u], dataSignal_u32,
                                                                Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataEndianness_u8);
                                                    }
                                                }
#endif
#if (DCM_CFG_DSP_OBDMODE1_SINT16_SR_ENABLED != DCM_CFG_OFF)
                                                /* Signal is of sint16 type */
                                                else if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8 == DCM_SINT16)
                                                {
                                                    nrElements_u16 = (uint16)(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16/16u);

                                                    if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16 == 16u)
                                                    {   /* Read sint16 signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue5_pf))(&dataSignal_s16);
                                                        Dcm_ObdArraySignal_as16[0] = dataSignal_s16;
                                                    }
                                                    else
                                                    {   /* Read sint16 array of signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue5_pf))(Dcm_ObdArraySignal_as16);
                                                    }
                                                    /* Copy the signals to Global buffer */
                                                    for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(dataGetPIDRetVal_u8 == E_OK)); idxLoop_u16++)
                                                    {
                                                        dataSignal_u32 = (uint32)Dcm_ObdArraySignal_as16[idxLoop_u16];
                                                        /* Store the read signal before proceeding to next signal */
                                                        Dcm_StoreSignal(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8,
                                                                (uint16)((Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Pos_data_u16+(uint16)(idxLoop_u16*16u))),
                                                                &adrRespBuf_pu8[idxPIDRes_qu16 + 1u], dataSignal_u32,
                                                                Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataEndianness_u8);
                                                    }
                                                }
#endif
#if (DCM_CFG_DSP_OBDMODE1_SINT32_SR_ENABLED != DCM_CFG_OFF)
                                                /* Signal is of sint32 type */
                                                else if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8 == DCM_SINT32)
                                                {
                                                    nrElements_u16 = (uint16)(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16/32u);

                                                    if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Length_data_u16 == 32u)
                                                    {   /* Read sint32 signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue6_pf))(&dataSignal_s32);
                                                        Dcm_ObdArraySignal_as32[0] = dataSignal_s32;
                                                    }
                                                    else
                                                    {   /* Read sint16 array of signal */
                                                        dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue6_pf))(Dcm_ObdArraySignal_as32);
                                                    }
                                                    /* Copy the signals to Global buffer */
                                                    for(idxLoop_u16=0; ((idxLoop_u16 < nrElements_u16)&&(dataGetPIDRetVal_u8 == E_OK)); idxLoop_u16++)
                                                    {
                                                        dataSignal_u32 = (uint32)Dcm_ObdArraySignal_as32[idxLoop_u16];
                                                        /* Store the read signal before proceeding to next signal */
                                                        Dcm_StoreSignal(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8,
                                                                (uint16)((Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Pos_data_u16+(uint16)(idxLoop_u16*32u))),
                                                                &adrRespBuf_pu8[idxPIDRes_qu16 + 1u], dataSignal_u32,
                                                                Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataEndianness_u8);
                                                    }
                                                }
#endif
#if (DCM_CFG_DSP_OBDMODE1_BOOL_SR_ENABLED != DCM_CFG_OFF)

                                                /* Signal is of bool type */
                                                else if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8 == DCM_BOOLEAN)
                                                {
                                                    dataGetPIDRetVal_u8 = (*(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].GetPIDvalue_pf.GetPIDvalue7_pf))(&dataSignal_b);
                                                    dataSignal_u32 = 0x0u;

                                                    if(FALSE != dataSignal_b)
                                                    {
                                                        dataSignal_u32 = (uint32)0x01;
                                                    }
                                                    /* Store the read signal before proceeding to next signal */
                                                    Dcm_StoreSignal(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataType_u8,
                                                            (Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].Pos_data_u16),
                                                            &adrRespBuf_pu8[idxPIDRes_qu16 + 1u], dataSignal_u32,
                                                            Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].dataEndianness_u8);
                                                }
#endif
                                                else
                                                {
                                                    /* Dummy else - Code will not reach here */
                                                }
                                            }
#endif
                                        }
                                        /* Check for infrastructure errors in case of rte*/
                                        if((Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].PidUsePort == OBD_USE_DATA_SYNCH_CLIENT_SERVER) || (Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].PidUsePort == OBD_USE_DATA_SENDER_RECEIVER))
                                        {
                                            if (Dcm_IsInfrastructureErrorPresent_b(dataGetPIDRetVal_u8) != FALSE)
                                            {
                                                /*Update InfrastructureError to True*/
                                                stInfrastructureError_b = TRUE;
                                            }
                                        }
                                    }
                                    /* If all data elements returns E_OK */
                                    if(dataGetPIDRetVal_u8==E_OK)
                                    {
                                        /* Copy PID from temporary buffer to response buffer */
                                        adrRespBuf_pu8[idxPIDRes_qu16] = adrTmpBuf_au8[nrPIDChk_qu8];

                                        /* Increment the response buffer index */
                                        idxPIDRes_qu16++;

                                        /*TRACE[SWS_Dcm_00621]*/
                                        /* Loop through the support information of each data element of the PID and fill the support information bytes */
                                        for(idxDataSource_u16=(uint16)idxPIDData_qu8;idxDataSource_u16<(idxPIDData_qu8+Dcm_Dsp_Mode1PidArray_acs[nrPid_qu8].Num_DataSourcePids_u8);idxDataSource_u16++)
                                        {
                                            /* If support information is configured for the PID data */
                                            if(Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].ptr_supportinfo_pcs!=NULL_PTR)
                                            {
                                                /* Calculate the support information byte corresponding to support information bit configured.
                                                     The byte calculated will be bitwise ORed with the byte in response where the support info bit
                                                     needs to be set  */
                                                /* Support Info bit mask */
                                                /*TRACE[SWS_Dcm_00622]*/
                                                dataSupportInfoByte_u8 = (uint8)(0x01u<< ((Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].SupportInfoBit_u8)%8u));

                                                /* Calculate the index of byte in the response where the support information bit is to be set*/
                                                idxSupportInfo_u16 = (uint16)(idxPIDRes_qu16 +
                                                        Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].ptr_supportinfo_pcs->SupportInfoPos_u8 +
                                                        (uint16)((Dcm_Dsp_Mode1DataSourcePid_acs[idxDataSource_u16].SupportInfoBit_u8)/8u));

                                                /* Fill the support information in the response */
                                                adrRespBuf_pu8[idxSupportInfo_u16]= (adrRespBuf_pu8[idxSupportInfo_u16] | dataSupportInfoByte_u8);
                                            }
                                        }
                                        /* Increment the buffer length by configured data length*/
                                        idxPIDRes_qu16 = (idxPIDRes_qu16 +(uint16)Dcm_Dsp_Mode1PidArray_acs[nrPid_qu8].Pid_Size_u8);

                                        /* Update the maximum available response buffer size */
                                        nrResMaxDataLen_u32 = (Dcm_MsgLenType) (nrResMaxDataLen_u32 - (1u + (Dcm_MsgLenType)Dcm_Dsp_Mode1PidArray_acs[nrPid_qu8].Pid_Size_u8));

                                        /* Reset the negative response code to 0x00 */
                                        *dataNegRespCode_u8   = 0x0u;
                                    }

                                    /*Check whether InfrastructureError is True*/
                                    else if(stInfrastructureError_b!=FALSE)
                                    {
                                        /* Report development error "DCM_E_RET_E_INFRASTRUCTURE_ERROR" to DET module if the DET module is enabled */
                                        DCM_DET_ERROR(DCM_OBDMODE01_ID, DCM_E_RET_E_INFRASTRUCTURE_ERROR)
                                                    /*Update InfrastructureError to False */
                                                    stInfrastructureError_b=FALSE;
                                    }
                                    /* For any other return value */
                                    else
                                    {
                                        /* Report development error "E_NOT_OK" to DET module if the DET module is enabled */
                                        DCM_DET_ERROR(DCM_OBDMODE01_ID, DCM_E_RET_E_NOT_OK)
                                    }
                                }
                                else
                                {
                                    /* Set Negative response code 0x12 */
                                    /* NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing */
                                    *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

                                    /* Report development error "DCM_E_INTERFACE_BUFFER_OVERFLOW" to DET module if the DET module is enabled */
                                    DCM_DET_ERROR(DCM_OBDMODE01_ID, DCM_E_INTERFACE_BUFFER_OVERFLOW)
                                    /* Set index to request data length to exit out of the for loop */

                                    nrReqDataLen_u32=nrPIDChk_qu8;

                                    break;
                                }
                            }
                        }  /* End of for loop */
                    }
                }
            } /* End of for loop */
            /* Set the response length for requested PID */
            /* In case availability PID, response length = pid number + 4 data bytes for each requested PID  */
            /* In case of PID which report data value, response length = pid number + data length configured in
               data source configuration structure Dcm_Dsp_Mode1DataSourcePid_acs */
            pMsgContext->resDataLen = idxPIDRes_qu16;
        }
        /*TRACE[SWS_Dcm_00943]*/
        else
        {
            /*Set Negative response code 0x12 for the combination of both supported PID
            and PID which report data value in single request format
            NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing*/
            *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

            /* Report development error "Mixed mode" to DET module if the DET module is enabled */
            DCM_DET_ERROR(DCM_OBDMODE01_ID, DCM_E_MIXED_MODE)
        }
    }
    else
    {
        /* Requested data length is not greater than 1 or less than 7 then Send Negative response code Request
        Length Invalid NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing */
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

        /* Report development error "Invalid length" to DET module if the DET module is enabled */
        DCM_DET_ERROR(DCM_OBDMODE01_ID, DCM_E_INVALID_LENGTH)
    }

  /* Is there any failure occurred? */
    if(*dataNegRespCode_u8 != 0)
    {
        dataReturnValue_u8=E_NOT_OK;
    }
    else{
        dataReturnValue_u8=E_OK;
    }

    return dataReturnValue_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif

