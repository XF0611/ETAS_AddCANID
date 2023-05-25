/* *****************************************************************************
 * BEGIN: Banner
 *-----------------------------------------------------------------------------
 *                                 ETAS GmbH
 *                      D-70469 Stuttgart, Borsigstr. 14
 *-----------------------------------------------------------------------------
 *    Administrative Information (automatically filled in by ISOLAR)         
 *-----------------------------------------------------------------------------
 * Project : ETAS_BIP_PT
 * Component:  NVM_SWC
 * Description: Testcode for ASW_NVM
 * Version         Author        Date               Update information
 * 1.0             IHP1HC        27-Jul-2021        Create software
 *-----------------------------------------------------------------------------
 * END: Banner
 ******************************************************************************/

#ifndef NVMUT_H
#define NVMUT_H 


#define CYCLIC_BLOCK_LENGHT      1024
#define IMMEDIATE_BLOCK_LENGHT      384
#define SHUTDOWN_BLOCK_LENGHT      96

#define IMMEDIATE_BLOCK_SIZE      IMMEDIATE_BLOCK_LENGHT/4

typedef enum NvM_Test_t{
    SWC_ZERO = 0,
    ENABLE_WRITE_BLOCK,
    ENABLE_WRITE_CYCLIC,
    ENABLE_WRITE_IMMEDIATE,
    ENABLE_WRITE_SHUTDOWN,      
    FILL_SECTOR_WITH_FULL_DATA_SR,
    ENABLE_READ_BLOCK,
    ENABLE_WRITE_CYCLIC_D,
	ENABLE_FAST_WRITE,
    FILL_SECTOR_WITH_FULL_DATA_HR,
    IMMEDIATE_WRITNG_TMON
}NvM_Test_t;

typedef enum Req_reset_t{
    RESET_NONE,
    RESET_GET_INFO,
    RESET_REQUEST
}Req_reset_t;

#endif /* NVMUT_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */