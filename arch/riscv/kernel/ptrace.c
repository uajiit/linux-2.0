#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/ptrace.h>
#include <linux/user.h>
#include <asm/segment.h>

static inline struct task_struct *get_task(int pid)
{
	int i;

	for (i = 1; i < NR_TASKS; i++) {
		if (task[i] != NULL && task[i]->pid == pid)
			return task[i];
	}
	return NULL;
}

asmlinkage int sys_ptrace(long request, long pid, long addr, long data)
{
	struct task_struct *child;

	if (request == PTRACE_TRACEME) {
		if (current->flags & PF_PTRACED)
			return -EPERM;
		current->flags |= PF_PTRACED;
		return 0;
	}
	if (pid == 1)
		return -EPERM;
	if (!(child = get_task(pid)))
		return -ESRCH;
	if (request == PTRACE_ATTACH) {
		if (child == current)
			return -EPERM;
		if (child->flags & PF_PTRACED)
			return -EPERM;
		child->flags |= PF_PTRACED;
		if (child->p_pptr != current) {
			REMOVE_LINKS(child);
			child->p_pptr = current;
			SET_LINKS(child);
		}
		send_sig(SIGSTOP, child, 1);
		return 0;
	}
	if (!(child->flags & PF_PTRACED))
		return -ESRCH;
	if (child->p_pptr != current)
		return -ESRCH;
	switch (request) {
	case PTRACE_KILL:
		send_sig(SIGKILL, child, 1);
		return 0;
	case PTRACE_DETACH:
		child->flags &= ~PF_PTRACED;
		child->exit_code = data;
		REMOVE_LINKS(child);
		child->p_pptr = child->p_opptr;
		SET_LINKS(child);
		wake_up_process(child);
		return 0;
	case PTRACE_CONT:
		child->exit_code = data;
		wake_up_process(child);
		return 0;
	default:
		return -EIO;
	}
}
