#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <asm/system.h>
#include <asm/pgtable.h>

extern void die(const char *str, struct pt_regs *regs, unsigned long err);

void do_page_fault(struct pt_regs *regs, unsigned long address, int write)
{
	struct vm_area_struct *vma;

	if (intr_count || !current->mm)
		die("page fault in interrupt", regs, address);

	vma = find_vma(current->mm, address);
	if (!vma || vma->vm_start > address)
		goto bad;
	if (write) {
		if (!(vma->vm_flags & VM_WRITE))
			goto bad;
	} else {
		if (!(vma->vm_flags & (VM_READ | VM_EXEC)))
			goto bad;
	}
	handle_mm_fault(vma, address, write);
	return;

bad:
	if (user_mode(regs)) {
		force_sig(SIGSEGV, current);
		return;
	}
	printk("kernel page fault at %08lx epc %08lx\n", address, regs->epc);
	die("Oops", regs, address);
}
