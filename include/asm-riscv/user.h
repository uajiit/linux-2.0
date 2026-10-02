#ifndef _ASM_RISCV_USER_H
#define _ASM_RISCV_USER_H

#include <asm/page.h>
#include <linux/ptrace.h>

struct user_i387_struct {
	unsigned long fpregs[32];
	unsigned long fcsr;
};

struct user {
	struct pt_regs regs;
	int u_fpvalid;
	struct user_i387_struct i387;
	unsigned long u_tsize;
	unsigned long u_dsize;
	unsigned long u_ssize;
	unsigned long start_code;
	unsigned long start_stack;
	long signal;
	int reserved;
	struct pt_regs *u_ar0;
	struct user_i387_struct *u_fpstate;
	unsigned long magic;
	char u_comm[32];
	int u_debugreg[8];
};

#define NBPG			PAGE_SIZE
#define UPAGES			1
#define HOST_TEXT_START_ADDR	(u.start_code)
#define HOST_STACK_END_ADDR	(u.start_stack + u.u_ssize * NBPG)

#endif /* _ASM_RISCV_USER_H */
