#ifndef _ASM_RISCV_PGTABLE_H
#define _ASM_RISCV_PGTABLE_H

#include <asm/page.h>

/*
 * Linux 2.0 software page tables, folded to two levels the way the i386
 * port does it. The geometry matches Sv32 (10 + 10 bits, 4KiB pages) so a
 * later hardware walk can be built from the same indices.
 *
 * The PTE itself stores a kernel virtual address in the upper bits and
 * protection in the low bits. That is what pte_page() / MAP_NR() expect.
 * It is not a raw Sv32 PTE. The kernel runs in M-mode on the direct map,
 * so it does not need the hardware walker to reach its own memory.
 */

extern unsigned long high_memory;

#define PMD_SHIFT	22
#define PMD_SIZE	(1UL << PMD_SHIFT)
#define PMD_MASK	(~(PMD_SIZE-1))

#define PGDIR_SHIFT	22
#define PGDIR_SIZE	(1UL << PGDIR_SHIFT)
#define PGDIR_MASK	(~(PGDIR_SIZE-1))

#define PTRS_PER_PTE	1024
#define PTRS_PER_PMD	1
#define PTRS_PER_PGD	1024

/*
 * No hardware walker is installed, so vmalloc addresses must be real
 * DRAM. setup_arch() leaves the top of RAM above high_memory for this.
 */
#define VMALLOC_OFFSET	PAGE_SIZE
#define VMALLOC_START	high_memory
#define VMALLOC_VMADDR(x) ((unsigned long)(x))

#define _PAGE_PRESENT	0x001
#define _PAGE_RW	0x002
#define _PAGE_USER	0x004
#define _PAGE_ACCESSED	0x020
#define _PAGE_DIRTY	0x040

#define _PAGE_TABLE	(_PAGE_PRESENT | _PAGE_RW | _PAGE_USER | _PAGE_ACCESSED | _PAGE_DIRTY)
#define _PAGE_CHG_MASK	(PAGE_MASK | _PAGE_ACCESSED | _PAGE_DIRTY)

#define PAGE_NONE	__pgprot(_PAGE_PRESENT | _PAGE_ACCESSED)
#define PAGE_SHARED	__pgprot(_PAGE_PRESENT | _PAGE_RW | _PAGE_USER | _PAGE_ACCESSED)
#define PAGE_COPY	__pgprot(_PAGE_PRESENT | _PAGE_USER | _PAGE_ACCESSED)
#define PAGE_READONLY	__pgprot(_PAGE_PRESENT | _PAGE_USER | _PAGE_ACCESSED)
#define PAGE_KERNEL	__pgprot(_PAGE_PRESENT | _PAGE_RW | _PAGE_DIRTY | _PAGE_ACCESSED)

#define __P000	PAGE_NONE
#define __P001	PAGE_READONLY
#define __P010	PAGE_COPY
#define __P011	PAGE_COPY
#define __P100	PAGE_READONLY
#define __P101	PAGE_READONLY
#define __P110	PAGE_COPY
#define __P111	PAGE_COPY

#define __S000	PAGE_NONE
#define __S001	PAGE_READONLY
#define __S010	PAGE_SHARED
#define __S011	PAGE_SHARED
#define __S100	PAGE_READONLY
#define __S101	PAGE_READONLY
#define __S110	PAGE_SHARED
#define __S111	PAGE_SHARED

#define flush_cache_all()			do { } while (0)
#define flush_cache_mm(mm)			do { } while (0)
#define flush_cache_range(mm, start, end)	do { } while (0)
#define flush_cache_page(vma, vmaddr)		do { } while (0)
#define flush_page_to_ram(page)			do { } while (0)

static inline void flush_tlb(void)
{
	__asm__ __volatile__ ("fence" : : : "memory");
}
#define flush_tlb_all() flush_tlb()

static inline void flush_tlb_mm(struct mm_struct *mm)
{
	if (mm == current->mm)
		flush_tlb();
}

static inline void flush_tlb_page(struct vm_area_struct *vma, unsigned long addr)
{
	if (vma->vm_mm == current->mm)
		flush_tlb();
}

static inline void flush_tlb_range(struct mm_struct *mm,
				   unsigned long start, unsigned long end)
{
	if (mm == current->mm)
		flush_tlb();
}

extern unsigned long empty_zero_page[1024];

extern pte_t __bad_page(void);
extern pte_t * __bad_pagetable(void);

#define BAD_PAGETABLE __bad_pagetable()
#define BAD_PAGE __bad_page()
#define ZERO_PAGE ((unsigned long) empty_zero_page)

#define BITS_PER_PTR		(8*sizeof(unsigned long))
#define PTR_MASK		(~(sizeof(void*)-1))
#define SIZEOF_PTR_LOG2		2

#define PAGE_PTR(address) \
((unsigned long)(address)>>(PAGE_SHIFT-SIZEOF_PTR_LOG2)&PTR_MASK&~PAGE_MASK)

#define SET_PAGE_DIR(tsk,pgdir) \
do { \
	(tsk)->tss.pgd = (unsigned long)(pgdir); \
} while (0)

static inline int pte_none(pte_t pte)		{ return !pte_val(pte); }
static inline int pte_present(pte_t pte)	{ return pte_val(pte) & _PAGE_PRESENT; }
static inline void pte_clear(pte_t *ptep)	{ pte_val(*ptep) = 0; }

static inline int pmd_none(pmd_t pmd)		{ return !pmd_val(pmd); }
static inline int pmd_bad(pmd_t pmd)
{
	return (pmd_val(pmd) & ~PAGE_MASK) != _PAGE_TABLE ||
		pmd_val(pmd) > high_memory;
}
static inline int pmd_present(pmd_t pmd)	{ return pmd_val(pmd) & _PAGE_PRESENT; }
static inline void pmd_clear(pmd_t *pmdp)	{ pmd_val(*pmdp) = 0; }

static inline int pgd_none(pgd_t pgd)		{ return 0; }
static inline int pgd_bad(pgd_t pgd)		{ return 0; }
static inline int pgd_present(pgd_t pgd)	{ return 1; }
static inline void pgd_clear(pgd_t *pgdp)	{ }

static inline int pte_read(pte_t pte)		{ return pte_val(pte) & _PAGE_USER; }
static inline int pte_write(pte_t pte)		{ return pte_val(pte) & _PAGE_RW; }
static inline int pte_exec(pte_t pte)		{ return pte_val(pte) & _PAGE_USER; }
static inline int pte_dirty(pte_t pte)		{ return pte_val(pte) & _PAGE_DIRTY; }
static inline int pte_young(pte_t pte)		{ return pte_val(pte) & _PAGE_ACCESSED; }

static inline pte_t pte_wrprotect(pte_t pte)	{ pte_val(pte) &= ~_PAGE_RW; return pte; }
static inline pte_t pte_rdprotect(pte_t pte)	{ pte_val(pte) &= ~_PAGE_USER; return pte; }
static inline pte_t pte_exprotect(pte_t pte)	{ pte_val(pte) &= ~_PAGE_USER; return pte; }
static inline pte_t pte_mkclean(pte_t pte)	{ pte_val(pte) &= ~_PAGE_DIRTY; return pte; }
static inline pte_t pte_mkold(pte_t pte)	{ pte_val(pte) &= ~_PAGE_ACCESSED; return pte; }
static inline pte_t pte_mkwrite(pte_t pte)	{ pte_val(pte) |= _PAGE_RW; return pte; }
static inline pte_t pte_mkread(pte_t pte)	{ pte_val(pte) |= _PAGE_USER; return pte; }
static inline pte_t pte_mkexec(pte_t pte)	{ pte_val(pte) |= _PAGE_USER; return pte; }
static inline pte_t pte_mkdirty(pte_t pte)	{ pte_val(pte) |= _PAGE_DIRTY; return pte; }
static inline pte_t pte_mkyoung(pte_t pte)	{ pte_val(pte) |= _PAGE_ACCESSED; return pte; }

static inline pte_t mk_pte(unsigned long page, pgprot_t pgprot)
{
	pte_t pte;
	pte_val(pte) = page | pgprot_val(pgprot);
	return pte;
}

static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
	pte_val(pte) = (pte_val(pte) & _PAGE_CHG_MASK) | pgprot_val(newprot);
	return pte;
}

static inline unsigned long pte_page(pte_t pte)
{
	return pte_val(pte) & PAGE_MASK;
}

static inline unsigned long pmd_page(pmd_t pmd)
{
	return pmd_val(pmd) & PAGE_MASK;
}

static inline pgd_t *pgd_offset(struct mm_struct *mm, unsigned long address)
{
	return mm->pgd + (address >> PGDIR_SHIFT);
}

static inline pmd_t *pmd_offset(pgd_t *dir, unsigned long address)
{
	return (pmd_t *)dir;
}

static inline pte_t *pte_offset(pmd_t *dir, unsigned long address)
{
	return (pte_t *)pmd_page(*dir) +
		((address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1));
}

static inline void pte_free_kernel(pte_t *pte)
{
	free_page((unsigned long)pte);
}

static inline pte_t *pte_alloc_kernel(pmd_t *pmd, unsigned long address)
{
	address = (address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
	if (pmd_none(*pmd)) {
		pte_t *page = (pte_t *)get_free_page(GFP_KERNEL);
		if (pmd_none(*pmd)) {
			if (page) {
				pmd_val(*pmd) = _PAGE_TABLE | (unsigned long)page;
				return page + address;
			}
			pmd_val(*pmd) = _PAGE_TABLE | (unsigned long)BAD_PAGETABLE;
			return NULL;
		}
		free_page((unsigned long)page);
	}
	if (pmd_bad(*pmd)) {
		pmd_val(*pmd) = _PAGE_TABLE | (unsigned long)BAD_PAGETABLE;
		return NULL;
	}
	return (pte_t *)pmd_page(*pmd) + address;
}

static inline void pmd_free_kernel(pmd_t *pmd)
{
	pmd_val(*pmd) = 0;
}

static inline pmd_t *pmd_alloc_kernel(pgd_t *pgd, unsigned long address)
{
	return (pmd_t *)pgd;
}

static inline void pte_free(pte_t *pte)
{
	free_page((unsigned long)pte);
}

static inline pte_t *pte_alloc(pmd_t *pmd, unsigned long address)
{
	return pte_alloc_kernel(pmd, address);
}

static inline void pmd_free(pmd_t *pmd)
{
	pmd_val(*pmd) = 0;
}

static inline pmd_t *pmd_alloc(pgd_t *pgd, unsigned long address)
{
	return (pmd_t *)pgd;
}

static inline void pgd_free(pgd_t *pgd)
{
	free_page((unsigned long)pgd);
}

static inline pgd_t *pgd_alloc(void)
{
	return (pgd_t *)get_free_page(GFP_KERNEL);
}

extern pgd_t swapper_pg_dir[1024];

static inline void update_mmu_cache(struct vm_area_struct *vma,
				    unsigned long address, pte_t pte)
{
}

#define SWP_TYPE(entry)		(((entry) >> 1) & 0x7f)
#define SWP_OFFSET(entry)	((entry) >> 8)
#define SWP_ENTRY(type,offset)	(((type) << 1) | ((offset) << 8))

#endif /* _ASM_RISCV_PGTABLE_H */
