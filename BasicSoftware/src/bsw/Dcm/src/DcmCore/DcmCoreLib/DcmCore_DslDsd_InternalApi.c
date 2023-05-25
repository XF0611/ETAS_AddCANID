
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if(DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)

boolean Dcm_Prv_IsRxPduIdOBD(PduIdType DcmRxPduId)
{
    uint8 Counter;
    boolean IsRxPduIdObd_b = FALSE;

    for(Counter = 0u;Counter < DCM_OBD_RXSIZE;Counter++)
    {
        if(Dcm_Dsl_OBDRxPduIds[Counter] == DcmRxPduId)
        {
            IsRxPduIdObd_b = TRUE;
            break;
        }
    }
    return IsRxPduIdObd_b;
}

boolean Dcm_Prv_IsTxPduIdOBD(PduIdType DcmTxPduId)
{
    uint8 Counter;
    boolean IsTxPduIdObd_b = FALSE;

    for(Counter = 0u;Counter < DCM_OBD_TXSIZE;Counter++)
    {
        if(Dcm_Dsl_OBDTxPduIds[Counter] == DcmTxPduId)
        {
            IsTxPduIdObd_b = TRUE;
            break;
        }
    }
    return IsTxPduIdObd_b;
}
#endif


/*
***********************************************************************************************************
*  Dcm_DsldSetsessionTiming
*  Function to set the new timings of sessions (this function is called by the DSC service).
*  Inputs:   None
*  Output:   None
*  Return:   None
***********************************************************************************************************/
void Dcm_DsldSetsessionTiming(
                                               uint32 nrP2StarMax_u32,
                                               uint32 nrP2Max_u32
                                             )
{
    /* Lock required here to maintain consistency between P2Max, P2StarMax */
    SchM_Enter_Dcm_DsldTimer_NoNest();
#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)


    /*Check if the KWP type return value is FALSE*/
    if(DCM_IS_KWPPROT_ACTIVE() == FALSE)
#endif
    {
        /* set the P2 max and P2 star max timings of new session */
        Dcm_DsldTimer_st.dataTimeoutP2max_u32 = nrP2Max_u32;
        Dcm_DsldTimer_st.dataTimeoutP2StrMax_u32 = nrP2StarMax_u32;
    }
    SchM_Exit_Dcm_DsldTimer_NoNest();
}




/**
 **************************************************************************************************
 * Dcm_DsldGetActiveSecurityMask_u32 : API to get the active security mask
 * \param           None
 *
 *
 * \retval          active security mask
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
uint32 Dcm_DsldGetActiveSecurityMask_u32 (void)
{
    return (uint32)(0x01uL<<Dcm_DsldGlobal_st.idxActiveSecurity_u8);
}



/**
 **************************************************************************************************
 * Dcm_DsldGetActiveSessionMask_u32 : API to get the active session mask
 * \param           None
 *
 *
 * \retval          active session mask
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
uint32 Dcm_DsldGetActiveSessionMask_u32 (void)
{
    return (uint32)(0x01uL<<Dcm_DsldGlobal_st.idxActiveSession_u8);
}



/**
 **************************************************************************************************
 * Dcm_ForceRespPend : API used to trigger wait pend response by the Service
 *
 * \param           None
 *
 *
 * \retval          Std_ReturnType
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager-7331][BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager-7332]*/
Std_ReturnType Dcm_ForceRespPend(void)
{
    Std_ReturnType dataRetValue_u8 = E_NOT_OK;
    Dcm_DsdStatesType_ten DsdState_en = Dcm_Prv_GetDsdState();

    /*Do not Proceed further if the call is triggered by ROE/RDPI ,or if the call is from application and none of the Dcm service is active */
    if((Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest != DCM_ROE_SOURCE) && (Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest != DCM_RDPI_SOURCE) && (DSD_CALL_SERVICE  == DsdState_en))
    {
        /* If DCM is already sending response pend then this API returns E_NOT_OK */
        if(Dcm_DslTransmit_st.isForceResponsePendRequested_b == FALSE)
        {
            /* This application triggered wait pend */

            Dcm_DslTransmit_st.isForceResponsePendRequested_b = TRUE;

            /* no need to frame the response. It is done in separate function */
            Dcm_DslTransmit_st.TxBuffer_tpu8 = NULL_PTR;

            /* while framing the response proper length info is filled */
            Dcm_DslTransmit_st.TxResponseLength_u32 = 0x00;

            /* trigger the Tx in DSL */
            Dcm_Prv_TriggerTransmit();

            /* Update return value */
            dataRetValue_u8 = E_OK;
        }
    }
    return(dataRetValue_u8);
}

/**
 **************************************************************************************************
 * Dcm_IsInfrastructureErrorPresent_b : API to check for infrastructure error
 * \param           dataInfrastrutureCode_u8 : Parameter to be checked for infrastructure Error
 *
 *
 * \retval          TRUE : if infrastructure error is present
 * 					FALSE : if infrastructure error is not present
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

boolean Dcm_IsInfrastructureErrorPresent_b(uint8 dataInfrastrutureCode_u8)
{
	boolean stInfrastructStatus_b;
	if((dataInfrastrutureCode_u8 & 0x80u) != (0x00u))
	{
	    /*Infrastructure Error status is set to TRUE*/
		stInfrastructStatus_b= TRUE;
	}
	else
	{
	    /*Infrastructure Error status is set to FALSE*/
		stInfrastructStatus_b= FALSE;
	}
	return(stInfrastructStatus_b);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

