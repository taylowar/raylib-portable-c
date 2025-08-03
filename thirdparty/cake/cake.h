#ifndef CAKE_H_
#define CAKE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

#define CAKE_DA_INIT_CAPACITY 256 
#define cake_da_reserve(da, size)                                       \
    do {                                                                \
        if ((da)->capacity == 0) {                                      \
            (da)->capacity = CAKE_DA_INIT_CAPACITY;                     \
        }                                                               \
        while (size >= (da)->capacity) {                                \
            (da)->capacity = (da)->capacity*2;                          \
        }                                                               \
        (da)->es = realloc((da)->es, (da)->capacity*sizeof(*(da)->es)); \
        assert((da)->es != NULL && "Buy more RAM");                     \
    } while (0)

#define cake_da_append_many(cmd, items_count, items)                                \
    do {                                                                            \
        cake_da_reserve((cmd), (cmd)->size + (items_count));                        \
        memcpy((cmd)->es + (cmd)->size, (items), (items_count)*sizeof(*(cmd)->es)); \
        (cmd)->size += (items_count);                                               \
    } while (0)

#define cake_da_append(da, item)         \
do {                                     \
    cake_da_reserve((da), (da)->size+1); \
    (da)->es[(da)->size] = (item);       \
    (da)->size += 1;                     \
} while (0)

#define cake_da_free(da) \
do {                     \
    free((da)->es);      \
    (da)->size = 0;      \
    (da)->capacity = 0;  \
} while (0)

#define TEMP_ALLOC_CAPACITY 64*1024*1024
static size_t temp_alloc_size = 0;
static char temp_alloc[TEMP_ALLOC_CAPACITY] = {0};

void* cake_talloc_poll(size_t size);
char* cake_talloc_sprintf(const char* format, ...);
void  cake_talloc_reset();

#ifdef CAKE_IMPLEMENTATION

void* cake_talloc_poll(size_t size)
{
    assert(temp_alloc_size+size < TEMP_ALLOC_CAPACITY && "temporary allocator overflow");
    void* space = &temp_alloc[temp_alloc_size];
    temp_alloc_size+=size;
    return space;
}

char* cake_talloc_sprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int n = vsnprintf(NULL, 0, format, args);
    va_end(args);
    char* allocd_str = cake_talloc_poll(n+1); 
    assert(allocd_str != NULL && "Extend the size of the temporary allocator");
    va_start(args, format);
    vsnprintf(allocd_str, n+1, format, args);
    va_end(args);
    return allocd_str;
}

void cake_talloc_reset()
{
    temp_alloc_size = 0;
}

#endif // CAKE_IMPLEMENTATION

#ifdef CAKE_EXT_CMD
#define CAKE_CMD_IMPLEMENTATION
#include "./extensions/cake_cmd.h"
#endif // CAKE_EXR_CMD

#ifdef CAKE_EXT_HEAP
#define CAKE_HEAP_IMPLEMENTATION
#include "./extensions/cake_heap.h"
#endif // CAKE_EXR_HEAP

#ifdef CAKE_EXT_STRING
#define CAKE_STRING_IMPLEMENTATION
#include "./extensions/cake_string.h"
#endif // CAKE_EXT_STRING

#ifdef CAKE_EXT_FS
#define CAKE_FS_IMPLEMENTATION
#include "./extensions/cake_fs.h"
#endif // CAKE_EXT_FS

#endif // CAKE_H_

