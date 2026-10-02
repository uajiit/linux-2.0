#ifndef _ASM_RISCV_FLOPPY_H
#define _ASM_RISCV_FLOPPY_H

/* No PC floppy controller on this machine. */

#define fd_inb(port)			0
#define fd_outb(port, value)		do { } while (0)
#define fd_enable_dma()			do { } while (0)
#define fd_disable_dma()		do { } while (0)
#define fd_request_dma()		(-1)
#define fd_free_dma()			do { } while (0)
#define fd_clear_dma_ff()		do { } while (0)
#define fd_set_dma_mode(mode)		do { } while (0)
#define fd_set_dma_addr(addr)		do { } while (0)
#define fd_set_dma_count(count)		do { } while (0)
#define fd_enable_irq()			do { } while (0)
#define fd_disable_irq()		do { } while (0)
#define fd_cacheflush(addr, size)	do { } while (0)
#define fd_request_irq()		(-1)
#define fd_free_irq()			do { } while (0)

static inline void virtual_dma_init(void) { }

#endif /* _ASM_RISCV_FLOPPY_H */
