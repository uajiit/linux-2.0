/*
 * Timer for the QEMU virt machine. The CLINT mtime register ticks at
 * 10 MHz. HZ is 100, so each tick is 100000 cycles.
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/ptrace.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>

#define CLINT_MTIME	0x0200bff8UL
#define CLINT_MTIMECMP	0x02004000UL
#define CLINT_FREQ	10000000UL

static unsigned long timer_interval = CLINT_FREQ / HZ;

static unsigned long long read_mtime(void)
{
	volatile unsigned int *lo = (volatile unsigned int *)CLINT_MTIME;
	volatile unsigned int *hi = lo + 1;
	unsigned int h1, h2, l;

	do {
		h1 = *hi;
		l = *lo;
		h2 = *hi;
	} while (h1 != h2);
	return ((unsigned long long)h1 << 32) | l;
}

static void write_mtimecmp(unsigned long long val)
{
	volatile unsigned int *lo = (volatile unsigned int *)CLINT_MTIMECMP;
	volatile unsigned int *hi = lo + 1;

	*hi = 0xffffffff;
	*lo = (unsigned int)val;
	*hi = (unsigned int)(val >> 32);
}

static void timer_set_next(void)
{
	write_mtimecmp(read_mtime() + timer_interval);
}

void riscv_timer_interrupt(struct pt_regs *regs)
{
	timer_set_next();
	do_timer(regs);
}

void do_gettimeofday(struct timeval *tv)
{
	unsigned long flags;

	save_flags(flags);
	cli();
	*tv = xtime;
	restore_flags(flags);
}

void do_settimeofday(struct timeval *tv)
{
	cli();
	xtime = *tv;
	sti();
}

void time_init(void)
{
	xtime.tv_sec = 0;
	xtime.tv_usec = 0;
	timer_set_next();
}
