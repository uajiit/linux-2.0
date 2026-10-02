#ifndef _ASM_RISCV_SYSTEM_H
#define _ASM_RISCV_SYSTEM_H

#define mb()	__asm__ __volatile__ ("fence" : : : "memory")
#define nop()	__asm__ __volatile__ ("nop")

/*
 * Interrupt enable is mstatus.MIE (bit 3). cli/sti touch only that bit.
 * save_flags/restore_flags keep the whole register so nesting works.
 */
#define cli() \
	__asm__ __volatile__ ("csrci mstatus, 8" : : : "memory")
#define sti() \
	__asm__ __volatile__ ("csrsi mstatus, 8" : : : "memory")

#define save_flags(x) \
	__asm__ __volatile__ ("csrr %0, mstatus" : "=r" (x) : : "memory")
#define restore_flags(x) \
	__asm__ __volatile__ ("csrw mstatus, %0" : : "r" (x) : "memory")

#define xchg(ptr, x) \
	((__typeof__(*(ptr)))__xchg((unsigned long)(x), (ptr), sizeof(*(ptr))))
#define tas(ptr) xchg((ptr), 1)

static inline unsigned long __xchg(unsigned long val, volatile void *ptr, int size)
{
	unsigned long ret;

	if (size != 4)
		return val;
	__asm__ __volatile__ (
		"amoswap.w %0, %2, (%1)"
		: "=r" (ret)
		: "r" (ptr), "r" (val)
		: "memory");
	return ret;
}

struct task_struct;
extern void riscv_switch(unsigned long *prev_ksp, unsigned long next_ksp);

#define switch_to(prev, next) do { \
	current_set[0] = (next); \
	riscv_switch(&(prev)->tss.ksp, (next)->tss.ksp); \
} while (0)

#endif /* _ASM_RISCV_SYSTEM_H */
