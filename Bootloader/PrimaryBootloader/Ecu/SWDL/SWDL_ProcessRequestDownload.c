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
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/
#define DEFAULT_DATA_FORMAT_IDENTIFIER      0x000000000U
/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/
typedef enum
{
    IMAGEM_SWDL_STATE_NONE = 0,     
    IMAGEM_SWDL_STATE_ABORT,        /*Reject further request */
}ImageM_SWDL_StateType;
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
 * @brief An UDS Wrapper API
 * 
 */
/* MISRA RULE 3206: The parameters is not used in this function. */
FUNC(Std_ReturnType,DCM_APPL_CODE) DcmAppl_Dcm_ProcessRequestDownload(                                                      /* PRQA S 1503, 3408 */
                                            VAR(Dcm_OpStatusType,AUTOMATIC) OpStatus,/* PRQA S 3206 */ /* This function does not use this parameter */
                                            VAR(uint8,AUTOMATIC) DataFormatIdentifier,
                                            VAR(uint32,AUTOMATIC) MemoryAddress,
                                            VAR(uint32,AUTOMATIC) MemorySize,
                                            P2VAR(uint32,DCM_INTERN_DATA,AUTOMATIC) BlockLength,
                                            P2VAR(Dcm_NegativeResponseCodeType,DCM_INTERN_DATA,AUTOMATIC) ErrorCode)
{
    Std_ReturnType RetValue = E_NOT_OK;
    uint8 Compression;
    uint8 Encryption;
    uint8 *ErrCode;
    ImageM_ActionType *Action;
    ImageM_ImageContextType *pImgContext;
    *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;


    Compression = (DataFormatIdentifier & 0xF0u) >> 4u;
    Encryption = DataFormatIdentifier & 0x0Fu;

    if((Compression & Encryption) != DEFAULT_DATA_FORMAT_IDENTIFIER)                                                                                                   /* PRQA S 3344 */
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }
    else
    {
        RetValue = ImageM_InspectRequestImageImpl(MemoryAddress, MemorySize, &Action, &ErrCode);
        if(RetValue == E_OK)
        {
            pImgContext = ImageM_GetImgContextInstance();
            if(ImageM_InitImageBlock(pImgContext, Action->FaId, Action->Offset, Action->Len) == E_OK)
            {
                /* Set block len for generic download */
                *BlockLength = IMAGEM_CHUNK_DATA_SIZE + 2;          /* PRQA S 1840 */                                                                               /* PRQA S 4491, 1860 */
                *ErrorCode = E_OK;
            }
        }
    }
    return RetValue;
}
