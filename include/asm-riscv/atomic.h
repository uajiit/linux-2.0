#ifndef _ASM_RISCV_ATOMIC_H
#define _ASM_RISCV_ATOMIC_H

typedef int atomic_t;

static inline void atomic_add(atomic_t i, atomic_t *v)
{
	unsigned long tmp;

	__asm__ __volatile__ (
		"amoadd.w %0, %2, (%1)"
		: "=r" (tmp) : "r" (v), "r" (i) : "memory");
}

static inline void atomic_sub(atomic_t i, atomic_t *v)
{
	atomic_add(-i, v);
}

static inline void atomic_inc(atomic_t *v)
{
	atomic_add(1, v);
}

static inline void atomic_dec(atomic_t *v)
{
	atomic_add(-1, v);
}

/* Non-zero if the value became zero. */
static inline int atomic_dec_and_test(atomic_t *v)
{
	int old;

	__asm__ __volatile__ (
		"amoadd.w %0, %2, (%1)"
		: "=r" (old) : "r" (v), "r" (-1) : "memory");
	return old == 1;
}

#endif /* _ASM_RISCV_ATOMIC_H */
