

/***************************************************************************************************
*                              Include files                                                       *
***************************************************************************************************/
/* CanIf priv header file */
#include "CanIf_Prv.h"

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */


/****************************************************************************************************
 *                              Function Definition                                                 *
 ****************************************************************************************************/

/**********************************************************************************************************************
** Function name     : CanIf_RxIndication_Internal                                                                   **
** Syntax            : void CanIf_RxIndication_Internal(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr)    **
** Service ID[hex]   : 0x14                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : Mailbox, PduInfoPtr                                                                           **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : This service indicates a successful reception of a received CAN Rx LPDU to the CanIf after    **
**                     passing all filters and validation checks.                                                    **
**********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* HIS METRIC PATH,v(G),STMT,LEVEL,RETURN VIOLATION IN CanIf_RxIndication_Internal: Function contains very simple "else if" statements and "switch-cases".
 * HIS metric compliance would decrease readability and maintainability. Also Function contains more than one return statement.
 * This is needed mainly because of DET. */
/* BSW-415 */
/* [$DD_BSWCODE 1621] */
void CanIf_RxIndication_Internal( const Can_HwType * Mailbox,
        const PduInfoType * PduInfoPtr)
{
    #if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)

    /* Pointer to HRH configuration */
    const CanIf_Cfg_Hrhtype_tst * HrhCfg_pcst;

    /* Pointer to RxPdu configuration */
    const CanIf_Cfg_RxPduType_tst * RxPduCfg_pcst;

    #if CANIF_CFG_UL_RX == STD_ON
    /*Local variable to hold data for UL */
    PduInfoType ULPduInfoTyp_tst;
    #endif

    /*Local variable to hold data and dlc */
    PduInfoType PduInfoTemp_tst;

    /*Local variable to hold Hrh Id */
    uint16 HrhId_u16;

    /*Local pointer to controller structure */
#if (CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON)
    const CanIf_Cfg_CtrlConfig_tst * lCtrlConfig_pst;
#endif
    #if CANIF_CFG_USER_RX_ENABLED == STD_ON
    /*Local variable to hold data for USER UL */
    CanIf_PduInfoType ULPduInfoTypUSER_tst;
    #endif

    /*variable used as an index for looping conditions */
    uint8 Index_u8;

    /*variable used as a status current channel mode */
    uint8 ModeChkStatus = CANIF_PRV_INVALID_CHANNEL_MODE;

    /*variable to save pduId */
    PduIdType PduId_qu8 =0;

    /*variable to save pduId for Range or list structure */
    #if (CANIF_BASIC_CAN_SUPPORT == STD_ON)
    uint8_least RngLstPduId_qu8;
    #endif

    /*store canid in the local variable removing Idtype information */
    Can_IdType CanIdNoIdType_t;

/* local variable to store Lower and upper range of CAN Identifier of a receive CAN L-PDU*/
#if ((CANIF_BASIC_CAN_SUPPORT == STD_ON) && (CANIF_CFG_RX_RANGE_CONFIG == STD_ON))
#if (CANIF_RB_CALIBRATION == STD_ON)
    Can_IdType CanIdNoIdType_lower;
    Can_IdType CanIdNoIdType_upper;
#endif
#endif

/*local variable to store CAN Identifier of a receive CAN L-PDU */
#if (CANIF_RB_CALIBRATION == STD_ON)
    Can_IdType CanIdNoIdType_get;
#endif

    /* To store the status of search algorithm */
    boolean IdFound_b = FALSE;

    /*Extracting 2MSB's of recieved CanId */
     uint8 CanIdPduType_u8;
#if( CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON || CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON )
     /* Local variable to store controller CustId */
     uint8 lCtrlCustId_u8;
#endif
     /* Pointer to Controller State configuration */
     CanIf_ControllerStateType * ControllerState_ps;

     /*Local pointer to hold the data of range type of Pdu */

    #if CANIF_CFG_RX_RANGE_CONFIG == STD_ON
     const CanIf_RxPduRangeConfigType_tst * CanIdRangeCfg_pcs;
    #endif
     /* FC_VariationPoint_START */
    #if CANIF_CFG_HRH_LIST_ENABLED == STD_ON
     const CanIf_RxPduListConfigType_tst * CanIdListCfg_pcs;
    #endif
     /* FC_VariationPoint_END */

    #if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
    /*Pointer to store address of Dynamic RxPduCanId array*/
    uint32 * dynRxPduId_pu32;

    uint32 rxcanid_u32 = 0;
    #endif

    #if(CANIF_CFG_DLC_CHECK == STD_ON)
    const CanIf_CallbackFuncType * CallBackPtr_Temp;
	/* This variable holds Dlc value and used for DLC checks */
    PduLengthType lConfigRxPduDlc;
    #endif
    #if(CANIF_METADATA_SUPPORT == STD_ON)

    /*Data buffer to store payload data and metadata canid bytes*/
    uint8 SduDataBuffer_au8[CANIF_SDU_DATA_BUFF_SIZE] = {0};

    #endif

    #if(CANIF_CANNM_TXID_FILTER != STD_OFF)
    const Can_IdType * CanNmTxId_pt;
    #endif


    /* FC_VariationPoint_START */
    #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
    uint8 localCoreId_u8 ;
    uint8 destCoreId_u8;
    uint16 pipeId_u16 ;
    CanIf_XCore_PipeTxRxParams_st xCorePipeTxRxParams;
    #endif
    /* FC_VariationPoint_END */

    #if (STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API)
    CanIf_NotifStatusType * RxNotifPtr_pen;

    RxNotifPtr_pen = CanIf_Prv_RxNotification_taen;

    #endif

    #if(CANIF_CFG_DLC_CHECK == STD_ON)
    /*Initializing callback pointer */
    CallBackPtr_Temp = &CanIf_Callback;
    #endif

    /* Det to check for correct inputs */
    /*[SWS_CANIF_00661] Det should be invoked if module is not initialized */
    CANIF_DET_REPORT_ERROR_VOID_RET(CanIf_Prv_InitStatus_b == FALSE, CANIF_RX_INDICATION_SID, CANIF_E_UNINIT);

    /*[SWS_CANIF_00419][CanIf_Ext-3484] Report Det if pointers are invalid */
    CANIF_DET_REPORT_ERROR_VOID_RET(((Mailbox == NULL_PTR) || (PduInfoPtr == NULL_PTR) ||((PduInfoPtr != NULL_PTR) &&
            (PduInfoPtr->SduDataPtr == NULL_PTR))),CANIF_RX_INDICATION_SID, CANIF_E_PARAM_POINTER);

    /*[CanIf_Ext-3483] Report DET if ControllerId is invalid */
    CANIF_DET_REPORT_ERROR_VOID_RET(Mailbox->ControllerId>= CANIF_TOTAL_CTRLS ,CANIF_RX_INDICATION_SID, CANIF_E_PARAM_POINTER);

    /*Fetching controller Information to local variable */
    ControllerState_ps= CanIf_Prv_ControllerState_ast + Mailbox->ControllerId;

#if( CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON || CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON )
    /* Obtain the custId for the requested controller in the selected variant */
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[Mailbox->ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((lCtrlCustId_u8 == 0xFFu), CANIF_RX_INDICATION_SID, CANIF_E_PARAM_CONTROLLERID)

#if( CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON)
    /*Fetch the controller details of Pdu mapped to the specific controller */

    lCtrlConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + lCtrlCustId_u8;
#endif
    /*[SWS_CANIF_00416] Report Det if Hoh is invalid
     * Checking if the received Hoh is of receive type and
     * checking if the hoh obtained is less than the total configure hoh*/
    CANIF_DET_REPORT_ERROR_VOID_RET((Mailbox->Hoh>=CANIF_CFG_TOTAL_HOH_NUM), CANIF_RX_INDICATION_SID, CANIF_E_PARAM_HOH);

    #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
    HrhId_u16 = Mailbox->Hoh + (CanIf_Prv_ConfigSet_tpst->CanCtrlDrvCont_Ptr[lCtrlConfig_pst->CanDrvIndx].CanDrvOffSet);
    #else
    HrhId_u16 = Mailbox->Hoh;
    #endif
    /*Fetch CanId without CanIdType*/
    CanIdNoIdType_t = Mailbox->CanId & CANIF_PRV_MSB_BIT_RESET;

    /*[SWS_CANIF_00416] Report Det if Hoh is invalid
     * Checking if the received Hoh is of receive type and
     * checking if the hoh obtained is less than the total configure hoh*/
    CANIF_DET_REPORT_ERROR_VOID_RET((CanIf_Prv_ConfigSet_tpst->HrhPduIdTable_Ptr[HrhId_u16] ==  CANIF_INVALID_ID), CANIF_RX_INDICATION_SID, CANIF_E_PARAM_HOH);

    /*Extracting 2MSB's of recieved CanId */
    /* MR12 RULE 10.3 VIOLATION:The Warning is "Not Critical". The left operand of shift operator is lower in size
     * compared to right operand, as the left operand is only used to store two bits for which unit8 is sufficient  */
    CanIdPduType_u8 = (uint8)((Mailbox->CanId)>>CANIF_CANID_BIT_SHIFT);

    /*[SWS_CANIF_00417] Report Det if canid is invalid */
    CANIF_DET_REPORT_ERROR_VOID_RET((((CanIdPduType_u8 == CANIF_ZERO)||(CanIdPduType_u8 == CANIF_ONE))&& (CanIdNoIdType_t > CANIF_CAN_STD_VAL))||
                    (((CanIdPduType_u8 == CANIF_TWO)||(CanIdPduType_u8 == CANIF_THREE))&& (CanIdNoIdType_t > CANIF_CAN_XTD_VAL)), CANIF_RX_INDICATION_SID, CANIF_E_PARAM_CANID);

    /*[CanIf_Ext-3486]Report Det if Dlc is invalid*/
    CANIF_DET_REPORT_ERROR_VOID_RET((((CanIdPduType_u8 == CANIF_ZERO)||(CanIdPduType_u8 == CANIF_TWO))&& (PduInfoPtr->SduLength > 8u))||
                        (((CanIdPduType_u8 == CANIF_ONE)||(CanIdPduType_u8 == CANIF_THREE))&& (PduInfoPtr->SduLength > 64u)), CANIF_RX_INDICATION_SID, CANIF_E_DATA_LENGTH_MISMATCH);

    /* Get the pointer to desired hrh
     * Multi driver implementation needs to be taken care*/
    HrhCfg_pcst = CanIf_Prv_ConfigSet_tpst->HrhConfig_pcst + CanIf_Prv_ConfigSet_tpst->HrhPduIdTable_Ptr[HrhId_u16];
    #if(CANIF_CANNM_TXID_FILTER != STD_OFF)
    CanNmTxId_pt = CanIf_Prv_ConfigSet_tpst->NmtxIdPtr;
    #endif





    #if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON && CANIF_PUBLIC_BUSOFF_RECOVERY_FROM_RXINDICATION == STD_ON )

    /* Set Tx Confirmation state for the controller*/
    ControllerState_ps->CanIf_TxCnfmStatus = CANIF_TX_RX_NOTIFICATION;
    #endif


    /*[SWS_CANIF_00896-AR430] this requirement will be updated when PN id done in CanIf_Transmit */
    #if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    if(ControllerState_ps->BlPnTxFilter == TRUE)
    {
       ControllerState_ps->BlPnTxFilter = FALSE;
    }
    #endif

    /*Initialise Rxconfig pointer */
      RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst;


    /* Check the controller mode is valid */
    if((uint8)CANIF_CS_STARTED == (((ControllerState_ps->Ctrl_Pdu_mode)& CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT))
    {
        /*Check if the channel mode is enabled in receive path*/
        if(((uint8)CANIF_TX_OFFLINE == ((ControllerState_ps->Ctrl_Pdu_mode)& CANIF_BIT_MASK_CTRL_MODE))||((uint8)CANIF_ONLINE == ((ControllerState_ps->Ctrl_Pdu_mode)& CANIF_BIT_MASK_CTRL_MODE)))
        {
            ModeChkStatus = CANIF_PRV_VALID_CHANNEL_MODE;
        }
        /*For WakeUpValidation the channel mode shall be disabled for Rx Path */
        #if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
        else
        {
            ModeChkStatus = CANIF_PRV_WAKEUP_MODE;
        }
        #endif

        /*Filtering is needed if the channel mode is valid or is for WakeupValidation*/
        #if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_OFF)
        if(ModeChkStatus == CANIF_PRV_VALID_CHANNEL_MODE)
        #endif
        {
            /*Initializing the index for the loop */
            Index_u8 = 0;
            /*Software filtering to be performed */
            switch(HrhCfg_pcst->HrhInfo_e)
            {
                /*FULL CAN Hrh */
                case CANIF_PRV_FULL_E:
                {
                    /*Obtain Index of Rxpdu */
                    PduId_qu8 = HrhCfg_pcst->PduIdx_t;


                    /* [SWS_CANIF_00877] Check for correct recieved CanId type and CanId */
                    /* MR12 RULE 15.4 VIOLATION:The Warning is "Not Critical". Multiple break points are used for optimal
                     * operation of filtering feature*/
                    do{
                        /* Get the pointer to desired Rx pdu */
                        RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + PduId_qu8;

                        #if(CANIF_RB_CHANGERXPDUID_API != STD_OFF)
                        /* Check if the CanRxPduIdType is configured as Dynamic */
                        if((RxPduCfg_pcst->RxPduType) != CANIF_STATIC)
                        {

                            /* Get the address of DynamicRxPduCanId array which stores Dynamic RxPdu CanId.*/
                            dynRxPduId_pu32 = &CanIf_DynRxPduCanId_au32[0];

                            /*Get the CanId value from DynamicRxPduCanId array*/
                            rxcanid_u32 = (Can_IdType)*(dynRxPduId_pu32 + (RxPduCfg_pcst->RxPduType));

                            /* Remove the CanIdtype info(MSB bits) and get the CanId Value */
                            rxcanid_u32 = rxcanid_u32 & CANIF_PRV_MSB_BIT_RESET;

                            /* Check if the received CanIdType is matching with configured CanIdType and the received CanId is same as the CanId present the DynamicRxPduCanId array */
                            if((((RxPduCfg_pcst->CanIdtype_u8) &(CANIF_PRV_CANID_TYPE_MASK))==(((RxPduCfg_pcst->CanIdtype_u8)>>CANIF_PRV_FOUR)& CanIdPduType_u8)) && (CanIdNoIdType_t == rxcanid_u32))
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                                break;
                            }
                        }
                        else
                        #endif
                        {
                            #if (CANIF_RB_CALIBRATION == STD_ON)

                            CanIdNoIdType_get = RxPduCfg_pcst->getCanId();
                            /* MR12 RULE 13.5 VIOLATION:The Warning is "Not Critical". Brackets are used correctly to
                             * prevent any side effects of binary operator precedence */
                            if((((RxPduCfg_pcst->CanIdtype_u8)&(CANIF_PRV_CANID_TYPE_MASK))==(((RxPduCfg_pcst->CanIdtype_u8)>>CANIF_PRV_FOUR)& CanIdPduType_u8))&&(CanIdNoIdType_t == CanIdNoIdType_get))
                            #else
                            if((((RxPduCfg_pcst->CanIdtype_u8)&(CANIF_PRV_CANID_TYPE_MASK))==(((RxPduCfg_pcst->CanIdtype_u8)>>CANIF_PRV_FOUR)& CanIdPduType_u8))&&(CanIdNoIdType_t == RxPduCfg_pcst->RxPduCanId))
                            #endif
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                                break;
                            }
                        }

                        PduId_qu8++;
                        Index_u8++;
                    }while(HrhCfg_pcst->NumRxPdus_u32> Index_u8);
                }break;
                #if (CANIF_BASIC_CAN_SUPPORT == STD_ON)
                #if CANIF_CFG_RX_RANGE_CONFIG == STD_ON
                case CANIF_PRV_BASIC_RANGE_E:
                {
                    RngLstPduId_qu8 = HrhCfg_pcst->PduIdx_t;

                    /* [SWS_CANIF_00877] Check for correct recieved CanId type and CanId */
                    while((HrhCfg_pcst->NumRxPdus_u32)> Index_u8)
                    {
                        CanIdRangeCfg_pcs = CanIf_Prv_ConfigSet_tpst->RangeCfg_tpst + RngLstPduId_qu8;
                     #if (CANIF_RB_CALIBRATION == STD_ON)
                        CanIdNoIdType_lower = CanIdRangeCfg_pcs->getLowerCanId_t();
                        CanIdNoIdType_upper = CanIdRangeCfg_pcs->getUpperCanId_t();
                    #endif
                        /*Check if the filtering is of range or list type
                         * true: mask based filtering
                         * false: range based filtering */
                        if(HrhCfg_pcst->HrhRangeMask_b == TRUE)
                        {
                            /* Here CanIdRangeCfg_pcs->UpperCanId is RxPduCanIdMask and
                             *      CanIdRangeCfg_pcs->LowerCanId is RxPduCanId */
                            #if (CANIF_RB_CALIBRATION == STD_ON)
                            if((CanIdNoIdType_t & CanIdNoIdType_upper) == (CanIdNoIdType_upper & CanIdNoIdType_lower))
                            #else
                            if((CanIdNoIdType_t & CanIdRangeCfg_pcs->UpperCanId_t) == (CanIdRangeCfg_pcs->UpperCanId_t & CanIdRangeCfg_pcs->LowerCanId_t))
                            #endif
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                            }
                        }
                        else
                        {
                            #if (CANIF_RB_CALIBRATION == STD_ON)
                            if((CanIdNoIdType_t >= CanIdNoIdType_lower) && (CanIdNoIdType_t<= CanIdNoIdType_upper))
                            #else
                            if((CanIdNoIdType_t >= CanIdRangeCfg_pcs->LowerCanId_t) && (CanIdNoIdType_t<= CanIdRangeCfg_pcs->UpperCanId_t))
                            #endif
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                            }
                        }
                        if( TRUE == IdFound_b )
                        {
                            /* Get the Rx L-Pdu Id from the CanId Range configuration */
                            PduId_qu8 = CanIdRangeCfg_pcs->PduIdx_t;

                            /* Initialize pointer to Rx L-PDU structure */
                            RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + PduId_qu8;

                            /*Check If the type is not matching */
                            if(((RxPduCfg_pcst->CanIdtype_u8) & CANIF_PRV_CANID_TYPE_MASK) == (((RxPduCfg_pcst->CanIdtype_u8)>> CANIF_PRV_FOUR) & CanIdPduType_u8))
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                                break;
                            }
                            else
                            {
                                Index_u8++;

                                /* The Rx L-Pdu is not found */
                                IdFound_b = FALSE;


                                RngLstPduId_qu8++;
                            }
                        }
                        else
                        {
                            Index_u8++;

                             RngLstPduId_qu8++;
                        }
                    }
                }
                break;
                #endif
                /* FC_VariationPoint_START */
                #if CANIF_CFG_HRH_LIST_ENABLED == STD_ON
                case CANIF_PRV_BASIC_LIST_E:
                {
                    RngLstPduId_qu8 = HrhCfg_pcst->PduIdx_t;

                    /* [SWS_CANIF_00877] Check for correct recieved CanId type and CanId */
                    while((IdFound_b == FALSE) && (HrhCfg_pcst->NumRxPdus_u32> Index_u8))
                    {
                        CanIdListCfg_pcs = CanIf_Prv_ConfigSet_tpst->ListCfg_tpst + RngLstPduId_qu8;

                        /* Check whether the CanId received is same as configured */
                        #if (CANIF_RB_CALIBRATION == STD_ON)
                        if(CanIdNoIdType_t == CanIdListCfg_pcs->getCanIdLst_t())
                        #else
                        if(CanIdNoIdType_t == CanIdListCfg_pcs->CanId_t)
                        #endif
                        {
                            PduId_qu8 = CanIdListCfg_pcs->PduIdx_t;

                            /* Initialize pointer to Rx L-PDU structure */
                            RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + PduId_qu8;

                            /* validate if the canId type of recievied id is same as configured */
                            if(((RxPduCfg_pcst->CanIdtype_u8) & CANIF_PRV_CANID_TYPE_MASK) == (((RxPduCfg_pcst->CanIdtype_u8)>> CANIF_PRV_FOUR) & CanIdPduType_u8))
                            {
                                IdFound_b = TRUE;
                            }
                            else
                            {
                                IdFound_b = FALSE;
                                RngLstPduId_qu8++;
                                Index_u8++;
                            }
                        }
                        else
                        {
                            IdFound_b = FALSE;
                            RngLstPduId_qu8++;
                            Index_u8++;
                        }
                    }
                }
                break;
                #endif
                #endif
                /* FC_VariationPoint_END */
                default:

                    /* The Rx L-Pdu is not found */
                    IdFound_b = FALSE;
                    break;
             }/* end of switch(HrhCfg_pcst->HrhInfo_e) */
        }/*End of if(ModeChkStatus == CANIF_PRV_VALID_CHANNEL_MODE)...*/
    }/* end of if(CANIF_CS_STARTED == ControllerMode) */

    /*[SWS_CANIF_00389] If Software Filtering rejects the received L-PDU, the CanIf shall end the receive indication */
    if(IdFound_b == TRUE)
    {
        /*[SWS_CANIF_00286] Store First Valid Call of CanIf_RxIndication_Internal()*/
        #if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
        if(ModeChkStatus==  CANIF_PRV_WAKEUP_MODE)
        {
            #if(CANIF_PUBLIC_WAKEUP_CHECK_VALID_BY_NM == STD_ON)
            if(RxPduCfg_pcst->ULisCanNm_b == TRUE)
            #endif
            {
                ControllerState_ps->ValidationSts_b = CANIF_SET;
            }
        }
        else
        #endif
        {
            /* Filtering of CanId if it belongs to CANNM */
            #if(CANIF_CANNM_TXID_FILTER != STD_OFF)
            /* Check if the received CanId is among the CanNm transmit CanIds */
            if((RxPduCfg_pcst->IndexForUL_u8 == CANIF_ONE) && (CanNmTxId_pt != NULL_PTR) && (*(CanNmTxId_pt + (RxPduCfg_pcst->RxPduTargetId_t)) == CanIdNoIdType_t))
            {
                IdFound_b = FALSE;
            }
            if(IdFound_b==TRUE)
            #endif
            {
                /*[SWS_CANIF_00390] DLC check shall be processed after software filtering is accepted */
                #if ( CANIF_CFG_DLC_CHECK == STD_ON )
               RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + PduId_qu8;

                /* Get the config RxPdu DLC */
                #if (CANIF_RB_CALIBRATION == STD_ON)
                if( RxPduCfg_pcst->getRxPduDlc != NULL_PTR )
                {
                    lConfigRxPduDlc = RxPduCfg_pcst->getRxPduDlc();
                }
                else
                #endif
                {
                    lConfigRxPduDlc = RxPduCfg_pcst->RxPduDlc_u8;
                }

                /* FC_VariationPoint_START */
                /* Check whether the Dlc error function has been configured */
                if( (PduInfoPtr->SduLength < lConfigRxPduDlc) && (NULL_PTR != CallBackPtr_Temp->Dlc_Error_Notification) )
                {
                    /* Report DLC error */
                    /* MR12 RULE 10.3 VIOLATION:The Warning is "Not Critical". The operand SduLength is reduced in size to uint8. The length of pdu will be maximum 64 which is covered in uint8. Thus there is no issue in reducing the size. */
                    (void)CallBackPtr_Temp->Dlc_Error_Notification(PduId_qu8, (uint8)(PduInfoPtr->SduLength));
                }
                /* FC_VariationPoint_END */

                /* [SWS_CANIF_00168] Det should be reported if Dlc check fails */
                CANIF_DET_REPORT_ERROR_VOID_RET((PduInfoPtr->SduLength < lConfigRxPduDlc), CANIF_RX_INDICATION_SID, CANIF_E_INVALID_DLC)

                if( PduInfoPtr->SduLength >= lConfigRxPduDlc )
                #endif /* End of #if(CANIF_CFG_DLC_CHECK == STD_ON) */
                {

                    /* [SWS_CANIF_00844] The L-SDU length is set to the sum of the payload length and MetaDataLength.*/
                    #if(CANIF_METADATA_SUPPORT == STD_ON)
                    PduInfoTemp_tst.SduLength = PduInfoPtr->SduLength + RxPduCfg_pcst->MetadataLength_u8;

                    #else
                    PduInfoTemp_tst.SduLength = PduInfoPtr->SduLength;

                    #endif
                    #if CANIF_CFG_USER_RX_ENABLED == STD_ON
                    ULPduInfoTypUSER_tst.SduLength= PduInfoTemp_tst.SduLength;
                    #endif
                    #if CANIF_CFG_UL_RX == STD_ON
                    ULPduInfoTyp_tst.SduLength= PduInfoTemp_tst.SduLength;
                    #endif


                    #if(CANIF_METADATA_SUPPORT == STD_ON)

                    /*copy the payload data to SduDataBuffer as per DLC*/
                    for(Index_u8 =0; Index_u8 < PduInfoPtr->SduLength; Index_u8++)
                    {
                        SduDataBuffer_au8[Index_u8] = PduInfoPtr->SduDataPtr[Index_u8];
                    }

                    if(RxPduCfg_pcst->MetadataLength_u8 >= 1u)
                    {
                        /*[SWS_CANIF_00844]copy the received canid bytes into SduDataBuffer with LSB byte first after the payload data*/
                        for(Index_u8 = (uint8)(PduInfoPtr->SduLength); Index_u8 < (uint8)(PduInfoTemp_tst.SduLength); Index_u8++)
                        {
                            /* MR12 RULE 10.4, 10.1 VIOLATION:The Warning is "Not Critical". The left hand operand is reduced in size compared to right operand, as the left operand is only used to store two bits for which unit8 is sufficient  */
                            SduDataBuffer_au8[Index_u8]= (uint8)(CanIdNoIdType_t & CANIF_GET_ONE_BYTE);
                            CanIdNoIdType_t = (CanIdNoIdType_t>>CANIF_SHIFT_8);
                        }
                    }
                    PduInfoTemp_tst.SduDataPtr = (uint8*)(&SduDataBuffer_au8[0]);

                    #else
                    PduInfoTemp_tst.SduDataPtr =(uint8*)(PduInfoPtr->SduDataPtr);

                    #endif
                    #if CANIF_CFG_USER_RX_ENABLED == STD_ON
                    ULPduInfoTypUSER_tst.SduDataPtr= PduInfoTemp_tst.SduDataPtr;
                    #endif
                    #if CANIF_CFG_UL_RX == STD_ON
                    ULPduInfoTyp_tst.SduDataPtr= PduInfoTemp_tst.SduDataPtr;
                    #endif

                    /*[SWS_CANIF_00392] Notification status for valid reception is set */
                      #if(STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API)
                      if((CANIF_READ_NOTIFSTATUS_DATA == RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8) || (CANIF_READ_NOTIFSTATUS == RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8))
                      {
                          *(RxNotifPtr_pen + PduId_qu8) = CANIF_TX_RX_NOTIFICATION;
                      }
                      #endif

                    /*[SWS_CANIF_00297] Buffer the data if CanIfRxPduReadData parameter is enabled */
                    #if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)
                    if((RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_NOTIFSTATUS_DATA) || (RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8== CANIF_READ_DATA))
                    {
                        /* Buffer the received message */
                        CanIf_Prv_WriteRxBuffer(PduInfoTemp_tst.SduDataPtr, RxPduCfg_pcst,PduInfoPtr->SduLength,Mailbox->CanId);
                    }
                    #endif

                    /*[SWS_CANIF_00415] Notify the recive indication to upper layer */
                    /*Notification will be provided only if UL is configured or Name of UL(USER type) is configured */
                    if(RxPduCfg_pcst->IndexForUL_u8 != CANIF_PRV_NO_UL)
                    {
                        #if CANIF_CFG_USER_RX_ENABLED == STD_ON
                        if(RxPduCfg_pcst->IndexForUL_u8 > (CANIF_PRV_SEVEN+ CanIf_Prv_ConfigSet_tpst->NumCddRxPdus_t))
                        {
                            /* Notification to USER Type*/
                            ULPduInfoTypUSER_tst.SduCanId = (uint32)((Mailbox->CanId) & CANIF_MSB_BIT_RESET);
                            CanIf_Prv_ConfigSet_tpst->RxUSER_Ptr[(RxPduCfg_pcst->IndexForUL_u8)-(CANIF_PRV_EIGHT+CanIf_Prv_ConfigSet_tpst->NumCddRxPdus_t)].CanIfRxPduUserRxIndicationUser(RxPduCfg_pcst->RxPduTargetId_t, &ULPduInfoTypUSER_tst);
                        }
                        else
                        #endif
                        {
                            #if CANIF_CFG_UL_RX == STD_ON
                            /* FC_VariationPoint_START */
                            if (RxPduCfg_pcst->IndexForUL_u8 != CANIF_PRV_SIX)
                            {
                                #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
                                uint8_least RxPduCfg_IndexForUL_u8 = 0;
                                localCoreId_u8 = Schm_CanIf_XCore_GetCurrentCoreId();

                                if (RxPduCfg_pcst->IndexForUL_u8 > (uint8_least) CDD)
                                {
                                    RxPduCfg_IndexForUL_u8 = (uint8_least)CDD;
                                    destCoreId_u8 = CanIf_XCore_Config_pcst->CanIf_XCoreUserTypeCoreAffinity[RxPduCfg_IndexForUL_u8];
                                }
                                else
                                {
                                    destCoreId_u8 = CanIf_XCore_Config_pcst->CanIf_XCoreUserTypeCoreAffinity[RxPduCfg_pcst->IndexForUL_u8];
                                }
                                pipeId_u16 = CanIf_XCore_Config_pcst->CanIf_XCoreRxPipeMatrix[localCoreId_u8][destCoreId_u8];

                                if(pipeId_u16 == CANIF_XCORE_INVALID_PIPE_ID)
                                {
                                    /* Notification to the upper layer*/
                                   (void) CanIf_XCore_LocalCore_RxIndication(RxPduCfg_pcst, &ULPduInfoTyp_tst);
                                }
                                else
                                {
                                    xCorePipeTxRxParams.PduId = (PduIdType)PduId_qu8;
                                    xCorePipeTxRxParams.PduInfoPtr = &ULPduInfoTyp_tst;
                                    xCorePipeTxRxParams.callbackIndex_u8 = (uint8)RxPduCfg_pcst->IndexForUL_u8;
                                    xCorePipeTxRxParams.ControllerId_u8 = Mailbox->ControllerId;

                                    /* The request is sent to the pipe when the UL resides in a different core */
                                    (void)CanIf_XCore_PipeWrite(pipeId_u16, &xCorePipeTxRxParams);
                                }
                                #else
                                /* Notification to Autosar types */
                                (void)CanIf_XCore_LocalCore_RxIndication(RxPduCfg_pcst, &ULPduInfoTyp_tst);
                                #endif
                            }
                            else
                                /* FC_VariationPoint_END */
                            {
                                /* Notification to Autosar types */
                                (void)CanIf_XCore_LocalCore_RxIndication(RxPduCfg_pcst, &ULPduInfoTyp_tst);
                            }
                            #endif
                        }
                    }/*end of if(RxPduCfg_pcst->IndexForUL_u8 != CANIF_PRV_NO_UL)*/
                }/*end of if(RxPduCfg_pcst->IndexForUL_u8 > (CANIF_PRV_SEVEN+ CanIf_Prv_ConfigSet_tpst->NumCddRxPdus_t))*/
            }/*end of if(IdFound_b==TRUE)*/
        }/*end of if(ModeChkStatus==  CANIF_PRV_WAKEUP_MODE)*/
    }/*end of if(IdFound_b == TRUE)*/
#endif/*end of #if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)*/
}/* End of CanIf_RxIndication_Internal() */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
Std_ReturnType CanIf_XCore_LocalCore_RxIndication(const CanIf_Cfg_RxPduType_tst * CanIf_RXPduConfig_pst,
                                                                    const PduInfoType * CanIf_ULPduinfo_pst)
{
        Std_ReturnType lretval = E_OK;
#if CANIF_CFG_UL_RX == STD_ON
        /* Pointer to Tx Pdu configuration */
        const CanIf_Cfg_RxPduType_tst * RxPduConfig_pst;
#endif

        /* If Ptr is NULL, report to DET and return E_NOT_OK */
        CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_RXPduConfig_pst == NULL_PTR), CANIF_INIT_SID, CANIF_E_PARAM_POINTER)

        /* If Ptr is NULL, report to DET and return E_NOT_OK */
        CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_ULPduinfo_pst == NULL_PTR), CANIF_INIT_SID, CANIF_E_PARAM_POINTER)

#if CANIF_CFG_UL_RX == STD_ON
        RxPduConfig_pst = CanIf_RXPduConfig_pst;

        /* Notification to Autosar types */
        CanIf_Prv_ConfigSet_tpst->RxAutosarUL_Ptr[RxPduConfig_pst->IndexForUL_u8].CanIfRxPduIndicationName(RxPduConfig_pst->RxPduTargetId_t, CanIf_ULPduinfo_pst);
#endif

        return lretval;
}
#endif/*End of #if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)*/
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/**********************************************************************************************************
*                     End of File
**********************************************************************************************************/
