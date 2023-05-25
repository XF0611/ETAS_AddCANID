

#ifndef SOAD_PRV_H
#define SOAD_PRV_H

/*******************************************************************************
*                      Include Section                                       **
*******************************************************************************/

#include "ComStack_Types.h"
#include "SoAd.h"
#include "SoAd_Cfg_SchM.h"

#if ( SOAD_DEV_ERROR_DETECT_ENABLE != STD_OFF )
#include "Det.h"
#endif /* SOAD_DEV_ERROR_DETECT_ENABLE != STD_OFF */
/**************************************************************************************************/
/*  Common Published Information                                                                  */
/**************************************************************************************************/

/****************************************************************************************************************
**                                   External Variables Declarations                                           **
****************************************************************************************************************/
#if ( SOAD_NUM_TX_IF_UDP_PDU > 0 )
#define SOAD_START_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"

extern boolean SoAd_ULIfUdpTxConfirmFlag_ab[];

#define SOAD_STOP_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"
#endif /* SOAD_NUM_TX_IF_UDP_PDU > 0 */

#define SOAD_START_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"
extern boolean SoAd_InitFlag_b;

#if ( SOAD_ZERO_COPY_IF_UDP_TX != STD_OFF)
extern boolean SoAd_ZeroCopyConnChanged_b;
#endif /*SOAD_ZERO_COPY_IF_UDP_TX != STD_OFF*/

#define SOAD_STOP_SEC_VAR_INIT_8
#include "SoAd_MemMap.h"

#define SOAD_START_SEC_VAR_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"
extern SoAd_SoConIdType SoAd_DynSockIndx_auo[];
extern SoAd_SoConIdType SoAd_TcpIpSocHandleToDynSockIdx_auo[];
extern TcpIp_SocketIdType SoAd_GroupTcpIpSocketId_auo[];

#if (SOAD_NUM_TX_IF_TCP_PDU > 0)
extern SoAd_IfTcpPduTxPendingStatus_tst SoAd_IfTcpPduTxPendingStatus_ast[];
#endif /* SOAD_NUM_TX_IF_TCP_PDU > 0 */

/* Dynamic socket table */
extern SoAd_DyncSocConfigType_tst SoAd_DyncSocConfig_ast[];
/* List of all remote IPv4 address (together with port) for dynamic sockets */
extern SoAd_SockAddrInetType * SoAd_DynSoConRemoteAddrInet_pst;
/* List of all remote IPv6 address (together with port) for dynamic sockets */
extern SoAd_SockAddrInet6Type * SoAd_DynSoConRemoteAddrInet6_pst;


#define SOAD_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "SoAd_MemMap.h"

#if ( SOAD_GETANDRESET_MEASUREMENTDATA_API == STD_ON )

#define SOAD_START_SEC_VAR_INIT_32
#include "SoAd_MemMap.h"

/* variable to hold dropped udp packets count at SoAd level */
extern uint32 SoAd_RxUdpDropCnt_u32;
/* variable to hold dropped tcp packets count at SoAd level */
extern uint32 SoAd_RxTcpDropCnt_u32;

#define SOAD_STOP_SEC_VAR_INIT_32
#include "SoAd_MemMap.h"

#endif /* SOAD_GETANDRESET_MEASUREMENTDATA_API == STD_ON */


#define SOAD_START_SEC_VAR_INIT_32
#include "SoAd_MemMap.h"
extern const SoAd_ConfigType * SoAd_CurrConfig_cpst;
#define SOAD_STOP_SEC_VAR_INIT_32
#include "SoAd_MemMap.h"

#if ( TCPIP_TCP_ENABLED == STD_ON )
#define SOAD_TCPIP_TCPSOCKET_MAX    TCPIP_TCPSOCKETMAX
#else
#define SOAD_TCPIP_TCPSOCKET_MAX    SOAD_NUM_TCPIP_SOCKETS
#endif

#define SOAD_FORCE_RETRIEVE_TRANSMISSION_TRUE       TRUE
#define SOAD_FORCE_RETRIEVE_TRANSMISSION_FALSE      FALSE

#define SOAD_SOCKET_CONNECTION_REFERENCE            TRUE
#define SOAD_SOCKET_BUNDLE_REFERENCE                FALSE

#ifndef SOAD_PDUHDR_SIZE
/* Size of the PDU header of SoAd */
#define SOAD_PDUHDR_SIZE                            8U
#endif

/* Size of the PDU ID part in the PDU header of SoAd(Header Id / length) */
#define SOAD_PDUHDR_PDUID_LENGTH_SIZE               4U

/* Size of the PDU LEN part in the PDU header of SoAd */
#define INVALID                                     7U
#define SOAD_NULL                                   0U
#define SOAD_CFG_SOCON_ID_DEFAULT_VALUE             (SoAd_SoConIdType)0xFFFFU
#define SOAD_CFG_SOCKET_ID_DEFAULT_VALUE            (TcpIp_SocketIdType)0xFFFFU
#define SOAD_CFG_PDU_ID_DEFAULT_VALUE               (PduIdType)0xFFFFU
#define SOAD_CFG_RGRP_DEFAULT_VALUE                 (SoAd_RoutingGroupIdType)0xFFFFU
#define SOAD_CFG_PDU_LENGTH_DEFAULT_VALUE           (PduLengthType)0xFFFFFFFFU
#define SOAD_CFG_TCPKEEPALIVE_DEFAULT_VALUE         (SoAd_TcpSocGrpIndexType)0xFFFFFFFFU
#define SOAD_CFG_TP_UDP_TXBUFFER_DEFAULT_VALUE      (SoAd_TpUdpTxBuffIndexType)0xFFFFFFFFU
#define SOAD_CFG_nPDU_UDP_TX_DEFAULT_VALUE          (SoAd_nPduUdpTxIndexType)0xFFFFFFFFU
#define SOAD_CFG_UDPALIVESUPERVISION_DEFAULT_VALUE  (SoAd_UdpSocGrpIndexType)0xFFFFFFFFU
#define SOAD_DEFAULT_PORT                           0xFFFFU
#define SOAD_DEFAULT_IP                             0xFFFFFFFFU
#define SOAD_DEFAULT_PDU_HDR_ID                     0xFFFFFFFFU

/* Default config value of flow label */
#define SOAD_CFG_FLOWLABEL_DEFAULT_VALUE     0UL
#define SOAD_CFG_DSCP_DEFAULT_VALUE          0U
#define SOAD_TLS_CONN_DEFAULT_VALUE          (TcpIp_TlsConnectionIdType)0xFFFFU

/* SoAd DET Errors */
#define SOAD_E_NOTINIT                      0x01U
#define SOAD_E_PARAM_POINTER                0x02U
#define SOAD_E_INV_ARG                      0x03U
#define SOAD_E_NOBUFS                       0x04U
#define SOAD_E_INV_PDUHEADER_ID             0x05U
#define SOAD_E_INV_PDUID                    0x06U
#define SOAD_E_INV_SOCKETID                 0x07U
#define SOAD_E_INIT_FAILED                  0x08U
#define SOAD_E_INV_METADATA                 0x09U

/* FramePriority is a 3 bit field in VLAN header, so it's maximum value is 7 only */
#define SOAD_ETH_VLAN_FRAMEPRIO_MAXVAL      7U

#define SOAD_DHCP_RESERVED_SERVER_PORT      67U
#define SOAD_DHCP_RESERVED_CLIENT_PORT      68U

/* SoAd API Id's */
#define SOAD_INIT_API_ID                                    0x01U
#define SOAD_GET_VERSION_INFO_API_ID                        0x02U
#define SOAD_IF_TRANSMIT_API_ID                             0x03U
#define SOAD_TP_TRANSMIT_API_ID                             0x04U
#define SOAD_TP_CANCEL_TRANSMIT_API_ID                      0x05U
#define SOAD_TP_CANCEL_RECEIVE_API_ID                       0x06U
#define SOAD_GET_SOCON_API_ID                               0x07U
#define SOAD_OPEN_SOCON_API_ID                              0x08U
#define SOAD_CLOSE_SOCON_API_ID                             0x09U
#define SOAD_REQUEST_IPADDR_ASSIGNMENT_API_ID               0x0AU
#define SOAD_RELEASE_IPADDR_ASSIGNMENT_API_ID               0x0BU
#define SOAD_GET_LOCAL_ADDR_API_ID                          0x0CU
#define SOAD_GET_PHYS_ADDR_API_ID                           0x0DU
#define SOAD_ENABLE_ROUTING_API_ID                          0x0EU
#define SOAD_DISABLE_ROUTING_API_ID                         0x0FU
#define SOAD_SET_REMOTE_ADDRESS_API_ID                      0x10U
#define SOAD_CHANGE_PARAMETER_API_ID                        0x11U
#define SOAD_RX_INDICATION_API_ID                           0x12U
#define SOAD_COPY_TX_DATA_API_ID                            0x13U
#define SOAD_TX_CONFIRMATION_API_ID                         0x14U
#define SOAD_TCP_ACCEPTED_API_ID                            0x15U
#define SOAD_TCP_CONNECTED_API_ID                           0x16U
#define SOAD_TCPIP_EVENT_API_ID                             0x17U
#define SOAD_LOCAL_IPADDR_ASSIGNMENT_CHG_API_ID             0x18U
#define SOAD_MAIN_FUNCTION_API_ID                           0x19U
#define SOAD_READ_DHCP_HOST_NAME_OPTION_API_ID              0x1AU
#define SOAD_WRITE_DHCP_HOST_NAME_OPTION_API_ID             0x1BU
#define SOAD_GET_REMOTE_ADDR_API_ID                         0x1CU
#define SOAD_IF_ROUTING_GROUP_TRANSMIT_API_ID               0x1DU
#define SOAD_SET_UNIQUE_REMOTE_ADDRESS_API_ID               0x1EU
#define SOAD_IF_SPECIFIC_ROUTING_GROUP_TRANSMIT_API_ID      0x1FU
#define SOAD_ENABLE_SPECIFIC_ROUTING_API_ID                 0x20U
#define SOAD_DISABLE_SPECIFIC_ROUTING_API_ID                0x21U
#define SOAD_INFORM_START_OF_RECEPTION_API_ID               0x22U
#define SOAD_RELEASE_REMOTE_ADDR_API_ID                     0x23U
#define SOAD_TP_TX_CONFIRMATION_TO_UL_API_ID                0x24U
#define SOAD_TCPIP_CLOSE_API_ID                             0x25U
#define SOAD_GET_ROUTING_GROUP_STATUS_API_ID                0x26U
#define SOAD_GETANDRESET_MEASUREMENTDATA_API_ID             0x45U
#define SOAD_ZERO_COPY_IF_TRANSMIT_API_ID                   0x46U

/* DET macro definition */
#if ( SOAD_DEV_ERROR_DETECT_ENABLE != STD_OFF )
        /* For functions which should not be exit */
#define SOAD_DET_REPORT_ERROR(API, ERROR )  (void)Det_ReportError( (SOAD_MODULE_ID), (SOAD_INSTANCE_ID), (API), (ERROR) );
#else
#define SOAD_DET_REPORT_ERROR( API, ERROR )
#endif

    /*  Macro for functions to report DET for void return type */
#define SOAD_DET_CHECK_ERROR_VOID(CONDITION, API, ERROR )                   \
    if(CONDITION)                                                           \
    {                                                                       \
        SOAD_DET_REPORT_ERROR(API, ERROR)                                   \
        return;                                                             \
    }
    /*  Macro for functions to report DET with Return value */
#define SOAD_DET_CHECK_ERROR_RETVAL( CONDITION, API, ERROR, RETVAL)         \
    if(CONDITION)                                                           \
    {                                                                       \
        SOAD_DET_REPORT_ERROR(API, ERROR)                                   \
        return (RETVAL);                                                    \
    }

/****************************************************************************************************************
**                                   Function Prototypes                                                       **
****************************************************************************************************************/
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"

#if (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF )
LOCAL_INLINE boolean SoAd_GetRoutingGroupsEnabledStatus( \
                         const SoAd_RoutingGroupIdType * RoutingGroups_cpuo, \
                         SoAd_RoutingGroupIdType         NumOfTxRoutingGroups_uo, \
                         SoAd_SoConIdType                SoConId_uo );

/**
 *******************************************************************************************************************
 * \Function Name : SoAd_GetRoutingGroupsEnabledStatus()
 *
 * \Function description
 * This service shall be invoked to get the Routing Group enabled status for particular pduRouteDest or socketRouteDest.
 * If multiple routing groups are configured for any pduRouteDest/socketRouteDest, then this service shall loop through for all
 * routing groups to identify any one of the routing group is enabled for the given SoConId combination.
 *
 * Parameters (in):
 * \param   const SoAd_RoutingGroupIdType*
 * \arg     RoutingGroups_cpuo - list of Routing Group id's for particular PduRouteDest
 * \param   SoAd_RoutingGroupIdType
 * \arg     NumOfTxRoutingGroups_uo - Number of Routing Group id's passed in RoutingGroups_cpuo argument
 * \param   SoAd_SoConIdType
 * \arg     SoConId_uo - static socket id
 *
 * Parameters (inout): None
 *
 * Parameters (out): None
 *
 * Return value: boolean
 *               TRUE - Routing group is enbaled for atleast one of routing group available at RoutingGroups_cpuo[]
 *               FALSE - Routing group is disbaled for all routing groups available at RoutingGroups_cpuo[]
 *
 ********************************************************************************************************************
 */
LOCAL_INLINE boolean SoAd_GetRoutingGroupsEnabledStatus( const SoAd_RoutingGroupIdType * RoutingGroups_cpuo, \
                                                         SoAd_RoutingGroupIdType         NumOfTxRoutingGroups_uo, \
                                                         SoAd_SoConIdType                SoConId_uo )
{
    /* Local pointer to store the SoAd_RoutingGroupSoConStructMap_tst array for given index*/
    SoAd_RoutingGroupSoConStructMap_tst * lSoAdRoutingGroupSoConStructMap_pst;

    /* Loop through for NumofSocket Mapping to RG Id */
    SoAd_SoConIdType lIdxSoCon_uo;

    /* Loop through for NumofTxRoutingGroups Mapping to PduRouteDests */
    SoAd_RoutingGroupIdType lIdxOfRoutingGroup_uo;

    /* Variable to hold the Routing group Id */
    SoAd_RoutingGroupIdType lRgId_uo;

    /* Routing Group Status */
    boolean lRgStatus_b;

    /* Initialize the local variables which are declared above */
    lRgStatus_b = FALSE;

    for( lIdxOfRoutingGroup_uo = 0;
                ( ( lIdxOfRoutingGroup_uo < NumOfTxRoutingGroups_uo ) && ( FALSE == lRgStatus_b ) ); lIdxOfRoutingGroup_uo++)
    {
        /* fetch the Routing Group Id ono by one for checking the routing group status for this pduroute dest,
         * If any one of Routing group status is enabled then packet will be trnasmitted to LL or forwarded to UL */
        lRgId_uo = RoutingGroups_cpuo[lIdxOfRoutingGroup_uo];

        /* check routing group id is valid */
        if(lRgId_uo < SoAd_CurrConfig_cpst->SoAd_noRoutingGrps_cuo)
        {
            /* copy the global array address to local pointer for given index */
            lSoAdRoutingGroupSoConStructMap_pst    = &(SoAd_CurrConfig_cpst->SoAd_RoutingGroupSoConStructMap_pst[lRgId_uo]);

            /* The exclusive area SoAd_RoutingGroupStatus is used to avoid race condition when rg status flag is enabled / Disabled */
            SchM_Enter_SoAd_RoutingGroupStatus();

            /* check if Specific rg flag is enabled */
            if(lSoAdRoutingGroupSoConStructMap_pst->specificRgCheck_b == TRUE)
            {
                /* Loop for each SoConId's whcih are mapped to given Routing Group Id */
                for(lIdxSoCon_uo = 0; \
                        lIdxSoCon_uo < lSoAdRoutingGroupSoConStructMap_pst->numOfSoConIdMapped_uo; \
                        lIdxSoCon_uo++)
                {
                    /* Check for matched SoconId for the given Routing Group Id */
                    if(lSoAdRoutingGroupSoConStructMap_pst->routingGrpSoConMap_pst[lIdxSoCon_uo].mappedSoConId_uo == SoConId_uo)
                    {
                        /* Get the Specific routing Group status */
                        lRgStatus_b = lSoAdRoutingGroupSoConStructMap_pst->routingGrpSoConMap_pst[lIdxSoCon_uo].specificRgStatus_b;
                        break;
                    }
                }

                SchM_Exit_SoAd_RoutingGroupStatus();

                SOAD_DET_CHECK_ERROR_RETVAL( (lIdxSoCon_uo == lSoAdRoutingGroupSoConStructMap_pst->numOfSoConIdMapped_uo ) , SOAD_GET_ROUTING_GROUP_STATUS_API_ID, SOAD_E_INV_ARG, FALSE )
            }
            else
            {
                /* Get the Routing Group status */
                lRgStatus_b = lSoAdRoutingGroupSoConStructMap_pst->rgStatus_b;

                SchM_Exit_SoAd_RoutingGroupStatus();
            }

        }
    }


    return lRgStatus_b;
}

#endif /* SOAD_ROUTING_GROUPS_ENABLE != STD_OFF */

void              SoAd_ResetDynSockTblByEntry                   ( SoAd_SoConIdType           StaticSocketId_uo,\
                                                                  SoAd_SoConIdType           DynSocketId_uo, \
                                                                  SoAd_SoConModeType         Mode_en );

void              SoAd_ModeChgCallbackToUl                      ( SoAd_SoConIdType           StaticSocketId_uo,\
                                                                  SoAd_SoConModeType         Mode_en);

void              SoAd_ReserveFreeDynSocket                     ( SoAd_SoConIdType           StaticSocketId_uo,\
                                                                  SoAd_SoConIdType *         DynSocketId_puo);

#if (SOAD_TP_PRESENT != STD_OFF)
void              SoAd_InformTpStartOfReception                 ( SoAd_SoConIdType           StaticSocketId_uo,\
                                                                  PduIdType                  RxPduId_uo);

void              SoAd_TpTxConfirmationToUl                     ( SoAd_SoConIdType           StaticSocketId_uo,\
                                                                  PduIdType                  TxPduId_uo,\
                                                                  PduLengthType              PduLength_uo );

void              SoAd_ResetTpTxDyncParameters                  ( SoAd_SoConIdType           DynSocketId_uo,\
                                                                  PduIdType                  TxPduId_uo,\
                                                                  Std_ReturnType             Result_u8 );
#endif /* SOAD_TP_PRESENT != STD_OFF */

Std_ReturnType    SoAd_BestMatchAlgorithm                       ( SoAd_SoConIdType           SoConId_uo, \
                                                                  SoAd_SoConIdType           SoConGrpId_uo , \
                                                                  const TcpIp_SockAddrType * RemoteAddrPtr_cpst, \
                                                                  SoAd_SoConIdType *         SocketIdBestMatch_puo );
#if (SOAD_BEST_MATCH_ALG_RX_HEADERID_CHK == STD_ON)
Std_ReturnType SoAd_BestMatchAlgRxHeaderIdChk                   (SoAd_SoConIdType            SoConGrpId_uo , \
                                                                 const TcpIp_SockAddrType *  RemoteAddrPtr_cpst, \
                                                                 SoAd_SoConIdType *          SocketIdBestMatch_puo,
                                                                 uint8 **                    Buffer_ppcu8, \
                                                                 uint16 *                    Length_pu16);
#endif /* SOAD_BEST_MATCH_ALG_RX_HEADERID_CHK == STD_ON */

boolean           SoAd_IsMatchRxPduFound                         ( SoAd_SoConIdType          StaticSocketId_uo,\
                                                                   uint32                    RxPduHdrId_32 );

void              SoAd_PerformPostPduTransmissionAction          ( SoAd_SoConIdType          StaticSocketId_uo );

void              SoAd_InsertPduHdrToBuffer                      ( uint32                    PduHeaderId_u32,\
                                                                   uint32                    PduLength_u32,\
                                                                   uint8 *                   Buffer_pu8 );

void              SoAd_TerminateActiveSession                    ( SoAd_SoConIdType          DynSocketId_uo );

/* nPdu */
#if ( SOAD_NUM_nPDU_UDP_TX_SOCKET > 0 )
void              SoAd_ResetnPduUdpTx                            ( SoAd_SoConIdType             StaticSocketId_uo );

SoAd_IfTransmitReqStatus_ten SoAd_nPduUdpTransmit                ( SoAd_SoConIdType             StaticSocketId_uo, \
                                                                   PduLengthType                PduLength_uo, \
                                                                   const PduRouteDestType_tst * PduRouteDest_cpst );

void              SoAd_nPduUdpTxSendAllPdus                      ( SoAd_SoConIdType          StaticSocketId_uo );

BufReq_ReturnType SoAd_nPduUdpTxCopyPdusToTcpIpBuffer            ( SoAd_SoConIdType          StaticSocketId_uo,
                                                                   uint8 *                   Buffer_pu8,
                                                                   uint16                    BufferLength_u16 );
#endif /* SOAD_NUM_nPDU_UDP_TX_SOCKET > 0 */

#if ( (SOAD_TCP_SERVER_GRP_PRESENT != STD_OFF) || (SOAD_UDP_GRP_PRESENT != STD_OFF) )
boolean           SoAd_GetOtherActiveDynSocketForGroup           ( SoAd_SoConIdType          StaticSocketId_uo,\
                                                                   SoAd_SoConIdType *        DynSocketId_puo );
#endif /* (SOAD_TCP_SERVER_GRP_PRESENT != STD_OFF) || (SOAD_UDP_GRP_PRESENT != STD_OFF) */

#if (SOAD_NUM_TX_IF_TCP_PDU > 0)
void              SoAd_IfTcpTxConfirmationToUl                   ( SoAd_SoConIdType          DynSocketId_uo, \
                                                                   PduIdType                 TxPduId_uo, \
                                                                   uint16                    Length_u16, \
                                                                   boolean                   CloseReq_b);
#endif /* SOAD_NUM_TX_IF_TCP_PDU > 0 */


/* RxIndication */
Std_ReturnType    SoAd_SelectRxSocketRoute                       ( SoAd_SoConIdType          StaticSocketId_uo,
                                                                   uint32                    RxHeaderId_32,
                                                                   boolean                   HeaderEnableFlag_b,
                                                                   PduIdType *               RxPduId_puo );

/* RxBuffer */
#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
void              SoAd_ResetRxBuffer                             ( SoAd_SoConIdType          StaticSocketId_uo );

uint16            SoAd_GetRxBufLnrFreeLen                        ( SoAd_SoConIdType          StaticSocketId_uo );

Std_ReturnType    SoAd_AddFragmentedHeaderInRxBuffer             ( SoAd_SoConIdType          StaticSocketId_uo,
                                                                   const uint8 *             SrcPtr_cpu8,
                                                                   uint16                    SrcLen_u16,
                                                                   uint8 *                   CopiedLength_pu8 );

void              SoAd_GetFragmentedHeaderFromRxBuffer           ( SoAd_SoConIdType          StaticSocketId_uo,
                                                                   uint8 **                  PduHeaderPointer_ppu8 );

void              SoAd_ClearFragmentedHeaderFromRxBuffer         ( SoAd_SoConIdType          StaticSocketId_uo );

Std_ReturnType    SoAd_StartFragmentedDataInRxBuffer             ( SoAd_SoConIdType           StaticSocketId_uo,
                                                                   const SoAd_RxPduDesc_tst * RxPduDesc_cpst,
                                                                   boolean *                  HeaderSavedInBuff_pb );

Std_ReturnType    SoAd_AddFragmentedDataInRxBuffer               ( SoAd_SoConIdType          StaticSocketId_uo,
                                                                   const uint8 *             SrcPtr_cpu8,
                                                                   uint16                    SrcLen_u16,
                                                                   uint16 *                  MsgCopyLength_pu16);
/* BIP Porting TAO9SGH: This file has been edited for RTA-BSW 6.1.0 Defect-333112:
    Wrong Buffer pointer increment in SoAd_RxIndication for fragmented IF PDU on TCP with Rx buffer
*/

void              SoAd_GetFragmentedDataFromRxBuffer             ( SoAd_SoConIdType          StaticSocketId_uo,
                                                                   uint8**                   PduDataPointer_ppu8 );

uint16            SoAd_ClearFragmentedDataFromRxBuffer           ( SoAd_SoConIdType       StaticSocketId_uo,
                                                                   uint32 *               RemainingPduDataLength_pu32 );
#endif

#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TP_PRESENT != STD_OFF) )
void              SoAd_StartTpDataInRxBuffer                     ( SoAd_SoConIdType           StaticSocketId_uo,
                                                                   const SoAd_RxPduDesc_tst * RxPduDesc_cpst,
                                                                   uint16                      ForwardedDataLenToUl_u16 );

Std_ReturnType    SoAd_AddTpDataInRxBuffer                       ( SoAd_SoConIdType           StaticSocketId_uo,
                                                                   const uint8 *              SrcPtr_cpu8,
                                                                   uint16                     SrcLen_u16 );

Std_ReturnType    SoAd_AddExtraTpDataInRxBuffer                  ( SoAd_SoConIdType           StaticSocketId_uo,
                                                                   const SoAd_RxPduDesc_tst * RxPduDesc_cpst,
                                                                   const uint8 *              SrcPtr_cpu8,
                                                                   uint16                     SrcLen_u16 );

void              SoAd_CopyTpDataFromRxBufferToUl                ( SoAd_SoConIdType           StaticSocketId_uo,
                                                                   boolean                    TerminationRequested_b );
#endif


#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"

#endif /* SOAD_PRV_H */
