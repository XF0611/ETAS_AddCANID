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
#include "DcmDspUds_Seca_Inf.h"
#include "Rte_Dcm.h"
/*TESTCODE-START
#include "DcmTest.h"
TESTCODE-END*/
#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF) )

 #define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCM_TPL
 * DcmAppl_DcmGetUpdatedDelayForPowerOn :-\n
 *  This API is used for providing the means to the application to change the power On delay for security access service on COLD start.\n
 *  More Information :The user can change the power on Delay for the particular security level during the power on Init.
 *  Failed attempt count before the reset is restored from NVM(application) and passed as an input parameter to this appl funtion.
 *  The user can refer to this failed attempt count and accordingly return the delay for power ON.
 *  If the user does not want to change the power on Delay at this point, then no need to change the appl function as the input value DelayTime is returned by default from this appl function.
 *  DelayTime is the configured power on delay for the particular security level. Hence the configured delay shall be retained in Dcm if this appl function is not modified by the user.
 *
 * @param[in]           SecurityLevel    :    Security level for which the updated power on delay is requested.
 * @param[in]           Delaycount       :    Failed attempt count restored from NVM
 * @param[in]           DelayTime        :    configured power on Delay time in (milliseconds) for that security level
 * @param[out]          None
 * @retval              Updated power on delay time in milliseconds
 *
 */
uint32 DcmAppl_DcmGetUpdatedDelayForPowerOn(
                                                            uint8 SecurityLevel,
                                                            uint8 Delaycount,
                                                            uint32 DelayTime
                                                                )
{
    /* dummy code to remove compiler warning */
    (void)(SecurityLevel);
    (void)(Delaycount);

    /*TESTCODE-START
    DelayTime = DcmTest_DcmAppl_DcmGetUpdatedDelayForPowerOn(SecurityLevel,Delaycount,DelayTime);
    TESTCODE-END*/
    return (DelayTime);
}


#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif /*(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)*/
