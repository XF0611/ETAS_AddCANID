

/*---------------------------------------------------------------------------------------*/
/*- Include files                                                                       -*/
/*---------------------------------------------------------------------------------------*/
#include "BswM.h"
#include "BswM_Prv.h"
#if (defined(BSWM_RTE_IN_USE) && (BSWM_RTE_IN_USE == TRUE))
#include "Rte_Main.h"
#endif

/*---------------------------------------------------------------------------------------*/
/*- Global Variables                                                                    -*/
/*---------------------------------------------------------------------------------------*/
#define BSWM_START_SEC_VAR_INIT_UNSPECIFIED
#include "BswM_MemMap.h"

#if (defined(BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL) && (BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL > 0))

/* Deadline Monitoring Ipdu Groups Vector */
boolean BswM_AC_DeadlineMonitoring_ExecFlag_b = FALSE;
Com_IpduGroupVector BswM_AC_DeadlineMonitoring_IpduGroup;

#endif

#define BSWM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "BswM_MemMap.h"


/*---------------------------------------------------------------------------------------*/
/*- Function Definition                                                                 -*/
/*---------------------------------------------------------------------------------------*/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/*****************************************************************************************
* Function name :   BswM_Prv_Action_ComMAllowCom
* Description   :   This function allows or blocks a communication channel.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_ComMAllowCom( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_COMM_ALLOW_COM) && (BSWM_NO_OF_AC_COMM_ALLOW_COM > 0))
/*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
* to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_ComMAllowComType_tst * dataActionPara_pst = (const BswM_Cfg_AC_ComMAllowComType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    /* ComM Channel communication is allowed or blocked by invoking following function */
    ComM_CommunicationAllowed(dataActionPara_pst->idNetworkReference_u8, dataActionPara_pst->isComAllowed_b);

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}


/*****************************************************************************************
* Function name :   BswM_Prv_Action_ComMModeLimit
* Description   :   This functions limits the Communication channel to No mode.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_ComMModeLimit( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_COMM_MODE_LIMITATION) && (BSWM_NO_OF_AC_COMM_MODE_LIMITATION > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_ComMModeLimitationType_tst * dataActionPara_pst = (const BswM_Cfg_AC_ComMModeLimitationType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    *action_RetVal_pu8 = ComM_LimitChannelToNoComMode(dataActionPara_pst->idNetworkReference_u8, dataActionPara_pst->isComMLimitMode_b);

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}


/*****************************************************************************************
* Function name :   BswM_Prv_Action_ComMModeSwitch
* Description   :   This function requests for either Full communication or no communication of a ComM Channel.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_ComMModeSwitch( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_COMM_MODE_SWITCH) && (BSWM_NO_OF_AC_COMM_MODE_SWITCH > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_ComMModeSwitchType_tst * dataActionPara_pst = (const BswM_Cfg_AC_ComMModeSwitchType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    if (BSWM_FULL_COM == dataActionPara_pst->dataComMRequestedMode_en)
    {
        *action_RetVal_pu8 = ComM_RequestComMode(dataActionPara_pst->idComMUserReference_u8, COMM_FULL_COMMUNICATION);
    }
    else
    {
        *action_RetVal_pu8 = ComM_RequestComMode(dataActionPara_pst->idComMUserReference_u8, COMM_NO_COMMUNICATION);
    }

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}


/*****************************************************************************************
* Function name :   BswM_Prv_Action_DeadlineMntCtrl
* Description   :   This function enables or disables deadline monitoring of ipdu groups.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_DeadlineMntCtrl( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL) && (BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL > 0))

    uint16 idxCounter = 0;
    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_DeadlineMonitoringControlType_tst * dataActionPara_pst = (const BswM_Cfg_AC_DeadlineMonitoringControlType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    for (idxCounter = 0; (idxCounter < dataActionPara_pst->nrEnabledDMPduGroupRef_u16); idxCounter++)
    {
        Com_SetIpduGroup(BswM_AC_DeadlineMonitoring_IpduGroup, dataActionPara_pst->adrEnabledDMPduGroupRefID_puo[idxCounter], TRUE);
    }

    for (idxCounter = 0; (idxCounter < dataActionPara_pst->nrDisabledDMPduGroupRef_u16); idxCounter++)
    {
        Com_SetIpduGroup(BswM_AC_DeadlineMonitoring_IpduGroup, dataActionPara_pst->adrDisabledDMPduGroupRefID_puo[idxCounter], FALSE);
    }

    BswM_AC_DeadlineMonitoring_ExecFlag_b = TRUE;

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}
/*****************************************************************************************
* Function name :   void BswM_Prv_Action_EcuMDriverInitListBswM( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This container defines the action to trigger an EcuM driver initialization list.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for EcuM Driver Init List BswM action .
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_EcuMDriverInitListBswM( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_ECUMDRIVERINITLISTBSWM) && (BSWM_NO_OF_AC_ECUMDRIVERINITLISTBSWM > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_EcuMDriverInitListBswMType_tst * dataActionPara_pst = (const BswM_Cfg_AC_EcuMDriverInitListBswMType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    /* Set the state of ECU State Manager */
    BswM_Cfg_EcuMDriverInitListBswMFunction(dataActionPara_pst->idBswMEcuMDriverInitListBswM_u16);

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}
/*****************************************************************************************
* Function name :   void BswM_Prv_Action_EcuMStateSwitch( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This function switch a State of the ECU State Manager.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for EcuState Switch action .
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_EcuMStateSwitch( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_ECUMSTATESWITCH) && (BSWM_NO_OF_AC_ECUMSTATESWITCH > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_EcuMStateSwitchType_tst * dataActionPara_pst = (const BswM_Cfg_AC_EcuMStateSwitchType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    /* Set the state of ECU State Manager */
    EcuM_SetState(dataActionPara_pst->dataState_u8);

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   void BswM_Prv_Action_EthIfSwitchPortGroup( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This function switch mode of EthIf PortGroup.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for BswMEthIfSwitchPortGroupRequestMode action .
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_EthIfSwitchPortGroup( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_ETHIFMODESWITCH) && (BSWM_NO_OF_AC_ETHIFMODESWITCH > 0))
    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_EthIfModeSwitch_tst * dataActionPara_pst = (const BswM_Cfg_AC_EthIfModeSwitch_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    if(BSWM_ETHTRCV_MODE_ACTIVE == dataActionPara_pst->dataPortMode)
    {
        *action_RetVal_pu8 = EthIf_SwitchPortGroupRequestMode(dataActionPara_pst->dataPortGroupIdx,ETHTRCV_MODE_ACTIVE);
    }
    else
    {
        *action_RetVal_pu8 = EthIf_SwitchPortGroupRequestMode(dataActionPara_pst->dataPortGroupIdx,ETHTRCV_MODE_DOWN);
    }


#else /* BSWM_NO_OF_AC_ETHIFMODESWITCH  */

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif /* BSWM_NO_OF_AC_ETHIFMODESWITCH  */
    return;
}

/*****************************************************************************************
* Function name :   void BswM_Prv_Action_PduRGrpCtrl( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This function is called by BswM_Prv_Evaluate_Action corresponding to ACTION enum  BswM_ActionPDU_ROUTER_CNTRL and does enabling or disabling of Pdu Groups.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for PduR Router Control .
* Return value   :  None
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_PduRGrpCtrl( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_PDUROUTERCONTROL) && (BSWM_NO_OF_AC_PDUROUTERCONTROL > 0 ))

    uint8 idx_u8 = 0;
    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_PduRouterControlType_tst * dataActionPara_pst = (const BswM_Cfg_AC_PduRouterControlType_tst *)dataActionParameter_pv;
    /* Initialize return value to "no return value" */
    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    /*Check if the RouterAction is BSWM_PDUR_ENABLE */
    if(dataActionPara_pst->stPduRouterAction_en == BSWM_PDUR_ENABLE)
    {
         /* Enable  all PduR_RoutingPathGroupIds */
         for (idx_u8 = 0; idx_u8 < dataActionPara_pst->nrRoutingPathGroupID_uo; idx_u8++)
         {
              PduR_EnableRouting(dataActionPara_pst->idRoutingPathGroupRefID_pao[idx_u8]);
         }
    }
    else
    {
         /*Check if the RouterAction is BSWM_PDUR_DISABLE and DisableInitBuffer is TRUE */
         if(dataActionPara_pst->isPduRouterDisableInitBuffer_b == TRUE)
         {
             /* Disable  all PduR_RoutingPathGroupIdTypes with DisableInitBuffer is TRUE  */
             for (idx_u8 = 0; idx_u8 < dataActionPara_pst->nrRoutingPathGroupID_uo ; idx_u8++)
             {
                 PduR_DisableRouting(dataActionPara_pst->idRoutingPathGroupRefID_pao[idx_u8], TRUE );
             }
         }
         /*Check if the RouterAction is BSWM_PDUR_DISABLE and DisableInitBuffer is FALSE */
         else
         {
             /* Disable  all PduR_RoutingPathGroupIdTypes with DisableInitBuffer is FALSE */
             for (idx_u8 = 0; idx_u8 < dataActionPara_pst->nrRoutingPathGroupID_uo; idx_u8++)
             {
                 PduR_DisableRouting(dataActionPara_pst->idRoutingPathGroupRefID_pao[idx_u8], FALSE );
             }
        }
    }
#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}
/*****************************************************************************************
* Function name :   BswM_Prv_Action_SdClientServiceModeRequest
* Description   :   This function allows or blocks a communication channel.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_SdClientServiceModeRequest( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_SDCLIENT_SRVMR) && (BSWM_NO_OF_AC_SDCLIENT_SRVMR > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_SdClientSrvMRType_tst * dataActionPara_pst = (const BswM_Cfg_AC_SdClientSrvMRType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    if ( BSWM_SD_CLIENT_SERVICE_RELEASED == dataActionPara_pst->ClientServiceMode)
    {
        /* Sd_ClientServiceSetState is allowed or blocked by invoking following function */
        *action_RetVal_pu8 =Sd_ClientServiceSetState(dataActionPara_pst->ClientServiceInstanceID, SD_CLIENT_SERVICE_RELEASED);

    }
    else
    {
        /* Sd_ClientServiceSetState is allowed or blocked by invoking following function */
        *action_RetVal_pu8 =Sd_ClientServiceSetState(dataActionPara_pst->ClientServiceInstanceID, SD_CLIENT_SERVICE_REQUESTED);

    }

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_SdConsumedEventGroupModeRequest
* Description   :   This function allows or blocks a communication channel.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_SdConsumedEventGroupModeRequest( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_SDCONSMD_EVNTGRPMR) && (BSWM_NO_OF_AC_SDCONSMD_EVNTGRPMR > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_SdConsumedeEvntGrpMRType_tst * dataActionPara_pst = (const BswM_Cfg_AC_SdConsumedeEvntGrpMRType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;


    if (BSWM_SD_CONSUMED_EVENTGROUP_RELEASED == dataActionPara_pst->ConsumedEventGroupState)
    {
        /*Sd_ConsumedEventGroupSetState is allowed or blocked by invoking following function */
        *action_RetVal_pu8 =Sd_ConsumedEventGroupSetState(dataActionPara_pst->SdConsumedEventGroupHandleId, SD_CONSUMED_EVENTGROUP_RELEASED);

    }
    else
    {
        /*Sd_ConsumedEventGroupSetState is allowed or blocked by invoking following function */
        *action_RetVal_pu8 =Sd_ConsumedEventGroupSetState(dataActionPara_pst->SdConsumedEventGroupHandleId, SD_CONSUMED_EVENTGROUP_REQUESTED);

    }



#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_SdServerServiceModeRequest
* Description   :   This function allows or blocks a communication channel.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_SdServerServiceModeRequest( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_SDSRVR_SRVMR) && (BSWM_NO_OF_AC_SDSRVR_SRVMR > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_SdServerSrvMRType_tst * dataActionPara_pst = (const BswM_Cfg_AC_SdServerSrvMRType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    if (BSWM_SD_SERVER_SERVICE_AVAILABLE == dataActionPara_pst->ServerServiceState)
    {
        /* Sd_ServerServiceSetState is allowed or blocked by invoking following function */
        *action_RetVal_pu8 =Sd_ServerServiceSetState(dataActionPara_pst->SdServerServiceHandleId, SD_SERVER_SERVICE_AVAILABLE);
    }
    else
    {
        /* Sd_ServerServiceSetState is allowed or blocked by invoking following function */
        *action_RetVal_pu8 =Sd_ServerServiceSetState(dataActionPara_pst->SdServerServiceHandleId, SD_SERVER_SERVICE_DOWN);
    }




#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   void BswM_Prv_Action_BswMSwitchIPduMode( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This function is called by BswM_Prv_Evaluate_Action corresponding to ACTION enum BswM_ActionSWITCH_IPDU_MODE and does Selection of the transmission mode an I-PDU to be sent by COM.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for Switch Ipdu Mode .
* Return value   :  None
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_BswMSwitchIPduMode( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_SWITCHIPDUMODE) && (BSWM_NO_OF_AC_SWITCHIPDUMODE > 0 ))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_SwitchIPduModeType_tst * dataActionPara_pst = (const BswM_Cfg_AC_SwitchIPduModeType_tst *)dataActionParameter_pv;
    /* Initialize to "no return value" */
    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    /*Selection of the transmission mode an I-PDU to be sent by COM.*/
    Com_SwitchIpduTxMode( dataActionPara_pst->idSwitchIPduModeRef_uo ,dataActionPara_pst->isSwitchIPduMode_b );

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   void BswM_Prv_Action_BswMPduGroupSwitch( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This function is called by BswM_Prv_Evaluate_Action corresponding to ACTION enum BSWM_ACTION_PDU_GRP_SWITCH and does Disbling and Enabling of PDU Groups.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for IpduGroup Switch .
* Return value   :  None
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_BswMPduGroupSwitch( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_IPDUGROUPSWITCH) && (BSWM_NO_OF_AC_IPDUGROUPSWITCH > 0 ))

    uint8 idx_u8 = 0;
    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_PduGroupSwitchType_tst * dataActionPara_pst = (const BswM_Cfg_AC_PduGroupSwitchType_tst *)dataActionParameter_pv;
    /* Initialize to "no return value" */
    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    /* PDU Groups that are newly added or need reinitilisation*/
    if(FALSE != dataActionPara_pst->isPduGroupSwitchReinit_b)
    {
        /* PDU Groups that should be disabled*/
        for (idx_u8 = 0; idx_u8 < dataActionPara_pst->nrOfDisabledPduGroupRef_u8 ; idx_u8++)
        {
            Com_SetIpduGroup( BswM_IPduGrpVctr_ReinitTrue_au8 ,dataActionPara_pst->adrDisabledPduGroupRefID_pu8[idx_u8] ,FALSE);
            Com_SetIpduGroup( BswM_IPduGrpVctr_ReinitAll_au8 ,dataActionPara_pst->adrDisabledPduGroupRefID_pu8[idx_u8] ,FALSE);
        }
        /* PDU Groups that should be Enabled*/
        for (idx_u8 = 0; idx_u8 < dataActionPara_pst->nrEnabledPduGroupRef_u8 ; idx_u8++)
        {
            Com_SetIpduGroup( BswM_IPduGrpVctr_ReinitTrue_au8 ,dataActionPara_pst->adrEnabledPduGroupRefID_pu8[idx_u8] ,TRUE);
            Com_SetIpduGroup( BswM_IPduGrpVctr_ReinitAll_au8 ,dataActionPara_pst->adrEnabledPduGroupRefID_pu8[idx_u8] ,TRUE);
        }

    }
	else
	{
	    /* PDU Groups that should be disabled*/
	    for (idx_u8 = 0; idx_u8 < dataActionPara_pst->nrOfDisabledPduGroupRef_u8 ; idx_u8++)
	    {
	        Com_SetIpduGroup( BswM_IPduGrpVctr_ReinitAll_au8 ,dataActionPara_pst->adrDisabledPduGroupRefID_pu8[idx_u8] ,FALSE);
	    }
	    /* PDU Groups that should be Enabled*/
	    for (idx_u8 = 0; idx_u8 < dataActionPara_pst->nrEnabledPduGroupRef_u8 ; idx_u8++)
	    {
	       Com_SetIpduGroup( BswM_IPduGrpVctr_ReinitAll_au8 ,dataActionPara_pst->adrEnabledPduGroupRefID_pu8[idx_u8] ,TRUE);
	    }
	}


    /* Enable the flag, PDU group switch action has been performed */
    BswM_PduGrpSwt_b = TRUE;
#else
    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;
#endif
    return;
}

/*****************************************************************************************
* Function name :   void BswM_Prv_Action_BswMTimerControl( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This container defines the action to trigger a timer.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for Timer Control BswM action .
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_BswMTimerControl( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_BSWMTIMERCONTROL) && (BSWM_NO_OF_AC_BSWMTIMERCONTROL > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_BswMTimerControlType_tst * dataActionPara_pst = (const BswM_Cfg_AC_BswMTimerControlType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    if (BSWM_TIMER_START == dataActionPara_pst->BswMTimerControlState)
        {
            /* Call the function to start the timer */
            BswM_Cfg_BswMTimerControlFunction(BSWM_TIMER_START, dataActionPara_pst->BswMTimerValue_f32);
        }
    else
        {
            /* Call the function to stop the timer */
            BswM_Cfg_BswMTimerControlFunction(BSWM_TIMER_STOP, dataActionPara_pst->BswMTimerValue_f32);
        }

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   void BswM_Prv_Action_BswMTriggerIPduSend( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This function is called by BswM_Prv_Evaluate_Action corresponding to ACTION enum BswM_ActionTRIG_IPDU_SEND and does triggering for transmission for I-PDUs.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for Trigger Ipdu Send .
* Return value   :  None
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_BswMTriggerIPduSend( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv )
{
#if (defined(BSWM_NO_OF_AC_TRIGIPDUSEND) && (BSWM_NO_OF_AC_TRIGIPDUSEND > 0 ))

    uint8 idx_u8 = 0;
    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_TriggerIPduSendType_tst * dataActionPara_pst = (const BswM_Cfg_AC_TriggerIPduSendType_tst *)dataActionParameter_pv;
    /* Initialize to "no return value" */
    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    /* I-PDUs that should be triggered for transmission */
    for (idx_u8 = 0; idx_u8 < dataActionPara_pst->nrComIPduHandleID_uo ; idx_u8++)
    {
        Com_TriggerIPDUSend(dataActionPara_pst->idComIPduHandleRefID_pauo[idx_u8]);
    }

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}
/*****************************************************************************************
* Function name :   void BswM_Prv_Action_UserCallout( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This function is called by BswM_Prv_Evaluate_Action corresponding to ACTION enum BswM_ActionUSR_CALLOUT and does invoking of user defined functions.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for User Callout .
* Return value   :  None
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_UserCallout( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv )
{
#if (defined(BSWM_NO_OF_AC_USERCALLOUT) && (BSWM_NO_OF_AC_USERCALLOUT > 0 ))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_UserCalloutType_tst * dataActionPara_pst =  (const BswM_Cfg_AC_UserCalloutType_tst *)dataActionParameter_pv;
    /* Initialize to "no return value" */
    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    /* Invoke the UserCallout */
    BswM_Cfg_ProcessUserCalloutFunction(dataActionPara_pst->idUserCallout_u16);

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   void BswM_Prv_Action_NmControl( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This function is called by BswM_Prv_Evaluate_Action corresponding to ACTION enum BswM_Prv_ActionNM_CNTRL and does invoking of user defined functions.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for User Callout .
* Return value   :  None
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_NmControl( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv )
{
#if (defined(BSWM_NO_OF_AC_NM_CNTRL) && (BSWM_NO_OF_AC_NM_CNTRL > 0 ))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_NmControlType_tst * dataActionPara_pst =  (const BswM_Cfg_AC_NmControlType_tst *)dataActionParameter_pv;

    if(BSWM_NM_DISABLE == dataActionPara_pst->dataNMRequestedAction_en)
    {
        *action_RetVal_pu8 = Nm_DisableCommunication(dataActionPara_pst->idChannel_u8);
    }
    else
    {
        *action_RetVal_pu8 = Nm_EnableCommunication(dataActionPara_pst->idChannel_u8);
    }

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_J1939DcmStateSwitch
* Description   :   Diagnostic Communication Managers network state for a J1939 node. J1939Dcm_SetState is called when this action is configured
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_J1939DcmStateSwitch( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_J1939DCM_STATE_SWITCH) && (BSWM_NO_OF_AC_J1939DCM_STATE_SWITCH > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_J1939DcmStateSwitchType_tst * dataActionPara_pst = (const BswM_Cfg_AC_J1939DcmStateSwitchType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    if (BSWM_J1939DCM_STATE_ONLINE == dataActionPara_pst->dataJ1939DcmRequestedMode_en)
    {
        /* J1939Dcm State Change by invoking following function */
        *action_RetVal_pu8 = J1939Dcm_SetState(dataActionPara_pst->idNetworkReference_u8, dataActionPara_pst->idNodeReference_u8 ,J1939DCM_STATE_ONLINE);
    }
    else
    {
        /* J1939Dcm State Change by invoking following function */
        *action_RetVal_pu8 = J1939Dcm_SetState(dataActionPara_pst->idNetworkReference_u8, dataActionPara_pst->idNodeReference_u8 ,J1939DCM_STATE_OFFLINE);
    }

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_LINScheduleSwitch
* Description   :   This action is to switch LIN schedule table. LinSM_ScheduleRequest is called when this action is configured.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_LINScheduleSwitch( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_LINSM_SCHEDULESWITCH) && (BSWM_NO_OF_AC_LINSM_SCHEDULESWITCH > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
     * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_LinScheduleSwitchType_tst * dataActionPara_pst = (const BswM_Cfg_AC_LinScheduleSwitchType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    /*  LIN schedule table switching by invoking following function */
    *action_RetVal_pu8 = LinSM_ScheduleRequest(dataActionPara_pst->idNetwork_u8, dataActionPara_pst->idSchedule_u8 );

#else

   *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
   (void)dataActionParameter_pv;

#endif
   return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_J1939RmStateSwitch
* Description   :   Request Managers network state for a J1939 node. J1939Rm_SetState is called when this action is configured
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_J1939RmStateSwitch( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_J1939RM_STATE_SWITCH) && (BSWM_NO_OF_AC_J1939RM_STATE_SWITCH > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_J1939RmStateSwitchType_tst * dataActionPara_pst = (const BswM_Cfg_AC_J1939RmStateSwitchType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    if (BSWM_J1939RM_STATE_ONLINE == dataActionPara_pst->dataJ1939RmRequestedMode_en)
    {
        /* J1939Rm State Change by invoking following function */
        *action_RetVal_pu8 = J1939Rm_SetState(dataActionPara_pst->idNetworkReference_u8, dataActionPara_pst->idNodeReference_u8 ,J1939RM_STATE_ONLINE);
    }
    else
    {
        /* J1939Rm State Change by invoking following function */
        *action_RetVal_pu8 = J1939Rm_SetState(dataActionPara_pst->idNetworkReference_u8, dataActionPara_pst->idNodeReference_u8 ,J1939RM_STATE_OFFLINE);
    }

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_RteModeRequest
* Description   :   Requests the RTE to switch the mode when this action is invoked.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_RteModeRequest( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_RTE_MODE_REQUEST) && (BSWM_NO_OF_AC_RTE_MODE_REQUEST > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_RteModeRequestType_tst * dataActionPara_pst = (const BswM_Cfg_AC_RteModeRequestType_tst*)dataActionParameter_pv;

    /* Initialize to "no return value" */
    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    *action_RetVal_pu8 = BswM_Cfg_ProcessRteModeRequestFunction(dataActionPara_pst->idRteModeRequest_u16);

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_RteStart
* Description   :   This action is used to start the RTE from BswM.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_RteStart( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_RTE_START) && (BSWM_NO_OF_AC_RTE_START > 0))

     /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
     * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
     const BswM_Cfg_AC_RteStartType_tst * dataActionPara_pst = (const BswM_Cfg_AC_RteStartType_tst*)dataActionParameter_pv;

     (void)dataActionPara_pst->idRteStart_u16;
     *action_RetVal_pu8=BSWM_NO_RET_VALUE;
     (void)dataActionParameter_pv;

     *action_RetVal_pu8 = Rte_Start();

#else

     *action_RetVal_pu8=BSWM_NO_RET_VALUE;
     (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_RteStop
* Description   :   This action is used to stop the RTE from BswM.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_RteStop( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_RTE_STOP) && (BSWM_NO_OF_AC_RTE_STOP > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_RteStopType_tst * dataActionPara_pst = (const BswM_Cfg_AC_RteStopType_tst*)dataActionParameter_pv;

    (void)dataActionPara_pst->idRteStop_u16;
    *action_RetVal_pu8=BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

    *action_RetVal_pu8 = Rte_Stop();

#else

    *action_RetVal_pu8=BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_RteSwitch
* Description   :   switch the mode when this action is invoked.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_RteSwitch( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_RTE_SWITCH) && (BSWM_NO_OF_AC_RTE_SWITCH > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_RteSwitchType_tst * dataActionPara_pst = (const BswM_Cfg_AC_RteSwitchType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BswM_Cfg_ProcessRteSwitchFunction(dataActionPara_pst->idRteSwitch_u16);

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   BswM_Prv_Action_SchMSwitch
* Description   :   switch the mode when this action is invoked.
* Parameter     :   Std_ReturnType*, const void*
* Return value  :   void
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_SchMSwitch( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv)
{
#if (defined(BSWM_NO_OF_AC_SCHM_SWITCH) && (BSWM_NO_OF_AC_SCHM_SWITCH > 0))

    /*MR12 RULE 11.5 VIOLATION: The original configuration is of the specific action type, which is then typecast
    * to void and passed as void pointer by BswM_Prv_Evaluate_Action and then typecast back to the corresponding action type here*/
    const BswM_Cfg_AC_SchMSwitchType_tst * dataActionPara_pst = (const BswM_Cfg_AC_SchMSwitchType_tst*)dataActionParameter_pv;

    *action_RetVal_pu8 = BswM_Cfg_ProcessSchMSwitchFunction(dataActionPara_pst->idSchMSwitch_u16);

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

/*****************************************************************************************
* Function name :   void BswM_Prv_Action_RbSwcUserCallout( Std_ReturnType * action_RetVal_pu8,const void * dataActionParameter_pv)
* Description   :   This function is called by BswM_Prv_Evaluate_Action corresponding to ACTION enum BSWM_ACTION_RB_SWC_USR_CALLOUT and does invoking of user defined functions.
* Parameter  action_RetVal_pu8 :   This variable is a pointer that hold return value of ActionFunction
* Parameter  dataActionParameter_pv    :  This variable is a pointer that holds the ActionStructure for User Callout .
* Return value   :  None
* Remarks       :
*****************************************************************************************/
static void BswM_Prv_Action_RbSwcUserCallout( Std_ReturnType * action_RetVal_pu8, const void * dataActionParameter_pv )
{
#if (defined(BSWM_NO_OF_AC_RB_SWC_USERCALLOUT) && (BSWM_NO_OF_AC_RB_SWC_USERCALLOUT > 0 ))

    const BswM_Cfg_AC_RbSwcUserCalloutType_tst * dataActionPara_pst =  (const BswM_Cfg_AC_RbSwcUserCalloutType_tst *)dataActionParameter_pv;
    /* Initialize to "no return value" */
    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;

    /* Invoke the RbSwcUserCallout */
    BswM_Cfg_ProcessRbSwcUserCalloutFunction(dataActionPara_pst->idRbSwcUserCallout_u16);

#else

    *action_RetVal_pu8 = BSWM_NO_RET_VALUE;
    (void)dataActionParameter_pv;

#endif
    return;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/***********************************************************************************************************************
* Array of Functions Pointers to ActionFunction
***********************************************************************************************************************/
#define BSW_START_SEC_CONST_UNSPECIFIED
#include "Bsw_MemMap.h"
static const BswM_Action_FuncPtr_t BswM_Prv_AC_BswMActionFunction_FunPtr_cafct[BSWM_ACTIONLIST_SIZE] =
{
    NULL_PTR,                               			/*ActionPointer pointing to NULL as we dont refer to ACTIONLIST here            */
    &BswM_Prv_Action_ComMAllowCom,          			/*Action Function corresponding to enum BSWM_ACTION_COMM_ALLOW_COM              */
    &BswM_Prv_Action_ComMModeLimit,         			/*Action Function corresponding to enum BSWM_ACTION_COMM_MODE_LMTN              */
    &BswM_Prv_Action_ComMModeSwitch,        			/*Action Function corresponding to enum BSWM_ACTION_COMM_MODE_SWITCH            */
    &BswM_Prv_Action_DeadlineMntCtrl,       			/*Action Function corresponding to enum BSWM_ACTION_DEADLINE_MNT_CTRL           */
    &BswM_Prv_Action_EcuMStateSwitch,                               /*Action Function corresponding to enum BswM_Prv_ActionECUM_STATE_SWITCH*/
    &BswM_Prv_Action_EcuMDriverInitListBswM,            /*Action Function corresponding to enum BswM_Prv_ActionECUM_STATE_SWITCH*/
    &BswM_Prv_Action_EthIfSwitchPortGroup,
    &BswM_Prv_Action_J1939DcmStateSwitch,  				/*Action Function corresponding to enum BSWM_ACTION_J1939DCM_STATE_SWITCH       */
    &BswM_Prv_Action_J1939RmStateSwitch,    			/*Action Function corresponding to enum BSWM_ACTION_J1939RM_STATE_SWITCH        */
    &BswM_Prv_Action_LINScheduleSwitch,					/*Action Function corresponding to enum BSWM_ACTION_LIN_SCHDL_SWITCH            */
    &BswM_Prv_Action_NmControl,             			/*Action Function corresponding to enum BSWM_ACTION_NM_CNTRL                    */
    &BswM_Prv_Action_BswMPduGroupSwitch,    			/*Action Function corresponding to enum BSWM_ACTION_PDU_GRP_SWITCH              */
    &BswM_Prv_Action_PduRGrpCtrl,           			/*Action Function corresponding to enum BSWM_ACTION_PDU_ROUTER_CNTRL            */
    &BswM_Prv_Action_RteModeRequest,        			/*Action Function corresponding to enum BSWM_ACTION_RTE_MODE_REQUEST            */
    &BswM_Prv_Action_RteStart,                          /*Action Function corresponding to enum BSWM_ACTION_RTE_START                   */
    &BswM_Prv_Action_RteStop,                           /*Action Function corresponding to enum BSWM_ACTION_RTE_STOP                    */
    &BswM_Prv_Action_RteSwitch,             			/*Action Function corresponding to enum BSWM_ACTION_RTE_SWITCH                  */
    &BswM_Prv_Action_SchMSwitch,            			/*Action Function corresponding to enum BSWM_ACTION_SCHM_SWITCH                 */
    &BswM_Prv_Action_SdClientServiceModeRequest,        /*Action Function corresponding to enum BswM_Prv_ActionSD_CLNT_SERV_MODE_REQ*/
    &BswM_Prv_Action_SdConsumedEventGroupModeRequest,   /*Action Function corresponding to enum BswM_Prv_ActionSD_CNSMD_EVNT_GRP_MODE_REQ*/
    &BswM_Prv_Action_SdServerServiceModeRequest,        /*Action Function corresponding to enum BswM_Prv_ActionSD_SERVR_SERV_MODE_REQ*/
    &BswM_Prv_Action_BswMSwitchIPduMode,    /*Action Function corresponding to enum BSWM_ACTION_SWITCH_IPDU_MODE            */
    &BswM_Prv_Action_BswMTimerControl,                  /*Action Function corresponding to enum BswM_Prv_Action_Timer_Control*/
    &BswM_Prv_Action_BswMTriggerIPduSend,   /*Action Function corresponding to enum BSWM_ACTION_TRIG_IPDU_SEND              */
    &BswM_Prv_Action_UserCallout,            /*Action Function corresponding to enum BSWM_ACTION_USR_CALLOUT*/
    &BswM_Prv_Action_RbSwcUserCallout      /*Action Function corresponding to enum BSWM_ACTION_RB_SWC_USR_CALLOUT*/
};

#define BSW_STOP_SEC_CONST_UNSPECIFIED
#include "Bsw_MemMap.h"

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

/*****************************************************************************************
* Function name :   Std_ReturnType BswM_Prv_Evaluate_Action(const void * dataActionParameter_pv, BswM_Prv_ActionListItemType_ten isActionType_en)
* Description   :   This function is called by BswM_Prv_Evaluate_ActionList to execute Actions that are member of ActionList.
* Parameter  dataActionParameter_pv :   This variable is pointer pointing to structure that hold Action function parameters
* Parameter  isActionType_en    :   This variable indicates the ActionType.
* Return value  action_RetVal_u8 :This function returns Std_ReturnType.
* Remarks       :
*****************************************************************************************/
Std_ReturnType BswM_Prv_Evaluate_Action
(
    const void * dataActionParameter_pv,
    BswM_ActionListItemType_ten isActionType_en
)
{
    Std_ReturnType action_RetVal_u8 = BSWM_NO_RET_VALUE;
/* Check if any Rule is configured in the Project */
#ifdef BSWM_MAX_NO_OF_RULES

    /* Check if the isActionType_en is within the array boundary */
    if(isActionType_en < BSWM_ACTIONLIST_SIZE)
    {
        /* Check if the function pointer  is pointing to a valid address  */
        if(BswM_Prv_AC_BswMActionFunction_FunPtr_cafct[isActionType_en] != NULL_PTR)
        {
            /* Invoke the function pointer from the array corresponding to this ActionType  */
            BswM_Prv_AC_BswMActionFunction_FunPtr_cafct[isActionType_en](&action_RetVal_u8,dataActionParameter_pv);

        }

    }
#else
    (void)dataActionParameter_pv;
    (void)isActionType_en;
#endif
    return action_RetVal_u8;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/**********************************************************************************************************************
 *
 **********************************************************************************************************************/
