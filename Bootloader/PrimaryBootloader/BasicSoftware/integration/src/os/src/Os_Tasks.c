/*********************************************************************************************************************
*
* Description:     Os_Tasks module implementation
* FileName:        Os_Tasks.c
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2019. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
* FIF5MI        22/02/2018      First Version.
**********************************************************************************************************************/

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Os_Tasks.h"
#include "Os.h"

#include "CanIf.h"
#include "CanTp.h"
#include "Can.h"
#include "Dcm.h"
#include "CanSM.h"
#include "BLSM.h"
#include "Fls.h"
#include "SchM_Dcm.h"
#include "Fee.h"

#include "FBL_BootM.h"
#include "FBL_DataM.h"
#include "EcuMgt.h"
#include "Sleep.h"
#include "ImageM.h"
#include "FBL_WdgM.h"

// #include "Eth.h"
// #include "EthIf.h"
// #include "SoAd.h"
// #include "DoIP.h"
// #include "EthSM.h"
// #include "TcpIp.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/


/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/


/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/


/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Function Name:     OsTask_100uS
* Return Value:      None
* Parameters List:   None
* Description:       100us Task.
*********************************************************************************************************************/


/**
 * @brief 
 * 
 */
TASK(osTask_StartUp)
{
    /* Start scheduleTable when the counter wraps to zero */
    StartScheduleTableRel(Fbl_ScheduleTable, (TickType)4u );       /* PRQA S 3200 */
    TerminateTask();                            /* PRQA S 3200 */
}

/*********************************************************************************************************************
* Function Name:     OsTask_1ms
* Return Value:      None
* Parameters List:   None
* Description:       1ms Task.
*********************************************************************************************************************/
TASK(osTask_1ms)        /* PRQA S 1503 */
{
    /* -- 1 ms Task -- */
    //Can_MainFunction_Write();
    //Can_MainFunction_Read();
    Can_MainFunction_BusOff();
    Can_MainFunction_Mode();

    // Eth_Mainfunction();
    // EthIf_MainFunctionState();
    // EthIf_MainFunctionRx_EthIfPhysCtrlRxMainFunctionPriorityProcessing();
    // TcpIp_MainFunction();
    // SoAd_MainFunction();
    TerminateTask();                                    /* PRQA S 3200 */
}

/*********************************************************************************************************************
* Function Name:     OsTask_5ms
* Return Value:      None
* Parameters List:   None
* Description:       5ms Task.
*********************************************************************************************************************/
TASK(osTask_5ms)        /* PRQA S 1503 */
{
    /* -- 5 ms Task -- */
    // DoIP_MainFunction();
    Dcm_MainFunction();
    Fee_MainFunction();
    Fls_MainFunction();
    TerminateTask();                    /* PRQA S 3200 */
}

/*********************************************************************************************************************
* Function Name:     OsTask_10ms
* Return Value:      None
* Parameters List:   None
* Description:       10ms Task.
*********************************************************************************************************************/
TASK(osTask_10ms)       /* PRQA S 1503 */
{
    /* -- 10 ms Task -- */
    Fbl_BootM_MainFunction();
    CanTp_MainFunction();
    BLSM_MainFunction();
    TerminateTask();                                /* PRQA S 3200 */
}


/*********************************************************************************************************************
* Function Name:     OsTask_15ms
* Return Value:      None
* Parameters List:   None
* Description:       15ms Task.
*********************************************************************************************************************/
TASK(osTask_15ms)       /* PRQA S 1503 */
{
    /* -- 15 ms Task -- */
    Fbl_DataM_MainFunction();
    NvM_MainFunction();
    BLSM_MainFunction();
    TerminateTask();                            /* PRQA S 3200 */
}


/*********************************************************************************************************************
* Function Name:     OsTask_20ms
* Return Value:      None
* Parameters List:   None
* Description:       20ms Task.
*********************************************************************************************************************/
TASK(osTask_20ms)       /* PRQA S 1503 */
{
    /* -- 20 ms Task -- */
    Fbl_WdgM_Mainfunction();
    CanSM_MainFunction();
    // EthSM_MainFunction();
    Fbl_BLSM_MainFunction();
    Sleep_MonitorMainFunction();
    TerminateTask();                                        /* PRQA S 3200 */
}


/*********************************************************************************************************************
* Function Name:     BIP OS Tasks scheduled by Rte_ScheduleTable
* Return Value:      None
* Parameters List:   None
* Description:       FBL non-use Tasks
*********************************************************************************************************************/
TASK(OsTask_ECU_Startup)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_Startup)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_BSW_SwcRequest)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_DRE)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_BswBg_1ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_BswBg_100ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_BSW_1ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_BSW_10ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_ASW_10ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_ASW_50ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_SysCore_ASW_100ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_AppCore_ASW_10ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_AppCore_BSW_10ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_ComCore_ASW_10ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}

TASK(OsTask_ComCore_BSW_10ms)       /* PRQA S 1503 */
{
    TerminateTask();                                        /* PRQA S 3200 */
}
