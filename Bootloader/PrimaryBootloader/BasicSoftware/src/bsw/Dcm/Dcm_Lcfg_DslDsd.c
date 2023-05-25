
#include "Dcm_Cfg_Prot.h"
#include "Dcm.h"
#include "Dcm_Lcfg_DspUds.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"
#include "Rte_Dcm.h"
#include "SchM_Dcm.h"



/* Variables to store the Array of signals for different Data Types */
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
    






#define DCM_START_SEC_CONST_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* This array of structure gives info about received rxpduid belongs to which CONNECTION                        */
/* In Dcm_providRxbuffer() function DCM will get the Rx pduid.Now this rxpduid act as index,                    */
/* go to this array get the info of this rxpduid belongs to which connection                                    */
/* First physical rx pduids CONNECTION info is configured and then functional ids CONNECTION info is configured.*/
/* One index is generated in dcm_Lcfg.h file (DCM_INDEX_FUNC_RX_PDUID). If rxpduid is less than this index      */
/* means this request is physical request.*/
static const uint8 Dcm_Dsld_rx_table_acu8[DCM_CFG_NUM_RX_PDUID]={


0x0, /*Rxpduid 0 belonging to the connection DcmDslConnection_CAN*/
0x0, /*Rxpduid 1 belonging to the connection DcmDslConnection_CAN*/


};


#define DCM_STOP_SEC_CONST_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static const PduIdType Dcm_DsldTxTable_cast[DCM_NUM_TX_PDUID] =
{
    PduRConf_PduRSrcPdu_Src_Dcm2CanTp_0x7B7};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


/* Generation of Diagnosis Buffers for Rx/Tx, one single buffer */
static uint8 Dcm_DsldDiagBuffer_au8[2050+2];
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* Connection number acts as the index to get the channel and TxPdu information for a particular connetion */
/* Each connection will be associated with single TxPduId                                           */
static const Dcm_Dsld_connType Dcm_DsldConnTable_cast[DCM_CFG_NUM_CONN]=
{
                    {0x0,PduRConf_PduRSrcPdu_Src_Dcm2CanTp_0x7B7,0xFF,0xFF,0x0,0x0,0x00,0x00}
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* Array for storing ComM channels configured in DCM  and their states*/
Dcm_Dsld_ComMChannel Dcm_active_commode_e[DCM_NUM_COMM_CHANNEL]=
{

{ComMConf_ComMChannel_ComMChannel_Can_Network_Channel,DCM_DSLD_NO_COM_MODE}

};
#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* Const Array for storing ComM channels configured in DCM.The same will be used for initialising  Dcm_active_commode_e[]*/
const uint8 Dcm_Dsld_ComMChannelId_acu8[DCM_NUM_COMM_CHANNEL]={
ComMConf_ComMChannel_ComMChannel_Can_Network_Channel,
};
#define DCM_STOP_SEC_CONST_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/* configuration of protocol table */
static const Dcm_Dsld_protocol_tableType Dcm_DsldProtocol_cast[]=
{
    {
      &Dcm_DsldDiagBuffer_au8[0],     /* Tx buffer address */
      &Dcm_DsldDiagBuffer_au8[0],     /* Rx buffer address */
      1026,                            /* Tx buffer size          */
      2050,                         /* Rx buffer size          */

      10000u,   /* P2 server time adjust*/
      10000u,   /* P2 star server time adjust*/
      DCM_UDS_ON_CAN,               /* Protocol id             */
      0,                            /* Id of Sid table         */
      0,                    /* Preemption Level      */

      0,          /* Index to the Temporary Pdu Info Structure */
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
      ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
       (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)), /* Configuration mask which indicates the availability of protocol in different configsets*/
#endif
      255u, /* If Client ID is not configured default value is assigned */
      FALSE,            /* Silently ignore new request on this protocol if received and rejected during pre-emption assertion */
      FALSE             /* No WaitPend sent on transit to Boot */
   }};

#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

 
 




/* Subservice of SID 0x3E of Service table DcmDsdServiceTable_UDS*/
static const Dcm_Dsld_SubServiceType Dcm_SrvTab0_SubSrv0_acs[]=
{

    {
      0x7uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &DcmAppl_UserSubServiceModeRuleService,                     /* default Mode rule function */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x0,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
};


/* Subservice of SID 0x10 of Service table DcmDsdServiceTable_UDS*/
static const Dcm_Dsld_SubServiceType Dcm_SrvTab0_SubSrv1_acs[]=
{

    {
      0x7uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &DcmAppl_UserSubServiceModeRuleService,                     /* default Mode rule function */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x1,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
,

    {
      0x7uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &DcmAppl_UserSubServiceModeRuleService,                     /* default Mode rule function */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x2,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
,

    {
      0x5uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &DcmAppl_UserSubServiceModeRuleService,                     /* default Mode rule function */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x3,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
};


/* Subservice of SID 0x27 of Service table DcmDsdServiceTable_UDS*/
static const Dcm_Dsld_SubServiceType Dcm_SrvTab0_SubSrv2_acs[]=
{

    {
      0x6uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &DcmAppl_UserSubServiceModeRuleService,                     /* default Mode rule function */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x11,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
,

    {
      0x6uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &DcmAppl_UserSubServiceModeRuleService,                     /* default Mode rule function */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x12,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
};




/* Subservice of SID 0x11 of Service table DcmDsdServiceTable_UDS*/
static const Dcm_Dsld_SubServiceType Dcm_SrvTab0_SubSrv5_acs[]=
{

    {
      0x7uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &DcmAppl_UserSubServiceModeRuleService,                     /* default Mode rule function */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x1,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
};






/* Subservice of SID 0x85 of Service table DcmDsdServiceTable_UDS*/
static const Dcm_Dsld_SubServiceType Dcm_SrvTab0_SubSrv10_acs[]=
{

    {
      0x4uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &Fbl_ProgM_EnableDtc,   /* User specific mode rule function  */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x1,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
,

    {
      0x4uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &Fbl_ProgM_DisableDtc,   /* User specific mode rule function  */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x2,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
};


/* Subservice of SID 0x28 of Service table DcmDsdServiceTable_UDS*/
static const Dcm_Dsld_SubServiceType Dcm_SrvTab0_SubSrv11_acs[]=
{

    {
      0x4uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &Fbl_ProgM_EnableRxAndTx,   /* User specific mode rule function  */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x0,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
,

    {
      0x4uL,                                  /* Allowed sessions for the subservice  */
              0xffffffffuL,                                              /* Allowed in all security levels */
      &Fbl_ProgM_DisableRxAndTx,   /* User specific mode rule function  */
      
      NULL_PTR, /* Service Handler for Subfunction */ 
      
       0x3,           /* Subservice Identifier */
      
      FALSE /* Is DSP subfunction or not*/ 
            
    }
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


/* Service table for 
DCM_UDS_ON_CAN  protocol  */
static const Dcm_Dsld_ServiceType Dcm_Dsld_sid_table0_acs[]=
{
{

      0x7uL,                               /* Allowed sessions                */

      0xffffffffuL,                  /* Allowed in all security levels                */
      &Dcm_DcmTesterPresent,             /* Service handler    */
      NULL_PTR,                        /* Init function of service  */
      0x3E,                          /* Service id              */
      
      TRUE,                             /* SubFunction Exists */
      TRUE,                          /* Service is located within Dcm */

      Dcm_SrvTab0_SubSrv0_acs,       /* Pointer to subservice table */
      1,                             /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &DcmAppl_DcmConfirmation      /* Reference Service confirmation Apis */
 },
{

      0x7uL,                               /* Allowed sessions                */

      0xffffffffuL,                  /* Allowed in all security levels                */
      &Dcm_DcmDiagnosticSessionControl,     /* Service handler    */
      &Dcm_Dsp_DscIni,                  /* Init function of service  */
      0x10,                          /* Service id              */
      
      TRUE,                             /* SubFunction Exists */
      TRUE,                          /* Service is located within Dcm */

      Dcm_SrvTab0_SubSrv1_acs,       /* Pointer to subservice table */
      3,                             /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &Dcm_Prv_DspDscConfirmation   /* Reference Service confirmation Apis */
 },
{

      0x6uL,                               /* Allowed sessions                */

      0xffffffffuL,                  /* Allowed in all security levels                */
      &Dcm_DcmSecurityAccess,            /* Service handler    */
      &Dcm_Dsp_SecaIni,                 /* Init function of service  */
      0x27,                          /* Service id              */
      
      TRUE,                             /* SubFunction Exists */
      TRUE,                          /* Service is located within Dcm */

      Dcm_SrvTab0_SubSrv2_acs,       /* Pointer to subservice table */
      2,                             /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &Dcm_Prv_DspSecurityConfirmation /* Reference Service confirmation Apis */
 },
{

      0x6uL,                               /* Allowed sessions                */

      
      0x2uL,                                /* Allowed security levels                */
      &Dcm_DcmWriteDataByIdentifier,     /* Service handler    */
      &Dcm_Dcm_WDBIInit,                /* Init function of service  */
      0x2E,                          /* Service id              */
      
      FALSE,                      /* SubFunction Doesn't Exist */
      TRUE,                          /* Service is located within Dcm */

      NULL_PTR,                     /* Pointer to subservice table  */
      0,                                                       /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &DcmAppl_DcmConfirmation      /* Reference Service confirmation Apis */
 },
{

      0x7uL,                               /* Allowed sessions                */

      0xffffffffuL,                  /* Allowed in all security levels                */
      &Dcm_DcmReadDataByIdentifier,      /* Service handler    */
      &Dcm_Dsp_RdbiIni,                 /* Init function of service  */
      0x22,                          /* Service id              */
      
      FALSE,                      /* SubFunction Doesn't Exist */
      TRUE,                          /* Service is located within Dcm */

      NULL_PTR,                     /* Pointer to subservice table  */
      0,                                                       /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &Dcm_Prv_DspRdbiConfirmation  /* Reference Service confirmation Apis */
 },
{

      0x7uL,                               /* Allowed sessions                */

      0xffffffffuL,                  /* Allowed in all security levels                */
      &Dcm_DcmEcuReset,                  /* Service handler    */
      &Dcm_Dsp_EcuReset_Ini,            /* Init function of service  */
      0x11,                          /* Service id              */
      
      TRUE,                             /* SubFunction Exists */
      TRUE,                          /* Service is located within Dcm */

      Dcm_SrvTab0_SubSrv5_acs,       /* Pointer to subservice table */
      1,                             /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &Dcm_Prv_DspEcuResetConfirmation /* Reference Service confirmation Apis */
 },
{

      0x7uL,                               /* Allowed sessions                */

      0xffffffffuL,                  /* Allowed in all security levels                */
      &Dcm_DcmRoutineControl,            /* Service handler    */
      &Dcm_Dsp_RC_Ini,                  /* Init function of service  */
      0x31,                          /* Service id              */
      
      FALSE,                      /* SubFunction Doesn't Exist */
      TRUE,                          /* Service is located within Dcm */

      NULL_PTR,                     /* Pointer to subservice table  */
      0,                                                       /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &Dcm_Prv_DspRCConfirmation    /* Reference Service confirmation Apis */
 },
{

      0x2uL,                               /* Allowed sessions                */

      
      0x2uL,                                /* Allowed security levels                */
      &Dcm_DcmRequestDownload,           /* Service handler    */
      &Dcm_Dsp_RequestDownload_Ini,     /* Init function of service  */
      0x34,                          /* Service id              */
      
      FALSE,                      /* SubFunction Doesn't Exist */
      TRUE,                          /* Service is located within Dcm */

      NULL_PTR,                     /* Pointer to subservice table  */
      0,                                                       /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &Dcm_Prv_DspReqDownloadConfirmation /* Reference Service confirmation Apis */
 },
{

      0x2uL,                               /* Allowed sessions                */

      
      0x2uL,                                /* Allowed security levels                */
      &Dcm_DcmTransferData,              /* Service handler    */
      &Dcm_Dsp_TransferDataIni,         /* Init function of service  */
      0x36,                          /* Service id              */
      
      FALSE,                      /* SubFunction Doesn't Exist */
      TRUE,                          /* Service is located within Dcm */

      NULL_PTR,                     /* Pointer to subservice table  */
      0,                                                       /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &DcmAppl_DcmConfirmation      /* Reference Service confirmation Apis */
 },
{

      0x2uL,                               /* Allowed sessions                */

      
      0x2uL,                                /* Allowed security levels                */
      &Dcm_DcmRequestTransferExit,       /* Service handler    */
      &Dcm_Dsp_RequestTransferExit_Ini,    /* Init function of service  */
      0x37,                          /* Service id              */
      
      FALSE,                      /* SubFunction Doesn't Exist */
      TRUE,                          /* Service is located within Dcm */

      NULL_PTR,                     /* Pointer to subservice table  */
      0,                                                       /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &Dcm_Prv_DspReqTrfExitConfirmation /* Reference Service confirmation Apis */
 },
{

      0x4uL,                               /* Allowed sessions                */

      0xffffffffuL,                  /* Allowed in all security levels                */
      &Dcm_DcmControlDTCSetting,         /* Service handler    */
      &Dcm_Dsp_CDTCSIni,                /* Init function of service  */
      0x85,                          /* Service id              */
      
      TRUE,                             /* SubFunction Exists */
      TRUE,                          /* Service is located within Dcm */

      Dcm_SrvTab0_SubSrv10_acs,      /* Pointer to subservice table */
      2,                             /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &DcmAppl_DcmConfirmation      /* Reference Service confirmation Apis */
 },
{

      0x4uL,                               /* Allowed sessions                */

      0xffffffffuL,                  /* Allowed in all security levels                */
      &Dcm_DcmCommunicationControl,      /* Service handler    */
      &Dcm_Dsp_CCIni,                   /* Init function of service  */
      0x28,                          /* Service id              */
      
      TRUE,                             /* SubFunction Exists */
      TRUE,                          /* Service is located within Dcm */

      Dcm_SrvTab0_SubSrv11_acs,      /* Pointer to subservice table */
      2,                             /* Number of subservices */
      &DcmAppl_UserServiceModeRuleService,                      /* No User specific mode rule function configured */
      &Dcm_Prv_DspCommCntrlConfirmation /* Reference Service confirmation Apis */
 }
};

#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* Sid table configuration */
static const Dcm_Dsld_sid_tableType Dcm_DsldServiceTables_cast[]=
{
    {
      Dcm_Dsld_sid_table0_acs,      /* Pointer to service table */
      12,                           /* No of services in the Service table  */
      0x7F,                          /* NRC for service not supported for active session */
      10 /* Index of the Dcm_DcmControlDTCSetting service in the service table */
    }

};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* This is the array contains the supported sessions in this ECU                             */
/* sessions id are configured here (eg: default session=0x01, programming session=0x02)      */
/* number of sessions in DSC service is same as here                                         */
/* this lookup table is used to calculate the bit mask from session ID                       */
/* these ids are always in ascending order                                                    */

/* The DscmDsp session and security configuration is used for the UDS protocol */
static const uint8 Dcm_DsldSupportedSessions_cau8[]= {0x1,0x2,0x3};

#if (DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
const uint8 Dcm_Dsld_KWPsupported_sessions_acu8[]= {0x86};
#endif

#define DCM_STOP_SEC_CONST_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"






#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
 const Dcm_Dsld_RoeRxToTestSrcMappingType Dcm_Dsld_RoeRxToTestSrcMappingTable[DCM_CFG_NUM_RX_PDUID]=
{
{
    0 , 0
}

,
{
    1 , 0
}


};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/*Mode rule definitions*/
/* Generating corresponding Check API's for each ModeRules */
static boolean Dcm_DcmModeCondition_0(const uint8 *Nrc_u8)
{
            /* Flag to update the result of the check operation */
            boolean RetFlag_b;
             
        
        if(RTE_MODE_Mdg2_Mode1 == Rte_Mode_Rp_Mdg2_MDGP_Mdg2())
        {
           RetFlag_b = TRUE;
        }
        else
        {
           RetFlag_b = FALSE;
        }       
    (void)(Nrc_u8);
    /* Return the updated value of the Flag to indicate the result of the check operation */
    return RetFlag_b;
}
boolean Dcm_DcmModeRule_0(uint8 *Nrc_u8)
{
/*MR12 RULE 13.5 VIOLATION: Right hand operand of '&&' or '||' is an expression with possible side effects. 
Pointer to NRC is passed to each moderule so that NRC can be updated by the first moderule which fails according to 
requirement. No side effects will be caused. MISRA C:2012 Rule-13.5*/
    if(Dcm_DcmModeCondition_0(Nrc_u8))
    {
        *Nrc_u8=0;
        return(TRUE);
    }
    else
    {
        if(*Nrc_u8==0)
        {
            /* One of the ModeCondition or ModeRule referred by this ModeRule has failed but nrc is not yet set to a non-zero value */
            *Nrc_u8 = 1; /* User Specific NR Code */
        }
        return(FALSE);
    }
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/* this is the array contains the supported security level in this ECU                       */
/* security levels are configured here (eg: DCM_SEC_LEV_LOCKED =0x00)                        */
/* number of security levels here is one more than in SECA service                           */
/* this lookup table is used to calculate the bit mask from security level                   */
/* this ids are always in ascending order                                                    */
/* Allowed range of security levels are 0x00,0x01,0x02,.....0x3F                             */
#define DCM_START_SEC_CONST_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static const uint8 Dcm_Dsld_supported_security_acu8[]= {    0x0,    0x9};
#define DCM_STOP_SEC_CONST_8  /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* final structre of DSL-DSD */
const Dcm_Dsld_confType Dcm_Dsld_Conf_cs =
{
   Dcm_Dsld_rx_table_acu8,                 /* Reference to Rx table array */
   Dcm_DsldTxTable_cast,                      /* Reference to Tx table array */
   Dcm_DsldConnTable_cast,                /* Reference to Connection table array */
   Dcm_DsldProtocol_cast,                  /* Reference to protocol table */
   Dcm_DsldServiceTables_cast,            /* Reference to sid table      */
   Dcm_DsldSupportedSessions_cau8,       /* Session look up table       */
   Dcm_Dsld_supported_security_acu8        /* Security look up table      */
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

