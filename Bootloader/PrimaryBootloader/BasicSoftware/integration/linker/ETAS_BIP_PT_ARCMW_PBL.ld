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
  BOOT_CODE                             : ORIGIN = 0x00770000, LENGTH = 548K

  /*------------------------- Stack Heap Field --------------------------*/
  

  /*======================= Baseband Shared Memory ======================*/
  /*-------------------------- Application Code -------------------------*/
  RADAR_SPD_RAM                         : ORIGIN = 0x00800000, LENGTH = 1024K    /* Radar Signal Processing Data - ETAS BIP not used */
  RADAR_AD_RAM                          : ORIGIN = 0x00900000, LENGTH = 1024K    /* Radar Algorithm Data - ETAS BIP not used */
}

ENTRY(_arc_start)

SECTIONS
{
    GROUP : 
    {
        .init_bootstrap:
        {
            *(.init_vector)
            *(.init_bootstrap)
        }

        .vector ALIGN(1024): 
        {
            _f_vector = .;
            *(.vector)
            _e_vector = .;
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
    } > BOOT_CODE

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
    } > BOOT_CODE AT > BOOT_CODE

    GROUP (NOLOAD) : 
    {
        .bss ALIGN(8): 
        {
            _f_bss = .;
            *(TYPE bss)
            _e_bss = .;
        }
    } > BOOT_CODE

    GROUP (NOLOAD) : 
    {
        .stack ALIGN(4) SIZE(4096): {}
    } > BOOT_CODE

    GROUP (NOLOAD) : 
    {
        .heap? ALIGN(4) SIZE(4): {}
    } > BOOT_CODE

    _f_stack = ADDR(.stack);
    _e_stack = ADDR(.stack) + SIZEOF(.stack);
    _f_heap  = ADDR(.heap);
    _e_heap  = ADDR(.heap) + SIZEOF(.heap);

    _load_addr_text   = LOADADDR(.text);
    _load_addr_rodata = LOADADDR(.rodata);
    _load_addr_data   = LOADADDR(.data);

    __SYSCORE_STACK_START = _e_stack;
    __SYSCORE_STACK_END   = _f_stack;
}
