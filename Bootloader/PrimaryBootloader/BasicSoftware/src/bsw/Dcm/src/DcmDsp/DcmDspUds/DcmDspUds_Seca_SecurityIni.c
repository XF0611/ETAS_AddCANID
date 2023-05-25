
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Seca_Inf.h"
#include "Rte_Dcm.h"
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Seca_Prv.h"


#if(DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* Array for all the security levels in the SECA configuration */
Dcm_Dsp_SecaType Dcm_Dsp_Seca[DCM_CFG_DSP_NUMSECURITY];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
boolean Dcm_DspChgSecLevel_b;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_OpStatusType Dcm_DspSecaOpStatus_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if(DCM_CFG_DSP_SECA_USEFLEXIBLELENGTH != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name  : Dcm_Prv_SecaIniForFlexibleLen
 Description    : Cancellation of application function for flexible length
***********************************************************************************************************************/
static void Dcm_Prv_SecaIniForFlexibleLen(const Dcm_Dsp_Security_t* ptrSecurityConfig,
                                          Dcm_NegativeResponseCodeType dataNegRespCode_u8)
{
#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
    Dcm_SecLevelType   dataSecLevel_u8;
#endif
    if(Dcm_SrvOpstatus_u8 == DCM_CHECKDATA)
    {
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
        if(ptrSecurityConfig->usePort == USE_ASYNCH_CLIENT_SERVER)
        {
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
            /* Is Asynchronous server call point handling is requested */
            if(ptrSecurityConfig->UseAsynchronousServerCallPoint_b)
            {
                /* Call the Rte_Call API with In and InOut parameters to cancel the ongoing operation */
                if(ptrSecurityConfig->AccDataRecsize_u32!=0x00u)
                   {
                       (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr10))(0x00u,NULL_PTR,DCM_CANCEL);
                   }
                   else
                   {
                       (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr11))(0x00u,DCM_CANCEL);
                   }
            }
            else
#endif
            {
                if(ptrSecurityConfig->AccDataRecsize_u32!=0x00u)
                {
                    (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr1))(NULL_PTR,NULL_PTR,DCM_CANCEL,NULL_PTR,&dataNegRespCode_u8);
                }
                else
                {
                    (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr9))(NULL_PTR,DCM_CANCEL,NULL_PTR,&dataNegRespCode_u8);
                }
            }
            (void)dataNegRespCode_u8;
        }
#endif

#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
        if(ptrSecurityConfig->usePort == USE_ASYNCH_FNC)
        {
            /* Calculate the security level */
            dataSecLevel_u8  = ((Dcm_DspSecAccType_u8+1u)>>1u);
            (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr8))(
                                        dataSecLevel_u8,
                                        NULL_PTR,
                                        ptrSecurityConfig->AccDataRecsize_u32,
                                        NULL_PTR,NULL_PTR,DCM_CANCEL,&dataNegRespCode_u8);
            (void)dataNegRespCode_u8;
        }
#endif
    }
    if((Dcm_SrvOpstatus_u8 == DCM_PROCESSSERVICE) && (FALSE != Dcm_Prv_CompareKeyActive()))
    {
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
        if(ptrSecurityConfig->usePort == USE_ASYNCH_CLIENT_SERVER)
        {
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
            /* Is ASynchronous server call point handling is requested */
            if(ptrSecurityConfig->UseAsynchronousServerCallPoint_b)
            {
                (void)(*(ptrSecurityConfig->Dsp_CompareKey_fp.Dcm_CompareKey_ptr6))(0x00u,NULL_PTR, DCM_CANCEL);
            }
            else
#endif
            {
                (void)(*(ptrSecurityConfig->Dsp_CompareKey_fp.Dcm_CompareKey_ptr3))(0x00u,NULL_PTR, DCM_CANCEL,&dataNegRespCode_u8);
            }
        }
#endif
#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
        if(ptrSecurityConfig->usePort == USE_ASYNCH_FNC)
        {
            (void)(*(ptrSecurityConfig->Dsp_CompareKey_fp.Dcm_CompareKey_ptr3))(ptrSecurityConfig->Key_size_u32,NULL_PTR,
            DCM_CANCEL,&dataNegRespCode_u8);
        }
#endif
    }
}
#endif

/**
 ***********************************************************************************************************************
 * Dcm_Dsp_SecaIni : Initialization function for SECA Service
 *
 * \param           None
 *
 * \retval          None
 ***********************************************************************************************************************
*/
void Dcm_Dsp_SecaIni (void)
{

#if((DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF) || (DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF))
#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
    Dcm_SecLevelType dataSecLevel_u8;     /* Local variable to store Security level */
#endif
    const Dcm_Dsp_Security_t * ptrSecurityConfig;
    Dcm_NegativeResponseCodeType dataNegRespCode_u8; /* Local variable to store the NRC */
#endif


#if((DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)|| (DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF))
    dataNegRespCode_u8 = 0x0; /* Initialise the local variable */
#endif

    /* If there is a PENDING operation */
    if(Dcm_DspSecaOpStatus_u8 == DCM_PENDING)
    {   /* Pending Get Seed operation ?*/
#if((DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF) || (DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF))
        ptrSecurityConfig =&Dcm_Dsp_Security[Dcm_DspSecTabIdx_u8];
#endif
#if(DCM_CFG_DSP_SECA_USEFLEXIBLELENGTH != DCM_CFG_OFF)
        if(ptrSecurityConfig->UseFlexibleLength)
        {
            Dcm_Prv_SecaIniForFlexibleLen(ptrSecurityConfig,dataNegRespCode_u8);
        }
        else
#endif
        {
            if(Dcm_SrvOpstatus_u8 == DCM_CHECKDATA)
            {
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
                if(ptrSecurityConfig->usePort == USE_ASYNCH_CLIENT_SERVER)
                {
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
                    /* Is Asynchronous server call point handling is requested */
                    if(ptrSecurityConfig->UseAsynchronousServerCallPoint_b)
                    {
                        /* Call the Rte_Call API with In and InOut parameters to cancel the ongoing operation */
                        if(ptrSecurityConfig->AccDataRecsize_u32!=0x00u)
                           {
                               (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr5))(NULL_PTR,DCM_CANCEL);
                           }
                           else
                           {
                               (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr6))(DCM_CANCEL);
                           }
                    }
                    else
#endif
                    {
                        if(ptrSecurityConfig->AccDataRecsize_u32!=0x00u)
                        {
                            (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr))(NULL_PTR,DCM_CANCEL,NULL_PTR,&dataNegRespCode_u8);
                        }
                        else
                        {
                            (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr3))(DCM_CANCEL,NULL_PTR,&dataNegRespCode_u8);
                        }
                    }
                    (void)dataNegRespCode_u8;
                }
#endif

#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
                if(ptrSecurityConfig->usePort == USE_ASYNCH_FNC)
                {
                    /* Calculate the security level */
                    dataSecLevel_u8  = ((Dcm_DspSecAccType_u8+1u)>>1u);
                    (void)(*(ptrSecurityConfig->Dsp_GetSeed_fp.Dcm_GetSeed_ptr4))(
                                                dataSecLevel_u8,
                                                ptrSecurityConfig->Seed_size_u32,
                                                ptrSecurityConfig->AccDataRecsize_u32,
                                                NULL_PTR,NULL_PTR,DCM_CANCEL,&dataNegRespCode_u8);
                    (void)dataNegRespCode_u8;
                }
#endif
            }
            if(Dcm_SrvOpstatus_u8 == DCM_PROCESSSERVICE)
            {
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
                if(ptrSecurityConfig->usePort == USE_ASYNCH_CLIENT_SERVER)
                {
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
                    /* Is ASynchronous server call point handling is requested */
                    if(ptrSecurityConfig->UseAsynchronousServerCallPoint_b)
                    {
                        (void)(*(ptrSecurityConfig->Dsp_CompareKey_fp.Dcm_CompareKey_ptr4))(NULL_PTR, DCM_CANCEL);
                    }
                    else
#endif
                    {
                        (void)(*(ptrSecurityConfig->Dsp_CompareKey_fp.Dcm_CompareKey_ptr1))(NULL_PTR, DCM_CANCEL,&dataNegRespCode_u8);
                    }
                }
#endif
#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
                if(ptrSecurityConfig->usePort == USE_ASYNCH_FNC)
                {
                    (void)(*(ptrSecurityConfig->Dsp_CompareKey_fp.Dcm_CompareKey_ptr3))(ptrSecurityConfig->Key_size_u32,NULL_PTR,
                    DCM_CANCEL,&dataNegRespCode_u8);
                }
#endif
            }
        }
    }

    /* Reset the flags defined for Asynchonous Server point handling */
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
    Dcm_ResetAsynchSecaFlag();
#endif
    /* Reset the Seca Opstatus */
    Dcm_DspSecaOpStatus_u8 = DCM_INITIAL;
    // Reset Compare Key StateMachine
    Dcm_Prv_ResetCKStateMachine();
    /* Re-Initialisation of Security Access State and Global parameters */
    Dcm_Dsp_SecaSessIni();
}

/**
 **************************************************************************************************
 * Dcm_Dsp_SecaPowerOnDelayIni : Initialization of PowerOn delay timer for all security levels configured
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
*/
void Dcm_Dsp_SecaPowerOnDelayIni (void)
{
    uint8_least idxSecTab_qu8;  /* Local variable to iterate through the security table */
    uint32 dataDelayOnPowerOn_u32;

    for (idxSecTab_qu8=0;idxSecTab_qu8 < DCM_CFG_DSP_NUMSECURITY;idxSecTab_qu8++)
    {
        if(Dcm_Dsp_Seca[idxSecTab_qu8].FailedAtm_cnt_u8 >= Dcm_Dsp_Security[idxSecTab_qu8].NumAttDelay_u8)
        {
            dataDelayOnPowerOn_u32 =
            (Dcm_Dsp_Security[idxSecTab_qu8].DelayTime_u32 > Dcm_Dsp_Security[idxSecTab_qu8].PowerOnDelay_u32) ?
            Dcm_Dsp_Security[idxSecTab_qu8].DelayTime_u32 : Dcm_Dsp_Security[idxSecTab_qu8].PowerOnDelay_u32;
            if(dataDelayOnPowerOn_u32 > 0x00u)
            {
                dataDelayOnPowerOn_u32 = dataDelayOnPowerOn_u32 * DCM_CFG_TASK_TIME_MS;
                /* DcmAppl_DcmGetUpdatedDelayTime should return Residual Delay time in Counts of DcmTaskTime */
                dataDelayOnPowerOn_u32 =
                DcmAppl_DcmGetUpdatedDelayTime(Dcm_Dsp_Security[idxSecTab_qu8].Security_level_u8,
                Dcm_Dsp_Seca[idxSecTab_qu8].FailedAtm_cnt_u8,dataDelayOnPowerOn_u32);
                Dcm_Dsp_Seca[idxSecTab_qu8].Residual_delay_u32 =
                (Dcm_Dsp_SecaGlobaltimer_u32 + (dataDelayOnPowerOn_u32 / DCM_CFG_TASK_TIME_MS));
            }
            else
            {
            /*if both PowerOnDelay_u32 and DelayTime_u32 are initialized with 0 then assign Residual_delay_u32 with 0*/
                Dcm_Dsp_Seca[idxSecTab_qu8].Residual_delay_u32 = 0x00u;
            }
        }
        else
        {
            if(Dcm_Dsp_Security[idxSecTab_qu8].PowerOnDelay_u32 > 0x00u)
            {
                dataDelayOnPowerOn_u32 =
                DcmAppl_DcmGetUpdatedDelayForPowerOn(Dcm_Dsp_Security[idxSecTab_qu8].Security_level_u8,
                Dcm_Dsp_Seca[idxSecTab_qu8].FailedAtm_cnt_u8,
                (Dcm_Dsp_Security[idxSecTab_qu8].PowerOnDelay_u32 * DCM_CFG_TASK_TIME_MS));
                Dcm_Dsp_Seca[idxSecTab_qu8].Residual_delay_u32 = (dataDelayOnPowerOn_u32/DCM_CFG_TASK_TIME_MS);
            }
            else
            {
                Dcm_Dsp_Seca[idxSecTab_qu8].Residual_delay_u32 = 0x00u;
            }
        }
    }
    return;
}
/**
 ***********************************************************************************************************************
 * Dcm_Dsp_SecaSesIni : Re-Initialization of all security parameters due to session timeout or change in session.
 *
 * \param           None
 *
 * \retval          None
 ***********************************************************************************************************************
*/
void Dcm_Dsp_SecaSessIni (void)
{
    /* Initialise SECA state machine */
    Dcm_SrvOpstatus_u8 = DCM_INITIAL;
    //Dcm_DspSecaStatus_en = DCM_DSP_SECA_REQUEST;
    /* Initialise change security level */
    Dcm_DspChgSecLevel_b = FALSE;
    /* Resetting Stored AccessType */
    Dcm_DspSecAccType_u8 = 0x0;
    /* Resetting Security Index */
    Dcm_DspSecTabIdx_u8  = 0x0;

}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#endif
