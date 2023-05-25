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

/******************************************************************************
*                            Includes
******************************************************************************/
#include "CDD_I2c.h"
#include "I2c_Alps_RegDef.h"
#include "CDD_I2c_Alps.h"


#if ( I2C_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif

#include "Mcu.h"


#define I2C_MASTER_CODE		(0U)
#define I2C_DISABLE         (0x0U)
#define I2C_ENABLE          (0x1U)
#define I2C_RxTimeout       (0x0FFFF0U)

/******************************************************************************
*                      Global variable  Definitions
******************************************************************************/
#define I2C_START_SEC_VAR_INIT_UNSPECIFIED
/* Getting base address of the I2C module  */
static I2c_RegType *I2cHwReg =  ( I2c_RegType*)0xB50000U;
static I2c_StatusType I2cStatus = I2C_STATUS_UNINIT ;

#define I2C_STOP_SEC_VAR_INIT_UNSPECIFIED

/******************************************************************************
*                    Private Function Declaration
******************************************************************************/
#define I2C_START_SEC_CODE
/******************************************************************************
*                         Public Functions                                    *
******************************************************************************/

void I2c_Hal_Init(const I2c_ConfigType *CfgPtr)
{
	uint32 TempValue = 0u;

	I2cStatus = I2C_STATUS_UNINIT;

	I2cHwReg->IC_ENABLE = I2C_DISABLE;        			    /*disable the I2C */
	I2cHwReg->IC_INTR_MASK = 0x0u;     				/*mask the interrupts in default*/
	I2cHwReg->IC_HS_MADDR = I2C_MASTER_CODE ;       /*set master mode in default */

	/*set the threshold for the tx and rx FIFO to be default 0*/
    I2cHwReg->IC_RX_TL = 0x0U;
    I2cHwReg->IC_TX_TL = 0x0U;

    /* Set the I2C operating speed , corresponding count register , spike suppression limit */
    switch (CfgPtr->I2C_Speed)
    {
		case I2C_STANDARD:
		{
			TempValue = (0x01U << 1U);
			I2cHwReg->IC_SS_SCL_HCNT = CfgPtr->I2C_SCL_highcnt;
			I2cHwReg->IC_SS_SCL_LCNT = CfgPtr->I2C_SCL_lowcnt;
			I2cHwReg->IC_FS_SPKLEN   = CfgPtr->I2C_suppressionlimit;
			break;
		}
		case I2C_FAST  :
		{
			TempValue = (0x02U << 1U);
			I2cHwReg->IC_FS_SCL_HCNT = CfgPtr->I2C_SCL_highcnt;
			I2cHwReg->IC_FS_SCL_LCNT = CfgPtr->I2C_SCL_lowcnt;
			I2cHwReg->IC_FS_SPKLEN   = CfgPtr->I2C_suppressionlimit;
			break;
		}
		case I2C_HIGHSPEED:
		{
			TempValue = (0x03U << 1U);
			I2cHwReg->IC_HS_SCL_HCNT = CfgPtr->I2C_SCL_highcnt;
			I2cHwReg->IC_HS_SCL_LCNT = CfgPtr->I2C_SCL_lowcnt;
			I2cHwReg->IC_HS_SPKLEN   = CfgPtr->I2C_suppressionlimit;
			break;
		}
		default:
		{
			//compliant to misra-c
			break;
		}
    }/*end of switch*/

    /*set address mode to be 7 bit or 10 bit*/
    switch (CfgPtr->I2C_addrmode)
    {
		case I2C_7Bit_Addr:
		{
			TempValue = TempValue | (0U << 4U);
			break;
		}
		case I2C_10Bit_Addr  :
		{
			TempValue = TempValue | (0x01U << 4U);
			break;
		}
		default:
		{
			//compliant to misra-c
			break;
		}
    }/*end of switch*/
    TempValue  = TempValue | (0x01U<< 0U);    /*set as master mode in default*/
    TempValue  = TempValue | (0x01U<< 5U);    /*support restart in default */
    TempValue  = TempValue | (0x00U<< 8U);    /*support txFIFO empty interrupt in default*/
    I2cHwReg->IC_CON = TempValue;

    /*set the hold time*/
    TempValue = (CfgPtr->I2C_txholdtime & 0xFFFFU) | ((CfgPtr->I2C_rxholdtime & 0x0FFU)<<16U) ;
    I2cHwReg->IC_SDA_HOLD = TempValue;

    /*no interrupt related setting , need to check later*/
    // set interrupt handler here  	actually , it should be handled in OS in Autosar
    // enable the interrupt here

    I2cHwReg->IC_ENABLE = I2C_ENABLE;        			    /*enable the I2C */

    I2cStatus = I2C_STATUS_IDLE;

} /*end of I2c_Hal_Init */


void I2c_Hal_DeInit(void)
{
	/*disable the I2C and set the register to default value*/
	I2cHwReg->IC_ENABLE    	 = I2C_DISABLE;        			    /*disable the I2C */
	I2cHwReg->IC_CON 	   	 = 0x3FU;
	I2cHwReg->IC_HS_MADDR  	 = 0x01U;
	I2cHwReg->IC_INTR_MASK 	 = 0x0U;
	I2cHwReg->IC_SS_SCL_HCNT = 0x0u;
	I2cHwReg->IC_SS_SCL_LCNT = 0x0u;
	I2cHwReg->IC_FS_SPKLEN   = 0x0u;
    I2cHwReg->IC_RX_TL 	     = 0x0U;
    I2cHwReg->IC_TX_TL 	     = 0x0U;
    I2cHwReg->IC_STATUS      = 0x02U;
    I2cHwReg->IC_SDA_HOLD 	 = 0x01U;

    I2cStatus = I2C_STATUS_UNINIT;
} /*end of I2c_Hal_DeInit */



void I2c_Hal_SetSlaveAddress(I2c_AddrType* addr_info)
{
		uint32 Idx;
		uint32 TempValue;

	    I2cStatus = I2C_STATUS_BUSY;

	    /*send extra 0xF0 if the address mode is 10 bit mode*/
	    I2cHwReg->IC_ENABLE = I2C_ENABLE;
	    if(addr_info->addr_mode ==  I2C_10Bit_Addr)
	    {
	    	I2cHwReg->IC_DATA_CMD = 0xF0U;
	    }
	    else
	    {}

	    /*disable to set the slave address */
	    I2cHwReg->IC_ENABLE = I2C_DISABLE;

	    TempValue = I2cHwReg->IC_CON;
	    if(addr_info->addr_mode ==  I2C_10Bit_Addr)
	    {
	    	I2cHwReg->IC_TAR = (addr_info->address & 0x3FFU) | (0x01U << 12U) ;
	    	I2cHwReg->IC_CON = TempValue | (0x01U << 4U);

	    }
	    else if(addr_info->addr_mode ==  I2C_7Bit_Addr)
	    {
	    	I2cHwReg->IC_TAR = (addr_info->address & 0x3FFU) | (0x0U << 12U) ;
	    	I2cHwReg->IC_CON = TempValue & 0xFFFFFFEF;
	    }
	    else
	    {
	    	/*compliant to misra-c*/
	    }

	    /*enable again to send out the extra address*/
	    I2cHwReg->IC_ENABLE = I2C_ENABLE;

	    /*if addr_info->ext_addr_len=0, not for loop*/
		for(Idx = 0U; Idx < addr_info->ext_addr_len ; Idx++)
		{
			TempValue = (addr_info->ext_addr >> ((addr_info->ext_addr_len - Idx - 1U) << 3U)) & 0xFFU;
			I2cHwReg->IC_DATA_CMD = TempValue ;
		}

		I2cStatus = I2C_STATUS_IDLE;
}

Std_ReturnType I2c_Hal_Transmit( uint8 TxBuf, I2c_RestartType Restart ,I2c_StopType Stop_F, I2c_CmdType CMD, uint8* RxBuf)
{
	 Std_ReturnType ReturnValue = E_OK;
	uint32 RxTimecnt = 0x0U;

	I2cStatus = I2C_STATUS_BUSY;

	while( 0U == (I2cHwReg->IC_STATUS & 0x02U)) {}

	I2cHwReg->IC_DATA_CMD =(uint32)( (Restart << 10U) |  (Stop_F << 9) | (CMD<<8) | TxBuf);

	if(I2c_READ == CMD)
	{
			while( 0U == (I2cHwReg->IC_STATUS & 0x08U))
	    	{
	    		RxTimecnt = RxTimecnt + 1U;
	    		if(RxTimecnt > I2C_RxTimeout )
	    		{
	    			ReturnValue = E_NOT_OK;
	    			break;
	    		}
	    		else
	    		{}
	    	}
	    	/*Get data from the register*/
	    	*RxBuf = (uint8)(I2cHwReg->IC_DATA_CMD & 0x0FFU);
	}

	I2cStatus = I2C_STATUS_IDLE;

	return ReturnValue;
}


/************************************************************************************
*     I2c_Hal_GetStatus                                                                *
************************************************************************************/
I2c_StatusType I2c_Hal_GetStatus(void)
{
    return(I2cStatus);

} /* end of I2c_Hal_GetStatus*/


#define I2C_STOP_SEC_CODE
