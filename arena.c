#include "arena.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

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
        errno = ENOMEM;
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

void* Arena_grow(Arena* arena, size_t size){
    return Arena_alloc_aligned(arena, size, 1);
}

#define SGR_CLEAR "\x1b[m"
#define SGR_MAGENTA "\x1b[35m"
#define SGR_GRAY "\x1b[90m"

void Arena_print(Arena* arena){
    size_t ind = 0;
    while(ind < arena->size){
        if (ind % 0x100 == 0){
            if (ind > 0) printf("\n\n");
            printf("== ");
            printf(SGR_MAGENTA);
            for (size_t i = 0; i < min(arena->size, 16); i++)
                printf(" %.2lx", i);
            printf(SGR_CLEAR);
        }

        printf(SGR_MAGENTA "\n%.2lx: " SGR_CLEAR, ind % 0x100);
        size_t i = 0;
        for (i = 0; i < 16 && ind < arena->size; i++,ind++)
            printf("%.2x ", ((unsigned char*)(arena->buffer))[ind]);

        for (size_t j = i; j < 16; j++) printf("   ");

        for (size_t j = 0; j < i; j++){
            char c = ((char*)(arena->buffer))[ind - i + j];
            if (isalnum(c)) printf("'%c' ", c);
            else if (isblank(c)) printf(SGR_GRAY "' ' " SGR_CLEAR);
            else if (iscntrl(c)) printf(SGR_GRAY "<\\> " SGR_CLEAR);
            else printf(SGR_GRAY "--- " SGR_CLEAR);
        }
    }
    printf("\n");
}

