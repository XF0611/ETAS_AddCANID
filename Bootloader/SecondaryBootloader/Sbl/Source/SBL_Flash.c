#include "SBL_Flash.h"
#include "Fls.h"
#include "Fls_Alps.h"
#include "Std_Types.h"


extern uint8 errId;

#pragma data(".sbl_const_flash_routine_header");
const tFlashHeader FlashHeader =
{
    SBL_FlashInit,
    SBL_FlashDeinit,
    SBL_FlashErase,
    SBL_FlashWrite,
    SBL_FlashRead,
};
#pragma data();

static uint8 SBL_WriteBuffer[FLASH_DRIVER_MAX_OF_BYTE_TO_WRITE];

void main(void)
{
    /* joking. */
}

static void* _MemCopy(void* xDest_pv, const void* xSrc_pcv, uint32 numBytes_u32)
{
    uint32* xDest_pu32       = (uint32*)xDest_pv;
    const uint32* xSrc_pcu32 = (const uint32*)xSrc_pcv;
    uint16* xDest_pu16;
    const uint16* xSrc_pcu16;
    uint8* xDest_pu8;
    const uint8* xSrc_pcu8;
    uint32 ctLoop_u32;

    /* 32 bit aligned copy */
    /* MISRA RULE 11.3 VIOLATION: cast cannot be avoided here */
    if ((numBytes_u32 >= 4) && ((((uint32)xDest_pu32 | (uint32)xSrc_pcu32) & 0x03) == 0))
    {
        ctLoop_u32 = numBytes_u32 / 4;
        numBytes_u32 &= 0x03;
        do
        {
            *xDest_pu32 = *xSrc_pcu32;
            xDest_pu32++;
            xSrc_pcu32++;
            ctLoop_u32--;
        } while(ctLoop_u32 > 0);
    }
    /* MISRA RULE 11.4 VIOLATION: cast cannot be avoided here */
    xDest_pu16 = (uint16*)xDest_pu32;
    xSrc_pcu16 = (const uint16*)xSrc_pcu32;

    /* 16 bit aligned copy */
    /* MISRA RULE 11.3 VIOLATION: cast cannot be avoided here */
    if ((numBytes_u32 >= 2) && ((((uint32)xDest_pu16 | (uint32)xSrc_pcu16) & 0x01) == 0))
    {
        ctLoop_u32 = numBytes_u32 / 2;
        numBytes_u32 &= 0x01;
        do
        {
            *xDest_pu16 = *xSrc_pcu16;
            xDest_pu16++;
            xSrc_pcu16++;
            ctLoop_u32--;
        } while(ctLoop_u32 > 0);
    }
    /* MISRA RULE 11.4 VIOLATION: cast cannot be avoided here */
    xDest_pu8 = (uint8*)xDest_pu16;
    xSrc_pcu8 = (const uint8*)xSrc_pcu16;

    /* 8 bit copy for remaining data */
    for(ctLoop_u32 = 0; ctLoop_u32 < numBytes_u32; ctLoop_u32++)
    {
        *xDest_pu8 = *xSrc_pcu8;
        xDest_pu8++;
        xSrc_pcu8++;
    }
    return xDest_pv;
}


/* [$Satisfies $HME_FBL_DD_SBL 101] */
void SBL_FlashInit(FlashParamType* FlashParam)
{
    _arc_reset_stage3();

    /* ETAS BIP Porting: we still initialize Fls again since Fls driver need to access SPI, Fls
     * variables which only available after Fls_Init
     */
    if((IsInitializedType)UnInitialized == FlashParam->isInit)
    {
        /* Initialise fls */
        Fls_Init(&Fls_Config);

        FlashParam->isInit = Initialized;
        FlashParam->errorcode = FlashOk;
    }
}

/* [$Satisfies $HME_FBL_DD_SBL 104] */
void SBL_FlashDeinit(FlashParamType* FlashParam)
{
    if((IsInitializedType)Initialized == FlashParam->isInit)
    {
        /* just need to set UnInitialized status*/
        FlashParam->isInit = UnInitialized;
        FlashParam->errorcode = FlashOk;
    }
    else
    {
        FlashParam->errorcode = UnInitialized;
    }
}

/* [$Satisfies $HME_FBL_DD_SBL 102] */
void SBL_FlashErase(FlashParamType* FlashParam)
{
    uint32 ret = E_NOT_OK;

    if(FlashParam != NULL_PTR)
    {
        if((IsInitializedType)Initialized == FlashParam->isInit)
        {
            /*trigger wdg before executing the erase*/
            /*convert from cached address to non-cached address*/

            /* invoke the erase routine*/
            ret = Fls_Erase(FlashParam->address, FlashParam->length);
            if(E_OK == ret)
            {
                Fls_MainFunction();
                if(MEMIF_JOB_OK == Fls_GetJobResult())
                {
                    FlashParam->errorcode = FlashOk;
                }
                else
                {
                    FlashParam->errorcode = FlashFailed;
                }
            }
            else
            {
                FlashParam->errorcode = FlashInvalidParam;
            }
        }
        else
        {
            FlashParam->errorcode = UnInitialized;
        }
    }
}

/* [$Satisfies $HME_FBL_DD_SBL 103] */
void SBL_FlashWrite(FlashParamType* FlashParam)
{
    uint32 ret;
    AddressType writeAddress;
    LengthType writeLength;

    if(FlashParam != NULL_PTR)
    {
        if((IsInitializedType)Initialized == FlashParam->isInit)
        {
            if(FlashParam->data != NULL_PTR)
            {
                /*Trigger wdg*/
                /*Convert address to non-cached*/

                /*Copy data to aligned buffer*/
                writeAddress = FlashParam->address;
                writeLength = FlashParam->length;

                if((writeLength <= FLASH_DRIVER_MAX_OF_BYTE_TO_WRITE) &&
                   (writeLength % FLASH_DRIVER_MIN_OF_BYTE_TO_WRTIE == 0))
                {
                    _MemCopy((DataType*)SBL_WriteBuffer, (DataType*)FlashParam->data, writeLength);
                     /*Write data to Pflash*/
                    ret = Fls_Write(writeAddress, (const DataType *)SBL_WriteBuffer, writeLength);
                    if(E_OK == ret)
                    {
                        Fls_MainFunction();
                        if(MEMIF_JOB_OK == Fls_GetJobResult())
                        {
                            FlashParam->errorcode = FlashOk;
                        }
                        else
                        {
                            FlashParam->errorcode = FlashFailed;
                        }
                    }
                    else
                    {
                        FlashParam->errorcode = FlashInvalidParam;
                    }
                }
                else
                {
                    FlashParam->errorcode = FlashInvalidSize;
                }
            }
            else
            {
                FlashParam->errorcode = FlashInvalidParam;
            }
        }
        else
        {
            FlashParam->errorcode = UnInitialized;
        }
    }
    else
    {
        FlashParam->errorcode = FlashInvalidParam;
    }
}

/**
 * @brief ETAS BIP Porting
 */
void SBL_FlashRead(FlashParamType* FlashParam)
{
    uint32 ret;

    if(FlashParam != NULL_PTR)
    {
        if((IsInitializedType)Initialized == FlashParam->isInit)
        {
            if((FlashParam->address != NULL_PTR) && (FlashParam->data != NULL_PTR))
            {
                ret = Fls_Read(FlashParam->address, FlashParam->data, FlashParam->length); 
                if(E_OK == ret)
                {
                    Fls_MainFunction();
                    if(MEMIF_JOB_OK == Fls_GetJobResult())
                    {
                        FlashParam->errorcode = FlashOk;
                    }
                    else
                    {
                        FlashParam->errorcode = FlashFailed;
                    }
                }
                else
                {
                    FlashParam->errorcode = FlashInvalidParam;
                }
            }
            else
            {
                FlashParam->errorcode = FlashInvalidParam;
            }
        }
        else
        {
            FlashParam->errorcode = UnInitialized;
        }
    }
    else
    {
        FlashParam->errorcode = FlashInvalidParam;
    }
}
