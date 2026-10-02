/*
 * RISC-V trap dispatch for Linux 2.0.
 * The kernel runs in M-mode. ecall from M-mode is a system call.
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/ptrace.h>
#include <linux/interrupt.h>
#include <linux/sys.h>
#include <asm/system.h>
#include <asm/ptrace.h>

struct pt_regs *current_regs;

extern void *sys_call_table[];
extern void riscv_timer_interrupt(struct pt_regs *regs);
extern void do_page_fault(struct pt_regs *regs, unsigned long addr, int write);
extern void riscv_putc(char c);
extern void handle_exception(void);

static char *cause_name(unsigned long cause)
{
	switch (cause) {
	case 0: return "instruction address misaligned";
	case 1: return "instruction access fault";
	case 2: return "illegal instruction";
	case 3: return "breakpoint";
	case 4: return "load address misaligned";
	case 5: return "load access fault";
	case 6: return "store address misaligned";
	case 7: return "store access fault";
	case 8: return "ecall from U-mode";
	case 11: return "ecall from M-mode";
	case 12: return "instruction page fault";
	case 13: return "load page fault";
	case 15: return "store page fault";
	default: return "unknown";
	}
}

void show_regs(struct pt_regs *regs)
{
	int i;

	printk("epc %08lx  status %08lx  cause %08lx  badaddr %08lx\n",
		regs->epc, regs->status, regs->cause, regs->badaddr);
	for (i = 1; i < 32; i++) {
		printk("x%-2d %08lx%s", i, regs->regs[i], (i % 4) ? "  " : "\n");
	}
}

static void console_verbose(void)
{
	extern int console_loglevel;

	console_loglevel = 15;
}

void die(const char *str, struct pt_regs *regs, unsigned long err)
{
	console_verbose();
	printk("%s: %08lx\n", str, err);
	show_regs(regs);
	if (user_mode(regs))
		do_exit(SIGSEGV);
	while (1)
		asm volatile("wfi");
}

void die_if_kernel(const char *str, struct pt_regs *regs, unsigned long err)
{
	if (!user_mode(regs))
		die(str, regs, err);
}

static void do_syscall(struct pt_regs *regs)
{
	long nr = regs->regs[17];
	long (*fn)(long, long, long, long, long, long);
	struct pt_regs *old = current_regs;

	regs->epc += 4;
	current_regs = regs;
	if (nr < 0 || nr >= NR_syscalls || !sys_call_table[nr]) {
		regs->regs[10] = -ENOSYS;
		current_regs = old;
		return;
	}
	fn = sys_call_table[nr];
	regs->regs[10] = fn(regs->regs[10], regs->regs[11], regs->regs[12],
			    regs->regs[13], regs->regs[14], regs->regs[15]);
	current_regs = old;
}

asmlinkage void do_trap(struct pt_regs *regs)
{
	unsigned long cause = regs->cause;
	int write = 0;

	if (cause & 0x80000000UL) {
		cause &= ~0x80000000UL;
		intr_count++;
		if (cause == 7)
			riscv_timer_interrupt(regs);
		else
			printk("unexpected interrupt %lu\n", cause);
		intr_count--;
		if (!intr_count && (bh_active & bh_mask))
			do_bottom_half();
		return;
	}

	if (cause == 8 || cause == 11) {
		do_syscall(regs);
		return;
	}

	if (cause == 15 || cause == 7 || cause == 6)
		write = 1;
	if (cause == 12 || cause == 13 || cause == 15 ||
	    cause == 1 || cause == 5 || cause == 7) {
		do_page_fault(regs, regs->badaddr, write);
		return;
	}

	printk("trap: %s  epc %08lx  badaddr %08lx\n",
		cause_name(cause), regs->epc, regs->badaddr);
	die("Oops", regs, cause);
}

void trap_init(void)
{
	asm volatile("csrw mtvec, %0" : : "r" (handle_exception));
}
