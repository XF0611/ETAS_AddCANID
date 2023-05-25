/* BASDKey_start */

/* BASDKey_end */
/* Administrative Information (automatically filled in)
 * $Domain____:BASD$
 * $Namespace_:\Comp\Dcm$
 * $Class_____:C$
 * $Name______:DcmDspUds_Iocbi$
 * $Variant___:AR40.11.0.0$
 * $Revision__:1$
 **********************************************************************************************************************
 </BASDKey>*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "DcmDspUds_Iocbi_Inf.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF)
#include "DcmDspUds_Iocbi_Priv.h"

/*
 **********************************************************************************************************************
 * Globals
 **********************************************************************************************************************
 */
/* Index of the requested DID in the array Dcm_DspUDS_IOCBI_DIDs_a */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* State machine control variable for IOCBI service */
static Dcm_DspIOCBIStates_ten Dcm_stDspIocbiState_en;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint8 Dcm_dataDspIocbiCtrlParam_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_OpStatusType Dcm_stDspIocbiOpStatus_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_DIDIndexType_tst s_Dcm_idxIocbiDidIndexType_st; /*Store the index of DID of either Dcm_DidConfig or Dcm_DIDRangeCOnfig structure*/
static uint16 s_ActiveDid_u16;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_ReadSignalLength_u16;
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static boolean s_IsIOCBISubfuncCalled_b;
#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)
static boolean s_IocbiRteCallPlaced_b; /* Variable to hold whether RTE call is placed for async client server*/
#endif
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
static boolean s_DcmReadLengthRteCallPlaced_b; /* Variable to indicate whether Rte_Call API is invoked for ReadLength call */
#endif
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **********************************************************************************************************************
 Array for storing Active IOCBI DIDs and their active status
 **********************************************************************************************************************
 **/
#if(DCM_CFG_NUM_IOCBI_DIDS != 0x0)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_Dsp_IocbiStatusType_tst DcmDsp_IocbiStatus_array[DCM_CFG_NUM_IOCBI_DIDS];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
/* Length of Control option record */

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **********************************************************************************************************************
 * Dcm_Dsp_IOCBI_Ini :
 *  This function initializes the Input Output Control by Identifier Service to DCM_IOCBI_IDLE.
 *  This function has to be called by DSLDSD during a protocol start and before any service request.
 *
 * \param           :None
 * \return          :None
 * \retval          :Not Applicable
 * \seealso
 * \usedresources
 **********************************************************************************************************************
 */

void Dcm_Dsp_IOCBI_Ini(void)
{
    Dcm_NegativeResponseCodeType dataNegResCode_u8; /* Negative response code indicator */
    Std_ReturnType dataRetIocbiFunc_u8;
    uint16_least idxIocbiIndex_u16; /*Index to loop through the IOCBI status array DcmDsp_IocbiStatus_array*/
    const Dcm_DIDConfig_tst * ptrDidConfig;
    const Dcm_DataInfoConfig_tst * ptrSigConfig;
    const Dcm_SignalDIDSubStructConfig_tst * ptrIOSigConfig;
    uint16_least idxDidSignal_u16;
#if(DCM_CFG_DSP_CONTROL_ASYNCH_FNC_ENABLED == DCM_CFG_ON)
    const Dcm_ExtendedDIDConfig_tst * ptrDidExtendedConfig;
#if(DCM_CFG_DSP_SHORTTERMADJUSTMENT_ENABLED == DCM_CFG_ON)
    un_TermAdjstmnt ShortTermAdjustment_un; /*ShortTermAdjustment function pointers union*/
    ShortTermAdjustment_un.ShortTermAdjustment1_pfct = NULL_PTR;
#endif
#if(DCM_CFG_DSP_FREEZECURRENTSTATE_ENABLED == DCM_CFG_ON)
    un_FreezeState  FreezeCurrentState_un;  /*FreezeCurrentState function pointers union*/
    FreezeCurrentState_un.FreezeCurrentState1_pfct = NULL_PTR;
#endif
#if(DCM_CFG_DSP_RESETTODEFAULT_ENABLED == DCM_CFG_ON)
    un_ResetDefault ResetToDefault_un;      /*ResetToDefault function pointers union*/
    ResetToDefault_un.ResetToDefault1_pfct = NULL_PTR;
#endif
#endif

#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
    uint8 ControlMask_u8;
    uint16 ControlMask_u16;
    uint32 ControlMask_u32;
    ControlMask_u8 = 0;
    ControlMask_u16 = 0;
    ControlMask_u32 = 0;
#endif

    idxDidSignal_u16 = 0;

    /*Initialising local variables*/
    dataNegResCode_u8 = 0x00;
    dataRetIocbiFunc_u8 = 0x00;
    ptrDidConfig = NULL_PTR;
    ptrSigConfig = NULL_PTR;
    ptrIOSigConfig = NULL_PTR;


    /* To call cancellation of SWCD operation in case API was called with Opstatus as DCM_PENDING */
#if(DCM_CFG_DSP_CONTROL_ASYNCH_FNC_ENABLED == DCM_CFG_ON)
    if (Dcm_stDspIocbiOpStatus_u8 == DCM_PENDING)
    {
        ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16];
        ptrSigConfig = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
        ptrIOSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];
        ptrDidExtendedConfig = ptrDidConfig->adrExtendedConfig_pcst;

        s_ActiveDid_u16 = ptrDidConfig->dataDid_u16;
        if ((ptrSigConfig->idxDcmDspControlInfo_u16 > 0) && (ptrIOSigConfig->idxDcmDspIocbiInfo_u16 > 0))
        {
            /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
            s_IsIOCBISubfuncCalled_b = TRUE;
#if(DCM_CFG_DSP_SHORTTERMADJUSTMENT_ENABLED == DCM_CFG_ON)
            if((Dcm_dataDspIocbiCtrlParam_u8 == DCM_IOCBI_SHORTTERMADJUSTMENT) && ((ptrDidConfig->adrExtendedConfig_pcst->statusmaskIOControl_u8 & 0x08u)==0x08u))
            {
                ShortTermAdjustment_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrShortTermAdjustment_cpv;
                /* Check if the Control function is configured */
                if((ShortTermAdjustment_un.ShortTermAdjustment1_pfct != NULL_PTR) && ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC)))
                {
#if(DCM_CFG_DSP_IOCBI_ASP_ENABLED == DCM_CFG_ON)
                    if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                    {
                        if (ptrDidExtendedConfig->dataCtrlMask_en != DCM_CONTROLMASK_EXTERNAL)
                        {
                            /* Short Term Adjustment was requested */
                            /* Call the Rte_Call API with In and Inout parameter to cancel the ongoing operation */
                            dataRetIocbiFunc_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment9_pfct)) (NULL_PTR,DCM_CANCEL);
                        }
                        else
                        {
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
                            /*add with control mask*/
                            if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment10_pfct)) (NULL_PTR,DCM_CANCEL, ControlMask_u8);
                            }
                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment11_pfct)) (NULL_PTR,DCM_CANCEL, ControlMask_u16);
                            }
                            else
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment12_pfct)) (NULL_PTR,DCM_CANCEL, ControlMask_u32);
                            }
#endif
                        }
                    }
                    else
#endif
                    {
                        if (ptrDidExtendedConfig->dataCtrlMask_en != DCM_CONTROLMASK_EXTERNAL)
                        {
                            /* Short Term Adjustment was requested */
                            /* Call the configured API  with Opstatus as DCM_CANCEL */
                            dataRetIocbiFunc_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment2_pfct))  (NULL_PTR,DCM_CANCEL, &dataNegResCode_u8);
                        }
                        else
                        {
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
                            /*add with control mask*/
                            if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment6_pfct)) (NULL_PTR,DCM_CANCEL, ControlMask_u8,&dataNegResCode_u8);
                            }
                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment7_pfct)) (NULL_PTR,DCM_CANCEL, ControlMask_u16,&dataNegResCode_u8);
                            }
                            else
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment8_pfct)) (NULL_PTR,DCM_CANCEL, ControlMask_u32,&dataNegResCode_u8);
                            }
#endif
                        }
                    }
                }
            }
#endif
#if(DCM_CFG_DSP_FREEZECURRENTSTATE_ENABLED == DCM_CFG_ON)
            if((Dcm_dataDspIocbiCtrlParam_u8 == DCM_IOCBI_FREEZECURRENTSTATE) && ((ptrDidConfig->adrExtendedConfig_pcst->statusmaskIOControl_u8 & 0x04u)==0x04u ))
            {
                FreezeCurrentState_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrFreezeCurrentState_cpv;
                /* Check if the Control function is configured */
                if((FreezeCurrentState_un.FreezeCurrentState1_pfct != NULL_PTR) &&  ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC)))
                {
#if(DCM_CFG_DSP_IOCBI_ASP_ENABLED == DCM_CFG_ON)
                    if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                    {
                        if (ptrDidExtendedConfig->dataCtrlMask_en != DCM_CONTROLMASK_EXTERNAL)
                        {
                            /* Freeze current state was requested */
                            /* Invoke the Generated Rte_Call API to cancel the pending operation */
                            dataRetIocbiFunc_u8=(*(FreezeCurrentState_un.FreezeCurrentState9_pfct)) (DCM_CANCEL);
                        }
                        else
                        {
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
                            /*add with control mask*/
                            if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                            {
                                /* Freeze current state was requested */
                                /* Invoke the Generated Rte_Call API to cancel the pending operation */
                                dataRetIocbiFunc_u8=(*(FreezeCurrentState_un.FreezeCurrentState10_pfct)) (DCM_CANCEL, ControlMask_u8);
                            }
                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                            {
                                /* Freeze current state was requested */
                                /* Invoke the Generated Rte_Call API to cancel the pending operation */
                                dataRetIocbiFunc_u8=(*(FreezeCurrentState_un.FreezeCurrentState11_pfct)) (DCM_CANCEL, ControlMask_u16);
                            }
                            else
                            {
                                /* Freeze current state was requested */
                                /* Invoke the Generated Rte_Call API to cancel the pending operation */
                                dataRetIocbiFunc_u8=(*(FreezeCurrentState_un.FreezeCurrentState12_pfct)) (DCM_CANCEL, ControlMask_u32);
                            }
#endif
                        }
                    }
                    else
#endif
                    {
                        if (ptrDidExtendedConfig->dataCtrlMask_en != DCM_CONTROLMASK_EXTERNAL)
                        {
                            /* Freeze current state was requested */
                            /* Call the configured API  with Opstatus as DCM_CANCEL */
                            dataRetIocbiFunc_u8=(*(FreezeCurrentState_un.FreezeCurrentState2_pfct)) (DCM_CANCEL, &dataNegResCode_u8);
                        }
                        else
                        {
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
                            /*add with control mask*/
                            if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(FreezeCurrentState_un.FreezeCurrentState6_pfct)) (DCM_CANCEL, ControlMask_u8,&dataNegResCode_u8);
                            }
                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(FreezeCurrentState_un.FreezeCurrentState7_pfct)) (DCM_CANCEL, ControlMask_u16,&dataNegResCode_u8);
                            }
                            else
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(FreezeCurrentState_un.FreezeCurrentState8_pfct)) (DCM_CANCEL, ControlMask_u32,&dataNegResCode_u8);
                            }
#endif
                        }
                    }
                }
            }
#endif
#if(DCM_CFG_DSP_RESETTODEFAULT_ENABLED == DCM_CFG_ON)
            if((Dcm_dataDspIocbiCtrlParam_u8 == DCM_IOCBI_RESETTODEFAULT) && ((ptrDidConfig->adrExtendedConfig_pcst->statusmaskIOControl_u8 & 0x02u)==0x02u ))
            {
                ResetToDefault_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrResetToDefault_cpv;
                /* Check if the Control function is configured */
                if((ResetToDefault_un.ResetToDefault1_pfct != NULL_PTR)&&((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC)))
                {
#if(DCM_CFG_DSP_IOCBI_ASP_ENABLED == DCM_CFG_ON)
                    if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                    {
                        if (ptrDidExtendedConfig->dataCtrlMask_en != DCM_CONTROLMASK_EXTERNAL)
                        {
                            /* Reset to Default value was requested */
                            /* Call the generated Rte_call  with Opstatus as DCM_CANCEL */
                            dataRetIocbiFunc_u8 = (*(ResetToDefault_un.ResetToDefault9_pfct))(DCM_CANCEL);
                        }
                        else
                        {
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
                            /*add with control mask*/
                            if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                            {
                                /* Reset to Default value was requested */
                                /* Call the generated Rte_call  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ResetToDefault_un.ResetToDefault10_pfct)) (DCM_CANCEL, ControlMask_u8);
                            }
                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                            {
                                /* Reset to Default value was requested */
                                /* Call the generated Rte_call  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ResetToDefault_un.ResetToDefault11_pfct)) (DCM_CANCEL, ControlMask_u16);
                            }
                            else
                            {
                                /* Reset to Default value was requested */
                                /* Call the generated Rte_call  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ResetToDefault_un.ResetToDefault12_pfct)) (DCM_CANCEL, ControlMask_u32);
                            }
#endif
                        }
                    }
                    else
#endif
                {
                    if (ptrDidExtendedConfig->dataCtrlMask_en != DCM_CONTROLMASK_EXTERNAL)
                    {
                        /* Reset to Default value was requested */
                        /* Call the configured API  with Opstatus as DCM_CANCEL */
                        dataRetIocbiFunc_u8 = (*(ResetToDefault_un.ResetToDefault2_pfct))(DCM_CANCEL, &dataNegResCode_u8);
                    }
                    else
                    {
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
                        /*add with control mask*/
                        if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                        {
                            /* Short Term Adjustment was requested */
                            /* Call the configured API  with Opstatus as DCM_CANCEL */
                            dataRetIocbiFunc_u8=(*(ResetToDefault_un.ResetToDefault6_pfct)) (DCM_CANCEL, ControlMask_u8,&dataNegResCode_u8);
                        }
                        else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                        {
                            /* Short Term Adjustment was requested */
                            /* Call the configured API  with Opstatus as DCM_CANCEL */
                            dataRetIocbiFunc_u8=(*(ResetToDefault_un.ResetToDefault7_pfct)) (DCM_CANCEL, ControlMask_u16,&dataNegResCode_u8);
                        }
                        else
                        {
                            /* Short Term Adjustment was requested */
                            /* Call the configured API  with Opstatus as DCM_CANCEL */
                            dataRetIocbiFunc_u8=(*(ResetToDefault_un.ResetToDefault8_pfct)) (DCM_CANCEL, ControlMask_u32,&dataNegResCode_u8);
                        }
#endif
                    }
                }
            }
            }
#endif
            /*subfunction called is not valid and reset to FALSE*/
            s_IsIOCBISubfuncCalled_b = FALSE;
            (void) dataRetIocbiFunc_u8;
        }
    }
#endif

    /*Loop through all the IOCBI DIDs through DcmDsp_IocbiStatus_array, this case is mainly to handle the issue of protocol preemption,
    to reset all the IOCTRLs which was active in the previos protocol before preemption*/
    for(idxIocbiIndex_u16=0u;idxIocbiIndex_u16<DCM_CFG_NUM_IOCBI_DIDS;idxIocbiIndex_u16++)
    {
        ptrDidConfig =&Dcm_DIDConfig[DcmDsp_IocbiStatus_array[idxIocbiIndex_u16].idxindex_u16];
        s_ActiveDid_u16 = ptrDidConfig->dataDid_u16 ;
    }
    if (s_Dcm_idxIocbiDidIndexType_st.dataopstatus_b == DCM_PENDING)
    {
        ptrSigConfig = &Dcm_DspDataInfo_st[Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16].adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
        /* If the read operation is Asynchronous */
        if((ptrSigConfig->adrReadFnc_cpv.ReadFunc2_ptr!=NULL_PTR) && ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)|| (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC)))
        {
            /* Call the Read function with  Opstatus as DCM_CANCEL */
            /*subfunction called is valid and set to TRUE*/
            s_IsIOCBISubfuncCalled_b = TRUE;
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
			if ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
			{
				/* Call the Rte_Call Read Function ot cancel the operation */
			    dataRetIocbiFunc_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc11_ptr))(DCM_CANCEL);
			}
			else
#endif
			{
	            /* Call the Read function with  Opstatus as DCM_CANCEL */
	            dataRetIocbiFunc_u8 = (*(ptrSigConfig->adrReadFnc_cpv.ReadFunc2_ptr))(DCM_CANCEL, NULL_PTR);
			}
				/*subfunction called is not valid and set to FALSE*/
            s_IsIOCBISubfuncCalled_b = FALSE;
            (void) dataRetIocbiFunc_u8;
        }
    }

    /* Initialize IOCBI Service state machine to IDLE state */
    Dcm_stDspIocbiState_en = DCM_IOCBI_IDLE;
    Dcm_DidSignalIdx_u16 = 0x0;
    Dcm_stDspIocbiOpStatus_u8 = DCM_INITIAL;
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
		s_DcmReadLengthRteCallPlaced_b = FALSE;
#endif
#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)
	s_IocbiRteCallPlaced_b = FALSE;
#endif
    Dcm_ResetDIDIndexstruct(&s_Dcm_idxIocbiDidIndexType_st); /*This function is invoked to reset all the elements of DID index structure to its default value*/
    DCM_UNUSED_PARAM(dataNegResCode_u8);
    DCM_UNUSED_PARAM(idxIocbiIndex_u16);
    DCM_UNUSED_PARAM(Dcm_dataDspIocbiCtrlParam_u8);
    DCM_UNUSED_PARAM(ptrDidConfig);
    DCM_UNUSED_PARAM(ptrIOSigConfig);
    DCM_UNUSED_PARAM(ptrSigConfig);
    DCM_UNUSED_PARAM(idxDidSignal_u16);
}

/**
 **********************************************************************************************************************
 * Dcm_DcmInputOutputControlbyIdentifier :
 *  Service Interpreter for Input Output Control by Identifier (0x2F) service.
 *  The InputOutputControlByIdentifier service is used by the client to substitute a value for an input signal,
 *  internal server function and/or force control to a value for an output (actuator) of an electronic system.
 *  There are four ways in which tester can make use of this service by sending the various values for
 *  Input Output Control Parameter in the request:
 *  0x00:   Return Control To ECU. This value shall indicate to the server that the client does no longer have
 *          control about the input signal(s), internal parameter(s) and/or output signal(s) referenced by the
 *          dataIdentifier.
 *  0x01:   This value shall indicate to the server that it is requested to reset the input signal(s), internal
 *          parameter(s) and/or output signal(s) referenced by the dataIdentifier to its default state.
 *  0x02:   This value shall indicate to the server that it is requested to freeze the current state of the
 *          input signal(s), internal parameter(s) and/or output signal referenced by the dataIdentifier.
 *  0x03:   This value shall indicate to the server that it is requested to adjust the input signal(s), internal
 *          parameter(s) and/or controlled output signal(s) referenced by the dataIdentifier in RAM to the value(s)
 *          included in the controlOption parameter(s) (e.g., set Idle Air Control Valve to a specific step number,
 *          set pulse width of valve to a specific value/duty cycle).
 *
 * \param   pMsgContext    Pointer to message structure
 *                                 (parameter in : RequestLength, Response buffer size, request bytes)
 *                                 (parameter out: Response bytes and Response length )
 *
 * \retval  None
 * \seealso
 *
 **********************************************************************************************************************
 */

Std_ReturnType Dcm_DcmInputOutputControlByIdentifier(Dcm_SrvOpStatusType OpStatus,Dcm_MsgContextType * pMsgContext,Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    uint32 dataLength_u32; /* Local variable of store length of DID */
    const Dcm_DIDConfig_tst * ptrDidConfig;
    const Dcm_DataInfoConfig_tst * ptrSigConfig;
    const Dcm_SignalDIDSubStructConfig_tst * ptrIOSigConfig;
    const Dcm_SignalDIDIocbiConfig_tst * ptrIOCBIsigConfig;
    const Dcm_ExtendedDIDConfig_tst * ptrDidExtendedConfig;
    Dcm_Dsp_IocbiStatusType_tst * ptrIOCBIStatusArrayConfig;
    un_RetCntrlEcu  ReturnControlToEcu_un;   /*ReturnControlToEcu Function Pointers union*/
    uint16 dataSignalLength_u16; /* Variable to hold the length of the signal*/
    uint16 posnSigByte_u16;/* Signal Byte position*/
    uint16 dataControlMaskLen_u16;
    uint16 dataCtlMaskOffset_u16;/* Offset for control mask*/
    uint16 nrDID_u16; /* Data Identifier */
    uint16_least idxIocbiIndex_u16; /*Index to loop through the IOCBI status array DcmDsp_IocbiStatus_array*/
    Std_ReturnType dataIocbiExeResult_u8; /* Return value of IOCBI execution function */
    Std_ReturnType dataRetGetDid_u8;
    Std_ReturnType dataValidateIoMaskStatus_u8; /* Return value for IOCBI Mask and Status Validation Appl */
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
//BIP Porting tan9sgh #if((DCM_CFG_DSP_CONTROL_SYNCH_FNC_ENABLED   != DCM_CFG_OFF) || (DCM_CFG_DSP_CONTROL_ASYNCH_FNC_ENABLED   != DCM_CFG_OFF) )
    uint8 ControlMask_u8; /* Control Mask */
    uint16 ControlMask_u16; /* Control Mask */
    uint32 ControlMask_u32; /* Control Mask */
//tan9sgh #endif
#endif
    uint8 dataIoCtrlParam_u8; /* Input Output Control parameter */
    uint8 posnSigBit_u8; /* Signal Bit position*/
    uint8 posnCtlMaskByte_u8;/* Control mask Byte position*/
    uint8 posnCtlMaskBit_u8; /* Control mask Bit position*/
    boolean isModeChkRetval_b; /*Return value of mode rule check API*/
    boolean flgProcessReq_b; /*Flag process request*/
    Dcm_NegativeResponseCodeType dataNrc_u8; /* Negative response code indicator */
    Std_ReturnType dataretVal_u8;
    Std_ReturnType dataFuncRetVal_u8; /* Local variable to store function return value */
    Std_ReturnType dataServretVal_u8;/* Local variable to store service return value */

    /* Initialization of local variables */
    *dataNegRespCode_u8 = 0x00;
    dataNrc_u8 = 0x00;
    dataIocbiExeResult_u8 = E_OK;
#if(DCM_CFG_DSP_SHORTTERMADJUSTMENT_ENABLED  == DCM_CFG_ON)
    un_TermAdjstmnt ShortTermAdjustment_un;  /*ShortTermAdjustment function pointers union*/
    ShortTermAdjustment_un.ShortTermAdjustment1_pfct = NULL_PTR;
#endif
#if(DCM_CFG_DSP_FREEZECURRENTSTATE_ENABLED == DCM_CFG_ON)
    un_FreezeState  FreezeCurrentState_un;   /*FreezeCurrentState function pointers union*/
    FreezeCurrentState_un.FreezeCurrentState1_pfct = NULL_PTR;
#endif
#if(DCM_CFG_DSP_FREEZECURRENTSTATE_ENABLED == DCM_CFG_ON)&&(DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)
    un_FCS_Result   FreezeCurrentStateResults_un;  /*FreezeCurrentState Results Function Pointer union*/
    FreezeCurrentStateResults_un.FreezeCurrentState13_pfct = NULL_PTR;
#endif
#if(DCM_CFG_DSP_RESETTODEFAULT_ENABLED == DCM_CFG_ON)
    un_ResetDefault ResetToDefault_un;       /*ResetToDefault function pointers union*/
    ResetToDefault_un.ResetToDefault1_pfct = NULL_PTR;
#endif

#if( DCM_CFG_DSP_IOCBI_SR_ENABLED != DCM_CFG_OFF)
    un_IOCntrlReqst ControlRequest_un;       /*ControlRequest function pointers union*/
    ControlRequest_un.IOControlrequest_pfct = NULL_PTR;
#endif
#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_CONTROL_ASYNCH_FNC_ENABLED == DCM_CFG_ON)
    un_RTD_Result   ResetToDefaultResult_un; /*ResetToDefaultResult function pointers union*/
    un_STA_Result   ShortTermAdjustmentResult_un; /*ShortTermAdjustment Results Function Pointers union*/
    /* Initialization of local variables */
    ResetToDefaultResult_un.ResetToDefault13_pfct = NULL_PTR;
    ShortTermAdjustmentResult_un.ShortTermAdjustment13_pfct = NULL_PTR;

#endif
    ReturnControlToEcu_un.ReturnControlEcu1_pfct = NULL_PTR;
    /*Initialize the Process request flag to value TRUE*/
    flgProcessReq_b = TRUE;
    dataFuncRetVal_u8 = E_OK;
    dataServretVal_u8=DCM_E_PENDING;
    idxIocbiIndex_u16=0;

    /* If OpStatus is set to DCM_CANCEL then call the ini function for resetting */
    if (OpStatus == DCM_CANCEL)
    {
        /* Call the Ini Function */
        Dcm_Dsp_IOCBI_Ini();
        /* Set the return value to E_OK as the Ini function will always be serviced  */
        dataServretVal_u8 = E_OK;
    }
    else
    {
        /* Process the request, Check for the state machine */
        switch (Dcm_stDspIocbiState_en)
        {
            case DCM_IOCBI_IDLE:
            { /* Validate the request, check for the minimum request length first */
                if (pMsgContext->reqDataLen >= DSP_IOCBI_MINREQLEN)
                { /* Check if it is a valid Input Output Control parameter byte */
                    if (pMsgContext->reqData[2] <= 3)
                    { /* Get the DID from the request */
                        nrDID_u16 = (uint16)(DSP_CONV_2U8_TO_U16 (pMsgContext->reqData[0], pMsgContext->reqData[1]));
                        /* Check if DID is configured */
                        if (Dcm_Prv_GetIndexOfDID (nrDID_u16,&s_Dcm_idxIocbiDidIndexType_st) == E_OK)
                        {
#if(DCM_CFG_DSP_NUMISDIDAVAIL>0)
                            /* Check if the DID is supported in current variant */
                            /*If the data range of Did is not range did and supported in current variant */
                            if((s_Dcm_idxIocbiDidIndexType_st.dataRange_b==FALSE) && (*Dcm_DIDIsAvail[Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16].idxDIDSupportedFnc_u16] != NULL_PTR))
                            {
                                if((*(IsDIDAvailFnc_pf)(Dcm_DIDIsAvail[Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16].idxDIDSupportedFnc_u16]))(nrDID_u16)!=E_OK)
                                {
                                    /* DID is not supported in current variant */
                                    *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                }
                            }
#endif
                            Dcm_DidSignalIdx_u16 = 0x0;
                            Dcm_ReadSignalLength_u16 = 0x0;
                            /*Loop through the complete list of IOCBI Dids and check if the status of the index matches with any in the status array*/
                            for(idxIocbiIndex_u16=0u;((idxIocbiIndex_u16<DCM_CFG_NUM_IOCBI_DIDS) && (*dataNegRespCode_u8 ==0x00u ));idxIocbiIndex_u16++)
                            {
                                if(s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16 == DcmDsp_IocbiStatus_array[idxIocbiIndex_u16].idxindex_u16)
                                {
                                    ptrDidConfig = &Dcm_DIDConfig[DcmDsp_IocbiStatus_array[idxIocbiIndex_u16].idxindex_u16];
                                    s_ActiveDid_u16 = ptrDidConfig->dataDid_u16 ;
                                }
                            }
                            /*If Process request flag is set to TRUE and negative response code is set to 0x00 */
                            if((flgProcessReq_b!= FALSE)&&(*dataNegRespCode_u8==0x00u))
                            {
                                Dcm_stDspIocbiState_en = DCM_IOCBI_CHKSUPPORT;
                            }
                        }
                        else
                        { /* DID is not configured */
                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                        }
                    }
                    else
                    { /* Invalid IO control parameter */
                        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                        /* Report development error "DCM_E_INVALID_CONTROL_PARAM "to DET module if the DET module is enabled */
                        DCM_DET_ERROR(DCM_IOCBI_ID , DCM_E_INVALID_CONTROL_PARAM)
                    }
                }
                else
                { /* Minimum number of bytes are not received */
                    *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                }
                if(*dataNegRespCode_u8 != 0x0)
                {
                    break;
                }
            }
            /* MR12 RULE 16.3 VIOLATION: Controlling expression is not and does not end with a 'jump' statement. Execution will fall through. MISRA C:2012 Rule-16.3 */
            case DCM_IOCBI_CHKSUPPORT:
            {
                ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16];
                /* Check if this DID is allowed in the current active session*/
                if ((Dcm_DsldGetActiveSessionMask_u32()& (ptrDidConfig->adrExtendedConfig_pcst->dataSessBitMask_u32))!= 0x00uL)
                {
                    /* Get the DID from the request */
                    nrDID_u16 = (uint16)(DSP_CONV_2U8_TO_U16 (pMsgContext->reqData[0], pMsgContext->reqData[1]));
                    while((Dcm_DidSignalIdx_u16<ptrDidConfig->nrSig_u16) && (*dataNegRespCode_u8==0u))
                    {
                        ptrSigConfig = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
                        ptrIOSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];
                        ptrIOCBIsigConfig = &Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16];

                        /*Check if the DID is supported for IOCTRL or not by comparing against the structure index*/
                        if((ptrSigConfig->idxDcmDspControlInfo_u16 >0u) && (ptrIOSigConfig->idxDcmDspIocbiInfo_u16>0u))
                        {
                            /* Check if short term adjustment is requested */
                            /*TRACE[SWS_Dcm_00563]*/
                            if((pMsgContext->reqData[2] == DCM_IOCBI_SHORTTERMADJUSTMENT) && ((ptrDidConfig->adrExtendedConfig_pcst->statusmaskIOControl_u8 & 0x08u)==0x08u ))
                            { /* Check if the control function is configured */
                                /* MR12 RULE 12.1 VIOLATION: Nesting of control structures (statements) exceeds 15 - program is non-conforming - The code should be  reached after multiple checks done prior to this */
                                if( ((ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER_AS_SERVICE) && (ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER)) && (ptrIOCBIsigConfig->adrShortTermAdjustment_cpv.ShortTermAdjustment1_pfct == NULL_PTR))
                                {
                                    /* Control Function is not configured */
                                    *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                    /* Report development error "DCM_E_CONTROL_FUNC_NOT_CONFIGURED "to DET module if the DET module is enabled */
                                    DCM_DET_ERROR(DCM_IOCBI_ID , DCM_E_CONTROL_FUNC_NOT_CONFIGURED)
                                }
                                else
                                {
#if( DCM_CFG_DSP_IOCBI_SR_ENABLED != DCM_CFG_OFF)
                                    if(FALSE == ptrDidConfig->AtomicorNewSRCommunication_b)
                                    {
                                        if((USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8) && ((ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER_AS_SERVICE) || (ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER)) && (ptrIOCBIsigConfig->ioControlRequest_cpv.IOControlrequest_pfct == NULL_PTR))
                                        {
                                            /* Control Function is not configured */
                                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                        }
                                    }
                                    else
                                    {
                                        if(((USE_ATOMIC_SENDER_RECEIVER_INTERFACE == ptrDidConfig->didUsePort_u8) || (USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE == ptrDidConfig->didUsePort_u8)) && (NULL_PTR == ptrDidConfig->ioControlRequest_cpv.IOControlrequest_pfct))
                                        {
                                            /* Control Function is not configured */
                                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                        }
                                    }
#endif
                                }
                            }
                            else if((pMsgContext->reqData[2] == DCM_IOCBI_FREEZECURRENTSTATE)&& ((ptrDidConfig->adrExtendedConfig_pcst->statusmaskIOControl_u8 & 0x04u)==0x04u ) )
                            { /* Check if the Control function is configured */
                                /* Check if the Control function is configured */
                                if(((ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER_AS_SERVICE) && (ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER)) && (ptrIOCBIsigConfig->adrFreezeCurrentState_cpv.FreezeCurrentState1_pfct == NULL_PTR))
                                {
                                    /* Control Function is not configured */
                                    *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                    /* Report development error "DCM_E_CONTROL_FUNC_NOT_CONFIGURED "to DET module if the DET module is enabled */
                                    DCM_DET_ERROR(DCM_IOCBI_ID , DCM_E_CONTROL_FUNC_NOT_CONFIGURED)
                                }
                                else
                                {
#if( DCM_CFG_DSP_IOCBI_SR_ENABLED != DCM_CFG_OFF)
                                    if(FALSE == ptrDidConfig->AtomicorNewSRCommunication_b)
                                    {
                                        if((USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8) && ((ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER_AS_SERVICE) || (ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER)) && (ptrIOCBIsigConfig->ioControlRequest_cpv.IOControlrequest_pfct == NULL_PTR))
                                        {
                                            /* Control Function is not configured */
                                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                        }
                                    }
                                    else
                                    {
                                        if(((USE_ATOMIC_SENDER_RECEIVER_INTERFACE == ptrDidConfig->didUsePort_u8) || (USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE == ptrDidConfig->didUsePort_u8)) && (NULL_PTR == ptrDidConfig->ioControlRequest_cpv.IOControlrequest_pfct))
                                        {
                                            /* Control Function is not configured */
                                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                        }
                                    }
#endif
                                }
                            }
                            else if((pMsgContext->reqData[2] == DCM_IOCBI_RESETTODEFAULT) &&((ptrDidConfig->adrExtendedConfig_pcst->statusmaskIOControl_u8 & 0x02u)==0x02u ))
                            { /* Check if the Control function is configured */
                                if(((ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER_AS_SERVICE) && (ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER)) && (ptrIOCBIsigConfig->adrResetToDefault_cpv.ResetToDefault1_pfct == NULL_PTR))
                                {
                                    /* Control Function is not configured */
                                    *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                    /* Report development error "DCM_E_CONTROL_FUNC_NOT_CONFIGURED "to DET module if the DET module is enabled */
                                    DCM_DET_ERROR(DCM_IOCBI_ID , DCM_E_CONTROL_FUNC_NOT_CONFIGURED)
                                }
                                else
                                {
#if( DCM_CFG_DSP_IOCBI_SR_ENABLED != DCM_CFG_OFF)
                                    if(FALSE == ptrDidConfig->AtomicorNewSRCommunication_b)
                                    {
                                        if((USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8) && ((ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER_AS_SERVICE) || (ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER)) && (ptrIOCBIsigConfig->ioControlRequest_cpv.IOControlrequest_pfct == NULL_PTR))
                                        {
                                            /* Control Function is not configured */
                                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                        }
                                    }
                                    else
                                    {
                                        if(((USE_ATOMIC_SENDER_RECEIVER_INTERFACE == ptrDidConfig->didUsePort_u8) || (USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE == ptrDidConfig->didUsePort_u8)) && (NULL_PTR == ptrDidConfig->ioControlRequest_cpv.IOControlrequest_pfct))
                                        {
                                            /* Control Function is not configured */
                                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                        }
                                    }
#endif
                                }
                            }
                            else if((pMsgContext->reqData[2] == DCM_IOCBI_RETURNCONTROLTOECU) &&((ptrDidConfig->adrExtendedConfig_pcst->statusmaskIOControl_u8 & 0x01u)==0x01u ))
                            { /* Check if the Control function is configured */

                                if(((ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER_AS_SERVICE) && (ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER)) && (ptrIOCBIsigConfig->adrReturnControlEcu_cpv.ReturnControlEcu1_pfct == NULL_PTR))
                                {
                                    /* Control Function is not configured */
                                    *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                    /* Report development error "DCM_E_CONTROL_FUNC_NOT_CONFIGURED "to DET module if the DET module is enabled */
                                    DCM_DET_ERROR(DCM_IOCBI_ID , DCM_E_CONTROL_FUNC_NOT_CONFIGURED)
                                }
                                else
                                {
#if( DCM_CFG_DSP_IOCBI_SR_ENABLED != DCM_CFG_OFF)
                                    if(FALSE == ptrDidConfig->AtomicorNewSRCommunication_b)
                                    {
                                        if((USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8) && ((ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER_AS_SERVICE) || (ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER)) && (ptrIOCBIsigConfig->ioControlRequest_cpv.IOControlrequest_pfct == NULL_PTR))
                                        {
                                            /* Control Function is not configured */
                                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                        }
                                    }
                                    else
                                    {
                                        if(((USE_ATOMIC_SENDER_RECEIVER_INTERFACE == ptrDidConfig->didUsePort_u8) || (USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE == ptrDidConfig->didUsePort_u8)) && (NULL_PTR == ptrDidConfig->ioControlRequest_cpv.IOControlrequest_pfct))
                                        {
                                            /* Control Function is not configured */
                                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                        }
                                    }
#endif
                                }
                            }
                            else
                            {
                                /* Control Function is not configured */
                                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                /* Report development error "DCM_E_CONTROL_FUNC_NOT_CONFIGURED "to DET module if the DET module is enabled */
                                DCM_DET_ERROR(DCM_IOCBI_ID , DCM_E_CONTROL_FUNC_NOT_CONFIGURED)
                            }
                        }
                        else
                        {
                            /* Control Function is not configured */
                            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                            /* Report development error "DCM_E_CONTROL_FUNC_NOT_CONFIGURED "to DET module if the DET module is enabled */
                            DCM_DET_ERROR(DCM_IOCBI_ID , DCM_E_CONTROL_FUNC_NOT_CONFIGURED)
                        }
					 Dcm_DidSignalIdx_u16++;
                    }
                    if(*dataNegRespCode_u8 == 0x00)
                    {
                        /*Initialize the control mask and control state size as 0*/
                        dataControlMaskLen_u16 = 0x0;
                        dataLength_u32 = 0x0;
#if(DCM_CFG_DSP_SHORTTERMADJUSTMENT_ENABLED  == DCM_CFG_ON)
                        /* Control state is applicable for short term adjustment*/
                        if(pMsgContext->reqData[2] == DCM_IOCBI_SHORTTERMADJUSTMENT)
                        {
                            /*Set the flag for s_IsIOCBISubfuncCalled to TRUE*/
                            s_IsIOCBISubfuncCalled_b = TRUE;
                            /* Get the total length for the signal*/
                            dataFuncRetVal_u8=Dcm_GetLengthOfDIDIndex(&s_Dcm_idxIocbiDidIndexType_st,&dataLength_u32,nrDID_u16);
                            /*Set the flag for s_IsIOCBISubfuncCalled to FALSE*/
                            s_IsIOCBISubfuncCalled_b = FALSE;
                        }
#endif
                        if(dataFuncRetVal_u8==E_OK)
                        {
                            /* IF the IOCONTROL DID is of typ DCM_CONTROLMASK_INTERNAL or DCM_CONTROLMASK_EXTERNAL
                             * obtain the ccontrlmask size from the conifugruation and check for the length if not report Incorrect Msg length NRC*/
                            ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16];
                            ptrDidExtendedConfig = ptrDidConfig->adrExtendedConfig_pcst;
                            if(ptrDidExtendedConfig->dataCtrlMask_en != DCM_CONTROLMASK_NO)
                            {
                                dataControlMaskLen_u16 = ptrDidExtendedConfig->dataCtrlMaskSize_u8;
                            }
                            /**Exact length check*/
                            if ((pMsgContext->reqDataLen-3u) != (dataLength_u32 + dataControlMaskLen_u16))
                            { /* Request length error, Send negative response */
                                *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                            }
                            else
                            {
                                /* If control state or control mask is present*/
                                if((dataLength_u32 != 0u) || (dataControlMaskLen_u16 != 0u))
                                {
                                    /* Call the appl function to validate the control mask and control state*/
                                    dataValidateIoMaskStatus_u8 = DcmAppl_DcmCheckControlMaskAndState(nrDID_u16,pMsgContext->reqData[2],&(pMsgContext->reqData[3]),(uint16)(pMsgContext->reqDataLen-3u));
                                    if(dataValidateIoMaskStatus_u8 != E_OK)
                                    {
                                        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                        DCM_DET_ERROR(DCM_IOCBI_ID , DCM_E_INVALID_CONTROL_DATA)
                                    }
                                }
                            }

                        }
                        /* check for infrastructural errors*/
                        else if(dataFuncRetVal_u8 == DCM_INFRASTRUCTURE_ERROR)
                        {
                            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                        }
                        else if (dataFuncRetVal_u8 == DCM_E_PENDING)
                        {
                            /* Do nothing, just call again*/
                        }
                        else
                        {
                            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                        }
                    }
                }
                else
                { /* DID is not allowed in the current session */
                    *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                    /* Report development error "DCM_E_NOT_SUPPORTED_IN_CURRENT_SESSION "to DET module if the DET module is enabled */
                    DCM_DET_ERROR(DCM_IOCBI_ID , DCM_E_NOT_SUPPORTED_IN_CURRENT_SESSION)
                }

                if((*dataNegRespCode_u8 == 0x0) && (dataFuncRetVal_u8 != DCM_E_PENDING))
                { /* Move to next state where the request length is validated */
                    Dcm_stDspIocbiState_en = DCM_IOCBI_CHKCOND;
                    /* Reset the signal index */
                    Dcm_DidSignalIdx_u16 = 0x0;
                }
                else
                {
                    break;
                }
            }
            /* MR12 RULE 16.3 VIOLATION: Controlling expression is not and does not end with a 'jump' statement. Execution will fall through. MISRA C:2012 Rule-16.3 */
            case DCM_IOCBI_CHKCOND:
            {
                if (*dataNegRespCode_u8 == 0)
                {
                    ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16];
                    ptrDidExtendedConfig = ptrDidConfig->adrExtendedConfig_pcst;

                    /* Check if this DID is allowed in the current security level */
                    if ((Dcm_DsldGetActiveSecurityMask_u32()& (ptrDidExtendedConfig->dataSecBitMask_u32))!=0x00uL)
                    {
                        /*Set the flag for mode check return value to TRUE*/
                        isModeChkRetval_b = TRUE;
                        if(ptrDidExtendedConfig->adrUserControlModeRule_pfct!=NULL_PTR)
                        {
                            /* Call the configured API to do DID specific checks before IO control operation */
                            dataFuncRetVal_u8 = (*ptrDidExtendedConfig->adrUserControlModeRule_pfct)(&dataNrc_u8,ptrDidConfig->dataDid_u16,DCM_SUPPORT_IOCONTROL);
                        }
                        else
                        {
                            /* Call the Dcm Appl API to do DID specific checks before IO control operation */
                            dataFuncRetVal_u8 = DcmAppl_UserDIDModeRuleService(&dataNrc_u8,ptrDidConfig->dataDid_u16,DCM_SUPPORT_IOCONTROL);
                        }
                        if(dataFuncRetVal_u8!=E_OK)
                        {
                            if(dataNrc_u8==0x00)
                            {
                                dataNrc_u8 = DCM_E_CONDITIONSNOTCORRECT;
                            }
                            /*Set the flag for mode check return value to FALSE*/
                            isModeChkRetval_b = FALSE;
                        }
                        else
                        {
                            dataNrc_u8=0;
                        }
#if(DCM_CFG_DSP_MODERULEFORDIDCONTROL!=DCM_CFG_OFF)
                        if((dataNrc_u8==0x00 ) &&(NULL_PTR != ptrDidExtendedConfig->adrIocbiModeRuleChkFnc_pfct))
                        {
                            isModeChkRetval_b = (*(ptrDidExtendedConfig->adrIocbiModeRuleChkFnc_pfct))(&dataNrc_u8);
                        }
#endif
                        /*chek if the flag for mode check return value is set to TRUE*/
                        if(FALSE!= isModeChkRetval_b)
                        {
                            /* Check if the response length fits into the buffer */
                            if (pMsgContext->resMaxDataLen >= (uint16)(ptrDidConfig->dataMaxDidLen_u16+3u))
                            { /* Move the state to perform the Control operation */
                                Dcm_stDspIocbiState_en = DCM_IOCBI_RUNNING;
                            }
                            else
                            { /* Response length is too long, Send negative response */
                                *dataNegRespCode_u8 = DCM_E_RESPONSETOOLONG;
                            }
                        }
                        else
                        {
                            *dataNegRespCode_u8 = dataNrc_u8;
                        }
                    }
                    else
                    { /* DID is not allowed in the current security level */
                        *dataNegRespCode_u8 = DCM_E_SECURITYACCESSDENIED;
                    }
                }
                if(*dataNegRespCode_u8 != 0x0)
                /* Nothing to do */
                {
                    break;
                }
            }
            /* MR12 RULE 16.3 VIOLATION:The preceding 'switch' clause is not empty and does not end with a 'jump' statement. Execution will fall through. MISRA C:2012 Rule-16.3 */
            case DCM_IOCBI_RUNNING:
            {
                ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16];
                ptrDidExtendedConfig = ptrDidConfig->adrExtendedConfig_pcst;
                /* Copy the control parameter to local variable */
                dataIoCtrlParam_u8 = pMsgContext->reqData[2];
                Dcm_dataDspIocbiCtrlParam_u8 = pMsgContext->reqData[2];
                if(ptrDidConfig->adrExtendedConfig_pcst->dataCtrlMask_en != DCM_CONTROLMASK_EXTERNAL)
                {
                    /* Get the start index of the control mask */
                    dataCtlMaskOffset_u16 = (uint16)(pMsgContext->reqDataLen - ((uint32)((ptrDidConfig->nrSig_u16-1u)/8u) + 1u));
                    /* Loop for all the signals configured for a DID */
                    /*MR12 RULE 15.4 VIOLATION :More than one 'break' statement has been used to terminate this iteration statement. MISRA C:2012 Rule-15.4*/
                    while((Dcm_DidSignalIdx_u16 < ptrDidConfig->nrSig_u16)&&(*dataNegRespCode_u8==0x0u))
                    {
                        posnCtlMaskBit_u8 = 0x80;
                        if(ptrDidConfig->nrSig_u16 > 1u)
                        {
                            /*Get the byte position of the signal*/
                            posnSigByte_u16 = (uint16)(Dcm_DidSignalIdx_u16/8u);
                            /*Get the bit position of the signal*/
                            posnSigBit_u8 = (uint8)(Dcm_DidSignalIdx_u16%8);
                            /*Get the control mask value byte*/
                            posnCtlMaskByte_u8 = pMsgContext->reqData[dataCtlMaskOffset_u16 + posnSigByte_u16];
                            /*Get the control mask bit value for the signal*/
                            posnCtlMaskBit_u8 = (uint8)(((uint8)(posnCtlMaskByte_u8 << posnSigBit_u8)) & ((uint8)0x80));

                        }
                        /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                        s_IsIOCBISubfuncCalled_b = TRUE;
                        /* Get the length for each signal, which will be for passing the pointer for short term adjustment and for sending the response*/
                        dataFuncRetVal_u8=Dcm_GetLengthOfSignal(&dataSignalLength_u16);
                        /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                        s_IsIOCBISubfuncCalled_b = FALSE;
                        if(dataFuncRetVal_u8 == E_OK)
                        {
                            /* Do nothing. Base pointer will be incremented at the end of for loop */
                        }
                        else if(dataFuncRetVal_u8 == DCM_E_PENDING)
                        {
                            /* Wait for next cycle*/
                            dataIocbiExeResult_u8= DCM_E_PENDING;
                            break;
                        }
                        /* Send GeneralReject for either Infrastructure Error or for any other return values */
                        else
                        {
                            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                        }

                        /*Check whether signal is masked or not*/
                        s_ActiveDid_u16 = ptrDidConfig->dataDid_u16;
                        ptrSigConfig = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
                        ptrIOSigConfig= &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];

                        if((posnCtlMaskBit_u8 == 0x80) && (*dataNegRespCode_u8 == 0x00))
                        {
#if(DCM_CFG_DSP_SHORTTERMADJUSTMENT_ENABLED  == DCM_CFG_ON)
                            /* Short Term adjustment is requested? */
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_SHORTTERMADJUSTMENT)
                            {
                                ShortTermAdjustment_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrShortTermAdjustment_cpv;
                                /* Check if the Control function is configured */
                                if(ShortTermAdjustment_un.ShortTermAdjustment1_pfct!= NULL_PTR)
                                {
                                    /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                                    s_IsIOCBISubfuncCalled_b = TRUE;

#if(DCM_CFG_DSP_CONTROL_SYNCH_FNC_ENABLED   == DCM_CFG_ON)
                                    if((ptrDidExtendedConfig->dataCtrlMask_en!=DCM_CONTROLMASK_EXTERNAL) && ((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC)))
                                    {
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment1_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN+Dcm_ReadSignalLength_u16)],dataNegRespCode_u8);
                                    }

#endif
#if(DCM_CFG_DSP_CONTROL_ASYNCH_FNC_ENABLED   == DCM_CFG_ON)
                                    if((ptrDidExtendedConfig->dataCtrlMask_en!=DCM_CONTROLMASK_EXTERNAL) && ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC)))
                                    {
#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)
                                        if ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                                        {
                                            /* Check whether Rte_Call API hasn't been invoked already */
                                            if (!s_IocbiRteCallPlaced_b)
                                            {
                                                dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment9_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN+Dcm_ReadSignalLength_u16)],Dcm_stDspIocbiOpStatus_u8);
                                                if(dataIocbiExeResult_u8 == E_OK)
                                                {
                                                    /* Set the Rte_Call Flag to TRUE and return Pending so that Rte_Result will be invoked in the next cycle */
                                                    s_IocbiRteCallPlaced_b = TRUE;
                                                    dataIocbiExeResult_u8 = DCM_E_PENDING;
                                                }
                                                else
                                                {
                                                    /* For any return value other than E_OK send NRC General Reject */
                                                    *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                                }
                                            }
                                            else
                                            {
                                                ShortTermAdjustmentResult_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrShortTermAdjustmentResults_cpv;
                                               dataIocbiExeResult_u8=(*(ShortTermAdjustmentResult_un.ShortTermAdjustment13_pfct)) (dataNegRespCode_u8);
                                               if(dataIocbiExeResult_u8 == E_OK)
                                               {
                                                   s_IocbiRteCallPlaced_b = FALSE;
                                               }
                                               else if(dataIocbiExeResult_u8 == RTE_E_NO_DATA)
                                               {
                                                    dataIocbiExeResult_u8 = DCM_E_PENDING;
                                               }
                                               else
                                               {
                                                   /* For any return value other than E_OK , RTE_E_NO_DATA ,  reset the Flag and send NRC General Reject */
                                                   s_IocbiRteCallPlaced_b = FALSE;
                                                   *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                               }
                                            }
                                        }
                                        else
#endif
                                        {
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment2_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN+Dcm_ReadSignalLength_u16)],Dcm_stDspIocbiOpStatus_u8, dataNegRespCode_u8);
                                       }
                                    }

#endif
                                    /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                                    s_IsIOCBISubfuncCalled_b = FALSE;
                                }
                            }
#endif
#if(DCM_CFG_DSP_FREEZECURRENTSTATE_ENABLED == DCM_CFG_ON)
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_FREEZECURRENTSTATE)
                            {
                                FreezeCurrentState_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrFreezeCurrentState_cpv;
                                /* Check if the Control function is configured */
                                if(FreezeCurrentState_un.FreezeCurrentState1_pfct != NULL_PTR)
                                {
                                    /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                                    s_IsIOCBISubfuncCalled_b = TRUE;

                                    if((ptrDidExtendedConfig->dataCtrlMask_en!=DCM_CONTROLMASK_EXTERNAL) && ((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC)))
                                    {
                                        /* Freeze current state is requested */
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState1_pfct)) (dataNegRespCode_u8);
                                    }

                                    if((ptrDidExtendedConfig->dataCtrlMask_en!=DCM_CONTROLMASK_EXTERNAL) && ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC)))
                                    {
#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)
                                        if ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                                        {
                                            /* Check whether Rte_Call API hasn't been invoked already */
                                            if (!s_IocbiRteCallPlaced_b)
                                            {
                                                dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState9_pfct)) (Dcm_stDspIocbiOpStatus_u8);
                                                if(dataIocbiExeResult_u8 == E_OK)
                                                {
                                                    /* Set the Rte_Call Flag to TRUE and return Pending so that Rte_Result will be invoked in the next cycle */
                                                    s_IocbiRteCallPlaced_b = TRUE;
                                                    dataIocbiExeResult_u8 = DCM_E_PENDING;
                                                }
                                                else
                                                {
                                                    /* For any return value other than E_OK send NRC General Reject */
                                                    *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                                }
                                            }
                                            else
                                            {
                                                FreezeCurrentStateResults_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrFreezeCurrentStateResults_cpv;
                                                dataIocbiExeResult_u8=(*(FreezeCurrentStateResults_un.FreezeCurrentState13_pfct)) (dataNegRespCode_u8);
                                               if(dataIocbiExeResult_u8 == E_OK)
                                               {
                                                   s_IocbiRteCallPlaced_b = FALSE;
                                               }
                                               else if(dataIocbiExeResult_u8 == RTE_E_NO_DATA)
                                               {
                                                    dataIocbiExeResult_u8 = DCM_E_PENDING;
                                               }
                                               else
                                               {
                                                   /* For any return value other than E_OK , RTE_E_NO_DATA ,  reset the Flag and send NRC General Reject */
                                                   s_IocbiRteCallPlaced_b = FALSE;
                                                   *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                               }
                                            }
                                        }
                                        else
#endif
                                        {
                                            /* Freeze current state is requested */
                                            /* Call the configured API */
                                            dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState2_pfct))(Dcm_stDspIocbiOpStatus_u8,dataNegRespCode_u8);
                                        }
                                    }
                                    /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                                    s_IsIOCBISubfuncCalled_b = FALSE;
                                }
                            }
#endif
#if(DCM_CFG_DSP_RESETTODEFAULT_ENABLED == DCM_CFG_ON)
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_RESETTODEFAULT)
                            {
                                ResetToDefault_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrResetToDefault_cpv;
                                /* Check if the Control function is configured */
                                if(ResetToDefault_un.ResetToDefault1_pfct != NULL_PTR)
                                {
                                    /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                                    s_IsIOCBISubfuncCalled_b = TRUE;

#if(DCM_CFG_DSP_CONTROL_SYNCH_FNC_ENABLED== DCM_CFG_ON)
                                    if((ptrDidExtendedConfig->dataCtrlMask_en!=DCM_CONTROLMASK_EXTERNAL) && ((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC)))
                                    {
                                        /* Freeze current state is requested */
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault1_pfct)) (dataNegRespCode_u8);
                                    }
#endif

#if(DCM_CFG_DSP_CONTROL_ASYNCH_FNC_ENABLED == DCM_CFG_ON)
                                    if((ptrDidExtendedConfig->dataCtrlMask_en!=DCM_CONTROLMASK_EXTERNAL) && ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC)))
                                    {
#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)
                                        if ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                                        {
                                            /* Check whether Rte_Call API hasn't been invoked already */
                                            if (!s_IocbiRteCallPlaced_b)
                                            {
                                                dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault9_pfct)) (Dcm_stDspIocbiOpStatus_u8);
                                                if(dataIocbiExeResult_u8 == E_OK)
                                                {
                                                    /* Set the Rte_Call Flag to TRUE and return Pending so that Rte_Result will be invoked in the next cycle */
                                                    s_IocbiRteCallPlaced_b = TRUE;
                                                    dataIocbiExeResult_u8 = DCM_E_PENDING;
                                                }
                                                else
                                                {
                                                    /* For any return value other than E_OK send NRC General Reject */
                                                    *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                                }
                                            }
                                            else
                                            {
                                                ResetToDefaultResult_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrResetToDefaultResults_cpv;
                                               dataIocbiExeResult_u8=(*(ResetToDefaultResult_un.ResetToDefault13_pfct)) (dataNegRespCode_u8);
                                               if(dataIocbiExeResult_u8 == E_OK)
                                               {
                                                   s_IocbiRteCallPlaced_b = FALSE;
                                               }
                                               else if(dataIocbiExeResult_u8 == RTE_E_NO_DATA)
                                               {
                                                    dataIocbiExeResult_u8 = DCM_E_PENDING;
                                               }
                                               else
                                               {
                                                   /* For any return value other than E_OK , RTE_E_NO_DATA ,  reset the Flag and send NRC General Reject */
                                                   s_IocbiRteCallPlaced_b = FALSE;
                                                   *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                               }
                                            }
                                        }
                                        else
#endif
                                        {
                                            /* Freeze current state is requested */
                                            /* Call the configured API */
                                            dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault2_pfct))(Dcm_stDspIocbiOpStatus_u8,dataNegRespCode_u8);
                                        }
                                    }
#endif
                                    /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                                    s_IsIOCBISubfuncCalled_b = FALSE;
                                }
                            }
#endif
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_RETURNCONTROLTOECU)
                            {
                                ReturnControlToEcu_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrReturnControlEcu_cpv;
                                /* Check if the Control function is configured */
                                if(ReturnControlToEcu_un.ReturnControlEcu1_pfct != NULL_PTR)
                                {
                                    /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                                    s_IsIOCBISubfuncCalled_b = TRUE;

                                    /*As ReturnControlToEcu is a synchronous API , the same API is invoked for both Synch and ASynch Fnc and ClientServer Configuration */
                                    if( (ptrDidExtendedConfig->dataCtrlMask_en!=DCM_CONTROLMASK_EXTERNAL) && (ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER_AS_SERVICE) && (ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER))
                                    {
                                        /* ReturnControlToECU is a synchronous API irrespective of any UsePort configuration.So the function signature is same for Asynch as well as Sync implementation */
                                        dataIocbiExeResult_u8=(*(ReturnControlToEcu_un.ReturnControlEcu1_pfct)) (dataNegRespCode_u8);
                                    }

                                    /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                                    s_IsIOCBISubfuncCalled_b = FALSE;
                                }
                            }
                            /* Check for infrastructural errors in case of RTE*/

                            if((Dcm_IsInfrastructureErrorPresent_b(dataIocbiExeResult_u8) != FALSE) && ((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER) || (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)))
                            {
                                /* Set the nrc as general reject*/
                                *dataNegRespCode_u8 =DCM_E_GENERALREJECT;
                            }
                            else if(dataIocbiExeResult_u8 == E_OK)
                            {
                                *dataNegRespCode_u8 = 0x00;
                            }
                            else
                            {
                                /* MR12 RULE 15.4 VIOLATION: More than one break statement is being used to terminate an iteration statement */
                                break;
                            }
                        }

                        if((dataIocbiExeResult_u8 == E_OK) && (*dataNegRespCode_u8 == 0x00) )
                        {
                            Dcm_ReadSignalLength_u16 += dataSignalLength_u16;
                        }

					Dcm_DidSignalIdx_u16++;
                    }

                    if ((E_OK == dataIocbiExeResult_u8) && (*dataNegRespCode_u8 == 0x0))
                    { /* Move the state to read control status record */
                        Dcm_stDspIocbiState_en = DCM_IOCBI_READSTREC;
                        /* Reset the signal index */
                        Dcm_DidSignalIdx_u16 = 0x0;
                        Dcm_stDspIocbiOpStatus_u8 = DCM_INITIAL;

                    }
                    else if((E_NOT_OK == dataIocbiExeResult_u8) && (*dataNegRespCode_u8 == 0x0))
                    {
                        /* Send negative response code DCM_E_GENERALREJECT as the application has returned E_NOT_OK */
                        *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                        Dcm_stDspIocbiOpStatus_u8 = DCM_INITIAL;
                        break;
                    }
                    else
                    {
                        Dcm_stDspIocbiOpStatus_u8 = DCM_INITIAL;

                        if(DCM_E_PENDING == dataIocbiExeResult_u8)
                        {
                            *dataNegRespCode_u8 = 0x00;
                            dataServretVal_u8=DCM_E_PENDING;
                            Dcm_stDspIocbiOpStatus_u8 = DCM_PENDING;
                        }
                        break;
                    }
                }
                else if(ptrDidConfig->adrExtendedConfig_pcst->dataCtrlMask_en == DCM_CONTROLMASK_EXTERNAL)
                {
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)

                    dataFuncRetVal_u8=Dcm_GetLengthOfSignal(&dataSignalLength_u16);
                    if(dataFuncRetVal_u8 == E_OK)
                    {
                        /* Do nothing. Base pointer will be incremented at the end of for loop */
                    }
                    else if(dataFuncRetVal_u8 == DCM_E_PENDING)
                    {
                        /* Wait for next cycle*/
                        dataIocbiExeResult_u8= DCM_E_PENDING;
                        dataServretVal_u8=DCM_E_PENDING;
                        break;
                    }
                    else
                    {
                        /* Send GeneralReject for either Infrastructure Error or for any other return values */
                        *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                    }

                    /*Check whether signal is masked or not*/
                    s_ActiveDid_u16 = ptrDidConfig->dataDid_u16;
                    ptrSigConfig = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
                    ptrIOSigConfig= &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];
#if( DCM_CFG_DSP_IOCBI_SR_ENABLED != DCM_CFG_OFF)
                    if((ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER_AS_SERVICE) || (ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER))
                    {
                        if(FALSE == ptrDidConfig->AtomicorNewSRCommunication_b)
                        {
                            /* According to SWS 4.2.2 */
                            ControlRequest_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].ioControlRequest_cpv;
                        }
                        else
                        {
                            /* According to SWS 4.4.0 */
                            ControlRequest_un= ptrDidConfig->ioControlRequest_cpv;
                        }

                        if(ControlRequest_un.IOControlrequest_pfct != NULL_PTR)
                        {
                            /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                            s_IsIOCBISubfuncCalled_b = TRUE;
                            dataIocbiExeResult_u8 =(*(ControlRequest_un.IOControlrequest_pfct))(Dcm_dataDspIocbiCtrlParam_u8,&pMsgContext->reqData[DSP_IOCBI_MINREQLEN],dataSignalLength_u16,ptrDidExtendedConfig->dataCtrlMaskSize_u8,Dcm_stDspIocbiOpStatus_u8,dataNegRespCode_u8);
                            /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                            s_IsIOCBISubfuncCalled_b = FALSE;
                        }
                    }
                    else
#endif
                    {
#if(DCM_CFG_DSP_SHORTTERMADJUSTMENT_ENABLED  == DCM_CFG_ON)
                        /* Short Term adjustment is requested? */
                        if(dataIoCtrlParam_u8 == DCM_IOCBI_SHORTTERMADJUSTMENT)
                        {
                            ShortTermAdjustment_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrShortTermAdjustment_cpv;
                            /* Check if the Control function is configured */
                            if(ShortTermAdjustment_un.ShortTermAdjustment1_pfct != NULL_PTR)
                            {
                                /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                                s_IsIOCBISubfuncCalled_b = TRUE;

#if(DCM_CFG_DSP_CONTROL_SYNCH_FNC_ENABLED   == DCM_CFG_ON)

                                if((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC))
                                {
                                    if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                    {
                                        ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment3_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN)],ControlMask_u8,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                    {
                                        ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment4_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN)],ControlMask_u16,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                    {
                                        ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],                  \
                                        pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment5_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN)],ControlMask_u32,dataNegRespCode_u8);
                                    }
                                    else
                                    {
                                        /*Dummy else clause*/
                                    }
                                }
#endif
#if(DCM_CFG_DSP_CONTROL_ASYNCH_FNC_ENABLED   == DCM_CFG_ON)
                                if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC))
                                {

#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED == DCM_CFG_ON)
                                    if ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                                    {
                                        if (!s_IocbiRteCallPlaced_b)
                                        {
                                    if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                    {
                                        ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment10_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN)],Dcm_stDspIocbiOpStatus_u8,ControlMask_u8);
                                            }
                                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                            {
                                                ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                                /* Call the configured API */
                                                dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment11_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN)],Dcm_stDspIocbiOpStatus_u8,ControlMask_u16);
                                            }
                                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                            {
                                                ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],                  \
                                                pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                                /* Call the configured API */
                                                dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment12_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN)],Dcm_stDspIocbiOpStatus_u8,ControlMask_u32);
                                            }
                                            else
                                            {
                                                /*Dummy else clause*/
                                            }

                                            if(dataIocbiExeResult_u8 == E_OK)
                                            {
                                                /* Set the Rte_Call Flag to TRUE and return Pending so that Rte_Result will be invoked in the next cycle */
                                                 s_IocbiRteCallPlaced_b = TRUE;
                                                 dataIocbiExeResult_u8 = DCM_E_PENDING;
                                            }
                                            else
                                            {
                                                /* For any return value other than E_OK send NRC General Reject */
                                                *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                            }
                                        }
                                        else
                                        {
                                            ShortTermAdjustmentResult_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrShortTermAdjustmentResults_cpv;
                                            /* Call the configured API */
                                            dataIocbiExeResult_u8=(*(ShortTermAdjustmentResult_un.ShortTermAdjustment13_pfct)) (dataNegRespCode_u8);

                                            if(dataIocbiExeResult_u8 == E_OK)
                                            {
                                               s_IocbiRteCallPlaced_b = FALSE;
                                            }
                                            else if(dataIocbiExeResult_u8 == RTE_E_NO_DATA)
                                            {
                                                dataIocbiExeResult_u8 = DCM_E_PENDING;
                                            }
                                            else
                                            {
                                               /* For any return value other than E_OK , RTE_E_NO_DATA ,  reset the Flag and send NRC General Reject */
                                               s_IocbiRteCallPlaced_b = FALSE;
                                               *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                            }
                                        }
                                    }
                                    else
#endif
                                    {
                                        if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                        {
                                            ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                            /* Call the configured API */
                                            dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment6_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN)],Dcm_stDspIocbiOpStatus_u8,ControlMask_u8,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                    {
                                        ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment7_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN)],Dcm_stDspIocbiOpStatus_u8,ControlMask_u16,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                    {
                                        ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],                  \
                                        pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ShortTermAdjustment_un.ShortTermAdjustment8_pfct)) (&pMsgContext->reqData[(DSP_IOCBI_MINREQLEN)],Dcm_stDspIocbiOpStatus_u8,ControlMask_u32,dataNegRespCode_u8);
                                    }
                                    else
                                    {
                                        /*Dummy else clause*/
                                        }
                                    }
                                }
#endif
                                /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                                s_IsIOCBISubfuncCalled_b = FALSE;
                            }

                        }
#endif
#if(DCM_CFG_DSP_FREEZECURRENTSTATE_ENABLED  == DCM_CFG_ON)
                        /* Short Term adjustment is requested? */
                        if(dataIoCtrlParam_u8 == DCM_IOCBI_FREEZECURRENTSTATE)
                        {
                            FreezeCurrentState_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrFreezeCurrentState_cpv;
                            /* Check if the Control function is configured */
                            if(FreezeCurrentState_un.FreezeCurrentState1_pfct != NULL_PTR)
                            {
                                /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                                s_IsIOCBISubfuncCalled_b = TRUE;

                                if((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC))
                                {
                                    if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                    {
                                        ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState3_pfct))(ControlMask_u8,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                    {
                                        ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState4_pfct))(ControlMask_u16,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                    {
                                        ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],                  \
                                        pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState5_pfct))(ControlMask_u32,dataNegRespCode_u8);
                                    }
                                    else
                                    {
                                        /*Dummy else clause*/
                                    }
                                }

                                if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC))
                                {

#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED == DCM_CFG_ON)
                                    if ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                                    {
                                        /* If the Rte_Call API is not invoked already */
                                        if (!s_IocbiRteCallPlaced_b)
                                        {
                                    if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                    {
                                        ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                                /* Call the configured API */
                                                dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState10_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u8);
                                            }
                                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                            {
                                                ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                                /* Call the configured API */
                                                dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState11_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u16);
                                            }
                                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                            {
                                                ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],                  \
                                                pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                                /* Call the configured API */
                                                dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState12_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u32);
                                            }
                                            else
                                            {
                                                /*Dummy else clause*/
                                            }

                                            if(dataIocbiExeResult_u8 == E_OK)
                                            {
                                                /* Set the Rte_Call Flag to TRUE and return Pending so that Rte_Result will be invoked in the next cycle */
                                                 s_IocbiRteCallPlaced_b = TRUE;
                                                 dataIocbiExeResult_u8 = DCM_E_PENDING;
                                            }
                                            else
                                            {
                                                /* For any return value other than E_OK send NRC General Reject */
                                                *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                            }
                                        }
                                        else
                                        {
                                            FreezeCurrentStateResults_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrFreezeCurrentStateResults_cpv;
                                                /* Call the configured API */
                                                dataIocbiExeResult_u8=(*(FreezeCurrentStateResults_un.FreezeCurrentState13_pfct)) (dataNegRespCode_u8);
                                            if(dataIocbiExeResult_u8 == E_OK)
                                            {
                                               s_IocbiRteCallPlaced_b = FALSE;
                                            }
                                            else if(dataIocbiExeResult_u8 == RTE_E_NO_DATA)
                                            {
                                                dataIocbiExeResult_u8 = DCM_E_PENDING;
                                            }
                                            else
                                            {
                                               /* For any return value other than E_OK , RTE_E_NO_DATA ,  reset the Flag and send NRC General Reject */
                                               s_IocbiRteCallPlaced_b = FALSE;
                                               *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                            }
                                        }
                                    }
                                    else
#endif
                                    {
                                        if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                        {
                                            ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                            dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState6_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u8,dataNegRespCode_u8);
                                        }
                                        else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                        {
                                            ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                            /* Call the configured API */
                                            dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState7_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u16,dataNegRespCode_u8);
                                        }
                                        else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                        {
                                            ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],                  \
                                                    pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                            /* Call the configured API */
                                            dataIocbiExeResult_u8=(*(FreezeCurrentState_un.FreezeCurrentState8_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u32,dataNegRespCode_u8);
                                        }
                                        else
                                        {
                                            /*Dummy else clause*/
                                        }
                                    }
                                }

                                /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                                s_IsIOCBISubfuncCalled_b = FALSE;
                            }
                        }
#endif
#if(DCM_CFG_DSP_RESETTODEFAULT_ENABLED  == DCM_CFG_ON)
                        /* Short Term adjustment is requested? */
                        if(dataIoCtrlParam_u8 == DCM_IOCBI_RESETTODEFAULT)
                        {
                            ResetToDefault_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrResetToDefault_cpv;
                            /* Check if the Control function is configured */
                            if(ResetToDefault_un.ResetToDefault1_pfct != NULL_PTR)
                            {
                                /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                                s_IsIOCBISubfuncCalled_b = TRUE;
#if(DCM_CFG_DSP_CONTROL_SYNCH_FNC_ENABLED== DCM_CFG_ON)
                                if((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC))
                                {
                                    if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                    {
                                        ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault3_pfct))(ControlMask_u8,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                    {
                                        ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault4_pfct))(ControlMask_u16,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                    {
                                        ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],                  \
                                        pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault5_pfct))(ControlMask_u32,dataNegRespCode_u8);
                                    }
                                    else
                                    {
                                        /*Dummy else clause*/
                                    }
                                }
#endif

#if(DCM_CFG_DSP_CONTROL_ASYNCH_FNC_ENABLED == DCM_CFG_ON)
                                if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)||(ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC))
                                {

#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED == DCM_CFG_ON)
                                    if ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                                    {
                                        /* If the Rte_Call API is not invoked already */
                                        if (!s_IocbiRteCallPlaced_b)
                                        {
                                            /* Based on the Control Mask Size invoke the Rte_Call API with In and InOut parameters */
                                            if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                            {
                                               ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                                /* Call the configured API */
                                                dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault10_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u8);
                                            }
                                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                            {
                                                ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                                /* Call the configured API */
                                                dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault11_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u16);
                                            }
                                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                            {
                                                ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],                  \
                                                pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                                /* Call the configured API */
                                                dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault12_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u32);
                                            }
                                            else
                                            {
                                                /*Dummy else clause*/
                                            }

                                            if(dataIocbiExeResult_u8 == E_OK)
                                            {
                                                /* Set the Rte_Call Flag to TRUE and return Pending so that Rte_Result will be invoked in the next cycle */
                                                 s_IocbiRteCallPlaced_b = TRUE;
                                                 dataIocbiExeResult_u8 = DCM_E_PENDING;
                                            }
                                            else
                                            {
                                                /* For any return value other than E_OK send NRC General Reject */
                                                *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                            }
                                        }
                                        else
                                        {
                                            ResetToDefaultResult_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrResetToDefaultResults_cpv;
                                            /* Based on the size of controlMaskSize configured invoke the Rte_Result API */
                                                                                    /* Call the configured API */
                                            dataIocbiExeResult_u8=(*(ResetToDefaultResult_un.ResetToDefault13_pfct)) (dataNegRespCode_u8);
                                            if(dataIocbiExeResult_u8 == E_OK)
                                            {
                                               s_IocbiRteCallPlaced_b = FALSE;
                                            }
                                            else if(dataIocbiExeResult_u8 == RTE_E_NO_DATA)
                                            {
                                                dataIocbiExeResult_u8 = DCM_E_PENDING;
                                            }
                                            else
                                            {
                                               /* For any return value other than E_OK , RTE_E_NO_DATA ,  reset the Flag and send NRC General Reject */
                                               s_IocbiRteCallPlaced_b = FALSE;
                                               *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                                            }
                                        }
                                    }
                                    else
#endif
                                    {
                                        if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                        {
                                            ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                            dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault6_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u8,dataNegRespCode_u8);
                                        }
                                        else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                        {
                                            ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                            /* Call the configured API */
                                            dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault7_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u16,dataNegRespCode_u8);
                                        }
                                        else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                        {
                                            ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],                  \
                                            pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                            /* Call the configured API */
                                            dataIocbiExeResult_u8=(*(ResetToDefault_un.ResetToDefault8_pfct))(Dcm_stDspIocbiOpStatus_u8,ControlMask_u32,dataNegRespCode_u8);
                                        }
                                        else
                                        {
                                            /*Dummy else clause*/
                                        }
                                    }
                                }
#endif
                                /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                                s_IsIOCBISubfuncCalled_b = FALSE;
                            }
                        }
#endif
                        /* Return Control to is requested? */
                        if(dataIoCtrlParam_u8 == DCM_IOCBI_RETURNCONTROLTOECU)
                        {
                            ReturnControlToEcu_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrReturnControlEcu_cpv;
                            /* Check if the Control function is configured */
                            if(ReturnControlToEcu_un.ReturnControlEcu1_pfct != NULL_PTR)
                            {
                                /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                                s_IsIOCBISubfuncCalled_b = TRUE;

                                    /*As ReturnControlToEcu is a synchronous API , the same API is invoked for both Synch and ASynch Fnc and ClientServer Configuration */
                                if((ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER)&& (ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER_AS_SERVICE))
                                {
                                    if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                                    {
                                        ControlMask_u8=pMsgContext->reqData[(pMsgContext->reqDataLen-1u)];
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ReturnControlToEcu_un.ReturnControlEcu3_pfct))(ControlMask_u8,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                                    {
                                        ControlMask_u16=DSP_CONV_2U8_TO_U16(pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ReturnControlToEcu_un.ReturnControlEcu4_pfct))(ControlMask_u16,dataNegRespCode_u8);
                                    }
                                    else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8>=3)
                                    {
                                        ControlMask_u32=DSP_CONV_4U8_TO_U32(pMsgContext->reqData[(pMsgContext->reqDataLen-4u)],pMsgContext->reqData[(pMsgContext->reqDataLen-3u)],
                                        pMsgContext->reqData[(pMsgContext->reqDataLen-2u)],pMsgContext->reqData[(pMsgContext->reqDataLen-1u)]);
                                        /* Call the configured API */
                                        dataIocbiExeResult_u8=(*(ReturnControlToEcu_un.ReturnControlEcu5_pfct))(ControlMask_u32,dataNegRespCode_u8);
                                    }
                                    else
                                    {
                                        /*Dummy else clause*/
                                    }
                                }

                                /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                                s_IsIOCBISubfuncCalled_b = FALSE;
                            }
                        }

                    }
                    if (E_OK == dataIocbiExeResult_u8)
                    { /* Move the state to read control status record */
                        Dcm_stDspIocbiState_en = DCM_IOCBI_READSTREC;
                        /* Reset the signal index */
                        Dcm_DidSignalIdx_u16 = 0x0;
                        Dcm_stDspIocbiOpStatus_u8 = DCM_INITIAL;
                        Dcm_ReadSignalLength_u16 = dataSignalLength_u16;
                    }
                    else if((E_NOT_OK == dataIocbiExeResult_u8) && (*dataNegRespCode_u8 == 0x0))
                    {
                        /* Send negative response code DCM_E_GENERALREJECT as the application has returned E_NOT_OK */
                        *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                        Dcm_stDspIocbiOpStatus_u8 = DCM_INITIAL;
                        Dcm_ReadSignalLength_u16=0;
                        break;
                    }
                    else
                    {
                        Dcm_stDspIocbiOpStatus_u8 = DCM_INITIAL;
                        if(DCM_E_PENDING == dataIocbiExeResult_u8)
                        {
                            *dataNegRespCode_u8 = 0x00;
                            dataServretVal_u8=DCM_E_PENDING;
                            Dcm_stDspIocbiOpStatus_u8 = DCM_PENDING;
                        }
                        break;
                    }
#endif
                }
                else
                {
                    /*Dummy else clause*/
                    dataServretVal_u8=E_NOT_OK;
                }
            }
            /* MR12 RULE 16.3 VIOLATION: Controlling expression is not and does not end with a 'jump' statement. Execution will fall through. MISRA C:2012 Rule-16.3 */
            case DCM_IOCBI_READSTREC:
            { /* Copy the control parameter */
                dataIoCtrlParam_u8 = pMsgContext->reqData[2];
                /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                s_IsIOCBISubfuncCalled_b = TRUE;
                /* Obtain ControlStatusRecord by calling ReadFunction */
                dataRetGetDid_u8 = Dcm_GetDIDData(&s_Dcm_idxIocbiDidIndexType_st,&pMsgContext->resData[3]);
                /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                s_IsIOCBISubfuncCalled_b = FALSE;
                /* if ReadFunc is OK */
                if(dataRetGetDid_u8 == E_OK)
                {
                    /*Update the DcmDsp_IocbiStatus_array with the state of subfunction which is active */
                    for(idxIocbiIndex_u16=0;idxIocbiIndex_u16<DCM_CFG_NUM_IOCBI_DIDS;idxIocbiIndex_u16++)
                    {
                        ptrIOCBIStatusArrayConfig=&DcmDsp_IocbiStatus_array[idxIocbiIndex_u16];
                        if(s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16 == ptrIOCBIStatusArrayConfig->idxindex_u16)
                        {
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_RETURNCONTROLTOECU)
                            {
                                /*Set the status flag to IDLE state since returncontrol to ECU was successful*/
                                ptrIOCBIStatusArrayConfig->IocbiStatus_en = DCM_IOCBI_IDLESTATE;
                            }
#if(DCM_CFG_DSP_RESETTODEFAULT_ENABLED  == DCM_CFG_ON)
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_RESETTODEFAULT)
                            {
                                /*Set the status flag to RTD active state*/
                                ptrIOCBIStatusArrayConfig->IocbiStatus_en = DCM_IOCBI_RTD_ACTIVE;
                            }
#endif
#if(DCM_CFG_DSP_FREEZECURRENTSTATE_ENABLED  == DCM_CFG_ON)
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_FREEZECURRENTSTATE)
                            {
                                /*Set the status flag to FCS active state*/
                                ptrIOCBIStatusArrayConfig->IocbiStatus_en = DCM_IOCBI_FCS_ACTIVE;
                            }
#endif
#if(DCM_CFG_DSP_SHORTTERMADJUSTMENT_ENABLED  == DCM_CFG_ON)
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_SHORTTERMADJUSTMENT)
                            {
                                /*Set the status flag to STA active state*/
                                ptrIOCBIStatusArrayConfig->IocbiStatus_en = DCM_IOCBI_STA_ACTIVE;
                            }
#endif
                        }
                    }
                    /* Reset the state machine */
                    Dcm_stDspIocbiState_en = DCM_IOCBI_IDLE;
                    s_Dcm_idxIocbiDidIndexType_st.dataopstatus_b = DCM_INITIAL;
                    s_Dcm_idxIocbiDidIndexType_st.nrNumofSignalsRead_u16 = 0x0; /*All the signals read correctly, therefore reset it to zero*/
                    s_Dcm_idxIocbiDidIndexType_st.dataSignalLengthInfo_u32 = 0x0; /*All the signals read correctly, therefore reset the signal data length to zero*/
                    /* Prepare the DID bytes and the Input Output Control parameter in the response buffer */
                    pMsgContext->resData[0] = pMsgContext->reqData[0];
                    pMsgContext->resData[1] = pMsgContext->reqData[1];
                    pMsgContext->resData[2] = dataIoCtrlParam_u8;
                    pMsgContext->resDataLen = Dcm_ReadSignalLength_u16+DSP_IOCBI_MINREQLEN;
                    dataServretVal_u8=E_OK;
                }
                else if(dataRetGetDid_u8 == DCM_E_PENDING)
                { /* Do nothing, wait for next call */
                    s_Dcm_idxIocbiDidIndexType_st.dataopstatus_b = DCM_PENDING;
                    dataServretVal_u8=DCM_E_PENDING;
                    Dcm_DidSignalIdx_u16 = s_Dcm_idxIocbiDidIndexType_st.nrNumofSignalsRead_u16;

                    /*Update the DcmDsp_IocbiStatus_array with the state of subfunction which is active */
                    for(idxIocbiIndex_u16=0;idxIocbiIndex_u16<DCM_CFG_NUM_IOCBI_DIDS;idxIocbiIndex_u16++)
                    {
                        ptrIOCBIStatusArrayConfig=&DcmDsp_IocbiStatus_array[idxIocbiIndex_u16];
                        if(s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16 == ptrIOCBIStatusArrayConfig->idxindex_u16)
                        {

                            if(dataIoCtrlParam_u8 == DCM_IOCBI_RETURNCONTROLTOECU)
                            {
                                /*Set the status flag to RCE pending state*/
                                ptrIOCBIStatusArrayConfig->IocbiStatus_en = DCM_IOCBI_RCE_PENDING;
                            }

#if(DCM_CFG_DSP_RESETTODEFAULT_ENABLED  == DCM_CFG_ON)
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_RESETTODEFAULT)
                            {
                                /*Set the status flag to RTD pending state*/
                                ptrIOCBIStatusArrayConfig->IocbiStatus_en = DCM_IOCBI_RTD_PENDING;
                            }
#endif
#if(DCM_CFG_DSP_FREEZECURRENTSTATE_ENABLED  == DCM_CFG_ON)
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_FREEZECURRENTSTATE)
                            {
                                /*Set the status flag to FCS pending state*/
                                ptrIOCBIStatusArrayConfig->IocbiStatus_en = DCM_IOCBI_FCS_PENDING;
                            }
#endif
#if(DCM_CFG_DSP_SHORTTERMADJUSTMENT_ENABLED  == DCM_CFG_ON)
                            if(dataIoCtrlParam_u8 == DCM_IOCBI_SHORTTERMADJUSTMENT)
                            {
                                /*Set the status flag to STA pending state*/
                                ptrIOCBIStatusArrayConfig->IocbiStatus_en = DCM_IOCBI_STA_PENDING;
                            }
#endif
                        }
                    }
                }

                else
                {
                    if(dataRetGetDid_u8 == E_NOT_OK)
                    {
                        /* Invoke the DcmAPPl API to obtain the NRC from the Application*/
                        dataretVal_u8 = DcmAppl_DcmReadDataNRC(Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16].dataDid_u16,Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16].adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16,dataNegRespCode_u8);
                        if(dataretVal_u8!=E_OK)
                        {
                            /* Send negative response code DCM_E_GENERALREJECT as the application has returned E_NOT_OK from ReadNrc applfunction
                             * or has not set the NRC */
                            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                        }
                    }
                    else
                    {
                        *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                    }
                    s_Dcm_idxIocbiDidIndexType_st.nrNumofSignalsRead_u16 = 0x0; /*E_NOT_OK set stop reading the signals, therefore reset the number of singals read to zero*/
                    s_Dcm_idxIocbiDidIndexType_st.dataSignalLengthInfo_u32 = 0x0; /*All the signals not read correctly, therefore reset the signal data length to zero*/
                    s_Dcm_idxIocbiDidIndexType_st.dataopstatus_b = DCM_INITIAL;
                }
                break;
            }

            default:
            { /* IOCBI service is not in a valid state, Send negative response */
                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                break;
            }
        }

        if(*dataNegRespCode_u8 != 0x00 )
        { /* Reset signal index */
            Dcm_DidSignalIdx_u16 = 0x0;
            dataServretVal_u8 = E_NOT_OK;
            Dcm_stDspIocbiState_en = DCM_IOCBI_IDLE;
        }
    }
    return dataServretVal_u8;
}

/**
 **************************************************************************************************
 * Dcm_ResetActiveIoCtrl:
 *
 * Invoke the Return Control to ECU for all the signals of an IOCBI DID.
 *
 * \param     dataSessionMask_u32           :           Session mask in which the IOCBI DID is supported
 *            dataSecurityMask_u32          :           Security mask in which the IOCBI DID is supported
 *            flgSessChkReqd_b              :           Flag to check if session check needs to be done.
 *                                                      For security transition this flag is false,which
 *                                                      indicates no session check necessary
 * \retval
 *  void
 *
 **************************************************************************************************
 */
void Dcm_ResetActiveIoCtrl(uint32 dataSessionMask_u32,
uint32 dataSecurityMask_u32,
boolean flgSessChkReqd_b)
{
    /**The code which would loop through all the IOCBI DIDs and call all the returncontrol to ECU functions to reset all the  active IOctrls
     supported in the current session level*/

    uint16 idxIocbiIndex_u16; /*Index to loop through all the IOCTRL DID*/
    uint16 idxSig_u16; /*Index to loop through all the signals of the DID*/
    Std_ReturnType dataRetIocbiFunc_u8;
    Dcm_NegativeResponseCodeType dataNegResCode_u8; /* Negative response code indicator */
    const Dcm_DIDConfig_tst * ptrDidConfig;
    const Dcm_DataInfoConfig_tst * ptrSigConfig;
    const Dcm_SignalDIDSubStructConfig_tst * ptrIOSigConfig;
    const Dcm_ExtendedDIDConfig_tst * ptrDidExtendedConfig;
    un_RetCntrlEcu  ReturnControlToEcu_un;   /*ReturnControlToEcu Function Pointers union*/
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
    uint8 ControlMask_u8;
    uint16 ControlMask_u16;
    uint32 ControlMask_u32;
    /* control mask with all bits set, application can execute return control for appropriate bit mapped signals */
    ControlMask_u8 = 0xFFu;
    ControlMask_u16 = 0xFFFFu;
    ControlMask_u32 = 0xFFFFFFFFu;
#endif
    /* Initialization of local variables */
    dataRetIocbiFunc_u8 = E_NOT_OK;
    dataNegResCode_u8 = 0x00;
#if( DCM_CFG_DSP_IOCBI_SR_ENABLED != DCM_CFG_OFF)
    un_IOCntrlReqst ControlRequest_un;       /*ControlRequest function pointers union*/
    ControlRequest_un.IOControlrequest_pfct = NULL_PTR;
#endif
	ReturnControlToEcu_un.ReturnControlEcu1_pfct = NULL_PTR;

    for (idxIocbiIndex_u16 = 0; idxIocbiIndex_u16 < DCM_CFG_NUM_IOCBI_DIDS; idxIocbiIndex_u16++)
    {
        /*Check if the DID is active,*/
        /*Check if IOCBI DIDs status is not ideal state and enable the iocontrol to be reset on session/security change */
        if((DCM_IOCBI_IDLESTATE != DcmDsp_IocbiStatus_array[idxIocbiIndex_u16].IocbiStatus_en)                                                                           &&     \
                (Dcm_DIDConfig[DcmDsp_IocbiStatus_array[idxIocbiIndex_u16].idxindex_u16].adrExtendedConfig_pcst->dataIocbirst_b  != FALSE)                                    &&     \
                (((0x0u == ((Dcm_DIDConfig[DcmDsp_IocbiStatus_array[idxIocbiIndex_u16].idxindex_u16].adrExtendedConfig_pcst->dataSecBitMask_u32)  & dataSecurityMask_u32)))   ||     \
                        (((flgSessChkReqd_b) && (0x0u  == ((Dcm_DIDConfig[DcmDsp_IocbiStatus_array[idxIocbiIndex_u16].idxindex_u16].adrExtendedConfig_pcst->dataSessBitMask_u32) & dataSessionMask_u32))))))

        {
            ptrDidConfig = &Dcm_DIDConfig[DcmDsp_IocbiStatus_array[idxIocbiIndex_u16].idxindex_u16];

            /*Loop through all the signals of the IOCBI DID and invoke return control to ECU*/
            for (idxSig_u16 = 0; idxSig_u16 < ptrDidConfig->nrSig_u16; idxSig_u16++)
            {
                ptrSigConfig = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[idxSig_u16].idxDcmDspDatainfo_u16];
                ptrIOSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];

#if( DCM_CFG_DSP_IOCBI_SR_ENABLED != DCM_CFG_OFF)
                if(((USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8) && ((ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER_AS_SERVICE) || (ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER))) || \
                   ((USE_ATOMIC_SENDER_RECEIVER_INTERFACE == ptrDidConfig->didUsePort_u8)||(USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE == ptrDidConfig->didUsePort_u8)))

                {
                    if(FALSE == ptrDidConfig->AtomicorNewSRCommunication_b)
                    {
                        ControlRequest_un= Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].ioControlRequest_cpv;
                    }
                    else
                    {
                        ControlRequest_un= ptrDidConfig->ioControlRequest_cpv;
                    }

                    if(ControlRequest_un.IOControlrequest_pfct != NULL_PTR)
                    {
                        /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                        s_IsIOCBISubfuncCalled_b = TRUE;
                        dataRetIocbiFunc_u8 =(*(ControlRequest_un.IOControlrequest_pfct))(DCM_RETURN_CONTROL_TO_ECU,NULL_PTR,0,0,DCM_INITIAL,&dataNegResCode_u8);
                        /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                        s_IsIOCBISubfuncCalled_b = FALSE;
                    }
                    (void)dataRetIocbiFunc_u8;
                    (void)dataNegResCode_u8;
                }

#endif
                ReturnControlToEcu_un = Dcm_DspIOControlInfo[ptrIOSigConfig->idxDcmDspIocbiInfo_u16].adrReturnControlEcu_cpv;
                ptrDidExtendedConfig = ptrDidConfig->adrExtendedConfig_pcst;
                /*Cheeck if return control to ECU is supported or not for the DID, lower most bit represents return control to ECU*/
                if (((ptrDidConfig->adrExtendedConfig_pcst->statusmaskIOControl_u8 & 0x01u) > 0u)
                        && (ReturnControlToEcu_un.ReturnControlEcu1_pfct != NULL_PTR))
                {
                    s_ActiveDid_u16 = ptrDidConfig->dataDid_u16;
                    /*Set the flag for s_IsIOCBISubfuncCalled is to TRUE*/
                    s_IsIOCBISubfuncCalled_b = TRUE;

                        /*As ReturnControlToEcu is a synchronous API , the same API is invoked for both Synch and ASynch Fnc and ClientServer Configuration */
                    if ((ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER)
                            && (ptrSigConfig->usePort_u8 != USE_DATA_SENDER_RECEIVER_AS_SERVICE))
                    {
                        if (ptrDidExtendedConfig->dataCtrlMask_en != DCM_CONTROLMASK_EXTERNAL)
                        {
                            /* Return Control to ECU is requested */
                            /* Call the configured API */
                            dataRetIocbiFunc_u8 = (*(ReturnControlToEcu_un.ReturnControlEcu1_pfct))(&dataNegResCode_u8);
                        }
                        else
                        {
#if(DCM_CFG_DSP_CONTROLMASK_EXTERNAL_ENABLED == DCM_CFG_ON)
                            if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==1)
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ReturnControlToEcu_un.ReturnControlEcu3_pfct)) (ControlMask_u8,&dataNegResCode_u8);
                            }
                            else if(ptrDidExtendedConfig->dataCtrlMaskSize_u8==2)
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ReturnControlToEcu_un.ReturnControlEcu4_pfct)) (ControlMask_u16,&dataNegResCode_u8);
                            }
                            else
                            {
                                /* Short Term Adjustment was requested */
                                /* Call the configured API  with Opstatus as DCM_CANCEL */
                                dataRetIocbiFunc_u8=(*(ReturnControlToEcu_un.ReturnControlEcu5_pfct)) (ControlMask_u32,&dataNegResCode_u8);
                            }
#endif
                        }
                    }

                    /*if the resetting the IOcontrol was successful, reset the status array to DCM_IOCBI_IDLESTATE*/
                    if (E_OK == dataRetIocbiFunc_u8)
                    {
                        /*Update the Iocbistatus arrays status with DCM_IOCBI_IDLESTATE  when E_OK is returned from return control to ecu function pointer*/
                        DcmDsp_IocbiStatus_array[idxIocbiIndex_u16].IocbiStatus_en = DCM_IOCBI_IDLESTATE;
                    }
                    /*Set the flag for s_IsIOCBISubfuncCalled is to FALSE*/
                    s_IsIOCBISubfuncCalled_b = FALSE;
                    (void) dataNegResCode_u8;
                }
            }/*End of idxSig_u16 for loop*/
        }/*End of IF condition to check if IOCBI did is active*/

    }/*End of idxIocbiIndex_u16 for loop*/

    (void) dataSessionMask_u32;
    (void) dataSecurityMask_u32;
    (void) flgSessChkReqd_b;
}

/**
 ****************************************************************************************************************************
 * Dcm_GetLengthOfSignal:
 *
 * Get the read data length of the current active signal for IOCBI request
 *
 * \param     dataSigLength_u16 :   Pointer to hold the length of the signal
 * \retval     E_OK: calculation finished successfully
 *             E_NOT_OK: error in configuration or in the called length calculating function
 *             DCM_INFRASTRUCTURE_ERROR : Infrastructure error occured
 * \seealso
 *
 ****************************************************************************************************************************
 */

Std_ReturnType Dcm_GetLengthOfSignal(uint16 * dataSigLength_u16)
{
    uint32 dataSigLength_u32;
    Std_ReturnType dataRetVal_u8; /* Local variable to store return value */
    const Dcm_DIDConfig_tst * ptrDidConfig;
    const Dcm_DataInfoConfig_tst * ptrSigConfig;
    const Dcm_SignalDIDSubStructConfig_tst * ptrIOSigConfig;
    un_ReadDataLength ptrReadDataLenFnc; /*Read Data Length function pointers union*/

    /* Initialisations */
    dataRetVal_u8 = E_NOT_OK;
	ptrReadDataLenFnc.ReadDataLengthFnc1_pf = NULL_PTR;
    *dataSigLength_u16 = 0x0;
    ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxIocbiDidIndexType_st.idxIndex_u16];
    if((ptrDidConfig->didUsePort_u8 == USE_ATOMIC_SENDER_RECEIVER_INTERFACE) || (ptrDidConfig->didUsePort_u8 == USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE))
    {
        while(Dcm_DidSignalIdx_u16 < ptrDidConfig->nrSig_u16)
        {
            ptrSigConfig= &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
#if(DCM_CFG_DSP_CONTROL_FIXED_LEN_ENABLED != DCM_CFG_OFF)
            /*If the interface is RTE AR4.x retain the signal size in bits, else convert it to bytes*/
            *dataSigLength_u16 += ptrSigConfig->dataSize_u16;
#endif
            Dcm_DidSignalIdx_u16++;
        }
        Dcm_DidSignalIdx_u16=0x0;
        dataRetVal_u8 = E_OK;
    }
    else
    {
        ptrSigConfig= &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
        ptrIOSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];
        ptrReadDataLenFnc = ptrIOSigConfig->adrReadDataLengthFnc_pfct;

        /* Check for USE FUNC and valid Read Data length function configuration */
        if((ptrSigConfig->idxDcmDspControlInfo_u16 >0)&& (ptrReadDataLenFnc.ReadDataLengthFnc1_pf != NULL_PTR))
        {
#if(DCM_CFG_DSP_CONTROL_VAR_LEN_ENABLED != DCM_CFG_OFF)
            /* Get the length from the configured API */
            if((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC) || (ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER))
            {
                dataRetVal_u8 = (*(ptrReadDataLenFnc.ReadDataLengthFnc1_pf))(dataSigLength_u16);
            }
            else
            {
                if ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC) || (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER))
                {
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
                    if ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                    {
                        if(!s_DcmReadLengthRteCallPlaced_b)
                        {
                            dataRetVal_u8 = (*(ptrReadDataLenFnc.ReadDataLengthFnc6_pf))(s_Dcm_idxIocbiDidIndexType_st.dataopstatus_b);
                            if(dataRetVal_u8 == E_OK)
                            {
                                /* Set the Rte_Call Flag to TRUE and return Pending so that Rte_Result will be invoked in the next cycle */
                                s_DcmReadLengthRteCallPlaced_b = TRUE;
                                dataRetVal_u8 = DCM_E_PENDING;
                            }
                        }
                        else
                        {
                            dataRetVal_u8 = (*(ptrIOSigConfig->adrReadDataLengthFncResults_pfct.ReadDataLenResultFnc1_pf))(dataSigLength_u16);

                            if(dataRetVal_u8 == E_OK)
                            {
                                s_DcmReadLengthRteCallPlaced_b = FALSE;
                            }
                            else if(dataRetVal_u8 == RTE_E_NO_DATA)
                            {
                                dataRetVal_u8 = DCM_E_PENDING;
                            }
                            else
                            {
                                /* For any return value other than E_OK , RTE_E_NO_DATA ,  reset the Flag */
                                s_DcmReadLengthRteCallPlaced_b = FALSE;
                            }
                        }
                    }
                    else
#endif
                    {
                        dataRetVal_u8 = (*(ptrReadDataLenFnc.ReadDataLengthFnc4_pf))(s_Dcm_idxIocbiDidIndexType_st.dataopstatus_b,dataSigLength_u16);
                    }
                }
            }
            if(dataRetVal_u8==E_OK)
            {
                /* If the length received in more than the configured maximum length for a lengththen return error */
                if((*dataSigLength_u16 > ptrSigConfig->dataSize_u16) ||(*dataSigLength_u16==0))
                {
                    dataRetVal_u8 = E_NOT_OK;
                }
            }
            else if((dataRetVal_u8 == DCM_E_PENDING) && ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC) || (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)))
            {
            }
            else
            {
                /* Check for infrastructural errors in case of RTE*/

                if((Dcm_IsInfrastructureErrorPresent_b(dataRetVal_u8) != FALSE)&&((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER) || (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER)))
                {
                    dataRetVal_u8=DCM_INFRASTRUCTURE_ERROR;
                }
            }
#endif
        }
        else
        {
#if(DCM_CFG_DSP_CONTROL_FIXED_LEN_ENABLED != DCM_CFG_OFF)
            /*If the interface is RTE AR4.x retain the signal size in bits, else convert it to bytes*/
            *dataSigLength_u16 = ptrSigConfig->dataSize_u16;
            dataRetVal_u8 = E_OK;
#endif
        }
    }

    if(dataRetVal_u8 == E_OK)
    {
        dataSigLength_u32=*dataSigLength_u16;
        *dataSigLength_u16=(uint16)dataSigLength_u32;
    }
    return dataRetVal_u8;
}

/**
 *************************************************************************************************************************************************************
 * Dcm_SetFlagforIOCBI:
 *
 * This API sets/resets the flag which is required for getting the active DID based on the input parameter
 *
 *
 * \param     boolean isFlag_b : parameter for setting or resetting the flag to get the active DID
 * \retval    None
 * \seealso
 *
 **************************************************************************************************************************************************************
 */
static void Dcm_SetFlagforIOCBI(boolean isFlag_b)
{
    /* Setting/resetting the flag so that the application can call GetActiveDID
	   function from DcmAppl_DcmConfirmation */
    s_IsIOCBISubfuncCalled_b = isFlag_b;
}

/**
 *******************************************************************************
 * Dcm_Prv_DspIOCBIConfirmation : API used for confirmation of response sent for
 *							      InputOutputControlByIdentifier (0x2F) service.
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
 void Dcm_Prv_DspIOCBIConfirmation(
 Dcm_IdContextType dataIdContext_u8,
 PduIdType dataRxPduId_u8,
 uint16 dataSourceAddress_u16,
 Dcm_ConfirmationStatusType status_u8)
{
        if(dataIdContext_u8 == DCM_DSP_SID_INPUTOUTPUTCONTROLBYIDENTIFIER)
        {

            Dcm_SetFlagforIOCBI(TRUE);
        }
DcmAppl_DcmConfirmation(dataIdContext_u8,dataRxPduId_u8,dataSourceAddress_u16,
																	status_u8);
        if(dataIdContext_u8 == DCM_DSP_SID_INPUTOUTPUTCONTROLBYIDENTIFIER)
        {
            Dcm_SetFlagforIOCBI(FALSE);
        }
}

/**
 *************************************************************************************************************************************************************
 * Dcm_GetActiveIOCBIDid:
 *
 * This API returns the current active DID. This function is called from the Dcm API Dcm_GetActiveDid.
 *
 *
 * \param     uint16* dataDid_u16 : Parameter for updating of the DID under processing. The DID value returned is valid only if return value is E_OK.
 * \retval    Std_ReturnType : E_OK : The DID under processing is a normal DID.The parameter dataDid_u16 contains valid DID value in this case.
 *                             E_NOT_OK: The DID under processing is a range DID. The parameter dataDid_u16 contains invalid data in this case.
 *                             Also if the API is called from any other Appl function other than IO control parameter functions, E_NOT_OK shall be returned.
 * \seealso
 *
 **************************************************************************************************************************************************************
 */
Std_ReturnType Dcm_GetActiveIOCBIDid(uint16 * dataDid_u16)
{

    /* Update the DID value currently under process */
    /*Check if s_IsIOCBISubfuncCalled is set to TRUE*/
    if (s_IsIOCBISubfuncCalled_b != FALSE)
    {
        *dataDid_u16 = s_ActiveDid_u16;
        return (E_OK);
    }
    else
    {
        /* if this API is called from anywhere else, other than from the supported sub-functions */
        return E_NOT_OK;
    }
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif      /* #if (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF) */

/* FC_VariationPoint_START */
/*<BASDKey>
 **********************************************************************************************************************
 * $History___:
 *
 * AR40.11.0.0; 1     07.01.2016 MER3KOR
 *   CSCRM01026022
 *
 * AR40.11.0.0; 0     21.12.2015 MER3KOR
 *   CSCRM1014809
 *
 * AR40.10.0.0; 5     14.07.2015 WUG3ABT
 *   Checkout by wug3abt
 *
 * AR40.10.0.0; 4     01.07.2015 GET1COB
 *   CSCRM00788438
 *
 * AR40.10.0.0; 3     14.05.2015 CRA4KOR
 *   CSCRM00737827
 *
 * AR40.10.0.0; 2     14.05.2015 CRA4KOR
 *   CSCRM00836371
 *
 * AR40.10.0.0; 1     17.02.2015 PJR4KOR
 *   CSCRM00649704
 *
 * AR40.10.0.0; 0     13.02.2015 GET1COB
 *   CSCRM00747026
 *
 * AR40.9.0.0; 3     30.12.2014 GET1COB
 *   CSCRM00724044
 *
 * AR40.9.0.0; 2     15.11.2014 GET1COB
 *   CSCRM00726622
 *
 * $
 **********************************************************************************************************************
 </BASDKey>*/

/* FC_VariationPoint_END */

