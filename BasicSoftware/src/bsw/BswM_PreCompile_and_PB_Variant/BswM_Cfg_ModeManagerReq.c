
#include "BswM.h" // required for BswM_RequestMode

#if (BSWM_RTE_IN_USE == TRUE)
#include "Rte_BswM.h"
#include "SchM_BswM.h"
#endif

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

/* Set of Immediate BswMBswModeNotification Functions (Called Entities on Mode Switch Event )*/
/* These functions are used for all Immediate Mode Notifications from BSW modules via SchM */

/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_APP_RUN( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_APP_RUN(void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_BSWM_MDG, /* user */
                         RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_APP_RUN ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_POST_RUN( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_POST_RUN(void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_BSWM_MDG, /* user */
                         RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_POST_RUN ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_PREP_SHUTDOWN( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_PREP_SHUTDOWN(
		void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_BSWM_MDG, /* user */
                         RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_PREP_SHUTDOWN ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_RUN( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_RUN(void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_BSWM_MDG, /* user */
                         RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_RUN ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_SHUTDOWN( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_SHUTDOWN(void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_BSWM_MDG, /* user */
                         RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_SHUTDOWN ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_ONE( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_ONE(
		void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_BSWM_MDG, /* user */
                         RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_STARTUP_ONE ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_TWO( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_TWO(
		void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_BSWM_MDG, /* user */
                         RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_STARTUP_TWO ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_DEFAULT_SESSION( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_DEFAULT_SESSION(
		void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_DCM_DIAGNOSTICSESSIONCONTROL, /* user */
                         RTE_MODE_DcmDiagnosticSessionControl_DEFAULT_SESSION ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_EXTENDED_DIAGNOSTIC_SESSION( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_EXTENDED_DIAGNOSTIC_SESSION(
		void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_DCM_DIAGNOSTICSESSIONCONTROL, /* user */
                         RTE_MODE_DcmDiagnosticSessionControl_EXTENDED_DIAGNOSTIC_SESSION ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_PROGRAMMING_SESSION( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_PROGRAMMING_SESSION(
		void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_DCM_DIAGNOSTICSESSIONCONTROL, /* user */
                         RTE_MODE_DcmDiagnosticSessionControl_PROGRAMMING_SESSION ); /* mode */
    #endif
}

/* Set of Deferred BswMBswModeNotification Functions (Called Entities on Mode Switch Event )*/
/* These functions are used for all deferred mode notifications from BSW modules via SchM */

/***********************************************************
 * Function name: void BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_HARD( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_HARD(
		void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET, /* user */
                         RTE_MODE_DcmEcuReset_HARD ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_JUMPTOBOOTLOADER( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_JUMPTOBOOTLOADER(
		void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET, /* user */
                         RTE_MODE_DcmEcuReset_JUMPTOBOOTLOADER ); /* mode */
    #endif
}
/***********************************************************
 * Function name: void BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_SOFT( void )
 * Description: Called Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_SOFT(
		void) {
	/* Call the generic request function */

#if (defined(BSWM_SCHM_ENABLED) && (BSWM_SCHM_ENABLED == TRUE))

    BswM_RequestMode(    BSWM_CFG_USERID_BSWM_MRP_DCM_MODENOTIFICATION_ECURESET, /* user */
                         RTE_MODE_DcmEcuReset_SOFT ); /* mode */
    #endif
}

/* Set of Deferred BswMSwcModeNotification Functions (Runnable Entities on Mode Switch Event )*/
/* These functions are used for all deferred mode notifications from SWC modules via RTE */

/***********************************************************
 * Function name: void BswM_Cfg_DfrdSwcNotification_BswM_MRP_SWC_AppMode_ECUM_STATE_APP_RUN( void )
 * Description: Runnable Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_DfrdSwcNotification_BswM_MRP_SWC_AppMode_ECUM_STATE_APP_RUN(
		void) {
	/* Call the generic request function */
	BswM_RequestMode(BSWM_CFG_USERID_BSWM_MRP_SWC_APPMODE, /* user */
	RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_APP_RUN); /* mode */
}

/***********************************************************
 * Function name: void BswM_Cfg_DfrdSwcNotification_BswM_MRP_SWC_AppMode_ECUM_STATE_POST_RUN( void )
 * Description: Runnable Entity on Mode Switch Event.
 * Parameter: None
 * Return value: None
 * Remarks:
 ***********************************************************/
void BswM_Cfg_DfrdSwcNotification_BswM_MRP_SWC_AppMode_ECUM_STATE_POST_RUN(
		void) {
	/* Call the generic request function */
	BswM_RequestMode(BSWM_CFG_USERID_BSWM_MRP_SWC_APPMODE, /* user */
	RTE_MODE_MDG_ECUM_STATE_ECUM_STATE_POST_RUN); /* mode */
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

