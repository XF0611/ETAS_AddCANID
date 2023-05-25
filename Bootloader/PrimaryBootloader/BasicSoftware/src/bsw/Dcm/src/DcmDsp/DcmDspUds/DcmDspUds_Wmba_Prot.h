

#ifndef DCMDSPUDS_WMBA_PROT_H
#define DCMDSPUDS_WMBA_PROT_H


/**
 ***************************************************************************************************
            Write Memory By Address (WMBA) service
 ***************************************************************************************************
 */
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_WRITEMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF)

#include "DcmDspUds_Memaddress_Calc_Prot.h"

typedef struct
{
	uint32 dataWriteMemoryRangeLow_u32;		/*Lower memory address range allowed for writing*/
	uint32 dataWriteMemoryRangeHigh_u32;	/*High memory address range allowed for writing*/
	uint32 dataAllowedSecWrite_u32;         /* Allowed security levels(for writing) */
	uint32 dataAllowedSessWrite_u32;        /* Allowed session (for writing)*/
	Std_ReturnType (*adrUserMemWriteModeRule_pfct) (Dcm_NegativeResponseCodeType * Nrc_u8,uint32 adrMemoryAddress_u32,uint32 dataDataLength_u32,Dcm_Direction_t dataDirection_en); /* User Mode rule check function pointer */
#if (DCM_CFG_DSP_MODERULEFORWRITEMEMORY != DCM_CFG_OFF)
	boolean (*addrWmbaModeRuleChkFnc_pfct) (Dcm_NegativeResponseCodeType * Nrc_u8); /* Mode rule check function pointer */
#endif
	uint8 dataMemoryValue_u8;				/*Value of Memory Identifier to select the desired memory device*/
} Dcm_WMBAConfig_tst;
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const Dcm_WMBAConfig_tst Dcm_WMBAConfig_cast [];
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint16 Dcm_WmbacalculateTableSize_u16(void);

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#endif


#endif   /* _DCMDSPUDS_WMBA_PROT_H */
#endif
