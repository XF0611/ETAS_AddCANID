/*
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 ************************************************************************************************
 * Component : ExtWdg_MemMap.h
 * Date      : Feb 21, 2020
 * Version   : 1.0
 * Description  : This module provides ExtWdg MemMap.
 ***********************************************************************************************
*/
#if defined 	EXTWDG_START_SEC_CODE
    #undef		EXTWDG_START_SEC_CODE
    #define  	BSW_START_SEC_CODE
    #include "Bsw_MemMap.h"
	#undef		BSW_START_SEC_CODE
#elif defined 	EXTWDG_STOP_SEC_CODE
    #undef		EXTWDG_STOP_SEC_CODE
    #define		BSW_STOP_SEC_CODE
    #include "Bsw_MemMap.h"
	#undef		BSW_STOP_SEC_CODE

#elif defined EXTWDG_START_SEC_VAR_CLEARED_BOOLEAN
    #undef EXTWDG_START_SEC_VAR_CLEARED_BOOLEAN
    #define BSW_START_SEC_VAR_CLEARED_BOOLEAN
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_VAR_CLEARED_BOOLEAN
#elif defined EXTWDG_STOP_SEC_VAR_CLEARED_BOOLEAN
    #undef EXTWDG_STOP_SEC_VAR_CLEARED_BOOLEAN
    #define BSW_STOP_SEC_VAR_CLEARED_BOOLEAN
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_VAR_CLEARED_BOOLEAN
#elif defined EXTWDG_START_SEC_VAR_CLEARED_8
    #undef EXTWDG_START_SEC_VAR_CLEARED_8
    #define BSW_START_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_VAR_CLEARED_8
#elif defined EXTWDG_STOP_SEC_VAR_CLEARED_8
    #undef EXTWDG_STOP_SEC_VAR_CLEARED_8
    #define BSW_STOP_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_VAR_CLEARED_8
#elif defined EXTWDG_START_SEC_VAR_CLEARED_16
    #undef EXTWDG_START_SEC_VAR_CLEARED_16
    #define BSW_START_SEC_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_VAR_CLEARED_16
#elif defined EXTWDG_STOP_SEC_VAR_CLEARED_16
    #undef EXTWDG_STOP_SEC_VAR_CLEARED_16
    #define BSW_STOP_SEC_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_VAR_CLEARED_16
#elif defined EXTWDG_START_SEC_VAR_CLEARED_32
    #undef EXTWDG_START_SEC_VAR_CLEARED_32
    #define BSW_START_SEC_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_VAR_CLEARED_32
#elif defined EXTWDG_STOP_SEC_VAR_CLEARED_32
    #undef EXTWDG_STOP_SEC_VAR_CLEARED_32
    #define BSW_STOP_SEC_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_VAR_CLEARED_32
#elif defined EXTWDG_START_SEC_VAR_CLEARED_UNSPECIFIED
    #undef EXTWDG_START_SEC_VAR_CLEARED_UNSPECIFIED
    #define BSW_START_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_VAR_CLEARED_UNSPECIFIED
#elif defined EXTWDG_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #undef EXTWDG_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#elif defined EXTWDG_START_SEC_VAR_INIT_16
    #undef EXTWDG_START_SEC_VAR_INIT_16
    #define BSW_START_SEC_VAR_INIT_16
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_VAR_INIT_16
#elif defined EXTWDG_STOP_SEC_VAR_INIT_16
    #undef EXTWDG_STOP_SEC_VAR_INIT_16
    #define BSW_STOP_SEC_VAR_INIT_16
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_VAR_INIT_16
#endif /* INTEGRATION_MEMMAP_BSW_EXTWDG_MEMMAP_H_ */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
