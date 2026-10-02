#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/ptrace.h>
#include <asm/segment.h>
#include <asm/ptrace.h>
#include <asm/bitops.h>

#define _S(nr) (1<<((nr)-1))
#define _BLOCKABLE (~(_S(SIGKILL) | _S(SIGSTOP)))

/*
 * Default-action delivery. A caught signal while returning to user mode
 * is left pending; building a full sigframe is future work.
 */
asmlinkage int do_signal(unsigned long oldmask, struct pt_regs *regs)
{
	unsigned long mask = ~current->blocked;
	unsigned long pending;
	unsigned long signr;
	struct sigaction *sa;

	while ((pending = current->signal & mask) != 0) {
		signr = ffz(~pending) + 1;
		current->signal &= ~(1UL << (signr - 1));
		sa = current->sig->action + signr - 1;
		if (sa->sa_handler == SIG_IGN)
			continue;
		if (sa->sa_handler == SIG_DFL) {
			if (signr == SIGCONT || signr == SIGCHLD ||
			    signr == SIGWINCH || signr == SIGURG)
				continue;
			if (signr == SIGSTOP || signr == SIGTSTP ||
			    signr == SIGTTIN || signr == SIGTTOU) {
				if (current->flags & PF_PTRACED)
					continue;
				current->state = TASK_STOPPED;
				current->exit_code = signr;
				if (!(current->p_pptr->sig->action[SIGCHLD-1].sa_flags &
				      SA_NOCLDSTOP))
					send_sig(SIGCHLD, current->p_pptr, 1);
				schedule();
				continue;
			}
			do_exit(signr);
		}
		/* Caught. Put it back until a user sigframe exists. */
		current->signal |= 1UL << (signr - 1);
		return 0;
	}
	return 0;
}

asmlinkage int sys_sigreturn(void)
{
	return -ENOSYS;
}

asmlinkage int sys_sigsuspend(int restart, unsigned long oldmask, unsigned long set)
{
	unsigned long mask;

	mask = current->blocked;
	current->blocked = set & _BLOCKABLE;
	if (current_regs)
		current_regs->regs[10] = -EINTR;
	for (;;) {
		current->state = TASK_INTERRUPTIBLE;
		schedule();
		if (do_signal(mask, current_regs))
			return -EINTR;
	}
}
