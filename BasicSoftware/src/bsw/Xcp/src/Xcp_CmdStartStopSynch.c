

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
#if (XCP_CMD_START_STOP_SYNCH_AVAILABLE == STD_ON)

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
This service handles a start, stop for selected DaqList or stop all running lists.
\param[in]  XcpPacket     Pointer to the received command packet
\param[in]  protLayerId   Protocol Layer Id
\return     -
***************************************************************************************************/
/* HIS METRIC LEVEL VIOLATION IN Xcp_CmdStartStopSynch: Function contains very simple "else if" statements. HIS metric compliance would decrease readability and maintainability. */
void Xcp_CmdStartStopSynch(const PduInfoType* XcpPacket, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Command variables */
  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: This function is called with data (a PDU) from the CanIf/FrIf/EthIf which ensure 4-Byte-Alignment.
                                    The cast allows easier (= safer) access to the data in the PDU.
                                    Casting over void* is necessary as otherwise some compilers complain */
  const Xcp_CmdStartStopSynch_t* const CmdPtr = (const Xcp_CmdStartStopSynch_t*) (void*) XcpPacket->SduDataPtr;

  /* Local variables */
  Xcp_ErrorCode Error;

  /* initialization */
  Error = Xcp_DaqTriggerStateStartStop(XcpPacket, protLayerId);

  /* check if mode parameter is valid otherwise send error response ERR_MODE_NOT_VALID */
  /* Do this check also Xcp_DaqTriggerStateStartStop() has an error, because ERR_SEQUENCE is not specified in ASAM. */
#if (XCP_CMD_START_STOP_SYNCH_PREPARE_AVAILABLE == STD_ON)
  if (CmdPtr->Mode_u8 > XCP_DAQLIST_SYNC_PREPSTARTSEL)
#else
  if (CmdPtr->Mode_u8 > XCP_DAQLIST_SYNC_STOPSEL)
#endif
  {
    /* Prepare error response XCP_ERR_MODE_NOT_VALID */
    Error = XCP_ERR_MODE_NOT_VALID;
  }
  else if (Error == XCP_NO_ERROR)
  {
    /*--------------------------------------------------------------------------------------------------
      if mode == stop all
    --------------------------------------------------------------------------------------------------*/
    if (CmdPtr->Mode_u8 == XCP_DAQLIST_SYNC_STOPALL)
    {
      Error = Xcp_DaqListStopAll(protLayerId);
    }
    /*--------------------------------------------------------------------------------------------------
      if mode == start selected
    --------------------------------------------------------------------------------------------------*/
    else if (CmdPtr->Mode_u8 == XCP_DAQLIST_SYNC_STARTSEL)
    {
      Xcp_DaqListStartSelected(protLayerId);
    }
#if (XCP_CMD_START_STOP_SYNCH_PREPARE_AVAILABLE == STD_ON)
    /*--------------------------------------------------------------------------------------------------
      if mode == prepare for start selected
    --------------------------------------------------------------------------------------------------*/
    else if (CmdPtr->Mode_u8 == XCP_DAQLIST_SYNC_PREPSTARTSEL)
    {
      /* The preparation is done in Xcp_DaqTriggerStateStartStop
       * The current DAQ State shall be XCP_DAQ_STATE_READY_TO_RUN if no Error occurs
       * Return sequence error if daq is already running */
      if(Xcp_NoInit[protLayerId].DaqConfig.DaqState_en == XCP_DAQ_STATE_RUNNING)
      {
        Error = XCP_ERR_SEQUENCE;
      }
    }
#endif
    /*--------------------------------------------------------------------------------------------------
      if mode == stop selected
    --------------------------------------------------------------------------------------------------*/
    else
    {
      Error = Xcp_DaqListStopSelected(protLayerId);
    }
  }
  else
  {
    /* error occurred or no response requested */
  }

  if (Error == XCP_NO_ERROR)
  {
    /* Send positive response */
    Xcp_SendPosRes(protLayerId);
  }
  else if (Error == XCP_NO_RESPONSE)
  {
    /* do not send anything */
  }
  else if (Error == XCP_REPEAT_COMMAND)
  {
    /* do not send anything, store command for later processing */
    Xcp_StoreCommand(XcpPacket, protLayerId);
  }
  else if (Error == XCP_ERR_ACCESS_DENIED)
  {
    /* Function Xcp_DaqTriggerStateStartStop() can set ACCESS_DENIED. Send ERR_DAQ_CONFIG instead as allowed error in ASAM. */
    Xcp_SendErrRes(XCP_ERR_DAQ_CONFIG, protLayerId);
  }
  else
  {
    /* Send negative response */
    Xcp_SendErrRes(Error, protLayerId);
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
}

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#else
  /* HIS METRIC COMF VIOLATION IN Xcp_CmdStartStopSynch.c: The command is not enabled - no code is needed in this file */
#endif



