 

#include "Dem_Internal.h"
#include "Dem_Lib.h"
#include "Dem_Nvm.h"

#include "Dem_Bfm.h"

#include "Dem_EvMem.h"
#include "Dem_DisturbanceMemory.h"
#include "Dem_EventStatus.h"
#include "Dem_GenericNvData.h"
#include "Dem_Obd.h"
#if (DEM_CFG_ERASE_ALL_DATA_NOT_EQUAL_TO_ERASE_ALL_OFF)
#include "Dem_PrjEraseAllNvMDataHandling.h"
#endif


#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

DEM_ARRAY_DEFINE(Dem_NvMBlockStatusDoubleBufferType, Dem_NvMBlockStatusDoubleBuffer, DEM_NVM_CFG_NUM_BLOCKS_TOTAL);
DEM_ARRAY_DEFINE(Dem_NvMStorageBufferType, Dem_NvMStorageBuffer, DEM_NVM_CFG_NUM_STORAGEBUFFER);

Dem_EraseAllStatusType Dem_EraseAllNvMDataStatus;
/* MR12 RULE 14.3 VIOLATION: The operand depends on configuration and is not a constant. */
DEM_STATIC_ASSERT((DEM_ERASE_ALL_STATUS_NOREQUEST == 0), DEM_ERASE_ALL_STATUS_NOREQUEST_MUST_BE_ZERO);
boolean Dem_NvMAnyClearFailed;

#if(DEM_CFG_TRIGGER_TO_STORE_NVM_SUPPORTED)
boolean Dem_NvMImmediateStorageRequested;
#endif

#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

#define DEM_START_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"

// Call MACRO DEM_DONT_RESET_NVM
#if (DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
DEM_ARRAY_DEFINE(Dem_NvmHashIdType, Dem_HashIdInNvm, DEM_NVM_HASH_ID_SIZE);
#endif
#define DEM_STOP_SEC_VAR_SAVED_ZONE
#include "Dem_MemMap.h"


#define DEM_START_SEC_CONST
#include "Dem_MemMap.h"

/* MR12 RULE 20.7 VIOLATION: Functions like MACROS do not need to be parenthesized.*/
DEM_ARRAY_DEFINE_CONST(NvM_BlockIdType, Dem_NvMBlockMap2NvmId, DEM_NVM_CFG_NUM_BLOCKS_TOTAL, DEM_NVM_CFG_NVM_BLOCK_IDS);
/* MR12 RULE 20.7, DIR 1.1 VIOLATION:
   20.7: Functions like MACROS do not need to be parenthesized.
   1.1: Address pointer is declared as void pointer as it is required for mem copy function.
*/
DEM_ARRAY_DEFINE_CONST(Dem_NvmBlockIdExtendedType, Dem_NvmBlockIdExtended, DEM_NVM_CFG_NUM_BLOCKS_EXTENDED, DEM_NVM_CFG_BLOCKS_EXTENDED);
#if (DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
/* MR12 RULE 20.7 VIOLATION: Due to the fact that array content and array size are both generated based on the same input it is ensured that the data always fit in to the array */
DEM_ARRAY_DEFINE_CONST(Dem_NvmHashIdType, Dem_HashIdInRom, DEM_NVM_HASH_ID_SIZE, DEM_HASH_ID_ROM);
#endif
DEM_ARRAY_DEFINE_CONST(Dem_NvmCopyFunction, DemCopyFctPtrTable, DEM_NVM_CFG_COPYFCT_NUM, DEM_NVM_CFG_COPYFCT_INIT);

#define DEM_STOP_SEC_CONST
#include "Dem_MemMap.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"


DEM_INLINE boolean Dem_NvMBlockIsUsingStorageBuffer(Dem_NvmBlockIdType demNvMId)
{
    return (demNvMId < DEM_NVM_CFG_NUM_BLOCKS_EXTENDED);
}


DEM_INLINE void Dem_NvMBlockStatusClearBit(Dem_NvmBlockIdType demNvMId, Dem_NvmBlockStatusType bitmask)
{
    Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_INTERNAL] &= (Dem_NvmBlockStatusType)(~bitmask);
}


DEM_INLINE void Dem_NvMBlockStatusSetBit(Dem_NvmBlockIdType demNvMId, Dem_NvmBlockStatusType bitmask)
{
    Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_INTERNAL] |= bitmask;
}


DEM_INLINE boolean Dem_NvMBlockStatusIsBitSet(Dem_NvmBlockIdType demNvMId, Dem_NvmBlockStatusType bitmask)
{
    return ((Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_INTERNAL] & bitmask) != 0);
}


DEM_INLINE void Dem_NvMBlockStatusDoubleBufferHandling(void)
{
	/* BSW-15297 */ /* [$DD_BSWCODE 36501] */
    Dem_NvmBlockIdType demNvMId;
	/* END BSW-15297 */

    for(demNvMId = 0; demNvMId < DEM_NVM_CFG_NUM_BLOCKS_TOTAL; demNvMId++)
    {
        /* Shutdown */
        if(Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_SHUTDOWN] != DEM_NVM_STATUSBITMASK_NONE)
        {
            Dem_NvMBlockStatusSetBit(demNvMId, Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_SHUTDOWN]);
            Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_SHUTDOWN] = DEM_NVM_STATUSBITMASK_NONE;
        }
        /* Immediate */
        if(Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_IMMEDIATE] != DEM_NVM_STATUSBITMASK_NONE)
        {
            Dem_NvMBlockStatusSetBit(demNvMId, Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_IMMEDIATE]);
            Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_IMMEDIATE] = DEM_NVM_STATUSBITMASK_NONE;
        }
        /* Clear */
        if(Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_CLEAR] != DEM_NVM_STATUSBITMASK_NONE)
        {
            Dem_NvMBlockStatusSetBit(demNvMId, Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_CLEAR]);
            Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_CLEAR] = DEM_NVM_STATUSBITMASK_NONE;
        }
        /* Exclude */
        if(Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_EXCLUDE] != DEM_NVM_STATUSBITMASK_NONE)
        {
            Dem_NvMBlockStatusSetBit(demNvMId, DEM_NVM_STATUSBITMASK_EXCLUDE);
        }
        else
        {
            Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_EXCLUDE);
        }
    }
}


DEM_INLINE Dem_NvMStorageBufferType* Dem_NvMCopyToStorageBuffer(Dem_NvmBlockIdType demNvMId)
{
    uint8 storageBufferId;
    uint8 copyfctpIndex;

    if(demNvMId < DEM_NVM_CFG_NUM_BLOCKS_EXTENDED)
    {
        for(storageBufferId = 0; storageBufferId < DEM_NVM_CFG_NUM_STORAGEBUFFER; storageBufferId++)
        {
            if(!Dem_NvMStorageBuffer[storageBufferId].busy)
            {
                Dem_NvMStorageBuffer[storageBufferId].busy = TRUE;
                Dem_NvMStorageBuffer[storageBufferId].id = demNvMId;

                copyfctpIndex = Dem_NvmBlockIdExtended[demNvMId].copyFunctionPointerIndex;

                /* MR12 DIR 1.1 VIOLATION: Address pointer is declared as void pointer as it is required for mem copy function */
                (*DemCopyFctPtrTable[copyfctpIndex])(&(Dem_NvMStorageBuffer[storageBufferId].data), Dem_NvmBlockIdExtended[demNvMId].ramAdress, Dem_NvmBlockIdExtended[demNvMId].nvmBlockSize);

                return &Dem_NvMStorageBuffer[storageBufferId];
            }
        }
    }
    else
    {
        DEM_DET(DEM_DET_APIID_NVMSTATEMACHINE, 0, 0);
    }

    return NULL_PTR;
}


DEM_INLINE void Dem_NvMReleaseStorageBuffer(Dem_NvMStorageBufferType* storageBuffer)
{
    storageBuffer->busy = FALSE;
}


DEM_INLINE void Dem_NvMReleaseCompletedStorageBufferLocations(void)
{
    uint8 storageBufferId;

    for(storageBufferId = 0; storageBufferId < DEM_NVM_CFG_NUM_STORAGEBUFFER; storageBufferId++)
    {
        if(Dem_NvMStorageBuffer[storageBufferId].busy)
        {
            if(Dem_NvmGetStatus (Dem_NvMStorageBuffer[storageBufferId].id) != DEM_NVM_PENDING)
            {
                Dem_NvMReleaseStorageBuffer(&Dem_NvMStorageBuffer[storageBufferId]);
            }
        }
    }
}


DEM_INLINE Std_ReturnType Dem_NvMWriteImmediate(Dem_NvmBlockIdType demNvMId)
{
    Std_ReturnType returnValue = E_NOT_OK;
    Dem_NvMStorageBufferType* demNvMStorageBuffer = NULL_PTR;

    if(Dem_NvMBlockIsUsingStorageBuffer(demNvMId))
    {
        demNvMStorageBuffer = Dem_NvMCopyToStorageBuffer(demNvMId);
        if(demNvMStorageBuffer != NULL_PTR)
        {
            /* MR12 DIR 1.1 VIOLATION: Address pointer is declared as void pointer as it is required for mem copy function */
            returnValue = NvM_WriteBlock(Dem_NvMBlockMap2NvmId[demNvMId], &(demNvMStorageBuffer->data));
            if(returnValue != E_OK)
            {
                Dem_NvMReleaseStorageBuffer(demNvMStorageBuffer);
            }
        }
    }
    else
    {
        returnValue = NvM_WriteBlock(Dem_NvMBlockMap2NvmId[demNvMId], NULL_PTR);
    }
    return returnValue;
}


/* Set changed flag for blocks which are always written on shutdown */
DEM_INLINE void Dem_NvMMarkUntrackedBlocksForShutdown(void)
{
    Dem_NvMWriteBlockOnShutdown(DEM_NVM_ID_EVT_STATUSBYTE);

#if (DEM_CFG_FFPRESTORAGESUPPORT == DEM_CFG_FFPRESTORAGESUPPORT_ON)
    /* Store prestored freeze frame in NVRAM*/
    Dem_PreStoredFFShutdown();
#endif

#if (DEM_CFG_EVT_INDICATOR == DEM_CFG_EVT_INDICATOR_ON)
    Dem_NvMWriteBlockOnShutdown(DEM_NVM_ID_INDICATOR_ATTRIBUTES);
#endif

#if (DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)
    Dem_NvMWriteBlockOnShutdown(DEM_NVM_ID_DISTURBANCE_MEMORY);
#endif
}


#if(DEM_CFG_TRIGGER_TO_STORE_NVM_SUPPORTED)
Std_ReturnType Dem_TriggerStorageToNvm(void)
{
    Dem_NvMMarkUntrackedBlocksForShutdown();
    Dem_NvMSetImmediateStorageRequested(TRUE);
    return E_OK;
}


DEM_INLINE void Dem_NvMTriggerImmediateWrite4All(boolean writeAllBlocks)
{
    Dem_NvmBlockIdType demNvMId;
    for(demNvMId = 0; demNvMId < DEM_NVM_CFG_NUM_BLOCKS_TOTAL; demNvMId++)
    {
        if( Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_EXCLUDE) )
        {
            continue;
        }

        if( (Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_SHUTDOWN)) || (writeAllBlocks))
        {
            Dem_NvMBlockStatusSetBit(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE);
        }
    }
}
#endif


DEM_INLINE boolean Dem_NvMHandleStates(Dem_NvmBlockIdType demNvMId)
{
    Dem_NvmResultType nvmResult;
    boolean returnValue = TRUE;
    /* Check pending */
    nvmResult = Dem_NvmGetStatus (demNvMId);

    if(nvmResult != DEM_NVM_PENDING)
    {
        /* Last request was clear, check result */
        if( Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR_REQ) )
        {
            Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR_REQ);
            Dem_NvMAnyClearFailed = ((Dem_NvMAnyClearFailed) || (nvmResult == DEM_NVM_FAILED));
        }

        /* Last request was immediate write*/
        if( Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE_REQ))
        {
            Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE_REQ);
        }


        /* Check exclude bit */
        if(!Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_EXCLUDE))
        {
            /* Clear with immediate write */
            if( Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR) )
            {
                nvmResult = Dem_NvMWriteImmediate(demNvMId);

                if(nvmResult == E_OK)
                {
                    Dem_NvMBlockStatusSetBit(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR_REQ);
                    Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_SHUTDOWN);
                    Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE);
                    Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_INVALIDATE);
                    Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR);
                }
                else
                {
                    returnValue = FALSE;
                }
            }
            /* Clear with invalidate */
            else if( Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_INVALIDATE) )
            {
                nvmResult = NvM_InvalidateNvBlock(Dem_NvMBlockMap2NvmId[demNvMId]);

                if(nvmResult == E_OK)
                {
                    Dem_NvMBlockStatusSetBit(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR_REQ);
                    Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_INVALIDATE);
                }
                else
                {
                    returnValue = FALSE;
                }
            }
            /* Immediate write */
            else if( Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE) )
            {
                nvmResult = Dem_NvMWriteImmediate(demNvMId);

                if(nvmResult == E_OK)
                {
                    Dem_NvMBlockStatusSetBit(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE_REQ);
                    Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_SHUTDOWN);
                    Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE);
                }
                else
                {
                    returnValue = FALSE;
                }
            }
            else
            {
                /* Done in Shutdown */
            }
        }
    }
    return returnValue;
}


DEM_INLINE boolean Dem_NvMBlockIdExtendedGetNextIndex(Dem_NvmBlockIdType* it, Dem_NvmBlockIdType start)
{
    (*it)++;
    if(*it >= DEM_NVM_CFG_NUM_BLOCKS_EXTENDED)
    {
        (*it) = 0;
    }
    return ((*it) != start);
}


void Dem_NvMInit(void)
{
#if (DEM_CFG_ERASE_ALL_DATA_NOT_EQUAL_TO_ERASE_ALL_OFF)
    if(Dem_PrjEraseAllNvMDataHandling(Dem_NvmIsHashIDChanged()) == DEM_ERASE_ALL_REQUEST_ALL)
    {
        Dem_EraseAllNvMDataStatus = DEM_ERASE_ALL_STATUS_PENDING;

        /* Reset clear status */
        Dem_NvMStartClear();
    }
    else
#endif
    {
	 	/* Call rba_DemObdBasic_NvMInit if OBD is enabled */
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
        rba_DemObdBasic_NvMInit();
#endif
    }
}


void Dem_NvMMainFunction(void)
{
	/* BSW-15297 */ /* [$DD_BSWCODE 36502] */
    static Dem_NvmBlockIdType lastIndexPreviousMain = 0;
    Dem_NvmBlockIdType currentWriteLocation = lastIndexPreviousMain;
	/* END BSW-15297 */

    /* Handle StorageBuffer */
    Dem_NvMReleaseCompletedStorageBufferLocations();

    /* Copy from the different statusarrays to the internal one*/
    Dem_NvMBlockStatusDoubleBufferHandling();

#if(DEM_CFG_TRIGGER_TO_STORE_NVM_SUPPORTED)
    if(Dem_NvMIsImmediateStorageRequested())
    {
        Dem_NvMSetImmediateStorageRequested(FALSE);
        Dem_NvMTriggerImmediateWrite4All(FALSE);
    }
#endif

    /* Handle States of storagebuffer-blocks*/
    do
    {
        if(!Dem_NvMHandleStates(currentWriteLocation))
        {
            break;
        }
    } while(Dem_NvMBlockIdExtendedGetNextIndex(&currentWriteLocation, lastIndexPreviousMain));
    /* Save location for next cycle */
    lastIndexPreviousMain = currentWriteLocation;


    /* Handle States of callback-blocks*/
    for (currentWriteLocation = DEM_NVM_CFG_NUM_BLOCKS_EXTENDED; currentWriteLocation < DEM_NVM_CFG_NUM_BLOCKS_TOTAL; currentWriteLocation++)
    {
        if(!Dem_NvMHandleStates(currentWriteLocation))
        {
            break;
        }
    }

#if(DEM_CFG_ERASE_ALL_DATA_NOT_EQUAL_TO_ERASE_ALL_OFF)
    if(Dem_NvMIsInvalidateAllNVMBlocksRequested() && !Dem_NvMIsClearPending())
    {
        if(Dem_NvMAnyClearFailed)
        {
            Dem_EraseAllNvMDataStatus = DEM_ERASE_ALL_STATUS_FAILED;
        }
        else
        {
#if(DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
            if((Dem_EraseAllNvMDataStatus != DEM_ERASE_ALL_STATUS_HASHID_WRITE_PENDING)&&(Dem_EraseAllNvMDataStatus != DEM_ERASE_ALL_STATUS_SUCCESS))
            {
                DEM_MEMCPY(&Dem_HashIdInNvm,&Dem_HashIdInRom,DEM_SIZEOF_VAR(Dem_HashIdInRom));
                Dem_NvMClearBlockByWrite(DEM_NVM_ID_DEM_HASH_ID);
                Dem_EraseAllNvMDataStatus = DEM_ERASE_ALL_STATUS_HASHID_WRITE_PENDING;
            }
            else
#endif
            {
                Dem_EraseAllNvMDataStatus = DEM_ERASE_ALL_STATUS_SUCCESS;
            }
        }
    }
#endif
}


void Dem_NvMShutdown(void)
{
    Dem_NvmBlockIdType demNvMId;

    Dem_NvMMarkUntrackedBlocksForShutdown();

    Dem_NvMBlockStatusDoubleBufferHandling();

    /* If Blocks couldn't be written with MainFunction, call setRamBlockStatusChanged for all changed Blocks*/
    for(demNvMId = 0; demNvMId < DEM_NVM_CFG_NUM_BLOCKS_TOTAL; demNvMId++)
    {
        if( Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_EXCLUDE))
        {
            continue;
        }

        if( Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_SHUTDOWN)
            || Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE)
            || Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR)
            || Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_INVALIDATE))
        {
            if(NvM_Rb_SetWriteAllTrigger(Dem_NvMBlockMap2NvmId[demNvMId], TRUE) == E_OK)
            {
                Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_SHUTDOWN);
                Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE);
                Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR);
                Dem_NvMBlockStatusClearBit(demNvMId, DEM_NVM_STATUSBITMASK_INVALIDATE);
            }
            else
            {
                DEM_DET(DEM_DET_APIID_SHUTDOWN, DEM_E_INTERNAL,0u);
            }
        }
    }

#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
    rba_DemObdBasic_NvMShutdown();
#endif

}


boolean Dem_NvMIsClearPending(void)
{
    Dem_NvmBlockIdType demNvMId;
    for(demNvMId = 0; demNvMId < DEM_NVM_CFG_NUM_BLOCKS_TOTAL; demNvMId++)
    {
        if(     Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR)
            ||  Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_INVALIDATE)
            ||  Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR_REQ)
            || (Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_CLEAR] != DEM_NVM_STATUSBITMASK_NONE)
          )
        {
            return TRUE;
        }
    }
    return FALSE;
}

/* If the MARCO of DEM_MEMCPY can not be used, this standard function can support for the function point */
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
void Dem_NvMNormalMemCopy(void* Dest_pv, const void* Src_pcv, uint32 numBytes_s32)
{
    /* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(Dest_pv, Src_pcv, numBytes_s32);
}

/* check single block the clear action is pending or not */
boolean Dem_NvMIsBlockClearPending( Dem_NvmBlockIdType demNvMId)
{
    if (        Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR)
            ||  Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR_REQ)
            || (Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_CLEAR] != DEM_NVM_STATUSBITMASK_NONE)
            ||  Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_INVALIDATE)
       )
    {
        return TRUE;
    }
    return FALSE;

}


boolean Dem_NvMIsBlockAllowedToBeExcluded( Dem_NvmBlockIdType demNvMId)
{
    boolean RetVal = TRUE;

    if (    Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE_REQ)
         || Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_CLEAR_REQ)
         || Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_EXCLUDE)
       )
    {
        RetVal = FALSE;
    }

    return RetVal;
}

boolean Dem_NvMIsBlockExcluded( Dem_NvmBlockIdType demNvMId)
{
    return Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_EXCLUDE);
}




boolean Dem_NvMIsImmediatePending(void)
{
    Dem_NvmBlockIdType demNvMId;
#if(DEM_CFG_TRIGGER_TO_STORE_NVM_SUPPORTED)
    if(Dem_NvMIsImmediateStorageRequested())
    {
        return TRUE;
    }
#endif

    for(demNvMId = 0; demNvMId < DEM_NVM_CFG_NUM_BLOCKS_TOTAL; demNvMId++)
    {
        if( Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE)||
            Dem_NvMBlockStatusIsBitSet(demNvMId, DEM_NVM_STATUSBITMASK_IMMEDIATE_REQ)||
            (Dem_NvMBlockStatusDoubleBuffer[demNvMId][DEM_NVM_STATUSINDEX_IMMEDIATE] != DEM_NVM_STATUSBITMASK_NONE))
        {
            return TRUE;
        }
    }
    return FALSE;
}

#if (DEM_CFG_ERASE_ALL_DATA_NOT_EQUAL_TO_ERASE_ALL_OFF)
Dem_HashIdCheckResultType Dem_NvmIsHashIDChanged(void)
{
#if (DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
    Dem_NvmResultType NvmResult;
    uint8 i;
    Dem_HashIdCheckResultType HashCheckResult = DEM_HASH_ID_NO_CHANGE;

    NvmResult = Dem_NvmGetStatus (DEM_NVM_ID_DEM_HASH_ID);
    /* Data read successfully */
    if (NvmResult == DEM_NVM_SUCCESS)
    {
        for(i=0;i<DEM_NVM_HASH_ID_SIZE;i++)
        {
            if(Dem_HashIdInNvm[i] != Dem_HashIdInRom[i])
            {
                HashCheckResult = DEM_HASH_ID_MISMATCH;
                break;
            }
        }
    }
    else
    {
        HashCheckResult = DEM_HASH_ID_CHECK_NOT_POSSIBLE;
    }

    return HashCheckResult;
#else
    return DEM_HASH_ID_CHECK_DISABLED;
#endif
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

