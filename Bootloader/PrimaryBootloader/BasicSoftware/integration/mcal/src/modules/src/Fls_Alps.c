/************************************************************************************
 *  Copyright (c) 2020, Calterah, Inc. All rights reserved.
 *
 *  Flash driver is External Flash driver, will use QSPI driver of Alps Inc.
 *  QSPI drvier is property of Calterah Semiconductor, Inc.
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
#include "Fls.h"
#include "Spi.h"
#include "Fls_Alps.h"

#if(FLS_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#define FLS_START_SEC_VAR_NO_INIT_UNSPECIFIED


#define FLS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED


#define FLS_START_SEC_VAR_INIT_UNSPECIFIED

/* Macro */
#define Flg_High   1u
#define Flg_Low    0u

/* Local variable declarations */
static MemIf_StatusType FlsModuleState = MEMIF_UNINIT;
static MemIf_JobResultType Fls_JobResult = MEMIF_JOB_OK;
static const Fls_ConfigType* Fls_ConfigPtr;
static Fls_Erase_Type Fls_Erase_Job;
static Fls_Write_Type Fls_Write_Job;
static Fls_Read_Type  Fls_Read_Job;
static Fls_Compare_Type Fls_Compare_Job;
static Fls_BlankCheck_Type Fls_BlankCheck_Job;
static Fls_Region_Type Fls_Region;
static uint32 Fls_SectorSize;
static uint8 Fls_EraseIns;
static uint32 Fls_MaxRead;
static uint32 Fls_MaxWrite;

#define FLS_STOP_SEC_VAR_INIT_UNSPECIFIED


#define FLS_START_SEC_CODE

/* Private Local Function Declarations */
static Std_ReturnType Fls_ID_Check(uint8* ID);
static void Fls_Hal_MainFunction_Handling_Erase(void);
static void Fls_Hal_MainFunction_Handling_Write(void);
static void Fls_Hal_MainFunction_Handling_Read(void);
static void Fls_Hal_MainFunction_Handling_Compare(void);
static void Fls_Hal_MainFunction_Handling_BlankCheck(void);
static Std_ReturnType Fls_GetSPIStatusRegister(uint8* RdStatus);
static void Fls_EnableWrite(void);
static Std_ReturnType Fls_ErasedCheck(Fls_AddressType SourceAddress , Fls_LengthType Length );
static uint32 Fls_Sum(const uint8* data,uint16 length);
static void Fls_CallBackFun(void);

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
void Fls_Hal_Init (const Fls_ConfigType *ConfigPtr)
{
    Std_ReturnType RetVal = E_OK;
    uint8 FlashIdBuf[30u] = {0u};
    uint8 FlashID[2u] = {0u};

    /*SWS_Fls_00086 ,SWS_Fls_00191*/
    Fls_ConfigPtr = ConfigPtr ;

    Fls_MaxRead = ConfigPtr->FlsMaxReadNormalMode;
    Fls_MaxWrite = Fls_ConfigPtr->FlsMaxWriteNormalMode;

    /*SWS_Fls_00268*/
#if (FLS_DEV_ERROR_DETECT == STD_ON)
    if(FlsModuleState == MEMIF_BUSY)
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_INIT_SID, FLS_E_BUSY);
    }
    else
    {
#endif

        /*use SPI to communicate to external Flash device*/
        Spi_Init( &SpiConfigData );

        /*Get Flash ID by QSPI command*/
        RetVal = Spi_WriteIB( QSPI_CHANNEL_READID, NULL_PTR);
        RetVal |= Spi_SetupEB( QSPI_CHANNEL_READIDBUF, NULL_PTR, FlashIdBuf, 26 );
        RetVal |= Spi_SyncTransmit( FLS_READIDBUF );
        RetVal |= Spi_ReadIB( QSPI_CHANNEL_READID, FlashID);

        if(((E_OK == RetVal)) && ( E_OK == Fls_ID_Check(FlashID)))
        {
            /* SWS_Fls_00323 */
            FlsModuleState = MEMIF_IDLE;
            /*SWS_Fls_00324*/
            Fls_JobResult = MEMIF_JOB_OK;
        }
        else
        {
#if (FLS_DEV_ERROR_DETECT == STD_ON)
            /* calls Det_ReportError from Det module */
            (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_INIT_SID, FLS_E_UNEXPECTED_FLASH_ID);
#endif

            FlsModuleState = MEMIF_UNINIT ;
        }/*SWS_Fls_00144*/
#if (FLS_DEV_ERROR_DETECT == STD_ON)
    }
#endif
}

/******************************************************************************
 *  Service name    : Fls_Hal_Erase
 *  Syntax          : Std_ReturnType Fls_Hal_Erase(Fls_AddressType TargetAddress,Fls_LengthType Length)
 *  Reentrancy      : Non reentrant
 *  Parameters (in) : ConfigPtr - Pointer to configuration set
 *  Parameters (out): None
 *  Return value    : Std_ReturnType
 *  Description     : Service for FLASH erase
 ******************************************************************************/
Std_ReturnType Fls_Hal_Erase(Fls_AddressType TargetAddress,Fls_LengthType Length)
{
    Std_ReturnType RetVal = E_OK;
    uint32 Fls_Real_Addr = 0u;

    Fls_Real_Addr = FlsBaseAddress + TargetAddress;

    /*judge which region the address locate in*/
    if(Fls_Real_Addr < Fls_Region.BoundAddr)
    {
        Fls_SectorSize = Fls_Region.SectorSize1;
        Fls_EraseIns = Fls_Region.Erase_Ins1;
    }
    else
    {
        Fls_SectorSize = Fls_Region.SectorSize2;
        Fls_EraseIns = Fls_Region.Erase_Ins2;
    }

#if (FLS_DEV_ERROR_DETECT == STD_ON)
    /*SWS_Fls_00065*/
    if(FlsModuleState == MEMIF_UNINIT )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_UNINIT);
        RetVal = E_NOT_OK;
    }
    /*SWS_Fls_00023*/
    else if((FlsModuleState == MEMIF_BUSY) || (FlsModuleState == MEMIF_BUSY_INTERNAL) )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_BUSY);
        RetVal = E_NOT_OK;
    }
    else
    {
        /*SWS_Fls_00020*/
        if(( (Fls_Real_Addr % Fls_SectorSize) !=  0u  ) || ((Fls_Real_Addr >= FLS_ENDADDRESS)) )
        {
            (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_PARAM_ADDRESS);
            RetVal = E_NOT_OK;
        }
        else
        {
            /*SWS_Fls_00021*/
            if((((Fls_Real_Addr + Length ) % Fls_SectorSize) != 0u) || ((Fls_Real_Addr + Length) > FLS_ENDADDRESS ))
            {
                (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_PARAM_LENGTH);
                RetVal = E_NOT_OK;
            }
            else
            {
#endif
                /*SWS_Fls_00327 ,SWS_Fls_00221 */
                Fls_Erase_Job.Erase_Flg = Flg_High;
                Fls_Erase_Job.Erase_address = Fls_Real_Addr;
                Fls_Erase_Job.Numof_Sector   = Length / Fls_SectorSize;              //Length will be rounded up to the next full sector boundary
                Fls_Erase_Job.Numof_Sector_Backup   = Fls_Erase_Job.Numof_Sector;

                /*SWS_Fls_00328*/
                FlsModuleState = MEMIF_BUSY;
                /*SWS_Fls_00329*/
                Fls_JobResult = MEMIF_JOB_PENDING;
                /*SWS_Fls_00330*/
                RetVal = E_OK ;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
            }
        }
    }
#endif

    return RetVal;
}

Std_ReturnType Fls_Hal_Erase_FBL(Fls_AddressType TargetAddress,Fls_LengthType Length)
{
    Std_ReturnType RetVal = E_OK;
    uint32 Fls_Real_Addr = 0u;

    Fls_Real_Addr = TargetAddress;

    /*judge which region the address locate in*/
    if(Fls_Real_Addr < Fls_Region.BoundAddr)
    {
        Fls_SectorSize = Fls_Region.SectorSize1;
        Fls_EraseIns = Fls_Region.Erase_Ins1;
    }
    else
    {
        Fls_SectorSize = Fls_Region.SectorSize2;
        Fls_EraseIns = Fls_Region.Erase_Ins2;
    }

#if (FLS_DEV_ERROR_DETECT == STD_ON)
    /*SWS_Fls_00065*/
    if(FlsModuleState == MEMIF_UNINIT )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_UNINIT);
        RetVal = E_NOT_OK;
    }
    /*SWS_Fls_00023*/
    else if((FlsModuleState == MEMIF_BUSY) || (FlsModuleState == MEMIF_BUSY_INTERNAL) )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_BUSY);
        RetVal = E_NOT_OK;
    }
    else
    {
        /*SWS_Fls_00020*/
        if(( (Fls_Real_Addr % Fls_SectorSize) !=  0u  ) || ((Fls_Real_Addr >= FLS_ENDADDRESS)) )
        {
            (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_PARAM_ADDRESS);
            RetVal = E_NOT_OK;
        }
        else
        {
            /*SWS_Fls_00021*/
            if((((Fls_Real_Addr + Length ) % Fls_SectorSize) != 0u) || ((Fls_Real_Addr + Length) > FLS_ENDADDRESS ))
            {
                (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_PARAM_LENGTH);
                RetVal = E_NOT_OK;
            }
            else
            {
#endif
                /*SWS_Fls_00327 ,SWS_Fls_00221 */
                Fls_Erase_Job.Erase_Flg = Flg_High;
                Fls_Erase_Job.Erase_address = Fls_Real_Addr;
                Fls_Erase_Job.Numof_Sector   = Length / Fls_SectorSize;              //Length will be rounded up to the next full sector boundary
                Fls_Erase_Job.Numof_Sector_Backup   = Fls_Erase_Job.Numof_Sector;

                /*SWS_Fls_00328*/
                FlsModuleState = MEMIF_BUSY;
                /*SWS_Fls_00329*/
                Fls_JobResult = MEMIF_JOB_PENDING;
                /*SWS_Fls_00330*/
                RetVal = E_OK ;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
            }
        }
    }
#endif

    return RetVal;
}

/******************************************************************************
 *  Service name    : Fls_Hal_Write
 *  Syntax          : Std_ReturnType Fls_Hal_Write( Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length)
 *  Reentrancy      : Non reentrant
 *  Parameters (in) :
 *  Parameters (out): None
 *  Return value    : Std_ReturnType
 *  Description     : Service for FLASH erase
 ******************************************************************************/
Std_ReturnType Fls_Hal_Write( Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length)
{
    Std_ReturnType RetVal = E_OK;
    uint32 Fls_Real_Addr = 0u;;

    Fls_Real_Addr = FlsBaseAddress + TargetAddress;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
    /*SWS_Fls_00066*/
    if(FlsModuleState == MEMIF_UNINIT )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_UNINIT);
        RetVal = E_NOT_OK;
    }
    /*SWS_Fls_00030*/
    else if((FlsModuleState == MEMIF_BUSY) || (FlsModuleState == MEMIF_BUSY_INTERNAL) )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_BUSY);
        RetVal = E_NOT_OK;
    }
    else
    {
        /*SWS_Fls_00026*/
        if(( (Fls_Real_Addr % FLS_PAGESIZE) !=  0u  ) || ((Fls_Real_Addr >= FLS_ENDADDRESS)) )
        {
            (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_PARAM_ADDRESS);
            RetVal = E_NOT_OK;
        }
        else
        {
            /*SWS_Fls_00027*/
            if( (  ((Fls_Real_Addr + Length ) % FLS_PAGESIZE) != 0u) || ((Fls_Real_Addr + Length) > FLS_ENDADDRESS) )
            {
                (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_PARAM_LENGTH);
                RetVal = E_NOT_OK;
            }
            else
            {
#endif

                /* SWS_Fls_00331 */
                Fls_Write_Job.Write_Flg		= Flg_High;
                Fls_Write_Job.Write_address = Fls_Real_Addr;
                Fls_Write_Job.Src_Addr_Ptr  = SourceAddressPtr;
                Fls_Write_Job.Length        = Length;   //Length will be rounded up to the next full sector boundary

                /*SWS_Fls_00332*/
                FlsModuleState = MEMIF_BUSY;
                /*SWS_Fls_00333*/
                Fls_JobResult = MEMIF_JOB_PENDING;
                /*SWS_Fls_00334]*/
                RetVal = E_OK ;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
            }
        }
    }
#endif

    return RetVal;
}

Std_ReturnType Fls_Hal_Write_FBL( Fls_AddressType TargetAddress,const uint8* SourceAddressPtr,Fls_LengthType Length)
{
    Std_ReturnType RetVal = E_OK;
    uint32 Fls_Real_Addr = 0u;;

    Fls_Real_Addr = TargetAddress;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
    /*SWS_Fls_00066*/
    if(FlsModuleState == MEMIF_UNINIT )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_UNINIT);
        RetVal = E_NOT_OK;
    }
    /*SWS_Fls_00030*/
    else if((FlsModuleState == MEMIF_BUSY) || (FlsModuleState == MEMIF_BUSY_INTERNAL) )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_BUSY);
        RetVal = E_NOT_OK;
    }
    else
    {
        /*SWS_Fls_00026*/
        if(( (Fls_Real_Addr % FLS_PAGESIZE) !=  0u  ) || ((Fls_Real_Addr >= FLS_ENDADDRESS)) )
        {
            (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_PARAM_ADDRESS);
            RetVal = E_NOT_OK;
        }
        else
        {
            /*SWS_Fls_00027*/
            if( (  ((Fls_Real_Addr + Length ) % FLS_PAGESIZE) != 0u) || ((Fls_Real_Addr + Length) > FLS_ENDADDRESS) )
            {
                (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_PARAM_LENGTH);
                RetVal = E_NOT_OK;
            }
            else
            {
#endif

                /* SWS_Fls_00331 */
                Fls_Write_Job.Write_Flg		= Flg_High;
                Fls_Write_Job.Write_address = Fls_Real_Addr;
                Fls_Write_Job.Src_Addr_Ptr  = SourceAddressPtr;
                Fls_Write_Job.Length        = Length;   //Length will be rounded up to the next full sector boundary

                /*SWS_Fls_00332*/
                FlsModuleState = MEMIF_BUSY;
                /*SWS_Fls_00333*/
                Fls_JobResult = MEMIF_JOB_PENDING;
                /*SWS_Fls_00334]*/
                RetVal = E_OK ;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
            }
        }
    }
#endif

    return RetVal;
}

/******************************************************************************
 *  Service name    : Fls_Hal_GetStatus
 *  Syntax          : MemIf_StatusType Fls_Hal_GetStatus(void)
 *  Reentrancy      : Reentrant
 *  Parameters (in) : None
 *  Parameters (out): None
 *  Return value    : MemIf_StatusType
 *  Description     : Service for FLASH status get
 ******************************************************************************/
MemIf_StatusType Fls_Hal_GetStatus(void)
{
    /*SWS_Fls_00034*/
    return FlsModuleState;
}

/******************************************************************************
 *  Service name    : Fls_Hal_GetJobResult
 *  Syntax          : MemIf_JobResultType Fls_Hal_GetJobResult(void)
 *  Reentrancy      : Reentrant
 *  Parameters (in) : None
 *  Parameters (out): None
 *  Return value    : MemIf_JobResultType
 *  Description     : Service for FLASH job result get
 ******************************************************************************/
MemIf_JobResultType Fls_Hal_GetJobResult(void)
{
    /*SWS_Fls_00035 ,  SWS_Fls_00036*/
    return Fls_JobResult;
}

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
/*SWS_Fls_00236*/
Std_ReturnType Fls_Hal_Read(Fls_AddressType SourceAddress,uint8* TargetAddressPtr,Fls_LengthType Length)
{
    Std_ReturnType RetVal = E_OK;
    uint32 Fls_Real_Addr = 0u;;

    /* SWS_Fls_00239*/
    Fls_Real_Addr = FlsBaseAddress + SourceAddress;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
    /* SWS_Fls_00099  SWS_Fls_00240 */
    if(FlsModuleState == MEMIF_UNINIT )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_SID, FLS_E_UNINIT);
        RetVal = E_NOT_OK;
    }
    /* SWS_Fls_00100 */
    else if((FlsModuleState == MEMIF_BUSY) || (FlsModuleState == MEMIF_BUSY_INTERNAL) )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_SID, FLS_E_BUSY);
        RetVal = E_NOT_OK;
    }
    else
    {
        /* SWS_Fls_00097 */
        if(Fls_Real_Addr >= FLS_ENDADDRESS)
        {
            (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_SID, FLS_E_PARAM_ADDRESS);
            RetVal = E_NOT_OK;
        }
        else
        {
            /* SWS_Fls_00098 */
            if( (Fls_Real_Addr + Length) > FLS_ENDADDRESS)
            {
                (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_SID, FLS_E_PARAM_LENGTH);
                RetVal = E_NOT_OK;
            }
            else
            {
#endif
                /*SWS_Fls_00337*/
                Fls_Read_Job.Read_Flg		= Flg_High;
                Fls_Read_Job.Read_address   = Fls_Real_Addr;
                Fls_Read_Job.Dest_Addr_Ptr  = TargetAddressPtr;
                Fls_Read_Job.Length         = Length;   //Length will be rounded up to the next full sector boundary

                /*SWS_Fls_00338*/
                FlsModuleState = MEMIF_BUSY;
                /*SWS_Fls_00339*/
                Fls_JobResult = MEMIF_JOB_PENDING;
                /*SWS_Fls_00340*/
                RetVal = E_OK ;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
            }
        }
    }
#endif

    return RetVal;
}

Std_ReturnType Fls_Hal_Read_FBL(Fls_AddressType SourceAddress,uint8* TargetAddressPtr,Fls_LengthType Length)
{
    Std_ReturnType RetVal = E_OK;
    uint32 Fls_Real_Addr = 0u;;

    /* SWS_Fls_00239*/
    Fls_Real_Addr = SourceAddress;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
    /* SWS_Fls_00099  SWS_Fls_00240 */
    if(FlsModuleState == MEMIF_UNINIT )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_SID, FLS_E_UNINIT);
        RetVal = E_NOT_OK;
    }
    /* SWS_Fls_00100 */
    else if((FlsModuleState == MEMIF_BUSY) || (FlsModuleState == MEMIF_BUSY_INTERNAL) )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_SID, FLS_E_BUSY);
        RetVal = E_NOT_OK;
    }
    else
    {
        /* SWS_Fls_00097 */
        if(Fls_Real_Addr >= FLS_ENDADDRESS)
        {
            (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_SID, FLS_E_PARAM_ADDRESS);
            RetVal = E_NOT_OK;
        }
        else
        {
            /* SWS_Fls_00098 */
            if( (Fls_Real_Addr + Length) > FLS_ENDADDRESS)
            {
                (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_SID, FLS_E_PARAM_LENGTH);
                RetVal = E_NOT_OK;
            }
            else
            {
#endif
                /*SWS_Fls_00337*/
                Fls_Read_Job.Read_Flg		= Flg_High;
                Fls_Read_Job.Read_address   = Fls_Real_Addr;
                Fls_Read_Job.Dest_Addr_Ptr  = TargetAddressPtr;
                Fls_Read_Job.Length         = Length;   //Length will be rounded up to the next full sector boundary

                /*SWS_Fls_00338*/
                FlsModuleState = MEMIF_BUSY;
                /*SWS_Fls_00339*/
                Fls_JobResult = MEMIF_JOB_PENDING;
                /*SWS_Fls_00340*/
                RetVal = E_OK ;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
            }
        }
    }
#endif

    return RetVal;
}

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
Std_ReturnType Fls_Hal_Compare(Fls_AddressType SourceAddress,const uint8* TargetAddressPtr,Fls_LengthType Length)
{
    Std_ReturnType RetVal = E_OK;
    uint32 Fls_Real_Addr = 0u;;

    /* SWS_Fls_00244 */
    Fls_Real_Addr = FlsBaseAddress + SourceAddress;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
    /* SWS_Fls_00152  */
    if(FlsModuleState == MEMIF_UNINIT )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_COMPARE_SID, FLS_E_UNINIT);
        RetVal = E_NOT_OK;
    }
    /* SWS_Fls_00153 */
    else if((FlsModuleState == MEMIF_BUSY) || (FlsModuleState == MEMIF_BUSY_INTERNAL) )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_COMPARE_SID, FLS_E_BUSY);
        RetVal = E_NOT_OK;
    }
    else
    {
        /* SWS_Fls_00150 */
        if(Fls_Real_Addr >= FLS_ENDADDRESS)
        {
            (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_COMPARE_SID, FLS_E_PARAM_ADDRESS);
            RetVal = E_NOT_OK;
        }
        else
        {
            /* SWS_Fls_00151 */
            if( (Fls_Real_Addr + Length) > FLS_ENDADDRESS)
            {
                (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_COMPARE_SID, FLS_E_PARAM_LENGTH);
                RetVal = E_NOT_OK;
            }
            else
            {
#endif
                /* SWS_Fls_00341 */
                Fls_Compare_Job.Compare_Flg		  = Flg_High;
                Fls_Compare_Job.Compare_address   = Fls_Real_Addr;
                Fls_Compare_Job.Src_Addr_Ptr      = TargetAddressPtr;
                Fls_Compare_Job.Length	          = Length;

                /* SWS_Fls_00342 */
                FlsModuleState = MEMIF_BUSY;
                /* SWS_Fls_00343 */
                Fls_JobResult = MEMIF_JOB_PENDING;
                /* SWS_Fls_00344 */
                RetVal = E_OK ;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
            }
        }
    }
#endif


    return RetVal;
}

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
void Fls_Hal_SetMode(MemIf_ModeType Mode)
{
    /*SWS_Fls_00155*/

#if (FLS_DEV_ERROR_DETECT == STD_ON)
    if((FlsModuleState == MEMIF_BUSY) || (FlsModuleState == MEMIF_BUSY_INTERNAL) )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_SETMODE_SID, FLS_E_BUSY);
    }
    else
    {
#endif
        switch( Mode )
        {
        case MEMIF_MODE_FAST:
        {
            Fls_MaxRead = Fls_ConfigPtr->FlsMaxReadFastMode;
            Fls_MaxWrite = Fls_ConfigPtr->FlsMaxWriteFastMode;
            break;
        }
        case MEMIF_MODE_SLOW:
        {
            Fls_MaxRead = Fls_ConfigPtr->FlsMaxReadNormalMode;
            Fls_MaxWrite = Fls_ConfigPtr->FlsMaxWriteNormalMode;
            break;
        }
        default:
        {
            //misra-c complicant
            break;
        }
        }
#if (FLS_DEV_ERROR_DETECT == STD_ON)
    }
#endif
}

/******************************************************************************
 *  Service name    : Fls_Hal_BlankCheck
 *  Syntax          : Std_ReturnType Fls_Hal_BlankCheck(Fls_AddressType TargetAddress,Fls_LengthType Length)
 *  Reentrancy      : Non reentrant
 *  Parameters (in) : TargetAddressPtr: Pointer to target data buffer
 *  					 Length: Number of bytes to be checked whether erased
 *  Parameters (out): None
 *  Return value    : Std_ReturnType
 *  Description     : Service for FLASH check
 ******************************************************************************/
/* SWS_Fls_00373 */
Std_ReturnType Fls_Hal_BlankCheck(Fls_AddressType TargetAddress,Fls_LengthType Length)
{
    Std_ReturnType RetVal = E_OK;
    uint32 Fls_Real_Addr = 0u;;

    Fls_Real_Addr = FlsBaseAddress + TargetAddress;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
    /* SWS_Fls_00382 */
    if(FlsModuleState == MEMIF_UNINIT )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANKCHECK_SID, FLS_E_UNINIT);
        RetVal = E_NOT_OK;
    }
    /*SWS_Fls_00383 */
    else if((FlsModuleState == MEMIF_BUSY) || (FlsModuleState == MEMIF_BUSY_INTERNAL) )
    {
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANKCHECK_SID, FLS_E_BUSY);
        RetVal = E_NOT_OK;
    }
    else
    {
        /*SWS_Fls_00380*/
        if(Fls_Real_Addr >= FLS_ENDADDRESS)
        {
            (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANKCHECK_SID, FLS_E_PARAM_ADDRESS);
            RetVal = E_NOT_OK;
        }
        else
        {
            /* SWS_Fls_00381 */
            if( (Fls_Real_Addr + Length) > FLS_ENDADDRESS)
            {
                (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANKCHECK_SID, FLS_E_PARAM_LENGTH);
                RetVal = E_NOT_OK;
            }
            else
            {
#endif
                /* SWS_Fls_00374 */
                Fls_BlankCheck_Job.BlankCheck_Flg		= Flg_High;
                Fls_BlankCheck_Job.BlankCheck_address   = Fls_Real_Addr;
                Fls_BlankCheck_Job.Length				= Length;

                /* SWS_Fls_00375 */
                FlsModuleState = MEMIF_BUSY;
                /* SWS_Fls_00376 */
                Fls_JobResult = MEMIF_JOB_PENDING;
                /* SWS_Fls_00377 */
                RetVal = E_OK ;

#if (FLS_DEV_ERROR_DETECT == STD_ON)
            }
        }
    }
#endif

    return RetVal;
}

/******************************************************************************
 *  Service name    : Fls_Hal_MainFunction_Handling
 *  Syntax          : void Fls_Hal_MainFunction_Handling(void)
 *  Reentrancy      : Reentrant
 *  Parameters (in) : None
 *  Parameters (out): None
 *  Return value    : None
 *  Description     : Performs the processing of jobs.
 ******************************************************************************/
/* SWS_Fls_00037  SWS_Fls_00038 SWS_Fls_00039 SWS_Fls_00040 */
void Fls_Hal_MainFunction_Handling(void)
{
    /*SWS_Fls_00220*/
    if(Fls_Erase_Job.Erase_Flg == Flg_High)
    {
        Fls_Hal_MainFunction_Handling_Erase();
    }
    else
    {}

    /*SWS_Fls_00225*/
    if(Fls_Write_Job.Write_Flg  == Flg_High)
    {
        Fls_Hal_MainFunction_Handling_Write();
    }
    else
    {}

    /* SWS_Fls_00238*/
    if(Fls_Read_Job.Read_Flg  == Flg_High)
    {
        Fls_Hal_MainFunction_Handling_Read();
    }
    else
    {}

#if (FlsCompareApi == STD_ON)
    /*SWS_Fls_00243*/
    if(Fls_Compare_Job.Compare_Flg == Flg_High)
    {
        Fls_Hal_MainFunction_Handling_Compare();
    }
    else
    {}
#endif

    /*SWS_Fls_00378  SWS_Fls_00379*/
    if(Fls_BlankCheck_Job.BlankCheck_Flg  == Flg_High)
    {
        /* ETAS BIP Porting */
        Fls_Hal_MainFunction_Handling_BlankCheck();
    }
    else
    {}
}


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
void Fls_lclParamCheck(boolean Expr, uint8 ApiId, uint8 ErrId, Std_ReturnType *RetVal)
{
    /* checks if the expr is TRUE */
    if ((TRUE == Expr) && (E_OK == *RetVal))
    {
#if (FLS_DEV_ERROR_DETECT == STD_ON)
        /* calls Det_ReportError from Det module */
        (void)Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE_ID, ApiId, ErrId);
#endif

        *RetVal = E_NOT_OK;
    }
}


/*******************************************************************************
 *  Service name     : Fls_ID_Check
 *  Syntax           : static Std_ReturnType Fls_ID_Check(uint8* ID)
 *  Parameters (in)  : ID: pointer to the ID to be checked
 *  Parameter(in-out): None
 *  Return value     : None
 *  Description      : service for the Flash ID check
 *******************************************************************************/
static Std_ReturnType Fls_ID_Check(uint8* ID)
{
    Std_ReturnType Ret_Val = E_OK;

    if( (ID[0u] == S25FL128_Manu_ID ) && (ID[1u] == S25FL128_Device_ID ) )
    {
        /*Different sector size to erase in S25L128*/
        Fls_Region.BoundAddr = 0x20000u;
        Fls_Region.SectorSize1 = 0x1000u;
        Fls_Region.Erase_Ins1 = 0x20;
        Fls_Region.SectorSize2 = 0x10000u;
        Fls_Region.Erase_Ins2 = 0xD8;
    }
    else if((ID[0u] == W25Q32JV_Manu_ID ) && (ID[1u] == W25Q32JV_Device_ID ))
    {
        Fls_Region.BoundAddr = 0x400000;
        Fls_Region.Erase_Ins1 = 0x20;
        Fls_Region.SectorSize1 = 0x1000u;
    }
    else if((ID[0u] == MX25L3233F_Manu_ID ) && (ID[1u] == MX25L3233F_Device_ID ))
    {
        /*Misra-c*/
        Fls_Region.BoundAddr = 0x400000;
        Fls_Region.Erase_Ins1 = 0x20;
        Fls_Region.SectorSize1 = 0x1000u;
    }
    else if((ID[0u] == IS25WP032D_Manu_ID ) && (ID[1u] == IS25WP032D_Device_ID ))
    {
        Fls_Region.BoundAddr = 0x400000;
        Fls_Region.Erase_Ins1 = 0x20;
        Fls_Region.SectorSize1 = 0x1000u;
    }
    else if((ID[0u] == IS25WP016D_Manu_ID ) && (ID[1u] == IS25WP016D_Device_ID ))
    {
        Fls_Region.BoundAddr = 0x200000;
        Fls_Region.Erase_Ins1 = 0x20;
        Fls_Region.SectorSize1 = 0x1000u;
    }
    else if((ID[0u] == GD25Q32C_Manu_ID ) && (ID[1u] == GD25Q32C_Device_ID ))
    {
        Fls_Region.BoundAddr = 0x400000;
        Fls_Region.Erase_Ins1 = 0x20;
        Fls_Region.SectorSize1 = 0x1000u;
    }
    else
    {
        Ret_Val = E_NOT_OK;
    }

    return Ret_Val;
}

/*******************************************************************************
 *  Service name     : Fls_Hal_MainFunction_Handling_Erase
 *  Syntax           : static void Fls_Hal_MainFunction_Handling_Erase(void)
 *  Parameters (in)  : None
 *  Parameter(in-out): None
 *  Return value     : None
 *  Description      : Erase main function function running of the Flash module
 *******************************************************************************/
static void Fls_Hal_MainFunction_Handling_Erase(void)
{
    Std_ReturnType RetVal = E_OK;
    uint8 RdStatus = 0u;
    uint32 Erase_Addr = 0u;
    uint8 Erase_cmd[4u] = {0u};

    RetVal = Fls_GetSPIStatusRegister(&RdStatus);

    if(RetVal == E_OK)
    {
        while( (RdStatus & 0x01u) == 0x01u)        //polling until it is not busy
        {
            Fls_GetSPIStatusRegister(&RdStatus);
        }

        /* SWS_Fls_00196 */
        while(Fls_Erase_Job.Numof_Sector > 0u)
        {
            Fls_Erase_Job.Numof_Sector = Fls_Erase_Job.Numof_Sector - 1U;
            Erase_Addr = Fls_Erase_Job.Erase_address + (Fls_Erase_Job.Numof_Sector * Fls_SectorSize);

            Fls_EnableWrite();
            RetVal = Fls_GetSPIStatusRegister(&RdStatus);

            if( ((RdStatus & 0x02u) == 0x02u) && (E_OK== RetVal) )    /*check write enable*/
            {
                SpiConfigData.SpiExternalDevice[QSPI_ID].SpiDeviceType = SPI_DEVICE ;     //fixme

                //SpiConfigData.SpiJobConfig[QSPI_ERASEJOB].SpiInstruction = Fls_EraseIns;
                //SpiConfigData.SpiJobConfig[QSPI_ERASEJOB].SpiAddress = Erase_Addr;
                Erase_cmd[0u] = 0x20u;
                Erase_cmd[1u] = (uint8)((Erase_Addr & 0xFF0000u ) >> 16u);
                Erase_cmd[2u] = (uint8)((Erase_Addr & 0xFF00u ) >> 8u);
                Erase_cmd[3u] = (uint8)( Erase_Addr & 0xFFu );

                //RetVal |= Spi_WriteIB( QSPI_CHANNEL_ERASEPAGE, NULL_PTR ) ;
                Spi_WriteIB( QSPI_CHANNEL_ERASEPAGE, Erase_cmd ) ;
                RetVal |= Spi_SyncTransmit( FLS_ERASEPAGE );

                SpiConfigData.SpiExternalDevice[QSPI_ID].SpiDeviceType = QSPI_DEVICE ;    //fixme

                RetVal |= Fls_GetSPIStatusRegister(&RdStatus);
                while( (RdStatus & 0x01u) == 0x01u)        //polling until write is finished
                {
                    RetVal |= Fls_GetSPIStatusRegister(&RdStatus);
                }

                /* fix bug */
                /*SWS_Fls_00345*/
                Fls_JobResult = MEMIF_JOB_OK;
                Fls_Erase_Job.Erase_Flg = Flg_Low;
                FlsModuleState = MEMIF_IDLE;
            }
            else
            {
                /* SWS_Fls_00104 */
#if( FLS_DEV_ERROR_DETECT == STD_ON )
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_ERASE_FAILED);
#endif
                Fls_JobResult = MEMIF_JOB_FAILED;
                Fls_Erase_Job.Erase_Flg = Flg_Low;
                FlsModuleState = MEMIF_IDLE;
                Fls_CallBackFun();
            }
        }

        /* no more sectors to be erased. */
        {
            /* SWS_Fls_00022 */
#if(FlsEraseVerificationEnabled == STD_ON)
            RetVal = Fls_ErasedCheck( Fls_Erase_Job.Erase_address , Fls_Erase_Job.Numof_Sector_Backup* Fls_SectorSize );
            if( E_OK == RetVal)
            {
                Fls_JobResult = MEMIF_JOB_OK;
            }
            else
            {
#if( FLS_DEV_ERROR_DETECT == STD_ON )
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_VERIFY_ERASE_FAILED);
#endif
                Fls_JobResult = MEMIF_JOB_FAILED;
            }
#endif
            Fls_Erase_Job.Erase_Flg = Flg_Low;
            /* SWS_Fls_00346 */
            FlsModuleState = MEMIF_IDLE;
            Fls_CallBackFun();
        }
    }
    else	  		/*SPI fail*/
    {
        /* SWS_Fls_00104 */
#if( FLS_DEV_ERROR_DETECT == STD_ON )
        (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_ERASE_SID, FLS_E_ERASE_FAILED);
#endif
        Fls_JobResult = MEMIF_JOB_FAILED;
        Fls_Erase_Job.Erase_Flg = Flg_Low;
        FlsModuleState = MEMIF_IDLE;
        Fls_CallBackFun();
    }
}

/*******************************************************************************
 *  Service name     : Fls_Hal_MainFunction_Handling_Write
 *  Syntax           : static void Fls_Hal_MainFunction_Handling_Write(void)
 *  Parameters (in)  : None
 *  Parameter(in-out): None
 *  Return value     : None
 *  Description      : Write main function function running of the Flash module
 *******************************************************************************/
static void Fls_Hal_MainFunction_Handling_Write(void)
{
    Std_ReturnType RetVal = E_OK;
    uint16 i=0u;
    uint8  RdStatus = 0u;
#if(FlsWriteVerificationEnabled == STD_ON)
    uint8 Destdata[FLS_PAGESIZE];
    uint32 SrcdataIdx = 0u;
#endif

    /*SWS_Fls_00055*/
#if(FlsEraseVerificationEnabled == STD_ON)
    RetVal = Fls_ErasedCheck(Fls_Write_Job.Write_address , Fls_Write_Job.Length );

    if( E_OK == RetVal)
    {
#endif
        Fls_EnableWrite();
        RetVal = Fls_GetSPIStatusRegister(&RdStatus);

        if((RetVal == E_OK) && ((RdStatus & 0x02u) == 0x02u))
        {
            /*SWS_Fls_00027*/
            Fls_AddressType sourceAddress = Fls_Write_Job.Write_address;
            Fls_LengthType  remainBytes   = Fls_Write_Job.Length;
            const uint8*    pSourceData   = Fls_Write_Job.Src_Addr_Ptr;
            uint16 pageTail = (uint16)(sourceAddress % FLS_PAGESIZE);
            uint16 opBytes = 0u;

            do
            {
                if(pageTail != 0u)
                {/* source address is not page aligned. */
                    opBytes = (remainBytes >= (FLS_PAGESIZE - pageTail))?(FLS_PAGESIZE - pageTail):remainBytes;
                    pageTail = 0u;
                }
                else
                {
                    opBytes = (remainBytes >= FLS_PAGESIZE)?FLS_PAGESIZE:remainBytes;
                }

                {
                    Fls_EnableWrite();

                    //SpiConfigData.SpiExternalDevice[QSPI_ID].SpiDeviceType = SPI_DEVICE ;     //fixme

                    SpiConfigData.SpiJobConfig[QSPI_WRITEJOB].SpiAddress = sourceAddress;
                    RetVal |= Spi_SetupEB( QSPI_CHANNEL_WRITEPAGE, pSourceData, NULL_PTR, opBytes );
                    RetVal |= Spi_SyncTransmit( FLS_WRITEDATA );

                    //SpiConfigData.SpiExternalDevice[QSPI_ID].SpiDeviceType = QSPI_DEVICE ;     //fixme

                    Fls_GetSPIStatusRegister(&RdStatus);
                    while( (RdStatus & 0x01u) == 0x01u)        //polling until it is not busy
                    {
                        Fls_GetSPIStatusRegister(&RdStatus);
                    }

                    sourceAddress += opBytes;
                    remainBytes -= opBytes;
                    pSourceData += opBytes;
                }
            }while(remainBytes > 0u);

            if(RetVal == E_OK)
            {
                /*SWS_Fls_00345*/
                Fls_JobResult = MEMIF_JOB_OK;
                /* SWS_Fls_00056 */
#if(FlsWriteVerificationEnabled == STD_ON)
                Fls_AddressType sourceAddress = Fls_Write_Job.Write_address;
                Fls_LengthType  remainBytes   = Fls_Write_Job.Length;
                uint16 pageTail = (uint16)(sourceAddress % FLS_PAGESIZE);
                uint16 opBytes = 0u;

                do
                {
                    if(pageTail != 0u)
                    {/* source address is not page aligned. */
                        opBytes = (remainBytes >= (FLS_PAGESIZE - pageTail))?(FLS_PAGESIZE - pageTail):remainBytes;
                        pageTail = 0u;
                    }
                    else
                    {
                        opBytes = (remainBytes >= FLS_PAGESIZE)?FLS_PAGESIZE:remainBytes;
                    }

                    {
                        SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiAddress = sourceAddress;
                        SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiReadCount = opBytes;

                        RetVal |= Spi_SetupEB(QSPI_CHANNEL_READBUFFER, NULL_PTR, &Destdata[0] , opBytes);
                        RetVal |= Spi_SyncTransmit( FLS_READDATA );

                        sourceAddress += opBytes;
                        remainBytes -= opBytes;
                    }

                    {
                        if( RetVal == E_OK)
                        {
                            if(Fls_Sum(&Fls_Write_Job.Src_Addr_Ptr[SrcdataIdx], opBytes) == Fls_Sum(Destdata, opBytes))
                            {
                                SrcdataIdx = SrcdataIdx + opBytes;
                            }
                            else
                            {
                                Fls_JobResult = MEMIF_JOB_FAILED;
                                break;
                            }
                        }
                        else
                        {
                            /* SWS_Fls_00154 */
#if( FLS_DEV_ERROR_DETECT == STD_ON )
                            (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_VERIFY_WRITE_FAILED );
#endif
                            Fls_JobResult = MEMIF_JOB_FAILED;
                            break;
                        }
                    }
                }while(remainBytes > 0u);
#endif
            }
            else
            {
                /* SWS_Fls_00105 */
#if( FLS_DEV_ERROR_DETECT == STD_ON )
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_WRITE_FAILED );
#endif
                Fls_JobResult = MEMIF_JOB_FAILED;
            }
        }
        else	/*SPI fail*/
        {
            /* SWS_Fls_00105 */
#if( FLS_DEV_ERROR_DETECT == STD_ON )
            (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_WRITE_FAILED );
#endif
            Fls_JobResult = MEMIF_JOB_FAILED;
        }
#if(FlsEraseVerificationEnabled == STD_ON)
    }
    else
    {
#if( FLS_DEV_ERROR_DETECT == STD_ON )
        (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_WRITE_SID, FLS_E_VERIFY_ERASE_FAILED);
#endif
        Fls_JobResult = MEMIF_JOB_FAILED;
    }
#endif

    Fls_Write_Job.Write_Flg = Flg_Low;
    FlsModuleState = MEMIF_IDLE;
    Fls_CallBackFun();
}


/*******************************************************************************
 *  Service name     : Fls_Hal_MainFunction_Handling_Read
 *  Syntax           : static void Fls_Hal_MainFunction_Handling_Read(void)
 *  Parameters (in)  : None
 *  Parameter(in-out): None
 *  Return value     : None
 *  Description      : Read main function function running of the Flash module
 *******************************************************************************/
static void Fls_Hal_MainFunction_Handling_Read(void)
{
    Std_ReturnType RetVal = E_OK;
    uint16 i=0u;

    Fls_AddressType sourceAddress = Fls_Read_Job.Read_address;
    Fls_LengthType  remainBytes   = Fls_Read_Job.Length;
    uint8*          pDsteData     = Fls_Read_Job.Dest_Addr_Ptr;
    uint16 pageTail = (uint16)(sourceAddress % FLS_PAGESIZE);
    uint16 opBytes = 0u;

    do
    {
        if(pageTail != 0u)
        {/* source address is not page aligned. */
            opBytes = (remainBytes >= (FLS_PAGESIZE - pageTail))?(FLS_PAGESIZE - pageTail):remainBytes;
            pageTail = 0u;
        }
        else
        {
            opBytes = (remainBytes >= FLS_PAGESIZE)?FLS_PAGESIZE:remainBytes;
        }

        {
            SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiAddress = sourceAddress;
            SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiReadCount = opBytes;

            /*read data from the flash space*/
            RetVal |= Spi_SetupEB(QSPI_CHANNEL_READBUFFER, NULL_PTR, pDsteData, opBytes);
            RetVal |= Spi_SyncTransmit( FLS_READDATA );

            sourceAddress += opBytes;
            remainBytes -= opBytes;
            pDsteData += opBytes;

            if(RetVal == E_OK)
            {
                Fls_JobResult = MEMIF_JOB_OK;
            }
            else
            {
                /* SWS_Fls_00106 */
#if( FLS_DEV_ERROR_DETECT == STD_ON )
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_READ_SID, FLS_E_READ_FAILED );
#endif
                Fls_JobResult = MEMIF_JOB_FAILED;
                break;
            }
        }

    }while(remainBytes > 0u);

    Fls_Read_Job.Read_Flg =  Flg_Low;
    FlsModuleState = MEMIF_IDLE;
    Fls_CallBackFun();
}

#if (FlsCompareApi == STD_ON)
/*******************************************************************************
 *  Service name     : Fls_Hal_MainFunction_Handling_Compare
 *  Syntax           : static void Fls_Hal_MainFunction_Handling_Compare(void)
 *  Parameters (in)  : None
 *  Parameter(in-out): None
 *  Return value     : None
 *  Description      : Compare main function function running of the Flash module
 *******************************************************************************/
static void Fls_Hal_MainFunction_Handling_Compare(void)
{
    Std_ReturnType RetVal = E_OK;
    uint16 i=0u;
    uint8  Destdata = 0u;

    for(i = 0; i < Fls_Compare_Job.Length; i++)
    {
        SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiAddress = Fls_Compare_Job.Compare_address + i;
        SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiReadCount = 1u ;      							 /*read one byte and compare*/

        /*read data from the flash space*/
        RetVal |= Spi_SetupEB( QSPI_CHANNEL_READBUFFER, NULL_PTR, &Destdata, 1u);
        RetVal |= Spi_SyncTransmit( FLS_READDATA );

        if( RetVal == E_OK)
        {
            if(Fls_Compare_Job.Src_Addr_Ptr[i] != Destdata)
            {
                /*SWS_Fls_00200*/
                Fls_JobResult = MEMIF_BLOCK_INCONSISTENT;
                break;
            }
        }
        else
        {
            /* SWS_Fls_00154 */
#if( FLS_DEV_ERROR_DETECT == STD_ON )
            (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_COMPARE_SID, FLS_E_COMPARE_FAILED );
#endif

            Fls_JobResult = MEMIF_JOB_FAILED;
            break;
        }
        Fls_JobResult = MEMIF_JOB_OK;
    }

    Fls_Compare_Job.Compare_Flg =  Flg_Low;
    FlsModuleState = MEMIF_IDLE;
    Fls_CallBackFun();
}
#endif

/**
 * @brief ETAS BIP Porting
 */
#if (FlsBlankCheckApi == STD_ON) 
static void Fls_Hal_MainFunction_Handling_BlankCheck(void)
{
    Std_ReturnType RetVal = E_OK;
    uint16 i=0u;
    uint8  Destdata[FLS_PAGESIZE] = {0};

    Fls_AddressType sourceAddress = Fls_BlankCheck_Job.BlankCheck_address;
    Fls_LengthType  remainBytes   = Fls_BlankCheck_Job.Length;
    uint16 pageTail = (uint16)(sourceAddress % FLS_PAGESIZE);
    uint16 opBytes = 0u;

    do
    {
        if(pageTail != 0u)
        {/* source address is not page aligned. */
            opBytes = (remainBytes >= (FLS_PAGESIZE - pageTail))?(FLS_PAGESIZE - pageTail):remainBytes;
            pageTail = 0u;
        }
        else
        {
            opBytes = (remainBytes >= FLS_PAGESIZE)?FLS_PAGESIZE:remainBytes;
        }

        {
            SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiAddress = sourceAddress;
            SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiReadCount = opBytes;

            /*read data from the flash space*/
            RetVal |= Spi_SetupEB( QSPI_CHANNEL_READBUFFER, NULL_PTR, &Destdata[0], opBytes);
            RetVal |= Spi_SyncTransmit( FLS_READDATA );

            sourceAddress += opBytes;
            remainBytes -= opBytes;

            if(RetVal == E_OK)
            {
                if(opBytes == FLS_PAGESIZE)
                {
                    if(Fls_Sum(Destdata, FLS_PAGESIZE) == 0xFF00u)
                    {
                        Fls_JobResult = MEMIF_JOB_OK;
                    }
                    else
                    {
                        /*SWS_Fls_00200*/
                        Fls_JobResult = MEMIF_BLOCK_INCONSISTENT; //zco7sgh MEMIF_JOB_FAILED
                        break;
                    }
                }
                else /* opBytes < FLS_PAGESIZE */
                {
                    for(i = 0U; i < opBytes; i++)
                    {
                        if(FLS_ERASE_PATTERN != Destdata[i])
                        {
                            /*SWS_Fls_00200*/
                            Fls_JobResult = MEMIF_BLOCK_INCONSISTENT; //zco7sgh MEMIF_JOB_FAILED
                            break;
                        }
                        else
                        {
                            Fls_JobResult = MEMIF_JOB_OK;
                        }
                    }
                }
            }
            else
            {
                /* SWS_Fls_00154 */
#if( FLS_DEV_ERROR_DETECT == STD_ON )
                (void)Det_ReportRuntimeError((uint16)FLS_MODULE_ID, FLS_INSTANCE_ID, FLS_BLANKCHECK_SID, FLS_E_VERIFY_ERASE_FAILED );
#endif
                Fls_JobResult = MEMIF_JOB_FAILED;
                break;
            }
        }
    }while(remainBytes > 0u);

    Fls_BlankCheck_Job.BlankCheck_Flg =  Flg_Low;
    FlsModuleState = MEMIF_IDLE;
    Fls_CallBackFun();
}
#endif

/*******************************************************************************
 *  Service name     : Fls_GetSPIStatusRegister
 *  Syntax           : static Std_ReturnType Fls_GetSPIStatusRegister(uint8* RdStatus )
 *  Parameters (in)  : None
 *  Parameter(in-out): RdStatus: pointer to store the data of the Flash status register
 *  Return value     : Std_ReturnType
 *  Description      : Get the status register of the external flash IC S25FL128S
 *******************************************************************************/

static Std_ReturnType Fls_GetSPIStatusRegister(uint8* RdStatus )
{
    Std_ReturnType RetVal = E_OK;

    RetVal = Spi_WriteIB( QSPI_CHANNEL_READSTATUS, NULL_PTR);
    RetVal |= Spi_SyncTransmit( FLS_READSTATUSCFG );
    RetVal |= Spi_ReadIB( QSPI_CHANNEL_READSTATUS, RdStatus);

    return RetVal;
}

/*******************************************************************************
 *  Service name     : Fls_EnableWrite
 *  Syntax           : static void Fls_EnableWrite(void)
 *  Parameters (in)  : None
 *  Parameter(in-out): None
 *  Return value     : None
 *  Description      : enable the write function of the external flash IC S25FL128S
 *******************************************************************************/
static void Fls_EnableWrite(void)
{
    Spi_WriteIB( QSPI_CHANNEL_WRITEENABLE, NULL_PTR);
    Spi_SyncTransmit( FLS_WRITEENABLEDATA );
}

/*******************************************************************************
 *  Service name     : Fls_ErasedCheck
 *  Syntax           : static Std_ReturnType Fls_ErasedCheck(Fls_AddressType SourceAddress , Fls_LengthType Length )
 *  Parameters (in)  : SourceAddress: Address of the Flash to be checked erased or not
 *  					  Length : Length of the Flash to be checked erased or not
 *  Parameter(in-out): None
 *  Return value     : Std_ReturnType
 *  Description      : check the specific place erased or not of the external flash IC S25FL128S
 *******************************************************************************/
static Std_ReturnType Fls_ErasedCheck(Fls_AddressType SourceAddress , Fls_LengthType Length )
{
    Std_ReturnType RetVal = E_OK;
    uint32 i = 0u ;
    uint8  Destdata[FLS_PAGESIZE] = {0};
    
    Fls_AddressType sourceAddress = SourceAddress;
    Fls_LengthType  remainBytes   = Length;
    uint16 pageTail = (uint16)(sourceAddress % FLS_PAGESIZE);
    uint16 opBytes = 0u;

    do
    {
        if(pageTail != 0u)
        {/* source address is not page aligned. */
            opBytes = (remainBytes >= (FLS_PAGESIZE - pageTail))?(FLS_PAGESIZE - pageTail):remainBytes;
            pageTail = 0u;
        }
        else
        {
            opBytes = (remainBytes >= FLS_PAGESIZE)?FLS_PAGESIZE:remainBytes;
        }

        {
            SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiAddress = sourceAddress;
            SpiConfigData.SpiJobConfig[QSPI_READJOB].SpiReadCount = opBytes;

            RetVal |= Spi_SetupEB(QSPI_CHANNEL_READBUFFER, NULL_PTR, &Destdata[0] , opBytes);
            RetVal |= Spi_SyncTransmit( FLS_READDATA );

            sourceAddress += opBytes;
            remainBytes -= opBytes;

            if(RetVal == E_OK)
            {
                if(opBytes == FLS_PAGESIZE)
                {
                    if(Fls_Sum(Destdata, FLS_PAGESIZE) != 0xFF00u)
                    {
                        RetVal = E_NOT_OK;
                        break;
                    }
                }
                else /* opBytes < FLS_PAGESIZE */
                {
                    for(i = 0U; i < opBytes; i++)
                    {
                        if(FLS_ERASE_PATTERN != Destdata[i])
                        {
                            RetVal = E_NOT_OK;
                            break;
                        }
                    }
                }
            }
            else
            {
                RetVal = E_NOT_OK;
                break;
            }
        }
    }while(remainBytes > 0u);

    return RetVal;
}

/*******************************************************************************
 *  Service name     : Fls_Sum
 *  Syntax           : static uint32 Fls_Sum(uint8* data,uint16 length)
 *  Parameters (in)  : None
 *  Parameter(in-out): None
 *  Return value     : None
 *  Description      : Calculate the sum of the data input
 *******************************************************************************/
static uint32 Fls_Sum(const uint8* data,uint16 length)
{
    uint32 sum = 0;
    uint16 i;
    for(i=0;i<length;i++)
    {
        sum = sum + (uint32)(data[i]);
    }
    return sum;
}

/*******************************************************************************
 *  Service name     : Fls_CallBackFun
 *  Syntax           : static void Fls_CallBackFun(void)
 *  Parameters (in)  : None
 *  Parameter(in-out): None
 *  Return value     : None
 *  Description      : Call-back function running of the Flash module
 *******************************************************************************/
static void Fls_CallBackFun(void)
{
    if( NULL_PTR != Fls_ConfigPtr )                             //For unit-test
    {
        /* SWS_Fls_00109  SWS_Fls_00110  */
        if( MEMIF_JOB_OK == Fls_JobResult)
        {
            /*SWS_Fls_00262  SWS_Fls_00167 */
            if(Fls_ConfigPtr->FlsJobEndNotification != NULL_PTR)
            {
                Fls_ConfigPtr->FlsJobEndNotification();
            }
            else
            {}
        }
        else
        {
            /* SWS_Fls_00263   SWS_Fls_00347  SWS_Fls_00349 */
            if(Fls_ConfigPtr->FlsJobErrorNotification != NULL_PTR)
            {
                Fls_ConfigPtr->FlsJobErrorNotification();
            }
            else
            {}
        }
    }
    else
    {}
}

#define FLS_STOP_SEC_CODE

