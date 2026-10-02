#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/stddef.h>
#include <linux/unistd.h>
#include <linux/ptrace.h>
#include <linux/malloc.h>
#include <linux/user.h>
#include <linux/a.out.h>
#include <linux/elf.h>
#include <linux/elfcore.h>
#include <asm/segment.h>
#include <asm/system.h>
#include <asm/io.h>
#include <asm/processor.h>

extern void ret_from_kernel_thread(void);
extern void ret_from_fork(void);

asmlinkage int sys_idle(void)
{
	if (current->pid != 0)
		return -EPERM;
	for (;;) {
		asm volatile("wfi");
		schedule();
	}
}

void hard_reset_now(void)
{
	/* SiFive test finisher: 0x7777 resets the QEMU virt machine. */
	*(volatile unsigned int *)0x100000 = 0x7777;
	while (1)
		asm volatile("wfi");
}

void exit_thread(void)
{
}

void flush_thread(void)
{
}

void release_thread(struct task_struct *dead_task)
{
}

void copy_thread(int nr, unsigned long clone_flags, unsigned long usp,
		 struct task_struct *p, struct pt_regs *regs)
{
	struct pt_regs *childregs;
	struct riscv_switch_frame *frame;
	unsigned long stack = p->kernel_stack_page + PAGE_SIZE;
	int kernel_thread = (regs->epc == (unsigned long)ret_from_kernel_thread);

	childregs = ((struct pt_regs *)stack) - 1;
	*childregs = *regs;
	if (!kernel_thread)
		childregs->regs[10] = 0;
	if (usp)
		childregs->regs[2] = usp;

	frame = ((struct riscv_switch_frame *)childregs) - 1;
	memset(frame, 0, sizeof(*frame));
	if (kernel_thread) {
		frame->s[0] = regs->regs[10];	/* fn */
		frame->s[1] = regs->regs[11];	/* arg */
		frame->ra = (unsigned long)ret_from_kernel_thread;
	} else {
		frame->ra = (unsigned long)ret_from_fork;
	}
	p->tss.ksp = (unsigned long)frame;
	p->tss.usp = usp;
}

void dump_thread(struct pt_regs *regs, struct user *dump)
{
	int i;

	dump->magic = CMAGIC;
	dump->start_code = current->mm->start_code;
	dump->start_stack = regs->regs[2] & ~(PAGE_SIZE - 1);
	dump->u_tsize = (current->mm->end_code - current->mm->start_code) >> PAGE_SHIFT;
	dump->u_dsize = (current->mm->brk + (PAGE_SIZE - 1) - current->mm->start_code) >> PAGE_SHIFT;
	dump->u_dsize -= dump->u_tsize;
	dump->u_ssize = 0;
	if (dump->start_stack < TASK_SIZE)
		dump->u_ssize = (TASK_SIZE - dump->start_stack) >> PAGE_SHIFT;
	dump->regs = *regs;
	dump->u_fpvalid = 0;
	for (i = 0; i < 8; i++)
		dump->u_debugreg[i] = current->debugreg[i];
}

int dump_fpu(elf_fpregset_t *fpu)
{
	return 0;
}

asmlinkage int sys_fork(void)
{
	return do_fork(SIGCHLD, current_regs->regs[2], current_regs);
}

asmlinkage int sys_clone(unsigned long flags, unsigned long usp)
{
	if (!usp)
		usp = current_regs->regs[2];
	return do_fork(flags, usp, current_regs);
}

asmlinkage int sys_execve(char *filename, char **argv, char **envp)
{
	int error;
	char *name;

	error = getname(filename, &name);
	if (error)
		return error;
	error = do_execve(name, argv, envp, current_regs);
	putname(name);
	return error;
}

unsigned long get_wchan(struct task_struct *p)
{
	return 0;
}
