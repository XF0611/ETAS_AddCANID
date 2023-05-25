/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */


/*********************************************************************************************************************
* CONTAINS THE ECUM-MEMMAP SEC CODE DECLARATIONS
*********************************************************************************************************************/
/* MR12 RULE 1.2 VIOLATION: The MemMap header shall have no include protection because of the concept of MemMap.
   The MemMap header could be included multiple times in a c file. */
#ifndef ECUM_CFG_MEMMAP_H
#define ECUM_CFG_MEMMAP_H
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
#if defined ECUM_START_SEC_VAR_CLEARED_8
    /*  8 bit */
    #define BSW_START_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_CLEARED_8
#elif defined ECUM_STOP_SEC_VAR_CLEARED_8
    #define BSW_STOP_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_CLEARED_8
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_CLEARED_16
    /* 16 bit */
    #define BSW_START_SEC_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_CLEARED_16
#elif defined ECUM_STOP_SEC_VAR_CLEARED_16
    #define BSW_STOP_SEC_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_CLEARED_16
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_CLEARED_32
    /* 32 bit */
    #define BSW_START_SEC_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_CLEARED_32
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_VAR_CLEARED_32
    #define BSW_STOP_SEC_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_CLEARED_32
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_CLEARED_BOOLEAN
    #define BSW_START_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_CLEARED_BOOLEAN
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_VAR_CLEARED_BOOLEAN
    #define BSW_STOP_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_CLEARED_BOOLEAN
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_INIT_16
    #define BSW_START_SEC_VAR_INIT_16
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_INIT_16
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_VAR_INIT_16
    #define BSW_STOP_SEC_VAR_INIT_16
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_INIT_16
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
    /* Unspecified size */
    #define BSW_START_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_POWER_ON_CLEARED_8
    #define BSW_START_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_POWER_ON_CLEARED_8
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_VAR_POWER_ON_CLEARED_8
    #define BSW_STOP_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_POWER_ON_CLEARED_8
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
    #define BSW_START_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_SAVED_ZONE0_BOOLEAN
     #define BSW_START_SEC_SAVED_ZONE_8
     #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_SAVED_ZONE0_BOOLEAN
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_VAR_SAVED_ZONE0_BOOLEAN
    #define BSW_STOP_SEC_SAVED_ZONE_8
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_SAVED_ZONE0_BOOLEAN
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
    #define BSW_START_SEC_SAVED_ZONE_UNSPECIFIED
     #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
     #define BSW_STOP_SEC_SAVED_ZONE_UNSPECIFIED
     #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_CONST_8
    /*  8 bit */
    #define BSW_START_SEC_CONST_8
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_CONST_8
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_CONST_8
    #define BSW_STOP_SEC_CONST_8
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_CONST_8
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_CONST_UNSPECIFIED
    #define BSW_START_SEC_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_CONST_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_CONST_UNSPECIFIED
    #define BSW_STOP_SEC_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_CONST_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_CONST_16
    /* 16 bit */
    #define BSW_START_SEC_CONST_16
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_CONST_16
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_CONST_16
    #define BSW_STOP_SEC_CONST_16
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_CONST_16
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_CONST_32
    /* 32 bit */
    #define BSW_START_SEC_CONST_32
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_CONST_32
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_CONST_32
    #define BSW_STOP_SEC_CONST_32
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_CONST_32
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_CONFIG_DATA_8
    #define BSW_START_SEC_CONST_8
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_CONFIG_DATA_8
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_CONFIG_DATA_8
    #define BSW_STOP_SEC_CONST_8
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_CONFIG_DATA_8
/*
**********************************************************************************************************************
*   Config_Data_postbuild
**********************************************************************************************************************
*/
#elif defined ECUM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
    #define BSW_START_SEC_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
    #define BSW_STOP_SEC_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_CALLOUT_CODE
     #define BSW_START_SEC_CODE
    #include "Bsw_MemMap.h"
    #undef ECUM_START_SEC_CALLOUT_CODE
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_CALLOUT_CODE
     #define BSW_STOP_SEC_CODE
    #include "Bsw_MemMap.h"
    #undef ECUM_STOP_SEC_CALLOUT_CODE
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_VAR_INIT_8
      #define BSW_START_SEC_VAR_INIT_8
      #include "Bsw_MemMap.h"
      #undef ECUM_START_SEC_VAR_INIT_8
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_VAR_INIT_8
      #define BSW_STOP_SEC_VAR_INIT_8
      #include "Bsw_MemMap.h"
      #undef ECUM_STOP_SEC_VAR_INIT_8
/*
***************************************************************
*/
#elif defined ECUM_START_SEC_CODE
   #define BSW_START_SEC_CODE
   #include "Bsw_MemMap.h"
   #undef ECUM_START_SEC_CODE
/*
***************************************************************
*/
#elif defined ECUM_STOP_SEC_CODE
   #define BSW_STOP_SEC_CODE
   #include "Bsw_MemMap.h"
   #undef ECUM_STOP_SEC_CODE
/*
***************************************************************
*/
#elif defined EcuM_START_SEC_CODE
   #define BSW_START_SEC_CODE
   #include "Bsw_MemMap.h"
   #undef EcuM_START_SEC_CODE
/*
***************************************************************
*/
#elif defined EcuM_STOP_SEC_CODE
   #define BSW_STOP_SEC_CODE
   #include "Bsw_MemMap.h"
   #undef EcuM_STOP_SEC_CODE
/*
***************************************************************
*/
#else
    #error "EcuM_Cfg_MemMap.h: Unknown MemMap #define"
#endif

#undef ECUM_CFG_MEMMAP_H
#endif  /* end of #ifndef ECUM_CFG_MEMMAP_H */

 /**********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
