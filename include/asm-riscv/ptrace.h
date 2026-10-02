#ifndef _ASM_RISCV_PTRACE_H
#define _ASM_RISCV_PTRACE_H

/*
 * Registers saved on the kernel stack by the trap entry.
 * Offsets are shared with arch/riscv/kernel/entry.S.
 *
 * regs[0]  x0  (always 0)
 * regs[1]  ra
 * regs[2]  sp at the time of the trap
 * regs[10] a0
 * regs[17] a7  (syscall number)
 */
struct pt_regs {
	unsigned long regs[32];
	unsigned long epc;
	unsigned long status;
	unsigned long cause;
	unsigned long badaddr;
	unsigned long orig_a0;
	unsigned long edx;	/* binfmt_elf.c writes this */
	unsigned long pad0;
	unsigned long pad1;
};

#define PT_SIZE		160

/* mstatus.MPP == 0 means the trap came from U-mode. */
#define SR_MPP		0x00001800UL
#define user_mode(regs)	(((regs)->status & SR_MPP) == 0)
#define instruction_pointer(regs) ((regs)->epc)

#ifdef __KERNEL__
extern void show_regs(struct pt_regs *);
extern struct pt_regs *current_regs;
#endif

#endif /* _ASM_RISCV_PTRACE_H */
