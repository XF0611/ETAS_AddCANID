
/*****************************************************************************************************
 *                                   Includes
 *****************************************************************************************************/
#include "EcuM.h" /*BSW_HeaderInc_002*/
#include "EcuM_Cfg_Startup.h"

/*****************************************************************************************************
 *                                   Post Build  Constant - Definition
 *****************************************************************************************************/

/***************************************************************************************************
 Container Name    : EcuM_ConfigType
 Description       : Contains all post build parameters.
 ***************************************************************************************************/
#define ECUM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "EcuM_MemMap.h"

const EcuM_ConfigType EcuM_Config = { OSDEFAULTAPPMODE,

#if (ECUM_STARTUP_DURATION == TRUE) /*will activate the Run time measurement*/
		{ ECUM_SHUTDOWN_TARGET_OFF, 0, 0, 0x00 }, /* DefaultShutdownTarget */
#else
    {          ECUM_SHUTDOWN_TARGET_OFF    ,        0    ,        0    }, /* DefaultShutdownTarget */
#endif//ECUM_STARTUP_DURATION

		{ NULL_PTR/*   Det    */, NULL_PTR/*   Dem    */,
				&Mcu_Config/*   Mcu    */, &Gpt_Config/*   Gpt    */,
				&Port_Config/*   Port    */, &Dio_Config/*   Dio    */,
				&Fls_Config/*   Fls    */, &Can_Config/*   Can    */,
				&Wdg_Config/*   Wdg    */, &Uart_Config/*   Uart    */,

				&BswM_Config /*BswM*/
		}, &EcuM_Cfg_dataWkupPNCRef_cast[0], /* Pointer which refers to ComMPNC references associated with wakeups */
		{ 0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04, 0xE9, 0x80, 0x09,
				0x98, 0xEC, 0xF8, 0x42, 0x7E } };

/* The order in the array is generated according to the alphabetically sorted EcuM configuration. */
const EcuM_ConfigType *const EcuM_EcuMConfigurations_cpcast[ECUM_NO_OF_ECUMCONFIGSETS] =
		{

		&EcuM_Config };

#define ECUM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "EcuM_MemMap.h"

