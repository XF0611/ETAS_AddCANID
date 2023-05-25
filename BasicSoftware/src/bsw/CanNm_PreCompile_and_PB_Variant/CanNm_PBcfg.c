
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanNm / AR42.10.0.0             Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1               Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 </VersionHead>*/

/*
 ******************************************************************************
 * Includes
 ******************************************************************************
 */

#include "CanNm_Prv.h"
#include "CanNm_PBcfg.h"

#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"

/* Configuration structure of CanNm channels */
#if (defined(BSWSIM) && (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_POSTBUILD_SELECTABLE))
CanNm_ChannelConfigType CanNm_ChannelConfigData_cs[CANNM_NUMBER_OF_CHANNELS]=
#else
const CanNm_ChannelConfigType CanNm_ChannelConfigData_cs[CANNM_NUMBER_OF_CHANNELS] =
#endif
		{
		/*CanNmCC_CanNmCluster_ETAS_CAN_1*/
		{ 1000000, /* Message cycle time         */
		100000, /* Message cycle offset time  */
		500000, /* Message timeout time       */
		0, /* Message reduced time       */
		20000, /* Immediate Nm transmission cycle time         */
		3000000, /* Network Timeout for NM-Messages       */
		1000000, /* Timeout for Repeat Message State       */
		0, /* Timeout for Remote Sleep Indication       */
		4000000, /* Timeout for bus calm down phase       */
		3, /* CanIf transmit PDU Id      */
#if (CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
        0,   /* PduR Handle Id for TxConfirmation   */
#endif
				0, /* Handle of the ComM channel     */
				8, /* Pdu Length   */
				CANNM_PDU_BYTE_0, /* Node Id Position     */
				CANNM_PDU_BYTE_1, /* Control Bit Vector position      */
				TRUE, /* Node detection enabled      */
				TRUE, /* Node ID enabled      */
				FALSE, /* Repeat Message indication enabled      */
				0x0, /* CanNm Node Id              */
				6, /* User Data Length     */
				20, /* number of Immediate Nm Pdus     */
#if (CANNM_ERACALC_ENABLED != STD_OFF)
        0,    /* Pdu id of ERA referred pdu in Com */
        0,                                        /* channel index of the channel in ERA array */
        FALSE,                /* ERA status for each channel */
        0,/* Era PDU length */
#endif
				TRUE, /* Configure whether channel has to be initialized */
				FALSE, /* Enable/Disable bus load reduction */
#if (CANNM_PN_ENABLED != STD_OFF)
        FALSE,            /* PN enabled for the channel ? */
        FALSE,        /* Keep awake on any message */
        FALSE,        /* Enable/Disable pn multiple network request handle */
#endif
#if (CANNM_RETRY_FIRST_MSG_REQ_ENABLED != STD_OFF)
        FALSE, /* Support of CanNmRetryFirstMessageRequest */
#endif       
#if (CANNM_CAR_WAKEUP_RX_ENABLED!= STD_OFF)        
        FALSE,
#if (CANNM_CAR_WAKEUP_FILTER_ENABLED!= STD_OFF)         
        FALSE, 
        0,
#endif        
        0,
        0,        
#endif      

				TRUE /* Enables/Disables the handling of the Active Wakeup Bit */
		} };

#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"

#if ((CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)||(CANNM_PN_ENABLED != STD_OFF))
#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"


/* CanNm global configuration structure */
const CanNm_GlobalConfigType CanNm_GlobalConfigData_cs =
{
#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
        CanNm_ChannelConfigData_cs
#endif
#if (CANNM_PN_ENABLED != STD_OFF)
        CanNm_PnFilterMask
#endif
#if (CANNM_EIRACALC_ENABLED != STD_OFF)|| (CANNM_ERACALC_ENABLED != STD_OFF)
        ,CanNm_PNInfoTable
        ,0
#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
        ,CANNM_NUMBER_OF_PNS
#endif
#endif
};


#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
#endif

#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
static const Std_VersionInfoType CanNm_VersionInfo =
{
    6,              // Vendor Id
    31,              // Module Id
    10,       // Sw Major Version
    0,       // Sw Minor Version
    0        // Sw Patch Version
};
#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
#endif

#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"


/* Base configuration structure */
const CanNm_ConfigType CanNmGlobalConfig = {
    /* Void pointer initialised with pointer to an instance of structure CanNm_GlobalConfigType */
    /* MR12 DIR 1.1 VIOLATION: 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment */
    &CanNm_GlobalConfigData_cs,
    &CanNm_VersionInfo
};


#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
#endif
