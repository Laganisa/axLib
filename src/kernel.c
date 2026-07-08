#include "kernel.h"

// 임시로 막아둔거
/*
static inline long do_syscall(long nr,
                              long arg0,
                              long arg1,
                              long arg2,
                              long arg3,
                              long arg4,
                              long arg5)
{
    register long x0 asm("x0") = arg0;
    register long x1 asm("x1") = arg1;
    register long x2 asm("x2") = arg2;
    register long x3 asm("x3") = arg3;
    register long x4 asm("x4") = arg4;
    register long x5 asm("x5") = arg5;
    register long x8 asm("x8") = nr;

    asm volatile(
        "" // 컴파일러에게 레지스터 사용 강제
        : "+r"(x0)
        : "r"(x1),
          "r"(x2),
          "r"(x3),
          "r"(x4),
          "r"(x5),
          "r"(x8)
        : "memory");

    asm volatile(
        "svc #0"
        : "+r"(x0)
        :
        : "memory");

    return x0;
}
*/

/*
    커널과의 시스템 콜을 보내는 파일
*/

static inline long do_syscall(long nr, long arg0, long arg1, long arg2, long arg3, long arg4, long arg5)
{
    register long x0 asm("x0") = arg0;
    register long x1 asm("x1") = arg1;
    register long x2 asm("x2") = arg2;
    register long x3 asm("x3") = arg3;
    register long x4 asm("x4") = arg4;
    register long x5 asm("x5") = arg5;
    register long x8 asm("x8") = nr;

    asm volatile(
        "svc #0"
        : "+r"(x0)                                             // 결과값은 x0로 받음
        : "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5), "r"(x8) // 입력들
        : "memory"                                             // 메모리 변화가 있을 수 있음
    );

    return x0;
}

long axlib_syscall0(long nr)
{
    return do_syscall(nr, 0, 0, 0, 0, 0, 0);
}

long axlib_syscall1(long nr, long arg0)
{
    return do_syscall(nr, arg0, 0, 0, 0, 0, 0);
}

long axlib_syscall2(long nr, long arg0, long arg1)
{
    return do_syscall(nr, arg0, arg1, 0, 0, 0, 0);
}

long axlib_syscall3(long nr, long arg0, long arg1, long arg2)
{
    return do_syscall(nr, arg0, arg1, arg2, 0, 0, 0);
}

long axlib_syscall4(long nr, long arg0, long arg1, long arg2, long arg3)
{
    return do_syscall(nr, arg0, arg1, arg2, arg3, 0, 0);
}

long axlib_syscall5(long nr, long arg0, long arg1, long arg2, long arg3, long arg4)
{
    return do_syscall(nr, arg0, arg1, arg2, arg3, arg4, 0);
}

long axlib_syscall6(long nr, long arg0, long arg1, long arg2, long arg3, long arg4, long arg5)
{
    return do_syscall(nr, arg0, arg1, arg2, arg3, arg4, arg5);
}

long axlib_read(int fd, void *buf, size_t count)
{
    return axlib_syscall3(axLIB_SYS_READ, fd, (long)buf, (long)count);
}

long axlib_write(int fd, const void *buf, size_t count)
{
    return axlib_syscall3(axLIB_SYS_WRITE, fd, (long)buf, (long)count);
}

long axlib_open(const char *path, int flags, int mode)
{
    return axlib_syscall3(axLIB_SYS_OPEN, (long)path, flags, mode);
}

long axlib_close(int fd)
{
    return axlib_syscall1(axLIB_SYS_CLOSE, fd);
}

long axlib_yield(void)
{
    return axlib_syscall0(axLIB_SYS_YIELD);
}

void axlib_exit(int status)
{
    (void)axlib_syscall1(axLIB_SYS_EXIT, status);
}
