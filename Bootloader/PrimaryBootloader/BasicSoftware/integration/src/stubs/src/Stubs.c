#include "ComStack_Types.h"
#include "CanSM_BswM.h"
#include "Rte_Type.h"
#include "Dcm.h"
#include "FBL_DataM.h"
#include "NvM.h"
#include "BswM_CanSM.h"
#include "ComM_Dcm.h"
#include "DcmAppl.h"
#include "Os_Tasks.h"
#include "Can.h"
#include "Fls.h"
#include "BLSM_CallOuts.h"
// #include "Rte_DoIP.h"
// #include "DoIP_Types.h"
// #include "BswM_EthSM.h"
/* RHT2HC: Comment "Fls_Cbk.h" including
#include "Fls_Cbk.h"
*/

/**
 * @brief
 *
 * @param Network
 * @param CurrentState
 */
void BswM_CanSM_CurrentState (NetworkHandleType Network, CanSM_BswMCurrentStateType CurrentState)	/* PRQA S 3206 */
{
}

// FUNC(void, BSWM_CODE) BswM_EthSM_CurrentState
// (
//     VAR(NetworkHandleType, AUTOMATIC) Network,                              /* PRQA S 3206 */ /* This function does not use this parameter */
//     VAR(EthSM_NetworkModeStateType, AUTOMATIC) CurrentState                 /* PRQA S 3206 */ /* This function does not use this parameter */
// )
// {

// }


// FUNC(Std_ReturnType, RTE_CODE) FBL_DoIPPowerModeCallbackFunc(CONSTP2VAR(DoIP_PowerStateType, AUTOMATIC, RTE_APPL_DATA) PowerStateReady)
// {
//     *PowerStateReady = DOIP_READY;
//     return E_OK;
// }
/**
 * @brief
 *
 * @param EventId
 * @param EventStatus
 */
void Dem_ReportErrorStatus ( Dem_EventIdType EventId,		/* PRQA S 3206 */
                          Dem_EventStatusType EventStatus)	/* PRQA S 3206 */
{
}

/**
 * @brief
 *
 */
FUNC(void,COMM_CODE) ComM_DCM_ActiveDiagnostic(VAR(NetworkHandleType, AUTOMATIC) Channel)	/* PRQA S 3206 */
{
}

FUNC(void,COMM_CODE) ComM_DCM_InactiveDiagnostic(VAR(NetworkHandleType, AUTOMATIC) Channel)	/* PRQA S 3206 */
{
}

Std_ReturnType Dem_DisableDTCSetting (uint8 ClientId)	/* PRQA S 3206 */
{
    return E_OK;
}

Std_ReturnType Dem_EnableDTCSetting (uint8 ClientId)	/* PRQA S 3206 */
{
    return E_OK;
}

#if(DCM_CALLAPPLICATIONONREQRX_ENABLED!=DCM_CFG_OFF)

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_Cfg_MemMap.h"
/**
 * @ingroup DCM_TPL
 *  DcmAppl_TpRxIndication :-\n
 *  This API is called to inform the application regarding the the completion of copying of the request into the Dcm buffer.
 *  The RXPduId and the status  of reception is passed to the application.
 *  In case the reception is not successful, the  the application can do necessary steps to cancel the processing in the application
 *  @param[in]     idContext : SID of the service which is running.
 *  @param[in]     Result    : Status of reception.
 *  @param[out]    None
 *  @retval        None
 */
/* MISRA RULE 3206: The parameters is not used in this function. */
FUNC(void,DCM_APPL_CODE) DcmAppl_TpRxIndication( VAR(PduIdType, AUTOMATIC) DcmRxPduId,  /* PRQA S 3206 */ /* This function does not use this parameter */
                                                  VAR(Std_ReturnType, AUTOMATIC) Result)/* PRQA S 3206 */ /* This function does not use this parameter */
{
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_Cfg_MemMap.h"
#endif

/**
 **************************************************************************************************
 * Dcm_Dsp_DscIni : Initialization function for DSC Service
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
FUNC(void,DCM_CODE) Dcm_Dsp_CCIni(void)
{

}


//#define ENABLE_NVM_TEST_CODE
/* Test NvM blocks */
#ifdef ENABLE_NVM_TEST_CODE

typedef Std_ReturnType (*NvMReadFunc_ptr)	(uint8*);
typedef Std_ReturnType (*NvMWriteFunc_ptr)	(const uint8*);

typedef struct
{
    uint8	NvMBlockLeng;
    uint8*	NvMRAMBlock;
    NvMReadFunc_ptr		ReadNvMFnc;
    NvMWriteFunc_ptr	WriteNvMFnc;
}test_NvM_st;

static uint8 Test_Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ExternalReprogrammingRequestFlagBlock];
static uint8 Test_Fbl_DataM_RamMirrorNV_ValidityFlagsBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ValidityFlagsBlock];
static uint8 Test_Fbl_DataM_RamMirrorNV_ResetResponseBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ResetResponseFlagBlock];
static uint8 Test_Fbl_DataM_RamMirrorNV_FingerprintBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_FingerprintBlock];
static uint8 Test_Fbl_DataM_RamMirrorNV_CrcBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_CrcBlock];
static uint8 Test_Fbl_DataM_RamMirrorNV_ProgrammingAttemptCounterBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ProgrammingAttemptCounterBlock];
static uint8 Test_Fbl_DataM_RamMirrorNV_ProgrammingCounterBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ProgrammingCounterBlock];

test_NvM_st	testNvMBlockData[NVM_CFG_LAST_USER_BLOCK - 2 + 1] = {\
        {NVM_CFG_NV_BLOCK_LENGTH_NV_ExternalReprogrammingRequestFlagBlock,      Test_Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock, 		FBL_DataM_ReadNvmReprogrammingRequestFlag,					FBL_DataM_WriteNvmReprogrammingRequestFlag},\
        {NVM_CFG_NV_BLOCK_LENGTH_NV_ValidityFlagsBlock,							Test_Fbl_DataM_RamMirrorNV_ValidityFlagsBlock,					FBL_DataM_ReadNvmValidityFlags,					            FBL_DataM_WriteNvmValidityFlags},\
        {NVM_CFG_NV_BLOCK_LENGTH_NV_ResetResponseFlagBlock,						Test_Fbl_DataM_RamMirrorNV_ResetResponseBlock,					FBL_DataM_ReadNvmResetResponseFlag,							FBL_DataM_WriteNvmResetResponseFlag},\
        {NVM_CFG_NV_BLOCK_LENGTH_NV_FingerprintBlock,				            Test_Fbl_DataM_RamMirrorNV_FingerprintBlock,					FBL_DataM_ReadNvmFingerprint,				                FBL_DataM_WriteNvmFingerprint},\
        {NVM_CFG_NV_BLOCK_LENGTH_NV_CrcBlock,						            Test_Fbl_DataM_RamMirrorNV_CrcBlock,							FBL_DataM_ReadNvmCrc,						                FBL_DataM_WriteNvmCrc},\
        {NVM_CFG_NV_BLOCK_LENGTH_NV_ProgrammingAttemptCounterBlock,				Test_Fbl_DataM_RamMirrorNV_ProgrammingAttemptCounterBlock,		FBL_DataM_ReadNvmProgrammingAttemptCounter, 				FBL_DataM_WriteNvmProgrammingAttemptCounter},\
        {NVM_CFG_NV_BLOCK_LENGTH_NV_ProgrammingCounterBlock, 		            Test_Fbl_DataM_RamMirrorNV_ProgrammingCounterBlock, 			FBL_DataM_ReadNvmProgrammingCounter,					    FBL_DataM_WriteNvmProgrammingCounter}
};

volatile uint8	EnableNvMTestCode_Flg = 0;
void	Test_NvM_Blocks_MainFunction(void)
{
    uint8	blockIdx = 0;
    uint8	elmtIdx = 0;

    switch(EnableNvMTestCode_Flg)
    {
        /* Read data to RAM block */
        case 1:
            for(blockIdx = 0; blockIdx < (NVM_CFG_LAST_USER_BLOCK -1); blockIdx++)
            {
                testNvMBlockData[blockIdx].ReadNvMFnc(testNvMBlockData[blockIdx].NvMRAMBlock);
            }
            EnableNvMTestCode_Flg = 0;
            break;
        /* Update value of RAM block */
        case 2:
            for(blockIdx = 0; blockIdx < (NVM_CFG_LAST_USER_BLOCK -1); blockIdx++)
            {
                for(elmtIdx = 0; elmtIdx < testNvMBlockData[blockIdx].NvMBlockLeng; elmtIdx++)
                {
                    testNvMBlockData[blockIdx].NvMRAMBlock[elmtIdx] += blockIdx + 1;
                }
            }
            EnableNvMTestCode_Flg = 0;
            break;
        /* Request write data from RAM block to FLASH block */
        case 3:
            for(blockIdx = 0; blockIdx < (NVM_CFG_LAST_USER_BLOCK -1); blockIdx++)
            {
                testNvMBlockData[blockIdx].WriteNvMFnc(testNvMBlockData[blockIdx].NvMRAMBlock);
            }
            EnableNvMTestCode_Flg = 0;
            break;
        default:
            break;
    }
}
#endif

//#define FCL_CODE_TESTING
/* TODO: Description */
#ifdef FCL_CODE_TESTING

#include "FBL_Port.h"
#include "Mcu.h"

uint8 FCL_TestWriteData[FBL_PORT_WRITE_SIZE] = {
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA,
		0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA, 0xCA
};

void Test_FCL_Mainfunction(void)
{
	static uint8	FCL_TestCase = 0;
	static volatile Std_ReturnType	testFCLWriteResult = E_NOT_OK;
	static volatile Fbl_Port_Mem_ErrorSatusType TestEraseResult = FBL_PORT_MEM_E_FAIL;
	static volatile uint32 writeTime = 0;
	switch(FCL_TestCase)
	{
		case 1:
			TestEraseResult = Fbl_Port_EraseApplication();
			if(TestEraseResult != FBL_PORT_MEM_E_PENDING)
			{
				FCL_TestCase = 0;
			}
			break;
		case 2:
			TestEraseResult = Fbl_Port_EraseCalibration();
			if(TestEraseResult != FBL_PORT_MEM_E_PENDING)
			{
				FCL_TestCase = 0;
			}
			break;
		case 3:
			testFCLWriteResult = Fbl_Port_WriteFlash(FBL_PORT_CALIBRATION_PRIMARY_NONE_CACHE_REGION_START_ADDRESS + writeTime * FBL_PORT_WRITE_SIZE, FBL_PORT_WRITE_SIZE,(const uint8*) FCL_TestWriteData);
			FCL_TestCase = 0;
			break;
		case 4:
			Mcu_PerformReset();
			FCL_TestCase = 0;
			break;
		default:
			break;
	}
}

#endif
