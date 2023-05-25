


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/


#ifndef COM_PBCFG_VARIANT_H
#define COM_PBCFG_VARIANT_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
/* Start variant defines : __KW_COMMON */


#define COM_NUM_RX_SIGNALS                  18u
#define COM_NUM_TX_SIGNALS                  25u


#define COM_NUM_TX_SIGNALGRP                0u

#define COM_NUM_TX_GRPSIGNALS               0u


#define COM_NUM_RX_SIGNALGRP                0u

#define COM_NUM_RX_GRPSIGNALS               0u


#define COM_NUM_TX_SIGNALS_AND_GRPSIGNALS   25u

#define COM_NUM_RX_SIGNALS_AND_GRPSIGNALS   18u

#define COM_NUM_TX_IPDU                     4u
#define COM_NUM_RX_IPDU                     3u

#define COM_NUM_GWSRC_IPDUS                 0u
#define COM_NUM_GRPSIG_NOGW                 0u

#ifdef COM_F_ONEEVERYN
#define COM_NUM_OF_SIG_GRPSIG_WITH_ONE_EVERY_N 0u
#endif /* #ifdef COM_F_ONEEVERYN */

#define COM_NUM_IPDUS_IN_LAST_IPDUGRP       4u


#define COM_NUM_TOTAL_IPDU_GRP              2u
/* End variant defines : __KW_COMMON */
/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

/* Start variant declarations : __KW_COMMON */


/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#ifdef COM_SIGNALGATEWAY

/* Array of Signal gateway signal config structure */
extern const Com_Prv_xGwMapSigCfg_tst                       Com_Prv_xGwMapSigCfg_acst[];

extern const Com_Prv_xGwMapSigIdCfg_tst                     Com_Prv_xGwMapSigIdCfg_acst[];
#endif /* #ifdef COM_SIGNALGATEWAY */

#ifdef COM_SIGNALGROUPGATEWAY

/* Array of Signal gateway group signal config structure */
extern const Com_Prv_xGwMapGrpSigCfg_tst                    Com_Prv_xGwMapGrpSigCfg_acst[];

extern const Com_Prv_xGwMapGrpSigIdCfg_tst                  Com_Prv_xGwMapGrpSigIdCfg_acst[];
#endif /* #ifdef COM_SIGNALGROUPGATEWAY */

/* Begin section for constants */
/* Signal Details */
extern const Com_Prv_xTxSigCfg_tst                          Com_Prv_xTxSigCfg_acst[];

/* Signal Details */
extern const Com_Prv_xRxSigCfg_tst                          Com_Prv_xRxSigCfg_acst[];

/* Tx ipdu Details */
extern const Com_Prv_xTxIpduInfoCfg_tst                     Com_Prv_xTxIpduCfg_acst[];

/* Rx ipdu Details */
extern const Com_Prv_xRxIpduInfoCfg_tst                     Com_Prv_xRxIpduCfg_acst[];

#ifdef COM_TX_IPDUCOUNTER

/* Tx ipdu counter Details */
extern const Com_Prv_xTxIpduCntrCfg_tst                     Com_Prv_xTxIpduCntrCfg_acst[];
#endif /* #ifdef COM_TX_IPDUCOUNTER */

#ifdef COM_RX_IPDUCOUNTER

/* Rx ipdu counter Details */
extern const Com_Prv_xRxIpduCntrCfg_tst                     Com_Prv_xRxIpduCntrCfg_acst[];
#endif /* #ifdef COM_RX_IPDUCOUNTER */

/* Ipdu Group details */
extern const Com_Prv_xIpduGrpInfoCfg_tst                    Com_Prv_xIpduGrpCfg_acst[];

/* Internal config structure */
#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
extern const Com_ConfigData_tst                             Com_Prv_Config_cst;
#endif

/* Array of Ipdu's placed in the order they occur in Ipdu groups */
extern const Com_IpduId_tuo                                 Com_Prv_xIPduGrp_IpduRefCfg_acuo[];

#ifdef COM_TX_SIGNALGROUP

/* Array of internal Tx signal group configuration */
extern const Com_Prv_xTxSigGrpCfg_tst                       Com_Prv_xTxSigGrpCfg_acst[];

#ifdef COM_TX_SIGNALGROUP_ARRAY

/* Array of internal Tx signal group Array configuration information */
extern const Com_Prv_xTxSigGrpArrayCfg_tst                  Com_Prv_xTxSigGrpArrayCfg_acst[];
#endif /* #ifdef COM_TX_SIGNALGROUP_ARRAY */

#endif /* #ifdef COM_TX_SIGNALGROUP */

#ifdef COM_RX_SIGNALGROUP

/* Array of internal Rx signal group configuration */
extern const Com_Prv_xRxSigGrpCfg_tst                       Com_Prv_xRxSigGrpCfg_acst[];

#ifdef COM_RX_SIGNALGROUP_ARRAY

/* Array of internal Tx signal group Array configuration information */
extern const Com_Prv_xRxSigGrpArrayCfg_tst                  Com_Prv_xRxSigGrpArrayCfg_acst[];
#endif /* #ifdef COM_RX_SIGNALGROUP_ARRAY */

#endif /* #ifdef COM_RX_SIGNALGROUP */

/* Structure for GroupSignals */
#ifdef COM_TX_SIGNALGROUP

/* Array of Tx  group signal flags */
extern const Com_Prv_xTxGrpSigCfg_tst                       Com_Prv_xTxGrpSigCfg_acst[];
#endif

#ifdef COM_RX_SIGNALGROUP

/* Array of Rx group signal flags */
extern const Com_Prv_xRxGrpSigCfg_tst                       Com_Prv_xRxGrpSigCfg_acst[];
#endif

/* COM multiple MainFunction configuration for multi-core processing */
extern const Com_MainFunctionCfgType_tst                    Com_MainFunctionCfg_acst[];


/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"


/* Filter types */
#if defined (COM_F_MASKEDNEWEQUALSX) || defined(COM_F_MASKEDNEWDIFFERSX)

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
extern  const Com_MaskX_tst Com_MaskX_acst[];

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#endif

#ifdef COM_F_MASKEDNEWDIFFERSOLD

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_32
#include "Com_MemMap.h"
extern const uint32 Com_Mask_acu32[];

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_32
#include "Com_MemMap.h"


#endif

#if defined(COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS)

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
extern const Com_POSMinMax_tst Com_POSMinMax_acst[];

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#endif

#if defined(COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG)

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
extern const Com_NEGMinMax_tst Com_NEGMinMax_acst[];

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"

#endif

#ifdef COM_F_ONEEVERYN

/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"
extern const Com_OneEveryN_tst Com_OneEveryN_Const_acst[];

/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Com_MemMap.h"


#endif

/* End variant declarations : __KW_COMMON */



#endif  /* end of COM_PBCFG_VARIANT_H */


