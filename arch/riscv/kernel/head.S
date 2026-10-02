/*
 * Linux 2.0 RISC-V entry. QEMU virt loads the ELF at 0x80000000 and
 * starts in M-mode when booted with -bios none.
 */

.section .text.head,"ax"
.globl _start
_start:
	/* Supervisor-mode interrupts stay off until sti() in start_kernel. */
	csrci	mstatus, 8

	la	t0, handle_exception
	csrw	mtvec, t0

	/* 'R' before BSS clear. _end is not always 4-byte aligned. */
	li	t0, 0x10000000
	li	t1, 0x80
	sb	t1, 3(t0)
	li	t1, 1
	sb	t1, 0(t0)
	sb	zero, 1(t0)
	li	t1, 3
	sb	t1, 3(t0)
	li	t1, 'R'
	sb	t1, 0(t0)

	/* Clear BSS before using the boot stack, which lives in it. */
	la	t0, __bss_start
	la	t1, _end
	addi	t1, t1, 3
	andi	t1, t1, -4
1:
	bgeu	t0, t1, 2f
	sw	zero, 0(t0)
	addi	t0, t0, 4
	j	1b
2:
	la	sp, boot_stack_top
	andi	sp, sp, -16

	la	t0, init_task
	la	t1, current_set
	sw	t0, 0(t1)

	call	start_kernel
3:
	j	3b

.section .bss
.align 4
boot_stack:
	.space 4096
boot_stack_top:
