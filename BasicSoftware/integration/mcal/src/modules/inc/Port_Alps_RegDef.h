/*******************************************************************************
**                                                                            **
** Copyright (C) 2020, Calterah Semiconductor, Inc.                           **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Calterah       **
** Semiconductor, Inc. Passing on and copying of this document, and           **
** communication of its contents is not permitted without prior written       **
** authorization.                                                             **
**                                                                            **
*******************************************************************************/

#ifndef PORT_ALPS_REGDEF_H
#define PORT_ALPS_REGDEF_H

typedef volatile struct 
{ 
    uint32  DMU_CMD_SRC_SEL;        /* RW   0x000   Radio Command Source Selection*/
    uint32  DMU_CMD_OUT;            /* RW   0x004   Radio Command Write*/
    uint32  DMU_CMD_IN;             /* R    0x008   Radio Command Read*/
    uint32  DMU_ADC_RSTN;           /* RW   0x00C   Radio ADC Resetn Output*/
    uint32  DMU_ADC_CNT;            /* RW   0x010   Radio ADC Count Output*/
    uint32  DMU_FMCW_START_SRC;     /* RW   0x014   Radio FMCW Start Sourse*/
    uint32  DMU_FMCW_START;         /* RW   0x018   Radio FMCW Start*/
    uint32  DMU_FMCW_STATUS;        /* R    0x01C   Radio FMCW Status*/
    
    uint32  Reserved1[0x38];
    uint32  DMU_DBG_SRC;            /* RW   0x0100  DebugBus Source*/
    uint32  DMU_DBG_VAL_OEN;        /* RW   0x0104  DebugBus Valid Output Enable*/
    uint32  DMU_DBG_DAT_OEN;        /* RW   0x0108  DebugBus Data Ouput Enable*/
    uint32  DMU_DBG_DOUT;           /* RW   0x010C  DebugBus Output Data*/
    uint32  DMU_DBG_DIN;            /* R    0x0110  DebugBus Input Data*/
    uint32  DMU_HIL_ENA;            /* RW   0x0114  Hardware in Loop Enable*/
    uint32  DMU_HIL_DAT;            /* RW   0x0118  Hardware in Loop Data*/
    
    uint32  Reserved2[0x39];
    uint32  DMU_MUX_QSPI;           /* RW   0x0200  IO Mux for QSPI*/
    uint32  DMU_MUX_SPI_M1;         /* RW   0x204   IO Mux for SPI_M1*/
    uint32  DMU_MUX_UART_0;         /* RW   0x208   IO Mux for UART_0*/
    uint32  DMU_MUX_UART_1;         /* RW   0x20C   IO Mux for UART_1*/
    uint32  DMU_MUX_CAN_0;          /* RW   0x210   IO Mux for CAN_0*/
    uint32  DMU_MUX_CAN_1;          /* RW   0x214   IO Mux for CAN_1*/
    uint32  DMU_MUX_RESET;          /* RW   0x218   IO Mux for RESET*/
    uint32  DMU_MUX_SYNC;           /* RW   0x21C   IO Mux for SYNC*/
    uint32  DMU_MUX_I2C_M;          /* RW   0x220   IO Mux for I2C_M*/
    uint32  DMU_MUX_PWM_0;          /* RW   0x224   IO Mux for PWM_0*/
    uint32  DMU_MUX_PWM_1;          /* RW   0x228   IO Mux for PWM_1*/
    uint32  DMU_MUX_ADC_CLK;        /* RW   0x22C   IO Mux for ADC_CLK*/
    uint32  DMU_MUX_CAN_CLK;        /* RW   0x230   IO Mux for CAN_CLK*/
    uint32  DMU_MUX_SPI_M0;         /* RW   0x234   IO Mux for SPI_M0*/
    uint32  DMU_MUX_SPI_S;          /* RW   0x238   IO Mux for SPI_S*/
    uint32  DMU_MUX_SPI_S1_CLK;     /* RW   0x23C   IO Mux for SPI_S1_CLK*/
    uint32  DMU_MUX_SPI_S1_SEL;     /* RW   0x240   IO Mux for SPI_S1_SEL*/
    uint32  DMU_MUX_SPI_S1_MOSI;    /* RW   0x244   IO Mux for SPI_S1_MOSI*/
    uint32  DMU_MUX_SPI_S1_MISO;    /* RW   0x248   IO Mux for SPI_S1_MISO*/
    uint32  DMU_MUX_JTAG;           /* RW   0x24C   IO Mux for JTAG*/
    uint32  Reserved3[0x2c];
    
    uint32  DMU_SYS_DMA_ENDIAN;     /* RW   0x300   System DMA Endian*/
    uint32  DMU_SYS_SHSEL_NP;       /* RW   0x304   System EH2H shsel_np*/
    uint32  DMU_SYS_DMU_SEL;        /* RW   0x308   System IO MUX Select*/
    uint32  DMU_SYS_ICM0_FIX_P;     /* RW   0x30C   System ICM 0 fix Priority*/
    uint32  DMU_SYS_ICM1_FIX_P;     /* RW   0x310   System ICM 1 fix Priority*/
    uint32  DMU_SYS_PWM_ENA;        /* RW   0x314   System PWM Enable register*/
    uint32  DMU_SYS_MEMRUN_ENA;     /* RW   0x318   System MEMRUN Enable*/
    uint32  DMU_SYS_MEMINI_ENA;     /* RW   0x31C   System MEMINI Enable*/
    uint32  DMU_SYS_SPI_LOOP;       /* RW   0x320   System SPI Loopback*/
    uint32  DMU_SYS_OTP_PRGM_EN;    /* RW   0x324   System OTP Program Enable*/
    uint32  DMU_SYS_OTP_ECC_EN;     /* RW   0x328   System OTP ECC Enable*/
    uint32  DMU_SYS_MEM_CLR;        /*RW    0x32C   System Memory Clear*/ 
    uint32  DMU_SYS_MEM_CLR_DONE;   /*RW    0x330   System Memory Clear Done*/
    uint32  Reserved4[0x33];
    uint32  DMU_SW_IRQ_TRIG;        /* RW   0x400   CPU Interrupt Trigger*/
    uint32  DMU_SW_IRQ_STA;         /* R    0x404   CPU Interrupt Status*/
    uint32  Reserved5[0x3e];
    uint32  DMU_IRQ_ENA_0_31;       /* RW   0x500   Interrupt Enable*/
    uint32  DMU_IRQ_ENA_32_63;      /* RW   0x504   Interrupt Enable*/
    uint32  DMU_IRQ_ENA_64_95;      /* RW   0x508   Interrupt Enable*/
    uint32  DMU_IRQ_ENA_96_127;     /* RW   0x50C   Interrupt Enable*/ 
    uint32  DMU_IRQ25_SEL;          /* RW   0x510   IRQ25 Select*/
    uint32  DMU_IRQ26_SEL;          /* RW   0x514   IRQ26 Select*/
    uint32  DMU_IRQ27_SEL;          /* RW   0x518   IRQ27 Select*/
    uint32  DMU_IRQ28_SEL;          /* RW   0x51C   IRQ28 Select*/
    uint32  DMU_IRQ29_SEL;          /* RW   0x520   IRQ29 Select*/
    uint32  DMU_IRQ30_SEL;          /* RW   0x524   IRQ30 Select*/
    uint32  DMU_IRQ31_SEL;          /* RW   0x528   IRQ31 Select*/
}  Port_DMU_t;

typedef volatile struct 
{
    uint32   GPIO_SWPORTA_DR ;          /*  R/W     0x00 Port A data register*/
    uint32   GPIO_SWPORTA_DDR;          /*  R/W     0x04 Port A data direction register*/
    uint32   Reserved1;                 /*  NA      0x08 */
    uint32   GPIO_SWPORTB_DR;           /*  R/W     0x0c Port B data register */
    uint32   GPIO_SWPORTB_DDR;          /*  R/W     0x10 Port B data direction register*/
    uint32   Reserved2[7];              /*  NA      0x14 - 2C */
    uint32   GPIO_INTEN;                /*  R/W     0x30 Interrupt enable register*/
    uint32   GPIO_INTMASK;              /*  R/W     0x34 Interrupt mask register*/
    uint32   GPIO_INTTYPE_LEVEL;        /*  R/W     0x38 Interrupt level register*/
    uint32   GPIO_INT_POLARITY;         /*  R/W     0x3c Interrupt polarity register*/
    uint32   GPIO_INTSTATUS;            /*  R/W     0x40 Interrupt status of Port A*/
    uint32   GPIO_RAW_INTSTATUS;        /*  R/W     0x44 Raw interrupt status of Port A*/
    uint32   GPIO_DEBOUNCE;             /*  R/W     0x48 Debounce enable register*/
    uint32   GPIO_PORTA_EOI;            /*  R/W     0x4c Port A clear interrupt register*/
    uint32   GPIO_EXT_PORTA;            /*  W       0x50 Port A external port register*/
    uint32   GPIO_EXT_PORTB;            /*  W       0x54 Port B external port register*/
    uint32   Reserved3[2];              /*  NA      0x58 - 5C */
    uint32   GPIO_LS_SYNC;              /*  R       0x60 Level-sensitive synchronization enable register*/
    uint32   GPIO_ID_CODE;              /*  R/W     0x64   ID code register*/
    uint32   GPIO_INT_BOTHEDGE;         /*  R       0x68 Interrupt both edge type*/
    uint32   GPIO_VER_ID_CODE;          /*  R/W     0x6c Component Version register*/
    uint32   GPIO_CONFIG_REG2;          /*  R       0x70 Configuration Register 2*/
    uint32   GPIO_CONFIG_REG1;          /*  R       0X74 Configuration Register 1*/
}Port_GPIO_t;

typedef struct 
{
    Port_DMU_t *DMU;
    Port_GPIO_t *GPIO;

}Port_reg_t;

#endif


