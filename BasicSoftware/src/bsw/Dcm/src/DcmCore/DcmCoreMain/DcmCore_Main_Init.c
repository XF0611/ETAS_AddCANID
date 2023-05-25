
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_Main_Inf.h"
#include "Rte_Dcm.h"
#include  "DcmDspUds_Seca_Prv.h"
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if(DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
/*Address of any element of Dcm_ConfigPtr can be passed to Dcm_Init as Dcm_Init(&Dcm_ConfigPtr[x]). [where 'x' can be any of 0-7] */
const Dcm_ConfigType Dcm_Config[DCM_PRV_MAXNUM_OF_CONFIG]={{DCM_CFG_CONFIGSET1},{DCM_CFG_CONFIGSET2},{DCM_CFG_CONFIGSET3},{DCM_CFG_CONFIGSET4},
                                      {DCM_CFG_CONFIGSET5},{DCM_CFG_CONFIGSET6},{DCM_CFG_CONFIGSET7},{DCM_CFG_CONFIGSET8}};

#endif

#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if((DCM_CFG_DSPOBDSUPPORT_ENABLED!= DCM_CFG_OFF)&&(DCM_CFG_DSP_OBDMODE9_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_INFOTYPE_SUPPORT != DCM_CFG_OFF))

#define DCM_START_SEC_VAR_INIT_UNSPECIFIED  /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

boolean Dcm_SupportInfotype_b;
Dcm_Dsp_Mode9BitMask_t Dcm_Dsp_Mode9Bitmask_acs[8];

#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**
 **************************************************************************************************
 * Dcm_Prv_RestoreDddId :Restores all DDDID's from NvM if Restore Configuration Parameter is set to
 *                      TRUE.
 * \param[in]      None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if (DCM_CFG_DSP_DDDISTORINGTONVRAM_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_RestoreDddId(void)
{
    Dcm_RestoreDddiReturn_ten dataRetValFromNvMStorage_u8;  /* Local variable to store the return value from the NvM storage/Clear */
    uint32 nrNumOfDddIds;  /* Local variable to store the return value from the NvM storage/Clear */
    uint32 idxLoop_u32; /* Local variable to loop through the DDDIs configured */

    nrNumOfDddIds = Dcm_DddiCalculateTableSize_u16();

    /* restoring the DDDIDs from NvM */
    /* restore the DDDIDs */
    for(idxLoop_u32 = 0; idxLoop_u32<nrNumOfDddIds; idxLoop_u32++)
    {
        dataRetValFromNvMStorage_u8 = DcmAppl_DcmReadDddiFromNvM(&Dcm_DDDIBaseConfig_cast[idxLoop_u32],
                Dcm_DDDIBaseConfig_cast[idxLoop_u32].dataDddId_u16);
        if(dataRetValFromNvMStorage_u8 == DCM_DDDI_READ_NOT_OK)
        {
            /* reporting the DET error as the read is not successful */
            DCM_DET_ERROR(DCM_DDDI_ID , DCM_E_NO_READ_ACCESS )
        }
    }
}
#endif

/**
 **************************************************************************************************
 * Dcm_Prv_ROEInit : Initializes ROE service and call initialization of all ROE service table;
 *                  All events are made to ROE cleared.
 * \param[in]      None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_ROEInit(void)
{
    uint8_least idxIndex_qu8;
    const Dcm_Dsld_protocol_tableType * adrProtocol_pcst;

    /* Get the pointer to protocol array */
    adrProtocol_pcst = Dcm_DsldProtocol_pcst;

    /* Call the service ini of all ROE service table */
    for(idxIndex_qu8= DCM_CFG_NUM_PROTOCOL; idxIndex_qu8!=0x00u; idxIndex_qu8--)
    {

        if(adrProtocol_pcst->roe_info_pcs != NULL_PTR)
        {
            /* Call initialisation of all services */
            Dcm_Dsd_ServiceIni(adrProtocol_pcst->roe_info_pcs->servicetable_Id_u8);
        }

        /* Go to next protocol */
        adrProtocol_pcst++;
    }


#if(DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF)
#if(DCM_CFG_DSP_ROEONDTCSTATUSCHANGE_ENABLED != DCM_CFG_OFF)
    Dcm_RestoreROEDtcEvents();
#endif
#if(DCM_CFG_DSP_ROEDID_ENABLED!=DCM_CFG_OFF)
    Dcm_RestoreROEDidEvents();
#endif
#endif


#if(DCM_CFG_ROETYPE2_ENABLED != DCM_CFG_OFF)
    Dcm_DsdRoe2State_en = DSD_IDLE;
#endif
}
#endif

/**
 **************************************************************************************************
 * Dcm_Prv_RdpiInit : Initializes RDPI service; function to initialize periodic scheduler is called
 * \param[in]      None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if (DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_RdpiInit(void)
{
    const Dcm_Dsld_protocol_tableType * adrProtocol_pcst;

    /* Get the pointer to protocol array */
    adrProtocol_pcst = Dcm_DsldProtocol_pcst;

    if(adrProtocol_pcst->rdpi_info_pcs != NULL_PTR)
    {
        Dcm_DsldPeriodicSchedulerIni();
    }

}
#endif

/**
 **************************************************************************************************
 * Dcm_Prv_WarmStart : Calls Dcm_GetProgConditions to restore all the information for Warmstart;
 *                     checks ComM_Dcm_ActiveDiagnostic needs to be called or not id it is Warmrequest
 *                     or Warmresponse
 * \param[in]      None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if (DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_WarmStart(void)
{
    Dcm_EcuStartModeType dataRetGetProgCond_u8;
    /* TRACE: [SWS_Dcm_00536] */
    /* TRACE: [SWS_Dcm_00544] */
    /* Call the API to restore the information for WARM start */
    dataRetGetProgCond_u8 = Dcm_GetProgConditions(&Dcm_ProgConditions_st);
    /* Check for the WARM START  */
    if(dataRetGetProgCond_u8 == DCM_WARM_START)
    {
        /*Initialize the variable to the value TRUE*/
        Dcm_ReadyForBoot_b = TRUE;

        if (( Dcm_ProgConditions_st.StoreType == DCM_WARMREQUEST_TYPE ) || ( Dcm_ProgConditions_st.StoreType == DCM_WARMRESPONSE_TYPE ))
        {
            /* TRACE: [SWS_Dcm_00537] */
            /* Api to check whether ComM_Dcm_ActiveDiagnostic needs to be called or not*/
            Dcm_CheckActiveDiagnosticStatus(Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_GetActiveConnectionIdx_u8()].channel_idx_u8].ComMChannelId);
        }
    }
    else
    {
        /*Initialize the variable to the value FALSE*/
        Dcm_ReadyForBoot_b = FALSE;
        Dcm_ProgConditions_st.StoreType       = DCM_NOTVALID_TYPE;
    }

}
#endif

/**
 **************************************************************************************************
 * Dcm_Prv_SecaInit : Initialise Seca global timer to zero; Restore delay count; Clear seed;
 *                    Restore secatimer  and go to warmstart or power on delay function is
 *                    configured depending upon StoreType
 * \param[in]      None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if(DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_SecaInit(void)
{
    /* Initialize the required global variables */
#if (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
    /* Initialise Security Access Global Timer */
    Dcm_Dsp_SecaGlobaltimer_u32 = 0x0;
#endif


#if (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)
    /* Call DSP function to load the Delay count values of Security levels */
    Dcm_Dsp_RestoreDelayCount();
#if (DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF)
    Dcm_Dsp_SecaClearSeed();
#endif
#endif
#endif

#if(DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
    /* Restore the SECA timer for loading the SECA timers */
    Dcm_DslDsdRestoreSecaTimer();
#endif
}
#endif

/**
 **************************************************************************************************
 * Dcm_Prv_IOCBIIinit : Initialize the DcmDsp_IocbiStatus_array by storing their index from the
 *                      DID_Config structure
 * \param[in]      None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF) && (DCM_CFG_NUM_IOCBI_DIDS != 0x00u))
static void Dcm_Prv_IOCBIIinit(void)
{
    uint16 idxDid_u16;
    uint16 idxSig_u16;
    uint16 cntrIocbiDid_u16 = 0;
    uint16 DIDTablesize;

    DIDTablesize = Dcm_DIDcalculateTableSize_u16();
    /*Initialize the IOCBI DcmDsp_IocbiStatus_array */
    /*Loop through the complete DID structure to determine the index of all the IOCBI dids*/
    for(idxDid_u16=0; idxDid_u16<DIDTablesize; idxDid_u16++)
    {
        for(idxSig_u16=0; idxSig_u16<Dcm_DIDConfig[idxDid_u16].nrSig_u16; idxSig_u16++)
        {
            /*  Checking if the DID is meant for IO Control by checking if the session configured is not 0x00 */
            if((Dcm_DIDConfig[idxDid_u16].adrExtendedConfig_pcst->dataSessBitMask_u32!=0x00u) && (Dcm_DspDid_ControlInfo_st[Dcm_DspDataInfo_st[Dcm_DIDConfig[idxDid_u16].adrDidSignalConfig_pcst[idxSig_u16].idxDcmDspDatainfo_u16].idxDcmDspControlInfo_u16].idxDcmDspIocbiInfo_u16>0))
            {
                break;
            }
        }
        if(idxSig_u16<Dcm_DIDConfig[idxDid_u16].nrSig_u16)
        {
            /*Store the index of IOCBI did from Dcm_DIDConfig structure*/
            DcmDsp_IocbiStatus_array[cntrIocbiDid_u16].idxindex_u16 = idxDid_u16;
            DcmDsp_IocbiStatus_array[cntrIocbiDid_u16].IocbiStatus_en = DCM_IOCBI_IDLESTATE;
            cntrIocbiDid_u16++;
#if( DCM_CFG_DSP_IOCBI_SR_ENABLED != DCM_CFG_OFF)
            if((TRUE == Dcm_DIDConfig[idxDid_u16].AtomicorNewSRCommunication_b) && \
               ((USE_ATOMIC_SENDER_RECEIVER_INTERFACE == Dcm_DIDConfig[idxDid_u16].didUsePort_u8) || (USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE == Dcm_DIDConfig[idxDid_u16].didUsePort_u8)))
            {
                /*TRACE[SWS_Dcm_01436]*/
                /*TRACE[SWS_Dcm_01437]*/
                (void)(*(Dcm_DIDConfig[idxDid_u16].ioControlRequest_cpv.IOControlrequest_pfct))(DCM_IOCBI_INIT,NULL_PTR,DCM_VALUE_NULL,DCM_VALUE_NULL,DCM_INITIAL,NULL_PTR);
            }
#endif
        }
    }
    /*By the end of this for loop it is expected that all the indexes of IOCBI DIDs are stored and status is saved.*/
}
#endif
/**
 **************************************************************************************************
 * Dcm_Prv_UDSinit : Initialising session to Default session; setting timers P2 and P2*;
 *                  setting active protocol; Security access initialization; routine control array
 *                  initialization; Response On Event initialization; RDPI initialization;
 *                  IOCBI initialization
 * \param[in]      None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
static void Dcm_Prv_UDSinit(void)
{
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF))
    uint16_least idxLoop_qu16;
#endif
    /*TRACE[SWS_Dcm_00034]*/
    /* set the session as default session */
    Dcm_DsldGlobal_st.idxActiveSession_u8 = 0x0u;
    Dcm_CC_ActiveSession_u8 =  DCM_DEFAULT_SESSION;

    /*TRACE[SWS_Dcm_00033]*/
    Dcm_DsldGlobal_st.idxActiveSecurity_u8 = DCM_SEC_LEV_LOCKED;

    /* set the default session time */
    /* Spin lock Dcm_DsldTimer_NoNest is not needed here as Dcm_Init is called in Init context */
    Dcm_DsldTimer_st.dataTimeoutP2max_u32    =  DCM_CFG_DEFAULT_P2MAX_TIME;
    Dcm_DsldTimer_st.dataTimeoutP2StrMax_u32 =  DCM_CFG_DEFAULT_P2STARMAX_TIME;
    /* Set to active diagnostics mode*/
    Dcm_ActiveDiagnosticState_en = DCM_COMM_ACTIVE;

    /* Appl function to change the reference to the rx table*/
    DcmAppl_DcmUpdateRxTable();
    /*Initialize the variable to FALSE*/
    Dcm_isInitialised_b = FALSE;
    /* Initialize to  DCM_NO_ACTIVE_PROTOCOL (0x0C) as no protocol will be active */
    Dcm_CurProtocol_u8 = DCM_NO_ACTIVE_PROTOCOL;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    Dcm_CurOBDProtocol_u8 = DCM_NO_ACTIVE_PROTOCOL;
#endif
    /* Inititalze this Flag to FALSE as no StopProtocol is called */
    Dcm_isStopProtocolInvoked_b = FALSE;
    /* Call the API to reset the flag indicating a Reset To Default Session request by application */
    Dcm_Prv_ResetDefaultSessionRequestFlag();


#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF))
#if (DCM_CFG_RC_NUMRIDS > 0u)
    /*Initializing DcmDsp_RoutineStatusArray ;ie setting the status of all routines to default value. */
    for(idxLoop_qu16 = 0;idxLoop_qu16<DCM_CFG_RC_NUMRIDS;idxLoop_qu16++)
    {
        Dcm_RoutineStatus_aen[idxLoop_qu16] = DCM_ROUTINE_IDLE;
    }
#endif
#if (DCM_CFG_RC_RANGE_NUMRIDS > 0u)
    /*Initializing DcmDsp_RoutineRangeStatus Array ;ie setting the status of all routine ranges to default value. */
    for(idxLoop_qu16 = 0;idxLoop_qu16<DCM_CFG_RC_RANGE_NUMRIDS;idxLoop_qu16++)
    {
        Dcm_RoutineRangeStatus_aen[idxLoop_qu16] = DCM_ROUTINE_IDLE;
    }
#endif
#endif

    /* Initialisation of ROE and RDPI  Part */
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_ROEInit();
#endif

#if (DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_RdpiInit();
#endif

#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF) && (DCM_CFG_NUM_IOCBI_DIDS != 0x00u))
    Dcm_Prv_IOCBIIinit();
#endif
}

/**
 **************************************************************************************************
 * Dcm_Prv_BufqueueInit : Initialising Buffer index to point to first buffer and Setting the Queue
 *                       state to IDLE state.
 * \param[in]      None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if(DCM_BUFQUEUE_ENABLED != DCM_CFG_OFF)
static void Dcm_Prv_BufqueueInit(void)
{

    Dcm_DsldGlobal_st.adrBufferPtr_pu8 = Dcm_DsldProtocol_pcst[Dcm_DsldGlobal_st.idxCurrentProtocol_u8].rx_mainBuffer_pa;
    /* Initializing the buffer index to 1 so that it indices to the first buffer */
    Dcm_QueueStructure_st.idxBufferIndex_u8 = 1;
    /* Setting the Queue state to IDLE */
    Dcm_QueueStructure_st.Dcm_QueHandling_en = DCM_QUEUE_IDLE;
}
#endif

/**
 **************************************************************************************************
 * Dcm_Prv_ActiveConfigSet : This function will initialize the variable which will hold the mask
 *                          for current active configuration
 * \param[in]      None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF )
static void Dcm_Prv_ActiveConfigSet(void)
{
    if ( Dcm_ActiveConfigSet_Ptr != NULL_PTR )
    {   /*Check if the configsetid passed is  valid*/
        if((Dcm_ActiveConfigSet_Ptr->ConfigSetId) > DCM_PRV_MAXNUM_OF_CONFIG)
        {
            /* Invalid ConfigSetID switch to default config set which here is Dcm_ConfigType_dataSetName1*/
            Dcm_ActiveConfiguration_u8 = 1 ;
        }
        else
        {
            /* Initialize the Dcm_ActiveConfiguration_u8 variable which will hold the bit mask for current active config */
            Dcm_ActiveConfiguration_u8 = (1 << (Dcm_ActiveConfigSet_Ptr->ConfigSetId)) ;
        }
    }
    else
    {
        /* If Configptr is a null pointer,set active config Set global variable to be Default Config Set i.e. Dcm_ConfigType_dataSetName1 */
        Dcm_ActiveConfiguration_u8 = 1 ;
    }
}
#endif


/**
 **************************************************************************************************
 * Dcm_Init : Initialisation of DCM module
 * \param      ConfigPtr :  Dcm_ConfigType
 *             If PostBuild is enabled the pointer will be set to Active Configuration Set
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

void Dcm_Init(const Dcm_ConfigType * ConfigPtr )
{
    PduIdType idxPduIndex_u8;
    uint8 idxIndex_u8;
#if((DCM_CFG_DSPOBDSUPPORT_ENABLED!= DCM_CFG_OFF)&&(DCM_CFG_DSP_OBDMODE9_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_INFOTYPE_SUPPORT != DCM_CFG_OFF))
uint8 countbitmaxarrayIdx;
uint8 sizeofbitmaxarray=0x08u;
#endif
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF )
    Dcm_ActiveConfigSet_Ptr = ConfigPtr;
#else
    (void)ConfigPtr;
#endif

    /* get the pointer from configuration, then store them in global variable */
    Dcm_DsldProtocol_pcst      = Dcm_Dsld_Conf_cs.protocol_table_pcs;
    Dcm_DsldRxTable_pcu8       = Dcm_Dsld_Conf_cs.ptr_rxtable_pca;
    Dcm_DsldConnTable_pcst     = Dcm_Dsld_Conf_cs.ptr_conntable_pcs;
    Dcm_DsldSessionTable_pcu8 = Dcm_Dsld_Conf_cs.session_lookup_table_pcau8;

    /* setting session, security access, routine control, ROE, RDPI, IOCBI*/
    Dcm_Prv_UDSinit();

#if (DCM_CFG_DSP_DDDISTORINGTONVRAM_ENABLED != DCM_CFG_OFF)
    /*TRACE[SWS_Dcm_00866]*/
    /*TRACE[SWS_Dcm_00867]*/
    Dcm_Prv_RestoreDddId();
#endif

    /*Initialising Dcm_DsldGlobal_st.active_commode_e.ComMState */
    /*Reset channel states */
    for(idxIndex_u8 = 0;idxIndex_u8<DCM_NUM_COMM_CHANNEL;idxIndex_u8++)
    {
        Dcm_active_commode_e[idxIndex_u8].ComMState =   DCM_DSLD_NO_COM_MODE;
        Dcm_active_commode_e[idxIndex_u8].ComMChannelId= Dcm_Dsld_ComMChannelId_acu8[idxIndex_u8];
    }


    /* Initialize the service table pointer to the first service table. This is done only for initialization */
    Dcm_DsldSrvTable_pcst = Dcm_Dsld_Conf_cs.sid_table_pcs[0].ptr_service_table_pcs;

#if(DCM_CFG_PROTOCOL_PREMPTION_ENABLED != DCM_CFG_OFF)
    /* Make these Rx pdu id invalid */
    Dcm_DsldGlobal_st.dataNewRxPduId_u8      =  0xff;
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    Dcm_DsldGlobal_st.dataPassRxPduId_u8     =  0xff;
#endif
    /* Reset the state */
    Dcm_Prv_SetDslPreemptionState((DSL_STATE_IDLE));
#endif


    /* Initialisation of SID array and array to indicate if a Func TP request was received for each RXPDU */
    for(idxPduIndex_u8=0x0; idxPduIndex_u8 < DCM_CFG_NUM_RX_PDUID;idxPduIndex_u8++)
    {
        /* Default value is set */
        /* Multiocore: No lock needed as there cannot be parallel access during INIT */
        Dcm_DslRxPduArray_ast[idxPduIndex_u8].Dcm_DslServiceId_u8   = DCM_SERVICEID_DEFAULT_VALUE;
        /*Func TP request was not received for each RXPDU */
        Dcm_DslRxPduArray_ast[idxPduIndex_u8].Dcm_DslFuncTesterPresent_b      = FALSE;
    }


    Dcm_Prv_SetDsdState((DSD_IDLE));
    /* Multicore: No lock needed here as Dsl state is an atomic operation and there will not be any parallel access during INIT */
    Dcm_Prv_SetDslState((DSL_STATE_IDLE));
    Dcm_Prv_SetDslSubState((DSL_SUBSTATE_S3_OR_P3_TIMEMONITORING));
    /*Reset the Communication or protocol active status*/
    Dcm_DsldGlobal_st.flgCommActive_b       =  FALSE;
    Dcm_DsldGlobal_st.idxCurrentProtocol_u8 =  0x00u;
    Dcm_DsldGlobal_st.nrActiveConn_u8      =  0x00u;
    /*Reset the Application triggered wait pend status*/
    Dcm_DslTransmit_st.isForceResponsePendRequested_b   =  FALSE;
    Dcm_SrvOpstatus_u8 = DCM_INITIAL;
    Dcm_ExtSrvOpStatus_u8 = DCM_INITIAL;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_SetOBDState((DCM_OBD_IDLE));
    Dcm_OBDGlobal_st.flgCommActive_b = FALSE;
    Dcm_OBDGlobal_st.idxCurrentProtocol_u8 = 0x00;
    Dcm_OBDGlobal_st.nrActiveConn_u8 = 0x00u;
    Dcm_OBDSrvOpstatus_u8 = DCM_INITIAL;
    Dcm_OBDExtSrvOpStatus_u8 = DCM_INITIAL;
    Dcm_ObdSendTxConfirmation_b = FALSE;

#endif

#if( DCM_BUFQUEUE_ENABLED!= DCM_CFG_OFF)
    Dcm_Prv_BufqueueInit();
#endif

#if(DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF )
    Dcm_Prv_ActiveConfigSet();
#endif

#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    /* Paged buffer transmission is off */
    Dcm_DsldGlobal_st.flgPagedBufferTxOn_b = FALSE;
#endif

    /*Initialisation of timer variables */
#if (DCM_CFG_OSTIMER_USE != FALSE)
    Dcm_P2OrS3StartTick_u32 = 0x00u;
#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    Dcm_OBDP2OrS3StartTick_u32 = 0x00;
#endif
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    Dcm_TimerStartTick_u32 = 0x00u;
#endif
#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    Dcm_PagedBufferStartTick_u32 = 0x00u;
#endif
    Dcm_CurrTick_u32 = 0x00u;
#endif

#if ((DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)||(DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF))
#if (DCM_CFG_OSTIMER_USE != FALSE)
    /* Reset the tick value */
    Dcm_SysPrevTick_u32 = 0x0;
#endif
#endif

#if (DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_WarmStart();
#endif

#if(DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)
    Dcm_Prv_SecaInit();
#endif

#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF))
    Dcm_isFirstReponseSent_b = FALSE;
#endif

#if((DCM_CFG_VIN_SUPPORTED != DCM_CFG_OFF) && (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF))
    Dcm_VinReceived_b = FALSE;
    Dcm_GetvinConditionCheckRead = FALSE;
#endif

#if((DCM_CFG_DSPOBDSUPPORT_ENABLED!= DCM_CFG_OFF)&&(DCM_CFG_DSP_OBDMODE9_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_INFOTYPE_SUPPORT != DCM_CFG_OFF))
    for(countbitmaxarrayIdx=0x00u;countbitmaxarrayIdx<sizeofbitmaxarray;countbitmaxarrayIdx++)
    {
        /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type pointer is converted to void pointer*/
        DCM_MEMCOPY(&Dcm_Dsp_Mode9Bitmask_acs[countbitmaxarrayIdx],&Dcm_Dsp_Cfg_Mode9Bitmask_acs[countbitmaxarrayIdx],sizeofbitmaxarray);
    }
    Dcm_SupportInfotype_b=TRUE;
#endif

    /* Variable Set to TRUE to indicate that Dcm is initialised and can accept tester requests */
    Dcm_acceptRequests_b= TRUE;
    /**Donot add any code below Dcm_acceptRequests_b=TRUE, whatever code needs to be added has to be done above this
     * statement, this is necessary since flag Dcm_acceptRequests_b is used in the Dcm_MainFunction() api to ensure
     * Dcm_Init and Dcm_Mainfunction does not run in parallel*/
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
