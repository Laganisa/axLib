#include "kernel.h"
#include "call.h"
#include "debug.h"

// 배열과 그 (크기, 구조체 멤버의 크기)
// 첫인자는
void arr_dump(void *arr, const uint8_t size, const char *format, ...)
{
    va_list args;

    for (int i = 0; i < size; i++)
    {
        va_start(args, format);

        char *ptr = (char *)arr; // 메모리를 바이트 단위로 움직이면서 읽기 위함

        while (*format != '\0')
        {
            // 공백이나 구분자는 건너뛰기 등 처리 가능
            if (*format == ' ')
            {
                format++;
                continue;
            }

            // 예를 들어 문자가 '1', '2', '4', '8'을 의미한다고 가정할 때
            int size = *format - '0';

            switch (size)
            {
            case 1:
            {
                // 1바이트 (char 등) -> 승격 규칙 때문에 int로 받음
                int val = va_arg(args, int);
                write(0, "%d", val);
                ptr += 1;
                break;
            }
            case 2:
            {
                // 2바이트 (short 등) -> 역시 int로 받음
                int val = va_arg(args, int);
                write(0, "%d", val);
                ptr += 2;
                break;
            }
            case 4:
            {
                // 4바이트 (int 등)
                int val = va_arg(args, int);
                write(0, "%d", val);
                ptr += 4;
                break;
            }
            case 8:
            {
                // 8바이트 (long long, 포인터 등)
                long long val = va_arg(args, long long);
                write(0, "%d", val);
                ptr += 8;
                break;
            }
            default:
                // 잘못된 크기 처리
                break;
            }
            format++;
        }
    }
    va_end(args);
}