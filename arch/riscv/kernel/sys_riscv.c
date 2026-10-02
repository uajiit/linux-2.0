#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/fs.h>
#include <asm/segment.h>

extern int do_pipe(int *fd);
extern asmlinkage int sys_select(int n, fd_set *inp, fd_set *outp,
				 fd_set *exp, struct timeval *tvp);

asmlinkage int sys_pipe(int *fildes)
{
	int error;

	error = verify_area(VERIFY_WRITE, fildes, 2 * sizeof(int));
	if (error)
		return error;
	return do_pipe(fildes);
}

/*
 * i386 passed mmap and select through a memory block because the
 * syscall ABI had only a few registers. The numbers are unchanged,
 * so these wrappers still unpack that block.
 */
asmlinkage int old_mmap(unsigned long *buffer)
{
	int error;
	unsigned long flags;
	struct file *file = NULL;

	error = verify_area(VERIFY_READ, buffer, 6 * sizeof(unsigned long));
	if (error)
		return error;
	flags = get_user(buffer + 3);
	if (!(flags & MAP_ANONYMOUS)) {
		unsigned long fd = get_user(buffer + 4);
		if (fd >= NR_OPEN || !(file = current->files->fd[fd]))
			return -EBADF;
	}
	flags &= ~(MAP_EXECUTABLE | MAP_DENYWRITE);
	return do_mmap(file, get_user(buffer), get_user(buffer + 1),
		       get_user(buffer + 2), flags, get_user(buffer + 5));
}

asmlinkage int old_select(unsigned long *buffer)
{
	int error;
	int n;
	fd_set *inp, *outp, *exp;
	struct timeval *tvp;

	error = verify_area(VERIFY_READ, buffer, 5 * sizeof(unsigned long));
	if (error)
		return error;
	n = get_user(buffer);
	inp = (fd_set *)get_user(buffer + 1);
	outp = (fd_set *)get_user(buffer + 2);
	exp = (fd_set *)get_user(buffer + 3);
	tvp = (struct timeval *)get_user(buffer + 4);
	return sys_select(n, inp, outp, exp, tvp);
}
