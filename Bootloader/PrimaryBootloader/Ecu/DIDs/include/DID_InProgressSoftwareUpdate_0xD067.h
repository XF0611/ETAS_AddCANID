/*********************************************************************************************************************
*
* Description:     
* FileName:        DID_InProgressSoftwareUpdate_0xD067.h
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
 * @brief 
 * 
 * @param OpStatus 
 * @param Data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DID_InProgressSwUpdate_0xD067_ReadFunc (Dcm_OpStatusType OpStatus, uint8 *Data);

/**
 * @brief 
 * 
 * @param Address 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DID_InProgressSwUpdate_0xD067_WriteStartAddrBlock(uint32 Address);

