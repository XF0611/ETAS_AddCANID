
#include "DcmDspObd_Mode2_Priv.h"

#if (DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_OBDMODE2_ENABLED != DCM_CFG_OFF)



#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**********************************************************************************************************************
* Dcm_Prv_ReadDataOfOBDFreezeFrame :
This function is used to read the data of the OBD freeze frame which are not avaialability PID's and PID $02.
*
* \param           dataNegRespCode_u8
*
* \retval          None
* \seealso
*
***********************************************************************************************************************
*/
static void Dcm_Prv_ReadDataOfOBDFreezeFrame (
        Dcm_ObdContextType *ModeContext,
        const uint8 adrTmpBuf_au8[],
        uint8* adrRespBuf_pu8,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint16      idxDataSource_u16;/* Index to the data element of a PID */
    uint16      nrBufSize_u16; /* Size of response buffer size          */
    uint8_least idxPIDData_qu8;

    idxPIDData_qu8    = 0x00;

    /* Access data index of data structure Dcm_Dsp_Mode1DataPid_acs associated with each requested PID */
    idxPIDData_qu8 = Dcm_Dsp_Mode2PidArray_acs[ModeContext->nrPid_u8].DataSourcePid_ArrayIndex_u16;

    /* Fill the number of bytes corresponding to length of the PID with fill bytes */
    for(idxDataSource_u16=(uint16)(ModeContext->nrResDataLen_u32+2uL);
            idxDataSource_u16<(ModeContext->nrResDataLen_u32+Dcm_Dsp_Mode2PidArray_acs[ModeContext->nrPid_u8].Pid_Size_u8+2u);
            idxDataSource_u16++)
    {
        adrRespBuf_pu8[idxDataSource_u16] = 0x00;
    }
    /* Loop through each of the data elements and call the configured function to read PID data */
    for(idxDataSource_u16 = (uint16)idxPIDData_qu8;
            ((idxDataSource_u16<(idxPIDData_qu8+Dcm_Dsp_Mode2PidArray_acs[ModeContext->nrPid_u8].Num_DataSourcePids_u8)) && (ModeContext->dataRetGet_u8 == E_OK));
            idxDataSource_u16++)
    {
        /* Get the maximum length of the data element configured */
        nrBufSize_u16 = (uint8)Dcm_Dsp_Mode2DataSourcePid_acs[idxDataSource_u16].Length_data_u8;

        /* call the API to get the data and pass the pointer to the buffer location */
        ModeContext->dataRetGet_u8 =
                Dem_DcmReadDataOfOBDFreezeFrame(adrTmpBuf_au8[ModeContext->nrPIDChk_qu8],
                (uint8)(idxDataSource_u16-idxPIDData_qu8),
                &adrRespBuf_pu8[ModeContext->nrResDataLen_u32 +Dcm_Dsp_Mode2DataSourcePid_acs[idxDataSource_u16].Pos_data_u8+2u],
                &nrBufSize_u16);
        /* Check if API returns E_OK */
        if(ModeContext->dataRetGet_u8 == E_OK)
        {
            /* Check if length of data written is greater than the allowed length */
            if(nrBufSize_u16 > Dcm_Dsp_Mode2DataSourcePid_acs[idxDataSource_u16].Length_data_u8)
            {
                /* Set the return value to E_NOT_OK */
                ModeContext->dataRetGet_u8 = E_NOT_OK;
            }
        }
    }
    if(ModeContext->dataRetGet_u8 == E_OK)
    {
        /* Copy the PID Number from temporary buffer to response buffer */
        adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = adrTmpBuf_au8[ModeContext->nrPIDChk_qu8];
        ModeContext->nrResDataLen_u32++;

        /* Fill the freeze frame number as 0x00 */
        adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = 0x00;
        ModeContext->nrResDataLen_u32++;

        /* Increment the index of response buffer by    PID number(1byte)+freeze frame number(1byte)+Bufsize_u8
         nrBufSize_u16 is length of data bytes updated into response buffer.This variable is passed as input
         parameter to DEM function with maximum available response buffer size & DEM function returns the
         exact data value filled in response buffer which is updated in nrBufSize_u16 only if the return value is E_OK*/
        ModeContext->nrResDataLen_u32 =
                ModeContext->nrResDataLen_u32 + Dcm_Dsp_Mode2PidArray_acs[ModeContext->nrPid_u8].Pid_Size_u8;

        /* Update the maximum available response buffer size */
        ModeContext->nrResMaxDataLen_u32 =
                ModeContext->nrResMaxDataLen_u32 - (2uL + (Dcm_MsgLenType)Dcm_Dsp_Mode2PidArray_acs[ModeContext->nrPid_u8].Pid_Size_u8);

        /* Send positive response code */
        *dataNegRespCode_u8 = 0u;
    }
    else
    {
        /* do nothing */
    }
}
/**********************************************************************************************************************
* Dcm_Prv_specialPID02 :
This function Check whether special PID 02 which reports DTC number is requested.
*
* \param           dataNegRespCode_u8
*
* \retval          None
* \seealso
*
***********************************************************************************************************************
*/
static void Dcm_Prv_specialPid02 (
        Dcm_ObdContextType *ModeContext,
        uint8* adrRespBuf_pu8,
        const uint8 adrTmpBuf_au8[],
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    if(ModeContext->nrResMaxDataLen_u32 >= 0x04uL)
    {
        /* Copy the PID 02 number from temporary buffer into response buffer */
        adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = adrTmpBuf_au8[ModeContext->nrPIDChk_qu8];
        ModeContext->nrResDataLen_u32++;

        /* Fill the freeze frame number as 0x00 */
        adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = 0x00;
        ModeContext->nrResDataLen_u32++;

        /* In case if Dem interface function Dem_DcmGetDTCOfOBDFreezeFrame()
         returns E_OK then DTCnumber is available in parameter nrDTC_u32.
         nrDTC_u32    3 (MSB)       2       1       0(LSB)
         ---         DTC high  DTC Low  ----
         byte     byte

         byte 2 of nrDTC_u32 holds DTC high byte
         byte 1 of nrDTC_u32 holds DTC low byte */

        adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = (uint8)(ModeContext->nrDTC_u32 >> 16u);
        ModeContext->nrResDataLen_u32++;
        adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = (uint8)(ModeContext->nrDTC_u32 >> 8u);
        ModeContext->nrResDataLen_u32++;

        /* Update the remaining maximum response data length of response buffer*/
        ModeContext->nrResMaxDataLen_u32 = ModeContext->nrResMaxDataLen_u32 - 0x04uL;

        /* Send positive response code */
        *dataNegRespCode_u8 = 0u;
    }
    else
    {
        /* Set Negative response code 0x12
         NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing*/
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

        /* Break out of the for loop in case of buffer overflow */
        ModeContext->nrPIDValid_qu8 = ModeContext->nrPIDChk_qu8;
    }
}
/**********************************************************************************************************************
* Dcm_Prv_OtherPid :
This function checks the service $02 request with a
PID that is not an "availability PID" and is not $02, the Dcm shall call
Dem_DcmReadDataOfOBDFreezeFrame() for every data of the PID.
*
* \param           dataNegRespCode_u8
*
* \retval          None
* \seealso
*
***********************************************************************************************************************
*/
static void Dcm_Prv_OtherPid (
        Dcm_ObdContextType *ModeContext,
        uint8* adrRespBuf_pu8,
        const uint8 adrTmpBuf_au8[],
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    /* Byte 2,1 gives the DTC number high byte & low byte */
    if((ModeContext->nrDTC_u32 & 0xFFFF00uL) > 0uL)
    {
        /* If the DTCNum is non zero then freeze frame data is stored in memory */

        /* If the requested PID value is other than PID 02 do the following
         get the maximum available response data length in response buffer */
        /*if the available response data length is greater than 255 then pass the
         bufsize as 255bytes to DEM function otherwise pass the remaining buffer
         size to bufsize */
        /* Get the start index from the bit mask array */
        ModeContext->idxPIDStart_qu8 = Dcm_Dsp_Mode2Bitmask_acs[ModeContext->idxPID_u8].StartIndex_u8;

        /* Set the flag to FALSE to indicate the PID has not been found */
        /*Update PIDFound with False*/
        ModeContext->isPIDFound_b  = FALSE;
        ModeContext->dataRetGet_u8 = E_OK;

        /* Get the number of PIDS(its number of iterations) for which below for loop has to be
         looped for. Eg: If in the range PID 01 to 1F, only 5 PIDs are supported and PID 01 is
         requested by tester then its sufficient to search PID 01 in the range pid 01 to 1F
         with only 5 iterations since only 5 PIDs are configured in that range*/
        for(ModeContext->nrPid_u8 = ModeContext->idxPIDStart_qu8;
                ((ModeContext->nrPid_u8 <(ModeContext->idxPIDStart_qu8 +(Dcm_Dsp_Mode2Bitmask_acs[ModeContext->idxPID_u8].NumPids_u8))) && (ModeContext->isPIDFound_b == FALSE));
                ModeContext->nrPid_u8++)
        {
            /* If the PID is configured */
            if(adrTmpBuf_au8[ModeContext->nrPIDChk_qu8] == (Dcm_Dsp_Mode2PidArray_acs[ModeContext->nrPid_u8].Pid_Id_u8))
            {
                /* Set the PIDFound to TRUE. This indicates PID is found in configuration structure and
                 not required to continue executing the above for loop*/
                ModeContext->isPIDFound_b = TRUE;

                /* If response buffer is sufficient to hold the PID data, PID and freeze frame requested */
                if(ModeContext->nrResMaxDataLen_u32 >=
                        (uint32)(Dcm_Dsp_Mode2PidArray_acs[ModeContext->nrPid_u8].Pid_Size_u8+2))
                {
                    Dcm_Prv_ReadDataOfOBDFreezeFrame(ModeContext,
                                                     adrTmpBuf_au8,
                                                     adrRespBuf_pu8,
                                                     dataNegRespCode_u8);
                }
                else
                {
                    /* Set Negative response code 0x12
                     NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing*/
                    *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

                    /* Update the index to break out of the for loop */
                    ModeContext->nrPIDValid_qu8 = ModeContext->nrPIDChk_qu8;
                    break;
                }
            }
        } /* End of for loop */
    }
}
/**********************************************************************************************************************
* Dcm_Prv_AvailabiltyPid :
This function Checks whether requested PIDs are availability PIDS 0x00,0x20,0x40,0x60,0x80,0xa0,0xc0,0xe0 and process
these PID's and fill the response in the response buffer.
*
* \param           dataNegRespCode_u8
*
* \retval          None
* \seealso
*
***********************************************************************************************************************
*/
static void Dcm_Prv_AvailabiltyPid (
        Dcm_ObdContextType *ModeContext,
        uint8* adrRespBuf_pu8,
        const uint8 adrTmpBuf_au8[],
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    if(ModeContext->dataPIDBitMask_u32 > 0u)
    {
        /* Check if the response buffer is sufficient to hold the PID, freeze frame and PID bit mask  */
        if(ModeContext->nrResMaxDataLen_u32 >= 0x06uL)
        {
            /* Copy PID to response buffer */
            adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = adrTmpBuf_au8[ModeContext->nrPIDChk_qu8];
            ModeContext->nrResDataLen_u32++;
            /* Load the freeze frame number as 0x00 to next byte in response data buffer immediately after PID number */
            adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = 0x00;
            ModeContext->nrResDataLen_u32++;

            /* Copy bit mask to response buffer of service */
            adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = (uint8)(ModeContext->dataPIDBitMask_u32 >> 24u);
            ModeContext->nrResDataLen_u32++;
            adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = (uint8)(ModeContext->dataPIDBitMask_u32 >> 16u);
            ModeContext->nrResDataLen_u32++;
            adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = (uint8)(ModeContext->dataPIDBitMask_u32 >> 8u);
            ModeContext->nrResDataLen_u32++;
            adrRespBuf_pu8[ModeContext->nrResDataLen_u32] = (uint8) ModeContext->dataPIDBitMask_u32;
            ModeContext->nrResDataLen_u32++;

            /* Update the maximum response data length */
            ModeContext->nrResMaxDataLen_u32 = ModeContext->nrResMaxDataLen_u32 - 0x06uL;

            /* Send positive response code */
            *dataNegRespCode_u8 = 0x0u;
        }
        /* If response buffer is insufficient */
        else
        {
            /* Set Negative response code 0x12
             NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing*/
            *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        }
    }
}
/**********************************************************************************************************************
* Dcm_Prv_NonAvailabiltyPid :
This function checks if the requested PID is to obtain freeze frame data, Eg : PID 0x02,0x03 etc.
*
* \param           pMsgContext    Pointer to message structure
*
* \retval          None
* \seealso
*
***********************************************************************************************************************
*/
static void Dcm_Prv_NonAvailabiltyPid (
        Dcm_ObdContextType *ModeContext,
        uint8* adrRespBuf_pu8,
        const uint8 adrTmpBuf_au8[],
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    /* Calculate the Bit mask based on requested PID For eg: PID 01 is requested, this PID is within
     the range 01 to 1f hence availability PID is PID 00 & its Bit mask is of 4bytes in which BIT 31
     is mapped to PID 01.hence calculated bit mask 0x80000000 for PID 01*/
     ModeContext->dataCalBitMask_u32 =
             ((uint32)1u << ((uint32)32u -((adrTmpBuf_au8[ModeContext->nrPIDChk_qu8]) % 0x20)));

    /* If the requested PID is supported */
    if((ModeContext->dataPIDBitMask_u32 & ModeContext->dataCalBitMask_u32)> 0uL)
    {
        /* If PID requested is $02 if DTC that caused the freeze frame is to be obtained */
        /*Check whether flgGetDTCNum is True*/
        if((DCM_DSPMODE02_PID02 == adrTmpBuf_au8[ModeContext->nrPIDChk_qu8])||
           (ModeContext->flgGetDTCNum_b != FALSE))
        {
            /* If requested PID is 02, then below DEM API needs to be called to get
             the DTC number in case of other PID,the below DEM API needs to be executed only
             once in order to obtain DTC number for eg: Request 07 02 03 00 04 00 05 00,
             for this request DEM API function can be
             called only once in order to obtain DTC number*/
            /*Update flgGetDTCNum with False */
            ModeContext->flgGetDTCNum_b = FALSE;

            /* Call the DEM interface function to find whether there is DTC number stored
             in error memory for which freeze frame data is stored (either PID 02 is requested
             or not) */
            /*PID 02 is requested (read DTC which caused freeze frame to be stored),
             Dem_DcmGetDTCOfOBDFreezeFrame( ) is called and the parameter passed is record
             number (its always 00)*/
            if(Dem_DcmGetDTCOfOBDFreezeFrame(0x00, &ModeContext->nrDTC_u32, DEM_DTC_FORMAT_OBD) != E_OK)
            {
                ModeContext->nrDTC_u32 = 0x00;
            }
        }
        /* DEM API function has returned value E_OK or E_NOT_OK
         in case of PID 02 is supported then send positive response with DTC number in
         nrDTC_u32.in case of other PIDS, if DTC number is non zero then use DEM API to get
         freeze frame data else send no response for other PID (Since DTC number is zero which
         indicates no freeze frame data is available */

        /* Check whether special PID 02 which reports DTC number is requested */
        if(DCM_DSPMODE02_PID02 == adrTmpBuf_au8[ModeContext->nrPIDChk_qu8])
        {
            Dcm_Prv_specialPid02(ModeContext,
                                 adrRespBuf_pu8,
                                 adrTmpBuf_au8,
                                 dataNegRespCode_u8);
        }
        else
        {
            Dcm_Prv_OtherPid(ModeContext,
                             adrRespBuf_pu8,
                             adrTmpBuf_au8,
                             dataNegRespCode_u8);
        }
    }
}
/**********************************************************************************************************************
* Dcm_DcmObdMode02 :
The purpose of this service is to allow access to emission-related data values in a freeze frame.
The request message includes parameter identification (PID) value that indicates to the on-board system the specific
information requested.It's not necessarily to contain the same current data values as the required freeze frame data.
When the tester request for freeze frame data through mode 02. The ECU(s) shall respond to this message by
transmitting the requested data value stored by the system at the time of fault occurrence in error memory.
All data values returned for sensor readings shall be actual stored readings, not default or substitute values used
by the system be-cause of a fault with that sensor.
*
* \param           pMsgContext    Pointer to message structure
*
* \retval          None
* \seealso
*
***********************************************************************************************************************
*/
Std_ReturnType Dcm_DcmObdMode02 (
        Dcm_SrvOpStatusType OpStatus,
        Dcm_MsgContextType* pMsgContext,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint8*             adrRespBuf_pu8; /* Pointer to Response buffer */
    uint8*             adrReqBuf_pu8; /* Pointer to Request buffer */
    Dcm_MsgLenType     nrReqDataLen_u32; /* Requested data length check */
    uint8              adrTmpBuf_au8[3]; /* Temporary buffer maximum of 3PIDs */
    Std_ReturnType     dataRetVal_u8; /* Return type of API Dem_DcmReadDataOfOBDFreezeFrame */
    Dcm_ObdContextType ModeContext;

    /* Initialization of local variables */
    *dataNegRespCode_u8          = 0x0u;
    ModeContext.nrDTC_u32        = 0x00;
    adrTmpBuf_au8[0]             = 0x00; /* To remove Misra warning */
    nrReqDataLen_u32             = (pMsgContext->reqDataLen); /* Copy the requested data length into local variable */
    ModeContext.nrPIDValid_qu8   = 0x00;
    ModeContext.nrResDataLen_u32 = 0x00;
    ModeContext.nrMultiple_u8    = 0x00;

    /*Initialise flgGetDTCNum with True*/
    ModeContext.flgGetDTCNum_b  = TRUE;
    ModeContext.idxPIDStart_qu8 = 0x00;
    ModeContext.nrPid_u8        = 0x00;

    /*Initialise PIDFound with false*/
    ModeContext.isPIDFound_b = FALSE;
    dataRetVal_u8            = E_NOT_OK;

    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/
    DCM_UNUSED_PARAM(OpStatus);

    /* Check if requested data length is equal to 2 or 4 or 6*/
    if((nrReqDataLen_u32 == 0x02uL)||
       (nrReqDataLen_u32 == 0x04uL)||
       (nrReqDataLen_u32 == 0x06uL))
    {
        /* Pointer to request buffer */
        adrReqBuf_pu8 = pMsgContext->reqData;

        /* Check if it is pure multiple of 0x20 or pure non multiple of 0x20
         In mode $02, each PID number is associated with freeze frame number of 1byte */
        for(ModeContext.nrPIDChk_qu8 = 0;(ModeContext.nrPIDChk_qu8 < nrReqDataLen_u32);ModeContext.nrPIDChk_qu8 += 2u)
        {
            /* Check whether requested PIDs are availability PIDS 0x00,0x20,0x40,0x60,0x80,0xa0,0xc0,0xe0
             this check can be done by performing bitwise AND 00011111 with requested PID.
             In case of availability PID, pid number BIT 7(MSB),6,5 can be set to 1 and BIT 4,3,2,1,0(LSB)is set to 0
             always */
            if((adrReqBuf_pu8[ModeContext.nrPIDChk_qu8] & 0x1Fu) == 0u)
            {
                /* In case of pure multiple of 0x20, the counter value = reqdatalen/2
                 for eg: request 02 00 00 20 00 40 00, reqdatalen = 6 since all requested PIDs are special PIDs
                 hence the counter is incremented 3times (counter == reqdatalen/2).
                 In case all requested PIDs which reports freeze frame data is requested,then this counter is
                 always zero */
                ModeContext.nrMultiple_u8++;
            }

            /* Check whether freeze frame number is always 00 for all requested PIDs*/
            if((adrReqBuf_pu8[ModeContext.nrPIDChk_qu8 + 1u]) == 0x00u)
            {
                /* Copy only PID to temporary buffer */
                adrTmpBuf_au8[ModeContext.nrPIDValid_qu8] = adrReqBuf_pu8[ModeContext.nrPIDChk_qu8];

                /* Increment the index of temporary buffer */
                ModeContext.nrPIDValid_qu8++;
            }

        }
        /* The request is mixed up with multiple and non multiple of 0x20
         if only availability PID is requested then the counter value == requested data length / 2
         if only PID which report freeze frame data value is requested then the counter value is equal to 0*/
        if((ModeContext.nrMultiple_u8 == 0u) ||
           (ModeContext.nrMultiple_u8 == (uint8)(nrReqDataLen_u32 >> 1u)))
        {
            /* Pointer to response data buffer */
            adrRespBuf_pu8 = pMsgContext->resData;

            /* Set Negative response code 0x12
             NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing*/
            *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;

            /* Get the maximum response data length in response buffer */
            ModeContext.nrResMaxDataLen_u32 = pMsgContext->resMaxDataLen;

            /* Check for only availability PIDS in requested format */
            /* Check for PIDs which are requested by tester and which has freeze frame number as 00 associated
             with each PID. nrPIDValid_qu8 has count of valid PIDS which has freeze frame number as 00 for each PID*/
            for(ModeContext.nrPIDChk_qu8 = 0;(ModeContext.nrPIDChk_qu8 < ModeContext.nrPIDValid_qu8);
                    ModeContext.nrPIDChk_qu8++)
            {
                /* Index for Dcm_Dsp_Mode1Bitmask_acs is calculated based on PID number index number = PID number/0x20*/
                ModeContext.idxPID_u8 = (uint8)((adrTmpBuf_au8[ModeContext.nrPIDChk_qu8]) / DCM_OBDMODE02_SUPPPID);

                /* Based on PID number,access the PID mask value corresponding to index in Dcm_Dsp_Mode2Bitmask_acs*/
                ModeContext.dataPIDBitMask_u32 = Dcm_Dsp_Mode2Bitmask_acs[ModeContext.idxPID_u8].BitMask_u32;

                /* If only availability PID is requested then the counter value == requested data length/2
                 if only PID which report freeze frame data value is requested then the counter value is equal to 0*/
                if(ModeContext.nrMultiple_u8 != 0)
                {
                    Dcm_Prv_AvailabiltyPid(&ModeContext,
                                           adrRespBuf_pu8,
                                           adrTmpBuf_au8,
                                           dataNegRespCode_u8);
                }
                /* If the requested PID is to obtain freeze frame data, Eg : PID 0x02,0x03 etc */
                else
                {
                    Dcm_Prv_NonAvailabiltyPid (&ModeContext,
                                               adrRespBuf_pu8,
                                               adrTmpBuf_au8,
                                               dataNegRespCode_u8);
                }
            } /* End of for loop */

            /* Set the response length for requested PID */
            pMsgContext->resDataLen = ModeContext.nrResDataLen_u32;
        }
        else
        {
            /*Set Negative response code 0x12 for the combination of both supported PID
             and PID which report data value in single request format
             NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing*/
            *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        }
    }
    else
    {
        /* Requested data length is not equal to 2 or 4 or 6 then Send Negative response code Request
         Length Invalid NRC 0x12 is suppressed in DCM module for OBD service since its functional addressing*/
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
    }

    dataRetVal_u8 = (*dataNegRespCode_u8 != 0x0u) ? E_NOT_OK : E_OK;

    return dataRetVal_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif


