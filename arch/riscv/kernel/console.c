/*
 * Console for the 16550 UART at 0x10000000 on QEMU's virt machine.
 * The VGA/keyboard console in drivers/char is not built for this port.
 */

#include <linux/errno.h>
#include <linux/tty.h>
#include <linux/kernel.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/wait.h>

extern void register_console(void (*proc)(const char *));

/*
 * Layout matches drivers/char/vt_kern.h. That header lives outside
 * include/, so the type is repeated here.
 */
struct vt_struct {
	int vc_num;
	unsigned char vc_mode;
	unsigned char vc_kbdraw;
	unsigned char vc_kbde0;
	unsigned char vc_kbdleds;
	struct vt_mode vt_mode;
	int vt_pid;
	int vt_newvt;
	struct wait_queue *paste_wait;
};
#include <linux/sched.h>

#define UART_BASE	0x10000000UL
#define UART_THR	0
#define UART_IER	1
#define UART_FCR	2
#define UART_LCR	3
#define UART_LSR	5
#define LSR_THRE	0x20

struct screen_info screen_info = {
	0, 0, {0, 0}, 0, 0, 80, 0, 0, 0, 25, 1, 16
};

unsigned char aux_device_present;
unsigned char kbd_read_mask;
int shift_state;

/*
 * Same layout as drivers/char/kbd_kern.h. tty_io writes kbdmode.
 */
struct kbd_struct {
	unsigned char lockstate;
	unsigned char slockstate;
	unsigned char ledmode:2;
	unsigned char ledflagstate:3;
	unsigned char default_ledflagstate:3;
	unsigned char kbdmode:2;
	unsigned char modeflags:5;
};

struct kbd_struct kbd_table[MAX_NR_CONSOLES];

void no_scroll(char *str, int *ints)
{
}

void console_map_init(void)
{
}

void set_palette(void)
{
}

void reset_palette(int currcons)
{
}

int vc_cons_allocated(unsigned int console)
{
	return console < MAX_NR_CONSOLES;
}

void update_screen(int currcons)
{
}

void do_blank_screen(int nopowersave)
{
}

int set_selection(unsigned long arg, struct tty_struct *tty, int user)
{
	return -EINVAL;
}

int paste_selection(struct tty_struct *tty)
{
	return 0;
}

int sel_loadlut(unsigned long arg)
{
	return 0;
}

int mouse_reporting(void)
{
	return 0;
}

void set_vesa_blanking(unsigned long arg)
{
}

int vcs_init(void)
{
	return 0;
}

static struct vt_struct riscv_vt[MAX_NR_CONSOLES];
struct vt_struct *vt_cons[MAX_NR_CONSOLES];

static volatile unsigned char *uart = (volatile unsigned char *)UART_BASE;

void riscv_uart_init(void)
{
	uart[UART_LCR] = 0x80;		/* DLAB */
	uart[UART_THR] = 1;		/* divisor 1: QEMU accepts any baud */
	uart[UART_IER] = 0;
	uart[UART_LCR] = 0x03;		/* 8n1 */
	uart[UART_FCR] = 0x07;
	uart[UART_IER] = 0;
}

void riscv_putc(char c)
{
	int i;

	if (c == '\n')
		riscv_putc('\r');
	for (i = 0; i < 100000; i++) {
		if (uart[UART_LSR] & LSR_THRE)
			break;
	}
	uart[UART_THR] = c;
}

void console_print(const char *b)
{
	while (*b)
		riscv_putc(*b++);
}

void do_unblank_screen(void)
{
}

int kbd_init(void)
{
	return 0;
}

unsigned long con_init(unsigned long kmem_start)
{
	int i;

	riscv_uart_init();
	for (i = 0; i < MAX_NR_CONSOLES; i++) {
		riscv_vt[i].vc_mode = KD_TEXT;
		riscv_vt[i].vt_mode.mode = VT_AUTO;
		riscv_vt[i].vt_pid = -1;
		riscv_vt[i].vt_newvt = -1;
		vt_cons[i] = &riscv_vt[i];
	}
	register_console(console_print);
	return kmem_start;
}
