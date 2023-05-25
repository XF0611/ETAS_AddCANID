

#ifndef BSWM_PRV_MRP_H
#define BSWM_PRV_MRP_H

/*
 **********************************************************************************************************************
 * Function declarations
 **********************************************************************************************************************
*/

/*Note : This type represents the number of possible MRP types
 * i.e. it corresponds to the number of types available in the BswM_Cfg_MRPType_ten enum
 * If the number of elements in the enum changes, this macro must be changed to reflect the new count
 */
#define BSWM_PRV_NO_OF_MRPTYPES 22

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

#if (defined(BSWM_NO_OF_MRP_CANSM_INDICATION) && (BSWM_NO_OF_MRP_CANSM_INDICATION > 0))
boolean BswM_Prv_GetCanSMMRPIndex_b ( NetworkHandleType idNetwork_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_MRP_CANSM_INDICATION > 0 */

#if (defined(BSWM_NO_OF_MRP_COMM_INDICATION) && (BSWM_NO_OF_MRP_COMM_INDICATION > 0))
boolean BswM_Prv_GetComMMRPIndex_b  ( NetworkHandleType idNetwork_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_MRP_COMM_INDICATION > 0 */

#if (defined(BSWM_NO_OF_MRP_COMMPNCS_REQUEST) && (BSWM_NO_OF_MRP_COMMPNCS_REQUEST > 0))
boolean BswM_Prv_GetCurrentPncMRPIndex_b ( PNCHandleType PNC_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_MRP_COMMPNCS_REQUEST > 0 */

#if (defined(BSWM_NO_OF_MRP_DCM_COM_MODE_REQ) && (BSWM_NO_OF_MRP_DCM_COM_MODE_REQ > 0))
boolean BswM_Prv_GetDcmComModeRequestMRPIndex_b ( NetworkHandleType idNetwork_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_MRP_DCM_COM_MODE_REQ > 0 */

#if (defined(BSWM_NO_OF_ECUMWKP_SOURCES) && (BSWM_NO_OF_ECUMWKP_SOURCES > 0 ))
boolean BswM_Prv_GetEcuMWkpSrcMRPIndex_b( uint32 idEcuMWkpSrc_u32, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_ECUMWKP_SOURCES > 0 */

#if (defined(BSWM_NO_OF_MRP_ETHIF_PORTFGROUP) && (BSWM_NO_OF_MRP_ETHIF_PORTFGROUP > 0))
boolean BswM_Prv_GetEthIfMRPIndex_b ( EthIf_SwitchPortGroupIdxType idxPortGroup_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_MRP_ETHIF_PORTFGROUP > 0 */

#if (defined(BSWM_NO_OF_MRP_ETHSM_INDICATION) && (BSWM_NO_OF_MRP_ETHSM_INDICATION > 0))
boolean BswM_Prv_GetEthSMMRPIndex_b ( NetworkHandleType idNetwork_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_MRP_ETHSM_INDICATION > 0 */

#if (defined(BSWM_NO_OF_MRP_FRSM_INDICATION) && (BSWM_NO_OF_MRP_FRSM_INDICATION > 0))
boolean BswM_Prv_GetFrSMMRPIndex_b ( NetworkHandleType idNetwork_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_MRP_FRSM_INDICATION > 0 */

#if (defined(BSWM_NO_OF_GENREQ_TOTAL) && (BSWM_NO_OF_GENREQ_TOTAL > 0))
boolean BswM_Prv_GetGenReqMRPIndex_b( uint16 idRequester_u16, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_GENREQ_TOTAL > 0 */

#if (defined(BSWM_NO_OF_NM_CHANNELS) && (BSWM_NO_OF_NM_CHANNELS > 0 ))
boolean BswM_Prv_GetNmIfWkupIndMRPIndex_b( uint8 idChannel_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_NM_CHANNELS > 0 */

#if (defined(BSWM_NO_OF_NVM_REQUESTS) && (BSWM_NO_OF_NVM_REQUESTS > 0 ))
boolean BswM_Prv_GetNvMBlockModeMRPIndex_b( uint16 idBlock_u16, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_NVM_REQUESTS > 0 */

#if (defined(BSWM_NO_OF_NVM_SERVICE_IDS) && (BSWM_NO_OF_NVM_SERVICE_IDS > 0 ))
boolean BswM_Prv_GetNvMJobModeMRPIndex_b( uint8 idService_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_NVM_SERVICE_IDS > 0 */

#if (defined(BSWM_NO_OF_ECUMRUNREQ_STATES) && (BSWM_NO_OF_ECUMRUNREQ_STATES > 0 ))
boolean BswM_Prv_GetEcuMRUNReqIndicationMRPIndex_b  ( EcuM_StateType State, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_ECUMRUNREQ_STATES > 0 */

#if (defined(BSWM_NO_OF_J1939DCM_CHANNELS) && (BSWM_NO_OF_J1939DCM_CHANNELS > 0 ))
boolean BswM_Prv_GetJ1939DcmIndex_b( uint16 idNetworkMask_u16, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_J1939DCM_CHANNELS > 0 */

#if (defined(BSWM_NO_OF_J1939NM_CHANNELS) && (BSWM_NO_OF_J1939NM_CHANNELS > 0 ))
boolean BswM_Prv_GetJ1939NmNodeChnIndex_b(NetworkHandleType idNetwork_u8, uint8 idNode_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_J1939NM_CHANNELS > 0 */

#if (defined(BSWM_NO_OF_LINSM_CHANNELS) && (BSWM_NO_OF_LINSM_CHANNELS > 0 ))
boolean BswM_Prv_GetLinSmCurrentStateIndex_b(NetworkHandleType idNetwork_u8,  uint16 * idx_pu16);
#endif /* BSWM_NO_OF_LINSM_CHANNELS > 0 */

#if (defined(BSWM_NO_OF_LINSMSCH_CHANNELS) && (BSWM_NO_OF_LINSMSCH_CHANNELS > 0 ))
boolean BswM_Prv_GetLinSmCurrentScheduleChnIndex_b(NetworkHandleType idNetwork_u8, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_LINSMSCH_CHANNELS > 0 */

#if (defined(BSWM_NO_OF_LINTP_CHANNELS) && (BSWM_NO_OF_LINTP_CHANNELS > 0 ))
boolean BswM_Prv_GetLinTpModeReqChnIndex_b(NetworkHandleType idNetwork_u8,  uint16 * idx_pu16);
#endif /* BSWM_NO_OF_LINTP_CHANNELS > 0 */


#if (defined(BSWM_NO_OF_SDCLIENTSRV_CS) && (BSWM_NO_OF_SDCLIENTSRV_CS > 0 ))
boolean BswM_Prv_GetSdclientSrvCSMRPIndex_b(uint16 idNetwork_u16,  uint16 * idx_pu16);
#endif /* BSWM_NO_OF_SDCLIENTSRV_CS > 0 */

#if (defined(BSWM_NO_OF_SDCNSEVGRP_HANDLE_CS) && (BSWM_NO_OF_SDCNSEVGRP_HANDLE_CS > 0 ))
boolean BswM_Prv_GetSdConsEvntGrpCSMRPIndex_b(uint16 idNetwork_u16, uint16 * idx_pu16);
#endif /* BSWM_NO_OF_SDCNSEVGRP_HANDLE_CS > 0 */

#if (defined(BSWM_NO_OF_SDEVNTHNDLR_HANDLE_CS) && (BSWM_NO_OF_SDEVNTHNDLR_HANDLE_CS > 0 ))
boolean BswM_Prv_GetSdEvntHndrCSMRPIndex_b(uint16 idNetwork_u16,  uint16 * idx_pu16);
#endif /* BSWM_NO_OF_SDEVNTHNDLR_HANDLE_CS > 0 */

boolean BswM_Prv_ReqProcessOrQueue_b( BswM_Cfg_MRPType_ten dataMRPType_en, uint16 idChannel_u16, uint16 idxMRPChnl_u16, uint16 dataReqMode_u16);
void BswM_Prv_RuleEval ( BswM_ReqProcessing_ten dataReqProcessing_en, BswM_Cfg_MRPType_ten MRPType, uint16 requesting_user, const uint16 * adrRulesRef_pu16, uint16 nrAssociatedRules_u16);

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#endif /* BSWM_PRV_MRP_H_ */
