#ifndef __AXLIB_MEMORY_H__
#define __AXLIB_MEMORY_H__

#include "types.h"

void *axlib_memset(void *dest, int value, size_t count);
void *axlib_memcpy(void *dest, const void *src, size_t count);
void *axlib_memmove(void *dest, const void *src, size_t count);
int axlib_memcmp(const void *lhs, const void *rhs, size_t count);

void *axlib_malloc(size_t size);
void axlib_free(void *ptr);

#endif
