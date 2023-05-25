
#ifndef NVM_PRV_JOB_H
#define NVM_PRV_JOB_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Types.h"

#include "NvM_Prv_Job_Types.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
extern void NvM_Prv_Job_StartRecalcRamBlkCrc(NvM_BlockIdType idBlock_uo);

extern void NvM_Prv_Job_Prepare(NvM_Prv_idJob_ten idJob_en,
                                NvM_Prv_QueueEntry_tst* Request_pst,
                                NvM_Prv_idQueue_tuo idReqQueue_uo);

extern void NvM_Prv_Job_Start(NvM_Prv_stJob_ten* stJob_pen,
                              NvM_Prv_JobResult_ten* Result_pen,
                              NvM_Prv_JobData_tst const* JobData_pcst,
                              NvM_Prv_idJob_ten idJob_en,
                              uint8 idxDataset_u8);

extern void NvM_Prv_Job_DoStateMachine(NvM_Prv_Job_tst* Job_pst,
                                       NvM_Prv_Job_StateMachine_tpfct StateMachine_pfct);

extern NvM_Prv_Job_State_tpfct NvM_Prv_JobRead_GetStateFct(NvM_Prv_stJob_ten* stJob_pen);
extern NvM_Prv_Job_State_tpfct NvM_Prv_JobWrite_GetStateFct(NvM_Prv_stJob_ten* stJob_pen);
extern NvM_Prv_Job_State_tpfct NvM_Prv_JobRestore_GetStateFct(NvM_Prv_stJob_ten* stJob_pen);
extern NvM_Prv_Job_State_tpfct NvM_Prv_JobMaintain_GetStateFct(NvM_Prv_stJob_ten* stJob_pen);
extern NvM_Prv_Job_State_tpfct NvM_Prv_JobInvalidate_GetStateFct(NvM_Prv_stJob_ten* stJob_pen);
extern NvM_Prv_Job_State_tpfct NvM_Prv_JobValidate_GetStateFct(NvM_Prv_stJob_ten* stJob_pen);
extern NvM_Prv_Job_State_tpfct NvM_Prv_JobRecalcRamBlkCrc_GetStateFct(NvM_Prv_stJob_ten* stJob_pen);

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
extern NvM_Prv_stJob_ten NvM_Prv_Job_GetState(void);
#endif

/* NVM_PRV_JOB_H */
#endif

