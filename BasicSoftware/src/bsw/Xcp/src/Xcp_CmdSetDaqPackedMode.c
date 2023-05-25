
/***************************************************************************************************
* Includes
***************************************************************************************************/
#include "Xcp.h"
#include "Xcp_Priv.h"


/***************************************************************************************************
* Variables
***************************************************************************************************/
# if (XCP_DAQ_PACKED_MODE_SUPPORTED == STD_ON)

/***************************************************************************************************
* Functions
***************************************************************************************************/

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
Function updates the protocol and transport layer version information that is currently in use
\param[in]  protLayerId   Protocol Layer Id
\return     -
***************************************************************************************************/
void Xcp_CmdSetDaqPackedMode( const PduInfoType* XcpPacket, uint8 protLayerId )
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Command variables */
  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: This function is called with data (a PDU) from the CanIf/FrIf/EthIf which ensure 4-Byte-Alignment.
                                    The cast allows easier (= safer) access to the data in the PDU.
                                    Casting over void* is necessary as otherwise some compilers complain */
  const Xcp_CmdSetDaqPackedMode_t* const CmdPtr = (const Xcp_CmdSetDaqPackedMode_t*) (void*) XcpPacket->SduDataPtr;

  /* Local variables */
  Xcp_ErrorCode Error;

  /* prepare positive response */
  /* set length response */
  XCP_RES_BUFFER_LENGTH(protLayerId) = XCP_SUBRES_SET_DAQ_PACKED_MODE_LENGTH;

  Error = XCP_NO_ERROR;

  /* Check if DAQ list is running. */
  if (Xcp_NoInit[protLayerId].DaqConfig.DaqState_en == XCP_DAQ_STATE_RUNNING)
  {
    Error = XCP_ERR_DAQ_ACTIVE;
  }
  /* Check if the command parameter DaqList number is valid */
  else if (CmdPtr->DaqListNum_u16 >= Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16)
  {
    Error = XCP_ERR_OUT_OF_RANGE;
  }
  /* Check for not supported Modes */
  else if ((CmdPtr->DaqPackedMode_u8 != 0u) && (CmdPtr->DaqPackedMode_u8 != 2u) )
  {
    Error = XCP_ERR_MODE_NOT_VALID;
  }
  /*Check if DTO size is too short */
  else if (XCP_MAXDTO(protLayerId) <= 8u)
  {
    Error = XCP_ERR_MODE_NOT_VALID;
  }
  /*Check if Sample Count is 0 or 1*/
  else if ((CmdPtr->DpmSampleCount_u16 == 0) || (CmdPtr->DpmSampleCount_u16 == 1))
  {
      Error = XCP_ERR_MODE_NOT_VALID;
  }
  /* Check if Timestamp is enabled */
  else if (Xcp_PlCfgConst.TlCfg[XCP_ACTIVE_TL_ID(protLayerId)].TimestampType_en == XCP_TIMESTAMP_TYPE_NO_TIME_STAMP)
  {
    Error = XCP_ERR_DAQ_CONFIG;
  }
  else
  {
    /* nothing to do */
  }

  if (Error == XCP_NO_ERROR)
  {
    /* Set the DAQ_PACKED_MODE variable in the global variable */
    Xcp_NoInit[protLayerId].DaqConfig.DaqList_p[CmdPtr->DaqListNum_u16].DaqPackedMode_u8 = CmdPtr->DaqPackedMode_u8;

    /* Set the DPM_TIMESTAMP_MODE variable in the global variable */
    Xcp_NoInit[protLayerId].DaqConfig.DaqList_p[CmdPtr->DaqListNum_u16].DpmTimestampMode_u8 = CmdPtr->DpmTimestampMode_u8;

    /* Set the DPM_SAMPLE_COUNT variable in the global variable */
    Xcp_NoInit[protLayerId].DaqConfig.DaqList_p[CmdPtr->DaqListNum_u16].DpmSampleCount_u16 = CmdPtr->DpmSampleCount_u16;
    /* Send positive response */
    Xcp_SendPosRes(protLayerId);
  }
  else
  {
    /* Send negative response */
    Xcp_SendErrRes(Error, protLayerId);
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
}


/* ------------------------------------------------------------------------ */
/* End section for code */

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#else
  /* HIS METRIC COMF VIOLATION IN Xcp_CmdSetDaqPackedMode.c: The command is not enabled - no code is needed in this file */
#endif


