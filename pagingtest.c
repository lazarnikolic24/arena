#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "arena.h"

#define CHECK_ERROR(cnd, msg)\
    do{\
        if(!(cnd)){\
            if(errno!=0)perror(msg);\
            else fprintf(stderr, "Error: %s\n", (msg));\
            exit(EXIT_FAILURE);\
        }\
    } while (0)

#define ARENA_DEF 1
#define NUM_INTS 1024

int main(){
    Arena_ptr arena = Arena_create(ARENA_DEF);
    CHECK_ERROR(arena != NULL, "Arena_create");

    for (size_t i = 0; i < NUM_INTS; i++){
        int* a = Arena_alloc_aligned(arena, sizeof(*a), sizeof(*a));
        *a = i % 0x100;
    }

    Arena_print(arena);
    Arena_destroy(arena);
    return 0;
}
