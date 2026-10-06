#include "call.h"
#include "string.h"
#include "kernel.h"
#include <stdarg.h>

static int file_chg_auth(const char *auth)
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
void write(int fd, const char *format, ...)
{
    if (fd < 0 || format == NULL)
    {
        return;
    }

    va_list write_args;
    va_start(write_args, format);

    // 10진 정수 (%d)
    if (axlib_strcmp(format, "%d") == 0)
    {
        int64_t n = va_arg(write_args, int);

        if (n == 0)
        {
            axlib_write(fd, "0", 1);
        }
        else
        {
            char write_number_buffer[24];
            char write_output_buffer[24];
            uint64_t write_magnitude = n < 0
                                           ? (uint64_t)(-(n + 1)) + 1
                                           : (uint64_t)n;
            int write_number_length = 0;

            while (write_magnitude > 0)
            {
                write_number_buffer[write_number_length++] =
                    (char)(write_magnitude % 10) + '0';
                write_magnitude /= 10;
            }

            int write_output_length = 0;
            if (n < 0)
            {
                write_output_buffer[write_output_length++] = '-';
            }
            while (write_number_length > 0)
            {
                write_output_buffer[write_output_length++] =
                    write_number_buffer[--write_number_length];
            }

            axlib_write(fd, write_output_buffer, write_output_length);
        }
    }
    // 부호 없는 10진 정수 (%u)
    else if (axlib_strcmp(format, "%u") == 0)
    {
        uint64_t n = va_arg(write_args, unsigned int);
        if (n == 0)
        {
            axlib_write(fd, "0", 1);
        }
        else
        {
            char write_number_buffer[24];
            char write_output_buffer[24];
            int write_number_length = 0;

            while (n > 0)
            {
                write_number_buffer[write_number_length++] =
                    (char)(n % 10) + '0';
                n /= 10;
            }

            int write_output_length = 0;
            while (write_number_length > 0)
            {
                write_output_buffer[write_output_length++] =
                    write_number_buffer[--write_number_length];
            }
            axlib_write(fd, write_output_buffer, write_output_length);
        }
    }
    // 16진수 정수 (%x)
    else if (axlib_strcmp(format, "%x") == 0)
    {
        uint32_t n = va_arg(write_args, unsigned int);
        if (n == 0)
        {
            axlib_write(fd, "0", 1);
        }
        else
        {
            char write_number_buffer[16];
            char write_output_buffer[16];
            int write_number_length = 0;
            const char write_hex_digits[] = "0123456789ABCDEF";

            while (n > 0)
            {
                write_number_buffer[write_number_length++] =
                    write_hex_digits[n % 16];
                n /= 16;
            }

            int write_output_length = 0;
            while (write_number_length > 0)
            {
                write_output_buffer[write_output_length++] =
                    write_number_buffer[--write_number_length];
            }
            axlib_write(fd, write_output_buffer, write_output_length);
        }
    }
    // 문자열 (%s)
    else if (axlib_strcmp(format, "%s") == 0)
    {
        const char *write_text = va_arg(write_args, const char *);
        if (write_text != NULL)
        {
            axlib_write(fd, write_text, axlib_strlen(write_text));
        }
    }
    // 문자 (%c)
    else if (axlib_strcmp(format, "%c") == 0)
    {
        char write_character = (char)va_arg(write_args, int);
        axlib_write(fd, &write_character, 1);
    }

    va_end(write_args);
}

long read(int fd, char *buf, size_t size)
{
    if (!buf || size == 0)
    {
        return -1;
    }

    if (size == 1)
    {
        buf[0] = '\0';
        return 0;
    }

    long ret = axlib_read(fd, buf, size - 1, 0);
    if (ret < 0)
    {
        return ret;
    }

    size_t used = (size_t)ret;

    while (used > 0 && (buf[used - 1] == '\n' || buf[used - 1] == '\r'))
    {
        used--;
    }

    buf[used] = '\0';

    return ret;
}

/*

*/
void file_create(const char *path, const char *mode, uint32_t size)
{
    axlib_file_create(path, file_chg_auth(mode), size);
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
    axlib_l2_send(data, id, len, type);
}

void kernel_setup(uint8_t *buf, uint8_t rule)
{
    axlib_setup((uint64_t *)buf, rule);
}

void ipc_send(
    uint8_t *data,
    uint8_t len,
    uint8_t towho)
{
    axlib_ipc_send(data, len, towho);
}

void ipc_rece(
    uint8_t *data)
{
    axlib_ipc_rece(data);
}