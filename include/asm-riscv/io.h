#ifndef _ASM_RISCV_IO_H
#define _ASM_RISCV_IO_H

/*
 * Memory-mapped I/O. There is no port I/O space; inb/outb are stubs so
 * leftover ISA drivers do not dereference low port numbers as pointers.
 */

#define readb(addr)	(*(volatile unsigned char *)(addr))
#define readw(addr)	(*(volatile unsigned short *)(addr))
#define readl(addr)	(*(volatile unsigned int *)(addr))

#define writeb(b, addr)	(*(volatile unsigned char *)(addr) = (b))
#define writew(b, addr)	(*(volatile unsigned short *)(addr) = (b))
#define writel(b, addr)	(*(volatile unsigned int *)(addr) = (b))

#define memset_io(a, b, c)	memset((void *)(a), (b), (c))
#define memcpy_fromio(a, b, c)	memcpy((a), (void *)(b), (c))
#define memcpy_toio(a, b, c)	memcpy((void *)(a), (b), (c))

static inline unsigned char inb(unsigned long port) { return 0xff; }
static inline unsigned short inw(unsigned long port) { return 0xffff; }
static inline unsigned int inl(unsigned long port) { return 0xffffffff; }
static inline void outb(unsigned char v, unsigned long port) { }
static inline void outw(unsigned short v, unsigned long port) { }
static inline void outl(unsigned int v, unsigned long port) { }

#define inb_p(p)	inb(p)
#define inw_p(p)	inw(p)
#define inl_p(p)	inl(p)
#define outb_p(v, p)	outb((v), (p))
#define outw_p(v, p)	outw((v), (p))
#define outl_p(v, p)	outl((v), (p))

#define insb(p, d, c)	do { } while (0)
#define insw(p, d, c)	do { } while (0)
#define insl(p, d, c)	do { } while (0)
#define outsb(p, d, c)	do { } while (0)
#define outsw(p, d, c)	do { } while (0)
#define outsl(p, d, c)	do { } while (0)

static inline unsigned long virt_to_phys(volatile void *address)
{
	return (unsigned long)address;
}

static inline void *phys_to_virt(unsigned long address)
{
	return (void *)address;
}

#define virt_to_bus	virt_to_phys
#define bus_to_virt	phys_to_virt

#define eth_io_copy_and_sum(a, b, c, d) \
	eth_copy_and_sum((a), (void *)(b), (c), (d))

#endif /* _ASM_RISCV_IO_H */
