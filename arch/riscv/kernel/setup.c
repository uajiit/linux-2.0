#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/tty.h>
#include <asm/page.h>
#include <asm/system.h>

extern char _end;
extern void riscv_uart_init(void);
extern void riscv_putc(char c);

/*
 * QEMU virt DRAM starts at 0x80000000. 32MiB is inside the default
 * machine and is what `qemu-system-riscv32 -m 32M' provides. A larger
 * `-m' is safe: the extra RAM is simply left unused.
 */
#define RISCV_MEM_SIZE	(32UL << 20)
/* Top of DRAM is the vmalloc window. The CPU reaches it by address. */
#define RISCV_VMALLOC_RESERVE (4UL << 20)

static char command_line[256] = "";
char saved_command_line[256];

int abs(int x)
{
	return x < 0 ? -x : x;
}

int get_cpuinfo(char *buffer)
{
	return sprintf(buffer,
		"cpu\t\t: RISC-V\n"
		"mmu\t\t: none\n"
		"bogomips\t: %lu.%02lu\n",
		loops_per_sec / 500000, (loops_per_sec / 5000) % 100);
}

void setup_arch(char **cmdline_p, unsigned long *memory_start_p,
		unsigned long *memory_end_p)
{
	char *msg = "ISC-V\n";

	riscv_uart_init();
	while (*msg)
		riscv_putc(*msg++);

	*cmdline_p = command_line;
	saved_command_line[0] = 0;
	*memory_start_p = PAGE_ALIGN((unsigned long)&_end);
	*memory_end_p = RISCV_RAM_BASE + RISCV_MEM_SIZE - RISCV_VMALLOC_RESERVE;

	ROOT_DEV = MKDEV(1, 0);	/* /dev/ram */
}

asmlinkage int sys_ioperm(unsigned long from, unsigned long num, int on)
{
	return -ENOSYS;
}

asmlinkage int sys_iopl(unsigned long a, unsigned long b, unsigned long c,
			unsigned long d, unsigned long e, unsigned long f)
{
	return -ENOSYS;
}

asmlinkage int sys_vm86(unsigned long a)
{
	return -ENOSYS;
}

asmlinkage int sys_modify_ldt(int func, void *ptr, unsigned long bytecount)
{
	return -ENOSYS;
}
