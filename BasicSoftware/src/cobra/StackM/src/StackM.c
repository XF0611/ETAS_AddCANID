/*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
* The reproduction, distribution and utilization of this document as well as the communication of its contents to
* others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
* All rights reserved in the event of the grant of a patent, utility model or design.
*
**********************************************************************************************************************
 ************************************************************************************************
 * Component : StackM.c
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Stack monitor functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

/*! \file
 *
 * \brief Stack Monitor component.
 *
 * This component perform monitoring and measurement of the worst case stack consumption during runtime.
 * AUTOSAR multicore systems or multi stack operating systems are supported.
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */
#include "StackM_Prv.h"
#include "Det.h"

/* Include RTE header to access input structure */
#include "SchM_StackM.h"
#include "Rte_StackM.h"


#if(STACKM_CFG_ENABLE == STD_ON)
/*
 ***********************************************************************************************************************
 * Variable/Data definitions
 ***********************************************************************************************************************
 */
#define StackM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "StackM_MemMap.h"
VAR(StackM_StackDynType, STACKM_VAR_CLEARED) StackM_StackDynTable[OS_NUM_OS_CORES][STACKM_NUMBEROFSTACK];
#define StackM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "StackM_MemMap.h"

#define StackM_START_SEC_CONST_UNSPECIFIED
#include "StackM_MemMap.h"
CONST(StackM_StackCfgType, STACKM_VAR_CONST) StackM_StackCfgTable[OS_NUM_OS_CORES][STACKM_NUMBEROFSTACK] = {STACKM_DEFINE_STACKTABLE};
#define StackM_STOP_SEC_CONST_UNSPECIFIED
#include "StackM_MemMap.h"


/*
 ***********************************************************************************************************************
 * Code
 ***********************************************************************************************************************
 */
#define StackM_START_SEC_CODE
#include "StackM_MemMap.h"

/** StackM_lInitCoreStack **********************************************************************************************/
/*!
 * \brief       Private function for initializing stacks by filling with predefined pattern.
 *
 * \param     : coreId  - Core callint this function.
 *            : StackId - The stack to be initialized.
 * \return    : STACKM_OK - No error
 *            : STACKM_NOK_LIMIT - Stack configuration incorrect.
 *
 */
LOCAL_INLINE StackM_ReturnType StackM_lInitCoreStack(uint32 coreId, uint32 StackId)
{
    StackM_ReturnType RetVal = STACKM_OK;
    StackM_StackDynType* pDynStack = &StackM_StackDynTable[coreId][StackId];
    const StackM_StackCfgType* pCfgStack = &StackM_StackCfgTable[coreId][StackId];

    /* ensure the "low" part of the keyword is not out of the stack. */
    if(   (pCfgStack -> Limit0 == pCfgStack -> Start)
        ||(pCfgStack -> Limit1 == pCfgStack -> Start)
        ||(pCfgStack -> Limit2 == pCfgStack -> Start))
    {
        RetVal = STACKM_NOK_LIMIT;
    }
    else
    {
        /* filling up the stack within int lock. */
        SchM_Enter_StackM_Init();

        #if(STACKM_CFG_ENABLETRACE == STD_ON)    

        /*
         * initialization of the dynamic stack structure
         */
        /* initialize the scan result to the lowest satck point. */
        pDynStack -> ScanResult = STACKM_STACKPOINT_LOWEST;

        /* the scan will start from the last stack element */
        pDynStack -> CurrentAddress = pCfgStack -> End;

        #if(STACKM_CFG_PATTERNINIT == STD_ON)
        {
            uint32* i;

            /* 
             * Definition of the local variable spNow.
             * - It allows to find out which is the last position of the current stack pointer.
             * - This is defined as a macro for simulation purpose.
             */
            uint32 spNow;

            if(StackM_IsSPWithinStack(coreId, StackId, &spNow))
            {
                /* 
                 * Fill up the remaining stack with the pattern.
                 * - an security offset is added (STACKM_PRV_OFFSET) in order.
                 * - to ensure that the stack consumption of the init function is not overwritten.
                 */
                for(i = (uint32 *)(&spNow - STACKM_PRV_OFFSET); i >= pCfgStack -> End; i--)
                {
                    *i = STACKM_CFG_DEFAULTFILLUPVALUE;
                }
            }
            else
            {
                /* 
                 * Fill up the whole stack with the pattern.
                 * - stack pointer doesn't belong to the stack.
                 * - the init function must be called before this stack is used.
                 */
                for(i = pCfgStack -> Start; i >= pCfgStack -> End; i--)
                {
                    *i = STACKM_CFG_DEFAULTFILLUPVALUE;
                }
            }
        }
        #endif /* STACKM_CFG_PATTERNINIT == STD_ON */
        #endif /* STACKM_CFG_ENABLETRACE == STD_ON */

        /* Fill up the limits if they are configured. */
        /* 2*32bit words at configured adress and on the next word. */
        if(pCfgStack -> Limit0 != STACKM_LIMIT_NOTUSED)
        {
            *(pCfgStack -> Limit0    ) = STACKM_CFG_LIMITHIGH;
            *(pCfgStack -> Limit0 + 1) = STACKM_CFG_LIMITLOW;
        }

        if(pCfgStack -> Limit1 != STACKM_LIMIT_NOTUSED)
        {
            *(pCfgStack -> Limit1    ) = STACKM_CFG_LIMITHIGH;
            *(pCfgStack -> Limit1 + 1) = STACKM_CFG_LIMITLOW;
        }

        if(pCfgStack -> Limit2 != STACKM_LIMIT_NOTUSED)
        {
            *(pCfgStack -> Limit2    ) = STACKM_CFG_LIMITHIGH;
            *(pCfgStack -> Limit2 + 1) = STACKM_CFG_LIMITLOW;
        }

        /* Stack structure is now initialized and ready to be used. */
        pDynStack -> IniFlag = TRUE;

        SchM_Exit_StackM_Init();
    }

    return RetVal;
}

/** StackM_InitCoreStack **********************************************************************************************/
/*!
 * \brief       Initializing function for core stacks for Stack Monitor
 *
 * \param       void
 * \return      void
 * The function will be called in each core in which stacks are configured.
 * It shall be called by each core before starting the OS.
 * If AUTOSAR Memory protection for stack is used, it shall be called before MPU setup.
 */
FUNC(void, STACKM_CODE) StackM_InitCoreStack(void)
{
    StackM_ReturnType RetVal = STACKM_OK;
    uint32 StackId;
    uint32 coreId = StackM_GetCoreId();

    for(StackId=0; StackId<STACKM_NUMBEROFSTACK; StackId++)
    {
        if(StackM_StackCfgTable[coreId][StackId].CoreId == coreId)
        {
            /* stack is initialized if it belongs to the current core */
            RetVal = StackM_lInitCoreStack(coreId, StackId);

#if (STACKM_DEV_ERROR_DETECT != STD_OFF)
            if(RetVal != STACKM_OK)
            {
                STACKM_DET_REPORT_ERROR(STACKM_SID_INIT, STACKM_E_LIMIT);
            }
#endif
        }
    }
}

/** StackM_InitAllCoreStack *******************************************************************************************/
/*!
 * \brief       Initializing function for all core stacks for Stack Monitor
 *
 * \param       void
 * \return      void
 * The function will be called in master core.
 * It shall be called by each core before starting the OS.
 * If AUTOSAR Memory protection for stack is used, it shall be called before MPU setup.
 */
FUNC(void, STACKM_CODE) StackM_InitAllCoreStack(void)
{
    StackM_ReturnType RetVal = STACKM_OK;
    uint32 StackId;
    uint32 coreCnt;

    for(coreCnt = 0; coreCnt <OS_NUM_OS_CORES; coreCnt++)
    {
        for(StackId=0; StackId<STACKM_NUMBEROFSTACK; StackId++)
        {
            if(StackM_StackCfgTable[coreCnt][StackId].CoreId == coreCnt)
            {
                /* stack is initialized if it belongs to the current core */
                RetVal = StackM_lInitCoreStack(coreCnt, StackId);

#if (STACKM_DEV_ERROR_DETECT != STD_OFF)
                if(RetVal != STACKM_OK)
                {
                    STACKM_DET_REPORT_ERROR(STACKM_SID_INIT, STACKM_E_LIMIT);
                }
#endif
            }
        }
    }
}

/** StackM_lScanStack **********************************************************************************************/
/*!
 * \brief       Private function for scanning core stacks for Stack Monitor
 *
 * \param     : coreId  - Core callint this function.
 *            : StackId - The stack to be initialized.
 * \return    : STACKM_OK - No error
 *            : STACKM_NOK_INIT - StackM is not initialized before scanning.
 *
 */
#if(STACKM_CFG_ENABLETRACE == STD_ON)
LOCAL_INLINE StackM_ReturnType StackM_lScanStack(uint32 coreId, uint32 StackId)
{
    StackM_ReturnType RetVal = STACKM_OK;
    StackM_StackDynType* pDynStack = &StackM_StackDynTable[coreId][StackId];
    const StackM_StackCfgType* pCfgStack = &StackM_StackCfgTable[coreId][StackId];
    boolean IsSkipAddrScan = FALSE;

    if(pDynStack -> IniFlag != TRUE)
    {
        /* stack entry was not initialized, don't scan it. */
        RetVal = STACKM_NOK_INIT;
    }
    else
    {
        if(pCfgStack->SkipAddrScan_Cbk != STACKM_CBK_NOTUSED)
        {
            IsSkipAddrScan = pCfgStack->SkipAddrScan_Cbk(pDynStack->CurrentAddress);
        }

        /* scan until the pattern is not more recognized. */
        /* if limits are available, they must also be taken in account. */
        SchM_Enter_StackM_Scan();
        if ((IsSkipAddrScan == FALSE) &&
            (  (*(pDynStack -> CurrentAddress) == STACKM_CFG_DEFAULTFILLUPVALUE)
             ||(*(pDynStack -> CurrentAddress) == STACKM_CFG_LIMITHIGH)
             ||(*(pDynStack -> CurrentAddress) == STACKM_CFG_LIMITLOW)
            ))
        {
            pDynStack -> CurrentAddress++;
        }
        else
        {
            if(pDynStack->CurrentAddress == 0)
            {
                /* Stack not filled by user yet, however scanning address overflows from 0xFFFFFFFFUL. */
                pDynStack -> ScanResult = pCfgStack->Start;
            }
            else
            {
                /* last used stack word was reached -> store the result. */
                pDynStack -> ScanResult = pDynStack -> CurrentAddress;
            }
            /* the scan will restart from the last stack element. */
            pDynStack -> CurrentAddress = pCfgStack -> End;
        }
        SchM_Exit_StackM_Scan();
    }

    return RetVal;
}
#endif

/** StackM_ScanCoreStack **********************************************************************************************/
/*!
 * \brief       Scanning core stacks for Stack Monitor
 *
 * \param       void
 * \return      void
 * This function will be called in each core for which stacks are configured.
 * It shall be called in a low priority cyclically task or in an idle task.
 */
FUNC(void, STACKM_CODE) StackM_ScanCoreStack(void)
{
    StackM_ReturnType RetVal = STACKM_OK;
        
#if(STACKM_CFG_ENABLETRACE == STD_ON)
    {
        uint32 StackId;
        uint32 coreId = StackM_GetCoreId();

        for(StackId=0; StackId<STACKM_NUMBEROFSTACK; StackId++)
        {
            if(StackM_StackCfgTable[coreId][StackId].CoreId == coreId)
			{
				/* stack is scanned if it belongs to the current core. */
				RetVal = StackM_lScanStack(coreId, StackId);

#if (STACKM_DEV_ERROR_DETECT != STD_OFF)
				if(RetVal != STACKM_OK)
				{
					STACKM_DET_REPORT_ERROR(STACKM_SID_SCANSTACK, STACKM_E_NO_INIT);
				}
#endif
			}
        }
    }
#endif
}

/** StackM_GetStackInfo **********************************************************************************************/
/*!
 * \brief       Retrive measured result of core stacks.
 *
 * \param     : coreId  - Core ID whose stack information to retrive.
              : StackId - The stack of the core to be retrive.
              : pStackInfo - the data pool which to store the retrive information.
 * \return    : STACKM_OK - No error
 *            : STACKM_NOK_ID - stack entry is out of range
 *            : STACKM_NOK_INIT - stack entry was not initialized.
 *
 */
FUNC(Std_ReturnType, STACKM_CODE) StackM_GetStackInfo(uint32 coreId, uint8 StackId, StackM_StackMeasureType* pStackInfo)
{
    StackM_ReturnType RetVal = STACKM_OK;
    StackM_StackDynType* pDynStack = &StackM_StackDynTable[coreId][StackId];
    const StackM_StackCfgType* pCfgStack = &StackM_StackCfgTable[coreId][StackId];

    if(StackId >= STACKM_NUMBEROFSTACK)
    {
        /* stack entry is out of range. */
        RetVal = STACKM_NOK_ID;
    }
    else if(pDynStack -> IniFlag != TRUE)
    {
        /* stack entry was not initialized. */
        RetVal = STACKM_NOK_INIT;
    }
    else
    {
        uint8 CheckWordReached = 0;

        if(   (pCfgStack -> Limit0 != STACKM_LIMIT_NOTUSED)
            &&(   (*(pCfgStack -> Limit0    ) != STACKM_CFG_LIMITHIGH)
                ||(*(pCfgStack -> Limit0 + 1) != STACKM_CFG_LIMITLOW))
          )
        {
            CheckWordReached |= 0x01;
        }

        if(   (pCfgStack -> Limit1 != STACKM_LIMIT_NOTUSED)
            &&(   (*(pCfgStack -> Limit1    ) != STACKM_CFG_LIMITHIGH)
                ||(*(pCfgStack -> Limit1 + 1) != STACKM_CFG_LIMITLOW))
          )
        {
            CheckWordReached |= 0x02;
        }

        if(   (pCfgStack -> Limit2 != STACKM_LIMIT_NOTUSED)
            &&(   (*(pCfgStack -> Limit2    ) != STACKM_CFG_LIMITHIGH)
                ||(*(pCfgStack -> Limit2 + 1) != STACKM_CFG_LIMITLOW))
          )
        {
            CheckWordReached |= 0x04;
        }

        pStackInfo->CheckWordReached = CheckWordReached;

        #if(STACKM_CFG_ENABLETRACE == STD_ON)
        {
            if(pDynStack->ScanResult == STACKM_STACKPOINT_LOWEST)
            {
                /* scan not started yet. */
                pStackInfo->FreeSpace = 0xFFFFFFFF;
                pStackInfo->FreePercent = 0.0;
            }
            else
            {
                pStackInfo->FreeSpace = sizeof(uint32)*((uint32)(pDynStack->ScanResult - pCfgStack->End));
                pStackInfo->FreePercent = (float32)(pStackInfo->FreeSpace>>2) / (float32)(pCfgStack->Start - pCfgStack->End);
            }
        }
        #else
            pStackInfo->FreeSpace = 0xFFFFFFFF;
            pStackInfo->FreePercent = 0.0;
        #endif
    }

    return RetVal;
}

/** StackM_ScanAllStacks **********************************************************************************************/
/*!
 * \brief       Scanning all stacks for Stack Monitor
 *
 * \param       void
 * \return      void
 *
 */
FUNC(void, STACKM_CODE) StackM_ScanAllStacks(void)
{
    uint32 StackId;
    uint32 coreCnt;
    StackM_ReturnType RetVal = STACKM_OK;

#if(STACKM_CFG_ENABLETRACE == STD_ON)
    {
		for(coreCnt = 0; coreCnt <OS_NUM_OS_CORES; coreCnt++)
		{
			for(StackId=0; StackId<STACKM_NUMBEROFSTACK; StackId++)
			{
				if(StackM_StackCfgTable[coreCnt][StackId].CoreId == 0xFFFFFFFF)
				{
					RetVal = COREID_NOTUSED;
				}
				else /* stack is initialized if it belongs to the current core */
				{   
					RetVal = StackM_lScanStack(coreCnt, StackId);
				}

#if (STACKM_DEV_ERROR_DETECT != STD_OFF)
				if(RetVal != STACKM_OK)
				{
					STACKM_DET_REPORT_ERROR(STACKM_SID_SCANSTACK, STACKM_E_NO_INIT);
				}
#endif
			}
		}
    }
#endif
}
#define StackM_STOP_SEC_CODE
#include "StackM_MemMap.h"


#endif /* STACKM_CFG_ENABLE == STD_ON */

/*
 ***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************
*/

