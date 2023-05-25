
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"
#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_PeriodicInfoType_tst Dcm_PeriodicInfo_st[DCM_CFG_MAX_DID_SCHEDULER];  /* Periodic scheduler array */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint8 Dcm_NumOfActivePeriodicId_u8;            /* Variable to store the number of active identifiers in the periodic scheduler */
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_DIDIndexType_tst s_Dcm_idxDidIndexType_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static boolean s_isTxPduIdBusy_b =  FALSE;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* RDPI2 message context table */
Dcm_MsgContextType Dcm_Rdpi2MesContext_st;
/* response type */
Dcm_DsldResponseType_ten Dcm_Rdpi2ResponseType_en;
/* RDPI2 info structure */
PduInfoType Dcm_DsldRdpi2pduinfo_ast[DCM_CFG_NUM_RDPITYPE2_TXPDU];
/* RDPI2 TYPE2 TxPduid index*/
static uint8 s_idxRdpi2TxPduId_u8;
/* Pointer to RDPI info structure */
const Dcm_ProtocolExtendedInfo_type * Dcm_DsldRdpi2_pcst;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
**************************************************************************************************
* Dcm_RdpiPriorityInversion :  DCM function to perofrm the Priority Inversion for RDPI service
* \param  idxSchedulerIndexLoop_u16    :   Index of the scheduler for which Dcm_RdpiPriorityInversion has to be performed
*
* \retval          None
*
* \seealso
* \usedresources
**************************************************************************************************
*/
static void Dcm_RdpiPriorityInversion(uint16 idxSchedulerIndexLoop_u16)
{
#if (DCM_CFG_MAX_DID_SCHEDULER > 1u)
    uint16 idxIndex_u16;
#endif
    Dcm_PeriodicInfoType_tst Temp_PeriodicInfo_st;
    /*copy the values associated with the scheduled PDID into Temporary variables */
    Temp_PeriodicInfo_st.cntrTime_u32         = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].cntrTime_u32;
    Temp_PeriodicInfo_st.dataId_u16           = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataId_u16;
    Temp_PeriodicInfo_st.idxPeriodicId_u16    = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].idxPeriodicId_u16;
    Temp_PeriodicInfo_st.dataOverflowValue_en = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataOverflowValue_en;
    Temp_PeriodicInfo_st.dataRange_b          = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataRange_b;

    /*Perform Priority Inversion for the scheduled PDID*/
#if (DCM_CFG_MAX_DID_SCHEDULER > 1u)
    for(idxIndex_u16 = idxSchedulerIndexLoop_u16 ; idxIndex_u16 < (DCM_CFG_MAX_DID_SCHEDULER-1u) ; idxIndex_u16++)
    {
        /*Shift the PDID present in the RDPI scheduler up by one */
        Dcm_PeriodicInfo_st[idxIndex_u16].cntrTime_u32         = Dcm_PeriodicInfo_st[idxIndex_u16+1].cntrTime_u32;
        Dcm_PeriodicInfo_st[idxIndex_u16].dataId_u16           = Dcm_PeriodicInfo_st[idxIndex_u16+1].dataId_u16;
        Dcm_PeriodicInfo_st[idxIndex_u16].idxPeriodicId_u16    = Dcm_PeriodicInfo_st[idxIndex_u16+1].idxPeriodicId_u16;
        Dcm_PeriodicInfo_st[idxIndex_u16].dataOverflowValue_en = Dcm_PeriodicInfo_st[idxIndex_u16+1].dataOverflowValue_en;
        Dcm_PeriodicInfo_st[idxIndex_u16].dataRange_b          = Dcm_PeriodicInfo_st[idxIndex_u16+1].dataRange_b;
    }
#endif

        /*Place the scheduled PDID at the end of the RDPI scheduler */
        Dcm_PeriodicInfo_st[DCM_CFG_MAX_DID_SCHEDULER-1u].cntrTime_u32         = Temp_PeriodicInfo_st.cntrTime_u32;
        Dcm_PeriodicInfo_st[DCM_CFG_MAX_DID_SCHEDULER-1u].dataId_u16           = Temp_PeriodicInfo_st.dataId_u16;
        Dcm_PeriodicInfo_st[DCM_CFG_MAX_DID_SCHEDULER-1u].idxPeriodicId_u16    = Temp_PeriodicInfo_st.idxPeriodicId_u16;
        Dcm_PeriodicInfo_st[DCM_CFG_MAX_DID_SCHEDULER-1u].dataOverflowValue_en = Temp_PeriodicInfo_st.dataOverflowValue_en;
        Dcm_PeriodicInfo_st[DCM_CFG_MAX_DID_SCHEDULER-1u].dataRange_b          = Temp_PeriodicInfo_st.dataRange_b;
}


/**
 **************************************************************************************************
 * Dcm_DsldPeriodicSchedulerIni :  DCM function to initialise the periodic scheduler
 * \param           None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_DsldPeriodicSchedulerIni(void)
{
	uint16 idxIndex_u16;
	for(idxIndex_u16=0;idxIndex_u16<DCM_CFG_MAX_DID_SCHEDULER;idxIndex_u16++)
	{
		Dcm_PeriodicInfo_st[idxIndex_u16].dataId_u16=0x00;
		Dcm_PeriodicInfo_st[idxIndex_u16].idxPeriodicId_u16=0x00;
		Dcm_PeriodicInfo_st[idxIndex_u16].dataOverflowValue_en=DCM_RDPI_NO_TRANMISSION;
		Dcm_PeriodicInfo_st[idxIndex_u16].cntrTime_u32=0x00;
		/*Initialize that the did range is not valid*/
		Dcm_PeriodicInfo_st[idxIndex_u16].dataRange_b = FALSE;

	}
	Dcm_NumOfActivePeriodicId_u8 = 0x00;
	Dcm_PeriodicSchedulerRunning_b=FALSE;
	s_idxRdpi2TxPduId_u8=0;
	Dcm_ResetDIDIndexstruct(&s_Dcm_idxDidIndexType_st);		/*This function is invoked to reset all the elements of DID index structure to its default value*/
}


/**
**************************************************************************************************
* Dcm_SetRdpicounter :  Helper function to update counter time of the PID requested based on
*                       rate with which PID is scheduled.
* \param                dataSchedulerLoop_u16 : Index of the PID in periodic scheduler table
* \retval               void
* \seealso
* \usedresources
**************************************************************************************************
*/
static void Dcm_SetRdpicounter(uint16 dataSchedulerLoop_u16)
{
    if(Dcm_PeriodicInfo_st[dataSchedulerLoop_u16].dataOverflowValue_en==DCM_RDPI_SLOW_RATE)
     {
         Dcm_PeriodicInfo_st[dataSchedulerLoop_u16].cntrTime_u32=DCM_CFG_PERIODICTX_SLOWRATE;
     }
     else if(Dcm_PeriodicInfo_st[dataSchedulerLoop_u16].dataOverflowValue_en==DCM_RDPI_MEDIUM_RATE)
     {
         Dcm_PeriodicInfo_st[dataSchedulerLoop_u16].cntrTime_u32=DCM_CFG_PERIODICTX_MEDIUMRATE;
     }
     else if(Dcm_PeriodicInfo_st[dataSchedulerLoop_u16].dataOverflowValue_en== DCM_RDPI_FAST_RATE)
     {
         Dcm_PeriodicInfo_st[dataSchedulerLoop_u16].cntrTime_u32=DCM_CFG_PERIODICTX_FASTRATE;
     }
     else
     {
        /*Nothing to be done here*/
     }
}

/**
**************************************************************************************************
* Dcm_InitializeRdpiParameters :  DCM function to obtain the data of the DID and pass it on to lower layers
*                          for transmission
* \param           dataLength_u32    :   length of the DID
*                  dataSchedulerLoop_u16 : Index of the PID in periodic scheduler table
* \retval          STD_ReturnType      :    E_OK if transmission was successful
*
* \seealso
* \usedresources
**************************************************************************************************
*/
/*MR12 RULE 8.13 VIOLATION: The object addressed by the pointer parameter 'idxRdpiType2_pu8' is not modified and so the pointer could be of type 'pointer to const'. MISRA C:2012 Rule-8.13  */
static Std_ReturnType Dcm_InitializeRdpiParameters(uint8 * idxRdpiType2_pu8)
{
    Std_ReturnType dataCondChkRetVal_u8; /* Return value of Condition check API */
    Std_ReturnType idxIndex_u8;
    uint8 ConnectionTxpduOffset_u8;
    uint8 ConnectionNumberofTxpdus_u8;
    idxIndex_u8=0;
    ConnectionTxpduOffset_u8 = Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].ConnectionIndex_u8;
    ConnectionNumberofTxpdus_u8 = Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].NumberOfTxpdu_u8;
    dataCondChkRetVal_u8=E_NOT_OK;
    (void)idxRdpiType2_pu8;
    Dcm_DsldRdpi2_pcst = Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].rdpi_info_pcs;
    /* Fill the default response type */
    Dcm_Rdpi2ResponseType_en = DCM_DSLD_POS_RESPONSE;
    /* Response length (filled by the service) */
    Dcm_Rdpi2MesContext_st.resDataLen = 0x0u;
    /* Fill the addressing mode as physical  */
    Dcm_Rdpi2MesContext_st.msgAddInfo.reqType = DCM_PRV_PHYSICAL_REQUEST;
    for(idxIndex_u8 = ConnectionTxpduOffset_u8; idxIndex_u8 < (ConnectionTxpduOffset_u8 + ConnectionNumberofTxpdus_u8); idxIndex_u8++)
    {
        /*Check if the Rdpi type isTxPduId is set to the value false*/
        if(Dcm_Dsld_RDPIType2tx_table[idxIndex_u8].isTxPduId_Busy==FALSE)
        {
            Dcm_Rdpi2MesContext_st.resData = &Dcm_Dsld_RDPIType2tx_table[idxIndex_u8].txbuffer_ptr[0];
            break;
        }
    }
    /*Check if any TXPDU is available, if not wait for the next time slice*/
    if(idxIndex_u8<(ConnectionTxpduOffset_u8 + ConnectionNumberofTxpdus_u8))
    {
        /* Fill the maximum possible response length */
        Dcm_Rdpi2MesContext_st.resMaxDataLen = Dcm_DsldRdpi2_pcst->txbuffer_length_u32 - 1uL;
        /* This is RDPI requested service */
        Dcm_Rdpi2MesContext_st.msgAddInfo.sourceofRequest = DCM_RDPI_SOURCE;
        /* Is suppressed positive bit is always FALSE */

        Dcm_Rdpi2MesContext_st.msgAddInfo.suppressPosResponse = FALSE;
        /*Update the index of TxPdu to be used by the Dcm_FrameRdpiresponse()*/
        *idxRdpiType2_pu8=idxIndex_u8;
        s_idxRdpi2TxPduId_u8=idxIndex_u8;
        /*TXPDUID is available*/
        dataCondChkRetVal_u8=E_OK;
        s_isTxPduIdBusy_b = FALSE;
    }
    else
    {
        dataCondChkRetVal_u8=E_NOT_OK;
        s_isTxPduIdBusy_b = TRUE;
    }
    return dataCondChkRetVal_u8;
}

/**
**************************************************************************************************
* Dcm_GetRdpiType2Index :  DCM function to obtain the data of the DID and pass it on to lower layers
*                          for transmission
* \param           dataLength_u32    :   length of the DID
*                  dataSchedulerLoop_u16 : Index of the PID in periodic scheduler table
* \retval          STD_ReturnType      :    E_OK if transmission was successful
*
* \seealso
* \usedresources
**************************************************************************************************
*/

void Dcm_GetRdpiType2Index(uint8 * idxRdpi2TxPduId_u8)
{
    *idxRdpi2TxPduId_u8=s_idxRdpi2TxPduId_u8;
}

/**
**************************************************************************************************
* Dcm_FrameRdpiresponse :  DCM function to obtain the data of the DID and pass it on to lower layers
*                          for transmission
* \param           dataLength_u32    :   length of the DID
*                  dataSchedulerLoop_u16 : Index of the PID in periodic scheduler table
* \retval          STD_ReturnType      :    E_OK if transmission was successful
*
* \seealso
* \usedresources
**************************************************************************************************
*/
static Std_ReturnType Dcm_FrameRdpiresponse(uint32 dataLength_u32,uint16 dataSchedulerLoop_u16)
{
    Std_ReturnType dataReaddataRetVal_u8; /* Return value of Condition check API */
    uint8 idxRdpiType2_u8;
    idxRdpiType2_u8=0xFF;
    dataReaddataRetVal_u8=E_NOT_OK;

    dataReaddataRetVal_u8=Dcm_InitializeRdpiParameters(&idxRdpiType2_u8);

    if(E_OK==dataReaddataRetVal_u8)
    {
        /* Call the API to read data corresponding to the periodic identifier */
        dataReaddataRetVal_u8 = Dcm_GetDIDData (&s_Dcm_idxDidIndexType_st,&Dcm_Rdpi2MesContext_st.resData[1]);

        /*Check if data read successfully or if applicationtion has set any NRC*/
        if(s_Dcm_idxDidIndexType_st.dataNegRespCode_u8==0x00)
        {
            /* If data was read successfully */
            if(E_OK==dataReaddataRetVal_u8)
            {
                /*  Data ready successfully, trigger the response*/
               /* Update the response length for the SID and the identifier */
               Dcm_Rdpi2MesContext_st.resDataLen=dataLength_u32+1uL;
               /* Update the periodic identifier in the transmission buffer */
               Dcm_Rdpi2MesContext_st.resData[0]=(uint8)(Dcm_PeriodicInfo_st[dataSchedulerLoop_u16].dataId_u16);
               /* Send the response */
               /*txpduid is blocked in the processiong done before Pdur_DcmTransmit is triggered*/
               Dcm_ProcessingDone(&Dcm_Rdpi2MesContext_st);

               s_Dcm_idxDidIndexType_st.nrNumofSignalsRead_u16 = 0x0; /*All the signals read correctly, therefore reset it to zero*/
               s_Dcm_idxDidIndexType_st.dataSignalLengthInfo_u32 = 0x0;    /*All the signals read correctly, therefore reset the signal data length to zero*/

            }
            else
            {
                s_Dcm_idxDidIndexType_st.nrNumofSignalsRead_u16 = 0x0; /*All the signals read correctly, therefore reset it to zero*/
                s_Dcm_idxDidIndexType_st.dataSignalLengthInfo_u32 = 0x0;    /*All the signals read correctly, therefore reset the signal data length to zero*/
                dataReaddataRetVal_u8=E_NOT_OK;
            }
        }
        else
        {
            s_Dcm_idxDidIndexType_st.nrNumofSignalsRead_u16 = 0x0; /*All the signals read correctly, therefore reset it to zero*/
            s_Dcm_idxDidIndexType_st.dataSignalLengthInfo_u32 = 0x0;    /*All the signals read correctly, therefore reset the signal data length to zero*/
            dataReaddataRetVal_u8=E_NOT_OK;
        }
    }
    else
    {
        dataReaddataRetVal_u8=E_NOT_OK;
    }

    return dataReaddataRetVal_u8;
}

/**
**************************************************************************************************
* Dcm_VerifyLengthRdpiType :  DCM function to verify if the length of the DID is within the Tx buffer limits
*
* \param           dataLength_u32    :   length of the DID
*
* \retval          None
* \seealso
* \usedresources
**************************************************************************************************
*/
static Std_ReturnType Dcm_VerifyLengthRdpiType(uint32 dataLength_u32)
{
    Std_ReturnType dataCondChkRetVal_u8; /* Return value of Condition check API */
    dataCondChkRetVal_u8=E_NOT_OK;

    /*Verify if the DID can be accomodated within the buffer available*/
    if((dataLength_u32+1uL)<=Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].rdpi_info_pcs->txbuffer_length_u32)
    {
        dataCondChkRetVal_u8=E_OK;
    }

    return dataCondChkRetVal_u8;
}

/**
 **************************************************************************************************
 * Dcm_RdpiCheckPIDSupportConditions :  DCM function to monitor the session security and condition
 * check read function for the PID, if all checks are fine then the length of PID is obtained and
 * RDPI response frame function is invoked
 *
 * \param           idxPeriodicIndex_u16    :   Index of the DID in Dcm_DIDConfig[]
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
static Std_ReturnType Dcm_RdpiCheckPIDSupportConditions(uint16 idxPeriodicIndex_u16)
{
    Std_ReturnType dataCondChkRetVal_u8; /* Return value of Condition check API */

    uint32 dataLength_u32;

    dataLength_u32=0x0;

    /*initialize the file static DID structure variable with the index of DID in Dcm_DIDCOnfig*/
    s_Dcm_idxDidIndexType_st.idxIndex_u16 = Dcm_PeriodicInfo_st[idxPeriodicIndex_u16].idxPeriodicId_u16;
    dataCondChkRetVal_u8=E_NOT_OK;

    /*Obtain the length of the PDID*/
    if(E_OK==Dcm_GetLengthOfDIDIndex(&s_Dcm_idxDidIndexType_st, &dataLength_u32,(uint16)(Dcm_PeriodicInfo_st[idxPeriodicIndex_u16].dataId_u16|0xF200u)))
    {
        /*Verify if the length of DID fits in within the buffer*/
        if(E_OK==Dcm_VerifyLengthRdpiType(dataLength_u32))
        {
            /*All checks passed, frame the rdpi response*/
            dataCondChkRetVal_u8=Dcm_FrameRdpiresponse(dataLength_u32,idxPeriodicIndex_u16);
        }
        else
        {   /*Stop processing this ID length check has failed*/
            Dcm_PeriodicInfo_st[idxPeriodicIndex_u16].dataOverflowValue_en=DCM_RDPI_NO_TRANMISSION;
        }
    }
    else
    {   /*Stop processing when Get Length is unsuccessful*/
        Dcm_PeriodicInfo_st[idxPeriodicIndex_u16].dataOverflowValue_en=DCM_RDPI_NO_TRANMISSION;
    }

    return dataCondChkRetVal_u8;
}

/**
 **************************************************************************************************
 * Dcm_RdpiMainFunction :  DCM function to monitor the periodic scheduler and for initiating the
 * periodic transmissions of the active periodic identifiers.
 * \param           None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_RdpiMainFunction(void)
{
    /*TRACE[SWS_Dcm_01101]*/
    /*TRACE[SWS_Dcm_01102]*/
    /*TRACE[SWS_Dcm_01103]*/
    /*TRACE[SWS_Dcm_01104]*/
    /*TRACE[SWS_Dcm_01105]*/
    /*TRACE[SWS_Dcm_01107]*/
    /*TRACE[SWS_Dcm_01113]*/
    /*TRACE[SWS_Dcm_01114]*/
    /*TRACE[SWS_Dcm_01118]*/
    uint16 idxSchedulerIndexLoop_u16 = 0u;
    uint16 idxPriorityInversion_u16 = 0u;
    Std_ReturnType dataCondChkRetVal_u8; /* Return value of Condition check API */
    Std_ReturnType idxIndex_u8;
    idxIndex_u8=0;
    idxSchedulerIndexLoop_u16=0x0u;
    dataCondChkRetVal_u8=E_NOT_OK;

    if(Dcm_NumOfActivePeriodicId_u8>0u)
    {
        /*Loop throught the scheduler and check if any of the IDs time slice has expired*/
        for( idxIndex_u8=0;idxIndex_u8<DCM_CFG_NUM_RDPITYPE2_TXPDU;idxIndex_u8++)
        {
            if(Dcm_Dsld_RDPIType2tx_table[idxIndex_u8].isTxPduId_Busy!=FALSE)
            {
                if(Dcm_Dsld_RDPIType2tx_table[idxIndex_u8].cntFreeTxPduRdpi2Cntr_u8!=0)
                {
                    /*Decrrement the timer and chck if time out has occured*/
                    Dcm_Dsld_RDPIType2tx_table[idxIndex_u8].cntFreeTxPduRdpi2Cntr_u8--;
                    /*Check if the timer has timedout*/
                    if(Dcm_Dsld_RDPIType2tx_table[idxIndex_u8].cntFreeTxPduRdpi2Cntr_u8==0)
                    {
                        /*Release the TxPDU since the confirmation has not yet come and timer has timedout*/
                        Dcm_Dsld_RDPIType2tx_table[idxIndex_u8].isTxPduId_Busy=FALSE;
                        s_isTxPduIdBusy_b = FALSE;
                        /*Reload the counter, TXPDU is already released*/
                        Dcm_Dsld_RDPIType2tx_table[idxIndex_u8].cntFreeTxPduRdpi2Cntr_u8=DCM_CFG_RDPITYPE2FREETXPDUCOUNTER;
                    }
                }
            }
        }
        /*Loop throught the scheduler and check if any of the IDs time slice has expired*/
        for (idxSchedulerIndexLoop_u16=0;idxSchedulerIndexLoop_u16<DCM_CFG_MAX_DID_SCHEDULER;idxSchedulerIndexLoop_u16++)
        {
            if (Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataOverflowValue_en!=DCM_RDPI_NO_TRANMISSION)
            {
               /* Check if the identifier is ready for transmission*/
               if(Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].cntrTime_u32>0uL)
               {
                   /* Decrement the counter and see if the time slice is reached*/
                   Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].cntrTime_u32--;
               }
            }
        }

        if((DCM_CHKFULLCOMM_MODE(Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8))&&(NULL_PTR != Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].rdpi_info_pcs))
        {
            for (idxSchedulerIndexLoop_u16=0;idxSchedulerIndexLoop_u16<DCM_CFG_MAX_DID_SCHEDULER;idxSchedulerIndexLoop_u16++)
            {
                if ((Dcm_PeriodicInfo_st[idxPriorityInversion_u16].dataOverflowValue_en!=DCM_RDPI_NO_TRANMISSION) && \
                        (Dcm_PeriodicInfo_st[idxPriorityInversion_u16].cntrTime_u32 == 0uL))
                {
                   dataCondChkRetVal_u8=Dcm_RdpiCheckPIDSupportConditions(idxPriorityInversion_u16);

                   /*If response was triggered successfully on the bus, reload the counters and perforrm priority Inversion*/
                   if((s_isTxPduIdBusy_b != TRUE) || (dataCondChkRetVal_u8 != E_NOT_OK))
                   {
                      Dcm_SetRdpicounter(idxPriorityInversion_u16);
                      Dcm_RdpiPriorityInversion(idxPriorityInversion_u16);
                   }
                }
                else
                {
                    /*increment the scheduler counter when cntrTime_u32 is not zero and dataOverflowValue_en is not DCM_RDPI_NO_TRANMISSION*/
                    idxPriorityInversion_u16++;
                }
            }
        }
    }
}
 /***********************************************************************************************************************
 Function name    : Dcm_RdpiSessionCheck
 Syntax           : Dcm_RdpiSessionCheck(sessionMask_u32)
 Description      : Checking scheduled periodic DID is supported in Active Session else scheduled periodic DID shall be stopped.
 Parameter        : sessionMask_u32
 Return value     : None
***********************************************************************************************************************/
void Dcm_RdpiSessionCheck(uint32 sessionMask_u32)
{
    uint32 dataSessionMask_u32;   /* Variable to hold the configured sessions value */
    uint16 idxSchedulerIndexLoop_u16;
    const Dcm_ExtendedDIDConfig_tst * adrExtendedConfig_pcst; /* Pointer to extended configuration */

    dataSessionMask_u32=0x00u;
    adrExtendedConfig_pcst=NULL_PTR;

    /*TRACE[SWS_Dcm_01109]*/
    if(sessionMask_u32 != 0x01u)
    {
        for (idxSchedulerIndexLoop_u16=0x00u;idxSchedulerIndexLoop_u16<DCM_CFG_MAX_DID_SCHEDULER;idxSchedulerIndexLoop_u16++)
        {

            /*initialize the file static DID structure variable with the index of DID in Dcm_DIDCOnfig*/
            s_Dcm_idxDidIndexType_st.idxIndex_u16 = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].idxPeriodicId_u16;
            s_Dcm_idxDidIndexType_st.dataRange_b = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataRange_b;

            /*Is the DID range DID or not*/

            if(s_Dcm_idxDidIndexType_st.dataRange_b==FALSE)
            {
                adrExtendedConfig_pcst=Dcm_DIDConfig[s_Dcm_idxDidIndexType_st.idxIndex_u16].adrExtendedConfig_pcst;
            }
#if ( DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)
            /*Control comes here for range DIDS*/
            else
            {
                s_Dcm_idxDidIndexType_st.dataRangeDid_16 = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataId_u16;
                adrExtendedConfig_pcst=Dcm_DIDRangeConfig_cast[s_Dcm_idxDidIndexType_st.idxIndex_u16].adrExtendedConfig_pcst;
            }
#endif

            /*Obtain the session mask based on the type of DID*/
            dataSessionMask_u32  = adrExtendedConfig_pcst->dataAllowedSessRead_u32;
            if (Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataOverflowValue_en!=DCM_RDPI_NO_TRANMISSION)
            {
                /* If the identifier is not supported in the current session*/
                if ((Dcm_DsldGetActiveSessionMask_u32() & dataSessionMask_u32) == 0x0uL)
                {
                    /* any scheduled periodic DID shall be stopped */
                    Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataId_u16=0x00u;
                    Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].idxPeriodicId_u16=0x00u;
                    Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataOverflowValue_en=DCM_RDPI_NO_TRANMISSION;
                    Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].cntrTime_u32=0x00u;
                    Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataRange_b = FALSE;
                    Dcm_NumOfActivePeriodicId_u8--;
                }
            }
        }
    }
    else
    {
        /*TRACE[SWS_Dcm_01107]*/
        for (idxSchedulerIndexLoop_u16=0x00u;idxSchedulerIndexLoop_u16<DCM_CFG_MAX_DID_SCHEDULER;idxSchedulerIndexLoop_u16++)
        {
            /* any scheduled periodic DID shall be stopped */
            Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataId_u16=0x00u;
            Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].idxPeriodicId_u16=0x00u;
            Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataOverflowValue_en=DCM_RDPI_NO_TRANMISSION;
            Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].cntrTime_u32=0x00u;
            Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataRange_b = FALSE;
            Dcm_NumOfActivePeriodicId_u8--;
        }
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_RdpiSecurityCheck
 Syntax           : Dcm_RdpiSecurityCheck(void)
 Description      : Checking scheduled periodic DID is supported in Active Security else scheduled periodic DID shall be stopped.
 Parameter        : None
 Return value     : None
***********************************************************************************************************************/
void Dcm_RdpiSecurityCheck(void)
{
    uint32 dataSecurityMask_u32;  /* Variable to hold the configured security value */
    uint16 idxSchedulerIndexLoop_u16;
    const Dcm_ExtendedDIDConfig_tst * adrExtendedConfig_pcst; /* Pointer to extended configuration */

    dataSecurityMask_u32=0x00u;
    adrExtendedConfig_pcst=NULL_PTR;

    /*TRACE[SWS_Dcm_01108]*/
    /*TRACE[SWS_Dcm_01110]*/
    for (idxSchedulerIndexLoop_u16=0x00u;idxSchedulerIndexLoop_u16<DCM_CFG_MAX_DID_SCHEDULER;idxSchedulerIndexLoop_u16++)
    {
        /*initialize the file static DID structure variable with the index of DID in Dcm_DIDCOnfig*/
        s_Dcm_idxDidIndexType_st.idxIndex_u16 = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].idxPeriodicId_u16;
        s_Dcm_idxDidIndexType_st.dataRange_b = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataRange_b;

        /*Is the DID range DID or not*/

        if(s_Dcm_idxDidIndexType_st.dataRange_b==FALSE)
        {
            adrExtendedConfig_pcst=Dcm_DIDConfig[s_Dcm_idxDidIndexType_st.idxIndex_u16].adrExtendedConfig_pcst;
        }
#if ( DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)
        /*Control comes here for range DIDS*/
        else
        {
            s_Dcm_idxDidIndexType_st.dataRangeDid_16 = Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataId_u16;
            adrExtendedConfig_pcst=Dcm_DIDRangeConfig_cast[s_Dcm_idxDidIndexType_st.idxIndex_u16].adrExtendedConfig_pcst;
        }
#endif

/*Obtain the security mask based on the type of DID*/
        dataSecurityMask_u32 = adrExtendedConfig_pcst->dataAllowedSecRead_u32;

        if (Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataOverflowValue_en!=DCM_RDPI_NO_TRANMISSION)
        {
            /* If the identifier is not supported in the current security level */
            if ((Dcm_DsldGetActiveSecurityMask_u32() & dataSecurityMask_u32) == 0x0uL)
            {
                /* any scheduled periodic DID shall be stopped */
                Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataId_u16=0x00u;
                Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].idxPeriodicId_u16=0x00u;
                Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataOverflowValue_en=DCM_RDPI_NO_TRANMISSION;
                Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].cntrTime_u32=0x00u;
                Dcm_PeriodicInfo_st[idxSchedulerIndexLoop_u16].dataRange_b = FALSE;
                Dcm_NumOfActivePeriodicId_u8--;
            }
        }
    }
}
/*
***********************************************************************************************************
*  Dcm_DsldResetRDPI :TO Reset the state machine of RDPI type 1 or Type 2
                         Also reset of settings done for the active service table
*  \param         None
*  \retval        None
*  \seealso
*
***********************************************************************************************************/

void Dcm_DsldResetRDPI(void)
{
    Dcm_DsldPeriodicSchedulerIni();
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
