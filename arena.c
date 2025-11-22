#include "arena.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#define ASSERT(cnd, msg)\
    do{\
        if(!(cnd)){\
            fprintf(stderr, "Error: %s\n", (msg));\
            exit(EXIT_FAILURE);\
        }\
    } while (0)

typedef char Bool;
#define FALSE 0
#define TRUE 1

struct Arena {
    void* buffer;
    void* top;
    void* last_alloc;
    Arena_ptr next;
    Arena_ptr head;
    size_t size;
    size_t maxsize;
};

size_t min(size_t a, size_t b){
    return (a<b)?a:b;
}

size_t find_next_size(size_t target_size, size_t size){
    do {
        size *= 2;
    } while(target_size>size);
    return size;
}

Arena_ptr Arena_create(size_t size){
    Arena_ptr ret = malloc(sizeof(*ret));
    if (ret == NULL)
        return NULL;

    ret->maxsize = size;
    ret->size = 0;
    ret->buffer = malloc(size);
    if (ret->buffer == NULL){
        free(ret);
        return NULL;
    }
    ret->top = ret->buffer;
    ret->last_alloc = NULL;

    ret->head = ret;
    ret->next = NULL;

    return ret;
}

void Arena_destroy(Arena_ptr arena){
    if (arena->next) Arena_destroy(arena->next);
    free(arena->buffer);
    free(arena);
}

void Arena_update_heads(Arena_ptr arena, Arena_ptr new_head){
    arena->head = new_head;
    if (arena->next) Arena_update_heads(arena->next, new_head);
}

void Arena_update_last_alloc(Arena_ptr arena, void* new_alloc){
    arena->last_alloc = new_alloc;
    if (arena->next) Arena_update_last_alloc(arena->next, new_alloc);
}

void* Arena_alloc_internal(Arena_ptr arena, size_t size, size_t align, Bool grow_mode) {
    if (align == 0) align = MAX_ALIGN;

    size_t offset = 0;
    if (arena->head->size % align != 0){
        offset = align - arena->head->size % align;
    }

    if (arena->head->size + size + offset > arena->head->maxsize){
        if (grow_mode) {
/*
            fprintf(stderr, "%zu %zu %zu %zu\n", arena->head->size, size, offset, arena->head->maxsize);
            Arena_print(arena);
//*/
            errno = ENOMEM;
            return NULL;
        }
        size_t next_size = find_next_size(size, arena->head->maxsize);
        arena->head->next = Arena_create(next_size);
        if (arena->head->next == NULL) return NULL;
        Arena_update_heads(arena, arena->head->next);
        return Arena_alloc_internal(arena, size, align, grow_mode);
    }

    void* ret = arena->head->top;
    ret += offset;
    arena->head->top += size + offset;
    arena->head->size += size + offset;
    if (!grow_mode) Arena_update_last_alloc(arena, ret);

    return ret;
}

void* Arena_alloc(Arena_ptr arena, size_t size){
    return Arena_alloc_internal(arena, size, 0, FALSE);
}

void* Arena_alloc_aligned(Arena_ptr arena, size_t size, size_t align){
    return Arena_alloc_internal(arena, size, align, FALSE);
}

void* Arena_grow(Arena_ptr arena, void* data, size_t amount){
    ASSERT(arena->last_alloc == data, "Can't grow allocations aside from the most recent.");
    if (Arena_alloc_internal(arena, amount, 1, TRUE))
        return data;
    else
        return NULL;
}

void* Arena_grow_move(Arena_ptr arena, void* data, size_t amount){
    ASSERT(arena->last_alloc == data, "Can't grow allocations aside from the most recent.");
    if(Arena_grow(arena, data, amount)){
        return data;
    }
    size_t size = (arena->head->top - data) + amount;
    void* new = Arena_alloc(arena, size);
    if (new == NULL) return new;
    memcpy(new, data, size);
    return new;
}

void Arena_flush(Arena_ptr arena) {
    arena->head->size = 0;
    arena->head->top = arena->head->buffer;
    arena->head->last_alloc = NULL;
}

#define SGR_CLEAR "\x1b[m"
#define SGR_MAGENTA "\x1b[35m"
#define SGR_GRAY "\x1b[90m"
#define SGR_CYAN "\x1b[36m"

void Arena_print(Arena_ptr arena){
    printf(SGR_CYAN "%zu/%zu" SGR_CLEAR " bytes used (" SGR_CYAN "%.2f%%"\
            SGR_CLEAR ")\n", arena->size, arena->maxsize, 100.0f*arena->size/arena->maxsize);


    size_t ind = 0;
    while(ind < arena->size){
        if (ind % 0x100 == 0){
            if (ind > 0) printf("\n\n");
            printf("== ");
            printf(SGR_MAGENTA);
            for (size_t i = 0; i < min(arena->size, 16); i++)
                printf(" %.2zx", i);
            printf(SGR_CLEAR);
        }

        printf(SGR_MAGENTA "\n%.2zx: " SGR_CLEAR, ind % 0x100);
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
    printf(SGR_CLEAR "\n");
    if (arena->next) Arena_print(arena->next);
}

