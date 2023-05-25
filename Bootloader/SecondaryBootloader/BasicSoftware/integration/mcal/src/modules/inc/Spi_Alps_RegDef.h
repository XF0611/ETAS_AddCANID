/************************************************************************************ 
*  Copyright (c) 2020, Synopsys, Inc. All rights reserved.
*
*  SPI driver is the property of Synopsys, Inc.
*  QSPI driver is property of Calterah Semiconductor, Inc.
*
*  Redistribution and use in source and binary forms, with or without modification,
*  are permitted provided that the following conditions are met:
*
*  1) Redistributions of source code must retain the above copyright notice, this 
*     list of conditions and the following disclaimer.
*
*  2) Redistributions in binary form must reproduce the above copyright notice, this
*     list of conditions and the following disclaimer in the documentation and/or 
*     other materials provided with the distribution.
*
*  3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
*     be used to endorse or promote products derived from this software without 
*     specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
*  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
*  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
*  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
*  OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************************/

#ifndef SPI_ALPS_REGDEF_H
#define SPI_ALPS_REGDEF_H

/* status register. */
#define SPI_RX_FIFO_FULL          (1 << 4)
#define SPI_RX_FIFO_NOT_EMPTY     (1 << 3)
#define SPI_TX_FIFO_EMPTY         (1 << 2)
#define SPI_TX_FIFO_NOT_FULL      (1 << 1)
#define SPI_DEVICE_BUSY           (1 << 0)

typedef volatile struct {
    uint32 CTRLR0;              /* SPI Control Register 0  (0x0) */
    uint32 CTRLR1;              /* SPI Control Register 1  (0x4) */
    uint32 SSIENR;              /* SPI Enable Register  (0x8) */
    uint32 MWCR;                /* SPI Microwire Control Register  (0xC) */
    uint32 SER;                 /* SPI Slave Enable Register  (0x10) */
    uint32 BAUDR;               /* SPI Baud Rate Select Register  (0x14) */
    uint32 TXFTLR;              /* SPI Transmit FIFO Threshold Level Register  (0x18) */
    uint32 RXFTLR;              /* SPI Receive  FIFO Threshold Level Register  (0x1C) */
    uint32 TXFLR;               /* SPI Transmit FIFO Level Register  (0x20) */
    uint32 RXFLR;               /* SPI Receive  FIFO Level Register  (0x24) */
    uint32 SR;                  /* SPI Status Register  (0x28) */
    uint32 IMR;                 /* SPI Interrupt Mask Register  (0x2C) */
    uint32 ISR;                 /* SPI Interrupt Status Register  (0x30) */
    uint32 RISR;                /* SPI Raw Interrupt Status Register (0x34) */
    uint32 TXOICR;              /* SPI Transmit FIFO Overflow Interrupt Clear Register  (0x38) */
    uint32 RXOICR;              /* SPI Receive  FIFO Overflow Interrupt Clear Register  (0x3C) */
    uint32 RXUICR;              /* SPI Receive FIFO Underflow Interrupt Clear Register  (0x40) */
    uint32 MSTICR;              /* SPI Multi-Master Interrupt Clear Register  (0x44) */
    uint32 ICR;                 /* SPI Interrupt Clear Register  (0x48) */
    uint32 DMACR;               /* DMA Control Register  (0x4C) */
    uint32 DMATDLR;             /* DMA Transmit Data Level  (0x50) */
    uint32 DMARDLR;             /* DMA Receive Data Level  (0x54) */
    uint32 IDR;                 /* SPI Identification Register  (0x58) */
    uint32 SSI_VERSION_ID;      /* SPI CoreKit ID Register (Value after Reset : 0x3332322A)  (0x5C) */
    uint32 DR;                  /* Data Register for both Read and Write (0x60) */
    uint32 reserved_space[35];  /* Reserved Space (0x64-0xEC) */
    uint32 RX_SAMPLE_DLY;       /* RxD Sample Delay Register (0xf0) */
    uint32 SPI_CTRLR0;          /*SPI Control Register (0xf4) */
    uint32 RSVD;                /*RSVD - Reserved address location (0xfc) */
} Spi_reg_t;

#endif /* SPI_ALPS_REGDEF_H */

