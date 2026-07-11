#ifndef _STRING_H_
#define _STRING_H_

#include "types.h"

// 문자 비교
int axlib_strcmp(const char *s1, const char *s2);

char *axlib_strchr(const char *str, int c);

// 문자 자르기
void axlib_strslice(const char *src, char *dst, int start, int end);

size_t axlib_strlen(const char *str);

#endif