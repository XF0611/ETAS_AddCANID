/*********************************************************************************************************************
*
* Description:     BLSM module implementation
* FileName:        BLSM_CallOuts.c
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
UCN1HC          07/07/2019
**********************************************************************************************************************/
/*********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "PduR.h"
#include "CanIf.h"
#include "CanTp.h"
#include "CanSM.h"
#include "NvM.h"
#include "Fee.h"
#include "MemIf.h"
#include "Dcm.h"
//#include "CanTrcv.h"
#include "FBL_DataM.h"
#include "FBL_BootM.h"
#include "BLSM_CallOuts.h"
#include "BLSM.h"
#include "EcuMgt.h"
#include "Sleep.h"

// #include "Eth.h"
// #include "EthIf.h"
// #include "EthSM.h"
// #include "DoIP.h"
// #include "SoAd.h"
// #include "TcpIp.h"
// #include "DoIP_Cbk.h"

/*********************************************************************************************************************
 * Local Defines
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * Local Macros
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * Local Types
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * Exported Functions Implementation
 *********************************************************************************************************************/

/**
 * @brief the first initialization of BLSM which is to initialise low level of MCU.
 * @return none
 *
 */
extern uint32 tyFlg;
extern uint32 tyI;
extern uint32 tyAddr;
extern uint8 tyRead[4096];
void Fbl_BLSM_InitOne(void) /* PRQA S 1503 */
{
    NvM_Rb_StatusType NvM_ReadAllStatusflag;
    MemIf_StatusType stMemIf_en;
    /*EcuMgr Initialization*/
    EcuMgt_Init();

    if(tyFlg == 1)//tan9sgh test case
    {
        tyAddr = 0;
        /* page read with Flash direct address. */
        for(tyI = 0; tyI < 16; tyI++)
        {
            Fls_Read(tyAddr, &tyRead[tyI*256], 256);
            Fls_MainFunction();
            tyAddr += 0x100;
        }
        tyAddr = 0;
    }

    /*Fee_Init*/
    (void)Fee_Init(NULL_PTR);
    (void)Fee_Rb_EndInit();
    /*Nvm initialization */
    NvM_Init();
    /*DataM Init */
    Fbl_DataM_Init();
    /*Non-volatile memory read */
    NvM_ReadAll();
    do
    {
        NvM_MainFunction();
        MemIf_Rb_MainFunction();
        (void)NvM_Rb_GetStatus(&NvM_ReadAllStatusflag);
        stMemIf_en = MemIf_Rb_GetStatus();
    } while ((NvM_ReadAllStatusflag == NVM_RB_STATUS_BUSY) || (stMemIf_en == MEMIF_BUSY));

    /*Check the Ecu State.*/
    Fbl_BootM_CheckEcuState();
}


/**
 * @brief the second initialization of BLSM which is to initialise the communication
 *          and its routine.
 * @return none
 */
void Fbl_BLSM_InitTwo(void) /* PRQA S 1503 */
{
    /*Intitialize CanTrcv */
    // CanTrcv_Init(NULL_PTR);

    /*Initialize CanIf */
    CanIf_Init(NULL_PTR);
    /*Initialize CanSM */
    CanSM_Init(NULL_PTR);
    /*Initailize CanTp */
    CanTp_Init(NULL_PTR);
    /*Initialize EthIf*/
    // EthIf_Init(NULL_PTR);
    /* Initialize ethsm*/
    // EthSM_Init();
    /*Initalize Tcpip*/
    // TcpIp_Init(NULL_PTR);
    /*Initalize SoAd*/
    // SoAd_Init(NULL_PTR);                                        /* PRQA S 3320 */
    /*Doip Init*/
    // DoIP_Init(&DoIP_Config);
    /*Initialize PduR */
    PduR_Init(NULL_PTR);
    /*Initialize DCM module*/
    Dcm_Init(NULL_PTR);


}


/**
 * @brief the third initialization of BLSM which is to initialise UDS service and its dependencies.
 * @return none.
 */
void Fbl_BLSM_InitThree(void) /* PRQA S 1503 */
{
    // DoIP_ActivationLineSwitchActive();      /* PRQA S 3200 */ /* MISRA DEVIATION: Don't need return value because there is nothing we can do with it anyway. */
    /*Start the sleep timer*/
    Sleep_StartStopTimer();
}


/**
 * @brief initialise bootloader Application.
 * @return None
 *
 */
void Fbl_BLSM_Initialize_App(void) /* PRQA S 1503 */
{
    Fbl_BootM_Init();
}
