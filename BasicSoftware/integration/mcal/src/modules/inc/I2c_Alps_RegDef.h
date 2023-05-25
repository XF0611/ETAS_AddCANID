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

#ifndef MCAL_ALPS_I2C1_INC_MCAL_I2C_ALPS_REGDEF_H_
#define MCAL_ALPS_I2C1_INC_MCAL_I2C_ALPS_REGDEF_H_

typedef volatile struct
{
	uint32 IC_CON; 			/* 0x0 I2C Control Register*/
	uint32 IC_TAR;          /* 0x4 I2C Target Address Register*/
	uint32 rsvd1;		    /* Reserved Space (0x8) */
	uint32 IC_HS_MADDR;     /* 0xc I2C High Speed Master Mode Code Address Register*/
	uint32 IC_DATA_CMD;     /* 0x10 I2C Rx/Tx Data Buffer and Command Register */
	uint32 IC_SS_SCL_HCNT;  /* 0x14 Standard Speed I2C Clock SCL High Count Register*/
	uint32 IC_SS_SCL_LCNT;  /* 0x18 Standard Speed I2C Clock SCL Low Count Register*/
	uint32 IC_FS_SCL_HCNT;  /* 0x1c Standard Speed I2C Clock SCL High Count Register*/
	uint32 IC_FS_SCL_LCNT;  /* 0x20 Fast Mode or Fast Mode Plus I2C Clock SCL Low Count Register*/
	uint32 IC_HS_SCL_HCNT;  /* 0x24 Fast Mode or Fast Mode Plus I2C Clock SCL High Count Register*/
	uint32 IC_HS_SCL_LCNT;  /* 0x28 High Speed I2C Clock SCL Low Count Register*/
	uint32 IC_INTR_STAT;    /* 0x2c I2C Interrupt Status Register*/
	uint32 IC_INTR_MASK;    /* 0x30 I2C Interrupt Mask Register*/
	uint32 IC_RAW_INTR_STAT;/* 0x34 I2C Raw Interrupt Status Register*/
	uint32 IC_RX_TL;        /* 0x38 I2C Receive FIFO Threshold Register*/
	uint32 IC_TX_TL;        /* 0x3c I2C Transmit FIFO Threshold Register*/
	uint32 IC_CLR_INTR;     /* 0x40 Clear Individual Interrupt Register*/
	uint32 IC_CLR_RX_UNDER; /* 0x44 Clear RX_UNDER Interrupt Register*/
	uint32 IC_CLR_RX_OVER;  /* 0x48 Clear RX_OVER Interrupt Register*/
	uint32 IC_CLR_TX_OVER;  /* 0x4c Clear TX_OVER Interrupt Register*/
	uint32 rsvd2;		    /* Reserved Space (0x50) */
	uint32 IC_CLR_TX_ABRT;     /*0x54 Clear TX_ABRT Interrupt Register*/
	uint32 rsvd3;		       /* Reserved Space (0x58) */
	uint32 IC_CLR_ACTIVITY;    /* 0x5c Clear ACTIVITY Interrupt Register*/
	uint32 IC_CLR_STOP_DET;    /* 0x60 Clear STOP_DET Interrupt Register*/
	uint32 IC_CLR_START_DET;   /* 0x64 Clear START_DET Interrupt Register*/
	uint32 rsvd4;		       /* Reserved Space (0x68) */
	uint32 IC_ENABLE;          /* 0x6c I2C Enable Register*/
	uint32 IC_STATUS;  		   /* 0x70 I2C STATUS Register*/
	uint32 IC_TXFLR;  		   /* 0x74 I2C Transmit FIFO Level Register*/
	uint32 IC_RXFLR;  		   /* 0x78 I2C Receive FIFO Level Register*/
	uint32 IC_SDA_HOLD;  	   /* 0x7c I2C SDA Hold Time Length Register*/
	uint32 IC_TX_ABRT_SOURCE;  /* 0x80 I2C Transmit Abort Source Register*/
	uint32 rsvd5;		       /* Reserved Space (0x84) */
	uint32 IC_CLR_GEN_CALL;    /* 0x88 Clear GEN_CALL Interrupt Register*/
	uint32 rsvd6[4u];		   /* Reserved Space (0x8C - 0x98) */
	uint32 IC_ENABLE_STATUS;   /* 0x9c I2C Enable Status Register*/
	uint32 IC_FS_SPKLEN;  	   /* 0xa0 I2C SS FS or FM+ spike suppression limit*/
	uint32 IC_HS_SPKLEN;  	   /* 0xa4 I2C HS spike suppression limit register*/
	uint32 rsvd7[19u];		   /* Reserved Space (0xa8 - 0xf0) */
	uint32 IC_COMP_PARAM_1;    /* 0xf4 Component Parameter Register 1*/
	uint32 IC_COMP_VERSION;    /* 0xf8 I2C Component Version Register*/
	uint32 IC_COMP_TYPE;  	   /* 0xfc I2C Component Type Register*/

}I2c_RegType;



#endif /* MCAL_ALPS_I2C1_INC_MCAL_I2C_ALPS_REGDEF_H_ */
