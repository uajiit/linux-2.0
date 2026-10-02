#ifndef _ASM_RISCV_PROCESSOR_H
#define _ASM_RISCV_PROCESSOR_H

#include <asm/ptrace.h>
#include <asm/page.h>

/*
 * Callee-saved frame used by riscv_switch. Layout must match entry.S.
 * 16 registers * 4 bytes = 64, so the stack stays 16-byte aligned.
 */
struct riscv_switch_frame {
	unsigned long s[12];	/* s0 .. s11 */
	unsigned long ra;
	unsigned long pad[3];
};

struct thread_struct {
	unsigned long ksp;	/* kernel stack pointer (switch frame) */
	unsigned long usp;	/* user stack pointer */
	unsigned long pgd;	/* page directory */
	unsigned long last_pc;
};

#define INIT_TSS { \
	sizeof(init_kernel_stack) + (unsigned long)&init_kernel_stack, \
	0, \
	(unsigned long)&swapper_pg_dir, \
	0 \
}

#define INIT_MMAP { &init_mm, 0, 0x40000000, \
		PAGE_SHARED, VM_READ | VM_WRITE | VM_EXEC }

/*
 * User virtual addresses sit below the direct-mapped kernel window.
 */
#define TASK_SIZE	PAGE_OFFSET

#define alloc_kernel_stack()	get_free_page(GFP_KERNEL)
#define free_kernel_stack(page)	free_page((page))

static inline void start_thread(struct pt_regs *regs, unsigned long pc,
				unsigned long sp)
{
	int i;

	for (i = 0; i < 32; i++)
		regs->regs[i] = 0;
	regs->regs[2] = sp;
	regs->epc = pc;
	/* mret to U-mode with interrupts enabled. */
	regs->status = 0x80;		/* MPIE */
}

static inline unsigned long thread_saved_pc(struct thread_struct *t)
{
	return t->last_pc;
}

#define EISA_bus 0
#define EISA_bus__is_a_macro
#define MCA_bus 0
#define MCA_bus__is_a_macro
#define wp_works_ok 1
#define wp_works_ok__is_a_macro

#endif /* _ASM_RISCV_PROCESSOR_H */
