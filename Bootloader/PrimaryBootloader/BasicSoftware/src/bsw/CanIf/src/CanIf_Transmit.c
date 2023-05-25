


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/*version check to be done*/
#include "CanIf_Prv.h"


/*
 ***************************************************************************************************
 * used functions
 ***************************************************************************************************
 */

/************************************************************************************************************/
/* Function name    : CanIf_Transmit                                                                        */
/* Syntax           : Std_ReturnType CanIf_Transmit(
                                                        PduIdType CanIfTxSduId,
                                                        const PduInfoType * CanIfTxInfoPtr
                                                                      )   */
/* Description      : This service initiates a request for transmission of the CAN L-PDU specified by the
                      CanTxPduId and CAN related data in the L-PDU structure.                               */
/* Parameter        : CanTxPduId, PduInfoPtr                                                                */
/* Return value     : E_OK / E_NOT_OK                                                                       */
/************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

Std_ReturnType CanIf_Transmit(PduIdType CanIfTxSduId,
        const PduInfoType * CanIfTxInfoPtr)
{
    /* To store the return value */
    Std_ReturnType lRetVal_en;
    #if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
    uint16 ltxPduCustId_t;

    /* Pointer to Tx Pdu configuration */
    const CanIf_Cfg_TxPduConfig_tst * lTxPduConfig_pst;

    /* FC_VariationPoint_START */
    #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

    /* To store pipe info*/
    CanIf_XCore_PipeTxRxParams_st lpipeParams_st;

    /* To store the local core Id */
    uint8 llocalCoreId_u8;

    /* To store the local core Id */
    uint8 ldestCoreId_u8;

    /* To store the Pipe Id */
    uint16 lpipeId_u16;
    uint8 lControllerId_u8    = 0;
    #endif
    /* FC_VariationPoint_END */

    /* initialize the  return value */
    lRetVal_en = E_OK;

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_TRANSMIT_SID, CANIF_E_UNINIT)

    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIfTxSduId >= CANIF_TOTAL_TXPDUS), CANIF_TRANSMIT_SID, CANIF_E_INVALID_TXPDUID)

    /* If PduInfoPtr is null, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == CanIfTxInfoPtr), CANIF_TRANSMIT_SID, CANIF_E_PARAM_POINTER)

    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanIfTxSduId];

    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ltxPduCustId_t == 0xFFFFu), CANIF_TRANSMIT_SID, CANIF_E_INVALID_TXPDUID)

    /*Get the address of config structure of requested PDU*/
    lTxPduConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;

    /* When truncation is disable and dlc obtained is greater than configured. Then E_NOT_OK should be invoked */
    if((lTxPduConfig_pst->TxTruncEnabled_b == FALSE) && (CanIfTxInfoPtr->SduLength> ((uint32)lTxPduConfig_pst->TxPduLength_u8
            #if(CANIF_METADATA_SUPPORT == STD_ON)
            +lTxPduConfig_pst->MetaDataLength
            #endif
            )))
    {

            #if(CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
        /* MR12 RULE 14.3, VIOLATION: Warning is "Not Critical". DET condition is intentionally made as True to report
         * DET error as the transmit CAN frame is requested when Dlc to be transmitted is greater then configured when
         * truncation is disabled */
            CANIF_DET_REPORT_ERROR_NOT_OK(TRUE, CANIF_TRANSMIT_SID, CANIF_E_TXPDU_LENGTH_EXCEEDED)
            #else
            lRetVal_en = E_NOT_OK;
            #endif

    }
    else
    {
        /*Do nothing*/
    }


    if(lRetVal_en ==E_OK)
    {
        /* FC_VariationPoint_START */
        #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

        lpipeParams_st.PduId = CanIfTxSduId;
        lpipeParams_st.PduInfoPtr = CanIfTxInfoPtr;

        lControllerId_u8  = lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->CanIf_CtrlConfigPtr->CtrlId;

        lpipeParams_st.ControllerId_u8 = lControllerId_u8;

        /* Get the current core ID */
        llocalCoreId_u8 = Schm_CanIf_XCore_GetCurrentCoreId();

        /* Extract the destination core Id from CanIf_XCoreCanControllerCoreAffinity */
        ldestCoreId_u8 = CanIf_XCore_Config_pcst->CanIf_XCoreCanControllerCoreAffinity[lControllerId_u8];

    /* Get the Pipe Id from CanIf_XCoreTxPipeMatrix using source and destination core Id */
    lpipeId_u16 = CanIf_XCore_Config_pcst->CanIf_XCoreTxPipeMatrix[llocalCoreId_u8][ldestCoreId_u8];

    if(lpipeId_u16 == CANIF_XCORE_INVALID_PIPE_ID)
    {
        /* Request for transmission of the CAN L-PDU in the controller core  */
        lRetVal_en = CanIf_XCore_LocalCore_Transmit(CanIfTxSduId, CanIfTxInfoPtr);
    }
    else
    {
        /* Transmission request through pipe*/
        lRetVal_en = CanIf_XCore_PipeWrite(lpipeId_u16, &lpipeParams_st);
    }

    #else
    /* FC_VariationPoint_END */
    /* Request for transmission of the CAN L-PDU */
    lRetVal_en = CanIf_XCore_LocalCore_Transmit(CanIfTxSduId, CanIfTxInfoPtr);

        /* FC_VariationPoint_START */
        #endif
        /* FC_VariationPoint_END */
    }
    else
    {
        /*Do nothing*/
    }
#else
    /* Return E_NOT_OK */
    lRetVal_en = E_NOT_OK;
#endif
    return lRetVal_en;

}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/************************************************************************************************************/
/* Function name    : CanIf_XCore_LocalCore_Transmit                                                                        */
/* Syntax           : Std_ReturnType CanIf_XCore_LocalCore_Transmit(
                                                        PduIdType CanIfTxSduId,
                                                        const PduInfoType * CanIfTxInfoPtr
                                                                      )   */
/* Description      : This service initiates a request for transmission of the CAN L-PDU specified by the
                      CanTxPduId and CAN related data in the L-PDU structure.                               */
/* Parameter        : CanTxPduId, PduInfoPtr                                                                */
/* Return value     : E_OK / E_NOT_OK                                                                       */
/************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* HIS METRIC v(G),STMT,LEVEL VIOLATION IN CanIf_Transmit: Function contains very simple "if-else" statements.
 * HIS metric compliance would decrease readability and maintainability */
    #if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
Std_ReturnType CanIf_XCore_LocalCore_Transmit(
                                                    PduIdType CanIfTxSduId,
                                                    const PduInfoType * CanIfTxInfoPtr
                                               )
{
    Std_ReturnType lRetVal_en           = E_NOT_OK;
    /* Pointer to Tx Pdu configuration */
    const CanIf_Cfg_TxPduConfig_tst * lTxPduConfig_pst;

    #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
    /*local pointer to ROM structure of controller */
    const CanIf_Cfg_CtrlConfig_tst * lCtrlConfig_pst;

    /*Local variable to hold the custId for controller */
    uint8 lCtrlCustId_u8;

    /*Local pointer to controller Driver structure */
    const CanIf_CtrlDrvCfgStruct * lCtrlDrvConfig_pst;
    #endif

    /* local variable to store Ctrl_ptr */
    uint8 Ctrl_Pdu_Var;
    /* local variable to store Ctrl_Pdu_mode */
    uint8 Pdu_mode_Lcl;

    /* local variable to store Ctrl_Pdu_mode */
     uint8 Ctrl_mode_Lcl;

    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * lControllerState_en;

    Can_PduType lPduInfo_st          = {NULL_PTR, 0, 0, 0};
    Can_ReturnType lCanStatus_en        = CAN_NOT_OK;
    Can_HwHandleType lHth_uo              = 0;
    uint16 ltxPduCustId_t;

#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    uint8 lMaxSduLength_u8     = 0;
#if (CANIF_PUBLIC_SETDYNAMICTXID_API == STD_ON)
    uint8 CanifMSBbits_u8;
#endif
#endif
    PduLengthType lSduLength            = 0;
    uint8 lControllerId_u8    = 0;

    uint32 lCanId_u32             = 0;

#if(CANIF_METADATA_SUPPORT == STD_ON)
    uint32 lStoredCanId_u32       = 0;
    uint32 lMetadataCanId_u32     = 0;
    uint8_least lIndex_uo              = 0;
    uint8 * lMetaDataPtr_u8;
#endif

#if((CANIF_PUBLIC_TXBUFFERING == STD_ON)&&(CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON))
    uint8 lDataMaxLen;
#endif
#if(CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    /* local variable to store the PnTxFilter */
    boolean lPnTxFilter_b;
#endif

#if (CANIF_PUBLIC_SETDYNAMICTXID_API == STD_ON)
    uint32 * lDynId_p;
#endif

    #if(CANIF_RB_NODE_CALIBRATION == STD_ON)
    uint8 lCalibCtrlId;
    #endif

    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanIfTxSduId];

    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ltxPduCustId_t == 0xFFFFu), CANIF_TRANSMIT_SID, CANIF_E_INVALID_TXPDUID)

    /*Get the address of config structure of requested PDU*/
    lTxPduConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;

    /*[SWS_BSW_00212]*/
    /* If PduInfoPtr->SduDataPtr is null when not a trigger transmit pdu, report to DET and return E_NOT_OK*/
#if(CANIF_TRIGGERTRANSMIT_SUPPORT != STD_ON)
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == CanIfTxInfoPtr->SduDataPtr), CANIF_TRANSMIT_SID, CANIF_E_PARAM_POINTER)
#else
    CANIF_DET_REPORT_ERROR_NOT_OK(((NULL_PTR == CanIfTxInfoPtr->SduDataPtr) && (lTxPduConfig_pst->TxPduTriggerTransmit == FALSE)),\
                                CANIF_TRANSMIT_SID, CANIF_E_PARAM_POINTER)
#endif


    /*Get the configured Hth Id and controllerId for this PDU*/
    lHth_uo           = lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->CanObjectId;
#if(CANIF_RB_NODE_CALIBRATION == STD_OFF)
    lControllerId_u8  = lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->CanIf_CtrlConfigPtr->CtrlId;
#else
    lCalibCtrlId = lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->getCtrlCfg();
    lControllerId_u8  = CanIf_CanCtrl_CanIfCtrl_MappingTable_acst[lCalibCtrlId];
#endif

    #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
    /* Store custId for the requested controller for the selected variant */
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[lControllerId_u8];

    /* Get configured data of requested controller */
    lCtrlConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + lCtrlCustId_u8;

    lCtrlDrvConfig_pst = CanIf_Prv_ConfigSet_tpst->CanCtrlDrvCont_Ptr +(lCtrlConfig_pst->CanDrvIndx);
    #endif

    /*Get the current status of the linked controller */
    lControllerState_en = CanIf_Prv_ControllerState_ast + lControllerId_u8;

#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    lPnTxFilter_b   = lControllerState_en->BlPnTxFilter;
#endif

    Ctrl_Pdu_Var=lControllerState_en->Ctrl_Pdu_mode;
    Pdu_mode_Lcl=  Ctrl_Pdu_Var& CANIF_BIT_MASK_CTRL_MODE;
    Ctrl_mode_Lcl= (Ctrl_Pdu_Var& CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT;

    /* If Pdu mode is OFFLINE or Controller mode is STOPPED, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK(((Pdu_mode_Lcl == (uint8)CANIF_OFFLINE)||(Ctrl_mode_Lcl == (uint8)CANIF_CS_STOPPED)), \
                                                CANIF_TRANSMIT_SID, CANIF_E_STOPPED)

     /*[SWS_CANIF_00750] */
#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    if((lPnTxFilter_b == TRUE) && ((lTxPduConfig_pst->TxPduPnFilterPdu)==FALSE))
    {
        lRetVal_en = E_NOT_OK;
    }
    else
#endif
    {

        if(
        #if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
                ((lPnTxFilter_b == TRUE)&&((lTxPduConfig_pst->TxPduPnFilterPdu)==TRUE))||(((lPnTxFilter_b == FALSE)&&
        #endif
                /* Check the Controller mode and the Channel mode */
                (( Ctrl_mode_Lcl == (uint8)CANIF_CS_STARTED) && (Pdu_mode_Lcl == (uint8)CANIF_ONLINE))
#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
                        )
#endif
                /* FC_VariationPoint_START */
        #if(CANIF_USER_TP_TX_OFFLINE_MODE != STD_OFF)
        /* MR12 RULE 12.1 VIOLATION: Warning is "Not Critical".Verified through testing that rules of precedence and associativity are not affecting the functionality   */
                ||((Ctrl_mode_Lcl == (uint8)CANIF_CS_STARTED) && (((Pdu_mode_Lcl  == (uint8)CANIF_TX_TP_ONLINE) && \
                        (lTxPduConfig_pst->TxPduTxUserUL == CAN_TP)) ||(( Pdu_mode_Lcl == (uint8)CANIF_TX_USER_TP_ONLINE) &&       \
                        ((lTxPduConfig_pst->TxPduTxUserUL == USER) || (lTxPduConfig_pst->TxPduTxUserUL == CAN_TP)))))
        #endif
                /* FC_VariationPoint_END */
              )
        #if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
              )
        #endif

    {

#if (CANIF_PUBLIC_SETDYNAMICTXID_API == STD_ON)
        if(lTxPduConfig_pst->TxPduType!= CANIF_STATIC)        /*DYNAMIC*/
        {

            /* Get the CanId set by CanIf_SetDynamicId api.*/

            lDynId_p = &CanIf_DynTxPduCanId_au32[0];

            /*Get the reconfigured CanId from Dynamic CanId array*/
            lPduInfo_st.id = (Can_IdType)*(lDynId_p + (lTxPduConfig_pst->TxPduType));


            /*Set the length of sdu data to be sent to CanDrv*/
#if ( CANIF_RB_CALIBRATION == STD_ON )
            /* Check if the PDU is COM PDU or not, COM Pdus will have DLC calibration parameter */
            if( lTxPduConfig_pst->getTxPduDlc != NULL_PTR )
            {
                /* Calibrated DLC will be used for COM Pdus transmission */
                lSduLength = lTxPduConfig_pst->getTxPduDlc();
            }
            else
#endif
            {
                if(CanIfTxInfoPtr->SduLength>lTxPduConfig_pst->TxPduLength_u8)
                {
                    lSduLength = lTxPduConfig_pst->TxPduLength_u8;
                }
                else
                {
                    lSduLength = CanIfTxInfoPtr->SduLength;
                }
            }

#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
            /* Identify the type of CanId by processing 2 MSBs of the CanId(whether FD or non-FD).*/

            CanifMSBbits_u8 = (uint8)((lPduInfo_st.id)>>CANIF_CANID_BIT_SHIFT);

            /* As per CanId type, set the maximum sdulength possible*/
            if((CanifMSBbits_u8 == CANIF_ONE) ||(CanifMSBbits_u8 == CANIF_THREE))
            {
                lMaxSduLength_u8 = 64u;
            }
            else
            {
                lMaxSduLength_u8 = 8u;
            }
#endif
        }       /*end of if(lTxPduConfig_pst->TxPduType!= CANIF_STATIC)*/
        else                                                /*STATIC*/
#endif
        {

#if(CANIF_METADATA_SUPPORT == STD_ON)
            if(lTxPduConfig_pst->MetaDataLength >= 1)
            {
                if(CanIfTxInfoPtr->SduLength>((uint32)lTxPduConfig_pst->TxPduLength_u8+lTxPduConfig_pst->MetaDataLength))
                {
                    lSduLength = lTxPduConfig_pst->TxPduLength_u8;
                }
                else
                {
                    lSduLength = (CanIfTxInfoPtr->SduLength)- (lTxPduConfig_pst->MetaDataLength);
                }

               /* As per CanId type, set the maximum sdulength possible*/

#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
                /* Check if classical CAN Frame */
                if( (lTxPduConfig_pst->TxPduCanIdType == STANDARD_CAN) ||
                    (lTxPduConfig_pst->TxPduCanIdType == EXTENDED_CAN) )
                {
                    /* Classical CAN */
                    lMaxSduLength_u8 = 8u + lTxPduConfig_pst->MetaDataLength;
                }
                else
                {
                    /* FD CAN */
                    lMaxSduLength_u8 = 64u + lTxPduConfig_pst->MetaDataLength;
                }
#endif

                /*[SWS_CANIF_00844] [SWS_CANIF_00845]*/
                /* Get the configured CanId  */
#if (CANIF_RB_CALIBRATION == STD_ON)
                if(lTxPduConfig_pst->getTxPduCanId != NULL_PTR )
                {
                    lStoredCanId_u32 = lTxPduConfig_pst->getTxPduCanId();
                }
                else
#endif
                {
                    lStoredCanId_u32 = lTxPduConfig_pst->TxPduCanId;
                }

                /*Get the MetaData out of SduLength*/

                /*pointer to SDUPtr containing metadata*/
                lMetaDataPtr_u8 = (uint8 *)(CanIfTxInfoPtr->SduDataPtr);

                /* Set the pointer to end of SDU data to extract MetaData.
                * The sequence of the CanId bytes in the MetaData is little endian, i.e. the lowest byte of the
                * ID (the 8 least significant bits) is placed in the first byte after the actual L-SDU data.
                */

                lMetaDataPtr_u8 = lMetaDataPtr_u8 + (CanIfTxInfoPtr->SduLength-1u);

                /*Get the MetaData*/
                for(lIndex_uo = 0; lIndex_uo < (lTxPduConfig_pst->MetaDataLength);lIndex_uo++)
                {

                    lMetadataCanId_u32 = (lMetadataCanId_u32<<8u) | (*lMetaDataPtr_u8);
                    lMetaDataPtr_u8--;

                }

                if(lTxPduConfig_pst->TxPduCanId!= 0xFFFFFFFFu)    /*when a CanIfTxPduCanIdMask AND TxPduCanId are defined*/
                /*when a CanIfTxPduCanIdMask AND TxPduCanId are defined(CanIfTxPduCanIdMask default value is 0x1FFFFFFF)*/
                {
                    /* Do Mask operation */

                    /*[SWS_CANIF_00854]*/
                    /*Variable parts of the CanId provided via the MetaData must be merged with the
                    CanId by using CanIfTxPduCanIdMask:
                    */
                    lCanId_u32 = ((lStoredCanId_u32) & (lTxPduConfig_pst->TxPduCanIdMask)) |
                            ((lMetadataCanId_u32) & (~(lTxPduConfig_pst->TxPduCanIdMask)));
                }
                else                   /*When no CanIfTxPduCanId is configured*/
                {

                    /*MetaData shall be used directly as CanId (only if MetaDataLength>= CanID size)*/
                    /*Note: if CanIfTxPduCanId is not configured MetaDataLength is always ensured to be
                    large enough as actual CanId size. This is done in validations*/

                    /*As per [SWS_CANIF_00846], If MetaDataLength is larger than the CanId size, the space after
                     * the ID bytes shall be padded with zeros.*/
                    if( (lTxPduConfig_pst->TxPduCanIdType == STANDARD_CAN)  ||
                        (lTxPduConfig_pst->TxPduCanIdType == STANDARD_FD_CAN) )
                    {
                            lCanId_u32 = lMetadataCanId_u32 & 0x000007FFu;
                    }
                    else
                    {
                            lCanId_u32 = lMetadataCanId_u32 & 0x1FFFFFFFu;

                    }
                }

            }
            else
#endif
            {
                /* As per CanId type, set the maximum sdulength possible*/
#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
                /* Check if classical CAN Frame */
                if( (lTxPduConfig_pst->TxPduCanIdType == STANDARD_CAN) ||
                    (lTxPduConfig_pst->TxPduCanIdType == EXTENDED_CAN) )
                {
                    /* Classical CAN */
                    lMaxSduLength_u8 = 8u;
                }
                else
                {
                    /* FD CAN */
                    lMaxSduLength_u8 = 64u ;
                }
#endif
                /*Added validation that if pure static CanId should be configured*/
                if(lTxPduConfig_pst->TxPduCanId!= 0xFFFFFFFFu)            /*Default value*/
                {
                    /* Get the configured CanId */
#if (CANIF_RB_CALIBRATION == STD_ON)
                    if(lTxPduConfig_pst->getTxPduCanId != NULL_PTR )
                    {
                       lCanId_u32 = lTxPduConfig_pst->getTxPduCanId();
                    }
                    else
#endif
                    {
                       lCanId_u32 = lTxPduConfig_pst->TxPduCanId;
                    }

                }
                else
                {
                    /*CanId not configured(taken care in script) CanId
                     * should be configured if 1) static 2) #if(CANIF_METADATA_SUPPORT == STD_ON)
                     * but lTxPduConfig_pst->MetaDataLength == 0*/
                }

                /*Set the length of sdu data to be sent to CanDrv*/
#if ( CANIF_RB_CALIBRATION == STD_ON )
                /* Check if the PDU is COM PDU or not, COM Pdus will have DLC calibration parameter */
                if( lTxPduConfig_pst->getTxPduDlc != NULL_PTR )
                {
                    /* Calibrated DLC will be used for COM Pdus transmission */
                    lSduLength = lTxPduConfig_pst->getTxPduDlc();
                }
                else
#endif
                {
                    if( CanIfTxInfoPtr->SduLength > lTxPduConfig_pst->TxPduLength_u8 )
                    {
                        lSduLength = lTxPduConfig_pst->TxPduLength_u8;
                    }
                    else
                    {
                        lSduLength = CanIfTxInfoPtr->SduLength;
                    }
                }
            }

            lPduInfo_st.id = (Can_IdType)lCanId_u32;

            /* Set the type of CanId by setting 2 MSBs of the CanId(whether FD or non-FD)*/

            lPduInfo_st.id &= CANIF_MSB_BIT_RESET;

            /* setting two most significant bit based on CanIdType */
            lPduInfo_st.id |= ( ((Can_IdType)lTxPduConfig_pst->TxPduCanIdType) << CANIF_CANID_BIT_SHIFT );

        }

#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
            /* If SduLength exceeding the maximum length of the PDU referenced by Can_IdType is invalid, report to DET */
        CANIF_DET_REPORT_ERROR(((uint8)CanIfTxInfoPtr->SduLength > lMaxSduLength_u8), \
CANIF_TRANSMIT_SID, CANIF_E_DATA_LENGTH_MISMATCH)
#endif
        lPduInfo_st.swPduHandle = lTxPduConfig_pst->TxPduId;
        lPduInfo_st.sdu         = CanIfTxInfoPtr->SduDataPtr;
        lPduInfo_st.length      = (uint8)lSduLength;

#if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
        lCanStatus_en = lCtrlDrvConfig_pst->CanWrite(lHth_uo,&lPduInfo_st);
#else
    /* BSW-9662 */ /* [$DD_BSWCODE 17055] */
    lCanStatus_en = CanIf_Write_Integration(lHth_uo, &lPduInfo_st);
#endif

        if(lCanStatus_en == CAN_OK)
        {
            lRetVal_en = E_OK;
        }
        else if(lCanStatus_en == CAN_BUSY)
        {
    /*If transmit buffering is enabled, CanIf will store a Tx L-PDU in a CanIf Transmit
    L-PDU Buffer (CanIfTxBuffer), if it is rejected by CanDrv at Transmit Request.*/
    #if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
            /*  CanIfTxBuffer is configured with a buffer size greater than zero    */
            if((lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIfBufferSize) > 0)
            {
                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();

                /*[SWS_CANIF_00381], [SWS_CANIF_00881]*/
                lRetVal_en = CanIf_Prv_WriteTxBuffer(CanIfTxSduId, lPduInfo_st);

                /* Enter Critical Section */
                SchM_Exit_CanIf_TxBufAccessNoNest();
#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
                lDataMaxLen = lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIfBufferMaxDataLength;
#endif
                    /* If SduLength exceeding the maximum length of data accomodated in TxBuffer, report to DET */
                CANIF_DET_REPORT_ERROR(((lRetVal_en == E_OK) && ((uint8)lPduInfo_st.length > lDataMaxLen)),     \
                                                                    CANIF_TRANSMIT_SID, CANIF_E_DATA_LENGTH_MISMATCH)
            }
            else
            {
                lRetVal_en = E_NOT_OK;
            }
    #else
            lRetVal_en = E_NOT_OK;
    #endif
        }
        else
        {
            lRetVal_en = E_NOT_OK;
        }
    }
#if(CANIF_TXOFFLINEACTIVE_SUPPORT == STD_ON)
    else if(((uint8)CANIF_CS_STARTED == Ctrl_mode_Lcl )&&
                (Pdu_mode_Lcl == (uint8)CANIF_TX_OFFLINE_ACTIVE))
    {
        if(NULL_PTR != lTxPduConfig_pst->UserTxConfirmation)
        {
            lTxPduConfig_pst->UserTxConfirmation(lTxPduConfig_pst->TxPduTargetPduId);
        }


        /*Return E_OK*/
        lRetVal_en = E_OK;
    }/*end of check for OFFLINE-ACTIVE mode*/
#endif
    else
    {
        /*Intentionally empty*/
    }
  }
    return lRetVal_en;
}
#endif
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"




/************************************************************************************************************/
/* Function name    : CanIf_CancelTransmit                                                                        */
/* Syntax           : Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId)   */
/* Description      : This is a dummy API introduced for interface compatibility. This is
 *                    called by PduR to achieve bus agnostic behavior                               */
/* Parameter        : CanIfTxSduId                                                          */
/* Return value     : Always E_OK                                                                   */
/************************************************************************************************************/
#if (CANIF_PUBLIC_CANCEL_TRANSMIT_SUPPORT == STD_ON)

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_CancelTransmit(
                                                    PduIdType CanIfTxSduId

                                               )
{
    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIfTxSduId >= CANIF_TOTAL_TXPDUS), CANIF_CANCELTRANSMIT_SID, CANIF_E_INVALID_TXPDUID)


    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanIfTxSduId] == 0xFFFFu), \
                                                                    CANIF_CANCELTRANSMIT_SID, CANIF_E_INVALID_TXPDUID)

    (void)CanIfTxSduId;
       /*Always return E_OK*/
        return E_OK;
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif
