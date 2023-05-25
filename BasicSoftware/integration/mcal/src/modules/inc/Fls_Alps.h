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


#ifndef INC_MCAL_FLS_ALPS_H_
#define INC_MCAL_FLS_ALPS_H_

/******************************************************************************
*                         Structure Definitions
******************************************************************************/

typedef struct {
    uint32 Erase_address;
    uint32 Numof_Sector;
    uint32 Numof_Sector_Backup;
    uint8  Erase_Flg;
} Fls_Erase_Type;

typedef struct {
    uint32 Write_address;
    uint32 Length;
    const uint8* Src_Addr_Ptr;
    uint8  Write_Flg;
} Fls_Write_Type;

typedef struct {
    uint32 Read_address;
    uint32 Length;
    uint8* Dest_Addr_Ptr;
    uint8  Read_Flg;
} Fls_Read_Type;

typedef struct {
    uint32 Compare_address;
    uint32 Length;
    const uint8* Src_Addr_Ptr;
    uint8  Compare_Flg;
} Fls_Compare_Type;

typedef struct {
    uint32 BlankCheck_address;
    uint32 Length;
    uint8  BlankCheck_Flg;
} Fls_BlankCheck_Type;

typedef struct {
	uint32 BoundAddr;
	uint32 SectorSize1;
	uint8  Erase_Ins1;
	uint32 SectorSize2;
	uint8  Erase_Ins2;
} Fls_Region_Type;

/******************************************************************************
*                         Public Functions
******************************************************************************/

/******************************************************************************
*  Service name    : Fls_Hal_Init
*  Syntax          : void Fls_Hal_Init (const Fls_ConfigType *ConfigPtr)
*  Reentrancy      : Non reentrant
*  Parameters (in) : ConfigPtr - Pointer to configuration set
*  Parameters (out): None
*  Return value    : None
*  Description     : Service for FLASH initialization
******************************************************************************/
void Fls_Hal_Init (const Fls_ConfigType *ConfigPtr);

/******************************************************************************
*  Service name    : Fls_Hal_Erase
*  Syntax          : Fls_Hal_Erase(Fls_AddressType TargetAddress,Fls_LengthType Length)
*  Reentrancy      : Non reentrant
*  Parameters (in) : ConfigPtr - Pointer to configuration set
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : Service for FLASH erase
******************************************************************************/
Std_ReturnType Fls_Hal_Erase(Fls_AddressType TargetAddress,Fls_LengthType Length);

/******************************************************************************
*  Service name    : Fls_Hal_Write
*  Syntax          : Std_ReturnType Fls_Hal_Write( Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length)
*  Reentrancy      : Non reentrant
*  Parameters (in) :
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : Service for FLASH erase
******************************************************************************/
Std_ReturnType Fls_Hal_Write( Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length);

/******************************************************************************
*  Service name    : Fls_Hal_GetStatus
*  Syntax          : MemIf_StatusType Fls_Hal_GetStatus(void)
*  Reentrancy      : Reentrant
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : MemIf_StatusType
*  Description     : Service for FLASH status get
******************************************************************************/
MemIf_StatusType Fls_Hal_GetStatus(void);

/******************************************************************************
*  Service name    : Fls_Hal_GetJobResult
*  Syntax          : MemIf_JobResultType Fls_Hal_GetJobResult(void)
*  Reentrancy      : Reentrant
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : MemIf_JobResultType
*  Description     : Service for FLASH job result get
******************************************************************************/
MemIf_JobResultType Fls_Hal_GetJobResult(void);

/******************************************************************************
*  Service name    : Fls_Hal_Read
*  Syntax          : Std_ReturnType Fls_Hal_Read(Fls_AddressType SourceAddress,uint8* TargetAddressPtr,Fls_LengthType Length)
*  Reentrancy      : Non reentrant
*  Parameters (in) : SourceAddress : Source address in flash memory. This address offset will be added to the flash memory base address
*  					 TargetAddressPtr: Pointer to target data buffer
*  					 Length: Number of bytes to read
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : Service for FLASH read
******************************************************************************/
Std_ReturnType Fls_Hal_Read(Fls_AddressType SourceAddress,uint8* TargetAddressPtr,Fls_LengthType Length);

/******************************************************************************
*  Service name    : Fls_Hal_Compare
*  Syntax          : Std_ReturnType Fls_Hal_Compare(Fls_AddressType SourceAddress,const uint8* TargetAddressPtr,Fls_LengthType Length)
*  Reentrancy      : Non reentrant
*  Parameters (in) : SourceAddress: Source address in flash memory. This address offset will be added to the flash memory base address
*  					 TargetAddressPtr: Pointer to target data buffer
*  					 Length: Number of bytes to be compared
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : Service for FLASH compare
******************************************************************************/
/* SWS_Fls_00241 */
Std_ReturnType Fls_Hal_Compare(Fls_AddressType SourceAddress,const uint8* TargetAddressPtr,Fls_LengthType Length);

/******************************************************************************
*  Service name    : Fls_Hal_SetMode
*  Syntax          : void Fls_Hal_SetMode(MemIf_ModeType Mode)
*  Reentrancy      : Non reentrant
*  Parameters (in) : Mode MEMIF_MODE_SLOW: Slow read access / normal SPI access.
					 MEMIF_MODE_FAST: Fast read access / SPI burst access
*  Parameters (out): None
*  Return value    : None
*  Description     : Service for FLASH mode
******************************************************************************/
void Fls_Hal_SetMode(MemIf_ModeType Mode);
/******************************************************************************
*  Service name    : Fls_Hal_BlankCheck
*  Syntax          : Std_ReturnType Fls_Hal_BlankCheck(Fls_AddressType TargetAddress,Fls_LengthType Length)
*  Reentrancy      : Non reentrant
*  Parameters (in) : TargetAddressPtr: Pointer to target data buffer
*  					 Length: Number of bytes to be checked whether erased
*  Parameters (out): None
*  Return value    : Std_ReturnType
*  Description     : Service for FLASH compare
******************************************************************************/
/* SWS_Fls_00373 */
Std_ReturnType Fls_Hal_BlankCheck(Fls_AddressType TargetAddress,Fls_LengthType Length);

/******************************************************************************
*  Service name    : Fls_Hal_MainFunction_Handling
*  Syntax          : void Fls_Hal_MainFunction_Handling(void)
*  Reentrancy      : Reentrant
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : Performs the processing of jobs.
******************************************************************************/
void Fls_Hal_MainFunction_Handling(void);

/******************************************************************************
*                         Private Functions
******************************************************************************/

/*******************************************************************************
*  Service name     : Fls_lclParamCheck
*  Syntax           : void Fls_lclParamCheck(boolean Expr,
*                                           uint8 ApiId,
*                                           uint8 ErrId,
*                                           Std_ReturnType *RetVal)
*  Parameters (in)  : Expr - TRUE or FALSE
*                     ApiId - API ID
*                     ErrId - ERROR ID
*  Parameter(in-out): RetVal
*  Return value     : None
*  Description      : Reports DET error if FLS_DEV_ERROR_DETECT == STD_ON.
*******************************************************************************/
void Fls_lclParamCheck(boolean Expr, uint8 ApiId, uint8 ErrId, Std_ReturnType *RetVal);

#endif /* INC_MCAL_FLS_ALPS_H_ */
