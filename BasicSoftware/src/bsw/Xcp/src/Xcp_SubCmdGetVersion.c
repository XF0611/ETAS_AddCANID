
/***************************************************************************************************
* Includes
***************************************************************************************************/
#include "Xcp.h"
#include "Xcp_Priv.h"


/***************************************************************************************************
* Variables
***************************************************************************************************/
#if (XCP_SUBCMD_0XC0_GET_VERSION_AVAILABLE == STD_ON)

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
void Xcp_SubCmdGetVersion( const PduInfoType* XcpPacket, uint8 protLayerId )
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()


  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: Compiler ensures the correct alignment in the datastructure, thus casting is no problem.
                                    The cast allows easier (= safer) access to the data in the PDU.
                                    Casting over void* is necessary as otherwise some compilers complain */
  Xcp_SubResGetVersion_t* ResPtr = (Xcp_SubResGetVersion_t*) (void*) XCP_RES_BUFFER_PTR(protLayerId);


  /* XcpPacket is unused in this function */
  XCP_PARAM_UNUSED(XcpPacket);

  /* prepare positive response */
  /* set length response */
  XCP_RES_BUFFER_LENGTH(protLayerId) = XCP_SUBRES_GET_VERSION_LENGTH;

  /* Set Pid */
  ResPtr->PacketId_u8 = XCP_PACKET_ID_RES;

  /* Set protocol layer major version number */
  ResPtr->XcpMajorProtocolLayerVersion_u8 = XCP_ASAM_MAJOR_VERSION;

  /* Set protocol layer minor version number */
  ResPtr->XcpMinorProtocolLayerVersion_u8 = XCP_ASAM_MINOR_VERSION;

  /* Set transport layer major version number (Currently same as protocol layer major version)*/
  ResPtr->XcpMajorTransportLayerVersion_u8 = XCP_ASAM_MAJOR_VERSION;

  /* Set transport layer minor version number (Currently same as protocol layer minor version)*/
  ResPtr->XcpMinorTransportLayerVersion_u8 = XCP_ASAM_MINOR_VERSION;

  /* Send positive response */
  Xcp_SendRes(protLayerId);

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
}


/* ------------------------------------------------------------------------ */
/* End section for code */

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#else
/* HIS METRIC COMF VIOLATION IN Xcp_SubCmdGetVersion.c: The command is not enabled - no code is needed in this file */
#endif


