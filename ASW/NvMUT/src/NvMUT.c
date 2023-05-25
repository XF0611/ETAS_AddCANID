/* *****************************************************************************
 * BEGIN: Banner
 *-----------------------------------------------------------------------------
 *                                 ETAS GmbH
 *                      D-70469 Stuttgart, Borsigstr. 14
 *-----------------------------------------------------------------------------
 *    Administrative Information (automatically filled in by ISOLAR)         
 *-----------------------------------------------------------------------------
 * Name: 
 * Description:
 * Version: 1.0
 *-----------------------------------------------------------------------------
 * END: Banner
 ******************************************************************************

 * Project : ETAS_BIP_PT
 * Component: /ETAS_BIP/NvMUT
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool Version: ISOLAR-A/B 9.1
 * Author: TAN9SGH
 * Date : Mon Aug 23 13:38:49 2021
 ****************************************************************************/

/*PROTECTED REGION ID(FileHeaderUserDefinedIncludes :RE_ShutdownBlock_JobFinished) ENABLED START */
/* Start of user defined includes  - Do not remove this comment */

#include "Rte_NvMUT.h"
#include "NvMUT.h"
#include "Os.h"
#include "Rtm.h"
#include "rba_BswSrv.h"
#include "ExeMgrUT.h"

/* End of user defined includes - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedConstants :RE_ShutdownBlock_JobFinished) ENABLED START */
/* Start of user defined constant definitions - Do not remove this comment */
/* End of user defined constant definitions - Do not remove this comment */
/*PROTECTED REGION END */



/*PROTECTED REGION ID(FileHeaderUserDefinedVariables :RE_ShutdownBlock_JobFinished) ENABLED START */
/* Start of user variable defintions - Do not remove this comment  */

#define NvMUT_START_SEC_VAR_CLEARED_UNSPECIFIED  
#include "NvMUT_MemMap.h"
NvM_Test_t NvM_Test = SWC_ZERO;
Req_reset_t Req_reset = RESET_NONE;
EcuM_ShutdownCauseType resetReason = 0;
EcuM_ShutdownModeType resetGroup = 0;
#define NvMUT_STOP_SEC_VAR_CLEARED_UNSPECIFIED  
#include "NvMUT_MemMap.h"

#define NvMUT_START_SEC_VAR_INIT_8
#include "NvMUT_MemMap.h"
extern Testing_t StateMachine_Test;
uint8 Read_Block_Cyclic_u8[CYCLIC_BLOCK_LENGHT] = {0};
uint8 Write_Block_Cyclic_u8[CYCLIC_BLOCK_LENGHT] = {0,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
uint8 Read_Block_ShutDown_u8[SHUTDOWN_BLOCK_LENGHT] = {0};
uint8 Write_Block_ShutDown_u8[SHUTDOWN_BLOCK_LENGHT] = {0,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC};
uint8 control_execution = 0;
uint8 state_c, state_cp = 0;
#define NvMUT_STOP_SEC_VAR_INIT_8
#include "NvMUT_MemMap.h"

#define NvMUT_START_SEC_VAR_INIT_32
#include "NvMUT_MemMap.h"
uint32 Read_Block_Immediate_u32[IMMEDIATE_BLOCK_SIZE] = {0};
uint32 Write_Block_Immediate_u32[IMMEDIATE_BLOCK_SIZE] = {0,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB};
uint32 FWrite_Block_Immediate_u32[IMMEDIATE_BLOCK_SIZE] = {0,0xFFBB,0xBBBB,0xFFBB,0xFFBB,0xFFBB,0xFFBB,0xFFBB};
uint32 start_t, stop_t, time_us = 0;
#define NvMUT_STOP_SEC_VAR_INIT_32
#include "NvMUT_MemMap.h"



/* End of user variable defintions - Do not remove this comment  */
/*PROTECTED REGION END */

#define NvMUT_START_SEC_CODE                   
#include "NvMUT_MemMap.h"
FUNC (void , NvMUT_CODE) RE_ShutdownBlock_JobFinished/* return value & FctID */
(
        VAR(uint8, AUTOMATIC) ServiceId,
        VAR(NvM_RequestResultType, AUTOMATIC) JobResult
)
{

}

#define NvMUT_STOP_SEC_CODE                       
#include "NvMUT_MemMap.h"
#define NvMUT_START_SEC_CODE                   
#include "NvMUT_MemMap.h"
FUNC (void , NvMUT_CODE) RE_ImmdiateBlock_JobFinished/* return value & FctID */
(
        VAR(uint8, AUTOMATIC) ServiceId,
        VAR(NvM_RequestResultType, AUTOMATIC) JobResult
)
{
    if(NvM_Test == IMMEDIATE_WRITNG_TMON)
    {
        Rtm_ProcessEnd(Rtm_Mon_Cfg_ProcessId_NvM_WriteBlock);
    }
}

#define NvMUT_STOP_SEC_CODE                       
#include "NvMUT_MemMap.h"

#define NvMUT_START_SEC_CODE                   
#include "NvMUT_MemMap.h"
FUNC (void , NvMUT_CODE) RE_CyclicBlock_JobFinished/* return value & FctID */
(
        VAR(uint8, AUTOMATIC) ServiceId,
        VAR(NvM_RequestResultType, AUTOMATIC) JobResult
)
{
    
}

#define NvMUT_STOP_SEC_CODE                       
#include "NvMUT_MemMap.h"

#define NvMUT_START_SEC_CODE                   
#include "NvMUT_MemMap.h"
FUNC (void, NvMUT_CODE) RE_NvMUT_100ms/* return value & FctID */
(
        void
)
{

    switch (NvM_Test)
    {
        case ENABLE_WRITE_BLOCK:
            /* write each single block */
            Rte_Write_PRP_CyclicNvBlock_CyclicNvBlock(Write_Block_Cyclic_u8);
            Rte_Write_PRP_ImmediateNvBlock_ImmediateNvBlock(Write_Block_Immediate_u32);
            Rte_Write_PRP_ShutdownNvBlock_ShutDownNvBlock(Write_Block_ShutDown_u8);
            NvM_Test = SWC_ZERO;
            break;
        
        case ENABLE_WRITE_CYCLIC:
            /* Call Rte_Write_PRP_CyclicNvBlock_CyclicNvBlock to write Cyclic block in every 100ms cycle */
            Rte_Write_PRP_CyclicNvBlock_CyclicNvBlock(Write_Block_Cyclic_u8);
            break;

        case ENABLE_WRITE_IMMEDIATE:
            /* test block write immediately */
            Rte_Write_PRP_ImmediateNvBlock_ImmediateNvBlock(Write_Block_Immediate_u32);
            NvM_Test = SWC_ZERO;
            break;

        case ENABLE_WRITE_SHUTDOWN:
            /* test block write on shutdown phase */
            Rte_Write_PRP_ShutdownNvBlock_ShutDownNvBlock(Write_Block_ShutDown_u8);
            NvM_Test = SWC_ZERO;
            break;
        
        case ENABLE_WRITE_CYCLIC_D:
            /* Double write in 1 cycle test */
            Write_Block_Cyclic_u8[0]=1;
            Rte_Write_PRP_CyclicNvBlock_CyclicNvBlock(Write_Block_Cyclic_u8);
            Write_Block_Cyclic_u8[0]=2;
            Rte_Write_PRP_CyclicNvBlock_CyclicNvBlock(Write_Block_Cyclic_u8);
            break;

        case FILL_SECTOR_WITH_FULL_DATA_SR:
            /*Continuously write data until reaching Soft-reorg*/
            /*Can use T32 to check if Fee_Rb_WorkingState_en = FEE_RB_SOFT_SECTOR_REORG_MODE_E then manually break*/
            Write_Block_Immediate_u32[0]++;
            Rte_Write_PRP_ImmediateNvBlock_ImmediateNvBlock(Write_Block_Immediate_u32); 
            break;

        case IMMEDIATE_WRITNG_TMON:
            Rtm_ProcessStart(Rtm_Mon_Cfg_ProcessId_NvM_WriteBlock);
            Write_Block_Immediate_u32[0]++;
            Rte_Write_PRP_ImmediateNvBlock_ImmediateNvBlock(Write_Block_Immediate_u32);
            break;

        default:
            break;

        case ENABLE_READ_BLOCK:
            Rte_Read_PRP_CyclicNvBlock_CyclicNvBlock(Read_Block_Cyclic_u8);
            Rte_Read_PRP_ImmediateNvBlock_ImmediateNvBlock(Read_Block_Immediate_u32);
            Rte_Read_PRP_ShutdownNvBlock_ShutDownNvBlock(Read_Block_ShutDown_u8);
            NvM_Test = SWC_ZERO;
            break;
    }


    if (Req_reset == RESET_GET_INFO) 
    { 
        Rte_Call_RP_ResetInfo_GetLastResetInfo(&resetReason, &resetGroup);
    } 
    else 
    if (Req_reset == RESET_REQUEST)
    {
        StateMachine_Test = INVALID_WAKEUP_SOURCE;
        Rte_Call_RP_RequestReset_RequestReset(ECUM_CAUSE_ECU_STATE, ECUM_RESET_MCU);
        Req_reset = RESET_NONE;
    }


}

#define NvMUT_STOP_SEC_CODE                       
#include "NvMUT_MemMap.h"

#define NvMUT_START_SEC_CODE
#include "NvMUT_MemMap.h"
FUNC(void, NvMUT_CODE) RE_NvMUT_FastWrite_10ms (void)
{
    switch (NvM_Test)
    {
        case ENABLE_FAST_WRITE:
            Rte_Write_PRP_ImmediateNvBlock_ImmediateNvBlock(FWrite_Block_Immediate_u32);
            NvM_Test = SWC_ZERO;
        break;

        case FILL_SECTOR_WITH_FULL_DATA_HR:
            /*Continuously write data until reaching Hard-reorder*/
            /*Can use T32 to check if Fee_Rb_WorkingState_en = FEE_RB_HARD_SECTOR_REORG_MODE_E then manually break*/
            FWrite_Block_Immediate_u32[0]++;
            Rte_Write_PRP_ImmediateNvBlock_ImmediateNvBlock(FWrite_Block_Immediate_u32);    
        break;

        default:
        break;
    }
    
}
#define NvMUT_STOP_SEC_CODE                   
#include "NvMUT_MemMap.h"
