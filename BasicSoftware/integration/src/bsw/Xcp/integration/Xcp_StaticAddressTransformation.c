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

#if (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_ON)
/**
****************************************************************************************************
Transforms the given address into another address (Mainly used for HBB)
\param[inout] XcpAddrPtr            Address pointer that initially holds the address that is to be translated
                                    and after successful transformation it holds the new (transformed) address
\param[in]    Length                Length of the data that are located on the given address
\param[in]    XcpEventChannelNumber Number of the Event-Channel this Address belongs to (XCP_EVENT_CHANNEL_NOT_USED if not DAQ/STIM)
\param[in]    contextType           Context from which this function is called (DAQ/STIM/UPLOAD/DOWNLOAD)
\return       Xcp_ErrorCode
***************************************************************************************************/
Xcp_ErrorCode XcpAppl_StaticAddressTransformation(Xcp_AddrType_t* XcpAddrPtr, uint32 Length, uint16 XcpEventChannelNumber, Xcp_ContextType_t contextType)
{
  /*-----------------------------------------------------------------*/
  /* This function is project specific and shall be totally reworked */
  /*-----------------------------------------------------------------*/

  /* Remove when parameter used */
  XCP_PARAM_UNUSED(XcpAddrPtr);
  XCP_PARAM_UNUSED(Length);
  XCP_PARAM_UNUSED(XcpEventChannelNumber);
  XCP_PARAM_UNUSED(contextType);

  return XCP_NO_ERROR;
}

#endif /* (XCP_STATIC_ADDRESS_TRANSFORMATION == STD_ON) */

/* ------------------------------------------------------------------------ */
/* End section for code */

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

