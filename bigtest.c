#include "arena.h"
#include <stdio.h>
#include <string.h>

#define ARENA_DEF 1024

int main(){
    Arena* arena = Arena_create(ARENA_DEF);

    unsigned char* arr = Arena_alloc_aligned(arena, ARENA_DEF, sizeof(*arr));
    for (size_t i = 0; i < ARENA_DEF; i++)
        arr[i] = i % 0x100;

    Arena_print(arena);

    Arena_destroy(arena);

    return 0;
}

