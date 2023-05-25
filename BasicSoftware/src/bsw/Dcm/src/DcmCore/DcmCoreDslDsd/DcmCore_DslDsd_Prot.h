

#ifndef DCMCORE_DSLDSD_PROT_H
#define DCMCORE_DSLDSD_PROT_H


/**************************************************************************************************/
/* Included  header files                                                                         */
/**************************************************************************************************/

#if (DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)
#include "NvM.h"
#endif
 #if (DCM_CFG_DET_SUPPORT_ENABLED != DCM_CFG_OFF)
#include "Det.h"
#endif
#include "Dcm_Dsd.h"
/*
 ***************************************************************************************************
 *    Internal  definitions
 ***************************************************************************************************
 */
/* Endianness conversion for UINT16/SINT16 data */

#define DCM_ENDIANNESSCONVERSION16(data) ((data) = rba_BswSrv_ByteOrderSwap16((uint16)(data)))
/* Endianness conversion for UINT32/SINT32 data */

#define DCM_ENDIANNESSCONVERSION32(data) ((data) = rba_BswSrv_ByteOrderSwap32(data))

/* Abstraction to the MemSet and MemCopy Library function */
#define DCM_MEMCOPY(xDest_pv,xSrc_pcv,numBytes_u32)         (void)rba_BswSrv_MemCopy((xDest_pv),(xSrc_pcv),(uint32)(numBytes_u32))
#define DCM_MEMSET(xDest_pv,xPattern_u32,numBytes_u32)              (void)rba_BswSrv_MemSet((xDest_pv),(xPattern_u32),(uint32)(numBytes_u32))
#define DCM_UNUSED_PARAM(P)   ((void)(P))

#if(DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)||(DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
/* Types of storing used for Jump to Boot */
#define DCM_NOTVALID_TYPE     0x00u  /* Boot Loader is not active */
#define DCM_WARMREQUEST_TYPE  0x01u  /* Warm Request Type */
#define DCM_WARMINIT_TYPE     0x02u  /* Warm Init Type */
#define DCM_WARMRESPONSE_TYPE 0x03u  /* Warm Response Type */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_ProgConditionsType Dcm_ProgConditions_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if(DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)
typedef enum
{
    DCM_BOOT_IDLE = 0,           /* IDLE state */
    DCM_BOOT_PROCESS_RESET,      /* Process the Store Type and Trigger Force Response Pend */
    DCM_BOOT_SENDFORCEDRESPPEND, /* State where ForcedRespPend can be triggered */
    DCM_BOOT_WAIT,               /* Wait for confirmation of Response Pend */
    DCM_BOOT_STORE_WARMREQ,      /* Store Request for Warm Request type */
    DCM_BOOT_STORE_WARMINIT,     /* Store protocol information for Warm Init Type */
    DCM_BOOT_STORE_WARMRESP,     /* Store Response for Warm Response Type */
    DCM_BOOT_ERROR,              /* Process the error happened before the jump */
    DCM_BOOT_WAIT_FOR_RESET,     /* Wait till the reset happens*/
    DCM_BOOT_PERFORM_RESET,       /* State to do reset in case of Warm Request/Response */
    DCM_BOOT_PREPARE_RESET       /* State to check for conditions to send Forced RespPend before reset for  Warm Request/Response */
}Dcm_BootLoaderStates_ten;

#define DCM_JUMPTOOEMBOOTLOADER            0x00u
#define DCM_JUMPTOSYSSUPPLIERBOOTLOADER 0x01u
#define DCM_JUMPDRIVETODRIVE            0x02u
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_BootLoaderStates_ten Dcm_BootLoaderState_en;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_JumpToBootLoader(uint8 dataBootType_u8,Dcm_NegativeResponseCodeType * dataNegRespCode_u8);
extern void Dcm_ResetBootLoader(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
/* FC_VariationPoint_START */
/* API ID for OBD services in order to report development errors to DET module */
#define DCM_OBDMODE01_ID    0x81u
#define DCM_OBDMODE02_ID    0x82u
#define DCM_OBDMODE37A_ID   0x83u
#define DCM_OBDMODE04_ID    0x84u
#define DCM_OBDMODE06_ID    0x86u
#define DCM_OBDMODE08_ID    0x88u
#define DCM_OBDMODE09_ID    0x89u
/* FC_VariationPoint_END */

/* API ID for read did in order to report development errors to DET module */
#define DCM_RDBI_ID         0x8Au

#define DCM_WARMSTART_ID        0x8Bu
#define DCM_BOOTLOADER_ID       0x8Cu
#define DCM_COPYRXDATA_ID       0x8Du
#define DCM_KWPTIMING_ID        0x8Eu
#define DCM_PAGEDBUFFER_ID      0x8Fu
#define DCM_RDPI_ID             0x90u
#define DCM_ROE_ID              0x91u
#define DCM_ROEPROCESS_ID       0x92u
#define DCM_TPRXIND_ID          0x93u
#define DCM_SETSRVTABLE_ID      0x94u
#define DCM_SETSESSION_ID       0x95u
#define DCM_PROCESSINGDONE_ID   0x96u
#define DCM_CC_ID               0x97u
#define DCM_CDTCS_ID            0x98u
#define DCM_DDDI_ID             0x99u
#define DCM_DSC_ID              0x9Au
#define DCM_GETP2TIMINGS_ID     0x9Bu
#define DCM_ER_ID               0x9Cu
#define DCM_IOCBI_ID            0x9Du
#define DCM_WDBI_ID             0x9Eu
#define DCM_COPYTXDATA_ID       0x9Fu
#define DCM_CHKUSEDCOREMAIN_ID  0xA0u
#define DCM_TRANSFERDATA_ID     0xA1u
#define DCM_TRANSFEREXIT_ID     0xA2u
#define DCM_STARTOFRECEPTION_ID 0xA3u
#define DCM_TPTXCONFIRMATION_ID 0x48u
#define DCM_TPRXINDICATION_ID   0xA5u
#define DCM_SETSECURITYLEVEL    0xA6u
#define DCM_SETSESSIONLEVEL     0xA7u
#define DCM_RDTC_ID             0xA8u
#define DCM_SECURITYACCESS_ID   0xA9u
#define DCM_TXCONFIRMATION_ID   0xAAu
#define DCM_SUPPLIERNOTIFICATION_ID 0xABu
#define DCM_MANUFACTURENOTIFICATION_ID 0xACu


/* error ids for DET API interfaces, OBD services report the development errors to DET module */
#define DCM_DEFAULTSESSION                        0x01u
#define DCM_E_INTERFACE_TIMEOUT                   0x01u
#define DCM_E_INTERFACE_RETURN_VALUE              0x02u
#define DCM_E_INTERFACE_BUFFER_OVERFLOW           0x03u
#define DCM_E_UNINIT                              0x05u
#define DCM_E_PARAM                               0x06u
#define DCM_E_PARAM_POINTER                       0x07u
#define DCM_E_INIT_FAILED                         0x08u
#define DCM_E_SET_PROG_CONDITIONS_FAIL            0x09u
#define DCM_E_MIXED_MODE                          0x0Au
#define DCM_E_WRONG_STATUSVALUE                   0x0Bu
#define DCM_E_PROTOCOL_NOT_FOUND                  0x0Cu
#define DCM_E_NVM_UPDATION_NOT_OK                 0x0Du
#define DCM_E_FULLCOMM_DISABLED                   0x0Eu
#define DCM_E_PROTOCOL_NOT_STARTED                0x10u
#define DCM_E_PSUEDO_RECEPTION                    0x11u
#define DCM_E_SERVICE_TABLE_NOT_SET               0x12u
#define DCM_E_SESSION_NOT_CONFIGURED              0x13u
#define DCM_E_SUBNET_NOT_SUPPORTED                0x14u
#define DCM_E_DDDI_NOT_CONFIGURED                 0x15u
#define DCM_E_EXCEEDED_MAX_RECORDS                0x16u
#define DCM_E_NOT_SUPPORTED_IN_CURRENT_SESSION    0x17u
#define DCM_E_INVALID_ADDRLENGTH_FORMAT           0x18u
#define DCM_E_CONTROL_FUNC_NOT_CONFIGURED         0x19u
#define DCM_E_INVALID_CONTROL_PARAM               0x1Au
#define DCM_E_NO_WRITE_ACCESS                     0x1Bu
#define DCM_E_RET_E_INFRASTRUCTURE_ERROR          0x1Cu
#define DCM_E_INVALID_CONTROL_DATA                0x1Du
/* error Ids for DET API interfaces, Rdbi service report the development errors to DET module */
#define DCM_E_RET_E_NOT_OK                        0x1Eu
#define DCM_E_DCMRXPDUID_RANGE_EXCEED             0x20u
#define DCM_E_DCMTXPDUID_RANGE_EXCEED             0x21u
#define DCM_E_NO_READ_ACCESS                      0x22u
#define DCM_E_SERVICE_TABLE_OUTOFBOUNDS           0x23u
#define DCM_E_SECURITYLEVEL_OUTOFBOUNDS           0x24u
#define DCM_E_RET_E_PENDING                       0x25u
#define DCM_E_INVALID_LENGTH                      0x26u
#define DCM_E_FORCE_RCRRP_IN_SILENT_COMM          0x27u
#define DCM_E_ENABLEDTCRECORD_FAILED              0x28U
#define DCM_E_SUBNODE_NOT_SUPPORTED               0x29u
#define DCM_MAXNUMRESPONSEPENDING                 0xFFu
typedef uint8  Dcm_ReturnClearDTCType_tu8;

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Std_ReturnType Dcm_retVal_u8;
extern Dcm_SrvOpStatusType Dcm_SrvOpstatus_u8;
extern Dcm_OpStatusType    Dcm_ExtSrvOpStatus_u8;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern Dcm_SrvOpStatusType Dcm_OBDSrvOpstatus_u8;
extern Dcm_SrvOpStatusType Dcm_OBDExtSrvOpStatus_u8;
#endif

#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/*Service handler uses below mentioned DET API interface function to report the development error to DET Module */
#if (DCM_CFG_DET_SUPPORT_ENABLED != DCM_CFG_OFF)
#define     DCM_DET_ERROR(DCM_ApiId, DCM_ErrorId)                                 \
                                        (void)Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, DCM_ApiId, DCM_ErrorId);
#else
#define     DCM_DET_ERROR(DCM_ApiId, DCM_ErrorId)
#endif


/*
 ***************************************************************************************************
 *    Function prototypes (APIs of DCM which is used only in DSP)
 ***************************************************************************************************
 */
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint32 Dcm_DsldGetActiveSecurityMask_u32 (void);
extern uint32 Dcm_DsldGetActiveSessionMask_u32 (void);
extern void Dcm_DsldSetsessionTiming(uint32 nrP2StarMax_u32,
                                                      uint32 nrP2Max_u32);
extern void Dcm_Prv_SetSesCtrlType ( Dcm_SesCtrlType SesCtrlType_u8);
extern void Dcm_Prv_SetSecurityLevel (Dcm_SecLevelType dataSecurityLevel_u8);
extern void Dcm_Prv_ProcessResetToDefaultSession(void);
extern void Dcm_Prv_ResetDefaultSessionRequestFlag(void);
#if(DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)
extern void Dcm_Prv_ConfirmationRespPendForBootloader(Dcm_ConfirmationStatusType Status_u8);
#endif
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern boolean Dcm_Prv_IsTxPduIdOBD(PduIdType DcmTxPduId);
extern boolean Dcm_Prv_IsRxPduIdOBD(PduIdType DcmRxPduId);

#endif

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#if((DCM_CFG_DSPOBDSUPPORT_ENABLED!= DCM_CFG_OFF)&&(DCM_CFG_DSP_OBDMODE9_ENABLED != DCM_CFG_OFF)&&\
    (DCM_CFG_INFOTYPE_SUPPORT != DCM_CFG_OFF))

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
void Dcm_Prv_SupportInfoTypecheck(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_INIT_UNSPECIFIED  /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

extern boolean Dcm_SupportInfotype_b;

#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif

/* macros to handle the time monitoring */


#define DCM_TimerStop(timer)            ((timer)=0xFFFFFFFFu)

#define DCM_TimerElapsed(timer)         ((timer)==0u)

#define DCM_TimerStopped(timer)         ((timer)==0xFFFFFFFFu)


/* OS Timer usage for Timer handling */
#if ( DCM_CFG_OSTIMER_USE != FALSE )
#define DCM_TimerStart(timer,time,starttime,timerStatus) do                      /*Do while, to remove MISRA Warning 3458*/ \
                                            { \
                                            (timerStatus = Dcm_GetCounterValue(DCM_CFG_COUNTERID, (&(starttime)))); \
                                            SchM_Enter_Dcm_DsldTimer_NoNest();\
                                            if( timerStatus == E_OK ) \
                                            { \
                                                ((timer) = (time)); \
                                            } \
                                            else \
                                            { \
                                                ((timer) = ((time) / DCM_CFG_TASK_TIME_US)); \
                                            } \
                                            SchM_Exit_Dcm_DsldTimer_NoNest();\
                                            }while(0);

#define DCM_TimerProcess(timer,starttime,timerStatus)   do                    /*Do while, to remove MISRA Warning 3458*/ \
                                            { \
                                            if ( timerStatus == E_OK ) \
                                            { \
                                                timerStatus = Dcm_GetCounterValue(DCM_CFG_COUNTERID, (&Dcm_CurrTick_u32)); \
                                                SchM_Enter_Dcm_DsldTimer_NoNest();\
                                                if ( timerStatus == E_OK ) \
                                                { \
                                                    if ( (DCM_CFG_TICKS2US_COUNTER(Dcm_CurrTick_u32 - (starttime))) >= (timer) ) \
                                                    { \
                                                        ((timer) = 0u); \
                                                    } \
                                                } \
                                                else \
                                                { \
                                                    /* This else part is added for future use */ \
                                                } \
                                                SchM_Exit_Dcm_DsldTimer_NoNest();\
                                            } \
                                            else  \
                                            { \
                                                SchM_Enter_Dcm_DsldTimer_NoNest();\
                                                if( ((timer)!=0u) && ((timer)!=0xFFFFFFFFu) ) \
                                                { \
                                                    (timer)--; \
                                                } \
                                                SchM_Exit_Dcm_DsldTimer_NoNest();\
                                            } \
                                            }while(0);
/* This Macro used to set the new timing value of running timer */
/* Eg: After sending the response new value of P2 and P3 max is set */
/* Setting new P3 time is done in Dsp_DcmTxconfirm() function, before that */
/* old P3 max is set to the monitoring variable. from this Macro new time is set */
#define DCM_TimerSetNew(timer,time)   SchM_Enter_Dcm_DsldTimer_NoNest();\
                                    if((timer)!=0u)           \
                                    {                         \
                                        (timer) = (time);     \
                                    }\
                                      SchM_Exit_Dcm_DsldTimer_NoNest();


#else
/* Usage of Raster timer for timer monitoring */
#define DCM_TimerStart(timer,time,unused,timerStatus)  ((void)(timerStatus));\
                                                       SchM_Enter_Dcm_DsldTimer_NoNest();\
                                                      ((timer) = (uint32)((time) / DCM_CFG_TASK_TIME_US));\
                                                      SchM_Exit_Dcm_DsldTimer_NoNest();
#define DCM_TimerProcess(timer,unused,timerStatus)          \
                                            do                       /*Do while, to remove MISRA Warning 3458*/ \
                                            {   \
                                                ((void)(timerStatus));\
                                                SchM_Enter_Dcm_DsldTimer_NoNest();\
                                            if( ((timer)!=0u) && ((timer)!=0xFFFFFFFFuL) ) \
                                            { \
                                                (timer)--; \
                                            } \
                                            SchM_Exit_Dcm_DsldTimer_NoNest();\
                                            }\
                                            while(0);

/* This Macro used to set the new timing value of running timer */
/* Eg: After sending the response new value of P2 and P3 max is set */
/* Setting new P3 time is done in Dsp_DcmTxconfirm() function, before that */
/* old P3 max is set to the monitoring variable. from this Macro new time is set */
#define DCM_TimerSetNew(timer,time)        SchM_Enter_Dcm_DsldTimer_NoNest();\
                                            ((timer) = ((time) / DCM_CFG_TASK_TIME_US));\
                                            SchM_Exit_Dcm_DsldTimer_NoNest();
#endif



#define DCM_NEGRESPONSE_INDICATOR       0x7Fu         /* Indication byte of negative response */


/* Location of default session ID in session array table */
#define DCM_DEFAULT_SESSION_IDX                        0x00u

/* Parallel Tpr  bytes */
#define DCM_DSLD_PARALLEL_TPR_BYTE1                    0x3eu
#define DCM_DSLD_PARALLEL_TPR_BYTE2                    0x80u
#define DCM_DSLD_PARALLEL_DCM_TPR_REQ_LENGTH           0x02u

/* NRC for wait pend negative response */
#define DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING 0x78u


/* Macro to check running protocol is KLINE type  */
/* If KLINE is disabled this macro returns FALSE  */
#if( DCM_CFG_KLINE_ENABLED != DCM_CFG_OFF )
    #define DCM_IS_KLINEPROT_ACTIVE() \
    (((Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8) == DCM_CARB_ON_KLINE)\
/* FC_VariationPoint_START */\
    ||((Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8) == DCM_OBD_ON_KLINE)\
/* FC_VariationPoint_END */\
    ||((Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8) == DCM_KWP_ON_KLINE))
#else
#define DCM_IS_KLINEPROT_ACTIVE()    FALSE
#endif
/* Macro to check whether the ComM is in full communication mode for Drive and boot software */

#define DCM_CHKFULLCOMM_MODE(idx)  (Dcm_active_commode_e[idx].ComMState == DCM_DSLD_FULL_COM_MODE)

/* Macro to check whether the ComM is not in No communication mode for Drive and boot software */

#define DCM_CHKNOCOMM_MODE(idx) (Dcm_active_commode_e[idx].ComMState != DCM_DSLD_NO_COM_MODE)

#define DCM_PROTOCOL_ENDIANNESS (Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].Endianness_ConvEnabled_b)

/*
 ***************************************************************************************************
 *    Internal DCM Type definitions
 ***************************************************************************************************
 */
typedef enum
{
    DCM_DSLD_POS_RESPONSE,                          /* POS response */
    DCM_DSLD_NEG_RESPONSE                           /* NEG response */
}Dcm_DsldResponseType_ten;

#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
typedef enum
{
    DCM_QUEUE_IDLE,                          /* Queue is idle */
    DCM_QUEUE_RUNNING,                       /* QUeuing is taking place */
    DCM_QUEUE_COMPLETED                      /* QUeuing is completed, i.e. the second request is received */
}Dcm_DsldQueHandling_ten;
#endif
typedef struct
{
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
    Dcm_MsgType  adrBufferPtr_pu8;                      /* pointer to hold the address of the processing request buffer */
#endif
    PduIdType dataActiveRxPduId_u8;                     /* Rx PDU Id on which request received  */
    uint8 nrActiveConn_u8;                            /* Active Connection number  */
    uint8 idxActiveSession_u8;                     /* Active sessions index in session lookup table  */
    boolean flgMonitorP3timer_b;                       /* Bit to indicate P3 timer monitoring required  */
    uint8 idxCurrentProtocol_u8;                       /* Active protocol index */
    PduIdType dataActiveTxPduId_u8;                     /* Tx PDU Id on which request received  */
    uint8 datActiveSrvtable_u8;                        /* Active service table */
    boolean flgCommActive_b;                           /* Is communication or protocol active? */
    uint8 cntrWaitpendCounter_u8;                       /* Wait pend counter */
    Dcm_DsldResponseType_ten stResponseType_en;           /* Type of response (POS/NEG) */
    uint8 idxActiveSecurity_u8;                    /* Active security index in security lookup table */
    Std_ReturnType dataResult_u8;                       /* Confirmation result */
    uint8 idxService_u8;                            /* Active services index in active service table */
    boolean dataResponseByDsd_b;                        /* Response given by DSD                         */
    uint8 dataSid_u8;                                    /* Requested Sid  */
#if((DCM_ROE_ENABLED != DCM_CFG_OFF)||(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF))
    uint8  dataOldSrvtable_u8;                          /* Service table which is running when event response */
#endif
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    boolean flgPagedBufferTxOn_b;                      /* Bit to indicate Paged buffer Tx in progress         */
#endif
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    PduIdType dataNewRxPduId_u8;                        /* High protocols Rx PDU id */
#if((DCM_ROE_ENABLED != DCM_CFG_OFF)||(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF))
    PduIdType dataPassRxPduId_u8;                       /* Active Roe's Rx PDU id */
#endif
#endif
    PduLengthType dataRequestLength_u16;                /* Length of request */
    PduIdType dataOldtxPduId_u8;                         /* Copy of the old active Tx PDU in DCM */
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    Dcm_MsgLenType dataCurrentPageRespLength_u32;          /* Amount of data filled by the service       */
#endif
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    PduLengthType dataNewdataRequestLength_u16;             /* Request length of high priority protocols request */
#endif
#if((DCM_ROE_ENABLED != DCM_CFG_OFF)||(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF))
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    PduLengthType dataPassdataRequestLength_u16;            /* Request length of bypassing ROE request */
#endif
    uint32 dataTimerTimeout_u32;                      /* If ROE requested service unable to send */
#endif
    Dcm_MsgType adrActiveTxBuffer_tpu8;                /* Active Tx buffer pointer */
    uint32 dataTimeoutMonitor_u32;                      /* It holds the timeout time */
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
    uint32 datRoeType2Timeout_u32;                   /* Roe timer for type2 */
#endif
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    uint32 dataPagedBufferTimeout_u32;                 /* Paged buffer timer                                  */
#endif
    uint8  PreviousSessionIndex;                     /*sessions index for old Session   */

}Dcm_DsldInternalStructureType_tst;

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)

typedef struct
{
    PduIdType dataActiveRxPduId_u8;                     /* Rx PDU Id on which request received  */
    uint8 nrActiveConn_u8;                              /* Active Connection number  */
    uint8 idxCurrentProtocol_u8;                        /* Active protocol index */
    PduIdType dataActiveTxPduId_u8;                     /* Tx PDU Id on which request received  */
    uint8 datActiveSrvtable_u8;                         /* Active service table */
    boolean flgCommActive_b;                            /* Is communication or protocol active? */
    uint8 cntrWaitpendCounter_u8;                       /* Wait pend counter */
    Dcm_DsldResponseType_ten stResponseType_en;         /* Type of response (POS/NEG) */
    Std_ReturnType dataResult_u8;                       /* Confirmation result */
    uint8 idxService_u8;                                /* Active services index in active service table */
    boolean dataResponseByDsd_b;                        /* Response given by DSD */
    uint8 dataSid_u8;                                   /* Requested Sid  */
    PduLengthType dataRequestLength_u16;                /* Length of request */
    PduIdType dataOldtxPduId_u8;                        /* Copy of the old active Tx PDU in DCM */
    Dcm_MsgType adrActiveTxBuffer_tpu8;                 /* Active Tx buffer pointer */
    uint32 dataTimeoutMonitor_u32;                      /* It holds the timeout time */
}Dcm_OBDInternalStructureType_tst;

// State Machine definition for OBD in case of parallel UDS/OBD handling
typedef enum
{
    DCM_OBD_IDLE,                   /* Default state */
    DCM_OBD_REQUESTRECEIVING,       /* Reception of OBD request in ongoing */
    DCM_OBD_REQUESTRECEIVED,        /* Reception of OBD request is completed */
    DCM_OBD_VERIFYDATA,             /* Verification of received OBD request */
    DCM_OBD_PROCESSSERVICE,         /* Service Interpreter is called here */
    DCM_OBD_WAITFORTXCONF           /* Response is Transmitted , waiting for Confirmation */
} Dcm_OBDStateMachine_tst;

typedef struct
{
    PduInfoType Dcm_DslRxPduBuffer_st;
    uint8       Dcm_DslServiceId_u8;
    boolean     Dcm_DslCopyRxData_b;
}Dcm_DslOBDRxPduArray_tst;

#endif


#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
/* structure for queue handling */
typedef struct
{
    Dcm_DsldQueHandling_ten Dcm_QueHandling_en;         /* State handler for queue */
    Dcm_MsgType  adrBufferPtr_pu8;                      /* pointer to hold the address of the queuing buffer */
    PduLengthType dataQueueReqLength_u16;                /* Length of request */
    PduIdType dataQueueRxPduId_u8;                     /* Rx PDU Id on which request received  */
    uint8 idxBufferIndex_u8;                          /* index to point to the buffer */
}Dcm_QueueStructure_tst;
#endif

typedef struct
{
    PduInfoType Dcm_DslRxPduBuffer_st;
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
    Dcm_MsgType  Dcm_DslBufferPtr_pu8;        /* pointer to hold the address of the normal buffer(non-queue) */
#endif
    uint8      Dcm_DslServiceId_u8;
    boolean     Dcm_DslFuncTesterPresent_b;
    boolean     Dcm_DslCopyRxData_b;
}Dcm_DslRxPduArray_tst;

typedef struct
{
    uint32 dataTimeoutP2StrMax_u32;                     /* P2* max time in us */
    uint32 dataTimeoutP2max_u32;                        /* P2 max time in us */
#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
    uint32 dataTimeoutP3max_u32;                        /* P3 max time in us */
#endif
}Dcm_DsldTimingsType_tst;

typedef struct
{
    Dcm_MsgType TxBuffer_tpu8;                        /* Pointer to Tx buffer */
    Dcm_MsgLenType TxResponseLength_u32 ;             /* Length of response including Sid */
    boolean  isForceResponsePendRequested_b;          /* Application triggered wait pend */
}Dcm_DslTxType_tst;

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
typedef struct
{
    Dcm_MsgType TxBuffer_tpu8;                        /* Pointer to Tx buffer */
    Dcm_MsgLenType TxResponseLength_u32 ;             /* Length of response including Sid */
}Dcm_OBDTxType_tst;
#endif

#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * ROE structure for logging the info related to the postponement of the ROE event .\n
 * Member elements/n
  * uint8  EventId_u8;
   * boolean Is_EventTrigerred;
*/
typedef struct
{
    uint8   EventId_u8;/*Event Id reported from the apllication*/
    boolean Is_Queued;/*Will indocate whether the event is waiting to trigger the service to respond to*/
    boolean Is_Processed;/*Will indocate whether the event is waiting to trigger the service to respond to*/
    PduIdType   RxPduId_u8;/*protocol id tobe used for the reported event*/

} Dcm_DcmDspEventWaitlist_t;

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const Dcm_Dsld_RoeRxToTestSrcMappingType Dcm_Dsld_RoeRxToTestSrcMappingTable[DCM_CFG_NUM_RX_PDUID];
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if( (DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_ROEEVENT_ENABLED != DCM_CFG_OFF) )
#define DCM_START_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_DcmDspEventWaitlist_t Dcm_DcmDspEventQueue [DCM_MAX_SETUPROEEVENTS];
#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint8 DcmRoeQueueIndex_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_CheckActiveDiagnosticStatus(uint8 dataNetworkId);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/*
***************************************************************************************************
*    Variables prototypes
***************************************************************************************************
*/
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint8 Dcm_DslWaitPendBuffer_au8[8];
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern uint8 Dcm_OBDWaitPendBuffer_au8[8];
#endif
extern uint8 Dcm_CurProtocol_u8;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern uint8 Dcm_CurOBDProtocol_u8;
#endif
extern Dcm_SesCtrlType Dcm_CC_ActiveSession_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const Dcm_Dsld_protocol_tableType * Dcm_DsldProtocol_pcst;
extern const uint8 * Dcm_DsldRxTable_pcu8;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern Dcm_DslOBDRxPduArray_tst Dcm_DslOBDRxPduArray_ast[DCM_CFG_NUM_RX_PDUID];
#endif
extern Dcm_DslRxPduArray_tst Dcm_DslRxPduArray_ast[DCM_CFG_NUM_RX_PDUID];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern boolean Dcm_isFuncTPOnOtherConnection_b;
extern boolean Dcm_isInitialised_b;
extern boolean Dcm_acceptRequests_b;
extern boolean Dcm_isCancelTransmitInvoked_b;
extern boolean Dcm_isStopProtocolInvoked_b;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern boolean Dcm_ObdSendTxConfirmation_b;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if (DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern boolean Dcm_ReadyForBoot_b;
extern boolean Dcm_SesChgOnWarmResp_b;

#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const Dcm_Dsld_connType * Dcm_DsldConnTable_pcst;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_DsldInternalStructureType_tst Dcm_DsldGlobal_st;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern Dcm_OBDInternalStructureType_tst Dcm_OBDGlobal_st;
#endif
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
extern Dcm_QueueStructure_tst Dcm_QueueStructure_st;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_DsldTimingsType_tst Dcm_DsldTimer_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern boolean Dcm_isGeneralRejectSent_b;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern boolean Dcm_OBDisGeneralRejectSent_b;
#endif
extern boolean Dcm_isSessionStored_b;
#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
extern boolean Dcm_isFirstReponseSent_b;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern PduInfoType Dcm_DsldPduInfo_st ;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern PduInfoType Dcm_OBDPduInfo_st;
extern Dcm_OBDTxType_tst Dcm_OBDTransmit_st;
#endif
extern Dcm_DslTxType_tst Dcm_DslTransmit_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const Dcm_Dsld_ServiceType * Dcm_DsldSrvTable_pcst;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern const Dcm_Dsld_ServiceType* Dcm_OBDSrvTable_pcst;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_MsgContextType Dcm_DsldMsgContext_st;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern Dcm_MsgContextType Dcm_OBDMsgContext_st;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const uint8 * Dcm_DsldSessionTable_pcu8;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_DsdStatesType_ten Dcm_DsdRoe2State_en;
extern Dcm_MsgContextType Dcm_Roe2MesContext_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_MsgContextType Dcm_Rdpi2MesContext_st;
extern Dcm_DsldResponseType_ten Dcm_Rdpi2ResponseType_en;
extern PduInfoType Dcm_DsldRdpi2pduinfo_ast[DCM_CFG_NUM_RDPITYPE2_TXPDU];
extern const Dcm_ProtocolExtendedInfo_type * Dcm_DsldRdpi2_pcst;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if (DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_Dsld_KwpTimerServerType Dcm_DsldKwpReqTiming_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if (DCM_CFG_OSTIMER_USE != FALSE)
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint32 Dcm_P2OrS3StartTick_u32;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern uint32 Dcm_OBDP2OrS3StartTick_u32;
#endif
extern uint32 Dcm_CurrTick_u32;
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
extern uint32 Dcm_PagedBufferStartTick_u32;     /* Starting tick value for paged buffer timer */
#endif
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
extern uint32 Dcm_TimerStartTick_u32;             /* Starting tick value for Roe timer */
#endif
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if ((DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)||(DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF))
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint32 Dcm_SysPrevTick_u32;   /* Previous tick value */
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint32 Dcm_SysCurrTick_u32;   /* Current tick value */
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif


/* FC_VariationPoint_START */
#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern boolean Dcm_isObdRequestReceived_b;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
/* FC_VariationPoint_END */

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern StatusType Dcm_P2OrS3TimerStatus_uchr; /* global variable to get the return value of GetCounterValue for Timer related funtions*/
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern StatusType Dcm_OBDP2OrS3TimerStatus_uchr;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern StatusType Dcm_PagedBufferTimerStatus_uchr; /* global variable to get the return value of GetCounterValue for Timer related funtions*/
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if((DCM_ROE_ENABLED != DCM_CFG_OFF)||(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF))
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern StatusType Dcm_CounterValueTimerStatus_uchr; /* global variable to get the return value of GetCounterValue for Timer related funtions*/
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern StatusType Dcm_RoeType2TimerStatus_uchr ; /* global variable to get the return value of GetCounterValue for Timer related funtions*/
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif


#define DCM_START_SEC_VAR_INIT_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern boolean Dcm_LowPrioReject_b;
#define DCM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"

/*
 ***************************************************************************************************
 *    Function prototypes
 ***************************************************************************************************
*/
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


/************************TRIGGER TRANSMITT*****************************************************************************/
extern void Dcm_Prv_TriggerTransmit(void);
/**********************************************************************************************************************/



/************************DSL STATE MACHINE*****************************************************************************/
#if((DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF) && (DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF))
#define DCM_NUMBER_OF_DSL_STATES                  (0x08u)
#elif((DCM_CFG_PROTOCOL_PREMPTION_ENABLED == DCM_CFG_OFF) && (DCM_PAGEDBUFFER_ENABLED == DCM_CFG_OFF))
#define DCM_NUMBER_OF_DSL_STATES                  (0x06u)
#elif((DCM_CFG_PROTOCOL_PREMPTION_ENABLED == DCM_CFG_OFF) || (DCM_PAGEDBUFFER_ENABLED == DCM_CFG_OFF))
#define DCM_NUMBER_OF_DSL_STATES                  (0x07u)
#endif


#define DSL_STATE_IDLE                            (0x00u)
#define DSL_STATE_WAITING_FOR_RXINDICATION        (0x01u)
#define DSL_STATE_REQUEST_RECEIVED                (0x02u)
#define DSL_STATE_RESPONSE_TRANSMISSION           (0x03u)
#define DSL_STATE_WAITING_FOR_TXCONFIRMATION      (0x04u)
#if((DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF) && (DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF))
#define DSL_STATE_PROTOCOL_PREEMPTION             (0x05u)
#define DSL_STATE_PAGEDBUFFER_TRANSMISSION        (0x06u)
#define DSL_STATE_ROETYPE1_RECEIVED               (0x07u)
#elif((DCM_CFG_PROTOCOL_PREMPTION_ENABLED == DCM_CFG_OFF) && (DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF))
#define DSL_STATE_PAGEDBUFFER_TRANSMISSION        (0x05u)
#define DSL_STATE_ROETYPE1_RECEIVED               (0x06u)
#elif((DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF) && (DCM_PAGEDBUFFER_ENABLED == DCM_CFG_OFF))
#define DSL_STATE_PROTOCOL_PREEMPTION             (0x05u)
#define DSL_STATE_ROETYPE1_RECEIVED               (0x06u)
#elif((DCM_CFG_PROTOCOL_PREMPTION_ENABLED == DCM_CFG_OFF) && (DCM_PAGEDBUFFER_ENABLED == DCM_CFG_OFF))
#define DSL_STATE_ROETYPE1_RECEIVED               (0x05u)
#endif


#define DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING      (0x00u)
#define DSL_SUBSTATE_S3_OR_P3_TIMEOUT             (0x01u)
#define DSL_SUBSTATE_STOP_PROTOCOL                (0x02u)
#define DSL_SUBSTATE_STOP_PROTOCOL_RECEIVING      (0x03u)
#define DSL_SUBSTATE_STOP_ROE                     (0x04u)
#define DSL_SUBSTATE_START_PROTOCOL               (0x05u)
#define DSL_SUBSTATE_P2MAX_TIMEMONITORING         (0x06u)
#define DSL_SUBSTATE_SEND_PENDING_RESPONSE        (0x07u)
#define DSL_SUBSTATE_SEND_GENERAL_REJECT          (0x08u)
#define DSL_SUBSTATE_SEND_FINAL_RESPONSE          (0x09u)
#define DSL_SUBSTATE_DATA_READY                   (0x0Au)
#define DSL_SUBSTATE_WAIT_FOR_DATA                (0x0Bu)
#define DSL_SUBSTATE_WAIT_PAGE_TXCONFIRM          (0x0Cu)

extern void Dcm_Prv_DslStateMachine(void);

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern void Dcm_Prv_OBDStateMachine(void);
extern void Dcm_Prv_OBDTimerProcessing(void);
extern void Dcm_Prv_ConfirmationToOBDApl(void);
#endif
/**********************************************************************************************************************/


#if ( ( DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF ) && ( DCM_CFG_DSP_COMMUNICATIONCONTROL_ENABLED != DCM_CFG_OFF ) )
extern void Dcm_Prv_CC_Mainfunction (void);
#endif
extern void Dcm_Prv_CC_TxConfirmation (void);
extern void Dcm_Prv_SendResponse(const PduInfoType * adrPduStrucutre_pcst);
#if(DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern void Dcm_Prv_OBDSendResponse(const PduInfoType* adrPduStrucutre_pcst);
#endif
extern void Dcm_DslDsdRestoreSecaTimer(void);
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
extern Dcm_MsgItemType * Dcm_Prv_ProvideFreeBuffer(uint8 idxProtocolIndex_u8,boolean isQueuedReq_b);
#endif
extern Dcm_MsgItemType * Dcm_GetActiveBuffer(void);
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
extern boolean Dcm_Prv_CheckTotalResponseLength(Dcm_MsgLenType TotalResLen_u32 );
#endif

#if (DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
extern uint8 Dcm_GetActiveConnectionIdx_u8 (void);
extern void Dcm_DslDsdWarmStart(void);
#endif

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern void Dcm_Prv_ResetOBDCopyRxDataStatus(PduIdType id);
#endif

#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
extern void Dcm_Prv_PagedBufferTimeout(void);
#endif

#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
extern void Dcm_DsldRoeTimeOut(void);
extern void Dcm_Prv_ROEResetOnConfirmation(void);
#endif

#if((DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF) || (RBA_DCMPMA_CFG_PLANTMODEACTIVATION_ENABLED !=  DCM_CFG_OFF))
Std_ReturnType Dcm_Prv_CancelTransmit(void);
#endif

#if (DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF )
extern void Dcm_DsldResetRDPI(void);
#endif

extern void Dcm_Prv_ResetCopyRxDataStatus ( PduIdType RxPduId );

#if((DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF) && (DCM_ROE_ENABLED != DCM_CFG_OFF))

extern BufReq_ReturnType Dcm_DsldPersistentRoeHandling_en(
                                         const Dcm_Dsld_protocol_tableType * adrArrivedProt_pcst,
                                         PduLengthType nrTpSduLength_u16,
                                         PduIdType dataRxpduId_u8
                                                     );
#endif

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/****************************************************************************************************
*        APPLICATION API DECLARATIONS
****************************************************************************************************/
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if (((DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_WRITE_SR_ENABLED != DCM_CFG_OFF ))   ||     \
     (DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF) && ((DCM_CFG_ROUTINEVARLENGTH==DCM_CFG_OFF)||(DCM_CFG_RCRANGE_ROUTINEVARLENGTH==DCM_CFG_OFF)))

extern uint32 Dcm_GetSignal_u32(uint8 xDataType_u8,
                                           uint16 posnStart_u16,
                                           const uint8 * adrReqBuffer_u8,
                                           uint8 dataEndianness_u8);
#endif


#if (\
/* FC_VariationPoint_START */ \
((DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_OBDMODE1_ENABLED != DCM_CFG_OFF)&& (DCM_CFG_DSP_OBDMODE1_SR_ENABLED!=DCM_CFG_OFF)) ||            \
/* FC_VariationPoint_END */ \
     ((DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_READ_SR_ENABLED != DCM_CFG_OFF)) ||     \
     ((DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF)&&((DCM_CFG_ROUTINEVARLENGTH==DCM_CFG_OFF)||(DCM_CFG_RCRANGE_ROUTINEVARLENGTH==DCM_CFG_OFF)))||\
     (DCM_CFG_RBA_DEM_SR_ENABLED != DCM_CFG_OFF))
extern void Dcm_StoreSignal(uint8 xDataType_u8,
                                           uint16 posnStart_u16,
                                           uint8 * adrRespBuffer_u8,
                                           uint32 dataSignalValue_u32,
                                           uint8 dataEndianness_u8);
#endif

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if(DCM_CFG_RDPI_ENABLED!=DCM_CFG_OFF)
typedef enum
{
    DCM_RDPI_NO_TRANMISSION,
    DCM_RDPI_SLOW_RATE,
    DCM_RDPI_MEDIUM_RATE,
    DCM_RDPI_FAST_RATE
}Dcm_RdpiTxModeType_ten;


typedef struct
{
  uint32 cntrTime_u32;         /* Increment in each time raster if the overflowValue > 0 */
  uint16 dataId_u16;             /* periodicId */ /*---dataRdpiId_u16 */
  uint16 idxPeriodicId_u16;          /* Index of the periodic ID in Dcm_DIDConfig Table */
  Dcm_RdpiTxModeType_ten dataOverflowValue_en;  /* Off: -1, SlowRate, MediumRate, FastRate */
  boolean dataRange_b;          /*Flag to indicate if the did is a range did or not*/

} Dcm_PeriodicInfoType_tst;


#if(DCM_CFG_MAX_DID_SCHEDULER>0)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_PeriodicInfoType_tst Dcm_PeriodicInfo_st[DCM_CFG_MAX_DID_SCHEDULER];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint8 Dcm_NumOfActivePeriodicId_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_DsldPeriodicScheduler(void);
extern void Dcm_RdpiMainFunction(void);
extern void Dcm_DsldPeriodicSchedulerIni(void);
extern void Dcm_GetRdpiType2Index(uint8 * idxRdpi2TxPduId_u8);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#if (DCM_ROE_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_DsldProcessRoeQueue(void);
#if(DCM_CFG_DSP_ROEONDTCSTATUSCHANGE_ENABLED != DCM_CFG_OFF)
extern void Dcm_RestoreROEDtcEvents(void);
#endif
#if(DCM_CFG_DSP_ROEDID_ENABLED!=DCM_CFG_OFF)
extern void Dcm_RestoreROEDidEvents(void);
#endif
extern void Dcm_ResetROEEvents(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_KWPConfirmationForSplitResp(Dcm_ConfirmationStatusType status);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

LOCAL_INLINE void Dcm_DspConfirmation
                                       (
                             Dcm_IdContextType dataIdContext_u8,
                             PduIdType dataRxPduId_u8,
                             uint16 dataSourceAddress_u16,
                             Dcm_ConfirmationStatusType status_u8
                                       )

{    if(Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8].Dcm_ConfirmationService !=((Dcm_ConfirmationApiType)NULL_PTR))
    {
        (Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8].Dcm_ConfirmationService)(dataIdContext_u8,dataRxPduId_u8,dataSourceAddress_u16,status_u8);
    }
}

LOCAL_INLINE boolean DCM_IS_KWPPROT_ACTIVE(void)
{
    boolean retval_b = FALSE;
#if( DCM_CFG_KWP_ENABLED != DCM_CFG_OFF )
    retval_b = (((Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].protocolid_u8) & 0xF0u) == 0x80u);
#endif
    return retval_b;
}

#endif /* _DCMCORE_DSLDSD_PROT_H */

