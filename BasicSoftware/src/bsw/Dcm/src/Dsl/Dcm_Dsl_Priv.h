
#ifndef DCM_DSL_PRIV_H
#define DCM_DSL_PRIV_H


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
#define DCM_OBDSERVICEID_0x01                        (0x01u)
#define DCM_OBDSERVICEID_0x0A                        (0x0Au)
#define DCM_SERVICEID_DEFAULT_VALUE                  (0xFFu)
#define DCM_PDUID_DEFAULT_VALUE                      (0xFFu)
#define DCM_NEGATIVE_RESPONSE_LENGTH                 (0x03u)
#define DCM_SERVICEID_ADDEND                         (0x40u)

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
/* Sub State Machine definitions for OBD */
#define OBDSUBSTATE_SERVICETABLE_INI           (0x00u)
#define OBDSUBSTATE_MANUFACTURER_NOTIFICATION  (0x01u)
#define OBDSUBSTATE_VERIFYDATA                 (0x02u)
#define OBDSUBSTATE_SUPPLIER_NOTIFICATION      (0x03u)
#endif
#define DCM_OBDCONTEXT                      TRUE /*used to indicate processing for OBD in case of parallel processing*/
#define DCM_UDSCONTEXT                      FALSE


#define  DCM_SID_DIAGNOSTICSESSIONCONTROL            (0x10u)


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
extern uint8 Dcm_DslState_u8;
extern uint8 Dcm_DslSubState_u8;
extern uint8 Dcm_DslNextState_u8;
extern uint8 Dcm_DslNextSubState_u8;
#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
extern uint8 Dcm_DslPreemptionState_u8;
#endif
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
extern uint8 stObdSubState_en;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
extern Dcm_OBDStateMachine_tst Dcm_OBDState_en;
extern Dcm_OBDStateMachine_tst Dcm_OBD_PreviousState;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#endif


/***********************************************************************************************************************
 *    FUNCTION-LIKE MACROS
***********************************************************************************************************************/
#define Dcm_Prv_GetDslState()           (Dcm_DslState_u8)
#define Dcm_Prv_GetDslSubState()        (Dcm_DslSubState_u8)
#define Dcm_Prv_GetDslNextState()       (Dcm_DslNextState_u8)
#define Dcm_Prv_GetDslNextSubState()    (Dcm_DslNextSubState_u8)

#define Dcm_Prv_SetDslState(NewDslState_u8)                 (Dcm_DslState_u8 = NewDslState_u8)
#define Dcm_Prv_SetDslSubState(NewDslSubState_u8)           (Dcm_DslSubState_u8 = NewDslSubState_u8)
#define Dcm_Prv_SetDslNextState(NewDslNextState_u8)         (Dcm_DslNextState_u8 = NewDslNextState_u8)
#define Dcm_Prv_SetDslNextSubState(NewDslNextSubState_u8)   (Dcm_DslNextSubState_u8 = NewDslNextSubState_u8)

#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
#define Dcm_Prv_GetDslPreemptionState()   (Dcm_DslPreemptionState_u8)
#define Dcm_Prv_SetDslPreemptionState(NewDslPreemptionState_u8)  (Dcm_DslPreemptionState_u8 = NewDslPreemptionState_u8)
#endif

#define Dcm_isRequestTobeProcessedByDSP(DcmRxPduId)    (Dcm_DslRxPduArray_ast[DcmRxPduId].Dcm_DslCopyRxData_b)

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)

#define Dcm_Prv_SetOBDState(ObdState) (Dcm_OBDState_en = ObdState)
#define Dcm_Prv_GetOBDState()         (Dcm_OBDState_en)
/* To Reset OBD SubState Machine */
#define Dcm_ResetOBDSubStateMachine() (stObdSubState_en = OBDSUBSTATE_SERVICETABLE_INI)
/* Store OBD state , needed for handling Pending Transmission use case */
#define Dcm_Prv_SetOBDPreviousState() (Dcm_OBD_PreviousState = Dcm_OBDState_en)
#define Dcm_Prv_GetOBDPreviousState() (Dcm_OBD_PreviousState)

#endif



/***********************************************************************************************************************
 *    Inline functions
***********************************************************************************************************************/

/***********************************************************************************************************************
 Function name    : Dcm_Prv_isForcePendingResponse
 Syntax           : Dcm_Prv_isForcePendingResponse(void)
 Description      : This Inline function is used to check whether response is force response
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isForcePendingResponse(void)
{
    return (Dcm_DslTransmit_st.isForceResponsePendRequested_b);
}




/***********************************************************************************************************************
 Function name    : Dcm_Prv_isProtocolStarted
 Syntax           : Dcm_Prv_isProtocolStarted(void)
 Description      : This Inline function is used to check the Flag which indicated whether Protocol is started
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isProtocolStarted(void)
{
    return (Dcm_DsldGlobal_st.flgCommActive_b);
}




#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isPagedBufferTxStarted
 Syntax           : Dcm_Prv_isPagedBufferTxStarted(void)
 Description      : This Inline function is used to check whether paged buffer transmission hs stared
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isPagedBufferTxStarted(void)
{
    return (Dcm_DsldGlobal_st.flgPagedBufferTxOn_b);
}
#endif





#if(DCM_CFG_RXPDU_SHARING_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isRxPduShared
 Syntax           : Dcm_Prv_isRxPduShared(DcmRxPduId,ServiceId)
 Description      : This Inline function is used to check whether pduid is shared between OBD and UDS
 Parameter        : PduIdType ,uint8
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isRxPduShared(PduIdType DcmRxPduId,
        uint8 ServiceId)
{
    return((DcmRxPduId < (DCM_CFG_NUM_RX_PDUID-1u)) && (DcmRxPduId == DCM_CFG_SHARED_RX_PDUID) && \
            (ServiceId >= DCM_OBDSERVICEID_0x01) && (ServiceId <= DCM_OBDSERVICEID_0x0A));

}
#endif




/***********************************************************************************************************************
 Function name    : Dcm_Prv_Check_PendingResponseForKWP
 Syntax           : Dcm_Prv_Check_PendingResponseForKWP(void)
 Description      : This inline function is used to check whether pending response is for KWP service
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_Check_PendingResponseForKWP(void)
{
    boolean RetVal_b = TRUE;

#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF) && (DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))

    if((DCM_IS_KWPPROT_ACTIVE() != FALSE) && (Dcm_isFirstReponseSent_b != FALSE))
    {
        Dcm_KWPConfirmationForSplitResp(DCM_RES_POS_NOT_OK);
        Dcm_isFirstReponseSent_b = FALSE;
    }
    else
#endif
    {
        RetVal_b = FALSE;
    }

    return RetVal_b;
}



#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_isProtocolPreemptionInitiated
 Syntax           : Dcm_Prv_isProtocolPreemptionInitiated(void)
 Description      : This function is used to check whether high priority protocol request is received
 Parameter        : None
 Return value     : boolean
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_isProtocolPreemptionInitiated(void)
{
    return ((Dcm_Prv_GetDslPreemptionState() == DSL_SUBSTATE_STOP_PROTOCOL) || \
            (Dcm_Prv_GetDslPreemptionState() == DSL_SUBSTATE_STOP_ROE));
}
#endif


#endif /* DCM_DSL_PRIV_H */
