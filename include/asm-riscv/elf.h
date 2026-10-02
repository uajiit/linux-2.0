#ifndef _ASM_RISCV_ELF_H
#define _ASM_RISCV_ELF_H

/*
 * Official ELF machine number for RISC-V (EM_RISCV).
 */
#define EM_RISCV		243
#define ELF_ARCH		EM_RISCV
#define elf_check_arch(x)	((x) == EM_RISCV)

#define ELF_NGREG	32
#define ELF_NFPREG	32

typedef unsigned long elf_greg_t;
typedef elf_greg_t elf_gregset_t[ELF_NGREG];

typedef double elf_fpreg_t;
typedef elf_fpreg_t elf_fpregset_t[ELF_NFPREG];

#endif /* _ASM_RISCV_ELF_H */
