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
#if (XCP_CMD_GET_ID_AVAILABLE == STD_ON)
static uint8 Xcp_SlaveIdType0_au8[] = {0x54,0x65,0x78,0x74}; /* Text */ /* Length = 4 */
static uint8 Xcp_SlaveIdType1_au8[] = {0x41,0x32,0x4C,0x5F,0x66,0x69,0x6C,0x65,0x6E,0x61,0x6D,0x65,0x5F,0x77,0x6F,0x5F,0x70,0x61,0x74,0x68}; /* A2L_filename_wo_path */ /* Length = 20 */
#endif /* (XCP_CMD_GET_ID_AVAILABLE == STD_ON) */


/***************************************************************************************************
* Functions
***************************************************************************************************/
/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

#if (XCP_CMD_GET_ID_AVAILABLE == STD_ON)
/**
****************************************************************************************************
Function is called when command GET_ID is requested by master.
\param[in]  IdType            Identification Type from CMD [0..4 (0..7 for Xcp v1.4), 128..255]
\param[out] AddrPtr           Pointer to the address where identification shall be stored
\param[in]  ProtocolLayerId   Protocol layer id
\return     Length of identification
\retval     0  Id is not supported
***************************************************************************************************/
uint32 XcpAppl_GetIdSlave(uint8 IdType, uint8** AddrPtr, uint8 ProtocolLayerId)
{
  /*-----------------------------------------------------------------*/
  /* This function is project specific and shall be totally reworked */
  /* As an example identification type 0 and 1 is implemented        */
  /*-----------------------------------------------------------------*/

  uint32 IdLength = 0;

  /* Remove when parameter used */
  XCP_PARAM_UNUSED(ProtocolLayerId);

  /* Check if not null pointer */
  if(AddrPtr != NULL_PTR)
  {
    if(IdType == 0)
    {
      /* Set type0 address and length */
      *AddrPtr = Xcp_SlaveIdType0_au8;
      IdLength = sizeof(Xcp_SlaveIdType0_au8);
    }
    else if(IdType == 1u)
    {
      /* Set type1 address and length */
      *AddrPtr = Xcp_SlaveIdType1_au8;
      IdLength = sizeof(Xcp_SlaveIdType1_au8);
    }
    else
    {
      /* IdLength = 0 for not supported Ids already set */
    }
  }
  return IdLength;
}
#endif /* (XCP_CMD_GET_ID_AVAILABLE == STD_ON) */

/* ------------------------------------------------------------------------ */
/* End section for code */

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"



