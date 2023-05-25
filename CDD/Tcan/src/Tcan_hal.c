/*
 * tcan_hal.c
 *
 *  Created on: 2022年12月15日
 *      Author: fei.xu1
 */

#include "embARC_error.h"
#include "embARC_debug.h"
#include "Spi.h"
#include "tcan_hal.h"


uint16 tcan_spi_read(uint8 addr,uint16 read_len)
{
	uint8 test[100];
	sint32 result = E_OK;
	uint16 send_tcan_value;
	uint16 read_data;
	uint16 real_data = 0xFF;

	send_tcan_value = calc_tcan_sendval(addr,tcan_read_cmd);

	result = Spi_SetupEB(SpiConf_SpiChannel_SPI_Tcan,(const Spi_DataBufferType *)&send_tcan_value,(Spi_DataBufferType *)&read_data,read_len);
	/* spi read */

	if(result != E_OK)
	{
		/* error handle */
		sprintf(test,"tcan_spi_read Spi_SetupEB result is %d\r\n",result);
		Uart_Transmit(0,test,strlen(test));
	}else
	{
		result = Spi_SyncTransmit(SpiConf_SpiSequence_Tcan);
		if(result != E_OK)
		{
			/* error handle */
			sprintf(test,"tcan_spi_read Spi_SyncTransmit result is %d\r\n",result);
			Uart_Transmit(0,test,strlen(test));
		}else
		{
			// sprintf(test,"tcan read value is %x\r\n",read_data);
			// Uart_Transmit(0,test,strlen(test));
			real_data = (read_data >> 8) & 0xFF;
		}
	}



	return real_data;
}


sint32 tcan_spi_write(uint8 addr,uint16 writedata,uint16 write_len)
{
	sint32 result = E_OK;
	uint16 send_tcan_value;
	uint16 write_data;

	send_tcan_value = calc_tcan_sendval(addr,tcan_write_cmd);

	write_data = send_tcan_value | (writedata  << 8);

	result = Spi_SetupEB(SpiConf_SpiChannel_SPI_Tcan,(const Spi_DataBufferType *)&write_data,NULL,write_len);
	Spi_SyncTransmit(SpiConf_SpiSequence_Tcan);

	return result;
}
