
#ifndef BSWM_PRV_H
#define BSWM_PRV_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "BswM_Prv_AC.h"
#include "BswM_Prv_AL.h"
#include "BswM_Prv_RL.h"
#include "BswM_Prv_IntrptQueue.h"
#include "BswM_Prv_ProcessDelayedReqst.h"
#include "BswM_Prv_ProcessDeferredRequest.h"
#include "BswM_Prv_MRP.h"
#include "BswM_Cfg_SchM.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* Interface API IDs for DET interface*/
#define BSWM_VENDOR_ID                  ( 6 )
#define BSWM_MODULE_ID                  ( 42 )
#define BSWM_INSTANCE_ID                ( 0 )
#define BSWM_INIT_API_ID                ( 0x00 )
#define BSWM_GETVERINFO_API_ID          ( 0x01 )
#define BSWM_API_ID_REQSTMODE       	( 0x02 )
#define BSWM_API_ID_MAINFUNCTION        ( 0x03 )
#define BSWM_DEINIT_API_ID              ( 0x04 )
#define BSWM_API_ID_CANSM_CS            ( 0x05 )
#define BSWM_API_ID_DCM_COMMODE_CS      ( 0x06 )
#define BSWM_API_ID_FRSM_CS             ( 0x0C )
#define BSWM_API_ID_ETHIFGRPSTATE       ( 0x26 )
#define BSWM_API_ID_ETHSM_CS            ( 0x0D )
#define BSWM_API_ID_COMM_CM             ( 0x0E )
#define BSWM_API_ID_ECUMCRNTST          ( 0x0F )
#define BSWM_API_ID_ECUMCURRWAKEUP      ( 0x10 )
#define BSWM_API_ID_ECUMRUNREQ          ( 0x23 )
#define BSWM_API_ID_DCM_APP_UPDATE      ( 0x14 )
#define BSWM_API_ID_CUR_PNC_MODE        ( 0x15 )
#define BSWM_API_ID_NVMCURRBLOCKMODE    ( 0x16 )
#define BSWM_API_ID_NVMCURRJOBMODE      ( 0x17 )
#define BSWM_API_ID_COMM_INIT_RST       ( 0x22 )
#define BSWM_API_ID_NMIFCARWKUPIND      ( 0x24 )
#define BSWM_API_ID_J1939DCM            ( 0x1b )
#define BSWM_API_ID_J1939NM             ( 0x18 )
#define BSWM_API_ID_LINSMCRNTST         ( 0x09 )
#define BSWM_API_ID_LINSMCRNTSCD        ( 0x0a )
#define BSWM_API_ID_LINTPREQMD          ( 0x0b )

#define BSWM_API_SD_CLINTSERVCS         ( 0x1f )
#define BSWM_API_SD_CONSEVNTGRPCS       ( 0x21 )
#define BSWM_API_SD_EVNTHNDRCS          ( 0x20 )

#define BSWM_E_NO_INIT                  ( 0x01 )
#define BSWM_E_NULL_POINTER             ( 0x02 )
#define BSWM_E_PARAM_CONFIG             ( 0x06 )
#define BSWM_E_REQ_USER_OUT_OF_RANGE    ( 0x04 )
#define BSWM_E_REQ_MODE_OUT_OF_RANGE    ( 0x05 )


#if (BSWM_DEV_ERROR_DETECT == STD_ON)
#define BSWM_DET_REPORT_ERROR(ApiId, ErrorId)       (void)Det_ReportError(BSWM_MODULE_ID, BSWM_INSTANCE_ID, (ApiId), (ErrorId))
#else
#define BSWM_DET_REPORT_ERROR(ApiId, ErrorId)
#endif

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define BSWM_START_SEC_VAR_CLEARED_BOOLEAN
#include "BswM_MemMap.h"

extern boolean BswM_Prv_flgDelayedReqstProgress_b;
extern boolean BswM_Prv_flgDeferredReqstProgress_b;
extern boolean BswM_Prv_flgNewReqstProgress_b;
extern boolean BswM_Prv_flgDelayDeferredReqst_b;
extern boolean BswM_Prv_isReqstDelayed_b;
extern boolean BswM_Prv_flgTimerActionState_b;

#define BSWM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "BswM_MemMap.h"

#define BSWM_START_SEC_VAR_INIT_BOOLEAN
#include "BswM_MemMap.h"

/* Private Global variable to hold the BswM module initialisation status */
extern boolean BswM_Prv_isModuleInitialised_b;

#define BSWM_STOP_SEC_VAR_INIT_BOOLEAN
#include "BswM_MemMap.h"

#define BSWM_START_SEC_VAR_CLEARED_8
#include "BswM_MemMap.h"

extern uint8 BswM_Prv_ctrInterrupt_u8;

#define BSWM_STOP_SEC_VAR_CLEARED_8
#include "BswM_MemMap.h"

#define BSWM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
#include "BswM_MemMap.h"

extern uint8 BswM_Rb_ctrInterrupt_au8[2];

#define BSWM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
#include "BswM_MemMap.h"

#define BSWM_START_SEC_VAR_CLEARED_32
#include "BswM_MemMap.h"

extern const BswM_ConfigType * BswM_Prv_adrSelectedConfig_pcst;

#if (defined(BSWM_NO_OF_AC_BSWMTIMERCONTROL) && (BSWM_NO_OF_AC_BSWMTIMERCONTROL > 0))
extern float32 BswM_Prv_ctrTimerControlAction_f32;
extern float32 BswM_Prv_ctrvalue_f32;
#endif

#define BSWM_STOP_SEC_VAR_CLEARED_32
#include "BswM_MemMap.h"

#if (defined(BSWM_NO_OF_AC_IPDUGROUPSWITCH) && (BSWM_NO_OF_AC_IPDUGROUPSWITCH > 0 ))

#define BSWM_START_SEC_VAR_CLEARED_BOOLEAN
#include "BswM_MemMap.h"
/*Glabal flag that holds the information wheater PDU group switch action has been performed. */
extern boolean BswM_PduGrpSwt_b;
#define BSWM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "BswM_MemMap.h"

#define BSWM_START_SEC_VAR_CLEARED_8
#include "BswM_MemMap.h"
/* PDU group vector which holds the information of the PDu Groups that need initiliation or are newly added */
extern Com_IpduGroupVector BswM_IPduGrpVctr_ReinitTrue_au8;
/* PDU group vector which holds the information of all the  PDu Groups */
extern Com_IpduGroupVector BswM_IPduGrpVctr_ReinitAll_au8;
#define BSWM_STOP_SEC_VAR_CLEARED_8
#include "BswM_MemMap.h"

#endif /* ( BSWM_NO_OF_AC_IPDUGROUPSWITCH ) */


/*
 **********************************************************************************************************************
 * Function definitions
 **********************************************************************************************************************
*/

/* this macro checks whether there are any requests currently in progress
 * BswM makes use of three flags for New, Deferred and Delayed requests in progress.
 * returns TRUE: If there are no requests in progress, FALSE: If a request is in progress */

#define BSWM_PRV_ISNOREQSTINPROGRESS    \
        ( TRUE != BswM_Prv_flgDelayedReqstProgress_b   ) && \
        ( TRUE != BswM_Prv_flgDeferredReqstProgress_b  ) && \
        ( TRUE != BswM_Prv_flgNewReqstProgress_b ) && \
        ( TRUE != BswM_Prv_flgTimerActionState_b )

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

/* Function to copy the initial mode values from the constant post build structure */
extern void BswM_Prv_CopyModeInitValues(void);

/* Function to copy the initial rule states from the constant post build structure */
extern void BswM_Prv_CopyRuleInitSates(void);

/* Error handler for version incompatibility check */
extern void BswM_Appl_IncompatibleGenerator( void );

/* Function to perform I-Pdu Group action handling*/
void BswM_Prv_CalcPduGrpSwt(void);

/*Interrupt handling function : updates NvM section when requests are interrupted*/
void BswM_Prv_InterruptHandler (uint8 dataApiId_u8);

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#endif
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/
