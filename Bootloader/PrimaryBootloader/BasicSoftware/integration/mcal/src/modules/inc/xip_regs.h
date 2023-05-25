/*
 * xip_regs.h
 *
 * Created on: Sep 6, 2022
 * Author: Neo
 */
#ifndef _XIP_REGS_H_
#define _XIP_REGS_H_
#include <stdint.h>
/** @addtogroup Peripheral_registers_structures
  * @{
  */
/**
  * @brief eXecute In Place
  */
typedef struct
{
    volatile uint32_t XER;        /*!< Address offset:  0x00,  XIP Enable Register */
    volatile uint32_t XRDSRCR;    /*!< Address offset:  0x04,  XIP RDSR Command Register */
    volatile uint32_t XRDCR;      /*!< Address offset:  0x08,  XIP Read Command Register */
    volatile uint32_t XWENACR;    /*!< Address offset:  0x0C,  XIP WENA Command Register */
    volatile uint32_t XWECR;      /*!< Address offset:  0x10,  XIP Write Command Register */
    volatile uint32_t XSSCR;      /*!< Address offset:  0x14,  XIP Suspend Command Register */
    volatile uint32_t XRDFSRCR;   /*!< Address offset:  0x18,  XIP RDFSR Command Register */
    volatile uint32_t XISOR;      /*!< Address offset:  0x1C,  XIP Instruction Section Offset Register */
    volatile uint32_t XDSOR;      /*!< Address offset:  0x20,  XIP Data Section Offset Register */
    volatile uint32_t AMR;        /*!< Address offset:  0x24,  AES Mode Register */
    volatile uint32_t ACDMR;      /*!< Address offset:  0x28,  AES Cipher/Decipher Mode Register */
    volatile uint32_t ARSR;       /*!< Address offset:  0x2C,  AES Radom Seed Register */
    volatile uint32_t ATVR[4];    /*!< Address offset:  0x30,  AES Tweak Value Register 3 */
    volatile uint32_t AVBR;       /*!< Address offset:  0x40,  AES Valid Block Register */
    volatile uint32_t ALBSR;      /*!< Address offset:  0x44,  AES Last Block Size Register */
    volatile uint32_t AIDR[16];   /*!< Address offset:  0x48,  AES Input Data Register 15 */
    volatile uint32_t AWER;       /*!< Address offset:  0x88,  AES Write Enable Register */
    volatile uint32_t ANSR;       /*!< Address offset:  0x8C,  AES New Stream Register */
    volatile uint32_t AIDVR;      /*!< Address offset:  0x90,  AES Input Data Validation Register */
    volatile uint32_t ADSR;       /*!< Address offset:  0x94,  AES Done Signal Register */
    volatile uint32_t AODR[16];   /*!< Address offset:  0x98,  AES Output Data Register 15 */
    volatile uint32_t XABECR;     /*!< Address offset:  0xD8,  XIP AHB Bus Endian Control Register */
    volatile uint32_t XADBECR;    /*!< Address offset:  0xDC,  XIP AES Data Bus Endian Control Register */
    volatile uint32_t XIBCR;      /*!< Address offset:  0xE0,  XIP Instruction Buffer Control Register */
    volatile uint32_t XRBCR;      /*!< Address offset:  0xE4,  XIP Read Buffer Control Register */
    volatile uint32_t XWRBCR;     /*!< Address offset:  0xE8,  XIP Write Buffer Control Register */
    volatile uint32_t XWPBCR;     /*!< Address offset:  0xEC,  XIP WIP Bit Control Register */
    volatile uint32_t XXBCR;      /*!< Address offset:  0xF0,  XIP XSB Bit Control Register */
    volatile uint32_t XXLCR;      /*!< Address offset:  0xF4,  XIP XSB Latency Counter Register */
    volatile uint32_t XEFBR;      /*!< Address offset:  0xF8,  XIP Erase Flag Bit Register */
    volatile uint32_t XPFBR;      /*!< Address offset:  0xFC,  XIP Program Flag Bit Register */
}XIP_TypeDef;
/**
  * @}
  */

/** @addtogroup Peripheral_memory_map
  * @{
  */
#define XIP_BASE    (0x00D00100UL)
/**
  * @}
  */

/** @addtogroup Peripheral_declaration
  * @{
  */
#define XIP    ((XIP_TypeDef *) XIP_BASE)
/**
  * @}
  */

/** @addtogroup Exported_constants
  * @{
  */
/** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */
/**************************************************************/
/*                                                            */
/*                      eXecute In Place                      */
/*                                                            */
/**************************************************************/
/* Bits definition for XIP_XER register ***********************/
#define XIP_ENAB_Pos             (0U)
#define XIP_ENAB_Msk             (0x1UL << XIP_ENAB_Pos)
#define XIP_ENAB                 XIP_ENAB_Msk
/* Bits definition for XIP_XRDSRCR register *******************/
#define XIP_RDSR_INST_LEN_Pos    (25U)
#define XIP_RDSR_INST_LEN_Msk    (0x3UL << XIP_RDSR_INST_LEN_Pos)
#define XIP_RDSR_INST_LEN        XIP_RDSR_INST_LEN_Msk
#define XIP_RDSR_ADDR_LEN_Pos    (21U)
#define XIP_RDSR_ADDR_LEN_Msk    (0xFUL << XIP_RDSR_ADDR_LEN_Pos)
#define XIP_RDSR_ADDR_LEN        XIP_RDSR_ADDR_LEN_Msk
#define XIP_RDSR_DFS_Pos         (16U)
#define XIP_RDSR_DFS_Msk         (0x1FUL << XIP_RDSR_DFS_Pos)
#define XIP_RDSR_DFS             XIP_RDSR_DFS_Msk
#define XIP_RDSR_CMD_Pos         (0U)
#define XIP_RDSR_CMD_Msk         (0xFFFFUL << XIP_RDSR_CMD_Pos)
#define XIP_RDSR_CMD             XIP_RDSR_CMD_Msk
/* Bits definition for XIP_XRDCR register *********************/
#define XIP_READ_INST_LEN_Pos    (25U)
#define XIP_READ_INST_LEN_Msk    (0x3UL << XIP_READ_INST_LEN_Pos)
#define XIP_READ_INST_LEN        XIP_READ_INST_LEN_Msk
#define XIP_READ_ADDR_LEN_Pos    (21U)
#define XIP_READ_ADDR_LEN_Msk    (0xFUL << XIP_READ_ADDR_LEN_Pos)
#define XIP_READ_ADDR_LEN        XIP_READ_ADDR_LEN_Msk
#define XIP_READ_DFS_Pos         (16U)
#define XIP_READ_DFS_Msk         (0x1FUL << XIP_READ_DFS_Pos)
#define XIP_READ_DFS             XIP_READ_DFS_Msk
#define XIP_READ_CMD_Pos         (0U)
#define XIP_READ_CMD_Msk         (0xFFFFUL << XIP_READ_CMD_Pos)
#define XIP_READ_CMD             XIP_READ_CMD_Msk
/* Bits definition for XIP_XWENACR register *******************/
#define XIP_WENA_INST_LEN_Pos    (25U)
#define XIP_WENA_INST_LEN_Msk    (0x3UL << XIP_WENA_INST_LEN_Pos)
#define XIP_WENA_INST_LEN        XIP_WENA_INST_LEN_Msk
#define XIP_WENA_ADDR_LEN_Pos    (21U)
#define XIP_WENA_ADDR_LEN_Msk    (0xFUL << XIP_WENA_ADDR_LEN_Pos)
#define XIP_WENA_ADDR_LEN        XIP_WENA_ADDR_LEN_Msk
#define XIP_WENA_DFS_Pos         (16U)
#define XIP_WENA_DFS_Msk         (0x1FUL << XIP_WENA_DFS_Pos)
#define XIP_WENA_DFS             XIP_WENA_DFS_Msk
#define XIP_WENA_CMD_Pos         (0U)
#define XIP_WENA_CMD_Msk         (0xFFFFUL << XIP_WENA_CMD_Pos)
#define XIP_WENA_CMD             XIP_WENA_CMD_Msk
/* Bits definition for XIP_XWECR register *********************/
#define XIP_WRITE_INST_LEN_Pos   (25U)
#define XIP_WRITE_INST_LEN_Msk   (0x3UL << XIP_WRITE_INST_LEN_Pos)
#define XIP_WRITE_INST_LEN       XIP_WRITE_INST_LEN_Msk
#define XIP_WRITE_ADDR_LEN_Pos   (21U)
#define XIP_WRITE_ADDR_LEN_Msk   (0xFUL << XIP_WRITE_ADDR_LEN_Pos)
#define XIP_WRITE_ADDR_LEN       XIP_WRITE_ADDR_LEN_Msk
#define XIP_WRITE_DFS_Pos        (16U)
#define XIP_WRITE_DFS_Msk        (0x1FUL << XIP_WRITE_DFS_Pos)
#define XIP_WRITE_DFS            XIP_WRITE_DFS_Msk
#define XIP_WRITE_CMD_Pos        (0U)
#define XIP_WRITE_CMD_Msk        (0xFFFFUL << XIP_WRITE_CMD_Pos)
#define XIP_WRITE_CMD            XIP_WRITE_CMD_Msk
/* Bits definition for XIP_XSSCR register *********************/
#define XIP_SUSPEND_INST_LE_Pos  (25U)
#define XIP_SUSPEND_INST_LE_Msk  (0x3UL << XIP_SUSPEND_INST_LE_Pos)
#define XIP_SUSPEND_INST_LE      XIP_SUSPEND_INST_LE_Msk
#define XIP_SUSPEND_ADDR_L_Pos   (21U)
#define XIP_SUSPEND_ADDR_L_Msk   (0xFUL << XIP_SUSPEND_ADDR_L_Pos)
#define XIP_SUSPEND_ADDR_L       XIP_SUSPEND_ADDR_L_Msk
#define XIP_SUSPEND_DFS_Pos      (16U)
#define XIP_SUSPEND_DFS_Msk      (0x1FUL << XIP_SUSPEND_DFS_Pos)
#define XIP_SUSPEND_DFS          XIP_SUSPEND_DFS_Msk
#define XIP_SUSPEND_CMD_Pos      (0U)
#define XIP_SUSPEND_CMD_Msk      (0xFFFFUL << XIP_SUSPEND_CMD_Pos)
#define XIP_SUSPEND_CMD          XIP_SUSPEND_CMD_Msk
/* Bits definition for XIP_XRDFSRCR register ******************/
#define XIP_RDFSR_INST_LEN_Pos   (25U)
#define XIP_RDFSR_INST_LEN_Msk   (0x3UL << XIP_RDFSR_INST_LEN_Pos)
#define XIP_RDFSR_INST_LEN       XIP_RDFSR_INST_LEN_Msk
#define XIP_RDFSR_ADDR_LEN_Pos   (21U)
#define XIP_RDFSR_ADDR_LEN_Msk   (0xFUL << XIP_RDFSR_ADDR_LEN_Pos)
#define XIP_RDFSR_ADDR_LEN       XIP_RDFSR_ADDR_LEN_Msk
#define XIP_RDFSR_DFS_Pos        (16U)
#define XIP_RDFSR_DFS_Msk        (0x1FUL << XIP_RDFSR_DFS_Pos)
#define XIP_RDFSR_DFS            XIP_RDFSR_DFS_Msk
#define XIP_RDFSR_CMD_Pos        (0U)
#define XIP_RDFSR_CMD_Msk        (0xFFFFUL << XIP_RDFSR_CMD_Pos)
#define XIP_RDFSR_CMD            XIP_RDFSR_CMD_Msk
/* Bits definition for XIP_AMR register ***********************/
#define XIP_MISS_CNT_Pos         (8U)
#define XIP_MISS_CNT_Msk         (0xFFFFFFUL << XIP_MISS_CNT_Pos)
#define XIP_MISS_CNT             XIP_MISS_CNT_Msk
#define XIP_TMF_MASK_Pos         (8U)
#define XIP_TMF_MASK_Msk         (0xFFFFFFUL << XIP_TMF_MASK_Pos)
#define XIP_TMF_MASK             XIP_TMF_MASK_Msk
#define XIP_TMF_Pos              (2U)
#define XIP_TMF_Msk              (0x1UL << XIP_TMF_Pos)
#define XIP_TMF                  XIP_TMF_Msk
#define AES_MODE_Pos             (0U)
#define AES_MODE_Msk             (0x3UL << AES_MODE_Pos)
#define AES_MODE                 AES_MODE_Msk
/* Bits definition for XIP_ACDMR register *********************/
#define AES_CD_Pos               (0U)
#define AES_CD_Msk               (0x1UL << AES_CD_Pos)
#define AES_CD                   AES_CD_Msk
/* Bits definition for XIP_ARSR register **********************/
#define AES_SEED_Pos             (0U)
#define AES_SEED_Msk             (0xFFFFFUL << AES_SEED_Pos)
#define AES_SEED                 AES_SEED_Msk
/* Bits definition for XIP_AVBR register **********************/
#define AES_VLD_BLK_Pos          (0U)
#define AES_VLD_BLK_Msk          (0x7UL << AES_VLD_BLK_Pos)
#define AES_VLD_BLK              AES_VLD_BLK_Msk
/* Bits definition for XIP_ALBSR register *********************/
#define AES_LAST_BLK_SZ_Pos      (0U)
#define AES_LAST_BLK_SZ_Msk      (0xFFUL << AES_LAST_BLK_SZ_Pos)
#define AES_LAST_BLK_SZ          AES_LAST_BLK_SZ_Msk
/* Bits definition for XIP_AWER register **********************/
#define AES_UPDATE_Pos           (0U)
#define AES_UPDATE_Msk           (0x1UL << AES_UPDATE_Pos)
#define AES_UPDATE               AES_UPDATE_Msk
/* Bits definition for XIP_ANSR register **********************/
#define AES_NEW_STREAM_Pos       (0U)
#define AES_NEW_STREAM_Msk       (0x1UL << AES_NEW_STREAM_Pos)
#define AES_NEW_STREAM           AES_NEW_STREAM_Msk
/* Bits definition for XIP_AIDVR register *********************/
#define AES_DIN_VAL_Pos          (0U)
#define AES_DIN_VAL_Msk          (0x1UL << AES_DIN_VAL_Pos)
#define AES_DIN_VAL              AES_DIN_VAL_Msk
/* Bits definition for XIP_ADSR register **********************/
#define AES_DONE_Pos             (0U)
#define AES_DONE_Msk             (0x1UL << AES_DONE_Pos)
#define AES_DONE                 AES_DONE_Msk
/* Bits definition for XIP_XABECR register ********************/
#define XIP_WR_ENDIAN_Pos        (2U)
#define XIP_WR_ENDIAN_Msk        (0x3UL << XIP_WR_ENDIAN_Pos)
#define XIP_WR_ENDIAN            XIP_WR_ENDIAN_Msk
#define XIP_RD_ENDIAN_Pos        (0U)
#define XIP_RD_ENDIAN_Msk        (0x3UL << XIP_RD_ENDIAN_Pos)
#define XIP_RD_ENDIAN            XIP_RD_ENDIAN_Msk
/* Bits definition for XIP_XADBECR register *******************/
#define XIP_WORD_SWAP_Pos        (2U)
#define XIP_WORD_SWAP_Msk        (0x1UL << XIP_WORD_SWAP_Pos)
#define XIP_WORD_SWAP            XIP_WORD_SWAP_Msk
#define XIP_BYTE_SWAP_Pos        (1U)
#define XIP_BYTE_SWAP_Msk        (0x1UL << XIP_BYTE_SWAP_Pos)
#define XIP_BYTE_SWAP            XIP_BYTE_SWAP_Msk
#define XIP_BIT_SWAP_Pos         (0U)
#define XIP_BIT_SWAP_Msk         (0x1UL << XIP_BIT_SWAP_Pos)
#define XIP_BIT_SWAP             XIP_BIT_SWAP_Msk
/* Bits definition for XIP_XIBCR register *********************/
#define XIP_INSBUF_LEN_Pos       (0U)
#define XIP_INSBUF_LEN_Msk       (0xFUL << XIP_INSBUF_LEN_Pos)
#define XIP_INSBUF_LEN           XIP_INSBUF_LEN_Msk
/* Bits definition for XIP_XRBCR register *********************/
#define XIP_RDBUF_LEN_Pos        (0U)
#define XIP_RDBUF_LEN_Msk        (0xFUL << XIP_RDBUF_LEN_Pos)
#define XIP_RDBUF_LEN            XIP_RDBUF_LEN_Msk
/* Bits definition for XIP_XWRBCR register ********************/
#define XIP_WRBUF_LEN_Pos        (0U)
#define XIP_WRBUF_LEN_Msk        (0xFUL << XIP_WRBUF_LEN_Pos)
#define XIP_WRBUF_LEN            XIP_WRBUF_LEN_Msk
/* Bits definition for XIP_XWPBCR register ********************/
#define XIP_WIP_CNT_Pos          (16U)
#define XIP_WIP_CNT_Msk          (0x3UL << XIP_WIP_CNT_Pos)
#define XIP_WIP_CNT              XIP_WIP_CNT_Msk
#define XIP_WIP_BIT_Pos          (0U)
#define XIP_WIP_BIT_Msk          (0xFFFFUL << XIP_WIP_BIT_Pos)
#define XIP_WIP_BIT              XIP_WIP_BIT_Msk
/* Bits definition for XIP_XXBCR register *********************/
#define XIP_XSB_CNT_Pos          (16U)
#define XIP_XSB_CNT_Msk          (0x3UL << XIP_XSB_CNT_Pos)
#define XIP_XSB_CNT              XIP_XSB_CNT_Msk
#define XIP_XSB_BIT_Pos          (0U)
#define XIP_XSB_BIT_Msk          (0xFFFFUL << XIP_XSB_BIT_Pos)
#define XIP_XSB_BIT              XIP_XSB_BIT_Msk
/* Bits definition for XIP_XEFBR register *********************/
#define XIP_ERASE_FLAG_Pos       (0U)
#define XIP_ERASE_FLAG_Msk       (0x1UL << XIP_ERASE_FLAG_Pos)
#define XIP_ERASE_FLAG           XIP_ERASE_FLAG_Msk
/* Bits definition for XIP_XPFBR register *********************/
#define XIP_PRGM_FLAG_Pos        (0U)
#define XIP_PRGM_FLAG_Msk        (0x1UL << XIP_PRGM_FLAG_Pos)
#define XIP_PRGM_FLAG            XIP_PRGM_FLAG_Msk

#endif /* _XIP_REGS_H_ */
