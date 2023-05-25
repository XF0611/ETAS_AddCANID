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
#include "Xcp_Priv.h"
#include "Xcp_Integration.h"

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

#if (   (XCP_CMD_BUILD_CHECKSUM_AVAILABLE == STD_ON) \
     || ((XCP_CALIBRATION_CAL == STD_ON) && (XCP_UPLOAD_DOWNLOAD_MEMORY_ACCESS_BY_APPL == STD_ON)) )
/**
****************************************************************************************************
Function to do specific initialization or cancel pending background process
\param[in]  XcpInitStatus     Xcp Initialization status: XCP_INIT or XCP_UNINIT
\param[in]  ProtocolLayerId   protocol layer id
\return     Was initialization successful
\retval     E_OK              Initialization is finished
\retval     E_NOT_OK          Initialization is not finished, this function will be called
                              in next Xcp_MainFunction() again until it returns E_OK
***************************************************************************************************/
Std_ReturnType XcpAppl_Init(uint8 XcpInitStatus, uint8 ProtocolLayerId)
{
  /*-----------------------------------------------------------------*/
  /* This function is project specific and shall be totally reworked */
  /*-----------------------------------------------------------------*/

  /* Remove when parameter used */
  XCP_PARAM_UNUSED(XcpInitStatus);
  XCP_PARAM_UNUSED(ProtocolLayerId);

  /* Do checks here... */
  if (Xcp_GlobalNoInit.InitStatus_u8 == XCP_UNINIT)
  {
    Overlay_Sync();
    Overlay_Init();
  }

  return(E_OK);
}
#endif


/**
****************************************************************************************************
Function to do get current page
\param[in]  None
\param[out] None
\return     CalPage_T
				CalPage_Working: In case XCP is working on working page (on the RAM)
				CalPage_Reference: In case XCP is working on reference page (on the FLASH)
***************************************************************************************************/
CalPage_T XcpAppl_GetPage(void)
{
	return XcpCurrentCalPage;
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"


