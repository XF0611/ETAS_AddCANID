
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: PduR  / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#ifndef PDUR_CFG_SYMBOLICNAMES_H
#define PDUR_CFG_SYMBOLICNAMES_H

/* Note: Module variant generation is done here, specifically to make below macros available on the inclusion of 
 * PduR_memmap.h header file by other modules without PduR_Cfg.h inclusion */

#define PDUR_VARIANT_PRE_COMPILE    (0)

#define PDUR_VARIANT_POSTBUILD_SELECTABLE   (1)

#if !defined(PDUR_CONFIGURATION_VARIANT)
#define PDUR_CONFIGURATION_VARIANT    PDUR_VARIANT_PRE_COMPILE
#endif /* PDUR_CONFIGURATION_VARIANT */

/* For PduRRoutingTable: Symbolic Name reference are generated for Tx Paths and Rx paths
 For TxPaths:
 PduRConf_PduRSrcPdu_<shortname of PduRSrcPdu> will be used by module which gives PduR_<UpperLayer>Transmit request e.g Com,Dcm,Up-Cdd
 PduRConf_PduRDestPdu_<shortname of PduRDestPdu> will be used by module which gives PduR_<LowerLayer>TxConfirmation callback e.g CanIf,CanTp,Low-Cdd

 For RxPaths:
 PduRConf_PduRSrcPdu_<shortname of PduRSrcPdu> will be used by module which gives PduR_<LowerLayer>RxIndication callback e.g CanIf,CanTp,Low-Cdd */

#define PduRConf_PduRSrcPdu_ComRx_0x261_CanIf2PduR_Can_Network_ETAS_Channel_CAN    0
#define PduRConf_PduRDestPdu_ComRx_0x261_PduR2Com_Can_Network_ETAS_Channel_CAN  0

#define PduRConf_PduRSrcPdu_ComRx_0x456_CanIf2PduR_Can_Network_ETAS_Channel_CAN    1
#define PduRConf_PduRDestPdu_ComRx_0x456_PduR2Com_Can_Network_ETAS_Channel_CAN  1

#define PduRConf_PduRSrcPdu_ComRx_0x81_CanIf2PduR_Can_Network_ETAS_Channel_CAN    2
#define PduRConf_PduRDestPdu_ComRx_0x81_PduR2Com_Can_Network_ETAS_Channel_CAN  2

#define PduRConf_PduRSrcPdu_ComTx_0x123_Com2PduR_Can_Network_ETAS_Channel_CAN    0
#define PduRConf_PduRDestPdu_ComTx_0x123_PduR2CanIf_Can_Network_ETAS_Channel_CAN  0

#define PduRConf_PduRSrcPdu_ComTx_0x55_Com2PduR_Can_Network_ETAS_Channel_CAN    1
#define PduRConf_PduRDestPdu_ComTx_0x55_PduR2CanIf_Can_Network_ETAS_Channel_CAN  1

#define PduRConf_PduRSrcPdu_ComTx_0x98_Com2PduR_Can_Network_ETAS_Channel_CAN    2
#define PduRConf_PduRDestPdu_ComTx_0x98_PduR2CanIf_Can_Network_ETAS_Channel_CAN  2

#define PduRConf_PduRSrcPdu_NM_Tx_0x600_Com2PduR_Can_Network_ETAS_Channel_CAN    3
#define PduRConf_PduRDestPdu_NM_Tx_0x600_PduR2CanNm_Can_Network_ETAS_Channel_CAN  0

#define PduRConf_PduRSrcPdu_UdsRxFnc_CanTp2PduR_Can_Network_ETAS_Channel_CAN    0
#define PduRConf_PduRDestPdu_UdsRxFnc_PduR2Dcm_Can_Network_ETAS_Channel_CAN  0

#define PduRConf_PduRSrcPdu_UdsRxPhy_CanTp2PduR_Can_Network_ETAS_Channel_CAN    1
#define PduRConf_PduRDestPdu_UdsRxPhy_PduR2Dcm_Can_Network_ETAS_Channel_CAN  1

#define PduRConf_PduRSrcPdu_UdsTxPhy_Dcm2PduR_Can_Network_ETAS_Channel_CAN    0
#define PduRConf_PduRDestPdu_UdsTxPhy_PduR2CanTp_Can_Network_ETAS_Channel_CAN  0

#endif /* PDUR_CFG_SYMBOLICNAMES_H */
