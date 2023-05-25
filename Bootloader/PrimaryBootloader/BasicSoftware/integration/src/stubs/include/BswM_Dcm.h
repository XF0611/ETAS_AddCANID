#ifndef BSWM_DCM_H
#define BSWM_DCM_H

#include "BswM.h"

#include "Dcm.h"


/* From BswM Ar4.1.1 */
/* ! \brief This function is called by the DCM in order to report an updated application. */
extern FUNC(void, BSWM_CODE) BswM_Dcm_ApplicationUpdated ( void );
extern void BswM_Dcm_CommunicationMode_CurrentState( NetworkHandleType Network, Dcm_CommunicationModeType Mode );


#endif
