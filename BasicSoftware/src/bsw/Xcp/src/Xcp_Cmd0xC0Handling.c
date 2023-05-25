

/***************************************************************************************************
* Includes
***************************************************************************************************/
#include "Xcp.h"
#include "Xcp_Priv.h"


/***************************************************************************************************
* Variables
***************************************************************************************************/

#if (XCP_CMD_0XC0_AVAILABLE == STD_ON)

/***************************************************************************************************
* Functions
***************************************************************************************************/

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
This command handles the 0xC0 command with the sub-commands. Switch case used because only 4 sub-commands present
\param[in]  XcpPacket     Pointer to the received command packet
\param[in]  protLayerId   Protocol Layer Id
\return     -
***************************************************************************************************/
void Xcp_Cmd0xC0Handling(const PduInfoType* XcpPacket, uint8 protLayerId)
{
  uint8 Pid_SecondByte_u8;
  Pid_SecondByte_u8 = XcpPacket->SduDataPtr[1];

  /* Switch case for the sub-commands. First byte is 0xC0 we have checked in Xcp_ReceiveCommand */
  switch(Pid_SecondByte_u8)
  {
    case XCP_SUBCMD_GET_VERSION_ID :
	{
	    /* Check the packet length */
	  if (XcpPacket->SduLength < XCP_SUBCMD_GET_VERSION_LENGTH)
	  {
		/* Send Command Syntax error */
		Xcp_SendErrRes(XCP_ERR_CMD_SYNTAX, protLayerId);
	  }
	  else /* all checks have been passed */
	  {
	    /* Call the function for GET_VERSION INFO  */
	    Xcp_SubCmdGetVersion(XcpPacket, protLayerId);
	  }
	  break;
	}

	case XCP_SUBCMD_SET_DAQ_PACKED_MODE_ID :
	{
	  /* Check the packet length */
	  if (XcpPacket->SduLength < XCP_CMD_0XC0_MIN_LENGTH)
	  {
		/* Send Command Syntax error */
		Xcp_SendErrRes(XCP_ERR_CMD_SYNTAX, protLayerId);
	  }
#if(XCP_SEED_AND_KEY == STD_ON)
      else if (( XCP_RESOURCE_DAQ & Xcp_NoInit[protLayerId].Session.ResourceProtStatus_u8) != 0u)
      {
        /* Command is locked */
        /* Send Error packet code 0x25 */
        Xcp_SendErrRes(XCP_ERR_ACCESS_LOCKED, protLayerId);
      }
#endif
	  else /* all checks have been passed */
	  {
#if (XCP_DAQ_PACKED_MODE_SUPPORTED == STD_ON)
	    /* Call the function for SET_DAQ_PACKED_MODE  */
	    Xcp_CmdSetDaqPackedMode(XcpPacket, protLayerId);
#else
        /* Command not supported so we return an error */
        Xcp_SendErrRes(XCP_ERR_SUBCMD_UNKNOWN, protLayerId);
#endif
	  }
	  break;
	}

	case XCP_SUBCMD_GET_DAQ_PACKED_MODE_ID :
	{
	  /* Check the packet length */
	  if (XcpPacket->SduLength < XCP_CMD_0XC0_MIN_LENGTH)
	  {
		/* Send Command Syntax error */
		Xcp_SendErrRes(XCP_ERR_CMD_SYNTAX, protLayerId);
	  }
#if(XCP_SEED_AND_KEY == STD_ON)
      else if (( XCP_RESOURCE_DAQ & Xcp_NoInit[protLayerId].Session.ResourceProtStatus_u8) != 0u)
      {
        /* Command is locked */
        /* Send Error packet code 0x25 */
        Xcp_SendErrRes(XCP_ERR_ACCESS_LOCKED, protLayerId);
      }
#endif
	  else /* all checks have been passed */
	  {
#if (XCP_DAQ_PACKED_MODE_SUPPORTED == STD_ON)
        /* Call the function for SET_DAQ_PACKED_MODE  */
        Xcp_CmdGetDaqPackedMode(XcpPacket, protLayerId);
#else
        /* Command not supported so we return an error */
        Xcp_SendErrRes(XCP_ERR_SUBCMD_UNKNOWN, protLayerId);
#endif
	  }
	  break;
	}

#if ( XCP_SUBCMD_0XC0_ASAM_POD_AVAILABLE == STD_ON)
    case XCP_SUBCMD_ASAM_POD_ID :
	{
	    /* Check the packet length */
	  if (XcpPacket->SduLength < XCP_CMD_0XC0_MIN_LENGTH)
	  {
		/* Send Command Syntax error */
		Xcp_SendErrRes(XCP_ERR_CMD_SYNTAX, protLayerId);
	  }
	  else /* all checks have been passed */
	  {
	    /* Call the function for ASAM POD  */
	    /* Command not implemented so we return an error */
	    Xcp_SendErrRes(XCP_ERR_SUBCMD_UNKNOWN, protLayerId);
	  }
	  break;
	}
#endif

    default :
    {
      /* Unknown sub command or not implemented optional sub command */
      Xcp_SendErrRes(XCP_ERR_SUBCMD_UNKNOWN, protLayerId);
      break;
    }

  }
}

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#else
  /* HIS METRIC COMF VIOLATION IN Xcp_Cmd0xC0Handling.c: The command is not enabled - no code is needed in this file */
#endif

