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
 * Component: /ETAS_BIP/INP_SWC
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.1
 * Author: TAN9SGH
 * Date : Mon Aug 23 15:35:25 2021
 ****************************************************************************/

#include "Rte_INP_SWC.h"

/*PROTECTED REGION ID(FileHeaderUserDefinedIncludes :RE_INP_SWC) ENABLED START */
/* Start of user defined includes  - Do not remove this comment */
/* End of user defined includes - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedConstants :RE_INP_SWC) ENABLED START */
/* Start of user defined constant definitions - Do not remove this comment */
/* End of user defined constant definitions - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedVariables :RE_INP_SWC) ENABLED START */
/* Start of user variable defintions - Do not remove this comment  */
#define INP_SWC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "INP_SWC_MemMap.h"
uint8 read1;
Std_ReturnType retRead1;
uint16 read2;
Std_ReturnType retRead2;
uint16 read3;
Std_ReturnType retRead3;
boolean read4;
Std_ReturnType retRead4;
boolean read5;
Std_ReturnType retRead5;
uint8 read6;
Std_ReturnType retRead6;
boolean read7;
Std_ReturnType retRead7;
uint8 read8;
Std_ReturnType retRead8;
uint8 read9;
Std_ReturnType retRead9;
boolean read10;
Std_ReturnType retRead10;
uint8 read11;
Std_ReturnType retRead11;
uint8 read12;
Std_ReturnType retRead12;
uint8 read13;
Std_ReturnType retRead13;
uint8 read14;
Std_ReturnType retRead14;
uint8 read15;
Std_ReturnType retRead15;
uint8 read16;
Std_ReturnType retRead16;
uint8 read17;
Std_ReturnType retRead17;
uint8 read18;
Std_ReturnType retRead18;
#define INP_SWC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "INP_SWC_MemMap.h"
/* End of user variable defintions - Do not remove this comment  */
/*PROTECTED REGION END */
#define INP_SWC_START_SEC_CODE                   
#include "INP_SWC_MemMap.h"
FUNC (void, INP_SWC_CODE) RE_INP_SWC/* return value & FctID */
(
		void
)
{
	uint8 test[100];
	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :RE_INP_SWC) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */
	retRead1 = Rte_Read_RP_ETAS01_Sig03_ComRx_0x81_ETAS01_Sig03_ComRx_0x81(&read1);
	retRead2 = Rte_Read_RP_ETAS01_Sig07_ComRx_0x81_ETAS01_Sig07_ComRx_0x81(&read2);
	retRead3 = Rte_Read_RP_ETAS01_Sig08_ComRx_0x81_ETAS01_Sig08_ComRx_0x81(&read3);
	retRead4 = Rte_Read_RP_ETAS01_Sig05_ComRx_0x81_ETAS01_Sig05_ComRx_0x81(&read4);
	retRead5 = Rte_Read_RP_ETAS01_Sig04_ComRx_0x81_ETAS01_Sig04_ComRx_0x81(&read5);
	retRead6 = Rte_Read_RP_ETAS01_Sig00_ComRx_0x81_ETAS01_Sig00_ComRx_0x81(&read6);
	retRead7 = Rte_Read_RP_ETAS01_Sig01_ComRx_0x81_ETAS01_Sig01_ComRx_0x81(&read7);
	retRead8 = Rte_Read_RP_ETAS01_Sig06_ComRx_0x81_ETAS01_Sig06_ComRx_0x81(&read8);
	retRead9 = Rte_Read_RP_ETAS01_Sig02_ComRx_0x81_ETAS01_Sig02_ComRx_0x81(&read9);
	retRead10 = Rte_Read_RP_ETAS02_Sig09_ComRx_0x261_ETAS02_Sig09_ComRx_0x261(&read10);
	retRead11 = Rte_Read_RP_ETAS06_Sig34_ComRx_0x456_ETAS06_Sig34_ComRx_0x456(&read11);
	retRead12 = Rte_Read_RP_ETAS06_Sig35_ComRx_0x456_ETAS06_Sig35_ComRx_0x456(&read12);
	retRead13 = Rte_Read_RP_ETAS06_Sig36_ComRx_0x456_ETAS06_Sig36_ComRx_0x456(&read13);
	retRead14 = Rte_Read_RP_ETAS06_Sig37_ComRx_0x456_ETAS06_Sig37_ComRx_0x456(&read14);
	retRead15 = Rte_Read_RP_ETAS06_Sig38_ComRx_0x456_ETAS06_Sig38_ComRx_0x456(&read15);
	retRead16 = Rte_Read_RP_ETAS06_Sig39_ComRx_0x456_ETAS06_Sig39_ComRx_0x456(&read16);
	retRead17 = Rte_Read_RP_ETAS06_Sig40_ComRx_0x456_ETAS06_Sig40_ComRx_0x456(&read17);
	retRead18 = Rte_Read_RP_ETAS06_Sig41_ComRx_0x456_ETAS06_Sig41_ComRx_0x456(&read18);

	sprintf(test,"read11 is %d.\r\n",read11);
	Uart_Transmit(0,test,strlen(test));

	sprintf(test,"read12 is %d.\r\n",read12);
	Uart_Transmit(0,test,strlen(test));


	sprintf(test,"read13 is %d.\r\n",read13);
	Uart_Transmit(0,test,strlen(test));

	sprintf(test,"read14 is %d.\r\n",read14);
	Uart_Transmit(0,test,strlen(test));

	sprintf(test,"read15 is %d.\r\n",read15);
	Uart_Transmit(0,test,strlen(test));

	sprintf(test,"read16 is %d.\r\n",read16);
	Uart_Transmit(0,test,strlen(test));

	sprintf(test,"read17 is %d.\r\n",read17);
	Uart_Transmit(0,test,strlen(test));

	sprintf(test,"read18 is %d.\r\n",read18);
	Uart_Transmit(0,test,strlen(test));



	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :RE_INP_SWC) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}

#define INP_SWC_STOP_SEC_CODE                       
#include "INP_SWC_MemMap.h"
