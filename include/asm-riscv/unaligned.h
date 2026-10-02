#ifndef _ASM_RISCV_UNALIGNED_H
#define _ASM_RISCV_UNALIGNED_H

#define get_unaligned(ptr) ({ \
	__typeof__(*(ptr)) __val; \
	memcpy(&__val, (ptr), sizeof(__val)); \
	__val; \
})

#define put_unaligned(val, ptr) ({ \
	__typeof__(*(ptr)) __val = (val); \
	memcpy((ptr), &__val, sizeof(__val)); \
})

#endif /* _ASM_RISCV_UNALIGNED_H */
