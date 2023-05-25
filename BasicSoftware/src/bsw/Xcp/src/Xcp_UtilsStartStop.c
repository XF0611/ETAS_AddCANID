

/***************************************************************************************************
* Includes
***************************************************************************************************/
#include "Xcp.h"
#include "Xcp_Priv.h"


/***************************************************************************************************
* Variables
***************************************************************************************************/
#if (XCP_SYNCHRONOUS_DATA_ACQUISITION_DAQ == STD_ON)

#if (XCP_OM_ODT_OPTIMIZATION_ENABLED == STD_ON)
/** Map: OdtOptimizationType to CopyRoutine */
static const uint8 Map_OdtOptimizationType_to_CopyRoutine[] = {
    XCP_ODT_COPY_SAFE,  /* XCP_ODT_OPTIMIZATION_OM_DEFAULT            */
    XCP_ODT_COPY_16,    /* XCP_ODT_OPTIMIZATION_OM_ODT_TYPE_16        */
    XCP_ODT_COPY_32,    /* XCP_ODT_OPTIMIZATION_OM_ODT_TYPE_32        */
    XCP_ODT_COPY_32,    /* XCP_ODT_OPTIMIZATION_OM_ODT_TYPE_64        - not supported */
    XCP_ODT_COPY_SAFE,  /* XCP_ODT_OPTIMIZATION_OM_ODT_TYPE_ALIGNMENT - not supported */
    XCP_ODT_COPY_SAFE,  /* XCP_ODT_OPTIMIZATION_OM_MAX_ENTRY_SIZE     - not supported */
};
#endif


/***************************************************************************************************
* Local function declarations
***************************************************************************************************/
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
static Xcp_ErrorCode Xcp_DaqListStopDaqList(uint16 daqListNo, uint8 protLayerId);
static Std_ReturnType Xcp_DaqSetOdtLength(uint16 daqListNo_u16, uint8 protLayerId);
#if (XCP_OM_ODT_OPTIMIZATION_ENABLED == STD_ON)
static uint8 Xcp_DaqCalcCopyRoutine(uint64 value);
#endif
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"


/***************************************************************************************************
* Functions
***************************************************************************************************/

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
Function prepares the start of DaqList
\param[in]  daqListNo     DAQ to start
\param[in]  protLayerId   protocol layer id
\return     -
***************************************************************************************************/
void Xcp_DaqListStart(uint16 daqListNo, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Xcp_DaqList_t*  DaqListPtr;

  /* initialization */
  DaqListPtr = &XCP_DAQ(daqListNo, protLayerId);

  /* allowed to change write pointer without spinlock because DaqList is not started -> EventChannel processing of this DAQ is disabled */
  Xcp_InitDaqQueue(daqListNo, protLayerId);


#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  if ((DaqListPtr->Mode_u8 & XCP_DAQLISTMODE_DIRECTION_STIM) != 0u)
  {
    /* enter lock around buffer access */
    SchM_Enter_Xcp_BufferAccessActiveNoNest();
    /* set status flag for STIM */
    DaqListPtr->BufferInfo.Status_u8 |= XCP_STATUSBIT_STIM_STARTED;
    /* exit lock around buffer access */
    SchM_Exit_Xcp_BufferAccessActiveNoNest();
  }
  else
  {
    /* set status flag for DAQ */
    DaqListPtr->BufferInfo.Status_u8 |= XCP_STATUSBIT_DAQ_STARTED;
  }
#endif

  /* Start DaqList without delay */
  DaqListPtr->CycleCnt_u8 = DaqListPtr->Prescaler_u8;

  /* Set DaqList running flag */
  DaqListPtr->Mode_u8 |= XCP_DAQLISTMODE_RUNNING;

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
}


/**
****************************************************************************************************
Function prepares the start of selected DaqList
\param[in]  protLayerId   protocol layer id
\return     -
***************************************************************************************************/
void Xcp_DaqListStartSelected(uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  uint16  daqListNo;
  boolean DaqListStarted;

  /* initialization */
  DaqListStarted = FALSE;

  /* Loop all DaqList */
  for (daqListNo = 0; daqListNo < Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16; daqListNo++)
  {
    /* Process all selected DaqLists */
    if ( (XCP_DAQ_MODE(daqListNo, protLayerId) & (XCP_DAQLISTMODE_SELECTED)) == XCP_DAQLISTMODE_SELECTED)
    {
      /* If the DaqList is not running, start it */
      if ( (XCP_DAQ_MODE(daqListNo, protLayerId) & (XCP_DAQLISTMODE_RUNNING)) == 0u)
      {
        Xcp_DaqListStart(daqListNo, protLayerId);
        DaqListStarted = TRUE;
      }

      /* Reset selected flag */
      XCP_DAQ_MODE(daqListNo, protLayerId) &= (uint8)(~XCP_DAQLISTMODE_SELECTED);
    }
  }

  if (DaqListStarted == TRUE)
  {
    /* Recalculate PriorityList to add started DaqList */
    Xcp_DaqCalculatePriorityList(protLayerId);
    /* Set DAQ-state to running */
    Xcp_NoInit[protLayerId].DaqConfig.DaqState_en = XCP_DAQ_STATE_RUNNING;
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
}


/**
****************************************************************************************************
This function stops the DaqList
\param[in]  daqListNo     requested DaqList number
\param[in]  protLayerId   Protocol Layer Id
\return     ErrorCode
\retval     XCP_NO_ERROR:       no problem occurred
\retval     XCP_REPEAT_COMMAND: stopped DAQ which is currently processed (e.g. sampling)
***************************************************************************************************/
Xcp_ErrorCode Xcp_DaqListStop(uint16 daqListNo, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* local variables */
  Xcp_ErrorCode Error;
#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  SchM_Enter_Xcp_BufferAccessActiveNoNest();
#endif

  Error = Xcp_DaqListStopDaqList(daqListNo, protLayerId);

#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  SchM_Exit_Xcp_BufferAccessActiveNoNest();
#endif

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()

  return Error;
}


/**
****************************************************************************************************
This function stops selected DaqList
\param[in]  protLayerId   Protocol Layer Id
\return     ErrorCode
\retval     XCP_REPEAT_COMMAND      Eventchannel is currently running, delay processing
\retval     XCP_NO_ERROR            no problem occurred
***************************************************************************************************/
Xcp_ErrorCode Xcp_DaqListStopSelected(uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  uint16 daqListNo;
  boolean DaqListStopped;
  Xcp_ErrorCode Error;

  /* initialization */
  DaqListStopped = FALSE;
  Error = XCP_NO_ERROR;

#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  /* enter lock to change buffers */
  SchM_Enter_Xcp_BufferAccessActiveNoNest();
#endif

  /* Loop all DaqList */
  for (daqListNo = 0; daqListNo < Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16; daqListNo++)
  {
    /* Stop selected DaqList */
    if ( (XCP_DAQ_MODE(daqListNo, protLayerId) & XCP_DAQLISTMODE_SELECTED) == XCP_DAQLISTMODE_SELECTED )
    {
      DaqListStopped = TRUE;

      if (Xcp_DaqListStopDaqList(daqListNo, protLayerId) == XCP_REPEAT_COMMAND)
      {
        Error = XCP_REPEAT_COMMAND;
      }
    }
  }

#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  /* exit lock around buffer access */
  SchM_Exit_Xcp_BufferAccessActiveNoNest();
#endif

  if (DaqListStopped == TRUE)
  {
    /* Recalculate PriorityList to remove stopped DaqList */
    Xcp_DaqCalculatePriorityList(protLayerId);
    /* Process DAQ running flag */
    Xcp_DaqProcessDaqState(protLayerId);
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()

  return Error;
}


/**
****************************************************************************************************
This function stops all DaqList.
\param[in]  protLayerId   Protocol Layer Id
\return     ErrorCode
\retval     XCP_NO_ERROR:       no problem in stopping DAQs has occurred
\retval     XCP_REPEAT_COMMAND: one or more DAQs are being processed (e.g. sampling), cannot stop immediately
***************************************************************************************************/
Xcp_ErrorCode Xcp_DaqListStopAll(uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  uint16 daqListNo;
  Xcp_ErrorCode Error;

  /* initialization */
  Error = XCP_NO_ERROR;


#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  /* enter lock to change buffers */
  SchM_Enter_Xcp_BufferAccessActiveNoNest();
#endif

  /* Loop all DaqList */
  for (daqListNo = 0; daqListNo < Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16; daqListNo++)
  {
    /* check whether DAQ is marked running by Eventchannel */
    if (Xcp_DaqListStopDaqList(daqListNo, protLayerId) == XCP_REPEAT_COMMAND)
    {
      Error = XCP_REPEAT_COMMAND;
    }
  }

#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  /* exit lock around buffer access */
  SchM_Exit_Xcp_BufferAccessActiveNoNest();
#endif

  /* set DaqState */
  Xcp_DaqProcessDaqState(protLayerId);

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()

  return Error;
}


/**
****************************************************************************************************
This function sets the PL-DaqList-State (global) according to the state of the DaqLists of the PL
\param[in]  protLayerId   Protocol Layer Id
\return     -
***************************************************************************************************/
void Xcp_DaqProcessDaqState(uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  uint16_least  DaqListNo;
  boolean DaqModeRunning;
  boolean DaqProcessing;

  /* initialization */
  DaqModeRunning  = FALSE;
  DaqProcessing = FALSE;

  /* Loop through all DaqList
   * For each DaqList check
   * - is running, but should not run MODE is not RUNNING (= was stopped but already active and has not finished until now)
   * - Mode is RUNNING
   */
  for (DaqListNo = 0; DaqListNo < Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16; DaqListNo++)
  {
    if ( (XCP_DAQ_MODE(DaqListNo, protLayerId) & XCP_DAQLISTMODE_RUNNING) == XCP_DAQLISTMODE_RUNNING )
    { /* A running DaqList was found */
      DaqModeRunning = TRUE;
    }
    else if (XCP_DAQ(DaqListNo, protLayerId).CurrentlyRunning_b == TRUE)
    {
      /* DAQ_MODE not running, but processing is still active. "Daq should not run, but is still running" */
      DaqProcessing = TRUE;

      /* We can exit the loop now as the state will be XCP_DAQ_STATE_STOP_REQUESTED */
      break;
    }
    else
    {
      /* nothing */
    }
  }

  /* Now process the set internal flags and set the global state accordingly */
  if (DaqProcessing == TRUE)
  { /* running flag not set, but processing still active */
    Xcp_NoInit[protLayerId].DaqConfig.DaqState_en = XCP_DAQ_STATE_STOP_REQUESTED;
  }
  else
  { /* Now check the running flag */

    if (DaqModeRunning == TRUE)
    { /* At least on DaqList is running -> Global State is also running */
      Xcp_NoInit[protLayerId].DaqConfig.DaqState_en = XCP_DAQ_STATE_RUNNING;
    }
    else
    { /* No DaqList is running: Check if the global state is at least XCP_DAQ_STATE_STOP_REQUESTED and only update if true
       * - the function shall only be called by the Start/Stop-functions, but not in the config phase,
       *   so do not change anything if we are still configuring - someone did something wrong!
       */
      if (Xcp_NoInit[protLayerId].DaqConfig.DaqState_en >= XCP_DAQ_STATE_STOP_REQUESTED)
      {
        Xcp_NoInit[protLayerId].DaqConfig.DaqState_en = XCP_DAQ_STATE_READY_TO_RUN;
      }
    }
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
}


/**
****************************************************************************************************
This function calculates the priority list.
\param[in]  protLayerId   Protocol Layer Id
\return     -
***************************************************************************************************/
void Xcp_DaqCalculatePriorityList(uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  uint16* PriorityList;
  uint16  daqListNo;
  uint16  DaqListSendingCnt;
  uint16_least  i;
  uint16_least  j;
  uint16  daqListNoTemp;


  /* initialization */
  PriorityList = Xcp_NoInit[protLayerId].DaqConfig.PriorityList_p;
  DaqListSendingCnt = 0;

  /* Spin lock around reorganizing PriorityList */
  SchM_Enter_Xcp_SendingLong();
  SchM_Enter_Xcp_SendingShort();

  /* Loop all DaqList */
  for (daqListNo = 0; daqListNo < Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16; daqListNo++)
  {
    /* only running DaqList with direction DAQ */
    if ((XCP_DAQ_MODE(daqListNo, protLayerId) & (XCP_DAQLISTMODE_RUNNING|XCP_DAQLISTMODE_DIRECTION_STIM)) == XCP_DAQLISTMODE_RUNNING)
    {
      PriorityList[DaqListSendingCnt] = daqListNo;
      DaqListSendingCnt++;
    }
  }

  /* save number of sending DaqList globally */
  Xcp_NoInit[protLayerId].DaqConfig.DaqListSendingCnt_u16 = DaqListSendingCnt;

  /* Insertion sort: Sorting by DaqList Priority (high value first)*/
  for (i = 1u; i < DaqListSendingCnt; i++)
  {
    j = i;
    while ( (j > 0u) && (XCP_DAQ_PRIO(PriorityList[j-1u], protLayerId) < XCP_DAQ_PRIO(PriorityList[j], protLayerId)) )
    {
      XCP_SWAP(PriorityList[j-1u], PriorityList[j], daqListNoTemp);
      j--;
    }
  }

  /* Spin lock around reorganizing PriorityList */
  SchM_Exit_Xcp_SendingLong();
  SchM_Exit_Xcp_SendingShort();

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
}


/**
****************************************************************************************************
This function sets the global DaqState for the given PL and, if needed, finalizes the config of the DaqLists
\param[in]  XcpPacket     Xcp Packet Pointer to command
\param[in]  protLayerId   protocol layer id
\return     ErrorCode
\retval     XCP_NO_ERROR           ok to start
\retval     XCP_NO_RESPONSE        DaqRam shifting in progress, don't send response
\retval     XCP_ERR_ACCESS_DENIED  requested address is protected
\retval     XCP_ERR_DAQ_CONFIG     DaqList is not correctly configured
\retval     XCP_ERR_SEQUENCE       SetDaqListMode has not been sent
\retval     XCP_REPEAT_COMMAND     Daq is currently being processed, cannot stop immediately
***************************************************************************************************/
/* HIS METRIC LEVEL VIOLATION IN Xcp_DaqTriggerStateStartStop: Function contains very simple "else if" statements. HIS metric compliance would decrease readability and maintainability. */
Xcp_ErrorCode Xcp_DaqTriggerStateStartStop(const PduInfoType* XcpPacket, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* local variables */
  Xcp_ErrorCode Error;

  /* initialization */
  Error = XCP_NO_ERROR;

  /* Check for state XCP_DAQ_STATE_PREPARE_START */
  if (Xcp_NoInit[protLayerId].DaqConfig.DaqState_en == XCP_DAQ_STATE_PREPARE_START)
  {
    uint16 DaqList = 0;

    /* check, if all DaqList are completed */
    while ( (Error == XCP_NO_ERROR) && (DaqList < Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16) )
    {
      Error = Xcp_DaqListFinalize(DaqList, protLayerId);
      DaqList++;
    }
    if (Error == XCP_NO_ERROR)
    {
#if (XCP_VERSION_ABOVE_1_2 == STD_ON)
        /* Check if sufficient memory is available for the DAQ list with direction DAQ */
      if (Xcp_DaqRamCheck(0, 0, 0, protLayerId) == FALSE)
      {
        Error = XCP_ERR_MEMORY_OVERFLOW;
      }
#endif

#if (XCP_DAQRAM_SHIFTING == STD_ON)
      if (Xcp_DaqRamTriggerMovePL(XcpPacket, protLayerId) == TRUE)
      {
        /* protocol layer now busy moving */
        Xcp_NoInit[protLayerId].DaqConfig.DaqState_en = XCP_DAQ_STATE_SHIFTING;
        Error = XCP_NO_RESPONSE;
      }
      else
#else
      XCP_PARAM_UNUSED(XcpPacket);
#endif
      {
#if (XCP_VERSION_ABOVE_1_2 == STD_ON)
        if (Error == XCP_NO_ERROR)
#endif
        {
          /* set queue pointers */
          (void) Xcp_DaqQueRamCalc(0, TRUE, protLayerId);
          /* Set final DaqState to start DAQ and STIM */
          Xcp_NoInit[protLayerId].DaqConfig.DaqState_en = XCP_DAQ_STATE_READY_TO_RUN;
        }
      }
    }
  } /* Xcp_NoInit[protLayerId].DaqConfig.DaqState_en == XCP_DAQ_STATE_PREPARE_START */
#if (XCP_DAQRAM_SHIFTING == STD_ON)
  /* Check for state XCP_DAQ_STATE_SHIFTING */
  else if (Xcp_NoInit[protLayerId].DaqConfig.DaqState_en == XCP_DAQ_STATE_SHIFTING)
  {
    /* shifting is active, no further commands possible */
    Error = XCP_ERR_CMD_BUSY;
  }
#endif
  /* Check for not allowed states */
  else if (Xcp_NoInit[protLayerId].DaqConfig.DaqState_en < XCP_DAQ_STATE_STOP_REQUESTED)
  {
    Error = XCP_ERR_SEQUENCE;
  }
  /* Check if stop is requested */
  else if (Xcp_NoInit[protLayerId].DaqConfig.DaqState_en == XCP_DAQ_STATE_STOP_REQUESTED)
  {
    /* recheck stop condition */
    Xcp_DaqProcessDaqState(protLayerId);

    if (Xcp_NoInit[protLayerId].DaqConfig.DaqState_en == XCP_DAQ_STATE_STOP_REQUESTED)
    {
      Error = XCP_REPEAT_COMMAND;
    }
  }
  /* State is READY_TO_RUN or RUNNING */
  else
  {
    /* Error = XCP_NO_ERROR; already set before */
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return Error;
}


/**
****************************************************************************************************
This function performs various checks for a given address (access protection, OdtEntry NULL/Size)
and calculates needed values for the containing DaqList (e. g. AddressTransformation, OdtOptimization).
\param[inout] XcpAddrPtr      Pointer to the Address that has to be processed
\param[in]    Size            Size of the data Address points to
\param[in]    daqListNo_u16   DaqList number
\param[in]    protLayerId     Protocol Layer Id
\return     Xcp_ErrorCode
\retval     XCP_NO_ERROR            all checks passed successfully
\retval     XCP_ERR_ACCESS_DENIED   requested address is protected
\retval     XCP_ERR_DAQ_CONFIG      DaqList is not correctly configured
***************************************************************************************************/
/* MR12 RULE 8.13 VIOLATION: The pointer XcpAddrPtr may be changed if XCP_STATIC_ADDRESS_TRANSFORMATION is enabled - therefore it cannot be const. */
Xcp_ErrorCode Xcp_DaqTransformAndCheckOdtEntry(Xcp_AddrType_t* XcpAddrPtr, uint8 Size, uint16 daqListNo_u16, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Xcp_ErrorCode Error;

  /* Initialize variables */
  Error = XCP_NO_ERROR;

#if (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_ON)
  /* Do the Address Transformation */
  Error = XcpAppl_StaticAddressTransformation(XcpAddrPtr,
                                              Size,
                                              XCP_DAQ(daqListNo_u16, protLayerId).EventChannelNum_u16,
                                              ((XCP_DAQ_MODE(daqListNo_u16, protLayerId) & XCP_DAQLISTMODE_DIRECTION_STIM) == 0u) ? XCP_CONTEXT_DAQ : XCP_CONTEXT_STIM);

  /* Only do the remaining checks if we have successfully done the Address-Transformation
   * otherwise we have to do the checks lateron
   */
  if (Error != XCP_NO_ERROR)
  {
    /* Nothing more to do - there is an error */
  }
  else
#endif

/* If MemoryAccessByAppl is disabled the extension shall not be != 0 - otherwise the extension can be handled by the application */
#if (XCP_DAQ_STIM_MEMORY_ACCESS_BY_APPL == STD_OFF)
  /* Memory shall be accessed by Xcp itself, thus only direct access is possible, meaning AddressExtension has to be 0 */
  if (XcpAddrPtr->Extension != 0)
  {
    Error = XCP_ERR_ACCESS_DENIED;
  }
  else
#endif

  {
#if (XCP_MEMORY_READ_PROTECTION == STD_ON)
    if ((XCP_DAQ_MODE(daqListNo_u16, protLayerId)  & XCP_DAQLISTMODE_DIRECTION_STIM) == 0u)
    { /* Direction is DAQ */
      /* If Read Protect is detected for the address then set error */
      if(Xcp_MemReadProtectionCheck(*XcpAddrPtr, Size, protLayerId) != XCP_NO_ERROR)
      {
        XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) = XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) | XCP_DAQFLAG_READ_PROTECTED;
        Error = XCP_ERR_ACCESS_DENIED;
      }
    }
#endif

#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
    if ((XCP_DAQ_MODE(daqListNo_u16, protLayerId)  & XCP_DAQLISTMODE_DIRECTION_STIM) == XCP_DAQLISTMODE_DIRECTION_STIM)
    { /* Direction is STIM */
      /* If Write Protect is detected for the received address then set the Flag */
      if (Xcp_MemWriteProtectionCheck(*XcpAddrPtr, Size)!= XCP_NO_ERROR)
      {
        XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) = XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) | XCP_DAQFLAG_WRITE_PROTECTED;
        Error = XCP_ERR_ACCESS_DENIED;
      }
    }
#endif /* (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON) */

#if (XCP_OM_ODT_OPTIMIZATION_ENABLED == STD_ON)
    if (Error == XCP_NO_ERROR)
    {
      /* Check if the copy routine for this OdtEntry matches the one for the Odt (Set copy routine for the Odt if first OdtEntry or error) */
      Error = Xcp_DaqSetOdtCopyRoutine(*XcpAddrPtr, Size, Xcp_NoInit[protLayerId].DaqConfig.SelectedOdtEntry.AbsOdtNum_u16, protLayerId);
    }
#endif
  }

  /* mark unused parameters */
#if (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_OFF && XCP_MEMORY_READ_PROTECTION == STD_OFF && XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_OFF)

  XCP_PARAM_UNUSED(daqListNo_u16);
# if (XCP_OM_ODT_OPTIMIZATION_ENABLED == STD_OFF)
  XCP_PARAM_UNUSED(Size);
  XCP_PARAM_UNUSED(protLayerId);
# endif

#endif

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return Error;
}


/**
****************************************************************************************************
This function performs various checks for a DaqList (access protection, OdtEntry NULL/Size)
and calculates needed values for the DaqList (e. g. AddressTransformation, OdtOptimization).
\param[in]  daqListNo_u16   DaqList number
\param[in]  protLayerId     Protocol Layer Id
\return     Xcp_ErrorCode
\retval     XCP_NO_ERROR            required memory is allocated
\retval     XCP_ERR_SEQUENCE        DaqListMode not received
\retval     XCP_ERR_ACCESS_DENIED   requested address is protected
\retval     XCP_ERR_DAQ_CONFIG      DaqList is not correctly configured
***************************************************************************************************/
/* HIS METRIC LEVEL VIOLATION IN Xcp_DaqListFinalize: Function contains very simple "else if" statements. HIS metric compliance would decrease readability and maintainability. */
Xcp_ErrorCode Xcp_DaqListFinalize(uint16 daqListNo_u16, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Xcp_ErrorCode Error;
#if (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_ON)
  Xcp_AddrType_t  LocalAddress;
  uint8           LocalSize;
  uint16_least  OdtNo;
  uint16_least  AbsOdtEntryNo;
#endif
#if (XCP_DAQ_PACKED_MODE_SUPPORTED == STD_ON)
  Xcp_DaqList_t* DaqListPtr;
  DaqListPtr = &(XCP_DAQ(daqListNo_u16, protLayerId));
#endif
  /* Check if DaqList already checked */
  if ((XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) & XCP_DAQFLAG_CONFIG_COMPLETED) != 0u)
  {
    /* DaqList already checked, all other checks can be skipped */
    Error = XCP_NO_ERROR;
  }
  /* Check if SetDaqListMode has been received */
  else if ((XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) & XCP_DAQFLAG_RECEIVED_DAQ_LIST_MODE) == 0u)
  {
    Error = XCP_ERR_SEQUENCE;
  }

#if (XCP_DAQ_PACKED_MODE_SUPPORTED == STD_ON)

  else if((DaqListPtr->DaqPackedMode_u8 == XCP_DPM_EVENT_GROUPED_DATA_PACKING) && ((DaqListPtr->Mode_u8 & XCP_DAQLISTMODE_DIRECTION_STIM) == XCP_DAQLISTMODE_DIRECTION_STIM))
  {
    Error = XCP_ERR_DAQ_CONFIG;
  }
  /* Timestamp must be enabled if daq packed mode is configured */
  else if((DaqListPtr->DpmTimestampMode_u8 != 0u) && ((DaqListPtr->Mode_u8 & XCP_DAQLISTMODE_TIMESTAMP) == 0u))
  {
    Error = XCP_ERR_DAQ_CONFIG;
  }
#endif

#if (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_ON)
  /* Check if Address-Transformation was already done */
  else if ((XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) & XCP_DAQFLAG_ADDR_TRANSFORMATION_DONE) == 0u)
  { /* No, transformation was not yet done */
    Error = XCP_NO_ERROR;
    /* Loop through all the ODTs of this DaqList */
    for (OdtNo = XCP_DAQ_ODTFIRST(daqListNo_u16, protLayerId); (OdtNo < XCP_DAQ_ODTMAX(daqListNo_u16, protLayerId)) && (Error == XCP_NO_ERROR); OdtNo++)
    {
      /* Loop through all OdtEntries of this ODT */
      for (AbsOdtEntryNo = XCP_ODT_ODTENTRYFIRST(OdtNo, protLayerId); (AbsOdtEntryNo < XCP_ODT_ENTRYMAX(OdtNo, protLayerId)) && (Error == XCP_NO_ERROR); AbsOdtEntryNo++)
      {
        /* Get address and Size */
        LocalAddress.AddrValue  = XCP_ODTENTRY_ADDRESS(AbsOdtEntryNo, protLayerId);
#if (XCP_ADDRESS_EXTENSION == STD_ON)
        LocalAddress.Extension  = XCP_ODTENTRY_EXTENSION(AbsOdtEntryNo, protLayerId);
#else
        LocalAddress.Extension  = 0u;
#endif
        LocalSize               = XCP_ODTENTRY_SIZE(AbsOdtEntryNo, protLayerId);

        Error = Xcp_DaqTransformAndCheckOdtEntry((Xcp_AddrType_t*)&LocalAddress, LocalSize, daqListNo_u16, protLayerId);

        XCP_ODTENTRY_ADDRESS(AbsOdtEntryNo, protLayerId)    = LocalAddress.AddrValue;
#if (XCP_ADDRESS_EXTENSION == STD_ON)
        XCP_ODTENTRY_EXTENSION(AbsOdtEntryNo, protLayerId)  = LocalAddress.Extension;
#endif
      }
    }
  }
  else
  {
    /* Nothing more to do */
    Error = XCP_NO_ERROR;
  }

  if (Error != XCP_NO_ERROR)
  {
    /* Nothing to do - error already set */
  }
#endif
#if (XCP_MEMORY_READ_PROTECTION == STD_ON)
  /* Check if read protected */
  else if( ((XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) & XCP_DAQFLAG_READ_PROTECTED) != 0u)
        && ((XCP_DAQ_MODE(daqListNo_u16, protLayerId) & XCP_DAQLISTMODE_DIRECTION_STIM) == 0u)
         )
  {
    Error = XCP_ERR_ACCESS_DENIED;
  }
#endif
#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  /* Check if write protected */
  else if( ((XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) & XCP_DAQFLAG_WRITE_PROTECTED) != 0u)
        && ((XCP_DAQ_MODE(daqListNo_u16, protLayerId) & XCP_DAQLISTMODE_DIRECTION_STIM) != 0u)
         )
  {
    Error = XCP_ERR_ACCESS_DENIED;
  }
#endif /*(XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)*/
  /* Check for correct OdtLength and wrong OdtEntries */
  else if (Xcp_DaqSetOdtLength(daqListNo_u16, protLayerId) == E_NOT_OK)
  {
    Error = XCP_ERR_DAQ_CONFIG;
  }
  /* All checks passed, DaqList OK */
  else
  {
    /* If all the checks passed the DaqList is completed. */
    XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) = XCP_DAQ_FLAGS(daqListNo_u16, protLayerId) | XCP_DAQFLAG_CONFIG_COMPLETED;
    Error = XCP_NO_ERROR;
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return Error;
}


# if (XCP_OM_ODT_OPTIMIZATION_ENABLED == STD_ON)
/**
****************************************************************************************************
This function checks whether the copy routine for this ODT is also valid for this OdtEntry.
The copy optimization is set if it is the first OdtEntry of the Odt or an error was detected.
\param[in]  XcpAddr       OdtEntry address
\param[in]  Size          OdtEntry size
\param[in]  protLayerId   Protocol Layer Id
\param[in]  AbsOdtNum     Number of the Odt
\return     returns an error code in case that the conditions are not fulfilled
***************************************************************************************************/
Xcp_ErrorCode Xcp_DaqSetOdtCopyRoutine(const Xcp_AddrType_t XcpAddr, uint8 Size, uint16 AbsOdtNum, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Xcp_ErrorCode Error;

  uint8 CopyRoutine_Size;          /* Holds the copy routine determined analyzing the size */
  uint8 CopyRoutine_Address;       /* Holds the copy routine determined analyzing the address */
  uint8 CopyRoutine_MaxSupported;  /* Holds the copy routine given by the configuration */
  uint8 CopyRoutine;               /* Holds the copy routine to be used for the given OdtEntry */

  /* Initialize variables */
  Error = XCP_NO_ERROR;

  /* Check if the copy-routine for this Odt is Safe-copy: Nothing more has to be done */
  if (XCP_ODT_COPYROUTINE(AbsOdtNum, protLayerId) == XCP_ODT_COPY_SAFE)
  {
    /* Safe copy to be used - nothing to do */
  }
  else
  {
    /* Determine the copy routine to be used according to configuration */
    CopyRoutine_MaxSupported = Map_OdtOptimizationType_to_CopyRoutine[Xcp_PlCfgConst.TlCfg[XCP_ACTIVE_TL_ID(protLayerId)].OdtOptimizationType_en];

    /* Check if Optimization for this TL is active */
    if (CopyRoutine_MaxSupported == XCP_ODT_COPY_SAFE)
    { /* XCP_ODT_COPY_SAFE = no optimization -> Use the safe-copy routine */
      XCP_ODT_COPYROUTINE(AbsOdtNum, protLayerId) = XCP_ODT_COPY_SAFE;
    }
    else
    { /* Optimization requested */

      /* Check which copy routine can be used according to the size*/
      CopyRoutine_Size = Xcp_DaqCalcCopyRoutine((uint64)Size);

      /* Check which copy routine can be used according to the alignment*/
      /* No check for AddrExtension as this is not supported together with Optimization as of now */
      CopyRoutine_Address = Xcp_DaqCalcCopyRoutine((uint64)XcpAddr.AddrValue);

      /* Get the smaller copy routine - as it can also copy larger sizes */
      CopyRoutine = XCP_MIN(CopyRoutine_Size, CopyRoutine_Address);

      /* Limit to the configured max copy routine */
      CopyRoutine = XCP_MIN(CopyRoutine, CopyRoutine_MaxSupported);

      /* Check if the determined copy routine is the same for the Odt */
      if (CopyRoutine != XCP_ODT_COPYROUTINE(AbsOdtNum, protLayerId))
      { /* Copy routine mismatch */

        /* Check if there is a copy routine stored for this Odt.
         * This is done within the check whether the routines are equal,
         * as otherwise this check has to be done for each OdtEntry. */
        if (XCP_ODT_COPYROUTINE(AbsOdtNum, protLayerId) == XCP_ODT_COPY_UNKNOWN)
        { /* No copy routine -> use the copy routine for this OdtEntry as the one for the Odt */
          XCP_ODT_COPYROUTINE(AbsOdtNum, protLayerId) = CopyRoutine;
        }
        else
        { /* Copy routine for Odt was stored and does not match the one for this OdtEntry */

          /* Fall back to safe copy */
          XCP_ODT_COPYROUTINE(AbsOdtNum, protLayerId) = XCP_ODT_COPY_SAFE;

#  if (XCP_OM_ODT_OPTIMIZATION_ERR_REACTION == XCP_OM_ODT_OPTIMIZATION_ERR_FALLBACK_TO_OM_DEFAULT)
          /* TODO HWUNDERLICH: Output SERV-Text: Fallback to OM_DEFAULT -> Higher Runtime */
#  else
          /* Return error */
          Error = XCP_ERR_DAQ_CONFIG;

          /* TODO HWUNDERLICH: Output SERV-Text: DAQ configuration aborted due to errors with OM_OPTIMIZATION */
#  endif
        }
      }
      else
      {
        /* Copy routines identical - nothing to do */
      }
    }
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return Error;
}
# endif


/***************************************************************************************************
* Local functions
***************************************************************************************************/


# if (XCP_OM_ODT_OPTIMIZATION_ENABLED == STD_ON)
/**
****************************************************************************************************
This function returns the largest copy function that can be used for the given parameter
\param[in]  value   value for which the copy routine shall be returned
\return     returns the largest possible copy function for the given parameter
***************************************************************************************************/
static uint8 Xcp_DaqCalcCopyRoutine(uint64 value)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  uint8 CopyRoutine;

  if ((value % 8u) == 0u)
  { /* 64 bit */
    CopyRoutine = XCP_ODT_COPY_64;
  }
  else
  if ((value % 4u) == 0u)
  { /* 32 bit */
    CopyRoutine = XCP_ODT_COPY_32;
  }
  else
  if ((value % 2u) == 0u)
  { /* 16 bit */
    CopyRoutine = XCP_ODT_COPY_16;
  }
  else
  /* if ((value % 1u) == 0u) */
  { /* 8 bit */
    CopyRoutine = XCP_ODT_COPY_8;
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return CopyRoutine;
}
# endif


/**
****************************************************************************************************
This function stops the given DAQ-List
\param[in]  daqListNo       requested DaqList number
\param[in]  protLayerId     Protocol Layer Id
\return     ErrorCode
\retval     XCP_NO_ERROR:       no problem occurred
\retval     XCP_REPEAT_COMMAND: stopped DAQ which is currently processed (e.g. sampling), cannot stop immediately
***************************************************************************************************/
static Xcp_ErrorCode Xcp_DaqListStopDaqList(uint16 daqListNo, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()
  Xcp_ErrorCode Error;
#if (XCP_BYPASS_CONSISTENCY == STD_ON)
  uint16 EventChannelNum;
#endif

#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  /* enter lock to change buffers - no lock needed as this function is only called under lock */
  /* Reset running flag */
  XCP_DAQ_BUFFERINFO(daqListNo, protLayerId).Status_u8 = XCP_STATUSBIT_STIM_ERR_NO_DATA_AVAILABLE;
  /* exit lock around buffer access */
#endif

  /* Remove DaqList running flag */
  XCP_DAQ_MODE(daqListNo, protLayerId) &= (uint8)(~XCP_DAQLISTMODE_RUNNING);

  if (XCP_DAQ(daqListNo, protLayerId).CurrentlyRunning_b == FALSE)
  { /* DAQ is not running in Eventchannel at the moment, can be stopped immediatly */
    /* Remove selected flag */
    XCP_DAQ_MODE(daqListNo, protLayerId) &= (uint8)(~XCP_DAQLISTMODE_SELECTED);

#if (XCP_DAQ_PACKED_MODE_SUPPORTED == STD_ON)
    /*drop the incomplete data and reset the sample time*/
    if(XCP_DAQ(daqListNo, protLayerId).DaqPackedMode_u8 == XCP_DPM_EVENT_GROUPED_DATA_PACKING)
    {
      XCP_DAQ(daqListNo, protLayerId).DpmCurrentSampledTime_u16 = 0u;
      Xcp_InitDaqQueue(daqListNo,protLayerId);
    }
#endif

#if (XCP_BYPASS_CONSISTENCY == STD_ON)
    EventChannelNum = XCP_DAQ(daqListNo, protLayerId).EventChannelNum_u16;
    /* reset the tolerance counter for the event channel linked to this daqlist */
    XCP_BYPASS(EventChannelNum).InconsistencyTolerance_u8 = 0u;
    /* reset the consistency check flag */
    XCP_BYPASS(EventChannelNum).ConsistencyCheckFlag_u8 = TRUE;
#endif

    Error = XCP_NO_ERROR;
  }
  else
  { /* DaqList is currently running, so we have to repeat the stop later */
    Error = XCP_REPEAT_COMMAND;
  }

  /* mark DAQ/STIM configuration to be stopped */
#if (XCP_SYNCHRONOUS_DATA_STIMULATION_STIM == STD_ON)
  if ((XCP_DAQ_MODE(daqListNo, protLayerId) & XCP_DAQLISTMODE_DIRECTION_STIM) != 0u)
  { /* DAQ-List with direction STIM */
    Xcp_GlobalNoInit.StimTransmissionStopped_b = TRUE;
  }
  else
#endif
  { /* DAQ-List with direction DAQ */
    Xcp_GlobalNoInit.DaqTransmissionStopped_b = TRUE;
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()

  return Error;
}


/**
****************************************************************************************************
This function set Odt length for given DaqList
\param[in]  daqListNo_u16   DaqList number
\param[in]  protLayerId     Protocol Layer Id
\return     E_OK: set was successful
***************************************************************************************************/
static Std_ReturnType Xcp_DaqSetOdtLength(uint16 daqListNo_u16, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Local variables */
  Std_ReturnType RetValue;
  uint32 OdtLength;
  uint16 OdtFirst;
  uint16 OdtMax;
  uint16_least OdtNo;
  uint16_least AbsOdtEntryNo;
#if (XCP_DAQ_PACKED_MODE_SUPPORTED == STD_ON)
  uint32 OdtGroupLength;
#endif
  /* Initial values */
  RetValue = E_OK;
  OdtFirst = XCP_DAQ_ODTFIRST(daqListNo_u16, protLayerId);
  OdtMax = XCP_DAQ_ODTMAX(daqListNo_u16, protLayerId);

  /* Loop through all the ODTs of this DaqList */
  for (OdtNo = OdtFirst; OdtNo < OdtMax; OdtNo++)
  {
    /* header size */
#if (XCP_IDFIELD_OTHER_THAN_ABSOLUTE == STD_ON)
    OdtLength = (uint8) (Xcp_NoInit[protLayerId].Session.IdFieldType_en);  /* DAQ header size */
#else
    OdtLength = (uint8) (XCP_IDENTIFICATION_FIELD_TYPE_ABSOLUTE);  /* DAQ header size absolute = 1 Byte */
#endif

#if (XCP_BYPASS_CONSISTENCY == STD_ON)
    /* check if the DTO_CTR bit is set for this daqlist */
    if ((XCP_DAQ_MODE(daqListNo_u16, protLayerId) & XCP_DAQLISTMODE_DTO_CTR) == XCP_DAQLISTMODE_DTO_CTR)
    {
      /*check if first ODT and IdFieldType is any type other than relative word aligned */
      if ((OdtNo == OdtFirst) && (Xcp_NoInit[protLayerId].Session.IdFieldType_en != XCP_IDENTIFICATION_FIELD_TYPE_RELATIVE_WORD_ALIGNED))
      {
        /* when bypass is on, increment the length due to the event counter in the first ODT in the daqlist */
        OdtLength = OdtLength + 1u;
      }
    }
#endif


#if (XCP_TIMESTAMP == STD_ON)
    /* If ODT with timestamp, Add number of bytes required for timestamp */
    if ( (OdtNo == OdtFirst)
      && ((XCP_DAQ_MODE(daqListNo_u16, protLayerId) & XCP_DAQLISTMODE_TIMESTAMP) != 0u)
       )
    {
      /* bytes for timestamp according TL config*/
      OdtLength = OdtLength + (uint8) (Xcp_PlCfgConst.TlCfg[XCP_ACTIVE_TL_ID(protLayerId)].TimestampType_en);
    }
#endif

#if (XCP_DAQ_PACKED_MODE_SUPPORTED == STD_ON)
    /*store temporarily the identification field length plus the timestamp length*/
    OdtGroupLength = OdtLength;
#endif

    /* Loop through all OdtEntries of this ODT */
    for (AbsOdtEntryNo = XCP_ODT_ODTENTRYFIRST(OdtNo, protLayerId); AbsOdtEntryNo < XCP_ODT_ENTRYMAX(OdtNo, protLayerId); AbsOdtEntryNo++)
    {
      if ((   (XCP_ODTENTRY_ADDRESS(AbsOdtEntryNo, protLayerId)   == 0u)
#if (XCP_ADDRESS_EXTENSION == STD_ON)
           /* If address-extension is enabled only for Extension 0 the Address 0 is invalid per default
            * - everything else should be done by MemProtection-Check */
           && (XCP_ODTENTRY_EXTENSION(AbsOdtEntryNo, protLayerId) == 0u)
#endif
          )
          || (XCP_ODTENTRY_SIZE(AbsOdtEntryNo, protLayerId) == 0)
         )
      {
        /* DaqList is not correctly configured. */
        RetValue = E_NOT_OK;
        OdtLength = 0xFFFFFFFFu;
        break;
      }
      else
      {
        OdtLength += XCP_ODTENTRY_SIZE(AbsOdtEntryNo, protLayerId);
      }
    }

#if (XCP_DAQ_PACKED_MODE_SUPPORTED == STD_ON)
    /* Only the event-grouped data packing is supported currently */
    if( XCP_DAQ(daqListNo_u16, protLayerId).DaqPackedMode_u8 == XCP_DPM_EVENT_GROUPED_DATA_PACKING)
    {
      OdtGroupLength = OdtLength - OdtGroupLength;
      OdtLength = (OdtLength + (OdtGroupLength * (XCP_DAQ(daqListNo_u16, protLayerId).DpmSampleCount_u16 - 1u)));
    }
    else
    {
      /* Nothing to do */
    }
#endif

    /* Check if OdtLength <= MaxDto */
    if (OdtLength <= XCP_MAXDTO(protLayerId))
    {
      /* store OdtLength */
      XCP_ODT_LENGTH(OdtNo, protLayerId) = (uint16) OdtLength;
    }
    else
    {
      /* OdtLength too big */
      RetValue = E_NOT_OK;
      break;
    }
  } /* end of loop through ODTs */

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
  return RetValue;
}


#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#endif


