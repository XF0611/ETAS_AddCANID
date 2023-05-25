

#ifndef DCMDSPUDS_RMBA_PROT_H
#define DCMDSPUDS_RMBA_PROT_H


/**
 ***************************************************************************************************
            Read Memory By Address (RMBA) service
 ***************************************************************************************************
 */

#if (DCM_CFG_DSP_READMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF)

#include "DcmDspUds_Memaddress_Calc_Prot.h"

typedef struct
{
	uint32 dataReadMemoryRangeLow_u32;			/*Lower memory address range allowed for writing*/
	uint32 dataReadMemoryRangeHigh_u32;			/*High memory address range allowed for writing*/
	uint32 dataAllowedSecRead_u32;          /* Allowed security levels(for Reading) */
	uint32 dataAllowedSessRead_u32;         /* Allowed session (for Reading)*/
	Std_ReturnType (*adrUserMemReadModeRule_pfct) (Dcm_NegativeResponseCodeType * Nrc_u8,uint32 adrMemoryAddress_u32,uint32 dataDataLength_u32,Dcm_Direction_t dataDirection_en); /* User Mode rule check function pointer */
#if (DCM_CFG_DSP_MODERULEFORREADMEMORY != DCM_CFG_OFF)
	boolean (*addrRmbaModeRuleChkFnc_pfct) (Dcm_NegativeResponseCodeType * Nrc_u8); /* Mode rule check funtion pointer */
#endif
	uint8 dataMemoryValue_u8;				/*Value of Memory Identifier to select the desired memory device*/
} Dcm_RMBAConfig_tst;
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const Dcm_RMBAConfig_tst Dcm_RMBAConfig_cast [];
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint16 Dcm_RmbacalculateTableSize_u16(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#endif

#endif   /* _DCMDSPUDS_RMBA_PROT_H */

