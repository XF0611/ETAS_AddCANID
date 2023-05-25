/*
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 ************************************************************************************************
 * Component : Rtm_Cfg_SchM.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef RTM_CFG_SCHM_H
#define RTM_CFG_SCHM_H

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */
#include "SchM_Default.h"


/*
 ***********************************************************************************************************************
 * Preprocessor defines
 ***********************************************************************************************************************
 */

/* Note on exclusive area "CtrlMeasure":
 * This lock secures the controlled measures from measurements that could take place in a preemption
 * It must not lock other cores and should disable all interrupts on local core
 * If no CAT 1 interrupts will be measured it would be enough to only lock on OS level
 * It is likely that it is uses from different cores at the same time
 */
# define SchM_Enter_Rtm_CtrlMeasure() SuspendOSInterrupts()
# define  SchM_Exit_Rtm_CtrlMeasure()  ResumeOSInterrupts()

/* Note on exclusive area "CtrlTrigger":
 * This lock secures the controlled sample generation from measurements that could take place in a preemption
 * It must not lock other cores and should disable all interrupts on local core
 * If no CAT 1 interrupts will be measured it would be enough to only lock on OS level
 * This is for sure not used nested on the same core and only used from one core
 */
# define SchM_Enter_Rtm_CtrlTrigger() SuspendOSInterrupts()
# define  SchM_Exit_Rtm_CtrlTrigger()  ResumeOSInterrupts()

/* Note on exclusive area "MTMeasure":
 * This lock secures the MT measures from measurements that could take place in a preemption
 * It must not lock other cores and should disable all interrupts on local core
 * If no CAT 1 interrupts will be measured it would be enough to only lock on OS level
 * It is likely that it is uses from different cores at the same time
 */
# define SchM_Enter_Rtm_MTMeasure() SuspendOSInterrupts()
# define  SchM_Exit_Rtm_MTMeasure()  ResumeOSInterrupts()

/* Which algorithm should be used to calculate load per 64k
 * Count leading zeros will use a 32 bit divide but the core should provide a special assembler command for this
 * Default uses 64 bit divide */
# define RTM_USE_COUNT_LEADING_ZEROS_ALGO STD_ON

/* Generic "CountLeadingZero" algo, not used if (RTM_USE_COUNT_LEADING_ZEROS_ALGO == STD_OFF) */
LOCAL_INLINE uint32 Rtm_CountLeadingZero32 (uint32 u)
{
  uint32 i = 0;
  if(u!=0)
  {
    while ( (u & 0x80000000ul) == 0)
    {
      u <<= 1;
      i++;
    }
  }
  return i;
}
/* Generic no operation */
# define RTM_NOP() do { volatile uint32 nop = 0; } while (0)


#endif /* RTM_CFG_SCHM_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

