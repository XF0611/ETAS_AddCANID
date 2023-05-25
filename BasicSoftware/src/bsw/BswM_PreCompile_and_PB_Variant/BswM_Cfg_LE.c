
/**********************************************************************************************************************
 *  Include files                                                                                                    
 **********************************************************************************************************************/

#include "BswM.h"
#include "BswM_Prv.h"

/**********************************************************************************************************************
 *  Definition of Global Functions                                                                                                    
 **********************************************************************************************************************/

/********************************  LogicalExpressionEvaluateFunctions  ***************************************/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_AppRun2PostRun (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_AppRun2PostRun(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_APPRUN2POSTRUN) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_APPRUN2POSTRUN) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_NM (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_NM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_DISABLE_NM) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_DISABLE_NM) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_ENABLE_TX_NORM (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_ENABLE_TX_NORM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_DISABLE_RX_ENABLE_TX_NORM) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_DISABLE_RX_ENABLE_TX_NORM) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_TX_NORM (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_TX_NORM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_DISABLE_RX_TX_NORM) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_DISABLE_RX_TX_NORM) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_TX_NORM_NM (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_DISABLE_RX_TX_NORM_NM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_DISABLE_RX_TX_NORM_NM) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_DISABLE_RX_TX_NORM_NM) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_NM (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_NM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_ENABLE_NM) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_ENABLE_NM) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_DISABLE_TX_NORM (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_DISABLE_TX_NORM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_ENABLE_RX_DISABLE_TX_NORM) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_ENABLE_RX_DISABLE_TX_NORM) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_TX_NORM (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_TX_NORM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_ENABLE_RX_TX_NORM) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_ENABLE_RX_TX_NORM) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_TX_NORM_NM (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_ENABLE_RX_TX_NORM_NM(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_ENABLE_RX_TX_NORM_NM) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_ENABLE_RX_TX_NORM_NM) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_EcuReset_HARD (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_EcuReset_HARD(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_ECURESET_HARD) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_ECURESET_HARD) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_DCM_EcuReset_SOFT (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_DCM_EcuReset_SOFT(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_DCM_ECURESET_SOFT) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_DCM_ECURESET_SOFT) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_PostRun2AppRun (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_PostRun2AppRun(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_POSTRUN2APPRUN) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_POSTRUN2APPRUN) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_PostRun2AppRun_SubLE (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_PostRun2AppRun_SubLE(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_POSTRUN2APPRUN_SUBLE) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_POSTRUN2APPRUN_SUBLE) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_PostRun2PrepShutdown (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_PostRun2PrepShutdown(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_POSTRUN2PREPSHUTDOWN) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_POSTRUN2PREPSHUTDOWN) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_PrepShutdown2Shutdown (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_PrepShutdown2Shutdown(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_PREPSHUTDOWN2SHUTDOWN) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_PREPSHUTDOWN2SHUTDOWN) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_Run2AppRun (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_Run2AppRun(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_RUN2APPRUN) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_RUN2APPRUN) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_Run2PrepShutdown (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_Run2PrepShutdown(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_RUN2PREPSHUTDOWN) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_RUN2PREPSHUTDOWN) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_StartupOne2StartupTwo (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_StartupOne2StartupTwo(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_STARTUPONE2STARTUPTWO) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_STARTUPONE2STARTUPTWO) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

/*****************************************************************************************
 * Function name :   void BswM_Cfg_LE_BswM_LE_StartupTwo2Run (boolean *validMode_pb,boolean *evalResult_pb)
 * Description   :   Evaluates the logical expression if the mode value is initialized and returns the result  
 * Parameter     :   *validMode_pb: evaluates if all the modes are valid and assigns true to this address if valid, 
 *evalResult_pb: result of the logical expression is copied to this address.
 * Return value  :   void
 * Remarks       :   
 *****************************************************************************************/
void BswM_Cfg_LE_BswM_LE_StartupTwo2Run(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb) {
	/* Initialize the pointers with default values */
	*isValidMode_pb = FALSE;
	*hasLogExpRes_pb = FALSE;

	if (FALSE != BSWMMODEVALUE_BSWM_LE_STARTUPTWO2RUN) {
		/* All the mode condition values are valid, assign TRUE to pointer */
		*isValidMode_pb = TRUE;
		if (FALSE != BSWMLOGEXP_BSWM_LE_STARTUPTWO2RUN) {
			/* Logical Expression evaluated to TRUE */
			*hasLogExpRes_pb = TRUE;
		}

	}

	return;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**********************************************************************************************************************
 *																										
 **********************************************************************************************************************/
