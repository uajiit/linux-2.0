#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/malloc.h>
#include <linux/kernel_stat.h>
#include <asm/system.h>
#include <asm/irq.h>

static struct irqaction *irq_action[NR_IRQS];

int get_irq_list(char *buf)
{
	int i, len = 0;
	struct irqaction *action;

	for (i = 0; i < NR_IRQS; i++) {
		action = irq_action[i];
		if (!action)
			continue;
		len += sprintf(buf + len, "%2d: %8d %s\n",
			i, kstat.interrupts[i], action->name);
	}
	return len;
}

void disable_irq(unsigned int irq)
{
}

void enable_irq(unsigned int irq)
{
}

int request_irq(unsigned int irq,
		void (*handler)(int, void *, struct pt_regs *),
		unsigned long flags, const char *name, void *dev_id)
{
	struct irqaction *action;

	if (irq >= NR_IRQS)
		return -EINVAL;
	if (!handler)
		return -EINVAL;
	action = (struct irqaction *)kmalloc(sizeof(struct irqaction), GFP_KERNEL);
	if (!action)
		return -ENOMEM;
	action->handler = handler;
	action->flags = flags;
	action->mask = 0;
	action->name = name;
	action->dev_id = dev_id;
	action->next = NULL;
	irq_action[irq] = action;
	return 0;
}

void free_irq(unsigned int irq, void *dev_id)
{
	if (irq >= NR_IRQS)
		return;
	if (irq_action[irq]) {
		kfree(irq_action[irq]);
		irq_action[irq] = NULL;
	}
}

unsigned long probe_irq_on(void)
{
	return 0;
}

int probe_irq_off(unsigned long irqs)
{
	return 0;
}

/*
 * Machine-timer interrupts are not routed through request_irq. Enable
 * the CLINT timer bit in mie; start_kernel() calls sti() later.
 */
void init_IRQ(void)
{
	asm volatile("csrs mie, %0" : : "r" (1 << IRQ_TIMER));
}
