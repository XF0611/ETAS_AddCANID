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
 * Component: /ETAS_BIP/OUTP_SWC
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.1
 * Author: TAN9SGH
 * Date : Mon Aug 23 15:36:07 2021
 ****************************************************************************/

#include "Rte_OUTP_SWC.h"

/*PROTECTED REGION ID(FileHeaderUserDefinedIncludes :RE_OUTP_SWC) ENABLED START */
/* Start of user defined includes  - Do not remove this comment */
/* End of user defined includes - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedConstants :RE_OUTP_SWC) ENABLED START */
/* Start of user defined constant definitions - Do not remove this comment */
/* End of user defined constant definitions - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedVariables :RE_OUTP_SWC) ENABLED START */
/* Start of user variable defintions - Do not remove this comment  */
#define OUTP_SWC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "OUTP_SWC_MemMap.h"
boolean iWrite1;
boolean iWriteRef1;
boolean *retIWriteRef1;
boolean iWrite2;
boolean iWriteRef2;
boolean *retIWriteRef2;
uint16 iWrite3;
uint16 iWriteRef3;
uint16 *retIWriteRef3;
uint16 iWrite4;
uint16 iWriteRef4;
uint16 *retIWriteRef4;
uint8 iWrite5;
uint8 iWriteRef5;
uint8 *retIWriteRef5;
uint8 iWrite6;
uint8 iWriteRef6;
uint8 *retIWriteRef6;
uint8 iWrite7;
uint8 iWriteRef7;
uint8 *retIWriteRef7;
boolean iWrite8;
boolean iWriteRef8;
boolean *retIWriteRef8;
boolean iWrite9;
boolean iWriteRef9;
boolean *retIWriteRef9;
uint8 iWrite10;
uint8 iWriteRef10;
uint8 *retIWriteRef10;
uint8 write11;
Std_ReturnType retWrite11;
uint16 write12;
Std_ReturnType retWrite12;
uint16 write13;
Std_ReturnType retWrite13;
uint16 write14;
Std_ReturnType retWrite14;
uint8 write15;
Std_ReturnType retWrite15;
uint16 write16;
Std_ReturnType retWrite16;
uint8 write17;
Std_ReturnType retWrite17;
uint8 write18;
Std_ReturnType retWrite18;
uint8 write19;
Std_ReturnType retWrite19;
uint8 write20;
Std_ReturnType retWrite20;
uint8 write21;
Std_ReturnType retWrite21;
uint8 write22;
Std_ReturnType retWrite22;
uint8 write23;
Std_ReturnType retWrite23;
uint8 write24;
Std_ReturnType retWrite24;
#define OUTP_SWC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "OUTP_SWC_MemMap.h"
/* End of user variable defintions - Do not remove this comment  */
/*PROTECTED REGION END */
#define OUTP_SWC_START_SEC_CODE                   
#include "OUTP_SWC_MemMap.h"
FUNC (void, OUTP_SWC_CODE) RE_OUTP_SWC/* return value & FctID */
(
		void
)
{
	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :RE_OUTP_SWC) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig10_ComTx_0x55_ETAS03_Sig10_ComTx_0x55(iWrite1);

	retIWriteRef1 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig10_ComTx_0x55_ETAS03_Sig10_ComTx_0x55();

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig18_ComTx_0x55_ETAS03_Sig18_ComTx_0x55(iWrite2);

	retIWriteRef2 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig18_ComTx_0x55_ETAS03_Sig18_ComTx_0x55();

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig13_ComTx_0x55_ETAS03_Sig13_ComTx_0x55(iWrite3);

	retIWriteRef3 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig13_ComTx_0x55_ETAS03_Sig13_ComTx_0x55();

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig15_ComTx_0x55_ETAS03_Sig15_ComTx_0x55(iWrite4);

	retIWriteRef4 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig15_ComTx_0x55_ETAS03_Sig15_ComTx_0x55();

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig19_ComTx_0x55_ETAS03_Sig19_ComTx_0x55(iWrite5);

	retIWriteRef5 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig19_ComTx_0x55_ETAS03_Sig19_ComTx_0x55();

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig12_ComTx_0x55_ETAS03_Sig12_ComTx_0x55(iWrite6);

	retIWriteRef6 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig12_ComTx_0x55_ETAS03_Sig12_ComTx_0x55();

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig17_ComTx_0x55_ETAS03_Sig17_ComTx_0x55(iWrite7);

	retIWriteRef7 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig17_ComTx_0x55_ETAS03_Sig17_ComTx_0x55();

	iWrite8 = (iWrite8 == TRUE) ? FALSE : TRUE;

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig14_ComTx_0x55_ETAS03_Sig14_ComTx_0x55(iWrite8);

	retIWriteRef8 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig14_ComTx_0x55_ETAS03_Sig14_ComTx_0x55();

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig16_ComTx_0x55_ETAS03_Sig16_ComTx_0x55(iWrite9);

	retIWriteRef9 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig16_ComTx_0x55_ETAS03_Sig16_ComTx_0x55();

	Rte_IWrite_RE_OUTP_SWC_PP_ETAS03_Sig11_ComTx_0x55_ETAS03_Sig11_ComTx_0x55(iWrite10);

	retIWriteRef10 = Rte_IWriteRef_RE_OUTP_SWC_PP_ETAS03_Sig11_ComTx_0x55_ETAS03_Sig11_ComTx_0x55();
	retWrite11 = Rte_Write_PP_ETAS04_Sig21_ComTx_0x98_ETAS04_Sig21_ComTx_0x98(write11);
	retWrite12 = Rte_Write_PP_ETAS04_Sig22_ComTx_0x98_ETAS04_Sig22_ComTx_0x98(write12);
	retWrite13 = Rte_Write_PP_ETAS04_Sig25_ComTx_0x98_ETAS04_Sig25_ComTx_0x98(write13);
	retWrite14 = Rte_Write_PP_ETAS04_Sig20_ComTx_0x98_ETAS04_Sig20_ComTx_0x98(write14);
	retWrite15 = Rte_Write_PP_ETAS04_Sig24_ComTx_0x98_ETAS04_Sig24_ComTx_0x98(write15);
	retWrite16 = Rte_Write_PP_ETAS04_Sig23_ComTx_0x98_ETAS04_Sig23_ComTx_0x98(write16);

	write17 = 9;
	retWrite17 = Rte_Write_PP_ETAS05_Sig26_ComTx_0x123_ETAS05_Sig26_ComTx_0x123(write17);
	write18 = 8;
	retWrite18 = Rte_Write_PP_ETAS05_Sig27_ComTx_0x123_ETAS05_Sig27_ComTx_0x123(write18);
	write19 = 7;
	retWrite19 = Rte_Write_PP_ETAS05_Sig28_ComTx_0x123_ETAS05_Sig28_ComTx_0x123(write19);
	write20 = 6;
	retWrite20 = Rte_Write_PP_ETAS05_Sig29_ComTx_0x123_ETAS05_Sig29_ComTx_0x123(write20);
	write21 = 5;
	retWrite21 = Rte_Write_PP_ETAS05_Sig30_ComTx_0x123_ETAS05_Sig30_ComTx_0x123(write21);
	write22 = 4;
	retWrite22 = Rte_Write_PP_ETAS05_Sig31_ComTx_0x123_ETAS05_Sig31_ComTx_0x123(write22);
	write23 = 3;
	retWrite23 = Rte_Write_PP_ETAS05_Sig32_ComTx_0x123_ETAS05_Sig32_ComTx_0x123(write23);
	write24 = 2;
	retWrite24 = Rte_Write_PP_ETAS05_Sig33_ComTx_0x123_ETAS05_Sig33_ComTx_0x123(write24);

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :RE_OUTP_SWC) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}

#define OUTP_SWC_STOP_SEC_CODE                       
#include "OUTP_SWC_MemMap.h"
