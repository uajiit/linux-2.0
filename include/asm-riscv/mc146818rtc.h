#ifndef _ASM_RISCV_MC146818RTC_H
#define _ASM_RISCV_MC146818RTC_H

/* No PC CMOS RTC. Time comes from the CLINT. */

#define RTC_PORT(x)	(0x70 + (x))
#define RTC_ALWAYS_BCD	1

#endif /* _ASM_RISCV_MC146818RTC_H */
