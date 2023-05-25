
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv.h"

#if (NVM_RB_REQ_MONITOR == NVM_RB_REQ_MON_WRITE)

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define NVM_START_SEC_VAR_SAVED_ZONE_16
#include "NvM_MemMap.h"
/**
 * Number of write requests since last power-on.
 */
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3119]
uint16 NvM_Rb_cntrWrites_u16;
#define NVM_STOP_SEC_VAR_SAVED_ZONE_16
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_SAVED_ZONE_32
#include "NvM_MemMap.h"
/**
 * Number of bytes written since last power-on.
 */
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3123]
uint32 NvM_Rb_nrBytesWritten_u32;
#define NVM_STOP_SEC_VAR_SAVED_ZONE_32
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_SAVED_ZONE_UNSPECIFIED
#include "NvM_MemMap.h"
/**
 * ID of the last written block.
 */
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3129]
NvM_BlockIdType NvM_Rb_idLastWrittenBlock_uo;
/**
 * Write request counters which will be stored in persistent way.
 */
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3128]
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3135]
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3136]
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3137]
NvM_Rb_WriteMon_tst NvM_Rb_WriteMon_st;
#define NVM_STOP_SEC_VAR_SAVED_ZONE_UNSPECIFIED
#include "NvM_MemMap.h"


/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
 */
LOCAL_INLINE void NvM_Prv_IncreaseWriteCounters(NvM_BlockIdType idBlock_uo, uint16 BlockLength_u16);

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/**
 * \brief
 * This function clears all write monitoring counters.
 *
 * \details
 * This function always returns E_OK.
 *
 * \param[global out] NvM_Rb_WriteMon_st
 * Structure to store write request counters in persistant way.
 * \param[global out] NvM_Rb_idLastWrittenBlock_uo
 * ID of the last written block.
 * \param[global out] NvM_Rb_nrBytesWritten_u32
 * Number of bytes written since last power-on.
 * \param[global out] NvM_Rb_cntrWrites_u16
 * Number of write requests since last power-on.
 *
 * \return
 * - E_OK = write monitoring counters cleared successfully
 */
Std_ReturnType NvM_Rb_ClrWriteMonCntrs(void)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3122] Service to clear write monitoring counters.

    NvM_Rb_WriteMon_st.cntrWritesAbs_u32 = 0u;
    NvM_Rb_WriteMon_st.cntrWritesMax_u16 = 0u;
    NvM_Rb_WriteMon_st.nrBytesWrittenMax_u32 = 0u;

    NvM_Rb_idLastWrittenBlock_uo = 0u;
    NvM_Rb_nrBytesWritten_u32 = 0u;
    NvM_Rb_cntrWrites_u16 = 0u;

    return E_OK;
}

/**
 * This function initializes all NvM internal counters required for write request monitoring.
 * \details
 * NvM internal counters will be initialized only if data in the saved zone is lost.
 *
 * \param[in] isSavedZoneDataLost_b
 * Status information of the data in the saved zone RAM:
 * - true = data in the saved zone RAM is lost after the reset
 * - false = data in the saved zone RAM is unchanged after the reset
 */
void NvM_Prv_InitializeWriteMonCntrs(boolean isSavedZoneDataLost_b)
{
    if (isSavedZoneDataLost_b)
    {
        // Write request monitoring
        NvM_Rb_cntrWrites_u16 = 0u;
        NvM_Rb_nrBytesWritten_u32 = 0u;
        NvM_Rb_idLastWrittenBlock_uo = 0u;
    }
}

/**
 * \brief
 * Function handels the increase the increase of the write-counters.
 *
 * \details
 * Function increases the write counter if the function itself
 * 1. is called before the CRC calculation for the current block AND the current block is the WriteMon-block itself
 * OR
 * 2. is called after the CRC calculation is finished for the current block and the block is NOT the WriteMon-block
 *
 * Reason is that the WriteMon block is always written and that for the WriteMon-block the counters have to be
 * increased before the CRC calculation step.
 *
 * \param [in] idBlock_uo
 * Block identifier of current block
 *
 * \param [in] BlockLength_u16
 * Block length of the current block
 *
 * \param [in] isCallBeforeCrcCalc
 * - TRUE: Function is called before the CRC calculation step for the current block
 * - FALSE: Function is called after the CRC calculation step for the current block
 */
void NvM_Prv_WriteMonCntrHandling(NvM_BlockIdType idBlock_uo, uint16 BlockLength_u16, boolean isCallBeforeCrcCalc_b)
{
    // Condition under which write-counters should be increased:
    //    ( (( TRUE == isCallBeforeCrcCalc_b) && (NvMConf_NvMBlockDescriptor_NvM_WriteMon == idBlock_uo)) ||
    //      ((FALSE == isCallBeforeCrcCalc_b) && (NvMConf_NvMBlockDescriptor_NvM_WriteMon != idBlock_uo)) )
    //
    // "Xor"-version of above mentioned condition to easily achieve 100% MC/DC coverage
    if ( (TRUE == isCallBeforeCrcCalc_b) != (NvMConf_NvMBlockDescriptor_NvM_WriteMon != idBlock_uo) )
    {
        NvM_Prv_IncreaseWriteCounters(idBlock_uo, BlockLength_u16);
    }
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline code
 **********************************************************************************************************************
 */
/**
 * \brief
 * This function increases all NvM internal counters required for write request monitoring.
 *
 * \details
 * All counters have an overflow protection, i.e. a counter cannot exceed the maximum value.
 *
 * \param idBlock_uo
 * ID of the last written block.
 * \param BlockLength_u16
 * Length of the last written block in bytes.
 * \param[global inout] NvM_Rb_WriteMon_st
 * Structure to store write request counters in persistant way.
 * \param[global inout] NvM_Rb_idLastWrittenBlock_uo
 * ID of the last written block.
 * \param[global in] NvM_Rb_nrBytesWritten_u32
 * Number of bytes written since last power-on.
 * \param[global in] NvM_Rb_cntrWrites_u16
 * Number of write requests since last power-on.
 */
LOCAL_INLINE void NvM_Prv_IncreaseWriteCounters(NvM_BlockIdType idBlock_uo, uint16 BlockLength_u16)
{
    // save last written block ID
    NvM_Rb_idLastWrittenBlock_uo = idBlock_uo;

    // increase the accumulated number of all write requests only if it has not reached the maximum
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3156] All counters shall have overflow protection
    if (NvM_Rb_WriteMon_st.cntrWritesAbs_u32 < NVM_PRV_CNTR_WRITES_ABS_MAX_VALUE)
    {
        ++NvM_Rb_WriteMon_st.cntrWritesAbs_u32;
    }

    // increase the number of write requests since last power-on only if it has not reached the maximum
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3156] All counters shall have overflow protection
    if (NvM_Rb_cntrWrites_u16 < NVM_PRV_CNTR_WRITES_MAX_VALUE)
    {
        ++NvM_Rb_cntrWrites_u16;

        // store new maximum value if current maximum value has been exceeded
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3156] All counters shall have overflow protection
        if (NvM_Rb_cntrWrites_u16 > NvM_Rb_WriteMon_st.cntrWritesMax_u16)
        {
            NvM_Rb_WriteMon_st.cntrWritesMax_u16 = NvM_Rb_cntrWrites_u16;
        }
    }

    // increase the number of bytes written since last power-on only if it has not reached the maximum
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3156] All counters shall have overflow protection
    if ((NVM_PRV_NR_BYTES_WRITTEN_MAX_VALUE > NvM_Rb_nrBytesWritten_u32) &&
        ((NVM_PRV_NR_BYTES_WRITTEN_MAX_VALUE - NvM_Rb_nrBytesWritten_u32) >= BlockLength_u16))
    {
        NvM_Rb_nrBytesWritten_u32 += BlockLength_u16;

        // store new maximum value if current maximum value has been exceeded
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3156] All counters shall have overflow protection
        if (NvM_Rb_nrBytesWritten_u32 > NvM_Rb_WriteMon_st.nrBytesWrittenMax_u32)
        {
            NvM_Rb_WriteMon_st.nrBytesWrittenMax_u32 = NvM_Rb_nrBytesWritten_u32;
        }
    }
}

#endif

