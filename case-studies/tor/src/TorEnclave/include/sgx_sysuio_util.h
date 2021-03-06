#ifndef SGX_SYSUIO_UTIL_H
#define SGX_SYSUIO_UTIL_H

#include "proxy/sgx_sysuio_t.h"
#include <sgx_unistd_util.h>
#include <string.h>

static inline ssize_t sgx_wrapper_readv (int __fd, const struct iovec *__iovec, int __count)
{
	ssize_t retval;
	ocall_readv(&retval, __fd, __iovec, sizeof(struct iovec), __count);
	return retval;
}

static inline ssize_t sgx_wrapper_writev (int __fd, const struct iovec *__iovec, int __count)
{
	ssize_t retval;
	int outsize_iovec_id;
	ocall_init_multiple_iovec_outside(&outsize_iovec_id, __iovec, sizeof(struct iovec), __count);
	int i=0;
	for (i=0; i<__count; i++)
		ocall_copy_base_to_outside(outsize_iovec_id, i, __iovec[i].iov_base, __iovec[i].iov_len);

	ocall_writev(&retval, __fd, outsize_iovec_id, sizeof(struct iovec), __count);
	ocall_free_iovec_outside(outsize_iovec_id, sizeof(struct iovec), __count);
	return retval;
}

static inline ssize_t sgx_wrapper_process_vm_readv (pid_t __pid, const struct iovec *__lvec, unsigned long int __liovcnt, 
	const struct iovec *__rvec, unsigned long int __riovcnt, unsigned long int __flags)
{
	ssize_t retval;
	ocall_process_vm_readv(&retval, __pid, __lvec, __liovcnt, __rvec, __riovcnt, __flags);
	return retval;
}

static inline ssize_t sgx_wrapper_process_vm_writev (pid_t __pid, const struct iovec *__lvec, unsigned long int __liovcnt,
	const struct iovec *__rvec, unsigned long int __riovcnt, unsigned long int __flags)
{
	ssize_t retval;
	ocall_process_vm_writev(&retval, __pid, __lvec, __liovcnt, __rvec, __riovcnt, __flags);
	return retval;
}

#define readv(A, B, C) sgx_wrapper_readv(A, B, C)
#define writev(A, B, C) sgx_wrapper_writev(A, B, C)
#define process_vm_readv(A, B, C, D, E, F) sgx_wrapper_process_vm_readv(A, B, C, D, E, F)
#define process_vm_writev(A, B, C, D, E, F) sgx_wrapper_process_vm_writev(A, B, C, D, E, F)

#endif
