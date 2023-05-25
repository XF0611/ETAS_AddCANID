/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_SwapImage.h
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

#ifndef  IMAGEM_SWAPSTATUS_H
#define  IMAGEM_SWAPSTATUS_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "ImageM_FlashPort.h"

/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/
typedef uint32  ImageM_ReprogrammingRequestFlagType;
typedef uint32  ImageM_ApplicationValidFlagType;
typedef uint32  ImageM_CalibrationValidFlagType;
typedef uint32  ImageM_ValidateFlagType;
typedef uint32  ImageM_PBLValidFlagType;

/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/
Std_ReturnType ImageM_ImageStatus_SynchWriteImageUpdateState(uint32 ImageIdx, const uint8 *data);
Std_ReturnType ImageM_ImageStatus_ReadImageUpdateState(uint32 ImageIdx, uint8 *data); 
Std_ReturnType ImageM_ImageStatus_ReadApplicationValidFlag( uint8 *data);
Std_ReturnType ImageM_ImageStatus_SynchWriteApplicationValidFlag(uint8 isValib , uint8 ImageId);


#endif /*IMAGEM_SWAPSTATUS_H*/
