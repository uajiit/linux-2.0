#
# Makefile for the Linux RISC-V kernel.
#

.S.o:
	$(CC) -D__ASSEMBLY__ -c -o $*.o $<

all: kernel.o head.o

O_TARGET := kernel.o
O_OBJS   := entry.o setup.o traps.o process.o signal.o irq.o time.o \
	    sys_riscv.o sys_call_table.o ptrace.o ksyms.o console.o

head.o: head.S
	$(CC) -D__ASSEMBLY__ -c -o head.o head.S

include $(TOPDIR)/Rules.make
