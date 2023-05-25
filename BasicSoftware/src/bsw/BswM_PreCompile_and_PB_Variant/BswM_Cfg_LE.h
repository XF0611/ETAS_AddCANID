#ifndef BSWM_CFG_LE_H
#define BSWM_CFG_LE_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Std_Types.h"

#include "BswM_PBDefine.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */

/******************************     BswM Logical Expression    *****************************************/

#define BSWMLOGEXP_BSWM_LE_APPRUN2POSTRUN  \
                     ( ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_POST_RUN  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_SWC_APPMODE].dataMode_u16 )  \
                     && ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_APP_RUN  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].dataMode_u16 )  \
                     && ( ECUM_WKSTATUS_NONE  ==  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].dataState ) )

#define BSWMLOGEXP_BSWM_LE_DCM_DISABLE_NM  \
                    ( DCM_DISABLE_RX_TX_NM  ==  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].dataMode_u8 )

#define BSWMLOGEXP_BSWM_LE_DCM_DISABLE_RX_ENABLE_TX_NORM  \
                    ( DCM_DISABLE_RX_ENABLE_TX_NORM  ==  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].dataMode_u8 )

#define BSWMLOGEXP_BSWM_LE_DCM_DISABLE_RX_TX_NORM  \
                    ( DCM_DISABLE_RX_TX_NORMAL  ==  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].dataMode_u8 )

#define BSWMLOGEXP_BSWM_LE_DCM_DISABLE_RX_TX_NORM_NM  \
                    ( DCM_DISABLE_RX_TX_NORM_NM  ==  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].dataMode_u8 )

#define BSWMLOGEXP_BSWM_LE_DCM_ENABLE_NM  \
                    ( DCM_ENABLE_RX_TX_NM  ==  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].dataMode_u8 )

#define BSWMLOGEXP_BSWM_LE_DCM_ENABLE_RX_DISABLE_TX_NORM  \
                    ( DCM_ENABLE_RX_DISABLE_TX_NORM  ==  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].dataMode_u8 )

#define BSWMLOGEXP_BSWM_LE_DCM_ENABLE_RX_TX_NORM  \
                    ( DCM_ENABLE_RX_TX_NORM  ==  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].dataMode_u8 )

#define BSWMLOGEXP_BSWM_LE_DCM_ENABLE_RX_TX_NORM_NM  \
                    ( DCM_ENABLE_RX_TX_NORM_NM  ==  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].dataMode_u8 )

#define BSWMLOGEXP_BSWM_LE_DCM_ECURESET_HARD  \
                     ( ( RTE_MODE_DcmEcuReset_HARD  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET].dataMode_u16 )  \
                     || ( RTE_MODE_DcmEcuReset_JUMPTOBOOTLOADER  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET].dataMode_u16 ) )

#define BSWMLOGEXP_BSWM_LE_DCM_ECURESET_SOFT  \
                    ( RTE_MODE_DcmEcuReset_SOFT  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET].dataMode_u16 )

#define BSWMLOGEXP_BSWM_LE_POSTRUN2APPRUN  \
                     ( ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_POST_RUN  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].dataMode_u16 )  \
                     && BSWMLOGEXP_BSWM_LE_POSTRUN2APPRUN_SUBLE )

#define BSWMLOGEXP_BSWM_LE_POSTRUN2APPRUN_SUBLE  \
                     ( ( ECUM_WKSTATUS_VALIDATED  ==  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].dataState )  \
                     || ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_APP_RUN  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_SWC_APPMODE].dataMode_u16 ) )

#define BSWMLOGEXP_BSWM_LE_POSTRUN2PREPSHUTDOWN  \
                     ( ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_POST_RUN  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].dataMode_u16 )  \
                     && ( ECUM_WKSTATUS_NONE  ==  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].dataState )  \
                     && ( COMM_NO_COMMUNICATION  ==  BswM_Cfg_ComMIndicationModeInfo_ast[BSWM_IDX_BSWM_MRP_COMM_CAN_NETWORK_ETAS].dataMode_u8 ) )

#define BSWMLOGEXP_BSWM_LE_PREPSHUTDOWN2SHUTDOWN  \
                    ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_PREP_SHUTDOWN  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].dataMode_u16 )

#define BSWMLOGEXP_BSWM_LE_RUN2APPRUN  \
                     ( ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_RUN  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].dataMode_u16 )  \
                     && ( ECUM_WKSTATUS_VALIDATED  ==  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].dataState ) )

#define BSWMLOGEXP_BSWM_LE_RUN2PREPSHUTDOWN  \
                     ( ( ECUM_WKSTATUS_NONE  ==  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].dataState )  \
                     && ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_RUN  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].dataMode_u16 ) )

#define BSWMLOGEXP_BSWM_LE_STARTUPONE2STARTUPTWO  \
                    ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_STARTUP_ONE  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].dataMode_u16 )

#define BSWMLOGEXP_BSWM_LE_STARTUPTWO2RUN  \
                     ( ( RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_STARTUP_TWO  ==  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].dataMode_u16 )  \
                     && ( NVM_REQ_PENDING  !=  BswM_Cfg_NvMJobModeInfo_ast[BSWM_IDX_BSWM_MRP_NVM_READALLCOMPLETE].dataMode_en ) )

/******************   Macros for checking whether the ModeValues are defined   ******************************/

#define BSWMMODEVALUE_BSWM_LE_APPRUN2POSTRUN  \
        ( ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_SWC_APPMODE].isValidModePresent_b )  \
         && ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].isValidModePresent_b )  \
         && ( FALSE  !=  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].isValidModePresent_b ) )

#define BSWMMODEVALUE_BSWM_LE_DCM_DISABLE_NM  \
        ( FALSE  !=  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_DCM_DISABLE_RX_ENABLE_TX_NORM  \
        ( FALSE  !=  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_DCM_DISABLE_RX_TX_NORM  \
        ( FALSE  !=  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_DCM_DISABLE_RX_TX_NORM_NM  \
        ( FALSE  !=  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_DCM_ENABLE_NM  \
        ( FALSE  !=  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_DCM_ENABLE_RX_DISABLE_TX_NORM  \
        ( FALSE  !=  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_DCM_ENABLE_RX_TX_NORM  \
        ( FALSE  !=  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_DCM_ENABLE_RX_TX_NORM_NM  \
        ( FALSE  !=  BswM_Cfg_DcmComModeRequestModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_COMCONTROL_CAN_NETWORK_ETAS].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_DCM_ECURESET_HARD  \
        ( ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET].isValidModePresent_b )  \
         && ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET].isValidModePresent_b ) )

#define BSWMMODEVALUE_BSWM_LE_DCM_ECURESET_SOFT  \
        ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_POSTRUN2APPRUN  \
        ( ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].isValidModePresent_b )  \
         && BSWMMODEVALUE_BSWM_LE_POSTRUN2APPRUN_SUBLE )

#define BSWMMODEVALUE_BSWM_LE_POSTRUN2APPRUN_SUBLE  \
        ( ( FALSE  !=  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].isValidModePresent_b )  \
         && ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_SWC_APPMODE].isValidModePresent_b ) )

#define BSWMMODEVALUE_BSWM_LE_POSTRUN2PREPSHUTDOWN  \
        ( ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].isValidModePresent_b )  \
         && ( FALSE  !=  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].isValidModePresent_b )  \
         && ( FALSE  !=  BswM_Cfg_ComMIndicationModeInfo_ast[BSWM_IDX_BSWM_MRP_COMM_CAN_NETWORK_ETAS].isValidModePresent_b ) )

#define BSWMMODEVALUE_BSWM_LE_PREPSHUTDOWN2SHUTDOWN  \
        ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_RUN2APPRUN  \
        ( ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].isValidModePresent_b )  \
         && ( FALSE  !=  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].isValidModePresent_b ) )

#define BSWMMODEVALUE_BSWM_LE_RUN2PREPSHUTDOWN  \
        ( ( FALSE  !=  BswM_Cfg_EcuMWkpSrcInfo_ast[BSWM_IDX_BSWM_MRP_WKSRC_PWR].isValidModePresent_b )  \
         && ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].isValidModePresent_b ) )

#define BSWMMODEVALUE_BSWM_LE_STARTUPONE2STARTUPTWO  \
        ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].isValidModePresent_b )

#define BSWMMODEVALUE_BSWM_LE_STARTUPTWO2RUN  \
        ( ( FALSE  !=  BswM_Cfg_GenericReqModeInfo_ast[BSWM_IDX_BSWM_MRP_BSWM_MDG].isValidModePresent_b )  \
         && ( FALSE  !=  BswM_Cfg_NvMJobModeInfo_ast[BSWM_IDX_BSWM_MRP_NVM_READALLCOMPLETE].isValidModePresent_b ) )

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */

/********************************  LogicalExpressionEvaluateFunctions  ***************************************/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

extern void BswM_Cfg_LE_BswM_LE_AppRun2PostRun(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_NM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_ENABLE_TX_NORM(
		boolean *isValidMode_pb, boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_TX_NORM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_TX_NORM_NM(
		boolean *isValidMode_pb, boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_NM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_DISABLE_TX_NORM(
		boolean *isValidMode_pb, boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_TX_NORM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_TX_NORM_NM(
		boolean *isValidMode_pb, boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_EcuReset_HARD(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_DCM_EcuReset_SOFT(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_PostRun2AppRun(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_PostRun2AppRun_SubLE(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_PostRun2PrepShutdown(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_PrepShutdown2Shutdown(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_Run2AppRun(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_Run2PrepShutdown(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_StartupOne2StartupTwo(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);
extern void BswM_Cfg_LE_BswM_LE_StartupTwo2Run(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#endif  /* BSWM_CFG_LE_H */
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/
