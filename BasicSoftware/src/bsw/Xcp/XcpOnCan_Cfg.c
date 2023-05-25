
/*<VersionHead>
****************************************************************************************************
* This Configuration File is generated using versions (automatically filled in) as listed below.
*
* $Generator__: Xcp / AR42.10.0.0                 Module Package Version
* $Editor_____: ISOLAR-A/B 9.2.1_9.2.1             Tool Version
****************************************************************************************************
</VersionHead>*/

/***************************************************************************************************
* Includes
***************************************************************************************************/
#include "Xcp.h"
#include "XcpOnCan_Cfg.h"


/***************************************************************************************************
* Variables
***************************************************************************************************/

#define XCP_START_SEC_CONST_UNSPECIFIED
#include "Xcp_MemMap.h"

const Xcp_CanCfgConst_t Xcp_CanCfgConst =
{
  /* ------------------------ TransportLayerCanConfig - Configuration table for CAN channels ----------------------------------------------------------------------------------------- */
  {
    {
       0u  /* Offset where this CAN transport layer starts in TxPduConfigTable table */
      ,1u  /* Number of elements of this CAN transport layer in TxPduConfigTable table */
#if (XCP_DEV_ERROR_DETECT == STD_ON)
      ,8u  /* MAX_CTO - Maximum length of command packet */
      ,8u  /* MAX_DTO - Maximum length of data packet */
#endif
    }
  },

  /* ------------------------ TxPduConfigTable - Configuration table for TxPdus ------------------------------------------------------------------------------------------------------ */
  {
    {
      5u,  /* Tx PduId */
      XCP_EVENT_CHANNEL_NOT_USED  /* Event Channel Number */
    }
  },

  /* ------------------------ RxPduId2TransportLayerId - Routing table where index is RxPduId and value is corresponding TransportLayerId used by RxIndication ----------------------- */
  {
    0u   /* TranportLayerId */
  },

  /* ------------------------ TxPduId2TransportLayerId - Routing table where index is TxPduId and value is corresponding TransportLayerId used by TxConfirmation --------------------- */
  {
    0u   /* TranportLayerId */
  }
};

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include "Xcp_MemMap.h"

/***************************************************************************************************
* Functions
***************************************************************************************************/


