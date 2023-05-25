
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"

#if(DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 *******************************************************************************
 * Dcm_DslDsd_ConfirmRespPend : This function is called from DSD after getting
 *                              confirmation for the triggered force response
 *                               pending from the service.
 *
 * \param           dataSid_u8: Service Identifier for which the confirmation
 *								is received
 *                  status_u8: status of the transmitted response pending
 *
 * \retval          None
 * \seealso
 *
 *******************************************************************************
 */
void Dcm_Prv_ConfirmationRespPendForBootloader(Dcm_ConfirmationStatusType Status_u8)
{
	if(Status_u8 == DCM_RES_NEG_OK)
	{
		/* Waiting for confirm results for the triggered response pending? */
		if(Dcm_BootLoaderState_en == DCM_BOOT_WAIT)
		{
			if(Dcm_ProgConditions_st.StoreType == DCM_WARMREQUEST_TYPE)
			{
				/* Move the state to Store Protocol Information */
				Dcm_BootLoaderState_en = DCM_BOOT_STORE_WARMREQ;
			}
			else if(Dcm_ProgConditions_st.StoreType == DCM_WARMRESPONSE_TYPE)
			{
				/* Move the state to Store Protocol Information */
				Dcm_BootLoaderState_en = DCM_BOOT_STORE_WARMRESP;
			}
			else
			{
				/* Move the state to Store Protocol Information */
				Dcm_BootLoaderState_en = DCM_BOOT_STORE_WARMINIT;
			}
		}
		else
		{
		    if(Dcm_BootLoaderState_en ==DCM_BOOT_WAIT_FOR_RESET)
		    {
                if((Dcm_ProgConditions_st.StoreType == DCM_WARMREQUEST_TYPE)||
                        (Dcm_ProgConditions_st.StoreType == DCM_WARMRESPONSE_TYPE))
                {
                    /* Move the state to Store Protocol Information */
                    Dcm_BootLoaderState_en = DCM_BOOT_PERFORM_RESET;
                }
		    }
		}
	}
	else
	{   /* Boot Loader is activated */
		if((Dcm_BootLoaderState_en == DCM_BOOT_WAIT) || (Dcm_BootLoaderState_en ==DCM_BOOT_WAIT_FOR_RESET))
		{
			/* Reset Boot loader state */
			Dcm_BootLoaderState_en = DCM_BOOT_ERROR;
		}
	}
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif

