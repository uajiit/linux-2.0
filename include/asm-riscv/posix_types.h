#ifndef _ASM_RISCV_POSIX_TYPES_H
#define _ASM_RISCV_POSIX_TYPES_H

typedef unsigned short	__kernel_dev_t;
typedef unsigned long	__kernel_ino_t;
typedef unsigned short	__kernel_mode_t;
typedef unsigned short	__kernel_nlink_t;
typedef long		__kernel_off_t;
typedef int		__kernel_pid_t;
typedef unsigned short	__kernel_uid_t;
typedef unsigned short	__kernel_gid_t;
typedef unsigned int	__kernel_size_t;
typedef int		__kernel_ssize_t;
typedef int		__kernel_ptrdiff_t;
typedef long		__kernel_time_t;
typedef long		__kernel_clock_t;
typedef int		__kernel_daddr_t;
typedef char *		__kernel_caddr_t;

#ifdef __GNUC__
typedef long long	__kernel_loff_t;
#endif

typedef struct {
#if defined(__KERNEL__) || defined(__USE_ALL)
	int	val[2];
#else
	int	__val[2];
#endif
} __kernel_fsid_t;

#undef __FD_SET
static inline void __FD_SET(unsigned long fd, __kernel_fd_set *fdsetp)
{
	unsigned long tmp = fd / __NFDBITS;
	unsigned long rem = fd % __NFDBITS;
	fdsetp->fds_bits[tmp] |= (1UL << rem);
}

#undef __FD_CLR
static inline void __FD_CLR(unsigned long fd, __kernel_fd_set *fdsetp)
{
	unsigned long tmp = fd / __NFDBITS;
	unsigned long rem = fd % __NFDBITS;
	fdsetp->fds_bits[tmp] &= ~(1UL << rem);
}

#undef __FD_ISSET
static inline int __FD_ISSET(unsigned long fd, __kernel_fd_set *p)
{
	unsigned long tmp = fd / __NFDBITS;
	unsigned long rem = fd % __NFDBITS;
	return (p->fds_bits[tmp] & (1UL << rem)) != 0;
}

#undef __FD_ZERO
static inline void __FD_ZERO(__kernel_fd_set *p)
{
	unsigned long *tmp = p->fds_bits;
	int i = __FDSET_LONGS;

	while (i) {
		i--;
		*tmp++ = 0;
	}
}

#endif /* _ASM_RISCV_POSIX_TYPES_H */
