#ifndef _ASM_RISCV_BITOPS_H
#define _ASM_RISCV_BITOPS_H

/*
 * Atomic bitops via the A extension. Bit 0 is the LSB of the first word.
 */

static inline int set_bit(int nr, volatile void *addr)
{
	unsigned long *p = ((unsigned long *)addr) + (nr >> 5);
	unsigned long mask = 1UL << (nr & 31);
	unsigned long old;

	__asm__ __volatile__ (
		"amoor.w %0, %2, (%1)"
		: "=r" (old) : "r" (p), "r" (mask) : "memory");
	return (old & mask) != 0;
}

static inline int clear_bit(int nr, volatile void *addr)
{
	unsigned long *p = ((unsigned long *)addr) + (nr >> 5);
	unsigned long mask = 1UL << (nr & 31);
	unsigned long old;

	__asm__ __volatile__ (
		"amoand.w %0, %2, (%1)"
		: "=r" (old) : "r" (p), "r" (~mask) : "memory");
	return (old & mask) != 0;
}

static inline int change_bit(int nr, volatile void *addr)
{
	unsigned long *p = ((unsigned long *)addr) + (nr >> 5);
	unsigned long mask = 1UL << (nr & 31);
	unsigned long old;

	__asm__ __volatile__ (
		"amoxor.w %0, %2, (%1)"
		: "=r" (old) : "r" (p), "r" (mask) : "memory");
	return (old & mask) != 0;
}

static inline int test_bit(int nr, const volatile void *addr)
{
	return ((1UL << (nr & 31)) &
		(((const unsigned long *)addr)[nr >> 5])) != 0;
}

static inline unsigned long ffz(unsigned long word)
{
	unsigned long bit = 0;

	word = ~word;
	while ((word & 1) == 0) {
		word >>= 1;
		bit++;
	}
	return bit;
}

static inline int find_first_zero_bit(void *addr, unsigned size)
{
	unsigned long *p = (unsigned long *)addr;
	unsigned words = (size + 31) >> 5;
	unsigned i, bit;

	for (i = 0; i < words; i++) {
		if (p[i] != ~0UL) {
			bit = (i << 5) + ffz(p[i]);
			return bit < size ? bit : size;
		}
	}
	return size;
}

static inline int find_next_zero_bit(void *addr, int size, int offset)
{
	unsigned long *p = (unsigned long *)addr;
	int bit = offset & 31;
	unsigned long word;
	int i;

	if (offset >= size)
		return size;
	p += offset >> 5;
	if (bit) {
		word = *p >> bit;
		for (i = bit; i < 32 && (offset + i - bit) < size; i++) {
			if ((word & 1) == 0)
				return offset + (i - bit);
			word >>= 1;
		}
		offset += 32 - bit;
		p++;
	}
	if (offset >= size)
		return size;
	return offset + find_first_zero_bit(p, size - offset);
}

#endif /* _ASM_RISCV_BITOPS_H */
