

/***************************************************************************************************
* Includes
***************************************************************************************************/
#include "Xcp.h"
#include "Xcp_Priv.h"


/***************************************************************************************************
* Variables
***************************************************************************************************/


/***************************************************************************************************
* Functions
***************************************************************************************************/
#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

static void Xcp_StoreStim(const PduInfoType* XcpPacketPtr, Xcp_DaqList_t* daqListPtr, uint16 AbsOdtNo, uint8 protLayerId);

/**
****************************************************************************************************
This function stores the Odt into the STIM buffer and updates the buffer information
\param[in]  XcpPacketPtr  pointer to Odt data and length
\param[in]  daqListPtr    pointer to DaqList
\param[in]  AbsOdtNo      absolute Odt number
\param[in]  protLayerId   protocol layer id
\return     -
***************************************************************************************************/
static void Xcp_StoreStim(const PduInfoType* XcpPacketPtr, Xcp_DaqList_t* daqListPtr, uint16 AbsOdtNo, uint8 protLayerId)
{
  /* local variables */
  uint8* destBufPos_pu8;
  uint16 bytesToCopy;
  uint8  OdtCnt;
  uint8  BufferNumWriteActive;
  uint8  copyOffset;

  /* initialization */
  BufferNumWriteActive = 0xFFu;

#if (XCP_IDFIELD_OTHER_THAN_ABSOLUTE == STD_ON)
  copyOffset = (uint8) Xcp_NoInit[protLayerId].Session.IdFieldType_en;
#else
  copyOffset = (uint8) (XCP_IDENTIFICATION_FIELD_TYPE_ABSOLUTE);
#endif

#if (XCP_BYPASS_CONSISTENCY == STD_ON)
  /* check if this is is the first ODT in the daq list and if mode is set as DTO_CTR*/
  if ((AbsOdtNo == daqListPtr->OdtFirst_u16) && ((daqListPtr->Mode_u8 & XCP_DAQLISTMODE_DTO_CTR) == XCP_DAQLISTMODE_DTO_CTR))
  {
    /* exclude the event counter byte from the copyOffset */
    if (Xcp_NoInit[protLayerId].Session.IdFieldType_en == XCP_IDENTIFICATION_FIELD_TYPE_RELATIVE_WORD_ALIGNED)
    {
      /* nothing to do as the copyOffset is set correctly in this case */
    }
    else
    {
      /* Add 1 to the offset as the event counter consumes 1 byte */
      copyOffset += 1u;
    }
  }
#endif

  /* if in direction STIM */
  if (  ( (daqListPtr->Mode_u8 & XCP_DAQLISTMODE_DIRECTION_STIM) != 0u )
      /* check if daq config is in right state */
      &&  (Xcp_NoInit[protLayerId].DaqConfig.DaqState_en >= XCP_DAQ_STATE_READY_TO_RUN)
      /* check if Odt has the right length */
      && ( XCP_ODT_LENGTH(AbsOdtNo, protLayerId) >= XcpPacketPtr->SduLength )
     )
  {
    /* store stim data into buffer */
    /* subtract id field bytes */
    bytesToCopy = XCP_ODT_LENGTH(AbsOdtNo, protLayerId) - copyOffset;
    /* cache into local variables */
    OdtCnt = daqListPtr->OdtCnt_u8;

    /* enter lock to change buffers */
    SchM_Enter_Xcp_BufferAccessActiveNoNest();

    if (AbsOdtNo != daqListPtr->OdtFirst_u16)
    {
      /* fetch active buffer */
      BufferNumWriteActive = daqListPtr->BufferInfo.BufferNumWriteActive_u8;
      daqListPtr->BufferInfo.OdtReceiveCtr_u8++;
      /* exit lock around buffer access */
      SchM_Exit_Xcp_BufferAccessActiveNoNest();
    }
    else /* new loop begins, pid == FirstPID */
    {
      /* local variables needed only here */
      uint8 BufferNumReadActive;
      uint8 BufferNumReadNext;

      BufferNumReadNext   = daqListPtr->BufferInfo.BufferNumReadNext_u8;
      BufferNumReadActive = daqListPtr->BufferInfo.BufferNumReadActive_u8;

      /* find next available buffer to write */
      for (BufferNumWriteActive = 0; BufferNumWriteActive < XCP_ODT_STIM_BUFFER_ELEMENT_SIZE; BufferNumWriteActive++)
      {
        if(   (BufferNumWriteActive != BufferNumReadActive)
           && (BufferNumWriteActive != BufferNumReadNext)
          )
        {
          /* store buffer to use for next receive */
          daqListPtr->BufferInfo.BufferNumWriteActive_u8 = BufferNumWriteActive;
          daqListPtr->BufferInfo.OdtReceiveCtr_u8 = 1;
          break;
        }
      }
#if ( XCP_ODT_STIM_BUFFER_ELEMENT_SIZE < 3u )
     if (  (BufferNumWriteActive >= XCP_ODT_STIM_BUFFER_ELEMENT_SIZE)
        && (BufferNumReadNext != BufferNumReadActive)
        )
     {
       /* no available buffer found, use ReadNext Buffer */
       BufferNumWriteActive = BufferNumReadNext;
       daqListPtr->BufferInfo.BufferNumWriteActive_u8 = BufferNumWriteActive;

  #if ( XCP_ODT_STIM_BUFFER_ELEMENT_SIZE < 2u )
       /* mark ReadNext as invalid */
       daqListPtr->BufferInfo.BufferNumReadNext_u8 = 0xFFu;
       /* for single buffer, no full data set available */
       daqListPtr->BufferInfo.Status_u8 = (uint8) (daqListPtr->BufferInfo.Status_u8 | XCP_STATUSBIT_STIM_ERR_NO_DATA_AVAILABLE);
  #else
       /* for double buffer, reuse old buffer for now */
       daqListPtr->BufferInfo.BufferNumReadNext_u8 = BufferNumReadActive;
  #endif
     }
#endif /* ( XCP_ODT_STIM_BUFFER_ELEMENT_SIZE < 3 ) */
      /* exit lock around buffer access */
      SchM_Exit_Xcp_BufferAccessActiveNoNest();
#if (XCP_TIMESTAMP == STD_ON)
      if((daqListPtr->Mode_u8 & XCP_DAQLISTMODE_TIMESTAMP) != 0u)
      {
        uint8 timestampBytes = (uint8) (Xcp_PlCfgConst.TlCfg[XCP_ACTIVE_TL_ID(protLayerId)].TimestampType_en);

        /* subtract timestamp bytes, because we are in first ODT and timestamp is enabled */
        bytesToCopy = bytesToCopy - timestampBytes;
        /* add timestamp to offset */
        copyOffset = copyOffset + timestampBytes;
      }
#endif
    }

    if (BufferNumWriteActive < XCP_ODT_STIM_BUFFER_ELEMENT_SIZE)
    {
      /* store the received data */
      /* calculate the destination buffer position */
      destBufPos_pu8 = &(daqListPtr->DaqListQue_p[XCP_MAXDTO_ALIGNED(protLayerId) * (((BufferNumWriteActive * OdtCnt) + AbsOdtNo) - daqListPtr->OdtFirst_u16)]);
      /* store into buffer, only copy expected length */
      /* MR12 DIR 1.1 VIOLATION: The input parameters of rba_BswSrv_MemCopy() are declared as (void*) for generic implementation of MemCopy. */
      Xcp_MemCopy( destBufPos_pu8, &(XcpPacketPtr->SduDataPtr[copyOffset]), bytesToCopy );

      /* check if all Odts have been received */
      if (daqListPtr->BufferInfo.OdtReceiveCtr_u8 == OdtCnt)
      {
        /* enter lock to change buffers */
        SchM_Enter_Xcp_BufferAccessActiveNoNest();

        /* set buffer to read for next eventchannel */
        daqListPtr->BufferInfo.BufferNumReadNext_u8 = BufferNumWriteActive;
        daqListPtr->BufferInfo.BufferNumWriteActive_u8 = 0xFFu;

        /* increase number of complete stim buffers received */
        if (daqListPtr->BufferInfo.CntStimReceived_u8 < 0xFE)
        {
          daqListPtr->BufferInfo.CntStimReceived_u8++;
        }

        daqListPtr->BufferInfo.Status_u8 = (daqListPtr->BufferInfo.Status_u8 & (uint8) (~XCP_STATUSBIT_STIM_ERR_NO_DATA_AVAILABLE));

        daqListPtr->BufferInfo.OdtReceiveCtr_u8 = 0;

        /* exit lock around buffer access */
        SchM_Exit_Xcp_BufferAccessActiveNoNest();
      }
    }
    else /* if (BufferNumWriteActive_u8 < XCP_ODT_STIM_BUFFER_ELEMENT_SIZE) */
    {
      /* do not process any further */
      /* enter lock to change buffers */
      SchM_Enter_Xcp_BufferAccessActiveNoNest();

      daqListPtr->BufferInfo.OdtReceiveCtr_u8 = 0;

      /* exit lock around buffer access */
      SchM_Exit_Xcp_BufferAccessActiveNoNest();
      XCP_DET_REPORT_ERROR_NO_RET(XCP_STIM_SID, XCP_E_NO_BUF_AVAIL);
    }
  }
  else /* not (XCP_DAQLISTMODE_RUNNING|XCP_DAQLISTMODE_DIRECTION_STIM) or wrong length */
  {
    /* received STIM packets for DAQ not in STIM mode, not started yet, or pdu length is wrong */
    XCP_DET_REPORT_ERROR_NO_RET(XCP_STIM_SID, XCP_E_STIM_PACKET_NOK);
  }
}


/**
****************************************************************************************************
This function finds out the daq list number and Odt number of given STIM packet and calls Xcp_StoreStim
\param[in]  XcpPacketPtr  pointer to xcp packet data and length
\param[in]  protLayerId   protocol layer id
\return     -
***************************************************************************************************/
void Xcp_ReceiveStim(const PduInfoType* XcpPacketPtr, uint8 protLayerId)
{
  /* Local variables */
  Xcp_DaqList_t* daqListPtr;
  uint16_least daqNo;
  uint16 CntDAQListsAvailable_u16;
  uint16 AbsOdtNo;
  uint8  pid;
  uint8  FirstPID;

  /* Initialization */
  pid = XcpPacketPtr->SduDataPtr[0];
  FirstPID = 0;
  daqListPtr = NULL_PTR;

#if (XCP_IDFIELD_OTHER_THAN_ABSOLUTE == STD_ON)
  {
    Xcp_IdField_t IdFieldType;
    IdFieldType = Xcp_NoInit[protLayerId].Session.IdFieldType_en;

    if (IdFieldType == XCP_IDENTIFICATION_FIELD_TYPE_ABSOLUTE)
    {
#endif
      /* ID Field Type == Absolute */
      AbsOdtNo = pid;
      CntDAQListsAvailable_u16 = Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16;
      /* find daq containing this pid */
      for (daqNo = 0; daqNo < CntDAQListsAvailable_u16; daqNo++)
      {
        daqListPtr = &(XCP_DAQ(daqNo, protLayerId));
        FirstPID = (uint8) daqListPtr->OdtFirst_u16; /* absolute ID Field is smaller than 256 */
        if (   (pid >= FirstPID)
            && (pid < (FirstPID + daqListPtr->OdtCnt_u8))
           )
        {
          /* found the right daq */
          break;
        }
      }
      /* check, if end of loop has been reached without finding DAQ */
      if ( daqNo == CntDAQListsAvailable_u16 )
      {
        /* no corresponding daq found */
        XCP_DET_REPORT_ERROR_NO_RET(XCP_STIM_SID, XCP_E_STIM_PACKET_NOK);
      }

#if (XCP_IDFIELD_OTHER_THAN_ABSOLUTE == STD_ON)
    }
    else if (IdFieldType == XCP_IDENTIFICATION_FIELD_TYPE_RELATIVE_BYTE)
    {
      daqListPtr = &(XCP_DAQ(XcpPacketPtr->SduDataPtr[1], protLayerId));
      AbsOdtNo = daqListPtr->OdtFirst_u16 + pid;
    }
    else if (IdFieldType == XCP_IDENTIFICATION_FIELD_TYPE_RELATIVE_WORD)
    {
  #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
      daqNo = (XcpPacketPtr->SduDataPtr[1]) | ((uint16_least)(XcpPacketPtr->SduDataPtr[2]) << 8u);
  #else /* CPU_BYTE_ORDER != HIGH_BYTE_FIRST */
      daqNo = (XcpPacketPtr->SduDataPtr[2]) | ((uint16_least)(XcpPacketPtr->SduDataPtr[1]) << 8u);
  #endif
      daqListPtr = &(XCP_DAQ(daqNo, protLayerId));
      AbsOdtNo = daqListPtr->OdtFirst_u16 + pid;
    }
    else /* IdFieldType_en == XCP_IDENTIFICATION_FIELD_TYPE_RELATIVE_WORD_ALIGNED */
    {
  #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
      daqNo = (XcpPacketPtr->SduDataPtr[2]) | ((uint16_least)(XcpPacketPtr->SduDataPtr[3]) << 8u);
  #else /* CPU_BYTE_ORDER != HIGH_BYTE_FIRST */
      daqNo = (XcpPacketPtr->SduDataPtr[3]) | ((uint16_least)(XcpPacketPtr->SduDataPtr[2]) << 8u);
  #endif
      daqListPtr = &(XCP_DAQ(daqNo, protLayerId));
      AbsOdtNo = daqListPtr->OdtFirst_u16 + pid;
    }
  }
#endif

  if (daqListPtr != NULL_PTR)
  {
#if (XCP_BYPASS_CONSISTENCY == STD_ON)
    /* check if the first ODT is received and if mode is set to DTO_CTR */
    if ((AbsOdtNo == daqListPtr->OdtFirst_u16) && ((daqListPtr->Mode_u8 & XCP_DAQLISTMODE_DTO_CTR) == XCP_DAQLISTMODE_DTO_CTR))
    {
      if (Xcp_NoInit[protLayerId].Session.IdFieldType_en == XCP_IDENTIFICATION_FIELD_TYPE_RELATIVE_WORD_ALIGNED)
      {
        /* copy the event counter byte into the global variable. when relative word aligned, counter is at position 1 */
        XCP_BYPASS(daqListPtr->EventChannelNum_u16).EventCounterReceived_u8 = XcpPacketPtr->SduDataPtr[(uint8)Xcp_NoInit[protLayerId].Session.IdFieldType_en - 3u];
      }
      else
      {
        /* copy the event counter byte into the global variable */
        XCP_BYPASS(daqListPtr->EventChannelNum_u16).EventCounterReceived_u8 = XcpPacketPtr->SduDataPtr[Xcp_NoInit[protLayerId].Session.IdFieldType_en];
      }
    }
#endif

    /* corresponding daq was found, process the received stim packet */
    Xcp_StoreStim(XcpPacketPtr, daqListPtr, AbsOdtNo, protLayerId);
  }
  else /* daqListPtr != NULL_PTR */
  {
    /* daq list could not be determined */
    XCP_DET_REPORT_ERROR_NO_RET(XCP_STIM_SID, XCP_E_STIM_PACKET_NOK);
  }
}


#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"


#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
This function stimulates the data by copying the data from the buffer into ecu memory
\param[in]  daqNo         daq list number
\param[in]  protLayerId   protocol layer id
\return     Status of the Eventchannel
\retval     status_u8   May contain the following status-bits
                        - 0x10: XCP_STATUSBIT_STIM_STARTED: set, when STIM is being processed
                        - 0x20: XCP_STATUSBIT_STIM_ERR_NO_DATA_AVAILABLE: set, when STIM does not have a complete set of data
\retval     numOfReceivedStimdata_u8  counter, how many complete sets of STIM data have been received
***************************************************************************************************/
Xcp_EventChannelStatus_t Xcp_StimEvent(uint16 daqNo, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Xcp_DaqList_t*        daqListPtr;
  const Xcp_AddrValue*  OdtEntryAdrPtr;
#if (XCP_ADDRESS_EXTENSION == STD_ON)
  const uint8*          OdtEntryExtensionPtr;
#endif
  const uint8*          OdtEntrySizePtr;
  uint8                 OdtEntrySize;
  Xcp_AddrType_t        OdtEntryXcpAddr;
  const uint8*          source_pu8;
  Xcp_EventChannelStatus_t RetSt;
  uint16 maxDtoAligned;
  uint16 OdtFirst;
  uint16_least AbsOdt_i;
  uint16_least OdtEntry_i;
  uint16 OdtEntryMax;
  uint8  OdtCnt;
  uint8  readBufferActive;

  XCP_TESTINJECTION_FUNC(XCP_TESTINJECTION_POS_DAQ_EVENTCHANNEL)
  daqListPtr = &(XCP_DAQ(daqNo, protLayerId));
  RetSt.reserved_u16 = 0;

  /* enter lock around buffer access */
  SchM_Enter_Xcp_BufferAccessActiveNoNest();
  /* store status */
  RetSt.status_u8 = daqListPtr->BufferInfo.Status_u8;
  /* store received stim counter */
  RetSt.numOfReceivedStimdata_u8 = daqListPtr->BufferInfo.CntStimReceived_u8;

  /* check, if a complete data set has been received */
  if ((RetSt.status_u8 & XCP_STATUSBIT_STIM_ERR_NO_DATA_AVAILABLE) == 0u)
  {
    readBufferActive = daqListPtr->BufferInfo.BufferNumReadNext_u8;
    /* set active buffer */
    daqListPtr->BufferInfo.BufferNumReadActive_u8 = readBufferActive;
    /* reset receive counter */
    daqListPtr->BufferInfo.CntStimReceived_u8 = 0;

    /* exit lock around buffer access */
    SchM_Exit_Xcp_BufferAccessActiveNoNest();

    /* get data into local variables for fast access */
    OdtFirst = daqListPtr->OdtFirst_u16;
    OdtCnt   = daqListPtr->OdtCnt_u8;
    maxDtoAligned = XCP_MAXDTO_ALIGNED(protLayerId);

    /* Initialize local pointer to OdtEntryAdr and OdtEntrySize for first OdtEntry */
    OdtEntryAdrPtr        = Xcp_NoInit[protLayerId].DaqConfig.OdtEntryAddress_p;
    OdtEntrySizePtr       = Xcp_NoInit[protLayerId].DaqConfig.OdtEntrySize_p;
#if (XCP_ADDRESS_EXTENSION == STD_ON)
    OdtEntryExtensionPtr  = Xcp_NoInit[protLayerId].DaqConfig.OdtEntryExtension_p;
#endif

    /*--------------------------------------------------------------------------------------------------
      Main loop for STIM
    --------------------------------------------------------------------------------------------------*/
    /* loop all Odts, check mode for running every loop-step */
    for (AbsOdt_i = OdtFirst; (AbsOdt_i < (OdtFirst + OdtCnt)) && ((daqListPtr->Mode_u8 & XCP_DAQLISTMODE_RUNNING) == XCP_DAQLISTMODE_RUNNING); AbsOdt_i++)
    {
      XCP_TESTINJECTION_FUNC(XCP_TESTINJECTION_POS_ODT_EVENTCHANNEL)

      OdtEntry_i = XCP_ODT_ODTENTRYFIRST(AbsOdt_i, protLayerId);

      /* Get pointer to STIM buffer */
      source_pu8 = &(daqListPtr->DaqListQue_p[ maxDtoAligned * ( ((readBufferActive * OdtCnt) + AbsOdt_i) - OdtFirst) ]);

      /* Fetch number of OdtEntries for this Odt */
      OdtEntryMax = XCP_ODT_ENTRYMAX(AbsOdt_i, protLayerId);

      /* loop all OdtEntries for this Odt */
      for (/* OdtEntry_i already initialized */; OdtEntry_i < OdtEntryMax; OdtEntry_i++)
      {
        XCP_TESTINJECTION_FUNC(XCP_TESTINJECTION_POS_ODTENTRY_EVENTCHANNEL)
        /* Local copy of OdtEntrySize for faster access */
        OdtEntrySize = OdtEntrySizePtr[OdtEntry_i];
        OdtEntryXcpAddr.AddrValue = OdtEntryAdrPtr[OdtEntry_i];
#if (XCP_ADDRESS_EXTENSION == STD_ON)
        OdtEntryXcpAddr.Extension = OdtEntryExtensionPtr[OdtEntry_i];
#else
        OdtEntryXcpAddr.Extension = 0u;
#endif

#if (XCP_DAQ_STIM_MEMORY_ACCESS_BY_APPL == STD_ON)
        /* Copy the STIM data from internal buffer to ecu memory using Application-function */
        XcpAppl_OdtEntryWrite(OdtEntryXcpAddr, source_pu8, OdtEntrySize);
#else
        /* copy the STIM data - Extension is ignored as it shall be 0 now (either Extension is disabled or an AddressTransformation was already done during config) */
        /* MR12 RULE 11.6, DIR 1.1 VIOLATION: The input parameters of rba_BswSrv_MemCopy() are declared as (void*) for generic implementation of MemCopy. */
        Xcp_MemCopy((void*)OdtEntryXcpAddr.AddrValue, source_pu8, OdtEntrySize);
#endif

        /* increment source pointer */
        source_pu8 += OdtEntrySize;
      }
    }

    /* enter lock around buffer access */
    SchM_Enter_Xcp_BufferAccessActiveNoNest();
    /* reset read active Buffer */
    daqListPtr->BufferInfo.BufferNumReadActive_u8 = 0xFFu;
    /* exit lock around buffer access */
    SchM_Exit_Xcp_BufferAccessActiveNoNest();

#if (XCP_EV_STIM_TIMEOUT_ENABLED == STD_ON)
    /* if no fresh data has been received */
    if (RetSt.numOfReceivedStimdata_u8 == 0u)
    {
      /* local variables only needed here */
      Xcp_Cto8_t XcpEvPacket;
      Xcp_EvStimTimeout_t* EvPtr;

      /* initialize event packet */
      XcpEvPacket.Length_u32 = XCP_EV_STIM_TIMEOUT_LENGTH;
       /* MR12 RULE 11.5, DIR 1.1 VIOLATION: The calling layers ensure correct alignment. This cast allows easier (= safer) access to the data in the buffer
                                                  Casting over void* is necessary as otherwise some compilers complain */

      EvPtr = (Xcp_EvStimTimeout_t*) (void*) XcpEvPacket.Buffer_au8;

      /* prepare timeout event */
      EvPtr->PacketId_u8  = XCP_PACKET_ID_EV;
      EvPtr->EventCode_u8 = XCP_EV_STIM_TIMEOUT;
      EvPtr->InfoType_u8  = XCP_EV_STIM_TIMEOUT_INFOTYPE_DAQLISTNO;
      EvPtr->Reserved_u8  = 0;
      EvPtr->Number_u16   = daqNo;

      /* send out event */
      Xcp_SendEv(&XcpEvPacket, protLayerId);
    }
#endif
  }
  else
  {
    /* no complete stim dataset received */
    /* exit lock around buffer access */
    SchM_Exit_Xcp_BufferAccessActiveNoNest();
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return RetSt;
}


#if (XCP_BYPASS_CONSISTENCY == STD_ON)
/**
****************************************************************************************************
This function checks the bypass STIM event channels for the bypass consistency
\param[in]  EventChannelNum   event channel number
\param[in]  protLayerId   protocol layer id
\return     none
***************************************************************************************************/
void Xcp_ConsistencyCheck(uint16 EventChannelNum, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Xcp_DaqList_t*        daqListPtrSTIM;
  boolean allStimDtosReceived = TRUE;
  uint16 EventChannelNum_local;
  uint16 relatedEventChannelNum;
  uint16 daqListNo;
  uint8 ConsistencyStatus = 0u;
  uint8 offset;
  uint8 failureType = 0u;
  boolean BypassStimFound = FALSE;

  /* store the event channel number into local variable */
  EventChannelNum_local = EventChannelNum;
  /* store the related event channel number */
  relatedEventChannelNum = XCP_BYPASS(EventChannelNum_local).RelatedEventChannelNumber_u16;

  /* loop through all daq lists */
  for (daqListNo = 0; daqListNo < (Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16); daqListNo++)
  {
    daqListPtrSTIM = &(XCP_DAQ(daqListNo, protLayerId));
    /* check if daqlist event channel number matches current STIM channel and daqlist is also in running state */
    if ( (daqListPtrSTIM->EventChannelNum_u16 == EventChannelNum_local)
      &&  ((daqListPtrSTIM->Mode_u8 & XCP_DAQLISTMODE_RUNNING) == XCP_DAQLISTMODE_RUNNING ) )
    {
      /* obtained a running STIM list linked to the bypass event channel */
      BypassStimFound = TRUE;
      /* check if a complete set of data has been received for this STIM list */
      if (daqListPtrSTIM->BufferInfo.OdtReceiveCtr_u8 != 0u)
      {
        /* if any one STIM list is incomplete, the allStimDtosReceived must be set to FALSE */
        allStimDtosReceived = FALSE;
        /* set failure type to timeout */
        failureType = 0u;
        /* update the tolerance counter for this event channel */
        XCP_BYPASS(EventChannelNum_local).InconsistencyTolerance_u8 += 1u;
      }
      else
      {
        /* do nothing as the default value of allStimDtosReceived is TRUE */
      }

      /* we can break as we only have one STIM list linked to an event channel */
      break;
    }
  }

  /* check if we have found a daqlist linked to the STIM bypass event channel */
  if (BypassStimFound == TRUE)
  {
    /* check if we have received all the DTOs for the STIM lists of this bypass event channel */
    if (allStimDtosReceived == TRUE )
    {
      /* check if the bypass relationship is 1:N or N:1 */
      if (relatedEventChannelNum == EventChannelNum_local )
      {
        /* we have N:1 configured as STIM channel is related to itself */
        offset = 1u;
      }
      else
      {
        /* else it is 1:N configured */
        offset = 0u;
      }

      /* check if the event counter received from master matches the related event channel counter */
      if ((XCP_BYPASS(EventChannelNum_local).EventCounterReceived_u8 + offset) == XCP_BYPASS(relatedEventChannelNum).EventCounter_u8)
      {
        /* event counter matches. nothing to be done */
      }
      else
      {
        /* consistency check fails */
        /* set failure time to DTO CTR check failed */
        failureType = 1u;
        /* update the tolerance counter for this event channel */
        XCP_BYPASS(EventChannelNum_local).InconsistencyTolerance_u8 += 1u;
      }
    }
    else
    {
      /* incomplete STIM data received. processed in below check */
    }

    /* check if the tolerance counter is equal to the threshold value */
    if (XCP_BYPASS(EventChannelNum_local).InconsistencyTolerance_u8 >= XCP_BYPASS_TIMEOUT_THRESHOLD)
    {
      ConsistencyStatus = 0u;
    }
    else
    {
      /* if threshold is not reached yet, do not send error packet */
      ConsistencyStatus = 1u;
    }

    /* check the bypass consistency status */
    if (ConsistencyStatus == 0u)
    {
      /* Bypass consistency failed. send STIM TIMEOUT response */
      /* local variables only needed here */
      Xcp_Cto8_t XcpEvPacket;
      Xcp_EvStimTimeout_t* EvPtr;

      /* update global flag to false */
      XCP_BYPASS(EventChannelNum_local).ConsistencyCheckFlag_u8 = FALSE;

      /* initialize event packet */
      XcpEvPacket.Length_u32 = XCP_EV_STIM_TIMEOUT_LENGTH;
      /* MR12 RULE 11.5, DIR 1.1 VIOLATION: The calling layers ensure correct alignment. This cast allows easier (= safer) access to the data in the buffer
                                              Casting over void* is necessary as otherwise some compilers complain */
      EvPtr = (Xcp_EvStimTimeout_t*) (void*) XcpEvPacket.Buffer_au8;

      /* prepare timeout event */
      EvPtr->PacketId_u8  = XCP_PACKET_ID_EV;
      EvPtr->EventCode_u8 = XCP_EV_STIM_TIMEOUT;
      EvPtr->InfoType_u8  = XCP_EV_STIM_TIMEOUT_INFOTYPE_EVENTCHANNEL;
      EvPtr->Reserved_u8  = failureType;
      EvPtr->Number_u16   = EventChannelNum_local; /* return the event channel number */

      /* send out event */
      Xcp_SendEv(&XcpEvPacket, protLayerId);
    }
    else
    {
      /* consistency is successful. do not send any timeout response */
    }
  }
  else
  {
    /* the correct STIM bypass daqlist was not found. do nothing */
  }
}
#endif


#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#endif /* XCP_SYNCHRONOUS_DATA_STIMULATION_STIM */


