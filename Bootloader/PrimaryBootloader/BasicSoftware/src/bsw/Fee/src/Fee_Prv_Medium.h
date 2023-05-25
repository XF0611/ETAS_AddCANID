

/*
 * The medium unit is the lowest layer in the Fee architecture.
 * It abstracts direct flash access and Fls_Read flash access.
 * The Fee assumes that it is the only flash user.
 * To reduce the number of error paths it is not visible to higher layers,
 * whether starting a flash job returned with E_NOT_OK or
 * whether the flash job finished with MEMIF_JOB_FAILED.
 * Additionally it contains a big buffer which is used for various operations,
 * mainly the buffer is used for writing internal management data and during the data sector change.
 * The medium unit does also provide blocking flash access,
 * this means polling until Fls_GetJobResult != MEMIF_JOB_PENDING
 * or asynchronous flash access.
*/

#ifndef FEE_PRV_MEDIUM_H
#define FEE_PRV_MEDIUM_H

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
# if (defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/
#include "Fee_Cfg_SchM.h"

/* in order to avoid creating a new Paramdef,
 * the parameter FEE_RB_WRITE_PADDING_REQUIRED should be defined in Fee_Cfg_Schm.h only to test this feature
 */
#ifndef FEE_RB_WRITE_PADDING_REQUIRED
#if ((defined(FLS_VENDOR_ID)) && (FLS_VENDOR_ID == 0x06))
// RTA-BSW FLS is used
#define FEE_RB_WRITE_PADDING_REQUIRED FALSE
#else
// non RTA-BSW FLS is used
#define FEE_RB_WRITE_PADDING_REQUIRED TRUE
#endif
#endif

/* Prefetch modes */
typedef enum
{
    FEE_PRV_PRELOAD_E,
    FEE_PRV_POSTLOAD_E
} Fee_Prv_MediumLoadMode_ten;

/* States for loading data from flash to RAM */
typedef enum
{
    FEE_PRV_LOAD_IDLE_E = 0,    /* Nothing to do        */
    FEE_PRV_LOAD_BUSY_E         /* Currently loading    */
} Fee_Prv_MediumLoad_ten;

/* Sequences for programming data from RAM to flash */
typedef enum
{
    FEE_PRV_MEDIUM_SEQ_WRITE_BLNKCHK_COPY_COMP_E = 0,   /* Write sequence: blank check -> copy -> program -> compare */
    FEE_PRV_MEDIUM_SEQ_WRITE_BLNKCHK_COMP_E,            /* Write sequence: blank check         -> program -> compare */
    FEE_PRV_MEDIUM_SEQ_WRITE_COPY_COMP_E,               /* Write sequence:                copy -> program -> compare */
    FEE_PRV_MEDIUM_SEQ_WRITE_COMP_E,                    /* Write sequence:                        program -> compare */
    FEE_PRV_MEDIUM_SEQ_WRITE_ONLY_E                     /* Yet unsupported write sequence:        program            */
} Fee_Prv_MediumSeqWrite_ten;

/* State for programming data from RAM to flash */
typedef enum
{
    FEE_PRV_MEDIUM_WRITE_IDLE_E = 0,    /* Nothing to do                                        */
    FEE_PRV_MEDIUM_WRITE_BLANKCHECK_E,  /* Make sure the target space is not already programmed */
    FEE_PRV_MEDIUM_WRITE_COPYDATA_E,    /* copy data from user buffer to internal buffer        */
    FEE_PRV_MEDIUM_WRITE_PROGRAM_E,     /* Program data from RAM to flash                       */
    FEE_PRV_MEDIUM_WRITE_COMPARE_E,     /* Compare RAM to flash                                 */
    FEE_PRV_MEDIUM_WRITE_FINAL_E        /* Programming finished successfully                    */
} Fee_Prv_MediumWrite_ten;

/* Sequences for copying data from one flash location to another (using internal buffer) */
typedef enum
{
    FEE_PRV_MEDIUM_SEQ_COPY_BLNKCHK_COMP_E = 0,    /* Copy sequence:  blank check -> copy -> compare */
    FEE_PRV_MEDIUM_SEQ_COPY_COMP_E,                /* Copy sequence:              -> copy -> compare */
    FEE_PRV_MEDIUM_SEQ_COPY_ONLY_E                 /* Yet unsupported copy sequence: copy            */
} Fee_Prv_MediumSeqCopy_ten;


/* State for copying data from one flash location to another (using internal buffer) */
typedef enum
{
    FEE_PRV_MEDIUM_COPY_IDLE_E = 0,     /* Nothing to do                            */
    FEE_PRV_MEDIUM_COPY_READ_E,         /* Read data from flash to internal buffer  */
    FEE_PRV_MEDIUM_COPY_WRITE_E,        /* Write data from internal buffer to flash */
    FEE_PRV_MEDIUM_COPY_FINAL_E         /* Copy finished succcessfully              */
} Fee_Prv_MediumCopy_ten;

/* State for comparing data from one flash location to another (using internal buffer) */
typedef enum
{
    FEE_PRV_MEDIUM_COMPAREFLS2FLS_IDLE_E = 0,     /* Nothing to do                                              */
    FEE_PRV_MEDIUM_COMPAREFLS2FLS_READ_E,         /* Read data from flash to internal buffer                    */
    FEE_PRV_MEDIUM_COMPAREFLS2FLS_COMPARE_E,      /* Compare data from internal buffer to 2nd flash location    */
    FEE_PRV_MEDIUM_COMPAREFLS2FLS_FINAL_E         /* Compare finished succcessfully                             */
} Fee_Prv_MediumCompareFls2Fls_ten;

/* Union for the big buffer forces 32bit alignment */
typedef union
{
    uint32  u32[FEE_PRV_CFG_BFRSIZE/4u];    /* This union member shall NEVER be used - it only forces 32bit alignment */
    uint8   u8[FEE_PRV_CFG_BFRSIZE];        /* uint8 indexing of the buffer                                           */
} Fee_Prv_MediumBuffer_tun;

typedef struct
{
    uint32                      iterator_u32;       /* Iterator used to iterate through the flash in steps  */
    uint32                      limitHi_u32;        /* Upper limit of data from flash in the medium buffer  */
    uint32                      limitLo_u32;        /* Lower limit of data from flash in the medium buffer  */
    uint32                      maxPrefetchLen_u32; /* Maximum amount of data to load including prefetch    */
    uint32                      flsAdr_u32;         /* Address to which the Fls request was placed. This is needed to get the result. */
    Std_ReturnType              lastFlsResult_en;   /* Result of the last flash job start                   */
    boolean                     sync_b;             /* Polling = TRUE or Asynchronous = FALSE flash access  */
    Fls_Rb_FlsDevice_ten        flsDevId_en;        /* name of the device index that needs to be used for Fls accesses */
}  Fee_Prv_Medium_tst;

typedef struct
{
    uint32                  reloadAdr_u32;      /* Address to reload from       */
    uint32                  nrBytes2Load_u32;   /* Number of bytes to load      */
    Fee_Prv_MediumLoad_ten  state_en;           /* Current state                */
} Fee_Prv_MediumLoad_tst;

typedef struct
{
    uint32      nrBytesDone_u32;    /* Already considered number of bytes   */
    uint32      length_u32;         /* Total number of bytes to consider    */
    uint32 *    crc_pu32;           /* Pointer to the result                */
    boolean     firstCall_b;        /* First CRC call yes or no             */
} Fee_Prv_MediumCalcCrc_tst;

typedef struct
{
    uint8 const *           adr_pcu8;           /* Address where to read from                       */
    uint8 *                 bfr_pu8;            /* Buffer where to write into (= user buffer)       */
    uint32                  length_u32;         /* Bytes to read in total                           */
    uint32                  nrBytesDone_u32;    /* Number of bytes already processed                */
    boolean                 isBufferHit_b;      /* Flag indicates if src address is in Flash or RAM */
} Fee_Prv_MediumRead_tst;

typedef struct
{
    void const *               bfr_pcv;             /* Buffer where to program from (= user buffer)                  */
    void const *               data2Program_pcv;    /* Pointer to data, which is programmed in current chunk         */
    uint32                     adrDest_u32;         /* Address where to program to                                   */
    uint32                     length_u32;          /* Bytes to program in total                                     */
    uint32                     done_u32;            /* Bytes already programmed                                      */
    uint32                     bytes2Program_u32;   /* Bytes to program in current chunk                             */
    Fee_Prv_MediumWrite_ten    state_en;            /* State of the programming operation                            */
    boolean                    isMedBfrByp_b;       /* Bypass internal medium buffer (i.e. program from user buffer) */
    boolean                    entry_b;             /* Next state shall be entered                                   */
} Fee_Prv_MediumWrite_tst;

typedef struct
{
    uint32                        adrSrc_u32;        /* Source address in flash                     */
    uint32                        adrDest_u32;       /* Destination address in flash                */
    uint32                        length_u32;        /* Bytes to copy in total                      */
    uint32                        done_u32;          /* Bytes already copied to flash               */
    uint32                        bytes2Copy_u32;    /* Bytes to copy in current chunk              */
    Fee_Prv_MediumCopy_ten        state_en;          /* State of the copy operation                 */
    Fee_Prv_MediumSeqWrite_ten    writeSeq_en;       /* Requested write sequence for copy operation */
    boolean                       entry_b;           /* Next state shall be entered                 */
} Fee_Prv_MediumCopy_tst;

typedef struct
{
    uint32                              adrFls1_u32;                /* Address of 1st flash location     */
    uint32                              adrFls2_u32;                /* Address of 2nd flash location     */
    uint32                              length_u32;                 /* Bytes to compare in total         */
    uint32                              nrBytesDone_u32;            /* Bytes already compared in flash   */
    uint32                              nrBytesCurrentChunk_u32;    /* Bytes to compare in current chunk */
    Fee_Prv_MediumCompareFls2Fls_ten    state_en;                   /* State of the compare operation    */
    boolean                             entry_b;                    /* Next state shall be entered       */
} Fee_Prv_MediumCompareFls2Fls_tst;

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
extern void Fee_Prv_MediumSetSync(boolean newSync_b);
extern void Fee_Prv_MediumInit(Fee_Rb_DeviceName_ten FeeDeviceName_en);
extern void Fee_Prv_MediumRead(uint32 adr_u32, void * bfr_pv, uint32 length_u32);
extern void Fee_Prv_MediumWrite(uint32 adr_u32, void const * bfr_pcv, uint32 length_u32, Fee_Prv_MediumSeqWrite_ten sequence_en);
extern void Fee_Prv_MediumErase(uint32 adr_u32, uint32 length_u32);
extern void Fee_Prv_MediumBlankCheck(uint32 adr_u32, uint32 length_u32);
extern void Fee_Prv_MediumShredder(uint32 adr_u32, uint8 value_u8, uint32 length_u32);
extern void Fee_Prv_MediumPatShredder(uint32 adr_u32, uint32 patOffs_u32, uint32 patSize_u32, void const * patBfr_pcv, uint32 length_u32);
extern void Fee_Prv_MediumCalcCrc(uint32 adr_u32, uint32 length_u32, uint32 * crc_pu32, boolean firstCall_b);
extern void Fee_Prv_MediumCopy(uint32 adrSrc_u32, uint32 adrDest_u32, uint32 length_u32, Fee_Prv_MediumSeqCopy_ten sequence_en);
extern void Fee_Prv_MediumCompareFls2Buf(uint32 adrFls_u32, void const * bfr_pcv, uint32 length_u32);
extern void Fee_Prv_MediumCompareFls2Fls(uint32 adrFls1_u32, uint32 adrFls2_u32, uint32 length_u32);

extern MemIf_JobResultType Fee_Prv_MediumReadDo             (void);
extern MemIf_JobResultType Fee_Prv_MediumWriteDo            (void);
extern MemIf_JobResultType Fee_Prv_MediumEraseDo            (void);
extern MemIf_JobResultType Fee_Prv_MediumBlankCheckDo       (void);
extern MemIf_JobResultType Fee_Prv_MediumShredderDo         (void);
extern MemIf_JobResultType Fee_Prv_MediumCalcCrcDo          (void);
extern MemIf_JobResultType Fee_Prv_MediumCopyDo             (void);
extern MemIf_JobResultType Fee_Prv_MediumCompareFls2BufDo   (void);
extern MemIf_JobResultType Fee_Prv_MediumCompareFls2FlsDo   (void);

extern void Fee_Prv_MediumSetIterator(uint32 adr_u32, uint32 maxPrefetchLen_u32);
extern uint32 Fee_Prv_MediumGetIterator(void);
extern MemIf_JobResultType Fee_Prv_MediumNext(uint32 length_u32, void const * * ptr_ppcv);
extern MemIf_JobResultType Fee_Prv_MediumPre(uint32 length_u32, void const * * ptr_ppcv);

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

#  if (defined(FLS_RB_ROBUST_PROG_STATUS_API) && (STD_OFF != FLS_RB_ROBUST_PROG_STATUS_API))

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
extern uint32           Fee_Prv_MediumGetWlSize(void);
extern boolean          Fee_Prv_MediumHasWlError(void);
extern uint32           Fee_Prv_MediumGetWlErrorAdr(void);
extern uint32           Fee_Prv_MediumGetWlErrorLength(void);
extern void             Fee_Prv_MediumRescueWlData(void);
extern uint8    const * Fee_Prv_MediumGetWlData(void);
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

#  else

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
*/

LOCAL_INLINE uint32             Fee_Prv_MediumGetWlSize(void)           { return(0u);       }
LOCAL_INLINE boolean            Fee_Prv_MediumHasWlError(void)          { return(FALSE);    }
LOCAL_INLINE uint32             Fee_Prv_MediumGetWlErrorAdr(void)       { return(0u);       }
LOCAL_INLINE uint32             Fee_Prv_MediumGetWlErrorLength(void)    { return(0u);       }
LOCAL_INLINE void               Fee_Prv_MediumRescueWlData(void)        {                   }
LOCAL_INLINE uint8      const * Fee_Prv_MediumGetWlData(void)           { return(NULL_PTR); }

/*
 * Use inlines instead of defines since defines lead to compiler warnings, example: Comparison >= 0
 * Compiler should still be able to perfomr maximum optimizations with inlines.
 */
#  endif

LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumPreSync(uint32 adr_u32, uint32 length_u32, uint32 maxPrefetchLen_u32, void const * * ptr_ppcv);
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumPatShredderDo(void);

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
*/

/**
 * \brief   Without direct DFLASH access, set the current iterator to the given address,
 *          set a pointer to the current iterator then decrease the iterator for specified length.
 *          With direct DFLASH access, return the address into the DFLASH. Without, point into the buffered area.
 *          This function only is allowed during Fee_init (synchronous Fls job execution).
 * \param   adrRead_u32         Address in DFLASH
 * \param   length_u32          Amount of bytes to step after pointer is set to current iterator
 * \param   nrBytesPrefetch_u32 Amount of bytes to be buffered if a rebuffering is required
 * \param   ptr_ppcv            Pointer to the pointer which shall be set to the address of the iterator
 *
 * \retval  MEMIF_JOB_OK        Job finished successfully
 * \retval  MEMIF_JOB_FAILED    Job finished with errors. With direct DFLASH access, this return value cannot occur.
 * \retval  MEMIF_JOB_PENDING   This return value is not possible when the function is called during Fee_init. Pay attention to not call this function outside Fee_Init execution
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumPreSync(uint32 adr_u32 , uint32 length_u32 , uint32 maxPrefetchLen_u32 , void const * * ptr_ppcv)
{
    MemIf_JobResultType result_en;

#  if (defined(FEE_PRV_CFG_MED_ALLOW_DIRECT) && (TRUE == FEE_PRV_CFG_MED_ALLOW_DIRECT)) && (!defined(RBA_FLSVECU_FLASH))

    (void)length_u32;
    (void)maxPrefetchLen_u32;
    /* MR12 RULE 11.6 VIOLATION: Memory addresses of type uint32 can safely be casted to void * */
    *ptr_ppcv = (void const *)(FEE_PRV_CFG_FLS_BASE_ADR + adr_u32);

    result_en = MEMIF_JOB_OK;
#  else
    // set the iterator to the requested address
    Fee_Prv_MediumSetIterator(adr_u32 , maxPrefetchLen_u32);

    // get the requested address with backward buffering
    result_en = Fee_Prv_MediumPre(length_u32 , ptr_ppcv);
#  endif
    return result_en;
}

/**
 * \brief   Map Fee_Prv_MediumPatShredderDo to Fee_Prv_MediumShredderDo
*/
LOCAL_INLINE MemIf_JobResultType Fee_Prv_MediumPatShredderDo(void)
{
    return(Fee_Prv_MediumShredderDo());
}


# endif

/* FEE_PRV_MEDIUM_H */
#endif
