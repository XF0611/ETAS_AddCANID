#ifndef SYSTEM_H
#define SYSTEM_H

#include "common.h"

#define LITTLE_ENDIAN			(0)

#define WARM_RESET			(0xF)
#define FLASH_BOOT_MODE			(0x0)
#define S_UART_BOOT_MODE		(0x4)
#define S_SPI_BOOT_MODE			(0x5)
#define S_CAN_BOOT_MODE			(0x6)


#define DEVICE_NAME_MLEN		(8)
struct sdevice_ops {
	char dname[DEVICE_NAME_MLEN];

	void (*init)(int port, void *param);
	void (*reset)(int port);

	int (*speed)(int port, unsigned int speed);
	int (*receive_char)(int port, uint8_t *byte);
	int (*write_char)(int port, const uint8_t *byte);
	void (*d_cleanup)(int port);
	int (*read)(int port, unsigned char *buf, unsigned int cnt);
	int (*write)(int port, const unsigned char *buf, unsigned int cnt);
};

/* Trace: maybe for simulation. */
#define	TRACE_FLOW_M_			0x000000FF
#define TRACE_FLOW_HOST_UART		0x0000FF00
#define TRACE_FLOW_HOST_SPI		0x00FF0000
#define	TRACE_FLOW_HOST_CAN		0xFF000000


/* cannot intent host... maybe enter reset flow or hardware abort. */
#define ERR_SYS_DEVICE_ERROR		(-1)
#define ERR_SYS_HSHAKE_TIMEOUT		(-2)


/*
 * ms: range 1~10,000.
 * */
int arc_timer(uint16_t ms);
uint8_t arc_timer_timeout(void);
void rtc_count_enable(int en);
void mdelay(uint32_t ms);
void udelay(uint32_t us);


/*
 * TODO:
 * data section size, and stack size.
 * */
#define DATA_SECTION_SIZE		(0x2000)
#define STACK_SIZE			(0x1000)
#define RESRVED_REGION_SIZE		(DATA_SECTION_SIZE + STACK_SIZE) /* TODO: if firmware local on its data section and stack on RAM space. */

//#define XTAL_CLOCK_FREQ (50000000UL)

#ifdef XTAL_CLOCK_FREQ
#define SYSTEM_REF_CLOCK		(XTAL_CLOCK_FREQ)
#define UART_SOURCE_CLOCK		(50000000UL)
#define QSPI_SOURCE_CLOCK		(50000000UL)
#define SSI_SOURCE_CLOCK		(50000000UL)
#define CAN_SOURCE_CLOCK		(50000000UL)
#endif


void hardware_abort(int assert);



#endif /* __SYSTEM_H__ */

