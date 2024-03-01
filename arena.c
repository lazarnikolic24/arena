#include "arena.h"
#include <stdlib.h>
#include <stdio.h>

size_t min(size_t a, size_t b){
    return (a<b)?a:b;
}

Arena* Arena_create(size_t size){
    Arena* ret = malloc(sizeof(*ret));

    ret->maxsize = size;
    ret->size = 0;
    ret->buffer = malloc(size);
    if (ret->buffer == NULL){
        free(ret);
        return NULL;
    }
    ret->top = ret->buffer;

    return ret;
}

void Arena_destroy(Arena* arena){
    free(arena->buffer);
    free(arena);
}

void* Arena_alloc(Arena* arena, size_t size){
    return Arena_alloc_aligned(arena, size, 0);
}

void* Arena_alloc_aligned(Arena* arena, size_t size, size_t align){
    if (arena->size + size > arena->maxsize){
        return NULL;
    }

    if (align == 0) align = MAX_ALIGN;

    void* ret = arena->top;
    size_t offset = 0;

    if (arena->size % align != 0){
        offset = align - arena->size % align;
    }
    
    ret += offset;
    arena->top += size + offset;
    arena->size += size + offset;
    
    return ret;
}

void Arena_print(Arena* arena){
    printf("   ");
    for (size_t i = 0; i < min(arena->size, 16); i++)
        printf(" %.2x", i);

    for (size_t i = 0; i < arena->size; i++){
        if (i%16 == 0) printf("\n%.2x: ", i);
        printf("%.2x ", ((unsigned char*)(arena->buffer))[i]);
    }
    printf("\n");
}

