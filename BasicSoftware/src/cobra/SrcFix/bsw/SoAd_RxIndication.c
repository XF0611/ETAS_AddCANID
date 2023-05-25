/*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, 2011 Robert Bosch GmbH. All rights reserved.
* The reproduction, distribution and utilization of this document as well as the communication of its contents to
* others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
* All rights reserved in the event of the grant of a patent, utility model or design.
*
**********************************************************************************************************************
*/

/* BIP Porting TAO9SGH: This file has been edited for RTA-BSW 6.1.0 Defect-333112:
    Wrong Buffer pointer increment in SoAd_RxIndication for fragmented IF PDU on TCP with Rx buffer
*/

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "SoAd.h"
#ifdef SOAD_CONFIGURED

#include "TcpIp.h"

#include "SoAd_Cbk.h"
#include "SoAd_Prv_Domain.h"

#ifdef SOAD_DEBUG_AND_TEST
#include "TestSoAdApplication.h"
#endif
#if (!defined(TCPIP_AR_RELEASE_MAJOR_VERSION) || (TCPIP_AR_RELEASE_MAJOR_VERSION != SOAD_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched  - SoAd and TcpIp"
#endif

#if (!defined(TCPIP_AR_RELEASE_MINOR_VERSION) || (TCPIP_AR_RELEASE_MINOR_VERSION != SOAD_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched - SoAd and TcpIp"
#endif


/*
 ***************************************************************************************************
 * Static function declaration
 ***************************************************************************************************
 */

#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"

static Std_ReturnType           SoAd_IdentifySocketRouteSocket    ( TcpIp_SocketIdType         TcpIpSocketId_uo,
                                                                    const TcpIp_SockAddrType * RemoteAddrPtr_cpst,
#if (SOAD_BEST_MATCH_ALG_RX_HEADERID_CHK == STD_ON)
                                                                    uint8 **                   Buffer_ppu8,
                                                                    uint16 *                   Length_pu16,
#endif
                                                                    SoAd_SoConIdType *         StaticSocketId_puo );

static Std_ReturnType           SoAd_MsgAcceptancePolicy           ( SoAd_SoConIdType           StaticSocketId_uo,
                                                                     const TcpIp_SockAddrType * RemoteAddrPtr_cpst );

#if (SOAD_UDP_PRESENT != STD_OFF)
static boolean                  SoAd_MakeUdpSocketOnline           ( SoAd_SoConIdType           StaticSocketId_uo,
                                                                     const TcpIp_SockAddrType * RemoteAddrPtr_cpst );
#endif /* SOAD_UDP_PRESENT != STD_OFF */

static boolean                  SoAd_ProcessRcvdMessage            ( SoAd_SoConIdType StaticSocketId_uo,
                                                                     uint8 *          MessagePtr_pu8,
                                                                     uint16           MessageLength_u16 );

static uint16                   SoAd_DiscardPdu                    ( SoAd_SoConIdType StaticSocketId_uo,
                                                                     uint32           TotalPduDataLength_u32,
                                                                     uint16           RemainingLength_u16 );


static SoAd_RxPduAcceptance_ten SoAd_ConvertMessageToPdu           ( SoAd_SoConIdType     StaticSocketId_uo,
                                                                     const uint8 *        CurrentPointer_cpu8,
                                                                     uint16               RemainingLength_u16,
                                                                     SoAd_RxPduDesc_tst * RxPduDesc_pst,
                                                                     uint8 *              PresentHeaderLength_pu8 );

static SoAd_RxPduAcceptance_ten SoAd_ExtractPduHeader              ( SoAd_SoConIdType StaticSocketId_uo,
                                                                     const uint8 *    CurrentPointer_cpu8,
                                                                     uint16 *         RemainingAndPresentLength_pu16,
                                                                     uint32 *         PduHeaderId_pu32,
                                                                     uint32 *         PduDataLength_pu32 );

static SoAd_RxPduAcceptance_ten SoAd_ProcessPduData                ( SoAd_SoConIdType   StaticSocketId_uo,
                                                                     uint8 *            PduData_pu8,
                                                                     uint16             RemainingLength_u16,
                                                                     SoAd_RxPduDesc_tst RxPduDesc_st,
                                                                     uint16 *           CopiedDataToUl_pu16,
                                                                     uint16 *           MsgCopyLength_pu16);

#if (SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE != STD_OFF)
static SoAd_RxPduAcceptance_ten SoAd_CheckCompletePduDataReceived    ( uint16        MessageLength_u16 ,
                                                                       const uint8 * MessagePtr_pu8 );
#endif

#if ( SOAD_IF_PRESENT != STD_OFF )
static SoAd_RxPduAcceptance_ten SoAd_ProcessPduDataIF              (
                                                                     SoAd_SoConIdType   StaticSocketId_uo,
                                                                     uint16             RemainingLength_u16,
                                                                     uint8 *            PduData_pu8,
                                                                     SoAd_RxPduDesc_tst RxPduDesc_st,
                                                                     uint16 *           CopiedDataToUl_pu16,
                                                                     uint16 *           MsgCopyLength_pu16);
#endif

#if ( SOAD_TP_PRESENT != STD_OFF )
static SoAd_RxPduAcceptance_ten SoAd_ProcessPduDataTPHeaderDisabled ( SoAd_SoConIdType   StaticSocketId_uo,
                                                                      uint8 *            PduData_pu8,
                                                                      uint16             RemainingLength_u16,
                                                                      SoAd_RxPduDesc_tst RxPduDesc_st,
                                                                      uint16 *           CopiedDataToUl_pu16 );

static SoAd_RxPduAcceptance_ten SoAd_ProcessPduDataTPHeaderEnabled  ( SoAd_SoConIdType   StaticSocketId_uo,
                                                                      uint8 *            PduData_pu8,
                                                                      uint16             RemainingLength_u16,
                                                                      SoAd_RxPduDesc_tst RxPduDesc_st,
                                                                      uint16 *           CopiedDataToUl_pu16 );

static SoAd_RxPduAcceptance_ten SoAd_ProcessPduDataTPExecCopy       ( SoAd_SoConIdType   StaticSocketId_uo,
                                                                      uint8 *            PduData_pu8,
                                                                      SoAd_RxPduDesc_tst RxPduDesc_st,
                                                                      PduLengthType      InitialDataLenUpperLayer_uo,
                                                                      uint16 *           AvailableAndCopiedDataToUl_pu16 );
#endif

static void SoAd_PostActionAfterRxProcess       ( SoAd_SoConIdType StaticSocketId_uo);

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"


/*
 ***************************************************************************************************
 * Interface functions
 ***************************************************************************************************
 */

#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"

/*********************************************************************************************************************************/
/** SoAd_RxIndication()     - The TCP/IP stack calls this primitive after the reception of data on a socket.                    **/
/**                         The socket identifier along with configuration information determines which module is to be called. **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** SocketId                - TcpIp socket identifier of the related local socket resource                                      **/
/** RemoteAddrPtr           - Pointer to memory containing IP address and port of the remote host which sent the data           **/
/** BufPtr                  - Pointer to the received data                                                                      **/
/** Length                  - Data length of the received TCP segment or UDP datagram                                           **/
/**                                                                                                                             **/
/** Parameters (inout):     None                                                                                                **/
/**                                                                                                                             **/
/** Parameters (out):       None                                                                                                **/
/**                                                                                                                             **/
/** Return value:           None                                                                                                **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
void SoAd_RxIndication(  TcpIp_SocketIdType         SocketId,
                         const TcpIp_SockAddrType * RemoteAddrPtr,
                         uint8 *                    BufPtr,
                         uint16                     Length )
{
    /* Declare local variables */
    const SoAd_StaticSocConfigType_tst *      lSoAdStaticSocConfig_cpst;
    SoAd_DyncSocConfigType_tst *              lSoAdDyncSocConfig_pst;
    Std_ReturnType                            lRetVal_u8;
    SoAd_SoConIdType                          lIdxStaticSocket_uo;
    SoAd_SoConIdType                          lIdxDynSocket_uo;
    boolean                                   lRemoteAddressOverwritten_b;
    boolean                                   lDataForwrdedToUl_b;
    boolean                                   lIsReconnectActionReq_b;

    /* Variable to store the port for remote address */
    uint16           lRemoteAddrPort_u16;

    /* Variable to hold if remote IP address is null (all zeros) */
    boolean          lIsNullAddr_b;

    /* Variable to store index of the socket connection Group */
    SoAd_SoConIdType lIdxSoConGroup_uo;

#if ( SOAD_CONFIG_NUM_OF_UDP_SUPERVISION_TIMEOUT > 0U ) || (SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE != STD_OFF)
    const SoAd_StaticSoConGrpConfigType_tst * lSoAdStaticSoConGrpConfig_cpst;
    const SoAd_StaticUdpConfigType_tst *      lStaticUdpConfigType_pcst;
#endif

    /* Check if SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE is Enabled */
#if (SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE != STD_OFF)

    SoAd_RxPduAcceptance_ten                  lPduAcceptance_en;

    lPduAcceptance_en   = SOAD_PDU_OK;

#endif

    /* Initialization of local variables declared above */
    lRemoteAddressOverwritten_b = FALSE;

    lIsReconnectActionReq_b     = FALSE;

    /********************************* Start: DET Checking *************************************/

    /* Check whether SoAd Init function was already called or not */
    SOAD_DET_CHECK_ERROR_VOID( ( FALSE == SoAd_InitFlag_b ), SOAD_RX_INDICATION_API_ID, SOAD_E_NOTINIT )


    /* Check whether BufPtr & RemoteAddrPtr pointers are valid or not */
    SOAD_DET_CHECK_ERROR_VOID( ( ( NULL_PTR == BufPtr ) || ( NULL_PTR == RemoteAddrPtr )), SOAD_RX_INDICATION_API_ID, SOAD_E_PARAM_POINTER )


    /* MR12 RULE 11.3 VIOLATION: The cast is safe because the generic type is used by the caller function and extracts the info depending on the domain. */
    lRemoteAddrPort_u16 = SOAD_GET_PORT_FROM_SOCK_ADDR(RemoteAddrPtr);
    /* MR12 RULE 11.3 VIOLATION: The cast is safe because the generic type is used by the caller function and extracts the info depending on the domain. */
    lIsNullAddr_b       = SOAD_SOCK_ADDR_HAS_WILDCARD_IPADDR(RemoteAddrPtr);

    /* Get the static group socket index from static socket table */
    lIdxSoConGroup_uo = SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[SoAd_DyncSocConfig_ast[SoAd_TcpIpSocHandleToDynSockIdx_auo[SocketId]].idxStaticSoc_uo].soConGrpId_uo ;

    /* Check whether SocketId is valid or not */
    SOAD_DET_CHECK_ERROR_VOID( (SocketId >= SOAD_NUM_TCPIP_SOCKETS), SOAD_RX_INDICATION_API_ID, SOAD_E_INV_SOCKETID )

    /* Check whether Length & RemoteAddrPtr parameters are valid or not */
    SOAD_DET_CHECK_ERROR_VOID( ( ( 0U == Length )                ||
                            lIsNullAddr_b                       ||
                            ( SOAD_DEFAULT_PORT == lRemoteAddrPort_u16 )  ||
                            ( TCPIP_PORT_ANY == lRemoteAddrPort_u16 )  ||
                            ( !((RemoteAddrPtr->domain == TCPIP_AF_INET)||(RemoteAddrPtr->domain == TCPIP_AF_INET6))) ) , SOAD_RX_INDICATION_API_ID, SOAD_E_INV_ARG )

    /* Check whether RemoteAddr_cpst and connection domain are equal */
    SOAD_DET_CHECK_ERROR_VOID( ( SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lIdxSoConGroup_uo].domain_en != RemoteAddrPtr->domain ), SOAD_RX_INDICATION_API_ID, SOAD_E_INV_ARG )

    /********************************* End: DET Checking *************************************/

    /* Initialization local variables which are declared above */
    lIdxStaticSocket_uo = SOAD_CFG_SOCON_ID_DEFAULT_VALUE;
    lIdxDynSocket_uo    = SOAD_CFG_SOCON_ID_DEFAULT_VALUE;

    /* --------------------------------------------- */
    /* Identify related socket Route                 */
    /* --------------------------------------------- */

    /* Step (1) of [SWS_SoAd_00562] */
    /* For the reception of a message from an UDP or TCP socket and forwarding of the received data as PDU to the related upper layer the SoAd shall */
    /* (1) Identify the related socket connection and socket routes by using the SocketId provided at SoAd_RxIndication() */

    /* Get SoAd static socket from TcpIp SocketId provided at RxIndication */
    lRetVal_u8 = SoAd_IdentifySocketRouteSocket( SocketId,
                                                 RemoteAddrPtr,
    #if (SOAD_BEST_MATCH_ALG_RX_HEADERID_CHK == STD_ON)
                                                 &BufPtr,
                                                 &Length,
    #endif
                                                 &lIdxStaticSocket_uo );

    /* Continue processing only if the SoAd socket was successfully identified */
    /* (If E_OK is returned, that means that the static and dynamic socket index are valid) */
    if( E_OK == lRetVal_u8 )
    {
        /* Get static socket configuration pointers */
        lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[lIdxStaticSocket_uo]);

        /* Get dynamic socket from static socket */
        lIdxDynSocket_uo = (*lSoAdStaticSocConfig_cpst->SoAd_Prv_idxDynSocTable_puo);

        /* Copy the global array index address to local pointer for further processing */
        lSoAdDyncSocConfig_pst = &(SoAd_DyncSocConfig_ast[lIdxDynSocket_uo]);

        /* The exclusive area SoAd_TxRxReq is used to avoid race condition when there is Req for Tx/Rx or for discard Tx/Rx req */
        SchM_Enter_SoAd_TxRxReq();

        /* Increment counter for Rx actions on this socket */
        lSoAdDyncSocConfig_pst->rxOngoingCnt_u8++;

        SchM_Exit_SoAd_TxRxReq();

        /* --------------------------------------------- */
        /* Message Acceptance Policy                     */
        /* --------------------------------------------- */

        /* Step (2) of [SWS_SoAd_00562] */
        /* For the reception of a message from an UDP or TCP socket and forwarding of the received data as PDU to the related upper layer the SoAd shall */
        /* (2) Filter messages according to the message acceptance policy */

        /* Execute SoAd message acceptance policy */
        lRetVal_u8 = SoAd_MsgAcceptancePolicy( lIdxStaticSocket_uo,
                                               RemoteAddrPtr );

        /* Continue processing only if message is accepted */
        if(E_OK == lRetVal_u8 )
        {
    #if (SOAD_UDP_PRESENT != STD_OFF)
            /* --------------------------------------------- */
            /* Make UDP socket online                        */
            /* --------------------------------------------- */

            /* [SWS_SoAd_00592] */
            /* Within SoAd_RxIndication() and before analyzing or forwarding of any message data, SoAd shall */
            /* (a) overwrite the remote address parts specified with wildcards (e.g. remote IP address set to TCPIP_IPADDR_ANY) with the related source */
            /*     address parts of the received message and */
            /* (b) change the state of the socket connection to SOAD_SOCON_ONLINE in case all of the following conditions are true */

            /* Change the socket state to online when the conditions of [SWS_SoAd_00592] are verified */

            lRemoteAddressOverwritten_b = SoAd_MakeUdpSocketOnline( lIdxStaticSocket_uo,
                                                                    RemoteAddrPtr );

    #if ( SOAD_CONFIG_NUM_OF_UDP_SUPERVISION_TIMEOUT > 0U ) || (SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE != STD_OFF)

            /* Get static socket Group configuration pointers */
            lSoAdStaticSoConGrpConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo]);

            lStaticUdpConfigType_pcst = lSoAdStaticSoConGrpConfig_cpst->udpProtocolConf_cpst;
    #endif

    #if ( SOAD_CONFIG_NUM_OF_UDP_SUPERVISION_TIMEOUT > 0U )
            /* --------------------------------------------- */
            /* Udp Supervision timeout                       */
            /* --------------------------------------------- */
            /* [SWS_SoAd_00694] */
            /* If a UDP socket connection is configured with a SoAdSocketUdpAliveSupervisionTimeout and the remote address was overwritten, as described in [SWS_SoAd_00592] */
            /* and the alive supervision timer for this socket connection shall be started with the value specified by the configuration parameter SoAdSocketUdpAliveSupervisionTimeout */
            /* and every further reception from the same remote node shall reset the timer back to the initial value at SoAd_RxIndication()     */
            /* The exclusive area UdpSupervisionTimer is used to avoid race condition when SoAd_ULIfUdpTxConfirm flag is modified */
            SchM_Enter_SoAd_UdpSupervisionTimer();

            if( (NULL_PTR != lStaticUdpConfigType_pcst)                                                                          &&
                (SOAD_CFG_UDPALIVESUPERVISION_DEFAULT_VALUE != lStaticUdpConfigType_pcst->udpAliveSupervisionTimeoutIdx_uo)      &&
                ((TRUE == lRemoteAddressOverwritten_b)                                   ||
                 ((0xFFFFFFFFU > lSoAdDyncSocConfig_pst->UdpAliveSupervisionTimer_u32 )  &&
                  (0U < lSoAdDyncSocConfig_pst->UdpAliveSupervisionTimer_u32))) )
            {
                /* Load the configured UdpAliveSuperVision timeout value into the dynamic socket structure */
                lSoAdDyncSocConfig_pst->UdpAliveSupervisionTimer_u32 = \
                        SoAd_CurrConfig_cpst->SoAd_Config_UdpAliveSupervisionTimeOut_cpu32 \
                            [lStaticUdpConfigType_pcst->udpAliveSupervisionTimeoutIdx_uo];
            }

            SchM_Exit_SoAd_UdpSupervisionTimer();
    #endif
    #endif

            /* Check if SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE is Enabled */
    #if (SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE != STD_OFF)
            /* Condition to check If protocol is UDP and udpStrictHdrLenChkEnabled_b is TRUE  */
            if ( (TCPIP_IPPROTO_UDP == lSoAdStaticSoConGrpConfig_cpst->soConGrpProtocol_en ) && \
                 (TRUE == lStaticUdpConfigType_pcst->udpStrictHdrLenChkEnabled_b ) )
            {
                /* Check if the Message Length equals the accummulated PDU length */
                lPduAcceptance_en = SoAd_CheckCompletePduDataReceived( Length, BufPtr );
            }

            /* --------------------------------------------- */
            /* Process received message                      */
            /* --------------------------------------------- */

            /* condition to check if the Message Length does not equal the accummulated PDU length */
            lDataForwrdedToUl_b = (lPduAcceptance_en == SOAD_PDU_DISCARD ) ? \
                                    FALSE : \
                                    /* Process the received message further */
                                    SoAd_ProcessRcvdMessage( lIdxStaticSocket_uo, BufPtr,Length );

    #else
            /* --------------------------------------------- */
            /* Process received message                      */
            /* --------------------------------------------- */

            /* Process the received message further */
            lDataForwrdedToUl_b = SoAd_ProcessRcvdMessage( lIdxStaticSocket_uo, BufPtr,Length );

    #endif /* (SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE != STD_OFF) */

            /* --------------------------------------------- */
            /* Reconnect in case no valid PDU forwarded      */
            /* --------------------------------------------- */

            /* [SWS_SoAd_00710] */
            /* In case no valid PDU data was forwarded to an upper layer and the remote address of the socket connection was overwritten */
            /* according to [SWS_SoAd_00592] in context of the same SoAd_RxIndication(), SoAd shall revert the remote address change and set the state of */
            /* the socket connection back to SOAD_SOCON_RECONNECT. */

            /* If no data has been forwrded to the UL and reomte address was overwritten */
            if( ( FALSE == lDataForwrdedToUl_b ) && ( TRUE == lRemoteAddressOverwritten_b ) )
            {
                /* Set the flag to indicate that the socket connection has to be set back to SOAD_SOCON_RECONNECT*/
                lIsReconnectActionReq_b = TRUE;

    #if ( SOAD_CONFIG_NUM_OF_UDP_SUPERVISION_TIMEOUT > 0U )
                /* Reset the configured UdpAliveSuperVision timeout value into the dynamic socket structure */
                lSoAdDyncSocConfig_pst->UdpAliveSupervisionTimer_u32 = 0xFFFFFFFFU;
    #endif
            }

            /* If remote address has to be overwritten to configured remote address */
            if( (TRUE == lIsReconnectActionReq_b) || ( SOAD_EVENT_RECONNECTREQ_SOAD == lSoAdDyncSocConfig_pst->socConRequestedMode_en))
            {
                /*call function to perform reconnect request*/
                SoAd_PostActionAfterRxProcess(lIdxStaticSocket_uo);
            }
        }

        /* The exclusive area SoAd_TxRxReq is used to avoid race condition when there is Req for Tx/Rx or for discard Tx/Rx req */
        SchM_Enter_SoAd_TxRxReq();

        /* Decrement counter for Rx actions on this socket */
        lSoAdDyncSocConfig_pst->rxOngoingCnt_u8--;

        SchM_Exit_SoAd_TxRxReq();
    }

    return;
}

/**************************************************************************************************************************************/
/** SoAd_IdentifySocketRouteSocket() - This function shall determine the SoAd static socket index corresponding the the TcpIp socket **/
/**                                    id given in argument                                                                          **/
/**                                                                                                                                  **/
/** Parameters (in):                                                                                                                 **/
/** TcpIpSocketId_uo            - TcpIp socket id for which the SoAd socket should be found                                          **/
/** RemoteAddrPtr_cpst          - Pointer to memory containing IP address and port of the remote host which sent the data            **/
/** Buffer_cpu8                 - Rx buffer received from TcpIp                                                                      **/
/** Length_u16                  - Length of the Rx Buffer                                                                            **/
/**                                                                                                                                  **/
/** Parameters (inout):         None                                                                                                 **/
/**                                                                                                                                  **/
/** Parameters (out):                                                                                                                **/
/** StaticSocketId_puo          - Identified static socket id                                                                        **/
/**                                                                                                                                  **/
/** Return value:               - Std_ReturnType                                                                                     **/
/**                                 E_OK: SoAd socket has been identified                                                            **/
/**                                 E_NOT_OK: SoAd socket has not been identified                                                    **/
/**                                                                                                                                  **/
/**************************************************************************************************************************************/
static Std_ReturnType SoAd_IdentifySocketRouteSocket ( TcpIp_SocketIdType         TcpIpSocketId_uo,
                                                       const TcpIp_SockAddrType * RemoteAddrPtr_cpst,
#if (SOAD_BEST_MATCH_ALG_RX_HEADERID_CHK == STD_ON)
                                                       uint8 **                   Buffer_ppu8,
                                                       uint16 *                   Length_pu16,
#endif
                                                       SoAd_SoConIdType *         StaticSocketId_puo )
{
    /* Declare local variables */
    SoAd_SoConIdType lIdxDynSocket_uo;
    Std_ReturnType   lFunctionRetVal_en;

#if (SOAD_UDP_GRP_PRESENT != STD_OFF)
    SoAd_SoConIdType lIdxStaticSocket_uo;
    SoAd_SoConIdType lIdxSoConGroup_uo;
    boolean          lSocketMatchForUdpGrp_b;
#endif

    /* Initialize the return value of the function to E_NOT_OK */
    lFunctionRetVal_en      = E_NOT_OK;

    /* [SWS_SoAd_00562] */
    /* For the reception of a message from an UDP or TCP socket and forwarding of the received data as PDU to the related upper layer the SoAd shall */
    /* (1) Identify the related socket connection and socket routes by using the SocketId provided at SoAd_RxIndication() */

    /* Get the index of dynamic config table from the given SocketId by using array */
    lIdxDynSocket_uo = SoAd_TcpIpSocHandleToDynSockIdx_auo[TcpIpSocketId_uo];

    if( SoAd_CurrConfig_cpst->SoAd_noActiveSockets_uo > lIdxDynSocket_uo )
    {
#if (SOAD_UDP_GRP_PRESENT != STD_OFF)
        /* --------------------------------------------- */
        /* Case of Udp multi socket                      */
        /* --------------------------------------------- */

        /* Reset the lSocketMatchForUdpGrp_b to FALSE as default value */
        lSocketMatchForUdpGrp_b = FALSE;

        /* Condition Check: TcpIp socket is is fall in the UDP range, As per the TcpIp design TCP socket id will
         * be in the range of 0 to TCPIP_TCPSOCKETMAX abd UDP socket id will be in the range of TCPIP_TCPSOCKETMAX to TCPIP_SOCKETMAX */
#if(TCPIP_TCPSOCKETMAX > 0)
        if( TCPIP_TCPSOCKETMAX <= TcpIpSocketId_uo )
#endif
        {
            /* Get the static socket index from dynamic socket table */
            lIdxStaticSocket_uo = SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].idxStaticSoc_uo;

            /* Get the static group socket index from static socket table */
            lIdxSoConGroup_uo = SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[lIdxStaticSocket_uo].soConGrpId_uo ;

            /* Check if it is UDP Group socket, If so get the group id belongs to the static socket and apply best match algorithm
             * to get the best matched socket as per autosar
             */
            if( ( SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lIdxSoConGroup_uo].maxSoConChannel_uo > 1U ) &&  \
                ( TCPIP_IPPROTO_UDP == SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lIdxSoConGroup_uo].soConGrpProtocol_en ) )
            {
                /* Set the lSocketMatchForUdpGrp_b to TRUE to identify Tcpip socket id is belongs to UDP group socket */
                lSocketMatchForUdpGrp_b = TRUE;

#if (SOAD_BEST_MATCH_ALG_RX_HEADERID_CHK == STD_ON)

                /* [SWS_SoAd_00657] */
                /* In case more than one socket connection belongs to a UDP socket connection group, a UDP socket is shared between all socket */
                /* connections of the group and the related socket connection shall be selected according to the best match algorithm (see [SWS_SoAd_00680]). */

                /* Get the socket id with the help of best match algorithm */
                lFunctionRetVal_en = SoAd_BestMatchAlgRxHeaderIdChk(lIdxSoConGroup_uo,
                                                                    RemoteAddrPtr_cpst,
                                                                    StaticSocketId_puo,
                                                                    Buffer_ppu8,
                                                                    Length_pu16);
#else

                /* [SWS_SoAd_00657] */
                /* In case more than one socket connection belongs to a UDP socket connection group, a UDP socket is shared between all socket */
                /* connections of the group and the related socket connection shall be selected according to the best match algorithm (see [SWS_SoAd_00680]). */

                /* Get the socket id with the help of best match algorithm */
                lFunctionRetVal_en = SoAd_BestMatchAlgorithm( SOAD_CFG_SOCON_ID_DEFAULT_VALUE,
                                                              lIdxSoConGroup_uo,
                                                              RemoteAddrPtr_cpst,
                                                              StaticSocketId_puo );

#endif /* SOAD_BEST_MATCH_ALG_RX_HEADERID_CHK == STD_ON */
            }
        }

        if( FALSE == lSocketMatchForUdpGrp_b )
#endif /* end of (SOAD_UDP_GRP_PRESENT != STD_OFF) */
        {

            /* ------------------------------------------------------ */
            /* Case of Tcp client/server sockets or Udp single socket */
            /* ------------------------------------------------------ */

            /* The socket id given by TcpIp layer is not matched with Tcp Server sockets, Udp group sockets. */
            /* So, it can be a part of either Tcp client socket or Udp single socket. */

            /* Set the static socket index as out argument of the function */
            (*StaticSocketId_puo) = SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].idxStaticSoc_uo;

            /* SoAd socket is found so return E_OK */
            lFunctionRetVal_en = E_OK;
        }
    }
    (void)RemoteAddrPtr_cpst;
    return lFunctionRetVal_en;
}


/*********************************************************************************************************************************/
/** SoAd_MsgAcceptancePolicy()  - This function shall check whether received message (TCP connections or UDP datagrams) shall   **/
/**                             be accepted according to the messsage acceptance policy from remote nodes with an remote        **/
/**                             address that matches the remote address specified in the socket connection                      **/
/**                             id given in argument                                                                            **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo           - SoAd static socket index                                                                      **/
/** RemoteAddrPtr_pst           - Pointer to memory containing IP address and port of the remote host which sent the data       **/
/**                                                                                                                             **/
/** Parameters (inout):         None                                                                                            **/
/**                                                                                                                             **/
/** Parameters (out):           None                                                                                            **/
/**                                                                                                                             **/
/** Return value:               - Std_ReturnType                                                                                **/
/**                                 E_OK: Received message is accepted according to the message acceptance policy               **/
/**                                 E_NOT_OK: Received message isn't accepted according to the message acceptance policy        **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
static Std_ReturnType SoAd_MsgAcceptancePolicy ( SoAd_SoConIdType           StaticSocketId_uo,
                                                 const TcpIp_SockAddrType * RemoteAddrPtr_cpst )
{
    /* Declare local variables */
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    SoAd_DyncSocConfigType_tst *         lSoAdDyncSocConfig_pst;
    SoAd_SoConIdType                     lIdxDynSocket_uo;
    Std_ReturnType                       lFunctionRetVal_en;

    /* Variable to store if two IP addresses are equal */
    boolean                              lAreIpAddrEqual_b;

    /* Variable to store if the remote address of dynamic socket is wildcard (ANY)  */
    boolean                              lDyncSocConfigHasWildcardAnyAddr_b;

    /* Variable to store the port for dynamic socket */
    uint16                               lDyncSocPort_u16;

    /* Variable to store the port for remote address */
    uint16                               lRemoteAddrPort_u16;


    /* Initialize the return value of the function to E_NOT_OK */
    lFunctionRetVal_en = E_NOT_OK;

    /* Get static socket configuration pointers */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);

    /* Get dynamic socket from static socket */
    lIdxDynSocket_uo = (*lSoAdStaticSocConfig_cpst->SoAd_Prv_idxDynSocTable_puo);

    /* Get dynamic socket configuration pointers */
    lSoAdDyncSocConfig_pst = &(SoAd_DyncSocConfig_ast[lIdxDynSocket_uo]);

    /* [SWS_SoAd_00524] */
    /* If SoAdSocketMsgAcceptanceFilterEnabled is TRUE, SoAd shall only accept TCP connections or UDP datagrams from remote nodes with a source address that */
    /* matches the remote address specified in the socket connection (either via configuration parameters SoAdSocketRemoteIpAddress and SoAdSocketRemotePort */
    /* or set online with SoAd_SetRemoteAddr() API). */

    /* -------------------- */
    /* Udp socket           */
    /* -------------------- */

    /* Socket Group Protocol is UDP */
    if ( SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_UDP )
    {
        /* Message acceptance policy is done on reception of UDP message */
#if (SOAD_MSG_ACCEPT_POLICY_ENABLED != STD_OFF)
        /* Check if the MsgAcceptedFilter is TRUE for static group id */
        if  (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].socMsgAcceptanceFiletEnable_b == TRUE )
        {
            /* [SWS_SoAd_00525] */
            /* A remote address matches if both IP address and port match. The IP addresses match if they are identical or if the specified IP address is set to */
            /* TCPIP_IPADDR_ANY (TCPIP_IP6ADDR_ANY). The port matches if they are identical or if the specified port is set to TCPIP_PORT_ANY. */
            /* MR12 RULE 11.3 VIOLATION: The cast is safe because the generic type is used by the caller function and extracts the info depending on the domain. */
            lRemoteAddrPort_u16                = SOAD_GET_PORT_FROM_SOCK_ADDR(RemoteAddrPtr_cpst);

            /* The exclusive area RemoteAddrUpdate is used to avoid race condition when remote address is read */
            SchM_Enter_SoAd_RemoteAddrUpdate();

            /* MR12 RULE 11.3 VIOLATION: The cast is safe because the generic type is used by the caller function and extracts the info depending on the domain. */
            lAreIpAddrEqual_b                  = SOAD_SOCK_IPADDR_IS_EQUAL_TO_DYNC_SOC_CONFIG_REMOTE_IPADDR(RemoteAddrPtr_cpst, lSoAdDyncSocConfig_pst);
            lDyncSocConfigHasWildcardAnyAddr_b = SOAD_DYNC_SOC_CONFIG_HAS_WILDCARD_REMOTE_IPADDR(lSoAdDyncSocConfig_pst);
            lDyncSocPort_u16                   = SOAD_GET_REMOTE_PORT_FROM_DYNC_SOC_CONFIG(lSoAdDyncSocConfig_pst);

            SchM_Exit_SoAd_RemoteAddrUpdate();

            /* When remote address equal to wildcard or message remote address */
            if ( ( lAreIpAddrEqual_b || lDyncSocConfigHasWildcardAnyAddr_b) &&
                 ( ( lRemoteAddrPort_u16 == lDyncSocPort_u16)   ||
                   ( TCPIP_PORT_ANY == lDyncSocPort_u16 ) ) )
            {
                /* Set the return value of the function to E_OK */
                lFunctionRetVal_en = E_OK;
            }
        }
        /* Message acceptance filter is disabled */
        else
#endif
        {
            /* [SWS_SoAd_00635] */
            /* If SoAdSocketMsgAcceptanceFilterEnabled is FALSE, SoAd shall accept all UDP datagrams from remote nodes. */

            /* Set the return value of the function to E_OK */
            lFunctionRetVal_en = E_OK;
        }
    }

    /* -------------------- */
    /* Tcp socket           */
    /* -------------------- */

    /* Socket Group Protocol is TCP */
    else
    {
        /* Message acceptance policy does not apply on the reception of Tcp data segments */
        lFunctionRetVal_en = E_OK;
    }

#ifdef SOAD_DEBUG_AND_TEST
    SoAd_MsgAccept_tst = (uint8)lFunctionRetVal_en;
#endif
    (void)RemoteAddrPtr_cpst;
    return lFunctionRetVal_en;
}


/*********************************************************************************************************************************/
/** SoAd_MakeUdpSocketOnline()  - This function is used to put the Udp socket online according to SWS_SoAd_00592                **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo           - SoAd static socket index                                                                      **/
/** RemoteAddrPtr_cpst          - Pointer to memory containing IP address and port of the remote host which sent the data       **/
/**                                                                                                                             **/
/** Parameters (inout):         None                                                                                            **/
/**                                                                                                                             **/
/** Parameters (out):           None                                                                                            **/
/**                                                                                                                             **/
/** Return value:               - Boolean                                                                                       **/
/**                                 TRUE: Remote address of the socket has been overwritten                                     **/
/**                                 FALSE: Remote address of the socket has not been overwritten                                **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
#if (SOAD_UDP_PRESENT != STD_OFF)
static boolean SoAd_MakeUdpSocketOnline ( SoAd_SoConIdType           StaticSocketId_uo,
                                          const TcpIp_SockAddrType * RemoteAddrPtr_cpst )
{

    /* Declare local variables */
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    SoAd_DyncSocConfigType_tst *         lSoAdDyncSocConfig_pst;
    SoAd_SoConIdType                     lIdxDynSocket_uo;
    boolean                              lRemoteAddressOverwritten_b;
    boolean                              lDyncSocConfigHasWildcardAnyAddr_b;
    uint16                               lDyncSocPort_u16;
#if (SOAD_TP_PRESENT != STD_OFF)
    PduIdType                            lIdxRxPdu_uo;
#endif

    /* Get static socket configuration pointers */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);

    /* Get dynamic socket from static socket */
    lIdxDynSocket_uo = (*lSoAdStaticSocConfig_cpst->SoAd_Prv_idxDynSocTable_puo);

    /* Get dynamic socket configuration pointers */
    lSoAdDyncSocConfig_pst = &(SoAd_DyncSocConfig_ast[lIdxDynSocket_uo]);

    /* Initialize the flag indicating if the remote address has been overwritten to FALSE */
    lRemoteAddressOverwritten_b = FALSE;

    /* The exclusive area RemoteAddrUpdate is used to avoid race condition when remote address is read */
    SchM_Enter_SoAd_RemoteAddrUpdate();

    /* Read remote address in dynamic socket */
    lDyncSocConfigHasWildcardAnyAddr_b = SOAD_DYNC_SOC_CONFIG_HAS_WILDCARD_REMOTE_IPADDR(lSoAdDyncSocConfig_pst);
    lDyncSocPort_u16                   = SOAD_GET_REMOTE_PORT_FROM_DYNC_SOC_CONFIG(lSoAdDyncSocConfig_pst);

    /* --------------------------------------------- */
    /* Modify remote address                         */
    /* --------------------------------------------- */

    /* [SWS_SoAd_00592] */
    /* Within SoAd_RxIndication() and before analyzing or forwarding of any message data, SoAd shall */
    /* (a) overwrite the remote address parts specified with wildcards (e.g. remote IP address set to TCPIP_IPADDR_ANY) with the related source */
    /*     address parts of the received message and */

    /* Modify the Remote Ip address, if remote Ip address contains wildcard */
    /* or if the bypass for message acceptance filter FALSE is enabled - Deviation of SWS_SoAd_00592 */
    if ( (TRUE == lDyncSocConfigHasWildcardAnyAddr_b)
#if (SOAD_BYPASS_MSGACCEPTFILTER_POLICY == STD_ON)
          ||
          (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].bypassMsgAcceptanceFilterPolicy_b != FALSE)
#endif
        )
    {
        /* MR12 RULE 11.3 VIOLATION: The cast is safe because the generic type is used by the caller function and extracts the info depending on the domain. */
        SoAd_CopyFromSockAddrToDyncSocConfigRemoteIpAddr(RemoteAddrPtr_cpst, lSoAdDyncSocConfig_pst);

        /* Set a flag to remember that remote address has been overwritten in the dynamic socket */
        lRemoteAddressOverwritten_b = TRUE;
    }

    /* Modify the Remote port address, if remote port address contains wildcard */
    /* or if the bypass for message acceptance filter FALSE is enabled - Deviation of SWS_SoAd_00592 */
    if ( (TCPIP_PORT_ANY == lDyncSocPort_u16)
#if (SOAD_BYPASS_MSGACCEPTFILTER_POLICY == STD_ON)
          ||
          (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].bypassMsgAcceptanceFilterPolicy_b != FALSE)
#endif
       )
    {
        /* MR12 RULE 11.3 VIOLATION: The cast is safe because the generic type is used by the caller function and extracts the info depending on the domain. */
        SoAd_CopyFromSockAddrToDyncSocConfigRemotePort(RemoteAddrPtr_cpst, lSoAdDyncSocConfig_pst);

        /* Set a flag to remember that remote address has been overwritten in the dynamic socket */
        lRemoteAddressOverwritten_b = TRUE;
    }
    SchM_Exit_SoAd_RemoteAddrUpdate();
    /* --------------------------------------------- */
    /* Change socket state                           */
    /* --------------------------------------------- */

    /* [SWS_SoAd_00592] */
    /* Within SoAd_RxIndication() and before analyzing or forwarding of any message data, SoAd shall */
    /* (b) change the state of the socket connection to SOAD_SOCON_ONLINE */
    /* in case all of the following conditions are true */
    /* (1) Current connection state is not SOAD_SOCON_ONLINE */
    /* (2) UDP socket */
    /* (3) SoAdSocketUdpListenOnly is set to FALSE (Socket state goes to online even if SoAdSocketUdpListenOnly is True to avoid packet drop in below cases:*/
    /*     SoAd_SetRemoteAddr( ) with Wildcard address - Socket shall goes to RECONNECT state*/
    /*     SoAd_RxIndication( ) for the same socket - Packet could be dropped since socket is not ONLINE) */
    /* (4) SoAdSocketMsgAcceptanceFilterEnabled is set to TRUE (state goes online event if SoAdSocketMsgAcceptanceFilterEnabled is False when the bypass is enabled - Deviation of SWS_SoAd_00592) */
    /* (5) Remote address is set, but contains wildcards */
    /* (6) Received message is accepted according to the message acceptance policy */

    /* The exclusive area SocConModeProperties is used to avoid race condition when the soccket mode and submode are updated */
    SchM_Enter_SoAd_SocConModeProperties();

    if( (( SOAD_SOCON_RECONNECT == lSoAdDyncSocConfig_pst->socConMode_en ) && ( SOAD_SOCON_BOUND == lSoAdDyncSocConfig_pst->socConSubMode_en ))             &&
        (TCPIP_IPPROTO_UDP == SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en)           &&
        ((TRUE == SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].socMsgAcceptanceFiletEnable_b)
#if (SOAD_BYPASS_MSGACCEPTFILTER_POLICY == STD_ON)
         ||
         (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].bypassMsgAcceptanceFilterPolicy_b != FALSE)
#endif
        ) &&
        ((TRUE == lDyncSocConfigHasWildcardAnyAddr_b) || (TCPIP_PORT_ANY == lDyncSocPort_u16)) )
    {
        /* Change the state of the socket connection to SOAD_SOCON_ACTIVE */
        lSoAdDyncSocConfig_pst->socConSubMode_en = SOAD_SOCON_ACTIVE;

        lSoAdDyncSocConfig_pst->socConMode_en = SOAD_SOCON_ONLINE;

        SchM_Exit_SoAd_SocConModeProperties();

        /* Call a function which shall invoke the upper layer call back for mode change */
        SoAd_ModeChgCallbackToUl( StaticSocketId_uo, SOAD_SOCON_ONLINE );

#if (SOAD_TP_PRESENT != STD_OFF)
        /* If header is not enabled for the socket connection group, call the UL startOfReception as per SWS_SoAd_00595*/
        /* start of reception shall be called for upper layer of a socket only if the type is TP */
        if( (FALSE == SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].soConGrpId_uo].headerEnable_b) && \
            ( 1U == SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].numberOfRxPdusConfigured_u8 ) )
        {
            /* get the first RxPduId mapped to socket */
            lIdxRxPdu_uo = SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].rxPduIds_pcuo[0];

            if( SOAD_TP == SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[lIdxRxPdu_uo].rxUpperLayerType_en )
            {
                /* call start of reception related to all Rx pdus*/
                SoAd_InformTpStartOfReception(StaticSocketId_uo, lIdxRxPdu_uo);
            }
        }
#endif
    }
    else
    {
        SchM_Exit_SoAd_SocConModeProperties();
    }

    return lRemoteAddressOverwritten_b;
}
#endif /* SOAD_UDP_PRESENT != STD_OFF */


/*********************************************************************************************************************************/
/** SoAd_ProcessRcvdMessage()   - This function execute the processing of a received message (loop through all Pdus packed in   **/
/**                             the received message)                                                                           **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo           - SoAd static socket index                                                                      **/
/** MessagePtr_pu8              - Pointer to beginning of the received message                                                  **/
/** MessageLength_u16           - Length in bytes of the received message                                                       **/
/**                                                                                                                             **/
/** Parameters (inout):         None                                                                                            **/
/**                                                                                                                             **/
/** Parameters (out):           None                                                                                            **/
/**                                                                                                                             **/
/** Return value:               - Boolean                                                                                       **/
/**                                 TRUE: All data or a part of the data have been forwarded to the Upper Layer                 **/
/**                                 FALSE: No data have been Forwarded to the Upper Layer                                       **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
static boolean SoAd_ProcessRcvdMessage ( SoAd_SoConIdType StaticSocketId_uo,
                                         uint8 *          MessagePtr_pu8,
                                         uint16           MessageLength_u16 )
{
    /* Declare local variables */
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    SoAd_DyncSocConfigType_tst *         lSoAdDyncSocConfig_pst;
    SoAd_SoConIdType                     lIdxDynSocket_uo;
    SoAd_RxPduAcceptance_ten             lPduAcceptance_en;
    SoAd_RxPduDesc_tst                   lRxPduDesc_st;
    uint8 *                              lMessageCurrentPointer_pu8;
    uint16                               lMessageParsedLength_u16;
    uint16                               lDiscardedLength_u16;
    uint8                                lPresentHeaderLength_u8;
    uint16                               lCopiedDataToUl_u16;
    uint16                               lMsgCopyLength_u16;
    boolean                              lDataForwrdedToUl_b;

    /* Initialization of local variables declared above */
    /* Get static socket configuration pointers */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);

    /* Get dynamic socket from static socket */
    lIdxDynSocket_uo = (*lSoAdStaticSocConfig_cpst->SoAd_Prv_idxDynSocTable_puo);

    /* Get dynamic socket configuration pointers */
    lSoAdDyncSocConfig_pst = &(SoAd_DyncSocConfig_ast[lIdxDynSocket_uo]);

    /* Initialize the flag indicating of data have been forwarded to the Upper Layer to FALSE */
    lDataForwrdedToUl_b = FALSE;

    /* --------------------------------------------- */
    /* Initialize message pointer and parsed length  */
    /* --------------------------------------------- */

    /* Initialize variables before processing Pdu packed inside the message */
    lMessageCurrentPointer_pu8 = MessagePtr_pu8;
    lMessageParsedLength_u16 = 0U;

    /* --------------------------------------------- */
    /* Remaining length to be discarded              */
    /* --------------------------------------------- */
#if (SOAD_TCP_PRESENT != STD_OFF)
    /* Check if upper layer type is TCP */
    if( (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_TCP) &&
        (lSoAdDyncSocConfig_pst->rxTcpRemainingPduLengthToDicard_u32 > 0U) )
    {
        /* Discard if there is remaining length to be discarded from a previous Pdu */
        lDiscardedLength_u16 = SoAd_DiscardPdu( StaticSocketId_uo,
                                                lSoAdDyncSocConfig_pst->rxTcpRemainingPduLengthToDicard_u32,
                                                MessageLength_u16 );

        /* Increment message pointer by the discarded length */
        lMessageCurrentPointer_pu8 += lDiscardedLength_u16;
        lMessageParsedLength_u16 += lDiscardedLength_u16;
    }
#endif

     /* --------------------------------------------- */
    /* Process PDUs within the message               */
    /* --------------------------------------------- */
    /* While there are Pdus to process inside the message */
    while( lMessageParsedLength_u16 < MessageLength_u16 )
    {
        /* --------------------------------------------- */
        /* Initialize Rx Pdu desc                        */
        /* --------------------------------------------- */
        lRxPduDesc_st.PduDataLength_u32 = (uint32)( MessageLength_u16 - lMessageParsedLength_u16 );
        lRxPduDesc_st.RxPduId_uo = 0U;


        /* --------------------------------------------- */
        /* Initialize loop variables                     */
        /* --------------------------------------------- */
        lPduAcceptance_en = SOAD_PDU_DISCARD;
        lPresentHeaderLength_u8 = 0U;
        lCopiedDataToUl_u16 = 0U;
        lMsgCopyLength_u16 = 0U;

        /* --------------------------------------------- */
        /* Convert message into Pdu                      */
        /* --------------------------------------------- */

        /* Step (3) of [SWS_SoAd_00562] */
        /* For the reception of a message from an UDP or TCP socket and forwarding of the received data as PDU to the related upper layer the SoAd shall */
        /* (3) Convert the message into a PDU */

        /* Call a function to extract the current Pdu */
        /* (if Pdu header is present and completely processed by SoAd (header not fragmented and not saved in buffer), then processed length will be set to SOAD_PDUHDR_SIZE) */
        lPduAcceptance_en = SoAd_ConvertMessageToPdu( StaticSocketId_uo,
                                                      lMessageCurrentPointer_pu8,
                                                      (MessageLength_u16 - lMessageParsedLength_u16),
                                                      &lRxPduDesc_st,
                                                      &lPresentHeaderLength_u8 );

        /* Set the current pointer after the present Pdu header length */
        lMessageCurrentPointer_pu8 += lPresentHeaderLength_u8;
        lMessageParsedLength_u16 += lPresentHeaderLength_u8;

        /* If the Pdu is accepted for further processing (complete Pdu data is present or Pdu data is fragmented) */
        if( (lPduAcceptance_en == SOAD_PDU_OK) || (lPduAcceptance_en == SOAD_PDU_FRAGMENTED_DATA) )
        {
            /* --------------------------------------------- */
            /* Check socket state                            */
            /* --------------------------------------------- */

            /* If the socket is active and not requested to be closed (by UL or SoAd) */
            if( (lSoAdDyncSocConfig_pst->socConSubMode_en == SOAD_SOCON_ACTIVE) &&
                (lSoAdDyncSocConfig_pst->discardNewTxRxReq_b == FALSE) )
            {
                /* --------------------------------------------- */
                /* Check if Pdu length is valid                  */
                /* --------------------------------------------- */

                /* Step (4) of [SWS_SoAd_00562] */
                /* For the reception of a message from an UDP or TCP socket and forwarding of the received data as PDU to the related upper layer the SoAd shall */
                /* (4) Skip further processing if PDU length is 0 and (SoAdPduHeaderEnable is FALSE or SoAdRxUpperLayerType is TP) */

                /* Check if the Pdu length is invalid */
                if ( (lRxPduDesc_st.PduDataLength_u32 == 0U)    && \
                     ((SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].headerEnable_b == FALSE) || \
                      (SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[lRxPduDesc_st.RxPduId_uo].rxUpperLayerType_en == SOAD_TP)) )
                {
                    /* Raise DET Error */
                    /* To notify about Length error in PDU */
                    SOAD_DET_REPORT_ERROR(SOAD_RX_INDICATION_API_ID, SOAD_E_INV_ARG )

                    /* Discard Pdu when length does not pass */
                    lPduAcceptance_en = SOAD_PDU_DISCARD;
                }

                /* If the Pdu length is valid */
                else
                {
                    /* --------------------------------------------- */
                    /* Process Pdu data (complete or fragment)       */
                    /* --------------------------------------------- */

                    /* Step (5) of [SWS_SoAd_00562] */
                    /* For the reception of a message from an UDP or TCP socket and forwarding of the received data as PDU to the related upper layer the SoAd shall */
                    /* (5) Call the upper layer type related reception functions of the configured upper layer module depending on the SoAdRxUpperLayerType specified */
                    /*     in SocketRouteDest configuration */

                    /* Execute the Pdu data processing (IF / TP) */
                    lPduAcceptance_en = SoAd_ProcessPduData( StaticSocketId_uo,
                                                             lMessageCurrentPointer_pu8,
                                                             (MessageLength_u16 - lMessageParsedLength_u16),
                                                             lRxPduDesc_st,
                                                             &lCopiedDataToUl_u16,
                                                             &lMsgCopyLength_u16);
                }
            }
            else
            {
                /* Discard Pdu when close is requested */
                lPduAcceptance_en = SOAD_PDU_DISCARD;
            }
        }


        /* --------------------------------------------- */
        /* Check Pdu acceptance                          */
        /* --------------------------------------------- */
        switch(lPduAcceptance_en)
        {
            /* In case the current Pdu was fully present or Pdu data fragmented */
            case SOAD_PDU_OK:
            case SOAD_PDU_FRAGMENTED_DATA:
            {
                /* If some data have been forwarded to the Upper Layer */
                if( lCopiedDataToUl_u16 > 0U )
                {
                    /* Set a flag to remember that some data have been forwrded to the UL */
                    lDataForwrdedToUl_b = TRUE;

#if (SOAD_TCP_PRESENT != STD_OFF)
                    /* --------------------------------------------- */
                    /* Confirm TCP reception (UL data)               */
                    /* --------------------------------------------- */

                    /* [SWS_SoAd_00564] */
                    /* In case of a TCP socket connection the SoAd shall confirm the reception of all data either forwarded to the upper layer or finally handled */
                    /* by SoAd (e.g. discarded data or processed PDU Header) by calling TcpIp_TcpReceived() within SoAd_RxIndication() or SoAd_MainFunction() respectively. */

                    /* Check if upper layer type is TCP and if it secure connection if tls is enabled */
                    /* For secure connection , the  TcpIp_TcpReceived will called by TLS module itself */
                    if(( SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_TCP )
#if(SOAD_TCP_TLS_ENABLED != STD_OFF)
                            && ( SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].isSecureConnection_b == FALSE)
#endif/* SOAD_TCP_TLS_ENABLED != STD_OFF */
                       )
                    {
                        /* The reception of socket data is confirmed to the TCP/IP stack */
                        (void)TcpIp_TcpReceived( SoAd_DyncSocConfig_ast[(*lSoAdStaticSocConfig_cpst->SoAd_Prv_idxDynSocTable_puo)].socketHandleTcpIp_uo,
                                                 lCopiedDataToUl_u16 );
                    }
#endif
                }

                /* Increment the current pointer after the current Pdu data to go to the next Pdu in the message */
                /*MR12 RULE 14.3 VIOLATION: The Value of the conditional expression is always false and its not a critical issue  */
                lMessageCurrentPointer_pu8 += lMsgCopyLength_u16;

                /*MR12 RULE 14.3 VIOLATION: The Value of the conditional expression is always false and its not a critical issue  */
                lMessageParsedLength_u16 +=  lMsgCopyLength_u16;

            }
            break;

            /* In case the current Pdu header is fragmented */
            case SOAD_PDU_FRAGMENTED_HEADER:
            {
                /* Nothing to do */
                /* (message pointer and message parsed length already incremented) */
            }
            break;
#if (SOAD_REST_UDP_MSG_DISCARD_BY_INVALID_HEADERID == STD_ON)

            case SOAD_PDU_DISCARD_REST_MSG:
            {
                /* Request the discarding of the Pdu */
                lDiscardedLength_u16 = SoAd_DiscardPdu( StaticSocketId_uo,
                                                        (MessageLength_u16 - lMessageParsedLength_u16),
                                                        (MessageLength_u16 - lMessageParsedLength_u16) );

                /* Increment the current pointer after the discarded length */
                lMessageCurrentPointer_pu8 += lDiscardedLength_u16;
                lMessageParsedLength_u16 += lDiscardedLength_u16;

#ifdef SOAD_DEBUG_AND_TEST
                SoAd_ProcessRcvdMessage_discarded_len_tst += lDiscardedLength_u16;
#endif
            }
            break;
#endif /* SOAD_REST_UDP_MSG_DISCARD_BY_INVALID_HEADERID == STD_ON */
            /* In case the current Pdu is discarded */
            case SOAD_PDU_DISCARD:
            default:
            {
                /* Request the discarding of the Pdu */
                lDiscardedLength_u16 = SoAd_DiscardPdu( StaticSocketId_uo,
                                                        lRxPduDesc_st.PduDataLength_u32,
                                                        (MessageLength_u16 - lMessageParsedLength_u16) );

                /* Increment the current pointer after the discarded length */
                lMessageCurrentPointer_pu8 += lDiscardedLength_u16;
                lMessageParsedLength_u16 += lDiscardedLength_u16;

#ifdef SOAD_DEBUG_AND_TEST
                SoAd_ProcessRcvdMessage_discarded_len_tst += lDiscardedLength_u16;
#endif
            }
            break;
        }
#ifdef SOAD_DEBUG_AND_TEST
        SoAd_ProcessRcvdMessage_iter_cnt_tst = SoAd_ProcessRcvdMessage_iter_cnt_tst + 1;
#endif
    }

    return lDataForwrdedToUl_b;
}

#if (SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE != STD_OFF)

/*************************************************************************************************************************************/
/** SoAd_CheckCompletePduDataReceived() - This function checks if the received message is valid or not, i.e if the received message **/
/**                                       length equals the total accumulated length of all the PDU's.                              **/
/** Parameters (in):                                                                                                                **/
/** MessageLength_u16               - Length in bytes of the received message                                                       **/
/** CurrentPointer_pu8              - Pointer to the received message from which it is needed to extract Pdu                        **/
/** Parameters (inout):             None                                                                                            **/
/**                                                                                                                                 **/
/** Parameters (out):               None                                                                                            **/
/**                                                                                                                                 **/
/** Return value:                   SoAd_RxPduAcceptance_ten                                                                        **/
/**                                 SOAD_PDU_OK: The Pdu is valid for further processing                                            **/
/**                                 SOAD_PDU_FRAGMENTED_HEADER: Received Pdu header is fragmented                                   **/
/**                                 SOAD_PDU_DISCARD: The Pdu need to be discarded                                                  **/
/**                                                                                                                                 **/
/*************************************************************************************************************************************/
static SoAd_RxPduAcceptance_ten SoAd_CheckCompletePduDataReceived ( uint16 MessageLength_u16, \
                                                                    const uint8 * MessagePtr_pu8
                                                                  )
{
    /* Declare local variables */
    uint32                   lPduLength_u32;

    uint32                   lAccumulatedPduLength_u32;

    const uint8 *            lMessageCurrentPointer_pu8;

    SoAd_RxPduAcceptance_ten lPduAcceptance_en;

    /* Initialize the lPduAcceptance_en to SOAD_PDU_DISCARD */
    lPduAcceptance_en = SOAD_PDU_DISCARD;

    lMessageCurrentPointer_pu8 = MessagePtr_pu8;

    /* Initialize the accumulated PDU length to 0 */
    lAccumulatedPduLength_u32 = 0U;

    /* Loop to check if the length of the received message equals the length of the accumulated PDU length */
    while( lAccumulatedPduLength_u32 < MessageLength_u16 )
    {
        /* Reading Pdu length from Ethernet header which shall be used for finding out Pdu length*/
        /*converting the data from Big Endian to controller specific Endianness*/
        lPduLength_u32 = (uint32)(lMessageCurrentPointer_pu8[7]) | (((uint32)lMessageCurrentPointer_pu8[6])<<8U) |
                                (((uint32)lMessageCurrentPointer_pu8[5])<<16U) | (((uint32)lMessageCurrentPointer_pu8[4])<<24U);

        /* Total length will be the PDU length + the PDU Header Size */
        lAccumulatedPduLength_u32 += lPduLength_u32 + SOAD_PDUHDR_SIZE;

        /* Updating the Current position of the Current pointer */
        lMessageCurrentPointer_pu8 += lPduLength_u32 + SOAD_PDUHDR_SIZE;
    }

    /* Checking  if the accumulated PDU length equals the received Message Length, i.e if it is a valid PDU */
    if(MessageLength_u16 == lAccumulatedPduLength_u32)
    {
        lPduAcceptance_en = SOAD_PDU_OK;
    }

    return lPduAcceptance_en;
}

#endif /* (SOAD_STRICT_UDP_HDR_LEN_CHECK_ENABLE != STD_OFF) */

/*********************************************************************************************************************************/
/** SoAd_DiscardPdu()           - This function discard a Pdu                                                                   **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo           - SoAd static socket index                                                                      **/
/** TotalPduDataLength_u32      - Total Pdu data length of the Pdu that need to be discarded                                    **/
/** RemainingLength_u16         - Remaining length in the message                                                               **/
/**                                                                                                                             **/
/** Parameters (inout):         None                                                                                            **/
/**                                                                                                                             **/
/** Parameters (out):           None                                                                                            **/
/**                                                                                                                             **/
/** Return value:               - Length that has been effectively discarded                                                    **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
static uint16 SoAd_DiscardPdu ( SoAd_SoConIdType StaticSocketId_uo,
                                uint32           TotalPduDataLength_u32,
                                uint16           RemainingLength_u16 )
{
#if (SOAD_TCP_PRESENT != STD_OFF)
    /* Declare local variables */
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    SoAd_SoConIdType                     lIdxDynSocket_uo;
#endif

    uint32                               lRemainingPduDataLengthToDiscard_u32;

#if (( SOAD_RX_BUFF_ENABLE != STD_OFF ) || (SOAD_TCP_PRESENT != STD_OFF))
    uint16                               lDiscardedLength_u16;
#endif

    uint16                               lEffectiveDiscardLength_u16;

#if (SOAD_TCP_PRESENT != STD_OFF)
    /* Get static socket configuration pointers */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);

    /* Get dynamic socket from static socket */
    lIdxDynSocket_uo = (*SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].SoAd_Prv_idxDynSocTable_puo);
#endif

    /* Initialize Pdu data length to discard */
    lRemainingPduDataLengthToDiscard_u32 = TotalPduDataLength_u32;

#if (( SOAD_RX_BUFF_ENABLE != STD_OFF ) || (SOAD_TCP_PRESENT != STD_OFF))
    /* Initialize the discarded length to 0 */
    lDiscardedLength_u16 = 0U;
#endif

#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
    /* ------------------------------------- */
    /* Discard fragments in Rx buffer        */
    /* ------------------------------------- */

    /* If Rx buffer is configured */
    if( SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].dynRxBuffTableEntry_pst != NULL_PTR )
    {
        /* Clear fragmented header from Rx buffer */
        SoAd_ClearFragmentedHeaderFromRxBuffer( StaticSocketId_uo );

        /* Clear fragmented data from Rx buffer and update remaining Pdu data length to discard accordingly */
        lDiscardedLength_u16 = SoAd_ClearFragmentedDataFromRxBuffer( StaticSocketId_uo, &lRemainingPduDataLengthToDiscard_u32 );
    }
#endif


    /* --------------------------------------------- */
    /* Compute effective discard length              */
    /* --------------------------------------------- */

    /* If remaining length to discard is lower or equal to the the remaining length in the message */
    if( lRemainingPduDataLengthToDiscard_u32 <= (uint32)(RemainingLength_u16) )
    {
        /* Complete remaining Pdu length can be discarded */
        /* (the complete remaining Pdu length is contained in the message itself) */
        lEffectiveDiscardLength_u16 = (uint16)( lRemainingPduDataLengthToDiscard_u32 );
    }
    else
    {
        /* Only remaining length in the message can be discarded */
        lEffectiveDiscardLength_u16 = RemainingLength_u16;
    }


#if (SOAD_TCP_PRESENT != STD_OFF)
    /* --------------------------------------------- */
    /* Save remaining length to discard for Tcp      */
    /* --------------------------------------------- */

    /* If we are in case of Tcp, we need to remember how many byte still need to be discarded */
    /* (fragmentation can occur for Tcp but not Udp and if there is fragmentation the following fragments of the current Pdu to discard need to be discarded in following RxIndication) */
    if( (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_TCP)   &&
        (lRemainingPduDataLengthToDiscard_u32 >= (uint32)(lEffectiveDiscardLength_u16)) )
    {
        /* Save remaining length to be discarded in dynamic socket */
        SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].rxTcpRemainingPduLengthToDicard_u32 = ( lRemainingPduDataLengthToDiscard_u32 - (uint32)(lEffectiveDiscardLength_u16) );

    }

    /* --------------------------------------------- */
    /* Confirm TCP reception (discard)               */
    /* --------------------------------------------- */

    /* [SWS_SoAd_00564] */
    /* In case of a TCP socket connection the SoAd shall confirm the reception of all data either forwarded to the upper layer or finally handled */
    /* by SoAd (e.g. discarded data or processed PDU Header) by calling TcpIp_TcpReceived() within SoAd_RxIndication() or SoAd_MainFunction() respectively. */

    /* Check if upper layer type is TCP and if it secure connection if tls is enabled */
    /* For secure connection , the  TcpIp_TcpReceived will called by TLS module itself */
    if(( SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_TCP )
#if(SOAD_TCP_TLS_ENABLED != STD_OFF)
            && ( SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].isSecureConnection_b == FALSE)
#endif/* SOAD_TCP_TLS_ENABLED != STD_OFF */
       )
    {
        /* The reception of socket data is confirmed to the TCP/IP stack */
        (void)TcpIp_TcpReceived(    SoAd_DyncSocConfig_ast[(*lSoAdStaticSocConfig_cpst->SoAd_Prv_idxDynSocTable_puo)].socketHandleTcpIp_uo,
                                    (uint16)(lDiscardedLength_u16 + lEffectiveDiscardLength_u16) );
    }
#endif

    return lEffectiveDiscardLength_u16;
}


/*********************************************************************************************************************************/
/** SoAd_ConvertMessageToPdu()  - This function convert the current part of the received message into a Pdu as per              **/
/**                             [SWS_SoAd_00562]                                                                                **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo           - SoAd static socket index                                                                      **/
/** CurrentPointer_pu8          - Pointer to the received message from which it is needed to extract Pdu                        **/
/** RemainingLength_u16         - Length in bytes of the remaining part of the received message                                 **/
/**                                                                                                                             **/
/** Parameters (inout):         None                                                                                            **/
/**                                                                                                                             **/
/** Parameters (out):                                                                                                           **/
/** RxPduDesc_pst               - Detected Pdu description                                                                      **/
/** PresentHeaderLength_pu8     - Present header length                                                                         **/
/**                                                                                                                             **/
/** Return value:               - SoAd_RxPduAcceptance_ten                                                                      **/
/**                                 SOAD_PDU_OK: The Pdu is valid for further processing                                        **/
/**                                 SOAD_PDU_FRAGMENTED_HEADER: Received Pdu header is fragmented                               **/
/**                                 SOAD_PDU_DISCARD: The Pdu need to be discarded                                              **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
static SoAd_RxPduAcceptance_ten SoAd_ConvertMessageToPdu ( SoAd_SoConIdType     StaticSocketId_uo,
                                                           const uint8 *        CurrentPointer_cpu8,
                                                           uint16               RemainingLength_u16,
                                                           SoAd_RxPduDesc_tst * RxPduDesc_pst,
                                                           uint8 *              PresentHeaderLength_pu8 )
{
    /* Declare local variables */
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    SoAd_RxPduAcceptance_ten             lPduAcceptance_en;
    uint32                               lPduHeaderId_u32;
    uint32                               lPduDataLength_u32;
    uint16                               lRemainingAndPresentLength_u16;
    PduIdType                            lRxPduId_uo;
    Std_ReturnType                       lRetVal_en;
    boolean                              lHeaderEnableFlag_b;
#if (SOAD_TCP_PRESENT != STD_OFF)
    boolean                              lHeaderSavedInBuff_b;
#endif

    /* Get static socket configuration pointers */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);

    /* Initialize the Pdu acceptance flag to DISCARD */
    lPduAcceptance_en = SOAD_PDU_DISCARD;

    /* Get the PDU header flag enabling from socket connection Group configuration table */
    lHeaderEnableFlag_b = SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].headerEnable_b;

#if (SOAD_TCP_PRESENT != STD_OFF)
    /* Set the flag indicating if header has been saved in buffer to FALSE */
    lHeaderSavedInBuff_b = FALSE;
#endif

    /* Rx buffer and Tcp enabled (fragmentation can be handled only in that case) */
#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )
    /* ------------------------------------------------ */
    /* Check fragmented Pdu data reception in progress  */
    /* ------------------------------------------------ */

    /* If Rx buffer is configured and fragmented Pdu data reception is in progress in the SoAd Rx buffer */
    if( (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR)                            &&
        (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragReceptionInProgress_b == TRUE) )
    {
        /* Set the Rx Pdu desc in out argument of the function */
        RxPduDesc_pst->PduDataLength_u32 = lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragCurrentRxPduDesc_st.PduDataLength_u32;
        RxPduDesc_pst->RxPduId_uo = lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragCurrentRxPduDesc_st.RxPduId_uo;

        /* Set the present header length in out argument of the function */
        (*PresentHeaderLength_pu8) = 0U;

        /* The Pdu data is fragmented */
        /* (Pdu header has already been extracted in the previously received message) */
        /* (we need to terminate the reception of the fragmented data before extracting again a Pdu header) */
        lPduAcceptance_en = SOAD_PDU_FRAGMENTED_DATA;
    }

    /* No Pdu data fragmentation in progress */
    else
#endif
    {
        /* --------------------------------------------- */
        /* SoAd header disabled                          */
        /* --------------------------------------------- */

        /* [SWS_SoAd_00563] */
        /* In case PDU header option is disabled (SoAdPduHeader-Enable is FALSE) for a socket connection, SoAd shall convert the received UDP or TCP message 1:1 into a PDU within */
        /* SoAd_RxIndication(), i.e. each TCP segment and UDP message forms a separate PDU. */

        /* In case SoAd header option is disabled */

        /* Set Pdu header id to default value as no header is present */
        lPduHeaderId_u32 = 0U;

        /* Set Pdu data length to the total length of the message for header disabled case */
        lPduDataLength_u32 = (uint32)( RemainingLength_u16 );

        if( lHeaderEnableFlag_b == FALSE )
        {
            /* --------------------------------------------- */
            /* Convert UDP or TCP message 1:1                */
            /* --------------------------------------------- */

            /* Set the present header length in out argument of the function to 0 for header disabled case */
            (*PresentHeaderLength_pu8) = 0U;

            /* The Pdu is accepted for further processing */
            lPduAcceptance_en = SOAD_PDU_OK;
        }

        /* --------------------------------------------- */
        /* SoAd header enabled                           */
        /* --------------------------------------------- */

        /* [SWS_SoAd_00559] */
        /* In case PDU header option is enabled (SoAdPduHeaderEnable is TRUE) for a socket connection, SoAd shall convert the message into a PDU within */
        /* SoAd_RxIndication() according to the following: */
        /* (1) assemble the PDU Header into a SoAd receive buffer if it is fragmented in multiple TCP segments */
        /* (2) extract the PDU Header from the received message */
        /* (3) select the related socket route according to the received PDU Header ID (SoAdRxPduHeaderId); if no socket route can be found, simply discard the PDU and if development */
        /*     error detection is enabled: raise the error SOAD_E_INV_PDUHEADER_ID. */
        /* (4) use the length field of the PDU Header to identify the length of the actual PDU and the start of the next PDU to proceed with (2) until the end of the message is reached. */
        /* If the remainder is smaller than a PDU Header or the indicated length within the header SoAd shall stop processing and ignore the rest of the message. */

        /* In case SoAd header option is enabled */
        else
        {
            /* --------------------------------------------- */
            /* Assemble and extract header                   */
            /* --------------------------------------------- */

            /* Set remaining length in the message needed by SoAd_ExtractPduHeader */
            lRemainingAndPresentLength_u16 = RemainingLength_u16;

            /* Extract information from the Pdu header */
            lPduAcceptance_en = SoAd_ExtractPduHeader( StaticSocketId_uo,
                                                       CurrentPointer_cpu8,
                                                       &lRemainingAndPresentLength_u16,
                                                       &lPduHeaderId_u32,
                                                       &lPduDataLength_u32 );

            /* Set the present header length in out argument of the function now that this value has been identified */
            (*PresentHeaderLength_pu8) = (uint8)( lRemainingAndPresentLength_u16 );
        }


        /* If Pdu is valid for further processing (means the message has been converted to PDU) */
        /* (when Pdu header is enabled and Pdu is valid for further processing, that means that the complete header has been received, we can now confirm it to Tcp) */
        if( lPduAcceptance_en == SOAD_PDU_OK )
        {
            RxPduDesc_pst->PduDataLength_u32 = lPduDataLength_u32;

            lRxPduId_uo = SOAD_CFG_PDU_ID_DEFAULT_VALUE;

            /* --------------------------------------------- */
            /* Select related socket route                   */
            /* --------------------------------------------- */

            /* Get the socket route according to the received Pdu header Id */
            lRetVal_en = SoAd_SelectRxSocketRoute( StaticSocketId_uo,
                                                   lPduHeaderId_u32,
                                                   lHeaderEnableFlag_b,
                                                   &lRxPduId_uo );

            /* In case the socket route is found */
            if( lRetVal_en == E_OK )
            {
                /* Set the out arguments of the function */
                /* (decoding of the header and selecting of the socket route is a success, Pdu is accepted for further processing) */
                RxPduDesc_pst->RxPduId_uo = lRxPduId_uo;
            }

            /* In case the socket route is not found */
            else
            {
                /* Raise DET Error */
                /* To notify about the Invalid Rx Pdu Id */
                if(lHeaderEnableFlag_b == FALSE)
                {
                    SOAD_DET_REPORT_ERROR( SOAD_RX_INDICATION_API_ID, SOAD_E_INV_PDUID )
                }
                /* Raise DET Error */
                /* To notify about the Invalid Rx Pdu Id */
                if(lHeaderEnableFlag_b != FALSE)
                {
                    SOAD_DET_REPORT_ERROR( SOAD_RX_INDICATION_API_ID, SOAD_E_INV_PDUHEADER_ID )
                }

                /* Set the Pdu acceptance flag to DISCARD */
                lPduAcceptance_en = SOAD_PDU_DISCARD;

                /* Set the Pdu acceptance flag to DISCARD the Rest of the Message for UDP */
#if (SOAD_REST_UDP_MSG_DISCARD_BY_INVALID_HEADERID == STD_ON)
                if(TCPIP_IPPROTO_UDP == SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en)
                {
                    lPduAcceptance_en = SOAD_PDU_DISCARD_REST_MSG;
                }
#endif

#if ( SOAD_GETANDRESET_MEASUREMENTDATA_API == STD_ON )
				/* The exclusive area SoAd_MeasurementData is used to avoid race condition when measurement data is stored */
				SchM_Enter_SoAd_MeasurementData();
#if (SOAD_TCP_PRESENT != STD_OFF)
                /* Check if upper layer type is TCP */
                if( TCPIP_IPPROTO_TCP == SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en )
                {
                    /* incrementing the SoAd_RxTcpDropCnt_u32 */
                    /* [SWS_SoAd_00754] All measurement data which counts data shall not overrun. */
                    SoAd_RxTcpDropCnt_u32 = ((0xFFFFFFFFu > SoAd_RxTcpDropCnt_u32) ? (SoAd_RxTcpDropCnt_u32 + 1u) : \
                                                0xFFFFFFFFu);
                }
                else
#endif /* (SOAD_TCP_PRESENT != STD_OFF) */
                {
                    /* Meassurement is not necessary when discard the rest of the message is on */
#if (SOAD_REST_UDP_MSG_DISCARD_BY_INVALID_HEADERID == STD_OFF)
                    /* incrementing the SoAd_RxUdpDropCnt_u32 */
                    /* [SWS_SoAd_00754] All measurement data which counts data shall not overrun. */
                    SoAd_RxUdpDropCnt_u32 = ((0xFFFFFFFFu > SoAd_RxUdpDropCnt_u32) ? (SoAd_RxUdpDropCnt_u32 + 1u) : \
                                                0xFFFFFFFFu);
#endif
                }
				SchM_Exit_SoAd_MeasurementData();
#endif /* ( SOAD_GETANDRESET_MEASUREMENTDATA_API == STD_ON ) */

            }

            /* --------------------------------------------- */
            /* Handle the starting of fragmented data        */
            /* --------------------------------------------- */

            /* If Pdu is valid for further processing and Pdu data is not complete in the message */
            if( (lPduAcceptance_en == SOAD_PDU_OK)   &&
                 ((uint32)(RemainingLength_u16 - (*PresentHeaderLength_pu8)) < RxPduDesc_pst->PduDataLength_u32) )
            {
                /* Rx buffer and Tcp enabled (fragmentation can be handled only in that case) */
#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )
                /* If Rx buffer is configured for the socket and protocol is Tcp */
                /* (saving of fragments is only possible for Tcp, order of Udp frames is not guarenteed) */
                if( (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR)                                            &&
                    (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_TCP) )
                {
                    /* Start the fragmented Pdu reception in the SoAd Rx buffer */
                    lRetVal_en = SoAd_StartFragmentedDataInRxBuffer( StaticSocketId_uo, RxPduDesc_pst, &(lHeaderSavedInBuff_b) );

                    /* Set the Pdu acceptance to PDU_FRAGMENTED_DATA or PDU_DISCARD depending of the return code */
                    lPduAcceptance_en = ( (lRetVal_en == E_OK)? (SOAD_PDU_FRAGMENTED_DATA) : (SOAD_PDU_DISCARD) );
                }
                else
#endif
                {
                    /*Raise DET Error*/
                    /* To notify about Length error in PDU (reassembly is not supported for Tcp without Rx buffer and for Udp) */
                    SOAD_DET_REPORT_ERROR(SOAD_RX_INDICATION_API_ID, SOAD_E_INV_ARG )

                    /* Set the Pdu acceptance flag to DISCARD */
                    lPduAcceptance_en = SOAD_PDU_DISCARD;
                }
            }

#if (SOAD_TCP_PRESENT != STD_OFF)
            /* --------------------------------------------- */
            /* Confirm TCP reception (header)                */
            /* --------------------------------------------- */

            /* [SWS_SoAd_00564] */
            /* In case of a TCP socket connection the SoAd shall confirm the reception of all data either forwarded to the upper layer or finally handled */
            /* by SoAd (e.g. discarded data or processed PDU Header) by calling TcpIp_TcpReceived() within SoAd_RxIndication() or SoAd_MainFunction() respectively. */

            /* Check if the following conditions are true */
            /* Check if upper layer type is TCP and if it secure connection if tls is enabled */
            /* For secure connection , the  TcpIp_TcpReceived will called by TLS module itself */
            /* 3. Header is enabled and header is not saved in the SoAd Rx buffer (if header is in buffer that means that it is not processed by SoAd yet and TcpReceived shall not be called) */
            if( (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_TCP)   &&
                ((lHeaderEnableFlag_b == TRUE) && (lHeaderSavedInBuff_b == FALSE))
#if(SOAD_TCP_TLS_ENABLED != STD_OFF)
                && ( SoAd_DyncSocConfig_ast[*lSoAdStaticSocConfig_cpst->SoAd_Prv_idxDynSocTable_puo].isSecureConnection_b == FALSE)
#endif/* SOAD_TCP_TLS_ENABLED != STD_OFF */
              )
            {
                /* The reception of socket data is confirmed to the TCP/IP stack */
                (void)TcpIp_TcpReceived( SoAd_DyncSocConfig_ast[(*lSoAdStaticSocConfig_cpst->SoAd_Prv_idxDynSocTable_puo)].socketHandleTcpIp_uo,
                                         SOAD_PDUHDR_SIZE );
            }
#endif
        }
    }

    return lPduAcceptance_en;
}


/*********************************************************************************************************************************/
/** SoAd_ExtractPduHeader()     - This function assemble and extract the Pdu header                                             **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo           - SoAd static socket index                                                                      **/
/** CurrentPointer_pu8          - Pointer to the received message from which it is needed to extract Pdu                        **/
/** RemainingLength_u16         - Length in bytes of the remaining part of the received message                                 **/
/**                                                                                                                             **/
/** Parameters (inout):                                                                                                         **/
/** RemainingAndPresentLength_pu16 - Length in bytes of the remaining part of the received message as in argument, present      **/
/**                                  Header length in out argument (set when SOAD_PDU_FRAGMENTED_HEADER or SOAD_PDU_OK)         **/
/**                                                                                                                             **/
/** Parameters (out):                                                                                                           **/
/** PduHeaderId_pu32            - Extracted Pdu header Id (set when SOAD_PDU_OK)                                                **/
/** PduDataLength_pu32          - Extracted Pdu header length (set when SOAD_PDU_OK)                                            **/
/**                                                                                                                             **/
/** Return value:               - SoAd_RxPduAcceptance_ten                                                                      **/
/**                                 SOAD_PDU_OK: The Pdu is valid for further processing                                        **/
/**                                 SOAD_PDU_FRAGMENTED_HEADER: Received Pdu header is fragmented                               **/
/**                                 SOAD_PDU_DISCARD: The Pdu need to be discarded                                              **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
static SoAd_RxPduAcceptance_ten SoAd_ExtractPduHeader ( SoAd_SoConIdType StaticSocketId_uo,
                                                        const uint8 *    CurrentPointer_cpu8,
                                                        uint16 *         RemainingAndPresentLength_pu16,
                                                        uint32 *         PduHeaderId_pu32,
                                                        uint32 *         PduDataLength_pu32 )
{
    /* Declare local variables */
    SoAd_RxPduAcceptance_ten             lPduAcceptance_en;

#if (SOAD_TCP_PRESENT != STD_OFF)
    SoAd_SoConIdType lIdxDynSocket_uo;
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
#endif

#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )
    uint8 *                              lPduHeaderPointer_pu8;
    uint8                                lPresentHeaderLength_u8;
    Std_ReturnType                       lRetVal_en;
    boolean                              lErrorFlag_b;
#endif

#if (SOAD_TCP_PRESENT != STD_OFF)
    /* Get static socket configuration pointer */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);

    /* Get dynamic socket from static socket */
    lIdxDynSocket_uo = (*SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].SoAd_Prv_idxDynSocTable_puo);
#endif

    /* Initialize the Pdu acceptance flag to DISCARD */
    lPduAcceptance_en = SOAD_PDU_DISCARD;

    /* Rx buffer and Tcp enabled (fragmentation of headercan be handled only in that case) */
#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )

    /* Initialize present header length */
    lPresentHeaderLength_u8 = SOAD_PDUHDR_SIZE;

    /* Initialize the error flag to FALSE */
    lErrorFlag_b = FALSE;

    /* --------------------------------------------- */
    /* Case of Tcp with Rx buffer                    */
    /* --------------------------------------------- */

    /* Check if Rx buffer is configured for the socket and protocol is Tcp */
    /* (saving of fragments is only possible for Tcp, order of Udp frames is not guarenteed) */
    if( (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR)    &&
        (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_TCP) )
    {
        /* --------------------------------------------- */
        /* Save fragmented header                        */
        /* --------------------------------------------- */

        /* If there is already a fragmented header in progress in the Rx buffer */
        /* Or if there is no fragmented header in progress in the Rx buffer but the remaining length of the message is not big enough for a complete header */
        if( (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->HeaderFragReceptionInProgress_b == TRUE) ||
            ((lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->HeaderFragReceptionInProgress_b == FALSE) && ((*RemainingAndPresentLength_pu16) < SOAD_PDUHDR_SIZE)) )
        {
            /* Save it in SoAd Rx buffer */
            lRetVal_en = SoAd_AddFragmentedHeaderInRxBuffer( StaticSocketId_uo,
                                                             CurrentPointer_cpu8,
                                                             (*RemainingAndPresentLength_pu16),
                                                             &lPresentHeaderLength_u8 );

            /* If error occured (no space in the buffer) */
            if( lRetVal_en == E_NOT_OK )
            {
                /* When header is not complete and fragment cannot be saved then the communication cannot continue */
                /* Socket need to be closed because all the following message will be wrongly interpreted as beginning of SoAd header */

                /* Set the global variables to skip the further transmission and reception */
                SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].discardNewTxRxReq_b = TRUE;

                /* As an error occurred, so request need to be taken to abort the socket connection */
                SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].socConRequestedMode_en = SOAD_EVENT_OFFLINEREQ_SOAD;

                /* Set the present header length in out argument of the function */
                (*RemainingAndPresentLength_pu16) = 0U;

                /* Set a flag to indicate that there is an error */
                lErrorFlag_b = TRUE;

                /* Remaining bytes in the message will be discarded and fragments in rx buffer deleted */
            }
        }


        /* --------------------------------------------- */
        /* Extract Pdu header if possible                */
        /* --------------------------------------------- */

        /* No error occured previously */
        if( lErrorFlag_b == FALSE )
        {
            /* If fragmentation is in progress */
            if( lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->HeaderFragReceptionInProgress_b == TRUE )
            {
                /* Set the present header length in out argument of the function */
                (*RemainingAndPresentLength_pu16) = lPresentHeaderLength_u8;

                /* Set the acceptance flag to indicate that the header is fragmented and not complete for the moment */
                lPduAcceptance_en = SOAD_PDU_FRAGMENTED_HEADER;
            }

            /* No fragmentation in progress */
            /* (this means we have access to the complete data) */
            else
            {
                /* Complete header is present in the Rx buffer */
                if( lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->HeaderFragCurrentSize_u8 == SOAD_PDUHDR_SIZE )
                {
                    /* Get complete header from Rx buffer */
                    SoAd_GetFragmentedHeaderFromRxBuffer( StaticSocketId_uo, &lPduHeaderPointer_pu8 );

                    /* Reading Pdu Id from Ethernet header which shall be used for de-multiplexing and finding out Pdu owner*/
                    /*converting the data from Big Endian to controller specific Endianness*/
                    (*PduHeaderId_pu32) = (lPduHeaderPointer_pu8[3]) | (((uint32)lPduHeaderPointer_pu8[2])<<8U) |
                                      (((uint32)lPduHeaderPointer_pu8[1])<<16U) |(((uint32)lPduHeaderPointer_pu8[0])<<24U);

                    /* Reading Pdu length from Ethernet header which shall be used for finding out Pdu length*/
                    /*converting the data from Big Endian to controller specific Endianness*/
                    (*PduDataLength_pu32) = (lPduHeaderPointer_pu8[7]) | (((uint32)lPduHeaderPointer_pu8[6])<<8U) |
                                         (((uint32)lPduHeaderPointer_pu8[5])<<16U) | (((uint32)lPduHeaderPointer_pu8[4])<<24U);

                    /* Set the present header length in out argument of the function */
                    (*RemainingAndPresentLength_pu16) = lPresentHeaderLength_u8;

                    /* Clear header from Rx buffer */
                    SoAd_ClearFragmentedHeaderFromRxBuffer( StaticSocketId_uo );

                    /* The Pdu is accepted for further processing */
                    lPduAcceptance_en = SOAD_PDU_OK;
                }

                /* If not, means complete header is present in the message itself */
                else
                {
                    /* Reading Pdu Id from Ethernet header which shall be used for de-multiplexing and finding out Pdu owner*/
                    /*converting the data from Big Endian to controller specific Endianness*/
                    (*PduHeaderId_pu32) = (CurrentPointer_cpu8[3]) | (((uint32)CurrentPointer_cpu8[2])<<8U) |
                                      (((uint32)CurrentPointer_cpu8[1])<<16U) |(((uint32)CurrentPointer_cpu8[0])<<24U);

                    /* Reading Pdu length from Ethernet header which shall be used for finding out Pdu length*/
                    /*converting the data from Big Endian to controller specific Endianness*/
                    (*PduDataLength_pu32) = (CurrentPointer_cpu8[7]) | (((uint32)CurrentPointer_cpu8[6])<<8U) |
                                         (((uint32)CurrentPointer_cpu8[5])<<16U) | (((uint32)CurrentPointer_cpu8[4])<<24U);

                    /* Set the present header length in out argument of the function */
                    (*RemainingAndPresentLength_pu16) = lPresentHeaderLength_u8;

                    /* The Pdu is accepted for further processing */
                    lPduAcceptance_en = SOAD_PDU_OK;
                }
            }
        }
    }
    else
#endif
    /* --------------------------------------------- */
    /* Case of Udp or Tcp without Rx buffer          */
    /* --------------------------------------------- */
    {
        /* --------------------------------------------- */
        /* Extract Pdu header                            */
        /* --------------------------------------------- */

        /* Check Rx Frame Length as passed in parameter is at least greater than the Pdu header length*/
        if ( (*RemainingAndPresentLength_pu16) >= SOAD_PDUHDR_SIZE )
        {
            /* Reading Pdu Id from Ethernet header which shall be used for de-multiplexing and finding out Pdu owner*/
            /*converting the data from Big Endian to controller specific Endianness*/
            (*PduHeaderId_pu32) = (CurrentPointer_cpu8[3]) | (((uint32)CurrentPointer_cpu8[2])<<8U) |
                              (((uint32)CurrentPointer_cpu8[1])<<16U) |(((uint32)CurrentPointer_cpu8[0])<<24U);

            /* Reading Pdu length from Ethernet header which shall be used for finding out Pdu length*/
            /*converting the data from Big Endian to controller specific Endianness*/
            (*PduDataLength_pu32) = (CurrentPointer_cpu8[7]) | (((uint32)CurrentPointer_cpu8[6])<<8U) |
                                 (((uint32)CurrentPointer_cpu8[5])<<16U) | (((uint32)CurrentPointer_cpu8[4])<<24U);

            /* Set the present header length in out argument of the function */
            (*RemainingAndPresentLength_pu16) = SOAD_PDUHDR_SIZE;

            /* The Pdu is accepted for further processing */
            lPduAcceptance_en = SOAD_PDU_OK;
        }

        /* Not enough bytes in the message */
        else
        {
#if (SOAD_TCP_PRESENT != STD_OFF)
            /* When header is not complete and fragment cannot be saved then the communication cannot continue */
            /* Socket need to be closed because all the following message will be wrongly interpreted as beginning of SoAd header */
            /* For TCP the Socket should be closed and for UDP, SoAd shall discard the remaining bytes in the message */
            if(SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_TCP)
            {
                /* Set the global variables to skip the further transmission and reception */
                SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].discardNewTxRxReq_b = TRUE;

                /* As an error occurred, so request need to be taken to abort the socket connection */
                SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].socConRequestedMode_en = SOAD_EVENT_OFFLINEREQ_SOAD;
            }
#endif
            /*Raise DET Error*/
            /* To notify about Length error in PDU */
            SOAD_DET_REPORT_ERROR( SOAD_RX_INDICATION_API_ID, SOAD_E_INV_ARG)

            /* Set the present header length in out argument of the function */
            (*RemainingAndPresentLength_pu16) = 0U;

            /* Remaining bytes in the message will be discarded */
        }
    }

    return lPduAcceptance_en;
}


/*********************************************************************************************************************************/
/** SoAd_SelectRxSocketRoute()  - This function convert the current part of the received message into a Pdu as per              **/
/**                             [SWS_SoAd_00562]                                                                                **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo           - SoAd static socket index                                                                      **/
/** RxHeaderId_32               - Rx Pdu Header Id to be used to find the SoAd socket route                                     **/
/** HeaderEnableFlag_b          - Header to be used to find the SoAd socket route                                               **/
/**                                                                                                                             **/
/** Parameters (inout):         None                                                                                            **/
/**                                                                                                                             **/
/** Parameters (out):                                                                                                           **/
/** RxPduId_puo                 - Found Rx socket route (Rx Pdu id)                                                             **/
/**                                                                                                                             **/
/** Return value:               - SoAd_RxPduAcceptance_ten                                                                      **/
/**                                 E_OK: The socket route has been found                                                       **/
/**                                 E_NOT_OK: The socket route has not been found or there is multiple socket routes configured **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
Std_ReturnType SoAd_SelectRxSocketRoute ( SoAd_SoConIdType StaticSocketId_uo,
                                          uint32           RxHeaderId_32,
                                          boolean          HeaderEnableFlag_b,
                                          PduIdType *      RxPduId_puo )
{
    /* Declare local variables */
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    PduIdType                            lIdxRxPdu_uo;
    Std_ReturnType                       lFunctionRetVal_en;
    /* Index for looping across all Rx Pdu's afor particular socket */
    PduIdType                            lIdxRxPduLoop_uo ;

    /* Initialize the return value of the function to E_NOT_OK */
    lFunctionRetVal_en = E_NOT_OK;

    /* Get static socket configuration pointer */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);

    /* Loop to search the socket route */
    for( lIdxRxPduLoop_uo = 0; \
             lIdxRxPduLoop_uo < lSoAdStaticSocConfig_cpst->numberOfRxPdusConfigured_u8; \
                lIdxRxPduLoop_uo++ )
    {
        lIdxRxPdu_uo = lSoAdStaticSocConfig_cpst->rxPduIds_pcuo[lIdxRxPduLoop_uo];

        /* Condition Check:
         * The static socket id given in argument is matching with the static socket id of the current route
         * Header is disabled or header is enabled and header id is exactly matching
         */
        if( ( FALSE == HeaderEnableFlag_b ) || \
            ( SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[lIdxRxPdu_uo].rxPduHeaderId_u32 == RxHeaderId_32 ) )
        {
            /* Set the found Rx Pdu Id in the out argument of the function */
            (* RxPduId_puo) = lIdxRxPdu_uo;

            /* Set the return value of the function to E_OK */
            lFunctionRetVal_en = E_OK;

            break;
        }
    }

    return lFunctionRetVal_en;
}


/*********************************************************************************************************************************/
/** SoAd_ProcessPduData()       - This function process the data contained in a received Pdu and pass it to the Upper Layer     **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo           - SoAd static socket index                                                                      **/
/** PduData_pu8                 - Pointer to memory containing Pdu data                                                         **/
/** RemainingLength_u16         - Remaining length f the message                                                                **/
/** RxPduDesc_st                - Description of the received Pdu to be handled                                                 **/
/**                                                                                                                             **/
/** Parameters (inout):         None                                                                                            **/
/**                                                                                                                             **/
/** Parameters (out):                                                                                                           **/
/** CopiedDataToUl_pu16         - Length of data that has been copied to the Upper Layer buffer                                 **/
/**                                                                                                                             **/
/** Return value:               - SoAd_RxPduAcceptance_ten                                                                      **/
/**                                 SOAD_PDU_OK: The Pdu data have been forwarded to UL                                         **/
/**                                 SOAD_PDU_FRAGMENTED_DATA: The Pdu data is fragmented, processing need to continue later     **/
/**                                 SOAD_PDU_DISCARD: The Pdu need to be discarded                                              **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
static SoAd_RxPduAcceptance_ten SoAd_ProcessPduData ( SoAd_SoConIdType   StaticSocketId_uo,
                                                      uint8 *            PduData_pu8,
                                                      uint16             RemainingLength_u16,
                                                      SoAd_RxPduDesc_tst RxPduDesc_st,
                                                      uint16 *           CopiedDataToUl_pu16,
                                                      uint16 *           MsgCopyLength_pu16)
{
    /* Declare local variables */
    SoAd_RxPduAcceptance_ten             lPduAcceptance_en;

#if (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF)
    boolean                              lRgStatus_b;
#endif /* SOAD_ROUTING_GROUPS_ENABLE != STD_OFF */

#if (SOAD_TP_PRESENT != STD_OFF)
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
#endif /* SOAD_TP_PRESENT != STD_OFF */

    /* Initialize the Pdu acceptance flag to DISCARD */
    lPduAcceptance_en = SOAD_PDU_DISCARD;

#if (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF)
    lRgStatus_b       = TRUE;
#endif /* SOAD_ROUTING_GROUPS_ENABLE != STD_OFF */

    /* --------------------------------------------- */
    /* Get routing group status                      */
    /* --------------------------------------------- */

#if (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF)
    /* Check Routing Group is enabled for the fetched socket */
    lRgStatus_b = ( 0U < SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].numOfRxRoutingGroups_uo ) ? \
                            ( SoAd_GetRoutingGroupsEnabledStatus( \
                                    SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].rxRoutingGroupConf_cpuo, \
                                    SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].numOfRxRoutingGroups_uo, \
                                    StaticSocketId_uo ) ):
                                    TRUE;
#endif /* SOAD_ROUTING_GROUPS_ENABLE != STD_OFF */

    /* --------------------------------------------- */
    /* Reception for IF type                         */
    /* --------------------------------------------- */

    /* [SWS_SoAd_00565] */
    /* SoAd shall process TP- and IF-PDUs independently and within each type according to the received order per socket connection. */

#if ( SOAD_IF_PRESENT != STD_OFF )
    /* If upper layer type is IF type */
    if( SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].rxUpperLayerType_en == SOAD_IF )
    {
        /* If pointers to upper layer callbacks are valid */
        if( SoAd_CurrConfig_cpst->SoAdIf_RxCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxIfRxIndication_u8].RxCallback_pfn != NULL_PTR )
        {
            /* --------------------------------------------- */
            /* Check routing                                 */
            /* --------------------------------------------- */

            /* [SWS_SoAd_00600] */
            /* If a PDU is received according to SoAdSocketRouteDest which belongs only to inactive RoutingGroups, SoAd shall simply discard the PDU. */

            /* If the routing is possible */
            /* (for IF, header enabled: if routing is disabled, then SoAd shall discard the complete Pdu (even if the routing group is disabled between 2 fragments of the Pdu, the complete Pdu shall be discarded because for IF a Pdu is not considered as in active reception by UL)) */
            /* (for IF, header disabled: if routing is disabled then SoAd shall discard because according to [SWS_SoAd_00563] each meassge shall be considered as a Pdu) */
#if (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF)
            if( FALSE != lRgStatus_b )
#endif /* SOAD_ROUTING_GROUPS_ENABLE != STD_OFF */
            {
                /* Process for IF */
                lPduAcceptance_en = SoAd_ProcessPduDataIF( StaticSocketId_uo,
                                                            RemainingLength_u16,
                                                            PduData_pu8,
                                                            RxPduDesc_st,
                                                            &(*CopiedDataToUl_pu16),
                                                            &(*MsgCopyLength_pu16));
            }
        }
    }
    else
#endif

    /* --------------------------------------------- */
    /* Reception for TP type                         */
    /* --------------------------------------------- */

    /* [SWS_SoAd_00565] */
    /* SoAd shall process TP- and IF-PDUs independently and within each type according to the received order per socket connection. */

    /* If upper layer type is IF type */
    {
#if (SOAD_TP_PRESENT != STD_OFF)

        /* Get static socket configuration pointers */
        lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);

        /* -------------------- */
        /* Header disabled      */
        /* -------------------- */
        if( SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].headerEnable_b == FALSE )
        {
            /* If pointers to upper layer callbacks are valid */
            if( SoAd_CurrConfig_cpst->SoAdTp_copyRxDataCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxTpCopyRxDataCbk_u8].soAdCopyRxDataCbk_pfn != NULL_PTR )
            {
                /* --------------------------------------------- */
                /* Check routing                                 */
                /* --------------------------------------------- */

                /* [SWS_SoAd_00600] */
                /* If a PDU is received according to SoAdSocketRouteDest which belongs only to inactive RoutingGroups, SoAd shall simply discard the PDU. */

                /* If the routing is possible */
                /* (for TP, header disabled: if routing is disabled then SoAd shall discard because according to [SWS_SoAd_00563] each meassge shall be considered as a Pdu) */
#if (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF)
                if( lRgStatus_b != FALSE )
#endif /* SOAD_ROUTING_GROUPS_ENABLE != STD_OFF */
                {
                    *MsgCopyLength_pu16 = RemainingLength_u16;
                    /* Process for TP with header disabled */
                    lPduAcceptance_en = SoAd_ProcessPduDataTPHeaderDisabled(    StaticSocketId_uo,
                                                                                PduData_pu8,
                                                                                RemainingLength_u16,
                                                                                RxPduDesc_st,
                                                                                &(*CopiedDataToUl_pu16) );
                }
            }
        }

        /* -------------------- */
        /* Header enabled       */
        /* -------------------- */
        else
        {
            /* If pointers to upper layer callbacks are valid */
            if( (SoAd_CurrConfig_cpst->SoAdTp_copyRxDataCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxTpCopyRxDataCbk_u8].soAdCopyRxDataCbk_pfn != NULL_PTR)           &&
                (SoAd_CurrConfig_cpst->SoAdTp_StartOfReceptionCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxTpStartReceptionCbk_u8].StartOfReception_pfn != NULL_PTR ) &&
                (SoAd_CurrConfig_cpst->SoAdTp_RxCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxTpRxIndication_u8].RxCallback_pfn != NULL_PTR ) )
            {
                /* --------------------------------------------- */
                /* Check routing status                          */
                /* --------------------------------------------- */

                /* [SWS_SoAd_00600] */
                /* If a PDU is received according to SoAdSocketRouteDest which belongs only to inactive RoutingGroups, SoAd shall simply discard the PDU. */

                /* Note */
                /* Activation/deactivation of a routing group only affects new PDUs, i.e. PDUs which are already in an active reception or transmission process */
                /* by an upper layer (e.g. long TP-PDU which is received via a multiple CopyRxData calls) are not affected. */

                /* If routing group is enabled or there is a TP Pdu transfer form Rx buffer to UL for which we do not have all data */
                /* (It is needed to wait for the end of the TP reception ongoing because the end of the current Pdu shall be received before the disabling of the routing can be taken into account) */
#if ( (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF) || ( SOAD_RX_BUFF_ENABLE != STD_OFF ) )
                if(
#if (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF)
                   (lRgStatus_b != FALSE)
#endif /* SOAD_ROUTING_GROUPS_ENABLE != STD_OFF */

#if ( (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF) && ( SOAD_RX_BUFF_ENABLE != STD_OFF ) )
                   ||
#endif /* (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF) && ( SOAD_RX_BUFF_ENABLE != STD_OFF ) */

#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
                    ((lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR)   && \
                     (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->TpDataRemainingLenToTransfer_u32 > (uint32)(lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->TpDataCurrentSize_u16 + lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragCurrentSize_u16)))
#endif /* SOAD_RX_BUFF_ENABLE != STD_OFF */
                   )
#endif /* (SOAD_ROUTING_GROUPS_ENABLE != STD_OFF) || ( SOAD_RX_BUFF_ENABLE != STD_OFF ) */
                {
                    *MsgCopyLength_pu16 = ( (RxPduDesc_st.PduDataLength_u32 < (uint32)(RemainingLength_u16))? (uint16)(RxPduDesc_st.PduDataLength_u32) : (RemainingLength_u16) );
                    /* Process for TP with header enabled */
                    lPduAcceptance_en = SoAd_ProcessPduDataTPHeaderEnabled( StaticSocketId_uo,
                                                                            PduData_pu8,
                                                                            RemainingLength_u16,
                                                                            RxPduDesc_st,
                                                                            &(*CopiedDataToUl_pu16) );
                }
            }
        }
#endif
    }

    (void) RemainingLength_u16;
    return lPduAcceptance_en;
}


/*********************************************************************************************************************************/
/** SoAd_ProcessPduDataIF()     - This function process the data contained in a received Pdu and pass it to the Upper Layer     **/
/**                             (case of Upper Layer type IF)                                                                   **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo           - SoAd static socket index                                                                      **/
/** PduData_pu8                 - Pointer to memory containing Pdu data                                                         **/
/** RemainingLength_u16         - Remaining length in the message                                                               **/
/** RxPduDesc_st                - Description of the received Pdu to be handled                                                 **/
/**                                                                                                                             **/
/** Parameters (inout):         None                                                                                            **/
/**                                                                                                                             **/
/** Parameters (out):                                                                                                           **/
/** CopiedDataToUl_pu16         - Length of data that has been copied to the Upper Layer buffer                                 **/
/**                                                                                                                             **/
/** Return value:               - SoAd_RxPduAcceptance_ten                                                                      **/
/**                                 SOAD_PDU_OK: The Pdu data have been completely forwarded to UL                              **/
/**                                 SOAD_PDU_FRAGMENTED_DATA: The Pdu data is fragmented, processing need to continue later     **/
/**                                 SOAD_PDU_DISCARD: The Pdu need to be discarded                                              **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
#if ( SOAD_IF_PRESENT != STD_OFF )
static SoAd_RxPduAcceptance_ten SoAd_ProcessPduDataIF (
                                                        SoAd_SoConIdType   StaticSocketId_uo,
                                                        uint16             RemainingLength_u16,
                                                        uint8 *            PduData_pu8,
                                                        SoAd_RxPduDesc_tst RxPduDesc_st,
                                                        uint16 *           CopiedDataToUl_pu16,
                                                        uint16 *           MsgCopyLength_pu16)
{
    /* Declare local variables */
    SoAd_RxPduAcceptance_ten             lPduAcceptance_en;
    uint8 *                              CompletePduData_pu8;
    PduInfoType                          lPduInfo_st;
    boolean                              lStopProcessing_b;
	boolean								 lSavedRxFragmentToBuffer_b;
#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    Std_ReturnType                       lRetVal_en;
    uint32                               lRemainingPduDataLength_u32;
#endif

#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )
    /* Get static socket configuration pointer */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);
#endif

    /* Initialize the Pdu acceptance flag to DISCARD */
    lPduAcceptance_en = SOAD_PDU_DISCARD;

    /* Initialize the stop processing flag to FALSE */
    lStopProcessing_b = FALSE;

    /* Set the pointer to the complete Pdu data at the beginning itself */
    CompletePduData_pu8 = PduData_pu8;

    /* Initialize the Fragment saved to Buffer flag to FALSE */
    lSavedRxFragmentToBuffer_b = FALSE;

    /* [SWS_SoAd_00567] */
    /* SoAd shall perform the following further actions within the SoAd_RxIndication() function for reception of a PDU to an upper layer using the IF-API: */
    /* (1) assemble all data of a fragmented IF-PDU into a SoAd receive buffer if PDU Header is used */
    /* (2) call <Up>_[SoAd][If]RxIndication() of the related upper layer module (with RxPduId set to the ID specified by the upper layer module for the PDU */
    /*     referenced by SoAdRxPduRef) for each completely received PDU */
    /* (3) dispatch the next IF-PDU (if any) if PDU Header mode is used */

    /* Case of Rx buffer and Tcp enabled (fragmentation can occurs only in that case) */
#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )

    /* --------------------------------------------- */
    /* Handle fragmentation for Tcp with Rx buffer   */
    /* --------------------------------------------- */

    /* Check if Rx buffer is configured for the socket and protocol is Tcp */
    /* (saving of fragments is only possible for Tcp, order of Udp frames is not guarenteed) */
    if( (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR)    &&
        (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].soConGrpProtocol_en == TCPIP_IPPROTO_TCP) )
    {
        /* --------------------------------------------- */
        /* Save fragmented data                          */
        /* --------------------------------------------- */

        /* If there is already fragmented data in progress in the Rx buffer */
        if( lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragReceptionInProgress_b == TRUE )
        {
            /* Save it in SoAd Rx buffer */
            lRetVal_en = SoAd_AddFragmentedDataInRxBuffer( StaticSocketId_uo,
                                                           PduData_pu8,
                                                           RemainingLength_u16,
                                                           &(*MsgCopyLength_pu16));

            lSavedRxFragmentToBuffer_b = TRUE;
            /* If error occured (no space in the buffer) */
            if( lRetVal_en == E_NOT_OK )
            {
                /* Set a flag to indicate that the processing need to be stopped */
                lStopProcessing_b = TRUE;

                /* Pdu will be discarded and fragments in Rx buffer deleted */
            }
        }


        /* --------------------------------------------- */
        /* Assemble and extract Pdu data                 */
        /* --------------------------------------------- */

        /* No error occured previously */
        if( lStopProcessing_b == FALSE )
        {
            /* If fragmentation is in progress */
            if( lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragReceptionInProgress_b == TRUE )
            {
                /* Set a flag to indicate that the processing need to be stopped */
                /* (as the Pdu is not complete, it is not possible to pass it to the UL for the moment, we have to wait for the next message) */
                lStopProcessing_b = TRUE;

                /* Set the acceptance flag to indicate that the Pdu data is fragmented and not complete for the moment */
                lPduAcceptance_en = SOAD_PDU_FRAGMENTED_DATA;
            }

            /* No fragmentation in progress */
            /* (this means we have access to the complete data) */
            else
            {
                /* Complete data is present in the Rx buffer */
                if( (uint32)(lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragCurrentSize_u16) == RxPduDesc_st.PduDataLength_u32 )
                {
                    /* Get complete Pdu from Rx buffer */
                    SoAd_GetFragmentedDataFromRxBuffer( StaticSocketId_uo, &CompletePduData_pu8 );
                }
            }
        }
    }
#endif

    /* No error occured previously */
    if( lStopProcessing_b == FALSE )
    {
        /* --------------------------------------------- */
        /* Copy Rx Data                                  */
        /* --------------------------------------------- */

        /* Data pointer is at starting of the Buffer pointer */
        lPduInfo_st.SduDataPtr = CompletePduData_pu8;

        /* Pdu length shall be equal to the length extracted from received frame (if header is enabled) OR
         * Length information given as the parameter in SoAd_RxIndication (If header is disabled) */
        lPduInfo_st.SduLength = (PduLengthType)RxPduDesc_st.PduDataLength_u32;

#if(STD_ON == SOAD_METADATA_ENABLED)
        /* In case of a reception related to a SoAdSocketRouteDest, that refers to a global PDU structure configured with
         *  a MetaDataItem of the type SOCKET_CONNECTION_ID_16, which is contained in a SoAdSocketRoute that refers to a
         *   socket connection group, SoAd shall use PduInfoType.MetaDataPtr to provide the socket connection identifier (SoConId)
         *    where the PDU was received with <Up>_[SoAd][If]RxIndication().*/

        /* Check if RxPdu has meta data item of the type SOCKET_CONNECTION_ID_16 configured */
        if(TRUE == SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].isRxPduMetaDataCfgd_b )
        {
            /*[SWS_SoAd_00740] : In case of a reception related to a SoAdSocketRouteDest, that refers to a global PDU
             * structure configured with a MetaDataItem of the type SOCKET_CONNECTION_ID_16, which is contained in a SoAdSocketRoute
             *  that refers to a socket connection group, SoAd shall use PduInfoType.MetaDataPtr to provide the socket connection
             *  identifier (SoConId) where the PDU was received with <Up>_[SoAd][If]RxIndication().*/

            /*Update metadata pointer with socket connection identifier (SoConId) where the PDU was received  */
           lPduInfo_st.MetaDataPtr = (uint8 *)&StaticSocketId_uo;
        }
        else
        {
            /*If Metadata is not enabled for Rx Pdu,update pointer to NULL */
            lPduInfo_st.MetaDataPtr = NULL_PTR;
        }
#endif/* (STD_ON == SOAD_METADATA_ENABLED) */

        /*RxCallback function to the Upper layer*/
        SoAd_CurrConfig_cpst->SoAdIf_RxCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxIfRxIndication_u8].RxCallback_pfn ( \
                                        SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].ulRxPduId_uo, \
                                        (const PduInfoType *)&lPduInfo_st );

        /* For IF Rx packet, set the CopiedDataToUl_pu16 variable to Pdu length, so that we can assume that PDU is processed */
        (*CopiedDataToUl_pu16) = (uint16)( RxPduDesc_st.PduDataLength_u32 );
        if ( FALSE == lSavedRxFragmentToBuffer_b )
        {
            *MsgCopyLength_pu16 = *CopiedDataToUl_pu16;
        }

#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )
        /* Check if Rx buffer is configured */
        if( lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR )
        {
            /* Clear data fragments from Rx buffer if any */
            (void)SoAd_ClearFragmentedDataFromRxBuffer( StaticSocketId_uo, &lRemainingPduDataLength_u32 );
        }
#endif

        /* The Pdu is fully forwarded to the Upper Layer */
        lPduAcceptance_en = SOAD_PDU_OK;
    }

    (void)StaticSocketId_uo;
    (void)RemainingLength_u16;

    return lPduAcceptance_en;
}
#endif


/*********************************************************************************************************************************/
/** SoAd_ProcessPduDataTPHeaderDisabled()   - This function process the data contained in a received Pdu and pass it to the     **/
/**                                         Upper Layer (case of Upper Layer type TP with header disabled)                      **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo                       - SoAd static socket index                                                          **/
/** PduData_pu8                             - Pointer to memory containing Pdu data                                             **/
/** RemainingLength_u16                     - Remaining length in the message                                                   **/
/** RxPduDesc_st                            - Description of the received Pdu to be handled                                     **/
/**                                                                                                                             **/
/** Parameters (inout):                     None                                                                                **/
/**                                                                                                                             **/
/** Parameters (out):                                                                                                           **/
/** CopiedDataToUl_pu16                     - Length of data that has been copied to the Upper Layer buffer                     **/
/**                                                                                                                             **/
/** Return value:                           - SoAd_RxPduAcceptance_ten                                                          **/
/**                                             SOAD_PDU_OK: The Pdu data have been completely processed (UL or Rx buffer)      **/
/**                                             SOAD_PDU_DISCARD: The Pdu need to be discarded                                  **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
#if ( SOAD_TP_PRESENT != STD_OFF )
static SoAd_RxPduAcceptance_ten SoAd_ProcessPduDataTPHeaderDisabled ( SoAd_SoConIdType   StaticSocketId_uo,
                                                                      uint8 *            PduData_pu8,
                                                                      uint16             RemainingLength_u16,
                                                                      SoAd_RxPduDesc_tst RxPduDesc_st,
                                                                      uint16 *           CopiedDataToUl_pu16 )
{
    /* Declare local variables */
    SoAd_RxPduAcceptance_ten             lPduAcceptance_en;
    SoAd_SoConIdType                     lIdxDynSocket_uo;
    BufReq_ReturnType                    lBufferRetVal_en;
    PduInfoType                          lPduInfo_st;
    uint16                               lAvailablePduDataLength_u16;
    uint16                               lAvailableAndCopiedDataToUl_u16;
    PduLengthType                        lDataLenUpperLayer_uo;
#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    Std_ReturnType                       lRetVal_en;
    uint16                               lRxBuffFreeLen_u16;
#endif

#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )
    /* Get static socket configuration pointer */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);
#endif

    /* Get dynamic socket from static socket */
    lIdxDynSocket_uo = (*SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].SoAd_Prv_idxDynSocTable_puo);

    /* Initialize the Pdu acceptance flag to DISCARD */
    lPduAcceptance_en = SOAD_PDU_DISCARD;

    /* Initialize the available and copied length to 0 */
    lAvailableAndCopiedDataToUl_u16 = 0U;

#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
    /* --------------------------------------------- */
    /* Check reception in progress                   */
    /* --------------------------------------------- */

    /* [SWS_SoAd_00568] */
    /* SoAd shall perform the following further actions within the SoAd_RxIndication() function for reception of a PDU from a socket connection with PDU Header mode */
    /* disabled to an upper layer using the TP-API: */
    /* (1) if the SoAd receive buffer does not contain any TP data for this socket connection */

    /* Check if Rx buffer is not configured for the socket or if it is configured and it does not contain TP data */
    if( (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst == NULL_PTR)                                                                                            ||
        ((lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR) && (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->TpDataCurrentSize_u16 == 0U)) )
#endif
    {
        /* --------------------------------------------- */
        /* Query UL buffer size                          */
        /* --------------------------------------------- */

        /* Step (a) of [SWS_SoAd_00568] */
        /* (a) Query the available amount of data at the upper layer by calling the configurable callback function <Up>_[SoAd][Tp]CopyRxData() with PduInfoType.SduLength = 0. */

        /* Initialization of data length available at Upper layer to 0 */
        lDataLenUpperLayer_uo = 0;

        /* To get the available length from UL, call the CopyRxData function with SduLength set to 0 and SduDataPtr set to NULL */
        lPduInfo_st.SduDataPtr = NULL_PTR;
        lPduInfo_st.SduLength  = 0;

        /* RxCallback function to the Upper layer to get the available length */
        lBufferRetVal_en = SoAd_CurrConfig_cpst->SoAdTp_copyRxDataCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxTpCopyRxDataCbk_u8].soAdCopyRxDataCbk_pfn ( \
                                    SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].ulRxPduId_uo, \
                                    (const PduInfoType*)&lPduInfo_st, \
                                    &lDataLenUpperLayer_uo );

        /* In case of success */
        if( lBufferRetVal_en == BUFREQ_OK )
        {
            /* --------------------------------------------- */
            /* Check length                                  */
            /* --------------------------------------------- */

            /* Step (b) of [SWS_SoAd_00568] */
            /* (b) If not all data can be processed (i.e. forwarded to an upper layer or stored in a SoAd receive buffer), discard all received data and skip further processing. */

            /* Compute available Pdu data length */
            /* (when header is disabled, the Pdu data length is equal to the remaining length of the message) */
            lAvailablePduDataLength_u16 = RemainingLength_u16;

#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
            /* Get linear free length in SoAd Rx buffer */
            lRxBuffFreeLen_u16 = SoAd_GetRxBufLnrFreeLen( StaticSocketId_uo );
#endif

            /* If there is sufficient space to handle the data */
#if ( SOAD_RX_BUFF_ENABLE == STD_OFF )
            if( (uint32)(lDataLenUpperLayer_uo) >= (uint32)(lAvailablePduDataLength_u16) )
#else
            if( ((uint32)(lDataLenUpperLayer_uo) + (uint32)(lRxBuffFreeLen_u16)) >= (uint32)(lAvailablePduDataLength_u16) )
#endif
            {
                lPduAcceptance_en = SOAD_PDU_OK;
            }

            /* If there is not sufficient space to handle the data */
            else
            {
                /* Report DET error as per [SWS_SoAd_00693]*/
                SOAD_DET_REPORT_ERROR( SOAD_RX_INDICATION_API_ID, SOAD_E_NOBUFS )
            }
        }

        /*In case of error */
        else
        {
            /* [SWS_SoAd_00570] */
            /* If <Up>_[SoAd][Tp]StartOfReception()or <Up>_[SoAd][Tp]CopyRxData() return with BUFREQ_E_NOT_OK for a socket connection with PDU Header mode disabled, SoAd shall */
            /* (a) disable further transmission or reception for this socket connection (i.e. new transmit requests shall be rejected with E_NOT_OK and received messages shall simply be discarded) and */
            /* (b) close the socket connection in the next SoAd_MainFunction(). */

            if( lBufferRetVal_en == BUFREQ_E_NOT_OK )
            {
                /* Set the global variables to skip the further transmission and reception */
                SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].discardNewTxRxReq_b  = TRUE;

                /* The exclusive area SoAd_OpenCloseReq is used to avoid race condition when socket open or close is requestedby UL or SoAd */
                SchM_Enter_SoAd_OpenCloseReq();

                /* Check if there is no close request from UL or Tcp/Ip*/
                if((SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].socConRequestedMode_en  != SOAD_EVENT_OFFLINEREQ_UL) && \
                   (SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].socConRequestedMode_en  != SOAD_EVENT_OFFLINEREQ_TCPIP))
                {
                    /* As an error occurred, so request need to be taken to abort the socket connection */
                    SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].socConRequestedMode_en = SOAD_EVENT_OFFLINEREQ_SOAD;

                }

                SchM_Exit_SoAd_OpenCloseReq();
           }
        }


        /* --------------------------------------------- */
        /* Copy Rx Data                                  */
        /* --------------------------------------------- */

        /* Step (c) of [SWS_SoAd_00568] */
        /* (c) Copy all received data which can be accepted by the upper layer module determined at (a) to the upper layer by calling <Up>_[SoAd][Tp]Copy-RxData() */

        /* No error occured previously */
        if( lPduAcceptance_en == SOAD_PDU_OK )
        {
            /* Compute length to be copied to UL */
            lAvailableAndCopiedDataToUl_u16 = lAvailablePduDataLength_u16;

            /* Copy data to UL */
            lPduAcceptance_en = SoAd_ProcessPduDataTPExecCopy(  StaticSocketId_uo,
                                                                PduData_pu8,
                                                                RxPduDesc_st,
                                                                lDataLenUpperLayer_uo,
                                                                &lAvailableAndCopiedDataToUl_u16 );
        }
    }

#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
    /* Rx buffer is configured and some TP data are already present in it */
    else
    {
        /* [SWS_SoAd_00568] */
        /* (2) if the SoAd receive buffer already contains TP data for this socket connection and is able to store all (newley) received data: */
        /* copy all received data to the SoAd receive buffer for later processing by SoAd_MainFunction() */

        /* Add the Pdu data in Rx buffer */
        lRetVal_en = SoAd_AddExtraTpDataInRxBuffer( StaticSocketId_uo,
                                                    &RxPduDesc_st,
                                                    PduData_pu8,
                                                    RemainingLength_u16 );

        /* If error occured (no space in the buffer) */
        /* Pdu will be discarded and fragments in Rx buffer deleted */
        lPduAcceptance_en = ( (lRetVal_en == E_OK)? (SOAD_PDU_OK) : (SOAD_PDU_DISCARD) );
    }
#endif

    /* --------------------------------------------- */
    /* Finalisation                                  */
    /* --------------------------------------------- */

    /* If no error occured during the copy and all data have been copied to the UL and to the buffer */
    if( lPduAcceptance_en == SOAD_PDU_OK )
    {
        /* Set the out argument of the function with the length of copied data to the UL */
        (*CopiedDataToUl_pu16) = lAvailableAndCopiedDataToUl_u16;
    }

    return lPduAcceptance_en;
}
#endif


/*********************************************************************************************************************************/
/** SoAd_ProcessPduDataTPHeaderEnabled()    - This function process the data contained in a received Pdu and pass it to the     **/
/**                                         Upper Layer (case of Upper Layer type TP with header enabled)                       **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo                       - SoAd static socket index                                                          **/
/** PduData_pu8                             - Pointer to memory containing Pdu data                                             **/
/** RemainingLength_u16                     - Remaining length in the message                                                   **/
/** RxPduDesc_st                            - Description of the received Pdu to be handled                                     **/
/**                                                                                                                             **/
/** Parameters (inout):                     None                                                                                **/
/**                                                                                                                             **/
/** Parameters (out):                                                                                                           **/
/** CopiedDataToUl_pu16                     - Length of data that has been copied to the Upper Layer buffer                     **/
/**                                                                                                                             **/
/** Return value:                           - SoAd_RxPduAcceptance_ten                                                          **/
/**                                             SOAD_PDU_OK: The Pdu data have been completely processed (UL or Rx buffer)      **/
/**                                             SOAD_PDU_FRAGMENTED_DATA: The Pdu data is fragmented, processing need to        **/
/**                                                                       continue later (fragment saved in buffer)             **/
/**                                             SOAD_PDU_DISCARD: The Pdu need to be discarded                                  **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
#if ( SOAD_TP_PRESENT != STD_OFF )
static SoAd_RxPduAcceptance_ten SoAd_ProcessPduDataTPHeaderEnabled ( SoAd_SoConIdType   StaticSocketId_uo,
                                                                     uint8 *            PduData_pu8,
                                                                     uint16             RemainingLength_u16,
                                                                     SoAd_RxPduDesc_tst RxPduDesc_st,
                                                                     uint16 *           CopiedDataToUl_pu16 )
{
    /* Declare local variables */
    SoAd_RxPduAcceptance_ten             lPduAcceptance_en;
    SoAd_SoConIdType                     lIdxDynSocket_uo;
    BufReq_ReturnType                    lBufferRetVal_en;
    uint16                               lAvailablePduDataLength_u16;
    uint16                               lAvailableAndCopiedDataToUl_u16;
    PduLengthType                        lDataLenUpperLayer_uo;
#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    Std_ReturnType                       lRetVal_en;
    uint16                               lRxBuffFreeLen_u16;
#endif

#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
    /* Get static socket configuration pointer */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);
#endif

    /* Get dynamic socket from static socket */
    lIdxDynSocket_uo = (*SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].SoAd_Prv_idxDynSocTable_puo);

    /* Initialize the Pdu acceptance flag to DISCARD */
    lPduAcceptance_en = SOAD_PDU_DISCARD;

    /* Initialize the available and copied length to 0 */
    lAvailableAndCopiedDataToUl_u16 = 0U;

    /* --------------------------------------------- */
    /* Check reception in progress                   */
    /* --------------------------------------------- */

    /* [SWS_SoAd_00571] */
    /* SoAd shall perform the following further actions within the SoAd_RxIndication() function for reception of a PDU from a socket connection with PDU Header mode */
    /* enabled to an upper layer using the TP-API: */
    /* (1) if no TP reception is in progress for the related socket connection */

    /* With header enabled, there should be no TP reception ongoing */
    if( SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].tpReceptionOngoing_b == FALSE )
    {
        /* --------------------------------------------- */
        /* Call StartOfReception                         */
        /* --------------------------------------------- */

        /* Step (a) of [SWS_SoAd_00571] */
        /* (a) After reception of a complete PDU Header, call <Up>_[SoAd][Tp]StartOfReception() of the related upper layer module with RxPduId set to the ID specified by */
        /*     the upper layer module for the PDU referenced by SoAdRxPduRef, set TpSduLength to the length specified in the PDU Header, and set PduInfoType.SduDataPtr and */
        /*     PduInfoType.SduLength to provide already received PDU data to the upper layer. */

        /* Initialization of data length available at Upper layer to 0 */
        lDataLenUpperLayer_uo = 0;

        /* Start of reception function to the Upper layer */
        lBufferRetVal_en = SoAd_CurrConfig_cpst->SoAdTp_StartOfReceptionCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxTpStartReceptionCbk_u8].StartOfReception_pfn (
                                    SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].ulRxPduId_uo,
                                    NULL_PTR,
                                    0U,
                                    &lDataLenUpperLayer_uo );

        /* In case of success */
        if( lBufferRetVal_en == BUFREQ_OK )
        {
            /* TP reception is now ongoing */
            SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].tpReceptionOngoing_b = TRUE;

            SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].idxRxPduTable_uo = RxPduDesc_st.RxPduId_uo;

            /* --------------------------------------------- */
            /* Check length                                  */
            /* --------------------------------------------- */

            /* Step (b) of [SWS_SoAd_00571] */
            /* (b) if not all data can be processed (i.e. forwarded to an upper layer or stored in a SoAd receive buffer), discard all received data, */
            /*     call <Up>_[SoAd][Tp]RxIndication()with the same RxPduId as used at <Up>_[SoAd][Tp]StartOfReception() and result set to E_NOT_OK and skip further processing */

            /* Compute available Pdu data length */
            lAvailablePduDataLength_u16 = ( (RxPduDesc_st.PduDataLength_u32 < (uint32)(RemainingLength_u16))? (uint16)(RxPduDesc_st.PduDataLength_u32) : (RemainingLength_u16) );

#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
            /* Get linear free length in SoAd Rx buffer */
            lRxBuffFreeLen_u16 = SoAd_GetRxBufLnrFreeLen( StaticSocketId_uo );
#endif

            /* If there is sufficient space to handle the data */
#if ( SOAD_RX_BUFF_ENABLE == STD_OFF )
            if( (uint32)(lDataLenUpperLayer_uo) >= (uint32)(lAvailablePduDataLength_u16) )
#else
            if( ((uint32)(lDataLenUpperLayer_uo) + (uint32)(lRxBuffFreeLen_u16)) >= (uint32)(lAvailablePduDataLength_u16) )
#endif
            {
                lPduAcceptance_en = SOAD_PDU_OK;
            }

            /* If there is not sufficient space to handle the data */
            else
            {
                /* Report DET error as per [SWS_SoAd_00693]*/
                SOAD_DET_REPORT_ERROR( SOAD_RX_INDICATION_API_ID, SOAD_E_NOBUFS )
            }
        }


        /* --------------------------------------------- */
        /* Copy Rx Data                                  */
        /* --------------------------------------------- */

        /* Step (c) of [SWS_SoAd_00571] */
        /* (c) call <Up>_[SoAd][Tp]CopyRxData() of the related upper layer module with the same RxPduId as used at <Up>_[SoAd][Tp]StartOf-Reception() and PduInfoType.SduDataPtr pointing to the PDU data provided by SoAd_RxIndication() */
        /* and PduInfoType.SduLength set to minimum of the received PDU data and the available receive buffer in the upper layer module specified by bufferSizePtr at <Up>_[SoAd][Tp]-StartOfReception()) */

        /* No error occured previously */
        if( lPduAcceptance_en == SOAD_PDU_OK )
        {
            /* Compute length to be copied to UL */
            lAvailableAndCopiedDataToUl_u16 = lAvailablePduDataLength_u16;

            /* Copy data to UL */
            lPduAcceptance_en = SoAd_ProcessPduDataTPExecCopy(  StaticSocketId_uo,
                                                                PduData_pu8,
                                                                RxPduDesc_st,
                                                                lDataLenUpperLayer_uo,
                                                                &lAvailableAndCopiedDataToUl_u16 );
        }
    }

#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
    /* TP reception in progress */
    else
    {
        /* [SWS_SoAd_00571] */
        /* (2) if a TP reception is in progress for the related socket connection and the related SoAd receive buffer is able to store all received data: */
        /* copy all received data to the related SoAd receive buffer for later processing by SoAd_MainFunction() */

        /* If Rx buffer is configured */
        if( lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR )
        {
            /* If data fragmentation is in progress */
            if( lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragReceptionInProgress_b == TRUE )
            {
                /* Save fragment in SoAd Rx buffer */
                lRetVal_en = SoAd_AddFragmentedDataInRxBuffer( StaticSocketId_uo,
                                                               PduData_pu8,
                                                               RemainingLength_u16,
                                                               NULL_PTR);
            }

            /* No fragmentation in progress */
            else
            {
                /* Add the complete new extra Pdu data in Rx buffer */
                lRetVal_en = SoAd_AddExtraTpDataInRxBuffer( StaticSocketId_uo,
                                                            &RxPduDesc_st,
                                                            PduData_pu8,
                                                            (uint16)RxPduDesc_st.PduDataLength_u32 );
            }

            /* If error occured (no space in the buffer) */
            /* Pdu will be discarded and fragments in Rx buffer deleted */
            lPduAcceptance_en = ( (lRetVal_en == E_NOT_OK)? (SOAD_PDU_DISCARD) : (SOAD_PDU_OK) );
        }
    }
#endif

    /* --------------------------------------------- */
    /* Finalisation                                  */
    /* --------------------------------------------- */

    /* No error occured previously */
    if( lPduAcceptance_en == SOAD_PDU_OK )
    {
        /* Set the out argument of the function with the length of copied data to the UL */
        (*CopiedDataToUl_pu16) = lAvailableAndCopiedDataToUl_u16;

#if ( (SOAD_RX_BUFF_ENABLE != STD_OFF) && (SOAD_TCP_PRESENT != STD_OFF) )
        /* If Rx buffer is configured and fragmented Pdu data reception is in progress in the SoAd Rx buffer */
        if( (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR)                            &&
            (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragReceptionInProgress_b == TRUE) )
        {
            /* The Pdu is data are not completely received yet, overwrite Pdu acceptance to SOAD_PDU_FRAGMENTED_DATA */
            /* (first part has been handled and now TpReceptionOngoing is true, new fragments will be received later) */
            lPduAcceptance_en = SOAD_PDU_FRAGMENTED_DATA;
        }
#endif
    }

    /* In case an error occured (it was not possible to forward data to UL or to store them in SoAd Rx buffer) */
    else
    {
        /* [SWS_SoAd_00572] */
        /* If <Up>_[SoAd][Tp]StartOfReception() does not return BUFREQ_OK for a socket connection with PDU Header mode enabled, SoAd shall simply discard all data of the PDU. */

        /* [SWS_SoAd_00573] */
        /* If <Up>_[SoAd][Tp]CopyRxData() does not return BUFREQ_OK for a socket connection with PDU Header mode enabled, */
        /* SoAd shall terminate the TP receive session, simply discard all data of the PDU and call <Up>_[SoAd][Tp]RxIndication() with E_NOT_OK */

        /* The exclusive area SoAd_TxRxReq is used to avoid race condition when there is Req for Tx/Rx or for discard Tx/Rx req */
        SchM_Enter_SoAd_TxRxReq();

        /* If TP reception was ongoing */
        if( SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].tpReceptionOngoing_b == TRUE )
        {
            /* Terminate the TP reception session */
            SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].tpReceptionOngoing_b = FALSE;

            SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].idxRxPduTable_uo = (PduIdType)0xFFFF;

            SchM_Exit_SoAd_TxRxReq();

            /* Call the upper layer with notify reslut type E_NOT_OK */
            SoAd_CurrConfig_cpst->SoAdTp_RxCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxTpRxIndication_u8].RxCallback_pfn(
                                            SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].ulRxPduId_uo,
                                            E_NOT_OK);
        }
        else
        {
            SchM_Exit_SoAd_TxRxReq();
        }
    }

    return lPduAcceptance_en;
}
#endif


/*********************************************************************************************************************************/
/** SoAd_ProcessPduDataTPExecCopy()    - This function executes the copying of the Pdu data to the UL and to the SoAd Rx buffer **/
/**                                      (case of Upper Layer type TP with header enabled or disabled)                          **/
/**                                                                                                                             **/
/** Parameters (in):                                                                                                            **/
/** StaticSocketId_uo                       - SoAd static socket index                                                          **/
/** PduData_pu8                             - Pointer to memory containing Pdu data                                             **/
/** RxPduDesc_st                            - Description of the received Pdu to be handled                                     **/
/** InitialDataLenUpperLayer_uo             - Initial available buffer size at Upper Layer                                      **/
/**                                                                                                                             **/
/** Parameters (inout):                                                                                                         **/
/** AvailableAndCopiedDataToUl_pu16         - Total available length to be copied as in argument and copied length as out       **/
/**                                                                                                                             **/
/** Parameters (out):                       None                                                                                **/
/**                                                                                                                             **/
/** Return value:                           - SoAd_RxPduAcceptance_ten                                                          **/
/**                                             SOAD_PDU_OK: The Pdu data have been copied                                      **/
/**                                             SOAD_PDU_DISCARD: The Pdu need to be discarded                                  **/
/**                                                                                                                             **/
/*********************************************************************************************************************************/
#if ( SOAD_TP_PRESENT != STD_OFF )
static SoAd_RxPduAcceptance_ten SoAd_ProcessPduDataTPExecCopy (  SoAd_SoConIdType   StaticSocketId_uo,
                                                                 uint8 *            PduData_pu8,
                                                                 SoAd_RxPduDesc_tst RxPduDesc_st,
                                                                 PduLengthType      InitialDataLenUpperLayer_uo,
                                                                 uint16 *           AvailableAndCopiedDataToUl_pu16 )
{
    /* Declare local variables */
    const SoAd_StaticSocConfigType_tst * lSoAdStaticSocConfig_cpst;
    SoAd_RxPduAcceptance_ten             lPduAcceptance_en;
    BufReq_ReturnType                    lBufferRetVal_en;
    PduInfoType                          lPduInfo_st;
    uint16                               lRemainingDataToBeCopiedToUL_u16;
    uint16                               lCopiedDataToUL_u16;
    SoAd_SoConIdType                     lIdxDynSocket_uo;
    PduLengthType                        lDataLenUpperLayer_uo;
    boolean lCopyError_b;
#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
    Std_ReturnType                       lRetVal_en;
#endif

    /* Get static socket configuration pointer */
    lSoAdStaticSocConfig_cpst = &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo]);

    /* Get dynamic socket from static socket */
    lIdxDynSocket_uo = (*SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].SoAd_Prv_idxDynSocTable_puo);

    /* Initialize the Pdu acceptance flag to DISCARD */
    lPduAcceptance_en = SOAD_PDU_DISCARD;

    /* Initialize the error flag to FALSE */
    lCopyError_b = FALSE;

    /* Initialize the copied length to the UL to 0 */
    lCopiedDataToUL_u16 = 0U;

    /* --------------------------------------------- */
    /* Copy data to UL                               */
    /* --------------------------------------------- */

    /* Step (c) of [SWS_SoAd_00568] */
    /* (c) Copy all received data which can be accepted by the upper layer module determined at (a) to the upper layer by calling <Up>_[SoAd][Tp]Copy-RxData() */

    /* Step (c) of [SWS_SoAd_00571] */
    /* (c) call <Up>_[SoAd][Tp]CopyRxData() of the related upper layer module with the same RxPduId as used at <Up>_[SoAd][Tp]StartOf-Reception() and PduInfoType.SduDataPtr pointing to the PDU data provided by SoAd_RxIndication() */
    /* and PduInfoType.SduLength set to minimum of the received PDU data and the available receive buffer in the upper layer module specified by bufferSizePtr at <Up>_[SoAd][Tp]-StartOfReception()) */

    /* Initialize the data length at UL */
    lDataLenUpperLayer_uo = InitialDataLenUpperLayer_uo;

    /* Compute the remaining length of data to be copied */
    lRemainingDataToBeCopiedToUL_u16 = (*AvailableAndCopiedDataToUl_pu16);

    /* Initialize Pdu Info */
    lPduInfo_st.SduDataPtr = PduData_pu8;
    lPduInfo_st.SduLength = (PduLengthType)( ((uint32)(lDataLenUpperLayer_uo) < (uint32)(lRemainingDataToBeCopiedToUL_u16))? (uint16)(lDataLenUpperLayer_uo) : (lRemainingDataToBeCopiedToUL_u16) );

    /* While there is some data to forward to the UL and there is available size in the UL buffer */
    while( (lRemainingDataToBeCopiedToUL_u16 > 0U) && (lDataLenUpperLayer_uo > 0U) )
    {
        /* RxCallback function to the Upper layer to copy the data */
        lBufferRetVal_en = SoAd_CurrConfig_cpst->SoAdTp_copyRxDataCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxTpCopyRxDataCbk_u8].soAdCopyRxDataCbk_pfn (
                                        SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].ulRxPduId_uo,
                                        &lPduInfo_st,
                                        &lDataLenUpperLayer_uo);

        /* In case of success */
        if( lBufferRetVal_en == BUFREQ_OK )
        {
            /* Increment the copied length to the UL */
            lCopiedDataToUL_u16 += lPduInfo_st.SduLength;

            /* Calculate the remaining data to be copied */
            lRemainingDataToBeCopiedToUL_u16 -= lPduInfo_st.SduLength;

            /* Update the Pdu Info for next iteration */
            lPduInfo_st.SduDataPtr = PduData_pu8 + lCopiedDataToUL_u16;
            lPduInfo_st.SduLength = (PduLengthType)( ((uint32)(lDataLenUpperLayer_uo) < (uint32)(lRemainingDataToBeCopiedToUL_u16))? (uint16)(lDataLenUpperLayer_uo) : (lRemainingDataToBeCopiedToUL_u16) );
        }

        /* In case of error */
        else
        {
            /* [SWS_SoAd_00570] */
            /* If <Up>_[SoAd][Tp]StartOfReception()or <Up>_[SoAd][Tp]CopyRxData() return with BUFREQ_E_NOT_OK for a socket connection with PDU Header mode disabled, SoAd shall */
            /* (a) disable further transmission or reception for this socket connection (i.e. new transmit requests shall be rejected with E_NOT_OK and received messages shall simply be discarded) and */
            /* (b) close the socket connection in the next SoAd_MainFunction(). */

            /* If the header is disabled and we get BUFREQ_E_NOT_OK */
            if( (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].headerEnable_b == FALSE)    &&
                (lBufferRetVal_en == BUFREQ_E_NOT_OK) )
            {
                /* Set the global variables to skip the further transmission and reception */
                SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].discardNewTxRxReq_b = TRUE;

                /* The exclusive area SoAd_OpenCloseReq is used to avoid race condition when socket open or close is requestedby UL or SoAd */
                SchM_Enter_SoAd_OpenCloseReq();

                /* Check if there is no close request from UL or Tcp/Ip*/
                if((SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].socConRequestedMode_en  != SOAD_EVENT_OFFLINEREQ_UL) && \
                   (SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].socConRequestedMode_en  != SOAD_EVENT_OFFLINEREQ_TCPIP))
                {
                    /* As an error occurred, so request need to be taken to abort the socket connection */
                    SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].socConRequestedMode_en = SOAD_EVENT_OFFLINEREQ_SOAD;

                }

                SchM_Exit_SoAd_OpenCloseReq();
            }

            /* Set the error flag to TRUE */
            lCopyError_b = TRUE;

            /* Stop the copying of the data */
            break;
        }
    }

    /* If no error occured during copy to UL */
    if( lCopyError_b == FALSE )
    {
#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
        /* ----------------------------------------------- */
        /* Check for starting of Tp reception in Rx buffer */
        /* ----------------------------------------------- */

        /* If the buffer is configured and not all Pdu data have been copied or received for the moment */
        if( (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR)    &&
            ((lRemainingDataToBeCopiedToUL_u16 > 0U) || (lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst->DataFragReceptionInProgress_b == TRUE)) )
        {
            /* Execute the starting of the Tp reception in the SoAd Rx buffer */
            SoAd_StartTpDataInRxBuffer(  StaticSocketId_uo,
                                         &RxPduDesc_st,
                                         lCopiedDataToUL_u16 );
        }
#endif


        /* --------------------------------------------- */
        /* Available data fully copied to UL             */
        /* --------------------------------------------- */
        if( lRemainingDataToBeCopiedToUL_u16 == 0U )
        {
            /* If header is enabled and complete Pdu data have been copied to UL */
            if( (SoAd_CurrConfig_cpst->SoAd_StaticSocGroupConfig_cpst[lSoAdStaticSocConfig_cpst->soConGrpId_uo].headerEnable_b == TRUE) &&
                ((uint32)(lCopiedDataToUL_u16) == RxPduDesc_st.PduDataLength_u32) )
            {
                /* Step (d) of [SWS_SoAd_00571] */
                /* (d) call <Up>_[SoAd][Tp]RxIndication() if the complete PDU has been forwarded to the upper layer, otherwise copy all received data which could not be forwarded */
                /*     to the upper layer to a SoAd receive buffer for later processing by SoAd_MainFunction() */

                /* Call the upper layer with notify reslut type E_OK */
                SoAd_CurrConfig_cpst->SoAdTp_RxCallbacks_cpst[SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].idxTpRxIndication_u8].RxCallback_pfn(
                                                SoAd_CurrConfig_cpst->SoAd_RxPduConfig_cpst[RxPduDesc_st.RxPduId_uo].ulRxPduId_uo,
                                                E_OK);

                /* Terminate the TP reception session */
                SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].tpReceptionOngoing_b = FALSE;

                SoAd_DyncSocConfig_ast[lIdxDynSocket_uo].idxRxPduTable_uo = (PduIdType)0xFFFF;
            }

            /* Set the out argument of the function */
            (*AvailableAndCopiedDataToUl_pu16) = lCopiedDataToUL_u16;

            /* Set the return value of the function to SOAD_PDU_OK */
            /* (No more processing is required because all the available data have been given to UL) */
            lPduAcceptance_en = SOAD_PDU_OK;
        }


        /* --------------------------------------------- */
        /* Available data partly copied to UL            */
        /* --------------------------------------------- */
        else
        {
#if ( SOAD_RX_BUFF_ENABLE != STD_OFF )
            /* --------------------------------------------- */
            /* Save remaining data in Rx buffer              */
            /* --------------------------------------------- */

            /* Step (d) of [SWS_SoAd_00568] */
            /* (d) Copy all remaining data (i.e. data which are received but not delivered to the upper layer) to a SoAd receive buffer for later processing by SoAd_MainFunction() */

            /* Step (d) of [SWS_SoAd_00571] */
            /* otherwise copy all received data which could not be forwarded to the upper layer to a SoAd receive buffer for later processing by SoAd_MainFunction() */

            /* Check if Rx buffer is configured */
            if( lSoAdStaticSocConfig_cpst->dynRxBuffTableEntry_pst != NULL_PTR )
            {
                /* Add the remaining data in Rx buffer */
                lRetVal_en = SoAd_AddTpDataInRxBuffer(  StaticSocketId_uo,
                                                        (PduData_pu8 + lCopiedDataToUL_u16),
                                                        lRemainingDataToBeCopiedToUL_u16 );

                /* If no error occured */
                if( lRetVal_en == E_OK )
                {
                    /* Set the out argument of the function */
                    (*AvailableAndCopiedDataToUl_pu16) = lCopiedDataToUL_u16;

                    /* Set the return value of the function to SOAD_PDU_OK */
                    /* (not all data have been given to the UL but the remaining part is successfully saved in the SoAd Rx buffer) */
                    lPduAcceptance_en = SOAD_PDU_OK;
                }
            }
#endif
        }
    }

    return lPduAcceptance_en;
}
#endif

/**
 ***********************************************************************************************************************
 * \Function Name : SoAd_PostActionAfterRxProcess()
 *
 * \Function description
 * This service shall check whether socket mode can be made to Reconnect or should be tried in next main function.
 *
 * Parameters (in):
 * \param   SoAd_SoConIdType
 * \arg     StaticSocketId_uo - static socket id whose Tcp config table or Udp config table is to be initiated
 *
 * Parameters (inout): None
 *
 * Parameters (out): None
 *
 * Return value: NONE
 *
 ***********************************************************************************************************************
 */
static void SoAd_PostActionAfterRxProcess( SoAd_SoConIdType StaticSocketId_uo)
{
    SoAd_DyncSocConfigType_tst * lSoAdDyncSocConfig_pst;

    /* variable to store the dynamic socket connection table index */
    SoAd_SoConIdType             lIdxDynSocket_uo;

    /* Get dynamic socket from static socket */
    lIdxDynSocket_uo = (*SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo].SoAd_Prv_idxDynSocTable_puo);

    /* Copy the global array index address to local pointer for further processing */
    lSoAdDyncSocConfig_pst = &(SoAd_DyncSocConfig_ast[lIdxDynSocket_uo]);

    /* The exclusive area SoAd_TxRxReq is used to avoid race condition when there is Req for Tx/Rx or for discard Tx/Rx req */
    SchM_Enter_SoAd_TxRxReq();

    /* check if there is no other ongoing Tx and Rx */
    if(
#if (SOAD_IF_PRESENT != STD_OFF )
       (lSoAdDyncSocConfig_pst->ifTransmissionOnGoing_b == FALSE) &&
#endif
       (lSoAdDyncSocConfig_pst->rxOngoingCnt_u8 == 1)
#if ( SOAD_IF_TRIGGER_TX_ENABLED != STD_OFF )
       && (lSoAdDyncSocConfig_pst->ifRgTxOngoing_b == FALSE)
#endif
       )
    {
        /* The exclusive area SocConModeProperties is used to avoid race condition when the soccket mode and submode are updated */
        SchM_Enter_SoAd_SocConModeProperties();

        /* Set the connection request state to reconnect such that it shall be tried in next main function */
        lSoAdDyncSocConfig_pst->socConSubMode_en = SOAD_SOCON_BOUND;

        lSoAdDyncSocConfig_pst->socConMode_en = SOAD_SOCON_RECONNECT;

        SchM_Exit_SoAd_SocConModeProperties();

        /* The exclusive area RemoteAddrUpdate is used to avoid race condition when remote address is modified */
        SchM_Enter_SoAd_RemoteAddrUpdate();

        SoAd_CopyRemoteAddrFromStaticSocToDynSoc( ((const SoAd_StaticSocConfigType_tst *) \
                &(SoAd_CurrConfig_cpst->SoAd_StaticSocConfig_cpst[StaticSocketId_uo])), lSoAdDyncSocConfig_pst);

        SchM_Exit_SoAd_RemoteAddrUpdate();

        SchM_Exit_SoAd_TxRxReq();
        /* Call a function which shall invoke the upper layer call back for mode change */
        SoAd_ModeChgCallbackToUl( StaticSocketId_uo, SOAD_SOCON_RECONNECT );

        /* check if requested mode is only for reconnect request in context of ongoing Tx/Rx*/
        if( SOAD_EVENT_RECONNECTREQ_SOAD == lSoAdDyncSocConfig_pst->socConRequestedMode_en)
        {
            /* reset flag to allow new Tx/Rx*/
            lSoAdDyncSocConfig_pst->discardNewTxRxReq_b = FALSE;
        }

    }
    else
    {
        /* Block further Tx and Rx*/
        lSoAdDyncSocConfig_pst->discardNewTxRxReq_b = TRUE;

        SchM_Exit_SoAd_TxRxReq();

        /* The exclusive area SoAd_OpenCloseReq is used to avoid race condition when socket open or close is requestedby UL or SoAd */
        SchM_Enter_SoAd_OpenCloseReq();

        /* Check if there is no close request from UL or Tcp/Ip*/
        if((lSoAdDyncSocConfig_pst->socConRequestedMode_en  != SOAD_EVENT_OFFLINEREQ_UL) && \
           (lSoAdDyncSocConfig_pst->socConRequestedMode_en  != SOAD_EVENT_OFFLINEREQ_TCPIP) && \
           (lSoAdDyncSocConfig_pst->socConRequestedMode_en  != SOAD_EVENT_OFFLINEREQ_SOAD))
        {
            /* As an error occurred, so request need to be taken to abort the socket connection */
            lSoAdDyncSocConfig_pst->socConRequestedMode_en = SOAD_EVENT_RECONNECTREQ_SOAD;

        }

        SchM_Exit_SoAd_OpenCloseReq();

    }
}


#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"

#endif /* SOAD_CONFIGURED */
