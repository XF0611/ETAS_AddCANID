#ifndef _ALPS_MMAP_H_
#define _ALPS_MMAP_H_

#define REL_REGBASE_ROM			(0x00000000U)
#define REL_REGBASE_ICCM		(0x00100000U)

#define REL_REGBASE_XIP_MMAP		(0x00300000U)

#define REL_BASE_SRAM			(0x00770000U)
#define REL_BASE_BB_SRAM		(0x00800000U)
#define REL_BASE_DCCM			(0x00A00000U)


#define REL_REGBASE_EMU			(0x00B00000U)
#define REL_REGBASE_TIMER0		(0x00B10000U)
#define REL_REGBASE_CLKGEN		(0x00B20000U)
#define REL_REGBASE_UART0		(0x00B30000U)
#define REL_REGBASE_UART1		(0x00B40000U)
#define REL_REGBASE_I2C0		(0x00B50000U)
#define REL_REGBASE_SPI0		(0x00B60000U)
#define REL_REGBASE_SPI1		(0x00B70000U)
#define REL_REGBASE_SPI2		(0x00B80000U)
#define REL_REGBASE_QSPI		(0x00B90000U)
#define REL_REGBASE_DMU			(0x00BA0000U)
#define REL_REGBASE_CAN0		(0x00BB0000U)
#define REL_REGBASE_CAN1		(0x00BC0000U)
#define REL_REGBASE_PWM			(0x00BD0000U)
#define REL_REGBASE_GPIO		(0x00BE0000U)
#define REL_REGBASE_EFUSE		(0x00BF0000U)

#define REL_REGBASE_BB			(0x00C00000U)
#define REL_REGBASE_CRC			(0x00C10000U)
#define REL_REGBASE_HDMA		(0x00C20000U)
#define REL_REGBASE_XIP			(0x00D00000U)
#define REL_REGBASE_LVDS		(0x00D10000U)

#define REL_NON_CACHED_BASE		(REL_BASE_BB_SRAM)

#define REL_XIP_MMAP_LEN		(0x00400000U)
#define REL_XIP_MMAP_MAX		(0x00700000U)

#endif
