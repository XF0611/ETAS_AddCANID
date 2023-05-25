/* *****************************************************************************
 * BEGIN: Banner
 *-----------------------------------------------------------------------------
 *                                 ETAS GmbH
 *                      D-70469 Stuttgart, Borsigstr. 14
 *-----------------------------------------------------------------------------
 *    Administrative Information (automatically filled in by ISOLAR)         
 *-----------------------------------------------------------------------------
 * Name: 
 * Description:
 * Version: 1.0
 *-----------------------------------------------------------------------------
 * END: Banner
 ******************************************************************************

 * Project : ETAS_BIP_PT
 * Component: /ETAS_BIP/CalUT
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.1
 * Author: TAN9SGH
 * Date : Mon Aug 23 16:59:44 2021
 ****************************************************************************/

#include "Rte_CalUT.h"

/*PROTECTED REGION ID(FileHeaderUserDefinedIncludes :RE_CalUT) ENABLED START */
/* Start of user defined includes  - Do not remove this comment */
/* End of user defined includes - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedConstants :RE_CalUT) ENABLED START */
/* Start of user defined constant definitions - Do not remove this comment */
/* End of user defined constant definitions - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedVariables :RE_CalUT) ENABLED START */
/* Start of user variable defintions - Do not remove this comment  */
#define CalUT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CalUT_MemMap.h"
volatile sint8 curveAxisInputVar;
volatile uint32 mapAxisXInputVar;
volatile sint32 mapAxisYInputVar;
#define CalUT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CalUT_MemMap.h"
/* End of user variable defintions - Do not remove this comment  */
/*PROTECTED REGION END */
#define CalUT_START_SEC_CODE                   
#include "CalUT_MemMap.h"
FUNC (void, CalUT_CODE) RE_CalUT/* return value & FctID */
(
		void
)
{

	uint32* pSpeedMin;
	uint32* pSpeedMax;
	uint32* Pim_ArTypedPIM_1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :RE_CalUT) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	curveAxisInputVar++;
	mapAxisXInputVar = Rte_Prm_RP_SpeedLimit_speedMin();
	mapAxisYInputVar = Rte_Prm_RP_SpeedLimit_speedMax();
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */

	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	IDT_CURVE* pSharedCurve = (IDT_CURVE*)Rte_CData_sharedCURVE();
	IDT_MAP* pSharedMap = (IDT_MAP*)Rte_CData_sharedMAP();

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */


	/*PROTECTED REGION ID(User Logic :RE_CalUT) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}

#define CalUT_STOP_SEC_CODE                       
#include "CalUT_MemMap.h"
