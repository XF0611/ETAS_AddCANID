

#ifndef RBA_MEMLIB_CFG_H
#define RBA_MEMLIB_CFG_H




/*********************************************************************************************************************
 * Macros/Defines
 *********************************************************************************************************************
*/

/* Series production configuration */

/* Activation of VECU handling */
#if defined(RBA_VECU_ENABLED)
#define RBA_MEMLIB_VECU_ENABLE          STD_ON
#endif


#define RBA_MEMLIB_VERSION_INFO         STD_OFF


#define RBA_MEMLIB_SW_MAJOR_VERSION                          (17)
#define RBA_MEMLIB_SW_MINOR_VERSION                          (0)
#define RBA_MEMLIB_SW_PATCH_VERSION                          (0)

#define RBA_MEMLIB_AR_RELEASE_MAJOR_VERSION                  (4)
#define RBA_MEMLIB_AR_RELEASE_MINOR_VERSION                  (2)
#define RBA_MEMLIB_AR_RELEASE_REVISION_VERSION               (2)

#endif
