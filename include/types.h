#ifndef __TYPES_H__
#define __TYPES_H__

// 기본 데이터 타입 정의
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef unsigned long size_t;
typedef long ssize_t;

// 포인터 타입
typedef unsigned long uintptr_t;
typedef long intptr_t;

// NULL 정의
#define NULL ((void *)0)

// TRUE / FALSE
#define TRUE 1
#define FALSE 0

#endif // __TYPES_H__