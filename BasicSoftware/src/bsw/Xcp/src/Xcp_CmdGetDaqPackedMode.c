
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
void Xcp_CmdGetDaqPackedMode( const PduInfoType* XcpPacket, uint8 protLayerId )
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()


  /* Command variables */
  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: Compiler ensures the correct alignment in the datastructure, thus casting is no problem.
                                            The cast allows easier (= safer) access to the data in the PDU.
                                            Casting over void* is necessary as otherwise some compilers complain */
  Xcp_SubResGetDaqPackedMode_t* ResPtr = (Xcp_SubResGetDaqPackedMode_t*) (void*) XCP_RES_BUFFER_PTR(protLayerId);

  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: This function is called with data (a PDU) from the CanIf/FrIf/EthIf which ensure 4-Byte-Alignment.
                                    The cast allows easier (= safer) access to the data in the PDU.
                                    Casting over void* is necessary as otherwise some compilers complain */
  const Xcp_SubCmdGetDaqPackedMode_t* const CmdPtr = (const Xcp_SubCmdGetDaqPackedMode_t*) (void*) XcpPacket->SduDataPtr;

  /* Local variables */
  Xcp_ErrorCode Error;

  /* initialization */
  Error = XCP_NO_ERROR;

  /* Check if the command parameter DaqList number is valid */
  if (CmdPtr->DaqListNum_u16 >= Xcp_NoInit[protLayerId].DaqConfig.DaqListCnt_u16)
  {
    Error = XCP_ERR_OUT_OF_RANGE;
  }
  else
  {
    /*nothing to do */
  }

  if (Error == XCP_NO_ERROR)
  {
    /* prepare positive response */
    /* set response length  */
    if ( XCP_DAQ(CmdPtr->DaqListNum_u16, protLayerId).DaqPackedMode_u8 == 0u )
    {
      XCP_RES_BUFFER_LENGTH(protLayerId) = XCP_SUBRES_GET_DAQ_PACKED_MODE_LENGTH_MODE0;
    }
    else
    {
      XCP_RES_BUFFER_LENGTH(protLayerId) = XCP_SUBRES_GET_DAQ_PACKED_MODE_LENGTH_MODE2;
      /*Daq packed mode is enabled. Return the structure variables */
      /* Set the DPM_TIMESTAMP_MODE variable */
      ResPtr->DpmTimestampMode_u8 = Xcp_NoInit[protLayerId].DaqConfig.DaqList_p[CmdPtr->DaqListNum_u16].DpmTimestampMode_u8;

      /* Set the DPM_SAMPLE_COUNT variable */
      ResPtr->DpmSampleCount_u16 = Xcp_NoInit[protLayerId].DaqConfig.DaqList_p[CmdPtr->DaqListNum_u16].DpmSampleCount_u16;
    }

    /* Set Pid */
    ResPtr->PacketId_u8 = XCP_PACKET_ID_RES;

    /* Set the Daq Packed Mode status*/
    ResPtr->DaqPackedMode_u8 = Xcp_NoInit[protLayerId].DaqConfig.DaqList_p[CmdPtr->DaqListNum_u16].DaqPackedMode_u8;

    /* Send positive response */
    Xcp_SendRes(protLayerId);
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
  /* HIS METRIC COMF VIOLATION IN Xcp_CmdGetDaqPackedMode.c: The command is not enabled - no code is needed in this file */
#endif


