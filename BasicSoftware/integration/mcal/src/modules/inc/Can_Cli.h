/*
 * Can_Cli.h
 *
 *  Created on: 2022Äê8ÔÂ31ÈÕ
 *      Author: fei.xu1
 */

#ifndef INC_CAN_CLI_H_
#define INC_CAN_CLI_H_
#define CAN_CLI_CNT_MAX 64

typedef sint32 (*can_cli_callback)(uint8 *data,uint32 len);

typedef struct {
	uint32 lock;
	uint32 msg_id;
	can_cli_callback callback;

}can_cli_item_t;

extern sint32 Can_Cli_Commands(void);

#endif /* INC_CAN_CLI_H_ */
