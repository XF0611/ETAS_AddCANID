#===============================================
#     Linker Settings: ETAS_BIP_PT_ARCMW       #  
#===============================================

MEMORY
{
  /*================================ ICCM ===============================*/
  /*----------------  Instruction Close Coupled Memory ------------------*/
  ICCM                                  : ORIGIN = 0x00100000, LENGTH = 32K        /* ETAS BIP not used */
  
  /*================================ DCCM ===============================*/
  /*---------------------  Data Close Coupled Memory --------------------*/
  DCCM                                  : ORIGIN = 0x00A00000, LENGTH = 32K        /* ETAS BIP not used */

  /*=============================== SRAM ===============================*/
  /*---------------------------- Bootloader ----------------------------*/
  BOOT_CODE                             : ORIGIN = 0x00770000, LENGTH = 256K
  /*----------------------------- SBL Field -----------------------------*/
  SBL_CODE                              : ORIGIN = 0x007B0000, LENGTH = 32K
  /*-------------------------- Application Data -------------------------*/
  APPL_DATA                             : ORIGIN = 0x007B8000, LENGTH = 260K
  /*---------------------------- NVRAM Field ----------------------------*/
  APPL_NVRAM                            : ORIGIN = 0x007F9000, LENGTH = 4K
  /*------------------------- Calibration Field -------------------------*/
  Memlay_CALIB_EcucPartition_SysCore    : ORIGIN = 0x007FA000, LENGTH = 4K
  Memlay_CALIB_EcucPartition_ComCore    : ORIGIN = 0x007FB000, LENGTH = 4K
  Memlay_CALIB_EcucPartition_AppCore    : ORIGIN = 0x007FC000, LENGTH = 4K
  Memlay_CALIB_shared                   : ORIGIN = 0x007FD000, LENGTH = 4K
  /*------------------------- Stack Heap Field --------------------------*/
  APPL_HEAP                             : ORIGIN = 0x007FE000, LENGTH = 4K         /* ETAS BIP not used */
  APPL_STACK                            : ORIGIN = 0x007FF000, LENGTH = 4K

  /*======================= Baseband Shared Memory ======================*/
  /*-------------------------- Application Code -------------------------*/
  RADAR_SPD_RAM                         : ORIGIN = 0x00800000, LENGTH = 1024K    /* Radar Signal Processing Data - ETAS BIP not used */
  RADAR_AD_RAM                          : ORIGIN = 0x00900000, LENGTH = 1024K    /* Radar Algorithm Data - ETAS BIP not used */
}

ENTRY(FlashHeader)

SECTIONS
{
    GROUP : 
    {
        .rodata ALIGN(4): 
        {
            *(.sbl_const_flash_routine_header)
        }

        .text ALIGN(4): 
        {
            _f_text = .;
            *(TYPE text)
            _e_text = .;
        }

        .rodata ALIGN(4): 
        {
            _f_rodata = .;

            _fctors = .;
            *(.ctors*)
            _ectors = .;
            _fdtors = .;
            *(.dtors*)
            _edtors = .;
            _feh_frame = .;
            *(.eh_frame*)
            _eeh_frame = .;

            *(TYPE lit)

            _e_rodata = .;
        }
    } > SBL_CODE

    GROUP : 
    {
        .data ALIGN(4): 
        {
            _f_data = .;
            _f_sdata = .;
            *(.sdata)
            _e_sdata = .;
            *(TYPE data)
        }

        .tls ALIGN(8): 
        {
            *(.tls*)
            _e_data = .;
        }
    } > SBL_CODE

    GROUP (NOLOAD) : 
    {
        .bss ALIGN(8): 
        {
            _f_bss = .;
            *(TYPE bss)
            _e_bss = .;
        }
    } > SBL_CODE

    GROUP (NOLOAD) : 
    {
        .heap? ALIGN(4) SIZE(4096): {}
    } > APPL_HEAP

    GROUP (NOLOAD) : 
    {
        .stack ALIGN(4) SIZE(4096): {}
    } > APPL_STACK

    _f_stack = ADDR(.stack);
    _e_stack = ADDR(.stack) + SIZEOF(.stack);
    _f_heap = ADDR(.heap);
    _e_heap = ADDR(.heap) + SIZEOF(.heap);

    _load_addr_text = LOADADDR(.text);
    _load_addr_rodata = LOADADDR(.rodata);
    _load_addr_data = LOADADDR(.data);

    __SYSCORE_STACK_START = _e_stack;
    __SYSCORE_STACK_END   = _f_stack;
}
