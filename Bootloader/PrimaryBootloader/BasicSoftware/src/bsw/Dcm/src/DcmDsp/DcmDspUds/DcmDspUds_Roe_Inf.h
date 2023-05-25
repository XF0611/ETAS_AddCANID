

#ifndef DCMDSPUDS_ROE_INF_H
#define DCMDSPUDS_ROE_INF_H

/*
 ***************************************************************************************************
 * Public Includes
 ***************************************************************************************************
 */

#include "Dcm.h"

#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF) )
/*
 ***************************************************************************************************
 * Protected Includes (package wide includes)
 ***************************************************************************************************
 */

#include "DcmCore_DslDsd_Prot.h"
#include "DcmDspUds_Uds_Prot.h"
#include "Dcm_Dsl_Priv.h"
#include "DcmAppl.h"
#endif

/*
 ***************************************************************************************************
 *    Function prototypes (APIs of DCM which is used only in DSP)
 ***************************************************************************************************
 */
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
/* Timers for ROE inter message timer monitoring */

#define DCM_RoeTimerStart(timer,time)      ((timer) = (time));
#define DCM_RoeTimerProcess(timer)          do  /*Do while, to remove MISRA Warning 3458*/ \
                                            {   \
                                            if( ((timer)!=0u) && ((timer)!=0xFFFFFFFFuL) ) \
                                            { \
                                                (timer)--; \
                                            } \
                                            }\
                                            while(0);
#endif

/****************************************************************************************************
*        APPLICATION API DECLARATIONS
****************************************************************************************************/
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
boolean Dcm_ChkIfDidServiceIsActive(uint8 TesterReqSid_u8, uint8 RoeType2Sid_u8);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif


/*
 ***************************************************************************************************
 *    Internal DCM Type definitions
 ***************************************************************************************************
 */
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
typedef struct
{
    uint8 dataEventSID_u8;                             /* ROE event SID */
    PduIdType dataRoeRxPduId_u8;                       /* RxPdu on which ROE event was registered */
    Dcm_MsgLenType dataEventRespLen_u32;            /* Roe Event Request/Response length */
    Dcm_ConfirmationStatusType stEventConfStatus_u8; /* The confirmation status of the event */
}Dcm_DsldRoeQueueType_tst;

/* ROE Queue state machine */
typedef enum
{
    DCM_ROE_QUEUE_INIT,                /* ROE Queue Initialisation state */
    DCM_ROE_QUEUE_WAITFORCONFIRMATION  /* ROE Queue confirmation state */
}Dcm_DsldstRoeQueueState_ten;

typedef struct
{
    boolean flgTxQueueActive_b;                /* Indication status for ROE active/inactive */
    uint8   idxTxResponse_u8;               /* ROE response index in ROE queue */
    uint8   idxTxQueue_u8 ;                 /* ROE Queue index in ROE queue */
    Dcm_DsldstRoeQueueState_ten  stRoeQueueState_st;  /* ROE queue state machine */
    uint32  dataInterMsgTimer_u32;              /* ROE timer variable */
}Dcm_DsldRoeIntenalType_tst;
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_DsldRoeIntenalType_tst Dcm_DsldRoeScheduler_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_DslDsdRoeEventConfirmation(Dcm_ConfirmationStatusType dataConfirmResult_u8);
extern void Dcm_DslDsdRoeEventProcess(void);
extern void Dcm_DslDsdRoeProcessConfirm(Std_ReturnType dataResult_u8);
extern void Dcm_DslDsdRoeQueueResponse(void);
extern void Dcm_DslDsd_ResetROEScheduler(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

/*
***************************************************************************************************
*    Variables prototypes
***************************************************************************************************
*/
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_DsldResponseType_ten Dcm_Roe2ResponseType_en;
extern PduInfoType Dcm_DsldRoe2PduInfo_st;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint8 Dcm_DsldSrvIndex_u8;
extern Std_ReturnType Dcm_Roe2TxResult_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const Dcm_ProtocolExtendedInfo_type * Dcm_DsldRoe2_pcst;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
extern uint32 Dcm_Roe2StartTick_u32;             /* Starting tick value for Roe timer */
#endif

#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const Dcm_ProtocolExtendedInfo_type * Dcm_DsldRoe_pcst;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
/****************************************************************************************************
*        APPLICATION API DECLARATIONS
****************************************************************************************************/
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
void Dcm_DsldRoe2StateMachine(void);
#endif
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_ROE_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF))
/**********************************************************************************************************************
 Function name    : Dcm_Prv_RoeMainFunction
 Syntax           : Dcm_Prv_RoeMainFunction()
 Description      : Invoke RoeStateMachine function in Dcm_MainFunction cyclically.
 Parameter        :
 Return value     : None
**********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_RoeMainFunction (void)
{
#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
        if(Dcm_DsdRoe2State_en != DSD_IDLE)
        {
            /* Roe type 2 handling */
            Dcm_DsldRoe2StateMachine();
        }
        else
        {
            Dcm_DsldProcessRoeQueue();
        }
#else
        if(Dcm_Prv_GetDslState() == DSL_STATE_IDLE)
        {
            if(Dcm_Prv_GetDsdState() == DSD_IDLE)
            {
                Dcm_DsldProcessRoeQueue();
            }
        }
#endif
}
#endif


/* _DCMDSPUDS_ROE_INF_H */
#endif
