#ifndef __axLIB_CALL_H__
#define __axLIB_CALL_H__

#include "types.h"

// 입출력 디스크럽터 위치
#define STDIO 0

void write(int fd, const int8_t *format, const int64_t *text);
void read(int fd, char *buf, size_t size);
void file_creat(const char *path, const char *mode, uint32_t size);
long file_open(const char *path, char mod, uint8_t is_dev);
void file_close(int fd);

void kernel_setup(uint8_t *buf, uint8_t rule);
void net_send(uint8_t *data, uint8_t id, uint8_t len, uint16_t type);

#endif