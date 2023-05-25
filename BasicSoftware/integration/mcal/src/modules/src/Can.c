/*============================================================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc.
*   All Rights Reserved.
=============================================================================*/

#include "Can.h"
#include "Os.h"
#include "Can_Alps.h"

#include "SchM_Can.h"
#if(CAN_DEV_ERROR_DETECT == STD_ON)
#endif
/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/
#ifndef CAN_SW_MAJOR_VERSION
#error "CAN_SW_MAJOR_VERSION is not defined."
#endif

#ifndef CAN_SW_MINOR_VERSION
#error "CAN_SW_MINOR_VERSION is not defined."
#endif

#ifndef CAN_SW_PATCH_VERSION
#error "CAN_SW_PATCH_VERSION is not defined."
#endif

#if(CAN_SW_MAJOR_VERSION != 3U)
#error "CAN_SW_MAJOR_VERSION does not match."
#endif

#if(CAN_SW_MINOR_VERSION != 4U)
#error "CAN_SW_MINOR_VERSION does not match."
#endif

#if(CAN_SW_PATCH_VERSION != 0U)
#error "CAN_SW_PATCH_VERSION does not match."
#endif

/* AUTOSAR Secification File Version Check */
#ifndef CAN_AR_RELEASE_MAJOR_VERSION
#error "CAN_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

/* AUTOSAR Major release version check  */
#if(CAN_AR_RELEASE_MAJOR_VERSION != 4U)
#error "CAN_AR_RELEASE_MAJOR_VERSION does not match."
#endif

#ifndef CAN_AR_RELEASE_MINOR_VERSION
#error "CAN_AR_RELEASE_MINOR_VERSION is not defined."
#endif

/* AUTOSAR Minor release version check  */
#if(CAN_AR_RELEASE_MINOR_VERSION != 3U)
// #error "CAN_AR_RELEASE_MINOR_VERSION does not match." //zco7sgh
#endif

#ifndef CAN_AR_RELEASE_PATCH_VERSION
#error "CAN_AR_RELEASE_PATCH_VERSION is not defined."
#endif

/* AUTOSAR Minor release version check  */
#if(CAN_AR_RELEASE_PATCH_VERSION != 1U)
#error "CAN_AR_RELEASE_PATCH_VERSION does not match."
#endif

/******************************************************************************
*                           GLOBAL DECLARATIONS
******************************************************************************/
#define CAN_START_SEC_VAR_INIT_UNSPECIFIED
CanUnitType CanUnitInfo[CAN_ALPS_CTRL_CONFIG_COUNT];
Can_GlobalType Can_Global = { .initRun = CAN_UNINIT, }; /** SWS_Can_00103 Req-4.3.1*/
#define CAN_STOP_SEC_VAR_INIT_UNSPECIFIED
/******************************************************************************
*                    Private Function Declaration
******************************************************************************/


/******************************************************************************
*                         Public Functions
******************************************************************************/
#define CAN_START_SEC_CODE

/******************************************************************************
*  Service name    : Can_Init
*  Syntax          : void Can_Init(const Can_ConfigType* ConfigPtr)
*  Service ID      : 0x00
*  Sync/Async      : Synchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : ConfigPtr - Pointer to configuration set
*  Parameters (out): None
*  Return value    : None
*  Description     : This function initializes the CAN module.
******************************************************************************/
void Can_Init(const Can_ConfigType* ConfigPtr) {
    Std_ReturnType RetVal = E_OK;

    /* CAN_FSR_00001 - Check if Config pointer is NULL_PTR*/
    Can_lclParamCheck((boolean)(NULL_PTR == ConfigPtr),
    CAN_INIT_SID, CAN_E_PARAM_POINTER, &RetVal);
    /** SWS_CAN_00174 Req-4.3.1, CAN_FSR_00174*/
    Can_lclParamCheck((boolean)(Can_Global.initRun != CAN_UNINIT),
    CAN_INIT_SID, CAN_E_TRANSITION, &RetVal);
    if(E_OK == RetVal)
	{
        Can_Hal_Init(ConfigPtr);
    }
	/* SWS_CAN_00246 Req-4.3.1*/
    Can_Global.initRun = CAN_READY;
}

/******************************************************************************
*  Service name    : Can_GetVersionInfo
*  Syntax          : void Can_GetVersionInfo(Std_VersionInfoType* versioninfo )
*  Service ID      : 0x07
*  Sync/Async      : Synchronous
*  Reentrancy      : Reentrant
*  Parameters (in) : None
*  Parameters (out): versioninfo - Pointer to where to store the version information of this module.
*  Return value    : None
*  Description     : This function returns the version information of this module.
******************************************************************************/
#if (CAN_VERSION_INFO_API == STD_ON)
void Can_GetVersionInfo(Std_VersionInfoType* versioninfo ) {
    Std_ReturnType RetVal = E_OK;

    /** SWS_CAN_00177 Req-4.3.1, CAN_FSR_00177*/
    Can_lclParamCheck((boolean)(NULL_PTR == versioninfo),
    CAN_GETVERSIONINFO_SID, CAN_E_PARAM_POINTER, &RetVal);
    if(E_OK == RetVal)
	{
        versioninfo->vendorID = CAN_VENDOR_ID;
        versioninfo->moduleID = CAN_MODULE_ID;
        versioninfo->sw_major_version = CAN_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CAN_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CAN_SW_PATCH_VERSION;
    }
}
#endif


/******************************************************************************
*  Service name    : Can_DeInit
*  Syntax          : void Can_DeInit(void);
*  Service ID      : 0x10
*  Sync/Async      : Synchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : This function de-initializes the CAN module.
******************************************************************************/
void Can_DeInit(void) {
    Std_ReturnType RetVal = E_OK;

    /** SWS_CAN_91011 Req-4.3.1, CAN_FSR_91011*/
    Can_lclParamCheck((boolean)(Can_Global.initRun != CAN_READY),
    CAN_DEINIT_SID, CAN_E_TRANSITION, &RetVal);
    if(RetVal == E_OK)
	{
        RetVal = Can_Hal_DeInit();
		if(RetVal == E_OK)
		{
			/** SWS_CAN_91009 Req-4.3.1*/
			Can_Global.initRun = CAN_UNINIT;
		}
    }
}

#if (CAN_SET_BAUDRATE_API == STD_ON)
/******************************************************************************
*  Service name    : Can_SetBaudrate
*  Syntax          : Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID)
*  Service ID      : 0x0F
*  Sync/Async      : Synchronous
*  Reentrancy      : Reentrant for different Controllers. Non reentrant for the same Controller.
*  Parameters (in) : Controller - CAN controller, whose baud rate shall be set
*                  : BaudRateConfigID - references a baud rate configuration by ID
*  Parameters (out): None
*  Return value    : Std_ReturnType - E_OK: Request accepted, E_NOT_OK: Request not accepted
*  Description     : Sets the baud rate configuration of the CAN controller.
******************************************************************************/
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID) {
    Std_ReturnType RetVal = E_OK;

    /** SWS_CAN_00492 Req-4.3.1, CAN_FSR_00492*/
    Can_lclParamCheck((boolean)(Can_Global.initRun != CAN_READY),
    CAN_SETBAUDRATE_SID, CAN_E_UNINIT, &RetVal);
    /** SWS_CAN_00494 Req-4.3.1, CAN_FSR_00494*/
    Can_lclParamCheck((boolean)(Controller >= CAN_ALPS_CTRL_CONFIG_COUNT),
    CAN_SETBAUDRATE_SID, CAN_E_PARAM_CONTROLLER, &RetVal);
    if(RetVal == E_OK)
	{
        if(Can_Hal_SetBaudrate(Controller, BaudRateConfigID) != E_OK)
		{
            RetVal = E_NOT_OK;
        }
    }
    return RetVal;
}
#endif

/******************************************************************************
*  Service name    : Can_SetControllerMode
*  Syntax          : Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)
*  Service ID      : 0x03
*  Sync/Async      : ASynchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : Controller - CAN controller for which the status shall be changed
*                  : Transition - CAN controller State transition type
*  Parameters (out): None
*  Return value    : Std_ReturnType - E_OK: Request accepted, E_NOT_OK: Request not accepted
*  Description     : Performs software triggered state transitions of the CAN controller State machine.
******************************************************************************/
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition){
    Std_ReturnType RetVal = E_OK;

    /** SWS_CAN_00198 Req-4.3.1, CAN_FSR_00198*/
    Can_lclParamCheck((boolean)(Can_Global.initRun != CAN_READY),
    CAN_SETCONTROLLERMODE_SID, CAN_E_UNINIT, &RetVal);
    /** SWS_CAN_00199 Req-4.3.1, CAN_FSR_00199*/
    Can_lclParamCheck((boolean)(Controller >= CAN_ALPS_CTRL_CONFIG_COUNT),
    CAN_SETCONTROLLERMODE_SID, CAN_E_PARAM_CONTROLLER, &RetVal);

    if(RetVal == E_OK)
	{
        if(Can_Hal_SetControllerMode(Controller, Transition) != E_OK)
		{
            RetVal = E_NOT_OK;
        }
    }
    return RetVal;
}


/******************************************************************************
*  Service name    : Can_DisableControllerInterrupts
*  Syntax          : void Can_DisableControllerInterrupts(uint8 Controller)
*  Service ID      : 0x04
*  Sync/Async      : Synchronous
*  Reentrancy      : Reentrant
*  Parameters (in) : Controller - CAN controller for which interrupts shall be disabled.
*  Parameters (out): None
*  Return value    : None
*  Description     : Disables all interrupts for this CAN controller.
******************************************************************************/
void Can_DisableControllerInterrupts(uint8 Controller) {
    Std_ReturnType RetVal = E_OK;

    /** SWS_CAN_00205 Req-4.3.1, CAN_FSR_00205*/
    Can_lclParamCheck((boolean)(Can_Global.initRun != CAN_READY),
    CAN_DISABLECONTROLLERINTERRUPTS_SID, CAN_E_UNINIT, &RetVal);
    /** SWS_CAN_00206 Req-4.3.1, CAN_FSR_00206*/
    Can_lclParamCheck((boolean)(Controller >= CAN_ALPS_CTRL_CONFIG_COUNT),
    CAN_DISABLECONTROLLERINTERRUPTS_SID, CAN_E_PARAM_CONTROLLER, &RetVal);
    if(RetVal == E_OK)
	{
        /* Disable the OS interrupts*/
        SchM_Enter_Can_DisableControllerInterrupts();
		/** SWS_CAN_00049, SWS_CAN_00202*/
        if(CanUnitInfo[Controller].LockCount == 0u)
		{
            Can_Hal_DisableControllerInterrupts(Controller);
			CanUnitInfo[Controller].LockCount++;
        }

        /* Enable the OS interrupts*/
        SchM_Exit_Can_DisableControllerInterrupts();
    }
}


/******************************************************************************
*  Service name    : Can_EnableControllerInterrupts
*  Syntax          : void Can_EnableControllerInterrupts(uint8 Controller)
*  Service ID      : 0x05
*  Sync/Async      : Synchronous
*  Reentrancy      : Reentrant
*  Parameters (in) : Controller - CAN controller for which interrupts shall be re-enabled
*  Parameters (out): None
*  Return value    : None
*  Description     : Enables all allowed interrupts
******************************************************************************/
void Can_EnableControllerInterrupts(uint8 Controller) {
    Std_ReturnType RetVal = E_OK;

    /** SWS_CAN_00209 Req-4.3.1, CAN_FSR_00209*/
    Can_lclParamCheck((boolean)(Can_Global.initRun != CAN_READY),
    CAN_ENABLECONTROLLERINTERRUPTS_SID, CAN_E_UNINIT, &RetVal);
    /** SWS_CAN_00210 Req-4.3.1, CAN_FSR_00210*/
    Can_lclParamCheck((boolean)(Controller >= CAN_ALPS_CTRL_CONFIG_COUNT),
    CAN_ENABLECONTROLLERINTERRUPTS_SID, CAN_E_PARAM_CONTROLLER, &RetVal);
    if(RetVal == E_OK) {

        /* Disable the OS interrupts*/
        SchM_Enter_Can_EnableControllerInterrupts();

        if(CanUnitInfo[Controller].LockCount == 0)
		{
            /** SWS_CAN_00208 Req-4.3.1*/
            /* Do nothing */
        } else {
            CanUnitInfo[Controller].LockCount--;
            if(CanUnitInfo[Controller].LockCount == 0)
			{
                Can_Hal_EnableControllerInterrupts(Controller);
            }
        }

        /* Enable the OS interrupts*/
        SchM_Exit_Can_EnableControllerInterrupts();
    }
}


/******************************************************************************
*  Service name    : Can_CheckWakeup
*  Syntax          : Std_ReturnType Can_CheckWakeup( uint8 Controller );
*  Service ID      : 0x0B
*  Parameters (in) : Controller - CAN controller ID
*  Parameters (out): None
*  Return value    : Std_ReturnType - E_OK: API call has been accepted.
*                  : -E_NOT_OK: API call has not been accepted.
*  Description     : This function checks if a wakeup has occurred for the given controller.
******************************************************************************/
Std_ReturnType Can_CheckWakeup(uint8 Controller) {
    /*WakeUp functionality is not supported by the current module */
	return( (Std_ReturnType)E_NOT_OK );
}


/******************************************************************************
*  Service name    : Can_GetControllerErrorState
*  Syntax          : Std_ReturnType Can_GetControllerErrorState( uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr);
*  Service ID      : 0x11
*  Sync/Async      : Synchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : ControllerId - Abstracted CanIf ControllerId which is assigned to a CAN controller, which is requested for ErrorState.
*  Parameters (out): ErrorStatePtr - Pointer to a memory location, where the error state of the CAN controller will be stored.
*  Return value    : Std_ReturnType - E_OK: Error state request has been accepted.
*                  : -E_NOT_OK: Error state request has not been accepted.
*  Description     : This service obtains the error state of the CAN controller.
******************************************************************************/
Std_ReturnType Can_GetControllerErrorState( uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr) {
    Std_ReturnType RetVal = E_OK;

    /** SWS_CAN_91005 Req-4.3.1, CAN_FSR_91005*/
    Can_lclParamCheck((boolean)(Can_Global.initRun != CAN_READY),
    CAN_GETCONTROLLERERRORSTATE_SID, CAN_E_UNINIT, &RetVal);
    /** SWS_CAN_91006 Req-4.3.1, CAN_FSR_91006*/
    Can_lclParamCheck((boolean)(ControllerId > CAN_ALPS_CTRL_CONFIG_COUNT),
    CAN_GETCONTROLLERERRORSTATE_SID, CAN_E_PARAM_CONTROLLER, &RetVal);
    /** SWS_CAN_91007 Req-4.3.1 CAN_FSR_91007*/
    Can_lclParamCheck((boolean)(NULL_PTR == ErrorStatePtr), CAN_GETCONTROLLERERRORSTATE_SID, CAN_E_PARAM_POINTER, &RetVal);
    if(RetVal == E_OK)
	{
        Can_Hal_GetControllerErrorState(ControllerId, ErrorStatePtr);
    }
    return RetVal;
}


/******************************************************************************
*  Service name    : Can_GetControllerMode
*  Syntax          : Std_ReturnType Can_GetControllerMode( uint8 Controller, Can_ControllerStateType* ControllerModePtr);
*  Service ID      : 0x12
*  Sync/Async      : Synchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : Controller - CAN controller for which the status shall be requested.
*  Parameters (out): ControllerModePtr - Pointer to a memory location, where the current mode of the CAN controller will be stored.
*  Return value    : Std_ReturnType - E_OK: Controller mode request has been accepted.
*                  : -E_NOT_OK: Controller mode request has not been accepted.
*  Description     : This service reports about the current status of the requested CAN controller.
******************************************************************************/
Std_ReturnType Can_GetControllerMode( uint8 Controller, Can_ControllerStateType* ControllerModePtr) {
    Std_ReturnType RetVal = E_OK;

    /** SWS_CAN_91016 Req-4.3.1, CAN_FSR_91016*/
    Can_lclParamCheck((boolean)(Can_Global.initRun != CAN_READY),
    CAN_GETCONTROLLERMODE_SID, CAN_E_UNINIT, &RetVal);
    /** SWS_CAN_91017 Req-4.3.1, CAN_FSR_91017*/
    Can_lclParamCheck((boolean)(Controller >= CAN_ALPS_CTRL_CONFIG_COUNT),
    CAN_GETCONTROLLERMODE_SID, CAN_E_PARAM_CONTROLLER, &RetVal);
    /** SWS_CAN_91018 Req-4.3.1, CAN_FSR_91018 */
    Can_lclParamCheck((boolean)(NULL_PTR == ControllerModePtr), CAN_GETCONTROLLERMODE_SID, CAN_E_PARAM_POINTER, &RetVal);
    if(RetVal == E_OK)
	{
        Can_Hal_GetControllerMode(Controller, ControllerModePtr);
    }
    return RetVal;
}


/******************************************************************************
*  Service name    : Can_Write
*  Syntax          : Std_ReturnType Can_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo );
*  Service ID      : 0x06
*  Sync/Async      : Synchronous
*  Reentrancy      : Non reentrant
*  Parameters (in) : Hth - information which HW-transmit handle shall be used for transmit.
*                  : PduInfo - Pointer to SDU user memory, Data Length and Identifier.
*  Parameters (out): None
*  Return value    : Std_ReturnType - E_OK: Write command has been accepted.
*                  : -E_NOT_OK: development error occurred.
*                  : -CAN_BUSY: No TX hardware buffer available.
*  Description     : This function is called by CanIf to pass a CAN message to CanDrv for transmission.
*
*
******************************************************************************/
Std_ReturnType Can_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo ){
    Std_ReturnType RetVal = E_OK;

    /* SWS_CAN 00216 Req-4.3.1, CAN_FSR_00216 */
    Can_lclParamCheck((boolean)(Can_Global.initRun != CAN_READY),
    CAN_WRITE_SID, CAN_E_UNINIT, &RetVal);
    /* SWS_CAN 00219 Req-4.3.1, CAN_FSR_00219 */
    Can_lclParamCheck((boolean)(NULL_PTR == PduInfo),
    CAN_WRITE_SID, CAN_E_PARAM_POINTER, &RetVal);
    /* SWS_CAN 00217 Req-4.3.1, CAN_FSR_00217 */
    Can_lclParamCheck((boolean)((Hth - CAN_NUM_OF_HRHS) > CAN_NUM_OF_HTHS),
    CAN_WRITE_SID, CAN_E_PARAM_HANDLE, &RetVal);

    /* SWS_CAN 00218 Req-4.3.1, CAN_FSR_00218 - Report Error If the length is more than 64 byte.*/

    Can_lclParamCheck((boolean)(PduInfo->length > 64),
    CAN_WRITE_SID, CAN_E_PARAM_DATA_LENGTH, &RetVal);

    /* SWS_CAN 00218 Req-4.3.1, CAN_FSR_00218 - If CAN-FD is not supported - Report Error if Length is more than 8 bytes */
    #if (CAN_FD_SUPPORTED == STD_OFF)
	Can_lclParamCheck((boolean)(PduInfo->length > 8),
    		CAN_WRITE_SID, CAN_E_PARAM_DATA_LENGTH, &RetVal);
	#endif
    /* SWS_CAN 00218 Req-4.3.1, CAN_FSR_00218 - Report Error If length > 8 bytes & the CAN FD flag is not set*/
    Can_lclParamCheck((boolean)((uint32)((PduInfo->length > 8) && (!((uint32)((PduInfo->id >> 30) != 0x1) ^ ((uint32)((PduInfo->id >> 30) != 0x3)))))),
    CAN_WRITE_SID, CAN_E_PARAM_DATA_LENGTH, &RetVal);

    if(RetVal == E_OK)
	{
        RetVal = Can_Hal_Write(Hth, PduInfo);
    }
    return RetVal;

}


/******************************************************************************
*  Service name    : Can_MainFunction_Write
*  Syntax          : void Can_MainFunction_Write( void );
*  Service ID      : 0x01
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : This function performs the polling of TX confirmation when CAN_TX_PROCESSING is set to POLLING..
******************************************************************************/
#if (CAN_MAIN_FUNCTION_WRITE_POLLING_ALLOWED == STD_ON)
void Can_MainFunction_Write( void )
{
    Can_Hal_MainFunctionWrite();
}
#endif


/******************************************************************************
*  Service name    : Can_MainFunction_Read
*  Syntax          : void Can_MainFunction_Read( void );
*  Service ID      : 0x08
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : This function performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING..
******************************************************************************/
#if (CAN_MAIN_FUNCTION_READ_POLLING_ALLOWED == STD_ON)
void Can_MainFunction_Read( void ) {

    Can_Hal_MainFunctionRead();
}
#endif


/******************************************************************************
*  Service name    : Can_MainFunction_BusOff
*  Syntax          : void Can_MainFunction_BusOff( void );
*  Service ID      : 0x09
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : Performs the polling of bus-off events that are configured statically as 'to be polled'.
******************************************************************************/
#if (CAN_MAIN_FUNCTION_BUSOFF_POLLING_ALLOWED == STD_ON)
void Can_MainFunction_BusOff( void ) {

    Can_Hal_MainFunctionBusOff();
}
#endif


/******************************************************************************
*  Service name    : Can_MainFunction_Wakeup
*  Syntax          : void Can_MainFunction_Wakeup( void );
*  Service ID      : 0x0A
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : This function performs the polling of wake-up events.
******************************************************************************/
void Can_MainFunction_Wakeup( void ){
    /*WakeUp functionality is not supported by the current module */
}


/******************************************************************************
*  Service name    : Can_MainFunction_Mode
*  Syntax          : void Can_MainFunction_Mode( void );
*  Service ID      : 0x0C
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : This function performs the polling of CAN controller mode transitions.
******************************************************************************/
void Can_MainFunction_Mode( void ) {

    Can_Hal_MainFunctionMode();
}


/*******************************************************************************
*  Service name    : Can_GetStatisticsInfo
*  Syntax          : void Can_GetStatisticsInfo(uint8 Controller, Can_StatisticsType *Can_StatisticsData)
*  Parameters (in) : Controller - CAN Controller Id
*                  : Can_StatisticsData - Pointer to store statistics info
*  Parameter (out) : --
*  Return value    : --
*  Description     : Gives information about CAN statistics
*******************************************************************************/
#if (SUPPORT_CAN_STATISTICS == STD_ON)
void Can_GetStatisticsInfo(uint8 Controller, Can_StatisticsType *Can_StatisticsData)
{
	if(Can_Global.initRun == CAN_READY)
	{
		CanUnitType *UnitPtr = &CanUnitInfo[Controller];
		*Can_StatisticsData = UnitPtr->Stats;
	}
	else
	{
		/*compliant to misra-c*/
	}
}
#endif


/******************************************************************************
*  Service name    : OS_ISR_Canx_InterruptHandler
*  Syntax          : void OS_ISR_Canx_InterruptHandler(void)
*  Parameters (in) : none
*  Parameters (out): None
*  Return value    : None
*  Description     : This function provides handles interrupts of CAN module.
******************************************************************************/
//ETAS BIP Porting void OS_ISR_Can0_InterruptHandler(void *ptr)
ISR(Can0_InterruptHandler_0)
{
   Can_Isr(CAN_CTRL_0);
}

ISR(Can0_InterruptHandler_1)
{
   Can_Isr(CAN_CTRL_0);
}

ISR(Can0_InterruptHandler_2)
{
   Can_Isr(CAN_CTRL_0);
}

ISR(Can0_InterruptHandler_3)
{
   Can_Isr(CAN_CTRL_0);
}

//ETAS BIP Porting void OS_ISR_Can1_InterruptHandler(void *ptr)
void OS_ISR_Can1_InterruptHandler(void)
{
   Can_Isr(CAN_CTRL_1);
}
#define CAN_STOP_SEC_CODE
