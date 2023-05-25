

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
#if (XCP_CMD_GET_DAQ_EVENT_INFO_AVAILABLE == STD_ON)

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
This service handles to get the DAQ Event Channel Info.
\param[in]  XcpPacket     Pointer to the received command packet
\param[in]  protLayerId   Protocol Layer Id
\return     -
***************************************************************************************************/
void Xcp_CmdGetDaqEventInfo(const PduInfoType* XcpPacket, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Command variables */
  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: This function is called with data (a PDU) from the CanIf/FrIf/EthIf which ensure 4-Byte-Alignment.
                                    The cast allows easier (= safer) access to the data in the PDU.
                                    Casting over void* is necessary as otherwise some compilers complain */
  const Xcp_CmdGetDaqEventInfo_t* const CmdPtr = (const Xcp_CmdGetDaqEventInfo_t*) (void*) XcpPacket->SduDataPtr;
  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: Compiler ensures the correct alignment in the datastructure, thus casting is no problem.
                                    The cast allows easier (= safer) access to the data in the PDU.
                                    Casting over void* is necessary as otherwise some compilers complain */
  Xcp_ResGetDaqEventInfo_t* ResPtr = (Xcp_ResGetDaqEventInfo_t*) (void*) XCP_RES_BUFFER_PTR(protLayerId);

  /* Local variables */
  Xcp_ErrorCode Error;
  const char* GetEventChannelName;

  /* Initialize variables */
  Error = XCP_NO_ERROR;

  /* Specified event channel number is available? */
  if(CmdPtr->EventChannelNum_u16 >= XCP_MAX_EVENT_CHANNEL)
  {
    Error = XCP_ERR_OUT_OF_RANGE;
  }

  if (Error == XCP_NO_ERROR)
  {
    /* Set response length */
    XCP_RES_BUFFER_LENGTH(protLayerId) = XCP_RES_GET_DAQ_EVENT_INFO_LENGTH;

    /* Prepare response to send */
    ResPtr->PacketId_u8                = XCP_PACKET_ID_RES;

    /* Get consistency-information (stored per PL) & Event channel direction */

# if ((XCP_ASAM_MAJOR_VERSION <= 1u) && (XCP_ASAM_MINOR_VERSION <= 3u))
    /* Daq list properties updated without the packed flag as this feature is only available above XCP version 1.3*/
    ResPtr->DaqEventProperties_u8 = (uint8)((uint32)Xcp_PlCfgConst.TlCfg[XCP_ACTIVE_TL_ID(protLayerId)].Consistency_en << 6) | Xcp_PlCfgConst.EventChannelCfg[CmdPtr->EventChannelNum_u16].EventChannelDirection_u8;
#else
    /* Daq list properties updated with the packed flag as this feature is only available above XCP version 1.3*/
    /* Packed flag is TRUE. The Bit 4 of DAQ_LIST_PROPERTIES needs to be set to 1 */
    if(XCP_PLCFG_EV_PACKEDFLAG(CmdPtr->EventChannelNum_u16) == 1u)
    {
      ResPtr->DaqEventProperties_u8 = (uint8)((uint32)Xcp_PlCfgConst.TlCfg[XCP_ACTIVE_TL_ID(protLayerId)].Consistency_en << 6) | Xcp_PlCfgConst.EventChannelCfg[CmdPtr->EventChannelNum_u16].EventChannelDirection_u8 | XCP_DAQLIST_PROP_PACKEDFLAG;
    }
    else
    {
      /* Packed flag is FALSE. The Bit 4 of DAQ_LIST_PROPERTIES needs to be set to 0 */
      ResPtr->DaqEventProperties_u8 = (uint8)((uint32)Xcp_PlCfgConst.TlCfg[XCP_ACTIVE_TL_ID(protLayerId)].Consistency_en << 6) | Xcp_PlCfgConst.EventChannelCfg[CmdPtr->EventChannelNum_u16].EventChannelDirection_u8 ;
    }
#endif
    ResPtr->MaxDaqList_u8               = 0x01u;
    ResPtr->EventChannelNameLength_u8   = Xcp_PlCfgConst.EventChannelCfg[CmdPtr->EventChannelNum_u16].EventChannelNameLength_u8;
    ResPtr->EventChannelTimeCycle_u8    = Xcp_PlCfgConst.EventChannelCfg[CmdPtr->EventChannelNum_u16].EventChannelTimeCycle_u8;
    ResPtr->EventChannelTimeUnit_u8     = Xcp_PlCfgConst.EventChannelCfg[CmdPtr->EventChannelNum_u16].EventChannelTimeUnit_u8;
    ResPtr->EventChannelPriority_u8     = Xcp_PlCfgConst.EventChannelCfg[CmdPtr->EventChannelNum_u16].EventChannelPriority_u8;

    /* get the address pointer for Event channel name requested by master */
    GetEventChannelName = Xcp_PlCfgConst.EventChannelCfg[CmdPtr->EventChannelNum_u16].EventChannelName;

    /* Set the MTA with the address of identification type */
    /* MR12 RULE 11.4 VIOLATION: Cast from pointer to XCP_ADDR_TYPE is OK as this type is able to store the address (see type definition) */
    XCP_MTA(protLayerId).AddrValue = (Xcp_AddrValue)GetEventChannelName;
    XCP_MTA(protLayerId).Extension = 0u;

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

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#else
  /* HIS METRIC COMF VIOLATION IN Xcp_CmdGetDaqEventInfo.c: The command is not enabled - no code is needed in this file */
#endif



