/*********************************************************************************************************************
*
* Description:     ProgM module, DCM call-back function implementation
* FileName:        FBL_ProgM.c
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2019. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Dcm.h"
#include "EcuMgt.h"
#include "Sleep.h"
#include "ImageM.h"
#include "FBL_SeedAndKey.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables Implementation
**********************************************************************************************************************/
/**
 * @brief An UDS callback API
 * 
 */
FUNC(void,DCM_APPL_CODE)  DcmAppl_Switch_DcmExecuteEcuReset(VAR(uint8,AUTOMATIC) ResetType_u8)	                                   /* PRQA S 3206, 1503 */ /* This function does not use this parameter */
{
    /* Call the reset rountine*/
    (void)EcuMgt_PerformReset();
}

/**
 * @brief An UDS callback API
 * 
 */
FUNC(Std_ReturnType,DCM_APPL_CODE) Fbl_ProgM_DisableDtc(    P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_INTERN_DATA) Nrc_u8,	/* PRQA S 3206, 1503 */ /* This function does not use this parameter */
                                                            VAR(uint8,AUTOMATIC) Sid_u8,	                                        /* PRQA S 3206 */ /* This function does not use this parameter */
                                                            VAR(uint8,AUTOMATIC) Subfunc_u8)	                                    /* PRQA S 3206 */ /* This function does not use this parameter */
{
    /* Always return positive response - DTC not supported in FBL */
    return E_OK;
}

/**
 * @brief An UDS callback API
 * 
 */
FUNC(Std_ReturnType,DCM_APPL_CODE) Fbl_ProgM_EnableDtc( P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_INTERN_DATA) Nrc_u8,       /* PRQA S 1503 */
                                                        VAR(uint8,AUTOMATIC) Sid_u8,		                                        /* PRQA S 3206, 1503 */ /* This function does not use this parameter */
                                                        VAR(uint8,AUTOMATIC) Subfunc_u8)	                                        /* PRQA S 3206 */ /* This function does not use this parameter */
{

    /* Always return negative response - DTC not supported in FBL */
    *Nrc_u8 = DCM_E_CONDITIONSNOTCORRECT;
    return E_NOT_OK;
}

/**
 * @brief An UDS callback API
 * 
 */
FUNC(Std_ReturnType,DCM_APPL_CODE) Fbl_ProgM_DisableRxAndTx(    P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_INTERN_DATA) Nrc_u8,   /* PRQA S 1503 */
                                                                VAR(uint8,AUTOMATIC) Sid_u8,		                                /* PRQA S 3206, 1503 */ /* This function does not use this parameter */
                                                                VAR(uint8,AUTOMATIC) Subfunc_u8)	                                /* PRQA S 3206 */ /* This function does not use this parameter */
{
    /* Always return positive response */
	*Nrc_u8 = E_OK;
    return E_OK;
}

/**
 * @brief An UDS callback API
 * 
 */
FUNC(Std_ReturnType,DCM_APPL_CODE) Fbl_ProgM_EnableRxAndTx( P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_INTERN_DATA) Nrc_u8,   /* PRQA S 1503 */
                                                            VAR(uint8,AUTOMATIC) Sid_u8,		                                    /* PRQA S 3206, 1503 */ /* This function does not use this parameter */
                                                            VAR(uint8,AUTOMATIC) Subfunc_u8)	                                    /* PRQA S 3206 */ /* This function does not use this parameter */
{
    /* Always return positive response */
	*Nrc_u8 = E_OK;
    return E_OK;
}

/***Seca dcmDspSecurityGetSeedFnc functions with Useport  USE_ASYNCH_FNC ***/
/**
 * @brief An UDS callback API
 * 
 */
FUNC(Std_ReturnType, DCM_APPL_CODE) DcmGetSeedDSP_Fbl_ProgM_GetSeedCallback(VAR(Dcm_SecLevelType,AUTOMATIC) SecLevel_u8,                                /* PRQA S 3206, 1503 */ /* This function does not use this parameter */
                                                                            VAR(uint32,AUTOMATIC) Seedlen_u32,                                          /* PRQA S 3206 */                  
                                                                            VAR(uint32,AUTOMATIC) AccDataRecsize_u32,                                   /* PRQA S 3206 */
                                                                            P2VAR(uint8,AUTOMATIC,DCM_INTERN_DATA) SecurityAccessDataRecord,            /* PRQA S 3206 */
                                                                            P2VAR(uint8,AUTOMATIC,DCM_INTERN_DATA) Seed,                                /* PRQA S 3206 */
                                                                            VAR(Dcm_OpStatusType,AUTOMATIC) OpStatus,                                   /* PRQA S 3206 */ /* This function does not use this parameter */
                                                                            P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_INTERN_DATA) ErrorCode)
{
    Std_ReturnType  retVal = E_OK;
    *ErrorCode = E_OK;

    if(E_OK != Fbl_SeedAndKey_GetSeed(Seedlen_u32, Seed))
    {
        retVal = E_NOT_OK;
        *ErrorCode  = DCM_E_CONDITIONSNOTCORRECT;
    }
    
    return retVal;
}

/***Seca dcmDspSecurityCompareKeyFnc functions with Useport  USE_ASYNCH_FNC ***/
/**
 * @brief An UDS callback API
 * 
 */
FUNC(Std_ReturnType, DCM_APPL_CODE) DcmCompareKeyDSP_Fbl_ProgM_CompareKeyCallback( VAR(uint32,AUTOMATIC) KeyLen_32,                 /* PRQA S 3206, 1503 */
                                                                                    const uint8 * Key,                              /* PRQA S 3206, 3673 */ /* The object addressed by the pointer parameter 'Key' is not modified and so the pointer could be of type 'pointer to const'*/
                                                                                    VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,      /* PRQA S 3206 */ /* This function does not use this parameter */
                                                                                    P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,
                                                                                    DCM_INTERN_DATA) ErrorCode)
{
    Std_ReturnType retVal = E_OK;
    *ErrorCode = E_OK;
    if(E_OK != FBl_SeedAndKey_CompareKey(KeyLen_32, Key))
    {
        retVal = E_NOT_OK;
        *ErrorCode = DCM_E_INVALIDKEY;
    }
    return retVal;
}

/**
 * @brief An UDS callback API
 * 
 */
FUNC(Std_ReturnType,DCM_APPL_CODE) DcmDspData_Fingerprint_WriteFunc (P2CONST(uint8,AUTOMATIC,DCM_INTERN_DATA) Data,     /* PRQA S 3206, 1503, 3408 */        
                                                                    VAR(Dcm_OpStatusType,AUTOMATIC) OpStatus,           /* PRQA S 3206 */ /* This function does not use this parameter */
                                                                    P2VAR(Dcm_NegativeResponseCodeType,
                                                                    AUTOMATIC,DCM_INTERN_DATA) ErrorCode)
{
    Std_ReturnType retVal = E_NOT_OK;

    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    //retVal =  EcuMgr_DIDService_StoreFingerPrintInfor(Data);

    if(E_OK == retVal)
    {
    	*ErrorCode = E_OK;
    }

    return retVal;
}

/**
 * @brief An UDS callback API
 * 
 */
FUNC(Std_ReturnType,DCM_APPL_CODE) DcmDspData_Fingerprint_ReadFunc (VAR(Dcm_OpStatusType,AUTOMATIC) OpStatus,           /* PRQA S 3206, 1503, 3408 */ /* This function does not use this parameter */
                                                                    P2VAR(uint8,AUTOMATIC,DCM_INTERN_DATA) Data)        /* PRQA S 3206 */
{
    Std_ReturnType	retVal = E_NOT_OK;

    //retVal = EcuMgr_DIDService_ReadFingerPrintInfor(Data);	                                                         /* PRQA S 0310 */ /* Casting from type of high layer to type of low layer */

    return retVal;
}

 /**
 * @ingroup DCM_TPL
 * DcmAppl_UserServiceModeRuleService :-\n
 * This API is used to do specific checks on the SID before processing the service.The ErrorCode parameter(Nrc_u8) needs to be updated by the application in case of failed checks.
 *
 * @param[in]     Sid_u8 : Service Id under processing
 * @param[out]    Nrc_u8 : NRC to be set by the application
 *
 * @retval               E_OK      : Service specific checks passed successfully \n
 * @retval               E_NOT_OK  : Service specific checks failed\n
 *
 */
FUNC(Std_ReturnType,DCM_APPL_CODE) DcmAppl_UserServiceModeRuleService(  P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_INTERN_DATA) Nrc_u8,   /* PRQA S 1503 */
                                                                        VAR(uint8,AUTOMATIC) Sid_u8)                                            /* PRQA S 3206 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
{
	VAR(Std_ReturnType,AUTOMATIC) retVal_u8 = E_OK;
	*Nrc_u8= E_OK;

	Sleep_StartStopTimer();
	return (retVal_u8);
}


/**
 * @brief DCM VIN call back func
 * 
 */
FUNC(Std_ReturnType,DCM_APPL_CODE) DcmDspData_VIN_ReadFunc (VAR(Dcm_OpStatusType,AUTOMATIC) OpStatus,                                          /* PRQA S 3206 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
                                                            P2VAR(uint8,AUTOMATIC,DCM_INTERN_DATA) Data)                                       /* PRQA S 3206 */ /* MISRA DEVIATION: The parameter is auto-generated and used in others. */
{
    return E_OK;
}

