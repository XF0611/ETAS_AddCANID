/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 


#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Roe_Inf.h"
#include "Rte_Dcm.h"
/*TESTCODE-START
#include "DcmTest.h"
TESTCODE-END*/

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)

#if(DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF)

 /**
 * @ingroup DCM_TPL
 * DcmAppl_DcmGetRoeDTCInfo :-\n
 * The API will be invoked in the init to recover Roe Events and its states from NvM in case of OnDTCStatusChange.
 *
 * @param[in]	  RoeeventID : Event Id of Roe Event.
 * @param[out]    RoeEventStatus : Status of Roe Event. The possible values of RoeEventStatus is DCM_ROE_CLEARED, DCM_ROE_STOPPED and DCM_ROE_STARTED
 * @param[out]    SourceAddress : Tester Source Address. Source of the address value is from configuration parameter DcmDslProtocolRxTesterSourceAddr
 * @param[out]    StatusMask : DTC Status Mask information
 * @param[out]    dspRoeCtrlStorageState_b : Storage bit information of the control request
 * @param[out]   dspRoeSetUpStorageState_b : Storage bit information of the set up request
 * @param[out]    dspRoeSession_b : If the current session is Default session, than this parameter will be  true
 * @retval        void
  *
 */
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Std_ReturnType DcmAppl_DcmGetRoeDTCInfo(uint8 RoeeventID,
                                                Dcm_DspRoeEventState_ten * RoeEventStatus,
                                                uint8 * RoeEventWindowTime_u8,
                                                uint16 * SourceAddress,
                                                uint8 * StatusMask,
                                                boolean * dspRoeCtrlStorageState_b,
                                                boolean * dspRoeSetUpStorageState_b,
                                                boolean * dspRoeSession_b)
{

    Std_ReturnType retVal_u8;
    retVal_u8 = E_OK;
    /*TESTCODE-START
         retVal_u8 =   DcmTest_DcmAppl_DcmGetRoeDTCInfo(RoeeventID,RoeEventStatus,RoeEventWindowTime_u8,SourceAddress,StatusMask,dspRoeCtrlStorageState_b,dspRoeSetUpStorageState_b,dspRoeSession_b);
            TESTCODE-END*/
    (void)(RoeeventID);
    (void)(RoeEventStatus);
    (void)(RoeEventWindowTime_u8);
    (void)(SourceAddress);
    (void)(StatusMask);
    (void)(dspRoeCtrlStorageState_b);
    (void)(dspRoeSetUpStorageState_b);
    (void)(dspRoeSession_b);
    return retVal_u8;

	}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif
#endif
