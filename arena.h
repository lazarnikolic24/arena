#pragma once
#include <stdlib.h>

#ifndef _LP64
#define MAX_ALIGN 8
#else
#define MAX_ALIGN 16
#endif

typedef struct{
    void* buffer;
    void* top;
    size_t size;
    size_t maxsize;
} Arena;

Arena* Arena_create(size_t size);
void Arena_destroy(Arena* arena);

void* Arena_alloc(Arena* arena, size_t size);

void Arena_print(Arena* arena);

