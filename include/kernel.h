#ifndef __axLIB_KERNEL_H__
#define __axLIB_KERNEL_H__

#include "types.h"

#define axLIB_SYS_EXIT 1
#define axLIB_SYS_YIELD 4
#define axLIB_SYS_WRITE 6
#define axLIB_SYS_READ 7
#define axLIB_SYS_FILE_CREAT 8
#define axLIB_SYS_FILE_DEL 9
#define axLIB_SYS_OPEN 10
#define axLIB_SYS_CLOSE 11

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
long axlib_read(int fd, void *buf, size_t count);
long axlib_write(int fd, const void *buf, size_t count);
long axlib_open(const char *path, int flags);
long axlib_close(int fd);
long axlib_yield(void);
long axlib_creat(const char *path, int mode, uint32_t size);

// 종료 시스템 콜
void axlib_exit(int status) __attribute__((noreturn));

/* 에러 체크 인라인 함수 */
static inline int axlib_is_error(long ret)
{
    return ret < 0;
}

#endif