/*
 * Trap entry, context switch and kernel-thread startup.
 * pt_regs offsets match include/asm-riscv/ptrace.h.
 * The switch frame matches struct riscv_switch_frame.
 */

.section .text
.align 2
.globl handle_exception
handle_exception:
	csrw	mscratch, t0
	addi	sp, sp, -160
	addi	t0, sp, 160
	sw	t0, 8(sp)		/* original sp */
	csrr	t0, mscratch
	sw	t0, 20(sp)		/* original t0 / x5 */

	sw	zero, 0(sp)
	sw	x1, 4(sp)
	sw	x3, 12(sp)
	sw	x4, 16(sp)
	sw	x6, 24(sp)
	sw	x7, 28(sp)
	sw	x8, 32(sp)
	sw	x9, 36(sp)
	sw	x10, 40(sp)
	sw	x11, 44(sp)
	sw	x12, 48(sp)
	sw	x13, 52(sp)
	sw	x14, 56(sp)
	sw	x15, 60(sp)
	sw	x16, 64(sp)
	sw	x17, 68(sp)
	sw	x18, 72(sp)
	sw	x19, 76(sp)
	sw	x20, 80(sp)
	sw	x21, 84(sp)
	sw	x22, 88(sp)
	sw	x23, 92(sp)
	sw	x24, 96(sp)
	sw	x25, 100(sp)
	sw	x26, 104(sp)
	sw	x27, 108(sp)
	sw	x28, 112(sp)
	sw	x29, 116(sp)
	sw	x30, 120(sp)
	sw	x31, 124(sp)

	csrr	t0, mepc
	sw	t0, 128(sp)
	csrr	t0, mstatus
	sw	t0, 132(sp)
	csrr	t0, mcause
	sw	t0, 136(sp)
	csrr	t0, mtval
	sw	t0, 140(sp)
	lw	t0, 40(sp)
	sw	t0, 144(sp)		/* orig_a0 */

	mv	a0, sp
	call	do_trap

	lw	t0, 128(sp)
	csrw	mepc, t0

	lw	x1, 4(sp)
	lw	x3, 12(sp)
	lw	x4, 16(sp)
	lw	x6, 24(sp)
	lw	x7, 28(sp)
	lw	x8, 32(sp)
	lw	x9, 36(sp)
	lw	x10, 40(sp)
	lw	x11, 44(sp)
	lw	x12, 48(sp)
	lw	x13, 52(sp)
	lw	x14, 56(sp)
	lw	x15, 60(sp)
	lw	x16, 64(sp)
	lw	x17, 68(sp)
	lw	x18, 72(sp)
	lw	x19, 76(sp)
	lw	x20, 80(sp)
	lw	x21, 84(sp)
	lw	x22, 88(sp)
	lw	x23, 92(sp)
	lw	x24, 96(sp)
	lw	x25, 100(sp)
	lw	x26, 104(sp)
	lw	x27, 108(sp)
	lw	x28, 112(sp)
	lw	x29, 116(sp)
	lw	x30, 120(sp)
	lw	x31, 124(sp)
	lw	x5, 20(sp)
	lw	sp, 8(sp)
	mret

/* a0 = &prev->tss.ksp, a1 = next->tss.ksp */
.globl riscv_switch
riscv_switch:
	addi	sp, sp, -64
	sw	s0, 0(sp)
	sw	s1, 4(sp)
	sw	s2, 8(sp)
	sw	s3, 12(sp)
	sw	s4, 16(sp)
	sw	s5, 20(sp)
	sw	s6, 24(sp)
	sw	s7, 28(sp)
	sw	s8, 32(sp)
	sw	s9, 36(sp)
	sw	s10, 40(sp)
	sw	s11, 44(sp)
	sw	ra, 48(sp)
	sw	sp, 0(a0)
	mv	sp, a1
	lw	s0, 0(sp)
	lw	s1, 4(sp)
	lw	s2, 8(sp)
	lw	s3, 12(sp)
	lw	s4, 16(sp)
	lw	s5, 20(sp)
	lw	s6, 24(sp)
	lw	s7, 28(sp)
	lw	s8, 32(sp)
	lw	s9, 36(sp)
	lw	s10, 40(sp)
	lw	s11, 44(sp)
	lw	ra, 48(sp)
	addi	sp, sp, 64
	ret

/* s0 = fn, s1 = arg. Placed in the switch frame by copy_thread. */
.globl ret_from_kernel_thread
ret_from_kernel_thread:
	mv	a0, s1
	jalr	s0
	li	a0, 0
	call	do_exit
1:
	j	1b

/* sp points at the child's pt_regs after the switch frame is popped. */
.globl ret_from_fork
ret_from_fork:
	lw	t0, 128(sp)
	csrw	mepc, t0
	lw	x1, 4(sp)
	lw	x3, 12(sp)
	lw	x4, 16(sp)
	lw	x5, 20(sp)
	lw	x6, 24(sp)
	lw	x7, 28(sp)
	lw	x8, 32(sp)
	lw	x9, 36(sp)
	lw	x10, 40(sp)
	lw	x11, 44(sp)
	lw	x12, 48(sp)
	lw	x13, 52(sp)
	lw	x14, 56(sp)
	lw	x15, 60(sp)
	lw	x16, 64(sp)
	lw	x17, 68(sp)
	lw	x18, 72(sp)
	lw	x19, 76(sp)
	lw	x20, 80(sp)
	lw	x21, 84(sp)
	lw	x22, 88(sp)
	lw	x23, 92(sp)
	lw	x24, 96(sp)
	lw	x25, 100(sp)
	lw	x26, 104(sp)
	lw	x27, 108(sp)
	lw	x28, 112(sp)
	lw	x29, 116(sp)
	lw	x30, 120(sp)
	lw	x31, 124(sp)
	lw	sp, 8(sp)
	mret
