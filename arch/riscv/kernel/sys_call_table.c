#include <linux/sys.h>
#include <linux/config.h>

#define _S(x) (void *)(x)

extern int sys_setup();
extern int sys_exit();
extern int sys_fork();
extern int sys_read();
extern int sys_write();
extern int sys_open();
extern int sys_close();
extern int sys_waitpid();
extern int sys_creat();
extern int sys_link();
extern int sys_unlink();
extern int sys_execve();
extern int sys_chdir();
extern int sys_time();
extern int sys_mknod();
extern int sys_chmod();
extern int sys_chown();
extern int sys_break();
extern int sys_stat();
extern int sys_lseek();
extern int sys_getpid();
extern int sys_mount();
extern int sys_umount();
extern int sys_setuid();
extern int sys_getuid();
extern int sys_stime();
extern int sys_ptrace();
extern int sys_alarm();
extern int sys_fstat();
extern int sys_pause();
extern int sys_utime();
extern int sys_stty();
extern int sys_gtty();
extern int sys_access();
extern int sys_nice();
extern int sys_ftime();
extern int sys_sync();
extern int sys_kill();
extern int sys_rename();
extern int sys_mkdir();
extern int sys_rmdir();
extern int sys_dup();
extern int sys_pipe();
extern int sys_times();
extern int sys_prof();
extern int sys_brk();
extern int sys_setgid();
extern int sys_getgid();
extern int sys_signal();
extern int sys_geteuid();
extern int sys_getegid();
extern int sys_acct();
extern int sys_phys();
extern int sys_lock();
extern int sys_ioctl();
extern int sys_fcntl();
extern int sys_mpx();
extern int sys_setpgid();
extern int sys_ulimit();
extern int sys_olduname();
extern int sys_umask();
extern int sys_chroot();
extern int sys_ustat();
extern int sys_dup2();
extern int sys_getppid();
extern int sys_getpgrp();
extern int sys_setsid();
extern int sys_sigaction();
extern int sys_sgetmask();
extern int sys_ssetmask();
extern int sys_setreuid();
extern int sys_setregid();
extern int sys_sigsuspend();
extern int sys_sigpending();
extern int sys_sethostname();
extern int sys_setrlimit();
extern int sys_getrlimit();
extern int sys_getrusage();
extern int sys_gettimeofday();
extern int sys_settimeofday();
extern int sys_getgroups();
extern int sys_setgroups();
extern int old_select();
extern int sys_symlink();
extern int sys_lstat();
extern int sys_readlink();
extern int sys_uselib();
extern int sys_swapon();
extern int sys_reboot();
extern int old_readdir();
extern int old_mmap();
extern int sys_munmap();
extern int sys_truncate();
extern int sys_ftruncate();
extern int sys_fchmod();
extern int sys_fchown();
extern int sys_getpriority();
extern int sys_setpriority();
extern int sys_profil();
extern int sys_statfs();
extern int sys_fstatfs();
extern int sys_ioperm();
extern int sys_socketcall();
extern int sys_syslog();
extern int sys_setitimer();
extern int sys_getitimer();
extern int sys_newstat();
extern int sys_newlstat();
extern int sys_newfstat();
extern int sys_uname();
extern int sys_iopl();
extern int sys_vhangup();
extern int sys_idle();
extern int sys_vm86();
extern int sys_wait4();
extern int sys_swapoff();
extern int sys_sysinfo();
extern int sys_fsync();
extern int sys_sigreturn();
extern int sys_clone();
extern int sys_setdomainname();
extern int sys_newuname();
extern int sys_modify_ldt();
extern int sys_adjtimex();
extern int sys_mprotect();
extern int sys_sigprocmask();
extern int sys_quotactl();
extern int sys_getpgid();
extern int sys_fchdir();
extern int sys_bdflush();
extern int sys_sysfs();
extern int sys_personality();
extern int sys_setfsuid();
extern int sys_setfsgid();
extern int sys_llseek();
extern int sys_getdents();
extern int sys_select();
extern int sys_flock();
extern int sys_msync();
extern int sys_readv();
extern int sys_writev();
extern int sys_getsid();
extern int sys_fdatasync();
extern int sys_sysctl();
extern int sys_mlock();
extern int sys_munlock();
extern int sys_mlockall();
extern int sys_munlockall();
extern int sys_sched_setparam();
extern int sys_sched_getparam();
extern int sys_sched_setscheduler();
extern int sys_sched_getscheduler();
extern int sys_sched_yield();
extern int sys_sched_get_priority_max();
extern int sys_sched_get_priority_min();
extern int sys_sched_rr_get_interval();
extern int sys_nanosleep();
extern int sys_mremap();
extern int sys_ni_syscall();

#ifdef CONFIG_MODULES
extern int sys_create_module();
extern int sys_init_module();
extern int sys_delete_module();
extern int sys_get_kernel_syms();
#define SYS_CREATE	sys_create_module
#define SYS_INIT	sys_init_module
#define SYS_DELETE	sys_delete_module
#define SYS_GETSYMS	sys_get_kernel_syms
#else
#define SYS_CREATE	sys_ni_syscall
#define SYS_INIT	sys_ni_syscall
#define SYS_DELETE	sys_ni_syscall
#define SYS_GETSYMS	sys_ni_syscall
#endif

#ifdef CONFIG_SYSVIPC
extern int sys_ipc();
#define SYS_IPC		sys_ipc
#else
#define SYS_IPC		sys_ni_syscall
#endif

void *sys_call_table[NR_syscalls] = {
	_S(sys_setup),			/* 0 */
	_S(sys_exit),
	_S(sys_fork),
	_S(sys_read),
	_S(sys_write),
	_S(sys_open),			/* 5 */
	_S(sys_close),
	_S(sys_waitpid),
	_S(sys_creat),
	_S(sys_link),
	_S(sys_unlink),			/* 10 */
	_S(sys_execve),
	_S(sys_chdir),
	_S(sys_time),
	_S(sys_mknod),
	_S(sys_chmod),			/* 15 */
	_S(sys_chown),
	_S(sys_break),
	_S(sys_stat),
	_S(sys_lseek),
	_S(sys_getpid),			/* 20 */
	_S(sys_mount),
	_S(sys_umount),
	_S(sys_setuid),
	_S(sys_getuid),
	_S(sys_stime),			/* 25 */
	_S(sys_ptrace),
	_S(sys_alarm),
	_S(sys_fstat),
	_S(sys_pause),
	_S(sys_utime),			/* 30 */
	_S(sys_stty),
	_S(sys_gtty),
	_S(sys_access),
	_S(sys_nice),
	_S(sys_ftime),			/* 35 */
	_S(sys_sync),
	_S(sys_kill),
	_S(sys_rename),
	_S(sys_mkdir),
	_S(sys_rmdir),			/* 40 */
	_S(sys_dup),
	_S(sys_pipe),
	_S(sys_times),
	_S(sys_prof),
	_S(sys_brk),			/* 45 */
	_S(sys_setgid),
	_S(sys_getgid),
	_S(sys_signal),
	_S(sys_geteuid),
	_S(sys_getegid),		/* 50 */
	_S(sys_acct),
	_S(sys_phys),
	_S(sys_lock),
	_S(sys_ioctl),
	_S(sys_fcntl),			/* 55 */
	_S(sys_mpx),
	_S(sys_setpgid),
	_S(sys_ulimit),
	_S(sys_olduname),
	_S(sys_umask),			/* 60 */
	_S(sys_chroot),
	_S(sys_ustat),
	_S(sys_dup2),
	_S(sys_getppid),
	_S(sys_getpgrp),		/* 65 */
	_S(sys_setsid),
	_S(sys_sigaction),
	_S(sys_sgetmask),
	_S(sys_ssetmask),
	_S(sys_setreuid),		/* 70 */
	_S(sys_setregid),
	_S(sys_sigsuspend),
	_S(sys_sigpending),
	_S(sys_sethostname),
	_S(sys_setrlimit),		/* 75 */
	_S(sys_getrlimit),
	_S(sys_getrusage),
	_S(sys_gettimeofday),
	_S(sys_settimeofday),
	_S(sys_getgroups),		/* 80 */
	_S(sys_setgroups),
	_S(old_select),
	_S(sys_symlink),
	_S(sys_lstat),
	_S(sys_readlink),		/* 85 */
	_S(sys_uselib),
	_S(sys_swapon),
	_S(sys_reboot),
	_S(old_readdir),
	_S(old_mmap),			/* 90 */
	_S(sys_munmap),
	_S(sys_truncate),
	_S(sys_ftruncate),
	_S(sys_fchmod),
	_S(sys_fchown),			/* 95 */
	_S(sys_getpriority),
	_S(sys_setpriority),
	_S(sys_profil),
	_S(sys_statfs),
	_S(sys_fstatfs),		/* 100 */
	_S(sys_ioperm),
	_S(sys_socketcall),
	_S(sys_syslog),
	_S(sys_setitimer),
	_S(sys_getitimer),		/* 105 */
	_S(sys_newstat),
	_S(sys_newlstat),
	_S(sys_newfstat),
	_S(sys_uname),
	_S(sys_iopl),			/* 110 */
	_S(sys_vhangup),
	_S(sys_idle),
	_S(sys_vm86),
	_S(sys_wait4),
	_S(sys_swapoff),		/* 115 */
	_S(sys_sysinfo),
	_S(SYS_IPC),
	_S(sys_fsync),
	_S(sys_sigreturn),
	_S(sys_clone),			/* 120 */
	_S(sys_setdomainname),
	_S(sys_newuname),
	_S(sys_modify_ldt),
	_S(sys_adjtimex),
	_S(sys_mprotect),		/* 125 */
	_S(sys_sigprocmask),
	_S(SYS_CREATE),
	_S(SYS_INIT),
	_S(SYS_DELETE),
	_S(SYS_GETSYMS),		/* 130 */
	_S(sys_quotactl),
	_S(sys_getpgid),
	_S(sys_fchdir),
	_S(sys_bdflush),
	_S(sys_sysfs),			/* 135 */
	_S(sys_personality),
	_S(sys_ni_syscall),
	_S(sys_setfsuid),
	_S(sys_setfsgid),
	_S(sys_llseek),			/* 140 */
	_S(sys_getdents),
	_S(sys_select),
	_S(sys_flock),
	_S(sys_msync),
	_S(sys_readv),			/* 145 */
	_S(sys_writev),
	_S(sys_getsid),
	_S(sys_fdatasync),
	_S(sys_sysctl),
	_S(sys_mlock),			/* 150 */
	_S(sys_munlock),
	_S(sys_mlockall),
	_S(sys_munlockall),
	_S(sys_sched_setparam),
	_S(sys_sched_getparam),		/* 155 */
	_S(sys_sched_setscheduler),
	_S(sys_sched_getscheduler),
	_S(sys_sched_yield),
	_S(sys_sched_get_priority_max),
	_S(sys_sched_get_priority_min),	/* 160 */
	_S(sys_sched_rr_get_interval),
	_S(sys_nanosleep),
	_S(sys_mremap)
};
