#
# arch/riscv/Makefile
#
# RV32 Linux 2.0, built to run in M-mode on QEMU's virt machine.
#
#   cp arch/riscv/defconfig .config
#   make ARCH=riscv oldconfig
#   make ARCH=riscv vmlinux
#   qemu-system-riscv32 -M virt -bios none -nographic -m 32M -kernel vmlinux
#

ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE := $(shell \
	if command -v riscv32-unknown-elf-gcc >/dev/null 2>&1; then \
		echo riscv32-unknown-elf-; \
	elif command -v riscv64-linux-gnu-gcc >/dev/null 2>&1; then \
		echo riscv64-linux-gnu-; \
	elif [ -x $(HOME)/zephyr-sdk-1.0.1/gnu/riscv64-zephyr-elf/bin/riscv64-zephyr-elf-gcc ]; then \
		echo $(HOME)/zephyr-sdk-1.0.1/gnu/riscv64-zephyr-elf/bin/riscv64-zephyr-elf-; \
	fi)
endif

ARCH_FLAGS = -march=rv32ima_zicsr -mabi=ilp32 -mcmodel=medany \
	-mno-relax -msmall-data-limit=0

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld -m elf32lriscv
CC		= $(CROSS_COMPILE)gcc -D__KERNEL__ -I$(HPATH) $(ARCH_FLAGS)
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip

# GCC 14 rejects the implicit declarations this tree relies on.
# -traditional is dropped for this architecture (see AS_NO_TRADITIONAL).
CFLAGS		= -Wall -O2 -fomit-frame-pointer -fsigned-char -fcommon \
		-fno-stack-protector -fno-builtin -ffreestanding \
		-std=gnu89 -fgnu89-inline \
		-Wno-error=implicit-function-declaration \
		-Wno-implicit-function-declaration \
		-Wno-implicit-int -Wno-int-conversion \
		-Wno-incompatible-pointer-types \
		-Wno-strict-prototypes -Wno-unused

AS_NO_TRADITIONAL = 1

HEAD := arch/riscv/kernel/head.o

ARCH_SUBDIRS = arch/riscv/kernel arch/riscv/mm arch/riscv/lib
SUBDIRS := $(SUBDIRS) $(ARCH_SUBDIRS)
ARCHIVES := arch/riscv/kernel/kernel.o arch/riscv/mm/mm.o $(ARCHIVES)

LIBGCC := $(shell $(CROSS_COMPILE)gcc $(ARCH_FLAGS) -print-libgcc-file-name)
LIBS := $(LIBS) arch/riscv/lib/lib.a $(LIBGCC)

LINKFLAGS = -T arch/riscv/ld.script --no-relax

archclean:
	rm -f arch/riscv/*/*.o arch/riscv/*/*.a

archdep:
