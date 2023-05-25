/* Register Definations */

#ifndef MCU_ALPS_REGDEF_H
#define MCU_ALPS_REGDEF_H

typedef volatile struct
{
	uint32  CLKGEN_READY_50M;           /* 0x000 Ready indicator for Crystal Clock */
	uint32  CLKGEN_READY_PLL;           /* 0x004 Ready indicator for PLL clock */
	uint32  CLKGEN_SEL_300M;            /* 0x008 Source selector for CPU */
	uint32  CLKGEN_SEL_400M;            /* 0x00C Source selector for other modules */
	uint32  CLKGEN_SEL_CAN0;            /* 0x010 Clock selector for CAN0 */
	uint32  CLKGEN_SEL_CAN1;            /* 0x014 Clock selector for CAN1 */
	uint32  CLKGEN_FSCM_BYPASS;         /* 0x018 Bypass fscm_o */
	uint32  CLKGEN_CRYSTAL_MODE;        /* 0x01C External crystal */
	uint32  Reserved1[56];
	uint32  CLKGEN_DIV_CPU;             /* 0x100 Clock divisor for CPU clock */
	uint32  CLKGEN_DIV_MEM;             /* 0x104 Clock divisor for Memory */
	uint32  CLKGEN_DIV_AHB;             /* 0x108 Clock divisor for AHB bus */
	uint32  CLKGEN_DIV_APB;             /* 0x10C Clock divisor for APB bus */
	uint32  CLKGEN_DIV_APB_REF;         /* 0x110 Clock divisor for APB REF */
	uint32  CLKGEN_DIV_CAN_0;           /* 0x114 Clock divisor for CAN 0 clock */
	uint32  CLKGEN_DIV_CAN_1;           /* 0x118 Clock divisor for CAN 1 clock */
	uint32  CLKGEN_DIV_RF_TEST;         /* 0x11C Clock divisor for RF test clock */
	uint32  CLKGEN_DIV_DIG_TEST;        /* 0x120 Clock divisor for digital test clock */
	uint32  Reserved2[55];
	uint32  CLKGEN_ENA_DMA;             /* 0x200 Clock enable for DMA */
	uint32  CLKGEN_ENA_ROM_CTRL;        /* 0x204 Clock enable for ROM */
	uint32  CLKGEN_ENA_RAM_CTRL;        /* 0x208 Clock enable for RAM */
	uint32  CLKGEN_ENA_BB_TOP;          /* 0x20C Clock enable for Baseband Top */
	uint32  CLKGEN_ENA_BB_CORE;         /* 0x210 Clock enable for Baseband Core */
	uint32  CLKGEN_ENA_FLASH_CTRL;      /* 0x214 Clock enable for XIP */
	uint32  CLKGEN_ENA_CRC;             /* 0x218 Clock enable for CRC */
	uint32  CLKGEN_ENA_DMU;             /* 0x21C Clock enable for DMU */
	uint32  CLKGEN_ENA_UART_0;          /* 0x220 Clock enable for UART 0 */
	uint32  CLKGEN_ENA_UART_1;          /* 0x224 Clock enable for UART 1 */
	uint32  CLKGEN_ENA_I2C;             /* 0x228 Clock enable for I2C */
	uint32  CLKGEN_ENA_SPI_M0;          /* 0x22C Clock enable for SPI_M0 */
	uint32  CLKGEN_ENA_SPI_M1;          /* 0x230 Clock enable for SPI_M1 */
	uint32  CLKGEN_ENA_SPI_S;           /* 0x234 Clock enable for SPI_S */
	uint32  CLKGEN_ENA_QSPI;            /* 0x238 Clock enable for QSPI */
	uint32  CLKGEN_ENA_GPIO;            /* 0x23C Clock enable for GPIO */
	uint32  CLKGEN_ENA_TIMER;           /* 0x240 Clock enable for TIMER */
	uint32  CLKGEN_ENA_PWM;             /* 0x244 Clock enable for PWM */
	uint32  CLKGEN_ENA_CAN_0;           /* 0x248 Clock enable for CAN_0 */
	uint32  CLKGEN_ENA_CAN_1;           /* 0x24C Clock enable for CAN_1 */
	uint32  CLKGEN_ENA_LVDS;            /* 0x250 Clock enable for LVDS */
	uint32  CLKGEN_ENA_DBGBUS;          /* 0x254 Clock enable for debug bus */
	uint32  CLKGEN_ENA_RF_TEST;         /* 0x258 Clock enable for RF test */
	uint32  CLKGEN_ENA_DIG_TEST;        /* 0x25C Clock enable for digital test */
	uint32  CLKGEN_ENA_FMCW;            /* 0x260 Clock enable for fmcw */
	uint32  Reserved3[39];
	uint32  CLKGEN_SW_REBOOT;           /* 0x300 software reboot */
	uint32  CLKGEN_RSTN_DMA;            /* 0x304 software reset for DMA */
	uint32  CLKGEN_RSTN_BB_TOP;         /* 0x308 software reset for Baseband Top */
	uint32  CLKGEN_RSTN_BB_CORE;        /* 0x30C software reset for Baseband Core */
	uint32  CLKGEN_RSTN_FLASH_CTRL;     /* 0x310 software reset for flash controller */
	uint32  CLKGEN_RSTN_CRC;            /* 0x314 software reset for CRC */
	uint32  CLKGEN_RSTN_DMU;            /* 0x318 software reset for for DMU */
	uint32  CLKGEN_RSTN_UART_0;         /* 0x31C software reset for UART 0 */
	uint32  CLKGEN_RSTN_UART_1;         /* 0x320 software reset for UART 1 */
	uint32  CLKGEN_RSTN_I2C;            /* 0x324 Soft reset for I2C */
	uint32  CLKGEN_RSTN_SPI_M0;         /* 0x328 software reset for SPI_M0 */
	uint32  CLKGEN_RSTN_SPI_M1;         /* 0x32C software reset for SPI_M1 */
	uint32  CLKGEN_RSTN_SPI_S;          /* 0x330 software reset for SPI_S */
	uint32  CLKGEN_RSTN_QSPI;           /* 0x334 software reset for QSPI */
	uint32  CLKGEN_RSTN_GPIO;           /* 0x338 software reset for GPIO */
	uint32  CLKGEN_RSTN_TIMER;          /* 0x33C software reset for TIMER */
	uint32  CLKGEN_RSTN_PWM;            /* 0x340 software reset for PWM */
	uint32  CLKGEN_RSTN_CAN_0;          /* 0x344 software reset for CAN 0 */
	uint32  CLKGEN_RSTN_CAN_1;          /* 0x348 software reset for CAN 1 */
	uint32  CLKGEN_RSTN_LVDS;           /* 0x34C software reset for LVDS */
	uint32  CLKGEN_RSTN_RF_TEST;        /* 0x350 software reset for RF test */
	uint32  CLKGEN_RSTN_DIG_TEST;       /* 0x354 software reset for digital test */
} Mcu_Reg_t;

#endif
