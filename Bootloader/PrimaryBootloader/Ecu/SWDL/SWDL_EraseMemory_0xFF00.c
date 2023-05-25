/*********************************************************************************************************************
*
* Description:     
* FileName:        SWDL_CheckConditionsForEnteringProgrammingSession_0x0215.c
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2021. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
UCN1HC          2021/25/12      initial commit
**********************************************************************************************************************/


/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Dcm.h"
#include "ImageM.h"
#include "ImageM_ImageContext.h"
#include "FBL_BootMCfg.h"
#include "ImageM_ImageUpdateStatus.h"

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
* Local Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/
static boolean SWDL_StartEraseMemory_Prv_IsEraseReady(void);
/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/
/*** Extern declaration for Fbl_ProgM_StartEraseMemoryCallback ***/
/**
 * @brief An UDS Wrapper API
 * 
 */
FUNC(Std_ReturnType, DCM_APPL_CODE) SWDL_StartEraseMemoryCallback_0xFF00(      
                        uint8  MemID,                                             /* PRQA S 1503,1330 *//* PRQA S 3206 */
                        VAR(uint32, AUTOMATIC) Address,                                                      /* PRQA S 1330 */
                        VAR(uint32, AUTOMATIC) Length,
                        VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,                                              /* PRQA S 1330 */
                        P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) RoutineInfo,
                        P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_APPL_DATA) ErrorCode)
{
    /* Default return is pending, unless operation is ready or error was detected */
    Std_ReturnType RetValue = E_NOT_OK;
    /* Assume no error unless there is a reason */
    *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;
    ImageM_ActionType *Action;
    uint8 *ErrCode;
    ImageM_ImageContextType *pImgContext;

	if(DCM_INITIAL == OpStatus)
	{
        RetValue = ImageM_InspectEraseImageImpl(Address, Length, &Action, &ErrCode);
        if(RetValue == E_OK)
        {
            pImgContext = ImageM_GetImgContextInstance();
            if(ImageM_InitImageBlock(pImgContext, Action->FaId, Action->Offset, Action->Len) == E_OK)
            {
                if(ImageM_ImageBlockErase(pImgContext) == E_OK)
                {   
                    pImgContext->isValid = FALSE;
                    ImageM_ImageStatus_SynchWriteApplicationValidFlag(pImgContext->isValid,pImgContext->imageId);                       /* PRQA S 3200 */
                    *ErrorCode = E_OK;
                    *RoutineInfo = 0x00;
                    RetValue = DCM_E_OK;
                }
                else
                {
                    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                }
            }
        }
        else
        {
            if(*ErrCode == IMAGEM_E_REQ_REGION_INVALID)
            {
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            }
        }
	}
    return RetValue;
}
