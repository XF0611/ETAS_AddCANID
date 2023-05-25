
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Roe_Inf.h"
#endif
#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
#define DCM_RDPI_SID 0x2Au
#endif


#if ((DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS != 0u) || (DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS !=0u))
/***********************************************************************************************************************
 Function name    : Dcm_Dsd_CallRTEConfirmation
 Syntax           : Dcm_Dsd_CallRTEConfirmation(ConfirmationStatus_u8)
 Description      : Function to give the confirmation to Supplier/Manufacturer in case the RTE is enabled
 Parameter        : Dcm_ConfirmationStatusType
 Return value     : void
***********************************************************************************************************************/
void Dcm_Dsd_CallRTEConfirmation(Dcm_ConfirmationStatusType ConfirmationStatus_u8,boolean context)
{
    uint8_least idxIndex_qu8;
    Dcm_MsgContextType MsgContext_st;

#if (DCM_PARALLELPROCESSING_ENABLED != DCM_CFG_OFF)
    if(context == DCM_OBDCONTEXT)
    {
        MsgContext_st = Dcm_OBDMsgContext_st;
    }
    else
#endif
    {
        MsgContext_st = Dcm_DsldMsgContext_st;
    }
#if(DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS != 0u)
    for(idxIndex_qu8=0x00u; idxIndex_qu8<DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS; idxIndex_qu8++)
    {
        (void)(*Dcm_Rte_ManufacturerConfirmation_a[idxIndex_qu8])
                (MsgContext_st.idContext,MsgContext_st.msgAddInfo.reqType,
                Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[MsgContext_st.dcmRxPduId]].testaddr_u16,
                ConfirmationStatus_u8);
    }
#endif
#if(DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS !=0u)
    for(idxIndex_qu8=0x00u; idxIndex_qu8<DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS; idxIndex_qu8++)
    {
        (void)(*Dcm_Rte_SupplierConfirmation_a[idxIndex_qu8])
                (MsgContext_st.idContext,MsgContext_st.msgAddInfo.reqType,
                Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[MsgContext_st.dcmRxPduId]].testaddr_u16,
                ConfirmationStatus_u8);
    }
#endif
#if (DCM_PARALLELPROCESSING_ENABLED == DCM_CFG_OFF)
    (void)context;
#endif
}
#endif

/***********************************************************************************************************************
 Function name    : Dsd_Prv_TesterSourceConfirmation
 Syntax           : Dsd_Prv_TesterSourceConfirmation(ConfirmationStatus_u8)
 Description      : Give confirmation to application for Tester triggered requests
 Parameter        : Dcm_ConfirmationStatusType
 Return value     : void
***********************************************************************************************************************/
LOCAL_INLINE void Dsd_Prv_TesterSourceConfirmation(Dcm_ConfirmationStatusType ConfirmationStatus_u8)
{
    /* Check if the response is given by service or not */
    if (Dcm_DsldGlobal_st.dataResponseByDsd_b == FALSE)
    {
        if (Dcm_DsldSrvTable_pcst[Dcm_DsldGlobal_st.idxService_u8].servicelocator_b != FALSE)
        {
            /* Service exists in DSP. Give the confirmation to DSP */
            Dcm_DspConfirmation(Dcm_DsldMsgContext_st.idContext, Dcm_DsldMsgContext_st.dcmRxPduId,
                    Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_DsldMsgContext_st.dcmRxPduId]].testaddr_u16,
                    ConfirmationStatus_u8);
        }
        else
        {
            /* Service exists out side the DSP. Give the confirmation to Application */
            DcmAppl_DcmConfirmation(Dcm_DsldMsgContext_st.idContext, Dcm_DsldMsgContext_st.dcmRxPduId,
                    Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_DsldMsgContext_st.dcmRxPduId]].testaddr_u16,
                    ConfirmationStatus_u8);
        }
    }
    else
    {
#if(DCM_CFG_NR_CONF_ENABLED != DCM_CFG_OFF)

        if ((ConfirmationStatus_u8 == DCM_RES_NEG_OK) || (ConfirmationStatus_u8 == DCM_RES_NEG_NOT_OK))
        {
            /* Negative response created inside DSD. Give separate confirmation to application. */
            DcmAppl_DcmConfirmation_DcmNegResp(Dcm_DsldMsgContext_st.idContext,
                    Dcm_DsldMsgContext_st.dcmRxPduId,ConfirmationStatus_u8);
        }
#endif
        /*Check if the max number of configured NRC 0x78 is transmitted*/
        if (Dcm_isGeneralRejectSent_b != FALSE)
        {
            DcmAppl_DcmConfirmation_GeneralReject(Dcm_DsldMsgContext_st.idContext, Dcm_DsldMsgContext_st.dcmRxPduId,
                    ConfirmationStatus_u8);
            /*Set the Dcm_DsldGenRejectSent flag is to False*/
            Dcm_isGeneralRejectSent_b = FALSE;
        }
        if ((ConfirmationStatus_u8 == DCM_RES_POS_OK) || (ConfirmationStatus_u8 == DCM_RES_POS_NOT_OK))
        {
            /* Service exist out side the DSP. Give the confirmation to Application */
            DcmAppl_DcmConfirmation(Dcm_DsldMsgContext_st.idContext, Dcm_DsldMsgContext_st.dcmRxPduId,
                    Dcm_DsldConnTable_pcst[Dcm_DsldRxTable_pcu8[Dcm_DsldMsgContext_st.dcmRxPduId]].testaddr_u16,
                    ConfirmationStatus_u8);
        }
    }
#if ((DCM_CFG_MANUFACTURERNOTIFICATION_NUM_PORTS != 0u) || (DCM_CFG_SUPPLIERNOTIFICATION_NUM_PORTS !=0u))
    Dcm_Dsd_CallRTEConfirmation(ConfirmationStatus_u8,DCM_UDSCONTEXT);
#endif
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DsdSendTxConfirmation
 Syntax           : Dcm_Prv_DsdSendTxConfirmation()
 Description      : Function to Give confirmation to application after response has been
                    transmitted and confirmation is obtained from lower layer.
 Parameter        : void
 Return value     : void
***********************************************************************************************************************/
void Dcm_Prv_DsdSendTxConfirmation(void)
{
    /* Update based on the Type of response sent and whether the response was sent successfully */
    Dcm_ConfirmationStatusType ConfirmationStatus_u8 =
            (Dcm_DsldGlobal_st.dataResult_u8 == E_OK) ?
                ((Dcm_DsldGlobal_st.stResponseType_en == DCM_DSLD_POS_RESPONSE)?DCM_RES_POS_OK:DCM_RES_NEG_OK) :
                ((Dcm_DsldGlobal_st.stResponseType_en == DCM_DSLD_POS_RESPONSE)?DCM_RES_POS_NOT_OK:DCM_RES_NEG_NOT_OK);

#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
    /*Check if paged buffer flag is active*/
    if (Dcm_DsldGlobal_st.flgPagedBufferTxOn_b != FALSE)
    {
        /* Full response is sent on Paged buffer *
         *Set the paged buffer flag to False */
        Dcm_DsldGlobal_st.flgPagedBufferTxOn_b = FALSE;
        if (ConfirmationStatus_u8 == DCM_RES_POS_NOT_OK)
        {
            /* Invoke callback function in case the paged buffer transmission is unsuccessful
             * since the service has not yet finished the processing */
            DcmAppl_DcmCancelPagedBufferProcessing(Dcm_DsldGlobal_st.dataSid_u8);
        }
    }
#endif

    if(Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest == DCM_UDS_TESTER_SOURCE)
    {
        Dsd_Prv_TesterSourceConfirmation(ConfirmationStatus_u8);
    }
#if(DCM_ROE_ENABLED != DCM_CFG_OFF)
    else if(Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest == DCM_ROE_SOURCE)
    {
        Dcm_Prv_ROEResetOnConfirmation(); /* Reset on ROE confirmation */
        /* Give the confirmation to application */
        DcmAppl_DcmConfirmation(Dcm_DsldMsgContext_st.idContext,Dcm_DsldMsgContext_st.dcmRxPduId,
        Dcm_Dsld_RoeRxToTestSrcMappingTable[Dcm_DsldMsgContext_st.dcmRxPduId].testsrcaddr_u16,ConfirmationStatus_u8);
    }
#endif
#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
    else if(Dcm_DsldMsgContext_st.msgAddInfo.sourceofRequest == DCM_RDPI_SOURCE)
    {
        /* Give the confirmation to application */
        DcmAppl_DcmConfirmationRDPI(DCM_RDPI_SID,Dcm_DsldMsgContext_st.dcmRxPduId,ConfirmationStatus_u8);
    }
#endif
    else
    {
        /* Do - nothing */
    }
    /* Change the DSD state to Idle state */
    Dcm_Prv_SetDsdState(DSD_IDLE);
}

/***********************************************************************************************************************
 Function name    : Dcm_Dsd_ServiceIni
 Syntax           : Dcm_Dsd_ServiceIni(idxSrvTable_u8)
 Description      : Calling the Initialisations of all services in the active service table
 Parameter        : uint8
 Return value     : void
***********************************************************************************************************************/
void Dcm_Dsd_ServiceIni(uint8 ServiceTableIndex_u8)
{

    uint8 idxIndex_u8;
    uint8 NumberOfServices_u8 = Dcm_Dsld_Conf_cs.sid_table_pcs[ServiceTableIndex_u8].num_services_u8;
    /* Pointer to active service table */
    const Dcm_Dsld_ServiceType * adrSrvTable_pcst =
                          Dcm_Dsld_Conf_cs.sid_table_pcs[ServiceTableIndex_u8].ptr_service_table_pcs;

    /* call the initialisations of all services in the service table */
    for(idxIndex_u8 = NumberOfServices_u8; idxIndex_u8 != 0x00u; idxIndex_u8--)
    {
        if(adrSrvTable_pcst->Service_init_fp != NULL_PTR )
        {
            (*adrSrvTable_pcst->Service_init_fp)();
        }
        adrSrvTable_pcst++;
    }
}
