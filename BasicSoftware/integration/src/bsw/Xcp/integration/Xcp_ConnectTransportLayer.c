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

#if (XCP_CONNECT_TRANSPORT_LAYER_CALLOUT == STD_ON)
/**
****************************************************************************************************
Function is called to check the connection is allowed to TransportLayer or not
\attention  XcpTransportLayerId is used and not XcpProtocolLayerId!
\param[in]  XcpTransportLayerId   Transport Layer Id
\return     State if connection is allowed
\retval     E_OK      Connection is allowed
\retval     E_NOT_OK  Connection is not allowed
***************************************************************************************************/
Std_ReturnType XcpAppl_ConnectTransportLayer(uint8 XcpTransportLayerId)
{
  /*-----------------------------------------------------------------*/
  /* This function is project specific and shall be totally reworked */
  /*-----------------------------------------------------------------*/

  /* Remove when parameter used */
  XCP_PARAM_UNUSED(XcpTransportLayerId);

  /* Return actual status based on implementation */
  return(E_OK);
}
#endif /* (XCP_CONNECT_TRANSPORT_LAYER_CALLOUT == STD_ON) */

/* ------------------------------------------------------------------------ */
/* End section for code */

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"


