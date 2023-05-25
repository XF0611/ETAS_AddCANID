

#ifndef RBA_MEMLIB_PRV_H
#define RBA_MEMLIB_PRV_H

#include "rba_MemLib_Cfg.h"
#include "rba_MemLib_CheckProto.h"


/* Check for enabled prototypes in the Mem stack */
#if defined(RBA_MEMLIB_CFG_ENABLE_PROTOTYPE)
# if defined(__TASKING__)
/* Usage of Prototypes has been enabled for the Mem package, this configuration is limited for testing purposes only - IT IS NOT RELEASED FOR SERIES PRODUCTION! */
/* The tasking compiler does not support hash warning, because of that here is only a comment - STILL THIS IS ONLY PROTOTYPE! */
# else
/* For all other compilers generate a compiler warning */
/* MR12 RULE 20.13 VIOLATION: warning pre-processor comment only available for some compilers in others might lead to error or be ignored */
#  warning (EnablePrototype) Usage of Prototypes has been enabled for the Mem package, this configuration is limited for testing purposes only - IT IS NOT RELEASED FOR SERIES PRODUCTION!
# endif
#endif



#endif
