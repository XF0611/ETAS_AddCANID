
#ifndef WDGIF_TYPES_H
#define WDGIF_TYPES_H

#include "Std_Types.h"

/*---- Version identification - --------------------------------------------*/

  /*refer to WDGIF023*/
#define WDGIF_TYPES_MAJOR_VERSION      (3U)
#define WDGIF_TYPES_MINOR_VERSION      (0U)
#define WDGIF_TYPES_PATCH_VERSION      (0U)

/*---- Switch between external and forward declaration ---------------------*/

/*---- API Type definitions ------------------------------------------------*/

typedef enum
{
    WDGIF_OFF_MODE = 0,
    WDGIF_SLOW_MODE,
    WDGIF_FAST_MODE,
    WDGIF_NO_MODE
} WdgIf_ModeType; /*refer to WDGIF016*/

/*---- Include of configuration (public section) ---------------------------*/

/* #include "WdgIf_Cfg.h" */
/*--------------------------------------------------------------------------*/
#endif  /* End of WdgIf_Types.h */
