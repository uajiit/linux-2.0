#ifndef _ASM_RISCV_SEGMENT_H
#define _ASM_RISCV_SEGMENT_H

/*
 * RV32 has a single address space. User and kernel pointers are ordinary
 * pointers; there is no separate fs segment.
 */

#define KERNEL_DS	0
#define USER_DS		1

#define put_user(x, ptr) __put_user((unsigned long)(x), (ptr), sizeof(*(ptr)))
#define get_user(ptr) ((__typeof__(*(ptr)))__get_user((ptr), sizeof(*(ptr))))

extern int bad_user_access_length(void);

static inline void __put_user(unsigned long x, void *y, int size)
{
	switch (size) {
	case 1: *(unsigned char *)y = x; break;
	case 2: *(unsigned short *)y = x; break;
	case 4: *(unsigned long *)y = x; break;
	default: bad_user_access_length();
	}
}

static inline unsigned long __get_user(const void *y, int size)
{
	switch (size) {
	case 1: return *(const unsigned char *)y;
	case 2: return *(const unsigned short *)y;
	case 4: return *(const unsigned long *)y;
	default: return bad_user_access_length();
	}
}

#define get_user_byte(addr)	(*(const unsigned char *)(addr))
#define get_user_word(addr)	(*(const unsigned short *)(addr))
#define get_user_long(addr)	(*(const unsigned long *)(addr))
#define put_user_byte(x, addr)	(*(unsigned char *)(addr) = (x))
#define put_user_word(x, addr)	(*(unsigned short *)(addr) = (x))
#define put_user_long(x, addr)	(*(unsigned long *)(addr) = (x))

#define get_fs_byte(addr)	get_user_byte(addr)
#define get_fs_word(addr)	get_user_word(addr)
#define get_fs_long(addr)	get_user_long(addr)
#define put_fs_byte(x, addr)	put_user_byte((x), (addr))
#define put_fs_word(x, addr)	put_user_word((x), (addr))
#define put_fs_long(x, addr)	put_user_long((x), (addr))

#define memcpy_fromfs(to, from, n)	memcpy((to), (from), (n))
#define memcpy_tofs(to, from, n)	memcpy((to), (from), (n))

static inline unsigned long get_fs(void) { return 0; }
static inline unsigned long get_ds(void) { return 0; }
static inline void set_fs(unsigned long val) { }

#endif /* _ASM_RISCV_SEGMENT_H */
