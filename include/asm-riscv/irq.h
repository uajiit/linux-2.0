#ifndef _ASM_RISCV_IRQ_H
#define _ASM_RISCV_IRQ_H

/*
 * The machine timer is dispatched directly from the trap handler.
 * These numbers are for request_irq clients (PLIC devices later).
 */
#define NR_IRQS		32
#define IRQ_TIMER	7

extern void disable_irq(unsigned int irq);
extern void enable_irq(unsigned int irq);

#endif /* _ASM_RISCV_IRQ_H */
