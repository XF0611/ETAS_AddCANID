/*
 * CDD_Uart_Printf.c
 *
 *  Created on: 2022Äê6ÔÂ21ÈÕ
 *      Author: fei.xu1
 */

#include "stddef.h"
#include "stdint.h"
#include "CDD_Uart.h"

//uint8_t teststr[100];

void uart_printfs(uint8_t *string,uint8_t *data)
{
	uint8_t teststr[100];

	if (data == NULL)
	{
		sprintf(teststr,string);
		Uart_Transmit(0,teststr,strlen(teststr));
	}
	else
	{
		uint64_t test_data = *data;
		sprintf(teststr,"%s %d.\r\n",string,test_data);
		Uart_Transmit(0,teststr,strlen(teststr));
	}

}


void uart_printff(uint8_t *string,float *data)
{
	uint8_t teststr[100];

	if (data == NULL)
	{
		sprintf(teststr,string);
		Uart_Transmit(0,teststr,strlen(teststr));
	}
	else
	{
		sprintf(teststr,"%s %f.\r\n",string,*data);
		Uart_Transmit(0,teststr,strlen(teststr));
	}

}


