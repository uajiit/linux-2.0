#ifndef _ASM_RISCV_DMA_H
#define _ASM_RISCV_DMA_H

#include <asm/io.h>

#define MAX_DMA_CHANNELS	8
#define MAX_DMA_ADDRESS		(~0UL)

#define dma_outb	outb
#define dma_inb		inb

/* No ISA DMA controller. The allocator in kernel/dma.c still uses the count. */

#endif /* _ASM_RISCV_DMA_H */
