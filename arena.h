#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>

#ifndef _LP64
#define MAX_ALIGN 8
#else
#define MAX_ALIGN 16
#endif

typedef struct Arena* Arena_ptr;

Arena_ptr Arena_create(size_t size);
void Arena_destroy(Arena_ptr arena);

void* Arena_alloc(Arena_ptr arena, size_t size);
void* Arena_alloc_aligned(Arena_ptr arena, size_t size, size_t align);
void* Arena_grow(Arena_ptr arena, void* data, size_t ammount);
void* Arena_grow_move(Arena_ptr arena, void* data, size_t ammount);
void* Arena_flush(Arena_ptr arena);

void Arena_print(Arena_ptr arena);

#endif /* ARENA_H */
