#ifndef SGX_TIME_T_H__
#define SGX_TIME_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */

#include "time.h"

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif



sgx_status_t SGX_CDECL ocall_time(time_t* retval, time_t* t);
sgx_status_t SGX_CDECL ocall_gettimeofday(int* retval, void* tv, void* tz);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
