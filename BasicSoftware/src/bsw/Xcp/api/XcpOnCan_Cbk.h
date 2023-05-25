

#ifndef XCPONCAN_CBK_H
#define XCPONCAN_CBK_H

/***************************************************************************************************
* Includes
***************************************************************************************************/
#include "Xcp_Symbolic_Names_Cfg.h"  /* For symbolic names which is used in lower layer */

/***************************************************************************************************
* Defines/Macros
***************************************************************************************************/


/***************************************************************************************************
* Type definitions
***************************************************************************************************/


/***************************************************************************************************
* Variables
***************************************************************************************************/


/***************************************************************************************************
* Function prototypes
***************************************************************************************************/
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
This function is called by the lower layers when an AUTOSAR XCP PDU has been received
\param[in]  XcpRxPduId    PduId that has been received
\param[in]  XcpRxPduPtr   Pointer to received Pdu
\return     -
***************************************************************************************************/
extern void Xcp_CanIfRxIndication(PduIdType XcpRxPduId, const PduInfoType* XcpRxPduPtr);

#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#define XCP_START_SEC_CODE_FAST
#include "Xcp_MemMap.h"

/**
****************************************************************************************************
This function is called by the lower layers when an AUTOSAR XCP PDU has been transmitted
\param[in]  XcpTxPduId  PduId that has been transmitted
\return     -
***************************************************************************************************/
#if((XCP_AR_RELEASE_MAJOR_VERSION == 4u) && (XCP_AR_RELEASE_MINOR_VERSION > 2u))
extern void Xcp_CanIfTxConfirmation(PduIdType XcpTxPduId, Std_ReturnType result);
#else
extern void Xcp_CanIfTxConfirmation(PduIdType XcpTxPduId);
#endif

#define XCP_STOP_SEC_CODE_FAST
#include "Xcp_MemMap.h"


#endif   /* #ifndef XCPONCAN_CBK_H */


