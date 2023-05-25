

/*---------------------------------------------------------------------------------------*/
/*- Include files                                                                       -*/
/*---------------------------------------------------------------------------------------*/
#include "BswM.h"
#include "BswM_Prv.h"
#include "BswM_PBcfg.h"

/*****************************************************************************************
* Function name :   void BswM_Prv_Evaluate_ActionList(const BswM_Cfg_ActionListType_tst * dataActionList_pst)
* Description   :   This function is called by BswM_Prv_Arbitrate_Rule to execute the true/false Actionlist.
* Parameter dataActionList_st   :   This parameter pointer to structure that holds all the information needed for executing Actionlist.
* Return value  :   None
* Remarks       :
*****************************************************************************************/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

void BswM_Prv_Evaluate_ActionList
(
    const BswM_Cfg_ActionListType_tst * dataActionList_pst
)
{
/* Check if any Rule is configured in the Project? */
#ifdef BSWM_MAX_NO_OF_RULES

    boolean isExitOnAbortFlag_b = FALSE;
    uint16 hasIndex_u16 = 0;
    Std_ReturnType isActionRet_u8 = E_NOT_OK;
    const BswM_Cfg_ActionListItemType_tst * dataActionListItem_pst = NULL_PTR ;

    /* Loop through all the members of ActionList */
    for (hasIndex_u16 = 0; hasIndex_u16 < dataActionList_pst->nrActionListItems_u8; hasIndex_u16++)
    {
          #if (defined(BSWM_NO_OF_AC_BSWMTIMERCONTROL) && (BSWM_NO_OF_AC_BSWMTIMERCONTROL > 0))
          if(TRUE != BswM_Prv_flgTimerActionState_b)
          #endif
          {
              dataActionListItem_pst = &dataActionList_pst->adrActionListItem_pst[hasIndex_u16];

         /* Check if the member of ActionList is refrence to another ACIONLIST */
         if (dataActionListItem_pst->dataActionListItemType_en == BSWM_ACTIONLIST)
         {
#if (defined(BSWM_TOTAL_NO_OF_ACTIONLIST) && (BSWM_TOTAL_NO_OF_ACTIONLIST > 0))
             if (FALSE == BswM_isMultipleActionListTriggered_ab[dataActionList_pst->idxActionListnum])
             {


                 /*MR12 RULE 11.5 VIOLATION: since only the configuration will determine whether the action list item is
                 *another action list or not, it is assigned to void pointer.It is low prio because it is typecast to
                 *the corresponding action list or action item type after first checking for the same in this function.*/
                 /*MR12 RULE 17.2 VIOLATION: if the action item is a reference to another action list then this function
                 *needs to be called recursively. Currently the action list structure may contain only up to 7 hierarchic
                 *levels so it will not block code execution*/
                 BswM_Prv_Evaluate_ActionList((const BswM_Cfg_ActionListType_tst *)(dataActionListItem_pst->adrActionListItemRef_pv));
				 /*check if all the action list items have been executed, only then set the flag for actionlist triggered to true*/
                 if (dataActionList_pst->nrActionListItems_u8 == (hasIndex_u16 +1))
                 {
                 BswM_isMultipleActionListTriggered_ab[dataActionList_pst->idxActionListnum] = TRUE;
                 }
             }
             else
             {
               /* Do nothing */
             }
#endif
         }

         /* The member of ActionList is reference to an action. Check whether the actionlist of which this action is a
          *  member, was already executed before. If yes, then dont call BswM_Prv_Evaluate_Action and dont execute the action*/
         else
#if (defined(BSWM_TOTAL_NO_OF_ACTIONLIST) && (BSWM_TOTAL_NO_OF_ACTIONLIST > 0))
             if (FALSE == BswM_isMultipleActionListTriggered_ab[dataActionList_pst->idxActionListnum])
#endif
             {
              isActionRet_u8 = BswM_Prv_Evaluate_Action(dataActionListItem_pst->adrActionListItemRef_pv, dataActionListItem_pst->dataActionListItemType_en);
              /* Check if the action returns E_NOT_OK and is configured for AbortOnFail as TRUE */
              if ((dataActionListItem_pst->isActionListAbortOnFail_b) && (isActionRet_u8 == E_NOT_OK))
              {
                   isExitOnAbortFlag_b = TRUE;
              }

              if (dataActionList_pst->nrActionListItems_u8 == (hasIndex_u16 + 1))
              {
#if (defined(BSWM_TOTAL_NO_OF_ACTIONLIST) && (BSWM_TOTAL_NO_OF_ACTIONLIST > 0))
                  BswM_isMultipleActionListTriggered_ab[dataActionList_pst->idxActionListnum] = TRUE;
#endif
              }
              else
              {
                  /* Do nothing */
              }

         }
#if (defined(BSWM_TOTAL_NO_OF_ACTIONLIST) && (BSWM_TOTAL_NO_OF_ACTIONLIST > 0))
             else
             {
                 //nothing to be executed
             }
#endif
        /* Check if break from looping to next member of ActionList*/
        if(isExitOnAbortFlag_b)
        {
             break;
        }
      }
#if (defined(BSWM_NO_OF_AC_BSWMTIMERCONTROL) && (BSWM_NO_OF_AC_BSWMTIMERCONTROL > 0))
	  else
	  {
	  /* Do nothing */
	  }
#endif
    }


#else
    (void)dataActionList_pst;
#endif
    return;
}
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/**********************************************************************************************************************
 *
 **********************************************************************************************************************/
