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
  BOOT_CODE                             : ORIGIN = 0x00770000, LENGTH = 548K
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

    //ETAS BIP Porting TODO: Limitation, during test, Memlay_CALIB_EcucPartition_SysCore need to be moved into ETAS_BIP_PT_ARCMW.ld
    GROUP SIZE(SIZEOF(Memlay_CALIB_EcucPartition_SysCore)) : 
    {
        .EcucPartition_SysCore_CALIB ALIGN(4):
        {
            __EcucPartition_SysCore_CALIB_START = .;

            *(.EcucPartition_SysCore_ASW_CALIB_16)
            *(.EcucPartition_SysCore_ASW_CALIB_32)
            *(.EcucPartition_SysCore_ASW_CALIB_8)
            *(.EcucPartition_SysCore_ASW_CALIB_BOOLEAN)
            *(.EcucPartition_SysCore_ASW_CALIB_UNSPECIFIED)

            __EcucPartition_SysCore_CALIB_END = .;
        }

        .EcucPartition_SysCore_CALIB_FAST ALIGN(4):
        {
            __EcucPartition_SysCore_CALIB_FAST_START = .;

            *(.EcucPartition_SysCore_ASW_CALIB_FAST_16)
            *(.EcucPartition_SysCore_ASW_CALIB_FAST_32)
            *(.EcucPartition_SysCore_ASW_CALIB_FAST_8)
            *(.EcucPartition_SysCore_ASW_CALIB_FAST_BOOLEAN)
            *(.EcucPartition_SysCore_ASW_CALIB_FAST_UNSPECIFIED)

            __EcucPartition_SysCore_CALIB_FAST_END = .;
        }

        .EcucPartition_SysCore_CALIB_SLOW ALIGN(4):
        {
            __EcucPartition_SysCore_CALIB_SLOW_START = .;

            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_16)
            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_32)
            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_8)
            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_BOOLEAN)
            *(.EcucPartition_SysCore_ASW_CALIB_SLOW_UNSPECIFIED)

            __EcucPartition_SysCore_CALIB_SLOW_END = .;
        }

        .EcucPartition_SysCore_CALPRM ALIGN(4):
        {
            __EcucPartition_SysCore_CALPRM_START = .;

            *(.EcucPartition_SysCore_ASW_CALPRM_16)
            *(.EcucPartition_SysCore_ASW_CALPRM_32)
            *(.EcucPartition_SysCore_ASW_CALPRM_8)
            *(.EcucPartition_SysCore_ASW_CALPRM_BOOLEAN)
            *(.EcucPartition_SysCore_ASW_CALPRM_UNSPECIFIED)

            __EcucPartition_SysCore_CALPRM_END = .;
        }
    } = FILL(0x00) > Memlay_CALIB_EcucPartition_SysCore
    //ETAS BIP Porting TODO: Limitation, during test, Memlay_CALIB_EcucPartition_ComCore need to be moved into ETAS_BIP_PT_ARCMW.ld
    GROUP SIZE(SIZEOF(Memlay_CALIB_EcucPartition_ComCore)) : 
    {
        .EcucPartition_ComCore_CALIB ALIGN(4):
        {
            __EcucPartition_ComCore_CALIB_START = .;

            *(.EcucPartition_ComCore_ASW_CALIB_16)
            *(.EcucPartition_ComCore_ASW_CALIB_32)
            *(.EcucPartition_ComCore_ASW_CALIB_8)
            *(.EcucPartition_ComCore_ASW_CALIB_BOOLEAN)
            *(.EcucPartition_ComCore_ASW_CALIB_UNSPECIFIED)

            __EcucPartition_ComCore_CALIB_END = .;
        }

        .EcucPartition_ComCore_CALIB_FAST ALIGN(4):
        {
            __EcucPartition_ComCore_CALIB_FAST_START = .;

            *(.EcucPartition_ComCore_ASW_CALIB_FAST_16)
            *(.EcucPartition_ComCore_ASW_CALIB_FAST_32)
            *(.EcucPartition_ComCore_ASW_CALIB_FAST_8)
            *(.EcucPartition_ComCore_ASW_CALIB_FAST_BOOLEAN)
            *(.EcucPartition_ComCore_ASW_CALIB_FAST_UNSPECIFIED)

            __EcucPartition_ComCore_CALIB_FAST_END = .;
        }

        .EcucPartition_ComCore_CALIB_SLOW ALIGN(4):
        {
            __EcucPartition_ComCore_CALIB_SLOW_START = .;

            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_16)
            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_32)
            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_8)
            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_BOOLEAN)
            *(.EcucPartition_ComCore_ASW_CALIB_SLOW_UNSPECIFIED)

            __EcucPartition_ComCore_CALIB_SLOW_END = .;
        }

        .EcucPartition_ComCore_CALPRM ALIGN(4):
        {
            __EcucPartition_ComCore_CALPRM_START = .;

            *(.EcucPartition_ComCore_ASW_CALPRM_16)
            *(.EcucPartition_ComCore_ASW_CALPRM_32)
            *(.EcucPartition_ComCore_ASW_CALPRM_8)
            *(.EcucPartition_ComCore_ASW_CALPRM_BOOLEAN)
            *(.EcucPartition_ComCore_ASW_CALPRM_UNSPECIFIED)

            __EcucPartition_ComCore_CALPRM_END = .;
        }
    } = FILL(0x00) > Memlay_CALIB_EcucPartition_ComCore
    //ETAS BIP Porting TODO: Limitation, during test, Memlay_CALIB_EcucPartition_AppCore need to be moved into ETAS_BIP_PT_ARCMW.ld
    GROUP SIZE(SIZEOF(Memlay_CALIB_EcucPartition_AppCore)) : 
    {
        .EcucPartition_AppCore_CALIB ALIGN(4):
        {
            __EcucPartition_AppCore_CALIB_START = .;

            *(.EcucPartition_AppCore_ASW_CALIB_16)
            *(.EcucPartition_AppCore_ASW_CALIB_32)
            *(.EcucPartition_AppCore_ASW_CALIB_8)
            *(.EcucPartition_AppCore_ASW_CALIB_BOOLEAN)
            *(.EcucPartition_AppCore_ASW_CALIB_UNSPECIFIED)

            __EcucPartition_AppCore_CALIB_END = .;
        }

        .EcucPartition_AppCore_CALIB_FAST ALIGN(4):
        {
            __EcucPartition_AppCore_CALIB_FAST_START = .;

            *(.EcucPartition_AppCore_ASW_CALIB_FAST_16)
            *(.EcucPartition_AppCore_ASW_CALIB_FAST_32)
            *(.EcucPartition_AppCore_ASW_CALIB_FAST_8)
            *(.EcucPartition_AppCore_ASW_CALIB_FAST_BOOLEAN)
            *(.EcucPartition_AppCore_ASW_CALIB_FAST_UNSPECIFIED)

            __EcucPartition_AppCore_CALIB_FAST_END = .;
        }

        .EcucPartition_AppCore_CALIB_SLOW ALIGN(4):
        {
            __EcucPartition_AppCore_CALIB_SLOW_START = .;

            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_16)
            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_32)
            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_8)
            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_BOOLEAN)
            *(.EcucPartition_AppCore_ASW_CALIB_SLOW_UNSPECIFIED)

            __EcucPartition_AppCore_CALIB_SLOW_END = .;
        }

        .EcucPartition_AppCore_CALPRM ALIGN(4):
        {
            __EcucPartition_AppCore_CALPRM_START = .;

            *(.EcucPartition_AppCore_ASW_CALPRM_16)
            *(.EcucPartition_AppCore_ASW_CALPRM_32)
            *(.EcucPartition_AppCore_ASW_CALPRM_8)
            *(.EcucPartition_AppCore_ASW_CALPRM_BOOLEAN)
            *(.EcucPartition_AppCore_ASW_CALPRM_UNSPECIFIED)
            *(.EcucPartition_AppCore_RTE_CALPRM_32)
            *(.shared_RTE_CALPRM_32)

            __EcucPartition_AppCore_CALPRM_END = .;
        }
    } = FILL(0x00) > Memlay_CALIB_EcucPartition_AppCore
    //ETAS BIP Porting TODO: Limitation, during test, Memlay_CALIB_shared need to be moved into ETAS_BIP_PT_ARCMW.ld
    .shared_CALPRM ALIGN(4):
    {
        __shared_CALPRM_START = .;

        *(.shared_RTE_CALPRM_32)

        __shared_CALPRM_END = .;
    } > Memlay_CALIB_shared

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

    _load_addr_text       = LOADADDR(.text);
    _load_addr_rodata     = LOADADDR(.rodata);
    _load_addr_data       = LOADADDR(.data);

    __SYSCORE_STACK_START = _f_stack;
    __SYSCORE_STACK_END   = _e_stack;
}

#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_EcucPartition_SysCore.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_EcucPartition_ComCore.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_EcucPartition_AppCore.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_OS.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_shared.ld"
#include "./BasicSoftware/integration/linke/ARCMW_CAL60S244AB_MCAL.ld"
