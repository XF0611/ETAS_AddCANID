/* ==========================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
========================================== */

#ifndef GPT_ALPS_REGDEF_H
#define GPT_ALPS_REGDEF_H

typedef volatile struct
{
	/* Timer 1 Registers */
	uint32 TIMER1_LOADCOUNT;          /* 0x00  Value to be loaded into Timer 1 */
	uint32 TIMER1_CURRENTVALUE;       /* 0x04  Current value of Timer 1 */
	uint32 TIMER1_CONTROLREG;         /* 0x08  Control Register for Timer 1 */
	uint32 TIMER1_EOI;                /* 0x0C  Timer 1 End-of-Interrupt Register */
	uint32 TIMER1_INTSTATUS;          /* 0x10  Timer 1 Interrupt Status Register */
	
	/* Timer 2 Registers */
	uint32 TIMER2_LOADCOUNT;          /* 0x14  Value to be loaded into Timer 2 */
	uint32 TIMER2_CURRENTVALUE;       /* 0x18  Current value of Timer 2 */
	uint32 TIMER2_CONTROLREG;         /* 0x1C  Control Register for Timer 2 */
	uint32 TIMER2_EOI;                /* 0x20  Timer 2 End-of-Interrupt Register */
	uint32 TIMER2_INTSTATUS;          /* 0x24  Timer 2 Interrupt Status Register */
	
	/* Timer 3 Registers */
	uint32 TIMER3_LOADCOUNT;          /* 0x28  Value to be loaded into Timer 3 */
	uint32 TIMER3_CURRENTVALUE;       /* 0x2C  Current value of Timer 3 */
	uint32 TIMER3_CONTROLREG;         /* 0x30  Control Register for Timer 3 */
	uint32 TIMER3_EOI;                /* 0x34  Timer 3 End-of-Interrupt Register */
	uint32 TIMER3_INTSTATUS;          /* 0x38  Timer 3 Interrupt Status Register */
	
	/* Timer 4 Registers */
	uint32 TIMER4_LOADCOUNT;          /* 0x3C  Value to be loaded into Timer 4 */
	uint32 TIMER4_CURRENTVALUE;       /* 0x40  Current value of Timer 4 */
	uint32 TIMER4_CONTROLREG;         /* 0x44  Control Register for Timer 4 */
	uint32 TIMER4_EOI;                /* 0x48  Timer 4 End-of-Interrupt Register */
	uint32 TIMER4_INTSTATUS;          /* 0x4C  Timer 4 Interrupt Status Register */
	
	uint32 Reserved[20];
	
	/* Global Timer Registers */
    uint32 TIMERS_INTSTATUS;          /* 0xA0  Timers Interrupt Status Register */
	uint32 TIMERS_EOI;                /* 0xA4  Timers End-of-Interrupt Register */
	uint32 TIMERS_RAWINTSTATUS;       /* 0xA8  Timers Raw Interrupt Status Register  */
	uint32 TIMERS_COMP_VERSION;       /* 0xAC  Current revision number of the Timer component */
	
} Gpt_Reg_t;

#endif
