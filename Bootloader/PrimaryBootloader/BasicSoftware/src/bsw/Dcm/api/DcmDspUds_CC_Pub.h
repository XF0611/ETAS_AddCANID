

#ifndef DCMDSPUDS_CC_PUB_H
#define DCMDSPUDS_CC_PUB_H


/**
 ***************************************************************************************************
            Communication Control service
 ***************************************************************************************************
 */
#if(DCM_CFG_DSP_COMMUNICATIONCONTROL_ENABLED != DCM_CFG_OFF)
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Dcm_Dsld_SpecificSubnetInfo:Structure contains information about the specific subnets supported by DCM.\n
 * Std_ReturnType (*switch_fp) (Dcm_CommunicationModeType ); Function pointer to the Schm_switch function.\n
 * boolean (*checkmode_fp) (void);Pointer to function to check whether the ComMCahnnel is in the default mode.\n
 * NetworkHandleType 	SpecificSubNetId_u8; Subnet ID\n
 * uint8   				SpecificComMChannelId; ComM channel ID of the subnet
 */
typedef struct
{
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
	Std_ReturnType (*switch_fp) (Dcm_CommunicationModeType Dcm_stCommunicationMode );
	boolean (*checkmode_fp) (void);
#endif
	NetworkHandleType 						SpecificSubNetId_u8;
	uint8   								SpecificComMChannelId;
}Dcm_Dsld_SpecificSubnetInfo;

/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Dcm_Dsld_AllChannelsInfoType:Structure contains information about ComMChannels supported by DCM.\n
 * Std_ReturnType (*switch_fp) (Dcm_CommunicationModeType ); Function pointer to the Schm_switch function.\n
 * boolean (*checkmode_fp) (void);Pointer to function to check whether the ComMCahnnel is in the default mode.\n
 * uint8   				AllComMChannelId; ComM channel ID
 */
typedef struct
{
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
	Std_ReturnType (*switch_fp) (Dcm_CommunicationModeType Dcm_stCommunicationMode );
	boolean (*checkmode_fp) (void);
#endif

	uint8   AllComMChannelId;
}Dcm_Dsld_AllChannelsInfoType;

#if (DCM_CFG_NUM_SUB_NODES != 0)
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Dcm_Dsld_SubNodeInfo: Structure contains information about the sub nodes supported by DCM.\n
 * Std_ReturnType (*switch_fp) (Dcm_CommunicationModeType ); Function pointer to the Schm_switch function.\n
 * boolean (*checkmode_fp) (void): Pointer to function to check whether the ComMCahnnel is in the default mode.\n
 * uint16    SubNodeId_u8: nodeIdentificationNumber of the Sub node \n
 * boolean   SubNodeUsed_b: Sub node used or not \n
 * uint8     SubNodeComMChannelId; ComM channel ID of the subnode
 */
typedef struct
{
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
    Std_ReturnType (*switch_fp) (Dcm_CommunicationModeType Dcm_stCommunicationMode );
    boolean (*checkmode_fp) (void);
#endif
    uint16  SubNodeId_u8;
    boolean SubNodeUsed_b;
    uint8   SubNodeComMChannelId;
}Dcm_Dsld_SubNodeInfo;
#endif

#if ( DCM_CFG_NUM_SPECIFIC_SUBNETS != 0 )
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Stucture array to store specific subent info.
 */
extern const Dcm_Dsld_SpecificSubnetInfo Dcm_specific_subnet_table[DCM_CFG_NUM_SPECIFIC_SUBNETS];
#define DCM_STOP_SEC_CONST_UNSPECIFIED/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Stucture array to store  ComM channels info which are referred in DCM.
 */
extern const Dcm_Dsld_AllChannelsInfoType Dcm_AllChannels_ForModeInfo[DCM_CFG_NUM_ALLCHANNELS_MODE_INFO];
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if(DCM_CFG_DSP_MODERULEFORCC != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Pointer to the function which checks whether the mode rule is failed or not.
 */
extern boolean (*Dcm_ComMReEnableModeRuleRef) (Dcm_NegativeResponseCodeType * Nrc_u8);

#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif
#define DCM_START_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Pointer to the user function which checks whether the mode rule is failed or not.
 */
extern Std_ReturnType (*Dcm_ComMUserReEnableModeRuleRef) (void);
#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if (DCM_CFG_NUM_SUB_NODES != 0)
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Stucture array to store sub node info.
 */
extern const Dcm_Dsld_SubNodeInfo Dcm_SubNode_table[DCM_CFG_NUM_SUB_NODES];
#define DCM_STOP_SEC_CONST_UNSPECIFIED/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#endif   /* _DCMDSPUDS_CC_PUB_H */

