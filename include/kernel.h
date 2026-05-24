#ifndef axLIB_KERNEL_H
#define axLIB_KERNEL_H

#include "../include/types.h"

/*
 * 사용자 라이브러리가 커널의 syscall 디스패치 테이블과 일치하도록,
 * axKernel 빌드에서 이 값들을 덮어써라(재정의하라).
 */

#ifndef axLIB_SYS_READ
#define axLIB_SYS_READ 0
#endif

#ifndef axLIB_SYS_WRITE
#define axLIB_SYS_WRITE 1
#endif

#ifndef axLIB_SYS_OPEN
#define axLIB_SYS_OPEN 2
#endif

#ifndef axLIB_SYS_CLOSE
#define axLIB_SYS_CLOSE 3
#endif

#ifndef axLIB_SYS_EXIT
#define axLIB_SYS_EXIT 4
#endif

#ifndef axLIB_SYS_YIELD
#define axLIB_SYS_YIELD 5
#endif

/* 시스템 콜 하위 인터페이스 (어셈블리 바인딩용) */
long axlib_syscall0(long nr);
long axlib_syscall1(long nr, long arg0);
long axlib_syscall2(long nr, long arg0, long arg1);
long axlib_syscall3(long nr, long arg0, long arg1, long arg2);
long axlib_syscall4(long nr, long arg0, long arg1, long arg2, long arg3);
long axlib_syscall5(long nr, long arg0, long arg1, long arg2, long arg3, long arg4);
long axlib_syscall6(long nr, long arg0, long arg1, long arg2, long arg3, long arg4, long arg5);

/* High-level API (사용자 프로그램용) */
long axlib_read(int fd, void *buf, size_t count);
long axlib_write(int fd, const void *buf, size_t count);
long axlib_open(const char *path, int flags, int mode);
long axlib_close(int fd);
long axlib_yield(void);
void axlib_exit(int status) __attribute__((noreturn));

/* 에러 체크 인라인 함수 */
static inline int axlib_is_error(long ret)
{
    return ret < 0;
}

#endif /* axLIB_KERNEL_H */