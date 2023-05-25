/*********************************************************************************************************************
*
* Description:     
* FileName:        FBL_SeedAndKey.c
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


/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "FBL_SeedAndKey.h"
#include "Os_Port.h"

/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define FBL_KEY_SIZE 4U
#define FBL_SEED_SIZE 4U
/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
static uint8 securityAccessSeed[FBL_SEED_SIZE] = {0};



/**
 * @brief 
 * 
 * @param KeyLen 
 * @param KeyBuf 
 * @return Std_ReturnType 
 */
Std_ReturnType FBl_SeedAndKey_CompareKey(uint32 KeyLen, const uint8 *KeyBuf)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32 index;
    uint8 calKey[FBL_KEY_SIZE];
    if(FBL_KEY_SIZE == KeyLen)
    {
    /* Implement a basic seed and key algorithms*/
        for(index = 0 ; index < FBL_KEY_SIZE; index++)
        {
                calKey[index] = (securityAccessSeed[index] + 1U);
        }
                /*Compare with the tester's Key */

        for(index = 0; index < FBL_KEY_SIZE; index++)
        {
            if(calKey[index] == KeyBuf[index])
            {
                ret = E_OK;
            }
            else
            {
            /*Nothing todo - return E_NOT_OK */
                break;
            }
        }
    }
    else
    {
     /* Nothing todo - return E_NOT_OK */
    }
    return ret;

}
/**
 * @brief 
 * 
 * @param SeedLen 
 * @param Seed 
 * @return Std_ReturnType 
 */
Std_ReturnType Fbl_SeedAndKey_GetSeed(uint32 SeedLen, uint8* Seed)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32 randvalue = GET_SYSTEM_TIMER(); /* PRQA S 0303 */ /* Read the value at Register */
    uint32 index;
      /*Get seed value*/
    if(FBL_SEED_SIZE == SeedLen)
    {
        for(index = 0; index < SeedLen; index++)
        {
            securityAccessSeed[index]= (uint8)((randvalue >> (index * 8U))) & 0xFFU;
            Seed[index]= securityAccessSeed[index];
        }
        ret = E_OK;
    }
    else
    {
    /*nothing todo - return E_NOT_OK */
    }
    return ret;
}
