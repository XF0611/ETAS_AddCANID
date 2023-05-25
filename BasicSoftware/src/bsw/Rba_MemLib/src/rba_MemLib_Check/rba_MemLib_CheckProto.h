


#ifndef RBA_MEMLIB_CHECKPROTO_H
#define RBA_MEMLIB_CHECKPROTO_H

#include "rba_MemLib_Cfg.h"


/***********************************************************************************************************************************/
/* Check for enabled prototype (RBA_MEMLIB_CFG_ENABLE_PROTOTYPE) */
/* <START USER EDITABLE AREA> */


#if defined(RBA_MEMLIB_CFG_ENABLE_PROTOTYPE)
/* Following #error line after this comment can be removed if a prototype is intentionally used.
 * Note however, that this and other modifications of the Mem-Stack are NOT ALLOWED FOR SERIES PRODUCTION PROJECTs as the release status of the SW is lost. */
#error (EnablePrototype) Usage of Prototypes has been enabled for the Mem package, this configuration is limited for testing purposes only - IT IS NOT RELEASED FOR SERIES PRODUCTION!
#endif


/* <END USER EDITABLE AREA> */
/***********************************************************************************************************************************/



















/* Integration integrity check */
#define RBA_MEMLIB_CHECKPROTO_MAGIC_NUMBER   (20200715uL)

#endif  /* multi-inclusion protection */
