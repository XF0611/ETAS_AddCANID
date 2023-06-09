
#ifndef BSWM_PRV_RL_H
#define BSWM_PRV_RL_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

extern void BswM_Prv_Arbitrate_Rule(uint16 nrOfAssociatedRules_u16,const uint16 * ruleRef_pu16);
extern void BswM_Prv_Evaluate_Rule(const BswM_Cfg_RuleType_tst * dataRule_pst,uint16 idxRule_u16);

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#endif /* BSWM_PRV_RL_H */
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/
