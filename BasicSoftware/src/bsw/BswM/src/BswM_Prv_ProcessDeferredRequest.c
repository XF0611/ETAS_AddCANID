

/*---------------------------------------------------------------------------------------*/
/*- Include files																		-*/
/*---------------------------------------------------------------------------------------*/

#include "BswM.h"
#if( defined(BSWM_RB_COM_ENABLED) && (BSWM_RB_COM_ENABLED==TRUE))
#include "Com.h"
#endif
#include "BswM_Prv.h"

/*---------------------------------------------------------------------------------------*/
/*- Definition of Global Functions														-*/
/*---------------------------------------------------------------------------------------*/


/*****************************************************************************************
 * Function name   :       BswM_Prv_ClearAllDeferredRequests
 * Description     :       The function clears the deferred request flags for all the MRPs.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :
 *****************************************************************************************/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

void BswM_Prv_ClearAllDeferredRequests (void)
{
#if (defined(BSWM_NO_OF_MRP_DEFERREDREQ) && (BSWM_NO_OF_MRP_DEFERREDREQ > 0))

    uint16 idxCount_u16 = 0;

    for (idxCount_u16 = 0; idxCount_u16 < BSWM_NO_OF_MRP_DEFERREDREQ; idxCount_u16++)
    {
        BswM_Prv_DeferredReqstInfo_ast[idxCount_u16].isDeferredReqstPresent_b = FALSE;
    }
#endif
    return;
}


/*****************************************************************************************
 * Function name   :       BswM_Prv_GetIndexFromDeferredRequest
 * Description     :       Retrieves the Index for the Matching MRP and Channel-ids (Source)
 * Parameter       :       BswM_Cfg_MRPType_ten, uint32, uint16*
 * Return value    :       void
 * Remarks         :
 *****************************************************************************************/

#if (defined(BSWM_NO_OF_MRP_DEFERREDREQ) && (BSWM_NO_OF_MRP_DEFERREDREQ > 0))
static boolean BswM_Prv_GetIndexFromDeferredRequest
(
        BswM_Cfg_MRPType_ten dataMRPType_en,
        uint32 dataChannelID_u32,
        uint16 * adrIndex_pu16
)
{
    uint16 idxCount_u16 = 0;
    boolean isValidIndex_b = FALSE;

    /* Perform Linear Search to find the Match for MRP Type and Channel-ID */
    for (idxCount_u16 = 0; idxCount_u16 < BSWM_NO_OF_MRP_DEFERREDREQ; idxCount_u16++)
    {
        if (BswM_Prv_DeferredReqstInfo_ast[idxCount_u16].dataMRPType_en == dataMRPType_en)
        {
            if (BswM_Prv_DeferredReqstInfo_ast[idxCount_u16].dataChannelID_u32 == dataChannelID_u32)
            {
                /* MRP and Channel-id Match found */
                *adrIndex_pu16 = idxCount_u16;
                isValidIndex_b = TRUE;
                break;
            }
        }
    }

    return isValidIndex_b;
}
#endif

/*****************************************************************************************
 * Function name   :       BswM_Prv_StoreDeferredRequest
 * Description     :       Stores the Deferred Request flag as TRUE for the MRP and its Channel.
 *                         Updates the new base address and number of associated rules.
 * Parameter       :       BswM_Cfg_MRPType_ten, uint32, uint16*, uint16
 * Return value    :       void
 * Remarks         :
 *****************************************************************************************/
void BswM_Prv_StoreDeferredRequest
(
        BswM_Cfg_MRPType_ten dataMRPType_en,
        uint32 dataChannelID_u32,
        const uint16 * adrAssociatedRules_pu16,
        uint16 nrAssociatedRules_u16
)
{
#if (defined(BSWM_NO_OF_MRP_DEFERREDREQ) && (BSWM_NO_OF_MRP_DEFERREDREQ > 0))

    uint16 nrIndex_u16 = 0;
    boolean isValidIndex_b = FALSE;

    /* Get the Index for the MRP and it's Channel-id (Source) */
    isValidIndex_b = BswM_Prv_GetIndexFromDeferredRequest(dataMRPType_en, dataChannelID_u32, &nrIndex_u16);

    /* Check if the Index is Valid */
    if (isValidIndex_b)
    {
        BswM_Prv_DeferredReqstInfo_ast[nrIndex_u16].adrAssociatedRules_pu16 = adrAssociatedRules_pu16;
        BswM_Prv_DeferredReqstInfo_ast[nrIndex_u16].nrAssociatedRules_u16 = nrAssociatedRules_u16;
        BswM_Prv_DeferredReqstInfo_ast[nrIndex_u16].isDeferredReqstPresent_b = TRUE;
    }
#else
    /* Nothing to do; no deferred request is configured */
    (void) dataMRPType_en;
    (void) dataChannelID_u32;
    (void) adrAssociatedRules_pu16;
    (void) nrAssociatedRules_u16;
#endif
    return;
}


#if (defined(BSWM_NO_OF_MRP_DEFERREDREQ) && (BSWM_NO_OF_MRP_DEFERREDREQ > 0))
#if (defined(BSWM_MAX_NO_OF_RULES) && (BSWM_MAX_NO_OF_RULES > 0))
/*****************************************************************************************
 * Function name   :       BswM_Prv_PopulateDeferredRules
 * Description     :       Process the Mode Requests that are configured as deffered request processing.
 * Parameter       :       uint16*
 * Return value    :       uint16 - number of unique rules for deferred processing
 * Remarks         :
 *****************************************************************************************/
static uint16 BswM_Prv_PopulateDeferredRules( uint16 * idxDeferredRules_pu16 )
{
    /* Declaration of Local variables */
    uint16 cntrdeferredIndex_u16 = 0;
    uint16 cntrRuleIndex_u16 = 0;
    uint16 ruleIndex_u16 = 0;
    uint16 noOfRuleAvailable_u16 = 0;

    for (cntrdeferredIndex_u16 = 0 ; cntrdeferredIndex_u16 < BSWM_NO_OF_MRP_DEFERREDREQ ; cntrdeferredIndex_u16++)
    {
        /* Check if the deferred request flag is set */
        if(FALSE != BswM_Prv_DeferredReqstInfo_ast[cntrdeferredIndex_u16].isDeferredReqstPresent_b)
        {
            for (cntrRuleIndex_u16 = 0; cntrRuleIndex_u16 < BswM_Prv_DeferredReqstInfo_ast[cntrdeferredIndex_u16].nrAssociatedRules_u16; cntrRuleIndex_u16++)
            {
                /* Get the rule index and update the RAM variable to proceed with rule evaluation */
                ruleIndex_u16 = BswM_Prv_DeferredReqstInfo_ast[cntrdeferredIndex_u16].adrAssociatedRules_pu16[cntrRuleIndex_u16];

                if(FALSE == BswM_Prv_DeferredRuleEvaluation_b[ruleIndex_u16])
                {
                    idxDeferredRules_pu16[noOfRuleAvailable_u16] = ruleIndex_u16;
                    BswM_Prv_DeferredRuleEvaluation_b[ruleIndex_u16] = TRUE;
                    noOfRuleAvailable_u16++;
                }
                else
                {
                    /* Do-nothing */
                }
            }

        }
        else
        {
            /* Do-nothing */
        }

    }

    /* Clear the Deferred request atructure  */
    BswM_Prv_ClearAllDeferredRequests();

    return noOfRuleAvailable_u16;

}
#endif /* BSWM_MAX_NO_OF_RULES */
#endif /* BSWM_NO_OF_MRP_DEFERREDREQ */

/*****************************************************************************************
 * Function name   :       BswM_Prv_ProcessDeferredReqst
 * Description     :       Process the Mode Requests that are configured as deferred request processing.
 * Parameter       :       void
 * Return value    :       void
 * Remarks         :
 *****************************************************************************************/
void BswM_Prv_ProcessDeferredReqst( void )
{
#if (defined(BSWM_NO_OF_MRP_DEFERREDREQ) && (BSWM_NO_OF_MRP_DEFERREDREQ > 0))
#if (defined(BSWM_MAX_NO_OF_RULES) && (BSWM_MAX_NO_OF_RULES > 0))
    const BswM_Cfg_RuleType_tst * dataRule_pst = BswM_Prv_adrSelectedConfig_pcst->dataModeArbitration_st.adrArbitrationRule_pst;
    uint16 noOfRuleAvailable_u16 = 0;
    uint16 cntrRuleIndex_u16 = 0;
    uint16 idxDeferredRules_au16[BSWM_MAX_NO_OF_RULES] = {0};

    /* Process requests that interrupted the processing of the previous mode request */
    if (FALSE != BswM_Prv_isReqstDelayed_b)
    {
        BswM_Prv_ProcessDelayedReqst();
    }

    noOfRuleAvailable_u16 = BswM_Prv_PopulateDeferredRules(&idxDeferredRules_au16[0]);

    if(noOfRuleAvailable_u16 > 0)
    {
        for (cntrRuleIndex_u16 = 0; cntrRuleIndex_u16 < noOfRuleAvailable_u16; cntrRuleIndex_u16++)
        {

            /* Call Evaulate rule to evaluate each rule with that index */
            BswM_Prv_Evaluate_Rule( &dataRule_pst[idxDeferredRules_au16[cntrRuleIndex_u16]], idxDeferredRules_au16[cntrRuleIndex_u16]);

            BswM_Prv_DeferredRuleEvaluation_b[idxDeferredRules_au16[cntrRuleIndex_u16]]= FALSE;
        }
#if ( BSWM_NO_OF_AC_IPDUGROUPSWITCH)
    /* IPdu Group Control action */
    BswM_Prv_CalcPduGrpSwt();
#endif /* (BSWM_NO_OF_AC_IPDUGROUPSWITCH) */

#if (defined(BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL) && (BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL > 0))
        if (FALSE != BswM_AC_DeadlineMonitoring_ExecFlag_b)
        {
            Com_ReceptionDMControl(BswM_AC_DeadlineMonitoring_IpduGroup);
            BswM_AC_DeadlineMonitoring_ExecFlag_b = FALSE;
        }
        else
        {
            /* Do-nothing */
        }
#endif /*BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL*/

    }
    else
    {
        /* Do-nothing */
    }
#endif /* BSWM_MAX_NO_OF_RULES */
#endif /* BSWM_NO_OF_MRP_DEFERREDREQ */

    return;
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

/**********************************************************************************************************************
 * End of source file
 **********************************************************************************************************************/
