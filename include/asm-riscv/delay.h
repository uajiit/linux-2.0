#ifndef _ASM_RISCV_DELAY_H
#define _ASM_RISCV_DELAY_H

extern unsigned long loops_per_sec;

static inline void __delay(unsigned long loops)
{
	unsigned long i;

	for (i = 0; i < loops; i++)
		__asm__ __volatile__ ("" : : : "memory");
}

static inline void udelay(unsigned long usecs)
{
	unsigned long loops;

	loops = (loops_per_sec / 1000000) * usecs;
	__delay(loops);
}

#endif /* _ASM_RISCV_DELAY_H */
