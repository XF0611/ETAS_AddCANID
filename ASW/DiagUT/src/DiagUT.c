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
 * Component: /Composition/DiagUT/SwComponentTypes/DiagUT
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.2.1
 * Author: ZCO7SGH
 * Date : Mon Jun 20 16:10:34 2022
 ****************************************************************************/

#include "Rte_DiagUT.h"

#define DiagUT_START_SEC_VAR_INIT_8                   
#include "DiagUT_MemMap.h"
uint8 DEM_OperationCycle_Start = 1;
#define DiagUT_STOP_SEC_VAR_INIT_8                   
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CONST_8                   
#include "DiagUT_MemMap.h"
const uint8 Dcm_Seed[8]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
const uint8 Dcm_Key[8]={0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80};
#define DiagUT_STOP_SEC_CONST_8                   
#include "DiagUT_MemMap.h"
#define DiagUT_START_SEC_VAR_INIT_UNSPECIFIED 

Dcm_IOControlRequest_DID_0xD901_New_Data_ObjectType      Dcm_IOControlRequest_DID_0xD901_New_Data        ;
Dcm_IOControlRequest_DID_0xE900_New_Data_ObjectType      Dcm_IOControlRequest_DID_0xE900_New_Data        ;
Dcm_DataArrayTypeUint8_DID_0x0100_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0x0100_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF110_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF110_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF115_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF115_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF121_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF121_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF187_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF187_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF18B_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF18B_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF18C_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF18C_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF190_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF190_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_Object_5Type  Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_5    ;
Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF198_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF1A8_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF1A8_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF1A9_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF1A9_New_Data      ;
Dcm_DataArrayTypeUint8_DID_0xF1AA_New_Data_ObjectType    Dcm_DataArrayTypeUint8_DID_0xF1AA_New_Data      ;

#include "DiagUT_MemMap.h"
#define DiagUT_STOP_SEC_VAR_INIT_UNSPECIFIED                   
#include "DiagUT_MemMap.h"
/*PROTECTED REGION ID(FileHeaderUserDefinedIncludes :CBClrEvt_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_ClearEventAllowed) ENABLED START */
/* Start of user defined includes  - Do not remove this comment */
/* End of user defined includes - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedConstants :CBClrEvt_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_ClearEventAllowed) ENABLED START */
/* Start of user defined constant definitions - Do not remove this comment */
/* End of user defined constant definitions - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedVariables :CBClrEvt_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_ClearEventAllowed) ENABLED START */
/* Start of user variable defintions - Do not remove this comment  */
/* End of user variable defintions - Do not remove this comment  */
/*PROTECTED REGION END */
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xc07388_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xc07388_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xc07388_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xc12687_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xc12687_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xc12687_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xc14587_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xc14587_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xc14587_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xc22a87_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xc22a87_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xc22a87_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xc41487_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xc41487_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xc41487_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xd56217_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xd56217_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xd56217_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xd56316_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xd56316_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xd56316_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xe00141_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xe00141_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xe00141_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xe00142_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xe00142_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xe00142_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) CBClrEvt_EventParameter_DTC_0xe26200_Event_ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :CBClrEvt_EventParameter_DTC_0xe26200_Event_ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :CBClrEvt_EventParameter_DTC_0xe26200_Event_ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) ClearEventAllowed/* return value & FctID */
(
		P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) Allowed
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :ClearEventAllowed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :ClearEventAllowed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) ClearPrestoredFreezeFrame_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Std_ReturnType syncCall1;
	Std_ReturnType syncCall2;
	Std_ReturnType syncCall3;
	Std_ReturnType syncCall4;
	Std_ReturnType syncCall5;
	Std_ReturnType syncCall6;
	Std_ReturnType syncCall7;
	Std_ReturnType syncCall8;
	Std_ReturnType syncCall9;
	Std_ReturnType syncCall10;
	Std_ReturnType syncCall11;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :ClearPrestoredFreezeFrame_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh te_Call_Event_EventParameter_DTC_0x000001_Event_ClearPrestoredFreezeFrame();
                
	syncCall2 = E_OK; //tan9sgh te_Call_Event_EventParameter_DTC_0xc07388_Event_ClearPrestoredFreezeFrame();
                
	syncCall3 = E_OK; //tan9sgh te_Call_Event_EventParameter_DTC_0xe26200_Event_ClearPrestoredFreezeFrame();
                
	syncCall4 = E_OK; //tan9sgh te_Call_Event_EventParameter_DTC_0xc12687_Event_ClearPrestoredFreezeFrame();
                
	syncCall5 = E_OK; //tan9sgh te_Call_Event_EventParameter_DTC_0xc14587_Event_ClearPrestoredFreezeFrame();
                
	syncCall6 = E_OK; //tan9sgh te_Call_Event_EventParameter_DTC_0xc22a87_Event_ClearPrestoredFreezeFrame();
                
	syncCall7 = E_OK; //tan9sgh te_Call_Event_EventParameter_DTC_0xc41487_Event_ClearPrestoredFreezeFrame();
                
	syncCall8 = E_OK; //tan9sgh te_Call_Event_EventParameter_DTC_0xd56217_Event_ClearPrestoredFreezeFrame();
                
	syncCall9 = E_OK; //tan9sgh te_Call_Event_EventParameter_DTC_0xd56316_Event_ClearPrestoredFreezeFrame();
                
	syncCall10 = E_OK; //tan9sghRte_Call_Event_EventParameter_DTC_0xe00141_Event_ClearPrestoredFreezeFrame();
                
	syncCall11 = E_OK; //tan9sghRte_Call_Event_EventParameter_DTC_0xe00142_Event_ClearPrestoredFreezeFrame();
                
	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :ClearPrestoredFreezeFrame_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dcm_GetActiveProtocol_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dcm_ProtocolType ActiveProtocol1;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dcm_GetActiveProtocol_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_DCMServices_GetActiveProtocol(&ActiveProtocol1);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dcm_GetActiveProtocol_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dcm_GetSecurityLevel_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dcm_SecLevelType SecLevel2;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dcm_GetSecurityLevel_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_DCMServices_GetSecurityLevel(&SecLevel2);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dcm_GetSecurityLevel_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dcm_GetSesCtrlType_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dcm_SesCtrlType SesCtrlType3;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dcm_GetSesCtrlType_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_DCMServices_GetSesCtrlType(&SesCtrlType3);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dcm_GetSesCtrlType_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dcm_MainFunction_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dcm_IOControlRequest_DID_0xD901_New_Data_ObjectType read1;
	Std_ReturnType retRead1;
	Dcm_IOControlRequest_DID_0xE900_New_Data_ObjectType read2;
	Std_ReturnType retRead2;
	Dcm_DataArrayTypeUint8_DID_0x0100_New_Data_ObjectType read3;
	Std_ReturnType retRead3;
	Dcm_DataArrayTypeUint8_DID_0xF110_New_Data_ObjectType read4;
	Std_ReturnType retRead4;
	Dcm_DataArrayTypeUint8_DID_0xF115_New_Data_ObjectType read5;
	Std_ReturnType retRead5;
	Dcm_DataArrayTypeUint8_DID_0xF121_New_Data_ObjectType read6;
	Std_ReturnType retRead6;
	Dcm_DataArrayTypeUint8_DID_0xF187_New_Data_ObjectType read7;
	Std_ReturnType retRead7;
	Dcm_DataArrayTypeUint8_DID_0xF18B_New_Data_ObjectType read8;
	Std_ReturnType retRead8;
	Dcm_DataArrayTypeUint8_DID_0xF18C_New_Data_ObjectType read9;
	Std_ReturnType retRead9;
	Dcm_DataArrayTypeUint8_DID_0xF190_New_Data_ObjectType read10;
	Std_ReturnType retRead10;
	Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_Object_5Type read11;
	Std_ReturnType retRead11;
	Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_ObjectType read12;
	Std_ReturnType retRead12;
	Dcm_DataArrayTypeUint8_DID_0xF1A8_New_Data_ObjectType read13;
	Std_ReturnType retRead13;
	Dcm_DataArrayTypeUint8_DID_0xF1A9_New_Data_ObjectType read14;
	Std_ReturnType retRead14;
	Dcm_DataArrayTypeUint8_DID_0xF1AA_New_Data_ObjectType read15;
	Std_ReturnType retRead15;
	Dcm_IOOperationResponseType write16;
	Std_ReturnType retWrite16;
	Dcm_IOOperationResponseType write17;
	Std_ReturnType retWrite17;
	Dcm_DataArrayTypeUint8_DID_0x0100_New_Data_ObjectType write18;
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
	uint8 write25;
	Std_ReturnType retWrite25;
	Dcm_DataArrayTypeUint8_DID_0xB950_New_Data_ObjectType write26;
	Std_ReturnType retWrite26;
	uint8 write27;
	Std_ReturnType retWrite27;
	uint8 write28;
	Std_ReturnType retWrite28;
	Dcm_DataArrayTypeUint8_DID_0xE101_New_Data_ObjectType write29;
	Std_ReturnType retWrite29;
	uint8 write30;
	Std_ReturnType retWrite30;
	Dcm_DataArrayTypeUint8_DID_0xF100_New_Data_Object_1Type write31;
	Std_ReturnType retWrite31;
	Dcm_DataArrayTypeUint8_DID_0xF100_New_Data_ObjectType write32;
	Std_ReturnType retWrite32;
	Dcm_DataArrayTypeUint8_DID_0xF110_New_Data_ObjectType write33;
	Std_ReturnType retWrite33;
	Dcm_DataArrayTypeUint8_DID_0xF111_New_Data_ObjectType write34;
	Std_ReturnType retWrite34;
	Dcm_DataArrayTypeUint8_DID_0xF112_New_Data_ObjectType write35;
	Std_ReturnType retWrite35;
	Dcm_DataArrayTypeUint8_DID_0xF113_New_Data_ObjectType write36;
	Std_ReturnType retWrite36;
	Dcm_DataArrayTypeUint8_DID_0xF114_New_Data_ObjectType write37;
	Std_ReturnType retWrite37;
	Dcm_DataArrayTypeUint8_DID_0xF115_New_Data_ObjectType write38;
	Std_ReturnType retWrite38;
	Dcm_DataArrayTypeUint8_DID_0xF116_New_Data_ObjectType write39;
	Std_ReturnType retWrite39;
	Dcm_DataArrayTypeUint8_DID_0xF117_New_Data_ObjectType write40;
	Std_ReturnType retWrite40;
	Dcm_DataArrayTypeUint8_DID_0xF118_New_Data_ObjectType write41;
	Std_ReturnType retWrite41;
	Dcm_DataArrayTypeUint8_DID_0xF119_New_Data_ObjectType write42;
	Std_ReturnType retWrite42;
	Dcm_DataArrayTypeUint8_DID_0xF11A_New_Data_ObjectType write43;
	Std_ReturnType retWrite43;
	Dcm_DataArrayTypeUint8_DID_0xF11B_New_Data_ObjectType write44;
	Std_ReturnType retWrite44;
	Dcm_DataArrayTypeUint8_DID_0xF11C_New_Data_ObjectType write45;
	Std_ReturnType retWrite45;
	Dcm_DataArrayTypeUint8_DID_0xF11D_New_Data_ObjectType write46;
	Std_ReturnType retWrite46;
	Dcm_DataArrayTypeUint8_DID_0xF11E_New_Data_ObjectType write47;
	Std_ReturnType retWrite47;
	Dcm_DataArrayTypeUint8_DID_0xF11F_New_Data_ObjectType write48;
	Std_ReturnType retWrite48;
	Dcm_DataArrayTypeUint8_DID_0xF120_New_Data_ObjectType write49;
	Std_ReturnType retWrite49;
	Dcm_DataArrayTypeUint8_DID_0xF121_New_Data_ObjectType write50;
	Std_ReturnType retWrite50;
	Dcm_DataArrayTypeUint8_DID_0xF122_New_Data_Object_2Type write51;
	Std_ReturnType retWrite51;
	Dcm_DataArrayTypeUint8_DID_0xF122_New_Data_ObjectType write52;
	Std_ReturnType retWrite52;
	Dcm_DataArrayTypeUint8_DID_0xF187_New_Data_ObjectType write53;
	Std_ReturnType retWrite53;
	Dcm_DataArrayTypeUint8_DID_0xF18A_New_Data_ObjectType write54;
	Std_ReturnType retWrite54;
	Dcm_DataArrayTypeUint8_DID_0xF18B_New_Data_ObjectType write55;
	Std_ReturnType retWrite55;
	Dcm_DataArrayTypeUint8_DID_0xF18C_New_Data_ObjectType write56;
	Std_ReturnType retWrite56;
	Dcm_DataArrayTypeUint8_DID_0xF190_New_Data_ObjectType write57;
	Std_ReturnType retWrite57;
	Dcm_DataArrayTypeUint8_DID_0xF192_New_Data_Object_3Type write58;
	Std_ReturnType retWrite58;
	Dcm_DataArrayTypeUint8_DID_0xF192_New_Data_ObjectType write59;
	Std_ReturnType retWrite59;
	Dcm_DataArrayTypeUint8_DID_0xF194_New_Data_Object_4Type write60;
	Std_ReturnType retWrite60;
	Dcm_DataArrayTypeUint8_DID_0xF194_New_Data_ObjectType write61;
	Std_ReturnType retWrite61;
	Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_Object_5Type write62;
	Std_ReturnType retWrite62;
	Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_ObjectType write63;
	Std_ReturnType retWrite63;
	Dcm_DataArrayTypeUint8_DID_0xF1A0_New_Data_ObjectType write64;
	Std_ReturnType retWrite64;
	Dcm_DataArrayTypeUint8_DID_0xF1A1_New_Data_ObjectType write65;
	Std_ReturnType retWrite65;
	Dcm_DataArrayTypeUint8_DID_0xF1A2_New_Data_ObjectType write66;
	Std_ReturnType retWrite66;
	Dcm_DataArrayTypeUint8_DID_0xF1A5_New_Data_ObjectType write67;
	Std_ReturnType retWrite67;
	Dcm_DataArrayTypeUint8_DID_0xF1A8_New_Data_ObjectType write68;
	Std_ReturnType retWrite68;
	Dcm_DataArrayTypeUint8_DID_0xF1A9_New_Data_ObjectType write69;
	Std_ReturnType retWrite69;
	Dcm_DataArrayTypeUint8_DID_0xF1AA_New_Data_ObjectType write70;
	Std_ReturnType retWrite70;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dcm_MainFunction_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */
	retRead1 = Rte_Read_IOControlRequest_DID_0xD901_New_Data_Object_IOControlRequest_DID_0xD901_New_Data_Object(&read1);
	retRead2 = Rte_Read_IOControlRequest_DID_0xE900_New_Data_Object_IOControlRequest_DID_0xE900_New_Data_Object(&read2);
	retRead3 = Rte_Read_R_DataServices_DID_0x0100_New_Data_Object_Data((uint8 *)&read3);
	retRead4 = Rte_Read_R_DataServices_DID_0xF110_New_Data_Object_Data((uint8 *)&read4);
	retRead5 = Rte_Read_R_DataServices_DID_0xF115_New_Data_Object_Data((uint8 *)&read5);
	retRead6 = Rte_Read_R_DataServices_DID_0xF121_New_Data_Object_Data((uint8 *)&read6);
	retRead7 = Rte_Read_R_DataServices_DID_0xF187_New_Data_Object_Data((uint8 *)&read7);
	retRead8 = Rte_Read_R_DataServices_DID_0xF18B_New_Data_Object_Data((uint8 *)&read8);
	retRead9 = Rte_Read_R_DataServices_DID_0xF18C_New_Data_Object_Data((uint8 *)&read9);
	retRead10 = Rte_Read_R_DataServices_DID_0xF190_New_Data_Object_Data((uint8 *)&read10);
	retRead11 = Rte_Read_R_DataServices_DID_0xF198_New_Data_Object_5_Data((uint8 *)&read11);
	retRead12 = Rte_Read_R_DataServices_DID_0xF198_New_Data_Object_Data((uint8 *)&read12);
	retRead13 = Rte_Read_R_DataServices_DID_0xF1A8_New_Data_Object_Data((uint8 *)&read13);
	retRead14 = Rte_Read_R_DataServices_DID_0xF1A9_New_Data_Object_Data((uint8 *)&read14);
	retRead15 = Rte_Read_R_DataServices_DID_0xF1AA_New_Data_Object_Data((uint8 *)&read15);

	Rte_memcpy( &Dcm_IOControlRequest_DID_0xD901_New_Data                      , &read1  ,sizeof( Dcm_IOControlRequest_DID_0xD901_New_Data_ObjectType             )) ;
	Rte_memcpy( &Dcm_IOControlRequest_DID_0xE900_New_Data                      , &read2  ,sizeof( Dcm_IOControlRequest_DID_0xE900_New_Data_ObjectType             )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0x0100_New_Data                    , &read3  ,sizeof( Dcm_DataArrayTypeUint8_DID_0x0100_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF110_New_Data                    , &read4  ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF110_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF115_New_Data                    , &read5  ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF115_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF121_New_Data                    , &read6  ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF121_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF187_New_Data                    , &read7  ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF187_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF18B_New_Data                    , &read8  ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF18B_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF18C_New_Data                    , &read9  ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF18C_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF190_New_Data                    , &read10 ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF190_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_5                  , &read11 ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_Object_5Type         )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF198_New_Data                    , &read12 ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF1A8_New_Data                    , &read13 ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF1A8_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF1A9_New_Data                    , &read14 ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF1A9_New_Data_ObjectType           )) ;
	Rte_memcpy( &Dcm_DataArrayTypeUint8_DID_0xF1AA_New_Data                    , &read15 ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF1AA_New_Data_ObjectType           )) ;
	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	Rte_memcpy( &write16                    , &Dcm_IOControlRequest_DID_0xD901_New_Data        ,sizeof( Dcm_IOControlRequest_DID_0xD901_New_Data_ObjectType             )) ;
	Rte_memcpy( &write17                    , &Dcm_IOControlRequest_DID_0xE900_New_Data        ,sizeof( Dcm_IOControlRequest_DID_0xE900_New_Data_ObjectType             )) ;
	Rte_memcpy( &write18                    , &Dcm_DataArrayTypeUint8_DID_0x0100_New_Data  ,sizeof( Dcm_DataArrayTypeUint8_DID_0x0100_New_Data_ObjectType           )) ;
	Rte_memcpy( &write33                    , &Dcm_DataArrayTypeUint8_DID_0xF110_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF110_New_Data_ObjectType           )) ;
	Rte_memcpy( &write38                    , &Dcm_DataArrayTypeUint8_DID_0xF115_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF115_New_Data_ObjectType           )) ;
	Rte_memcpy( &write50                    , &Dcm_DataArrayTypeUint8_DID_0xF121_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF121_New_Data_ObjectType           )) ;
	Rte_memcpy( &write53                    , &Dcm_DataArrayTypeUint8_DID_0xF187_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF187_New_Data_ObjectType           )) ;
	Rte_memcpy( &write55                    , &Dcm_DataArrayTypeUint8_DID_0xF18B_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF18B_New_Data_ObjectType           )) ;
	Rte_memcpy( &write56                    , &Dcm_DataArrayTypeUint8_DID_0xF18C_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF18C_New_Data_ObjectType           )) ;
	Rte_memcpy( &write57                    , &Dcm_DataArrayTypeUint8_DID_0xF190_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF190_New_Data_ObjectType           )) ;
	Rte_memcpy( &write62                    , &Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_5    ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_Object_5Type         )) ;
	Rte_memcpy( &write63                    , &Dcm_DataArrayTypeUint8_DID_0xF198_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF198_New_Data_ObjectType           )) ;
	Rte_memcpy( &write68                    , &Dcm_DataArrayTypeUint8_DID_0xF1A8_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF1A8_New_Data_ObjectType           )) ;
	Rte_memcpy( &write69                    , &Dcm_DataArrayTypeUint8_DID_0xF1A9_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF1A9_New_Data_ObjectType           )) ;
	Rte_memcpy( &write70                    , &Dcm_DataArrayTypeUint8_DID_0xF1AA_New_Data      ,sizeof( Dcm_DataArrayTypeUint8_DID_0xF1AA_New_Data_ObjectType           )) ;

	retWrite16 = Rte_Write_IOControlResponse_DID_0xD901_New_Data_Object_IOOperationResponse(write16);
	retWrite17 = Rte_Write_IOControlResponse_DID_0xE900_New_Data_Object_IOOperationResponse(write17);
	retWrite18 = Rte_Write_P_DataServices_DID_0x0100_New_Data_Object_Data((uint8 *)&write18);
	retWrite19 = Rte_Write_P_DataServices_DID_0x010B_Day_Data(write19);
	retWrite20 = Rte_Write_P_DataServices_DID_0x010B_Hour_Data(write20);
	retWrite21 = Rte_Write_P_DataServices_DID_0x010B_Minute_Data(write21);
	retWrite22 = Rte_Write_P_DataServices_DID_0x010B_Month_Data(write22);
	retWrite23 = Rte_Write_P_DataServices_DID_0x010B_Second_Data(write23);
	retWrite24 = Rte_Write_P_DataServices_DID_0x010B_Year_Data(write24);
	retWrite25 = Rte_Write_P_DataServices_DID_0x0112_New_Data_Object_Data(write25);
	retWrite26 = Rte_Write_P_DataServices_DID_0xB950_New_Data_Object_Data((uint8 *)&write26);
	retWrite27 = Rte_Write_P_DataServices_DID_0xD001_New_Data_Object_Data(write27);
	retWrite28 = Rte_Write_P_DataServices_DID_0xD901_New_Data_Object_Data(write28);
	retWrite29 = Rte_Write_P_DataServices_DID_0xE101_New_Data_Object_Data((uint8 *)&write29);
	retWrite30 = Rte_Write_P_DataServices_DID_0xE900_New_Data_Object_Data(write30);
	retWrite31 = Rte_Write_P_DataServices_DID_0xF100_New_Data_Object_1_Data((uint8 *)&write31);
	retWrite32 = Rte_Write_P_DataServices_DID_0xF100_New_Data_Object_Data((uint8 *)&write32);
	retWrite33 = Rte_Write_P_DataServices_DID_0xF110_New_Data_Object_Data((uint8 *)&write33);
	retWrite34 = Rte_Write_P_DataServices_DID_0xF111_New_Data_Object_Data((uint8 *)&write34);
	retWrite35 = Rte_Write_P_DataServices_DID_0xF112_New_Data_Object_Data((uint8 *)&write35);
	retWrite36 = Rte_Write_P_DataServices_DID_0xF113_New_Data_Object_Data((uint8 *)&write36);
	retWrite37 = Rte_Write_P_DataServices_DID_0xF114_New_Data_Object_Data((uint8 *)&write37);
	retWrite38 = Rte_Write_P_DataServices_DID_0xF115_New_Data_Object_Data((uint8 *)&write38);
	retWrite39 = Rte_Write_P_DataServices_DID_0xF116_New_Data_Object_Data((uint8 *)&write39);
	retWrite40 = Rte_Write_P_DataServices_DID_0xF117_New_Data_Object_Data((uint8 *)&write40);
	retWrite41 = Rte_Write_P_DataServices_DID_0xF118_New_Data_Object_Data((uint8 *)&write41);
	retWrite42 = Rte_Write_P_DataServices_DID_0xF119_New_Data_Object_Data((uint8 *)&write42);
	retWrite43 = Rte_Write_P_DataServices_DID_0xF11A_New_Data_Object_Data((uint8 *)&write43);
	retWrite44 = Rte_Write_P_DataServices_DID_0xF11B_New_Data_Object_Data((uint8 *)&write44);
	retWrite45 = Rte_Write_P_DataServices_DID_0xF11C_New_Data_Object_Data((uint8 *)&write45);
	retWrite46 = Rte_Write_P_DataServices_DID_0xF11D_New_Data_Object_Data((uint8 *)&write46);
	retWrite47 = Rte_Write_P_DataServices_DID_0xF11E_New_Data_Object_Data((uint8 *)&write47);
	retWrite48 = Rte_Write_P_DataServices_DID_0xF11F_New_Data_Object_Data((uint8 *)&write48);
	retWrite49 = Rte_Write_P_DataServices_DID_0xF120_New_Data_Object_Data((uint8 *)&write49);
	retWrite50 = Rte_Write_P_DataServices_DID_0xF121_New_Data_Object_Data((uint8 *)&write50);
	retWrite51 = Rte_Write_P_DataServices_DID_0xF122_New_Data_Object_2_Data((uint8 *)&write51);
	retWrite52 = Rte_Write_P_DataServices_DID_0xF122_New_Data_Object_Data((uint8 *)&write52);
	retWrite53 = Rte_Write_P_DataServices_DID_0xF187_New_Data_Object_Data((uint8 *)&write53);
	retWrite54 = Rte_Write_P_DataServices_DID_0xF18A_New_Data_Object_Data((uint8 *)&write54);
	retWrite55 = Rte_Write_P_DataServices_DID_0xF18B_New_Data_Object_Data((uint8 *)&write55);
	retWrite56 = Rte_Write_P_DataServices_DID_0xF18C_New_Data_Object_Data((uint8 *)&write56);
	retWrite57 = Rte_Write_P_DataServices_DID_0xF190_New_Data_Object_Data((uint8 *)&write57);
	retWrite58 = Rte_Write_P_DataServices_DID_0xF192_New_Data_Object_3_Data((uint8 *)&write58);
	retWrite59 = Rte_Write_P_DataServices_DID_0xF192_New_Data_Object_Data((uint8 *)&write59);
	retWrite60 = Rte_Write_P_DataServices_DID_0xF194_New_Data_Object_4_Data((uint8 *)&write60);
	retWrite61 = Rte_Write_P_DataServices_DID_0xF194_New_Data_Object_Data((uint8 *)&write61);
	retWrite62 = Rte_Write_P_DataServices_DID_0xF198_New_Data_Object_5_Data((uint8 *)&write62);
	retWrite63 = Rte_Write_P_DataServices_DID_0xF198_New_Data_Object_Data((uint8 *)&write63);
	retWrite64 = Rte_Write_P_DataServices_DID_0xF1A0_New_Data_Object_Data((uint8 *)&write64);
	retWrite65 = Rte_Write_P_DataServices_DID_0xF1A1_New_Data_Object_Data((uint8 *)&write65);
	retWrite66 = Rte_Write_P_DataServices_DID_0xF1A2_New_Data_Object_Data((uint8 *)&write66);
	retWrite67 = Rte_Write_P_DataServices_DID_0xF1A5_New_Data_Object_Data((uint8 *)&write67);
	retWrite68 = Rte_Write_P_DataServices_DID_0xF1A8_New_Data_Object_Data((uint8 *)&write68);
	retWrite69 = Rte_Write_P_DataServices_DID_0xF1A9_New_Data_Object_Data((uint8 *)&write69);
	retWrite70 = Rte_Write_P_DataServices_DID_0xF1AA_New_Data_Object_Data((uint8 *)&write70);

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dcm_MainFunction_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dcm_ResetToDefaultSession_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dcm_ResetToDefaultSession_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_DCMServices_ResetToDefaultSession();

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dcm_ResetToDefaultSession_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dcm_SetActiveDiagnostic_Adapt_Appl/* return value & FctID */
(
		void
)
{

	boolean active4;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dcm_SetActiveDiagnostic_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_DCMServices_SetActiveDiagnostic(active4);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dcm_SetActiveDiagnostic_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetDTCOfEvent_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId5;
	Dem_DTCFormatType DTCFormat6;
	uint32 DTCOfEvent7;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetDTCOfEvent_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_GeneralEvtInfo_GetDTCOfEvent(EventId5,DTCFormat6,&DTCOfEvent7);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetDTCOfEvent_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetEventExtendedDataRecordEx_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId8;
	uint8 RecordNumber9;
	Dem_MaxDataValueType DestBuffer10;
	uint16 Bufsize11;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetEventExtendedDataRecordEx_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_GeneralEvtInfo_GetEventExtendedDataRecordEx(EventId8,RecordNumber9,&DestBuffer10,&Bufsize11);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetEventExtendedDataRecordEx_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetEventExtendedDataRecord_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId12;
	uint8 RecordNumber13;
	Dem_MaxDataValueType DestBuffer14;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetEventExtendedDataRecord_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_GeneralEvtInfo_GetEventExtendedDataRecord(EventId12,RecordNumber13,&DestBuffer14);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetEventExtendedDataRecord_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetEventFailed_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId15;
	boolean EventFailed16;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetEventFailed_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_GeneralEvtInfo_GetEventFailed(EventId15,&EventFailed16);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetEventFailed_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetEventFreezeFrameDataEx_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId17;
	uint8 RecordNumber18;
	uint16 DataId19;
	Dem_MaxDataValueType DestBuffer20;
	uint16 BufSize21;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetEventFreezeFrameDataEx_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_GeneralEvtInfo_GetEventFreezeFrameDataEx(EventId17,RecordNumber18,DataId19,&DestBuffer20,&BufSize21);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetEventFreezeFrameDataEx_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetEventFreezeFrameData_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId22;
	uint8 RecordNumber23;
	boolean ReportTotalRecord24;
	uint16 DataId25;
	Dem_MaxDataValueType DestBuffer26;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetEventFreezeFrameData_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_GeneralEvtInfo_GetEventFreezeFrameData(EventId22,RecordNumber23,ReportTotalRecord24,DataId25,&DestBuffer26);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetEventFreezeFrameData_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetEventIdCausingLastDetError_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId27;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetEventIdCausingLastDetError_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_GeneralEvtInfo_GetEventIdCausingLastDetError(&EventId27);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetEventIdCausingLastDetError_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetEventStatus_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId28;
	Dem_UdsStatusByteType EventStatusByte29;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetEventStatus_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh  Rte_Call_GeneralEvtInfo_GetEventStatus(EventId28,&EventStatusByte29);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetEventStatus_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetEventTested_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId30;
	boolean EventTested31;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetEventTested_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh  Rte_Call_GeneralEvtInfo_GetEventTested(EventId30,&EventTested31);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetEventTested_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) Dem_GetFaultDetectionCounter_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId32;
	sint8 FaultDetectionCounter33;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Dem_GetFaultDetectionCounter_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_GeneralEvtInfo_GetFaultDetectionCounter(EventId32,&FaultDetectionCounter33);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Dem_GetFaultDetectionCounter_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetCycleQualified_Adapt_Appl/* return value & FctID */
(
		void
)
{

	boolean CycleState34;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetCycleQualified_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_OpCycle_DemOpCycle_DEM_POWER_GetCycleQualified(&CycleState34);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetCycleQualified_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetDTCOfEvent_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_DTCFormatType DTCFormat35;
	uint32 DTCOfEvent36;
	Std_ReturnType syncCall1;
	Dem_DTCFormatType DTCFormat37;
	uint32 DTCOfEvent38;
	Std_ReturnType syncCall2;
	Dem_DTCFormatType DTCFormat39;
	uint32 DTCOfEvent40;
	Std_ReturnType syncCall3;
	Dem_DTCFormatType DTCFormat41;
	uint32 DTCOfEvent42;
	Std_ReturnType syncCall4;
	Dem_DTCFormatType DTCFormat43;
	uint32 DTCOfEvent44;
	Std_ReturnType syncCall5;
	Dem_DTCFormatType DTCFormat45;
	uint32 DTCOfEvent46;
	Std_ReturnType syncCall6;
	Dem_DTCFormatType DTCFormat47;
	uint32 DTCOfEvent48;
	Std_ReturnType syncCall7;
	Dem_DTCFormatType DTCFormat49;
	uint32 DTCOfEvent50;
	Std_ReturnType syncCall8;
	Dem_DTCFormatType DTCFormat51;
	uint32 DTCOfEvent52;
	Std_ReturnType syncCall9;
	Dem_DTCFormatType DTCFormat53;
	uint32 DTCOfEvent54;
	Std_ReturnType syncCall10;
	Dem_DTCFormatType DTCFormat55;
	uint32 DTCOfEvent56;
	Std_ReturnType syncCall11;
	Dem_DTCFormatType DTCFormat57;
	uint32 DTCOfEvent58;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetDTCOfEvent_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetDTCOfEvent(DTCFormat35,&DTCOfEvent36);
                
	syncCall2 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetDTCOfEvent(DTCFormat37,&DTCOfEvent38);
                
	syncCall3 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetDTCOfEvent(DTCFormat39,&DTCOfEvent40);
                
	syncCall4 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetDTCOfEvent(DTCFormat41,&DTCOfEvent42);
                
	syncCall5 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetDTCOfEvent(DTCFormat43,&DTCOfEvent44);
                
	syncCall6 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetDTCOfEvent(DTCFormat45,&DTCOfEvent46);
                
	syncCall7 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetDTCOfEvent(DTCFormat47,&DTCOfEvent48);
                
	syncCall8 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetDTCOfEvent(DTCFormat49,&DTCOfEvent50);
                
	syncCall9 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetDTCOfEvent(DTCFormat51,&DTCOfEvent52);
                
	syncCall10 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetDTCOfEvent(DTCFormat53,&DTCOfEvent54);
                
	syncCall11 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetDTCOfEvent(DTCFormat55,&DTCOfEvent56);
                
	syncCall12 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetDTCOfEvent(DTCFormat57,&DTCOfEvent58);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetDTCOfEvent_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetDebouncingOfEvent_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_DebouncingStateType DebouncingState59;
	Std_ReturnType syncCall1;
	Dem_DebouncingStateType DebouncingState60;
	Std_ReturnType syncCall2;
	Dem_DebouncingStateType DebouncingState61;
	Std_ReturnType syncCall3;
	Dem_DebouncingStateType DebouncingState62;
	Std_ReturnType syncCall4;
	Dem_DebouncingStateType DebouncingState63;
	Std_ReturnType syncCall5;
	Dem_DebouncingStateType DebouncingState64;
	Std_ReturnType syncCall6;
	Dem_DebouncingStateType DebouncingState65;
	Std_ReturnType syncCall7;
	Dem_DebouncingStateType DebouncingState66;
	Std_ReturnType syncCall8;
	Dem_DebouncingStateType DebouncingState67;
	Std_ReturnType syncCall9;
	Dem_DebouncingStateType DebouncingState68;
	Std_ReturnType syncCall10;
	Dem_DebouncingStateType DebouncingState69;
	Std_ReturnType syncCall11;
	Dem_DebouncingStateType DebouncingState70;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetDebouncingOfEvent_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetDebouncingOfEvent(&DebouncingState59);
                
	syncCall2 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetDebouncingOfEvent(&DebouncingState60);
                
	syncCall3 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetDebouncingOfEvent(&DebouncingState61);
                
	syncCall4 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetDebouncingOfEvent(&DebouncingState62);
                
	syncCall5 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetDebouncingOfEvent(&DebouncingState63);
                
	syncCall6 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetDebouncingOfEvent(&DebouncingState64);
                
	syncCall7 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetDebouncingOfEvent(&DebouncingState65);
                
	syncCall8 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetDebouncingOfEvent(&DebouncingState66);
                
	syncCall9 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetDebouncingOfEvent(&DebouncingState67);
                
	syncCall10 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetDebouncingOfEvent(&DebouncingState68);
                
	syncCall11 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetDebouncingOfEvent(&DebouncingState69);
                
	syncCall12 = E_OK; //tan9sgh Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetDebouncingOfEvent(&DebouncingState70);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetDebouncingOfEvent_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetDebouncingOfEvent_GeneralEvtInfo_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventIdType EventId71;
	Dem_DebouncingStateType DebouncingState72;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetDebouncingOfEvent_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_GeneralEvtInfo_GetDebouncingOfEvent(EventId71,&DebouncingState72);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetDebouncingOfEvent_GeneralEvtInfo_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetEventExtendedDataRecordEx_Adapt_Appl/* return value & FctID */
(
		void
)
{

	uint8 RecordNumber73;
	Dem_MaxDataValueType DestBuffer74;
	uint16 Bufsize75;
	Std_ReturnType syncCall1;
	uint8 RecordNumber76;
	Dem_MaxDataValueType DestBuffer77;
	uint16 Bufsize78;
	Std_ReturnType syncCall2;
	uint8 RecordNumber79;
	Dem_MaxDataValueType DestBuffer80;
	uint16 Bufsize81;
	Std_ReturnType syncCall3;
	uint8 RecordNumber82;
	Dem_MaxDataValueType DestBuffer83;
	uint16 Bufsize84;
	Std_ReturnType syncCall4;
	uint8 RecordNumber85;
	Dem_MaxDataValueType DestBuffer86;
	uint16 Bufsize87;
	Std_ReturnType syncCall5;
	uint8 RecordNumber88;
	Dem_MaxDataValueType DestBuffer89;
	uint16 Bufsize90;
	Std_ReturnType syncCall6;
	uint8 RecordNumber91;
	Dem_MaxDataValueType DestBuffer92;
	uint16 Bufsize93;
	Std_ReturnType syncCall7;
	uint8 RecordNumber94;
	Dem_MaxDataValueType DestBuffer95;
	uint16 Bufsize96;
	Std_ReturnType syncCall8;
	uint8 RecordNumber97;
	Dem_MaxDataValueType DestBuffer98;
	uint16 Bufsize99;
	Std_ReturnType syncCall9;
	uint8 RecordNumber100;
	Dem_MaxDataValueType DestBuffer101;
	uint16 Bufsize102;
	Std_ReturnType syncCall10;
	uint8 RecordNumber103;
	Dem_MaxDataValueType DestBuffer104;
	uint16 Bufsize105;
	Std_ReturnType syncCall11;
	uint8 RecordNumber106;
	Dem_MaxDataValueType DestBuffer107;
	uint16 Bufsize108;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetEventExtendedDataRecordEx_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetEventExtendedDataRecordEx(RecordNumber73,&DestBuffer74,&Bufsize75);
                
	syncCall2 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetEventExtendedDataRecordEx(RecordNumber76,&DestBuffer77,&Bufsize78);
                
	syncCall3 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetEventExtendedDataRecordEx(RecordNumber79,&DestBuffer80,&Bufsize81);
                
	syncCall4 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetEventExtendedDataRecordEx(RecordNumber82,&DestBuffer83,&Bufsize84);
                
	syncCall5 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetEventExtendedDataRecordEx(RecordNumber85,&DestBuffer86,&Bufsize87);
                
	syncCall6 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetEventExtendedDataRecordEx(RecordNumber88,&DestBuffer89,&Bufsize90);
                
	syncCall7 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetEventExtendedDataRecordEx(RecordNumber91,&DestBuffer92,&Bufsize93);
                
	syncCall8 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetEventExtendedDataRecordEx(RecordNumber94,&DestBuffer95,&Bufsize96);
                
	syncCall9 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetEventExtendedDataRecordEx(RecordNumber97,&DestBuffer98,&Bufsize99);
                
	syncCall10 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetEventExtendedDataRecordEx(RecordNumber100,&DestBuffer101,&Bufsize102);
                
	syncCall11 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetEventExtendedDataRecordEx(RecordNumber103,&DestBuffer104,&Bufsize105);
                
	syncCall12 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetEventExtendedDataRecordEx(RecordNumber106,&DestBuffer107,&Bufsize108);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetEventExtendedDataRecordEx_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetEventExtendedDataRecord_Adapt_Appl/* return value & FctID */
(
		void
)
{

	uint8 RecordNumber109;
	Dem_MaxDataValueType DestBuffer110;
	Std_ReturnType syncCall1;
	uint8 RecordNumber111;
	Dem_MaxDataValueType DestBuffer112;
	Std_ReturnType syncCall2;
	uint8 RecordNumber113;
	Dem_MaxDataValueType DestBuffer114;
	Std_ReturnType syncCall3;
	uint8 RecordNumber115;
	Dem_MaxDataValueType DestBuffer116;
	Std_ReturnType syncCall4;
	uint8 RecordNumber117;
	Dem_MaxDataValueType DestBuffer118;
	Std_ReturnType syncCall5;
	uint8 RecordNumber119;
	Dem_MaxDataValueType DestBuffer120;
	Std_ReturnType syncCall6;
	uint8 RecordNumber121;
	Dem_MaxDataValueType DestBuffer122;
	Std_ReturnType syncCall7;
	uint8 RecordNumber123;
	Dem_MaxDataValueType DestBuffer124;
	Std_ReturnType syncCall8;
	uint8 RecordNumber125;
	Dem_MaxDataValueType DestBuffer126;
	Std_ReturnType syncCall9;
	uint8 RecordNumber127;
	Dem_MaxDataValueType DestBuffer128;
	Std_ReturnType syncCall10;
	uint8 RecordNumber129;
	Dem_MaxDataValueType DestBuffer130;
	Std_ReturnType syncCall11;
	uint8 RecordNumber131;
	Dem_MaxDataValueType DestBuffer132;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetEventExtendedDataRecord_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetEventExtendedDataRecord(RecordNumber109,&DestBuffer110);
                
	syncCall2 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetEventExtendedDataRecord(RecordNumber111,&DestBuffer112);
                
	syncCall3 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetEventExtendedDataRecord(RecordNumber113,&DestBuffer114);
                
	syncCall4 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetEventExtendedDataRecord(RecordNumber115,&DestBuffer116);
                
	syncCall5 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetEventExtendedDataRecord(RecordNumber117,&DestBuffer118);
                
	syncCall6 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetEventExtendedDataRecord(RecordNumber119,&DestBuffer120);
                
	syncCall7 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetEventExtendedDataRecord(RecordNumber121,&DestBuffer122);
                
	syncCall8 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetEventExtendedDataRecord(RecordNumber123,&DestBuffer124);
                
	syncCall9 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetEventExtendedDataRecord(RecordNumber125,&DestBuffer126);
                
	syncCall10 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetEventExtendedDataRecord(RecordNumber127,&DestBuffer128);
                
	syncCall11 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetEventExtendedDataRecord(RecordNumber129,&DestBuffer130);
                
	syncCall12 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetEventExtendedDataRecord(RecordNumber131,&DestBuffer132);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetEventExtendedDataRecord_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetEventFailed_Adapt_Appl/* return value & FctID */
(
		void
)
{

	boolean EventFailed133;
	Std_ReturnType syncCall1;
	boolean EventFailed134;
	Std_ReturnType syncCall2;
	boolean EventFailed135;
	Std_ReturnType syncCall3;
	boolean EventFailed136;
	Std_ReturnType syncCall4;
	boolean EventFailed137;
	Std_ReturnType syncCall5;
	boolean EventFailed138;
	Std_ReturnType syncCall6;
	boolean EventFailed139;
	Std_ReturnType syncCall7;
	boolean EventFailed140;
	Std_ReturnType syncCall8;
	boolean EventFailed141;
	Std_ReturnType syncCall9;
	boolean EventFailed142;
	Std_ReturnType syncCall10;
	boolean EventFailed143;
	Std_ReturnType syncCall11;
	boolean EventFailed144;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetEventFailed_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetEventFailed(&EventFailed133);
                
	syncCall2 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetEventFailed(&EventFailed134);
                
	syncCall3 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetEventFailed(&EventFailed135);
                
	syncCall4 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetEventFailed(&EventFailed136);
                
	syncCall5 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetEventFailed(&EventFailed137);
                
	syncCall6 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetEventFailed(&EventFailed138);
                
	syncCall7 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetEventFailed(&EventFailed139);
                
	syncCall8 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetEventFailed(&EventFailed140);
                
	syncCall9 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetEventFailed(&EventFailed141);
                
	syncCall10 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetEventFailed(&EventFailed142);
                
	syncCall11 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetEventFailed(&EventFailed143);
                
	syncCall12 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetEventFailed(&EventFailed144);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetEventFailed_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetEventFreezeFrameDataEx_Adapt_Appl/* return value & FctID */
(
		void
)
{

	uint8 RecordNumber145;
	uint16 DataId146;
	Dem_MaxDataValueType DestBuffer147;
	uint16 BufSize148;
	Std_ReturnType syncCall1;
	uint8 RecordNumber149;
	uint16 DataId150;
	Dem_MaxDataValueType DestBuffer151;
	uint16 BufSize152;
	Std_ReturnType syncCall2;
	uint8 RecordNumber153;
	uint16 DataId154;
	Dem_MaxDataValueType DestBuffer155;
	uint16 BufSize156;
	Std_ReturnType syncCall3;
	uint8 RecordNumber157;
	uint16 DataId158;
	Dem_MaxDataValueType DestBuffer159;
	uint16 BufSize160;
	Std_ReturnType syncCall4;
	uint8 RecordNumber161;
	uint16 DataId162;
	Dem_MaxDataValueType DestBuffer163;
	uint16 BufSize164;
	Std_ReturnType syncCall5;
	uint8 RecordNumber165;
	uint16 DataId166;
	Dem_MaxDataValueType DestBuffer167;
	uint16 BufSize168;
	Std_ReturnType syncCall6;
	uint8 RecordNumber169;
	uint16 DataId170;
	Dem_MaxDataValueType DestBuffer171;
	uint16 BufSize172;
	Std_ReturnType syncCall7;
	uint8 RecordNumber173;
	uint16 DataId174;
	Dem_MaxDataValueType DestBuffer175;
	uint16 BufSize176;
	Std_ReturnType syncCall8;
	uint8 RecordNumber177;
	uint16 DataId178;
	Dem_MaxDataValueType DestBuffer179;
	uint16 BufSize180;
	Std_ReturnType syncCall9;
	uint8 RecordNumber181;
	uint16 DataId182;
	Dem_MaxDataValueType DestBuffer183;
	uint16 BufSize184;
	Std_ReturnType syncCall10;
	uint8 RecordNumber185;
	uint16 DataId186;
	Dem_MaxDataValueType DestBuffer187;
	uint16 BufSize188;
	Std_ReturnType syncCall11;
	uint8 RecordNumber189;
	uint16 DataId190;
	Dem_MaxDataValueType DestBuffer191;
	uint16 BufSize192;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetEventFreezeFrameDataEx_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetEventFreezeFrameDataEx(RecordNumber145,DataId146,&DestBuffer147,&BufSize148);
                
	syncCall2 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetEventFreezeFrameDataEx(RecordNumber149,DataId150,&DestBuffer151,&BufSize152);
                
	syncCall3 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetEventFreezeFrameDataEx(RecordNumber153,DataId154,&DestBuffer155,&BufSize156);
                
	syncCall4 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetEventFreezeFrameDataEx(RecordNumber157,DataId158,&DestBuffer159,&BufSize160);
                
	syncCall5 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetEventFreezeFrameDataEx(RecordNumber161,DataId162,&DestBuffer163,&BufSize164);
                
	syncCall6 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetEventFreezeFrameDataEx(RecordNumber165,DataId166,&DestBuffer167,&BufSize168);
                
	syncCall7 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetEventFreezeFrameDataEx(RecordNumber169,DataId170,&DestBuffer171,&BufSize172);
                
	syncCall8 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetEventFreezeFrameDataEx(RecordNumber173,DataId174,&DestBuffer175,&BufSize176);
                
	syncCall9 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetEventFreezeFrameDataEx(RecordNumber177,DataId178,&DestBuffer179,&BufSize180);
                
	syncCall10 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetEventFreezeFrameDataEx(RecordNumber181,DataId182,&DestBuffer183,&BufSize184);
                
	syncCall11 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetEventFreezeFrameDataEx(RecordNumber185,DataId186,&DestBuffer187,&BufSize188);
                
	syncCall12 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetEventFreezeFrameDataEx(RecordNumber189,DataId190,&DestBuffer191,&BufSize192);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetEventFreezeFrameDataEx_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetEventFreezeFrameData_Adapt_Appl/* return value & FctID */
(
		void
)
{

	uint8 RecordNumber193;
	boolean ReportTotalRecord194;
	uint16 DataId195;
	Dem_MaxDataValueType DestBuffer196;
	Std_ReturnType syncCall1;
	uint8 RecordNumber197;
	boolean ReportTotalRecord198;
	uint16 DataId199;
	Dem_MaxDataValueType DestBuffer200;
	Std_ReturnType syncCall2;
	uint8 RecordNumber201;
	boolean ReportTotalRecord202;
	uint16 DataId203;
	Dem_MaxDataValueType DestBuffer204;
	Std_ReturnType syncCall3;
	uint8 RecordNumber205;
	boolean ReportTotalRecord206;
	uint16 DataId207;
	Dem_MaxDataValueType DestBuffer208;
	Std_ReturnType syncCall4;
	uint8 RecordNumber209;
	boolean ReportTotalRecord210;
	uint16 DataId211;
	Dem_MaxDataValueType DestBuffer212;
	Std_ReturnType syncCall5;
	uint8 RecordNumber213;
	boolean ReportTotalRecord214;
	uint16 DataId215;
	Dem_MaxDataValueType DestBuffer216;
	Std_ReturnType syncCall6;
	uint8 RecordNumber217;
	boolean ReportTotalRecord218;
	uint16 DataId219;
	Dem_MaxDataValueType DestBuffer220;
	Std_ReturnType syncCall7;
	uint8 RecordNumber221;
	boolean ReportTotalRecord222;
	uint16 DataId223;
	Dem_MaxDataValueType DestBuffer224;
	Std_ReturnType syncCall8;
	uint8 RecordNumber225;
	boolean ReportTotalRecord226;
	uint16 DataId227;
	Dem_MaxDataValueType DestBuffer228;
	Std_ReturnType syncCall9;
	uint8 RecordNumber229;
	boolean ReportTotalRecord230;
	uint16 DataId231;
	Dem_MaxDataValueType DestBuffer232;
	Std_ReturnType syncCall10;
	uint8 RecordNumber233;
	boolean ReportTotalRecord234;
	uint16 DataId235;
	Dem_MaxDataValueType DestBuffer236;
	Std_ReturnType syncCall11;
	uint8 RecordNumber237;
	boolean ReportTotalRecord238;
	uint16 DataId239;
	Dem_MaxDataValueType DestBuffer240;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetEventFreezeFrameData_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetEventFreezeFrameData(RecordNumber193,ReportTotalRecord194,DataId195,&DestBuffer196);
                
	syncCall2 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetEventFreezeFrameData(RecordNumber197,ReportTotalRecord198,DataId199,&DestBuffer200);
                
	syncCall3 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetEventFreezeFrameData(RecordNumber201,ReportTotalRecord202,DataId203,&DestBuffer204);
                
	syncCall4 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetEventFreezeFrameData(RecordNumber205,ReportTotalRecord206,DataId207,&DestBuffer208);
                
	syncCall5 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetEventFreezeFrameData(RecordNumber209,ReportTotalRecord210,DataId211,&DestBuffer212);
                
	syncCall6 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetEventFreezeFrameData(RecordNumber213,ReportTotalRecord214,DataId215,&DestBuffer216);
                
	syncCall7 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetEventFreezeFrameData(RecordNumber217,ReportTotalRecord218,DataId219,&DestBuffer220);
                
	syncCall8 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetEventFreezeFrameData(RecordNumber221,ReportTotalRecord222,DataId223,&DestBuffer224);
                
	syncCall9 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetEventFreezeFrameData(RecordNumber225,ReportTotalRecord226,DataId227,&DestBuffer228);
                
	syncCall10 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetEventFreezeFrameData(RecordNumber229,ReportTotalRecord230,DataId231,&DestBuffer232);
                
	syncCall11 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetEventFreezeFrameData(RecordNumber233,ReportTotalRecord234,DataId235,&DestBuffer236);
                
	syncCall12 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetEventFreezeFrameData(RecordNumber237,ReportTotalRecord238,DataId239,&DestBuffer240);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetEventFreezeFrameData_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetEventStatus_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_UdsStatusByteType EventStatusByte241;
	Std_ReturnType syncCall1;
	Dem_UdsStatusByteType EventStatusByte242;
	Std_ReturnType syncCall2;
	Dem_UdsStatusByteType EventStatusByte243;
	Std_ReturnType syncCall3;
	Dem_UdsStatusByteType EventStatusByte244;
	Std_ReturnType syncCall4;
	Dem_UdsStatusByteType EventStatusByte245;
	Std_ReturnType syncCall5;
	Dem_UdsStatusByteType EventStatusByte246;
	Std_ReturnType syncCall6;
	Dem_UdsStatusByteType EventStatusByte247;
	Std_ReturnType syncCall7;
	Dem_UdsStatusByteType EventStatusByte248;
	Std_ReturnType syncCall8;
	Dem_UdsStatusByteType EventStatusByte249;
	Std_ReturnType syncCall9;
	Dem_UdsStatusByteType EventStatusByte250;
	Std_ReturnType syncCall10;
	Dem_UdsStatusByteType EventStatusByte251;
	Std_ReturnType syncCall11;
	Dem_UdsStatusByteType EventStatusByte252;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetEventStatus_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetEventStatus(&EventStatusByte241);
                
	syncCall2 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetEventStatus(&EventStatusByte242);
                
	syncCall3 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetEventStatus(&EventStatusByte243);
                
	syncCall4 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetEventStatus(&EventStatusByte244);
                
	syncCall5 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetEventStatus(&EventStatusByte245);
                
	syncCall6 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetEventStatus(&EventStatusByte246);
                
	syncCall7 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetEventStatus(&EventStatusByte247);
                
	syncCall8 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetEventStatus(&EventStatusByte248);
                
	syncCall9 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetEventStatus(&EventStatusByte249);
                
	syncCall10 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetEventStatus(&EventStatusByte250);
                
	syncCall11 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetEventStatus(&EventStatusByte251);
                
	syncCall12 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetEventStatus(&EventStatusByte252);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetEventStatus_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetEventTested_Adapt_Appl/* return value & FctID */
(
		void
)
{

	boolean EventTested253;
	Std_ReturnType syncCall1;
	boolean EventTested254;
	Std_ReturnType syncCall2;
	boolean EventTested255;
	Std_ReturnType syncCall3;
	boolean EventTested256;
	Std_ReturnType syncCall4;
	boolean EventTested257;
	Std_ReturnType syncCall5;
	boolean EventTested258;
	Std_ReturnType syncCall6;
	boolean EventTested259;
	Std_ReturnType syncCall7;
	boolean EventTested260;
	Std_ReturnType syncCall8;
	boolean EventTested261;
	Std_ReturnType syncCall9;
	boolean EventTested262;
	Std_ReturnType syncCall10;
	boolean EventTested263;
	Std_ReturnType syncCall11;
	boolean EventTested264;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetEventTested_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetEventTested(&EventTested253);
                
	syncCall2 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetEventTested(&EventTested254);
                
	syncCall3 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetEventTested(&EventTested255);
                
	syncCall4 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetEventTested(&EventTested256);
                
	syncCall5 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetEventTested(&EventTested257);
                
	syncCall6 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetEventTested(&EventTested258);
                
	syncCall7 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetEventTested(&EventTested259);
                
	syncCall8 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetEventTested(&EventTested260);
                
	syncCall9 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetEventTested(&EventTested261);
                
	syncCall10 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetEventTested(&EventTested262);
                
	syncCall11 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetEventTested(&EventTested263);
                
	syncCall12 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetEventTested(&EventTested264);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetEventTested_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetFaultDetectionCounter_Adapt_Appl/* return value & FctID */
(
		void
)
{

	sint8 FaultDetectionCounter265;
	Std_ReturnType syncCall1;
	sint8 FaultDetectionCounter266;
	Std_ReturnType syncCall2;
	sint8 FaultDetectionCounter267;
	Std_ReturnType syncCall3;
	sint8 FaultDetectionCounter268;
	Std_ReturnType syncCall4;
	sint8 FaultDetectionCounter269;
	Std_ReturnType syncCall5;
	sint8 FaultDetectionCounter270;
	Std_ReturnType syncCall6;
	sint8 FaultDetectionCounter271;
	Std_ReturnType syncCall7;
	sint8 FaultDetectionCounter272;
	Std_ReturnType syncCall8;
	sint8 FaultDetectionCounter273;
	Std_ReturnType syncCall9;
	sint8 FaultDetectionCounter274;
	Std_ReturnType syncCall10;
	sint8 FaultDetectionCounter275;
	Std_ReturnType syncCall11;
	sint8 FaultDetectionCounter276;
	Std_ReturnType syncCall12;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetFaultDetectionCounter_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x000001_Event_GetFaultDetectionCounter(&FaultDetectionCounter265);
                
	syncCall2 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_GetFaultDetectionCounter(&FaultDetectionCounter266);
                
	syncCall3 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc07388_Event_GetFaultDetectionCounter(&FaultDetectionCounter267);
                
	syncCall4 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc12687_Event_GetFaultDetectionCounter(&FaultDetectionCounter268);
                
	syncCall5 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc14587_Event_GetFaultDetectionCounter(&FaultDetectionCounter269);
                
	syncCall6 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc22a87_Event_GetFaultDetectionCounter(&FaultDetectionCounter270);
                
	syncCall7 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xc41487_Event_GetFaultDetectionCounter(&FaultDetectionCounter271);
                
	syncCall8 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56217_Event_GetFaultDetectionCounter(&FaultDetectionCounter272);
                
	syncCall9 = E_OK; //tan9sgh RRte_Call_EvtInfo_EventParameter_DTC_0xd56316_Event_GetFaultDetectionCounter(&FaultDetectionCounter273);
                
	syncCall10 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00141_Event_GetFaultDetectionCounter(&FaultDetectionCounter274);
                
	syncCall11 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe00142_Event_GetFaultDetectionCounter(&FaultDetectionCounter275);
                
	syncCall12 = E_OK; //tan9sgh  Rte_Call_EvtInfo_EventParameter_DTC_0xe26200_Event_GetFaultDetectionCounter(&FaultDetectionCounter276);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetFaultDetectionCounter_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetOperationCycleState_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_OperationCycleStateType CycleState277;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetOperationCycleState_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_OpCycle_DemOpCycle_DEM_POWER_GetOperationCycleState(&CycleState277);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetOperationCycleState_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) GetStorageCondition_Adapt_Appl/* return value & FctID */
(
		void
)
{

	boolean ConditionFulfilled278;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :GetStorageCondition_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_StorageCond_DemStorageCondition_0_GetStorageCondition(&ConditionFulfilled278);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :GetStorageCondition_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) PrestoreFreezeFrame_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Std_ReturnType syncCall1;
	Std_ReturnType syncCall2;
	Std_ReturnType syncCall3;
	Std_ReturnType syncCall4;
	Std_ReturnType syncCall5;
	Std_ReturnType syncCall6;
	Std_ReturnType syncCall7;
	Std_ReturnType syncCall8;
	Std_ReturnType syncCall9;
	Std_ReturnType syncCall10;
	Std_ReturnType syncCall11;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :PrestoreFreezeFrame_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0x000001_Event_PrestoreFreezeFrame();
                
	syncCall2 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc07388_Event_PrestoreFreezeFrame();
                
	syncCall3 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe26200_Event_PrestoreFreezeFrame();
                
	syncCall4 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc12687_Event_PrestoreFreezeFrame();
                
	syncCall5 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc14587_Event_PrestoreFreezeFrame();
                
	syncCall6 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc22a87_Event_PrestoreFreezeFrame();
                
	syncCall7 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc41487_Event_PrestoreFreezeFrame();
                
	syncCall8 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56217_Event_PrestoreFreezeFrame();
                
	syncCall9 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56316_Event_PrestoreFreezeFrame();
                
	syncCall10 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe00141_Event_PrestoreFreezeFrame();
                
	syncCall11 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe00142_Event_PrestoreFreezeFrame();

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :PrestoreFreezeFrame_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) ResetEventDebounceStatus_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_DebounceResetStatusType DebounceResetStatus279;
	Std_ReturnType syncCall1;
	Dem_DebounceResetStatusType DebounceResetStatus280;
	Std_ReturnType syncCall2;
	Dem_DebounceResetStatusType DebounceResetStatus281;
	Std_ReturnType syncCall3;
	Dem_DebounceResetStatusType DebounceResetStatus282;
	Std_ReturnType syncCall4;
	Dem_DebounceResetStatusType DebounceResetStatus283;
	Std_ReturnType syncCall5;
	Dem_DebounceResetStatusType DebounceResetStatus284;
	Std_ReturnType syncCall6;
	Dem_DebounceResetStatusType DebounceResetStatus285;
	Std_ReturnType syncCall7;
	Dem_DebounceResetStatusType DebounceResetStatus286;
	Std_ReturnType syncCall8;
	Dem_DebounceResetStatusType DebounceResetStatus287;
	Std_ReturnType syncCall9;
	Dem_DebounceResetStatusType DebounceResetStatus288;
	Std_ReturnType syncCall10;
	Dem_DebounceResetStatusType DebounceResetStatus289;
	Std_ReturnType syncCall11;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :ResetEventDebounceStatus_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0x000001_Event_ResetEventDebounceStatus(DebounceResetStatus279);
                
	syncCall2 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc07388_Event_ResetEventDebounceStatus(DebounceResetStatus280);
                
	syncCall3 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe26200_Event_ResetEventDebounceStatus(DebounceResetStatus281);
                
	syncCall4 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc12687_Event_ResetEventDebounceStatus(DebounceResetStatus282);
                
	syncCall5 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc14587_Event_ResetEventDebounceStatus(DebounceResetStatus283);
                
	syncCall6 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc22a87_Event_ResetEventDebounceStatus(DebounceResetStatus284);
                
	syncCall7 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc41487_Event_ResetEventDebounceStatus(DebounceResetStatus285);
                
	syncCall8 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56217_Event_ResetEventDebounceStatus(DebounceResetStatus286);
                
	syncCall9 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56316_Event_ResetEventDebounceStatus(DebounceResetStatus287);
                
	syncCall10 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe00141_Event_ResetEventDebounceStatus(DebounceResetStatus288);
                
	syncCall11 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe00142_Event_ResetEventDebounceStatus(DebounceResetStatus289);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :ResetEventDebounceStatus_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) ResetEventStatus_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Std_ReturnType syncCall1;
	Std_ReturnType syncCall2;
	Std_ReturnType syncCall3;
	Std_ReturnType syncCall4;
	Std_ReturnType syncCall5;
	Std_ReturnType syncCall6;
	Std_ReturnType syncCall7;
	Std_ReturnType syncCall8;
	Std_ReturnType syncCall9;
	Std_ReturnType syncCall10;
	Std_ReturnType syncCall11;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :ResetEventStatus_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0x000001_Event_ResetEventStatus();
                
	syncCall2 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc07388_Event_ResetEventStatus();
                
	syncCall3 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe26200_Event_ResetEventStatus();
                
	syncCall4 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc12687_Event_ResetEventStatus();
                
	syncCall5 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc14587_Event_ResetEventStatus();
                
	syncCall6 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc22a87_Event_ResetEventStatus();
                
	syncCall7 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc41487_Event_ResetEventStatus();
                
	syncCall8 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56217_Event_ResetEventStatus();
                
	syncCall9 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56316_Event_ResetEventStatus();
                
	syncCall10 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe00141_Event_ResetEventStatus();
                
	syncCall11 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe00142_Event_ResetEventStatus();

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :ResetEventStatus_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) RoutineServices_DcmDspRoutine_Routine_Control_203_Start/* return value & FctID */
(
		VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,
		P2VAR(Dcm_StartDataOut_Routine_Control_203_DcmDspStartRoutineOutSignal_0Type, AUTOMATIC, RTE_APPL_DATA) DataOut_DcmDspStartRoutineOutSignal_0,
		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_APPL_DATA) ErrorCode
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :RoutineServices_DcmDspRoutine_Routine_Control_203_Start) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :RoutineServices_DcmDspRoutine_Routine_Control_203_Start) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) RoutineServices_DcmDspRoutine_Routine_Control_ABFF_RequestResults/* return value & FctID */
(
		VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,
		P2VAR(Dcm_RequestDataOut_Routine_Control_ABFF_DcmDspRequestRoutineResultsOutSignal_0Type, AUTOMATIC, RTE_APPL_DATA) DataOut_DcmDspRequestRoutineResultsOutSignal_0,
		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_APPL_DATA) ErrorCode
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :RoutineServices_DcmDspRoutine_Routine_Control_ABFF_RequestResults) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :RoutineServices_DcmDspRoutine_Routine_Control_ABFF_RequestResults) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) RoutineServices_DcmDspRoutine_Routine_Control_ABFF_Start/* return value & FctID */
(
		VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,
		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_APPL_DATA) ErrorCode
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :RoutineServices_DcmDspRoutine_Routine_Control_ABFF_Start) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :RoutineServices_DcmDspRoutine_Routine_Control_ABFF_Start) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) RoutineServices_DcmDspRoutine_X_Routine_Control_ABFE_Start/* return value & FctID */
(
		VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,
		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_APPL_DATA) ErrorCode
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :RoutineServices_DcmDspRoutine_X_Routine_Control_ABFE_Start) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :RoutineServices_DcmDspRoutine_X_Routine_Control_ABFE_Start) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) SecurityAccess_Unlocked_L1_CompareKey/* return value & FctID */
(
		P2CONST(uint8, AUTOMATIC, RTE_APPL_DATA) Key,
		VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,
		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_APPL_DATA) ErrorCode
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SecurityAccess_Unlocked_L1_CompareKey) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

    if ( ( Key[ 0 ] != Dcm_Key[ 0 ] ) || ( Key[ 1 ] != Dcm_Key[ 1 ] )||( Key[ 2 ] != Dcm_Key[ 2 ] ) || ( Key[ 3 ] != Dcm_Key[ 3 ] ) )
    {
        retValue = RTE_E_SecurityAccess_UnlockedL1_DCM_E_COMPARE_KEY_FAILED;
        //*ErrorCode = DCM_E_INVALIDKEY;
    }
    else
    {
    }
	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SecurityAccess_Unlocked_L1_CompareKey) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) SecurityAccess_Unlocked_L1_GetSeed/* return value & FctID */
(
		VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,
		P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) Seed,
		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_APPL_DATA) ErrorCode
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SecurityAccess_Unlocked_L1_GetSeed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

    *Seed = Dcm_Seed[0];
    *(Seed+1) = Dcm_Seed[1];
    *(Seed+2) = Dcm_Seed[2];
    *(Seed+3) = Dcm_Seed[3];
	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SecurityAccess_Unlocked_L1_GetSeed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) SecurityAccess_Unlocked_L3_CompareKey/* return value & FctID */
(
		P2CONST(uint8, AUTOMATIC, RTE_APPL_DATA) Key,
		VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,
		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_APPL_DATA) ErrorCode
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SecurityAccess_Unlocked_L3_CompareKey) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

    if ( ( Key[ 0 ] != Dcm_Key[ 4 ] ) || ( Key[ 1 ] != Dcm_Key[ 5 ] )||( Key[ 2 ] != Dcm_Key[ 6 ] ) || ( Key[ 3 ] != Dcm_Key[ 7 ] ) )
    {
        retValue = RTE_E_SecurityAccess_Unlocked_L3_DCM_E_COMPARE_KEY_FAILED;
        //*ErrorCode = DCM_E_INVALIDKEY;
    }
    else
    {
    }
	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SecurityAccess_Unlocked_L3_CompareKey) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (Std_ReturnType, DiagUT_CODE) SecurityAccess_Unlocked_L3_GetSeed/* return value & FctID */
(
		VAR(Dcm_OpStatusType, AUTOMATIC) OpStatus,
		P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) Seed,
		P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_APPL_DATA) ErrorCode
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SecurityAccess_Unlocked_L3_GetSeed) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

    *Seed = Dcm_Seed[0];
    *(Seed+1) = Dcm_Seed[1];
    *(Seed+2) = Dcm_Seed[2];
    *(Seed+3) = Dcm_Seed[3];
	/*  -------------------------------------- Server Call Point  --------------------------------  */

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SecurityAccess_Unlocked_L3_GetSeed) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) SetCycleQualified_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SetCycleQualified_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 =  E_OK; //tan9sgh Rte_Call_OpCycle_DemOpCycle_DEM_POWER_SetCycleQualified();

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SetCycleQualified_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) SetEventAvailable_Adapt_Appl/* return value & FctID */
(
		void
)
{

	boolean AvailableStatus290;
	Std_ReturnType syncCall1;
	boolean AvailableStatus291;
	Std_ReturnType syncCall2;
	boolean AvailableStatus292;
	Std_ReturnType syncCall3;
	boolean AvailableStatus293;
	Std_ReturnType syncCall4;
	boolean AvailableStatus294;
	Std_ReturnType syncCall5;
	boolean AvailableStatus295;
	Std_ReturnType syncCall6;
	boolean AvailableStatus296;
	Std_ReturnType syncCall7;
	boolean AvailableStatus297;
	Std_ReturnType syncCall8;
	boolean AvailableStatus298;
	Std_ReturnType syncCall9;
	boolean AvailableStatus299;
	Std_ReturnType syncCall10;
	boolean AvailableStatus300;
	Std_ReturnType syncCall11;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SetEventAvailable_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0x000001_Event_SetEventAvailable(AvailableStatus290);
                
	syncCall2 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xc07388_Event_SetEventAvailable(AvailableStatus291);
                
	syncCall3 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xe26200_Event_SetEventAvailable(AvailableStatus292);
                
	syncCall4 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xc12687_Event_SetEventAvailable(AvailableStatus293);
                
	syncCall5 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xc14587_Event_SetEventAvailable(AvailableStatus294);
                
	syncCall6 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xc22a87_Event_SetEventAvailable(AvailableStatus295);
                
	syncCall7 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xc41487_Event_SetEventAvailable(AvailableStatus296);
                
	syncCall8 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xd56217_Event_SetEventAvailable(AvailableStatus297);
                
	syncCall9 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xd56316_Event_SetEventAvailable(AvailableStatus298);
                
	syncCall10 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xe00141_Event_SetEventAvailable(AvailableStatus299);
                
	syncCall11 = E_OK; //tan9sgh Rte_Call_ControlEventAvailable_EventParameter_DTC_0xe00142_Event_SetEventAvailable(AvailableStatus300);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SetEventAvailable_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) SetEventStatusWithEnvdata_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventStatusType EventStatus301;
	Dem_DebugDataType debug0302;
	Dem_DebugDataType debug1303;
	Std_ReturnType syncCall1;
	Dem_EventStatusType EventStatus304;
	Dem_DebugDataType debug0305;
	Dem_DebugDataType debug1306;
	Std_ReturnType syncCall2;
	Dem_EventStatusType EventStatus307;
	Dem_DebugDataType debug0308;
	Dem_DebugDataType debug1309;
	Std_ReturnType syncCall3;
	Dem_EventStatusType EventStatus310;
	Dem_DebugDataType debug0311;
	Dem_DebugDataType debug1312;
	Std_ReturnType syncCall4;
	Dem_EventStatusType EventStatus313;
	Dem_DebugDataType debug0314;
	Dem_DebugDataType debug1315;
	Std_ReturnType syncCall5;
	Dem_EventStatusType EventStatus316;
	Dem_DebugDataType debug0317;
	Dem_DebugDataType debug1318;
	Std_ReturnType syncCall6;
	Dem_EventStatusType EventStatus319;
	Dem_DebugDataType debug0320;
	Dem_DebugDataType debug1321;
	Std_ReturnType syncCall7;
	Dem_EventStatusType EventStatus322;
	Dem_DebugDataType debug0323;
	Dem_DebugDataType debug1324;
	Std_ReturnType syncCall8;
	Dem_EventStatusType EventStatus325;
	Dem_DebugDataType debug0326;
	Dem_DebugDataType debug1327;
	Std_ReturnType syncCall9;
	Dem_EventStatusType EventStatus328;
	Dem_DebugDataType debug0329;
	Dem_DebugDataType debug1330;
	Std_ReturnType syncCall10;
	Dem_EventStatusType EventStatus331;
	Dem_DebugDataType debug0332;
	Dem_DebugDataType debug1333;
	Std_ReturnType syncCall11;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SetEventStatusWithEnvdata_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0x000001_Event_SetEventStatusWithEnvdata(EventStatus301,debug0302,debug1303);
                
	syncCall2 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc07388_Event_SetEventStatusWithEnvdata(EventStatus304,debug0305,debug1306);
                
	syncCall3 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe26200_Event_SetEventStatusWithEnvdata(EventStatus307,debug0308,debug1309);
                
	syncCall4 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc12687_Event_SetEventStatusWithEnvdata(EventStatus310,debug0311,debug1312);
                
	syncCall5 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc14587_Event_SetEventStatusWithEnvdata(EventStatus313,debug0314,debug1315);
                
	syncCall6 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc22a87_Event_SetEventStatusWithEnvdata(EventStatus316,debug0317,debug1318);
                
	syncCall7 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc41487_Event_SetEventStatusWithEnvdata(EventStatus319,debug0320,debug1321);
                
	syncCall8 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56217_Event_SetEventStatusWithEnvdata(EventStatus322,debug0323,debug1324);
                
	syncCall9 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56316_Event_SetEventStatusWithEnvdata(EventStatus325,debug0326,debug1327);
                
	syncCall10 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe00141_Event_SetEventStatusWithEnvdata(EventStatus328,debug0329,debug1330);
                
	syncCall11 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe00142_Event_SetEventStatusWithEnvdata(EventStatus331,debug0332,debug1333);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SetEventStatusWithEnvdata_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) SetEventStatus_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_EventStatusType EventStatus334;
	Std_ReturnType syncCall1;
	Dem_EventStatusType EventStatus335;
	Std_ReturnType syncCall2;
	Dem_EventStatusType EventStatus336;
	Std_ReturnType syncCall3;
	Dem_EventStatusType EventStatus337;
	Std_ReturnType syncCall4;
	Dem_EventStatusType EventStatus338;
	Std_ReturnType syncCall5;
	Dem_EventStatusType EventStatus339;
	Std_ReturnType syncCall6;
	Dem_EventStatusType EventStatus340;
	Std_ReturnType syncCall7;
	Dem_EventStatusType EventStatus341;
	Std_ReturnType syncCall8;
	Dem_EventStatusType EventStatus342;
	Std_ReturnType syncCall9;
	Dem_EventStatusType EventStatus343;
	Std_ReturnType syncCall10;
	Dem_EventStatusType EventStatus344;
	Std_ReturnType syncCall11;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SetEventStatus_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	EventStatus334 = DEM_EVENT_STATUS_FAILED;
	static uint8 test =0;//50ms testfailed demo
	test ++;
	if(test >=5)
	{
			syncCall1 = Rte_Call_Event_EventParameter_DTC_0x000001_Event_SetEventStatus(EventStatus334);
			test =0;
	}
	syncCall2 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc07388_Event_SetEventStatus(EventStatus335);
                
	syncCall3 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe26200_Event_SetEventStatus(EventStatus336);
                
	syncCall4 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc12687_Event_SetEventStatus(EventStatus337);
                
	syncCall5 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc14587_Event_SetEventStatus(EventStatus338);
                
	syncCall6 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc22a87_Event_SetEventStatus(EventStatus339);
                
	syncCall7 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xc41487_Event_SetEventStatus(EventStatus340);
                
	syncCall8 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56217_Event_SetEventStatus(EventStatus341);
                
	syncCall9 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xd56316_Event_SetEventStatus(EventStatus342);
                
	syncCall10 =E_OK; //tan9sgh  Rte_Call_Event_EventParameter_DTC_0xe00141_Event_SetEventStatus(EventStatus343);
                
	syncCall11 = E_OK; //tan9sgh Rte_Call_Event_EventParameter_DTC_0xe00142_Event_SetEventStatus(EventStatus344);
                
	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SetEventStatus_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
uint8 test_DiagUT =0;
FUNC (void, DiagUT_CODE) SetOperationCycleState_Adapt_Appl/* return value & FctID */
(
		void
)
{

	Dem_OperationCycleStateType CycleState345;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SetOperationCycleState_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */
	CycleState345 = test_DiagUT;//DEM_CYCLE_STATE_START;
	if(DEM_OperationCycle_Start == 1)
	{
		syncCall1 = Rte_Call_OpCycle_DemOpCycle_DEM_POWER_SetOperationCycleState(CycleState345);
		DEM_OperationCycle_Start = 0;
	}

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SetOperationCycleState_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) SetStorageCondition_Adapt_Appl/* return value & FctID */
(
		void
)
{

	boolean ConditionFulfilled346;
	Std_ReturnType syncCall1;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SetStorageCondition_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */
	ConditionFulfilled346 = 1;
	syncCall1 = Rte_Call_StorageCond_DemStorageCondition_0_SetStorageCondition(ConditionFulfilled346);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SetStorageCondition_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
FUNC (void, DiagUT_CODE) SetWIRStatus_Adapt_Appl/* return value & FctID */
(
		void
)
{

	boolean WIRStatus347;
	Std_ReturnType syncCall1;
	boolean WIRStatus348;
	Std_ReturnType syncCall2;
	boolean WIRStatus349;
	Std_ReturnType syncCall3;
	boolean WIRStatus350;
	Std_ReturnType syncCall4;
	boolean WIRStatus351;
	Std_ReturnType syncCall5;
	boolean WIRStatus352;
	Std_ReturnType syncCall6;
	boolean WIRStatus353;
	Std_ReturnType syncCall7;
	boolean WIRStatus354;
	Std_ReturnType syncCall8;
	boolean WIRStatus355;
	Std_ReturnType syncCall9;
	boolean WIRStatus356;
	Std_ReturnType syncCall10;
	boolean WIRStatus357;
	Std_ReturnType syncCall11;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :SetWIRStatus_Adapt_Appl) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */

	syncCall1 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0x000001_Event_SetWIRStatus(WIRStatus347);
                
	syncCall2 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xc07388_Event_SetWIRStatus(WIRStatus348);
                
	syncCall3 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xe26200_Event_SetWIRStatus(WIRStatus349);
                
	syncCall4 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xc12687_Event_SetWIRStatus(WIRStatus350);
                
	syncCall5 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xc14587_Event_SetWIRStatus(WIRStatus351);
                
	syncCall6 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xc22a87_Event_SetWIRStatus(WIRStatus352);
                
	syncCall7 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xc41487_Event_SetWIRStatus(WIRStatus353);
                
	syncCall8 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xd56217_Event_SetWIRStatus(WIRStatus354);
                
	syncCall9 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xd56316_Event_SetWIRStatus(WIRStatus355);
                
	syncCall10 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xe00141_Event_SetWIRStatus(WIRStatus356);
                
	syncCall11 = E_OK; //tan9sgh Rte_Call_EventStatus_EventParameter_DTC_0xe00142_Event_SetWIRStatus(WIRStatus357);

	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :SetWIRStatus_Adapt_Appl) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h" 

/*PROTECTED REGION ID(FileHeaderUserDefinedFunctions :DiagUT) ENABLED START */
/* Start of user defined functions  - Do not remove this comment */
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_FF_274_New_Data_Object(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetCSData_CS_FF_57601_Odometer_reading(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
	Buffer[0] = 0x11;
	Buffer[1] = 0x22;
	Buffer[2] = 0x33;
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_FF_53249_New_Data_Object(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Year(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Month(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Day(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Hour(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Minute(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Second(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_FF_55553_New_Data_Object(uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Fault_occurrence_counter(
		uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Fault_pending_counter(
		uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Aged_counter(
		uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"

#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
Std_ReturnType Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Ageing_counter(
		uint8* Buffer) {
	/* CalUT Integration Required. */
     Std_ReturnType retValue = RTE_E_OK;
    /*test code ,Application users need to change*/
    return retValue;
}
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"
/* End of user defined functions - Do not remove this comment */
/*PROTECTED REGION END */

