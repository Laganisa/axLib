#include "kernel.h"
#include "memory.h"

void *axlib_memset(void *dest, int value, size_t count)
{
    uint8_t *bytes = (uint8_t *)dest;
    for (size_t i = 0; i < count; i++)
    {
        bytes[i] = (uint8_t)value;
    }
    return dest;
}

void *axlib_memcpy(void *dest, const void *src, size_t count)
{
    uint8_t *dest_bytes = (uint8_t *)dest;
    const uint8_t *src_bytes = (const uint8_t *)src;

    for (size_t i = 0; i < count; i++)
    {
        dest_bytes[i] = src_bytes[i];
    }
    return dest;
}

void *axlib_memmove(void *dest, const void *src, size_t count)
{
    uint8_t *dest_bytes = (uint8_t *)dest;
    const uint8_t *src_bytes = (const uint8_t *)src;
    uintptr_t dest_addr = (uintptr_t)dest;
    uintptr_t src_addr = (uintptr_t)src;

    if (dest_addr <= src_addr || dest_addr - src_addr >= count)
    {
        for (size_t i = 0; i < count; i++)
        {
            dest_bytes[i] = src_bytes[i];
        }
    }
    else
    {
        for (size_t i = count; i > 0; i--)
        {
            dest_bytes[i - 1] = src_bytes[i - 1];
        }
    }
    return dest;
}

int axlib_memcmp(const void *lhs, const void *rhs, size_t count)
{
    const uint8_t *lhs_bytes = (const uint8_t *)lhs;
    const uint8_t *rhs_bytes = (const uint8_t *)rhs;

    for (size_t i = 0; i < count; i++)
    {
        if (lhs_bytes[i] != rhs_bytes[i])
        {
            return (int)lhs_bytes[i] - (int)rhs_bytes[i];
        }
    }
    return 0;
}

typedef struct axlib_heap_block_t
{
    size_t size;
    struct axlib_heap_block_t *next;
    struct axlib_heap_block_t *prev;
    uint8_t is_free;
} axlib_heap_block_t;

#define HEAP_ALIGNMENT 16ULL

static axlib_heap_block_t *heap_head;
static axlib_heap_block_t *heap_tail;

void *axlib_malloc(size_t size)
{
    if (size == 0 || size > (size_t)-1 - (HEAP_ALIGNMENT - 1))
    {
        return NULL;
    }

    size = (size + HEAP_ALIGNMENT - 1) & ~(HEAP_ALIGNMENT - 1);

    for (axlib_heap_block_t *block = heap_head; block != NULL; block = block->next)
    {
        if (!block->is_free || block->size < size)
        {
            continue;
        }

        if (block->size - size >= sizeof(axlib_heap_block_t) + HEAP_ALIGNMENT)
        {
            axlib_heap_block_t *split = (axlib_heap_block_t *)((uint8_t *)(block + 1) + size);
            split->size = block->size - size - sizeof(axlib_heap_block_t);
            split->next = block->next;
            split->prev = block;
            split->is_free = 1;

            if (split->next != NULL)
            {
                split->next->prev = split;
            }
            else
            {
                heap_tail = split;
            }

            block->next = split;
            block->size = size;
        }

        block->is_free = 0;
        return block + 1;
    }

    if (size > (size_t)-1 - sizeof(axlib_heap_block_t))
    {
        return NULL;
    }

    axlib_heap_block_t *block = (axlib_heap_block_t *)axlib_sbrk(sizeof(axlib_heap_block_t) + size);
    if (block == (void *)(intptr_t)(-1))
    {
        return NULL;
    }

    block->size = size;
    block->next = NULL;
    block->prev = heap_tail;
    block->is_free = 0;

    if (heap_tail != NULL)
    {
        heap_tail->next = block;
    }
    else
    {
        heap_head = block;
    }
    heap_tail = block;

    return block + 1;
}

void axlib_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    axlib_heap_block_t *block = (axlib_heap_block_t *)ptr - 1;
    block->is_free = 1;

    if (block->next != NULL && block->next->is_free)
    {
        block->size += sizeof(axlib_heap_block_t) + block->next->size;
        block->next = block->next->next;
        if (block->next != NULL)
        {
            block->next->prev = block;
        }
        else
        {
            heap_tail = block;
        }
    }

    if (block->prev != NULL && block->prev->is_free)
    {
        block->prev->size += sizeof(axlib_heap_block_t) + block->size;
        block->prev->next = block->next;
        if (block->next != NULL)
        {
            block->next->prev = block->prev;
        }
        else
        {
            heap_tail = block->prev;
        }
    }
}