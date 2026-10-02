#include <linux/config.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <asm/system.h>
#include <asm/pgtable.h>

unsigned long empty_zero_page[1024];
static unsigned long empty_bad_page[1024];
static unsigned long empty_bad_page_table[1024];

pgd_t swapper_pg_dir[1024] __attribute__((aligned(4096)));

pte_t *__bad_pagetable(void)
{
	int i;
	pte_t bad = __bad_page();

	for (i = 0; i < PTRS_PER_PTE; i++)
		((pte_t *)empty_bad_page_table)[i] = bad;
	return (pte_t *)empty_bad_page_table;
}

pte_t __bad_page(void)
{
	memset(empty_bad_page, 0, PAGE_SIZE);
	return pte_mkdirty(mk_pte((unsigned long)empty_bad_page, PAGE_SHARED));
}

void show_mem(void)
{
	int i, free = 0, total = 0, reserved = 0, shared = 0;

	i = MAP_NR(high_memory);
	while (i-- > 0) {
		total++;
		if (PageReserved(mem_map + i))
			reserved++;
		else if (!mem_map[i].count)
			free++;
		else
			shared += mem_map[i].count - 1;
	}
	printk("%d pages of RAM\n", total);
	printk("%d free pages\n", free);
	printk("%d reserved pages\n", reserved);
	printk("%d pages shared\n", shared);
}

extern unsigned long free_area_init(unsigned long, unsigned long);

/*
 * Build software page tables for the direct-mapped DRAM window.
 * Each top-level entry covers 4MiB, which is also one Sv32 megapage.
 */
unsigned long paging_init(unsigned long start_mem, unsigned long end_mem)
{
	unsigned long address = PAGE_OFFSET;
	pgd_t *pgd;
	pte_t *pte;
	int i;

	start_mem = PAGE_ALIGN(start_mem);
	memset(swapper_pg_dir, 0, sizeof(swapper_pg_dir));

	while (address < end_mem) {
		pgd = swapper_pg_dir + (address >> PGDIR_SHIFT);
		pte = (pte_t *)start_mem;
		start_mem += PAGE_SIZE;
		memset(pte, 0, PAGE_SIZE);
		pgd_val(*pgd) = _PAGE_TABLE | (unsigned long)pte;
		for (i = 0; i < PTRS_PER_PTE && address < end_mem; i++) {
			set_pte(pte + i, mk_pte(address, PAGE_KERNEL));
			address += PAGE_SIZE;
		}
	}
	return free_area_init(start_mem, end_mem);
}

void mem_init(unsigned long start_mem, unsigned long end_mem)
{
	unsigned long tmp;
	int codepages = 0, datapages = 0;
	extern char _etext;

	end_mem &= PAGE_MASK;
	high_memory = end_mem;
	memset(empty_zero_page, 0, PAGE_SIZE);
	start_mem = PAGE_ALIGN(start_mem);

	tmp = start_mem;
	while (tmp < high_memory) {
		clear_bit(PG_reserved, &mem_map[MAP_NR(tmp)].flags);
		tmp += PAGE_SIZE;
	}

	for (tmp = PAGE_OFFSET; tmp < high_memory; tmp += PAGE_SIZE) {
		if (PageReserved(mem_map + MAP_NR(tmp))) {
			if (tmp < (unsigned long)&_etext)
				codepages++;
			else
				datapages++;
			continue;
		}
		mem_map[MAP_NR(tmp)].count = 1;
		free_page(tmp);
	}
	printk("Memory: %luk/%luk available (%dk kernel code, %dk reserved, %dk data)\n",
		(unsigned long)nr_free_pages << (PAGE_SHIFT - 10),
		(high_memory - PAGE_OFFSET) >> 10,
		codepages << (PAGE_SHIFT - 10),
		0,
		datapages << (PAGE_SHIFT - 10));
}

void si_meminfo(struct sysinfo *val)
{
	int i;

	i = MAP_NR(high_memory);
	val->totalram = 0;
	val->sharedram = 0;
	val->freeram = nr_free_pages << PAGE_SHIFT;
	val->bufferram = buffermem;
	while (i-- > 0) {
		if (PageReserved(mem_map + i))
			continue;
		val->totalram++;
		if (!mem_map[i].count)
			continue;
		val->sharedram += mem_map[i].count - 1;
	}
	val->totalram <<= PAGE_SHIFT;
	val->sharedram <<= PAGE_SHIFT;
}
