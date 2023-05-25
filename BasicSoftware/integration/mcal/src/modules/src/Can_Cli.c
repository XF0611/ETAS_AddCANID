/*
 * Can_Cli.c
 *
 *  Created on: 2022��8��31��
 *      Author: fei.xu1
 */
#include "Platform_Types.h"
#include "Std_Types.h"
#include "arc_builtin.h"
#include "Can.h"
#include "Can_Cli.h"

extern uint8  Car_Indoor_Calib[8];
static can_cli_item_t can_cli_cmd_list[CAN_CLI_CNT_MAX];

sint32 Get_Car_Indoor_Calib_Info(uint8 *data,uint32 len)
{

	for(uint16 i = 0; i < len;i++)
	{
#if 0
		Car_Indoor_Calib[i] = data[i];
#endif
	}
}

sint32 Can_Cli_Commands(void)
{
	#define CAR_INDOOR_CALIB   (0x66Fu)

	sint32 result = E_OK;

	result = Can_Cli_Register(CAR_INDOOR_CALIB,Get_Car_Indoor_Calib_Info);


	return result;

}


sint32 Can_Cli_Register(uint32 msg_id,can_cli_callback handler)
{
	sint32 result = E_OK;
	can_cli_item_t *item = NULL;

	for(uint32 idx = 0; idx < CAN_CLI_CNT_MAX;idx++)
	{
		if (raw_spin_trylock(&can_cli_cmd_list[idx].lock))
		{
			item = &can_cli_cmd_list[idx];
			break;
		}
	}


	if (item != NULL)
	{
		item->msg_id = msg_id;
		item->callback = handler;
	}else
	{
		result = E_NOT_OK;
	}

	return result;
}

void Can_Cli_Handle(uint32 msg_id,uint32 data_len,uint8 *data)
{
	can_cli_item_t *item;
	uint8 test[40];

	for (uint32 idx = 0;idx < CAN_CLI_CNT_MAX;idx++)
	{
		if (msg_id == can_cli_cmd_list[idx].msg_id)
		{

			item = &can_cli_cmd_list[idx];
			break;
		}
	}

	if (item != NULL)
	{
		item->callback(data,data_len);
	}else
	{
		//msg idδע��
		uart_printfs("msg is not registered.\r\n");
	}
}
