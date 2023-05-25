/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 


#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Seca_Inf.h"
#include "Rte_Dcm.h"

#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF) )


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 *  @ingroup DCM_TPL
 *  DcmAppl_DcmCheckSeed :-\n
 *  Request to application for validation of seed value.This function checks whether the seed passed is Valid or not.\n
 *  ( More Information:- The API DcmAppl_DcmGetSeed() gives the seed from the application.
 *  To validate whether the seed sent by the application is valid or not this API is called again with the seed sent from the application as the input.)
 *
 *
 *  @param[in]           SeedLen :         Seed length as configured for access type\n
 *  @param[in]           Seed    :         Pointer for provided seed\n
 *  @param[out]          None
 *  @retval              DCM_E_OK  :       Seed is valid\n
 *  @retval              DCM_E_SEED_NOK :  Seed Invalid\n
 */
Dcm_StatusType DcmAppl_DcmCheckSeed(
                                                    uint32 SeedLen,
                                                    const uint8 * Seed
                                                  )
{
    uint8 ret_val;    /* Local varibale for return value */
    uint8 ctIndex;

    ctIndex = 0;   /* Initialisation */

    ret_val = E_OK;   /* Initialisation */

    /* If Seed is not equal to 0x0000.. or 0xFFFF.. then Seed is a valid seed */
    if((Seed[0] == 0x00) || (Seed[0] == 0xFF))
    {
        for(ctIndex = 1; ctIndex < SeedLen; ctIndex++)
        {
            if(Seed[ctIndex-1] != Seed[ctIndex])
            {
                break;
            }
        }
    }

    if(ctIndex >= SeedLen)
    {
        ret_val = DCM_E_SEED_NOK; /* seed is invalid */
    }

    return (ret_val);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif /*(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)*/
