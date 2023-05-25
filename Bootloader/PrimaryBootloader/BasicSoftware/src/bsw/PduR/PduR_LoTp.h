
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: PduR  / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#ifndef PDUR_LOTP_H
#define PDUR_LOTP_H

#include "PduR_Prv.h"
#include "PduR_Cfg.h"

#include "PduR.h"

#include "PduR_CanTp.h"
#include "PduR_CanTp_Lo.h"

/* Service IDs */

#define PDUR_SID_CANTPTPRXBUF  (11)    /* PduR_CanTpCopyRxData */
#define PDUR_SID_CANTPTPRXIND  (12)    /* PduR_CanTpRxIndication */
#define PDUR_SID_CANTPTPSTARTOFRECEP  (13)    /* PduR_CanTpStartOfReception */
#define PDUR_SID_CANTPTPTXBUF  (14)    /* PduR_CanTpCopyTxData */
#define PDUR_SID_CANTPTPTXCONF (15)    /* PduR_CanTpTxConfirmation */

#endif /* PDUR_LOTP_H */

