/*
 * qspi_regs.h
 *
 * Created on: Sep 6, 2022
 * Author: Neo
 */
#ifndef _QSPI_REGS_H_
#define _QSPI_REGS_H_
#include <stdint.h>
/** @addtogroup Peripheral_registers_structures
  * @{
  */
/**
  * @brief Queued Serial Peripheral Interface
  */
typedef struct
{
    volatile uint32_t QCTRLR0;        /*!< Address offset:  0x0,  Control Register 0 */
    volatile uint32_t QCTRLR1;        /*!< Address offset:  0x4,  Control Register 1 */
    volatile uint32_t QSSIENR;        /*!< Address offset:  0x8,  SSI Enable Register */
    volatile uint32_t QMWCR;          /*!< Address offset:  0xc,  Microwire Control Register */
    volatile uint32_t QSER;           /*!< Address offset:  0x10,  Slave Enable Register */
    volatile uint32_t QBAUDR;         /*!< Address offset:  0x14,  Baud Rate Select */
    volatile uint32_t QTXFTLR;        /*!< Address offset:  0x18,  Transmit FIFO Threshold Level */
    volatile uint32_t QRXFTLR;        /*!< Address offset:  0x1c,  Receive FIFO Threshold Level */
    volatile uint32_t QTXFLR;         /*!< Address offset:  0x20,  Transmit FIFO Level Register */
    volatile uint32_t QRXFLR;         /*!< Address offset:  0x24,  Receive FIFO Level Register */
    volatile uint32_t QSR;            /*!< Address offset:  0x28,  Status Register */
    volatile uint32_t QIMR;           /*!< Address offset:  0x2c,  Interrupt Mask Register */
    volatile uint32_t QISR;           /*!< Address offset:  0x30,  Interrupt Status Register */
    volatile uint32_t QRISR;          /*!< Address offset:  0x34,  Raw Interrupt Status Register */
    volatile uint32_t QTXOICR;        /*!< Address offset:  0x38,  Transmit FIFO Overflow Interrupt Clear Register */
    volatile uint32_t QRXOICR;        /*!< Address offset:  0x3c,  Receive FIFO Overflow Interrupt Clear Register */
    volatile uint32_t QRXUICR;        /*!< Address offset:  0x40,  Receive FIFO Underflow Interrupt Clear Register */
    volatile uint32_t QMSTICR;        /*!< Address offset:  0x44,  Multi-Master Interrupt Clear Register */
    volatile uint32_t QICR;           /*!< Address offset:  0x48,  Interrupt Clear Register */
    volatile uint32_t QDMACR;         /*!< Address offset:  0x4c,  DMA Control Register */
    volatile uint32_t QDMATDLR;       /*!< Address offset:  0x50,  DMA Transmit Data Level */
    volatile uint32_t QDMARDLR;       /*!< Address offset:  0x54,  DMA Receive Data Level */
    volatile uint32_t QIDR;           /*!< Address offset:  0x58,  Identification Register */
    volatile uint32_t QSSIVI;         /*!< Address offset:  0x5c,  coreKit version ID Register */
    volatile uint32_t QDR[36];        /*!< Address offset:  0x60,  Data Register x */
    volatile uint32_t QRXSD;          /*!< Address offset:  0xf0,  RX Sample Delay Register */
    volatile uint32_t QSPICR0;        /*!< Address offset:  0xf4,  SPI_CTRLR0 */
    volatile uint32_t QRSVD;          /*!< Address offset:  0xfc,  RSVD - Reserved address location */
} QSPI_TypeDef;
/**
  * @}
  */

/** @addtogroup Peripheral_memory_map
  * @{
  */
#define QSPI_M_BASE    (0x00B90000UL)
#define QSPI_X_BASE    (0x00D00000UL)
/**
  * @}
  */

/** @addtogroup Peripheral_declaration
  * @{
  */
#define QSPI_M    ((QSPI_TypeDef *) QSPI_M_BASE)
#define QSPI_X    ((QSPI_TypeDef *) QSPI_X_BASE)
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
/*             Queued Serial Peripheral Interface             */
/*                                                            */
/**************************************************************/
/* Bits definition for QSPI_QCTRLR0 register ******************/
#define QSPI_SSTE_Pos            (24U)
#define QSPI_SSTE_Msk            (0x1UL << QSPI_SSTE_Pos)
#define QSPI_SSTE                QSPI_SSTE_Msk
#define QSPI_RSVD_CTRLR0_23_Pos  (23U)
#define QSPI_RSVD_CTRLR0_23_Msk  (0x1UL << QSPI_RSVD_CTRLR0_23_Pos)
#define QSPI_RSVD_CTRLR0_23      QSPI_RSVD_CTRLR0_23_Msk
#define QSPI_RSVD_SPI_FRF_Pos    (21U)
#define QSPI_RSVD_SPI_FRF_Msk    (0x3UL << QSPI_RSVD_SPI_FRF_Pos)
#define QSPI_RSVD_SPI_FRF        QSPI_RSVD_SPI_FRF_Msk
#define QSPI_DFS_32_Pos          (16U)
#define QSPI_DFS_32_Msk          (0x1FUL << QSPI_DFS_32_Pos)
#define QSPI_DFS_32              QSPI_DFS_32_Msk
#define QSPI_CFS_Pos             (12U)
#define QSPI_CFS_Msk             (0xFUL << QSPI_CFS_Pos)
#define QSPI_CFS                 QSPI_CFS_Msk
#define QSPI_SRL_Pos             (11U)
#define QSPI_SRL_Msk             (0x1UL << QSPI_SRL_Pos)
#define QSPI_SRL                 QSPI_SRL_Msk
#define QSPI_RSVD_CTRLR0_10_Pos  (10U)
#define QSPI_RSVD_CTRLR0_10_Msk  (0x1UL << QSPI_RSVD_CTRLR0_10_Pos)
#define QSPI_RSVD_CTRLR0_10      QSPI_RSVD_CTRLR0_10_Msk
#define QSPI_TMOD_Pos            (8U)
#define QSPI_TMOD_Msk            (0x3UL << QSPI_TMOD_Pos)
#define QSPI_TMOD                QSPI_TMOD_Msk
#define QSPI_SCPOL_Pos           (7U)
#define QSPI_SCPOL_Msk           (0x1UL << QSPI_SCPOL_Pos)
#define QSPI_SCPOL               QSPI_SCPOL_Msk
#define QSPI_SCPH_Pos            (6U)
#define QSPI_SCPH_Msk            (0x1UL << QSPI_SCPH_Pos)
#define QSPI_SCPH                QSPI_SCPH_Msk
#define QSPI_FRF_Pos             (4U)
#define QSPI_FRF_Msk             (0x3UL << QSPI_FRF_Pos)
#define QSPI_FRF                 QSPI_FRF_Msk
#define QSPI_RSVD_CTRLR0_3_0_Pos (0U)
#define QSPI_RSVD_CTRLR0_3_0_Msk (0xFUL << QSPI_RSVD_CTRLR0_3_0_Pos)
#define QSPI_RSVD_CTRLR0_3_0     QSPI_RSVD_CTRLR0_3_0_Msk
/* Bits definition for QSPI_QCTRLR1 register ******************/
#define QSPI_NDF_Pos             (0U)
#define QSPI_NDF_Msk             (0xFFFFUL << QSPI_NDF_Pos)
#define QSPI_NDF                 QSPI_NDF_Msk
/* Bits definition for QSPI_QSSIENR register ******************/
#define QSPI_SSI_EN_Pos          (0U)
#define QSPI_SSI_EN_Msk          (0x1UL << QSPI_SSI_EN_Pos)
#define QSPI_SSI_EN              QSPI_SSI_EN_Msk
/* Bits definition for QSPI_QMWCR register ********************/
#define QSPI_MHS_Pos             (2U)
#define QSPI_MHS_Msk             (0x1UL << QSPI_MHS_Pos)
#define QSPI_MHS                 QSPI_MHS_Msk
#define QSPI_MDD_Pos             (1U)
#define QSPI_MDD_Msk             (0x1UL << QSPI_MDD_Pos)
#define QSPI_MDD                 QSPI_MDD_Msk
#define QSPI_MWMOD_Pos           (0U)
#define QSPI_MWMOD_Msk           (0x1UL << QSPI_MWMOD_Pos)
#define QSPI_MWMOD               QSPI_MWMOD_Msk
/* Bits definition for QSPI_QSER register *********************/
#define QSPI_SER_Pos             (0U)
#define QSPI_SER_Msk             (0x1UL << QSPI_SER_Pos)
#define QSPI_SER                 QSPI_SER_Msk
/* Bits definition for QSPI_QBAUDR register *******************/
#define QSPI_SCKDV_Pos           (0U)
#define QSPI_SCKDV_Msk           (0xFFFFUL << QSPI_SCKDV_Pos)
#define QSPI_SCKDV               QSPI_SCKDV_Msk
/* Bits definition for QSPI_QTXFTLR register ******************/
#define QSPI_TFT_Pos             (0U)
#define QSPI_TFT_Msk             (0x1FUL << QSPI_TFT_Pos)
#define QSPI_TFT                 QSPI_TFT_Msk
/* Bits definition for QSPI_QRXFTLR register ******************/
#define QSPI_RFT_Pos             (0U)
#define QSPI_RFT_Msk             (0x1FUL << QSPI_RFT_Pos)
#define QSPI_RFT                 QSPI_RFT_Msk
/* Bits definition for QSPI_QTXFLR register *******************/
#define QSPI_TXTFL_Pos           (0U)
#define QSPI_TXTFL_Msk           (0x3FUL << QSPI_TXTFL_Pos)
#define QSPI_TXTFL               QSPI_TXTFL_Msk
/* Bits definition for QSPI_QRXFLR register *******************/
#define QSPI_RXTFL_Pos           (0U)
#define QSPI_RXTFL_Msk           (0x3FUL << QSPI_RXTFL_Pos)
#define QSPI_RXTFL               QSPI_RXTFL_Msk
/* Bits definition for QSPI_QSR register **********************/
#define QSPI_DCOL_Pos            (6U)
#define QSPI_DCOL_Msk            (0x1UL << QSPI_DCOL_Pos)
#define QSPI_DCOL                QSPI_DCOL_Msk
#define QSPI_RSVD_SR_5_Pos       (5U)
#define QSPI_RSVD_SR_5_Msk       (0x1UL << QSPI_RSVD_SR_5_Pos)
#define QSPI_RSVD_SR_5           QSPI_RSVD_SR_5_Msk
#define QSPI_RFF_Pos             (4U)
#define QSPI_RFF_Msk             (0x1UL << QSPI_RFF_Pos)
#define QSPI_RFF                 QSPI_RFF_Msk
#define QSPI_RFNE_Pos            (3U)
#define QSPI_RFNE_Msk            (0x1UL << QSPI_RFNE_Pos)
#define QSPI_RFNE                QSPI_RFNE_Msk
#define QSPI_TFE_Pos             (2U)
#define QSPI_TFE_Msk             (0x1UL << QSPI_TFE_Pos)
#define QSPI_TFE                 QSPI_TFE_Msk
#define QSPI_TFNF_Pos            (1U)
#define QSPI_TFNF_Msk            (0x1UL << QSPI_TFNF_Pos)
#define QSPI_TFNF                QSPI_TFNF_Msk
#define QSPI_BUSY_Pos            (0U)
#define QSPI_BUSY_Msk            (0x1UL << QSPI_BUSY_Pos)
#define QSPI_BUSY                QSPI_BUSY_Msk
/* Bits definition for QSPI_QIMR register *********************/
#define QSPI_MSTIM_Pos           (5U)
#define QSPI_MSTIM_Msk           (0x1UL << QSPI_MSTIM_Pos)
#define QSPI_MSTIM               QSPI_MSTIM_Msk
#define QSPI_RXFIM_Pos           (4U)
#define QSPI_RXFIM_Msk           (0x1UL << QSPI_RXFIM_Pos)
#define QSPI_RXFIM               QSPI_RXFIM_Msk
#define QSPI_RXOIM_Pos           (3U)
#define QSPI_RXOIM_Msk           (0x1UL << QSPI_RXOIM_Pos)
#define QSPI_RXOIM               QSPI_RXOIM_Msk
#define QSPI_RXUIM_Pos           (2U)
#define QSPI_RXUIM_Msk           (0x1UL << QSPI_RXUIM_Pos)
#define QSPI_RXUIM               QSPI_RXUIM_Msk
#define QSPI_TXOIM_Pos           (1U)
#define QSPI_TXOIM_Msk           (0x1UL << QSPI_TXOIM_Pos)
#define QSPI_TXOIM               QSPI_TXOIM_Msk
#define QSPI_TXEIM_Pos           (0U)
#define QSPI_TXEIM_Msk           (0x1UL << QSPI_TXEIM_Pos)
#define QSPI_TXEIM               QSPI_TXEIM_Msk
/* Bits definition for QSPI_QISR register *********************/
#define QSPI_MSTIS_Pos           (5U)
#define QSPI_MSTIS_Msk           (0x1UL << QSPI_MSTIS_Pos)
#define QSPI_MSTIS               QSPI_MSTIS_Msk
#define QSPI_RXFIS_Pos           (4U)
#define QSPI_RXFIS_Msk           (0x1UL << QSPI_RXFIS_Pos)
#define QSPI_RXFIS               QSPI_RXFIS_Msk
#define QSPI_RXOIS_Pos           (3U)
#define QSPI_RXOIS_Msk           (0x1UL << QSPI_RXOIS_Pos)
#define QSPI_RXOIS               QSPI_RXOIS_Msk
#define QSPI_RXUIS_Pos           (2U)
#define QSPI_RXUIS_Msk           (0x1UL << QSPI_RXUIS_Pos)
#define QSPI_RXUIS               QSPI_RXUIS_Msk
#define QSPI_TXOIS_Pos           (1U)
#define QSPI_TXOIS_Msk           (0x1UL << QSPI_TXOIS_Pos)
#define QSPI_TXOIS               QSPI_TXOIS_Msk
#define QSPI_TXEIS_Pos           (0U)
#define QSPI_TXEIS_Msk           (0x1UL << QSPI_TXEIS_Pos)
#define QSPI_TXEIS               QSPI_TXEIS_Msk
/* Bits definition for QSPI_QRISR register ********************/
#define QSPI_MSTIR_Pos           (5U)
#define QSPI_MSTIR_Msk           (0x1UL << QSPI_MSTIR_Pos)
#define QSPI_MSTIR               QSPI_MSTIR_Msk
#define QSPI_RXFIR_Pos           (4U)
#define QSPI_RXFIR_Msk           (0x1UL << QSPI_RXFIR_Pos)
#define QSPI_RXFIR               QSPI_RXFIR_Msk
#define QSPI_RXOIR_Pos           (3U)
#define QSPI_RXOIR_Msk           (0x1UL << QSPI_RXOIR_Pos)
#define QSPI_RXOIR               QSPI_RXOIR_Msk
#define QSPI_RXUIR_Pos           (2U)
#define QSPI_RXUIR_Msk           (0x1UL << QSPI_RXUIR_Pos)
#define QSPI_RXUIR               QSPI_RXUIR_Msk
#define QSPI_TXOIR_Pos           (1U)
#define QSPI_TXOIR_Msk           (0x1UL << QSPI_TXOIR_Pos)
#define QSPI_TXOIR               QSPI_TXOIR_Msk
#define QSPI_TXEIR_Pos           (0U)
#define QSPI_TXEIR_Msk           (0x1UL << QSPI_TXEIR_Pos)
#define QSPI_TXEIR               QSPI_TXEIR_Msk
/* Bits definition for QSPI_QTXOICR register ******************/
#define QSPI_TXOICR_Pos          (0U)
#define QSPI_TXOICR_Msk          (0x1UL << QSPI_TXOICR_Pos)
#define QSPI_TXOICR              QSPI_TXOICR_Msk
/* Bits definition for QSPI_QRXOICR register ******************/
#define QSPI_RXOICR_Pos          (0U)
#define QSPI_RXOICR_Msk          (0x1UL << QSPI_RXOICR_Pos)
#define QSPI_RXOICR              QSPI_RXOICR_Msk
/* Bits definition for QSPI_QRXUICR register ******************/
#define QSPI_RXUICR_Pos          (0U)
#define QSPI_RXUICR_Msk          (0x1UL << QSPI_RXUICR_Pos)
#define QSPI_RXUICR              QSPI_RXUICR_Msk
/* Bits definition for QSPI_QMSTICR register ******************/
#define QSPI_MSTICR_Pos          (0U)
#define QSPI_MSTICR_Msk          (0x1UL << QSPI_MSTICR_Pos)
#define QSPI_MSTICR              QSPI_MSTICR_Msk
/* Bits definition for QSPI_QICR register *********************/
#define QSPI_ICR_Pos             (0U)
#define QSPI_ICR_Msk             (0x1UL << QSPI_ICR_Pos)
#define QSPI_ICR                 QSPI_ICR_Msk
/* Bits definition for QSPI_QDMACR register *******************/
#define QSPI_TDMAE_Pos           (1U)
#define QSPI_TDMAE_Msk           (0x1UL << QSPI_TDMAE_Pos)
#define QSPI_TDMAE               QSPI_TDMAE_Msk
#define QSPI_RDMAE_Pos           (0U)
#define QSPI_RDMAE_Msk           (0x1UL << QSPI_RDMAE_Pos)
#define QSPI_RDMAE               QSPI_RDMAE_Msk
/* Bits definition for QSPI_QDMATDLR register *****************/
#define QSPI_DMATDL_Pos          (0U)
#define QSPI_DMATDL_Msk          (0x1FUL << QSPI_DMATDL_Pos)
#define QSPI_DMATDL              QSPI_DMATDL_Msk
/* Bits definition for QSPI_QDMARDLR register *****************/
#define QSPI_DMARDL_Pos          (0U)
#define QSPI_DMARDL_Msk          (0x1FUL << QSPI_DMARDL_Pos)
#define QSPI_DMARDL              QSPI_DMARDL_Msk
/* Bits definition for QSPI_QRXSD register ********************/
#define QSPI_RSD_Pos             (0U)
#define QSPI_RSD_Msk             (0xFFUL << QSPI_RSD_Pos)
#define QSPI_RSD                 QSPI_RSD_Msk
/* Bits definition for QSPI_QSPICR0 register ******************/
#define QSPI_CCR0_RXDS_Pos       (18U)
#define QSPI_CCR0_RXDS_Msk       (0x1UL << QSPI_CCR0_RXDS_Pos)
#define QSPI_CCR0_RXDS           QSPI_CCR0_RXDS_Msk
#define QSPI_CCR0_INS_DDR_Pos    (17U)
#define QSPI_CCR0_INS_DDR_Msk    (0x1UL << QSPI_CCR0_INS_DDR_Pos)
#define QSPI_CCR0_INS_DDR        QSPI_CCR0_INS_DDR_Msk
#define QSPI_CCR0_DDR_Pos        (16U)
#define QSPI_CCR0_DDR_Msk        (0x1UL << QSPI_CCR0_DDR_Pos)
#define QSPI_CCR0_DDR            QSPI_CCR0_DDR_Msk
#define QSPI_WAIT_CYCLES_Pos     (11U)
#define QSPI_WAIT_CYCLES_Msk     (0x1FUL << QSPI_WAIT_CYCLES_Pos)
#define QSPI_WAIT_CYCLES         QSPI_WAIT_CYCLES_Msk
#define QSPI_RSVD_SPI_CTRLR0_10_Pos (10U)
#define QSPI_RSVD_SPI_CTRLR0_10_Msk (0x1UL << QSPI_RSVD_SPI_CTRLR0_10_Pos)
#define QSPI_RSVD_SPI_CTRLR0_10  QSPI_RSVD_SPI_CTRLR0_10_Msk
#define QSPI_INST_L_Pos          (8U)
#define QSPI_INST_L_Msk          (0x3UL << QSPI_INST_L_Pos)
#define QSPI_INST_L              QSPI_INST_L_Msk
#define QSPI_RSVD_SPI_CTRLR0_6_7_Pos (6U)
#define QSPI_RSVD_SPI_CTRLR0_6_7_Msk (0x3UL << QSPI_RSVD_SPI_CTRLR0_6_7_Pos)
#define QSPI_RSVD_SPI_CTRLR0_6_7 QSPI_RSVD_SPI_CTRLR0_6_7_Msk
#define QSPI_ADDR_L_Pos          (2U)
#define QSPI_ADDR_L_Msk          (0xFUL << QSPI_ADDR_L_Pos)
#define QSPI_ADDR_L              QSPI_ADDR_L_Msk
#define QSPI_TRANS_TYPE_Pos      (0U)
#define QSPI_TRANS_TYPE_Msk      (0x3UL << QSPI_TRANS_TYPE_Pos)
#define QSPI_TRANS_TYPE          QSPI_TRANS_TYPE_Msk

#endif /* _QSPI_REGS_H_ */
