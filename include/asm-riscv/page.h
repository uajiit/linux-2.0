#ifndef _ASM_RISCV_PAGE_H
#define _ASM_RISCV_PAGE_H

/* 4KiB pages, matching Sv32 and the Linux 2.0 VM. */
#define PAGE_SHIFT	12
#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))

#ifdef __KERNEL__

/*
 * QEMU's virt machine places DRAM at 0x80000000. The kernel is linked
 * and run there with a direct map, so a kernel pointer is also the
 * physical address. PAGE_OFFSET is the base of that window: mem_map[0]
 * is the first byte of DRAM, not physical address 0.
 */
#define RISCV_RAM_BASE	0x80000000UL
#define PAGE_OFFSET	RISCV_RAM_BASE
#define PHYS_OFFSET	RISCV_RAM_BASE

#define __pa(x)		((unsigned long)(x) - PAGE_OFFSET + PHYS_OFFSET)
#define __va(x)		((void *)((unsigned long)(x) - PHYS_OFFSET + PAGE_OFFSET))

#define MAP_NR(addr)	(((unsigned long)(addr) - PAGE_OFFSET) >> PAGE_SHIFT)
#define PAGE_ALIGN(addr)	(((addr)+PAGE_SIZE-1)&PAGE_MASK)

#define STRICT_MM_TYPECHECKS

#ifdef STRICT_MM_TYPECHECKS
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long pmd; } pmd_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;

#define pte_val(x)	((x).pte)
#define pmd_val(x)	((x).pmd)
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x)	((pte_t) { (x) })
#define __pmd(x)	((pmd_t) { (x) })
#define __pgd(x)	((pgd_t) { (x) })
#define __pgprot(x)	((pgprot_t) { (x) })
#else
typedef unsigned long pte_t;
typedef unsigned long pmd_t;
typedef unsigned long pgd_t;
typedef unsigned long pgprot_t;

#define pte_val(x)	(x)
#define pmd_val(x)	(x)
#define pgd_val(x)	(x)
#define pgprot_val(x)	(x)

#define __pte(x)	(x)
#define __pmd(x)	(x)
#define __pgd(x)	(x)
#define __pgprot(x)	(x)
#endif

#define set_pte(pteptr, pteval) ((*(pteptr)) = (pteval))

#endif /* __KERNEL__ */
#endif /* _ASM_RISCV_PAGE_H */
