

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
/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
Function reads data at the requested address (MTA) with requested length based on the Protocol layer
\param[in]  AddrPtrDst    destination address
\param[in]  Length        length of the data to read
\param[in]  protLayerId   Protocol Layer Id
\return     ErrorCode
\retval     XCP_NO_ERROR            Reading successful
\retval     XCP_ERR_ACCESS_DENIED   Source or destination is in restricted memory area
\retval     XCP_ERR_PAGE_NOT_VALID  Calibration page is not valid
***************************************************************************************************/
/* MR12 RULE 8.13 VIOLATION: The pointer AddrPtrDst cannot be const as data is modified in rba_BswSrv_MemCopy */
Xcp_ErrorCode Xcp_MemRead(uint8* AddrPtrDst, uint8 Length, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Xcp_ErrorCode   Error;
  Xcp_AddrType_t  LocalMta;

  /* Initialize error variable */
  Error = XCP_NO_ERROR;

  /* Store address in local variable */
  LocalMta = XCP_MTA(protLayerId);

  /* If Extension is 0 the AddrValue 0 is invalid */
  if ((LocalMta.AddrValue == 0u) && (LocalMta.Extension == 0u))
  {
    Error = XCP_ERR_WRITE_PROTECTED;
  }
  else if (Xcp_Cleared[protLayerId].BgActivityState != XCP_BG_IDLE)
  { /* Another background activity is running -> report BUSY */
    Error = XCP_ERR_CMD_BUSY;
  }
  else
  {
#if (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_ON)
    /* Call Address-transformation-function */
    Error = XcpAppl_StaticAddressTransformation(&LocalMta, Length, XCP_EVENT_CHANNEL_NOT_USED, XCP_CONTEXT_UPLOAD);
#endif
  }

#if (XCP_MEMORY_READ_PROTECTION == STD_ON)
  if (Error == XCP_NO_ERROR)
  {
    /* Check source memory protection */
    Error = Xcp_MemReadProtectionCheck(LocalMta, Length, protLayerId);
  }
#endif

  /*  XCP_NO_ACCESS_HIDE */
  if (Error == XCP_NO_ACCESS_HIDE)
  {
    /* Fill destination buffer with zeroes */
    /* MR12 DIR 1.1 VIOLATION: The input parameters of rba_BswSrv_MemSet() are declared as (void*) for generic implementation of MemSet. */
    Xcp_MemSet(AddrPtrDst, 0, Length);

    /* Set error code to OK */
    Error = XCP_NO_ERROR;
  }
  else if (Error == XCP_NO_ERROR)
  {

#if (XCP_UPLOAD_DOWNLOAD_MEMORY_ACCESS_BY_APPL == STD_ON)
    /* Copy data with application function */
    Error = XcpAppl_MemRead(AddrPtrDst, LocalMta, Length, protLayerId);
#else

# if (XCP_ADDRESS_EXTENSION == STD_ON)
    /* If the extension is not 0 it cannot be directly accessed here -> ACCESS_DENIED
     * The extension and address should have been transformed to a valid ECU-address in StaticAddressTransformation */
    if (LocalMta.Extension != 0u)
    {
      Error = XCP_ERR_ACCESS_DENIED;
    }
    else
# endif
    {
      /* Copy data with internal function */
      /* MR12 RULE 11.6, DIR 1.1 VIOLATION: The input parameters of rba_BswSrv_MemCopy() are declared as (void*) for generic implementation of MemCopy. */
      Xcp_MemCopy(AddrPtrDst, (void *)LocalMta.AddrValue, Length);
      Error = XCP_NO_ERROR;
    }
#endif

    /* Check if success */
    if (Error == XCP_NO_ERROR)
    {
      /* Update MTA pointer */
      XCP_MTA(protLayerId).AddrValue += Length;
    }
  }
  else
  {
    /* other errors are sent out directly */
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return(Error);
}

#if(XCP_CALIBRATION_CAL == STD_ON)

/**
****************************************************************************************************
Function writes data at the requested address with requested length based on the Protocol layer
\param[in]  AddrPtrSrc    source pointer
\param[in]  Length        length of the data to write
\param[in]  protLayerId   Protocol Layer Id
\return     ErrorCode
\retval     XCP_NO_ERROR              Writing successful
\retval     XCP_ERR_ACCESS_DENIED     Source or destination is in restricted memory area
\retval     XCP_ERR_PAGE_NOT_VALID    Calibration page is not valid
\retval     XCP_ERR_WRITE_PROTECTED   Destination is write protected
***************************************************************************************************/
Xcp_ErrorCode Xcp_MemWrite(const uint8* AddrPtrSrc, uint8 Length, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Xcp_ErrorCode   Error;
  Xcp_AddrType_t  LocalMta;

  /* Store address in local variable */
  LocalMta = XCP_MTA(protLayerId);

  /* If Extension is 0 the AddrValue 0 is invalid */
  if ((LocalMta.AddrValue == 0u) && (LocalMta.Extension == 0u))
  {
    Error = XCP_ERR_WRITE_PROTECTED;
  }
  else if (Xcp_Cleared[protLayerId].BgActivityState != XCP_BG_IDLE)
  { /* Another background activity is running -> report BUSY */
    Error = XCP_ERR_CMD_BUSY;
  }
  else
#if (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_ON)
  {
    /* Call Address-transformation-function */
    Error = XcpAppl_StaticAddressTransformation(&LocalMta, Length, XCP_EVENT_CHANNEL_NOT_USED, XCP_CONTEXT_DOWNLOAD);
  }

  if (Error == XCP_NO_ERROR)
#endif
  {
    /* Check dest memory protection */
    Error = Xcp_MemWriteProtectionCheck(LocalMta, Length);
  }


  if (Error == XCP_NO_ERROR)
  {
#if (XCP_UPLOAD_DOWNLOAD_MEMORY_ACCESS_BY_APPL == STD_ON)
    /* Check if data should be copied to global buffer */
    if (Xcp_Cleared[protLayerId].Mem.DownloadBuffer_au8 != AddrPtrSrc)
    {
      /* Copy data to global buffer. XcpAppl_MemWrite will not work with local variable if data cannot be copied at once. */
      /* MR12 RULE 11.6, DIR 1.1 VIOLATION: The input parameters of rba_BswSrv_MemCopy() are declared as (void*) for generic implementation of MemCopy. */
      Xcp_MemCopy(Xcp_Cleared[protLayerId].Mem.DownloadBuffer_au8, AddrPtrSrc, Length);
    }
    /* Write data with application function */
    Error = XcpAppl_MemWrite(LocalMta, Xcp_Cleared[protLayerId].Mem.DownloadBuffer_au8, Length, protLayerId);

    /* Check if success */
    if (Error == XCP_NO_ERROR)
    {
      /* Update MTA pointer */
      XCP_MTA(protLayerId).AddrValue += Length;
    }
    else if (Error == XCP_NO_RESPONSE)
    {
      /* Set Download size to trigger Xcp_MemWriteMainFunction */
      Xcp_Cleared[protLayerId].Mem.DownloadSize_u8 = Length;
      Xcp_Cleared[protLayerId].BgActivityState = XCP_BG_MEM_WRITE;
    }
    else
    {
      /* other errors are sent out directly */
    }
#else

# if (XCP_ADDRESS_EXTENSION == STD_ON)
    /* If the extension is not 0 it cannot be directly accessed here -> ACCESS_DENIED
     * The extension and address should have been transformed to a valid ECU-address in StaticAddressTransformation */
    if (LocalMta.Extension != 0u)
    {
      Error = XCP_ERR_ACCESS_DENIED;
    }
    else
# endif
    {
      /* Lock interrupts */
      SchM_Enter_Xcp_DownloadNoNest();

      /* Copy data with internal function */
      /* MR12 RULE 11.6, DIR 1.1 VIOLATION: The input parameters of rba_BswSrv_MemCopy() are declared as (void*) for generic implementation of MemCopy. */
      Xcp_MemCopy((void *)LocalMta.AddrValue, AddrPtrSrc, Length);

      /* Unlock interrupts */
      SchM_Exit_Xcp_DownloadNoNest();

      /* Update MTA pointer */
      XCP_MTA(protLayerId).AddrValue += Length;

      Error = XCP_NO_ERROR;
    }
#endif
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return(Error);
}

/**
****************************************************************************************************
Background process of WriteMTA for the requested Protocol layer
\param[in]  protLayerId   protocol layer id
\return     -
***************************************************************************************************/
void Xcp_MemWriteMainFunction(uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

#if (XCP_UPLOAD_DOWNLOAD_MEMORY_ACCESS_BY_APPL == STD_ON)
  /* Local variables */
  Xcp_ErrorCode Error;

  /* Call application function */
  Error = XcpAppl_MemWriteMainFunction(protLayerId);

  if (Error == XCP_NO_ERROR)
  {
    /* Update MTA pointer */
    XCP_MTA(protLayerId).AddrValue += Xcp_Cleared[protLayerId].Mem.DownloadSize_u8;

    /* Clear background activity */
    Xcp_Cleared[protLayerId].BgActivityState = XCP_BG_IDLE;
  }

  /* Send response */
  Xcp_DownloadRes(Error, protLayerId);
#else
  XCP_PARAM_UNUSED(protLayerId);
#endif /* #if (XCP_UPLOAD_DOWNLOAD_MEMORY_ACCESS_BY_APPL == STD_ON) */

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
}

#endif /* #if(XCP_CALIBRATION_CAL == STD_ON) */


#if (XCP_SYNCHRONOUS_DATA_ACQUISITION_DAQ == STD_ON)
/**
****************************************************************************************************
This function calculates the memory consumption of the daq config and sets the required pointers accordingly.
\param[in]  AddDaqList    number of DaqList to add for memory calculation
\param[in]  AddOdt        number of Odt to add for memory calculation
\param[in]  AddOdtEntry   number of OdtEntry to add for memory calculation
\param[in]  protLayerId   Protocol Layer Id
\return     bytes required for daq configuration
***************************************************************************************************/
uint32 Xcp_DaqRamCalc(uint16 AddDaqList, uint8 AddOdt, uint8 AddOdtEntry, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Xcp_DaqConfig_t* DaqConfig;
  uint32  memReq;
  uint32  DaqListCntNew_u32;
  uint32  OdtCntNew_u32;
  uint32  OdtEntryCntNew_u32;

  /* initialization */
  DaqConfig = &Xcp_NoInit[protLayerId].DaqConfig;

  DaqListCntNew_u32  = DaqConfig->DaqListCnt_u16  + AddDaqList;
  OdtCntNew_u32      = DaqConfig->OdtCnt_u16      + AddOdt;
  OdtEntryCntNew_u32 = DaqConfig->OdtEntryCnt_u16 + AddOdtEntry;

  /* check if all (new) values are within limits */
  if (   (DaqListCntNew_u32  > XCP_MAX_DAQ)
      || (OdtCntNew_u32      > XCP_MAX_ODT)
      || (OdtEntryCntNew_u32 > XCP_MAX_ODTENTRY)
      || (DaqConfig->DaqRamPtr_pu8 == NULL_PTR)
     )
  {
    /* to prevent counter overflow, force memory overflow error */
    memReq = 0xFFFFFFFFu;
  }
  else
  {
    /* --------------------------------------------------------------------------------------------------
     * DAQ pointer
     * ----------------------
     * Start:        Take over dynamic ram pointer
     * Mem required: DaqListCntNew_u32 * XCP_SIZEOF_DAQ
     * --------------------------------------------------------------------------------------------------
     */
    /* MR12 RULE 11.5, DIR 1.1 VIOLATION: The calling layers ensure correct alignment. No other possibility to assign the pointer to the other pointer
                                      Casting over void* is necessary as otherwise some compilers complain */
    DaqConfig->DaqList_p = (Xcp_DaqList_t*) (void*) DaqConfig->DaqRamPtr_pu8;

    /* --------------------------------------------------------------------------------------------------
     * ODT pointer
     * ----------------------
     * Start:        After DAQ-Lists - 2-byte-aligned
     * Mem required: OdtCntNew_u32 * XCP_SIZEOF_ODT
     * --------------------------------------------------------------------------------------------------
     */
    /* MR12 RULE 11.4, 11.5, 11.6 VIOLATION: We want to have a pointer to a 2-byte-aligned address at/after the given element (address), hence casting is necessary */
    DaqConfig->Odt_p = (Xcp_Odt_t*)(XCP_2BYTE_ALIGN((XCP_ADDR_TYPE)&DaqConfig->DaqList_p[DaqListCntNew_u32]));

    /* --------------------------------------------------------------------------------------------------
     * OdtEntryAddress-Pointer
     * ----------------------
     * Start:        After ODTs - 4/8-byte-aligned
     * Mem required: OdtEntryCntNew_u32 * 4/8 (Address is 32/64-bit)
     * --------------------------------------------------------------------------------------------------
     */
    /* MR12 RULE 11.4, 11.5, 11.6 VIOLATION: We want to have a pointer to a 4/8-byte-aligned address at/after the given element (address), hence casting is necessary */
    DaqConfig->OdtEntryAddress_p = (Xcp_AddrValue*)(XCP_MACHINE_ALIGN((XCP_ADDR_TYPE)&DaqConfig->Odt_p[OdtCntNew_u32]));

#if (XCP_ADDRESS_EXTENSION == STD_ON)
    /* --------------------------------------------------------------------------------------------------
     * OdtEntryExtension-Pointer
     * ----------------------
     * Start:        After OdtEntryAddresses - 4/8-byte-aligned (automatically because OdtEntryAddress is aligned and Addresses do not break alignment)
     * Mem required: OdtEntryCntNew_u32 * 1 (Size is 8-bit)
     * --------------------------------------------------------------------------------------------------
     */
    /* MR12 RULE 11.5, DIR 1.1 VIOLATION: No problem with alignment as an OdtEntry is 1 byte */
    DaqConfig->OdtEntryExtension_p = (uint8*) (void*)(&DaqConfig->OdtEntryAddress_p[OdtEntryCntNew_u32]);
#endif

    /* --------------------------------------------------------------------------------------------------
     * OdtEntrySize-Pointer
     * ----------------------
     * Start:        After OdtEntryExtension - not aligned if extension is enabled, 4/8-byte-aligned if not
     * Mem required: OdtEntryCntNew_u32 * 1 (Size is 8-bit)
     * --------------------------------------------------------------------------------------------------
     */
    /* MR12 RULE 11.5, DIR 1.1 VIOLATION: No problem with alignment as an OdtEntry is 1 byte */
#if (XCP_ADDRESS_EXTENSION == STD_ON)
    DaqConfig->OdtEntrySize_p = (uint8*) (void*)(&DaqConfig->OdtEntryExtension_p[OdtEntryCntNew_u32]);
#else
    DaqConfig->OdtEntrySize_p = (uint8*) (void*)(&DaqConfig->OdtEntryAddress_p[OdtEntryCntNew_u32]);
#endif

    /* --------------------------------------------------------------------------------------------------
     * PriorityList-Pointer
     * ----------------------
     * Start:        After OdtEntrySizes - 2-byte-aligned
     * Mem required: DaqListCntNew_u32 * sizeof(DaqConfig->DaqListCnt_u16) - 2 Bytes per configured DaqList (although STIM DAQs do not need an entry)
     * --------------------------------------------------------------------------------------------------
     */
    /* MR12 RULE 11.4, 11.5, 11.6 VIOLATION: We want to have a pointer to a 2-byte-aligned address at/after the given element (address), hence casting is necessary */
    DaqConfig->PriorityList_p = (uint16*)(XCP_2BYTE_ALIGN((XCP_ADDR_TYPE)&DaqConfig->OdtEntrySize_p[OdtEntryCntNew_u32]));

    /* --------------------------------------------------------------------------------------------------
     * Queue-Pointer
     * ----------------------
     * Start:        After PriorityList - 4/8-byte-aligned
     * Mem required: Xcp_DaqQueRamCalc(AddOdt, FALSE, protLayerId)
     * --------------------------------------------------------------------------------------------------
     */
    /* MR12 RULE 11.4, 11.5, 11.6 VIOLATION: We want to have a pointer to a 4-byte-aligned address at/after the given element (address), hence casting is necessary */
    DaqConfig->DaqQue_p = (uint8*)(XCP_MACHINE_ALIGN((XCP_ADDR_TYPE)&DaqConfig->PriorityList_p[DaqListCntNew_u32]));

    /* Calculate required memory for everything (last pointer + size required for last data - start pointer) */
    /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
    memReq = ((XCP_ADDR_TYPE)DaqConfig->DaqQue_p + Xcp_DaqQueRamCalc(AddOdt, FALSE, protLayerId)) - (XCP_ADDR_TYPE)DaqConfig->DaqList_p;
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return memReq;
}


/**
****************************************************************************************************
This function calculates the memory consumption of the daq queue and initializes the queue pointers.
\param[in]  AddOdt          number of Odt to add for memory calculation
\param[in]  setQuePointers  TRUE: Queue pointers are set, FALSE: only memory calculation
\param[in]  protLayerId     Protocol Layer Id
\return     bytes required for daqlist queue
***************************************************************************************************/
uint32 Xcp_DaqQueRamCalc(uint8 AddOdt, boolean setQuePointers, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

    /* Local variables */
    Xcp_DaqList_t *daqListPtr;
    uint32 MemReq;
    uint16_least DAQListNo;
    uint16 OdtElementsInQue;
    uint8  Odts;



    /* initialization */
    MemReq = 0;

    /* Loop through all DaqList */
    for (DAQListNo = 0; DAQListNo < Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16; DAQListNo++)
    {
      daqListPtr = &(XCP_DAQ(DAQListNo, protLayerId));
      if (setQuePointers == TRUE)
      {
        /* alloc Buffer */
        daqListPtr->DaqListQue_p = &(Xcp_NoInit[protLayerId].DaqConfig.DaqQue_p[MemReq]);
      }

      /* fetch the numbers of ODTs in this daqlist */
      Odts = daqListPtr->OdtCnt_u8;

      if (Odts == 0)
      {
        /* first daqlist with no odts, assume odts are added to this daqlist */
        Odts = AddOdt;
        /* reset AddOdt to avoid multiple counting */
        AddOdt = 0;
      }
      /* calc the memory needed */
#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
      if ( (daqListPtr->Mode_u8 & XCP_DAQLISTMODE_DIRECTION_STIM) != 0u)
      {
        MemReq += (Odts * XCP_MAXDTO_ALIGNED(protLayerId) * XCP_ODT_STIM_BUFFER_ELEMENT_SIZE);
      }
      else
#endif
      {
#if (XCP_VERSION_ABOVE_1_2 == STD_OFF)
        OdtElementsInQue = (uint16) Xcp_Daq_OdtElementsInDaqQue((Odts)) ; /*PRQA S 4119*/ /* Cast from float to uint32: rounding is intended */
        MemReq += (OdtElementsInQue * XCP_MAXDTO_ALIGNED(protLayerId));
        if (setQuePointers == TRUE)
        {

          /* Initialize the DAQ Queue */
          Xcp_CreateQue(daqListPtr, OdtElementsInQue);
        }
#else

        if((XCP_DAQ_FLAGS(DAQListNo, protLayerId) & XCP_DAQFLAG_RECEIVED_DAQ_LIST_MODE) == XCP_DAQFLAG_RECEIVED_DAQ_LIST_MODE)
        {
          OdtElementsInQue = (uint16) Xcp_Daq_OdtElementsInDaqQue((Odts), (daqListPtr->EventChannelNum_u16)) ;
          MemReq += (OdtElementsInQue * XCP_MAXDTO_ALIGNED(protLayerId));

          if (setQuePointers == TRUE)
          {
            /* Initialize the DAQ Queue */
            Xcp_CreateQue(daqListPtr, OdtElementsInQue);
          }
        }
#endif

      }
      /* alignment for next item */
      MemReq = XCP_MACHINE_ALIGN(MemReq);

    } /* end of loop through DAQs */

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return MemReq;
}


/**
****************************************************************************************************
This function checks the memory consumption of the daq config. If it fits into the reserved
memory, it will give a positive return value.
\param[in]  AddDaqList    number of DaqList to add for memory calculation
\param[in]  AddOdt        number of Odt to add for memory calculation
\param[in]  AddOdtEntry   number of OdtEntry to add for memory calculation
\param[in]  protLayerId   Protocol Layer Id
\return     TRUE: memory is sufficient
***************************************************************************************************/
boolean Xcp_DaqRamCheck(uint16 AddDaqList, uint8 AddOdt, uint8 AddOdtEntry, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  boolean retval;

  /* initialization */
  retval = FALSE;

  if (Xcp_NoInit[protLayerId].DaqConfig.DaqRamSize_u32 >= Xcp_DaqRamCalc(AddDaqList, AddOdt, AddOdtEntry, protLayerId))
  {
    retval = TRUE;
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return retval;
}
#endif

#if (XCP_CALIBRATION_CAL == STD_ON || XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)

/**
****************************************************************************************************
Function checks the write destination lies within xcp boundaries or not
\param[in]  XcpAddr   destination address
\param[in]  Length    length of the data to check write access
\return     ErrorCode
\retval     XCP_NO_ERROR            Reading successful
\retval     XCP_ERR_ACCESS_DENIED   Source or destination is in restricted memory area
\retval     XCP_ERR_PAGE_NOT_VALID  Calibration page is not valid
***************************************************************************************************/
Xcp_ErrorCode Xcp_MemWriteProtectionCheck(const Xcp_AddrType_t XcpAddr, uint8 Length)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  Xcp_ErrorCode Error;
#if (XCP_SYNCHRONOUS_DATA_ACQUISITION_DAQ == STD_ON)
  uint8_least i;
#endif

  /* initialization */
  Error = XCP_NO_ERROR;

#if (XCP_ADDRESS_EXTENSION == STD_ON)
  /* The checks can only be done if AddressExtension = 0 as only here memory is accessed directly */
  if (XcpAddr.Extension == 0u)
#endif
  {
    /* If address lies with in static Xcp memory respond with access denied error */
    /* Hint for 64bit: the XcpAddr should have run through StaticAddressTransformation and therefore contain a valid 64bit address - otherwise checks and access have to be done in Appl-Function */
    if (
            /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
        (   (((XcpAddr.AddrValue + Length) > (Xcp_AddrValue)&Xcp_NoInit[0])    && (XcpAddr.AddrValue < ((Xcp_AddrValue)&Xcp_NoInit[0]    + (Xcp_AddrValue)(sizeof(Xcp_NoInit_t) * XCP_PL_IDS))))
            /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
         || (((XcpAddr.AddrValue + Length) > (Xcp_AddrValue)&Xcp_GlobalNoInit) && (XcpAddr.AddrValue < ((Xcp_AddrValue)&Xcp_GlobalNoInit + (Xcp_AddrValue)(sizeof(Xcp_GlobalNoInit_t)))))
            /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
         || (((XcpAddr.AddrValue + Length) > (Xcp_AddrValue)&Xcp_Cleared[0])   && (XcpAddr.AddrValue < ((Xcp_AddrValue)&Xcp_Cleared[0]   + (Xcp_AddrValue)(sizeof(Xcp_Cleared_t) * XCP_PL_IDS))))
       ))
    {
      Error = XCP_ERR_ACCESS_DENIED;
    }
    else
    {
#if (XCP_SYNCHRONOUS_DATA_ACQUISITION_DAQ == STD_ON)
      for (i = 0; i < XCP_PL_IDS; i++)
      {
        /* check if address lies within the dynamic memory of any protocol layer */
        if (
                /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
            (   ((XcpAddr.AddrValue + Length) >  (Xcp_AddrValue)Xcp_NoInit[i].DaqConfig.DaqRamPtr_pu8)
                /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
             && ((XcpAddr.AddrValue)          < ((Xcp_AddrValue)Xcp_NoInit[i].DaqConfig.DaqRamPtr_pu8 + Xcp_NoInit[i].DaqConfig.DaqRamSize_u32))
           ))
        {
          Error = XCP_ERR_ACCESS_DENIED;
          break;
        }
      }
#endif
    }
  }

#if(XCP_MEMORY_WRITE_PROTECTION == STD_ON)
  if (Error == XCP_NO_ERROR)
  {
    Error = XcpAppl_MemWriteProtection(XcpAddr, Length);
  }
#endif

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return(Error);
}
#endif

#if (XCP_MEMORY_READ_PROTECTION == STD_ON)
/**
****************************************************************************************************
Function checks the read destination lies within protect area or not, if it lies within GetID
boundaries then skip read protect.
\param[in]  XcpAddr       destination address
\param[in]  Length        length of the data to check read access
\param[in]  protLayerId   Protocol Layer Id
\return     ErrorCode
\retval     XCP_NO_ERROR            Reading successful
\retval     XCP_ERR_ACCESS_DENIED   Source or destination is in restricted memory area
\retval     XCP_ERR_PAGE_NOT_VALID  Calibration page is not valid
***************************************************************************************************/
Xcp_ErrorCode Xcp_MemReadProtectionCheck(const Xcp_AddrType_t XcpAddr, uint8 Length, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  Xcp_ErrorCode Error;
# if (XCP_COMMANDS_GET_DAQ_INFO == STD_ON)
  uint8 EventChannelNumber = 0;
# endif

  Error = XcpAppl_MemReadProtection(XcpAddr, Length);

  /* If the MemReadProtection returns an error, then check for specific address boundaries */
  if(Error != XCP_NO_ERROR)
  {
# if (XCP_CMD_GET_ID_AVAILABLE == STD_ON)
    /* If address lies in Xcp GET_ID boundaries then no error */
    if ( ((Xcp_NoInit[protLayerId].GetId.GetIdLen_u32 != 0u)
           /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
        && ((XcpAddr.AddrValue)          >=  (Xcp_AddrValue)Xcp_NoInit[protLayerId].GetId.GetIdAddrPtr))
           /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
        && ((XcpAddr.AddrValue + Length) <= ((Xcp_AddrValue)Xcp_NoInit[protLayerId].GetId.GetIdAddrPtr + Xcp_NoInit[protLayerId].GetId.GetIdLen_u32))
#  if (XCP_ADDRESS_EXTENSION == STD_ON)
      /* If ADDRESS_EXTENSION is enabled this check is only needed if we have Extension == 0 -> Direct Memory access */
      && (XcpAddr.Extension == 0u)
#  endif
     )
    {
      Error = XCP_NO_ERROR;
    }
   else
# endif /* (XCP_CMD_GET_ID_AVAILABLE == STD_ON) */
    {
# if (XCP_COMMANDS_GET_DAQ_INFO == STD_ON)
     for(EventChannelNumber = 0; EventChannelNumber < XCP_MAX_EVENT_CHANNEL ; EventChannelNumber++ )
     {
       if( ((Xcp_PlCfgConst.EventChannelCfg[EventChannelNumber].EventChannelNameLength_u8 != 0u)
                /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
            && ((XcpAddr.AddrValue)          >=  (Xcp_AddrValue)Xcp_PlCfgConst.EventChannelCfg[EventChannelNumber].EventChannelName))
                /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
            && ((XcpAddr.AddrValue + Length) <= ((Xcp_AddrValue)Xcp_PlCfgConst.EventChannelCfg[EventChannelNumber].EventChannelName + Xcp_PlCfgConst.EventChannelCfg[EventChannelNumber].EventChannelNameLength_u8))
      #  if (XCP_ADDRESS_EXTENSION == STD_ON)
               /* If ADDRESS_EXTENSION is enabled this check is only needed if we have Extension == 0 -> Direct Memory access */
            && (XcpAddr.Extension == 0u)
      #  endif
         )
       {
         Error = XCP_NO_ERROR;
         break;
       }
     }
# endif
    XCP_PARAM_UNUSED(protLayerId);
    }
  }
  else
  {
      /* Nothing to do */
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return(Error);
}
#endif

/**
****************************************************************************************************
Function stores command for later processing
\param in     : XcpPacket       - received XcpPacket
\param in     : protLayerId     - Protocol Layer Id
\return         -
***************************************************************************************************/
void Xcp_StoreCommand(const PduInfoType* XcpPacket, uint8 protLayerId)
{
  /* check, whether buffer is free */
  if (Xcp_Cleared[protLayerId].BgActivityState != XCP_BG_REPEAT_CMD) /* should only occur, when master did not wait for Ack */
  {
    /* store Command into buffer for later usage, only 8 bytes are allowed for commands */
    /* MR12 DIR 1.1 VIOLATION: The input parameters of rba_BswSrv_MemCopy() are declared as (void*) for generic implementation of MemCopy. */
    Xcp_MemCopy(XCP_CMD_BUFFER_PTR(protLayerId), XcpPacket->SduDataPtr, XCP_MIN(XcpPacket->SduLength, 8u));
    XCP_CMD_BUFFER_LENGTH(protLayerId) = XcpPacket->SduLength;
    Xcp_Cleared[protLayerId].BgActivityState = XCP_BG_REPEAT_CMD;
  }
  else /* cmd buffer already full */
  {
    /* another command has been triggered, although last command not finished, discard command */
    XCP_DET_REPORT_ERROR_NO_RET(XCP_UTILS_SID, XCP_E_UNEXPECTED_FUNCTION_CALL);
  }
}

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

