
 /************************************************************************************
*  Copyright (c) 2020, Calterah, Inc. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification,
*  are permitted provided that the following conditions are met:
*
*  1) Redistributions of source code must retain the above copyright notice, this
*     list of conditions and the following disclaimer.
*
*  2) Redistributions in binary form must reproduce the above copyright notice, this
*     list of conditions and the following disclaimer in the documentation and/or
*     other materials provided with the distribution.
*
*  3) Neither the name of the Calterah, Inc., nor the names of its contributors may
*     be used to endorse or promote products derived from this software without
*     specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
*  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
*  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
*  OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************************/


#ifndef MCAL_ALPS_I2C1_INC_MCAL_CDD_I2C_H_
#define MCAL_ALPS_I2C1_INC_MCAL_CDD_I2C_H_

#include "CDD_I2c_Cfg.h"
#include "Std_Types.h"

//=============ID definition for the APIs==============================//
#define I2c_INIT_SERVICE_ID                0u
#define I2c_DEINIT_SERVICE_ID              1u
#define I2c_TRANSMIT_SERVICE_ID       	   2u
#define I2c_RECEIVE_SERVICE_ID  		   3u

//=============error definition=======================================//
#define I2c_E_UNINIT                        0u
#define I2c_E_ALREADY_INITIALIZED   		1u
#define I2c_E_PARAM_POINTER 				2u

/* Enum for I2C status */
typedef enum
{
    I2C_STATUS_UNINIT = 0x0U,
	I2C_STATUS_IDLE   = 0x01U,
	I2C_STATUS_BUSY   = 0x02U
}I2c_StatusType;

/* Enum for speed the I2C operates */
typedef enum
{
	I2C_STANDARD = 0x00u ,
	I2C_FAST = 0x01u ,
	I2C_HIGHSPEED = 0X02U
}I2c_SpeedType;

/* Enum for address mode the I2C operates */
typedef enum
{
	I2C_7Bit_Addr = 0x00u ,
	I2C_10Bit_Addr = 0x01u
}I2c_AddrModeType;

/* Cofiguration Structure for I2C */
typedef struct
{
	I2c_SpeedType     I2C_Speed;
	I2c_AddrModeType  I2C_addrmode;
	uint32            I2C_SCL_highcnt;
	uint32  		  I2C_SCL_lowcnt;
	uint32 			  I2C_rxholdtime;
	uint32 			  I2C_txholdtime;
	uint32 			  I2C_suppressionlimit;
}I2c_ConfigType;


/* Cofiguration Structure for I2C address information*/
typedef struct
{
	I2c_AddrModeType 	 addr_mode;
	uint32 			 	 address;
	uint32 			     ext_addr;
	uint32 				 ext_addr_len;
}I2c_AddrType;

typedef enum
{
	Restart_DIS = 0x00u ,
	Restart_EN = 0x01u
}I2c_RestartType;


typedef enum
{
	Stop_DIS = 0x00u ,
	Stop_EN = 0x01u
}I2c_StopType;

typedef enum
{
	I2c_WRITE = 0x00u ,
	I2c_READ = 0x01u
}I2c_CmdType;

extern const I2c_ConfigType I2C_CfgData;

extern void CDD_I2c_Init(const I2c_ConfigType *CfgPtr);
extern Std_ReturnType CDD_I2c_DeInit(void);

extern Std_ReturnType CDD_I2c_SetSlaveAddress(I2c_AddrType* addr_info  );
extern Std_ReturnType CDD_I2c_Transmit(uint8 TxBuf, I2c_RestartType Restart ,I2c_StopType Stop_F, I2c_CmdType CMD, uint8* RxBuf);




#endif /* MCAL_ALPS_I2C1_INC_MCAL_CDD_I2C_H_ */
