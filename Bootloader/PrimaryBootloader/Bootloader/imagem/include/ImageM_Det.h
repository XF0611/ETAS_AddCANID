/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_Det.h
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

#ifndef  IMAGEM_DET_H
#define  IMAGEM_DET_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/

#include "Std_Types.h"
#include "Det.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
#define IMAGEM_DEV_ERROR_DETECT     STD_ON
/* Check whether given condition is satisfied, call Det_ReportError() */
#if IMAGEM_DEV_ERROR_DETECT != STD_OFF
     /*Macro For functions with return type as 'void'*/
    #define IMAGEM_REPORT_ERROR(CONDITION, API, ERROR)                                                       \
                                   if(CONDITION)                                                            \
                                   {                                                                        \
                                        (void)Det_ReportError((IMAGEM_DEV_ERROR_MODULE_ID), (0u), (API), (ERROR));     \
                                        return;                                                             \
                                   }
     /*Macro For functions with return type as 'Std_ReturnType'*/
    #define IMAGEM_REPORT_ERROR_NOK(CONDITION, API, ERROR)                                                   \
                                   if(CONDITION)                                                            \
                                   {                                                                        \
                                        (void)Det_ReportError((IMAGEM_DEV_ERROR_MODULE_ID), (0u), (API), (ERROR));     \
                                        return E_NOT_OK;                                                    \
                                   }

  /*Macro For functions with return type as 'void'*/
    #define IMAGEM_REPORT_ERROR_NO_CONDITON(ERROR, API)                                                       \
                                   {                                                                        \
                                        (void)Det_ReportError((IMAGEM_DEV_ERROR_MODULE_ID), (0u), (API), (ERROR));     \
                                        return;                                                             \
                                   }
#else
    #define IMAGEM_REPORT_ERROR(CONDITION, ERROR)

    #define IMAGEM_REPORT_ERROR_NOK(CONDITION, ERROR)
    
    #define IMAGEM_REPORT_ERROR_NO_CONDITON(API, ERROR)
#endif


/*If the Position is incorrect */
#define IMAGEM_E_RET               E_NOT_OK
#define IMAGEM_E_PARAM_ADDRESS     0x03u
#define IMAGEM_E_INVALID_LENGTH    0x04u
#define IMAGEM_E_INVALID_ALIGN     0x05u
#define IMAGEM_E_INVALID_MEM       0x06u
#define IMAGEM_E_INVALID_ID        0x07u
#define IMAGEM_E_UNKNOWN           0x08u
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

#endif /*IMAGEM_DET_H*/
