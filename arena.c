#include "arena.h"
#include <stdlib.h>
#include <stdio.h>

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
    if (arena->size + size > arena->maxsize){
        return NULL;
    }

    void* ret = arena->top;

    if (size % MAX_ALIGN != 0){
        size += MAX_ALIGN - size % MAX_ALIGN;
    }

    arena->top += size;
    arena->size += size;
    
    return ret;
}

void Arena_print(Arena* arena){
    for (int i = 0; i < arena->size; i++){
        if (i%16 == 0) printf("\n");
        printf("%.2x ", ((unsigned char*)(arena->buffer))[i]);
    }
    printf("\n");
}

