#ifndef __axLIB_KERNEL_H__
#define __axLIB_KERNEL_H__

#include "types.h"

// General System Call (0 ~ 7)

#define axLIB_SYS_RESERVED0 0
#define axLIB_SYS_EXIT 1
#define axLIB_SYS_ABORT 2
#define axLIB_SYS_LOAD 3
#define axLIB_SYS_YIELD 4
#define axLIB_SYS_SETUP 5
#define axLIB_SYS_WRITE 6
#define axLIB_SYS_READ 7

// File System Call (8 ~ 15)

#define axLIB_SYS_OPEN 8
#define axLIB_SYS_CLOSE 9
#define axLIB_SYS_FILE_CREAT 10
#define axLIB_SYS_FILE_DEL 11
#define axLIB_SYS_DIR_CREAT 12
#define axLIB_SYS_DIR_DEL 13
#define axLIB_SYS_RESERVED12 14
#define axLIB_SYS_RESERVED15 15

// Process System Call (16 ~ 23)

#define axLIB_SYS_PROC_CREAT 16
#define axLIB_SYS_PROC_DEL 17
#define axLIB_SYS_RESERVED18 18
#define axLIB_SYS_RESERVED19 19
#define axLIB_SYS_RESERVED20 20
#define axLIB_SYS_RESERVED21 21
#define axLIB_SYS_RESERVED22 22
#define axLIB_SYS_RESERVED23 23

// IPC System Call (24 ~ 31)

#define axLIB_SYS_RESERVED24 24
#define axLIB_SYS_RESERVED25 25
#define axLIB_SYS_RESERVED26 26
#define axLIB_SYS_RESERVED27 27
#define axLIB_SYS_RESERVED28 28
#define axLIB_SYS_RESERVED29 29
#define axLIB_SYS_RESERVED30 30
#define axLIB_SYS_RESERVED31 31

// Network System Call (32 ~ 39)

#define axLIB_SYS_SEND_L2 32
#define axLIB_SYS_RESERVED33 33
#define axLIB_SYS_RESERVED34 34
#define axLIB_SYS_RESERVED35 35
#define axLIB_SYS_RESERVED36 36
#define axLIB_SYS_RESERVED37 37
#define axLIB_SYS_RESERVED38 38
#define axLIB_SYS_RESERVED39 39

/*
    시스템 콜 하위 인터페이스 (어셈블리 바인딩용)
*/
long axlib_syscall0(long nr);
long axlib_syscall1(long nr, long arg0);
long axlib_syscall2(long nr, long arg0, long arg1);
long axlib_syscall3(long nr, long arg0, long arg1, long arg2);
long axlib_syscall4(long nr, long arg0, long arg1, long arg2, long arg3);
long axlib_syscall5(long nr, long arg0, long arg1, long arg2, long arg3, long arg4);
long axlib_syscall6(long nr, long arg0, long arg1, long arg2, long arg3, long arg4, long arg5);

/*
    유저 프로세스 용 API
*/

// General System Call (0 ~ 7)
void axlib_exit(int status) __attribute__((noreturn));

void axlib_setup(uint64_t *buf, uint8_t rule);
long axlib_yield(void);
long axlib_read(int fd, void *buf, size_t count, uint32_t offset);
long axlib_write(int fd, const void *buf, size_t count);

// File System Call (8 ~ 15)

long axlib_file_creat(const char *path, int mode, uint32_t size);
long axlib_open(const char *path, int flags);
long axlib_close(int fd);

// Process System Call (16 ~ 23)

// IPC System Call (24 ~ 31)

// Network System Call (32 ~ 39)

void axlib_send(uint8_t *data, uint8_t id, uint8_t len, uint16_t type);

/* 에러 체크 인라인 함수 */
static inline int axlib_is_error(long ret)
{
    return ret < 0;
}

#endif