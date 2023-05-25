

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Fee_Idx_Cfg.h"
/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
#if(defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

#include "Fee_Prv.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

/* Struct used for measuring the effort spent during one main function call */
#define FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"
Fee_Prv_LibEffortMeasure_tst Fee_Prv_LibEffortMeasure_st;
#define FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"

/* Array which weights the actions */
#define FEE_START_SEC_CONST_8
#include "Fee_MemMap.h"
uint8 const Fee_Prv_LibEffortWeights_au8[FEE_PRV_LIMIT_MAX_E] =
{
    0,  /* FEE_PRV_LIMIT_CRCINRAM_CPYRAM_E */
    1,  /* FEE_PRV_LIMIT_CRCINFLS_E        */
    5,  /* FEE_PRV_LIMIT_HDR_E             */
    10  /* FEE_PRV_LIMIT_CACHEREORG_E      */
};
#define FEE_STOP_SEC_CONST_8
#include "Fee_MemMap.h"


#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
*/
/*
 * \brief init function for the Lib unit resetting all variables
 */
void Fee_Prv_LibInit(void)
{
    Fee_Prv_LibEffortMeasure_st.effortCtr_u32 = 0u;
    Fee_Prv_LibEffortMeasure_st.enabled_b     = FALSE;

    return;
}

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
#endif
