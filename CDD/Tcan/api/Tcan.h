/*
 * tcan.h
 *
 *  Created on: 2022年12月16日
 *      Author: fei.xu1
 */

#ifndef TCAN_H
#define TCAN_H

#define device_id_num 8
#define oper_len 16

typedef enum {
	standard,
	extend
}can_id_type;

typedef enum {
	Sleep_Mode_Cmd = 	1,
	Standby_Mode_Cmd =  4,
	Listen_Mode_Cmd =   5,
	Normal_Mode_Cmd =   7,
}tcan_mode;

typedef enum {
	Device_Id =			0x0,
	Rev_Id_Major =  	0x8,
	Rev_Id_Minor =  	0x9,
	Spi_Rsvd = 			0xA,
	Scratch_Pad_Spi = 	0xF,
	Mode_Cntrl = 		0x10,
	Wake_Pin_Config = 	0x11,
	Pin_Config = 		0x12,
	Wd_Config1 = 		0x13,
	Wd_Config2 = 		0x14,
	Wd_Input_Trig = 	0x15,
	Wd_Rst_Pluse = 		0x16,
	Fsm_Config = 		0x17,
	Fsm_Cntr = 			0x18,
	Device_Rst = 		0x19,
	Device_Config1 = 	0x1A,
	Device_Config2 = 	0x1B,
	Swe_Dis = 			0x1C,
	Sdo_Config = 		0x29,
	Wd_QA_Config = 		0x2D,
	Wd_QA_Answer = 		0x2E,
	Wd_QA_Question =    0x2F,
	Sw_Id1 = 			0x30,
	Sw_Id2 = 			0x31,
	Sw_Id3 = 			0x32,
	Sw_Id4 = 			0x33,
	Sw_Id_Mask1 = 		0x34,
	Sw_Id_Mask2 = 		0x35,
	Sw_Id_Mask3 = 		0x36,
	Sw_Id_Mask4 = 		0x37,
	Sw_Id_Mask_Dlc = 	0x38,
	Data_Y = 			0x39,
	Sw_Rsvd_Y = 		0x41,
	Sw_Config1 = 		0x44,
	Sw_Config2 = 		0x45,
	Sw_Config3 = 		0x46,
	Sw_Config4 = 		0x47,
	Sw_Config_Rsvd_Y = 	0x48,
	Int_Global = 		0x50,
	Int_1 = 			0x51,
	Int_2 = 			0x52,
	Int_3 = 			0x53,
	Int_CanBus = 		0x54,
	Int_Global_Enable = 0x55,
	Int_Enable_1 = 		0x56,
	Int_Enable_2 = 		0x57,
	Int_Enable_3 = 		0x58,
	Int_Enable_CanBus = 0x59,
	Int_Rsvd_Y = 		0x5A

}tcan_register;

void read_tcan_id(void);
void config_mode(tcan_mode mode);
void config_selective_wake(uint32 wake_id,boolean extend_id);

#endif /* WEIFU_TCAN_INC_TCAN_H_ */
