/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */

/**
 * \file
 * \ingroup ARC_HAL_STARTUP
 * \brief assembly part of startup process
 */

/**
 * \addtogroup ARC_HAL_STARTUP
 * @{
 */
/** @cond STARTUP_ASM */

#define __ASSEMBLY__

#include "arc.h"
#include "arc_feature_config.h"

	.file "arc_startup.S"

.weak	_f_sdata		/* start of small data, defined in link script */
.weak	init_hardware_hook	/* app hardware init hook */
.weak	init_software_hook	/* app software init hook */

.extern	main
.extern exc_entry_table

/* initial vector table */
	.section .init_vector, "a"
	.long _arc_reset
	.section .init_bootstrap, "ax"
	.global _arc_reset
	.global _arc_start
	.align 4
_arc_start:
_arc_reset:
_arc_reset_stage1:
	kflag	STATUS32_RESET_VALUE

/* STAGE 1 */

/* necessary hardware should be done first to speed up initialization
	1. system clk
	2. mem controller must be initialized before any access to external
	mem.
	3. others
*/
_arc_cache_init_start:
	lr	r0, [AUX_BCR_D_CACHE]
	cmp	r0, 2
	/* invalidate dcache */
	jle	_arc_icache_init
	mov	r0, 1
	sr	r0, [AUX_DC_IVDC]
	sr	r0, [AUX_DC_CTRL]
_arc_icache_init:
	lr	r0, [AUX_BCR_I_CACHE]
	cmp	r0, 2
	jle	_arc_cache_init_end
	/* invalidate icache */
	mov	r0, 1
	sr	r0, [AUX_IC_IVIC]
	nop_s
	nop_s
	nop_s
	sr	r0, [AUX_IC_CTRL]

_arc_cache_init_end:
	mov	r0, init_hardware_hook
	cmp	r0, 0
	jlne	[r0]

/* STAGE 2: init necessary registers */

_arc_reset_stage2:
	mov	r0, 0

/* interrupt related init */
	sr	r0, [AUX_IRQ_ACT]
	sr	r0, [AUX_IRQ_CTRL]
	sr	r0, [AUX_IRQ_HINT]

/* use the new vector table to replace the old one */
#if defined(ARC_FEATURE_SEC_PRESENT) && (SECURESHIELD_VERSION < 2)
	sr	exc_entry_table, [AUX_INT_VECT_BASE_S]
#else
	sr	exc_entry_table, [AUX_INT_VECT_BASE]
#endif

/* init stack */
#if ARC_FEATURE_RGF_BANKED_REGS >= 16 && ARC_FEATURE_RGF_BANKED_REGS > 1 && ARC_FEATURE_FIRQ == 1
#if _STACKSIZE < 512
#error "not enough stack size for irq and firq"
#endif

/* switch to register bank1 */
	lr      r0, [AUX_STATUS32]
	bic     r0, r0, 0x70000          //clear AUX_STATUS32.RB[2:0] &= 0b000
	or      r0, r0, 0x10000          //set   AUX_STATUS32.RB[2:0] |= 0b001
	kflag   r0                       //Write to Status Register in kernel mode
/* set sp, gp, fp in bank1 */
	mov     sp, _e_stack
	mov     gp, _f_sdata
	mov     fp, 0
/* come back to bank0 */
	lr      r0, [AUX_STATUS32]
	bic     r0, r0, 0x70000          //clear AUX_STATUS32.RB[2:0] &= 0b000
	kflag   r0                       //Write to Status Register in kernel mode
	mov	sp, _e_stack-256
#else
	mov	sp, _e_stack	/* init stack pointer */
#endif
	mov	gp, _f_sdata	/* init small-data base register */
	mov	fp, 0		/* init fp register */

_arc_reset_stage3:
_s3_copy_text:
	mov	r0, _f_text
	mov	r1, _load_addr_text
	cmp	r0, r1

/* if load addr == run addr, no need to copy */
#if (_LOAD_XIP_TEXT != 0)
	jeq	_s3_copy_fls_xip_text
#else
	jeq	_s3_copy_rodata
#endif
	mov	r3, _e_text

_s3_copy_text_loop:
	ld.ab	r2, [r1, 4]
	st.ab	r2, [r0, 4]
	cmp	r0, r3
	jlt	_s3_copy_text_loop

#if (_LOAD_XIP_TEXT != 0)
_s3_copy_fls_xip_text:
	mov r0, _f_text_xip
	mov r1, _load_addr_text_xip
	cmp r0, r1
	jeq _s3_copy_rodata
	mov	r3, _e_text_xip
_s3_fls_xip_text_loop:
	ld.ab r2, [r1, 4]
	st.ab r2, [r0, 4]
	cmp r0, r3
	jlt _s3_fls_xip_text_loop

	mov r0, _f_calib_xip_SysCore
	mov r1, _load_addr_calib_xip_SysCore
	cmp r0, r1
	jeq _s3_copy_rodata
	mov	r3, _e_calib_xip_SysCore
_s3_fls_xip_calib_SysCore_loop:
	ld.ab r2, [r1, 4]
	st.ab r2, [r0, 4]
	cmp r0, r3
	jlt _s3_fls_xip_calib_SysCore_loop

	mov r0, _f_calib_xip_ComCore
	mov r1, _load_addr_calib_xip_ComCore
	cmp r0, r1
	jeq _s3_copy_rodata
	mov	r3, _e_calib_xip_ComCore
_s3_fls_xip_calib_ComCore_loop:
	ld.ab r2, [r1, 4]
	st.ab r2, [r0, 4]
	cmp r0, r3
	jlt _s3_fls_xip_calib_ComCore_loop

	mov r0, _f_calib_xip_AppCore
	mov r1, _load_addr_calib_xip_AppCore
	cmp r0, r1
	jeq _s3_copy_rodata
	mov	r3, _e_calib_xip_AppCore
_s3_fls_xip_calib_AppCore_loop:
	ld.ab r2, [r1, 4]
	st.ab r2, [r0, 4]
	cmp r0, r3
	jlt _s3_fls_xip_calib_AppCore_loop

	mov r0, _f_calib_xip_Shared
	mov r1, _load_addr_calib_xip_Shared
	cmp r0, r1
	jeq _s3_copy_rodata
	mov	r3, _e_calib_xip_Shared
_s3_fls_xip_calib_Shared_loop:
	ld.ab r2, [r1, 4]
	st.ab r2, [r0, 4]
	cmp r0, r3
	jlt _s3_fls_xip_calib_Shared_loop		
#endif

_s3_copy_rodata:
	mov	r0, _f_rodata
	mov	r1, _load_addr_rodata
	cmp	r0, r1

/* if load addr == run addr, no need to copy */
	jeq	_s3_copy_data
	mov	r3, _e_rodata
_s3_copy_rodata_loop:
	ld.ab	r2, [r1, 4]
	st.ab	r2, [r0, 4]
	cmp	r0, r3
	jlt	_s3_copy_rodata_loop

_s3_copy_data:
	mov	r0, _f_data
	mov	r1, _load_addr_data
	cmp	r0, r1
	jeq	_s3_clear_bss

/* if load addr == run addr, no need to copy */
	mov	r3, _e_data
_s3_copy_data_loop:
	ld.ab	r2, [r1, 4]
	st.ab	r2, [r0, 4]
	cmp	r0, r3
	jlt	_s3_copy_data_loop
_s3_clear_bss:
	mov	r0, _f_bss
	mov	r1, _e_bss
	cmp	r0, r1
	/* jge	_arc_reset_call_main */
	jge	_com_stack_init
	mov	r2, 0
_s3_clear_bss_loop:
	st.ab	r2, [r0, 4]
	cmp	r0, r1
	jlt	_s3_clear_bss_loop


/* STAGE 3: go to main */
_com_stack_init:
#ifdef COMMON_STACK_INIT
	mov	r0, _f_stack
	mov	r1, _e_stack
	cmp	r0, r1
	jge	_arc_reset_call_main
	mov	r2, 0
_com_stack_init_loop:
	st.ab	r2, [r0, 4]
	cmp	r0, r1
	jlt	_com_stack_init_loop
#endif

_arc_reset_call_main:


_arc_d_cache_init_start:
	/*check D-cache valid or not*/
	lr	r0, [AUX_DC_CTRL]
	bbit1 r0, 0, _arc_d_cache_init_end
	/*D-cache flush*/
	mov r0, 1
	sr	r0, [AUX_DC_FLSH]
_arc_d_cache_flush:
	lr	r0, [AUX_DC_CTRL]
	bbit1 r0, 8, _arc_d_cache_flush
_arc_d_cache_init_end:

_arc_i_cache_init_start:
	/*check I-cache valid or not*/
	lr	r0, [AUX_IC_CTRL]
	bbit1 r0, 0, _arc_i_cache_init_end

	/*I-cache invalide*/
	mov r0, 1
	sr	r0, [AUX_IC_IVIC]
	nop
	nop
	nop
_arc_i_cache_init_end:

	mov	r0, init_software_hook
	cmp	r0, 0
	jlne	[r0]

/* early init of interrupt and exception */
	jl	exc_int_init
	/* init cache */
	jl	arc_cache_init

#if defined(__MW__)
	jl	_init
#elif defined(__GNU__)
	jl	__do_global_ctors_aux
	jl	__do_init_array_aux
#endif
	jl	main	/* board-level main */

#if defined(__MW__)
	jl	_fini
#elif defined(__GNU__)
	jl	__do_global_dtors_aux
#endif

	.global _exit_loop
	.global _arc_exit_halt
	.align 4
_arc_exit_halt:
_exit_loop:
	flag	AUX_STATUS_MASK_HALT
	nop
	nop
	nop
	b	_exit_loop
/** @endcond */

/** }@*/
