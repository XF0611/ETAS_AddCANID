

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

#if (XCP_CMD_BUILD_CHECKSUM_AVAILABLE == STD_ON)
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
This command starts the checksum calculation. \n
Checksum calculation is performed in the background
\param[in]  XcpPacket     Pointer to the received command packet
\param[in]  protLayerId   Protocol Layer Id
\return     -
***************************************************************************************************/
/* HIS METRIC LEVEL VIOLATION IN Xcp_CmdBuildChecksum: Function contains several necessary if statements. HIS metric compliance would decrease readability and maintainability. */
void Xcp_CmdBuildChecksum(const PduInfoType* XcpPacket, uint8 protLayerId)
{
  /*Start of run time measurement*/
  XCP_RTM_TIMETRACK_START()

  /* Command variables */
  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: This function is called with data (a PDU) from the CanIf/FrIf/EthIf which ensure 4-Byte-Alignment.
                                    The cast allows easier (= safer) access to the data in the PDU.
                                    Casting over void* is necessary as otherwise some compilers complain */
  const Xcp_CmdBuildChecksum_t* const CmdPtr = (const Xcp_CmdBuildChecksum_t*) (void*) XcpPacket->SduDataPtr;
  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: Compiler ensures the correct alignment in the datastructure, thus casting is no problem.
                                    The cast allows easier (= safer) access to the data in the PDU.
                                    Casting over void* is necessary as otherwise some compilers complain */
  Xcp_ResBuildChecksum_t* ResPtr = (Xcp_ResBuildChecksum_t*) (void*) XCP_RES_BUFFER_PTR(protLayerId);

  /* Local variables */
  Xcp_ErrorCode Error;
  Xcp_AddrType_t LocalMta;

  /* Store address in local variable */
  LocalMta = XCP_MTA(protLayerId);

  /* Check if another background process is running */
  if (Xcp_Cleared[protLayerId].BgActivityState != XCP_BG_IDLE)
  {
    Error = XCP_ERR_CMD_BUSY;
  }
#if (XCP_ADDRESS_EXTENSION == STD_OFF)
  /* Address-Extension is disabled so the extension has to be 0 */
  else if (LocalMta.Extension != 0)
  {
    Error = XCP_ERR_OUT_OF_RANGE;
  }
#endif
#if (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_ON)
  else
  {
    /* Call Address-transformation-function */
    Error = XcpAppl_StaticAddressTransformation(&LocalMta, CmdPtr->BlockSize_u32, XCP_EVENT_CHANNEL_NOT_USED, XCP_CONTEXT_CHECKSUM);
  }

  if (Error != XCP_NO_ERROR)
  {
    /* There is an error stored in Error, do nothing more */
  }
#endif

  /* Check if valid block size and alignment (only for Extension 0 as otherwise we do not know the right address)
   * Hint: Address should be aligned to speed up the operation and to remove the possibility to create checksums
   *       with start moved one byte -> this would allow to read out the memory */
  else if (    (CmdPtr->BlockSize_u32 <= XCP_CHECKSUM_MAX_BLOCK_SIZE)
            && ((CmdPtr->BlockSize_u32 % XCP_MTA_BLOCK_SIZE_ALIGN) == 0u)
            && (   ((LocalMta.AddrValue % XCP_MTA_BLOCK_SIZE_ALIGN) == 0u)
#if (XCP_ADDRESS_EXTENSION == STD_ON)
                || (LocalMta.Extension != 0u)
#endif
               )
          )
  {
    /* If AddrValue == 0 and Extension == 0 then we would try to access address 0 -> BAD, so forbid it */
    if (!((LocalMta.AddrValue == 0u)
#if (XCP_ADDRESS_EXTENSION == STD_ON)
            && (LocalMta.Extension == 0u)
#endif
    ))
    {
# if (XCP_MEMORY_READ_PROTECTION == STD_ON)
      /* Check if valid memory area to calculate the checksum value
       * XcpAppl_ChecksumProtection also has to take care of Extension as the memory may be accessed differently */
      Error = XcpAppl_ChecksumProtection(LocalMta, CmdPtr->BlockSize_u32);

      /* Checksum shall hide real value? */
      if (Error == XCP_NO_ACCESS_HIDE)
      {
        Xcp_Cleared[protLayerId].Checksum.FakeChecksum_b = TRUE;
      }
      else
      {
        Xcp_Cleared[protLayerId].Checksum.FakeChecksum_b = FALSE;
      }

      /* XCP_NO_ERROR and XCP_NO_ACCESS_HIDE are acceptable */
      if (Error != XCP_ERR_ACCESS_DENIED)
# endif /* (XCP_MEMORY_READ_PROTECTION == STD_ON) */
      {
        /* Trigger checksum calculation */
        Error = XcpAppl_BuildChecksumTrigger(LocalMta, CmdPtr->BlockSize_u32, protLayerId);
      }
    }
    else /* LocalMta != NULL_PTR */
    {
      Error = XCP_ERR_ACCESS_DENIED;
    }
  }
  else
  {
    /* Set Status to Out of range */
    Error = XCP_ERR_OUT_OF_RANGE;
  }

  /* Check if no error */
  if (Error == XCP_NO_ERROR)
  {
    /* Store block size */
    Xcp_Cleared[protLayerId].Checksum.BlockSize_u32 = CmdPtr->BlockSize_u32;

    /* Set state for background activity -> trigger for Xcp_BuildChecksumMainFunction to start calculation */
    /* Important!! Must be called after XcpAppl_BuildChecksumTrigger() */
    Xcp_Cleared[protLayerId].BgActivityState = XCP_BG_CHKSUM;

    /* Start the timeout monitor */
    Xcp_Cleared[protLayerId].ReqTimeoutCnt_u16 = 1u;

    /* No response - Checksum will be calculated in Xcp_BuildChecksumMainFunction */
  }
  /* Error */
  else
  {
    /* Error with no extra parameter? */
    if (Error == XCP_ERR_OUT_OF_RANGE)
    {
      /* Fill response data (using positive response structure, because structure entries are matching) */
      XCP_RES_BUFFER_LENGTH(protLayerId) = XCP_RES_BUILD_CHECKSUM_LENGTH;
      ResPtr->PacketId_u8 = XCP_PACKET_ID_ERR;
      ResPtr->ChecksumType_u8 = (uint8) Error;
      ResPtr->Reserved_u16 = XCP_MTA_BLOCK_SIZE_ALIGN;
      ResPtr->Checksum_u32 = XCP_CHECKSUM_MAX_BLOCK_SIZE;

      /* Send error packet */
      Xcp_SendRes(protLayerId);
    }
    else
    {
      /* Send error packet */
      Xcp_SendErrRes(Error, protLayerId);
    }
  }

  /*End of run time measurement*/
  XCP_RTM_TIMETRACK_FINISH()
}


/**
****************************************************************************************************
This function initializes all parameters used by Checksum command
\param[in]  protLayerId   Protocol Layer Id
\return     -
***************************************************************************************************/
void Xcp_InitChecksum(uint8 protLayerId)
{
  /* Initialize command parameters */
  Xcp_Cleared[protLayerId].Checksum.BlockSize_u32 = 0;
# if (XCP_MEMORY_READ_PROTECTION == STD_ON)
  Xcp_Cleared[protLayerId].Checksum.FakeChecksum_b = FALSE;
# endif
}

/**
****************************************************************************************************
Main function for background checksum calculation
\param[in]  protLayerId   Protocol Layer Id
\return     -
***************************************************************************************************/
void Xcp_BuildChecksumMainFunction(uint8 protLayerId)
{
  /* Command variables */
  /* MR12 RULE 11.5, DIR 1.1 VIOLATION: Compiler ensures the correct alignment in the datastructure, thus casting is no problem.
                                    The cast allows easier (= safer) access to the data in the PDU.
                                    Casting over void* is necessary as otherwise some compilers complain */
  Xcp_ResBuildChecksum_t* ResPtr = (Xcp_ResBuildChecksum_t*) (void*) XCP_RES_BUFFER_PTR(protLayerId);

  /* Local variables */
  Xcp_ErrorCode Error;

  /* Checksum background calculation */
  Error = XcpAppl_BuildChecksumMainFunction(&ResPtr->Checksum_u32, &ResPtr->ChecksumType_u8, protLayerId);

# if (XCP_MEMORY_READ_PROTECTION == STD_ON)
  /* Checksum shall hide real value? */
  if (Xcp_Cleared[protLayerId].Checksum.FakeChecksum_b == TRUE)
  {
    /* Response shall be send? */
    if (Error != XCP_NO_RESPONSE)
    {
      /* Set fake value of the checksum and send positive response */
      ResPtr->Checksum_u32 = 0;
      Error = XCP_NO_ERROR;
    }
  }
# endif

  /* Check if no error */
  if (Error == XCP_NO_ERROR)
  {
    /* Update MTA - BlockSize was set in Xcp_CmdBuildChecksum*/
    XCP_MTA(protLayerId).AddrValue +=  Xcp_Cleared[protLayerId].Checksum.BlockSize_u32;

    /* Fill response data */
    XCP_RES_BUFFER_LENGTH(protLayerId) = XCP_RES_BUILD_CHECKSUM_LENGTH;
    ResPtr->PacketId_u8 = XCP_PACKET_ID_RES;
    /* ResPtr->ChecksumType_u8 - already set in XcpAppl_BuildChecksumMainFunction */
    ResPtr->Reserved_u16 = 0;
    /* ResPtr->Checksum_u32 - already set in XcpAppl_BuildChecksumMainFunction */

    /* Clear flag that calculation in progress */
    Xcp_Cleared[protLayerId].BgActivityState = XCP_BG_IDLE;

    /* Send positive response */
    Xcp_SendRes(protLayerId);

    /* Reset the timeout counter */
    Xcp_Cleared[protLayerId].ReqTimeoutCnt_u16 = 0u;
  }
  else if (Error == XCP_NO_RESPONSE)
  {
    /* Still waiting for the calculation to be finished - no response */
    if(Xcp_Cleared[protLayerId].ReqTimeoutCnt_u16 != 0u)
    {
      Xcp_Cleared[protLayerId].ReqTimeoutCnt_u16++;
      /* Request the master to restart the timeout */
      if(Xcp_Cleared[protLayerId].ReqTimeoutCnt_u16 >= XCP_BUILDCHECKSUM_CMD_TIMEOUT_CNT)
      {
        Xcp_Cleared[protLayerId].ReqTimeoutCnt_u16 = 1u;
        Xcp_SendEv_Code(XCP_EV_CMD_PENDING, protLayerId);
      }
    }
  }
  else
  {/* Error */
    /* Clear flag that calculation in progress */
    Xcp_Cleared[protLayerId].BgActivityState = XCP_BG_IDLE;

    /* Send error packet */
    Xcp_SendErrRes(Error, protLayerId);
  }
}

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#else
/* HIS METRIC COMF VIOLATION IN Xcp_CmdBuildChecksum.c: Checksum disabled - no code is needed in this file */
#endif

