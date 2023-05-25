

#ifndef FEE_PRV_LIB_H
#define FEE_PRV_LIB_H

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
# if (defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

#include "Fls.h"

/*
 * The lib unit contains various helper functions that shall only be implemented once.
 * It also contains functions to limit the duration of a main function call.
 * See more about that topic below at the functions used for that.
 */

#  if ((STD_ON == FEE_DEV_ERROR_DETECT) || (FEE_AR_RELEASE_MAJOR_VERSION == 4) && (FEE_AR_RELEASE_MINOR_VERSION >=4))
#include "Det.h"
#  endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/*
 * Set the state of a state machine struct to another state.
 * Then set the entry_b variable to indicate the state is newly entered.
 * Since C neither supports templates nor inheritance this is the only way to do it type safe.
 */
#define FEE_PRV_LIBTRANS(FSM_STRUCT, FSM_STATE) \
{                                               \
    (FSM_STRUCT).state_en   = (FSM_STATE);      \
    (FSM_STRUCT).entry_b    = (TRUE);           \
}

/* Change to the new state FSM_STATE of the state machine *fsm_pst. */
#define FEE_PRV_LIBSC(FSM_STATE) FEE_PRV_LIBTRANS((*fsm_pst), (FSM_STATE))

/* Re-enter the current state of the state machine *fsm_pst from the beginning. */
#define FEE_PRV_LIBSR()  (fsm_pst->entry_b = (TRUE))

/* Check whether a state of the state machine *fsm_pst is entered for the first or a later time. */
#define FEE_PRV_LIBENTRY (Fee_Prv_LibEnter(&fsm_pst->entry_b))

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/* The following operations are considered when limiting the duration of a main function call */
typedef enum
{
    FEE_PRV_LIMIT_CRCINRAM_CPYRAM_E = 0,    /* CRC calculation in RAM or copy process from RAM to RAM           */
    FEE_PRV_LIMIT_CRCINFLS_E        = 1,    /* CRC calculation in flash                                         */
    FEE_PRV_LIMIT_HDR_E             = 2,    /* Handling one FAT entry - no matter what "handling" exactly means */
    FEE_PRV_LIMIT_CACHEREORG_E      = 3,    /* Reorganizing the cache is expensive                              */
    FEE_PRV_LIMIT_MAX_E             = 4     /* Maximum enum value used for defining an array                    */
} Fee_Prv_LibEffortLimit_ten;

/* Struct to measure the amount of already executed effort. */
typedef struct
{
    uint32  effortCtr_u32;  /* Effort that can still be spent - if 0 the main function shall not continue   */
    boolean enabled_b;      /* Effort limitation enabled, TRUE = yes, FALSE = no                            */
} Fee_Prv_LibEffortMeasure_tst;

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

#define FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"
extern Fee_Prv_LibEffortMeasure_tst         Fee_Prv_LibEffortMeasure_st;
#define FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"

#define FEE_START_SEC_CONST_8
#include "Fee_MemMap.h"
extern uint8                        const   Fee_Prv_LibEffortWeights_au8[FEE_PRV_LIMIT_MAX_E];
#define FEE_STOP_SEC_CONST_8
#include "Fee_MemMap.h"

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
 */

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"


extern void Fee_Prv_LibInit(void);

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

/*
 **********************************************************************************************************************
 * Inlines
 **********************************************************************************************************************
*/

/**
 * \brief   Forward an error to the Det module.
 *          AR4.2: If development error detection is not enabled, the errors are not forwarded to the Det module.
 *          AR4.4 and newer: If development error detection is not enabled, only runtime errors are forwarded to the Det module.
 *
 * \param   apiId_u8            The ID of the API
 * \param   FeeDeviceName_en    Device instance which had the error
 * \param   errorId_u8          The ID of the error
 */
LOCAL_INLINE void Fee_Prv_LibDetReport(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, uint8 errorId_u8)
{

#  if (FEE_AR_RELEASE_MAJOR_VERSION == 4) && (FEE_AR_RELEASE_MINOR_VERSION >=4)
    /* From Autosar 4.4 and newer */
    /* Report runtime error to Det module */
    if(FEE_E_BUSY == errorId_u8)
    {
        (void)Det_ReportRuntimeError (FEE_MODULE_ID, (uint8)FeeDeviceName_en, apiId_u8, errorId_u8);
    }
    else
    {
#   if (STD_ON == FEE_DEV_ERROR_DETECT)
        (void)Det_ReportError(FEE_MODULE_ID, (uint8)FeeDeviceName_en, apiId_u8, errorId_u8);
#   endif
    }
#  elif (STD_ON == FEE_DEV_ERROR_DETECT)
    (void)Det_ReportError(FEE_MODULE_ID, (uint8)FeeDeviceName_en, apiId_u8, errorId_u8);
#  else
    (void)apiId_u8;
    (void)errorId_u8;
#  endif
}

/**
 * \brief   Check whether a state machine's state is entered for the first or a later time.
 *
 * \param   entry_pb    Pointer to the entry_b variable of the state machine
 *
 * \retval  TRUE    State is entered for the first time
 * \retval  FALSE   State is not entered for the first, but a later time
*/
LOCAL_INLINE boolean Fee_Prv_LibEnter(boolean * entry_pb)
{
    boolean result_b = FALSE;

    /* If not already entered then set the entry_b to FALSE and return TRUE (= state is entered) */
    if(*entry_pb == TRUE)
    {
        result_b    = TRUE;
        *entry_pb   = FALSE;
    }

    return(result_b);
}

/**
 * \brief   Simply limiter.
 *
 * \param   in_u32  Value
 * \param   max_u32 Limit of the value, if smaller then the value max_u32 is returned
*/
LOCAL_INLINE uint32 Fee_Prv_LibLimit(uint32 in_u32, uint32 max_u32)
{
    uint32 result_u32 = in_u32;
    if(result_u32 > max_u32)
    {
        result_u32 = max_u32;
    }
    return(result_u32);
}

/**
 * \brief   Simple maximum chooser.
 *
 * \param   a_u32   First value
 * \param   b_u32   Second value
 *
 * \return  The bigger of both values
*/
LOCAL_INLINE uint32 Fee_Prv_LibMax(uint32 a_u32, uint32 b_u32)
{
    uint32 result_u32 = a_u32;
    if(a_u32 < b_u32)
    {
        result_u32 = b_u32;
    }
    return(result_u32);
}

/**
 * \brief   Simple minimum chooser.
 *
 * \param   a_u32   First value
 * \param   b_u32   Second value
 *
 * \return  The smaller of both values
*/
LOCAL_INLINE uint32 Fee_Prv_LibMin(uint32 a_u32, uint32 b_u32)
{
    uint32 result_u32 = a_u32;
    if(a_u32 > b_u32)
    {
        result_u32 = b_u32;
    }
    return(result_u32);
}

/**
 * \brief   Compare two 32-bit wide counters, and return the bigger considering a potential overflow.
 *
 * \param   cntr0_u32   First counter
 * \param   cntr1_u32   Second counter
 *
 * \return  The bigger of both counters
*/
LOCAL_INLINE uint32 Fee_Prv_LibMaxWithOvf(uint32 cntr0_u32, uint32 cntr1_u32)
{
    uint32  cntr0Dif1_u32 = cntr0_u32 - cntr1_u32;
    uint32  cntr1Dif0_u32 = cntr1_u32 - cntr0_u32;

    return((cntr1Dif0_u32 > cntr0Dif1_u32) ? cntr0_u32 : cntr1_u32);
}

/**
 * \brief   Compare two 32-bit wide counters, and return the smaller considering a potential overflow.
 *
 * \param   cntr0_u32   First counter
 * \param   cntr1_u32   Second counter
 *
 * \return  The smaller of both counters
*/
LOCAL_INLINE uint32 Fee_Prv_LibMinWithOvf(uint32 cntr0_u32, uint32 cntr1_u32)
{
    uint32  cntr0Dif1_u32 = cntr0_u32 - cntr1_u32;
    uint32  cntr1Dif0_u32 = cntr1_u32 - cntr0_u32;

    return((cntr1Dif0_u32 < cntr0Dif1_u32) ? cntr0_u32 : cntr1_u32);
}

/**
 * \brief   Calculate the remainder of a division (without using the C modulo operator, as this can be inefficient).
 *
 * \param   dividend_u32   numerator
 * \param   divisor_u32    denominator (unequal zero!)
 *
 * \return  The remainder of the division dividend_u32 / divisor_u32 (i.e. dividend_u32 % divisor_u32)
*/
LOCAL_INLINE uint32 Fee_Prv_LibModulo(uint32 dividend_u32, uint32 divisor_u32)
{
    return( dividend_u32 - ((dividend_u32/divisor_u32) * divisor_u32) );
}

/**
 * \brief   Convert uint32 to little endian buffer.
 *
 * \param   in_u32    uint32 that shall be converted to little endian
 * \param   out_pu8   Pointer to result buffer
*/
LOCAL_INLINE void Fee_Prv_LibU32ToLittleEndian(uint32 in_u32, uint8 * out_pu8)
{
    out_pu8[3] = (uint8)(in_u32 >> 24u);
    out_pu8[2] = (uint8)(in_u32 >> 16u);
    out_pu8[1] = (uint8)(in_u32 >> 8u );
    out_pu8[0] = (uint8)(in_u32 >> 0u );
}

/**
 * \brief   Convert uint32 to big endian buffer.
 *
 * \param   in_u32      uint32 that shall be converted to big endian
 * \param   out_pu8     Pointer to result buffer
*/
LOCAL_INLINE void Fee_Prv_LibU32ToBigEndian(uint32 in_u32, uint8 * out_pu8)
{
    out_pu8[0] = (uint8)(in_u32 >> 24u);
    out_pu8[1] = (uint8)(in_u32 >> 16u);
    out_pu8[2] = (uint8)(in_u32 >> 8u );
    out_pu8[3] = (uint8)(in_u32 >> 0u );
}

/**
 * \brief   Convert uint16 to big endian buffer.
 *
 * \param   in_u16      uint16 that shall be converted to big endian
 * \param   out_pu8     Pointer to result buffer
*/
LOCAL_INLINE void Fee_Prv_LibU16ToBigEndian(uint16 in_u16, uint8 * out_pu8)
{
    out_pu8[0] = (uint8)(in_u16 >> 8u);
    out_pu8[1] = (uint8)(in_u16 >> 0u);
}

/**
 * \brief   Create uint32 from little endian buffer.
 *
 * \param   in_pcu8    Little endian buffer that shall be converted to uint32
 * \param   out_pu32   Pointer to result buffer
*/
LOCAL_INLINE void Fee_Prv_LibU32FromLittleEndian(uint8 const * in_pcu8, uint32 * out_pu32)
{
    *out_pu32  = (((uint32)(in_pcu8[3])) << 24u);
    *out_pu32 |= (((uint32)(in_pcu8[2])) << 16u);
    *out_pu32 |= (((uint32)(in_pcu8[1])) <<  8u);
    *out_pu32 |= (((uint32)(in_pcu8[0])) <<  0u);
}

/**
 * \brief   Create uint32 from big endian buffer.
 *
 * \param   in_pcu8     Big endian buffer that shall be converted to uint32
 * \param   out_pu32    Pointer to result buffer
*/
LOCAL_INLINE void Fee_Prv_LibU32FromBigEndian(uint8 const * in_pcu8, uint32 * out_pu32)
{
    *out_pu32  = (((uint32)(in_pcu8[0])) << 24u);
    *out_pu32 |= (((uint32)(in_pcu8[1])) << 16u);
    *out_pu32 |= (((uint32)(in_pcu8[2])) <<  8u);
    *out_pu32 |= (((uint32)(in_pcu8[3])) <<  0u);
}

/**
 * \brief   Create uint16 from big endian buffer.
 *
 * \param   in_pcu8     Big endian buffer that shall be converted to uint16
 * \param   out_pu16    Pointer to result buffer
*/
LOCAL_INLINE void Fee_Prv_LibU16FromBigEndian(uint8 const * in_pcu8, uint16 * out_pu16)
{
    *out_pu16  = (uint16)(((uint32)in_pcu8[0]) << 8u);
    *out_pu16 |= (uint16)(((uint32)in_pcu8[1]) << 0u);
}

/**
 * \brief   Round value up to the next page border.
 *
 * \param   value_u32     Value to round up
 *
 * \return  Value rounded up to the next page border
 *
 * \attention   Whenever a flash must be supported which does NOT have 2^n page size this function must be changed!
*/
LOCAL_INLINE uint32 Fee_Prv_LibCeil(uint32 value_u32)
{
    /*
     * Example with value 0x12345 and page size 8.
     *
     *      0001 0010 0011 0100 0101
     *  &   0000 0000 0000 0000 0111 (VIRTUAL_PAGE_SIZE - 1)
     *  =   0000 0000 0000 0000 0101
     *  ~   1111 1111 1111 1111 1010
     * +1   1111 1111 1111 1111 1011
     *  &   0000 0000 0000 0000 0111 (VIRTUAL_PAGE_SIZE - 1)
     *  =   0000 0000 0000 0000 0011 (=3)
     *
     * Result: 0x12345 + 0x3 = 0x12348
     *
     * NOTE: VIRTUAL_PAGE_SIZE - 1 is a const expression and can be evaluated at compile time.
     * NOTE: This strategy only works with page sizes which are % 2 = 0!
     */
    uint32 summand_u32 = (((~(value_u32 & (uint32)(FEE_VIRTUAL_PAGE_SIZE - 1u)))) + 1u) & (uint32)(FEE_VIRTUAL_PAGE_SIZE - 1u);
    return(value_u32 + summand_u32);
}

/**
 * \brief   Round down an value to the next page border.
 *
 * \param   value_u32     Value to round down
 *
 * \return  Value rounded down to the next page border
 *
 * \attention   Whenever a flash must be supported which does NOT have 2^n page size this function must be changed!
*/
LOCAL_INLINE uint32 Fee_Prv_LibFloor(uint32 value_u32)
{
    /*
     * Example with value 0x12345 and page size 8.
     *
     *
     *      0000 0000 0000 0000 0111 (VIRTUAL_PAGE_SIZE - 1)
     *  ~   1111 1111 1111 1111 1000
     *  &   0001 0010 0011 0100 0101 (value)
     *  =   0001 0010 0011 0100 0000 (result)
     *
     * Result: 0x12345 => 0x12340
     *
     * NOTE: VIRTUAL_PAGE_SIZE - 1 is a const expression and can be evaluated at compile time.
     * NOTE: This strategy only works with page sizes which are % 2 = 0!
     */
    uint32 result_u32 = value_u32 & (~((uint32)(FEE_VIRTUAL_PAGE_SIZE - 1u)));
    return(result_u32);
}

/**
 * \brief   Set a piece of RAM to a specified value.
 *
 * \param   bfr_pv      Pointer to piece of RAM which shall be set
 * \param   value_cu8   New value of the RAM
 * \param   size_cu32   Size of the RAM block
 *
 * \TODO    The speed of this function could be increased.
*/
LOCAL_INLINE void Fee_Prv_LibMemSet(void * bfr_pv, const uint8 value_cu8, const uint32 size_cu32)
{
    uint32  a_u32;
    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    uint8 * bfr_pu8 = bfr_pv;

    for(a_u32 = 0u; a_u32 < size_cu32; a_u32++)
    {
        bfr_pu8[a_u32] = value_cu8;
    }
}

/**
 * \brief   Copy a piece of memory to another piece.
 *
 * \param   src_pcv     Source
 * \param   dst_pv      Destination
 * \param   size_cu32   Amount of bytes to copy
*/
LOCAL_INLINE void Fee_Prv_LibMemCopy(void const * src_pcv, void * dst_pv, const uint32 size_cu32)
{
    uint32          a_u32    = 0u;
    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    uint8   const * src_pcu8 = src_pcv;
    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    uint8         * dst_pu8  = dst_pv;

    for(a_u32 = 0u; a_u32 < size_cu32; a_u32++)
    {
        dst_pu8[a_u32] = src_pcu8[a_u32];
    }
}

/**
 * \brief   Compare two buffers.
 *
 * \param   dataBuf0_pcv   Buffer 0
 * \param   dataBuf1_pcv   Buffer 1
 * \param   nrByt_u32      Number of bytes to compare
 *
 * \retval  TRUE    The buffers contain the same data
 * \retval  FALSE   The buffers do not contain the same data
*/
LOCAL_INLINE boolean Fee_Prv_LibMemCompare(void const * dataBuf0_pcv, void const * dataBuf1_pcv, uint32 nrByt_u32)
{
    uint32          idxLoop_u32   = 0uL;
    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    uint8   const * dataBuf0_pcu8 = (uint8 const *)dataBuf0_pcv;
    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    uint8   const * dataBuf1_pcu8 = (uint8 const *)dataBuf1_pcv;
    boolean         isEqu_b       = TRUE;

    for(idxLoop_u32 = 0u; (isEqu_b) && (idxLoop_u32 < nrByt_u32); idxLoop_u32++)
    {
        if(dataBuf0_pcu8[idxLoop_u32] != dataBuf1_pcu8[idxLoop_u32])
        {
            isEqu_b = FALSE;
        }
        else
        {
            /* Continue comparision with next byte */
        }
    }

    return(isEqu_b);
}



/**
 * \brief   Find out whether a value is located within a certain range.
 *
 * \param   val_u32     Value to check
 * \param   loBound_u32 Lower bound
 * \param   hiBound_u32 Upper bound
 *
 * \pre     loBound_u32 <= hiBound_u32
 * \post    if loBound_u32 <= val_u32 <= hiBound_u32 then true else false
*/
LOCAL_INLINE boolean Fee_Prv_LibIsInRange(uint32 val_u32, uint32 loBound_u32, uint32 hiBound_u32)
{
    boolean result_b;

    if((loBound_u32 <= val_u32) && (val_u32 <= hiBound_u32))
    {
        result_b = TRUE;
    }
    else
    {
        result_b = FALSE;
    }

    return(result_b);
}

/**
 * \brief   Check whether some data is equal to the erase pattern of the flash.
 *
 * \param   bfr_pcv     Buffer to the data
 * \param   length_u32  Amount of bytes to check for the erase pattern.
*/
LOCAL_INLINE boolean Fee_Prv_LibIsErased(void const * bfr_pcv, uint32 length_u32)
{
    uint32          a_u32;
    /* MR12 RULE 11.5 VIOLATION: uint8 * alignment is not stricter than void * alignment */
    uint8   const * bfr_pcu8 = bfr_pcv;

    for(a_u32 = 0; a_u32 < length_u32; a_u32++)
    {
        if(FEE_PRV_CFG_ERASE_PATTERN_8BIT != bfr_pcu8[a_u32])
        {
            return(FALSE);
        }
    }
    return(TRUE);
}

/**
 * \brief   Set the effort counter to its initial value.
 *          That means the main function is allowed to do some work.
 *          Typically this is only called at the beginning of the main function.
*/
LOCAL_INLINE void Fee_Prv_LibEffortReset(void)
{
    Fee_Prv_LibEffortMeasure_st.effortCtr_u32 = Fee_Prv_ConfigMaxEffort_cu32;
}

/**
 * \brief   Check whether the main function is allowed to continue its work or whether it shall exit to not break timing limitations.
 *          This function is always called if the main function want to execute a time consuming action.
 *          It is possible to specify for each time consuming action what the "costs" of this action are.
 *          This function multiplies the number of time consuming actions the main function wants to do with a weight factor.
 *          Then it checks whether the main function has the right to execute this action (there was not yet so much work done).
 *          Or whether the main function already did too much and it is getting dangerous to continue regarding timing limitations.
 *          Example:
 *              The main function already calculated 2k of CRC in the RAM.
 *              Now it wants to calculate again 2k of CRC in the flash.
 *              Regarding only one of both actions it would be still ok and no timing constraints are violated.
 *              Regading both actions together it might get dangerous to violate timing constraints regarding main function execution time.
 *              With the help of this function the main function will notice that and calculate the 2k CRC in the flash the next time it is called.
 *
 * \param   id_en           Which operation shall be executed
 *                          Currently can be:
 *                          - CRC in RAM
 *                          - CRC in flash
 *                          - Handle one FAT entry
 * \param   requested_u32   How often shall the operation be executed
 *                          Can mean:
 *                          - Amount of bytes for CRC calculation
 *                          - Amount of FAT entries to handle
 *
 * \return  The main function may continue or it shall stop and continue with the next call
 *
 * \retval  MEMIF_JOB_OK        The main function can continue
 * \retval  MEMIF_JOB_PENDING   The main function shall stop and continue with its next call
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_LibEffortLimit(Fee_Prv_LibEffortLimit_ten id_en, uint32 requested_u32)
{
    MemIf_JobResultType result_en = MEMIF_JOB_OK;

    if(TRUE == Fee_Prv_LibEffortMeasure_st.enabled_b)
    {
        /*
         * Weighting an action is only allowed logarithmic.
         * That means an action costs 1,2,4 or 8..
         * This is to ensure fast calculation since the action*weight can be calculated with bit shifting.
         */
        uint32 cost_u32 = requested_u32 << Fee_Prv_LibEffortWeights_au8[(uint32)id_en];
        if(cost_u32 <= Fee_Prv_LibEffortMeasure_st.effortCtr_u32)
        {
            Fee_Prv_LibEffortMeasure_st.effortCtr_u32 -= cost_u32;
        }
        else
        {
            Fee_Prv_LibEffortMeasure_st.effortCtr_u32 = 0u;
            result_en = MEMIF_JOB_PENDING;
        }
    }

    return(result_en);
}


/**
 * \brief   Request an amount of work for a specific action.
 *          If it not possible to do all the work in one main function then return as much as possible.
 *
 * \param   id_en           ID of the action
 * \param   requested_u32   Requested amount of work to do
 *
 * \return  Allowed amount of work to do in this main function call
 *
 * \see also Fee_Prv_LibEffortLimit
*/
LOCAL_INLINE uint32 Fee_Prv_LibEffortGet(Fee_Prv_LibEffortLimit_ten id_en, uint32 requested_u32)
{
    uint32 result_u32 = requested_u32;

    if(TRUE == Fee_Prv_LibEffortMeasure_st.enabled_b)
    {
        uint32 cost_u32                             = requested_u32 << Fee_Prv_LibEffortWeights_au8[id_en];
        result_u32                                  = Fee_Prv_LibMin(cost_u32, Fee_Prv_LibEffortMeasure_st.effortCtr_u32);
        Fee_Prv_LibEffortMeasure_st.effortCtr_u32   -= result_u32;
        result_u32                                  >>= Fee_Prv_LibEffortWeights_au8[id_en];
    }

    return(result_u32);
}

/**
 * \brief   Enable or disable the usage of the effort limitation.
 *          During init this is disabled since the only goal is to initalize as fast as possible.
 *          During cyclic operation this is enabled in order to not violate task execution time constraints.
*/
LOCAL_INLINE void Fee_Prv_LibUseEffortLimit(boolean use_b)
{
    Fee_Prv_LibEffortMeasure_st.enabled_b = use_b;
}

#  if (defined(FEE_CFG_FEE_MULTIINSTANCE_ENABLED) && (TRUE == FEE_CFG_FEE_MULTIINSTANCE_ENABLED))
/* Multi instance is enabled. Route the accesses to Fls via the Fls_Rb_Idx_ functions */
LOCAL_INLINE void Fee_Prv_Lib_Fls_MainFunction(Fls_Rb_FlsDevice_ten idDevice)
{
    Fls_Rb_Idx_MainFunction(idDevice, TRUE);
}

LOCAL_INLINE MemIf_JobResultType Fee_Prv_Lib_Fls_GetJobResult(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType TargetAddress)
{
    return (Fls_Rb_Idx_GetJobResultAdr(idDevice, TargetAddress));
}

LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Write(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType TargetAddress,
                                                     const uint8* SourceAddressPtr, Fls_Rb_LengthType Length)
{
    return (Fls_Rb_Idx_Write(idDevice, TargetAddress, SourceAddressPtr, Length, FLS_RB_ST_DETAIL_STD));
}

LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Read(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType SourceAddress,
                                                    uint8* TargetAddressPtr, Fls_Rb_LengthType Length)
{
    return (Fls_Rb_Idx_Read(idDevice, SourceAddress, TargetAddressPtr, Length));
}

LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Erase(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType TargetAddress, Fls_Rb_LengthType Length)
{
    return (Fls_Rb_Idx_Erase(idDevice, TargetAddress, Length));
}

LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Compare(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType SourceAddress,
                                                       const uint8* TargetAddressPtr, Fls_Rb_LengthType Length)
{
    return (Fls_Rb_Idx_Compare(idDevice, SourceAddress, TargetAddressPtr, Length));
}

#   if (defined(FLS_BLANK_CHECK_API) && (STD_OFF != FLS_BLANK_CHECK_API))
LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_BlankCheck(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType TargetAddress,
                                                          Fls_Rb_LengthType Length)
{
    return (Fls_Rb_Idx_BlankCheck(idDevice, TargetAddress, Length));
}
#   endif

#   if (defined(FLS_SET_MODE_API) && (STD_OFF != FLS_SET_MODE_API))
LOCAL_INLINE void Fee_Prv_Lib_Fls_SetMode(Fls_Rb_FlsDevice_ten idDevice, MemIf_ModeType Mode)
{
    Fls_Rb_Idx_SetMode(idDevice, Mode);
}
#   endif

#   if (defined(FLS_RB_ROBUST_PROG_STATUS_API) && (STD_OFF != FLS_RB_ROBUST_PROG_STATUS_API))
LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Rb_GetRobustProgStatus(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType* xStartAddress,
                                                                   Fls_Rb_tpu8* DataBuffer, Fls_Rb_LengthType* numBytes)
{
    return (Fls_Rb_Idx_GetRobustProgStatus(idDevice, xStartAddress, DataBuffer, numBytes));
}
#   endif

#  else
/* Single instance of Fee is used. Route the accesses to Fls via the standard AUTOSAR Fls functions */
LOCAL_INLINE void Fee_Prv_Lib_Fls_MainFunction(Fls_Rb_FlsDevice_ten idDevice)
{
    (void)idDevice;
#   if ((defined(FLS_RB_VENDOR_ID)) && (FLS_RB_VENDOR_ID == 0x06))
    Fls_Rb_MainFunctionAndDependency();
#   else
    Fls_MainFunction();
#   endif
}

LOCAL_INLINE MemIf_JobResultType Fee_Prv_Lib_Fls_GetJobResult(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType TargetAddress)
{
    (void)idDevice;
    (void)TargetAddress;
    return (Fls_GetJobResult());
}

LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Write(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType TargetAddress,
                                                     const uint8* SourceAddressPtr, Fls_Rb_LengthType Length)
{
    (void)idDevice;
    return (Fls_Write(TargetAddress, SourceAddressPtr, Length));
}

LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Read(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType SourceAddress,
                                                    uint8* TargetAddressPtr, Fls_Rb_LengthType Length)
{
    (void)idDevice;
    return (Fls_Read(SourceAddress, TargetAddressPtr, Length));
}

LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Erase(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType TargetAddress, Fls_Rb_LengthType Length)
{
    (void)idDevice;
    return (Fls_Erase(TargetAddress, Length));
}

LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Compare(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType SourceAddress,
                                                       const uint8* TargetAddressPtr, Fls_Rb_LengthType Length)
{
    (void)idDevice;
    return (Fls_Compare(SourceAddress, TargetAddressPtr, Length));
}

#   if (defined(FLS_BLANK_CHECK_API_AVAIL) && (STD_OFF != FLS_BLANK_CHECK_API_AVAIL))
LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_BlankCheck(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType TargetAddress,
                                                          Fls_Rb_LengthType Length)
{
    (void)idDevice;
    return (Fls_BlankCheck(TargetAddress, Length));
}
#   endif

#   if (defined(FLS_SET_MODE_API_AVAIL) && (STD_OFF != FLS_SET_MODE_API_AVAIL))
LOCAL_INLINE void Fee_Prv_Lib_Fls_SetMode(Fls_Rb_FlsDevice_ten idDevice, MemIf_ModeType Mode)
{
    (void)idDevice;
    Fls_SetMode(Mode);
}
#   endif

#   if (defined(FLS_RB_GET_ROBUST_PROG_STATUS_API_AVAIL) && (STD_OFF != FLS_RB_GET_ROBUST_PROG_STATUS_API_AVAIL))
LOCAL_INLINE Std_ReturnType Fee_Prv_Lib_Fls_Rb_GetRobustProgStatus(Fls_Rb_FlsDevice_ten idDevice, Fls_Rb_AddressType* xStartAddress,
                                                                   Fls_Rb_tpu8* DataBuffer, Fls_Rb_LengthType* numBytes)
{
    (void)idDevice;
    return (Fls_Rb_GetRobustProgStatus(xStartAddress, DataBuffer, numBytes));
}
#   endif
#  endif

# endif

/* FEE_PRV_LIB_H */
#endif
