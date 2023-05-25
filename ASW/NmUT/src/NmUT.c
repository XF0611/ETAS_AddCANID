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
 * Component: /ETAS_BIP/NmUT
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.2.1
 * Author: TAO9SGH
 * Date : Wed Feb 23 09:53:10 2022
 ****************************************************************************/

#include "Rte_NmUT.h"

/*PROTECTED REGION ID(FileHeaderUserDefinedIncludes :RE_NmUT_func) ENABLED START */
/* Start of user defined includes  - Do not remove this comment */
#include "ComStack_Types.h"
#include "Rte_ComM_Type.h"
#include "Nm.h"
#include "NmUT.h"
/* End of user defined includes - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedConstants :RE_NmUT_func) ENABLED START */
/* Start of user defined constant definitions - Do not remove this comment */
/* End of user defined constant definitions - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedVariables :RE_NmUT_func) ENABLED START */
/* Start of user variable defintions - Do not remove this comment  */
#define NmUT_START_SEC_VAR_CLEARED_UNSPECIFIED  
#include "NmUT_MemMap.h"
Nm_Test_t Nm_Test[NM_NUMBER_OF_CHANNELS];
#define NmUT_STOP_SEC_VAR_CLEARED_UNSPECIFIED  
#include "NmUT_MemMap.h"


#define NmUT_START_SEC_VAR_CLEARED_8
#include "NmUT_MemMap.h"
uint8 shutdown_b;
#define NmUT_STOP_SEC_VAR_CLEARED_8
#include "NmUT_MemMap.H"
/* End of user variable defintions - Do not remove this comment  */
/*PROTECTED REGION END */
#define NmUT_START_SEC_CODE                   
#include "NmUT_MemMap.h"
FUNC (void, NmUT_CODE) RE_NmUT_func/* return value & FctID */
(
		void
)
{

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :RE_NmUT_func) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	char index_chanel;
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	// Std_ReturnType retValue = RTE_E_OK;
	/*  -------------------------------------- Data Read -----------------------------------------  */

	/*  -------------------------------------- Server Call Point  --------------------------------  */



	/*  -------------------------------------- CDATA ---------------------------------------------  */

	/*  -------------------------------------- Data Write ----------------------------------------  */


	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :RE_NmUT_func) ENABLED START */
	/* Start of user code - Do not remove this comment */
	for(index_chanel = 0; index_chanel < NM_NUMBER_OF_CHANNELS; index_chanel++)
    {
        /**
         * @brief ETAS BIP Integration Guide: 5.3.1	Mode Handling
         * Customer Integration Required.
         * Please see the integration guide section for further details.
         */

        if(Nm_Test[index_chanel].request_b)
        {
            /* S/R to request COMM_FULL_COMM mode by ExeMgrUT */
            Rte_Write_PP_NmState_uint8(APP_READY_FOR_NM_REQUEST);
            Nm_Test[index_chanel].request_b = 0;
        }
        else
        {
            Rte_Write_PP_NmState_uint8(APP_NM_INIT);
        }
        if(Nm_Test[index_chanel].release_b)
        {
            /* S/R to request COMM_NO_COMM mode by ExeMgrUT */
            Rte_Write_PP_NmState_uint8(APP_READY_FOR_NM_RELEASE);
            Nm_Test[index_chanel].release_b = 0;
        }
        else
        {
            Rte_Write_PP_NmState_uint8(APP_NM_INIT);
        }

        if(Nm_Test[index_chanel].IsNmReceived)
        {
            Nm_GetUserData(index_chanel,Nm_Test[index_chanel].nm_rx_data);
            Nm_Test[index_chanel].IsNmReceived = 0;
        }
    }
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}
#define NmUT_STOP_SEC_CODE  
#include "NmUT_MemMap.h" 

/*PROTECTED REGION ID(FileHeaderUserDefinedFunctions :NmUT) ENABLED START */
/* Start of user defined functions  - Do not remove this comment */
#define NmUT_START_SEC_CODE                   
#include "NmUT_MemMap.h"
FUNC(void, NM_CODE) Nm_RxIndicationCallback(CONST(NetworkHandleType, AUTOMATIC)   NetworkHandle )
{
    if (NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        Nm_Test[NetworkHandle].IsNmReceived = TRUE;
    }
}

FUNC(void, NM_CODE) Nm_StateChangeIndication(CONST(NetworkHandleType, AUTOMATIC)  NetworkHandle,
                                               CONST(Nm_StateType, AUTOMATIC)   nmPreviousState,
                                               CONST(Nm_StateType, AUTOMATIC)   nmCurrentState)
{

    if (NetworkHandle<NM_NUMBER_OF_CHANNELS)
    {
        Nm_Test[NetworkHandle].nm_PreviousState = nmPreviousState;
        Nm_Test[NetworkHandle].nm_CurrentState = nmCurrentState;
        if ((Nm_Test[NetworkHandle].nm_PreviousState == NM_STATE_PREPARE_BUS_SLEEP) && 
            (Nm_Test[NetworkHandle].nm_CurrentState == NM_STATE_BUS_SLEEP))
        {
            shutdown_b = 1;
            /* S/R to trigger mode(APP_MODE = POST_RUN) notification by ExeMgrUT */
            Rte_Write_PP_NmState_uint8(APP_NM_BUS_SLEEP);
        }
        if (Nm_Test[NetworkHandle].nm_CurrentState == NM_STATE_REPEAT_MESSAGE)
        {
            Nm_Test[NetworkHandle].nm_tx_data[0] = 0xAA;
            Rte_Write_PP_NM_Tx_Sig_NM_Tx_0x600_NM_Tx_Sig_NM_Tx_0x600(Nm_Test[NetworkHandle].nm_tx_data[0]);
            // Nm_SetUserData(NetworkHandle,Nm_Test[NetworkHandle].nm_tx_data);
        }

        if (Nm_Test[NetworkHandle].nm_CurrentState == NM_STATE_PREPARE_BUS_SLEEP)
        {
            Nm_Test[NetworkHandle].nm_tx_data[0] = 0xBB;
            Rte_Write_PP_NM_Tx_Sig_NM_Tx_0x600_NM_Tx_Sig_NM_Tx_0x600(Nm_Test[NetworkHandle].nm_tx_data[0]);
            // Nm_SetUserData(NetworkHandle,Nm_Test[NetworkHandle].nm_tx_data);
        }
        if (Nm_Test[NetworkHandle].nm_CurrentState == NM_STATE_NORMAL_OPERATION)
        {
            Nm_Test[NetworkHandle].nm_tx_data[0] = 0xCC;
            Rte_Write_PP_NM_Tx_Sig_NM_Tx_0x600_NM_Tx_Sig_NM_Tx_0x600(Nm_Test[NetworkHandle].nm_tx_data[0]);
            // Nm_SetUserData(NetworkHandle,Nm_Test[NetworkHandle].nm_tx_data);
        }

    }
    else
    {
        /*do nothing*/
    }
}

FUNC(void, NM_CODE) TestNm_RemoteSleepCancellation(CONST(NetworkHandleType, AUTOMATIC)   NetworkHandle)
{

}

FUNC(void, NM_CODE) TestNm_RemoteSleepIndication(CONST(NetworkHandleType, AUTOMATIC)   NetworkHandle)
{

}
#define NmUT_STOP_SEC_CODE  
#include "NmUT_MemMap.h" 
/* End of user defined functions - Do not remove this comment */
/*PROTECTED REGION END */

