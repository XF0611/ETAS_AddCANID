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
#include "Os_TimeServices.h"
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

/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/
/**
 * @ingroup DCM_TPL
 * Dcm_WriteMemory:-\n
 * This API implements Write to memory functionality.User Application should add the necessary code to write the data in to memory address specified by the tester.\n
 *
 *
 * @param[in]       Wmba_Opstatus   :   Information on the opstatus\n
 * @param[in]       memoryid        :   Identifier of the memory block\n
 * @param[in]       memoryaddress   :   Starting address of the server memory address to be copied\n
 * @param[in]       datalength      :   Number of bytes in memory data\n
 * @param[out]      reqbuf          :   Data will be written into this buffer(points to the diagnostic buffer).\n
 * @param[out]      ErrorCode       : The negative response code to be updated by the application\n
 *                                    In case the Wmba_Opstatus value is passed as DCM_CANCEL, then the ErrorCode parameter shall be a NULL_PTR.
 * @retval          DCM_WRITE_OK       :  WRITE was successful\n
 * @retval          DCM_WRITE_FAILED   :  WRITE was not successful\n
 * @retval          DCM_WRITE_PENDING  :  WRITE is not yet finished\n
 * @retval          DCM_WRITE_FORCE_RCRRP : If the Pending response (0x78) has to be triggered by Dcm immediately.
 * @retval          DCM_E_REQUESTOUTOFRANGE : Request Out Of Range
 */
 FUNC(Dcm_ReturnWriteMemoryType,DCM_APPL_CODE) DcmAppl_Dcm_WriteMemory(VAR(Dcm_OpStatusType,  AUTOMATIC) Wmba_Opstatus,	/* PRQA S 3206, 1503, 3408 */ /* This function does not use this parameter */
                                                     VAR(uint8,  AUTOMATIC) memoryid,	/* PRQA S 3206 */ /* This function does not use this parameter */
                                                     VAR(uint32,  AUTOMATIC) memoryaddress,
                                                     VAR(uint32,  AUTOMATIC) datalength,
                                                     P2CONST (uint8,AUTOMATIC,DCM_INTERN_DATA) reqbuf,
                                                     P2VAR (Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_INTERN_DATA) ErrorCode)
{
    /* Default return is pending, unless operation is ready or error was detected */
    VAR(Dcm_ReturnWriteMemoryType,AUTOMATIC) WriteRetVal = DCM_WRITE_FAILED;
    uint8 *ErrCode;
    ImageM_ActionType *Action;
    Std_ReturnType RetValue = E_NOT_OK;
    ImageM_ImageContextType *pImgContext;
    *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;

    RetValue = ImageM_InspectUploadImageImpl(memoryaddress, datalength, reqbuf, &Action, &ErrCode);
    if(RetValue == E_OK)
    {
        pImgContext = ImageM_GetImgContextInstance();
        if(E_OK == ImageM_ImageBlockWrite(pImgContext, Action->Offset, Action->DataPtr, Action->Len))
        {
            *ErrorCode = E_OK;
            WriteRetVal = DCM_WRITE_OK;
        }
        else
        {
            *ErrorCode = E_NOT_OK;
            WriteRetVal = DCM_WRITE_FAILED;
        }
    }
    else
    {
        if(*ErrCode == IMAGEM_E_REQ_OUT_OF_RANGE)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }
    return(WriteRetVal);
}
