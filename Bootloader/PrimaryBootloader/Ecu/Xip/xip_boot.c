/*
 * xip_boot.c
 *
 *  Created on: Sep 19, 2022
 *      Author: Neo
 */

#include "xip_boot.h"
#include "xip_regs.h"
#include "qspi_regs.h"
#include "Spi.h"
#include "arc_builtin.h"
#include "arc_cache.h"
#include "alps_clock.h"

#include <stddef.h>

typedef void (*next_image_entry)(void);

void flash_xip_init_early(void)
{
    xip_enable(1);

    /* QSPI related Register and XIP Register share the same start address - 0x00D00000U */
    /* The offset of QSPI related register is 0x0 */
    /* The offset of XIP related register is 0x100 */

    /* Configure QSPI related Register */
    QSPI_X->QCTRLR0 = ((2 << QSPI_RSVD_SPI_FRF_Pos) & QSPI_RSVD_SPI_FRF_Msk) |
            ((31 << QSPI_DFS_32_Pos) & QSPI_DFS_32_Msk) |
            ((2 << QSPI_TMOD_Pos) & QSPI_TMOD_Msk);
    QSPI_X->QCTRLR1 = 0x7ff;
    QSPI_X->QBAUDR = 0xa;
    QSPI_X->QRXSD = 0;
    QSPI_X->QSPICR0 = ((1 << QSPI_TRANS_TYPE_Pos) & QSPI_TRANS_TYPE_Msk) |
            ((6 << QSPI_ADDR_L_Pos) & QSPI_ADDR_L_Msk) |
            ((2 << QSPI_INST_L_Pos) & QSPI_INST_L_Msk) |
            ((6 << QSPI_WAIT_CYCLES_Pos) & QSPI_WAIT_CYCLES_Msk);
    QSPI_X->QSER = 1;
    QSPI_X->QTXFTLR = 1;
    QSPI_X->QRXFTLR = 0;
    QSPI_X->QIMR = 0x3f;

    /* Configure XIP related Register */
    /* XIP Read Command Register - XRDCR */
    XIP->XRDCR =  ((0xeb) & 0xff) |
            ((2 << XIP_READ_INST_LEN_Pos) & XIP_READ_INST_LEN_Msk) |
            ((6 << XIP_READ_ADDR_LEN_Pos) & XIP_READ_ADDR_LEN_Msk) |
            ((31 << XIP_READ_DFS_Pos) & XIP_READ_DFS_Msk);

    /* XIP Instruction Section Offset Register - XISOR */
    XIP->XISOR = 0;

    /* XIP AHB Bus Endian Control Register - XABECR */
    XIP->XABECR = ((0x3 << XIP_WR_ENDIAN_Pos) & XIP_WR_ENDIAN_Msk) |
            ((0x3 << XIP_RD_ENDIAN_Pos) & XIP_RD_ENDIAN_Msk);

    /* XIP AES Data Bus Endian Control Register - XADBECR */
    XIP->XADBECR = ((0x0 << XIP_WORD_SWAP_Pos) & XIP_WORD_SWAP_Msk) |
            ((0x0 << XIP_BYTE_SWAP_Pos) & XIP_BYTE_SWAP_Msk) |
            ((0x0 << XIP_BIT_SWAP_Pos) & XIP_BIT_SWAP_Msk);

    /* AES Mode Register - AMR */
    XIP->AMR = 0x07;

    /* XIP Instruction Buffer Control Register - XIBCR */
    XIP->XIBCR = 1;

    /* XIP Data Section Offset Register - XDSOR */
    XIP->XDSOR = 0;

    /* XIP Read Buffer Control Register - XRBCR */
    XIP->XRBCR = 1;

    /* Enable QSPI */
    QSPI_X->QSSIENR = 1;

    /* XIP Enable Register - XER */
    XIP->XER = 1;

    while (XIP->XER == 0) {
        chip_hw_mdelay(1);
    }
}

void xip_boot(void)
{
    next_image_entry firmware_entry;

    /* firmware memory mapping address */
    if (raw_readl(0xb00004) & 1) {

    }

    /* Boot firmware image under XIP mode */
    /* send command to configure external nor flash to Quad mode. */
    uint8_t tCmd[2] = {0x2, 0x2};
    Spi_WriteIB(SpiConf_SpiChannel_QSPI_WriteCfg, tCmd);
    Spi_SyncTransmit(SpiConf_SpiSequence_WriteCfg);

    /* configure XIP controller: */
    flash_xip_init_early();

    /* firmware memory mapping address */
//     firmware_entry = (next_image_entry)(0x350004);

//     _arc_aux_write(0x4B, 1);
//     while (_arc_aux_read(0x48) & 0x100);
//     icache_invalidate();

//     firmware_entry();
}
