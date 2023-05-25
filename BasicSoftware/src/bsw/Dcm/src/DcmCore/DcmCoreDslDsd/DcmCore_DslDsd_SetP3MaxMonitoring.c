
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**
 **************************************************************************************************
 * Dcm_SetP3MaxMonitoring : Function called by service to enable the P3 monitoring
 *
 * \param           active TRUE : P3 monitoring required.
 *                         FALSE : P3 monitoring not required.
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_SetP3MaxMonitoring (boolean active)
{
    /* Assign the bit depending on the application */
    Dcm_DsldGlobal_st.flgMonitorP3timer_b = active;

#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)
    /*If the Return value is TRUE and not active*/
   if(((DCM_IS_KWPPROT_ACTIVE() != FALSE) && (active == FALSE)))
    {
        /* Stop the communication if application wants to stop P3 monitoring */

        Dcm_DsldGlobal_st.flgCommActive_b = FALSE;
    }
#endif

}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
