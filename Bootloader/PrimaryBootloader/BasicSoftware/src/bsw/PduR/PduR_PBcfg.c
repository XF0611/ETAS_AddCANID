
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: PduR  / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#include "PduR_PBcfg.h"

#include "PduR_LoTp.h"

#include "PduR_UpTp.h"

#include "PduR_Mc.h"
#include "PduR_Gw.h"

#include "PduR_Gw_Cfg.h"
/* Generating PbCfg_c::PduR_UpIfToLo_PBcfg_c::upIf_To_Lo */
/* Generating PbCfg_c::PduR_UpTpToLo_PBcfg_c::upTp_To_Lo */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
#if defined(PDUR_CONFIG_SINGLE_TPTX_LO)
#define PduR_DcmToLo NULL_PTR
#else
static const PduR_RT_UpToLo PduR_DcmToLo[] = { {
		CanTpConf_CanTpTxNSdu_UdsOnCan_Tx_Phy_0x7B7,
		(PduR_loTransmitFP) PduR_RF_CanTp_Transmit,
		(PduR_loCancelTransmitFP) PduR_RF_CanTp_CancelTransmit } /*UDS_Tx_Phys_Id_0x7B7_CANNODE_0_Phys_Dcm2PduR*/

};
#endif /* PDUR_CONFIG_SINGLE_IFTX_LO */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
static const PduIdType PduR_DcmToLoMapTable[] = {

/*Dcm2CanTp_0x7B7 0 */
0 };

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
/* Generating PbCfg_c::PduR_LoIfRxToUp_PBcfg_c::loIfRx_To_Up */
/* Generating PbCfg_c::PduR_LoIfDTxToUp_PBcfg_c::loIf_DTxToUp */
/* Generating PbCfg_c::PduR_LoIfTTxToUp_PBcfg_c::loIf_TTxToUp */

/* Generating PbCfg_c::PduR_LoTpRxToUp_PBcfg_c::loTpRx_To_Up */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
#if defined ( PDUR_CONFIG_SINGLE_TPRX )
#define PduR_CanTpRxToUp   NULL_PTR
#else
static const PduR_RT_LoTpRxToUp PduR_CanTpRxToUp[] = { {
		DcmConf_DcmDslProtocolRx_UDS_Rx_Phys_Id_0x737_CANNODE_0_PduR2Dcm,
		(PduR_upTpStartOfReceptionFP) PduR_RF_Dcm_StartOfReception,
		(PduR_upTpProvideRxBufFP) PduR_RF_Dcm_CopyRxData,
		(PduR_upTpRxIndicationFP) PduR_RF_Dcm_TpRxIndication }, /*UDS_Rx_Phys_Id_0x737_CANNODE_0_CanTp2PduR*/
{ DcmConf_DcmDslProtocolRx_UDS_Rx_Func_Id_0x7DF_CANNODE_0_PduR2Dcm,
		(PduR_upTpStartOfReceptionFP) PduR_RF_Dcm_StartOfReception,
		(PduR_upTpProvideRxBufFP) PduR_RF_Dcm_CopyRxData,
		(PduR_upTpRxIndicationFP) PduR_RF_Dcm_TpRxIndication } /*UDS_Rx_Func_Id_0x7DF_CANNODE_0_CanTp2PduR*/
};
#endif  /* PDUR_CONFIG_SINGLE_TPRX */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
static const PduIdType PduR_CanTpRxToUpMapTable[] = {

/*CanTp2Dcm_0x737 0 */
0,
/*CanTp2Dcm_0x7DF 1 */
1 };

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/* Generating PbCfg_c::PduR_LoTpTxToUp_PBcfg_c::loTpTx_To_Up */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
#if defined(PDUR_CONFIG_SINGLE_TPTX_UP )
#define PduR_CanTpTxToUp NULL_PTR
#else
static const PduR_RT_LoTpTxToUp PduR_CanTpTxToUp[] = { {
		DcmConf_DcmDslProtocolTx_UDS_Tx_Phys_Id_0x7B7_CANNODE_0_Phys_Dcm2PduR,
		(PduR_upTpProvideTxBufFP) PduR_RF_Dcm_CopyTxData,
		(PduR_upTpTxConfirmationFP) PduR_RF_Dcm_TpTxConfirmation } /*Index: 0 src: UDS_Tx_Phys_Id_0x7B7_CANNODE_0_Phys_Dcm2PduR, dest : Dst_Dcm2CanTp_0x7B7 */
};
#endif  /* PDUR_CONFIG_SINGLE_TPTX_UP */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
static const PduIdType PduR_CanTpTxToUpMapTable[] = {

/*Dst_Dcm2CanTp_0x7B7 0 */
0 };

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/* Generating PbCfg_c::PduR_Mc_UpTpToLo_PBcfg_c::mcUpTp_To_Lo */
/* Generating PbCfg_c::PduR_Mc_UpIfToLo_PBcfg_c::mcUpIf_To_Lo */
/* Generating PbCfg_c::PduR_Mc_GwToLo_PBcfg_c::DisplayPduR_mcGwToLo */
/* Generating PbCfg_c::PduR_GwIfTx_PBcfg_c::display_GwIfTx */

/* Generating PbCfg_c::PduR_GwIf_PBcfg_c::display_GwIf */
/* Generating PbCfg_c::PduR_Gw_IfBuf_PBcfg_c::PduR_gw_Buf_If_structure */
/*****   __KW_COMMON   ***********/

/* Generating PbCfg_c::PduR_Rpg_PBcfg_c::display_PduR_RPG*/

#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)



/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

static const PduR_RPGInfoType PduR_RPGInfo[] = {

    {
     NULL_PTR,
     NULL_PTR,
     PDUR_RPGID_NULL,
     0,
     0
    },   /* PDUR_RPGID_NULL */

    
};

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
/* ------------------------------------------------------------------------ */
/* Begin section for constants */


/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_8

#include "PduR_MemMap.h"

/* Routing enable disbale flag to control routing. */
const boolean PduR_RPG_EnRouting[] =
{
  TRUE, /*PDUR_RPGID_NULL*/
  
};

/* ------------------------------------------------------------------------ */
/* End section for constants */


/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_8

#include "PduR_MemMap.h"

#endif /* #if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0) */

/* Generating PbCfg_c::PduR_Gw_TpBuf_PBcfg_c::PduR_gw_Buf_TP_structure*/

/*****   __KW_COMMON   ***********/
/* Generating PbCfg_c::PduR_GwTp_PBcfg_c::display_GwTp */

/* Generating PbCfg_c::PduR_RpgRxTp_PBcfg_c::display_RpgRxTp */

/* Generating PbCfg_c::PduR_PbConfigType_PBcfg_c::PduR_BswLoCfg */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
static const PduR_LoTpConfig PduR_LoTpCfg[] = { { PduR_CanTpRxToUp, /* CanTp */
PduR_CanTpTxToUp, /* CanTp */
PduR_CanTpTxToUpMapTable, PduR_CanTpRxToUpMapTable, 2, /* CanTp RxToUp Number Of Entries*/
1 /* CanTp TxToUp Number Of Entries*/
} };

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/* Generating PbCfg_c::PduR_PbConfigType_PBcfg_c::PduR_BswUpCfg */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
static const PduR_UpConfig PduR_UpTpCfg[] = { { NULL_PTR, /* mcDcmToLo */
		PduR_DcmToLo, /* Dcm */
		PduR_DcmToLoMapTable, 1 /* Dcm */
} };

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/* Generating PbCfg_c::PduR_Cdd_PBcfg_c::PduR_CddCfg */

/* Generating PbCfg_c::PduR_PbConfigType_PBcfg_c::PduR_BswUpToLoRxCfg */

/*
 These structures are generated by the code generator tool. Respective module's function names are generated
 only if it is present in the PduR_PbCfg.c file in any one of the entries.
 */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

const PduR_loTransmitFuncType PduR_loTransmitTable[] = { {
		&PduR_RF_CanTp_Transmit_Func } };

const PduR_loCancelReceiveFuncType PduR_loCancelRxTable[] = { { NULL_PTR } };

const PduR_loCancelTransmitFuncType PduR_loCancelTransmitTable[] = { {
		&PduR_RF_CanTp_CancelTransmit_Func } };

const PduR_upIfRxIndicationFuncType PduR_upIfRxIndicationTable[] = {
		{ NULL_PTR } };

const PduR_upIfTxConfirmationFuncType PduR_upIfTxConfirmationTable[] = { {
		NULL_PTR } };

const PduR_upIfTriggerTxFuncType PduR_upIfTriggerTxTable[] = { { NULL_PTR } };

const PduR_upTpCopyRxDataFuncType PduR_upTpCopyRxDataTable[] = { {
		&PduR_RF_Dcm_CopyRxData_Func } };

const PduR_upTpStartOfReceptionFuncType PduR_upTpStartOfReceptionTable[] = { {
		&PduR_RF_Dcm_StartOfReception_Func } };

const PduR_upTpRxIndicationFuncType PduR_upTpRxIndicationTable[] = { {
		&PduR_RF_Dcm_TpRxIndication_Func } };

const PduR_upTpCopyTxDataFuncType PduR_upTpCopyTxDataTable[] = { {
		&PduR_RF_Dcm_CopyTxData_Func } };

const PduR_upTpTxConfirmationFuncType PduR_upTpTxConfirmationTable[] = { {
		&PduR_RF_Dcm_TpTxConfirmation_Func } };

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
/* Generating PbCfg_c::PduR_PbConfigType_PBcfg_c::pdur_PBConfigType */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
const PduR_PBConfigType PduR_GlobalPBConfig = {
		(const PduR_CddConfig*) NULL_PTR, /* PduR_CddCfg */
		(const PduR_LoTpConfig*) PduR_LoTpCfg, /* Pointer to lowerlayer Tp config structure */
		(const PduR_LoIfDConfig*) NULL_PTR, /* PduR_LoIfDCfg */
		(const PduR_LoIfTTConfig*) NULL_PTR, /* PduR_LoIfTTCfg */
		(const PduR_UpConfig*) NULL_PTR, /* PduR_UpIfCfg */

		(const PduR_UpConfig*) PduR_UpTpCfg, /* Pointer to Upperlayer Tp config structure */
		(const PduR_MT_UpToLo*) NULL_PTR, /* mcGwToLo */
		(const PduR_GT_IfTx*) NULL_PTR, /* gwIfTx */
		(const PduR_GT_If*) NULL_PTR, /* gwIf        */
		(const PduR_GT_Tp*) NULL_PTR, /* GwTp */
		(const PduR_RPG_LoTpRxToUp*) NULL_PTR, /* rpgTp */
#if defined(PDUR_TPGATEWAY_SUPPORT) && (PDUR_TPGATEWAY_SUPPORT != STD_OFF)
    (const PduR_GwTp_SessionListType * ) NULL_PTR, /*PduR_TpSession_Dynamic*/
#endif
#if defined(PDUR_MULTICAST_TO_IF_SUPPORT) && (PDUR_MULTICAST_TO_IF_SUPPORT != 0)
     (const PduR_UpIfTxConf_Config * ) PduR_UpIfTxConf_ConfigList,
#endif
#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)
     (const PduR_RPGInfoType * )        PduR_RPGInfo,        /* RoutingPathGroup ConfigInfo */
     (const boolean * )   PduR_RPG_EnRouting,  /* RoutingControl StatusInfo */
     (boolean * )  PduR_RPG_Status,        /*RAM status for each RPG*/
     (const PduR_RoutingPathGroupIdType *) PduR_RPGIdMappingTab,
     (PduR_RoutingPathGroupIdType)                              0,        /* Number of RPGs.*/
#endif
		(const PduR_UpTpToLoTpRxConfig*) NULL_PTR, /* Pointer to PduR_UpTpToLoTpRxConfig structure for supporting Cancel Receive API */
		0, /* PDUR_CONFIGURATION_ID */
		0, /*Total no of Gw Tp Routing Path*/
		0, /*Total no of Gw If Routing path*/
};

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"
const PduR_ConfigType PduR_Config = { NULL_PTR /* Void pointer initialised with null pointer as PduR_Config will not be used in case of PDUR_VARIANT_PRE_COMPILE */
};

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

