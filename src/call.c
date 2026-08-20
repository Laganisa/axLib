#include "call.h"
#include "string.h"
#include "kernel.h"

static int file_chg_auth(uint8_t *auth)
{
    // 크기가 4바이트를 만족시키지 못하면 리턴하기
    if (axlib_strlen(auth) != 4)
    {
        return -1;
    }

    int ret = 0;

    for (int i = 0; i < 4; i++)
    {
        int digit = auth[i] - '0';

        ret = (ret << 3) | (digit & 0x7);
    }
    return ret;
}

/*
    범용 쓰기 함수
    format 형식에 따른 분기
    %d : 10진 정수
    %u : 부호없는 10진 정수
    %x : 16진수 정수
    %s : 문자열
    %c : 문자
*/
void write(int fd, const int8_t *format, const int64_t *text)
{
    // fd 하한 검사
    if (fd < 0)
    {
        return;
    }

    // 10진 정수 (%d)
    if (axlib_strcmp(format, "%d") == 0)
    {
        int64_t n = (int64_t)text; // 만약 주소가 아니라 값 자체를 캐스팅하는 구조라면 유지

        if (n == 0)
        {
            axlib_write(fd, "0", 1); // 0번 fd가 아니라 인자로 받은 fd 사용
            return;
        }

        char buf[24];
        int i = 0;
        int is_negative = 0;

        if (n < 0)
        {
            is_negative = 1;
            n = -n;
        }

        while (n > 0)
        {
            buf[i++] = (n % 10) + '0';
            n /= 10;
        }

        if (is_negative)
            buf[i++] = '-';

        char out[24];
        int out_len = 0;
        for (int j = 0; j < i; j++)
        {
            out[out_len++] = buf[i - 1 - j];
        }
        out[out_len] = '\0';

        axlib_write(fd, out, out_len);
    }
    // 부호 없는 10진 정수 (%u)
    else if (axlib_strcmp(format, "%u") == 0)
    {
        uint64_t n = (uint64_t)text;
        if (n == 0)
        {
            axlib_write(fd, "0", 1);
            return;
        }

        char buf[24];
        int i = 0;
        while (n > 0)
        {
            buf[i++] = (n % 10) + '0';
            n /= 10;
        }

        char out[24];
        int out_len = 0;
        for (int j = 0; j < i; j++)
        {
            out[out_len++] = buf[i - 1 - j];
        }
        out[out_len] = '\0';

        axlib_write(fd, out, out_len);
    }
    // 16진수 정수 (%x)
    else if (axlib_strcmp(format, "%x") == 0)
    {
        uint32_t n = (uint32_t)text;
        if (n == 0)
        {
            axlib_write(fd, "0", 1);
            return;
        }

        char buf[16];
        int i = 0;
        char hex_chars[] = "0123456789ABCDEF";

        while (n > 0)
        {
            buf[i++] = hex_chars[n % 16];
            n /= 16;
        }

        char out[16];
        int out_len = 0;
        for (int j = 0; j < i; j++)
        {
            out[out_len++] = buf[i - 1 - j];
        }
        out[out_len] = '\0';

        axlib_write(fd, out, out_len);
    }
    // 문자열 (%s)
    else if (axlib_strcmp(format, "%s") == 0)
    {
        char *s = (char *)text;
        axlib_write(fd, s, axlib_strlen(s)); // fd 수정
    }
    // 문자 (%c)
    else if (axlib_strcmp(format, "%c") == 0)
    {
        char c = (char)text;
        axlib_write(fd, &c, 1); // 문자의 주소(&c)를 넘기도록 수정
    }
}

/*
    범용 읽기 함수
*/
void read(int fd, char *buf, size_t size)
{
    if (!buf || size == 0)
    {
        return -1;
    }

    size_t used = 0;

    while (used < size - 1)
    {
        char ch;
        long ret = axlib_read(fd, &ch, 1, 0);

        if (ret < 0)
            return ret;

        if (ret == 0)
            // 데이터 없음, 계속 대기
            // ? 나중에 양보 함수 만들 예정
            continue;

        if (ch == '\n' || ch == '\r')
            break;

        buf[used++] = ch;
    }

    buf[used] = '\0';
    return (long)used;
}

/*

*/
void file_creat(const char *path, const char *mode, uint32_t size)
{
    axlib_file_creat(path, file_chg_auth(mode), size);
}

/*

*/
long file_open(const char *path, char mod, uint8_t is_dev)
{
    uint8_t is_read = 0;
    uint8_t is_write = 0;
    uint8_t is_append = 0;

    // 모드 문자 파싱 ('r', 'w', 'a', 'u' 등)
    if (mod == 'r')
    {
        is_read = 1;
    }
    else if (mod == 'w')
    {
        is_write = 1;
    }
    else if (mod == 'a')
    {
        is_write = 1;
        is_append = 1;
    }
    else if (mod == 'u') // 읽고 쓰기
    {
        is_read = 1;
        is_write = 1;
    }

    // 장치 파일인데 이어쓰기 모드인 경우 모순 방지
    if (is_dev != 0 && is_append != 0)
    {
        is_append = 0;
    }

    // 비트 플래그 조합 (이전에 만든 비트 구조 활용)
    uint8_t flag = 0;
    if (is_dev != 0)
        flag |= (1 << 0);
    if (is_write != 0)
        flag |= (1 << 1);
    if (is_read != 0)
        flag |= (1 << 2);
    if (is_append != 0)
        flag |= (1 << 3);

    // 3. 커널/라이브러리 오픈 함수 호출
    return axlib_open(path, flag);
}

/*

*/
void file_close(int fd)
{
    axlib_close(fd);
}

void net_send(uint8_t *data, uint8_t id, uint8_t len, uint16_t type)
{
    axlib_send(data, id, len, type);
}

void kernel_setup(uint8_t *buf, uint8_t rule)
{
    axlib_setup(buf, rule);
}