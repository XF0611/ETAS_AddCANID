/*
 * tcan.c
 *
 *  Created on: 2022年12月16日
 *      Author: fei.xu1
 */

#include "embARC_error.h"
#include "embARC_debug.h"
#include "tcan_hal.h"
#include "tcan.h"

void read_tcan_id(void)
{
	uint8 test[100];
	uint16 device_value;

	for(uint8 j = 0; j < device_id_num;j++)
	{
		device_value = tcan_spi_read(Device_Id + j,oper_len);
		sprintf(test,"addr:%d value is %x\r\n",Device_Id + j,device_value);
		Uart_Transmit(0,test,strlen(test));
	}

}

void config_mode(tcan_mode mode)
{
	uint16 read_data;
	uint16 write_data;
	uint8 test[100];
	read_data = tcan_spi_read(Mode_Cntrl,oper_len);
	read_data &= ~(7 << 0);
	read_data |= mode;

	write_data = read_data;

	
	sprintf(test,"mode final value is %x\r\n",write_data);
	Uart_Transmit(0,test,strlen(test));

	tcan_spi_write(Mode_Cntrl,write_data,oper_len);
}

void config_selective_wake(uint32 wake_id,boolean extend_id)
{
	uint16 read_before;
	uint16 read_back;
	uint16 write_value;
	uint8 test[100];

	/* sw config4 */
	write_value = 0x00;
	tcan_spi_write(Sw_Config4,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Config4,oper_len);
	sprintf(test,"read Sw_Config4 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));

	/* sw id1 */
	if(extend_id == true)
	{
		write_value = (wake_id >> 10) & 0xff;
	}else
	{
		write_value = 0x00;
	}
	tcan_spi_write(Sw_Id1,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Id1,oper_len);
	sprintf(test,"read Sw_Id1 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));

	/* sw id2 */
	if(extend_id == true)
	{
		write_value = (wake_id >> 2) & 0xff;
	}else
	{
		write_value = 0x00;
	}
	tcan_spi_write(Sw_Id2,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Id2,oper_len);
	sprintf(test,"read Sw_Id2 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw id3 */
	if(extend_id == true)
	{
		write_value = ((wake_id >> 24) & 0x1F) | extend << 5 | (wake_id & 0x3);
	}else{
		write_value = ((wake_id >> 6) & 0x1F) | standard << 5;
	}
	tcan_spi_write(Sw_Id3,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Id3,oper_len);
	sprintf(test,"read Sw_Id3 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw id4 */
	if(extend_id == true)
	{
		write_value = ((wake_id >> 18) & 0x3F) << 2;
	}else
	{
		write_value = (wake_id & 0x3F) << 2;
	}
	tcan_spi_write(Sw_Id4,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Id4,oper_len);
	sprintf(test,"read Sw_Id4 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw id mask1 */
	write_value = 0x00;
	tcan_spi_write(Sw_Id_Mask1,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Id_Mask1,oper_len);
	sprintf(test,"read Sw_Id_Mask1 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw id mask2 */
	write_value = 0x00;
	tcan_spi_write(Sw_Id_Mask2,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Id_Mask2,oper_len);
	sprintf(test,"read Sw_Id_Mask2 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw id mask3 */
	write_value = 0x00;
	tcan_spi_write(Sw_Id_Mask3,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Id_Mask3,oper_len);
	sprintf(test,"read Sw_Id_Mask3 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw id mask4 */
	write_value = 0x00;
	tcan_spi_write(Sw_Id_Mask4,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Id_Mask4,oper_len);
	sprintf(test,"read Sw_Id_Mask4 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw id maskdlc */
	write_value = 0x00;
	tcan_spi_write(Sw_Id_Mask_Dlc,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Id_Mask_Dlc,oper_len);
	sprintf(test,"read Sw_Id_Mask_Dlc is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw config1 */
	write_value = 0xD0;
	tcan_spi_write(Sw_Config1,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Config1,oper_len);
	sprintf(test,"read Sw_Config1 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw config3 */
	write_value = 0xFE;
	tcan_spi_write(Sw_Config3,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Config3,oper_len);
	sprintf(test,"read Sw_Config3 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* sw config4 */
	write_value = 0x80;
	tcan_spi_write(Sw_Config4,write_value,oper_len);
	read_back = tcan_spi_read(Sw_Config4,oper_len);
	sprintf(test,"read Sw_Config4 is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));

	/* set swe disable */
	write_value = 0x84;
	tcan_spi_write(Swe_Dis,write_value,oper_len);
	read_back = tcan_spi_read(Swe_Dis,oper_len);
	sprintf(test,"read Swe_Dis is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));


	/* mode cntl default mode is listen mode*/
	write_value = 0x84;
	tcan_spi_write(Mode_Cntrl,write_value,oper_len);
	read_back = tcan_spi_read(Mode_Cntrl,oper_len);
	sprintf(test,"read Mode_Cntrl is %x\r\n",read_back);
	Uart_Transmit(0,test,strlen(test));
}


