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

#if (XCP_SCHEDULECALL_FLAG == STD_ON)
/**
****************************************************************************************************
This function is called at the start of the processing of a specific DAQ List. It shall perform the function
of storing the current system time into a counter variable. This counter will then be used by the
XcpAppl_TriggerSchedule function in order to calculate total execution time by the DAQ List.
This is done to ensure that in co-operative multitasking, high prioritized tasks will not be blocked for too long.
This function shall then reset the counters for the given event channel.

\param[in]  EventChannelNum   Current event channel number
\return     -
\retval     -
***************************************************************************************************/
void XcpAppl_EnterDaqList(uint16 EventChannelNum)
{
  /*-----------------------------------------------------------------*/
  /* This function is project specific and shall be totally reworked */
  /*-----------------------------------------------------------------*/

  /* Remove when parameter used */
  XCP_PARAM_UNUSED(EventChannelNum);


  /* Do checks here... */

}
#endif

/* ------------------------------------------------------------------------ */
/* End section for code */

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

/*<BASDKey>
*********************************************************************************************************
* $History___:$
*********************************************************************************************************
</BASDKey>*/
