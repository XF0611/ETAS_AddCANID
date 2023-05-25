#ifndef TCAN_HAL_H
#define TCAN_HAL_H

#include "Std_Types.h"

typedef enum {
	tcan_read_cmd,
	tcan_write_cmd
}tcan_cmd;

#define calc_tcan_sendval(addr,cmd) ((uint16)addr << 1) | ((uint16)cmd)

uint16 tcan_spi_read(uint8 addr,uint16 read_len);
sint32 tcan_spi_write(uint8 addr,uint16 writedata,uint16 write_len);
#endif