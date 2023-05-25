

/*---------------------------------------------------------------------------------------*/
/*- Include files																		-*/
/*---------------------------------------------------------------------------------------*/
#include "BswM.h"
#include "BswM_WdgM.h"
#include "BswM_Prv.h"


/*---------------------------------------------------------------------------------------*/
/*- Definition of Global Functions														-*/
/*---------------------------------------------------------------------------------------*/

/*****************************************************************************************
* Function name	:
* Description	:
* Parameter  	:
* Return value	:
* Remarks		:
*****************************************************************************************/
/* BSW-15413 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/* END BSW-15413 */
void BswM_WdgM_RequestPartitionReset
(
   ApplicationType Application
)
{
    /*To avoid compiler warning of unused parameters*/
    (void)Application;
	return;
}
/* BSW-15413 */
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/* END BSW-15413 */
/**********************************************************************************************************************
 * End of source file
 **********************************************************************************************************************/

