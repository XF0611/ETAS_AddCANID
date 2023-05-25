
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "rba_MemLib.h"

/*
 * This .c file contains asserts that are executed on compile time.
 * This file shall not contain any functionality, it shall only assure a correct configuration passes a build.
 */

/*
 **********************************************************************************************************************
 * Asserts
 **********************************************************************************************************************
 */

/**
 * \brief
 * Assure the build aborts in case additional test code has been enabled, but prototypes are disabled
 *
 * \param TESTCD_NVM_ENABLED
 * In case this define is defined to STD_ON then test code inside the NvM is enabled
 * \param RBA_MEMLIB_CFG_ENABLE_PROTOTYPE
 * In case this define is defined then prototypes have been enabled for the Mem stack
 */
#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON) && (!defined(RBA_MEMLIB_CFG_ENABLE_PROTOTYPE)))
# error The test define TESTCD_NVM_ENABLED has been enabled but prototypes are not enabled. \
        Please enable prototypes in rba_MemLib. For details please see docu of rba_MemLib.
#endif
