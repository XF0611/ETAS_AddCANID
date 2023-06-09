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
 * \ingroup ARC_HAL_EXCEPTION_CPU
 * \brief  assembly part of exception and interrupt processing
 */

/**
 * \addtogroup ARC_HAL_EXCEPTION_CPU
 * @{
 */

/* function documentation */
/**
 * \fn void exc_entry_cpu(void)
 * \brief default entry of CPU exceptions, such as TLB miss and swap.
 *
 * \fn void exc_entry_int(void)
 * \brief normal interrupt exception entry.
 * 	In default, all interrupt exceptions are installed with normal entry.
 * 	If FIRQ is required, exc_entry_firq should be the entry.
 *
 * \fn void exc_entry_firq(void)
 * \brief firq exception entry
 */
/** }@ */

/** @cond EXCEPTION_ASM */

#define __ASSEMBLY__

#include "arc.h"
#include "arc_asm_common.h"

	.file "arc_exc_asm.S"

/* entry for cpu exception handling */
	.text
	.global exc_entry_cpu
	.weak exc_entry_cpu
	.align 4
exc_entry_cpu:

	EXCEPTION_PROLOGUE

	mov	r3, sp				/* as exception handler's para(exc_frame) */

/* find the exception cause */
	lr	r0, [AUX_ECR]
	lsr	r0, r0, 16
	bmsk	r0, r0, 7
	/* ETAS BIP Porting */
	cmp r0, 0x8
	beq etas_bip_handle
	mov	r1, exc_int_handler_table
	ld.as	r2, [r1, r0]

	mov	r0, r3
	jl	[r2]				/* jump to exception handler where interrupts are not allowed! */
exc_return:

	EXCEPTION_EPILOGUE
	rtie

/* ETAS BIP Porting */
etas_bip_handle:
	EXCEPTION_EPILOGUE
	cmp r3, 0x1
	bne etas_bip_kernelmode
	lr r0, [AUX_ERSTATUS]
	bset r0, r0, 0x7
	sr r0, [AUX_ERSTATUS]
	b etas_bip_swi_return
etas_bip_kernelmode:
	lr r0, [AUX_ERSTATUS]
	bclr r0, r0, 0x7
	sr r0, [AUX_ERSTATUS]
etas_bip_swi_return:
	lr r0, [AUX_ERRET]
	add_s r0, r0, 0x4
	sr r0, [AUX_ERRET]
	rtie


/****** entry for normal interrupt exception handling ******/
	.global exc_entry_int
	.weak exc_entry_int
	.align 4
exc_entry_int:
	clri						/* disable interrupt */

#if ARC_FEATURE_FIRQ == 1
#if ARC_FEATURE_RGF_NUM_BANKS > 1
	lr	r0, [AUX_IRQ_ACT]			/*  check whether it is P0 interrupt */
	btst	r0, 0
	bnz	exc_entry_firq
#else
	PUSH	r10
	lr	r10, [AUX_IRQ_ACT]
	btst	r10, 0
	POP	r10
	bnz	exc_entry_firq
#endif
#endif
	INTERRUPT_PROLOGUE				/* save scratch regs */

/* exc_nest_count +1 */
	ld	r0, [exc_nest_count]
	add	r0, r0, 1
	st	r0, [exc_nest_count]

	jl Os_IRQHandler /* ETAS BIP Porting */

/* exc_nest_count -1 */
	ld	r0, [exc_nest_count]
	sub	r0, r0, 1
	st	r0, [exc_nest_count]

	INTERRUPT_EPILOGUE
	rtie

/****** entry for fast irq exception handling ******/
	.global exc_entry_firq
	.weak exc_entry_firq
	.align 4
exc_entry_firq:
	clri						/* disable interrupt */
	SAVE_FIQ_EXC_REGS

/* exc_nest_count +1 */
	ld	r0, [exc_nest_count]
	add	r0, r0, 1
	st	r0, [exc_nest_count]

	lr	r0, [AUX_IRQ_CAUSE]
	mov	r1, exc_int_handler_table
	ld.as	r2, [r1, r0]				/* r2 = _kernel_exc_tbl + irqno *4 */

/* for the case of software triggered interrupt */
	lr	r3, [AUX_IRQ_HINT]
	cmp	r3, r0
	bne.d	firq_hint_handled
	xor	r3, r3, r3
	sr	r3, [AUX_IRQ_HINT]
firq_hint_handled:

	jl	[r2]					/* jump to interrupt handler */

firq_return:

/* exc_nest_count -1 */
	ld	r0, [exc_nest_count]
	sub	r0, r0, 1
	st	r0, [exc_nest_count]

	RESTORE_FIQ_EXC_REGS
	rtie

#ifndef OS_FREERTOS
	.global abort
abort:
	nop
#endif

/** @endcond */
