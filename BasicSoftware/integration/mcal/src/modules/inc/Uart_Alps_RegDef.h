/************************************************************************************ 
*  Copyright (c) 2020, Synopsys, Inc. All rights reserved.
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
#ifndef UART_ALPS_REGDEF_H
#define UART_ALPS_REGDEF_H

typedef volatile struct 
{
    /* union for (RBR,THR,DLL) register at Address Offset: 0x00 */
    union
    {
     /* Receive Buffer Register */
        uint32   RBR;
     /* Transmit Holding Register */
        uint32   THR;
     /* Divisor Latch Low */
        uint32   DLL;
    }Uart_0x00_Reg;
    /* union for (DLH,IER)register at Address Offset: 0x04 */
    union
    {
    /* Divisor Latch (High) Register */
        uint32 DLH;
    /* Interrupt Enable Register */
        uint32 IER;
    }Uart_0x04_Reg;
    /* union for (IIR,FCR)register at Address Offset: 0x08 */
    union
    {
    /* Interrupt Identification Register */
        uint32 IIR;
    /* FIFO Control Register */
        uint32 FCR;
    }Uart_0x08_Reg;
    /* Line Control Register at Address Offset: 0x0C */
        uint32 LCR;
    /* Modem Control Register at Address Offset: 0x10 */
        uint32 MCR;
    /* Line Status Register at Address Offset: 0x14 */
        uint32 LSR;
    /* Modem Status Register at Address Offset: 0x18 */
        uint32 MSR;
    /* Scratchpad Register at Address Offset: 0x1C */
        uint32 SCR;
    /* Low Power Divisor Latch (Low) Register at Address Offset: 0x20 */
        uint32 LPDLL;
    /* Low Power Divisor Latch (High) Register at Address Offset: 0x24 */
        uint32 LPDLH;
    /* Reserved space from 0x28 - 0x2C */
        uint32 Reserved_0x28_to_0x2C [2];
    /* union for (RBR,THR,DLL) register at Address Offset: 0x30 -0x6C */
    union
    {
    /* Shadow Receive Buffer Register */
        uint32 SRBR[16];
    /* Shadow Transmit Holding Register */
        uint32 STHR[16] ;
    }Uart_0x30_Reg;
    /* FIFO Access Register at Address Offset: 0x70 */
    uint32 FAR;
    /* Transmit FIFO Read at Address Offset: 0x74 */
    uint32 TFR;
    /* Receive FIFO Write at Address Offset: 0x78 */
    uint32 RFW;
    /* UART Status Register at Address Offset: 0x7C */
    uint32 USR;
    /* Transmit FIFO Level  at Address Offset: 0x80 */
    uint32 TFL;
    /* Receive FIFO Level at Address Offset: 0x84  */
    uint32  RFL;
    /* Software Reset Register at Address Offset: 0x88 */
    uint32 SRR;
    /* Shadow Request to Send at Address Offset: 0x8C */
    uint32 SRTS;
    /* Shadow Break Control Register at Address Offset: 0x90 */
    uint32 SBCR;
    /* Shadow DMA Mode Register at Address Offset: 0x94 */
    uint32 SDMAM;
    /* Shadow FIFO Enable Register at Address Offset: 0x98 */
    uint32 SFE;
    /* Shadow RCVR Trigger Register at Address Offset: 0x9C */
    uint32 SRT;
    /* Shadow TX Empty Trigger Register at Address Offset: 0xA0 */
    uint32 STET;
    /* Halt TX Trigger Register at Address Offset: 0xA4 */
    uint32 HTX;
    /* Reserved space from 0xD0 to 0xF0 i.e 8 bytes */
    uint32 Reserved_0xD0_to_0xF0[19];
    /* Component Parameter Register at Address Offset: 0xF4 */
    uint32 CPR;
    /* UART Component Version at Address Offset: 0xF8 */
    uint32 UCV;
    /* Component Type Register at Address Offset: 0xFC */
    uint32 CTR;

}Uart_RegType;

#endif /* UART_ALPS_REGDEF_H */
