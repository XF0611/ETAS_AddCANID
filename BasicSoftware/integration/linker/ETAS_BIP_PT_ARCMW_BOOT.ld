#===============================================
#     Linker Settings: ETAS_BIP_PT_ARCMW       #  
#===============================================

MEMORY
{
  /*================================ ICCM ===============================*/
  /*----------------  Instruction Close Coupled Memory ------------------*/
  ICCM                                  : ORIGIN = 0x00100000, LENGTH = 32K      /* ETAS BIP not used */
  
  /*================================ DCCM ===============================*/
  /*---------------------  Data Close Coupled Memory --------------------*/
  DCCM                                  : ORIGIN = 0x00A00000, LENGTH = 32K      /* ETAS BIP not used */

  /*=============================== SRAM ================================*/
  /*---------------------------- Bootloader -----------------------------*/
  EXT_ROM_CAL                           : ORIGIN = 0x0030C000, LENGTH = 16K
  EXT_ROM                               : ORIGIN = 0x00380000, LENGTH = 3584K
  /*----------------------------- SBL Field -----------------------------*/

  /*-------------------------- Application Data -------------------------*/
  RAM_XIP                               : ORIGIN = 0x00770000, LENGTH = 128K
  RAM                                   : ORIGIN = 0x00790000, LENGTH = 420K
  /*---------------------------- NVRAM Field ----------------------------*/
  
  /*------------------------- Calibration Field -------------------------*/
  Memlay_CALIB_EcucPartition_SysCore    : ORIGIN = 0x007FA000, LENGTH = 4K
  Memlay_CALIB_EcucPartition_ComCore    : ORIGIN = 0x007FB000, LENGTH = 4K
  Memlay_CALIB_EcucPartition_AppCore    : ORIGIN = 0x007FC000, LENGTH = 4K
  Memlay_CALIB_shared                   : ORIGIN = 0x007FD000, LENGTH = 4K
  /*------------------------- Stack Heap Field --------------------------*/
  APPL_HEAP                             : ORIGIN = 0x007FE000, LENGTH = 4K       /* ETAS BIP not used */
  APPL_STACK                            : ORIGIN = 0x007FF000, LENGTH = 4K

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
    } > EXT_ROM

    .text_xip ALIGN(4) : {
        _f_text_xip = .;
        *alps_timer.o (.text .text.* .rodata)
        *arc_timer.o (.text .text.* .rodata)
        *arc_exception.o (.text .text.* .rodata)
        *Mcu_Alps.o (.text .text.* .rodata)
        *Mcu.o (.text .text.* .rodata)
        *Port_Alps.o (.text .text.* .rodata)
        *Port.o (.text .text.* .rodata)
        *Spi_Alps.o (.text .text.* .rodata)
        *Spi.o (.text .text.* .rodata)
        *Fls_Alps.o (.text .text.* .rodata)
        *Fls.o (.text .text.* .rodata)
        *system_clock_init.o (.text .text.* .rodata)
        *Fls_PBCfg.o (.text .text.* .rodata)
        *Mcu_PBCfg.o (.text .text.* .rodata)
        *Port_PBcfg.o (.text .text.* .rodata)
        *Spi_PBCfg.o (.text .text.* .rodata)
        _e_text_xip = .;
    } > RAM_XIP AT > EXT_ROM

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
    } > RAM AT > EXT_ROM

    GROUP (NOLOAD) : 
    {
        .bss ALIGN(8): 
        {
            _f_bss = .;
            *(TYPE bss)
            _e_bss = .;
        }
    } > RAM

    //ETAS BIP Porting TODO: Limitation, during test, Memlay_CALIB_EcucPartition_SysCore need to be moved into ETAS_BIP_PT_ARCMW.ld
    GROUP :
    {
        .calib_xip_SysCore ALIGN(4) :
        {
            _f_calib_xip_SysCore = .;

            __EcucPartition_SysCore_CALIB_START = .;
            *(.EcucPartition_SysCore_ASW_CALIB_16)
            *(.EcucPartition_SysCore_ASW_CALIB_32)
            *(.EcucPartition_SysCore_ASW_CALIB_8)
            *(.EcucPartition_SysCore_ASW_CALIB_BOOLEAN)
            *(.EcucPartition_SysCore_ASW_CALIB_UNSPECIFIED)
            __EcucPartition_SysCore_CALIB_END = .;

            __EcucPartition_SysCore_CALIB_FAST_START = .;
            *(.EcucPartition_SysCore_ASW_CALIB_FAST_16)
            *(.EcucPartition_SysCore_ASW_CALIB_FAST_32)
            *(.EcucPartition_SysCore_ASW_CALIB_FAST_8)
            *(.EcucPartition_SysCore_ASW_CALIB_FAST_BOOLEAN)
            *(.EcucPartition_SysCore_ASW_CALIB_FAST_UNSPECIFIED)
            __EcucPartition_SysCore_CALIB_FAST_END = .;

            __EcucPartition_SysCore_CALIB_SLOW_START = .;
            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_16)
            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_32)
            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_8)
            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_BOOLEAN)
            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_UNSPECIFIED)
            __EcucPartition_SysCore_CALIB_SLOW_END = .;

            __EcucPartition_SysCore_CALPRM_START = .;
            *(.EcucPartition_SysCore_ASW_CALPRM_16)
            *(.EcucPartition_SysCore_ASW_CALPRM_32)
            *(.EcucPartition_SysCore_ASW_CALPRM_8)
            *(.EcucPartition_SysCore_ASW_CALPRM_BOOLEAN)
            *(.EcucPartition_SysCore_ASW_CALPRM_UNSPECIFIED)
            __EcucPartition_SysCore_CALPRM_END = .;

            _e_calib_xip_SysCore = .;
        }
    } > Memlay_CALIB_EcucPartition_SysCore AT > EXT_ROM_CAL

    GROUP:
    {
        .calib_xip_ComCore ALIGN(4) :
        {
            _f_calib_xip_ComCore = .;

            __EcucPartition_ComCore_CALIB_START = .;
            *(.EcucPartition_ComCore_ASW_CALIB_16)
            *(.EcucPartition_ComCore_ASW_CALIB_32)
            *(.EcucPartition_ComCore_ASW_CALIB_8)
            *(.EcucPartition_ComCore_ASW_CALIB_BOOLEAN)
            *(.EcucPartition_ComCore_ASW_CALIB_UNSPECIFIED)
            __EcucPartition_ComCore_CALIB_END = .;

            __EcucPartition_ComCore_CALIB_FAST_START = .;
            *(.EcucPartition_ComCore_ASW_CALIB_FAST_16)
            *(.EcucPartition_ComCore_ASW_CALIB_FAST_32)
            *(.EcucPartition_ComCore_ASW_CALIB_FAST_8)
            *(.EcucPartition_ComCore_ASW_CALIB_FAST_BOOLEAN)
            *(.EcucPartition_ComCore_ASW_CALIB_FAST_UNSPECIFIED)
            __EcucPartition_ComCore_CALIB_FAST_END = .;

            __EcucPartition_ComCore_CALIB_SLOW_START = .;
            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_16)
            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_32)
            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_8)
            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_BOOLEAN)
            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_UNSPECIFIED)
            __EcucPartition_ComCore_CALIB_SLOW_END = .;

            __EcucPartition_ComCore_CALPRM_START = .;
            *(.EcucPartition_ComCore_ASW_CALPRM_16)
            *(.EcucPartition_ComCore_ASW_CALPRM_32)
            *(.EcucPartition_ComCore_ASW_CALPRM_8)
            *(.EcucPartition_ComCore_ASW_CALPRM_BOOLEAN)
            *(.EcucPartition_ComCore_ASW_CALPRM_UNSPECIFIED)
            __EcucPartition_ComCore_CALPRM_END = .;

            _e_calib_xip_ComCore = .;
        }
    } > Memlay_CALIB_EcucPartition_ComCore AT > EXT_ROM_CAL

    //ETAS BIP Porting TODO: Limitation, during test, Memlay_CALIB_EcucPartition_AppCore need to be moved into ETAS_BIP_PT_ARCMW.ld
    GROUP:
    {
        .calib_xip_AppCore ALIGN(4) :
        {
            _f_calib_xip_AppCore = .;
            
            __EcucPartition_AppCore_CALIB_START = .;
            *(.EcucPartition_AppCore_ASW_CALIB_16)
            *(.EcucPartition_AppCore_ASW_CALIB_32)
            *(.EcucPartition_AppCore_ASW_CALIB_8)
            *(.EcucPartition_AppCore_ASW_CALIB_BOOLEAN)
            *(.EcucPartition_AppCore_ASW_CALIB_UNSPECIFIED)
            __EcucPartition_AppCore_CALIB_END = .;

            __EcucPartition_AppCore_CALIB_FAST_START = .;
            *(.EcucPartition_AppCore_ASW_CALIB_FAST_16)
            *(.EcucPartition_AppCore_ASW_CALIB_FAST_32)
            *(.EcucPartition_AppCore_ASW_CALIB_FAST_8)
            *(.EcucPartition_AppCore_ASW_CALIB_FAST_BOOLEAN)
            *(.EcucPartition_AppCore_ASW_CALIB_FAST_UNSPECIFIED)
            __EcucPartition_AppCore_CALIB_FAST_END = .;

            __EcucPartition_AppCore_CALIB_SLOW_START = .;
            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_16)
            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_32)
            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_8)
            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_BOOLEAN)
            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_UNSPECIFIED)
            __EcucPartition_AppCore_CALIB_SLOW_END = .;

            __EcucPartition_AppCore_CALPRM_START = .;
            *(.EcucPartition_AppCore_ASW_CALPRM_16)
            *(.EcucPartition_AppCore_ASW_CALPRM_32)
            *(.EcucPartition_AppCore_ASW_CALPRM_8)
            *(.EcucPartition_AppCore_ASW_CALPRM_BOOLEAN)
            *(.EcucPartition_AppCore_ASW_CALPRM_UNSPECIFIED)
            *(.EcucPartition_AppCore_RTE_CALPRM_32)
            *(.shared_RTE_CALPRM_32)
            __EcucPartition_AppCore_CALPRM_END = .;

            _e_calib_xip_AppCore = .;
        } 
    } > Memlay_CALIB_EcucPartition_AppCore AT > EXT_ROM_CAL

    //ETAS BIP Porting TODO: Limitation, during test, Memlay_CALIB_shared need to be moved into ETAS_BIP_PT_ARCMW.ld
    GROUP:
    {
        .calib_xip_Shared ALIGN(4) :
        {
            _f_calib_xip_Shared = .;

            __shared_CALPRM_START = .;
            *(.shared_RTE_CALPRM_32)
            __shared_CALPRM_END = .;

            _e_calib_xip_Shared = .;
        } 
    } > Memlay_CALIB_shared AT > EXT_ROM_CAL    

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
    _f_heap  = ADDR(.heap);
    _e_heap  = ADDR(.heap) + SIZEOF(.heap);

    _load_addr_text               = LOADADDR(.text);
    _load_addr_rodata             = LOADADDR(.rodata);
    _load_addr_data               = LOADADDR(.data);
    _load_addr_text_xip           = LOADADDR(.text_xip);
    _load_addr_calib_xip_SysCore  = LOADADDR(.calib_xip_SysCore);
    _load_addr_calib_xip_ComCore  = LOADADDR(.calib_xip_ComCore);
    _load_addr_calib_xip_AppCore  = LOADADDR(.calib_xip_AppCore);
    _load_addr_calib_xip_Shared   = LOADADDR(.calib_xip_Shared);    

    __SYSCORE_STACK_START = _f_stack;
    __SYSCORE_STACK_END   = _e_stack;
}

/* #include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_EcucPartition_SysCore.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_EcucPartition_ComCore.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_EcucPartition_AppCore.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_OS.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_shared.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_MCAL.ld" */
