#ifndef __axLIB_HASH_H__
#define __axLIB_HASH_H__

#include "types.h"

#define FNV_OFFSET_BASIS_64 0xcbf29ce484222325ULL
#define FNV_PRIME_64 0x100000001b3ULL

// 간단한 헤시 함수
uint64_t axlib_fnv1a_hash_64(const char *str);
uint64_t axlib_djb2_hash_64(const char *str);

#endif