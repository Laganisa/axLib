#include "string.h"

/*
    문자열 처리 파일
*/

// 문자열 비교함수
int axlib_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }

    return (unsigned char)*s1 - (unsigned char)*s2;
}

char *axlib_strchr(const char *str, int c)
{
    while (*str)
    {
        if (*str == (char)c)
            return (char *)str;

        str++;
    }

    if (c == '\0')
        return (char *)str;

    return NULL;
}

// 문자열 길이
size_t axlib_strlen(const char *str)
{
    const char *p = str;

    while (*p)
    {
        p++;
    }

    return (size_t)(p - str);
}

/*
    src[start] 부터 src[end-1] 까지 복사
*/
void axlib_strslice(const char *src, char *dst, int start, int end)
{
    int i = 0;

    while ((start < end) && src[start])
    {
        dst[i++] = src[start++];
    }

    dst[i] = '\0';
}