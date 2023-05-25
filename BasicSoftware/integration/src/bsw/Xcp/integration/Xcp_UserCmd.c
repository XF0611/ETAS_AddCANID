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
#if (XCP_CMD_USER_CMD_AVAILABLE == STD_ON)

/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
Function for user-defined command
\param[in]  XcpCmdPtr         Command pointer
\param[in]  CmdLength         Command Length
\param[out] XcpResPtr         Response pointer
\param[out] ResLength         Response Length
\param[in]  ProtocolLayerId   protocol layer id
\retval     XCP_NO_ERROR          success \n
\retval     XCP_ERR_CMD_UNKNOWN   Command not supported
\retval     XCP_ERR_CMD_SYNTAX    Command syntax invalid
\retval     XCP_ERR_OUT_OF_RANGE  Command parameter(s) out of range
***************************************************************************************************/
Xcp_ErrorCode XcpAppl_UserCmd(const uint8* XcpCmdPtr, uint8 CmdLength, uint8* XcpResPtr ,uint8* ResLength, uint8 ProtocolLayerId)
{
    /*-----------------------------------------------------------------*/
    /* This function is project specific and shall be totally reworked */
    /*-----------------------------------------------------------------*/

    /* Local variables */
    Xcp_ErrorCode Error;

    /* Command syntax shall be validated by integrator, if not valid ERR_CMD_SYNTAX shall be assigned to Error */

    /* Example for Response data and length */
    /* MR12 RULE 11.5, DIR 1.1 VIOLATION: Compiler ensures the correct alignment in the datastructure to which XcpResPtr points, thus casting is no problem.
                                      The cast allows easier (= safer) access to the data in the PDU.
                                      Casting over void* is necessary as otherwise some compilers complain */
    Xcp_ResUserCmd_t* ResPtr = (Xcp_ResUserCmd_t*) (void*) XcpResPtr;

    ResPtr->PacketId_u8 = 0xFF;
    *ResLength = 1;

    /* Remove when parameter used */
    XCP_PARAM_UNUSED(XcpCmdPtr);
    XCP_PARAM_UNUSED(CmdLength);
    XCP_PARAM_UNUSED(ProtocolLayerId);

    /* Initial values */
    Error = XCP_NO_ERROR;

    /* return Error Code */
    return(Error);
}

/* ------------------------------------------------------------------------ */
/* End section for code */

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#endif


