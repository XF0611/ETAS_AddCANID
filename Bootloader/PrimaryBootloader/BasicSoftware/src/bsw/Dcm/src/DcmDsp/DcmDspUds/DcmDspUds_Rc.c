

#include "DcmDspUds_Rc_Inf.h"
#include "DcmDspUds_Uds_Prot.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#if(DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint8 * Dcm_RCInPtr_pu8;
uint8 * Dcm_RCOutPtr_pu8;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if (DCM_CFG_RC_NUMRIDS > 0u)

static const Dcm_DspRoutineType_tst * Dcm_adrRoutinePtr_pcst;
static const Dcm_DspRoutineType_tst * s_adrRoutine_pcst;
#endif
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
static const Dcm_DspRoutineRangeType_tst * Dcm_adrRoutineRangePtr_pcst;
static const Dcm_DspRoutineRangeType_tst * s_adrRoutineRange_pcst;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_DspRCStateType_ten Dcm_stDspRCState_en;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
static uint16 s_dataRidRange_u16;
static uint16 s_PendingRidRange_u16;
#endif
static uint16 s_dataStatusIdx_u16;
#define DCM_STOP_SEC_VAR_CLEARED_16/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint8 Dcm_dataRCSubFunc_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_RCCurrDataLength_u16;
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_NegativeResponseCodeType Dcm_RCNegResCode_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_OpStatusType Dcm_RCOpStatus_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static boolean s_IsRCSubfunctionCalled_b;
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
/* Flag to indicate if RID is supported in a range routine or it is a single routine */
static boolean s_IsRidRange_b;
#endif
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**
 ******************************************************************************************************************
 * Dcm_Dsp_RC_Ini :
 *  Service Initialization for initializing Routine Control Service
 *
 * \param           None
 * \retval          None
 * \seealso
 ******************************************************************************************************************
 */

void Dcm_Dsp_RC_Ini ( void )
{
    uint16 idxLoop_qu16;
    Dcm_RCOpStatus_u8 = DCM_CANCEL;
    if ( Dcm_stDspRCState_en == DCM_RC_PENDING )
    {
        /* Service Initialization may have been triggered due to protocol preemption, thus
        it is required to inform the Application/RTE about the CANCEL operation */
        /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
         * Hence tehre is no data inconsistency involved and the lock is not necessary. */
        /*Set the flag for s_IsRCSubfunctionCalled_b is to TRUE*/
        s_IsRCSubfunctionCalled_b = TRUE;
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
        /* Check whether the supported RID is present in a Range or it is a single Routine */
        if(s_IsRidRange_b)
        {
            (void)(*Dcm_adrRoutineRangePtr_pcst->adrCallRoutine_pfct)(Dcm_dataRCSubFunc_u8);
        }
        else
#endif
        {
#if (DCM_CFG_RC_NUMRIDS > 0u)
            (void)(*Dcm_adrRoutinePtr_pcst->adrCallRoutine_pfct)(Dcm_dataRCSubFunc_u8);
#endif
        }
        /*Set the flag for s_IsRCSubfunctionCalled_b is to FALSE*/
        s_IsRCSubfunctionCalled_b = FALSE;
    }

#if (DCM_CFG_RC_NUMRIDS > 0u)



    /* Search through the complete list of Routines */
    for ( idxLoop_qu16 = 0 ; idxLoop_qu16 < DCM_CFG_RC_NUMRIDS ; idxLoop_qu16++ )
    {
        s_adrRoutine_pcst =  &Dcm_DspRoutine_cast[idxLoop_qu16];
#if(DCM_CFG_STOPSUPPORTED != DCM_CFG_OFF)
        /*Check the status of the routine;if it is  DCM_ROUTINE_STOP_PENDING, call the StopRoutine function with Opstatus set to CANCEL*/
        if ( (s_adrRoutine_pcst->flgStopRoutine_b!= FALSE) && (s_adrRoutine_pcst->flgStopRoutineOnSessionChange_b != FALSE) && (Dcm_RoutineStatus_aen[idxLoop_qu16]== DCM_ROUTINE_STOP_PENDING )  )
        {
            /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
             * Hence there is no data inconsistency involved and the lock is not necessary. */
            /*Set the flag for s_IsRCSubfunctionCalled is to TRUE*/
            s_IsRCSubfunctionCalled_b = TRUE;
            (void)(*s_adrRoutine_pcst->adrCallRoutine_pfct)(DCM_RC_STOPROUTINE);
            /*Set the flag for s_IsRCSubfunctionCalled is to FALSE*/
            s_IsRCSubfunctionCalled_b = FALSE;
        }
#endif
        if (Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 >= DCM_CFG_MAX_WAITPEND)
        {
            /* Do nothing*/
            /* Service ini is called because of general reject.no need to reset the status of started  routines*/
        }
        else
        {
            /*Now set the status of the routine to default*/
            Dcm_RoutineStatus_aen[idxLoop_qu16] = DCM_ROUTINE_IDLE;
        }


    }
#endif

#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)



    /* Initialize the routine range flag to FALSE*/
    s_IsRidRange_b = FALSE;

    /* Search through the complete list of Range Routines to find if the received RID is found within any of the configured routine ranges */
    for (idxLoop_qu16 = 0; idxLoop_qu16 < DCM_CFG_RC_RANGE_NUMRIDS; idxLoop_qu16++)
    {
        s_adrRoutineRange_pcst = &Dcm_DspRoutineRange_cast[idxLoop_qu16];

#if (DCM_CFG_RCRANGE_STOPSUPPORTED != DCM_CFG_OFF)
        /* Check the status of the routine range if it is  DCM_ROUTINE_STOP_PENDING, call the StopRoutine function with Opstatus set to CANCEL */
        if((s_adrRoutineRange_pcst->flgStopRoutine_b!= FALSE)&& (s_adrRoutineRange_pcst->flgStopRoutineOnSessionChange_b != FALSE) && (Dcm_RoutineRangeStatus_aen[idxLoop_qu16] == DCM_ROUTINE_STOP_PENDING))
        {
            /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
             * Hence there is no data inconsistency involved and the lock is not necessary. */
            /*Set the flag for s_IsRCSubfunctionCalled is to TRUE*/
            s_IsRCSubfunctionCalled_b = TRUE;
            (void)(*s_adrRoutineRange_pcst->adrCallRoutine_pfct)(DCM_RC_STOPROUTINE);
            /*Set the flag for s_IsRCSubfunctionCalled as FALSE*/
            s_IsRCSubfunctionCalled_b = FALSE;
        }
#endif
        if (Dcm_DsldGlobal_st.cntrWaitpendCounter_u8 >= DCM_CFG_MAX_WAITPEND)
        {
            /* Do nothing*/
            /* Service Ini is called because of general reject. No need to reset the status of started routines*/
        }
        else
        {
            /*Now set the status of the routine to default*/
            Dcm_RoutineRangeStatus_aen[idxLoop_qu16] = DCM_ROUTINE_IDLE;
        }


    }
#endif
    Dcm_RCOpStatus_u8 = DCM_INITIAL;
    Dcm_stDspRCState_en = DCM_RC_IDLE;
}


#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
/*
 ********************************************************************************************************************
 * Dcm_Prv_RcIsRIDRangeRoutine :
 *  This function is used to check if the RID is supported in the Ranges
 * \param  [in]     dataRId_u16     : Requested RID
 *         [out]    idxLoop_qu16    : index of the Rid in Routine or Routine Range structure
 *         [out]    NRC_u8          : NRC Value if RID is not supported
 *         [out]    flgProcessReq_b : TRUE --> RID is supported
 *                                    FALSE --> RID is not supported
 * \retval Std_ReturnType  Retval   : E_OK --> RID is supported
 *                                    E_NOT_OK --> RID is not supported
 *******************************************************************************************************************/
static Std_ReturnType Dcm_Prv_RcIsRIDRangeRoutine(boolean * flgRidSupported_b,
        uint16 dataRId_u16,
        uint16 * idxLoop_qu16,
        Dcm_NegativeResponseCodeType * NRC_u8)
{
    /* Return value to store the result of IsRidAvailable function */
    Std_ReturnType RetVal;
    /* Initialize the return value */
    RetVal = E_NOT_OK;
    /* Set the routine range flag to FALSE*/
    s_IsRidRange_b = FALSE;




    /* Search through the complete list of Range Routines to find if the received RID is found within any of the configured routine ranges */
    for (*idxLoop_qu16 = 0; *idxLoop_qu16 < DCM_CFG_RC_RANGE_NUMRIDS; (*idxLoop_qu16)++)
    {

        s_adrRoutineRange_pcst = &Dcm_DspRoutineRange_cast[*idxLoop_qu16];
        if((dataRId_u16 >= s_adrRoutineRange_pcst->RidRangeLowerLimit_u16) && (dataRId_u16 <= s_adrRoutineRange_pcst->RidRangeUpperLimit_u16))
        {
            /* RID is found within the current range */
            s_IsRidRange_b = TRUE;
            /* Update the RangeRID in the static variable */
            s_dataRidRange_u16 = dataRId_u16;
            /* Check if the RID range has gaps or not */
            if(s_adrRoutineRange_pcst->RoutineRangeHasGaps_b == TRUE)
            {
                RetVal = (*(s_adrRoutineRange_pcst->IsRIDRangeAvailFnc_pf))(dataRId_u16,NRC_u8);
                /* The Range has gaps. All routines in this range are not supported.
                 * Call IsRIDRangeAvailFnc_pf function to check if the passed RID is supported in the range or not */
                if(RetVal == E_OK)
                {
                    /* Requested RID is available */
                    *flgRidSupported_b = TRUE;
                    *NRC_u8=0x00;
                }
                else
                {
                    /* RID is not supported in the current range. Check if it is supported in the single Routines */
                    s_IsRidRange_b = FALSE;
                }
            }
            else
            {
                /* The Range does not have gaps. All routines in this range are supported */
                *flgRidSupported_b = TRUE;
                *NRC_u8=0x00;
            }
#if (DCM_CFG_RCRANGE_STOPSUPPORTED != DCM_CFG_OFF)
            /*If NRC is set to 0x00 and stop routine flag is set to TRUE for the routine session and Dcm Routine stop call returns pending */
            if((*NRC_u8 ==0x00) && (s_adrRoutineRange_pcst->flgStopRoutine_b!= FALSE)&& (s_adrRoutineRange_pcst->flgStopRoutineOnSessionChange_b != FALSE) && (Dcm_RoutineRangeStatus_aen[*idxLoop_qu16] == DCM_ROUTINE_STOP_PENDING ))
            {
                /*Since the status of the Routine is DCM_ROUTINE_STOP_PENDING ,call the StopRoutine with OpStatus set to DCM_CANCEL  */
                Dcm_RCOpStatus_u8 = DCM_CANCEL;
                /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
                 * Hence tehre is no data inconsistency involved and the lock is not necessary. */
                /*Set the flag for s_IsRCSubfunctionCalled is to TRUE*/
                s_IsRCSubfunctionCalled_b = TRUE;
                (void)(*s_adrRoutineRange_pcst->adrCallRoutine_pfct)(DCM_RC_STOPROUTINE);
                /*Set the flag for s_IsRCSubfunctionCalled and flgRidSupported_b as FALSE*/
                s_IsRCSubfunctionCalled_b = FALSE;
                *flgRidSupported_b = FALSE;
                s_IsRidRange_b = FALSE;
                Dcm_RoutineRangeStatus_aen[*idxLoop_qu16] = DCM_ROUTINE_STOP;
            }
            Dcm_RCOpStatus_u8 = DCM_INITIAL;
#endif
            /* If NRC is not set */
            if(*NRC_u8 == 0x00)
            {
                /*Store the index for getting the status in the next main function call*/
                s_dataStatusIdx_u16 = (uint16)(*idxLoop_qu16);
            }
            break;
        }

   }

    return RetVal;
}
#endif

#if (DCM_CFG_RC_NUMRIDS > 0u)
/*
 ********************************************************************************************************************
 * Dcm_Prv_RcIsRIDSingleRoutine :
 *  This function is used to check if the RID is supported in the single routines
 * \param  [in]     dataRId_u16     : Requested RID
 *         [out]    idxLoop_qu16    : index of the Rid in Routine or Routine Range structure
 *         [out]    NRC_u8          : NRC Value if RID is not supported
 *         [out]    flgProcessReq_b : TRUE --> RID is supported
 *                                    FALSE --> RID is not supported
 * \retval Std_ReturnType  Retval   : E_OK --> RID is supported
 *                                    E_NOT_OK --> RID is not supported
 *******************************************************************************************************************/
static Std_ReturnType Dcm_Prv_RcIsRIDSingleRoutine(boolean * flgRidSupported_b,
        uint16 dataRId_u16,
        uint16 * idxLoop_qu16,
        Dcm_NegativeResponseCodeType * NRC_u8)
{
    /*TRACE[SWS_Dcm_01139]*/
    /* Return value to store the result of IsRidAvailable function */
    Std_ReturnType RetVal;
    /* Initialize the return value */
    RetVal = E_NOT_OK;
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
    s_IsRidRange_b = FALSE;
#endif

    /* Search through the complete list of Routines to find if the received RID matches with any of the configured RIDs */
    for (*idxLoop_qu16 = 0; *idxLoop_qu16 < DCM_CFG_RC_NUMRIDS; (*idxLoop_qu16)++ )
    {
       s_adrRoutine_pcst =  &Dcm_DspRoutine_cast[*idxLoop_qu16];

	    if (dataRId_u16 == s_adrRoutine_pcst->dataRId_u16)
        {
            /* Match found, break free from the for loop */
            *flgRidSupported_b = TRUE;
            RetVal = E_OK;
#if(DCM_CFG_DSP_NUMISRIDAVAIL>0)
            /* Check if the RID is supported in current variant */
            if(*Dcm_RIDIsAvail[s_adrRoutine_pcst->idxRIDSupportedFnc_u16] != NULL_PTR)
            {
                if((*(IsRIDAvailFnc_pf)(Dcm_RIDIsAvail[s_adrRoutine_pcst->idxRIDSupportedFnc_u16]))(s_adrRoutine_pcst->dataRId_u16)!=E_OK)
                {
                    /* Routine Identifier is not supported  in the current variant */
                    *NRC_u8 = DCM_E_REQUESTOUTOFRANGE;
                    *flgRidSupported_b = FALSE;
                    RetVal = E_NOT_OK;
                }
            }
#else
            DCM_UNUSED_PARAM(NRC_u8);
#endif
#if (DCM_CFG_STOPSUPPORTED != DCM_CFG_OFF)
            /*If NRC is set to 0x00 and stop routine flag is set to TRUE for the routine session and Dcm Routine stop call returns pending */
            if((*NRC_u8 ==0x00) && (s_adrRoutine_pcst->flgStopRoutine_b!= FALSE)&& (s_adrRoutine_pcst->flgStopRoutineOnSessionChange_b != FALSE) && (Dcm_RoutineStatus_aen[*idxLoop_qu16] == DCM_ROUTINE_STOP_PENDING ))
            {
                /*Since the status of the Routine is DCM_ROUTINE_STOP_PENDING ,call the StopRoutine with OpStatus set to DCM_CANCEL  */
                Dcm_RCOpStatus_u8 = DCM_CANCEL;
                /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
                 * Hence tehre is no data inconsistency involved and the lock is not necessary. */
                /*Set the flag for s_IsRCSubfunctionCalled is to TRUE*/
                s_IsRCSubfunctionCalled_b = TRUE;
                (void)(*s_adrRoutine_pcst->adrCallRoutine_pfct)(DCM_RC_STOPROUTINE);
                /*Set the flag for s_IsRCSubfunctionCalled and flgRidSupported_b as FALSE*/
                s_IsRCSubfunctionCalled_b = FALSE;
                Dcm_RoutineStatus_aen[*idxLoop_qu16] = DCM_ROUTINE_STOP;
            }
            Dcm_RCOpStatus_u8 = DCM_INITIAL;
#endif
            /* If NRC is not set */
            if(*NRC_u8 ==0x00)
            {
                /*Store the index for getting the status in the next main function call*/
                s_dataStatusIdx_u16 = (uint16)(*idxLoop_qu16);
            }
            break;
        }

    }

    if((*idxLoop_qu16 == DCM_CFG_RC_NUMRIDS) && (*NRC_u8 == 0x00))
    {
        *NRC_u8 = DCM_E_REQUESTOUTOFRANGE;
    }

    return RetVal;
}
#endif

/*
 *******************************************************************************************************************
 * Dcm_RcIsRIDSupported :
 *  This function is used to check if the RID is supported in the Ranges or single Routines.
 * \param  [in]     dataRId_u16     : Requested RID
 *         [out]    idxLoop_qu16    : index of the Rid in Routine or Routine Range structure
 *         [out]    NRC_u8          : NRC Value if RID is not supported
 * \retval boolean  flgProcessReq_b : TRUE --> RID is supported
 *                                   FALSE --> RID is not supported
 *******************************************************************************************************************/
static boolean Dcm_Prv_RcIsRIDSupported (uint16 dataRId_u16,
        uint16 * idxLoop_qu16,
        Dcm_NegativeResponseCodeType * NRC_u8)
{
    /*TRACE[SWS_Dcm_01139]*/
    /*TRACE[SWS_Dcm_00568]*/
    /* Return value to store the result of IsRidAvailable function */
    Std_ReturnType RetVal;
    /* Return variable to indicate if RID is supported or not. Initialize it to FALSE */
    boolean flgRidSupported_b = FALSE;
    /* Initialize the variable for RID structure index to default value */
    s_dataStatusIdx_u16 = 0xFFFF;
    /* Initialize the return value */
    RetVal = E_NOT_OK;
/*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-4035]*/
#if (DCM_CFG_RC_NUMRIDS > 0u)
        RetVal = Dcm_Prv_RcIsRIDSingleRoutine(&flgRidSupported_b,dataRId_u16,idxLoop_qu16,NRC_u8);
#endif

/* If RID was not found within the Normal Routines, then search in Range routines */
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
        if(!flgRidSupported_b)
        {
            RetVal = Dcm_Prv_RcIsRIDRangeRoutine(&flgRidSupported_b,dataRId_u16,idxLoop_qu16,NRC_u8);
        }
#endif

    /* Send NRC if RID was not found within the Range Routines and single routines */
    if((!flgRidSupported_b) && ((RetVal == E_NOT_OK) && (*NRC_u8 == 0x00)))
    {
        /* Set NRC 0x31 if RID is not supported */
       /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3900]*/
        *NRC_u8 = DCM_E_REQUESTOUTOFRANGE;
    }

    return flgRidSupported_b;
}

#if (DCM_CFG_RC_NUMRIDS > 0u)
/*
 ***********************************************************************************************************************
 Function name    : Dcm_Prv_RcProcessRoutine
 Syntax           : Dcm_Prv_RcProcessRoutine(void)
 Description      : This Function is used to process the Routine
 Parameter        :
 Return value     : none
***********************************************************************************************************************/
static Std_ReturnType Dcm_Prv_RcProcessRoutine(const Dcm_MsgContextType * pMsgContext,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
        uint16 dataRId_u16)
{
    /*TRACE[SWS_Dcm_01139]*/
    const Dcm_DspRoutineSignalInfo_tst * adrSignal_pcst;
    uint32 dataSessionBitMask_u32;
    uint32 dataSecurityBitMask_u32;
#if (DCM_CFG_ROUTINEVARLENGTH == DCM_CFG_OFF)
    uint32 dataSigVal_u32;
    uint16 idxLoop_qu16;
#endif
    uint16 nrCtrlOptRecSize_u16;
    uint8 dataSubFunc_u8;
    uint8 nrSig_u8;
    boolean flgReqSequenceError_b;
    boolean flgModeRetVal_b;
    Std_ReturnType dataRetVal_u8;

    /*Initialize the flag for Request sequence error is to FALSE*/
    flgReqSequenceError_b = FALSE;
    /*Initialize the flag for mode return value is to TRUE*/
    flgModeRetVal_b =TRUE;
    adrSignal_pcst = NULL_PTR;
    dataRetVal_u8=DCM_E_PENDING;
    dataSubFunc_u8 = pMsgContext->reqData[0];
    /* From the received length indicated to this service check what is left after these three bytes */
    nrCtrlOptRecSize_u16 = (uint16)(pMsgContext->reqDataLen - 3u);
    dataSessionBitMask_u32 = s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataSessBitMask_u32;

    /*TRACE[SWS_Dcm_00570]*/
    /* If dataRId_u16 is valid, then check if its allowed to execute that Routine in the active session and check if it is available in current DCM configuration if post build is enabled */
    if(((Dcm_DsldGetActiveSessionMask_u32() & dataSessionBitMask_u32) != 0u)
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
            && ((Dcm_ActiveConfiguration_u8 & Dcm_DspRoutine_cast[s_dataStatusIdx_u16].dataConfigMask_u8)!= 0u)
#endif
    )
    {
        /* Check now if its allowed to execute that Routine in the active security level */
        dataSecurityBitMask_u32 = s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataSecBitMask_u32;

        /*TRACE[SWS_Dcm_00571]*/
        /* Check now if its allowed to execute that Routine in the active security level */
        if((Dcm_DsldGetActiveSecurityMask_u32() & dataSecurityBitMask_u32) != 0u)
        {

            /* StartRoutine sub-function is a mandatory for any routine, but RequestResults and StopRoutine
             * aren't. Thus, it is required to check if requested sub-function is supported for that routine
             * which was requested */
            /*TRACE[SWS_Dcm_00869]*/
            if (( ( dataSubFunc_u8 == 0x01u ) && ( s_adrRoutine_pcst->flgStartRoutine_b != FALSE ))||
                    ( ( dataSubFunc_u8 == 0x02u ) && ( s_adrRoutine_pcst->flgStopRoutine_b != FALSE ))||
                    ( ( dataSubFunc_u8 == 0x03u ) && ( s_adrRoutine_pcst->dataReqRslt_b != FALSE ) ) )
            {
                /* Initialize the global variables to retain the values in case of DCM_E_PENDING */
                Dcm_adrRoutinePtr_pcst = s_adrRoutine_pcst;
                Dcm_dataRCSubFunc_u8 = dataSubFunc_u8;
                Dcm_RCOpStatus_u8 = DCM_INITIAL;
                Dcm_RCNegResCode_u8 = 0;
                Dcm_RCCurrDataLength_u16 = 0;
                Dcm_stDspRCState_en = DCM_RC_PENDING;
                nrSig_u8 = 0;

                /*
                 * Now check if the total request length is valid based on
                 * 1. If that routine is FixedLength or not
                 *   a. If FixedLength is TRUE, then the match should be for the exact length
                 *   b. If FixedLength is FALSE, then the check should be that the received length is not more
                 *   than the maximum length, but also not less to miss out the fixed signals within that dataRId_u16
                 */
                /*TRACE[SWS_Dcm_00590]*/
                if ( ( dataSubFunc_u8 == 0x01u ) &&
                        ( ( nrCtrlOptRecSize_u16 == s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeStart_u16 ) ||
                                ( ( s_adrRoutine_pcst->dataFixedLen_b == FALSE ) &&
                                        ( nrCtrlOptRecSize_u16 <= s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeStart_u16 ) &&
                                        ( nrCtrlOptRecSize_u16 >= s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStart_u16 ) ) ) )
                {


#if(DCM_CFG_STARTSUPPORTED != DCM_CFG_OFF)
                    adrSignal_pcst = s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrStartInSignalRef_cpcst;
                    nrSig_u8 = s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->nrStartInSignals_u8;
                    Dcm_RCCurrDataLength_u16 = (uint16)(nrCtrlOptRecSize_u16 - s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStart_u16);
                    Dcm_RCInPtr_pu8 = &(pMsgContext->reqData[3+s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStart_u16]);
                    Dcm_RCOutPtr_pu8 = &(pMsgContext->resData[3+s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeStart_u16]);
#endif
                }

#if(DCM_CFG_STOPSUPPORTED != DCM_CFG_OFF)
                else if ( ( dataSubFunc_u8 == 0x02u ) &&
                        ( ( nrCtrlOptRecSize_u16 == s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeStop_u16 ) ||
                                ( ( s_adrRoutine_pcst->dataFixedLen_b == FALSE ) &&
                                        ( nrCtrlOptRecSize_u16 <= s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeStop_u16 ) &&
                                        ( nrCtrlOptRecSize_u16 >= s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStop_u16 ) ) ) )
                {
                    /* Process the Stop request in case -
                     * 1.If Stop request comes without a start request in case Start routine is not supported
                     * 2.If Stop request comes without a start request or after a stop request in case RequestSequenceError is disabled and Start Routine is supported
                     * 3.If Stop request comes after a start request or start and request results request in case RequestSequenceError is enabled or disabled
                     */

                    if (((DCM_ROUTINE_IDLE == Dcm_RoutineStatus_aen[s_dataStatusIdx_u16])&& (s_adrRoutine_pcst->flgStartRoutine_b==FALSE ))||
                            (((DCM_ROUTINE_IDLE == Dcm_RoutineStatus_aen[s_dataStatusIdx_u16]) || (DCM_ROUTINE_STOP == Dcm_RoutineStatus_aen[s_dataStatusIdx_u16]))&& ( s_adrRoutine_pcst->flgReqSequenceErrorSupported_b==FALSE))||
                            ((DCM_ROUTINE_IDLE != Dcm_RoutineStatus_aen[s_dataStatusIdx_u16]) &&(DCM_ROUTINE_STOP != Dcm_RoutineStatus_aen[s_dataStatusIdx_u16])))
                    {
                        adrSignal_pcst = s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrStopInSignalRef_cpcst;
                        nrSig_u8 = s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->nrStopInSignals_u8;
                        Dcm_RCCurrDataLength_u16 = (uint16)(nrCtrlOptRecSize_u16 - s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStop_u16);
                        Dcm_RCInPtr_pu8 = &(pMsgContext->reqData[3+s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStop_u16]);
                        Dcm_RCOutPtr_pu8 = &(pMsgContext->resData[3+s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeStop_u16]);
                    }
                    else
                    {
                        /* Checking for the request sequence error */
                        /*Flag for request Sequence Error is set TRUE*/
                        flgReqSequenceError_b = TRUE;
                    }
                }
#endif
#if(DCM_CFG_REQRESULTSSUPPORTED != DCM_CFG_OFF)
                /*Special case:Request results with control option record*/
                else if  ((dataSubFunc_u8 == 0x03u) &&
                        (( nrCtrlOptRecSize_u16 == s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeReqRslt_u16) ||
                                ( ( s_adrRoutine_pcst->dataFixedLen_b == FALSE ) &&
                                        ( nrCtrlOptRecSize_u16 <= s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeReqRslt_u16  ) &&
                                        ( nrCtrlOptRecSize_u16 >= s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeReqRslt_u16 ) ) ))

                {
                    /* Process the requestResults request -
                     * 1. If Request results request comes without a start request for routines which so not support Start Routine
                     * 2. If Request results request comes for a Routine which is not started but supports Start Routine and RequestSequenceError is disabled
                     * 3. If Request results request comes for a Routine which is stared irrespective of whether RequestSequence error is enabled/disabled */
                    if (((DCM_ROUTINE_IDLE == Dcm_RoutineStatus_aen[s_dataStatusIdx_u16]) && (s_adrRoutine_pcst->flgStartRoutine_b==FALSE) )||
                            (( DCM_ROUTINE_IDLE == Dcm_RoutineStatus_aen[s_dataStatusIdx_u16])&& ( s_adrRoutine_pcst->flgReqSequenceErrorSupported_b==FALSE))||
                            ((DCM_ROUTINE_IDLE != Dcm_RoutineStatus_aen[s_dataStatusIdx_u16])))
                    {
                        adrSignal_pcst = s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrReqRslInSignalRef_cpcst;
                        nrSig_u8 = s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->nrReqRslInSignals_u8;
                        Dcm_RCCurrDataLength_u16 = (uint16)(nrCtrlOptRecSize_u16 - s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeReqRslt_u16);
                        Dcm_RCInPtr_pu8 = &(pMsgContext->reqData[3+s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeReqRslt_u16]);
                        Dcm_RCOutPtr_pu8 = &(pMsgContext->resData[3+s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeReqRslt_u16]);
                    }
                    else
                    {
                        /* Checking for the request sequence error */
                        /*Flag for request Sequence Error is set TRUE*/
                        flgReqSequenceError_b = TRUE;
                    }
                }
#endif
                /*TRACE[SWS_Dcm_01140]*/
                else
                {
                    /* Received message has a invalid length for this RID */
                    *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                    /* Reset the RC State back to IDLE so that it does not process the request further */
                    Dcm_stDspRCState_en = DCM_RC_IDLE;
                }

                if(*dataNegRespCode_u8 == 0)
                {
                    /*If control option record is present in the request call the appl function to check the validity */
                    if(nrCtrlOptRecSize_u16 > 0)
                    {
                        dataRetVal_u8 = DcmAppl_DcmCheckRoutineControlOptionRecord(dataRId_u16,dataSubFunc_u8,&pMsgContext->reqData[3],nrCtrlOptRecSize_u16);
                        if (E_OK != dataRetVal_u8)
                        {   /*Control option record is incorrect.*/
                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                            Dcm_stDspRCState_en = DCM_RC_IDLE;
                        }
                    }
                    if(*dataNegRespCode_u8 == 0)
                    {
                        /**********Mode rule Check*************/
                        if(s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrUserRidModeRule_pfct!=NULL_PTR)
                        {
                            /* Call the configured Dcm Appl API to do RID specific checks */
                            dataRetVal_u8 = (*s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrUserRidModeRule_pfct)(dataNegRespCode_u8,dataRId_u16,dataSubFunc_u8);
                        }
                        else
                        {
                            /* Call the user Dcm Appl API to do RID specific checks */
                            dataRetVal_u8 = DcmAppl_UserRIDModeRuleService(dataNegRespCode_u8,dataRId_u16,dataSubFunc_u8);
                        }
                        /* Ignore the NRC in case the DcmAppl API returns TRUE */
                        if(dataRetVal_u8 !=E_OK)
                        {
                            if(*dataNegRespCode_u8==0x00)
                            {
                                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                            }
                            flgModeRetVal_b = FALSE;
                        }
                        else
                        {
                            *dataNegRespCode_u8=0x00;
                        }
#if(DCM_CFG_DSP_MODERULEFORROUTINES != DCM_CFG_OFF)
                        if(*dataNegRespCode_u8 == 0x00)
                        {
                            /*TRACE[SWS_Dcm_01169]*/
                            if (( dataSubFunc_u8 == 0x01u ) && (s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForStart_pfct != NULL_PTR))
                            {

                                flgModeRetVal_b=(*(s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForStart_pfct))(dataNegRespCode_u8);
                            }
                            /*TRACE[SWS_Dcm_01170]*/
                            else if (( dataSubFunc_u8 == 0x02u ) && (s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForStop_pfct != NULL_PTR))

                            {
                                flgModeRetVal_b=(*(s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForStop_pfct))(dataNegRespCode_u8);
                            }
                            /*TRACE[SWS_Dcm_01171]*/
                            else if (( dataSubFunc_u8 == 0x03u ) && (s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForRequestResult_pfct != NULL_PTR))
                            {
                                flgModeRetVal_b=(*(s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForRequestResult_pfct))(dataNegRespCode_u8);
                            }
                            else
                            {
                                /*dummy else */
                            }
                        }
#endif
                        if(flgModeRetVal_b == FALSE)
                        {
                            Dcm_stDspRCState_en = DCM_RC_IDLE;
                        }
                        else
                        {
                            /* Request sequence error check */

                            if(flgReqSequenceError_b !=FALSE)
                            {
                                *dataNegRespCode_u8 = DCM_E_REQUESTSEQUENCEERROR;
                                Dcm_stDspRCState_en = DCM_RC_IDLE;
                            }
                            /*TRACE[SWS_Dcm_01141]*/
                            else
                            {
                                /*DCM_CFG_ROUTINEVARLENGTH shall be DCM_CFG_ON only if all the RIDs are of variable length and no fixed length signals are configured under them*/
#if (DCM_CFG_ROUTINEVARLENGTH == DCM_CFG_OFF)
                                /*TRACE[SWS_Dcm_00641]*/
                                for ( idxLoop_qu16 = 0 ; ( ( idxLoop_qu16 < nrSig_u8 ) && (*dataNegRespCode_u8==0x00)&& (adrSignal_pcst!=NULL_PTR) &&( adrSignal_pcst->dataType_u8 != DCM_VARIABLE_LENGTH ) ) ; idxLoop_qu16++ )
                                {
#if (DCM_CFG_ROUTINEARRAY_INSIG != DCM_CFG_OFF)
                                    if(adrSignal_pcst->dataType_u8 >= DCM_RCARRAYINDEX)
                                    {
                                        Dcm_RcSetSignalArray(adrSignal_pcst,&(pMsgContext->reqData[3]));
                                    }
                                    else
#endif
                                    {
                                        dataSigVal_u32 = 0;
                                        dataSigVal_u32 = Dcm_GetSignal_u32(adrSignal_pcst->dataType_u8, adrSignal_pcst->posnStart_u16, &(pMsgContext->reqData[3]),adrSignal_pcst->dataEndianness_u8);
                                        Dcm_RCSetSigVal(adrSignal_pcst->dataType_u8,adrSignal_pcst->idxSignal_u16,dataSigVal_u32);
                                    }
                                    adrSignal_pcst++;
                                }

#endif
#if (DCM_CFG_IN_PARAM_MAXLEN > 0u)
                                if(adrSignal_pcst != NULL_PTR)
                                {
                                    /* Copy Varialble length Control Option Record into buffer */
                                    if((adrSignal_pcst->dataType_u8 == DCM_VARIABLE_LENGTH) && (Dcm_RCCurrDataLength_u16 != 0x00))
                                    {
                                        /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type pointer is converted to void pointer*/
                                        DCM_MEMCOPY(&(Dcm_InParameterBuf_au8[0]),Dcm_RCInPtr_pu8 ,Dcm_RCCurrDataLength_u16);
                                    }
                                }
#endif
                            }
                        }
                    }
                }
            }
            else
            {
                /* Requested Sub-Function is not supported for this RID */
                *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
            }
        }
        else
        {
            /* Routine Control has failed the Security Check */
            *dataNegRespCode_u8 = DCM_E_SECURITYACCESSDENIED;
        }
    }
    else
    {
        /* Routine Identifier is not supported */
        *dataNegRespCode_u8=DCM_E_REQUESTOUTOFRANGE;
    }
    return dataRetVal_u8;
}

/*
**********************************************************************************************************************
 Function name    : Dcm_Prv_RcPendingRoutine
 Syntax           : Dcm_Prv_RcPendingRoutine(void)
 Description      : This Function is used to process the Pending Routine
 Parameter        :
 Return value     : none
***********************************************************************************************************************/
static Std_ReturnType Dcm_Prv_RcPendingRoutine(Dcm_MsgContextType * pMsgContext,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    /*TRACE[SWS_Dcm_01139]*/
    const Dcm_DspRoutineSignalInfo_tst * adrSignal_pcst;
#if (DCM_CFG_ROUTINEVARLENGTH == DCM_CFG_OFF)
    uint32 dataSigVal_u32;
#endif
    uint16 idxLoop_qu16;
    uint8 nrSig_u8;
    Std_ReturnType dataRetVal_u8;
    adrSignal_pcst = NULL_PTR;
    dataRetVal_u8=DCM_E_PENDING;

    /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
     * Hence tehre is no data inconsistency involved and the lock is not necessary. */
    /*Set the Flag for s_IsRCSubfunctionCalled_b is set to TRUE*/
    s_IsRCSubfunctionCalled_b = TRUE;
    /*TRACE[SWS_Dcm_00400]*/
    /*TRACE[SWS_Dcm_00402]*/
    /*TRACE[SWS_Dcm_00404]*/
    dataRetVal_u8 = (*Dcm_adrRoutinePtr_pcst->adrCallRoutine_pfct)(Dcm_dataRCSubFunc_u8);
    /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
     * Hence tehre is no data inconsistency involved and the lock is not necessary. */
    s_IsRCSubfunctionCalled_b = FALSE;
    /*Check for Infrastructure Error present or not and use routine port is supported */
    if((Dcm_IsInfrastructureErrorPresent_b(dataRetVal_u8) != FALSE) && (Dcm_adrRoutinePtr_pcst->UsePort != FALSE))
    {
        dataRetVal_u8 = DCM_INFRASTRUCTURE_ERROR;
    }
    if (dataRetVal_u8 == E_OK)
    {
        /*TRACE[SWS_Dcm_00401]*/
        if ( Dcm_dataRCSubFunc_u8 == 0x01 )
        {
#if(DCM_CFG_STARTSUPPORTED != DCM_CFG_OFF)
            adrSignal_pcst = Dcm_adrRoutinePtr_pcst->adrRoutineInfoRef_cpcst->adrStartOutSignalRef_cpcst;
            nrSig_u8 = Dcm_adrRoutinePtr_pcst->adrRoutineInfoRef_cpcst->nrStartOutSignals_u8;
            pMsgContext->resDataLen = 3u + (uint32)Dcm_adrRoutinePtr_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeStart_u16;

            /*Routine is Started  ,Set the status to RC_START_OK  */
            Dcm_RoutineStatus_aen[s_dataStatusIdx_u16] = DCM_ROUTINE_STARTED;
#endif
        }
#if(DCM_CFG_STOPSUPPORTED != DCM_CFG_OFF)
        /*TRACE[SWS_Dcm_00403]*/
        else if ( Dcm_dataRCSubFunc_u8 == 0x02 )
        {
            adrSignal_pcst = Dcm_adrRoutinePtr_pcst->adrRoutineInfoRef_cpcst->adrStopOutSignalRef_cpcst;
            nrSig_u8 = Dcm_adrRoutinePtr_pcst->adrRoutineInfoRef_cpcst->nrStopOutSignals_u8;
            pMsgContext->resDataLen = 3u + (uint32)Dcm_adrRoutinePtr_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeStop_u16;
            /*Routine is Stopped   ,Set the  status to default status DCM_ROUTINE_STOP  */
            Dcm_RoutineStatus_aen[s_dataStatusIdx_u16] = DCM_ROUTINE_STOP;
        }
#endif
        /*TRACE[SWS_Dcm_00405]*/
        else
        {
#if(DCM_CFG_REQRESULTSSUPPORTED != DCM_CFG_OFF)
            adrSignal_pcst = Dcm_adrRoutinePtr_pcst->adrRoutineInfoRef_cpcst->adrReqRsltOutSignalRef_cpcst;
            nrSig_u8 = Dcm_adrRoutinePtr_pcst->adrRoutineInfoRef_cpcst->nrReqRsltOutSignals_u8;
            pMsgContext->resDataLen = 3u + (uint32)Dcm_adrRoutinePtr_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeReqRslt_u16;
#endif
        }
        /*TRACE[SWS_Dcm_00641]*/
        for ( idxLoop_qu16 = 0 ; ( ( idxLoop_qu16 < nrSig_u8 ) && ( adrSignal_pcst->dataType_u8 != DCM_VARIABLE_LENGTH ) ) ; idxLoop_qu16++ )
        {
#if(DCM_CFG_ROUTINEVARLENGTH == DCM_CFG_OFF)
#if(DCM_CFG_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF)
            if(adrSignal_pcst->dataType_u8 >= DCM_RCARRAYINDEX)
            {
                Dcm_RcStoreSignalArray(adrSignal_pcst,&(pMsgContext->resData[3]));
            }
            else
#endif
            {
                dataSigVal_u32 = Dcm_RCGetSigVal_u32 ( adrSignal_pcst->dataType_u8, adrSignal_pcst->idxSignal_u16 );
                Dcm_StoreSignal(adrSignal_pcst->dataType_u8, adrSignal_pcst->posnStart_u16, &(pMsgContext->resData[3]), dataSigVal_u32, adrSignal_pcst->dataEndianness_u8 );
            }
#endif
            adrSignal_pcst++;
        }

        if ( idxLoop_qu16 != nrSig_u8 )
        {
            /* You enter here only if the last signal has dynamic or variable length */
            pMsgContext->resDataLen = pMsgContext->resDataLen + Dcm_RCCurrDataLength_u16;
        }

        /* First byte after Service Id is the routineControlType or Sub-function byte */
        pMsgContext->resData[0] = Dcm_dataRCSubFunc_u8;
        /* Second and third byte is the two byte dataRId_u16 */
        pMsgContext->resData[1] = ( uint8 )( Dcm_adrRoutinePtr_pcst->dataRId_u16 >> 8u ) ;
        pMsgContext->resData[2] = ( uint8 )( Dcm_adrRoutinePtr_pcst->dataRId_u16 & 0x00ffu ) ;

        Dcm_stDspRCState_en = DCM_RC_IDLE;
    }
    else if (dataRetVal_u8 == E_NOT_OK)
    {
        /* Copy the NRC value which has been updated by Application to the local variable */
        if ( Dcm_RCNegResCode_u8 != 0 )
        {
            *dataNegRespCode_u8 = Dcm_RCNegResCode_u8;
        }
        else
        {
            /* Conditions Not Correct is set as the Application did not fill in any NegativeResponseCode but have returned E_NOT_OK */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
        }
    }
    else if (dataRetVal_u8 == DCM_E_PENDING)
    {
        /* Nothing to be done here except for setting the OpStatus, DSL/DSD shall trigger this service again in the next main function call */
        Dcm_RCOpStatus_u8 = DCM_PENDING;

        /* As NRC code has been updated, terminate processing and send NRC to tester */
    }
    else if (dataRetVal_u8 == DCM_E_FORCE_RCRRP)
    {
        /*Do nothing NRC78 is triggered from the Statemachine*/
        Dcm_RCOpStatus_u8=DCM_FORCE_RCRRP_OK;
    }
    else if ((dataRetVal_u8 == DCM_INFRASTRUCTURE_ERROR) && (Dcm_adrRoutinePtr_pcst->UsePort != FALSE))
    {
        *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
    }
    else
    {
        if ( Dcm_RCNegResCode_u8 == 0 )
        {
            /* Function returns a return value not defined within Routine Control service, but hasn't set NRC */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
        }
        else
        {
            /* As NRC code has been updated, terminate processing and send NRC to tester */
            *dataNegRespCode_u8 = Dcm_RCNegResCode_u8;
        }
    }
    return dataRetVal_u8;
}
#endif

#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
/*
 ***********************************************************************************************************************
 Function name    : Dcm_Prv_RcProcessRangeRoutine
 Syntax           : Dcm_Prv_RcProcessRangeRoutine(void)
 Description      : This Function is used to process the Range Routine
 Parameter        :
 Return value     : none
***********************************************************************************************************************/
static Std_ReturnType Dcm_Prv_RcProcessRangeRoutine(const Dcm_MsgContextType * pMsgContext,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
        uint16 dataRId_u16)
{
    /*TRACE[SWS_Dcm_01139]*/
    const Dcm_DspRoutineSignalInfo_tst * adrSignal_pcst;
    uint32 dataSessionBitMask_u32;
    uint32 dataSecurityBitMask_u32;
#if (DCM_CFG_RCRANGE_ROUTINEVARLENGTH == DCM_CFG_OFF)
    uint32 dataSigVal_u32;
    uint16 idxLoop_qu16;
#endif
    uint16 nrCtrlOptRecSize_u16;
    uint8 dataSubFunc_u8;
    uint8 nrSig_u8;
    boolean flgReqSequenceError_b;
    boolean flgModeRetVal_b;
    Std_ReturnType dataRetVal_u8;

    /*Initialize the flag for Request sequence error is to FALSE*/
    flgReqSequenceError_b = FALSE;
    /*Initialize the flag for mode return value is to TRUE*/
    flgModeRetVal_b =TRUE;
    adrSignal_pcst = NULL_PTR;
    dataRetVal_u8=DCM_E_PENDING;
    dataSubFunc_u8 = pMsgContext->reqData[0];
    /* From the received length indicated to this service check what is left after these three bytes */
    nrCtrlOptRecSize_u16 = (uint16)(pMsgContext->reqDataLen - 3u);
    dataSessionBitMask_u32 = s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataSessBitMask_u32;

    /*TRACE[SWS_Dcm_00570]*/
    /* If dataRId_u16 is valid, then check if its allowed to execute that Routine in the active session and check if it is available in current DCM configuration if post build is enabled */
    if(((Dcm_DsldGetActiveSessionMask_u32() & dataSessionBitMask_u32) != 0u)
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
            && ((Dcm_ActiveConfiguration_u8 & Dcm_DspRoutineRange_cast[s_dataStatusIdx_u16].dataConfigMask_u8)!= 0u)
#endif
    )
    {
        /* Check now if its allowed to execute that Routine in the active security level */
        dataSecurityBitMask_u32 = s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataSecBitMask_u32;

        /*TRACE[SWS_Dcm_00571]*/
        /* Check now if its allowed to execute that Routine in the active security level */
        if((Dcm_DsldGetActiveSecurityMask_u32() & dataSecurityBitMask_u32) != 0u)
        {

            /* StartRoutine sub-function is a mandatory for any routine, but RequestResults and StopRoutine
             * aren't. Thus, it is required to check if requested sub-function is supported for that routine
             * which was requested */
            /*TRACE[SWS_Dcm_00869]*/
            if (( ( dataSubFunc_u8 == 0x01u ) && ( s_adrRoutineRange_pcst->flgStartRoutine_b != FALSE ))||
                    ( ( dataSubFunc_u8 == 0x02u ) && ( s_adrRoutineRange_pcst->flgStopRoutine_b != FALSE ))||
                    ( ( dataSubFunc_u8 == 0x03u ) && ( s_adrRoutineRange_pcst->flgReqRsltRoutine_b != FALSE ) ) )
            {
                /* Initialize the global variables to retain the values in case of DCM_E_PENDING */
                Dcm_adrRoutineRangePtr_pcst = s_adrRoutineRange_pcst;
                s_PendingRidRange_u16 = dataRId_u16;
                Dcm_dataRCSubFunc_u8 = dataSubFunc_u8;
                Dcm_RCOpStatus_u8 = DCM_INITIAL;
                Dcm_RCNegResCode_u8 = 0;
                Dcm_RCCurrDataLength_u16 = 0;
                Dcm_stDspRCState_en = DCM_RC_PENDING;
                nrSig_u8 = 0;

                /*
                 * Now check if the total request length is valid based on
                 * 1. If that routine is FixedLength or not
                 *   a. If FixedLength is TRUE, then the match should be for the exact length
                 *   b. If FixedLength is FALSE, then the check should be that the received length is not more
                 *   than the maximum length, but also not less to miss out the fixed signals within that dataRId_u16
                 */
                /*TRACE[SWS_Dcm_00590]*/
                if ( ( dataSubFunc_u8 == 0x01u ) &&
                        ( ( nrCtrlOptRecSize_u16 == s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeStart_u16 ) ||
                                ( ( s_adrRoutineRange_pcst->dataFixedLen_b == FALSE ) &&
                                        ( nrCtrlOptRecSize_u16 <= s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeStart_u16 ) &&
                                        ( nrCtrlOptRecSize_u16 >= s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStart_u16 ) ) ) )
                {


#if(DCM_CFG_RCRANGE_STARTSUPPORTED != DCM_CFG_OFF)
                    adrSignal_pcst = s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrStartInSignalRef_cpcst;
                    nrSig_u8 = s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->nrStartInSignals_u8;
                    Dcm_RCCurrDataLength_u16 = (uint16)(nrCtrlOptRecSize_u16 - s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStart_u16);
                    Dcm_RCInPtr_pu8 = &(pMsgContext->reqData[3+s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStart_u16]);
                    Dcm_RCOutPtr_pu8 = &(pMsgContext->resData[3+s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeStart_u16]);
#endif
                }

#if(DCM_CFG_RCRANGE_STOPSUPPORTED != DCM_CFG_OFF)
                else if ( ( dataSubFunc_u8 == 0x02u ) &&
                        ( ( nrCtrlOptRecSize_u16 == s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeStop_u16 ) ||
                                ( ( s_adrRoutineRange_pcst->dataFixedLen_b == FALSE ) &&
                                        ( nrCtrlOptRecSize_u16 <= s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeStop_u16 ) &&
                                        ( nrCtrlOptRecSize_u16 >= s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStop_u16 ) ) ) )
                {
                    /* Process the Stop request in case -
                     * 1.If Stop request comes without a start request in case Start routine is not supported
                     * 2.If Stop request comes without a start request or after a stop request in case RequestSequenceError is disabled and Start Routine is supported
                     * 3.If Stop request comes after a start request or start and request results request in case RequestSequenceError is enabled or disabled
                     */

                    if (((DCM_ROUTINE_IDLE == Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16])&& (s_adrRoutineRange_pcst->flgStartRoutine_b==FALSE ))||
                            (((DCM_ROUTINE_IDLE == Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16]) || (DCM_ROUTINE_STOP == Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16]))&& ( s_adrRoutineRange_pcst->flgReqSequenceErrorSupported_b==FALSE))||
                            ((DCM_ROUTINE_IDLE != Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16]) &&(DCM_ROUTINE_STOP != Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16])))
                    {
                        adrSignal_pcst = s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrStopInSignalRef_cpcst;
                        nrSig_u8 = s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->nrStopInSignals_u8;
                        Dcm_RCCurrDataLength_u16 = (uint16)(nrCtrlOptRecSize_u16 - s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStop_u16);
                        Dcm_RCInPtr_pu8 = &(pMsgContext->reqData[3+s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeStop_u16]);
                        Dcm_RCOutPtr_pu8 = &(pMsgContext->resData[3+s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeStop_u16]);
                    }
                    else
                    {
                        /* Checking for the request sequence error */
                        /*Flag for request Sequence Error is set TRUE*/
                        flgReqSequenceError_b = TRUE;
                    }
                }
#endif
#if(DCM_CFG_RCRANGE_REQRESULTSSUPPORTED != DCM_CFG_OFF)
                /*Special case:Request results with control option record*/
                else if  ((dataSubFunc_u8 == 0x03u) &&
                        (( nrCtrlOptRecSize_u16 == s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeReqRslt_u16) ||
                                ( ( s_adrRoutineRange_pcst->dataFixedLen_b == FALSE ) &&
                                        ( nrCtrlOptRecSize_u16 <= s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataCtrlOptRecSizeReqRslt_u16  ) &&
                                        ( nrCtrlOptRecSize_u16 >= s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeReqRslt_u16 ) ) ))

                {
                    /* Process the requestResults request -
                     * 1. If Request results request comes without a start request for routines which so not support Start Routine
                     * 2. If Request results request comes for a Routine which is not started but supports Start Routine and RequestSequenceError is disabled
                     * 3. If Request results request comes for a Routine which is stared irrespective of whether RequestSequence error is enabled/disabled */
                    if (((DCM_ROUTINE_IDLE == Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16]) && (s_adrRoutineRange_pcst->flgStartRoutine_b==FALSE) )||
                            (( DCM_ROUTINE_IDLE == Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16])&& ( s_adrRoutineRange_pcst->flgReqSequenceErrorSupported_b==FALSE))||
                            ((DCM_ROUTINE_IDLE != Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16])))
                    {
                        adrSignal_pcst = s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrReqRslInSignalRef_cpcst;
                        nrSig_u8 = s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->nrReqRslInSignals_u8;
                        Dcm_RCCurrDataLength_u16 = (uint16)(nrCtrlOptRecSize_u16 - s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeReqRslt_u16);
                        Dcm_RCInPtr_pu8 = &(pMsgContext->reqData[3+s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinCtrlOptRecSizeReqRslt_u16]);
                        Dcm_RCOutPtr_pu8 = &(pMsgContext->resData[3+s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeReqRslt_u16]);
                    }
                    else
                    {
                        /* Checking for the request sequence error */
                        /*Flag for request Sequence Error is set TRUE*/
                        flgReqSequenceError_b = TRUE;
                    }
                }
#endif
                /*TRACE[SWS_Dcm_01140]*/
                else
                {
                    /* Received message has a invalid length for this RID */
                    *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                    /* Reset the RC State back to IDLE so that it does not process the request further */
                    Dcm_stDspRCState_en = DCM_RC_IDLE;
                }

                if(*dataNegRespCode_u8 == 0)
                {
                    /*If control option record is present in the request call the appl function to check the validity */
                    if(nrCtrlOptRecSize_u16 > 0)
                    {
                        dataRetVal_u8 = DcmAppl_DcmCheckRoutineControlOptionRecord(dataRId_u16,dataSubFunc_u8,&pMsgContext->reqData[3],nrCtrlOptRecSize_u16);
                        if (E_OK != dataRetVal_u8)
                        {   /*Control option record is incorrect.*/
                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                            Dcm_stDspRCState_en = DCM_RC_IDLE;
                        }
                    }
                    if(*dataNegRespCode_u8 == 0)
                    {
                        /**********Mode rule Check*************/
                        if(s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrUserRidModeRule_pfct!=NULL_PTR)
                        {
                            /* Call the configured Dcm Appl API to do RID specific checks */
                            dataRetVal_u8 = (*s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrUserRidModeRule_pfct)(dataNegRespCode_u8,dataRId_u16,dataSubFunc_u8);
                        }
                        else
                        {
                            /* Call the user Dcm Appl API to do RID specific checks */
                            dataRetVal_u8 = DcmAppl_UserRIDModeRuleService(dataNegRespCode_u8,dataRId_u16,dataSubFunc_u8);
                        }
                        /* Ignore the NRC in case the DcmAppl API returns TRUE */
                        if(dataRetVal_u8 !=E_OK)
                        {
                            if(*dataNegRespCode_u8==0x00)
                            {
                                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                            }
                            flgModeRetVal_b = FALSE;
                        }
                        else
                        {
                            *dataNegRespCode_u8=0x00;
                        }
#if(DCM_CFG_DSP_MODERULEFORRCRANGE != DCM_CFG_OFF)
                        if(*dataNegRespCode_u8 == 0x00)
                        {
                            /*TRACE[SWS_Dcm_01169]*/
                            if (( dataSubFunc_u8 == 0x01u ) && (s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForStart_pfct != NULL_PTR))
                            {

                                flgModeRetVal_b=(*(s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForStart_pfct))(dataNegRespCode_u8);
                            }
                            /*TRACE[SWS_Dcm_01170]*/
                            else if (( dataSubFunc_u8 == 0x02u ) && (s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForStop_pfct != NULL_PTR))

                            {
                                flgModeRetVal_b=(*(s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForStop_pfct))(dataNegRespCode_u8);
                            }
                            /*TRACE[SWS_Dcm_01171]*/
                            else if (( dataSubFunc_u8 == 0x03u ) && (s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForRequestResult_pfct != NULL_PTR))
                            {
                                flgModeRetVal_b=(*(s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->adrModeRuleForRequestResult_pfct))(dataNegRespCode_u8);
                            }
                            else
                            {
                                /*dummy else */
                            }
                        }
#endif
                        if(flgModeRetVal_b == FALSE)
                        {
                            Dcm_stDspRCState_en = DCM_RC_IDLE;
                        }
                        else
                        {
                            /* Request sequence error check */

                            if(flgReqSequenceError_b !=FALSE)
                            {
                                *dataNegRespCode_u8 = DCM_E_REQUESTSEQUENCEERROR;
                                Dcm_stDspRCState_en = DCM_RC_IDLE;
                            }
                            /*TRACE[SWS_Dcm_01141]*/
                            else
                            {
                                /*DCM_CFG_RCRANGE_ROUTINEVARLENGTH shall be DCM_CFG_ON only if all the RIDs are of variable length and no fixed length signals are configured under them*/
#if (DCM_CFG_RCRANGE_ROUTINEVARLENGTH == DCM_CFG_OFF)
                                /*TRACE[SWS_Dcm_00641]*/
                                for ( idxLoop_qu16 = 0 ; ( ( idxLoop_qu16 < nrSig_u8 ) && (*dataNegRespCode_u8==0x00)&& (adrSignal_pcst!=NULL_PTR) &&( adrSignal_pcst->dataType_u8 != DCM_VARIABLE_LENGTH ) ) ; idxLoop_qu16++ )
                                {
#if (DCM_CFG_RCRANGE_ROUTINEARRAY_INSIG != DCM_CFG_OFF)
                                    if(adrSignal_pcst->dataType_u8 >= DCM_RCARRAYINDEX)
                                    {
                                        Dcm_RcSetSignalArray(adrSignal_pcst,&(pMsgContext->reqData[3]));
                                    }
                                    else
#endif
                                    {
                                        dataSigVal_u32 = 0;
                                        dataSigVal_u32 = Dcm_GetSignal_u32(adrSignal_pcst->dataType_u8, adrSignal_pcst->posnStart_u16, &(pMsgContext->reqData[3]),adrSignal_pcst->dataEndianness_u8);
                                        Dcm_RCSetSigVal(adrSignal_pcst->dataType_u8,adrSignal_pcst->idxSignal_u16,dataSigVal_u32);
                                    }
                                    adrSignal_pcst++;
                                }

#endif
#if (DCM_CFG_IN_PARAM_MAXLEN > 0u)
                                if(adrSignal_pcst != NULL_PTR)
                                {
                                    /* Copy Varialble length Control Option Record into buffer */
                                    if((adrSignal_pcst->dataType_u8 == DCM_VARIABLE_LENGTH) && (Dcm_RCCurrDataLength_u16 != 0x00))
                                    {
                                        /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type pointer is converted to void pointer*/
                                        DCM_MEMCOPY(&(Dcm_InParameterBuf_au8[0]),Dcm_RCInPtr_pu8 ,Dcm_RCCurrDataLength_u16);
                                    }
                                }
#endif
                            }
                        }
                    }
                }
            }
            else
            {
                /* Requested Sub-Function is not supported for this RID */
                *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
            }
        }
        else
        {
            /* Routine Control has failed the Security Check */
            *dataNegRespCode_u8 = DCM_E_SECURITYACCESSDENIED;
        }
    }
    else
    {
        /* Routine Identifier is not supported */
        *dataNegRespCode_u8=DCM_E_REQUESTOUTOFRANGE;
    }
    return dataRetVal_u8;
}

/*
 ***********************************************************************************************************************
 Function name    : Dcm_Prv_RcPendingRangeRoutine
 Syntax           : Dcm_Prv_RcPendingRangeRoutine(void)
 Description      : This Function is used to process the Pending Range Routine
 Parameter        :
 Return value     : none
***********************************************************************************************************************/
static Std_ReturnType Dcm_Prv_RcPendingRangeRoutine(Dcm_MsgContextType * pMsgContext,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    /*TRACE[SWS_Dcm_01139]*/
    const Dcm_DspRoutineSignalInfo_tst * adrSignal_pcst;
#if (DCM_CFG_RCRANGE_ROUTINEVARLENGTH == DCM_CFG_OFF)
    uint32 dataSigVal_u32;
#endif
    uint16 idxLoop_qu16;
    uint8 nrSig_u8;
    Std_ReturnType dataRetVal_u8;
    adrSignal_pcst = NULL_PTR;
    dataRetVal_u8=DCM_E_PENDING;

    /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
     * Hence tehre is no data inconsistency involved and the lock is not necessary. */
    /*Set the Flag for s_IsRCSubfunctionCalled_b is set to TRUE*/
    s_IsRCSubfunctionCalled_b = TRUE;
    /*TRACE[SWS_Dcm_00400]*/
    /*TRACE[SWS_Dcm_00402]*/
    /*TRACE[SWS_Dcm_00404]*/
    dataRetVal_u8 = (*Dcm_adrRoutineRangePtr_pcst->adrCallRoutine_pfct)(Dcm_dataRCSubFunc_u8);
    /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
     * Hence tehre is no data inconsistency involved and the lock is not necessary. */
    s_IsRCSubfunctionCalled_b = FALSE;
    /*Check for Infrastructure Error present or not and use routine port is supported */
    if((Dcm_IsInfrastructureErrorPresent_b(dataRetVal_u8) != FALSE) && (Dcm_adrRoutineRangePtr_pcst->RoutineRangeUsePort_b != FALSE))
    {
        dataRetVal_u8 = DCM_INFRASTRUCTURE_ERROR;
    }
    if (dataRetVal_u8 == E_OK)
    {
        /*TRACE[SWS_Dcm_00401]*/
        if ( Dcm_dataRCSubFunc_u8 == 0x01 )
        {
#if(DCM_CFG_RCRANGE_STARTSUPPORTED != DCM_CFG_OFF)
            adrSignal_pcst = Dcm_adrRoutineRangePtr_pcst->adrRoutineInfoRef_cpcst->adrStartOutSignalRef_cpcst;
            nrSig_u8 = Dcm_adrRoutineRangePtr_pcst->adrRoutineInfoRef_cpcst->nrStartOutSignals_u8;
            pMsgContext->resDataLen = 3u + (uint32)Dcm_adrRoutineRangePtr_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeStart_u16;

            /*Routine is Started  ,Set the status to RC_START_OK  */
            Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16] = DCM_ROUTINE_STARTED;
#endif
        }
        /*TRACE[SWS_Dcm_00403]*/
#if(DCM_CFG_RCRANGE_STOPSUPPORTED != DCM_CFG_OFF)
        else if ( Dcm_dataRCSubFunc_u8 == 0x02 )
        {
            adrSignal_pcst = Dcm_adrRoutineRangePtr_pcst->adrRoutineInfoRef_cpcst->adrStopOutSignalRef_cpcst;
            nrSig_u8 = Dcm_adrRoutineRangePtr_pcst->adrRoutineInfoRef_cpcst->nrStopOutSignals_u8;
            pMsgContext->resDataLen = 3u + (uint32)Dcm_adrRoutineRangePtr_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeStop_u16;
            /*Routine is Stopped   ,Set the  status to default status DCM_ROUTINE_STOP  */
            Dcm_RoutineRangeStatus_aen[s_dataStatusIdx_u16] = DCM_ROUTINE_STOP;
        }
#endif
        /*TRACE[SWS_Dcm_00405]*/
        else
        {
#if(DCM_CFG_RCRANGE_REQRESULTSSUPPORTED != DCM_CFG_OFF)
            adrSignal_pcst = Dcm_adrRoutineRangePtr_pcst->adrRoutineInfoRef_cpcst->adrReqRsltOutSignalRef_cpcst;
            nrSig_u8 = Dcm_adrRoutineRangePtr_pcst->adrRoutineInfoRef_cpcst->nrReqRsltOutSignals_u8;
            pMsgContext->resDataLen = 3u + (uint32)Dcm_adrRoutineRangePtr_pcst->adrRoutineInfoRef_cpcst->dataMinStsOptRecSizeReqRslt_u16;
#endif
        }
        /*TRACE[SWS_Dcm_00641]*/
        for ( idxLoop_qu16 = 0 ; ( ( idxLoop_qu16 < nrSig_u8 ) && ( adrSignal_pcst->dataType_u8 != DCM_VARIABLE_LENGTH ) ) ; idxLoop_qu16++ )
        {
#if(DCM_CFG_RCRANGE_ROUTINEVARLENGTH == DCM_CFG_OFF)
#if(DCM_CFG_RCRANGE_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF)
            if(adrSignal_pcst->dataType_u8 >= DCM_RCARRAYINDEX)
            {
                Dcm_RcStoreSignalArray(adrSignal_pcst,&(pMsgContext->resData[3]));
            }
            else
#endif
            {
                dataSigVal_u32 = Dcm_RCGetSigVal_u32 ( adrSignal_pcst->dataType_u8, adrSignal_pcst->idxSignal_u16 );
                Dcm_StoreSignal(adrSignal_pcst->dataType_u8, adrSignal_pcst->posnStart_u16, &(pMsgContext->resData[3]), dataSigVal_u32, adrSignal_pcst->dataEndianness_u8 );
            }
#endif
            adrSignal_pcst++;
        }

        if ( idxLoop_qu16 != nrSig_u8 )
        {
            /* You enter here only if the last signal has dynamic or variable length */
            pMsgContext->resDataLen = pMsgContext->resDataLen + Dcm_RCCurrDataLength_u16;
        }

        /* First byte after Service Id is the routineControlType or Sub-function byte */
        pMsgContext->resData[0] = Dcm_dataRCSubFunc_u8;
        /* Second and third byte is the two byte dataRId_u16 */
        pMsgContext->resData[1] = ( uint8 )( s_PendingRidRange_u16 >> 8u ) ;
        pMsgContext->resData[2] = ( uint8 )( s_PendingRidRange_u16 & 0x00ffu ) ;

        Dcm_stDspRCState_en = DCM_RC_IDLE;
    }
    else if (dataRetVal_u8 == E_NOT_OK)
    {
        /* Copy the NRC value which has been updated by Application to the local variable */
        if ( Dcm_RCNegResCode_u8 != 0 )
        {
            *dataNegRespCode_u8 = Dcm_RCNegResCode_u8;
        }
        else
        {
            /* Conditions Not Correct is set as the Application did not fill in any NegativeResponseCode but have returned E_NOT_OK */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
        }
    }
    else if (dataRetVal_u8 == DCM_E_PENDING)
    {
        /* Nothing to be done here except for setting the OpStatus, DSL/DSD shall trigger this service again in the next main function call */
        Dcm_RCOpStatus_u8 = DCM_PENDING;

        /* As NRC code has been updated, terminate processing and send NRC to tester */
    }
    else if (dataRetVal_u8 == DCM_E_FORCE_RCRRP)
    {
        /*Do nothing NRC78 is triggered from the Statemachine*/
        Dcm_RCOpStatus_u8=DCM_FORCE_RCRRP_OK;
    }
    else if ((dataRetVal_u8 == DCM_INFRASTRUCTURE_ERROR) && (Dcm_adrRoutineRangePtr_pcst->RoutineRangeUsePort_b != FALSE))
    {
        *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
    }
    else
    {
        if ( Dcm_RCNegResCode_u8 == 0 )
        {
            /* Function returns a return value not defined within Routine Control service, but hasn't set NRC */
            *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
        }
        else
        {
            /* As NRC code has been updated, terminate processing and send NRC to tester */
            *dataNegRespCode_u8 = Dcm_RCNegResCode_u8;
        }
    }
    return dataRetVal_u8;
}
#endif


/**
 ******************************************************************************************************************
 * Dcm_DcmRoutineControl :
 *  Service Interpreter for controlling routines.
 *  This service is used for activation of routines in ECU from tester.
 *  This service has below given sub-functions.
 *  dataSubFunc_u8 0x01: 'StartRoutine'. Start the routine specified by RoutineIdentifier
 *  dataSubFunc_u8 0x02: 'StopRoutine'. Stop the routine specified by RoutineIdentifier
 *  dataSubFunc_u8 0x03: 'RequestRoutineResults'. Return the results of the routine specified by RoutineIdentifier
 *
 * \param           pMsgContext    Pointer to message structure
 *                                 (parameter in : RequestLength, Response buffer size, request bytes)
 *                                 (parameter out: Response bytes and Response length )
 *
 * \retval          None
 * \seealso
 *
 ******************************************************************************************************************
 */

Std_ReturnType Dcm_DcmRoutineControl(Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType * pMsgContext,Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    /*TRACE[SWS_Dcm_00257]*/
    /*TRACE[SWS_Dcm_01139]*/
    /*TRACE[BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3288]*/
    uint16 dataRId_u16;
    /* Loop variable to loop through all configured routines */
    uint16 idxLoop_qu16;
    Std_ReturnType dataRetVal_u8;
    boolean flgProcessReq_b;
    *dataNegRespCode_u8 = 0;

    /*Initialize the flag for Process request flag is to TRUE*/
    flgProcessReq_b =TRUE;
    /*Initialize the index variable to default value */
    idxLoop_qu16 = 0xFFFF;
    dataRetVal_u8=DCM_E_PENDING;

    /* If OpStatus is set to DCM_CANCEL then call the ini function for resetting */
    if (OpStatus == DCM_CANCEL)
    {
        /* Call the Ini Function */
        Dcm_Dsp_RC_Ini();
        /* Set the return value to E_OK as the Ini function will always be serviced  */
        dataRetVal_u8 = E_OK;
    }
    else
    {
        if ( Dcm_stDspRCState_en == DCM_RC_IDLE )
        {
            /* Routine Control Service is IDLE i.e. this is the first call of this service for this request */
            /*TRACE[SWS_Dcm_01140]*/
            if ( pMsgContext->reqDataLen >= DSP_RC_MINREQLEN )
            {
                /* At least the sub-function and two bytes of dataRId_u16 should have been received to validate further */
                /* uint32 variable for storing RID in a single variable */
                dataRId_u16 = (uint16)((uint16)pMsgContext->reqData[1] <<(uint8)8);
                dataRId_u16 = (uint16)(dataRId_u16 | pMsgContext->reqData[2]);

                /* Call the API to check if the RID is supported in ranges or single routines */
                flgProcessReq_b = Dcm_Prv_RcIsRIDSupported(dataRId_u16,&idxLoop_qu16,dataNegRespCode_u8);

                /*If Process request status is set to TRUE and negative response code is set 0x00 */
                if((flgProcessReq_b) && (*dataNegRespCode_u8 ==0x00u))
                {
                    /* If dataRId_u16 is valid, then process the Routine checks */
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
                    /* Check whether the supported RID is present in a Range or it is a single Routine */
                    if(s_IsRidRange_b)
                    {
                        /* Call the function to process the Range Routine */
                        dataRetVal_u8 = Dcm_Prv_RcProcessRangeRoutine(pMsgContext,dataNegRespCode_u8,dataRId_u16);
                    }
                    else
#endif
                    {
#if (DCM_CFG_RC_NUMRIDS > 0u)
                        /* Call the function to process the Routine */
                        dataRetVal_u8 = Dcm_Prv_RcProcessRoutine(pMsgContext,dataNegRespCode_u8,dataRId_u16);
#endif
                    }
                }
                else
                {
                    /* Requested RID is not supported (NRC0x31 has been set). Or Routine status was Stop Pending */
                }
            }
            else
            {
                /* Invalid message length */
                *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            }
        }
        if (Dcm_stDspRCState_en == DCM_RC_PENDING)
        {
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
            /* Check whether the supported RID is present in a Range or it is a single Routine */
            if(s_IsRidRange_b)
            {
                /* Call the function to process the pending Range Routine */
                dataRetVal_u8 = Dcm_Prv_RcPendingRangeRoutine(pMsgContext, dataNegRespCode_u8);
            }
            else
#endif
            {
#if (DCM_CFG_RC_NUMRIDS > 0u)
                /* Call the function to process the pending Routine */
                dataRetVal_u8 = Dcm_Prv_RcPendingRoutine(pMsgContext, dataNegRespCode_u8);
#endif
            }
        }

        if (*dataNegRespCode_u8 != 0 )
        {
            /* dataNegRespCode_u8 has been set, inform DSL-DSD of Negative Response and end the service processing */
            Dcm_stDspRCState_en = DCM_RC_IDLE;
            dataRetVal_u8=E_NOT_OK;
        }
    }
    return dataRetVal_u8;
}

/*
 ******************************************************************************************************************
 * Dcm_DcmcallStopRoutine :
 * This API stops all the routines in case there is a session transition.
 * The API is called by Dcm_RoutineSetSesCtrlType() function.
 *
 * \param           idxLoop_qu16    index of the Routine that has to be stopped
 *
 * \retval          None
 *
 * \seealso
 *
 ******************************************************************************************************************
 */
#if (DCM_CFG_RC_NUMRIDS > 0u)
#if(DCM_CFG_STOPSUPPORTED != DCM_CFG_OFF)
static void Dcm_DcmcallStopRoutine(uint16 idxLoop_qu16)
{
    Std_ReturnType dataRetType_u8;
    /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
     * Hence tehre is no data inconsistency involved and the lock is not necessary. */
    /*Set the s_IsRCSubfunctionCalled flag to TRUE*/
    s_IsRCSubfunctionCalled_b = TRUE;
    dataRetType_u8=(*s_adrRoutine_pcst->adrCallRoutine_pfct)(DCM_RC_STOPROUTINE);
    /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
     * Hence tehre is no data inconsistency involved and the lock is not necessary. */
    s_IsRCSubfunctionCalled_b = FALSE;

    /*If the return value is E_PENDING ,set the status of the routine to  DCM_ROUTINE_STOP_PENDING. that in the next
     * service call for the same RID the stop routine will be called with Opstatus set to DCM_CANCEL */
    if(dataRetType_u8 == DCM_E_PENDING)
    {
        /* StopRoutine returned DCM_E_PENDING. Set the routine status to DCM_ROUTINE_STOP_PENDING */
        Dcm_RoutineStatus_aen[idxLoop_qu16]  = DCM_ROUTINE_STOP_PENDING;
    }
    else
    {
        /* StopRoutine returned return value other than DCM_E_PENDING. Set the routine status to DCM_ROUTINE_STOP */
        Dcm_RoutineStatus_aen[idxLoop_qu16]  = DCM_ROUTINE_STOP;
    }
}
#endif
#endif

/*
 ******************************************************************************************************************
 * Dcm_DcmCallStopRoutineRange :
 * This API stops all the routine ranges in case there is a session transition.
 * The API is called by Dcm_RoutineSetSesCtrlType() function.
 *
 * \param           idxLoop_qu16    index of the Routine that has to be stopped
 *
 * \retval          None
 *
 * \seealso
 *
 ******************************************************************************************************************
 */
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
#if(DCM_CFG_RCRANGE_STOPSUPPORTED != DCM_CFG_OFF)
static void Dcm_DcmCallStopRoutineRange(uint16 idxLoop_qu16)
{
    Std_ReturnType dataRetType_u8;
    /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
     * Hence tehre is no data inconsistency involved and the lock is not necessary. */
    /*Set the s_IsRCSubfunctionCalled flag to TRUE*/
    s_IsRCSubfunctionCalled_b = TRUE;
    /* Check whether the supported RID is present in a Range or it is a single Routine */
    dataRetType_u8=(*s_adrRoutineRange_pcst->adrCallRoutine_pfct)(DCM_RC_STOPROUTINE);
    /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
     * Hence tehre is no data inconsistency involved and the lock is not necessary. */
    s_IsRCSubfunctionCalled_b = FALSE;

    /*If the return value is E_PENDING ,set the status of the routine to  DCM_ROUTINE_STOP_PENDING. that in the next
     * service call for the same RID the stop routine will be called with Opstatus set to DCM_CANCEL */
    if(dataRetType_u8 == DCM_E_PENDING)
    {
        /* StopRoutine returned DCM_E_PENDING. Set the routine status to DCM_ROUTINE_STOP_PENDING */
        Dcm_RoutineRangeStatus_aen[idxLoop_qu16]  = DCM_ROUTINE_STOP_PENDING;
    }
    else
    {
        /* StopRoutine returned return value other than DCM_E_PENDING. Set the routine status to DCM_ROUTINE_STOP */
        Dcm_RoutineRangeStatus_aen[idxLoop_qu16]  = DCM_ROUTINE_STOP;
    }
}
#endif
#endif

/*
 ******************************************************************************************************************
 * Dcm_RoutineSetSesCtrlType :
 * This API stops all the routines in case there is a session transition.
 * The API is called by Dcm_SetSesCtrlType() function.
 *
 * \param           dataSesCtrlType_u8    New session control type value
 *
 * \retval          None
 *
 * \seealso
 *
 ******************************************************************************************************************
 */

void Dcm_RoutineSetSesCtrlType (Dcm_SesCtrlType dataSesCtrlType_u8)
{
    /* Index Variable for loop */
    uint16 idxLoop_qu16;
    uint32 Sessionmask_u32;
    uint32 Securitymask_u32;
#if (DCM_CFG_RC_NUMRIDS > 0u)
#if(DCM_CFG_STOPSUPPORTED != DCM_CFG_OFF)



    /* Search through the complete list of Routines */
    for ( idxLoop_qu16 = 0 ; idxLoop_qu16 < DCM_CFG_RC_NUMRIDS ; idxLoop_qu16++ )
    {
        s_adrRoutine_pcst =  &Dcm_DspRoutine_cast[idxLoop_qu16];

        /* Check whether the flgStopRoutineOnSessionChange_b is supported for this RID and the routine is already started
         * The condition will never be satisfied for Routines which does not support Start sub function or Supports  Start start subfunction but sequence error is disabled as
         * the state of thr Routine will be DCM_ROUTINE_STARTED only if a Start request is received */
        if ( (s_adrRoutine_pcst->flgStopRoutine_b!= FALSE)&&(s_adrRoutine_pcst->flgStopRoutineOnSessionChange_b != FALSE) && (Dcm_RoutineStatus_aen[idxLoop_qu16] == DCM_ROUTINE_STARTED))
        {
            if(dataSesCtrlType_u8 == Dcm_DsldSessionTable_pcu8[DCM_DEFAULT_SESSION_IDX])
            {
                Dcm_DcmcallStopRoutine(idxLoop_qu16);
            }
            else
            {
                Sessionmask_u32=  (Dcm_DsldGetActiveSessionMask_u32() & ( s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataSessBitMask_u32 ));
                Securitymask_u32= (Dcm_DsldGetActiveSecurityMask_u32() & ( s_adrRoutine_pcst->adrRoutineInfoRef_cpcst->dataSecBitMask_u32 ));
                /* Check whether the RID is supported in the current session and security */
                if ((Sessionmask_u32 == 0u) || ( Securitymask_u32 == 0u ))
                {
                    /* Call stop routine for the RID*/
                    Dcm_DcmcallStopRoutine(idxLoop_qu16);
                }
            }
        }

   }
#endif
#endif

#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
#if(DCM_CFG_RCRANGE_STOPSUPPORTED != DCM_CFG_OFF)



    /* Search through the complete list of Range Routines */
    for (idxLoop_qu16 = 0; idxLoop_qu16 < DCM_CFG_RC_RANGE_NUMRIDS; idxLoop_qu16++)
    {
        s_adrRoutineRange_pcst = &Dcm_DspRoutineRange_cast[idxLoop_qu16];

        /* Check whether the flgStopRoutineOnSessionChange_b is supported for this RID and the routine is already started
         * The condition will never be satisfied for Routines which does not support Start sub function or Supports  Start start subfunction but sequence error is disabled as
         * the state of thr Routine will be DCM_ROUTINE_STARTED only if a Start request is received */
        if((s_adrRoutineRange_pcst->flgStopRoutine_b!= FALSE)&&(s_adrRoutineRange_pcst->flgStopRoutineOnSessionChange_b != FALSE) && (Dcm_RoutineRangeStatus_aen[idxLoop_qu16] == DCM_ROUTINE_STARTED))
        {
            if(dataSesCtrlType_u8 == Dcm_DsldSessionTable_pcu8[DCM_DEFAULT_SESSION_IDX])
            {
                Dcm_DcmCallStopRoutineRange(idxLoop_qu16);
            }
            else
            {
                Sessionmask_u32=(Dcm_DsldGetActiveSessionMask_u32() & (s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataSessBitMask_u32));
                Securitymask_u32=(Dcm_DsldGetActiveSecurityMask_u32() & (s_adrRoutineRange_pcst->adrRoutineInfoRef_cpcst->dataSecBitMask_u32));
                /* Check whether the RID is supported in the current session and security */
                if(( Sessionmask_u32 == 0u)|| (Securitymask_u32 == 0u))
                {
                    /* Call stop routine for the RID ranges*/
                    Dcm_DcmCallStopRoutineRange(idxLoop_qu16);
                }
            }
        }

    }
#endif
#endif

}

/*
 *************************************************************************************************************************************************************
 * Dcm_SetFlagforRC:
 *
 * This API sets/resets the flag which is required for getting the active RID based on the input parameter
 *
 *
 * \param     boolean isFlag_b : parameter for setting or resetting the flag to get the active RID
 * \retval    None
 * \seealso
 *
 **************************************************************************************************************************************************************
 */
static void Dcm_SetFlagforRC(boolean isFlag_b)
 {
    /* Setting/resetting the flag so that the application can call GetActiveRID function from DcmAppl_DcmConfirmation */
    s_IsRCSubfunctionCalled_b = isFlag_b;
}
/*
 *******************************************************************************
 * Dcm_Prv_DspRCConfirmation : API used for confirmation of response sent for
 *                                      RoutineControl (0x31) service.
 * \param           dataIdContext_u8    Service Id
 * \param           dataRxPduId_u8      PDU Id on which request is Received
 * \param           dataSourceAddress_u16    Tester Source address id
 * \param           status_u8                Status of Tx confirmation function
 *
 * \retval          None
 * \seealso
 *
 *******************************************************************************
 */
 void Dcm_Prv_DspRCConfirmation(
    Dcm_IdContextType dataIdContext_u8,
    PduIdType dataRxPduId_u8,
    uint16 dataSourceAddress_u16,
    Dcm_ConfirmationStatusType status_u8)
{
     if(dataIdContext_u8 == DCM_DSP_SID_ROUTINECONTROL)
     {
         Dcm_SetFlagforRC(TRUE);
     }
     DcmAppl_DcmConfirmation(dataIdContext_u8,dataRxPduId_u8,dataSourceAddress_u16,status_u8);
     if(dataIdContext_u8 == DCM_DSP_SID_ROUTINECONTROL)
     {
         Dcm_SetFlagforRC(FALSE);
     }
}

/**
 ******************************************************************************************************************
 * Dcm_GetActiveRid :
 * API to provide the current active RID.
 * This API has to be called from the application callbacks of Start, Stop and Req.Results routine configured for the project and also from the DcmAppl_DcmConfirmation.
 * Example scenarios:
 *  a) This API has to be called by the application
 *      a.1)during Start, Stop and Request Results operations for the normal tester request
 *      a.2)during canceling the pending operations either Start or Stop or Request Results due to CANCEL operation triggered by Dcm by setting the Opstatus to DCM_CANCEL in the RC Ini function.
 *      a.3)during the stop of the routines due to session transitions (either due to protocol start/stop or session time out/change).
 * By "Active RID", it means that the active RID index under processing and not the RIDs of already started routines.
 *
 * \param           dataRid_u16    Buffer to get the current active RID

 *
 * \retval          Std_ReturnType  E_OK : The parameter dataRid_u16 contains valid RID value in this case.
 *                                  E_NOT_OK : The parameter dataRid_u16 contains invalid data in case none of the Routine is under processing.
 * \seealso
 *
 ******************************************************************************************************************
 */
Std_ReturnType Dcm_GetActiveRid(uint16 * dataRid_u16)
{
    Std_ReturnType RetVal;
    /* Multicore: There is a constraint on the callback that GetActiveRid can only be called from Start, Stop and Request results subfunction.
     * Hence tehre is no data inconsistency involved and the lock is not necessary. */
     /*Check if the s_IsRCSubfunctionCalled flag is set to TRUE and current active id is valid*/
    if ((s_IsRCSubfunctionCalled_b != FALSE) && (dataRid_u16!= NULL_PTR))
    {
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
        if(s_IsRidRange_b)
        {

            if (Dcm_stDspRCState_en == DCM_RC_PENDING)
            {
                *dataRid_u16 = s_PendingRidRange_u16;
            }
            else
            {
                *dataRid_u16 = s_dataRidRange_u16;
            }
        }
        else
#endif
        {
#if (DCM_CFG_RC_NUMRIDS > 0u)
            if (Dcm_stDspRCState_en == DCM_RC_PENDING)
            {
                *dataRid_u16 = Dcm_adrRoutinePtr_pcst->dataRId_u16;
            }
            else
            {
                *dataRid_u16 = s_adrRoutine_pcst->dataRId_u16 ;
            }
#endif
        }
        RetVal = E_OK;
    }
    else
    {
        RetVal = E_NOT_OK;
    }
    return RetVal;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif      /* DCM_CFG_DSP_ROUTINECONTROL_ENABLED */
#endif
