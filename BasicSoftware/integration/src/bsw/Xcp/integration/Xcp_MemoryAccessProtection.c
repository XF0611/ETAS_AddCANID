/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



/***************************************************************************************************
* Includes
***************************************************************************************************/
#include "Xcp_Cbk.h"


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

#if (XCP_MEMORY_READ_PROTECTION == STD_ON)
/**
****************************************************************************************************
Check if is allowed to read from this area
\param[in]  XcpAddr       - Memory start address
\param[in]  Length        - Length of reading area
\retval     XCP_NO_ERROR            access granted
\retval     XCP_ERR_ACCESS_DENIED   access denied
\retval     XCP_NO_ACCESS_HIDE      access denied, but correct response is sent with data content 0x00
***************************************************************************************************/
Xcp_ErrorCode XcpAppl_MemReadProtection(const Xcp_AddrType_t XcpAddr, uint8 Length)
{
  /*-----------------------------------------------------------------*/
  /* This function is project specific and shall be totally reworked */
  /*-----------------------------------------------------------------*/

  /* Remove when parameter used */
  XCP_PARAM_UNUSED(XcpAddr);
  XCP_PARAM_UNUSED(Length);

  /* Do checks here... */

  return(XCP_NO_ERROR);
}

# if (XCP_CMD_BUILD_CHECKSUM_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Check if the requested area is allowed to read for Build Checksum
\param[in]  XcpAddr       - Memory start address
\param[in]  Length        - Length of reading area
\retval     XCP_NO_ERROR            access granted
\retval     XCP_ERR_ACCESS_DENIED   access denied
\retval     XCP_NO_ACCESS_HIDE      access denied, but correct response is sent with data content 0x00
***************************************************************************************************/
Xcp_ErrorCode XcpAppl_ChecksumProtection(const Xcp_AddrType_t XcpAddr, uint32 Length)
{
  /*-----------------------------------------------------------------*/
  /* This function is project specific and shall be totally reworked */
  /*-----------------------------------------------------------------*/

  /* Remove when parameter used */
  XCP_PARAM_UNUSED(XcpAddr);
  XCP_PARAM_UNUSED(Length);

  /* Do checks here... */

  return(XCP_NO_ERROR);
}
# endif /* (XCP_CMD_BUILD_CHECKSUM_AVAILABLE == STD_ON) */
#endif /* (XCP_MEMORY_READ_PROTECTION == STD_ON) */


#if (XCP_MEMORY_WRITE_PROTECTION == STD_ON)
/**
****************************************************************************************************
Check if is allowed to write to this area
\param[in]  XcpAddr   Memory start address
\param[in]  Length    Length of reading area
\retval     XCP_NO_ERROR            access granted
\retval     XCP_ERR_ACCESS_DENIED   access denied
***************************************************************************************************/
Xcp_ErrorCode XcpAppl_MemWriteProtection(const Xcp_AddrType_t XcpAddr, uint8 Length)
{
  /*-----------------------------------------------------------------*/
  /* This function is project specific and shall be totally reworked */
  /*-----------------------------------------------------------------*/

  /* Remove when parameter used */
  XCP_PARAM_UNUSED(XcpAddr);
  XCP_PARAM_UNUSED(Length);

  /* Do checks here... */

  return(XCP_NO_ERROR);
}
#endif /* (XCP_MEMORY_WRITE_PROTECTION == STD_ON) */

/* ------------------------------------------------------------------------ */
/* End section for code */

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"


