
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ErrorDetection.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

Std_ReturnType NvM_Rb_GetBlockId(uint16 PersistentId, NvM_BlockIdType *BlockIdPtr)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2840] Service to get the NvM block ID using the persistent ID input parameter
    Std_ReturnType stReturn_u8 = E_NOT_OK;

    NvM_Prv_ErrorDetection_Ptr_tun ptr_un;
    ptr_un.ptrIdBlock_puo = BlockIdPtr;
    if (NvM_Prv_ErrorDetection_IsPtrValid(NVM_SERVICE_ID_RB_GET_BLOCK_ID, 0, NVM_E_PARAM_DATA, &ptr_un))
    {
        uint16 left_u16 = 0;                                         // left_u16   : [0  .. 65534]
        sint32 right_s32 = (sint32)(NVM_PRV_NR_PERSISTENT_IDS - 1u); // right_s32  : [-1 .. 65533]  ;  #Persis IDs : [0 .. 65534]
        uint16 middle_u16;                                           // middle_u16 : [0  .. 65533]

        while((sint32)left_u16 <= right_s32)
        {
            middle_u16 = left_u16 + (uint16)(((uint16)right_s32 - left_u16) / 2);

            if (PersistentId == NvM_Prv_GetIdPersistent(middle_u16))
            {
                *BlockIdPtr = NvM_Prv_GetIdBlock(middle_u16);
                stReturn_u8 = E_OK;
                break;
            }
            else if (PersistentId > NvM_Prv_GetIdPersistent(middle_u16))
            {
                left_u16 = middle_u16 + 1;
            }
            else
            {
                right_s32 = middle_u16 - 1;
            }
        }
        // The persistent ID is not found if (left_u16 > right_s32) after the while loop.
        // In this case E_NOT_OK is returned.
    }

    return stReturn_u8;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

