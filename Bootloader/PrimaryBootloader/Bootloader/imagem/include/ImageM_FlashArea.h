/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_FLashArea.h
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2021. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
UCN1HC          2021/25/12      initial commit
**********************************************************************************************************************/

#ifndef  IMAGEM_FLASHAREA_H
#define  IMAGEM_FLASHAREA_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/

#include <Std_Types.h>
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
#define IMAGEM_IMAGE_NUM_SLOTS   2
#define IMAGEM_USE_EXTERNAL_FLASH    /* ETAS BIP Porting */

/**
 * @brief maping of flash area support multiple image mode
 * 
 * PRIMARY_0 <-> IMAGEM_FLASH_AREA_PRIMARY_0_IMAGE_0
 * PRIMARY_1 <-> IMAGEM_FLASH_AREA_PRIMARY_1_IMAGE_1
 * SECONDARY_0 <-> IMAGEM_FLASH_AREA_SECONDARY_0_IMAGE_2
 * SECONDARY_1 <-> IMAGEM_FLASH_AREA_sECONDARY_1_IMAGE_3
 */
#define IMAGEM_IMAGE_PRIMARY(x) ((x == 0u) ? \
                                IMAGEM_FLASH_AREA(PRIMARY_0_IMAGE_0)  : \
                                (x == 1u) ? \
                                IMAGEM_FLASH_AREA(PRIMARY_1_IMAGE_1)  : \
                                (x == 2u) ? \
                                 IMAGEM_FLASH_AREA(PRIMARY_2_IMAGE_2) : \
                                (x == 3u) ? \
                                 IMAGEM_FLASH_AREA(PRIMARY_3_IMAGE_3) : \
                                (x == 4u) ? \
                                 IMAGEM_FLASH_AREA(PRIMARY_4_IMAGE_4) : \
                                 (x == 5u) ? \
                                 IMAGEM_FLASH_AREA(PRIMARY_5_IMAGE_5) : \
                                 (x == 6u) ? \
                                 IMAGEM_FLASH_AREA(PRIMARY_6_IMAGE_6) : \
                                 (x == 7u) ? \
                                 IMAGEM_FLASH_AREA(PRIMARY_7_IMAGE_7) : \
                                 (x == 8u) ? \
                                 IMAGEM_FLASH_AREA(PRIMARY_8_IMAGE_8) : \
                                 (x == 9u) ? \
                                 IMAGEM_FLASH_AREA(PRIMARY_9_IMAGE_9) : \
                                 IMAGEM_FLASH_AREA_UNDEFINE)
                                 

#define IMAGEM_IMAGE_SECONDARY(x)   ((x == 0u) ? \
                                    IMAGEM_FLASH_AREA(SECONDARY_0_IMAGE_3): \
                                    (x == 1u) ? \
                                    IMAGEM_FLASH_AREA(SECONDARY_1_IMAGE_4): \
                                    (x == 2u) ? \
                                    IMAGEM_FLASH_AREA(SECONDARY_2_IMAGE_5): \
                                    IMAGEM_FLASH_AREA_UNDEFINE)


#define IMAGEM_FLASH_AREA(x)    IMAGEM_FLASH_AREA_##x

#define IMAGEM_FLASH_AREA_PRIMARY_0_IMAGE_0 1u
#define IMAGEM_FLASH_AREA_PRIMARY_1_IMAGE_1 2u
#define IMAGEM_FLASH_AREA_PRIMARY_2_IMAGE_2 3u
#define IMAGEM_FLASH_AREA_PRIMARY_3_IMAGE_3 4u
#define IMAGEM_FLASH_AREA_PRIMARY_4_IMAGE_4 5u
#define IMAGEM_FLASH_AREA_PRIMARY_5_IMAGE_5 6u
#define IMAGEM_FLASH_AREA_PRIMARY_6_IMAGE_6 7u
#define IMAGEM_FLASH_AREA_PRIMARY_7_IMAGE_7 8u
#define IMAGEM_FLASH_AREA_PRIMARY_8_IMAGE_8 9u
#define IMAGEM_FLASH_AREA_PRIMARY_9_IMAGE_9 10u

#define IMAGEM_FLASH_AREA_SECONDARY_0_IMAGE_3 11u
#define IMAGEM_FLASH_AREA_SECONDARY_1_IMAGE_4 12u
#define IMAGEM_FLASH_AREA_SECONDARY_2_IMAGE_5 13u


#define IMAGEM_FLASH_AREA_UNDEFINE 255u
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/

/**
 * @brief 
 * 
 */
typedef struct
{
    uint8 FaId;
    uint8 DeviceId;
    uint16 Pad;
    uint32 Address;
    uint32 Size;
}ImageM_FlashAreaType;

/**
 * @brief 
 * 
 */
typedef struct
{
    uint32 Offset;
    uint32 Size;
}ImageM_FlashSectorType;

/**
 * @brief 
 * 
 */
typedef enum
{
    IMAGEM_FLASH_DEVICE_INTERNAL_FLASH = 0x7F,
#ifdef IMAGEM_USE_EXTERNAL_FLASH
    IMAGEM_FLASH_DEVICE_EXTERNAL_FLASH = 0x80
#endif
}ImageM_DeviceIDType;

/**
 * @brief 
 * 
 */
typedef enum
{
    IMAGEM_PRIMARY_SLOT = 0,
    IMAGEM_SECONDARY_SLOT = 1,
#ifdef IMAGEM_SWAP_USING_SCRATCHPAD
    IMAGEM_SCRATCHPAD_SLOT = 2
#endif
}ImageM_SlotType;


/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

#endif /*IMAGEM_FLASHAREA_H*/
