

#ifndef DCM_CFG_DSLDSD_H
#define DCM_CFG_DSLDSD_H


/*
 **********************************************************************************************
 *  DCM Defines generated from configuration
 **********************************************************************************************
 */

#define     DCM_CFG_ON                          1u
#define     DCM_CFG_OFF                         0u
#define     DCM_CFG_S3MAX_TIME                      5000000u            /* S3 max time (in Micro sec) */
#define     DCM_CFG_DET_SUPPORT_ENABLED             DCM_CFG_ON
#define     DCM_CFG_TASK_TIME_US                    10000u              /* DCM task time in Micro sec */
#define     DCM_CFG_TASK_TIME_MS                    10u             /* DCM task time in Milli sec */
#define     DCM_CFG_KWP_ENABLED                     DCM_CFG_OFF
#define     DCM_CFG_KLINE_ENABLED                   DCM_CFG_OFF
#define     DCM_CFG_RESTORING_ENABLED               DCM_CFG_OFF         /* Restoring of BootLoader information is Disabled */
#define     DCM_CFG_STORING_ENABLED                 DCM_CFG_ON          /* Storing of BootLoader information is Enabled */
#define     DCM_CFG_NR_CONF_ENABLED                 DCM_CFG_OFF         /* Call of DcmAppl_DcmConfirmation_DcmNegResp callback is disabled for DSD generated negative responses */
#define     DCM_CFG_APPLTXCONF_REQ                  DCM_CFG_ON          /* Call of DcmAppl_DcmConfirmation callback is enabled */
#define     DCM_CFG_VERSION_INFO_API                DCM_CFG_OFF         /* Dcm Version Info API is disabled */
#define     DCM_CFG_SUPPLIER_NOTIFICATION_ENABLED   DCM_CFG_OFF

#define     DCM_CFG_INDICATION_DATASIZE_UINT32   DCM_CFG_OFF

#define     DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS                0
#define     DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED DCM_CFG_OFF
#define     DCM_CFG_CALL_BACK_NUM_PORTS                0
#define     DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS   0
#define     DCM_CFG_RBA_DIAGADAPT_SUPPORT_ENABLED       DCM_CFG_OFF         /* DiagAdapt Support from DCM is disabled */
#define     DCM_CFG_RBA_DEM_SR_ENABLED       DCM_CFG_OFF         /* Sender-Receiver Interface for Dem is disabled */
#define     DCM_CFG_RESPOND_ALLREQUEST              DCM_CFG_ON          /* Expanded range of SID Support */
#define     DCM_CFG_RESPOND_REQ_AFTERECURESET       DCM_CFG_ON          /* Respond to request after successful EcuReset service processing */

#define     DCM_CFG_ECURESET_TIME                   0u

#define     DCM_PAGEDBUFFER_ENABLED             DCM_CFG_OFF         /* Paged buffer Support is disabled */

#define     DCM_CFG_SECURITY_STOREDELAYCOUNTANDTIMERONJUMP          DCM_CFG_OFF         /* DelayCount and DelayTimer values shall not be stored during Boot Loader interactions */
#define     DCM_CFG_RTESUPPORT_ENABLED              DCM_CFG_ON          /* RTE Support within Dcm enabled */
#define     DCM_CFG_DSP_DDDISTORINGTONVRAM_ENABLED          DCM_CFG_OFF         /* Storing/restoring of DDDID definition to/from NvM is disabled */


#define 	DCM_SEPARATEBUFFERFORTXANDRX_ENABLED        	DCM_CFG_OFF			/* Same buffer for transmission and reception will be used */

#define     DCM_BUFQUEUE_ENABLED            DCM_CFG_OFF         /* Queuing of requests in Dcm is disabled  */

#define     DCM_CALLAPPLICATIONONREQRX_ENABLED            DCM_CFG_OFF         /* Calling the application while receiving the request in Dcm is disabled */

#define    DCM_PARALLELPROCESSING_ENABLED                 DCM_CFG_OFF /* Parallel Processing of protocols are disabled */





#define DCM_CFG_SUPPRESS_NRC(NegRespCode) (((NegRespCode) == 0x11) ||((NegRespCode) == 0x12) ||((NegRespCode) == 0x31) ||((NegRespCode) == 0x7E) ||((NegRespCode) == 0x7F) )   /* Suppress NRCs during Functional addressing */


#define DCM_CFG_NUM_CONN                    1u                /* Number of connections in DCM */

#define DCM_NUM_COMM_CHANNEL                    1                /* Number of unique comm channel references in DCM */

#define DCM_CFG_NUM_RX_PDUID                2u                /* Number of rx pdu ids DCM has */

#define DCM_INDEX_FUNC_RX_PDUID 1u               /* Index from where functional rx pdu id will starts */

#define DCM_NUM_TX_PDUID                1u  /* Total No. of Tx Pdu Ids supported within Dcm */


#ifndef DcmConf_DcmDslProtocolRow_DcmDslProtocolRow
#define DcmConf_DcmDslProtocolRow_DcmDslProtocolRow            DCM_UDS_ON_CAN
#endif

#define DCM_CFG_NUM_PROTOCOL                1u               /* Number of protocols DCM has */
#define DCM_CFG_NUM_SID_TABLE              1u               /* Number of SID table configured */

#define DCM_CFG_NUMPDUINFO_STRUCT 1u  /*Number of Pdu Info Structure Required */


#define     DCM_CFG_MAX_WAITPEND                    5u                 /* Max. no. of Wait Pend that can be sent before sending a General Reject NRC */

#define     DCM_CFG_WAIT_FOR_VIN                    DCM_CFG_MAX_WAITPEND               /* Max. no. of Wait Pend that can be sent before sending a General Reject NRC */

/* Symbolic name generation for Session */

#ifndef DcmConf_DcmDspSessionRow_DEFAULT_SESSION
#define DcmConf_DcmDspSessionRow_DEFAULT_SESSION          0x1
#endif

#ifndef DcmConf_DcmDspSessionRow_PROGRAMMING_SESSION
#define DcmConf_DcmDspSessionRow_PROGRAMMING_SESSION          0x2
#endif

#ifndef DcmConf_DcmDspSessionRow_EXTENDED_DIAGNOSTIC_SESSION
#define DcmConf_DcmDspSessionRow_EXTENDED_DIAGNOSTIC_SESSION          0x3
#endif




#define DCM_CFG_DEFAULT_P2MAX_TIME          50000u          /* P2 max time of default session in micro seconds */
#define DCM_CFG_DEFAULT_P2STARMAX_TIME      5000000u       /* P2 star max time of default session in micro seconds */
#define DCM_CFG_NUM_UDS_SESSIONS            3u               /* Number of sessions configured in DSC service */
#if (DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
#define DCM_CFG_NUM_KWP_SESSIONS            1u              /*Number of sessions for KWP protocol */

#ifndef     DCM_NUM_KWP_SESSIONS
#define     DCM_NUM_KWP_SESSIONS                    DCM_CFG_NUM_KWP_SESSIONS
#endif

#endif


/* Symbolic name generation for Security Level */

#ifndef DcmConf_DcmDspSecurityRow_UnlockedL1
#define DcmConf_DcmDspSecurityRow_UnlockedL1        0x1
#endif
#ifndef DcmConf_DcmDspSecurityRow_Unlocked_L3
#define DcmConf_DcmDspSecurityRow_Unlocked_L3        0x3
#endif
#define DCM_CFG_NUM_SECURITY_LEVEL          3u               /* Number of security levels configured in SECA service*/




#define DCM_CFG_IN_PARAM_MAXLEN  0u   /* Maximum length of in Parameters */

#define DCM_CFG_IN_PARAM_OBDMAXLEN u 




    
#define DCM_CFG_ATOMICREAD_DID 0u    /*Number of Configured Atomic DIDs having Read function */
    

    
#define DCM_CFG_ATOMICWRITE_DID 0u  /*Number of Configured Atomic DIDs having Write function */
    
    

#define     DCM_CFG_OSTIMER_USE                     FALSE


#define DCM_ROE_ENABLED                 DCM_CFG_OFF             /* Enable or Disable ROE */




#define DCM_CFG_ROETYPE2_ENABLED            DCM_CFG_OFF     /* Enable or Disable ROE TYPE 2 */




#define DCM_CFG_RDPI_ENABLED           DCM_CFG_OFF     /* Enable or Disable RDPI TYPE 2 */

#define DCM_CFG_MAX_PERIODIC_DID_READ        0u  /* Number of periodic identifiers allowed in a request */
#define DCM_CFG_MAX_DID_SCHEDULER            0u  /* Number of periodic identifiers allowed to be scheduled simultaneously */


#define DCM_UDS_DCMDSLPROTOCOLROW     0xF0           /*User specific Protocol ID */


#ifndef     DCM_ETAS_SERVICETABLE
#define     DCM_ETAS_SERVICETABLE     0x0
#endif




#ifdef DcmConf_DcmDslProtocolRx_UdsRxPhy_PduR2Dcm_Can_Network_ETAS_Channel_CAN
#error "Symbolic names generated for DcmDslProtocolRxPduId is incorrect as the short names of configured DcmDslProtocolRx in not unique, Run Generate Id and Config Code generation once again."
#else
#define DcmConf_DcmDslProtocolRx_UdsRxPhy_PduR2Dcm_Can_Network_ETAS_Channel_CAN       0x0      /*Rx PDU ID*/
#endif
#ifdef DcmConf_DcmDslProtocolRx_UdsRxFnc_PduR2Dcm_Can_Network_ETAS_Channel_CAN
#error "Symbolic names generated for DcmDslProtocolRxPduId is incorrect as the short names of configured DcmDslProtocolRx in not unique, Run Generate Id and Config Code generation once again."
#else
#define DcmConf_DcmDslProtocolRx_UdsRxFnc_PduR2Dcm_Can_Network_ETAS_Channel_CAN       0x1      /*Rx PDU ID*/
#endif

/* Symbolic Name generation for TxConfirmtionPduId */

#ifdef DcmConf_DcmDslProtocolTx_UdsTxPhy_Dcm2PduR_Can_Network_ETAS_Channel_CAN
#error "Symbolic names generated for DcmDslTxConfirmationPduId is incorrect as the short names of configured DcmDslProtocolTx in not unique, Run Generate Id and Config Code generation once again."
#else
#define DcmConf_DcmDslProtocolTx_UdsTxPhy_Dcm2PduR_Can_Network_ETAS_Channel_CAN         0x0       /*Tx Confirmation PDU ID*/
#endif


#define     DCM_CFG_PROTOCOL_PREMPTION_ENABLED      DCM_CFG_OFF


#define     DCM_CFG_DSL_BUFFER__DCMDSLPROTOCOLROWRXBUFFER 255   /* Buffer Size */
#define     DCM_CFG_DSL_BUFFER__DCMDSLPROTOCOLROWTXBUFFER 255   /* Buffer Size */

#define     DCM_CFG_DSPUDSSUPPORT_ENABLED           DCM_CFG_ON      /* Enable or Disable UDS services in DSP */


#define DCM_CFG_DSD_MODERULESERVICE_ENABLED         DCM_CFG_OFF     /* Mode Rules are not referred by services of Dcm */
#define DCM_CFG_DSD_MODERULESUBFNC_ENABLED         DCM_CFG_OFF      /* Mode Rules are not referred by Subservices of Dcm */

#define     DCM_CFG_SIGNAL_DEFAULT_VALUE            0u    /* Default Value for Signals with Gap. To reset signal data with default value */


#define     DCM_CFG_DSPOBDSUPPORT_ENABLED           DCM_CFG_OFF     /* Enabled or Disabled based on OBD services configured in DSP */



#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint8 Dcm_DspArraySignal_au8 [];
extern sint8 Dcm_DspArraySignal_as8 [];
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint16 Dcm_DspArraySignal_au16[];
extern sint16 Dcm_DspArraySignal_as16[];
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint32 Dcm_DspArraySignal_au32[];
extern sint32 Dcm_DspArraySignal_as32[];
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"






#define DCM_CFG_RXPDU_SHARING_ENABLED   DCM_CFG_OFF


/* This is the macro the customer/production team have to modify the
************Enable or Disable the Postbuild support*****************/
#define     DCM_CFG_POSTBUILD_SUPPORT               DCM_CFG_OFF
/* Bit Masks for different DCM configuration sets */
#define     DCM_CFG_CONFIGSET1                      0x01                /*Bit mask for DCM configuration set1*/
#define     DCM_CFG_CONFIGSET2                      0x02                /*Bit mask for DCM configuration set2*/
#define     DCM_CFG_CONFIGSET3                      0x04                /*Bit mask for DCM configuration set3*/
#define     DCM_CFG_CONFIGSET4                      0x08                /*Bit mask for DCM configuration set4*/
#define     DCM_CFG_CONFIGSET5                      0x10                /*Bit mask for DCM configuration set5*/
#define     DCM_CFG_CONFIGSET6                      0x20                /*Bit mask for DCM configuration set6*/
#define     DCM_CFG_CONFIGSET7                      0x40                /*Bit mask for DCM configuration set7*/
#define     DCM_CFG_CONFIGSET8                      0x80                /*Bit mask for DCM configuration set8*/


/* Mapping old macros to coding guideline compliant macros to ensure backward compatibility */
            /* Obsolete */
#ifndef         DCM_S3MAX_TIME
#define     DCM_S3MAX_TIME                          DCM_CFG_S3MAX_TIME
#endif
#ifndef     DCM_DET_SUPPORT_ENABLED
#define     DCM_DET_SUPPORT_ENABLED                 DCM_CFG_DET_SUPPORT_ENABLED
#endif
#ifndef     DCM_TASK_TIME_US
#define     DCM_TASK_TIME_US                        DCM_CFG_TASK_TIME_US
#endif
#ifndef     DCM_TASK_TIME_MS
#define     DCM_TASK_TIME_MS                        DCM_CFG_TASK_TIME_MS
#endif
#ifndef     DCM_KWP_ENABLED
#define     DCM_KWP_ENABLED                         DCM_CFG_KWP_ENABLED
#endif
#ifndef     DCM_RESTORING_ENABLED
#define     DCM_RESTORING_ENABLED                   DCM_CFG_RESTORING_ENABLED
#endif
#ifndef     DCM_STORING_ENABLED
#define     DCM_STORING_ENABLED                     DCM_CFG_STORING_ENABLED
#endif
#ifndef     DCM_NR_CONF_ENABLED
#define     DCM_NR_CONF_ENABLED                     DCM_CFG_NR_CONF_ENABLED
#endif
#ifndef     DCM_APPLTXCONF_REQ
#define     DCM_APPLTXCONF_REQ                      DCM_CFG_APPLTXCONF_REQ
#endif
#ifndef     DCM_VERSION_INFO_API
#define     DCM_VERSION_INFO_API                    DCM_CFG_VERSION_INFO_API
#endif
#ifndef     DCM_SUPPLIER_NOTIFICATION_ENABLED
#define     DCM_SUPPLIER_NOTIFICATION_ENABLED       DCM_CFG_SUPPLIER_NOTIFICATION_ENABLED
#endif
#ifndef     DCM_MANUFACTURER_NOTIFICATION_ENABLED
#define     DCM_MANUFACTURER_NOTIFICATION_ENABLED   DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED
#endif
#ifndef     DCM_RBA_DIAGADAPT_SUPPORT_ENABLED
#define     DCM_RBA_DIAGADAPT_SUPPORT_ENABLED       DCM_CFG_RBA_DIAGADAPT_SUPPORT_ENABLED
#endif
#ifndef     DCM_RESPOND_ALLREQUEST
#define     DCM_RESPOND_ALLREQUEST                  DCM_CFG_RESPOND_ALLREQUEST
#endif
#ifndef     DCM_RTESUPPORT_ENABLED
#define     DCM_RTESUPPORT_ENABLED                  DCM_CFG_RTESUPPORT_ENABLED
#endif

#ifndef     DCM_NUM_CONN
#define     DCM_NUM_CONN                            DCM_CFG_NUM_CONN
#endif
#ifndef     DCM_NUM_RX_PDUID
#define     DCM_NUM_RX_PDUID                        DCM_CFG_NUM_RX_PDUID
#endif
#ifndef     DCM_NUM_PROTOCOL
#define     DCM_NUM_PROTOCOL                        DCM_CFG_NUM_PROTOCOL
#endif
#ifndef     DCM_NUM_SID_TABLE
#define     DCM_NUM_SID_TABLE                       DCM_CFG_NUM_SID_TABLE
#endif
#ifndef     DCM_NUMPDUINFO_STRUCT
#define     DCM_NUMPDUINFO_STRUCT                   DCM_CFG_NUMPDUINFO_STRUCT
#endif
#ifndef     DCM_MAX_WAITPEND
#define     DCM_MAX_WAITPEND                        DCM_CFG_MAX_WAITPEND
#endif
#ifndef     DCM_DEFAULT_P2MAX_TIME
#define     DCM_DEFAULT_P2MAX_TIME                  DCM_CFG_DEFAULT_P2MAX_TIME
#endif
#ifndef     DCM_DEFAULT_P2STARMAX_TIME
#define     DCM_DEFAULT_P2STARMAX_TIME              DCM_CFG_DEFAULT_P2STARMAX_TIME
#endif
#ifndef     DCM_NUM_UDS_SESSIONS
#define     DCM_NUM_UDS_SESSIONS                    DCM_CFG_NUM_UDS_SESSIONS
#endif
#ifndef     DCM_NUM_SECURITY_LEVEL
#define     DCM_NUM_SECURITY_LEVEL                  DCM_CFG_NUM_SECURITY_LEVEL
#endif
#ifndef     DCM_OSTIMER_USE
#define     DCM_OSTIMER_USE                         DCM_CFG_OSTIMER_USE
#endif
#ifndef     DCM_ROETYPE2_ENABLED
#define     DCM_ROETYPE2_ENABLED                    DCM_CFG_ROETYPE2_ENABLED
#endif
#ifndef     DCM_RDPI_ENABLED
#define     DCM_RDPI_ENABLED                        DCM_CFG_RDPI_ENABLED
#endif
#ifndef     DCM_MAX_PERIODIC_DID_READ
#define     DCM_MAX_PERIODIC_DID_READ               DCM_CFG_MAX_PERIODIC_DID_READ
#endif
#ifndef     DCM_MAX_DID_SCHEDULER
#define     DCM_MAX_DID_SCHEDULER                   DCM_CFG_MAX_DID_SCHEDULER
#endif
#ifndef     DCM_ROE_INTERMESSAGE_TIME
#define     DCM_ROE_INTERMESSAGE_TIME               DCM_CFG_ROE_INTERMESSAGE_TIME
#endif
#ifndef     DCM_PROTOCOL_PREMPTION_ENABLED
#define     DCM_PROTOCOL_PREMPTION_ENABLED          DCM_CFG_PROTOCOL_PREMPTION_ENABLED
#endif
#ifndef     DCM_DSL_BUFFER_DCMDSLBUFFER_0
#define     DCM_DSL_BUFFER_DCMDSLBUFFER_0           DCM_CFG_DSL_BUFFER__DCMDSLBUFFER_0
#endif
#ifndef     DCM_DSL_BUFFER_DCMDSLBUFFER_1
#define     DCM_DSL_BUFFER_DCMDSLBUFFER_1           DCM_CFG_DSL_BUFFER__DCMDSLBUFFER_1
#endif
#ifndef     DCM_DSL_BUFFER_DCMDSLBUFFER_2
#define     DCM_DSL_BUFFER_DCMDSLBUFFER_2           DCM_CFG_DSL_BUFFER__DCMDSLBUFFER_2
#endif
#ifndef     DCM_DSL_BUFFER_DCMDSLBUFFER_3
#define     DCM_DSL_BUFFER_DCMDSLBUFFER_3           DCM_CFG_DSL_BUFFER__DCMDSLBUFFER_3
#endif
#ifndef     DCM_DSL_BUFFER_DCMDSLBUFFER_4
#define     DCM_DSL_BUFFER_DCMDSLBUFFER_4           DCM_CFG_DSL_BUFFER__DCMDSLBUFFER_4
#endif
#ifndef     DCM_DSP_UDSSUPPORT_ENABLED
#define     DCM_DSP_UDSSUPPORT_ENABLED              DCM_CFG_DSPUDSSUPPORT_ENABLED
#endif
#ifndef     DCM_DSP_OBDSUPPORT_ENABLED
#define     DCM_DSP_OBDSUPPORT_ENABLED              DCM_CFG_DSPOBDSUPPORT_ENABLED
#endif
#ifndef     DCM_SIGNAL_DEFAULT_VALUE
#define     DCM_SIGNAL_DEFAULT_VALUE                DCM_CFG_SIGNAL_DEFAULT_VALUE
#endif
#ifndef     DCM_RXPDU_SHARING_ENABLED
#define     DCM_RXPDU_SHARING_ENABLED               DCM_CFG_RXPDU_SHARING_ENABLED
#endif
#ifndef     DCM_CONFIGSET1
#define     DCM_CONFIGSET1                          DCM_CFG_CONFIGSET1
#endif
#ifndef     DCM_CONFIGSET2
#define     DCM_CONFIGSET2                          DCM_CFG_CONFIGSET2
#endif
#ifndef     DCM_CONFIGSET3
#define     DCM_CONFIGSET3                          DCM_CFG_CONFIGSET3
#endif
#ifndef     DCM_CONFIGSET4
#define     DCM_CONFIGSET4                          DCM_CFG_CONFIGSET4
#endif
#ifndef     DCM_CONFIGSET5
#define     DCM_CONFIGSET5                          DCM_CFG_CONFIGSET5
#endif
#ifndef     DCM_CONFIGSET6
#define     DCM_CONFIGSET6                          DCM_CFG_CONFIGSET6
#endif
#ifndef     DCM_CONFIGSET7
#define     DCM_CONFIGSET7                          DCM_CFG_CONFIGSET7
#endif
#ifndef     DCM_CONFIGSET8
#define     DCM_CONFIGSET8                          DCM_CFG_CONFIGSET8
#endif
#ifndef     DCM_SHARED_RX_PDUID
#define     DCM_SHARED_RX_PDUID         DCM_CFG_SHARED_RX_PDUID
#endif
#ifndef     DCM_POSTBUILD_SUPPORT
#define     DCM_POSTBUILD_SUPPORT       DCM_CFG_POSTBUILD_SUPPORT
#endif
#ifndef     DCM_ROE_RESUME_RXPDUID
#define     DCM_ROE_RESUME_RXPDUID      DCM_CFG_ROE_RESUME_RXPDUID
#endif
#ifndef     DCM_ROERDPI_TIMEOUT
#define     DCM_ROERDPI_TIMEOUT         DCM_CFG_GET_TIMEOUT
#endif
#ifndef     DCM_NUM_RDPITYPE2_TXPDU
#define     DCM_NUM_RDPITYPE2_TXPDU     DCM_CFG_NUM_RDPITYPE2_TXPDU
#endif
#ifndef     DCM_PERIODICTX_SLOWRATE
#define     DCM_PERIODICTX_SLOWRATE     DCM_CFG_PERIODICTX_SLOWRATE
#endif
#ifndef     DCM_PERIODICTX_MEDIUMRATE
#define     DCM_PERIODICTX_MEDIUMRATE   DCM_CFG_PERIODICTX_MEDIUMRATE
#endif
#ifndef     DCM_PERIODICTX_FASTRATE
#define     DCM_PERIODICTX_FASTRATE     DCM_CFG_PERIODICTX_FASTRATE
#endif



#endif /* _DCM_CFG_DSLDSD_H */

