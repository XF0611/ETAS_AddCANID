/*============================================================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
=============================================================================*/



/*
This file contain the register memory map for the Alps_Can
Note: All are 32 bit registers
*/
#ifndef CAN_ALPS_REGDEF_H
#define CAN_ALPS_REGDEF_H


/*****************************************************************
*                        Peripheral : CAN                        *
*****************************************************************/

volatile typedef struct
{
    /* Core Release Register at Address Offset: 0x00 */
    uint32 CRR;
    /* Mode Control Register at Address Offset: 0x0004 */
    uint32 MCR;
    /* Protocol Control Register at Address Offset: 0x0008  */
    uint32 PCR;
    /* Timestamp Counter Configuration Register at Address Offset: 0x000C */
    uint32 TSCCR;
    /* Timestamp Counter Value Register at Address Offset: 0x0010 */
    uint32 TSCVR;
    /* Data Bit Time Control Register at Address Offset: 0x0014 */
    uint32 DBTCR;
    /* Nominal Bit Time Control Register at Address Offset: 0x0018 */
    uint32 NBTCR;
    /* Error Counter Register at Address Offset: 0x001C  */
    uint32 ECR;
    /* Protocol status Register at Address Offset: 0x0020 */
    uint32 PSR;
    /* ECC Error Status Register at Address Offset: 0x0024 */
    uint32 EESR;
    /* Transmitter Delay Compensation Register at Address Offset: 0x0028 */
    uint32 TDCR;
    /* Timeout Counter Register at Address Offset: 0x002C */
    uint32 TOCR;
    /* Timeout Counter Value Register at Address Offset: 0x0030 */
    uint32 TOCVR;
    /* Interrupt Register at Address Offset: 0x0034  */
    uint32 IR;
    /* Interrupt Enable Register at Address Offset: 0x0038 */
    uint32 IE;
    /* Interrupt Line Select 0 Register at Address Offset: 0x003c */
    uint32 ILS0R;
    /* Interrupt Line Select 1 Register at Address Offset: 0x0040 */
    uint32 ILS1R;
    /* Interrupt Line Enable Register at Address Offset: 0x0044 */
    uint32 ILE;
    /* ID Filter Control Register at Address Offset: 0x0048  */
    uint32 IDFCR;
    /* Extended ID AND Mask Register at Address Offset: 0x004c */
    uint32 XIDAMR;

    /* RX Buffer/FIFO Element Size Configuration Register at Address Offset: 0x0050 */
    uint32 RXESCR;
    /* RX Buffer Status Register 0 at Address Offset: 0x0054 */
    uint32 RXBS0R;
    /* RX Buffer Status Register 1 at Address Offset: 0x0058 */
    uint32 RXBS1R;
    /* RX FIFO Configuration Register at Address Offset: 0x005c */
    uint32 RXFCFGR;
    /* RX FIFO Status Register at Address Offset: 0x0060  */
    uint32 RXFSTAR;

    /* TX Buffer/FIFO Element Size Configuration Register at Address Offset: 0x0064 */
    uint32 TXESCR;
    /* TX Buffer Request Pending Register 0 at Address Offset: 0x0068 */
    uint32 TXBRPR0;
    /* TX Buffer Request Pending Register 1 at Address Offset: 0x006c */
    uint32 TXBRPR1;
    /* TX Buffer Add Request Register 0 at Address Offset: 0x0070 */
    uint32 TXBARR0;
    /* TX Buffer Add Request Register 1 at Address Offset: 0x0074 */
    uint32 TXBARR1;
    /* TX Buffer Cancellation Request Register 0 at Address Offset: 0x0078  */
    uint32 TXBCRR0;
    /* TX Buffer Cancellation Request Register 1 at Address Offset: 0x007c */
    uint32 TXBCRR1;
    /* TX Buffer Transmission Occurred Register 0 at Address Offset: 0x0080 */
    uint32 TXBTOR0;
    /* TX Buffer Transmission Occurred Register 1 at Address Offset: 0x0084 */
    uint32 TXBTOR1;
    /* TX Buffer Cancellation Finished Register 0 at Address Offset: 0x0088 */
    uint32 TXBCFR0;
    /* TX Buffer Cancellation Finished Register 1 at Address Offset: 0x008c */
    uint32 TXBCFR1;
    /* TX Buffer Transmission Interrupt Enable Register 0 at Address Offset: 0x0090  */
    uint32 TXBTIER0;
    /* TX Buffer Transmission Interrupt Enable Register 1 at Address Offset: 0x0094 */
    uint32 TXBTIER1;
    /* TX Buffer Cancellation Finished Interrupt Enable Register 0 at Address Offset: 0x0098 */
    uint32 TXBCIER0;
    /* TX Buffer Cancellation Finished Interrupt Enable Register 1 at Address Offset: 0x009c */
    uint32 TXBCIER1;
    /* TX FIFO Configuration Register at Address Offset: 0x00A0 */
    uint32 TXFCFGR;
    /* TX FIFO Status Register at Address Offset: 0x00A4 */
    uint32 TXFSTAR;
    /* TX Event FIFO Configuration Register at Address Offset: 0x00A8 */
    uint32 TXEFCFGR;
    /* TX Event FIFO Status Register at Address Offset: 0x00Ac */
    uint32 TXEFSTAR;

    /* RX FIFO Element Register 0 at Address Offset: 0x00B0 */
    uint32 RXFELMR0;
    /* RX FIFO Element Register 1 at Address Offset: 0x00B4 */
    uint32 RXFELMR1;
    /* RX FIFO Element Register 2 at Address Offset: 0x00B8 */
    uint32 RXFELMR2;
    /* RX FIFO Element Register 3 at Address Offset: 0x00Bc */
    uint32 RXFELMR3;
    /* RX FIFO Element Register 4 at Address Offset: 0x00c0 */
    uint32 RXFELMR4;
    /* RX FIFO Element Register 5 at Address Offset: 0x00C4 */
    uint32 RXFELMR5;
    /* RX FIFO Element Register 6 at Address Offset: 0x00C8 */
    uint32 RXFELMR6;
    /* RX FIFO Element Register 7 at Address Offset: 0x00Cc */
    uint32 RXFELMR7;
    /* RX FIFO Element Register 8 at Address Offset: 0x00D0 */
    uint32 RXFELMR8;
    /* RX FIFO Element Register 9 at Address Offset: 0x00D4 */
    uint32 RXFELMR9;
    /* RX FIFO Element Register 10 at Address Offset: 0x00D8 */
    uint32 RXFELMR10;
    /* RX FIFO Element Register 11 at Address Offset: 0x00DC */
    uint32 RXFELMR11;
    /* RX FIFO Element Register 12 at Address Offset: 0x00E0 */
    uint32 RXFELMR12;
    /* RX FIFO Element Register 13 at Address Offset: 0x00E4 */
    uint32 RXFELMR13;
    /* RX FIFO Element Register 14 at Address Offset: 0x00E8 */
    uint32 RXFELMR14;
    /* RX FIFO Element Register 15 at Address Offset: 0x00EC */
    uint32 RXFELMR15;
    /* RX FIFO Element Register 16 at Address Offset: 0x00F0 */
    uint32 RXFELMR16;
    /* RX FIFO Element Register 17 at Address Offset: 0x00F4 */
    uint32 RXFELMR17;

    /* TX FIFO Element Register 0 at Address Offset: 0x00F8 */
    uint32 TXFELMR0;
    /* TX FIFO Element Register 1 at Address Offset: 0x00FC */
    uint32 TXFELMR1;
    /* TX FIFO Element Register 2 at Address Offset: 0x0100 */
    uint32 TXFELMR2;
    /* TX FIFO Element Register 3 at Address Offset: 0x0104 */
    uint32 TXFELMR3;
    /* TX FIFO Element Register 4 at Address Offset: 0x0108 */
    uint32 TXFELMR4;
    /* TX FIFO Element Register 5 at Address Offset: 0x010c */
    uint32 TXFELMR5;
    /* TX FIFO Element Register 6 at Address Offset: 0x0110 */
    uint32 TXFELMR6;
    /* TX FIFO Element Register 7 at Address Offset: 0x0114 */
    uint32 TXFELMR7;
    /* TX FIFO Element Register 8 at Address Offset: 0x0118 */
    uint32 TXFELMR8;
    /* TX FIFO Element Register 9 at Address Offset: 0x011c */
    uint32 TXFELMR9;
    /* TX FIFO Element Register 10 at Address Offset: 0x0120 */
    uint32 TXFELMR10;
    /* TX FIFO Element Register 11 at Address Offset: 0x0124 */
    uint32 TXFELMR11;
    /* TX FIFO Element Register 12 at Address Offset: 0x0128 */
    uint32 TXFELMR12;
    /* TX FIFO Element Register 13 at Address Offset: 0x012c */
    uint32 TXFELMR13;
    /* TX FIFO Element Register 14 at Address Offset: 0x0130 */
    uint32 TXFELMR14;
    /* TX FIFO Element Register 15 at Address Offset: 0x0134 */
    uint32 TXFELMR15;
    /* TX FIFO Element Register 16 at Address Offset: 0x0138 */
    uint32 TXFELMR16;
    /* TX FIFO Element Register 17 at Address Offset: 0x013c */
    uint32 TXFELMR17;
    /* TX Event FIFO Element Register 0 at Address Offset: 0x0140 */
    uint32 TXEFELMR0;
    /* TX Event FIFO Element Register 1 at Address Offset: 0x0144 */
    uint32 TXEFELMR1;

} Can_Reg_t;

#endif    

