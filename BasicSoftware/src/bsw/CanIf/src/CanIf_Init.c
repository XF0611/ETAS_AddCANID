


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"


/**********************************************************************************************************************
 * Function name    : CanIf_Init
 * Syntax           : void CanIf_Init(const CanIf_ConfigType * ConfigPtr)
 * Description      : This service Initializes internal and external interfaces of the CAN Interface for the
                      further processing.
 * Parameter        : ConfigPtr
 * Return value     : None
 **********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

void CanIf_Init(const CanIf_ConfigType * ConfigPtr)
{


    /* Pointer to Controller State */
    CanIf_ControllerStateType * controllerState_pst;

    /*Index variable to iterate through 'for' loop*/
    uint32 index_u32;

    /*Index variable to loop through the Pdu's*/
#if ( (CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON) || (CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API == STD_ON) || \
      (CANIF_PUBLIC_READRXPDU_DATA_API   == STD_ON) || (CANIF_RB_NODE_CALIBRATION                == STD_ON) )
    uint8_least pdu_uo;
#endif

    /*Variable to store number of controllers*/
    uint8 numControllers_u8;

#if (CANIF_PUBLIC_SETDYNAMICTXID_API == STD_ON)

    /*Pointer to store address of Dynamic CanId array*/
    uint32 * dynId_pu32;
    /*Variable to store CanId*/
    Can_IdType canId_t;
    /*Variable to store number of Tx PDUs*/
    uint32 numTxPduId_u32;

    /* Pointer to Tx Pdu configuration */
    const CanIf_Cfg_TxPduConfig_tst * txPduConfig_pcst;

#endif
    /* FC_VariationPoint_START */
    #if ((CANIF_RB_NODE_CALIBRATION ==STD_ON) && (CANIF_PUBLIC_PN_SUPPORT == STD_ON))
    /*Variable to store controller Id of Can Driver*/
    uint8 CanCtrlId_u8;
    #endif
    /* FC_VariationPoint_END */

#if(CANIF_RB_CHANGERXPDUID_API != STD_OFF)

    /* Pointer to RxPdu configuration */
    const CanIf_Cfg_RxPduType_tst * RxPduCfg_pcst;
    /*Pointer to store address of Dynamic RxPdu CanId array*/
    uint32 * dynRxPduId_pu32;

    /*Variable to store Rx Pdu CanId*/
    Can_IdType rxpducanId_t;

    /*Variable to store number of Rx PDUs*/
    uint32 numRxPduId_u32;

#endif

#if ((CANIF_PUBLIC_PN_SUPPORT == STD_ON) && (CANIF_RB_NODE_CALIBRATION != STD_ON))
    /* Pointer to Tx Pdu configuration */
    const CanIf_Cfg_CtrlConfig_tst * ctrlConfig_pcst;
#endif

    /*Set the variant pointer*/
    #if CANIF_CONFIGURATION_VARIANT == CANIF_CFG_VAR_PC
    CanIf_Prv_ConfigSet_tpst = &CanIf_Config;
    /* Unused input parameter to avoid warning */
    (void) ConfigPtr;
    #else
    CanIf_Prv_ConfigSet_tpst = ConfigPtr;

    /* If ConfigPtr is NULL, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((ConfigPtr == NULL_PTR), CANIF_INIT_SID, CANIF_E_PARAM_POINTER)
    #endif


    numControllers_u8 = CanIf_Prv_ConfigSet_tpst->NumCanCtrl_u8;


    /*Initialise 'controllerState_pst'*/
    controllerState_pst = CanIf_Prv_ControllerState_ast;

    /*Initialize TxNotification status per PDU*/
#if ((CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)&&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))

    for(pdu_uo = 0; pdu_uo < CanIf_Prv_ConfigSet_tpst->NumOfTxPdus; pdu_uo++)
    {
        CanIf_Prv_TxNotification_aen[pdu_uo] = CANIF_NO_NOTIFICATION;
    }
#endif

#if ((CANIF_PUBLIC_PN_SUPPORT == STD_ON) && (CANIF_RB_NODE_CALIBRATION != STD_ON))
    ctrlConfig_pcst = CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr;
#endif


#if(CANIF_PUBLIC_SETDYNAMICTXID_API== STD_ON)
    numTxPduId_u32 = CanIf_Prv_ConfigSet_tpst->NumOfTxPdus;

    txPduConfig_pcst = CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr;
    /* Get the address of array which stores Dynamic CanId.*/
    dynId_pu32 = &CanIf_DynTxPduCanId_au32[0];

    /* Copy the CanIds of dynamic TxPduIds from the configuration to the global array. This is needed if CanIf_Transmit()
     * is called before CanIf_SetDynamicTxId() api */

        /*First check if Dynamic Tx-Ids are configured for this configset*/
            for(index_u32 = 0; index_u32 < numTxPduId_u32; index_u32++)
            {
                if(((txPduConfig_pcst + index_u32)->TxPduType) != CANIF_STATIC)
                {
                    canId_t = ((txPduConfig_pcst +index_u32)->TxPduCanId);
                    canId_t &= CANIF_MSB_BIT_RESET;
                    canId_t |= (((Can_IdType)((txPduConfig_pcst +index_u32)->TxPduCanIdType))<<CANIF_CANID_BIT_SHIFT);
                    *(dynId_pu32 + ((txPduConfig_pcst +index_u32)->TxPduType)) = canId_t;
                }
            }
#endif

#if(CANIF_RB_CHANGERXPDUID_API != STD_OFF)
    /* Get the number of Rx-Pdus configured in the selected variant */
    numRxPduId_u32 = CanIf_Prv_ConfigSet_tpst->NumCanRxPduId_t;

    /* Get the pointer to Rx pdu  configuration */
    RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst;

    /* Get the address of array which stores Dynamic RxPdu CanId.*/
    dynRxPduId_pu32 = &CanIf_DynRxPduCanId_au32[0];

    /*First check if Dynamic RxPdu-Ids are configured for this configset*/
    for(index_u32 = 0; index_u32 < numRxPduId_u32; index_u32++)
    {
        /* Copy the CanIds of dynamic RxPdus from the configuration to the global array.  */
        if(((RxPduCfg_pcst + index_u32)->RxPduType) != CANIF_STATIC)
        {
            /* Copy the configured CanId value to local variable */
            rxpducanId_t = ((RxPduCfg_pcst +index_u32)->RxPduCanId);
            rxpducanId_t &= CANIF_MSB_BIT_RESET;
            /* Update the 2 MSB bits to CanId value based on the configured CanIdType */
            rxpducanId_t |= (((Can_IdType)((RxPduCfg_pcst +index_u32)->CanIdtype_u8))<<CANIF_CANID_BIT_SHIFT);
            /* Copy the CanId value to corresponding location in DynamicRxPduCanId array */
            *(dynRxPduId_pu32 + ((RxPduCfg_pcst +index_u32)->RxPduType)) = rxpducanId_t;
        }
    }
#endif


    /* Reset all the global variables and flags */
    for(index_u32 = 0; index_u32 < numControllers_u8; index_u32++)
    {
        /* Update the DeviceMode to CANIF_CS_STOPPED([SWS_CANIF_00476], [SWS_CANIF_00298]) &
         * Pdu Mode to CANIF_OFFLINE of each Controller */

        (controllerState_pst + index_u32)->Ctrl_Pdu_mode = (((uint8)CANIF_CS_STOPPED)<<CANIF_Ctrl_BIT_SHIFT)|((uint8)CANIF_OFFLINE);


        /*CANIF_00479 reset wakeup event*/
        #if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
        (controllerState_pst + index_u32)->ValidationSts_b = CANIF_RESET;
        #endif

        /* as per requirement [SWS_CANIF_00863] PnTxFilter is enabled  */

#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
#if (CANIF_RB_NODE_CALIBRATION != STD_ON)
        if((ctrlConfig_pcst + index_u32)->PnCtrlEn == TRUE)
        {
            (controllerState_pst + index_u32)->BlPnTxFilter = TRUE;
        }
        else
        {
            (controllerState_pst + index_u32)->BlPnTxFilter = FALSE;
        }
#else
        (controllerState_pst + index_u32)->BlPnCtrl_Calib = FALSE;
        (controllerState_pst + index_u32)->BlPnTxFilter = FALSE;
#endif
#endif

#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON)
        /* Reset Tx Confirmation state for each controller*/
        (controllerState_pst + index_u32)->CanIf_TxCnfmStatus = CANIF_NO_NOTIFICATION;
#endif

    }
/* FC_VariationPoint_START */
#if ( (CANIF_RB_NODE_CALIBRATION == STD_ON) && (CANIF_PUBLIC_PN_SUPPORT == STD_ON))
    /* Set PnTxFilter for the Controllers as per TxPduPnFilterPdu configuration */
    for(pdu_uo = 0; pdu_uo < CanIf_Prv_ConfigSet_tpst->NumOfTxPdus; pdu_uo++)
    {
        if( CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr[pdu_uo].TxPduPnFilterPdu == TRUE )
        {
            /*Get the calibrated controllerId of Can Driver*/
            CanCtrlId_u8 = CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr[pdu_uo].CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->getCtrlCfg();

            /*updated the PnTxFilter for the corresponding Controller */
            (controllerState_pst + CanIf_CanCtrl_CanIfCtrl_MappingTable_acst[CanCtrlId_u8])->BlPnTxFilter = TRUE;

            (controllerState_pst + CanIf_CanCtrl_CanIfCtrl_MappingTable_acst[CanCtrlId_u8])->BlPnCtrl_Calib = TRUE;
        }
    }
#endif
/* FC_VariationPoint_END */
#if((CANIF_PUBLIC_TXBUFFERING == STD_ON) &&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))
    CanIf_Prv_BufferInit();
#endif

    /* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
#if CANIF_CONFIGURATION_VARIANT == CANIF_CFG_VAR_PC
    CanIf_XCore_Init(NULL_PTR);
#else
    CanIf_XCore_Init(CanIf_Prv_ConfigSet_tpst->CanIf_XCoreConfigPtr);
#endif
#endif
    /* FC_VariationPoint_END */

    #if ((CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API != STD_OFF)  && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))

    for(pdu_uo=CANIF_ZERO; pdu_uo < CanIf_Prv_ConfigSet_tpst->NumCanRxPduId_t; pdu_uo++)
    {
        CanIf_Prv_RxNotification_taen[pdu_uo] = CANIF_NO_NOTIFICATION;
    }
    #endif
    #if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)
    for(pdu_uo=CANIF_ZERO; pdu_uo < CANIF_CFG_RX_BUFFER_SIZE; pdu_uo++)
        {
            CanIf_Prv_RxBuffer_tau8[pdu_uo]= 0x00;
        }
    #endif

    /*Store the Init status in Global variable*/
    CanIf_Prv_InitStatus_b = TRUE;

    /* TRUE implies CAN Interface is Initialised */

}/* End of CanIf_Init() */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"






/*CanIf_DeInit() function is required to de-initialise all the RAM areas of the previously active configset
 * This function is used only when Post-build is enabled. This function shall be exported via CanIf.h since
 * this needs to be used in the Integration code of EcuM*/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

void CanIf_DeInit(const CanIf_ConfigType * ConfigPtr)
{

    /*Set the variant pointer*/
#if CANIF_CONFIGURATION_VARIANT != CANIF_CFG_VAR_PC
    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * controllerState_pst;
    /*Index variable to loop through the Controllers*/
    uint8_least index_u32;

    /*Index variable to loop through the Pdu's*/
#if ( (CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON) || (CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API== STD_ON) || \
      (CANIF_PUBLIC_READRXPDU_DATA_API   == STD_ON) || (CANIF_RB_NODE_CALIBRATION                == STD_ON) )
    uint8_least pdu_uo;
#endif

    uint8 numControllers_u8;

    /* FC_VariationPoint_START */
    #if ((CANIF_RB_NODE_CALIBRATION ==STD_ON) && (CANIF_PUBLIC_PN_SUPPORT == STD_ON))
    /*Variable to store controller Id of Can Driver*/
    uint8 CanCtrlId_u8;
    #endif
    /* FC_VariationPoint_END */

#if (CANIF_PUBLIC_SETDYNAMICTXID_API == STD_ON)
    uint32 * dynId_pu32;

    Can_IdType canId_t;

    uint32 numTxPduId_u32;

    /* Pointer to Tx Pdu configuration */
    const CanIf_Cfg_TxPduConfig_tst * txPduConfig_pcst;

#endif


#if ((CANIF_PUBLIC_PN_SUPPORT == STD_ON) && (CANIF_RB_NODE_CALIBRATION != STD_ON))
    /* Pointer to Tx Pdu configuration */
    const CanIf_Cfg_CtrlConfig_tst * ctrlConfig_pcst;
#endif

#if(CANIF_RB_CHANGERXPDUID_API != STD_OFF)

    /* Pointer to RxPdu configuration */
    const CanIf_Cfg_RxPduType_tst * RxPduCfg_pcst;
    /*Pointer to store address of Dynamic RxPdu CanId array*/
    uint32 * dynRxPduId_pu32;

    /*Variable to store Rx Pdu CanId*/
    Can_IdType rxpducanId_t;

    /*Variable to store number of Rx PDUs*/
    uint32 numRxPduId_u32;

#endif

    /* If ConfigPtr is NULL, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((ConfigPtr == NULL_PTR), CANIF_DEINIT_SID, CANIF_E_PARAM_POINTER)
    CanIf_Prv_ConfigSet_tpst = ConfigPtr;


    /*Store the Init status in Global variable*/
    CanIf_Prv_InitStatus_b = FALSE;
    numControllers_u8 = CanIf_Prv_ConfigSet_tpst->NumCanCtrl_u8;


    /*Initialise 'controllerState_pst'*/
    controllerState_pst = CanIf_Prv_ControllerState_ast;

    #if ((CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API != STD_OFF) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))

    for(pdu_uo=0; pdu_uo < CanIf_Prv_ConfigSet_tpst->NumCanRxPduId_t; pdu_uo++)
    {
        CanIf_Prv_RxNotification_taen[pdu_uo] = CANIF_NO_NOTIFICATION;
    }
    #endif

#if ((CANIF_PUBLIC_PN_SUPPORT == STD_ON) && (CANIF_RB_NODE_CALIBRATION != STD_ON))
    ctrlConfig_pcst = CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr;
#endif





#if(CANIF_PUBLIC_SETDYNAMICTXID_API== STD_ON)
    numTxPduId_u32 = CanIf_Prv_ConfigSet_tpst->NumOfTxPdus;

    txPduConfig_pcst = CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr;
    /* Get the address of array which stores Dynamic CanId.*/
    dynId_pu32 = &CanIf_DynTxPduCanId_au32[0];

    /* Copy the CanIds of dynamic TxPduIds from the configuration to the global array. This is needed if CanIf_Transmit()
     * is called before CanIf_SetDynamicTxId() api */

        /*First check if Dynamic Tx-Ids are configured for this configset*/
            for(index_u32 = 0; index_u32 < numTxPduId_u32; index_u32++)
            {

                if((txPduConfig_pcst + index_u32)->TxPduType != CANIF_STATIC)
                {
                    canId_t = ((txPduConfig_pcst +index_u32)->TxPduCanId);
                    canId_t &= CANIF_MSB_BIT_RESET;
                    canId_t |= (((Can_IdType)((txPduConfig_pcst +index_u32)->TxPduCanIdType))<<CANIF_CANID_BIT_SHIFT);
                    *(dynId_pu32 + ((txPduConfig_pcst +index_u32)->TxPduType)) = canId_t;
                }
            }
#endif

#if(CANIF_RB_CHANGERXPDUID_API != STD_OFF)
    /* Get the number of Rx-Pdus configured in the selected variant */
    numRxPduId_u32 = CanIf_Prv_ConfigSet_tpst->NumCanRxPduId_t;

    /* Get the pointer to Rx pdu  configuration */
    RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst;

    /* Get the address of array which stores Dynamic RxPdu CanId.*/
    dynRxPduId_pu32 = &CanIf_DynRxPduCanId_au32[0];

    /*First check if Dynamic RxPdu-Ids are configured for this configset*/
    for(index_u32 = 0; index_u32 < numRxPduId_u32; index_u32++)
    {
        /* Copy the CanIds of dynamic RxPdus from the configuration to the global array.  */
        if(((RxPduCfg_pcst + index_u32)->RxPduType) != CANIF_STATIC)
        {
            /* Copy the configured CanId value to local variable */
            rxpducanId_t = ((RxPduCfg_pcst +index_u32)->RxPduCanId);
            rxpducanId_t &= CANIF_MSB_BIT_RESET;
            /* Update the 2 MSB bits to CanId value based on the configured CanIdType */
            rxpducanId_t |= (((Can_IdType)((RxPduCfg_pcst +index_u32)->CanIdtype_u8))<<CANIF_CANID_BIT_SHIFT);
            /* Copy the CanId value to corresponding location in DynamicRxPduCanId array */
            *(dynRxPduId_pu32 + ((RxPduCfg_pcst +index_u32)->RxPduType)) = rxpducanId_t;
        }
    }
#endif

    /* Reset all the global variables and flags */
    for(index_u32=0; index_u32 < numControllers_u8; index_u32++)
    {
        /* Update the DeviceMode to CANIF_CS_STOPPED([SWS_CANIF_00476], [SWS_CANIF_00298]) &
         * Pdu Mode to CANIF_OFFLINE of each Controller */
        (controllerState_pst + index_u32)->Ctrl_Pdu_mode = (((uint8)CANIF_CS_STOPPED)<<CANIF_Ctrl_BIT_SHIFT)|((uint8)CANIF_OFFLINE);

        /* as per requirement [SWS_CANIF_00863] PnTxFilter is enabled  */
#if ( (CANIF_RB_NODE_CALIBRATION != STD_ON) && (CANIF_PUBLIC_PN_SUPPORT == STD_ON) )
        if((ctrlConfig_pcst + index_u32)->PnCtrlEn == TRUE)
        {
            (controllerState_pst + index_u32)->BlPnTxFilter = TRUE;
        }
        else
        {
            (controllerState_pst + index_u32)->BlPnTxFilter = FALSE;
        }
#endif

#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON)
        /* Reset Tx Confirmation state for each controller*/
        (controllerState_pst + index_u32)->CanIf_TxCnfmStatus = CANIF_NO_NOTIFICATION;
#endif

    }
/* FC_VariationPoint_START */
#if ( (CANIF_RB_NODE_CALIBRATION == STD_ON) && (CANIF_PUBLIC_PN_SUPPORT == STD_ON) )
    /* Set PnTxFilter for the Controllers as per TxPduPnFilterPdu configuration */
    for(pdu_uo = 0; pdu_uo < CanIf_Prv_ConfigSet_tpst->NumOfTxPdus; pdu_uo++)
    {
        if( CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr[pdu_uo].TxPduPnFilterPdu == TRUE )
        {
            /*Get the calibrated controllerId of Can Driver*/
            CanCtrlId_u8 = CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr[pdu_uo].CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->getCtrlCfg();

            /*updated the PnTxFilter for the corresponding Controller */
            (controllerState_pst + CanIf_CanCtrl_CanIfCtrl_MappingTable_acst[CanCtrlId_u8])->BlPnTxFilter = TRUE;
        }
    }
#endif
/* FC_VariationPoint_END */

#if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)
for(pdu_uo=0; pdu_uo < CANIF_CFG_RX_BUFFER_SIZE; pdu_uo++)
    {
        CanIf_Prv_RxBuffer_tau8[pdu_uo]= 0x00;
    }
#endif

#if((CANIF_PUBLIC_TXBUFFERING == STD_ON)  &&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))
    CanIf_Prv_BufferInit();
#endif

#else
    /*To avoid compiler warning*/
    (void)ConfigPtr;
#endif
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


